mesh.md

## 目的
頂点データから描画に必要なVertexBufferを生成・所有し、
Rendererが描画に使用するVertexBufferViewを提供するクラス

## 処理のフロー

① 頂点配列を受け取る

② 必要なバッファサイズを計算

③ VertexBuffer用のResourceを生成
GPUリソースを、どんな種類のメモリ領域に置くかを決める
CPUから書き込みやすいメモリ領域に置くという指定

↓

GPUリソースの詳細な情報を定義する

↓

UPLOAD Heapに作成したリソースをGPU側から読み取るため、
そのGPUリソースを今どんな用途で使う状態なのかを指定する
④ Map（開始）
GPUリソースのメモリをCPUから触れるようにして、その先頭アドレスを書き込む関数

⑤ verticesをコピー(書き込み)
そのアドレス先に頂点データを書き込む。

⑥ Unmap(終了)
CPUからそのGPUリソースのメモリを直接触るのを終了する

⑦ VertexBufferView
作ったGPUリソースを、実際にGPUが「頂点データとしてどう読むか」を説明するための設定

## APIの関係
### 構造体・列挙型
`D3D12_HEAP_PROPERTIES`
このGPUリソースを、どんな種類のメモリ領域に置くかを決める構造体です。
```cpp
typedef struct D3D12_HEAP_PROPERTIES
{
    D3D12_HEAP_TYPE Type;                     // ヒープの種類（DEFAULT / UPLOAD / READBACK など）
    D3D12_CPU_PAGE_PROPERTY CPUPageProperty;   // CPUから見たメモリの性質
    D3D12_MEMORY_POOL MemoryPoolPreference;    // どのメモリプールを優先するか
    UINT CreationNodeMask;                     // どのGPUノードで作成するか
    UINT VisibleNodeMask;                      // どのGPUノードから見えるようにするか
} D3D12_HEAP_PROPERTIES;
```
`D3D12_HEAP_TYPE_UPLOAD`
CPUから書き込みやすいメモリ領域に置くという指定

`D3D12_RESOURCE_DESC`
GPUリソースの詳細な情報を定義する構造体
```cpp 
typedef struct D3D12_RESOURCE_DESC
{
    D3D12_RESOURCE_DIMENSION Dimension; // リソースの種類（Buffer、Texture1D/2D/3Dなど）
    UINT64 Alignment;                   // メモリ配置時のアラインメント
    UINT64 Width;                       // 幅。Bufferならバイト単位のサイズ
    UINT Height;                        // 高さ。Bufferなら1
    UINT16 DepthOrArraySize;            // 奥行き、または配列数
    UINT16 MipLevels;                   // ミップマップの段階数
    DXGI_FORMAT Format;                 // データのフォーマット
    DXGI_SAMPLE_DESC SampleDesc;        // マルチサンプリングの設定
    D3D12_TEXTURE_LAYOUT Layout;        // メモリ上でのデータ配置方法
    D3D12_RESOURCE_FLAGS Flags;         // リソースに許可する追加用途
} D3D12_RESOURCE_DESC;
```
`D3D12_RESOURCE_STATES`
GPUリソースが現在どの用途で使用される状態なのかを表す列挙型。

`D3D12_RESOURCE_STATE_GENERIC_READ`
GPUから読み取り用途で使用できる状態を表す値。
UPLOAD Heapではこの状態を指定する。
 ```cpp
D3D12_RESOURCE_STATES initialState =
    D3D12_RESOURCE_STATE_GENERIC_READ;
```

### 関数
`CreateCommittedResource`
デバイスを使用して、GPUで使うリソース用のメモリを確保し、そのメモリとリソースをセットで作る関数
```cpp
HRESULT CreateCommittedResource(
    const D3D12_HEAP_PROPERTIES* pHeapProperties, // どんな種類のメモリ領域に置くか
    D3D12_HEAP_FLAGS HeapFlags,                   // ヒープに対する追加設定
    const D3D12_RESOURCE_DESC* pDesc,             // 作るリソースの種類・サイズなど
    D3D12_RESOURCE_STATES InitialResourceState,   // 作成直後のリソース状態
    const D3D12_CLEAR_VALUE* pOptimizedClearValue,// Clear時の最適化情報
    REFIID riidResource,                          // 作成してほしいインターフェイスの種類
    void** ppvResource                            // 作成したリソースのアドレスを書き込む先
);
```

`Map`
GPUリソースのメモリをCPUから触れるようにして、その先頭アドレスを書き込む関数

## 所有関係

### Mesh 
- `ComPtr<ID3D12Resource>` vertexBuffer_
  - GPUリソース本体を所有し、描画中に破棄されないよう寿命を管理する

- `D3D12_VERTEX_BUFFER_VIEW` vertexBufferView_
  - GPUがVertexBufferをどう読み取るかという情報を保持する
  - GPUリソース自体は所有しない
