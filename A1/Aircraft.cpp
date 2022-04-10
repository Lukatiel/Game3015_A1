#include "Aircraft.hpp"
#include "Game.hpp"

Aircraft::Aircraft(Type type, Game* game) : Entity(game)
, mType(type)
{
	switch (type)
	{
	case (Type::Eagle):
		mSprite = "Eagle";
		break;
	case (Type::Raptor):
		mSprite = "Raptor";
		break;
	default:
		mSprite = "Eagle";
		break;
	}
}

unsigned int Aircraft::getCategory() const
{
	switch (mType)
	{
	case Type::Eagle:
		return Category::PlayerAircraft;

	default:
		return Category::EnemyAircraft;
	}
}

void Aircraft::drawCurrent() const
{
	UINT objCBByteSize = d3dUtil::CalcConstantBufferByteSize(sizeof(ObjectConstants));
	UINT matCBByteSize = d3dUtil::CalcConstantBufferByteSize(sizeof(MaterialConstants));

	auto objectCB = game->mCurrFrameResource->ObjectCB->Resource();
	auto matCB = game->mCurrFrameResource->MaterialCB->Resource();

	if (mAircraftRitem != nullptr)
	{
		game->getCmdList()->IASetVertexBuffers(0, 1, &mAircraftRitem->Geo->VertexBufferView());
		game->getCmdList()->IASetIndexBuffer(&mAircraftRitem->Geo->IndexBufferView());
		game->getCmdList()->IASetPrimitiveTopology(mAircraftRitem->PrimitiveType);

		CD3DX12_GPU_DESCRIPTOR_HANDLE tex(game->mSrvDescriptorHeap->GetGPUDescriptorHandleForHeapStart());
		tex.Offset(mAircraftRitem->Mat->DiffuseSrvHeapIndex, game->mCbvSrvDescriptorSize);

		D3D12_GPU_VIRTUAL_ADDRESS objCBAddress = objectCB->GetGPUVirtualAddress() + (UINT64)mAircraftRitem->ObjCBIndex * objCBByteSize;
		D3D12_GPU_VIRTUAL_ADDRESS matCBAddress = matCB->GetGPUVirtualAddress() + (UINT64)mAircraftRitem->Mat->MatCBIndex * matCBByteSize;

		game->getCmdList()->SetGraphicsRootDescriptorTable(0, tex);
		game->getCmdList()->SetGraphicsRootConstantBufferView(1, objCBAddress);
		game->getCmdList()->SetGraphicsRootConstantBufferView(3, matCBAddress);

		game->getCmdList()->DrawIndexedInstanced(mAircraftRitem->IndexCount, 1, mAircraftRitem->StartIndexLocation, mAircraftRitem->BaseVertexLocation, 0);
	}
}

void Aircraft::buildCurrent()
{
	auto render = std::make_unique<RenderItem>();
	mRenderItem = render.get();
	mRenderItem->World = getTransform();
	mRenderItem->ObjCBIndex = (UINT)game->getRenderItems().size();
	mRenderItem->Mat = game->getMaterials()[mSprite].get();
	mRenderItem->Geo = game->getGeometries()["boxGeo"].get();
	mRenderItem->PrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	mRenderItem->IndexCount = mRenderItem->Geo->DrawArgs["box"].IndexCount;
	mRenderItem->StartIndexLocation = mRenderItem->Geo->DrawArgs["box"].StartIndexLocation;
	mRenderItem->BaseVertexLocation = mRenderItem->Geo->DrawArgs["box"].BaseVertexLocation;
	mAircraftRitem = render.get();
	game->getRenderItems().push_back(std::move(render));
}
