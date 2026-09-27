# rendering-flow.md

## 概要

描画処理の流れ

---

## 初期化フロー

### WinMain

Graphics生成

↓

ShaderCompiler生成

↓

Renderer.Initialize(ShaderCompiler、Graphics.Device)

↓

SceneManager(Graphics)生成

↓

- GameMain
  - 初期シーン切り替え
- SceneManager.ChangeScene(初期シーン)
  - 初期シーン.Initialize(Renderer、Graphics)
  - 初期シーンの所有するMesh生成

---

## 描画フロー

### SceneManager.Render(Renderer)

現在のシーンのRenderを呼び出す

↓

現在のシーンが所有しているMeshを

```cpp
renderer.Draw(mesh_);
```

でRendererに渡し、Meshの描画を行う