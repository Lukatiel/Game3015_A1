#pragma once
#include "Common/d3dApp.h"
#include "FrameResource.h"
#include "Category.hpp"

#include <vector>
#include <memory>

using Microsoft::WRL::ComPtr;
using namespace DirectX;
using namespace DirectX::PackedVector;

// Lightweight structure stores parameters to draw a shape.  This will
// vary from app-to-app.
struct RenderItem
{
	RenderItem() = default;

	// World matrix of the shape that describes the object's local space
	// relative to the world space, which defines the position, orientation,
	// and scale of the object in the world.
	//XMFLOAT4X4 World = MathHelper::Identity4x4();

	XMFLOAT4X4 TexTransform = MathHelper::Identity4x4();

	// Dirty flag indicating the object data has changed and we need to update the constant buffer.
	// Because we have an object cbuffer for each FrameResource, we have to apply the
	// update to each FrameResource.  Thus, when we modify obect data we should set 
	// NumFramesDirty = gNumFrameResources so that each frame resource gets the update.
	int NumFramesDirty = gNumFrameResources;

	// Index into GPU constant buffer corresponding to the ObjectCB for this render item.
	UINT ObjCBIndex = -1;

	Material* Mat = nullptr;
	MeshGeometry* Geo = nullptr;

	// Primitive topology.
	D3D12_PRIMITIVE_TOPOLOGY PrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	// DrawIndexedInstanced parameters.
	UINT IndexCount = 0;
	UINT StartIndexLocation = 0;
	int BaseVertexLocation = 0;
};

class State;
struct Command;
struct FrameResource;


class SceneNode
{
public:
	typedef std::unique_ptr<SceneNode> Ptr;


public:
	SceneNode(State* state);
	~SceneNode();

	void					attachChild(Ptr child);
	Ptr						detachChild(const SceneNode& node);
	const XMFLOAT4X4* GetWorldMatrix() const { return &mWorldMatrix; }

	void					update(const GameTimer& gt);
	void					draw() const;
	void					build();

	void					SetRotation(XMVECTOR rotation) { XMStoreFloat3(&mRotation, rotation); }
	void					SetPosition(XMVECTOR position) { XMStoreFloat3(&mPosition, position); }
	void					SetScale(XMVECTOR scale)	   { XMStoreFloat3(&mScale, scale); }

	void					SetTexScale(XMVECTOR scale);

	XMFLOAT3				GetPosition() const { return mPosition; }
	XMFLOAT3				GetScale() const { return mScale; }

	void					onCommand(const Command& command, const GameTimer& gt);
	unsigned int			getCategory() const {return mCategory; }
	void					SetCategory(unsigned int category) { mCategory = category; }

	void					SetMatGeoDrawName(std::string materialName, std::string geometriesName, std::string drawArgsName);

private:
	virtual void			updateCurrent(const GameTimer& gt);
	void					updateChildren(const GameTimer& gt);

	virtual void			drawCurrent() const;
	void					drawChildren() const;
	virtual void			buildCurrent();
	void					buildChildren();

protected:
	State* mState;
	std::vector<Ptr> mChildren;
	SceneNode* mParent;
	RenderItem* mRenderItem;
	unsigned int mCategory = Category::Scene;

	std::string mMaterialName;
	std::string mGeometriesName;
	std::string mDrawArgsName;
	
	XMFLOAT3 mScale = XMFLOAT3(1.0f, 1.0f, 1.0f);
	XMFLOAT3 mRotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3 mPosition = XMFLOAT3(0.0f, 0.0f, 0.0f);

	XMFLOAT4X4 mWorldMatrix = MathHelper::Identity4x4();

};

