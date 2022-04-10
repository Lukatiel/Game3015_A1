#pragma once
#include "State.hpp"
#include "World.hpp"

class MenuState : public State
{
public:
	MenuState(StateStack& stack, Context context);

	virtual void Draw();
	virtual bool Update(const GameTimer& gt);
	virtual bool HandleEvent(WPARAM btnState);
	virtual bool HandleRealTimeInput();

	World* mWorld;
};

