#pragma once
#include "Entity.hpp"
#include <string>

class Aircraft : public Entity
{
public:

	enum class Type
	{
		Eagle,
		Raptor,
	};


public:
	Aircraft(State* state);
	~Aircraft();
	//virtual unsigned int getCategory() const;


private:
	virtual void		UpdateCurrent(const GameTimer& gt);
	virtual void		drawCurrent() const override;
	virtual void		buildCurrent() override;


private:
	Type				mType;
	std::string			mSprite;
	RenderItem* mAircraftRitem;
};
