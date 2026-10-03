#include "include/Renderer/Texture/TextureLoader.h"
#include "include/Renderer/Texture/TextureData.h"
#include <string>
#include <Windows.h>
#include <dxgiformat.h>
#include <vector>
#include <string.h>
#include "External/stb/stb_image.h"

TextureData TextureLoader::Load(const std::string& filePath)
{
	TextureData textureData{};

	//1.path の画像を読み込む
	int width = 0;
	int height = 0;
	int channels = 0;

	unsigned char* pixels = stbi_load(
		filePath.c_str(),
		&width,
		&height,
		&channels,
		4
	);
	//2.デコーダーから
	// width
	//height
	//format
	//pixels
	//を取得する

	if (!pixels) {
		// エラー処理
		return textureData;
	}
	const size_t dataSize =
		static_cast<size_t>(width) *
		static_cast<size_t>(height) *
		4;
	//3.TextureData に詰める
	textureData.width = static_cast<UINT>(width);
	textureData.height = static_cast<UINT>(height);
	textureData.format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureData.pixels.resize(dataSize);

	std::memcpy(textureData.pixels.data(), pixels, dataSize);

	stbi_image_free(pixels);

	return textureData;
}