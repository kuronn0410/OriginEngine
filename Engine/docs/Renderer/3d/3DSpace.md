## 概要
３ｄ空間に関する処理のフローと、
APIの関係について

## 処理のフロー
main 
Graphics graphics
ShaderCompiler shaderCompiler

シーンの初期化
Objectにデータを設定

### 描画
Renderer::Draw() を呼び出す
↓
Camera から View行列を取得
↓
Camera から Projection行列を取得
↓
Object から Transform を取得
↓
Transform から World行列を計算
↓
World / View / Projection を
ConstantBuffer に書き込む
↓
CommandListに RootSignature を設定
↓
CommandListに
「このConstantBufferをこのRootParameterで使う」
と記録
↓
頂点バッファやIndexBufferを設定
↓
DrawInstanced / DrawIndexedInstanced を記録
↓
CommandList を閉じる
↓
CommandQueue に実行依頼
↓
GPU が CommandList を実行
↓
VertexShader が ConstantBuffer から
World / View / Projection を読み取る
↓
頂点座標を変換して描画

## APIの関係

#### インターフェイス
` ` 
`ID3D12Resource`
ワールド行列を格納するために使用

## 所有・参照関係

## Math
### MakeIdentityMatrix();
単位行列を作成する
### MakeTranslateMatrix();
位置のワールド行列を生成する
### MakeScaleMatrix();
スケールを反映したワールド行列を生成する
### MakeRotateMatrix();
XYZの回転を反映したワールド行列を生成する
### MakeRotateZMatrix();
Z軸回りの回転を反映したワールド行列を生成する
### MakeRotateXMatrix();
X軸回りの回転を反映したワールド行列を生成する
### MakeRotateYMatrix();
Y軸回りの回転を反映したワールド行列を生成する
### MakeInverseMatrix();
逆行列を生成する
### MakePerspectiveFovMatrix();
透視投影行列を生成する
### Matrix4x4 Multiply();
行列の積を計算する

## 関連
### クラス
Camera
### 構造体

ProjectionSettings
CameraTransform
Object
Mesh

Matrix4x4
MatrixMath
WorldTransform