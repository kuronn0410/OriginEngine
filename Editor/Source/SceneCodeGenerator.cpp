#include "Include/SceneCodeGenerator.h"

#include <fstream>
#include <string>
using std::string;

bool SceneCodeGenerator::SceneCodeGenerate(const string& filename)
{
	std::ofstream ofs("Game/Generate/" + filename+ "Scene.cpp");
	std::ofstream ofsHeader("Game/Generate/" + filename + "Scene.h");
	
	if (!ofs)
	{
		return false;
	}
	if(!ofsHeader)
	{
		return false;
	}
	/*シーンcppファイルの生成*/
	ofs << "#include \"" << filename << "Scene.h\"\n\n";
	ofs << "void " << filename << "::Initialize()\n"
		<< "{\n"
		<< "\t// 初期化処理\n"
		<< "}\n\n";

	ofs << "void " << filename << "::Update()\n"
		<< "{\n"
		<< "\t// 更新処理\n"
		<< "}\n\n";

	ofs << "void " << filename << "::Draw()\n"
		<< "{\n"
		<< "\t// 描画処理\n"
		<< "}\n";


	/*シーンhファイルの生成*/
	ofsHeader << "#pragma once\n\n";
	ofsHeader << "#include \"Include/Scenes/IScene.h\"\n\n";
	ofsHeader << "class " << filename << "Scene\n"
		<< ": public IScene\n"
		<< "{\n"
		<< "public:\n"
		<< "\tvoid Initialize();\n"
		<< "\tvoid Update();\n"
		<< "\tvoid Draw();\n"
		<< "};\n";
	return true;
}
