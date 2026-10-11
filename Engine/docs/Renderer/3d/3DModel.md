## 概要
３ｄモデルに関する処理のフローと、
APIの関係について

## 処理のフロー
### 初期化
GameMain

ObjLoaderを使ってOBJファイルからMeshDataを生成
↓
ResourceManagerにMeshDataを登録
↓
ResourceManager内部でMeshを生成
↓
MeshHandle（MeshのID）を返す
↓
ゲーム側でMeshHandleを保持

### シーン初期化
Scene::Initialize()

Sceneが所有するObjectに

・MeshHandle
・Transform
・TextureHandle

を設定
### 描画
Scene
↓
ObjectをRenderer::Draw()へ渡す
↓
ObjectからMeshHandleを取得
↓
ResourceManagerからMeshを取得
↓
Meshから
VertexBufferView
IndexBufferView
Index数
を取得
↓
CommandListに
VertexBuffer / IndexBufferを設定
↓
ObjectのTransformからWorld行列を作成
↓
CameraからView / Projection行列を取得
↓
ConstantBufferに書き込み
↓
TextureHandleからTextureを取得
↓
SRVを設定
↓
DrawIndexedInstanced()

## APIの関係
#### 関数
` ` 
`DrawIndexedInstanced`

```cpp
commandList_->DrawIndexedInstanced(
    UINT IndexCountPerInstance, // 1インスタンスあたりに描画するIndexの数
    UINT InstanceCount,        // 何個のインスタンスを描画するか
    UINT StartIndexLocation,   // IndexBufferの何番目のIndexから使うか
    INT BaseVertexLocation,    // Indexが参照するVertex番号に加算する値
    UINT StartInstanceLocation // インスタンス番号を何番から開始するか
);
```
## 所有・参照関係

## 関連
ObjLoader
ObjVertexIndex
ResourceManager
