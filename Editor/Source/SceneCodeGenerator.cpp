#include "Include/SceneCodeGenerator.h"

#include <fstream>
#include <string>
#include <filesystem>
using std::string;

namespace Origin::Editor
{
	bool SceneCodeGenerator::SceneCodeGenerate(const string& filename)
	{
		// フォルダが存在しなければ生成
		namespace fs = std::filesystem;
		fs::create_directories("Games/TestGame1/Game/Generate");
		fs::create_directories("Games/TestGame1/Game/Generate/Scenes");
		fs::create_directories("Games/TestGame1/Game/Generate/Scenes/" + filename);

		std::string cppPath
			= "Games/TestGame1/Game/Generate/Scenes/" + filename + "/" + filename + "Scene.cpp";
		std::string hPath
			= "Games/TestGame1/Game/Generate/Scenes/" + filename + "/" + filename + "Scene.h";

		if (fs::exists(cppPath))
		{
			return false;
		}
		if (fs::exists(hPath))
		{
			return false;
		}

		std::ofstream ofs(
			"Games/TestGame1/Game/Generate/Scenes/" + filename + "/" + filename + "Scene.cpp"
		);
		std::ofstream ofsHeader(
			"Games/TestGame1/Game/Generate/Scenes/" + filename + "/" + filename + "Scene.h"
		);

		if (!ofs)
		{
			return false;
		}
		if (!ofsHeader)
		{
			return false;
		}
		/*シーンcppファイルの生成*/
		ofs << "#include \"" << filename << "Scene.h\"\n";
		ofs << "#include \"include/Renderer/Camera/Camera.h\"\n";
		ofs << "#include \"include/Scenes/SceneChangeRequest.h\"\n";
		ofs << "#include \"include/Scene/GameSceneType.h\"\n";
		ofs << "#include \"include/Renderer/Object/RenderObject.h\"\n";
		ofs << "#include \"include/Renderer/Render/Renderer.h\"\n";
		ofs << "#include \"include/Scenes/SceneChangeRequest.h\"\n";
		ofs << "#include \"include/Scene/GameResources.h\"\n\n";
		ofs << "namespace Game\n"
			<< "{\n";
		ofs << "\tbool " << filename << "Scene::Initialize(Origin::Graphics& graphics)\n"
			<< "\t{\n"
			<< "\t// 初期化処理\n"
			<< "\t\t/*--------Objectの初期化----------*/\n"
			<< "\t\tOrigin::RenderObject renderObject;\n"
			<< "\t\t//renderObject.texture = gameResources_->groundTexture;\n"
			<< "\t\t//renderObject.mesh = gameResources_->CubeMesh;\n"
			<< "\t\t//renderObject.transform = {\n"
			<< "\t\t\t//{ 0.0f, -3.0f, -0.5f },//P\n"
			<< "\t\t\t//{ 0.0f, 0.0f, 0.0f },//R\n"
			<< "\t\t\t//{ 10.5f, 0.2f, 10.5f }//S\n"
			<< "\t\t//};\n"
			<< "\t\t//object_.SetRenderObject(renderObject);\n"
			<< "\t\t/*--------Cameraの初期化----------*/\n"
			<< "\t\tcamera_.SetTransform({\n"
			<< "\t\t\t{ 0.0f, 0.0f, -5.0f },//P\n"
			<< "\t\t\t{ 0.0f, 0.0f, 0.0f },//R\n"
			<< "\t\t});\n"
			<< "\t\tcamera_.SetProjectionSettings({\n"
			<< "\t\t\t60.0f,//fov\n"
			<< "\t\t\t1280.0f / 720.0f,//aspectRatio\n"
			<< "\t\t\t0.1f,//nearPlane\n"
			<< "\t\t\t100.0f//farPlane\n\n"
			<< "\t\t});\n"
			<< "\t\treturn true;\n"
			<< "\t}\n\n";

		ofs << "\tvoid " << filename << "Scene::Render(Origin::Renderer& renderer)\n"
			<< "\t{\n"
			<< "\t// 描画処理\n"
			<< "\t\trenderer.SetCamera(camera_);\n"
			<< "\t\trenderer.Draw(object_);\n"
			<< "\t}\n";

		ofs << "\tvoid " << filename << "Scene::Update()\n"
			<< "\t{\n"
			<< "\t// 更新処理\n"
			<< "\t}\n\n";

		ofs << "}\n";


		/*シーンhファイルの生成*/
		ofsHeader << "#pragma once\n\n";
		ofsHeader << "#include \"Include/Scenes/Scene.h\"\n";
		ofsHeader << "#include \"include/Renderer/Camera/Camera.h\"\n";
		ofsHeader << "#include \"include/Scene/GameResources.h\"\n\n";
		ofsHeader << "#include \"include/Renderer/Object/Object.h\"\n\n";
		ofsHeader << "namespace Origin\n"
			<< "{\n"
			<< "\tclass SceneChangeRequest;\n"
			<< "\tclass Renderer;\n"
			<< "\tclass Graphics;\n"
			<< "}\n\n"; 
		ofsHeader << "namespace Game\n"
			<< "{\n";
		ofsHeader << "\tclass " << filename << "Scene : public Origin::Scene\n"
			<< "\t{\n"
			<< "\tpublic:\n"
			<< "\t\t" << filename << "Scene(\n"
			<< "\t\t\tOrigin::SceneChangeRequest& sceneRequest,\n"
			<< "\t\t\tGameResources* gameResources\n"
			<< "\t\t) :\n"
			<< "\t\t\tScene(sceneRequest),\n"
			<< "\t\t\tgameResources_(gameResources)\n"
			<< "\t\t{\n"
			<< "\t\t}\n"
			<< "\t\tbool Initialize(Origin::Graphics& graphics);\n"
			<< "\t\tvoid Update()override;\n"
			<< "\t\tvoid Render(Origin::Renderer& renderer);\n"
			<< "\tprivate:\n"
			<< "\t\tOrigin::Object object_;\n"
			<< "\t\tOrigin::Camera camera_;\n"
			<< "\t\tGameResources* gameResources_ = nullptr;\n"
			<< "\t};\n";
		ofsHeader << "}\n";
		return true;
	}
}
