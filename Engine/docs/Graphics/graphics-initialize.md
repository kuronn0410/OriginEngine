graphics-initialize.md

## 目的
グラフィックスシステムの初期化

## 処理の順番
CreateFactory();
オブジェクトの生成の入り口

↓

CreateAdapter();
使えるGPUを探す
Factoryを使う

↓

CreateDevice();
GPUを使って、DirectX12の機能を使えるようにする
Adapterを使う

↓

CreateCommandQueue(); 
CommandListに記録したGPU命令を、GPUへ送るためのCommandQueueを生成する
Deviceを使う

↓

CreateSwapChain(HWND hwnd); 
描画する画像（BackBuffer）と、画面に表示する画像を切り替える仕組み
factoryとCommandQueueを使う

↓

CreateRTVHeap();
RTV(Render Target View「このBackBufferを描画先として使う」という指定)
を置くための場所を作れるようにする
Deviceを使う

↓

CreateRenderTargets();
SwapChain が持っている各 BackBuffer を取得して、RTV Heap の各スロットに RTV を作る
RTVHeapとSwapChain、Deviceを使う

↓

CreateCommandAllocator();
CommandListがGPU命令を記録するために使うメモリを管理する
Deviceを使う

↓

CreateCommandList();
GPUに実行させる命令を記録するリストを作成する
DeviceとCommandAllocatorを使う

↓

CreateFence();
GPUの処理が完了したことを確認するための仕組み
Deviceを使う
