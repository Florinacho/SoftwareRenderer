#ifndef __MENU_MANAGER_H__
#define __MENU_MANAGER_H__

#include "Image.h"
#include "Input.h"

#if not defined (MenuMaxItemCount)
#define MenuMaxItemCount 8
#endif // MenuMaxItemCount

struct Menu;
class MenuManager;

typedef void (*MenuCallback)(MenuManager*);

struct MenuItem {
	const char *text;
	unsigned char scale;
	bool centered;
	MenuCallback callback;
	int valueCount;
	unsigned char selected;
	const char **values;
};

struct Menu {
	enum MenuType {
		DEFAULT,
		KEY_SELECTOR
	};
	
	Menu *parent;
	unsigned char type;
	unsigned char itemCount;
	MenuItem items[MenuMaxItemCount];
};

class MenuManager {
	Menu *activeMenu;
	ivec2 selected;
	Vector4ub colors[2];
	bool newSelection;

public:
	MenuManager(Menu* menu);

	void setActiveMenu(Menu *menu);

	Menu *getActiveMenu() const;
	
	void setSelected(int value);
	
	int getSelected() const;
	
	void setSelectedValue(int value);
	
	int getSelectedValue() const;

	bool onEvent(const Event& event);

	void draw(Image *output);
};

#endif // __MENU_MANAGER_H__