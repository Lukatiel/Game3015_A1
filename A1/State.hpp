#ifndef BOOK_STATE_HPP
#define BOOK_STATE_HPP

#include "StateStack.hpp"
#include "Common/d3dApp.h"
#include "FrameResource.h"
#include "SceneNode.hpp"

#include <memory>

using Microsoft::WRL::ComPtr;
using namespace DirectX;
using namespace DirectX::PackedVector;

namespace sf
{
	class RenderWindow;
}

namespace States
{
	enum ID
	{
		None,
		Title,
		Menu,
		Game,
		Loading,
		Pause
	};
}

class StateStack;
class Player;
class Game;
class SceneNode;

class State
{
public :
	typedef std::unique_ptr<State> StatePtr;

	struct Context
	{
		Context(Game* _game, Player* _player);

		Game* game;
		Player* player;
	};

public:
	State(StateStack* stack, Context* context);
	virtual ~State();

	virtual void Draw() = 0;

	virtual bool Update(const GameTimer& gt) = 0;
	virtual bool HandleEvent(WPARAM btnState) = 0;
	virtual bool HandleRealTimeInput() = 0;

	XMFLOAT3 GetCameraPos() { return mCameraPos; }
	XMFLOAT3 GetTargetPos() { return mTargetPos; }

	std::vector<std::unique_ptr<RenderItem>>& GetRenderItems() { return mAllRitems; }
	Context* GetContext() const;

protected:
	void RequestStackPush(States::ID stateID);	
	void RequestStackPop();
	void RequestStateClear();

	StateStack* mStack;
	Context* mContext;

	XMFLOAT3 mCameraPos;
	XMFLOAT3 mTargetPos;

	std::unique_ptr<SceneNode> mSceneGraph;
	std::vector<std::unique_ptr<RenderItem>> mAllRitems;
};

#endif //BOOK_STATE_HPP