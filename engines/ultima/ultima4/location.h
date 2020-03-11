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

#ifndef ULTIMA4_LOCATION_H
#define ULTIMA4_LOCATION_H

#include "ultima/ultima4/map.h"
#include "ultima/ultima4/movement.h"
#include "ultima/ultima4/observable.h"
#include "ultima/ultima4/types.h"

namespace Ultima {
namespace Ultima4 {

typedef enum {
    CTX_WORLDMAP    = 0x0001,
    CTX_COMBAT      = 0x0002,
    CTX_CITY        = 0x0004,
    CTX_DUNGEON     = 0x0008,
    CTX_ALTAR_ROOM  = 0x0010,
    CTX_SHRINE      = 0x0020
} LocationContext;

#define CTX_ANY             (LocationContext)(0xffff)
#define CTX_NORMAL          (LocationContext)(CTX_WORLDMAP | CTX_CITY)
#define CTX_NON_COMBAT      (LocationContext)(CTX_ANY & ~CTX_COMBAT)
#define CTX_CAN_SAVE_GAME   (LocationContext)(CTX_WORLDMAP | CTX_DUNGEON)

class TurnCompleter;

class Location : public Observable<Location *, MoveEvent &> {
public:
    Location(MapCoords coords, Map *map, int viewmode, LocationContext ctx, TurnCompleter *turnCompleter, Location *prev);

    Std::vector<MapTile> tilesAt(MapCoords coords, bool &focus);
    TileId getReplacementTile(MapCoords atCoords, Tile const * forTile);
    int getCurrentPosition(MapCoords *coords);
    MoveResult move(Direction dir, bool userEvent);

    MapCoords coords;    
    Map *map;
    int viewMode;
    LocationContext context;
    TurnCompleter *turnCompleter;
    Location *prev;
};

void locationFree(Location **stack);

} // End of namespace Ultima4
} // End of namespace Ultima

#endif
