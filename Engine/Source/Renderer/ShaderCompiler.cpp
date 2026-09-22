

#include <Windows.h>
#include <dxcapi.h>
#include "Include/Renderer/ShaderCompiler.h"
#include <wrl/client.h>
#include <cstdlib>


bool  ShaderCompiler::Initialize()
{
    dxcUtils_.Reset();
    dxcCompiler_.Reset();

	HRESULT utilsHr = DxcCreateInstance(
		CLSID_DxcUtils,
		IID_PPV_ARGS(dxcUtils_.GetAddressOf())
	);
	if (FAILED(utilsHr))
	{
		return false;
	}
	HRESULT compilerHr = DxcCreateInstance(
		CLSID_DxcCompiler,
		IID_PPV_ARGS(dxcCompiler_.GetAddressOf())
	);
	if (FAILED(compilerHr))
	{
		return false;
	}
	return true;
}

Microsoft::WRL::ComPtr<IDxcBlob> ShaderCompiler::CompileFromFile(
	const wchar_t* filePath,
	const wchar_t* profile
)
{
    if (!dxcUtils_ || !dxcCompiler_)
    {
        // エラー
        return nullptr;
    }

    // HLSLコードをDXILに変換する
    Microsoft::WRL::ComPtr<IDxcBlobEncoding> shaderSource;
    Microsoft::WRL::ComPtr<IDxcResult> result;
    HRESULT loadHr = dxcUtils_->LoadFile(
        filePath,
        nullptr,
        shaderSource.GetAddressOf()
    );

	if(FAILED(loadHr))
	{
		// エラー
		return nullptr;
	}


    LPCWSTR arguments[] =
    {
        L"-E", L"main",
        L"-T", profile
    };
    // DxcBuffer構造体にHLSLコードを設定
    DxcBuffer sourceBuffer = {};
    sourceBuffer.Ptr = shaderSource->GetBufferPointer();
    sourceBuffer.Size = shaderSource->GetBufferSize();
    sourceBuffer.Encoding = DXC_CP_UTF8;

    HRESULT compileHr = dxcCompiler_->Compile(
        &sourceBuffer,
        arguments,
        _countof(arguments),
        nullptr,
        IID_PPV_ARGS(result.GetAddressOf())
    );

	if(FAILED(compileHr))
	{
		// エラー
		return nullptr;
	}

    Microsoft::WRL::ComPtr<IDxcBlobUtf8> errors;

    // エラーを取得
    HRESULT errorHr = result->GetOutput(
        DXC_OUT_ERRORS,
        IID_PPV_ARGS(errors.GetAddressOf()),
        nullptr
    );
    if(FAILED(errorHr))
    {
        return nullptr;
    }


    if(errors && errors->GetStringLength() > 0)
    {
        OutputDebugStringA(errors->GetStringPointer());
    }
    HRESULT shaderStatus;
    HRESULT statusHr = result->GetStatus(&shaderStatus);
    if (FAILED(statusHr))//GetStatus()というAPI呼び出し自体が成功したか
    {
		return nullptr;
    }
	if (FAILED(shaderStatus))//コンパイル自体が成功したか
	{
		return nullptr;
	}

    Microsoft::WRL::ComPtr<IDxcBlob> shaderBlob;

    HRESULT blobHr = result->GetOutput(
        DXC_OUT_OBJECT,
        IID_PPV_ARGS(shaderBlob.GetAddressOf()),
        nullptr
    );
    if (FAILED(blobHr))
    {
        return nullptr;
    }

	return shaderBlob;
}