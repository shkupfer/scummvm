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

#ifndef ULTIMA6_MENUS_INPUT_DIALOG_H
#define ULTIMA6_MENUS_INPUT_DIALOG_H

#include "ultima/ultima6/gui/gui_Dialog.h"

namespace Ultima {
namespace Ultima6 {

class GUI;
class GUI_CallBack;
class GUI_Button;
class GUI_TextToggleButton;

class InputDialog : public GUI_Dialog {
protected:
	uint8 last_index;
	sint8 b_index_num;
	GUI_CallBack *callback_object;
	GUI_Button *save_button, *cancel_button;
	GUI_TextToggleButton *command_button, *direction_button, *doubleclick_button,
	                     *dragging_button, *interface_button, *look_button,
	                     *open_container_button, *party_targeting_button, *walk_button,
	                     *balloon_button;
	GUI_Button *button_index[12]; // add to here when you add a button. Keep buttons in order by height

public:
	InputDialog(GUI_CallBack *callback);
	~InputDialog();
	bool init();

	GUI_status close_dialog();
	GUI_status KeyDown(Common::KeyState key);
	GUI_status callback(uint16 msg, GUI_CallBack *caller, void *data);
};

} // End of namespace Ultima6
} // End of namespace Ultima

#endif
