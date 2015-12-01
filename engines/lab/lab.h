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

 /*
 * This code is based on Labyrinth of Time code with assistance of
 *
 * Copyright (c) 1993 Terra Nova Development
 * Copyright (c) 2004 The Wyrmkeep Entertainment Co.
 *
 */

#ifndef LAB_H
#define LAB_H

#include "common/array.h"
#include "common/events.h"

#include "engines/engine.h"
#include "lab/labfun.h"
#include "lab/interface.h"
#include "lab/mouse.h"
#include "lab/music.h"
#include "lab/resource.h"

struct ADGameDescription;

namespace Lab {

enum GameFeatures {
	GF_LOWRES = 1 << 0,
	GF_WINDOWS_TRIAL = 1 << 1
};

struct Image {
	uint16 Width;
	uint16 Height;
	byte *ImageData;
};

#define ONESECOND 1000

class LabEngine : public Engine {
public:
	LabEngine(OSystem *syst, const ADGameDescription *gameDesc);
	~LabEngine();

	virtual Common::Error run();
	void go();

	const ADGameDescription *_gameDescription;
	Common::Platform getPlatform() const;
	uint32 getFeatures() const;

	bool hasFeature(EngineFeature f) const;
	Common::String generateSaveFileName(uint slot);

	//void showMainMenu();

	LargeSet *_conditions, *_roomsFound;

	int _screenWidth;
	int _screenHeight;
	int _screenBytesPerPage;


	// timing.cpp
	void getTime(uint32 *secs, uint32 *micros);
	void addCurTime(uint32 sec, uint32 micros, uint32 *timeSec, uint32 *timeMicros);
	void anyTimeDiff(uint32 sec1, uint32 micros1, uint32 sec2, uint32 micros2, uint32 *diffSecs, uint32 *diffMicros);
	void timeDiff(uint32 sec, uint32 micros, uint32 *diffSec, uint32 *diffMicros);
	void waitForTime(uint32 sec, uint32 micros);

private:
	uint32 _extraGameFeatures;

	// timing.cpp
	void microDelay(uint32 secs, uint32 micros);

	// vga.cpp
	byte _curvgapal[256 * 3];
	byte _curapen;

public:
	EventManager *_event;
	Resource *_resource;
	Music *_music;

	int _roomNum;
	byte *_currentDisplayBuffer;
	Common::Point _mousePos;

	CrumbData _breadCrumbs[MAX_CRUMBS];
	uint16 _numCrumbs;
	bool _droppingCrumbs;
	bool _followingCrumbs;
	bool _followCrumbsFast;
	bool _isCrumbTurning;
	uint32 _crumbSecs, _crumbMicros;
	bool _isCrumbWaiting;
	byte *_tempScrollData;
	bool _isHiRes;

private:
	byte *_displayBuffer;

	int _lastWaitTOFTicks;

	uint16 _nextKeyIn;
	uint16 _keyBuf[64];
	uint16 _nextKeyOut;
	bool _mouseAtEdge;

	bool _lastMessageLong;
	bool _lastTooLong;

private:
	bool createScreen(bool HiRes);
	bool from_crumbs(uint32 tmpClass, uint16 code, uint16 Qualifier, Common::Point tmpPos, uint16 &curInv, IntuiMessage * curMsg, bool &forceDraw, uint16 gadgetId, uint16 &actionMode);

public:
	void waitTOF();
	void setAPen(byte pennum);
	void writeColorRegs(byte *buf, uint16 first, uint16 numreg);
	byte *getCurrentDrawingBuffer();
	void readScreenImage(Image *Im, uint16 x, uint16 y);
	void screenUpdate();
	void rectFill(uint16 x1, uint16 y1, uint16 x2, uint16 y2);
	void scrollDisplayX(int16 dx, uint16 x1, uint16 y1, uint16 x2, uint16 y2);
	void scrollDisplayY(int16 dy, uint16 x1, uint16 y1, uint16 x2, uint16 y2);
	void ghoastRect(uint16 pencolor, uint16 x1, uint16 y1, uint16 x2, uint16 y2);
	void bltBitMap(Image *ImSource, uint16 xs, uint16 ys, Image *ImDest, uint16 xd, uint16 yd, uint16 width, uint16 height);
	void setPalette(void *cmap, uint16 numcolors);
	void drawHLine(uint16 x, uint16 y1, uint16 y2);
	void drawVLine(uint16 x1, uint16 y, uint16 x2);
	void drawImage(Image *Im, uint16 x, uint16 y);
	bool haveNextChar();
	uint16 getNextChar();
	void processInput(bool can_delay = false);
	void writeColorReg(byte *buf, uint16 regnum);
	void writeColorRegsSmooth(byte *buf, uint16 first, uint16 numreg);

	void drawPanel();
	void drawRoomMessage(uint16 CurInv, CloseDataPtr cptr);
	void interfaceOff();
	void interfaceOn();
	void decIncInv(uint16 *CurInv, bool dec);
	int32 longDrawMessage(const char *str);
	void drawMessage(const char *str);
	void doScrollBlack();
	void doScrollWipe(char *filename);
	void doScrollBounce();
	void doWipe(uint16 WipeType, CloseDataPtr *CPtr, char *filename);
	void doTransWipe(CloseDataPtr *CPtr, char *filename);
	Gadget *checkNumGadgetHit(Gadget *gadlist, uint16 key);
	IntuiMessage *getMsg();
	void drawMap(uint16 CurRoom, uint16 CurMsg, uint16 Floor, bool fadeout, bool fadein);
	void processMap(uint16 CurRoom);
	void doMap(uint16 CurRoom);
	void diffNextFrame();
	void drawJournal(uint16 wipenum, bool needFade);
	void processJournal();
	void doJournal();
	void drawMonText(char *text, TextFont *monitorFont, uint16 x1, uint16 y1, uint16 x2, uint16 y2, bool isinteractive);
	void processMonitor(char *ntext, TextFont *monitorFont, bool isinteractive, uint16 x1, uint16 y1, uint16 x2, uint16 y2);
	void doMonitor(char *background, char *textfile, bool isinteractive, uint16 x1, uint16 y1, uint16 x2, uint16 y2);
	void eatMessages();
	void drawStaticMessage(byte index);
	void drawDirection(CloseDataPtr LCPtr);
	int followCrumbs();

private:
	void quickWaitTOF();

	/*---------- Drawing Routines ----------*/

	void drawMaskImage(Image *Im, uint16 x, uint16 y);
	Common::Point getMousePos();
	void changeVolume(int delta);
	void applyPalette(byte *buf, uint16 first, uint16 numreg, uint16 slow);

	// engine.cpp
	bool setUpScreens();
	void perFlipGadget(uint16 gadID);
	bool doCloseUp(CloseDataPtr cptr);
	void mainGameLoop();
	bool doUse(uint16 curInv);
	void mayShowCrumbIndicator();
	void mayShowCrumbIndicatorOff();
};

extern LabEngine *g_lab;

} // End of namespace Lab

#endif // LAB_H
