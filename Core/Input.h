#ifndef __INPUT_H__
#define __INPUT_H__

#include <vector>
#include "Event.h"

class Input {
	std::vector<int> inputList;

	int mouseX;
	int mouseY;

public:
	Input();

	~Input();

	bool addInput(const char* filename);

	bool addAllInputs();

	void removeInput(int index);

	void removeAllInputs();

	bool getEvent(Event& event);
};

#endif //__INPUT_H__
