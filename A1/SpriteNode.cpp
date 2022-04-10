#include "SpriteNode.h"
#include "Game.hpp"

SpriteNode::SpriteNode(Game* game) : Entity(game)
{
}

void SpriteNode::drawCurrent() const
{
	mRenderItem->World = getTransform();
	mRenderItem->NumFramesDirty++;

	UINT objCBByteSize = d3dUtil::CalcConstantBufferByteSize(sizeof(ObjectConstants));
	UINT matCBByteSize = d3dUtil::CalcConstantBufferByteSize(sizeof(MaterialConstants));

	auto objectCB = game->mCurrFrameResource->ObjectCB->Resource();
	auto matCB = game->mCurrFrameResource->MaterialCB->Resource();

	if (mSpriteNodeRitem != nullptr)
	{
		game->getCmdList()->IASetVertexBuffers(0, 1, &mSpriteNodeRitem->Geo->VertexBufferView());
		game->getCmdList()->IASetIndexBuffer(&mSpriteNodeRitem->Geo->IndexBufferView());
		game->getCmdList()->IASetPrimitiveTopology(mSpriteNodeRitem->PrimitiveType);

		CD3DX12_GPU_DESCRIPTOR_HANDLE tex(game->mSrvDescriptorHeap->GetGPUDescriptorHandleForHeapStart());
		tex.Offset(mSpriteNodeRitem->Mat->DiffuseSrvHeapIndex, game->mCbvSrvDescriptorSize);

		D3D12_GPU_VIRTUAL_ADDRESS objCBAddress = objectCB->GetGPUVirtualAddress() + mSpriteNodeRitem->ObjCBIndex * objCBByteSize;
		D3D12_GPU_VIRTUAL_ADDRESS matCBAddress = matCB->GetGPUVirtualAddress() + mSpriteNodeRitem->Mat->MatCBIndex * matCBByteSize;

		game->getCmdList()->SetGraphicsRootDescriptorTable(0, tex);
		game->getCmdList()->SetGraphicsRootConstantBufferView(1, objCBAddress);
		game->getCmdList()->SetGraphicsRootConstantBufferView(3, matCBAddress);

		game->getCmdList()->DrawIndexedInstanced(mSpriteNodeRitem->IndexCount, 1, mSpriteNodeRitem->StartIndexLocation, mSpriteNodeRitem->BaseVertexLocation, 0);
	}

}

void SpriteNode::buildCurrent()
{
	auto render = std::make_unique<RenderItem>();
	mRenderItem = render.get();
	mRenderItem->World = getTransform();
	XMStoreFloat4x4(&mRenderItem->TexTransform, XMMatrixScaling(50.0f, 50.0f, 1.0f));
	mRenderItem->ObjCBIndex = (UINT)game->getRenderItems().size();
	mRenderItem->Mat = game->getMaterials()[mMaterialName].get();
	mRenderItem->Geo = game->getGeometries()[mGeometriesName].get();
	mRenderItem->PrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	mRenderItem->IndexCount = mRenderItem->Geo->DrawArgs[mDrawArgsName].IndexCount;
	mRenderItem->StartIndexLocation = mRenderItem->Geo->DrawArgs[mDrawArgsName].StartIndexLocation;
	mRenderItem->BaseVertexLocation = mRenderItem->Geo->DrawArgs[mDrawArgsName].BaseVertexLocation;
	mSpriteNodeRitem = render.get();
	game->getRenderItems().push_back(std::move(render));
}
