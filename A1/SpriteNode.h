#pragma once
#include "Entity.hpp"

class SpriteNode :
	public Entity
{
public:
	SpriteNode(State* state);
	~SpriteNode();

private:
	virtual void		drawCurrent() const;
	virtual void		buildCurrent();
	virtual void		UpdateCurrent(const GameTimer& gt);
};
