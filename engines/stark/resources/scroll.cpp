/* ResidualVM - A 3D game interpreter
 *
 * ResidualVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the AUTHORS
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

#include "engines/stark/resources/scroll.h"

#include "engines/stark/formats/xrc.h"

namespace Stark {

Scroll::~Scroll() {
}

Scroll::Scroll(Resource *parent, byte subType, uint16 index, const Common::String &name) :
				Resource(parent, subType, index, name),
				_coordinate(0),
				_field_30(0),
				_field_34(0),
				_bookmarkIndex(0) {
	_type = TYPE;
}

void Scroll::readData(XRCReadStream *stream) {
	  _coordinate = stream->readUint32LE();
	  _field_30 = stream->readUint32LE();
	  _field_34 = stream->readUint32LE();
	  _bookmarkIndex = stream->readUint32LE();
}


void Scroll::printData() {
	debug("coordinate: %d", _coordinate);
	debug("field_30: %d", _field_30);
	debug("field_34: %d", _field_34);
	debug("bookmarkIndex: %d", _bookmarkIndex);
}

} // End of namespace Stark
