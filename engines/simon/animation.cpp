/* ScummVM - Scumm Interpreter
 * Copyright (C) 2001-2006 The ScummVM project
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * $URL$
 * $Id$
 *
 */

#include "common/stdafx.h"

#include "common/endian.h"
#include "common/system.h"

#include "simon/animation.h"
#include "simon/intern.h"
#include "simon/simon.h"

#include "sound/audiostream.h"
#include "sound/wave.h"


#ifdef USE_ZLIB
#include <zlib.h>
#endif

namespace Simon {

MoviePlayer::MoviePlayer(SimonEngine *vm, Audio::Mixer *mixer)
	: _vm(vm), _mixer(mixer) {
	_omniTV = false;
	_playing = false;

	_leftButtonDown = false;
	_rightButtonDown = false;

	_frameBuffer1 = 0;
	_frameBuffer2 = 0;

	_width = 0;
	_height = 0;

	_frameSize = 0;
	_framesCount = 0;
	_frameNum = 0;
	_framesPerSec = 0;
	_frameTicks = 0;
	_frameSkipped = 0;

	_ticks = 0;
}

MoviePlayer::~MoviePlayer() {
}

bool MoviePlayer::load(const char *filename) {
	char filename2[100];
	uint32 tag;
	int32 frameRate;

	// Change file extension to dxa
	strcpy(filename2, filename);
	int len = strlen(filename2) - 3;
	filename2[len++] = 'd';
	filename2[len++] = 'x';
	filename2[len++] = 'a';
	
	if (_fd.open(filename2) == false) {
		warning("Failed to load video file %s", filename2);
		return false;
	} 
	debug(0, "Playing video %s", filename2);

	_vm->_system->showMouse(false);

	tag = _fd.readUint32BE();
	assert(tag == MKID_BE('DEXA'));

	_fd.readByte();
	_framesCount = _fd.readUint16BE();
	frameRate = _fd.readUint32BE();

	if (frameRate > 0)
		_framesPerSec = 1000 / frameRate;
	else if (frameRate < 0)
		_framesPerSec = 100000 / (-frameRate);
	else
		_framesPerSec = 10;

        if (frameRate < 0)
                _frameTicks = -frameRate / 100;
	else
		_frameTicks = frameRate;

	_width = _fd.readUint16BE();
	_height = _fd.readUint16BE();
	debug(0, "frames_count %d width %d height %d rate %d ticks %d", _framesCount, _width, _height, _framesPerSec, _frameTicks);

	_frameSize = _width * _height;
	_frameBuffer1 = (uint8 *)malloc(_frameSize);
	_frameBuffer2 = (uint8 *)malloc(_frameSize);
	if (!_frameBuffer1 || !_frameBuffer2) {
		error("error allocating frame tables, size %d\n", _frameSize);
	}

	_frameNum = 0;
	_frameSkipped = 0;

	return true;
}

void MoviePlayer::playOmniTV() {
	// Load OmniTV video
	if (_fd.isOpen() == false) {
		_vm->_variableArray[254] = 6747;
		return;
	} else {
		_vm->setBitFlag(42, false);
		_omniTV = true;
		startSound();
		return;
	}
}

void MoviePlayer::play() {
	if (_vm->getBitFlag(40)) {
		playOmniTV();
		return;
	}

	if (_fd.isOpen() == false) {
		return;
	}

	_leftButtonDown = false;
	_rightButtonDown = false;

	_mixer->stopAll();

	// Resolution is smaller in Amiga verison so always clear screen
	if (_width == 384 && _height == 280)
		_vm->dx_clear_surfaces(480);

	_ticks = _vm->_system->getMillis();

	startSound();

	while (_frameNum < _framesCount)
		handleNextFrame();

	close();

	_vm->o_killAnimate();

	if (_vm->getBitFlag(41)) {
		memcpy(_vm->_backBuf, _vm->_frontBuf, _frameSize);
	} else {
		uint8 palette[1024];
		memset(palette, 0, sizeof(palette));
		_vm->dx_clear_surfaces(480);
		_vm->_system->setPalette(palette, 0, 256);
	}

	_vm->_fastFadeOutFlag = true;
}

void MoviePlayer::close() {
	_fd.close();
	free(_frameBuffer1);
	free(_frameBuffer2);
}

void MoviePlayer::startSound() {
	uint32 tag = _fd.readUint32BE();
	if (tag == MKID_BE('WAVE')) {
		uint32 size = _fd.readUint32BE();
		byte *buffer = (byte *)malloc(size);
		_fd.read(buffer, size);

		Common::MemoryReadStream stream(buffer, size);
		_bgSoundStream = Audio::makeWAVStream(stream);
		_mixer->stopHandle(_bgSound);
		_mixer->playInputStream(Audio::Mixer::kSFXSoundType, &_bgSound, _bgSoundStream);
		free(buffer);
	}
}

void MoviePlayer::nextFrame() {
	if (!_omniTV)
		return;

	if (_vm->getBitFlag(42)) {
		_omniTV = false;
		close();
		return;
	}

	if (_mixer->isSoundHandleActive(_bgSound) && (_mixer->getSoundElapsedTime(_bgSound) * _framesPerSec) / 1000 < _frameNum) {
		copyFrame(_vm->getBackBuf(), 465, 222);
		return;
	}

	if (_frameNum < _framesCount) {
		decodeFrame();
		copyFrame(_vm->getBackBuf(), 465, 222);
		_frameNum++;
	} else {
		_omniTV = false;
		close();
		_vm->_variableArray[254] = 6747;
	}
}

void MoviePlayer::handleNextFrame() {
	decodeFrame();
	processFrame();

	_vm->_system->updateScreen();
	_frameNum++;

	OSystem::Event event;
	while (_vm->_system->pollEvent(event)) {
		switch (event.type) {
		case OSystem::EVENT_KEYDOWN:
			if (event.kbd.ascii == 27) {
				_leftButtonDown = true;
				_rightButtonDown = true;
			}
			break;
		case OSystem::EVENT_LBUTTONDOWN:
			_leftButtonDown = true;
			break;
		case OSystem::EVENT_RBUTTONDOWN:
			_rightButtonDown = true;
			break;
		case OSystem::EVENT_LBUTTONUP:
			_leftButtonDown = false;
			break;
		case OSystem::EVENT_RBUTTONUP:
			_rightButtonDown = false;
			break;
		case OSystem::EVENT_QUIT:
			_vm->_system->quit();
			break;
		default:
			break;
		}
	}

	if (_leftButtonDown && _rightButtonDown && !_vm->getBitFlag(41)) {
		_frameNum = _framesCount;
	}
}

void MoviePlayer::copyFrame(byte *dst, uint x, uint y) {
	uint h = _height;
	uint w = _width;

	dst += y * _vm->_screenWidth + x;
	byte *src = _frameBuffer1;

	do {
		memcpy(dst, src, w);
		dst += _vm->_screenWidth;
		src += _width;
	} while (--h);
}

void MoviePlayer::decodeZlib(uint8 *data, int size, int totalSize) {
#ifdef USE_ZLIB
	uint8 *temp = (uint8 *)malloc(size);
	if (temp) {
		memcpy(temp, data, size);
    	z_stream d_stream;
    	d_stream.zalloc = (alloc_func)0;
    	d_stream.zfree = (free_func)0;
    	d_stream.opaque = (voidpf)0;
    	d_stream.next_in = temp;
    	d_stream.avail_in = size;
    	d_stream.total_in = size;
    	d_stream.next_out = data;
    	d_stream.avail_out = totalSize;
    	inflateInit(&d_stream);
        inflate(&d_stream, Z_FINISH);
    	inflateEnd(&d_stream);
		free(temp);
	}
#endif
}

void MoviePlayer::decodeFrame() {
	uint32 tag;

	tag = _fd.readUint32BE();
	if (tag == MKID_BE('CMAP')) {
		uint8 rgb[768];
		byte palette[1024];
		byte *p = palette;

		_fd.read(rgb, ARRAYSIZE(rgb));
		for (int i = 0; i <= 256; i++) {
			*p++ = rgb[i * 3 + 0];
			*p++ = rgb[i * 3 + 1];
			*p++ = rgb[i * 3 + 2];
			*p++ = 0;
		}
		_vm->_system->setPalette(palette, 0, 256);
	}

	tag = _fd.readUint32BE();
	if (tag == MKID_BE('FRAM')) {
		uint8 type = _fd.readByte();
		uint32 size = _fd.readUint32BE();
		debug(5, "frame %d type %d size %d", _frameNum, type, size);

		_fd.read(_frameBuffer2, size);

		switch (type) {
		case 2:
		case 3:
			decodeZlib(_frameBuffer2, size, _frameSize);
			break;
		default:
			error("decodeFrame: Unknown compression type %d", type);
		}
		if (type == 2) {
			memcpy(_frameBuffer1, _frameBuffer2, _frameSize);
		} else {
			for (int j = 0; j < _height; ++j) {
				for (int i = 0; i < _width; ++i) {
					const int offs = j * _width + i;
					_frameBuffer1[offs] ^= _frameBuffer2[offs];
				}
			}
		}
	}
}

void MoviePlayer::processFrame() {
	copyFrame(_vm->getFrontBuf(), (_vm->_screenWidth - _width) / 2, (_vm->_screenHeight - _height) / 2);
	_vm->_system->copyRectToScreen(_vm->getFrontBuf(), _vm->_screenWidth, 0, 0, _vm->_screenWidth, _vm->_screenHeight);

	if ((_bgSoundStream == NULL) || ((int)(_mixer->getSoundElapsedTime(_bgSound) * _framesPerSec) / 1000 < _frameNum + 1) ||
		_frameSkipped > _framesPerSec) {
		if (_frameSkipped > _framesPerSec) {
			warning("force frame %i redraw", _frameNum);
			_frameSkipped = 0;
		}

		if (_bgSoundStream && _mixer->isSoundHandleActive(_bgSound)) {
			while (_mixer->isSoundHandleActive(_bgSound) && (_mixer->getSoundElapsedTime(_bgSound) * _framesPerSec) / 1000 < _frameNum) {
				_vm->_system->delayMillis(10);
			}
			// In case the background sound ends prematurely, update
			// _ticks so that we can still fall back on the no-sound
			// sync case for the subsequent frames.
			_ticks = _vm->_system->getMillis();
		} else {
			_ticks += _frameTicks;
			while (_vm->_system->getMillis() < _ticks)
				_vm->_system->delayMillis(10);
		}
	} else {
		warning("dropped frame %i", _frameNum);
		_frameSkipped++;
	}
}

} // End of namespace Simon
