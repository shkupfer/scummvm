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

#include "common/algorithm.h"
#include "bagel/baglib/save_game_file.h"
#include "bagel/boflib/misc.h"

namespace Bagel {

#define WORLDDIR "$SBARDIR\\WLD\\%s"

void ST_VAR::synchronize(Common::Serializer &s) {
	s.syncBytes((byte *)m_szName, MAX_VAR_NAME);
	s.syncBytes((byte *)m_szValue, MAX_VAR_VALUE);
	s.syncAsUint16LE(m_nType);

	s.syncAsByte(m_bGlobal);
	s.syncAsByte(m_bConstant);
	s.syncAsByte(m_bReference);
	s.syncAsByte(m_bTimer);

	s.syncAsByte(m_bRandom);
	s.syncAsByte(m_bNumeric);
	s.syncAsByte(m_bAttached);

	s.syncAsByte(m_bUsed);
}

void ST_VAR::clear() {
	Common::fill(m_szName, m_szName + MAX_VAR_NAME, '\0');
	Common::fill(m_szValue, m_szValue + MAX_VAR_VALUE, '\0');

	m_nType = 0;
	m_bGlobal = 0;
	m_bConstant = 0;
	m_bReference = 0;
	m_bTimer = 0;
	m_bRandom = 0;
	m_bNumeric = 0;
	m_bAttached = 0;
	m_bUsed = 0;
}

void ST_OBJ::synchronize(Common::Serializer &s) {
	s.syncBytes((byte *)m_szName, MAX_OBJ_NAME);
	s.syncBytes((byte *)m_szSDev, MAX_SDEV_NAME);
	s.syncAsUint32LE(m_lState);

	s.syncAsUint32LE(m_lProperties);
	s.syncAsUint32LE(m_lType);

	s.syncAsUint32LE(m_lLoop);
	s.syncAsUint32LE(m_lSpeed);

	s.syncAsByte(m_bAttached);
	s.syncAsByte(m_bUsed);

	s.syncAsUint16LE(m_nFlags);
}

void ST_OBJ::clear() {
	Common::fill(m_szName, m_szName + MAX_OBJ_NAME, '\0');
	Common::fill(m_szSDev, m_szSDev + MAX_SDEV_NAME, '\0');

	m_lState = 0;
	m_lProperties = 0;
	m_lType = 0;
	m_lLoop = 0;
	m_lSpeed = 0;
	m_bAttached = 0;
	m_bUsed = 0;
	m_nFlags = 0;
}

void ST_BAGEL_SAVE::synchronize(Common::Serializer &s) {
	for (int i = 0; i < MAX_VARS; ++i)
		m_stVarList[i].synchronize(s);
	for (int i = 0; i < MAX_OBJS; ++i)
		m_stObjList[i].synchronize(s);
	for (int i = 0; i < MAX_OBJS; ++i)
		m_stObjListEx[i].synchronize(s);

	s.syncBytes((byte *)m_szScript, MAX_FNAME);
	s.syncAsUint32LE(m_nLocType);

	for (int i = 0; i < MAX_CLOSEUP_DEPTH; ++i)
		s.syncBytes((byte *)m_szLocStack[i], MAX_SDEV_NAME);

	s.syncAsUint16LE(m_nLocX);
	s.syncAsUint16LE(m_nLocY);
	s.syncAsUint16LE(m_bUseEx);
	s.syncAsUint16LE(m_nFiller);
}

void ST_BAGEL_SAVE::clear() {
	for (int i = 0; i < MAX_VARS; ++i)
		m_stVarList[i].clear();
	for (int i = 0; i < MAX_OBJS; ++i)
		m_stObjList[i].clear();
	for (int i = 0; i < MAX_OBJS; ++i)
		m_stObjListEx[i].clear();

	Common::fill(m_szScript, m_szScript + MAX_FNAME, '\0');
	m_nLocType = 0;

	for (int i = 0; i < MAX_CLOSEUP_DEPTH; ++i)
		Common::fill(m_szLocStack[i], m_szLocStack[i] + MAX_SDEV_NAME, '\0');

	m_nLocX = 0;
	m_nLocY = 0;
	m_bUseEx = 0;
	m_nFiller = 0;
}

} // namespace Bagel
