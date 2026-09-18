graphics-system.md

## 目的
DirectX12で描画するための初期環境を作成する。

## 処理の順番

CreateDXGIFactory2

↓

DXGI Factoryを作成

EnumAdapterByGpuPreference

↓

使用するGPU(Adapter)を取得

D3D12CreateDevice

↓

取得したAdapterを使ってDeviceを作成

## クラス・構造体構成


## 所有関係
Graphics
- ComPtr<IDXGIFactory6> factory_
- ComPtr<IDXGIAdapter1> adapter_
- ComPtr<ID3D12Device> device_

ComPtrでCOMオブジェクトの参照カウントを管理する。

## APIとの関係

### CreateDXGIFactory2
DXGIを利用するためのFactoryを生成する。

### EnumAdapterByGpuPreference
指定したGPU優先度に従ってAdapterを列挙する。

### D3D12CreateDevice
Adapterを利用してDirectX12 Deviceを生成する。

### DXGI_ADAPTER_DESC1
GPU（Adapter）の情報をまとめて入れておくための構造体

## ポインター・参照関係

### GetAddressOf()
ComPtr内部のポインター変数のアドレスを取得する。
APIからCOMオブジェクトのポインターを書き込んでもらうときに使用する。

### Get()
ComPtrが保持している生ポインターを取得する。
所有権はComPtr側に残る。


初期化
- Factory
- Adapter
- Device
- CommandQueue
- SwapChain
- RTV
- Fence

毎フレーム
- CommandAllocator Reset
- CommandList Reset
- ResourceBarrier
- Clear
- Execute
- Present
- GPU同期