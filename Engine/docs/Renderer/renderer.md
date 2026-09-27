renderer.md
## 目的
## 処理のフロー
### 初期化フロー
①シェーダーのコンパイル

②RootSignatureの作成
- RootSignature全体の構成を定義する
- ローカル変数で、変換後のバイナリデータを書き込む先、
エラー内容を書き込む先を確保する

↓

D3D12SerializeRootSignatureで定義したRootSignatureの設定を、
CreateRootSignature()に渡せるバイナリデータへ変換する


↓

- 確保したバイナリデータをCreateRootSignature()に渡す
- Rendererが保持するRootSignatureに作ったRootSignatureのアドレスを設定する

③PipelineStateの作成
- 頂点データの1要素を、シェーダーにどう渡すか定義する
- 描画する色と、すでに画面にある色をどう混ぜるかを定義する
- 1つのRenderTargetに対して、描画する色と既存の色をどう合成するかを定義する
- 奥行き判定（Depth Test）とStencil処理のルールを定義する

↓

グラフィックス描画で使うルール一式をまとめて定義する

↓

D3D12_GRAPHICS_PIPELINE_STATE_DESC にまとめた描画設定から、
実際にGPUで使うPipeline State Object（PSO）を生成して、
Rendererが保持するPipelineStateにPSOのアドレスを設定する


### 描画フロー
シーンが保持するMeshからVertexBufferViewを取得し、
Renderer内のローカル変数として参照する

↓

 GraphicsのCommandListに①から⑤までの描画コマンドを記録する

①RootSignatureの設定

②PipelineStateの設定

③PrimitiveTopologyの設定

④VertexBufferViewの設定

⑤DrawInstancedで描画

## APIの関係
### RootSignatureの作成
#### インターフェイス
`ID3DBlob`
バイナリデータを保持するためのDirect3Dのインターフェイス。
シェーダーのコンパイル結果などを保持するのに使う。
#### 構造体・列挙型
`D3D12_ROOT_SIGNATURE_DESC` 
RootSignature全体の構成を定義する構造体
```cpp
typedef struct D3D12_ROOT_SIGNATURE_DESC
{
    UINT NumParameters;                         // ルートパラメータの数
    const D3D12_ROOT_PARAMETER* pParameters;   // ルートパラメータ配列へのポインター

    UINT NumStaticSamplers;                    // StaticSamplerの数
    const D3D12_STATIC_SAMPLER_DESC* pStaticSamplers;
                                                // StaticSampler配列へのポインター

    D3D12_ROOT_SIGNATURE_FLAGS Flags;          // RootSignature全体の追加設定
} D3D12_ROOT_SIGNATURE_DESC;
```
#### 関数
`D3D12SerializeRootSignature`
D3D12_ROOT_SIGNATURE_DESC で定義したRootSignatureの設定を、
CreateRootSignature() に渡せるバイナリデータへ変換する関数
```cpp
HRESULT D3D12SerializeRootSignature(
    const D3D12_ROOT_SIGNATURE_DESC* pRootSignature, // RootSignatureの設定
    D3D_ROOT_SIGNATURE_VERSION Version,              // RootSignatureのバージョン
    ID3DBlob** ppBlob,                               // 変換後のバイナリデータを書き込む先
    ID3DBlob** ppErrorBlob                           // エラー内容を書き込む先
);
```
`CreateRootSignature`
RootSignatureを作成する関数
```cpp
HRESULT CreateRootSignature(
   UINT nodeMask,                    // 使用するGPUノード
    const void* pBlobWithRootSignature, // Serialize済みRootSignatureデータ
    SIZE_T blobLengthInBytes,         // バイナリデータのサイズ
    REFIID riid,                      // 作成するインターフェイスの種類
    void** ppvRootSignature           // 作成したRootSignatureを書き込む先
);
```
`OutputDebugStringA`
デバッグ出力を行う関数
```cpp
void OutputDebugStringA(
    LPCSTR lpOutputString  // 出力する文字列
);
```
### PipelineStateの作成
#### 構造体・列挙型

`D3D12_INPUT_ELEMENT_DESC`
頂点データの1要素を、シェーダーにどう渡すか定義する構造体
```cpp
typedef struct D3D12_INPUT_ELEMENT_DESC
{
    LPCSTR SemanticName;                 // シェーダー側の名前（POSITION、TEXCOORDなど）
    UINT SemanticIndex;                  // 同じSemanticNameを複数使う場合の番号
    DXGI_FORMAT Format;                  // データ形式（float3、float2など）
    UINT InputSlot;                      // どのVertexBufferから読むか
    UINT AlignedByteOffset;              // Vertex内の何バイト目から読むか
    D3D12_INPUT_CLASSIFICATION InputSlotClass;
                                         // 頂点ごとか、インスタンスごとか
    UINT InstanceDataStepRate;           // インスタンスデータを何回ごとに進めるか
} D3D12_INPUT_ELEMENT_DESC;
```
`D3D12_BLEND_DESC`
描画する色と、すでに画面にある色をどう混ぜるかを定義する構造体
```cpp
typedef struct D3D12_BLEND_DESC
{
    BOOL AlphaToCoverageEnable;                  // AlphaToCoverageを使うか
    BOOL IndependentBlendEnable;                // RenderTargetごとに別のBlend設定を使うか
    D3D12_RENDER_TARGET_BLEND_DESC RenderTarget[8];
                                                 // 各RenderTargetのBlend設定
} D3D12_BLEND_DESC;
```
`D3D12_RENDER_TARGET_BLEND_DESC`
1つのRenderTargetに対して、描画する色と既存の色をどう合成するかを定義する構造体
```cpp
typedef struct D3D12_RENDER_TARGET_BLEND_DESC
{
    BOOL BlendEnable;                // Blendを有効にするか
    BOOL LogicOpEnable;              // 論理演算を有効にするか

    D3D12_BLEND SrcBlend;            // 描画する側の色に掛ける係数
    D3D12_BLEND DestBlend;           // 既にある色に掛ける係数
    D3D12_BLEND_OP BlendOp;          // RGBをどう合成するか

    D3D12_BLEND SrcBlendAlpha;       // 描画する側のAlphaに掛ける係数
    D3D12_BLEND DestBlendAlpha;      // 既存のAlphaに掛ける係数
    D3D12_BLEND_OP BlendOpAlpha;     // Alphaをどう合成するか

    D3D12_LOGIC_OP LogicOp;          // 論理演算の種類
    UINT8 RenderTargetWriteMask;     // RGBAのどの成分を書き込むか
} D3D12_RENDER_TARGET_BLEND_DESC;
```

`D3D12_DEPTH_STENCIL_DESC`
奥行き判定（Depth Test）とStencil処理のルールを定義する構造体
```cpp
typedef struct D3D12_DEPTH_STENCIL_DESC
{
    BOOL DepthEnable;                       // Depth Testを有効にするか
    D3D12_DEPTH_WRITE_MASK DepthWriteMask; // DepthBufferへ書き込むか
    D3D12_COMPARISON_FUNC DepthFunc;        // 奥行きをどう比較するか

    BOOL StencilEnable;                     // Stencil Testを有効にするか
    UINT8 StencilReadMask;                  // Stencil値を読むときのマスク
    UINT8 StencilWriteMask;                 // Stencil値を書くときのマスク

    D3D12_DEPTH_STENCILOP_DESC FrontFace;   // 表面に対するStencil処理
    D3D12_DEPTH_STENCILOP_DESC BackFace;    // 裏面に対するStencil処理
} D3D12_DEPTH_STENCIL_DESC;
```

`D3D12_GRAPHICS_PIPELINE_STATE_DESC`
グラフィックス描画で使うルール一式をまとめて定義する構造体
```cpp
typedef struct D3D12_GRAPHICS_PIPELINE_STATE_DESC
{
    ID3D12RootSignature* pRootSignature;      // 使用するRootSignature

    D3D12_SHADER_BYTECODE VS;                 // Vertex Shader
    D3D12_SHADER_BYTECODE PS;                 // Pixel Shader
    D3D12_SHADER_BYTECODE DS;                 // Domain Shader
    D3D12_SHADER_BYTECODE HS;                 // Hull Shader
    D3D12_SHADER_BYTECODE GS;                 // Geometry Shader

    D3D12_STREAM_OUTPUT_DESC StreamOutput;    // Stream Outputの設定

    D3D12_BLEND_DESC BlendState;              // 色の合成ルール
    UINT SampleMask;                          // サンプリング時の有効ビット

    D3D12_RASTERIZER_DESC RasterizerState;    // ラスタライズのルール
    D3D12_DEPTH_STENCIL_DESC DepthStencilState;
                                                // Depth / Stencilのルール

    D3D12_INPUT_LAYOUT_DESC InputLayout;       // 頂点入力レイアウト

    D3D12_INDEX_BUFFER_STRIP_CUT_VALUE IBStripCutValue;
                                                // Strip描画時のIndex切断値

    D3D12_PRIMITIVE_TOPOLOGY_TYPE PrimitiveTopologyType;
                                                // 三角形・線などの種類

    UINT NumRenderTargets;                     // RenderTargetの数
    DXGI_FORMAT RTVFormats[8];                 // 各RenderTargetのフォーマット

    DXGI_FORMAT DSVFormat;                     // DepthStencilBufferのフォーマット

    DXGI_SAMPLE_DESC SampleDesc;               // MSAAなどのサンプリング設定

    UINT NodeMask;                             // 使用するGPUノード
    D3D12_CACHED_PIPELINE_STATE CachedPSO;     // キャッシュ済みPSO情報

    D3D12_PIPELINE_STATE_FLAGS Flags;          // PSOの追加設定
} D3D12_GRAPHICS_PIPELINE_STATE_DESC;
```
#### 関数
`CreateGraphicsPipelineState`
D3D12_GRAPHICS_PIPELINE_STATE_DESC にまとめた描画設定から、
実際にGPUで使うPipeline State Object（PSO）を生成する関数
```cpp
HRESULT CreateGraphicsPipelineState(
    const D3D12_GRAPHICS_PIPELINE_STATE_DESC* pDesc, // PSOの設定一式
    REFIID riid,                                     // 作成するインターフェイスの種類
    void** ppPipelineState                           // 作成したPSOを書き込む先
);
```



## 所有関係
### Graphics
- `ID3D12Device`
  - GPUリソースやPSOなどを生成するためのDeviceを所有する

- `ID3D12GraphicsCommandList`
  - GPUへ送るコマンドを記録するCommandListを所有する

### Renderer
- `ID3D12RootSignature`
  - 描画で使用するRootSignatureを所有する

- `ID3D12PipelineState`
  - 描画で使用するPSOを所有する

### Mesh
- `ID3D12Resource`
  - VertexBuffer本体を所有する

- `D3D12_VERTEX_BUFFER_VIEW`
  - VertexBufferの読み取り情報を保持する

### 一時的に使用するもの
- `ID3DBlob`
  - RootSignatureのSerialize結果を一時的に保持する
  - RootSignature生成後は不要

- `D3D12_VERTEX_BUFFER_VIEW`への参照
  - Draw中だけMeshから借りる