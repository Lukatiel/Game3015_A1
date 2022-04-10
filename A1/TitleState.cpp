#include "TitleState.hpp"
#include "SpriteNode.h"
#include "Game.hpp"

TitleState::TitleState(StateStack& stack, Context context)
	: State(stack, context)
	, mWorld(&(context.game->mWorld))
{
	context.game->mAllRitems.clear();
	context.game->mOpaqueRitems.clear();
	context.game->ResetFrameResources();
	

	//Background
	std::unique_ptr<SpriteNode> backgroundSprite = std::make_unique<SpriteNode>(context.game);
	backgroundSprite->SetMatGeoDrawName("TitleBackgroundMat", "shapeGeo", "quad");
	backgroundSprite->setScale(17, 11, 1);
	backgroundSprite->setPosition(0, -2, 0);
	mWorld->GetSceneGraph()->attachChild(std::move(backgroundSprite));

	// Text
	std::unique_ptr<SpriteNode> textSprite = std::make_unique<SpriteNode>(context.game);
	textSprite->SetMatGeoDrawName("TitleTextMat", "shapeGeo", "quad");
	textSprite->setScale(7, 4, 1);
	textSprite->setPosition(0, -2, 0);
	mWorld->GetSceneGraph()->attachChild(std::move(textSprite));


	context.game->BuildFrameResorces(context.game->mAllRitems.size());
}

void TitleState::Draw()
{
	mWorld->draw();

}

bool TitleState::Update(const GameTimer& gt)
{
	mWorld->update(gt);

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
