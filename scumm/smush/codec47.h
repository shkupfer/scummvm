/* ScummVM - Scumm Interpreter
 * Copyright (C) 2001/2002 The ScummVM project
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * $Header$
 *
 */

#ifndef CODEC_47_H
#define CODEC_47_H

#include "config.h"

#include "decoder.h"

class Codec47Decoder : public Decoder {
private:
	int32 _deltaSize;
	byte * _deltaBufs[2];
	byte * _deltaBuf;
	byte * _curBuf;
	int32 _prevSeqNb;
	int32 _lastTableWidth;

	void makeTables47(int32 width);
	void makeTables37(int32 param);
	void bompDecode(byte *dst, byte *src, int32 len);
	void decode2(byte * dst, byte * src, int32 offset1, int32 offset2, int32 pitch,
							 int16 * tmp_table, byte * param_ptr, int32 height,
							 byte * buf_small, byte * buf_big);

public:
	bool initSize(const Point &, const Rect &);
	Codec47Decoder();
	void clean();
	virtual ~Codec47Decoder();

public:
	bool decode(Blitter &, Chunk &);
};

#endif
