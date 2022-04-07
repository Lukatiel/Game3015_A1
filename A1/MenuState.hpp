#pragma once
#include "State.hpp"

class MenuState : public State
{
public: 
	MenuState(StateStack* stack, Context* context);

	virtual void Draw();
	virtual bool Update(const GameTimer& gt);
	virtual bool HandleEvent(WPARAM btnState);
	virtual bool HandleRealTimeInput();
};

