#include <stdio.h>

#include "MenuManager.h"
#include "SimpleFont.h"

template <typename T>
T wrap(const T value, const T min, const T max) {
	const T length = max - min;

	if (value < min) {
		return value + length;
	}
	if (value >= max) {
		return value - length;
	}

	return value;
}

MenuManager::MenuManager(Menu *menu) {
	activeMenu = NULL;
	newSelection = true;

	colors[0] = Vector4ub(255, 255, 255, 255);
	colors[1] = Vector4ub(  0, 255,   0, 255);

	setActiveMenu(menu);
}

void MenuManager::setActiveMenu(Menu* menu) {
	activeMenu = menu;
	selected.x = 0;
	selected.y = 0;
	newSelection = true;
	
	if (activeMenu == NULL) {
		return;
	}
	
	for (unsigned int index = 0; index < activeMenu->itemCount; ++index) {
		selected.y = wrap<int>(index, 0, activeMenu->itemCount);
		if (activeMenu->items[selected.y].text[0] != '\0' && activeMenu->items[selected.y].valueCount > -1) {
			return;
		}
	}
}

Menu* MenuManager::getActiveMenu() const {
	return activeMenu;
}

void MenuManager::setSelected(int value) {
	int tmp = wrap<int>(value, 0, activeMenu->itemCount);
	if (activeMenu->items[tmp].text[0] == '\0') {
		return;
	}
	selected.y = tmp;
}

int MenuManager::getSelected() const {
	return selected.y;
}

void MenuManager::setSelectedValue(int value) {
	selected.x = wrap<int>(value, 0, activeMenu->items[selected.y].valueCount);
}

int MenuManager::getSelectedValue() const {
	return selected.x;
}

bool MenuManager::onEvent(const Event& event) {
	if (activeMenu == NULL) {
		return false;
	}
	
	if (event.type != Event::KEYBOARD) {
		return false;
	}
	
	if (activeMenu->type == Menu::KEY_SELECTOR) {
		if (event.state == 0 && event.key != KEY_RETURN) {
			activeMenu->items[0].selected = event.key;
			activeMenu->items[selected.y].callback(this);
			return true;
		}
	}

	switch (event.key) {
	case KEY_UP :
		if (event.state == 1) {
			selected.y = wrap<int>(selected.y - 1, 0, activeMenu->itemCount);
			if (activeMenu->items[selected.y].text[0] == '\0' || activeMenu->items[selected.y].valueCount <= -1) {
				selected.y = wrap<int>(selected.y - 1, 0, activeMenu->itemCount);
			}
			newSelection = true;
		}
		return true;
	case KEY_DOWN :
		if (event.state == 1) {
			selected.y = wrap<int>(selected.y + 1, 0, activeMenu->itemCount);
			if (activeMenu->items[selected.y].text[0] == '\0' || activeMenu->items[selected.y].valueCount <= -1) {
				selected.y = wrap<int>(selected.y + 1, 0, activeMenu->itemCount);
			}
			newSelection = true;
		}
		return true;
	case KEY_LEFT :
		if (event.state == 1) {
			selected.x = wrap<int>(selected.x - 1, 0, activeMenu->items[selected.y].valueCount);
		}
		return true;
	case KEY_RIGHT :
		if (event.state == 1) {
			selected.x = wrap<int>(selected.x + 1, 0, activeMenu->items[selected.y].valueCount);
		}
		return true;
	case KEY_ENTER :
	case KEY_RETURN :
		if (event.state == 1) {
			if (activeMenu->items[selected.y].callback) {
				activeMenu->items[selected.y].callback(this);
			}
		}
		return true;
	case KEY_ESCAPE :
		if (event.state == 0) {
			if (activeMenu->parent) {
				setActiveMenu(activeMenu->parent);
				return true;
			}
		}
		break;
	}

	return false;
}

void MenuManager::draw(Image *output) {
	if (activeMenu == NULL) {
		return;
	}

	const uvec2 outputSize = output->getSize();
	const uvec2 halfOutputSize = outputSize / 2;

	unsigned int menuHeight = 0;
	for (unsigned int index = 0; index < activeMenu->itemCount; ++index) {
		menuHeight += activeMenu->items[index].scale * 8;
	}
	int offset = menuHeight / 2;

	if (newSelection) {
		selected.x = activeMenu->items[selected.y].selected;
	}

	activeMenu->items[selected.y].selected = selected.x = wrap<int>(selected.x, 0, activeMenu->items[selected.y].valueCount);

	for (unsigned int index = 0; index < activeMenu->itemCount; ++index) {
		const unsigned int textScale = activeMenu->items[index].scale;
		char buffer[64];
		
		if (activeMenu->items[index].text[0] == '\0') {
			offset -= textScale * 8;
			continue;
		}

		const int horizontalIndex = activeMenu->items[index].selected;
		const char **horizontalValues = activeMenu->items[index].values;

		switch (activeMenu->items[index].valueCount) {
		case 0 :
			snprintf(buffer, sizeof(buffer), activeMenu->items[index].text);
			break;
		case 1 :
			if (horizontalValues) {
				snprintf(buffer, sizeof(buffer), "%s      %s  ", activeMenu->items[index].text, horizontalValues[horizontalIndex]);
			} else {
				snprintf(buffer, sizeof(buffer), "%s      %d  ", activeMenu->items[index].text, horizontalIndex);
			}
			break;
		default :
			if (horizontalValues) {
				snprintf(buffer, sizeof(buffer), "%s    < %s >", activeMenu->items[index].text, horizontalValues[horizontalIndex]);
			} else {
				snprintf(buffer, sizeof(buffer), "%s    < %d >", activeMenu->items[index].text, horizontalIndex);
			}
			break;
		}

		const uvec2 textSize = GetTextSize(buffer, textScale);

		DrawText(output, 
				halfOutputSize - uvec2((activeMenu->items[index].centered ? textSize.x / 2 : halfOutputSize.x), offset),
				colors[index == selected.y], 
				textScale, 
				buffer);

		offset -= textScale * 8;
	}

	newSelection = false;
}