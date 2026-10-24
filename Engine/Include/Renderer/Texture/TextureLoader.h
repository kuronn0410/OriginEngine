#pragma once
#include "include/Renderer/Texture/TextureData.h"
#include <string>
namespace Origin
{
	class TextureLoader
	{
	public:
		TextureData Load(const std::string& path);

	};
}