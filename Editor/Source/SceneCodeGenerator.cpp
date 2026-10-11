#include "Include/SceneCodeGenerator.h"

#include <fstream>

bool SceneCodeGenerator::SceneCodeGenerate()
{
	std::ofstream ofs("Test.txt");
	if (!ofs)
	{
		return false;
	}
	ofs << "Hello, World!" << std::endl;
	return true;
}
