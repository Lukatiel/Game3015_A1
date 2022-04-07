#include "GameState.h"
#include "SpriteNode.h"
#include "Game.hpp"

GameState::GameState(StateStack* stack, Context* context)
	: State(stack, context)
	, mWorld(this)
	, mPlayer(context->player)
	, mPauseStateSceneGraph(std::make_unique<SceneNode>(this))
{
	mCameraPos = XMFLOAT3(0.0f, 0.0f, -30.0f);
	mTargetPos = XMFLOAT3(0.0f, 0.0f, 0.0f);

	// Build pause state scene graph. 
	std::unique_ptr<SpriteNode> overlaySprite = std::make_unique<SpriteNode>(this);
	overlaySprite->SetMatGeoDrawName("PauseOverlayMat", "shapeGeo", "quad");
	overlaySprite->SetScale(XMVectorSet(23, 20, 1, 0));
	overlaySprite->SetPosition(XMVectorSet(0, 0, -1, 0));
	mPauseStateSceneGraph->attachChild(std::move(overlaySprite));
	mPauseStateSceneGraph->build();

	mWorld.buildScene();
	mContext->game->ResetFrameResources();
	mContext->game->BuildFrameResorces(mAllRitems.size());
}

void GameState::Draw()
{
	mWorld.draw();
}

bool GameState::Update(const GameTimer& gt)
{
	mWorld.update(gt);

	return true;
}

bool GameState::HandleEvent(WPARAM btnState)
{
	//Handle player input
	CommandQueue& commands = mWorld.GetCommandQueue();
	mPlayer->handleEvent(commands, btnState);

	//If ESC is pressed
	if (btnState == VK_ESCAPE)
	{
		//Push Pause State
		RequestStackPush(States::Pause);
	}

	return true;
}

bool GameState::HandleRealTimeInput()
{
	//Handle player input
	CommandQueue& commands = mWorld.GetCommandQueue();
	mPlayer->handleRealtimeInput(commands);

	return true;
}