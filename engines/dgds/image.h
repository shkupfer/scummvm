/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#ifndef DGDS_IMAGE_H
#define DGDS_IMAGE_H

namespace Common {
class SeekableReadStream;
}

namespace Graphics {
struct Surface;
}

namespace Dgds {

class Decompressor;
class DgdsChunkReader;
class ResourceManager;

class Palette {
public:
	Palette();
	byte _palette[256 * 3];
};

class GamePalettes {
public:
	GamePalettes(ResourceManager *resourceMan, Decompressor *decompressor);
	int loadPalette(Common::String filename);
	void selectPalNum(int num);
	void setPalette();
	void clearPalette();

	// Fade the colors in the current palette toward black. Start at col, and fade ncols of the palette.
	// Add coloff to the result to move toward white.
	void setFade(int col, int ncols, int coloff, int fade);

private:
	ResourceManager *_resourceMan;
	Decompressor *_decompressor;

	Palette _curPal;
	uint _curPalNum;
	Common::Array<Palette> _palettes;
	Palette _blacks;
};

class Image {
public:
	Image(ResourceManager *resourceMan, Decompressor *decompressor);
	virtual ~Image();

	void drawScreen(Common::String filename, Graphics::Surface &surface);
	void loadBitmap(const Common::String &filename, int number);
	int frameCount(const Common::String &filename);
	void drawBitmap(int x, int y, const Common::Rect &drawWin, Graphics::Surface &surface);

	bool isLoaded() const { return _bmpData.getPixels() != nullptr; }
	int16 width() const;
	int16 height() const;

	void unload() { _bmpData.free(); }

	const Graphics::Surface &getSurface() { return _bmpData; }

private:
	void loadBitmap4(Graphics::Surface &surf, uint16 tw, uint16 th, uint32 toffset, Common::SeekableReadStream *stream, bool highByte);
	void loadBitmap8(Graphics::Surface &surf, uint16 tw, uint16 th, uint32 toffset, Common::SeekableReadStream *stream);
	uint32 loadVQT(Graphics::Surface &surf, uint16 tw, uint16 th, uint32 toffset, Common::SeekableReadStream *stream);
	bool loadSCN(Graphics::Surface &surf, uint16 tw, uint16 th, Common::SeekableReadStream *stream);

	Graphics::Surface _bmpData;
	ResourceManager *_resourceMan;
	Decompressor *_decompressor;
};

} // End of namespace Dgds

#endif // DGDS_IMAGE_H
