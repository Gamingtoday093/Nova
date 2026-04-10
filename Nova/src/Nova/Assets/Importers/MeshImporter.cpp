#include "novapch.h"
#include "MeshImporter.h"
#include "Nova/Assets/AssetManager.h"
#include "Nova/Graphics/RenderData/VertexFormats.h"
#include "Nova/Graphics/Resources/Mesh.h"
#include <Assimp/Importer.hpp>
#include <Assimp/postprocess.h>
#include <Assimp/scene.h>
#include "Nova/Assets/AssetFormats/MeshImportAsset.h"

bool Nova::Assets::MeshImporter::Supported(const std::filesystem::path& assetPath)
{
	return true;
	if (!assetPath.has_extension()) return false;

	static std::initializer_list<std::string> supportedFileTypes = { ".fbx", ".obj" };
	std::string extension = assetPath.extension().string();

	for (auto& fileType : supportedFileTypes)
		if (extension == fileType) return true;

	return false;
}

std::shared_ptr<Nova::MeshSourceAsset> Nova::Assets::MeshImporter::LoadFromFile(const std::filesystem::path& assetPath)
{
	// TODO: Read "assetPath .asset" Ex: SillyCat.fbx.asset

	int importFlags = 
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_CalcTangentSpace |
		aiProcess_ConvertToLeftHanded;

	// .asset Import Skeletons (Default: true)
	importFlags |=
		aiProcess_PopulateArmatureData |
		aiProcess_LimitBoneWeights;

	// TODO: Try get Data from custom cache instead of actual .fbx file, should be way faster!
	// This takes about 10-20ms, Cooked Binary format probably takes max 5ms
	Assimp::Importer importer;
	auto scene = importer.ReadFile(assetPath.string(), importFlags);

	if (!scene) return nullptr;

	std::shared_ptr<MeshSourceAsset> meshSource = std::make_shared<MeshSourceAsset>(
		AssetID::NewID(), // IF: .asset doesn't exist
		assetPath);

	// TODO: Create MeshImportAssets etc. from .asset so LoadFromNode() can use them

	LoadFromNodeRecursive(scene, scene->mRootNode, meshSource);

	// Create/Replace .asset
	// TODO: Create Cooked Binary Format

	return meshSource;
}

void Nova::Assets::MeshImporter::ReloadFromFile(std::shared_ptr<MeshSourceAsset> meshSource)
{
	// TODO: Combine Duplicate Logic
	int importFlags =
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_CalcTangentSpace |
		aiProcess_ConvertToLeftHanded;

	if (meshSource->GetSettings().Skeletons.ImportSkeletons)
	{
		importFlags |=
			aiProcess_PopulateArmatureData |
			aiProcess_LimitBoneWeights;
	}

	Assimp::Importer importer;
	auto scene = importer.ReadFile(meshSource->GetAssetPath().string(), importFlags);
	
	if (!scene)
	{
		NOVA_CORE_ERROR("Failed to Reload MeshSourceAsset ({0})", meshSource->GetAssetID().ToString());
		return;
	}

	LoadFromNodeRecursive(scene, scene->mRootNode, meshSource);

	// TODO: Create Cooked Binary Format
}

void Nova::Assets::MeshImporter::LoadFromNodeRecursive(const aiScene* scene, aiNode* node, std::shared_ptr<MeshSourceAsset> meshSource)
{
	LoadFromNode(scene, node, meshSource);

	for (uint32_t childIndex = 0; childIndex < node->mNumChildren; childIndex++)
		LoadFromNodeRecursive(scene, node->mChildren[childIndex], meshSource);
}

void Nova::Assets::MeshImporter::LoadFromNode(const aiScene* scene, aiNode* node, std::shared_ptr<MeshSourceAsset> meshSource)
{
	if (node->mNumMeshes == 0) return;

	std::shared_ptr<MeshImportAsset> importMesh;
	for (auto& importMeshAsset : meshSource->GetMeshAssets())
		if (node->mName.C_Str() == importMeshAsset->GetName())
		{
			importMesh = importMeshAsset;
			break;
		}

	const MeshOptionalSettings optionalSettings = importMesh ? importMesh->GetSettings() : MeshOptionalSettings();

	std::vector<Graphics::Vertex> vertices;
	std::vector<uint16_t> indices;
	std::vector<Graphics::SubMesh> subMeshes;

	for (size_t meshIndex = 0; meshIndex < node->mNumMeshes; meshIndex++)
	{
		auto mesh = scene->mMeshes[node->mMeshes[meshIndex]];

		uint32_t vertexOffset = uint32_t(vertices.size());
		uint32_t indexOffset = uint32_t(indices.size());

		for (size_t vertexIndex = 0; vertexIndex < mesh->mNumVertices; vertexIndex++)
		{
			auto vertex = mesh->mVertices[vertexIndex];

			aiVector3D normal;
			if (mesh->HasNormals())
				normal = mesh->mNormals[vertexIndex];

			float texCoord0U = 0, texCoord0V = 0;
			if (mesh->HasTextureCoords(0))
			{
				texCoord0U = mesh->mTextureCoords[0][vertexIndex].x;
				texCoord0V = mesh->mTextureCoords[0][vertexIndex].y;
			}

			vertices.emplace_back(vertex.x, vertex.y, vertex.z);
		}

		for (size_t faceIndex = 0; faceIndex < mesh->mNumFaces; faceIndex++)
		{
			auto& face = mesh->mFaces[faceIndex];

			for (size_t i = 0; i < face.mNumIndices; i++)
				indices.push_back(face.mIndices[i] + (optionalSettings.MergeSubMeshes ? indexOffset : 0));
		}

		if (!optionalSettings.MergeSubMeshes)
			subMeshes.emplace_back(indexOffset, uint32_t(indices.size()) - indexOffset, vertexOffset, mesh->mMaterialIndex);

		if (mesh->HasBones() && meshSource->GetSettings().Skeletons.ImportSkeletons)
		{

		}
	}

	if (!importMesh)
	{
		importMesh = AssetManager::CreateAsset<MeshImportAsset>(AssetID::NewID(), node->mName.C_Str(), meshSource);
		meshSource->m_MeshAssets.push_back(importMesh);
	}

	if (subMeshes.size() == 1) subMeshes.clear();
	auto meshData = Graphics::MeshData(std::move(vertices), std::move(indices), std::move(subMeshes));

	bool readWriteable = optionalSettings.ReadWriteable.value_or(meshSource->GetSettings().Meshes.ReadWriteable);
	if (importMesh->m_Mesh) *importMesh->m_Mesh = Graphics::Mesh(meshData, readWriteable);
	else importMesh->m_Mesh = std::make_shared<Graphics::Mesh>(meshData, readWriteable);
}
