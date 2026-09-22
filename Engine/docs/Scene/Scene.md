# シーン管理設計

## 目的

シーンの管理を行うクラス群の設計についてまとめる。

---

## 全体フロー

### 初期化時

```text
main.cpp

Graphics生成
↓
Graphics初期化
↓
SceneManager生成
↓
SceneChangeRequest(sceneManager)生成
↓
GameMain生成
↓
GameMain::Initialize(sceneRequest)
```

↓

```text
GameMain.cpp

GameMain::Initialize(sceneRequest)
↓
GameMainがGameSceneFactoryを生成し、一時的に所有する
```

↓

```text
SceneChangeRequestに
ISceneFactoryの所有権を渡す
```

↓

```text
最初に生成するシーンを
SceneChangeRequestに登録する
```

---

### シーン移動

```text
Sceneが参照しているSceneChangeRequestに
シーン移動の要求を登録する
```

↓

```text
ISceneFactory
（ゲーム固有のSceneFactory）

SceneTypeに応じたシーンを生成する
```

↓

```text
SceneChangeRequestが
生成したシーンの所有権をSceneManagerに渡す
```

↓

```text
SceneManagerが現在のシーンを破棄し、
新しいシーンに切り替える
```

---

## 各クラスの責務

### エンジン側

#### Graphics

グラフィックリソースの管理と描画を行う。

#### SceneManager

現在のシーンの管理と、シーンの切り替えを行う。

#### SceneChangeRequest

シーンの切り替え要求を管理し、SceneManagerにシーンの切り替えを依頼する。

#### ISceneFactory

ゲーム固有のシーンを生成するオブジェクトのインターフェース。

#### Scene

ゲームのシーンを表す抽象クラス。

#### SceneType

ゲームのシーンの種類を表す列挙型。

---

### ゲーム側

#### GameMain

ゲームの開始時に必要な初期化を行う。

#### GameSceneFactory（ISceneFactory）

ゲーム固有のシーンを生成するクラス。

#### TitleScene / TestScene（Scene）

ゲーム固有のシーンを表すクラス。

#### GameSceneType

ゲーム固有のシーンの種類を表す列挙型。

---

## 所有関係

### WindowMain

以下を所有する。

```text
WindowMain
├─ Graphics
├─ SceneManager
├─ SceneChangeRequest
└─ GameMain
```

### GameMain

初期化時に`GameSceneFactory`を生成し、一時的に所有する。

```text
GameMain
└─ GameSceneFactory
```

↓

```text
SceneChangeRequestに
GameSceneFactoryの所有権を渡す
```

---

## ゲーム側に用意するサービス

ゲーム側では、以下を用意する。

```text
GameMain
GameSceneFactory
GameSceneType
```
