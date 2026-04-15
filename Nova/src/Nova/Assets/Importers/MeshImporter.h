#pragma once
#include "Nova/Assets/AssetFormats/ModelSourceAsset.h"

struct aiNode;
struct aiScene;

namespace Nova::Assets
{
	class MeshImporter
	{
	public:
		static bool Supported(const std::filesystem::path& assetPath);
		static std::shared_ptr<ModelSourceAsset> LoadFromPath(const std::filesystem::path& assetPath);
		static void ReloadFromPath(std::shared_ptr<ModelSourceAsset> meshSource);

	private:
		static void LoadFromNodeRecursive(const aiScene* scene, aiNode* node, std::shared_ptr<ModelSourceAsset> meshSource);
		static void LoadFromNode(const aiScene* scene, aiNode* node, std::shared_ptr<ModelSourceAsset> meshSource);
	};
}
