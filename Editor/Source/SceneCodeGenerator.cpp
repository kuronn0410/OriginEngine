#include "Include/SceneCodeGenerator.h"

#include <fstream>
#include <string>
using std::string;

bool SceneCodeGenerator::SceneCodeGenerate(const string& filename)
{
	std::ofstream ofs("Game/Generate/" + filename+ ".cpp");
	std::ofstream ofsHeader("Game/Generate/" + filename + ".h");
	
	if (!ofs)
	{
		return false;
	}
	if(!ofsHeader)
	{
		return false;
	}
	ofs << "Hello, World!" << std::endl;
	return true;
}
