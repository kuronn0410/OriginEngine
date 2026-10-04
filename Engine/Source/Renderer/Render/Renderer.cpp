#include "Include/Renderer/Render/Renderer.h"
#include <d3d12.h>
#include "Include/Renderer/Render/ShaderCompiler.h"
#include <wrl/client.h>
#include <d3dcommon.h>
#include <Windows.h>
#include <dxcapi.h>
#include <dxgiformat.h>
#include <cstdlib>
#include <cstdint>
#include <climits>
#include "Include/Renderer/Mesh/Mesh.h"
#include "Include/Renderer/Object/Object.h"
#include "Include/Renderer/Resource/ResourceManager.h"
#include "Include/Renderer/Render/WorldTransform.h"
//#include <d3dx12.h>

bool Renderer::Initialize(
    ShaderCompiler& shaderCompiler, 
    ID3D12Device& device)
{
	// シェーダーのコンパイル
    auto vertexShader =
        shaderCompiler.CompileFromFile(
            L"Engine/Shader/BasicVS.hlsl",
            L"vs_6_0"
        );

    auto pixelShader =
        shaderCompiler.CompileFromFile(
            L"Engine/Shader/BasicPS.hlsl",
            L"ps_6_0"
        );
    if (!vertexShader || !pixelShader)
    {
		return false;
    }
	// パイプラインステート、ルートシグネチャ、頂点バッファの作成
    if(!CreateRootSignature(device))
    {

		return false;
    }
    
    if (!CreatePipelineState(device, *vertexShader.Get(), *pixelShader.Get()))
    {
		return false;
    }

    if(!CreateWorldConstantBuffer(device))
    {

		return false;
    }

	return true;
}

bool Renderer::CreateRootSignature(ID3D12Device& device)
{
    D3D12_DESCRIPTOR_RANGE range{};
    range.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
    range.NumDescriptors = 1;
    range.BaseShaderRegister = 0; // t0
    range.RegisterSpace = 0;
    range.OffsetInDescriptorsFromTableStart =
        D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
    D3D12_ROOT_DESCRIPTOR_TABLE table{};
    table.NumDescriptorRanges = 1;
    table.pDescriptorRanges = &range;

    D3D12_ROOT_PARAMETER rootParameters[2] = {};
	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[0].Descriptor.ShaderRegister = 0;
    rootParameters[0].Descriptor.RegisterSpace = 0;
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;


    rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParameters[1].DescriptorTable = table;
    rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

    D3D12_STATIC_SAMPLER_DESC samplerDesc{};
    samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;//ぼかし
    samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    samplerDesc.MipLODBias = 0.0f;
    samplerDesc.MaxAnisotropy = 1;
    samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_ALWAYS;
    samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
    samplerDesc.MinLOD = 0.0f;
    samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
    samplerDesc.ShaderRegister = 0; // s0
    samplerDesc.RegisterSpace = 0;
    samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

    D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};
    rootSignatureDesc.NumParameters = 2;
    rootSignatureDesc.pParameters = rootParameters;
	rootSignatureDesc.NumStaticSamplers = 1;//画像のサンプラーを使う場合はここに設定する
    rootSignatureDesc.pStaticSamplers = &samplerDesc;
    rootSignatureDesc.Flags =
        D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

    Microsoft::WRL::ComPtr<ID3DBlob> signatureBlob;
    Microsoft::WRL::ComPtr<ID3DBlob> errorBlob;

    HRESULT hr = D3D12SerializeRootSignature(
        &rootSignatureDesc,
        D3D_ROOT_SIGNATURE_VERSION_1,
        signatureBlob.GetAddressOf(),
        errorBlob.GetAddressOf()
    );

    if (FAILED(hr))
    {
        if (errorBlob)
        {
            OutputDebugStringA(
                static_cast<const char*>(
                    errorBlob->GetBufferPointer()
                    )
            );
        }

        return false;
    }

    hr = device.CreateRootSignature(
        0,
        signatureBlob->GetBufferPointer(),
        signatureBlob->GetBufferSize(),
        IID_PPV_ARGS(rootSignature_.GetAddressOf())
    );

    if (FAILED(hr))
    {
        return false;
    }

    return true;
}

bool Renderer::CreatePipelineState(
    ID3D12Device& device, 
    IDxcBlob& vertexShader, 
    IDxcBlob& pixelShader)
{
    /*---D3D12_GRAPHICS_PIPELINE_STATE_DESCの設定に必要な情報の設定---*/
    D3D12_INPUT_ELEMENT_DESC inputElementDesc[3] = {};
    inputElementDesc[0].SemanticName = "POSITION";
    inputElementDesc[0].SemanticIndex = 0;
    inputElementDesc[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
    inputElementDesc[0].InputSlot = 0;
    inputElementDesc[0].AlignedByteOffset = 0;
    inputElementDesc[0].InputSlotClass =
        D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
    inputElementDesc[0].InstanceDataStepRate = 0;

    inputElementDesc[1].SemanticName = "TEXCOORD";
    inputElementDesc[1].SemanticIndex = 0;
    inputElementDesc[1].Format = DXGI_FORMAT_R32G32_FLOAT;
    inputElementDesc[1].InputSlot = 0;
    inputElementDesc[1].AlignedByteOffset = 12;
    inputElementDesc[1].InputSlotClass =
        D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
    inputElementDesc[1].InstanceDataStepRate = 0;

    inputElementDesc[2].SemanticName = "NORMAL";
    inputElementDesc[2].SemanticIndex = 0;
    inputElementDesc[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
    inputElementDesc[2].InputSlot = 0;
    inputElementDesc[2].AlignedByteOffset = 20;
    inputElementDesc[2].InputSlotClass =
        D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
    inputElementDesc[2].InstanceDataStepRate = 0;

    D3D12_BLEND_DESC blendDesc = {};
    blendDesc.AlphaToCoverageEnable = FALSE;
    blendDesc.IndependentBlendEnable = FALSE;

    blendDesc.RenderTarget[0].BlendEnable = FALSE;
    blendDesc.RenderTarget[0].RenderTargetWriteMask =
        D3D12_COLOR_WRITE_ENABLE_ALL;


    D3D12_RENDER_TARGET_BLEND_DESC& rtBlend =
        blendDesc.RenderTarget[0];

    rtBlend.BlendEnable = FALSE;
    rtBlend.LogicOpEnable = FALSE;

    rtBlend.SrcBlend = D3D12_BLEND_ONE;
    rtBlend.DestBlend = D3D12_BLEND_ZERO;
    rtBlend.BlendOp = D3D12_BLEND_OP_ADD;

    rtBlend.SrcBlendAlpha = D3D12_BLEND_ONE;
    rtBlend.DestBlendAlpha = D3D12_BLEND_ZERO;
    rtBlend.BlendOpAlpha = D3D12_BLEND_OP_ADD;

    rtBlend.LogicOp = D3D12_LOGIC_OP_NOOP;

    rtBlend.RenderTargetWriteMask =
        D3D12_COLOR_WRITE_ENABLE_ALL;

    D3D12_DEPTH_STENCIL_DESC depthStencilDesc = {};

    depthStencilDesc.DepthEnable = TRUE;
    depthStencilDesc.DepthWriteMask =
        D3D12_DEPTH_WRITE_MASK_ALL;

    depthStencilDesc.DepthFunc =
        D3D12_COMPARISON_FUNC_LESS;

    depthStencilDesc.StencilEnable = FALSE;

    /*---D3D12_GRAPHICS_PIPELINE_STATE_DESCの設定---*/
    D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
    // ① RootSignature
    psoDesc.pRootSignature = rootSignature_.Get();
     //② VertexShader
    psoDesc.VS.pShaderBytecode = vertexShader.GetBufferPointer();
    psoDesc.VS.BytecodeLength = vertexShader.GetBufferSize();
     //③ PixelShader
	psoDesc.PS.pShaderBytecode = pixelShader.GetBufferPointer();
	psoDesc.PS.BytecodeLength = pixelShader.GetBufferSize();
     //④ InputLayout
    psoDesc.InputLayout.pInputElementDescs = inputElementDesc;
    psoDesc.InputLayout.NumElements = _countof(inputElementDesc);
    //⑤ Rasterizer
    psoDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
    //psoDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
    psoDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;
    psoDesc.RasterizerState.FrontCounterClockwise = FALSE;
    psoDesc.RasterizerState.DepthClipEnable = TRUE;
    // //⑥ Blend
    psoDesc.BlendState = blendDesc;
    // // ⑦ DepthStencil
    psoDesc.DepthStencilState = depthStencilDesc;
    psoDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
    //// ⑧ PrimitiveTopology
    psoDesc.PrimitiveTopologyType =
        D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    // ⑨ RenderTarget
    psoDesc.NumRenderTargets = 1;
    psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
    // ⑩ Sample
    psoDesc.SampleMask = UINT_MAX;
    psoDesc.SampleDesc.Count = 1;
    psoDesc.SampleDesc.Quality = 0;

    // ⑪ PSO生成
    HRESULT hr = device.CreateGraphicsPipelineState(
        &psoDesc,
        IID_PPV_ARGS(pipelineState_.GetAddressOf())
    );

    if (FAILED(hr))
    {
        return false;
    }

	return true;
}

bool Renderer::CreateWorldConstantBuffer(ID3D12Device& device)
{
    // ① ConstantBufferに必要なサイズを計算
   // ※256byte境界に揃える
    constexpr UINT MaxObjects = 100;

     alignedWorldTransformSize_ =
        (sizeof(WorldTransform) + 255) & ~255;

    UINT bufferSize =
        alignedWorldTransformSize_ * MaxObjects;

   // ② HEAP_PROPERTIESを設定
   // CPUから毎フレーム書き込みたいのでUPLOAD
    D3D12_HEAP_PROPERTIES heapProperties = {};
    heapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;

   // ③ RESOURCE_DESCを設定
   // BUFFERとして作る
    D3D12_RESOURCE_DESC resourceDesc = {};
    resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    resourceDesc.Width = bufferSize;
    resourceDesc.Height = 1;
    resourceDesc.DepthOrArraySize = 1;
    resourceDesc.MipLevels = 1;
    resourceDesc.SampleDesc.Count = 1;
    resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
    D3D12_RESOURCE_STATES initialState = D3D12_RESOURCE_STATE_GENERIC_READ;

   // ④ CreateCommittedResourceでResource生成
    HRESULT hr = device.CreateCommittedResource(
        &heapProperties,
        D3D12_HEAP_FLAG_NONE,
        &resourceDesc,
        initialState,
        nullptr,
        IID_PPV_ARGS(worldConstantBuffer_.GetAddressOf())
    );
    if (FAILED(hr))
    {
        return false;
    }

   // ⑤ MapしてCPUから書き込めるポインターを取得
    void* mappedData = nullptr;
    D3D12_RANGE readRange = { 0, 0 };
    hr = worldConstantBuffer_->Map(
        0,
        &readRange,
        &mappedData
    );

    if (FAILED(hr))
    {
        return false;
    }

	mappedWorldTransform_ = static_cast<WorldTransform*>(mappedData);

	return true;
}

void Renderer::BeginFrame(ID3D12GraphicsCommandList* commandList)
{
	commandList_ = commandList;
	drawCount_ = 0;
}

void Renderer::Draw(const Object& object)
{
    /*----Objectから情報を取得----*/
	const RenderObject& renderObject = object.GetRenderObject();
    if (camera_ == nullptr)
    {
        return;
    }
    //Meshの情報を取得
	const Mesh* mesh = resourceManager_->GetMesh(renderObject.mesh);
    if (mesh == nullptr)
    {
        return;
    }
    D3D12_VERTEX_BUFFER_VIEW view = mesh->GetVertexBufferView();
    UINT vertexCount = mesh->GetVertexCount();
    D3D12_INDEX_BUFFER_VIEW indexView = mesh->GetIndexBufferView();
    UINT indexCount = mesh->GetIndexCount();

	//Textureの情報を取得
	const Texture* texture = resourceManager_->GetTexture(renderObject.texture);
    if (texture == nullptr)
    {
        return;
    }

    ID3D12DescriptorHeap* heaps[] =
    {
        resourceManager_->GetSRVHeap()
    };

    
    
    UINT offset = drawCount_ * alignedWorldTransformSize_;

    WorldTransform* current =
        reinterpret_cast<WorldTransform*>(
            reinterpret_cast<uint8_t*>(mappedWorldTransform_) + offset
            );

    current->world = object.GetWorldMatrix();
	current->view = camera_->GetViewMatrix();
	current->projection = camera_->GetProjectionMatrix();


    D3D12_GPU_VIRTUAL_ADDRESS gpuAddress =
        worldConstantBuffer_->GetGPUVirtualAddress() + offset;

	// コマンドリストの設定
      commandList_->SetDescriptorHeaps(1,heaps);
      commandList_->SetGraphicsRootSignature(rootSignature_.Get());
      commandList_->SetPipelineState(pipelineState_.Get());
	  commandList_->SetGraphicsRootConstantBufferView(0, gpuAddress);
      commandList_->SetGraphicsRootDescriptorTable(1,texture->GetGPUHandle());
      commandList_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
      commandList_->IASetVertexBuffers(0, 1, &view);
      commandList_->IASetIndexBuffer(&indexView);
      //DrawIndexedInstancedで描画
      commandList_->DrawIndexedInstanced(
          indexCount,
          1,
          0,
          0,
          0
      );
      ++drawCount_;
}

