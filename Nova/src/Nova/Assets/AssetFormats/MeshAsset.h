#pragma once
#include "Nova/Assets/Asset.h"

namespace Nova::Graphics
{
	class Mesh;
}

namespace Nova
{
	class MeshAsset : public Asset
	{
	public:
		ASSET_TYPE(MeshAsset, EAssetType::Mesh)

		MeshAsset(const AssetID& assetID, const std::string& name);

		/// <summary>
		/// Create or Get the Mesh held by this MeshAsset. Can be nullptr if Disposed
		/// </summary>
		virtual std::shared_ptr<Graphics::Mesh> GetMesh();

	protected:
		std::shared_ptr<Graphics::Mesh> m_Mesh;
	};
}