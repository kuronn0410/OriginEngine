#pragma once
#include <dxcapi.h>//C++側でDXCを使うため
#include <wrl/client.h>//Microsoft::WRL::ComPtr
#pragma comment(lib, "dxcompiler.lib")
/*
IDxcUtils
IDxcCompiler3
IDxcBlob
*/
class ShaderCompiler
{
public:
	bool Initialize();
	Microsoft::WRL::ComPtr<IDxcBlob> CompileFromFile(
		const wchar_t* filePath,
		const wchar_t* profile
	);//HLSLをコンパイルして、DXILに変換する
private:
	Microsoft::WRL::ComPtr<IDxcUtils> dxcUtils_;
	Microsoft::WRL::ComPtr<IDxcCompiler3> dxcCompiler_;
	Microsoft::WRL::ComPtr<IDxcBlob> dxcBlob_;	
};