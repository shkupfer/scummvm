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

#ifndef M4_GRAPHICS_GR_SERIES_H
#define M4_GRAPHICS_GR_SERIES_H

#include "m4/m4_types.h"
#include "m4/wscript/ws_machine.h"

namespace M4 {

extern int32 series_load(const char *seriesName, int32 assetIndex, RGB8 *myPal);
extern void series_unload(int32 assetIndex);
extern bool series_draw_sprite(int32 spriteHash, int32 index, Buffer *destBuff, int32 x, int32 y);

extern bool series_show_frame(int32 spriteHash, int32 index, Buffer *destBuff, int32 x, int32 y);
extern machine *series_place_sprite(char *seriesName, int32 index, int32 x, int32 y, int32 s, int32 layer); // was series_show_sprite
extern machine *series_show_sprite(char *seriesName, int32 index, int32 layer);	// was simple_show_sprite

extern machine *series_play_xy(char *seriesName, int32 loopCount, uint32 flags,
	int32 x, int32 y,
	int32 s, int32 layer, int32 frameRate, int16 triggerNum);

extern machine *series_play(char *seriesName, int32 loopCount, uint32 flags,
	int32 s, int32 layer, int32 frameRate, int16 triggerNum,
	bool stick_when_done);

extern machine *series_simple_play(char *seriesName, frac16 layer, bool stick_when_done);

extern machine *series_ranged_play(char *seriesName, int32 loopCount, uint32 flags,
	int32 firstFrame, int32 lastFrame,
	int32 s, int32 layer, int32 frameRate,
	int16 triggerNum, bool stick_when_done);

machine *series_ranged_play_xy(char *seriesName, int32 loopCount, uint32 flags,
	int32 firstFrame, int32 lastFrame,
	int32 x, int32 y,
	int32 s, int32 layer, int32 frameRate,
	int16 triggerNum, bool stick_when_done);

} // namespace M4

#endif
