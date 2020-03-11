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

#include "ultima/ultima4/debug.h"
#include "ultima/ultima4/image.h"
#include "ultima/ultima4/settings.h"
#include "ultima/ultima4/error.h"
#include "ultima/shared/std/containers.h"
#include "ultima/shared/std/misc.h"
#include "common/system.h"
#include "common/list.h"
#include "common/textconsole.h"
#include "graphics/screen.h"

namespace Ultima {
namespace Ultima4 {

Image::Image() : surface(NULL) {
}

/**
 * Creates a new image.  Scale is stored to allow drawing using U4
 * (320x200) coordinates, regardless of the actual image scale.
 * Indexed is true for palette based images, or false for RGB images.
 * Image type determines whether to create a hardware (i.e. video ram)
 * or software (i.e. normal ram) image.
 */
Image *Image::create(int w, int h, bool indexed, Image::Type type) {
    uint32 rmask, gmask, bmask, amask;
    Image *im = new Image;

    im->w = w;
    im->h = h;
    im->indexed = indexed;

	if (indexed)
		im->surface = new Graphics::ManagedSurface(w, h);
	else
		im->surface = new Graphics::ManagedSurface(w, h,
			Graphics::PixelFormat(4, 8, 8, 8, 8, 24, 16, 8, 0)
		);

    if (!im->surface) {
        delete im;
        return NULL;
    }

    return im;
}

/**
 * Create a special purpose image the represents the whole screen.
 */
Image *Image::createScreenImage() {
    Image *screen = new Image();

	screen->surface = new Graphics::Screen(g_system->getWidth(), g_system->getHeight());
    screen->w = screen->surface->w;
    screen->h = screen->surface->h;
	screen->indexed = g_system->getScreenFormat().bytesPerPixel == 1;

    return screen;
}

/**
 * Creates a duplicate of another image
 */
Image *Image::duplicate(Image *image) {    
    bool alphaOn = image->isAlphaOn();
    Image *im = create(image->width(), image->height(), false, HARDWARE);
    
//    if (image->isIndexed())
//        im->setPaletteFromImage(image);

    /* Turn alpha off before blitting to non-screen surfaces */
    if (alphaOn)
        image->alphaOff();
    
    image->drawOn(im, 0, 0);

    if (alphaOn)
        image->alphaOn();

    im->backgroundColor = image->backgroundColor;

    return im;
}

/**
 * Frees the image.
 */
Image::~Image() {
    delete surface;
}

/**
 * Sets the palette
 */
void Image::setPalette(const RGBA *colors, unsigned n_colors) {
    ASSERT(indexed, "imageSetPalette called on non-indexed image");

	byte *pal = new byte[n_colors * 3];
	byte *palP = pal;
	for (unsigned i = 0; i < n_colors; i++, palP += 3) {
        palP[0] = colors[i].r;
        palP[1] = colors[i].g;
        palP[2] = colors[i].b;
    }

	surface->setPalette(pal, 0, n_colors);
    delete[] pal;
}

/**
 * Copies the palette from another image.
 */
void Image::setPaletteFromImage(const Image *src) {
    ASSERT(indexed && src->indexed, "imageSetPaletteFromImage called on non-indexed image");

	const uint32 *srcPal = src->surface->getPalette();
	surface->setPalette(srcPal, 0, PALETTE_COUNT);
}

// returns the color of the specified palette index
RGBA Image::getPaletteColor(int index) {
	RGBA color = RGBA(0, 0, 0, 0);

    if (indexed) {
		uint32 pal = surface->getPalette()[index];
		color.r = (pal & 0xff);
		color.g = (pal >> 8) & 0xff;
		color.b = (pal >> 16) & 0xff;
        color.a = IM_OPAQUE;
    }

	return color;
}

/* returns the palette index of the specified RGB color */
int Image::getPaletteIndex(RGBA color) {
    if (!indexed)
        return -1;

	const uint32 *pal = surface->getPalette();
	uint32 color32 = color;

	for (int i = 0; i < PALETTE_COUNT; ++i, ++pal) {
		if (*pal == color)
			return i;
	}

    // return the proper palette index for the specified color
    return -1;
}

RGBA Image::setColor(uint8 r, uint8 g, uint8 b, uint8 a) {
	RGBA color = RGBA(r, g, b, a);
    return color;
}

/* sets the specified font colors */
bool Image::setFontColor(ColorFG fg, ColorBG bg) {
    if (!setFontColorFG(fg)) return false;
    if (!setFontColorBG(bg)) return false;
    return true;
}

/* sets the specified font colors */
bool Image::setFontColorFG(ColorFG fg) {
    switch (fg) {
        case FG_GREY:
            if (!setPaletteIndex(TEXT_FG_PRIMARY_INDEX,   setColor(153,153,153))) return false;
            if (!setPaletteIndex(TEXT_FG_SECONDARY_INDEX, setColor(102,102,102))) return false;
            if (!setPaletteIndex(TEXT_FG_SHADOW_INDEX,    setColor(51,51,51))) return false;
            break;
        case FG_BLUE:
            if (!setPaletteIndex(TEXT_FG_PRIMARY_INDEX,   setColor(102,102,255))) return false;
            if (!setPaletteIndex(TEXT_FG_SECONDARY_INDEX, setColor(51,51,204))) return false;
            if (!setPaletteIndex(TEXT_FG_SHADOW_INDEX,    setColor(51,51,51))) return false;
            break;
        case FG_PURPLE:
            if (!setPaletteIndex(TEXT_FG_PRIMARY_INDEX,   setColor(255,102,255))) return false;
            if (!setPaletteIndex(TEXT_FG_SECONDARY_INDEX, setColor(204,51,204))) return false;
            if (!setPaletteIndex(TEXT_FG_SHADOW_INDEX,    setColor(51,51,51))) return false;
            break;
        case FG_GREEN:
            if (!setPaletteIndex(TEXT_FG_PRIMARY_INDEX,   setColor(102,255,102))) return false;
            if (!setPaletteIndex(TEXT_FG_SECONDARY_INDEX, setColor(0,153,0))) return false;
            if (!setPaletteIndex(TEXT_FG_SHADOW_INDEX,    setColor(51,51,51))) return false;
            break;
        case FG_RED:
            if (!setPaletteIndex(TEXT_FG_PRIMARY_INDEX,   setColor(255,102,102))) return false;
            if (!setPaletteIndex(TEXT_FG_SECONDARY_INDEX, setColor(204,51,51))) return false;
            if (!setPaletteIndex(TEXT_FG_SHADOW_INDEX,    setColor(51,51,51))) return false;
            break;
        case FG_YELLOW:
            if (!setPaletteIndex(TEXT_FG_PRIMARY_INDEX,   setColor(255,255,51))) return false;
            if (!setPaletteIndex(TEXT_FG_SECONDARY_INDEX, setColor(204,153,51))) return false;
            if (!setPaletteIndex(TEXT_FG_SHADOW_INDEX,    setColor(51,51,51))) return false;
            break;
        default:
            if (!setPaletteIndex(TEXT_FG_PRIMARY_INDEX,   setColor(255,255,255))) return false;
            if (!setPaletteIndex(TEXT_FG_SECONDARY_INDEX, setColor(204,204,204))) return false;
            if (!setPaletteIndex(TEXT_FG_SHADOW_INDEX,    setColor(68,68,68))) return false;
    }
    return true;
}

/* sets the specified font colors */
bool Image::setFontColorBG(ColorBG bg) {
    switch (bg) {
        case BG_BRIGHT:
            if (!setPaletteIndex(TEXT_BG_INDEX, setColor(0,0,102)))
                return false;
            break;
        default:
            if (!setPaletteIndex(TEXT_BG_INDEX, setColor(0,0,0)))
                return false;
    }
    return true;
}

/* sets the specified palette index to the specified RGB color */
bool Image::setPaletteIndex(unsigned int index, RGBA color) {
    if (!indexed)
        return false;

	uint32 color32 = color;
	surface->setPalette(&color32, index, 1);

    // success
    return true;
}

bool Image::getTransparentIndex(unsigned int &index) const {
	if (!indexed)
		return false;

	index = surface->getTransparentColor();
	return true;
}

void Image::initializeToBackgroundColor(RGBA backgroundColor)
{
	if (indexed)
		error("Not supported"); //TODO, this better
	this->backgroundColor = backgroundColor;
    this->fillRect(0,0,this->w,this->h,
    		backgroundColor.r,
    		backgroundColor.g,
    		backgroundColor.b,
    		backgroundColor.a);
}

bool Image::isAlphaOn() const {
	return !indexed;
}

void Image::alphaOn() {
}

void Image::alphaOff() {
}

void Image::putPixel(int x, int y, int r, int g, int b, int a) {
	assert(surface->format.bytesPerPixel == 4);
	uint32 color = surface->format.ARGBToColor(a, r, g, b);

	putPixelIndex(x, y, color);
}


void Image::makeBackgroundColorTransparent(int haloSize, int shadowOpacity) {
	uint32 bgColor = surface->format.ARGBToColor(
		static_cast<byte>(backgroundColor.a),
		static_cast<byte>(backgroundColor.r),
		static_cast<byte>(backgroundColor.g),
		static_cast<byte>(backgroundColor.b)
	);

	performTransparencyHack(bgColor, 1, 0, haloSize,shadowOpacity);
}

//TODO Separate functionalities found in here
void Image::performTransparencyHack(unsigned int colorValue, unsigned int numFrames,
		unsigned int currentFrameIndex, unsigned int haloWidth,
		unsigned int haloOpacityIncrementByPixelDistance) {
    Common::List<Std::pair<unsigned int,unsigned int> > opaqueXYs;
    unsigned int x, y;
    byte t_r, t_g, t_b;

	surface->format.colorToRGB(colorValue, t_r, t_g, t_b);

    unsigned int frameHeight = h / numFrames;
    //Min'd so that they never go out of range (>=h)
    unsigned int top = MIN(h, currentFrameIndex * frameHeight);
    unsigned int bottom = MIN(h, top + frameHeight);

    for (y = top; y < bottom; y++) {

        for (x = 0; x < w; x++) {
            unsigned int r, g, b, a;
            getPixel(x, y, r, g, b, a);
            if (r == t_r &&
                g == t_g &&
                b == t_b) {
                putPixel(x, y, r, g, b, IM_TRANSPARENT);
            } else {
                putPixel(x, y, r, g, b, a);
                if (haloWidth)
                	opaqueXYs.push_back(Std::pair<unsigned int, unsigned int>(x,y));
            }
        }
    }
    int ox, oy;
    for (Common::List<Std::pair<unsigned int,unsigned int> >::iterator xy = opaqueXYs.begin();
    		xy != opaqueXYs.end();
    		++xy)
    {
    	ox = xy->first;
    	oy = xy->second;
    	int span = int(haloWidth);
    	unsigned int x_start = MAX(0,ox - span);
    	unsigned int x_finish = MIN(int(w), ox + span + 1);
    	for (x = x_start; x < x_finish; ++x)
    	{
    		unsigned int y_start = MAX(int(top),oy - span);
    		unsigned int y_finish = MIN(int(bottom), oy + span + 1);
        	for (y = y_start; y < y_finish; ++y) {

        		int divisor = 1 + span * 2 - abs(int(ox - x)) - abs(int(oy - y));

                unsigned int r, g, b, a;
                getPixel(x, y, r, g, b, a);
                if (a != IM_OPAQUE) {
                    putPixel(x, y, r, g, b, MIN(IM_OPAQUE, a + haloOpacityIncrementByPixelDistance / divisor));
                }
        	}
    	}
    }


}

void Image::setTransparentIndex(unsigned int index) {
    if (indexed) {
		surface->setTransparentColor(index);
    } else {
    	//errorWarning("Setting transparent index for non indexed");
    }
}

/**
 * Sets the palette index of a single pixel.  If the image is in
 * indexed mode, then the index is simply the palette entry number.
 * If the image is RGB, it is a packed RGB triplet.
 */
void Image::putPixelIndex(int x, int y, unsigned int index) {
    int bpp;
    byte *p;

    bpp = surface->format.bytesPerPixel;
	p = (byte *)surface->getBasePtr(x, y);

    switch(bpp) {
    case 1:
        *p = index;
        break;

    case 2:
		*((uint16 *)p) = index;
        break;

    case 4:
        *reinterpret_cast<uint32 *>(p) = index;
        break;

	default:
		error("Unsupported format");
	}
}

/**
 * Fills a rectangle in the image with a given color.
 */
void Image::fillRect(int x, int y, int w, int h, int r, int g, int b, int a) {
	uint32 pixel = surface->format.ARGBToColor(a, r, g, b);
	surface->fillRect(Common::Rect(x, y, x + w, y + h), pixel);
}

/**
 * Gets the color of a single pixel.
 */
void Image::getPixel(int x, int y, unsigned int &r, unsigned int &g, unsigned int &b, unsigned int &a) const {
    unsigned int index;
    byte r1, g1, b1, a1;

    getPixelIndex(x, y, index);

	surface->format.colorToARGB(index, a1, r1, g1, b1);
    r = r1;
    g = g1;
    b = b1;
    a = a1;
}

/**
 * Gets the palette index of a single pixel.  If the image is in
 * indexed mode, then the index is simply the palette entry number.
 * If the image is RGB, it is a packed RGB triplet.
 */
void Image::getPixelIndex(int x, int y, unsigned int &index) const {
    int bpp = surface->format.bytesPerPixel;

	byte *p = (byte *)surface->getBasePtr(x, y);

    switch(bpp) {
    case 1:
        index = *p;
        break;

    case 2:
        index = *reinterpret_cast<uint16 *>(p);
        break;

    case 4:
        index = *reinterpret_cast<uint32 *>(p);

    default:
        error("Unsupported format");
    }
}

/**
 * Draws the image onto another image.
 */
void Image::drawOn(Image *d, int x, int y) const {
	// TODO: Support d being nullptr to draw to screen
	Graphics::ManagedSurface *destSurface = d->surface;
	destSurface->blitFrom(*surface, Common::Point(x, y));
}

/**
 * Draws a piece of the image onto another image.
 */
void Image::drawSubRectOn(Image *d, int x, int y, int rx, int ry, int rw, int rh) const {
	// TODO: Support d being nullptr to draw to screen
	Graphics::ManagedSurface *destSurface = d->surface;
	destSurface->blitFrom(*surface, Common::Rect(rx, ry, rx + rw, ry + rh), Common::Point(x, y));
}

/**
 * Draws a piece of the image onto another image, inverted.
 */
void Image::drawSubRectInvertedOn(Image *d, int x, int y, int rx, int ry, int rw, int rh) const {
	// TODO: Support d being nullptr to draw to screen
	Graphics::ManagedSurface *destSurface = d->surface;
	int i;
	Common::Rect src;
	Common::Point destPos;

    for (i = 0; i < rh; i++) {
        src.left = rx;
        src.top = ry + i;
        src.right = rx + rw;
        src.bottom = ry + i + 1;

		destPos.x = x;
		destPos.y = y + rh - i - 1;

		destSurface->blitFrom(*surface, src, destPos);
    }
}

/**
 * Dumps the image to a file.  The file is always saved in .bmp
 * format.  This is mainly used for debugging.
 */
void Image::save(const Common::String &filename) {
#ifdef TODO
	SDL_SaveBMP(surface, filename.c_str());
#endif
}


void Image::drawHighlighted() {
    RGBA c;
    for (unsigned i = 0; i < h; i++) {
        for (unsigned j = 0; j < w; j++) {
            getPixel(j, i, c.r, c.g, c.b, c.a);
            putPixel(j, i, 0xff - c.r, 0xff - c.g, 0xff - c.b, c.a);
        }
    }
}

} // End of namespace Ultima4
} // End of namespace Ultima
