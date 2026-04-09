#pragma once
#include "Nova/Assets/Asset.h"

namespace Nova::Graphics
{
	class Mesh;
}

// MeshSourceAsset Loaded from .fbx file
// Each Mesh gets a MeshAsset
//
// MeshAsset is an Asset Wrapper for a Mesh
// Mesh holds Mesh CPU and GPU Data

namespace Nova
{
	class MeshAsset : public Asset
	{
	public:
		ASSET_TYPE(MeshAsset, EAssetType::Mesh)

		MeshAsset(const AssetID& assetID, const std::string& name);

		// GetMesh() Return already Loaded Mesh/Load from CPU data?
		// Should also hold CPU data (MESH SHOULD ACTUALLY)

		/// <summary>
		/// Create or Get the Mesh held by this MeshAsset. Can be nullptr if Disposed
		/// </summary>
		virtual std::shared_ptr<Graphics::Mesh> GetMesh();

	protected:
		std::shared_ptr<Graphics::Mesh> m_Mesh;
	};
}