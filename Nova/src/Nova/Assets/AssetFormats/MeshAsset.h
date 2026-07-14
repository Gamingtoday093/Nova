#pragma once
#include "Nova/Assets/Asset.h"

namespace Nova
{
	class ModelSourceAsset;
}

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
	struct MeshOptionalSettings
	{
		std::optional<float> UniformScale; // TODO
		/// <summary>
		/// Merges all SubMeshes into a Single Mesh. SubMeshes allows for Multiple Materials per Mesh
		/// </summary>
		bool MergeSubMeshes = false;
		/// <summary>
		/// Allows for CPU Read and Write to the Mesh after Importing
		/// </summary>
		std::optional<bool> ReadWriteable;

		bool operator==(const MeshOptionalSettings& other) const;
	};

	class MeshAsset : public Asset
	{
	public:
		ASSET_TYPE(MeshAsset, EAssetType::Mesh)

		MeshAsset(const AssetID& assetID, const std::string& name, std::weak_ptr<ModelSourceAsset> meshSource);

		const MeshOptionalSettings& GetSettings() const;
		void SetSettings(const MeshOptionalSettings& settings);

		/// <summary>
		/// Create or Get the Mesh held by this MeshAsset. Returns nullptr if this Asset has been Disposed
		/// </summary>
		AssetRef<Graphics::Mesh> GetMesh();

		/// <summary>
		/// Release the Mesh held by this MeshAsset forcing ReloadFromSource() next GetMesh()
		/// </summary>
		void Invalidate();

	protected:
		void DisposeAsset() override;

	private:
		void ReloadFromSource();

		AssetRef<Graphics::Mesh> m_Mesh = nullptr;

		std::weak_ptr<ModelSourceAsset> m_MeshSource;
		MeshOptionalSettings m_Settings;

		friend class Assets::MeshImporter;
	};
}
