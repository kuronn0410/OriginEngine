#pragma once
#include <d3d12.h>
#include <wrl/client.h>

struct Vertex
{
    float x;
    float y;
    float z;
};

class ShaderCompiler;

class Renderer
{
public:
	bool Initialize(ShaderCompiler& shaderCompiler);
	void BeginFrame(ID3D12GraphicsCommandList* commandList);
	void Draw();
	
private:
	Vertex vertices_[3] = {
		{ 0.0f, 0.5f, 0.0f },   // 上の頂点
		{ 0.5f, -0.5f, 0.0f },  // 右下の頂点
		{ -0.5f, -0.5f, 0.0f }  // 左下の頂点
	};// 三角形の頂点座標
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState_;
	ID3D12GraphicsCommandList* commandList_ = nullptr;
};