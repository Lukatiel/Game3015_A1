#pragma once
#include "Command.h"

#include <queue>

class CommandQueue
{
private:
	std::queue<Command> mQueue;

public:
	void push(const Command& command);
	Command pop();
	bool isEmpty() const;
};

