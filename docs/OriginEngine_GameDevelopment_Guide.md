# OriginEngine ゲーム制作・プロジェクト登録ガイド

作成日：2026年11月4日  
対象：この作業フォルダーにある現在の実装。既存の未コミット変更も含めてコードを確認した資料です。ビルド・画面操作による動作確認は行っていません。

## 1. 現在の構成

OriginEngineは、ゲームをDLLとして作り、Editorから読み込む構成です。

| 部分 | 役割 | 出力・配置 |
| --- | --- | --- |
| OriginEngine | 描画、リソース、シーン管理などの共通機能 | 静的ライブラリ |
| Editor | ウィンドウ、プロジェクト選択、ゲームDLL読み込み、シーン雛形生成 | 実行ファイル |
| TestGame1 | 素材の登録、ゲーム固有のシーン、入力・ゲーム処理 | ゲームDLL |

```text
OriginEngine.slnx
OriginEngine.vcxproj
Engine/
Editor/
  Editor.vcxproj
Games/
  TestGame1/
    TestGame1.vcxproj
    Game/
      GameMain.cpp / .h
      GameExports/
      Include/Scene/
      Source/Scene/
      Generate/Scenes/
Assets/Resource/
x64/Release/
```

Editorにプロジェクト作成ウィザードはありません。Visual StudioでゲームのC++プロジェクトを用意し、Editorでそのフォルダーを開きます。ゲーム単体の実行ファイルを出力する手順は、現在の構成には用意されていません。

## 2. 既存のTestGame1を起動する

1. Visual Studioで `OriginEngine.slnx` を開きます。
2. C++開発環境、Windows SDK、プロジェクト指定のツールセット `v145` を用意します。ゲーム側の言語設定はC++20です。
3. 構成を **Release / x64** にします。現在のEditorはゲームDLLを `x64/Release` から読むため、この構成に揃えます。
4. ソリューションをビルドし、ルートの `x64/Release/TestGame1.dll` があることを確認します。
5. **Editorをスタートアッププロジェクト**にします。ゲームDLLを直接起動する構成ではありません。
6. Editorのデバッグ設定で「作業ディレクトリ」を `$(SolutionDir)` にします。現在の `.vcxproj.user` にもこの設定があります。
7. Editorを起動します。
8. `Open` 欄に、`Games/TestGame1` の**絶対フォルダーパス**を入力します。
9. **Open Game Project** を押し、その後に **Open Game** を押します。

この作業フォルダーでの入力例：

```text
C:\YY\ゲーム制作\DirectX\OriginEngine\OriginEngine\Games\TestGame1
```

### 起動前の素材確認

現在の `GameMain.cpp` は次の素材を読み込みます。

- モデル：`Assets/Resource/Try.obj`、`Cube.obj`、`Try1.obj`
- 画像：`Assets/Resource/Try.png`、`Try1.png`、`ground.png`、`rock.png`、`clear.png`

調査時点では、作業フォルダーの `Assets/Resource` で上記画像は確認できましたが、上記OBJファイルは確認できませんでした。実行前にモデルを用意するか、ゲーム側で実在するモデルへの参照に変更する必要があります。

シェーダーも `Engine/Shader/BasicVS.hlsl`、`BasicPS.hlsl` の相対パスから読み込むため、実行ファイルの場所だけでなく作業ディレクトリも重要です。

## 3. 新しいゲームプロジェクトを登録する

登録は「Visual Studioへの登録」と「Editorへの読み込み」の2段階です。以下は現在のローダーに合わせた手動手順です。

### 3.1 フォルダーとVisual Studioプロジェクトを用意する

例として `MyGame` を作る場合：

```text
Games/MyGame/MyGame.vcxproj
Games/MyGame/Game/...
```

1. Visual StudioでソリューションにC++のDLLプロジェクトを追加し、場所を `Games/MyGame` にします。既存プロジェクトを用意済みなら「追加 → 既存のプロジェクト」で `.vcxproj` を追加します。
2. `TestGame1/Game` の構成を参考に、`GameMain`、`GameExports`、シーン、リソース定義を用意し、ソースをゲームプロジェクトへ追加します。
3. 構成の種類を **ダイナミックライブラリ (.dll)**、プラットフォームをx64、言語標準をC++20にします。
4. インクルードディレクトリに以下を設定します。継承値も残します。

```text
$(SolutionDir)Engine\
$(SolutionDir)Games\MyGame\Game\
%(AdditionalIncludeDirectories)
```

5. 共通機能をリンクするため、`OriginEngine` へのプロジェクト参照を追加します。現在のTestGame1は追加依存ファイルに `d3d12.lib` も指定しています。
6. 出力ディレクトリを `$(SolutionDir)x64\Release\`、ターゲット名を `MyGame` にし、`MyGame.dll` を生成します。ゲームの `.vcxproj` 名とDLL名を一致させます。
7. Editorとゲームを同じRelease / x64構成でビルドします。C++オブジェクトをDLL境界で受け渡すため、ツールセットとランタイムライブラリ設定も揃えます。

既存のTestGame1をコピーして作る場合は、プロジェクト名・ファイル名・GUID・インクルードパス・参照パス・出力名を確認してください。現在のTestGame1には `OriginEngine` への参照が2つあり、`..\OriginEngine\OriginEngine.vcxproj` はこの配置では存在しない参照です。新規プロジェクトにそのまま引き継がず、ルートを指す `..\..\OriginEngine.vcxproj` を使います。

### 3.2 ゲームDLLの入口を用意する

`Game/GameExports/GameExports.cpp` を参考に、次のエクスポートを用意します。

| 名前 | 現在の契約 |
| --- | --- |
| `GetGameVersion` | `int GetGameVersion()`。TestGame1は1を返す |
| `InitializeGame` | `bool InitializeGame(Origin::SceneChangeRequest&, Origin::ResourceManager&)` |
| `ShutdownGame` | `void ShutdownGame()` |

名前で関数を探すため、TestGame1と同じ `extern "C" __declspec(dllexport)` を使います。現在のローダーはバージョン関数を呼びますが、値による互換性判定はしていません。

`InitializeGame` では `GameMain` を生成して初期化し、初期化後も保持します。ローカル変数だけにすると、シーンが参照しているリソース定義などの寿命が切れてしまいます。

### 3.3 Editorから開く

1. `Open` 欄に `Games/MyGame` の絶対パスを入力します。
2. **Open Game Project** を押します。
3. **Open Game** を押します。

現在の登録条件：

- 入力するのは `.vcxproj` 自体ではなく、その親フォルダーです。
- 指定フォルダーの直下に `.vcxproj` が**ちょうど1つ**必要です。サブフォルダーの再帰検索はしません。
- 読み込み先は次の式で決まります。

```text
指定フォルダーの2階層上 / x64 / Release / <vcxprojの拡張子を除いた名前>.dll
```

したがって `Games/MyGame` の配置を使います。任意の場所へ移すと、同じ式で別の場所を探します。

プロジェクト選択の永続保存は、確認した `ProjectManager` / `GameProjectLoader` にはありません。Editor起動後に選択する運用です。登録ボタンはビルドやDLL生成を行いません。

## 4. ゲームを作る基本の流れ

### 4.1 素材を登録する

`GameMain::Initialize` でOBJを `ObjLoader::Load` で読み、`ResourceManager::RegisterMesh` へ渡します。画像は `RegisterTexture` で登録します。返ってきた `MeshHandle` / `TextureHandle` を `GameResources` に保持します。

ゲーム側が持つのはリソースを指定するハンドルです。GPUリソースの生成・管理は `ResourceManager` が担当します。新しい素材を追加したら、次の3点を揃えます。

1. 実際の素材ファイル。
2. `GameResources.h` の対応するハンドル。
3. `GameMain.cpp` の読み込み・登録処理。

現在は素材パスをゲームプロジェクトのフォルダーへ自動切り替えしません。`Assets/...` は作業ディレクトリ基準です。

### 4.2 シーンを実装する

`Origin::Scene` を継承し、次の3つを実装します。既存例は `TestScene` と `TitleScene` です。

| 関数 | 記述する内容 |
| --- | --- |
| `bool Initialize(Origin::Graphics&)` | オブジェクト、カメラなどの初期設定 |
| `void Update()` | 入力、移動、ゲーム状態の更新 |
| `void Render(Origin::Renderer&)` | カメラ設定と描画要求 |

描画の基本は次の順序です。

1. `Origin::RenderObject` にmesh・textureのハンドルとtransformを設定します。
2. `object_.SetRenderObject(...)` で `Origin::Object` に設定します。
3. カメラのtransformと投影設定を設定します。
4. `Render` で `renderer.SetCamera(camera_)` を呼びます。
5. `renderer.Draw(object_)` で描画を要求します。

transformには位置・回転・拡大率を設定します。既存の `TestScene::Initialize` が実際の使用例です。更新時は `object_.GetRenderObject().transform` を操作できます。

### 4.3 シーンを登録する

シーンのファイルを作るだけでは切り替え対象になりません。

1. `.cpp` と `.h` をVisual Studioのゲームプロジェクトへ追加します。
2. `GameSceneType.h` に重複しないIDを追加します。
3. `GameSceneFactory.cpp` で新しいヘッダーをincludeします。
4. `CreateScene` のswitchにcaseを追加し、対応するシーンを `std::make_unique` で生成して返します。
5. 最初に開く場合は `GameMain::Initialize` の `RequestInitialScene(...)` をそのIDにします。

現在の `GameSceneType::Game` はFactoryの生成処理が空で、`Sample` の生成処理はコメントアウトされています。IDが定義されているだけのシーンには遷移しないでください。Factoryがnullptrを返すと、現在の `SceneManager::ChangeScene` はそのまま参照してしまいます。

### 4.4 シーン生成ボタンを使う場合

1. 先に **Open Game Project** でプロジェクトを選択します。未選択時の生成操作を防ぐチェックは現在ありません。
2. `Scene` 欄に、例として `Stage` と入力します。生成名に `Scene` が付くため、`StageScene` ではなく `Stage` とします。
3. **Generate Scene Code** を押します。
4. 次のファイルが生成されます。

```text
Game/Generate/Scenes/Stage/StageScene.h
Game/Generate/Scenes/Stage/StageScene.cpp
```

クラス名として使える単純な英数字名を使用し、空白・パス区切り・記号は避けます。同名のヘッダーまたはcppが存在すると生成は失敗します。

生成されるのは雛形だけです。**Visual Studioへの追加、ID追加、Factory登録は手動**です。またmesh・texture・オブジェクト設定はコメントアウトされているため、素材の設定を完成させてから描画します。

### 4.5 入力とシーン切り替え

現在のTestSceneは `GetAsyncKeyState` で入力を調べています。

| キー | TestSceneの処理 |
| --- | --- |
| W / S | オブジェクトとカメラのZ移動 |
| A / D | オブジェクトとカメラのX移動 |
| 左右矢印 | オブジェクトとカメラのY回転 |
| 上下矢印 | カメラのY移動 |
| Space / Shift | カメラのX回転 |
| 1 | Titleへのシーン切り替え要求 |

シーン切り替えには `sceneRequest_.RequestChangeScene(...)` を使います。ただし現在は要求が保留されず、その場でシーンを交換します。自分自身の `Update` 内から呼ぶと現在のシーンが破棄されるので、切り替え後にそのシーンのメンバーへアクセスしないようにします。遷移後も処理を続ける設計にする場合は、フレーム末尾に交換する方式などの検討が必要です。

現在の `Update()` にdeltaTime引数はありません。またEditorのメインループは複数のウィンドウ描画箇所から `sceneManager.Update()` を呼びます。移動量を「1秒あたり」として扱ったり、1周につき1回だけ更新されると仮定したりしないでください。

## 5. 所有権と責務の読み方

| データ | 現在の扱い・注意点 |
| --- | --- |
| `Object` / `Camera` | シーンが値として所有。通常はポインター化する必要がない |
| `RenderObject` | `SetRenderObject` で描画情報を設定。取得関数の参照を通じて同じ実体を編集できる |
| `GameResources` | `GameMain` のメンバーとして保持するハンドルの集合 |
| `GameResources*` | Factory・Sceneが借りる非所有ポインター。現在のシーンでは有効な値が必要。所有者より長く使わない |
| `SceneChangeRequest&` | 既存の要求窓口への非所有参照。nullptrを表現せず、参照先がシーンより長く存在する必要がある |
| `unique_ptr<Scene>` | SceneManagerがシーンを単独所有。切り替えで旧シーンを破棄する |
| `unique_ptr<ISceneFactory>` | GameMainからSceneChangeRequestへmoveして所有権を移す |

`GameResources*` は所有権を渡さないため、利用側でdeleteしたり、同じ実体を新たなunique_ptrで所有したりしません。必ず存在する契約を型で表すなら参照やconst参照も候補ですが、現行の呼び出しはポインターです。この資料では設計変更は行っていません。

ゲーム固有のルールや入力処理はGame側、描画やGPUリソース管理はEngine側、プロジェクト操作やコード生成はEditor側に置くと、既存の責務分担に沿います。小規模な段階では、新しいManagerやインターフェースを増やすより、シーン内で処理を整理するところから始められます。

## 6. 制作中の確認事項と現在の制約

| 症状・場面 | 確認すること |
| --- | --- |
| プロジェクトが開けない | 実在するフォルダーか、直下のvcxprojが1つか。無効なパスの例外処理や結果のUI表示は不十分 |
| Open Gameで開始しない | ルートのx64/Releaseに、vcxprojと同名のDLLがあるか。依存DLLとエクスポート名も確認 |
| Debugの変更が反映されない | 現在の読み込み先はRelease固定。Debug出力へ自動切り替えしない |
| 再ビルドできない・再読み込みできない | Editorを終了してビルドし直す。現行ローダーは読み込み済みDLLの再Loadを拒否する |
| 新しいシーンが動かない | cppのビルド登録、ID、Factory生成処理、初期シーン指定の4点を確認 |
| 何も描画されない | 素材ファイル、ハンドル設定、カメラ、Draw呼び出し、作業ディレクトリを確認 |
| 素材読み込み後に描画が不安定 | UIのゲーム初期化は起動時のBegin/EndInitializationCommands完了後に行われる。GPUアップロードのコマンド記録・実行タイミングは別途動作確認が必要 |

終了処理にも未完成部分があります。TestGame1の `ShutdownGame` と `GameMain::Finalize` は空です。DLLを切り替える機能を追加する場合は、DLL内のクラスを使うSceneやFactoryを先に破棄し、GPU処理の完了とゲーム資源の寿命を整理してからDLLを解放する必要があります。

## 7. 制作時のチェックリスト

- [ ] Release / x64でEngine・Editor・ゲームをビルドした。
- [ ] ゲームのプロジェクト参照に存在しないパスがない。
- [ ] DLL名とvcxproj名が一致し、ルートのx64/Releaseに出力した。
- [ ] Editorの作業ディレクトリをソリューションルートにした。
- [ ] 読み込むOBJ・画像・シェーダーが実在する。
- [ ] 新しいシーンのソースをゲームプロジェクトへ追加した。
- [ ] シーンIDとFactoryの生成処理を登録した。
- [ ] 描画するObjectに有効なmeshとtextureを設定した。
- [ ] Editorでプロジェクトフォルダーを開いてからゲームを開いた。
- [ ] コード変更後はEditorを終了してから再ビルドした。

## 8. 確認元のコード

以下はリポジトリルートからの相対パスです。

- `OriginEngine.slnx`：ソリューションのプロジェクト一覧。
- `Games/TestGame1/TestGame1.vcxproj`：DLL設定、インクルード、参照、ビルド対象。
- `Games/TestGame1/Game/GameExports/GameExports.cpp`：DLLの入口とGameMainの保持。
- `Games/TestGame1/Game/GameMain.cpp`：素材登録と初期シーン設定。
- `Games/TestGame1/Game/Include/Scene/GameResources.h`：リソースハンドル。
- `Games/TestGame1/Game/Source/Scene/GameSceneFactory.cpp`：シーン生成。
- `Games/TestGame1/Game/Source/Scene/TestScene.cpp`：配置・入力・描画の実例。
- `Editor/Editor/Source/GameProject/GameProjectLoader.cpp`：プロジェクト検出とDLLパス。
- `Editor/Editor/Source/EditorUI/ProjectPanel.cpp`：プロジェクトを開くUI。
- `Editor/Editor/Source/EditorUI/DesignSupportPanel.cpp`：シーン生成UI。
- `Editor/Editor/Source/SceneCodeGenerator.cpp`：生成する雛形の内容。
- `Editor/Editor/Source/GameModuleLoader/GameModuleLoader.cpp`：DLL読み込み。
- `Editor/main.cpp`：起動、描画、シーン更新の流れ。
- `Engine/Source/Scenes/SceneChangeRequest.cpp`、`SceneManager.cpp`：切り替えとシーン所有。

理解の確認：シーンが `GameResources*` を所有せず借りる構成で、GameMainを初期化直後に破棄すると、なぜ問題になるでしょうか。
