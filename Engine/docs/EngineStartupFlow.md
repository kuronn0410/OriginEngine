## 目的
シーンの管理を行うクラス群の設計についてまとめる。

## 全体フロー
初期化時
main
Graphics初期化
SceneManager 生成
ceneChangeRequest (sceneManager)　生成
GameMain 初期化
Initialize(sceneRequest))

↓

GameMain::Initialize(sceneRequest)
ceneChangeRequestに初期シーンを登録する