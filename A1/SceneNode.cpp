#include "SceneNode.hpp"
#include "Game.hpp"

SceneNode::SceneNode(State* state)
	: mState(state)
	, mChildren()
	, mParent(nullptr)
	
{
	
}

SceneNode::~SceneNode()
{

}

void SceneNode::attachChild(Ptr child)
{
	child->mParent = this;
	mChildren.push_back(std::move(child));
}

SceneNode::Ptr SceneNode::detachChild(const SceneNode& node)
{
	auto found = std::find_if(mChildren.begin(), mChildren.end(), [&](Ptr& p) { return p.get() == &node; });
	assert(found != mChildren.end());

	Ptr result = std::move(*found);
	result->mParent = nullptr;
	mChildren.erase(found);
	return result;
}

void SceneNode::update(const GameTimer& gt)
{
	updateCurrent(gt);
	updateChildren(gt);
}

void SceneNode::updateCurrent(const GameTimer& gt)
{
	XMStoreFloat4x4(&mWorldMatrix,
		XMMatrixScalingFromVector(XMLoadFloat3(&mScale)) *
		XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&mRotation)) *
		XMMatrixTranslationFromVector(XMLoadFloat3(&mPosition)));
}

void SceneNode::updateChildren(const GameTimer& gt)
{
	for (Ptr& child : mChildren)
	{
		child->update(gt);
	}
}

void SceneNode::draw() const
{
	drawCurrent();
	drawChildren();
}

void SceneNode::drawCurrent() const
{
	//Empty for now
}

void SceneNode::drawChildren() const
{
	for (const Ptr& child : mChildren)
	{
		child->draw();
	}
}

void SceneNode::build()
{
	buildCurrent();
	buildChildren();
}

void SceneNode::buildCurrent()
{
	//Empty for now
}

void SceneNode::buildChildren()
{
	for (const Ptr& child : mChildren)
	{
		child->build();
	}
}


void SceneNode::onCommand(const Command& command, const GameTimer& gt)
{
	// Command current node, if category matches
	if (command.category & getCategory())
		command.action(*this, gt);

	// Command children
	for (Ptr& child : mChildren)
		child->onCommand(command, gt);
}

void SceneNode::SetMatGeoDrawName(std::string materialName, std::string geometriesName, std::string drawArgsName)
{
	mMaterialName = materialName;
	mGeometriesName = geometriesName;
	mDrawArgsName = drawArgsName;
}

void SceneNode::SetTexScale(XMVECTOR scale)
{
	XMStoreFloat4x4(&mRenderItem->TexTransform, XMMatrixScalingFromVector(scale));
}