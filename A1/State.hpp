#ifndef BOOK_STATE_HPP
#define BOOK_STATE_HPP

#include "Common/d3dApp.h"

#include <memory>

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

class State
{
public:
	typedef std::unique_ptr<State> StatePtr;

	struct Context
	{
		Context(Game* _game, Player& _player);

		Game* game;
		Player* player;
	};

public:
	State(StateStack& stack, Context context);
	virtual ~State();

	virtual void Draw() = 0;

	virtual bool Update(const GameTimer& gt) = 0;
	virtual bool HandleEvent(WPARAM btnState) = 0;
	virtual bool HandleRealTimeInput() = 0;

	Context GetContext() const;

protected:
	void RequestStackPush(States::ID stateID);
	void RequestStackPop();
	void RequestStateClear();

	StateStack* mStack;
	Context mContext;
};

#endif //BOOK_STATE_HPP
