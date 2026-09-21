## 目的
シーンの管理を行うクラス群の設計についてまとめる。

## 全体フロー
初期化時
main
window生成
Graphics初期化
SceneManager 生成
SceneChangeRequest (sceneManager)　生成
GameMain 初期化
Initialize(sceneRequest))

↓

GameMain::Initialize(sceneRequest)
sceneChangeRequestに初期シーンを登録する
GameSceneFactory(sceneRequest)

## 修正

main.cpp
Window生成
Graphics初期化
SceneManager 生成
SceneManager 生成
SceneChangeRequest (sceneManager)　生成

↓

GameMain::Initialize(sceneRequest)
sceneChangeRequestに初期シーンを登録する

↓

GameSceneFactory(sceneRequest)

↓

sceneChangeRequestに生成したシーンの所有権を
SceneManagerに送る




