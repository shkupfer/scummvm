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

#include "hpl1/engine/game/low_level_game_setup.h"
#include "hpl1/engine/system/low_level_system.h"
#include "hpl1/engine/impl/KeyboardSDL.h"
#include "hpl1/engine/impl/LowLevelGraphicsSDL.h"
#include "hpl1/engine/impl/LowLevelInputSDL.h"
#include "hpl1/engine/impl/LowLevelPhysicsNewton.h"
#include "hpl1/engine/resources/low_level_resources.h"
#include "hpl1/engine/impl/LowLevelSoundOpenAL.h"
#include "hpl1/engine/impl/MouseSDL.h"
#include "hpl1/engine/haptic/LowLevelHaptic.h"

namespace hpl {

LowLevelGameSetup::LowLevelGameSetup() {
	_lowLevelSystem = hplNew(LowLevelSystem, ());
	_lowLevelGraphics = hplNew(cLowLevelGraphicsSDL, ());
	_lowLevelInput = hplNew(cLowLevelInputSDL, (_lowLevelGraphics));
	_lowLevelResources = hplNew(LowLevelResources, (_lowLevelGraphics));
	_lowLevelSound = hplNew(cLowLevelSoundOpenAL, ());
	_lowLevelPhysics = hplNew(cLowLevelPhysicsNewton, ());
	_lowLevelHaptic = nullptr;
}

LowLevelGameSetup::~LowLevelGameSetup() {
	Log("Deleting lowlevel stuff.\n");

	Log("Physics\n");
	hplDelete(_lowLevelPhysics);
	Log("Sound\n");
	hplDelete(_lowLevelSound);
	Log("Input\n");
	hplDelete(_lowLevelInput);
	Log("Resources\n");
	hplDelete(_lowLevelResources);
	Log("System\n");
	hplDelete(_lowLevelSystem);
	Log("Graphics\n");
	hplDelete(_lowLevelGraphics);
	Log("Haptic\n");
	if (_lowLevelHaptic)
		hplDelete(_lowLevelHaptic);
}

cScene *LowLevelGameSetup::createScene(cGraphics *graphics, cResources *resources, cSound *sound,
									   cPhysics *physics, cSystem *system, cAI *ai, cHaptic *haptic) {
	return hplNew(cScene, (graphics, resources, sound, physics, system, ai, haptic));
}

cResources *LowLevelGameSetup::createResources(cGraphics *graphics) {
	return hplNew(cResources, (_lowLevelResources, _lowLevelGraphics));
}

cInput *LowLevelGameSetup::createInput(cGraphics *graphics) {
	return hplNew(cInput, (_lowLevelInput));
}

cSystem *LowLevelGameSetup::createSystem() {
	return hplNew(cSystem, (_lowLevelSystem));
}

cGraphics *LowLevelGameSetup::createGraphics() {
	return hplNew(cGraphics, (_lowLevelGraphics, _lowLevelResources));
}

cSound *LowLevelGameSetup::createSound() {
	return hplNew(cSound, (_lowLevelSound));
}

cPhysics *LowLevelGameSetup::createPhysics() {
	return hplNew(cPhysics, (_lowLevelPhysics));
}

cAI *LowLevelGameSetup::createAi() {
	return hplNew(cAI, ());
}

cHaptic *LowLevelGameSetup::createHaptic() {
	return hplNew(cHaptic, (_lowLevelHaptic));
}

} // namespace hpl
