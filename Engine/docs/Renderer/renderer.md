renderer.md
## 目的
## 処理のフロー
### 初期化フロー
①シェーダーのコンパイル

②RootSignatureの作成
- RootSignature全体の構成を定義する
- 変換後のバイナリデータを書き込む先、
エラー内容を書き込む先を確保する

↓

D3D12SerializeRootSignatureで定義したRootSignatureの設定を、
CreateRootSignature()に渡せるバイナリデータへ変換する


↓

- 確保したバイナリデータをCreateRootSignature()に渡す
- Rendererが保持するRootSignatureに作ったRootSignatureのアドレスを設定する

③PipelineStateの作成


### 描画フロー
シーンが保持
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
    ID3D12Device* pDevice,                           // D3D12のデバイス
    const D3D12_ROOT_SIGNATURE_DESC* pRootSignature, // RootSignatureの設定
    REFIID riid,                                     // 取得するインターフェイスのID
    void** ppvObj                                    // 取得したインターフェイスのポインタ
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
#### インターフェイス

#### 構造体・列挙型
`D3D12_GRAPHICS_PIPELINE_STATE_DESC`
`D3D12_INPUT_ELEMENT_DESC`
`D3D12_BLEND_DESC`
`D3D12_RENDER_TARGET_BLEND_DESC`
`D3D12_DEPTH_STENCIL_DESC`
#### 関数
`CreateGraphicsPipelineState`
## 所有・参照関係
### Graphics
- Device
- CommandList
### Renderer
- Device
- CommandList