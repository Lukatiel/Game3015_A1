#define NOMINMAX
#include "World.hpp"
#include "Aircraft.hpp"
#include "SpriteNode.h"

World::World(State* state)
	: mSceneGraph(new SceneNode(state))
	, mState(state)
	, mPlayerAircraft(nullptr)
	, mBackground(nullptr)
{
}

World::~World()
{

}

void World::update(const GameTimer& gt)
{
	//Forward commands to scene graph, adapt velocity
	while (!mCommandQueue.isEmpty())
	{
		mSceneGraph->onCommand(mCommandQueue.pop(), gt);
	}

	adaptPlayerVelocity();

	mSceneGraph->update(gt);

	adaptPlayerPosition();
}

void World::draw()
{
	mSceneGraph->draw();
}

void World::buildScene()
{
	//Add background
	std::unique_ptr<SpriteNode> backgroundSprite = std::make_unique<SpriteNode>(mState);
	mBackground = backgroundSprite.get();
	backgroundSprite->SetMatGeoDrawName("DesertMat", "shapeGeo", "quad");
	backgroundSprite->SetScale(XMVectorSet(50, 200, 1, 0));
	backgroundSprite->SetPosition(XMVectorSet(0, 0, 1, 0));
	backgroundSprite->setVelocity(XMVectorSet(0.f, -3.f, 0.f, 0.f));
	mSceneGraph->attachChild(std::move(backgroundSprite));


	//Add player aircraft
	std::unique_ptr<Aircraft> playerAircraft = std::make_unique<Aircraft>(mState);
	mPlayerAircraft = playerAircraft.get();
	playerAircraft->SetMatGeoDrawName("EagleMat", "shapeGeo", "quad");
	playerAircraft->SetScale(XMVectorSet(1, 1, 1, 0));
	playerAircraft->SetPosition(XMVectorSet(0, 0, 0, 0));
	playerAircraft->setVelocity(XMVectorSet(0.f, 0.f, 0.f, 0.f));
	playerAircraft->SetCategory(Category::PlayerAircraft);
	mSceneGraph->attachChild(std::move(playerAircraft));

	//left Escort
	std::unique_ptr<Aircraft> leftEscort = std::make_unique<Aircraft>(mState);
	leftEscort->SetMatGeoDrawName("RaptorMat", "shapeGeo", "quad");
	leftEscort->SetScale(XMVectorSet(1, 1, 1, 0));
	leftEscort->SetPosition(XMVectorSet(-1, -1, 0, 0));
	leftEscort->SetCategory(Category::AlliedAircraft);
	mPlayerAircraft->attachChild(std::move(leftEscort));

	//right Escort
	std::unique_ptr<Aircraft> rightEscort = std::make_unique<Aircraft>(mState);
	rightEscort->SetMatGeoDrawName("RaptorMat", "shapeGeo", "quad");
	rightEscort->SetScale(XMVectorSet(1, 1, 1, 0));
	rightEscort->SetPosition(XMVectorSet(1, -1, 0, 0));
	rightEscort->SetCategory(Category::AlliedAircraft);
	mPlayerAircraft->attachChild(std::move(rightEscort));

	//Build scene graph with render item
	mSceneGraph->build();


	// Set background texture scale
	mBackground->SetTexScale(XMVectorSet(3, 10, 1, 0));
}

void World::adaptPlayerPosition()
{
	XMFLOAT3 position = mPlayerAircraft->GetPosition();

	if (position.x <= -15)
	{
		position.x = -15;
	}
	if (position.x >= 15)
	{
		position.x = 15;
	}

	if (position.y >= 10)
	{
		position.y = 10;
	}
	if (position.y <= -10)
	{
		position.y = -10;
	}

	mPlayerAircraft->SetPosition(XMLoadFloat3(&position));
}

void World::adaptPlayerVelocity()
{
	XMFLOAT3 velocity = mPlayerAircraft->getVelocity();

	XMFLOAT3 magnitude;
	XMStoreFloat3(&magnitude, XMVector3Length(XMLoadFloat3(&velocity)));

	if (magnitude.x >= mPlayerMaxSpeed)
	{
		XMVECTOR normalized = XMVector3Normalize(XMLoadFloat3(&velocity));
		XMVECTOR maxVelocity = normalized * mPlayerMaxSpeed;
		mPlayerAircraft->setVelocity(maxVelocity);
	}

}
