#include "TitleState.hpp"
#include "SpriteNode.h"
#include "Game.hpp"

TitleState::TitleState(StateStack* stack, Context* context)
	: State(stack, context)
{
	mCameraPos = XMFLOAT3(0.0f, 0.0f, -10.0f);
	mTargetPos = XMFLOAT3(0.0f, 0.0f, 0.0f);

	//Background
	std::unique_ptr<SpriteNode> backgroundSprite = std::make_unique<SpriteNode>(this);
	backgroundSprite->SetMatGeoDrawName("TitleBackgroundMat", "shapeGeo", "quad");
	backgroundSprite->SetScale(XMVectorSet(17, 11, 1, 0));
	backgroundSprite->SetPosition(XMVectorSet(0, -2, 0, 0));
	mSceneGraph->attachChild(std::move(backgroundSprite));

	// Text
	std::unique_ptr<SpriteNode> textSprite = std::make_unique<SpriteNode>(this);
	textSprite->SetMatGeoDrawName("TitleTextMat", "shapeGeo", "quad");
	textSprite->SetScale(XMVectorSet(7, 4, 1, 0));
	textSprite->SetPosition(XMVectorSet(0, -2, 0, 0));
	mSceneGraph->attachChild(std::move(textSprite));

	mSceneGraph->build();

	mContext->game->BuildFrameResorces(mAllRitems.size());
}

void TitleState::Draw()
{
	mSceneGraph->draw();
}

bool TitleState::Update(const GameTimer& gt)
{
	mSceneGraph->update(gt);

	return true;
}

bool TitleState::HandleEvent(WPARAM btnState)
{
	//For this state is doesnt matter what button is pressed
	RequestStackPop();
	RequestStackPush(States::Menu);

	return true;
}

bool TitleState::HandleRealTimeInput()
{
	return true;
}
