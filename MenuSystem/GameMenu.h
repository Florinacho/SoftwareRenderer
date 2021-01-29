#ifndef __GAME_MENU_H__
#define __GAME_MENU_H__

#include "MenuManager.h"

static const char *MapNames[] = {
	"Default S",
	"Default M",
	"Default L",
	"Default XL",
	"Garden",
	"Demo",
	"Random"
};

static const char *PlayerColors[] = {
	"N/A",
	"White",
	"Black",
	"Red",
	"Green",
	"Blue"
};

static const char *TrueFalseText[] = {
	"False",
	"True",
};

static const char *ResolutionText[] = {
	"640x480",
	"800x600",
	"1024x768",
	"1280x1024",
};

bool inGame = false;
bool running = true;

void HideMenu(MenuManager *manager);
void RestartGame(MenuManager *manager);
void GoToMainMenu(MenuManager *manager);
void GoToNewGame(MenuManager *manager);
void StartGame(MenuManager *manager);
void QuitGame(MenuManager *manager);
void GoToOptions(MenuManager *manager);
void ApplyOptions(MenuManager *manager);
void ExitGame(MenuManager *manager);

Menu MainMenu = {
	NULL,
	Menu::DEFAULT,
	3,
	{
		{"New Game", 5, true, GoToNewGame, 0, 0, NULL},
		{"Options",  5, true, GoToOptions, 0, 0, NULL},
		{"Exit",     5, true, ExitGame,    0, 0, NULL},
	}
};

Menu InGameMenu = {
	NULL,
	Menu::DEFAULT,
	3,
	{
		{"Resume",  3, true, HideMenu,    0, 0, NULL},
		{"Restart", 3, true, RestartGame, 0, 0, NULL},
		{"Quit",    3, true, QuitGame,    0, 0, NULL},
	}
};

Menu NewGameMenu = {
	&MainMenu,
	Menu::DEFAULT,
	7,
	{
		{"Map     ", 3, false, NULL,         7, 0, MapNames},
		{"",         3, false, NULL,         0, 0, NULL},
		{"Player 1", 3, false, NULL,         6, 1, PlayerColors},
		{"Player 2", 3, false, NULL,         6, 2, PlayerColors},
		{"",         3, false, NULL,         0, 0, NULL},
		{"Start",    3, false, StartGame,    0, 0, NULL},
		{"Back",     3, false, GoToMainMenu, 0, 0, NULL},
	}
};

Menu OptionsMenu = {
	&MainMenu,
	Menu::DEFAULT,
	6,
	{
		{"Rezolution", 3, false, NULL,           4, 0, ResolutionText},
		{"Fullscreen", 3, false, NULL,           2, 0, TrueFalseText},
		{"Volume    ", 3, false, NULL,          11, 5, NULL},
		{"",           3, false, NULL,           0, 0, NULL},
		{"Apply",      3, false, ApplyOptions,   0, 0, NULL},
		{"Back",       3, false, GoToMainMenu,   0, 0, NULL},
	}
};

void HideMenu(MenuManager *manager) {
	manager->setActiveMenu(NULL);
}

void RestartGame(MenuManager *manager) {
	printf("Restart game\n");
}

void GoToMainMenu(MenuManager *manager) {
	manager->setActiveMenu(&MainMenu);
}

void GoToNewGame(MenuManager *manager) {
	manager->setActiveMenu(&NewGameMenu);
}

void StartGame(MenuManager *manager) {
	manager->setActiveMenu(&InGameMenu);
	inGame = true;
}

void QuitGame(MenuManager *manager) {
	manager->setActiveMenu(&MainMenu);
	inGame = false;
}

void GoToOptions(MenuManager *manager) {
	manager->setActiveMenu(&OptionsMenu);
}

void ApplyOptions(MenuManager *manager) {
	int tmp;
	printf("Apply the following options:\n");
	tmp = OptionsMenu.items[0].selected;
	printf("Rezolution: %s\n", OptionsMenu.items[0].values[tmp]);
	tmp = OptionsMenu.items[1].selected;
	printf("Fullscreen: %s\n", OptionsMenu.items[1].values[tmp]);
	printf("Volume:     %d\n", OptionsMenu.items[2].selected);
	printf("\n");
}

void ExitGame(MenuManager *manager) {
	running = false;
}

#endif // __GAME_MENU_H__