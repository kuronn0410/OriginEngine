## 目的
Graphicsそれぞれの工程と繋がり、そこで使用されるAPIの概要を説明する。
## 全体の処理フロー　
1.　CreateFactory
`CreateDXGIFactory2`でDXGI　Factoryを生成して、Graphicsクラスが所有するFactoryのアドレスに格納する。

2.　CreateAdapter
`EnumAdapterByGpuPreference`でGraphicsクラスが所有するFactoryを使って、使用可能なGPUを探索し、使用可能なGPUを取得して、
スコープ内のローカル変数に格納する。

↓

格納したAdapterがDirectX12を使用可能か`D3D12CreateDevice`を使って確認し、
使用可能な場合は、Graphicsクラスが所有するAdapterのアドレスに格納する。

3.　CreateDevice
Graphicsクラスが所有するAdapterを使って、`D3D12CreateDevice`でDirectX12の機能を利用するためのDeviceを生成して、
Graphicsクラスが所有するDeviceのアドレスに格納する。

4.　CreateCommandQueue
`D3D12_COMMAND_QUEUE_DESC`でCommandQueueの形式を決める

↓

Graphicsクラスが所有するDeviceを使って`CreateCommandQueue`でCommandQueueを生成して、
Graphicsクラスが所有するCommandQueueのアドレスに格納する。

5.　CreateSwapChain
`DXGI_SWAP_CHAIN_DESC1`でSwapChainの形式を決める

↓

Graphicsクラスが所有するFactoryを使って`CreateSwapChainForHwnd`でSwapChainを生成して、
ローカル変数のIDXGISwapChain1にSwapChainのアドレスに格納する。

↓

 IDXGISwapChain1 → IDXGISwapChain4 に変換して、
 Graphicsクラスが所有するSwapChainに格納する。

6.　CreateRTVHeap
`D3D12_DESCRIPTOR_HEAP_DESC`でRTVHeapの形式を決める

↓

その形式を使ってGraphicsクラスが所有するDeviceを使って`CreateDescriptorHeap`でRTVHeapを生成して、
Graphicsクラスが所有するRTVHeapのアドレスに格納する。

7.　CreateRenderTargets
`D3D12_CPU_DESCRIPTOR_HANDLE`で、RTVHeapからCPUが使用するDescriptorのハンドルを取得する。
`UINT`でDeviceからDescriptorのサイズを取得する。

↓

BufferCount分ループして、SwapChainからBackBufferを取得し、
`D3D12_RENDER_TARGET_VIEW_DESC`でレンダーターゲットビューの形式を決める。
それぞれのループでGraphicsクラスが所有するDeviceを使って`CreateRenderTargetView`でRTVを生成して、
Graphicsクラスが所有するRTVHeapの各スロットに格納する。

8.　CreateCommandAllocator
Graphicsクラスが所有するDeviceを使って`CreateCommandAllocator`でCommandAllocatorを生成して、
Graphicsクラスが所有するCommandAllocatorのアドレスに格納する。

9.　CreateCommandList
Graphicsクラスが所有するDeviceを使って`CreateCommandList`でCommandListを生成して、
Graphicsクラスが所有するCommandListのアドレスに格納する。

↓

初期状態ではOpenなので、必要ならCloseする

10.　CreateFence
Graphicsクラスが所有するDeviceを使って`CreateFence`でFenceを生成して、
Graphicsクラスが所有するFenceのアドレスに格納する。


## 各工程の概要
1.　CreateFactory
* `Factory` DXGIの機能を使うための起点
* Graphicsが所有しているFactoryを生成する。

2.　CreateAdapter
* `Adapter` 使用可能なGPUを表すオブジェクト
* 使用できるGPUを探索し、使用可能なGPUを取得する。

3.　CreateDevice
* `Device` 選んだAdapterを使ってDirect3D_12の機能を利用するためのインターフェース
* GPUを使って、DirectX12の機能を使えるようにする。

4.　CreateCommandQueue
* `CommandQueue` D3D12_COMMAND_QUEUE_DESCで、生成するCommandQueueの種類や設定を決める。
* Queueの形式を決め、deviceを使ってCommandQueueを生成する。

5.　CreateSwapChain
* `SwapChain` 描画する画像（BackBuffer）と、画面に表示する画像を切り替える仕組み
* SwapChainの形式を決め、factoryを使ってSwapChainを生成する。

6.　CreateRTVHeap
* `RTVHeap`　RTV(Render Target View「このBackBufferを描画先として使う」という指定)を置くための場所を作れるようにする。
* RTVHeapの形式を決め、deviceを使ってRTVHeapを生成する。

7.　CreateRenderTargets
* `RenderTargets`　SwapChain が持っている各BackBufferを取得して、RTVHeapの各スロットにRTVを作る。
* SwapChainからBackBufferを取得し、RTVHeapの各スロットにRTVを作成する。

8.　CreateCommandAllocator
* `CommandAllocator` CommandListがGPU命令を記録するために使うメモリを管理する。
* deviceを使ってCommandAllocatorを生成する。

9.　CreateCommandList
* `CommandList` GPUに送る命令を記録するリスト。
* CommandAllocatorを使用して、deviceからCommandListを生成する。

10.　CreateFence
* `Fence` CPUとGPUの処理を同期し、GPUの処理が完了したことを確認するための仕組み。
* deviceを使ってFenceを生成する。


## APIとの関係
1.　CreateFactory
* `ComPtr<IDXGIFactory6> factory_` 
Graphicsクラスが所有するFactory
* `CreateDXGIFactory2`
  * 生成したFactoryをComPtr<IDXGIFactory6>の所有しているIDXGIFactory6のポインタに書き込む。
  * 引数(フラグ,ComPtr<IDXGIFactory6>の所有しているIDXGIFactory6のポインタのアドレス)

2.　CreateAdapter
* `ComPtr<IDXGIAdapter1> adapter_`
 *Graphicsクラスが所有するAdapter
* `ComPtr<IDXGIAdapter1> adapter`
 * スコープ内のローカル変数。使用可能なAdapterを取得するために使用する。
* `EnumAdapterByGpuPreference`
 * 指定されたGPUの優先度に基づいてAdapterを列挙する。
 * 引数(インデックス, GPUの優先度, ComPtr<IDXGIAdapter1>の所有しているIDXGIAdapter1のポインタのアドレス)
* `DXGI_ADAPTER_DESC1`
 * Adapterの詳細情報を格納する構造体。
 *  メンバ(Description, VendorId, DeviceId, SubSysId, Revision)
* `D3D12CreateDevice`
 * 指定されたAdapterを使ってDirect3D12のデバイスを作成する。
	* ここでは作成可能かどうかの確認のために使用する。 
 * 引数(Adapter, D3D_FEATURE_LEVEL, IID_PPV_ARGS)

3.　CreateDevice
* `ComPtr<ID3D12Device> device_`
 * Graphicsクラスが所有するDevice
* `D3D12CreateDevice`
 * 指定されたAdapterを使ってDirect3D12のデバイスを作成する。
 * 引数(Adapter, D3D_FEATURE_LEVEL, IID_PPV_ARGS)

4.　CreateCommandQueue
* `ComPtr<ID3D12CommandQueue> commandQueue_`
 * Graphicsクラスが所有するCommandQueue
*  `D3D12_COMMAND_QUEUE_DESC`
 * CommandQueueの設定を指定する構造体。
 * メンバ(型, 優先度, フラグ,マスク)
* `CreateCommandQueue`
 * 指定されたDeviceを使ってCommandQueueを作成する。
 * 引数(CommandQueueの設定,生成するInterfaceのIID(生成したCommandQueueを書き込むアドレス))

5.　CreateSwapChain
* `ComPtr<IDXGISwapChain4> swapChain_`
 * Graphicsクラスが所有するSwapChain
* `ComPtr<IDXGISwapChain1> swapChain`
 * CreateSwapChainForHwndで生成したSwapChainを一時的に保持するローカル変数。
 * その後IDXGISwapChain4に変換してswapChain_に格納する。
* `DXGI_SWAP_CHAIN_DESC1`
 * SwapChainの設定を指定する構造体。
 * メンバ(幅, 高さ, フォーマット, バッファ数, スワップ効果, サンプル数, スケーリング, フラグ)
* `CreateSwapChainForHwnd`
 * factoryと指定されたCommandQueueを使ってSwapChainを作成する。
 * 引数(使用するCommandQueue,HWND,SwapChainの設定,Fullscreen設定,出力先制限,生成したSwapChainを書き込むアドレス)

6.　CreateRTVHeap
* `ComPtr<ID3D12DescriptorHeap> rtvHeap_`
 * Graphicsクラスが所有するRTVHeap
* `D3D12_DESCRIPTOR_HEAP_DESC`
 * RTVHeapの設定を指定する構造体。
 * メンバ(タイプ, サイズ, フラグ, マスク)
* `CreateDescriptorHeap`
 * 指定されたDeviceを使ってRTVHeapを作成する。
 * 引数(D3D12_DESCRIPTOR_HEAP_DESC, IID_PPV_ARGS)

7.　CreateRenderTargets
* `ComPtr<ID3D12Resource> renderTargets_`
 * Graphicsクラスが所有するRenderTargets
* `D3D12_CPU_DESCRIPTOR_HANDLE`
 * CPUが使用するDescriptorのハンドル
* `UINT`
 * ディスクリプタのサイズ
* `D3D12_RENDER_TARGET_VIEW_DESC`
 * レンダーターゲットビューの設定を指定する構造体。
 * メンバ(フォーマット, ディメンション)
* `GetBuffer`
  * SwapChainが所有しているBackBufferを取得する。
  * 取得したBackBufferをGraphicsクラスのrenderTargets_[i]に格納する。
* `CreateRenderTargetView`
  * BackBufferを描画先として扱うためのRTVを作成する。
  * RTVそのものはRTVHeapの指定したスロットに書き込まれる。
  * 引数(ID3D12Resource*, D3D12_RENDER_TARGET_VIEW_DESC*, D3D12_CPU_DESCRIPTOR_HANDLE)

8.　CreateCommandAllocator
* `ComPtr<ID3D12CommandAllocator> commandAllocator_`
 * Graphicsクラスが所有するCommandAllocator
* `CommandAllocator`
 * 指定されたDeviceを使ってCommandAllocatorを作成する。
 * 引数( D3D12_COMMAND_LIST_TYPE, IID_PPV_ARGS(ComPtr<ID3D12CommandAllocator>の所有するID3D12CommandAllocatorのポインタのアドレス))

9.　CreateCommandList
* `ComPtr<ID3D12GraphicsCommandList> commandList_`
 * Graphicsクラスが所有するCommandList
* `CreateCommandList`
 * 指定されたDeviceを使ってCommandListを作成する。
 * 引数(NodeMask, CommandListType, CommandAllocator, InitialPipelineState, IID_PPV_ARGS)

10.　CreateFence
* `ComPtr<ID3D12Fence> fence_`
 * Graphicsクラスが所有するFence
* `CreateFence`
 * 指定されたDeviceを使ってFenceを作成する。
 * 引数( UINT64, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(ComPtr<ID3D12Fence>の所有するID3D12Fenceのポインタのアドレス))

## 所有関係
Graphicsクラス
ComPtr<IDXGIFactory6> factory_;
ComPtr<IDXGIAdapter1> adapter_;
ComPtr<ID3D12Device> device_;
ComPtr<ID3D12CommandQueue> commandQueue_;
ComPtr<IDXGISwapChain4> swapChain_;
ComPtr<ID3D12DescriptorHeap> rtvHeap_;
static constexpr UINT kFrameCount = 2;
ComPtr<ID3D12Resource> renderTargets_[kFrameCount];
ComPtr<ID3D12CommandAllocator> commandAllocator_;
ComPtr<ID3D12GraphicsCommandList> commandList_;
ComPtr<ID3D12Fence> fence_;
UINT64 fenceValue_ = 0;

