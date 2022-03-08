#pragma region step 2
#include "Player.h"
#include "CommandQueue.h"
#include "Aircraft.hpp"
#include "Common/MathHelper.h"
#include "Common/d3dApp.h"

#include <map>
#include <string>
#include <algorithm>
#include <stdio.h>

using namespace DirectX;

struct AircraftMover
{
	AircraftMover(float vx, float vy, float vz)
		: velocity(vx, vy, vz)
	{

	}

	void operator() (Aircraft& aircraft, const GameTimer&) const
	{
		aircraft.accelerate(velocity);
	}

	XMFLOAT3 velocity;
};

Player::Player()
{
	//Set initial Key Bindings


	mKeyBinding[VK_LEFT] = MoveLeft;
	mKeyBinding[VK_RIGHT] = MoveRight;
	mKeyBinding[VK_UP] = MoveUp;
	mKeyBinding[VK_DOWN] = MoveDown;

	mKeyBinding['A'] = MoveLeft;
	mKeyBinding['D'] = MoveRight;
	mKeyBinding['W'] = MoveUp;
	mKeyBinding['S'] = MoveDown;

	//Set Initial Action Bindings
	InitializeActions();

	for (auto pair : mKeyBinding)
	{
		mKeyFlag[pair.first] = false;
	}

	//Assign All Categories to Player's Aircraft
	for (auto& pair : mActionBinding)
	{
		pair.second.category = Category::PlayerAircraft;
	}
}

void Player::HandleEvent(CommandQueue& commands)
{
	for (auto& pair : mKeyBinding)
	{
		if (!isRealTimeAction(pair.second))
		{
			if (mKeyFlag[pair.first])
			{
				if (!GetAsyncKeyState(pair.first))
				{
					mKeyFlag[pair.first] = false; //RELEASED

				}
			}
			else
			{
				if (GetAsyncKeyState(pair.first) & 0x8000)
				{
					mKeyFlag[pair.first] = true; //PRESSED
					commands.push(mActionBinding[pair.second]);
				}
			}
		}
	}

	
}

void Player::HandeRealTimeInput(CommandQueue & commands)
{
	for (auto pair : mKeyBinding)
	{
		if (GetAsyncKeyState(pair.first) & 0x8000 && isRealTimeAction(pair.second))
		{
			commands.push(mActionBinding[pair.second]); //Holding Down
		}
	}
}

void Player::AssignKey(Action action, char key)
{
	//Remove all keys that already map action
	for (auto itr = mKeyBinding.begin(); itr != mKeyBinding.end();)
	{
		if (itr->second == action)
			mKeyBinding.erase(itr++);
		else
			++itr;
	}

	//Insert new binding
	mKeyBinding[key] = action;
}

char Player::getAssignedKey(Action action) const
{
	for (auto pair : mKeyBinding)
	{
		if (pair.second == action)
			return pair.first;
	}

	return 0x00;
}

void Player::InitializeActions()
{
	const float playerSpeed = 50.f;

	mActionBinding[MoveLeft].action = derivedAction<Aircraft>(AircraftMover(-playerSpeed, 0.f, 0.0f));
	mActionBinding[MoveRight].action = derivedAction<Aircraft>(AircraftMover(+playerSpeed, 0.f, 0.0f));
	mActionBinding[MoveUp].action = derivedAction<Aircraft>(AircraftMover( 0.f, 0.0f,+playerSpeed));
	mActionBinding[MoveDown].action = derivedAction<Aircraft>(AircraftMover(0.f, 0.0f, -playerSpeed));
}

bool Player::isRealTimeAction(Action action)
{
	switch (action)
	{
	case Player::MoveLeft:
	case Player::MoveRight:
	case Player::MoveDown:
	case Player::MoveUp:
		return true;
	default:
		return false;
	}
}

#pragma endregion