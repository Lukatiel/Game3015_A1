#pragma once
#include "State.hpp"
#include "World.hpp"

class GameState : public State
{
public:
	GameState(StateStack& stack, Context context);

	virtual void Draw();
	virtual bool Update(const GameTimer& gt);
	virtual bool HandleEvent(WPARAM btnState);
	virtual bool HandleRealTimeInput();

	std::unique_ptr<SpriteNode> back;

private:
	World* mWorld;
	Player* mPlayer;

public:
	std::unique_ptr<SceneNode> mPauseStateSceneGraph;
};

