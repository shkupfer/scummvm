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

#ifndef M4_BURGER_BURGER_VARS_H
#define M4_BURGER_BURGER_VARS_H

#include "m4/vars.h"
#include "m4/burger/gui/interface.h"
#include "m4/burger/inventory.h"
#include "m4/burger/globals.h"
#include "m4/burger/series_player.h"
#include "m4/burger/stream_break.h"
#include "m4/burger/walker.h"

namespace M4 {
namespace Burger {

enum GameMode {
	JUST_OVERVIEW = 0, INTERACTIVE_DEMO = 1, MAGAZINE_DEMO = 2, WHOLE_GAME = 3
};

enum global_triggers {
	gFIRST_GLOBAL_TRIGGER = 10000,

	// System triggers 
	gRESUME_CONVERSATION,			// Yep, that's what it does!
	gWILBURS_SPEECH_FINISHED,		// digi_play trigger, used in talk.cpp
	gNONPLAYERS_SPEECH_FINISHED,	// digi_play trigger, used in talk.cpp
	gSERIES_STREAM_BREAK,			// Used only by set_next_stream_break () in global.cpp
	gSERIES_PLAY_BREAK_0,			// Used only by set_next_play_break () in global.cpp
	gSERIES_PLAY_BREAK_1,			// Used only by set_next_play_break () in global.cpp
	gSERIES_PLAY_BREAK_2,			// Used only by set_next_play_break () in global.cpp
	gRELEASE_TRIGGER_DIGI_CHECK,	// Used only by release_trigger_digi_check() in global.cpp

	// General triggers
	gREACT_TO_WHISTLE,				// For anything that reacts to the wistle being blown

	// Wilbur specific triggers
	gCHANGE_WILBUR_ANIMATION,

	gLAST_GLOBAL_TRIGGER
};

class Vars : public M4::Vars {
private:
	void initMouseSeries(const Common::String &assetName, RGB8 *myPalette);
	void custom_ascii_converter_proc(char *string);
	static void escape_key_pressed(void *, void *);
	static void cb_F2(void *, void *);
	static void cb_F3(void *, void *);

protected:
	void main_cold_data_init() override;
	void initialize_game() override;

public:
	Burger::GlobalVars _globals;
	GameMode _executing = WHOLE_GAME;
	GUI::Interface _interface;
	SeriesPlayers _seriesPlayers;
	StreamBreak_Globals _streamBreak;
	Burger::Walker _walker;
	int _wilburTerm = 2;
	const Common::String _wilbur = "WILBUR";
	bool _menuSystemInitialized = false;
	bool _gameMenuFromMain = false;

public:
	Vars();
	virtual ~Vars();

	GlobalVars *getGlobals() override {
		return &_globals;
	}
	M4::Interface *getInterface() override {
		return &_interface;
	}
	M4::Walker *getWalker() override {
		return &_walker;
	}

	void global_menu_system_init() override;
};

extern Vars *g_globals;

#undef _G
#undef _GI
#define _G(X) (::M4::Burger::g_globals->_##X)
#define _GI(X) _G(interface)._##X
#define _GINT() _G(interface)
#define VAL(X) _G(_globals)[X]

} // namespace Burger
} // namespace M4

#endif
