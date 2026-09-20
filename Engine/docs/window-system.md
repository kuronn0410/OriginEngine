window-system.md

## 目的
ウィンドウの生成・破棄、イベントの受け取り、描画の制御などを行う。

## 処理の順番
WinMain

↓

WNDCLASSを作る

↓

RegisterClassで登録

↓

CreateWindow

↓

ShowWindow

↓

GetMessageで待つ

↓

WindowProcで処理

↓

×ボタン

↓

WM_DESTROY

↓

PostQuitMessage

↓

終了


## クラス・構造体構成


## 所有関係
Window
└─ HWND を保持

main.WinMain
└─ Window を保持

## 参照関係


## 依存関係

## リソースの生成・破棄
Window
- main.WinMain()でWindow.Initializeを呼び出して生成
- 終了時にWindow.Destroyを呼び出して破棄する。

## APIとの関係
- WNDCLASS
  - ウィンドウクラスの設定を保持
- RegisterClass
  - WNDCLASSをWindowsへ登録
- CreateWindow
  - 実際のウィンドウを生成
- ShowWindow
  - ウィンドウを表示
- GetMessage
  - メッセージを取得
- DispatchMessage
  - WindowProcへメッセージを送る

## 注意点