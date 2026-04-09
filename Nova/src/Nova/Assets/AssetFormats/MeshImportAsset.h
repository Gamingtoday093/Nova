#pragma once
#include "MeshAsset.h"

namespace Nova
{
	class MeshSourceAsset;
}

namespace Nova::Assets
{
	class MeshImporter;
}

namespace Nova
{
	struct MeshOptionalSettings
	{
		std::optional<float> UniformScale; // TODO
		/// <summary>
		/// Merges all SubMeshes into a Single Mesh. SubMeshes allows for Multiple Materials per Mesh
		/// </summary>
		bool MergeSubMeshes = false;
		std::optional<bool> ReadWriteable;

		bool operator==(const MeshOptionalSettings& other) const;
	};

	class MeshImportAsset : public MeshAsset
	{
	public:
		ASSET_TYPE(MeshImportAsset, EAssetType::Mesh)

		MeshImportAsset(const AssetID& assetID, const std::string& name, std::weak_ptr<MeshSourceAsset> meshSource);

		const MeshOptionalSettings& GetSettings() const;
		void SetSettings(const MeshOptionalSettings& settings);

		/// <summary>
		/// Create or Get the Mesh held by this MeshAsset. Returns nullptr if this Asset has been Disposed
		/// </summary>
		std::shared_ptr<Graphics::Mesh> GetMesh() override;

		/// <summary>
		/// Release the Mesh held by this MeshAsset forcing ReloadFromSource() next GetMesh()
		/// </summary>
		void Invalidate();

	protected:
		void DisposeAsset() override;

	private:
		void ReloadFromSource();

		std::weak_ptr<MeshSourceAsset> m_MeshSource;
		MeshOptionalSettings m_Settings;

		friend class Assets::MeshImporter;
	};
}
