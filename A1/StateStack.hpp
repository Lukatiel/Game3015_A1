#ifndef BOOK_STATEIDENTIFIERS_HPP
#define BOOK_STATEIDENTIFIERS_HPP

#include "State.hpp"
#include "Common/d3dApp.h"

#include <vector>
#include <utility>
#include <functional>
#include <map>

using Microsoft::WRL::ComPtr;
using namespace DirectX;
using namespace DirectX::PackedVector;

class StateStack
{
public:
	enum Action
	{
		Push,
		Pop,
		Clear
	};

public:
	explicit StateStack(State::Context context);

	template <typename T>
	void registerState(States::ID stateID);
	

	void Update(const GameTimer& timer);
	void FrameResourceUpdate();
	void Draw();
	void HandleEvent(WPARAM btnState);
	void handleRealTimeInput();

	void pushState(States::ID stateID);
	void popState();
	void clearStates();

	bool isEmpty() const { return mStack.empty(); }

	XMFLOAT3 GetCameraPos();
	XMFLOAT3 GetTargetPos();
	int GetStackSize() { return mStack.size(); }
	State* GetCurrentState() { return mStack.back().get(); }
	State* GetPreviousState();

private:
	State::StatePtr createState(States::ID stateID);
	void applyPendingChanges();

	struct PendingChange
	{
		explicit PendingChange(Action action, States::ID stateID = States::None);

		Action action;
		States::ID stateID;
	};

private:
	std::vector<State::StatePtr> mStack;
	std::vector<PendingChange> mPendingList;
	std::map<States::ID, std::function<State::StatePtr()>> mFactories;

	State::Context mContext;

};
#endif