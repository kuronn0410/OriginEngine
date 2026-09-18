# Scene設計メモ

## 目的

ゲームごとのSceneを、Engine側の共通Scene管理機能から扱えるようにする。

最初の目標は、Mainから直接Graphicsを呼ばず、

```text
Main
↓
SceneManager
↓
Scene
↓
Graphics
```

という流れで背景色を描画できるようにすること。

その後、Sceneを2つ作成して切り替えまで実装する。

---

## 現在の設計予定

```text
OriginEngine
├─ Engine
│  ├─ Window
│  ├─ Graphics
│  ├─ Input
│  └─ Scene
│      ├─ Scene.h
│      ├─ SceneManager.h
│      └─ SceneManager.cpp
│
└─ Game
   └─ Scene
      ├─ SampleScene.h
      └─ SampleScene.cpp
```

将来的にはGame側に、

```text
TitleScene
GameScene
BattleScene
```

などを追加する。

---

## Scene

### 配置

Engine側。

### 役割

Sceneが共通して持つ処理を定義する。

具体的なゲーム内容は持たない。

```cpp
class Scene
{
public:
    virtual ~Scene() = default;

    virtual void Update() = 0;
    virtual void Render(Graphics& graphics) = 0;
};
```

現状はインターフェイスとして使用するだけなので、`Scene.cpp` は作らない。

---

## SceneManager

### 配置

Engine側。

### 役割

現在使用しているSceneを管理する。

主な責務は、

* 現在のSceneを所有する
* SceneのUpdateを呼ぶ
* SceneのRenderを呼ぶ
* Scene変更要求を受け取る
* 安全なタイミングでSceneを切り替える

### 所有関係

```cpp
std::unique_ptr<Scene> currentScene_;
```

を持つ予定。

SceneManagerがSceneを所有する。

`unique_ptr` を使う理由は、

* Sceneの所有者をSceneManagerだけにしたい
* Scene切り替え時に古いSceneを自動で破棄したい
* 所有関係を明確にしたい

ため。

---

## Game側のScene

### SampleScene

最初の動作確認用Scene。

Engine側には置かない。

理由は、

```text
Sceneを扱う仕組み
→ Engine

具体的に何をするSceneか
→ Game
```

と分けたいため。

最初は背景色を指定するだけでよい。

例：

```cpp
class SampleScene : public Scene
{
public:
    void Update() override;
    void Render(Graphics& graphics) override;
};
```

Renderでは、

```cpp
void SampleScene::Render(Graphics& graphics)
{
    Color clearColor
    {
        0.2f,
        0.4f,
        0.8f,
        1.0f
    };

    graphics.Render(clearColor);
}
```

のようにGraphicsへ描画を依頼する。

---

## Graphics

GraphicsはSceneManagerやSceneが所有しない。

Main、または将来的なApplication / Engineクラスが所有する予定。

初期化もMain側で行う。

```text
Window初期化
↓
HWND取得
↓
Graphics初期化
↓
SceneManager初期化
↓
初期Scene設定
↓
メインループ開始
```

Scene側には必要なときだけ、

```cpp
Graphics&
```

として貸す。

---

## Main側の予定

現在Mainから直接、

```cpp
graphics.Render(...);
```

している処理をなくす。

最終的には、

```cpp
sceneManager.Update();
sceneManager.Render(graphics);
```

という形にする。

---

## 描画の流れ

```text
Main
↓
SceneManager::Render(Graphics&)
↓
currentScene_->Render(Graphics&)
↓
SampleScene::Render(Graphics&)
↓
Graphics::Render(Color)
↓
ClearRenderTargetView
↓
画面表示
```

---

## 初期Scene

Engine側では、

```text
最初にどのSceneを使うか
```

を決めない。

これはGame側の責務。

例えば、

```cpp
sceneManager.ChangeScene(
    std::make_unique<SampleScene>()
);
```

のようにGame側から初期Sceneを渡す。

SceneManagerの内部で、

```cpp
std::make_unique<SampleScene>()
```

とはしない。

理由はEngine側からGame側へ依存させないため。

```text
Game
↓
Engine
```

という依存関係にする。

```text
Engine
↓
Game
```

にはしない。

---

## Scene変更

Game側のSceneが、

```text
どのSceneへ移動するか
```

を判断する。

Engine側のSceneManagerが実際の切り替えを担当する。

予定している名前：

```cpp
RequestSceneChange()
```

`ChangeScene()` ではなくRequestにする理由は、

SceneのUpdate中に現在のSceneを即座に破棄するのではなく、

```text
Scene
↓
変更要求
↓
SceneManager
↓
安全なタイミングでScene切り替え
```

という形にしたいため。

---

## 所有関係

現状の予定：

```text
Main / Application
├─ owns Window
├─ owns Graphics
└─ owns SceneManager
       └─ owns currentScene_
              └─ std::unique_ptr<Scene>
```

SceneはGraphicsを所有しない。

Render中だけ、

```cpp
Graphics&
```

として参照する。

---

## 明日やること

### 1. Scene.hを作成

```cpp
virtual ~Scene() = default;
virtual void Update() = 0;
virtual void Render(Graphics& graphics) = 0;
```

を定義する。

---

### 2. Game側にSampleSceneを作成

```text
Game/Scene/SampleScene.h
Game/Scene/SampleScene.cpp
```

背景色を1色指定してGraphicsへ渡すだけでよい。

---

### 3. SceneManagerを作成

まずは、

```cpp
std::unique_ptr<Scene> currentScene_;
```

を持たせる。

最初はScene変更機能を完成させなくてもよい。

---

### 4. SceneManagerからUpdate / Renderを呼ぶ

イメージ：

```cpp
void SceneManager::Update()
{
    if (currentScene_)
    {
        currentScene_->Update();
    }
}
```

```cpp
void SceneManager::Render(Graphics& graphics)
{
    if (currentScene_)
    {
        currentScene_->Render(graphics);
    }
}
```

---

### 5. 初期Sceneを設定

Game側からSampleSceneをSceneManagerへ渡す。

---

### 6. Mainの直接描画を削除

現在の、

```cpp
graphics.Render(...);
```

をやめて、

```cpp
sceneManager.Update();
sceneManager.Render(graphics);
```

に変更する。

---

## 最初の完成条件

まずはScene切り替えまで作らなくてよい。

以下の流れで一色表示できれば第一段階完了。

```text
Main
↓
SceneManager
↓
SampleScene
↓
Graphics
↓
一色表示
```

これが動いたら、次に2つ目のSceneを作る。

```text
SampleScene
↓
別Scene
```

で背景色が変われば、Scene切り替えの動作確認とする。

---

## 今後追加する予定

第一段階が動いた後に考える。

* `RequestSceneChange()`
* 2つ目のScene
* InputによるScene変更
* Scene切り替えタイミング
* Scene初期化処理
* Scene終了処理
* Application / EngineクラスへのMain処理移動
* フェードなどのScene遷移演出

現時点では過剰に機能を追加せず、

```text
Main → SceneManager → Scene → Graphics
```

の流れを完成させることを優先する。
