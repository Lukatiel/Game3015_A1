#pragma once
#include "FrameResource.h"
#include "SceneNode.hpp"
#include "CommandQueue.h"
#include "Command.h"

#include "Common/d3dApp.h"
#include "Common/MathHelper.h"
#include "Common/UploadBuffer.h"
#include "Common/GeometryGenerator.h"

using Microsoft::WRL::ComPtr;
using namespace DirectX;
using namespace DirectX::PackedVector;

class Aircraft;
class SpriteNode;
class State;

class World
{
public:
	World(State* state);
	~World();
	void								update(const GameTimer& gt);
	void								draw();
	//void								loadTextures();
	void								buildScene();

	const Aircraft* GetPlayer() const { return mPlayerAircraft; }

	CommandQueue& GetCommandQueue() { return mCommandQueue; };
	void								adaptPlayerPosition();
	void								adaptPlayerVelocity();


private:
	enum class Layer
	{
		Air,
		Background,
		LayerCount
	};

	State* mState;
	CommandQueue mCommandQueue;
	SceneNode* mSceneGraph;
	std::array<SceneNode*, 2>	mSceneLayers;
	Aircraft* mPlayerAircraft;
	SpriteNode* mBackground;
	float mPlayerMaxSpeed = 5.0f;
};
