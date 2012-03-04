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

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

/*
 * This code is based on original Mortville Manor DOS source code
 * Copyright (c) 1988-1989 Lankhor
 */

#include "mortevielle/actions.h"
#include "mortevielle/dialogs.h"
#include "mortevielle/menu.h"
#include "mortevielle/mor.h"
#include "mortevielle/mor2.h"
#include "mortevielle/mortevielle.h"
#include "mortevielle/prog.h"
#include "mortevielle/taffich.h"
#include "mortevielle/var_mor.h"
#include "mortevielle/mouse.h"
#include "mortevielle/outtext.h"

namespace Mortevielle {

/**
 * Engine function - Change Graphical Device
 * @remarks	Originally called 'change_gd'
 */
void changeGraphicalDevice(int newDevice) {
	hideMouse();
	g_currGraphicalDevice = newDevice;
	hirs();
	initMouse();
	showMouse();
	dessine_rouleau();
	tinke();
	drawClock();
	if (g_ipers != 0)
		affper(g_ipers);
	else
		person();
	clsf2();
	clsf3();
	g_maff = 68;
	afdes(0);
	repon(2, g_crep);
	g_vm->_menu.displayMenu();
}

/**
 * Called when a savegame has been loaded.
 * @remarks	Originally called 'antegame'
 */
void MortevielleEngine::gameLoaded() {
	int k;
	char g[8];

	hideMouse();
	g_imen = false;
	g[1] = 'M';
	_loseGame = true;
	g_anyone = false;
	g_okdes = true;
	g[0] = '\040';
	g_col = false;
	g[7] = g[0];
	g[2] = 'A';
	g_cache = false;
	g_brt = false;
	g_maff = 68;
	g[5] = 'E';
	g_mnumo = 0;
	g_prebru = 0;
	g[4] = 'T';
	g_x = 0;
	g_y = 0;
	g_num = 0;
	g_hdb = 0;
	g_hfb = 0;
	g_cs = 0;
	g_is = 0;
	k = 0;
	g_ment = 0;
	g_syn = true;
	g_heroSearching = true;
	g_mchai = 0;
	g_inei = 0;
	initouv();
	g[3] = 'S';
	g[6] = 'R';
	g_iouv = 0;
	g_dobj = 0;
	affrep();
	g_hintPctMessage = deline(580);

	g_okdes = false;
	_endGame = true;
	_loseGame = false;
	g_heroSearching = false;

	person();
	tinke();
	drawClock();
	afdes(0);
	repon(2, g_crep);
	clsf3();
	_endGame = false;
	tmlieu(g_s._currPlace);
	modinv();
	if (g_s._selectedObjectId != 0)
		modobj(g_s._selectedObjectId + 400);
	showMouse();
}


/* NIVEAU 3 */
/* procedure PROGRAMME */
void tmaj3() {
	int j, h, m;

	calch(j, h, m);
	if (m == 30)
		m = 1;
	h += j * 24;
	m += h * 2;
	g_s._heure = chr(m);
}

void tsitu() {
	int h, j, m;

	if (!g_col)
		clsf2();
	g_syn = false;
	g_iesc = false;
	if (!g_anyone) {
		if (g_brt)
			if ((g_msg[3] == MENU_MOVE) || (g_msg[4] == OPCODE_LEAVE) || (g_msg[4] == OPCODE_SLEEP) || (g_msg[4] == OPCODE_EAT)) {
				g_ctrm = 4;
				mennor();
				return;
			}
		if (g_msg[3] == MENU_MOVE)
			fctMove();
		if (g_msg[3] == MENU_DISCUSS)
			fctDiscuss();
		if (g_msg[3] == MENU_INVENTORY)
			fctInventoryTake();
		if (g_msg[4] == OPCODE_ATTACH)
			fctAttach();
		if (g_msg[4] == OPCODE_WAIT)
			fctWait();
		if (g_msg[4] == OPCODE_FORCE)
			fctForce();
		if (g_msg[4] == OPCODE_SLEEP)
			fctSleep();
		if (g_msg[4] == OPCODE_LISTEN)
			fctListen();
		if (g_msg[4] == OPCODE_ENTER)
			fctEnter();
		if (g_msg[4] == OPCODE_CLOSE)
			fctClose();
		if (g_msg[4] == OPCODE_SEARCH)
			fctSearch();
		if (g_msg[4] == OPCODE_KNOCK)
			fctKnock();
		if (g_msg[4] == OPCODE_SCRATCH)
			fctScratch();
		if (g_msg[4] == OPCODE_READ)
			fctRead();
		if (g_msg[4] == OPCODE_EAT)
			fctEat();
		if (g_msg[4] == OPCODE_PLACE)
			fctPlace();
		if (g_msg[4] == OPCODE_OPEN)
			fctOpen();
		if (g_msg[4] == OPCODE_TAKE)
			fctTake();
		if (g_msg[4] == OPCODE_LOOK)
			fctLook();
		if (g_msg[4] == OPCODE_SMELL)
			fctSmell();
		if (g_msg[4] == OPCODE_SOUND)
			fctSound();
		if (g_msg[4] == OPCODE_LEAVE)
			fctLeave();
		if (g_msg[4] == OPCODE_LIFT)
			fctLift();
		if (g_msg[4] == OPCODE_TURN)
			fctTurn();
		if (g_msg[4] == OPCODE_SSEARCH)
			fctSelfSearch();
		if (g_msg[4] == OPCODE_SREAD)
			fctSelfRead();
		if (g_msg[4] == OPCODE_SPUT)
			fctSelfPut();
		if (g_msg[4] == OPCODE_SLOOK)
			fctSelftLook();
		g_cache = false;

		if (g_msg[4] == OPCODE_SHIDE)
			fctSelfHide();
	} else {
		if (g_anyone) {
			quelquun();
			g_anyone = false;
			mennor();
			return;
		}
	}
	calch(j, h, m);
	if ((((h == 12) || (h == 13) || (h == 19)) && (g_s._currPlace != 10)) ||
	        ((h > 0) && (h < 6) && (g_s._currPlace != 0)))
		++g_s._conf;
	if (((g_s._currPlace < CRYPT) || (g_s._currPlace > MOUNTAIN)) && (g_s._currPlace != 23)
	        && (g_s._currPlace != 0) && (g_s._selectedObjectId != 152) && (!g_vm->_loseGame)) {
		if ((g_s._conf > 99) && (h > 8) && (h < 16)) {
			g_crep = 1501;
			tperd();
		}
		if ((g_s._conf > 99) && (h > 0) && (h < 9)) {
			g_crep = 1508;
			tperd();
		}
		if ((j > 1) && (h > 8) && (!g_vm->_loseGame)) {
			g_crep = 1502;
			tperd();
		}
	}
	mennor();
}

/* NIVEAU 1 */

void theure() {
	g_vj = ord(g_s._heure);
	g_vh = g_vj % 48;
	g_vj /= 48;
	g_vm__ = g_vh % 2;
	g_vh /= 2;
	g_hour = g_vh;
	if (g_vm__ == 1)
		g_minute = 30;
	else
		g_minute = 0;
}

} // End of namespace Mortevielle
