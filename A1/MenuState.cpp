#include "MenuState.hpp"
#include "SpriteNode.h"
#include "Game.hpp"

MenuState::MenuState(StateStack* stack, Context* context)
	: State(stack, context)
{
	mCameraPos = XMFLOAT3(0.0f, 0.0f, -10.0f);
	mTargetPos = XMFLOAT3(0.0f, 0.0f, 0.0f);

	// Background
	std::unique_ptr<SpriteNode> backgroundSprite = std::make_unique<SpriteNode>(this);
	backgroundSprite->SetMatGeoDrawName("MenuBackgroundMat", "shapeGeo", "quad");
	backgroundSprite->SetScale(XMVectorSet(17, 11, 1, 0));
	backgroundSprite->SetPosition(XMVectorSet(0, 0, 1, 0));
	mSceneGraph->attachChild(std::move(backgroundSprite));

	// Text
	std::unique_ptr<SpriteNode> textSprite = std::make_unique<SpriteNode>(this);
	textSprite->SetMatGeoDrawName("MenuInstructionsMat", "shapeGeo", "quad");
	textSprite->SetScale(XMVectorSet(7, 4, 1, 0));
	textSprite->SetPosition(XMVectorSet(0, -2, 0, 0));
	mSceneGraph->attachChild(std::move(textSprite));

	mSceneGraph->build();

	mContext->game->ResetFrameResources();

	mContext->game->BuildFrameResorces(mAllRitems.size());
}

void MenuState::Draw()
{
	mSceneGraph->draw();
}

bool MenuState::Update(const GameTimer& gt)
{
	mSceneGraph->update(gt);

	return true;
}

bool MenuState::HandleEvent(WPARAM btnState)
{
	//If P is Pressed
	if (btnState == 'P' || btnState == 'p')
	{
		RequestStackPop();
		RequestStackPush(States::Game);
	}
	//If N is Pressed
	if (btnState == 'N' || btnState == 'n')
	{
		RequestStackPop();
	}

	return true;
}

bool MenuState::HandleRealTimeInput()
{
	return true;
}