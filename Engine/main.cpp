#include <Windows.h>
#include <d3d12.h>
#include <wrl.h>

#pragma comment(lib, "d3d12.lib")

int main()
{
    Microsoft::WRL::ComPtr<ID3D12Device> device;

    HRESULT result = D3D12CreateDevice(
        nullptr,
        D3D_FEATURE_LEVEL_11_0,
        IID_PPV_ARGS(&device)
    );

    if (FAILED(result))
    {
        MessageBox(
            nullptr,
            L"DirectX 12 Device の生成に失敗しました",
            L"Error",
            MB_OK
        );

        return -1;
    }

    MessageBox(
        nullptr,
        L"DirectX 12 Device の生成に成功しました",
        L"Success",
        MB_OK
    );

    return 0;
}