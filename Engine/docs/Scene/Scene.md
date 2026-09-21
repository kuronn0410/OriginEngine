## 目的
シーンの管理を行うクラス群の設計についてまとめる。

## 全体フロー
### 初期化時
main.cpp
Graphics生成
Graphics初期化
SceneManager 生成
ceneChangeRequest (sceneManager)　生成
GameMain 生成
Initialize(sceneRequest))　GameMain 初期化

↓
GmaeMain.cpp
GameMain::Initialize(sceneRequest)
GameMainがGameSceneFactoryを生成し、一時的に所有する

↓

SceneChangeRequesにISceneFactoryの所有権を渡す

↓

最初に生成するシーンをSceneChangeRequesに登録する


### シーン移動
参照しているSceneChangeRequesにシーン移動の要求を登録する

↓

ISceneFactory(ゲーム固有のSceneFactory)にTypeに応じたシーンを生成してもらう

↓

SceneChangeRequesが生成したシーンの所有権をSceneManagerに送る

↓

SceneManagerが現在のシーンを破棄して、新しいシーンに切り替える




## 各クラスの責務

## 所有関係

## シーン変更の流れ

## include / 前方宣言の考え方

## 現状の仮実装

## 今後直したい点