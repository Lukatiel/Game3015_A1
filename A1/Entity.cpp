#include "Entity.hpp"
#include "FrameResource.h"
#include "Game.hpp"
#include "State.hpp"

Entity::Entity(State* state) : SceneNode(state), mVelocity(0, 0, 0)
{
}

Entity::~Entity()
{

}

void Entity::setVelocity(FXMVECTOR velocity)
{
	XMStoreFloat3(&mVelocity, velocity);
}



XMFLOAT3 Entity::getVelocity() const
{
	return mVelocity;
}

void Entity::updateCurrent(const GameTimer& gt)
{
	//If an item needs to be rendered
	if (mRenderItem)
	{
		//Updating position based off velocity
		XMStoreFloat3(&mPosition, XMLoadFloat3(&mPosition) + XMLoadFloat3(&mVelocity) * gt.DeltaTime());

		// Make a new world matrix
		XMStoreFloat4x4(&mWorldMatrix,
			XMMatrixScalingFromVector(XMLoadFloat3(&mScale)) *
			XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&mRotation)) *
			XMMatrixTranslationFromVector(XMLoadFloat3(&mPosition)));

		// Take parent's matrix into account
		if (mParent != nullptr)
		{
			XMStoreFloat4x4(&mWorldMatrix,
				XMLoadFloat4x4(mParent->GetWorldMatrix()) * XMLoadFloat4x4(&mWorldMatrix));
		}

		Game* game = mState->GetContext()->game;

		auto currObjectCB = game->GetCurrFrameResource()->ObjectCB.get();

		// Dirty flag indicating the object data has changed and we need to update the constant buffer.
		// Because we have an object cbuffer for each FrameResource, we have to apply the
		// update to each FrameResource.  Thus, when we modify obect data we should set 
		// NumFramesDirty = gNumFrameResources so that each frame resource gets the update.
		//mRenderItem->NumFramesDirty = gNumFrameResources;
		mRenderItem->NumFramesDirty++;

		// Only update the cbuffer data if the constants have changed.  
		// This needs to be tracked per frame resource.
		if (mRenderItem->NumFramesDirty > 0)
		{
			XMMATRIX world = XMLoadFloat4x4(&mWorldMatrix);
			XMMATRIX texTransform = XMLoadFloat4x4(&mRenderItem->TexTransform);

			ObjectConstants objConstants;
			XMStoreFloat4x4(&objConstants.World, XMMatrixTranspose(world));
			XMStoreFloat4x4(&objConstants.TexTransform, XMMatrixTranspose(texTransform));

			currObjectCB->CopyData(mRenderItem->ObjCBIndex, objConstants);

			// Next FrameResource need to be updated too.
			mRenderItem->NumFramesDirty--;
		}
	}
}
