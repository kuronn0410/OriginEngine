#include "Include/Renderer/Renderer.h"
#include <d3d12.h>

#include "Include/Renderer/ShaderCompiler.h"

bool Renderer::Initialize(ShaderCompiler& shaderCompiler)
{
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
	return true;
}

void Renderer::BeginFrame(ID3D12GraphicsCommandList* commandList)
{
	commandList_ = commandList;
}

void Renderer::Draw()
{
	// 描画処理
	// Pipeline設定
	//D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
	//psoDesc.pRootSignature = nullptr;
	// VertexBuffer設定
	// DrawInstanced
}

