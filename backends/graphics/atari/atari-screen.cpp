/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "atari-screen.h"

#include <mint/falcon.h>

#include "graphics/blit.h"

#include "atari-graphics.h"
#include "atari-graphics-superblitter.h"

Screen::Screen(AtariGraphicsManager *manager, int width, int height, const Graphics::PixelFormat &format, const Palette *palette_)
	: _manager(manager)
	, palette(palette_) {
	const AtariGraphicsManager::AtariMemAlloc &allocFunc = _manager->getStRamAllocFunc();

	surf.init(
		width + (_manager->_tt ? 0 : 2 * MAX_HZ_SHAKE),
		height + 2 * MAX_V_SHAKE,
		(width + (_manager->_tt ? 0 : 2 * MAX_HZ_SHAKE)) * _manager->getBitsPerPixel(format) / 8,
		nullptr,
		format);

	void *pixelsUnaligned = allocFunc(sizeof(uintptr) + (surf.h * surf.pitch) + ALIGN - 1);
	if (!pixelsUnaligned) {
		error("Failed to allocate memory in ST RAM");
	}

	surf.setPixels((void *)(((uintptr)pixelsUnaligned + sizeof(uintptr) + ALIGN - 1) & (-ALIGN)));

	// store the unaligned pointer for later release
	*((uintptr *)surf.getPixels() - 1) = (uintptr)pixelsUnaligned;

	memset(surf.getPixels(), 0, surf.h * surf.pitch);

	_offsettedSurf.init(
		width, height, surf.pitch,
		surf.getBasePtr((surf.w - width) / 2, (surf.h - height) / 2),
		surf.format);
}

Screen::~Screen() {
	const AtariGraphicsManager::AtariMemFree &freeFunc = _manager->getStRamFreeFunc();

	freeFunc((void *)*((uintptr *)surf.getPixels() - 1));
}

void Screen::reset(int width, int height, int bitsPerPixel) {
	cursorPositionChanged = true;
	cursorSurfaceChanged = true;
	cursorVisibilityChanged = false;
	clearDirtyRects();
	oldCursorRect = Common::Rect();
	rez = -1;
	mode = -1;

	// erase old screen
	_offsettedSurf.fillRect(Common::Rect(_offsettedSurf.w, _offsettedSurf.h), 0);

	if (_manager->_tt) {
		if (width <= 320 && height <= 240) {
			surf.w = 320;
			surf.h = 240 + 2 * MAX_V_SHAKE;
			surf.pitch = 2 * surf.w * bitsPerPixel / 8;
			rez = kRezValueTTLow;
		} else {
			surf.w = 640;
			surf.h = 480 + 2 * MAX_V_SHAKE;
			surf.pitch = surf.w * bitsPerPixel / 8;
			rez = kRezValueTTMid;
		}
	} else {
		mode = VsetMode(VM_INQUIRE) & PAL;

		if (_manager->_vgaMonitor) {
			mode |= VGA | (bitsPerPixel == 4 ? BPS4 : (hasSuperVidel() ? BPS8C : BPS8));

			if (width <= 320 && height <= 240) {
				surf.w = 320;
				surf.h = 240;
				mode |= VERTFLAG | COL40;
			} else {
				surf.w = 640;
				surf.h = 480;
				mode |= COL80;
			}
		} else {
			mode |= TV | (bitsPerPixel == 4 ? BPS4 : BPS8);

			if (width <= 320 && height <= 200) {
				surf.w = 320;
				surf.h = 200;
				mode |= COL40;
			} else if (width <= 320*1.2 && height <= 200*1.2) {
				surf.w = 320*1.2;
				surf.h = 200*1.2;
				mode |= OVERSCAN | COL40;
			} else if (width <= 640 && height <= 400) {
				surf.w = 640;
				surf.h = 400;
				mode |= VERTFLAG | COL80;
			} else {
				surf.w = 640*1.2;
				surf.h = 400*1.2;
				mode |= VERTFLAG | OVERSCAN | COL80;
			}
		}

		surf.w += 2 * MAX_HZ_SHAKE;
		surf.h += 2 * MAX_V_SHAKE;
		surf.pitch = surf.w * bitsPerPixel / 8;
	}

	_offsettedSurf.init(
		width, height, surf.pitch,
		surf.getBasePtr((surf.w - width) / 2, (surf.h - height) / 2),
		surf.format);
}

void Screen::addDirtyRect(const Graphics::Surface &srcSurface, const Common::Rect &rect, bool directRendering) {
	if (fullRedraw)
		return;

	if ((rect.width() == srcSurface.w && rect.height() == srcSurface.h)
		|| dirtyRects.size() == 128) {	// 320x200 can hold at most 250 16x16 rectangles
		//debug("addDirtyRect[%d]: purge %d x %d", (int)dirtyRects.size(), srcSurface.w, srcSurface.h);

		dirtyRects.clear();
		dirtyRects.emplace(srcSurface.w, srcSurface.h);

		oldCursorRect = Common::Rect();

		fullRedraw = true;
		return;
	}

	dirtyRects.insert(rect);

	if (!oldCursorRect.isEmpty()) {
		const Common::Rect alignedOldCursorRect = _manager->alignRect(oldCursorRect);

		// we have to check *aligned* oldCursorRect because it is background which gets copied,
		// i.e. it has to be up to date even outside the cursor rectangle.
		// do it now to avoid complex checking in updateScreenInternal()
		if (rect.contains(alignedOldCursorRect)) {
			oldCursorRect = Common::Rect();
		} else if (rect.intersects(alignedOldCursorRect)) {
			if (!directRendering) {
				_manager->copyRectToSurface(
					*offsettedSurf, _manager->getBitsPerPixel(offsettedSurf->format), srcSurface,
					alignedOldCursorRect.left, alignedOldCursorRect.top,
					alignedOldCursorRect);
			} else {
				restoreBackground(alignedOldCursorRect);
			}

			oldCursorRect = Common::Rect();
		}
	}
}

void Screen::storeBackground(const Common::Rect &rect) {
	const int bitsPerPixel = _manager->getBitsPerPixel(offsettedSurf->format);

	if (_cursorBackgroundSurf.w != rect.width()
		|| _cursorBackgroundSurf.h != rect.height()
		|| _cursorBackgroundSurf.format != offsettedSurf->format) {
		_cursorBackgroundSurf.create(rect.width(), rect.height(), offsettedSurf->format);
		_cursorBackgroundSurf.pitch = _cursorBackgroundSurf.pitch * bitsPerPixel / 8;
	}

	Graphics::copyBlit(
		(byte *)_cursorBackgroundSurf.getPixels(),
		(const byte *)offsettedSurf->getPixels() + rect.top * offsettedSurf->pitch + rect.left * bitsPerPixel / 8,
		_cursorBackgroundSurf.pitch, offsettedSurf->pitch,
		rect.width() * bitsPerPixel / 8, rect.height(),	// fake 4bpp by 8bpp's width/2
		offsettedSurf->format.bytesPerPixel);
}

void Screen::restoreBackground(const Common::Rect &rect) {
	const int bitsPerPixel = _manager->getBitsPerPixel(offsettedSurf->format);

	Graphics::copyBlit(
		(byte *)offsettedSurf->getPixels() + rect.top * offsettedSurf->pitch + rect.left * bitsPerPixel / 8,
		(const byte *)_cursorBackgroundSurf.getPixels(),
		offsettedSurf->pitch, _cursorBackgroundSurf.pitch,
		rect.width() * bitsPerPixel / 8, rect.height(),	// fake 4bpp by 8bpp's width/2
		offsettedSurf->format.bytesPerPixel);
}
