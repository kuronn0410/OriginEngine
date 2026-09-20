## 目的
1フレームの描画処理の流れを理解するためのドキュメントです。
## 処理フロー
1. SwapChainから現在のBackBuffer番号(描画する場所)を取得
2. CommandAllocator(CommandListがGPU命令を記録するために使うメモリ)をリセット
3. CommandList(GPU命令を記録)をリセット
4. BackBufferのリソースバリアを設定（PRESENT → RENDER_TARGET(表示できる状態)）
5. RTV（どのBackBufferに描画するか）を取得
6. 描画先を設定
7. クリアカラーで描画領域をクリア
8. リソースバリアを設定（RENDER_TARGET → PRESENT(もとの状態)）
9. CommandListを閉じる(CommandListへの命令の記録が終わったから閉じる)
10. CommandListを実行する
11. SwapChainに表示する
12. GPUとCPUの同期を取る(Fenceを使用して)

## APIとの関係
`D3D12_RESOURCE_BARRIER`Resourceの状態を切り替えるための設定を指定する構造体
`ID3D12CommandList` GPUに実行させる命令リストを表す基底インターフェース
`ExecuteCommandLists` Close()で閉じたCommandListを、GPUに実行させるための関数
`Present` 描画が終わったBackBufferを画面表示に回す処理
`Signal` Fenceに値を設定して、GPUの処理が完了したことを通知する関数
`SetEventOnCompletion` Fenceの値が指定した値になったら、イベントを発生させる関数
`WaitForSingleObject` 指定されたイベントが発生するまで待機する関数
