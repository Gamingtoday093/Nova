#pragma once
#include "AssetID.h"

namespace Nova
{
	class AssetManager;
}

namespace Nova
{
	enum class EAssetType
	{
		None,
		MeshSource,
		Mesh,
		Skeleton,
		Animation,
		Texture,
		Material,
		Prefab
	};

	class Asset
	{
	public:
		explicit Asset(const AssetID& assetID, const std::string& name);
		virtual ~Asset() = default;

		const AssetID& GetAssetID() const;

		const std::string& GetName() const;
		void SetName(const std::string& name);

		virtual const char* GetAssetName() const = 0;
		inline static const char* GetAssetName_s() { return "BaseAsset"; }
		virtual EAssetType GetAssetType() const = 0;
		inline static EAssetType GetAssetType_s() { return EAssetType::None; }

#define ASSET_TYPE(assetName, assetType)										\
		const char* GetAssetName() const override { return GetAssetName_s(); }	\
		inline static const char* GetAssetName_s() { return #assetName; }		\
		EAssetType GetAssetType() const override { return GetAssetType_s(); }	\
		inline static EAssetType GetAssetType_s() { return assetType; }

	protected:
		virtual void DisposeAsset() { }

	private:
		AssetID m_AssetID;

		std::string m_Name;

		friend class AssetManager;
	};

	class SourceAsset : public Asset
	{
	public:
		explicit SourceAsset(const AssetID& assetID, const std::filesystem::path& assetPath);
		~SourceAsset() override = default;

		const std::filesystem::path& GetAssetPath() const;

		inline static const char* GetAssetName_s() { return "BaseSourceAsset"; }
		inline static EAssetType GetAssetType_s() { return EAssetType::None; }

	private:
		std::filesystem::path m_AssetPath;
	};

	template<typename TAsset>
	concept AssetType = std::derived_from<TAsset, Asset>;

	template<typename TAsset>
	concept SourceAssetType = std::derived_from<TAsset, SourceAsset>;
}