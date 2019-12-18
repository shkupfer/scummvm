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

#ifndef ULTIMA6_VIEWS_SPELL_VIEW_GUMP_H
#define ULTIMA6_VIEWS_SPELL_VIEW_GUMP_H

#include "ultima/ultima6/views/spell_view.h"
#include "ultima/ultima6/files/nuvie_bmp_file.h"

namespace Ultima {
namespace Ultima6 {

class Configuration;
class TileManager;
class ObjManager;
class Screen;
class Actor;
class Font;
class U6Bmp;
class Spell;

#define SPELLVIEWGUMP_WIDTH 162

class SpellViewGump : public SpellView {

	GUI_Button *gump_button;
	sint16 selected_spell;
	GUI_Font *font;
	NuvieBmpFile bmp;
public:
	SpellViewGump(Configuration *cfg);
	~SpellViewGump();

	bool init(Screen *tmp_screen, void *view_manager, uint16 x, uint16 y, Font *f, Party *p, TileManager *tm, ObjManager *om);

	void Display(bool full_redraw);

	virtual GUI_status MouseDown(int x, int y, int button);
	virtual GUI_status MouseUp(int x, int y, int button);
	virtual GUI_status MouseMotion(int x, int y, uint8 state) {
		return DraggableView::MouseMotion(x, y, state);
	}
	virtual GUI_status MouseWheel(sint32 x, sint32 y);
	virtual void MoveRelative(int dx, int dy) {
		return DraggableView::MoveRelative(dx, dy);
	}


	GUI_status callback(uint16 msg, GUI_CallBack *caller, void *data);
protected:

	sint16 getSpell(int x, int y);

	virtual uint8 fill_cur_spell_list();
	void loadCircleString(std::string datadir);
	void loadCircleSuffix(std::string datadir, std::string image);
	void printSpellQty(uint8 spell_num, uint16 x, uint16 y);

	void close_spellbook();
};

} // End of namespace Ultima6
} // End of namespace Ultima

#endif
