#ifdef __linux__

#include <unistd.h> // for close
#include <fcntl.h> // for open

#include <linux/input.h>
#include <sys/inotify.h>
// https://github.com/torvalds/linux/blob/master/include/uapi/linux/input-event-codes.h
// https://www.kernel.org/doc/Documentation/input/input.txt
// https://www.kernel.org/doc/Documentation/input/event-codes.txt
#include <stdio.h>

#include <dirent.h>

#include "Input.h"

Input::Input() {
	mouseX = 0;
	mouseY = 0;
}

Input::~Input() {
	removeAllInputs();
}

bool Input::addInput(const char* filename) {
	int fd = open(filename, O_RDONLY);
	if (fd < 0) {
		return false;
	}

	inputList.push_back(fd);
	return true;
}

bool Input::addAllInputs() {
	///proc/bus/input/devices
	removeAllInputs();

	static const char* path = "/dev/input";

	DIR* directory = NULL;
	struct dirent* ent = NULL;
	char tmp[512];
	int fd = 0;

	directory = opendir(path);
	if (directory == NULL) {
		return false;
	}

	while ((ent = readdir(directory)) != NULL) {
		sprintf(tmp, "%s/%s", path, ent->d_name);
		fd = open(tmp, O_RDONLY);

		if (fd < 0) {
			closedir(directory);
			return false;
		}

		inputList.push_back(fd);
	}

	closedir(directory);
	return true;
}

void Input::removeAllInputs() {
	for (unsigned int index = 0; index < inputList.size(); ++index) {
		close(inputList[index]);
	}
	inputList.clear();
}

void Input::removeInput(int index) {
	if (index < 0 || index >= inputList.size()) {
		return;
	}

	if (inputList[index] != -1) {
		close(inputList[index]);
		inputList[index] = -1;
	}

	inputList.erase(inputList.begin() + index);
}

bool Input::getEvent(Event& event) {
	input_event inputEvent;
	fd_set set;
	int maxFD = -1;

	FD_ZERO(&set);
	event.type = Event::UNKNOWN;

	for (unsigned int index = 0; index < inputList.size(); ++index) {
		FD_SET(inputList[index], &set);
		if (inputList[index] > maxFD) {
			maxFD = inputList[index];
		}
	}
	if (maxFD < 0) {
		return false;
	}

	if (select(maxFD + 1, &set, NULL, NULL, NULL) <= 0) {
		return false;
	}

	for (unsigned int index = 0; index < inputList.size(); ++index) {
		const int status = FD_ISSET(inputList[index], &set);
		if (status == -1) {
			removeInput(index);
			break;
		} else if (status == 0) {
			continue;
		}

		if (read(inputList[index], &inputEvent, sizeof(inputEvent)) != sizeof(inputEvent)) {
			continue;
		}

		switch (inputEvent.type) {
		case EV_SYN :
			break;

		case EV_KEY :
			switch (inputEvent.code) {
			case BTN_LEFT :
				event.type = Event::MOUSE_BUTTON;
				event.button = 1;
				event.state = inputEvent.value;
				return true;

			case BTN_RIGHT :
				event.type = Event::MOUSE_BUTTON;
				event.button = 2;
				event.state = inputEvent.value;
				return true;

			case BTN_MIDDLE :
				event.type = Event::MOUSE_BUTTON;
				event.button = 3;
				event.state = inputEvent.value;
				return true;

			case BTN_SIDE :
			case BTN_EXTRA :
			case BTN_BACK :
			case BTN_TASK :
				// Currently not used
				break;

			default :
				event.type = Event::KEYBOARD;
				event.keyCode = inputEvent.code;
				event.state = inputEvent.value;
				return true;
			}

		case EV_ABS :
			switch (inputEvent.code) {
			case REL_X :
				mouseX = inputEvent.value;

				event.type = Event::MOUSE_MOVE;
				event.x = mouseX;
				event.y = mouseY;
				return true;

			case REL_Y :
				mouseY = inputEvent.value;

				event.type = Event::MOUSE_MOVE;
				event.x = mouseX;
				event.y = mouseY;
				return true;
			}
			break;

		case EV_REL :
			switch (inputEvent.code) {
			case REL_X :
				mouseX += inputEvent.value;

				event.type = Event::MOUSE_MOVE;
				event.x = mouseX;
				event.y = mouseY;
				return true;
			case REL_Y :
				mouseY += inputEvent.value;

				event.type = Event::MOUSE_MOVE;
				event.x = mouseX;
				event.y = mouseY;
				return true;

			case REL_WHEEL :
				event.type = Event::MOUSE_WHEEL;
				event.state = inputEvent.value;
				return true;
			}
			break;

		default :
			//printf("Unhandled input_event(type: %d, code: %d, state: %d)\n", inputEvent.type, inputEvent.code, inputEvent.value);
			;
		}
	}

	return false;
}

#endif // __linux__