#include "Include/Renderer/Renderer.h"
#include <d3d12.h>
#include "Include/Renderer/ShaderCompiler.h"
#include <wrl/client.h>
#include <d3dcommon.h>
#include <Windows.h>
#include <dxcapi.h>
#include <dxgiformat.h>
#include <cstdlib>
#include <climits>
#include <cstring>
#include "Include/Renderer/Mesh.h"
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

	return true;
}

bool Renderer::CreateRootSignature(ID3D12Device& device)
{
    D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};
    rootSignatureDesc.NumParameters = 0;
    rootSignatureDesc.pParameters = nullptr;
    rootSignatureDesc.NumStaticSamplers = 0;
    rootSignatureDesc.pStaticSamplers = nullptr;
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
    D3D12_INPUT_ELEMENT_DESC inputElementDesc[1] = {};
    inputElementDesc[0].SemanticName = "POSITION";
    inputElementDesc[0].SemanticIndex = 0;
    inputElementDesc[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
    inputElementDesc[0].InputSlot = 0;
    inputElementDesc[0].AlignedByteOffset = 0;
    inputElementDesc[0].InputSlotClass =
        D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
    inputElementDesc[0].InstanceDataStepRate = 0;


    psoDesc.InputLayout.pInputElementDescs = inputElementDesc;
    psoDesc.InputLayout.NumElements = _countof(inputElementDesc);

    //⑤ Rasterizer
    psoDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
    //psoDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;
    psoDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;
    psoDesc.RasterizerState.FrontCounterClockwise = FALSE;
    psoDesc.RasterizerState.DepthClipEnable = TRUE;


    // //⑥ Blend
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
    psoDesc.BlendState = blendDesc;

    // // ⑦ DepthStencil
     D3D12_DEPTH_STENCIL_DESC depthStencilDesc = {};
	depthStencilDesc.DepthEnable = FALSE;// 深度バッファを使った奥行き判定を行う場合はTRUE
    depthStencilDesc.StencilEnable = FALSE;
    psoDesc.DepthStencilState = depthStencilDesc;

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

void Renderer::BeginFrame(ID3D12GraphicsCommandList* commandList)
{
	commandList_ = commandList;
}

void Renderer::Draw(Mesh& mesh)
{
     vertexBufferView_ = mesh.GetVertexBufferView();
  // ① RootSignatureを設定
      commandList_->SetGraphicsRootSignature(rootSignature_.Get());
  // ② PipelineStateを設定
      commandList_->SetPipelineState(pipelineState_.Get());

  // ③ PrimitiveTopologyを設定
      commandList_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

  // ④ VertexBufferViewを設定
      commandList_->IASetVertexBuffers(0, 1, &vertexBufferView_);

  // ⑤ DrawInstancedで描画
	  commandList_->DrawInstanced(3, 1, 0, 0);  
}

