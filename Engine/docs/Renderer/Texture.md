## 概要
テクスチャの読み込み、管理、使用に関する情報
## 処理のフロー
main 
Graphics初期化（）
Graphicのコマンドリストへの書き込めるようにする

↓

ResourceManagerにGraphicsのコマンドリスト,SRVHeap,Deviceを渡す

↓

GameMain初期化
ResourceManagerにパスを渡す

↓

ResourceManagerがパスからTextureLoaderを使ってテクスチャを読み込む
データをTextureDataに格納する

Textureにデータを渡し
リソース
GPU用のハンドルをSRVHeapに作成する
GPUに渡すデータを格納するバッファーを作成する
コマンドリストに書き込む
ResourceViewを作成してハンドルをSRVHeapに作成する



## APIの関係


#### インターフェイス
` ` 
```cpp
```
#### 構造体・列挙型
` ` 
```cpp
```
#### 関数
` ` 
```cpp
```
## 所有・参照関係

## 関連
### クラス
TextureLoader
ResourceManager
Texture

### 構造体
TextureHandle
TextureData
TextureUploadData

### 外部ライブラリ
stb_image
