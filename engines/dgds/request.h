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

#ifndef DGDS_REQUEST_H
#define DGDS_REQUEST_H

#include "common/stream.h"
#include "common/array.h"

#include "dgds/parser.h"

namespace Dgds {

struct RequestStruct1 {
	uint16 _vals[4];
	Common::String _str;
};

struct RequestStruct2 {
	uint16 _vals[6];
};

// basic gadget is 52 (0x34) bytes
struct Gadget {
	// NOTE: Most of these names are still guesses
	uint16 _gadgetNo;
	uint16 _x;
	uint16 _y;
	uint16 _width;
	uint16 _height;
	uint16 _gadgetType;
	uint16 _flags2;
	uint16 _flags3;

	uint16 _sval1Type; // if 1 then use the string, else use the int
	Common::String _sval1S;
	uint16 _sval1I;

	uint16 _sval2Type; // if 1 then use the string, else use the int
	Common::String _sval2S;
	uint16 _sval2I;

	Common::String _buttonName;

	// some other fields..
	uint16 _field14_0x20;
	uint16 _field15_0x22;
	uint16 _field16_0x24;
	uint16 _field17_0x26;

	uint16 _parentX;
	uint16 _parentY;
};

// extended gadget type 1 is 62 (0x3e) bytes
struct Gadget1 : public Gadget {
	uint16 _gadget1_i1;
	uint16 _gadget1_i2;
};

// extended gadget type 2 is 74 (0x4a) bytes
struct Gadget2 : public Gadget {
	uint16 _gadget2_i1;
	uint16 _gadget2_i2;
	uint16 _gadget2_i3;
	uint16 _gadget2_i4;
};

// extended gadget type 8 is 68 (0x44) bytes
struct Gadget8 : public Gadget {
	uint16 _gadget8_i1;
	uint16 _gadget8_i2;
};

class RequestData : public ParserData {
public:
	uint16 _fileNum;
	uint16 _x;
	uint16 _y;
	uint16 _vals[7];
	Common::Array<RequestStruct1> _struct1List;
	Common::Array<RequestStruct2> _struct2List;
	Common::Array<Common::SharedPtr<Gadget>> _gadgets;
};

/**
 * A "Request" is described by a REQ file.  Requests are things like menus,
 * inventory items, etc.
 *
 * Request files include REQ and GAD (Gadget) chunks.
 */
class Request : public DgdsParser {
public:
	Request(ResourceManager *resman, Decompressor *decompressor);
	virtual ~Request() {}

	bool handleChunk(DgdsChunkReader &chunk, ParserData *data) override;

protected:
	bool parseREQChunk(RequestData &data, DgdsChunkReader &chunk, int num);
	bool parseGADChunk(RequestData &data, DgdsChunkReader &chunk, int num);

};


} // End of namespace Dgds

#endif // DGDS_REQUEST_H
