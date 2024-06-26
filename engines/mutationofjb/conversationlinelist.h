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

#ifndef MUTATIONOFJB_CONVERSATIONLINELIST_H
#define MUTATIONOFJB_CONVERSATIONLINELIST_H

#include "common/array.h"
#include "common/path.h"
#include "common/str.h"

namespace MutationOfJB {

class ConversationLineList {
public:
	struct Speech {
		Common::String _text;
		Common::String _voiceFile;

		bool isRepeating() const {
			return _text.firstChar() == '*';
		}
		bool isFirstSpeaker() const {
			return _text.firstChar() == '~';
		}
		bool isSecondSpeaker() const {
			return _text.firstChar() == '`';
		}
	};

	typedef Common::Array<Speech> Speeches;
	struct Line {
		Speeches _speeches;
		Common::String _extra;
	};

	ConversationLineList(const Common::Path &fileName);
	const Line *getLine(uint index) const;

private:
	bool parseFile(const Common::Path &fileName);

	Common::Array<Line> _lines;
};

}

#endif
