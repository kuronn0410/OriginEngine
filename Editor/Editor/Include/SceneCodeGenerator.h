#pragma once
#include <string>
#include "GameProject/GameProject.h"
using std::string;
namespace Origin::Editor
{
	class SceneCodeGenerator
	{
	public:
		bool SceneCodeGenerate(const string& filename, const GameProject& project);

	private:

	};
}