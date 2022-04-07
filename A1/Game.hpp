#include "World.hpp"
#include "Player.h"
#include "FrameResource.h"
#include "StateStack.hpp"

#include "Common/d3dApp.h"
#include "Common/MathHelper.h"
#include "Common/UploadBuffer.h"
#include "Common/GeometryGenerator.h"

using Microsoft::WRL::ComPtr;
using namespace DirectX;
using namespace DirectX::PackedVector;

#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "D3D12.lib")

class Game : public D3DApp
{
public:
	Game(HINSTANCE hInstance);
	Game(const Game& rhs) = delete;
	Game& operator=(const Game& rhs) = delete;
	~Game();

	virtual bool Initialize()override;

	void BuildFrameResorces(int renderItemCount);
	void ResetFrameResources();

	std::unordered_map<std::string, std::unique_ptr<Material>>& GetMaterials() { return mMaterials; }
	std::unordered_map<std::string, std::unique_ptr<MeshGeometry>>& GetGeometries() { return mGeometries; }
	FrameResource* GetCurrFrameResource() { return mCurrFrameResource; }

	Microsoft::WRL::ComPtr<ID3D12Device>& GetDevice() { return md3dDevice; }
	Microsoft::WRL::ComPtr<ID3D12Fence>& GetFence() { return mFence; }
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& GetCommandList() { return mCommandList; }

	ID3D12GraphicsCommandList* GetCmdList() { return mCommandList.Get(); }
	ID3D12DescriptorHeap* GetSrvSrvDescriptionHeap() { return mSrvDescriptorHeap.Get(); }
	UINT GetCbvSrvDescriptorSize() { return mCbvSrvDescriptorSize; }

private:
	virtual void OnResize()override;
	//void ProcessInput();
	virtual void Update(const GameTimer& gt)override;
	virtual void Draw(const GameTimer& gt)override;

	virtual void OnMouseDown(WPARAM btnState, int x, int y)override;
	virtual void OnMouseUp(WPARAM btnState, int x, int y)override;
	virtual void OnMouseMove(WPARAM btnState, int x, int y)override;
	virtual void OnKeyDown(WPARAM btnState);
		 
	//void OnKeyboardInput(const GameTimer& gt);
	void HandleRealTimeInput();
	void UpdateCamera(const GameTimer& gt);
	void AnimateMaterials(const GameTimer& gt);
	//void UpdateObjectCBs(const GameTimer& gt);
	void UpdateMaterialCBs(const GameTimer& gt);
	void UpdateMainPassCB(const GameTimer& gt);

	void LoadTextures();
	void CreateTextures(std::string name, std::wstring path);
	void BuildRootSignature();
	void BuildDescriptorHeaps();

	void BuildShadersAndInputLayout();
	void BuildShapeGeometry();
	void BuildPSOs();

	//void BuildFrameResources();
	void BuildMaterials();
	void CreateMaterials(std::string name, XMFLOAT4 diffuseAlbedo, XMFLOAT3 fresnelR0, float roughness);
	//void BuildRenderItems();
	//void DrawRenderItems(ID3D12GraphicsCommandList* cmdList, const std::vector<RenderItem*>& ritems);

	std::array<const CD3DX12_STATIC_SAMPLER_DESC, 6> GetStaticSamplers();

	void RegisterStates();

public:

	POINT mLastMousePos;
	Player mPlayer;
	StateStack mStateStack;

	std::unordered_map<std::string, std::unique_ptr<Texture>> mTextures;		//Textures
	std::vector<std::string> mTexturesName;										//Textures Name
	std::unordered_map<std::string, std::unique_ptr<MeshGeometry>> mGeometries; //Geometries
	std::unordered_map<std::string, std::unique_ptr<Material>> mMaterials;		//Materials

	int mMatCBIdx = 0;
	int mDiffuseSrvHeapIdx = 0;

	std::vector<std::unique_ptr<FrameResource>> mFrameResources;				//Frame Resources
	FrameResource* mCurrFrameResource = nullptr;
	int mCurrFrameResourceIndex = 0;

	ComPtr<ID3D12RootSignature> mRootSignature = nullptr;

	ComPtr<ID3D12DescriptorHeap> mSrvDescriptorHeap = nullptr;
	UINT mCbvSrvDescriptorSize = 0;

	std::unordered_map<std::string, ComPtr<ID3DBlob>> mShaders;					//Shaders

	std::vector<D3D12_INPUT_ELEMENT_DESC> mInputLayout;							//Input Layout

	ComPtr<ID3D12PipelineState> mOpaquePSO = nullptr;
	ComPtr<ID3D12PipelineState> mAlphaTestPSO = nullptr;

	PassConstants mMainPassCB;

	//Floats
	XMFLOAT3 mEyePos = { 0.0f, 0.0f, -10.0f };
	XMFLOAT4X4 mView = MathHelper::Identity4x4();
	XMFLOAT4X4 mProj = MathHelper::Identity4x4();

	float mTheta = 1.3f * XM_PI;
	float mPhi = 0.4f * XM_PI;
	float mRadius = 2.5f;
};
