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

#ifndef ULTIMA4_H
#define ULTIMA4_H

#include "ultima/shared/engine/ultima.h"

namespace Ultima {
namespace Ultima4 {

#define CLOCKS_PER_SEC 50

/* info for loading city data from *.ult and *.tlk */
#define CITY_HEIGHT 32
#define CITY_WIDTH 32
#define CITY_MAX_PERSONS 32

/* info for loading area data from *.con */
#define CON_HEIGHT 11
#define CON_WIDTH 11

/* info for loading dungeon map data from *.dng */
#define DNG_HEIGHT 8
#define DNG_WIDTH 8

/* info for loading image data from shapes.ega */
#define N_TILES 256
#define TILE_WIDTH (2 * CHAR_WIDTH)
#define TILE_HEIGHT (2 * CHAR_HEIGHT)

/* info for loading image data from charset.ega */
#define CHAR_WIDTH 8
#define CHAR_HEIGHT 8

/* some character defines */
#define CHARSET_ANKH '\0'
#define CHARSET_REDDOT '\01'
#define CHARSET_SDOOR '\02'
#define CHARSET_WALL '\03'
#define CHARSET_LADDER_UPDOWN '\04'
#define CHARSET_LADDER_DOWN '\05'
#define CHARSET_LADDER_UP '\06'
#define CHARSET_BULLET '\010'
#define CHARSET_COPYRIGHT '\011'
#define CHARSET_REGISTERED '\012'
#define CHARSET_MALE '\013'
#define CHARSET_FEMALE '\014'
#define CHARSET_HORIZBAR '\015'
#define CHARSET_ROOM '\016'
#define CHARSET_ORB '\017'
#define CHARSET_PROMPT '\020'
#define CHARSET_FLOOR '\022'

/* map viewport size (in tiles) */
#define VIEWPORT_W 11
#define VIEWPORT_H 11

/* screen border size (in pixels) */
#define BORDER_WIDTH 8
#define BORDER_HEIGHT 8

/* text area (in character units) */
#define TEXT_AREA_X 24
#define TEXT_AREA_Y 12
#define TEXT_AREA_W 16
#define TEXT_AREA_H 12

/* moons/moongates */
#define MOON_PHASES 24
#define MOON_SECONDS_PER_PHASE 4
#define MOON_CHAR 20

/* wind */
#define WIND_AREA_X 7
#define WIND_AREA_Y 23
#define WIND_AREA_W 10
#define WIND_AREA_H 1
#define WIND_SECONDS_PER_PHASE 1

class Ultima4Engine : public Shared::UltimaEngine {
protected:
	// Engine APIs
	Common::Error run() override;

	bool initialize() override;
public:
	Ultima4Engine(OSystem *syst, const Ultima::UltimaGameDescription *gameDesc);
	~Ultima4Engine() override;

	/**
	 * Returns true if a savegame can be loaded
	 */
	bool canLoadGameStateCurrently(bool isAutosave = false) override {
		return false;
	}

	/**
	 * Returns true if the game can be saved
	 */
	bool canSaveGameStateCurrently(bool isAutosave = false) override {
		return false;
	}
};


} // End of namespace Ultima4
} // End of namespace Ultima

#endif
