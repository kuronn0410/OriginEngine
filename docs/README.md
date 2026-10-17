# OriginEngine

DirectX 12 / C++ を用いて制作している自作3Dゲームエンジンです。

ゲームエンジン内部の描画処理やリソース管理、シーン管理などを理解することを目的として、
Engine / Game / Editor を分離した構成で開発しています。

## Repository
GitHub: https://github.com/kuronn0410/OriginEngine
（URLを記載）

##
https://www.figma.com/board/3QGOxXxZzfJBgh7vldLhWb/OriginEngine?node-id=0-1&p=f&t=ozvxvqrMGHvogYJc-0
---

## 概要

- 開発言語：C++
- Graphics API：DirectX 12
- 対応OS：Windows
- 開発環境：Visual Studio 2026
- シェーダー：HLSL / DXC

---

## 開発目的

Unityなどのゲームエンジンを利用するだけでなく、
ゲームエンジン内部でどのように描画・リソース管理・シーン管理が行われているかを理解するために開発しています。

特に以下を意識しています。

- DirectX 12の描画パイプラインの理解
- GPUリソース管理の理解
- Engine側とGame側の責務分離
- 拡張しやすい設計
- 将来的なEditor機能の実装

---

## 現在実装している機能

### Graphics

- DirectX 12初期化
  - Device
  - CommandQueue
  - SwapChain
  - DescriptorHeap
  - CommandList
  - Fence

- フレーム描画
  - BeginFrame / EndFrame
  - ResourceBarrier
  - ClearRenderTarget
  - Present

- Shader
  - DXCによるHLSLコンパイル
  - Vertex Shader
  - Pixel Shader

- Pipeline
  - RootSignature
  - PipelineStateObject
  - InputLayout
  - Rasterizer
  - DepthStencil

---

### Mesh

- VertexBuffer
- IndexBuffer
- MeshData
- Meshリソース管理
- OBJモデル読み込み

---

### Transform / Camera

- Position
- Rotation
- Scale
- World Matrix
- View Matrix
- Projection Matrix

---

### Texture

- stb_imageを使用した画像読み込み
- Texture Resource生成
- Upload Buffer
- SRV生成
- DescriptorHeapによるSRV管理
- MeshへのTexture描画

---

### Resource Management

MeshやTextureをGame側から直接GPUリソースとして扱わず、
Handleを使用して管理しています。

```text
Game
 ↓
MeshHandle / TextureHandle
 ↓
ResourceManager
 ↓
Mesh / Texture
 ↓
GPU Resource
```

## アーキテクチャ
OriginEngine
│
├─ Engine
│   ├─ Graphics
│   ├─ Renderer
│   ├─ Resource
│   ├─ Scene
│   └─ Math
│
├─ Game
│   ├─ GameScene
│   ├─ GameObject
│   └─ GameLogic
│
└─ Editor
    └─ （今後実装予定）

Engine側はゲーム固有の処理を持たず、
Game側から利用できる機能を提供することを目標としています。

## 今後挑戦したいこと
DirectX 12を使用していることを活かし、
- Instancingによる大量描画
- Frustum Culling
- LOD
- 描画負荷の可視化
など、描画性能に関する機能も実装していく予定です。
また、Editor側の機能として
- AIを使用した設計サポート機能の実装


## 今後の予定
Phase 1
ゲームエンジンとして最低限必要な機能の実装
- 3D描画　実装済み
- Camera　実装済み
- Texture　実装済み
- Collider　未実装
- Scene　実装済み

Phase 2
描画機能の拡張
- Lighting　
- Material
- Instancing
- Culling
- LOD

Phase 3
Editor,設計,AIを使用した設計サポート機能の実装
- Object配置
- Transform編集
- Scene編集
- Resource確認
- 設計サポート機能の実装


## 使用方法(Game側で作ってもらうもの)

### GameMain
ゲームエンジン
```Text
ObjLoader「OBJファイルを読み込むためのクラス」
GameSceneFactory,ISceneFactory「ゲームのシーンを生成するためのクラス」

```

### GameSceneFactory

```Text
Scene　「GameSceneFactoryがSceneTypeに対応するScene派生クラスを生成する」
GameSceneType「リクエストするゲーム側固有のSceneの種類を宣言」

```
```cpp
例）
std::unique_ptr<Scene> GameSceneFactory::CreateScene(SceneType type)
{
	std::unique_ptr<Scene> scene;
    switch (type)
    {
    case GameSceneType::Test:
		scene = std::make_unique<TestScene>(sceneRequest_, gameResources_);
        break;

    case GameSceneType::Title:
		scene = std::make_unique<TitleScene>(sceneRequest_, gameResources_);
        break;
    }
    return scene;
}

```

### Scene
Scene内で使用可能性なもの
```Text
Object「Rendererに渡す描画するもののデータ」
Camera「描画する視点のデータ」
GameSceneType「リクエストするゲーム側固有のSceneの種類を宣言」

```
#### SceneType
SeneをGameSceneTypeに作成するシーンのTypeを登録する必要があります。
```cpp
例）
namespace GameSceneType
{
    constexpr SceneType Test= 0;
    constexpr SceneType Title = 1;
    constexpr SceneType Game = 2;
    
}

```

#### Object
GameObjectを作成する場合は、GameResourcesに作成するHandleを登録する必要があります。
```h
例）
Object object_;
```

```cpp
例）
    //Objectの描画情報を設定する
	RenderObject renderObject;
    renderObject.mesh = gameResources_->playerMesh;
    renderObject.texture = gameResources_->backgroundTexture;
    renderObject.transform ={
	{ 0.0f, -3.0f, 0.0f },//P
	{ 0.0f, 90.0f, 0.0f },//R
	{ 0.5f, 0.5f, 0.5f }//S
	};

    //設定した描画情報をObjectに設定する
    object_.SetRenderObject(renderObject);

    //描画
    void Render(Renderer& renderer)
    {
       renderer.SetCamera(camera_);

       renderer.Draw(object_);
       renderer.Draw(object1_);
       renderer.Draw(object2_);
    }


```
#### Camera
```h
例）
Camera camera_;
```
```cpp
例）
    //描画に使うCameraの指定
	camera_.SetTransform({
		{ 0.0f, 0.0f, -5.0f },
		{ 0.0f, 0.0f, 0.0f }//縦、横
		});

	camera_.SetProjectionSettings({
		60.0f,//fov
		1280.0f / 720.0f,//aspectRatio
		0.1f,//nearPlane
		100.0f//farPlane
		});


    //描画
    void Render(Renderer& renderer)
    {
      renderer.SetCamera(camera_);

       renderer.Draw(object_);
       renderer.Draw(object1_);
       renderer.Draw(object2_);
    }

```


