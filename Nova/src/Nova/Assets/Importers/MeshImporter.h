#pragma once
#include "Nova/Assets/AssetFormats/MeshSourceAsset.h"

struct aiNode;
struct aiScene;

namespace Nova::Assets
{
	class MeshImporter
	{
	public:
		static bool Supported(const std::filesystem::path& assetPath);
		static std::shared_ptr<MeshSourceAsset> LoadFromFile(const std::filesystem::path& assetPath);
		static void ReloadFromFile(std::shared_ptr<MeshSourceAsset> meshSource);

	private:
		static void LoadFromNodeRecursive(const aiScene* scene, aiNode* node, std::shared_ptr<MeshSourceAsset> meshSource);
		static void LoadFromNode(const aiScene* scene, aiNode* node, std::shared_ptr<MeshSourceAsset> meshSource);
	};
}
