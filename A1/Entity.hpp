#pragma once
#include "SceneNode.hpp"

class Entity : public SceneNode
{
public:
	Entity(State* state);
	~Entity();

	void				setVelocity(FXMVECTOR velocity);
	XMFLOAT3			getVelocity() const;


	virtual	void		updateCurrent(const GameTimer& gt);

public:
	XMFLOAT3		mVelocity;
};

