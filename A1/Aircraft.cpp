#include "Aircraft.hpp"
#include "Game.hpp"
#include "State.hpp"



Aircraft::Aircraft(State* state)
	: Entity(state)
{
}

Aircraft::~Aircraft()
{

}

//unsigned int Aircraft::getCategory() const
//{
//	switch (mType)
//	{
//	case Type::Eagle:
//		return Category::PlayerAircraft;
//
//	default:
//		return Category::EnemyAircraft;
//	}
//}

void Aircraft::UpdateCurrent(const GameTimer& gt)
{
	Entity::updateCurrent(gt);
}

void Aircraft::drawCurrent() const
{
	UINT objCBByteSize = d3dUtil::CalcConstantBufferByteSize(sizeof(ObjectConstants));
	UINT matCBByteSize = d3dUtil::CalcConstantBufferByteSize(sizeof(MaterialConstants));

	Game* game = mState->GetContext()->game;

	auto objectCB = game->mCurrFrameResource->ObjectCB->Resource();
	auto matCB = game->mCurrFrameResource->MaterialCB->Resource();

	if (mAircraftRitem != nullptr)
	{
		game->GetCmdList()->IASetVertexBuffers(0, 1, &mAircraftRitem->Geo->VertexBufferView());
		game->GetCmdList()->IASetIndexBuffer(&mAircraftRitem->Geo->IndexBufferView());
		game->GetCmdList()->IASetPrimitiveTopology(mAircraftRitem->PrimitiveType);

		CD3DX12_GPU_DESCRIPTOR_HANDLE tex(game->mSrvDescriptorHeap->GetGPUDescriptorHandleForHeapStart());
		tex.Offset(mAircraftRitem->Mat->DiffuseSrvHeapIndex, game->mCbvSrvDescriptorSize);

		D3D12_GPU_VIRTUAL_ADDRESS objCBAddress = objectCB->GetGPUVirtualAddress() + (UINT64)mAircraftRitem->ObjCBIndex * objCBByteSize;
		D3D12_GPU_VIRTUAL_ADDRESS matCBAddress = matCB->GetGPUVirtualAddress() + (UINT64)mAircraftRitem->Mat->MatCBIndex * matCBByteSize;

		game->GetCmdList()->SetGraphicsRootDescriptorTable(0, tex);
		game->GetCmdList()->SetGraphicsRootConstantBufferView(1, objCBAddress);
		game->GetCmdList()->SetGraphicsRootConstantBufferView(3, matCBAddress);

		game->GetCmdList()->DrawIndexedInstanced(mAircraftRitem->IndexCount, 1, mAircraftRitem->StartIndexLocation, mAircraftRitem->BaseVertexLocation, 0);
	}
}

void Aircraft::buildCurrent()
{
	Game* game = mState->GetContext()->game;
	
	auto render = std::make_unique<RenderItem>();
	mRenderItem = render.get();
	//renderer->World = getTransform();
	mRenderItem->ObjCBIndex = mState->GetRenderItems().size();
	mRenderItem->Mat = game->GetMaterials()[mMaterialName].get();
	mRenderItem->Geo = game->GetGeometries()[mGeometriesName].get();
	mRenderItem->PrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	mRenderItem->IndexCount = mRenderItem->Geo->DrawArgs[mDrawArgsName].IndexCount;
	mRenderItem->StartIndexLocation = mRenderItem->Geo->DrawArgs[mDrawArgsName].StartIndexLocation;
	mRenderItem->BaseVertexLocation = mRenderItem->Geo->DrawArgs[mDrawArgsName].BaseVertexLocation;
	mState->GetRenderItems().push_back(std::move(render));
}
