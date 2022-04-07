#include "SpriteNode.h"
#include "Game.hpp"

SpriteNode::SpriteNode(State* state) : Entity(state)
{
}

SpriteNode::~SpriteNode()
{

}

void SpriteNode::UpdateCurrent(const GameTimer& gt)
{
	Entity::updateCurrent(gt);
}

void SpriteNode::drawCurrent() const
{
	UINT objCBByteSize = d3dUtil::CalcConstantBufferByteSize(sizeof(ObjectConstants));
	UINT matCBByteSize = d3dUtil::CalcConstantBufferByteSize(sizeof(MaterialConstants));

	Game* game = mState->GetContext()->game;

	auto objectCB = game->mCurrFrameResource->ObjectCB->Resource();
	auto matCB = game->mCurrFrameResource->MaterialCB->Resource();

	if (mRenderItem != nullptr)
	{
		game->GetCmdList()->IASetVertexBuffers(0, 1, &mRenderItem->Geo->VertexBufferView());
		game->GetCmdList()->IASetIndexBuffer(&mRenderItem->Geo->IndexBufferView());
		game->GetCmdList()->IASetPrimitiveTopology(mRenderItem->PrimitiveType);

		CD3DX12_GPU_DESCRIPTOR_HANDLE tex(game->mSrvDescriptorHeap->GetGPUDescriptorHandleForHeapStart());
		tex.Offset(mRenderItem->Mat->DiffuseSrvHeapIndex, game->mCbvSrvDescriptorSize);

		D3D12_GPU_VIRTUAL_ADDRESS objCBAddress = objectCB->GetGPUVirtualAddress() + mRenderItem->ObjCBIndex * objCBByteSize;
		D3D12_GPU_VIRTUAL_ADDRESS matCBAddress = matCB->GetGPUVirtualAddress() + mRenderItem->Mat->MatCBIndex * matCBByteSize;

		game->GetCmdList()->SetGraphicsRootDescriptorTable(0, tex);
		game->GetCmdList()->SetGraphicsRootConstantBufferView(1, objCBAddress);
		game->GetCmdList()->SetGraphicsRootConstantBufferView(3, matCBAddress);

		game->GetCmdList()->DrawIndexedInstanced(mRenderItem->IndexCount, 1, mRenderItem->StartIndexLocation, mRenderItem->BaseVertexLocation, 0);
	}

}

void SpriteNode::buildCurrent()
{
	Game* game = mState->GetContext()->game;
	
	auto render = std::make_unique<RenderItem>();
	mRenderItem = render.get();

	mRenderItem->ObjCBIndex = mState->GetRenderItems().size();
	mRenderItem->Mat = game->GetMaterials()[mMaterialName].get();
	mRenderItem->Geo = game->GetGeometries()[mGeometriesName].get();
	mRenderItem->PrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	mRenderItem->IndexCount = mRenderItem->Geo->DrawArgs[mDrawArgsName].IndexCount;
	mRenderItem->StartIndexLocation = mRenderItem->Geo->DrawArgs[mDrawArgsName].StartIndexLocation;
	mRenderItem->BaseVertexLocation = mRenderItem->Geo->DrawArgs[mDrawArgsName].BaseVertexLocation;
	mState->GetRenderItems().push_back(std::move(render));

}
