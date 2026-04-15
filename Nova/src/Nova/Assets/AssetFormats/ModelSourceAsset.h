#pragma once
#include "Nova/Assets/Asset.h"

namespace Nova::Graphics
{
	class Mesh;
}

namespace Nova::Assets
{
	class MeshImporter;
}

namespace Nova
{
	class MeshImportAsset;
}

namespace Nova
{
	struct MeshImportSettings
	{
		struct MeshSettings
		{
			float UniformScale = 1.f; // TODO
			/// <summary>
			/// Merges all Meshes into a Single MeshAsset
			/// </summary>
			bool MergeMeshes = false; // TODO
			bool ReadWriteable = false;
		};

		struct SkeletonSettings
		{
			bool ImportSkeletons = true;
		};

		struct AnimationSettings
		{
			bool ImportAnimations = true;
		};

		MeshSettings Meshes;
		SkeletonSettings Skeletons;
		AnimationSettings Animations;

		bool operator==(const MeshImportSettings& other) const;
	};

	class ModelSourceAsset : public SourceAsset
	{
	public:
		ASSET_TYPE(ModelSourceAsset, EAssetType::MeshSource)

		ModelSourceAsset(const AssetID& assetID, const std::filesystem::path& assetPath);

		const MeshImportSettings& GetSettings() const;
		void SetSettings(const MeshImportSettings& settings);

		void ReloadFromSource();
		void UnloadUnused(bool loadedAssets = false);

		const std::vector<std::shared_ptr<MeshImportAsset>>& GetMeshAssets() const;

	private:
		MeshImportSettings m_Settings;

		std::vector<std::shared_ptr<MeshImportAsset>> m_MeshAssets;
		// TODO: m_Skeletons
		// TODO: m_Animations

		friend class Assets::MeshImporter;
	};
}
