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

#ifndef ULTIMA4_MENUITEM_H
#define ULTIMA4_MENUITEM_H

#include "common/str.h"
#include "ultima/shared/std/containers.h"

namespace Ultima {
namespace Ultima4 {

class MenuEvent;

/**
 * custom output types for with menu items that need
 * to perform special calculations before displaying
 * its associated value
 */
typedef enum {
    MENU_OUTPUT_INT,
    MENU_OUTPUT_GAMMA,
    MENU_OUTPUT_SHRINE,
    MENU_OUTPUT_SPELL,
    MENU_OUTPUT_VOLUME,
    MENU_OUTPUT_REAGENT
} menuOutputType;

class MenuItem {
public:
    MenuItem(Common::String text, short x, short y, int shortcutKey = -1);
    virtual ~MenuItem() {}

    virtual void activate(MenuEvent &event) {}

    // Accessor Methods
    int getId() const;
    short getX() const;
    short getY() const;
    int getScOffset() const;

    virtual Common::String getText() const;
    bool isHighlighted() const;
    bool isSelected() const;
    bool isVisible() const;
    const Std::set<int> &getShortcutKeys() const;
    bool getClosesMenu() const;

    void setId(int id);
    void setX(int xpos);
    void setY(int ypos);
    void setText(Common::String text);
    void setHighlighted(bool h = true);
    void setSelected(bool s = true);
    void setVisible(bool v = true);
    void addShortcutKey(int shortcutKey);
    void setClosesMenu(bool closesMenu);
    
protected:
    int id;
    short x, y;
    Common::String text;
    bool highlighted;
    bool selected;
    bool visible;
    int scOffset;
	Std::set<int> shortcutKeys;
    bool closesMenu;
};

/**
 * A menu item that toggles a boolean value, and displays the current
 * setting as part of the text.
 */
class BoolMenuItem : public MenuItem {
public:
    BoolMenuItem(Common::String text, short x, short y, int shortcutKey, bool *val);

    BoolMenuItem *setValueStrings(const Common::String &onString, const Common::String &offString);

    virtual void activate(MenuEvent &event);
    virtual Common::String getText() const;

protected:
    bool *val;
    Common::String on, off;
};

/**
 * A menu item that cycles through a list of possible Common::String values, and
 * displays the current setting as part of the text.
 */
class StringMenuItem : public MenuItem {
public:
    StringMenuItem(Common::String text, short x, short y, int shortcutKey, Common::String *val, const Std::vector<Common::String> &validSettings);

    virtual void activate(MenuEvent &event);
    virtual Common::String getText() const;

protected:
    Common::String *val;
    Std::vector<Common::String> validSettings;
};

/**
 * A menu item that cycles through a list of possible integer values,
 * and displays the current setting as part of the text.
 */
class IntMenuItem : public MenuItem {
public:
    IntMenuItem(Common::String text, short x, short y, int shortcutKey, int *val, int min, int max, int increment, menuOutputType output=MENU_OUTPUT_INT);

    virtual void activate(MenuEvent &event);
    virtual Common::String getText() const;

protected:
    int *val;
    int min, max, increment;
    menuOutputType output;
};

} // End of namespace Ultima4
} // End of namespace Ultima

#endif
