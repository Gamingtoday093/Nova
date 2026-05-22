#pragma once
#include "AssetID.h"
#include <variant>

namespace Nova
{
	class AssetManager;
}

namespace Nova
{
	enum class EAssetType
	{
		None,
		ModelSource,
		Mesh,
		Skeleton,
		Animation,
		Texture,
		Skybox,
		Material,
		Shader,
		Prefab
	};

	template<class T>
	using AssetRef = std::shared_ptr<T>;

	class Asset
	{
	public:
		const AssetID& GetAssetID() const;
		const std::string& GetName() const;

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
		explicit Asset(const AssetID& assetID, const std::string& name);
		virtual ~Asset() = default;
		
		virtual void DisposeAsset() { }

	private:
		AssetID m_AssetID;
		std::string m_Name;

		friend class AssetManager;
	};

	class SourceAsset : public Asset
	{
	public:
		const std::filesystem::path& GetAssetPath() const;

		inline static const char* GetAssetName_s() { return "BaseSourceAsset"; }
		inline static EAssetType GetAssetType_s() { return EAssetType::None; }

	protected:
		explicit SourceAsset(const AssetID& assetID, const std::string& name);
		explicit SourceAsset(const AssetID& assetID, const std::filesystem::path& assetPath);
		virtual ~SourceAsset() override = default;

	private:
		std::filesystem::path m_AssetPath;
	};

	template<typename T>
	class IResolvableAsset
	{
	public:
		virtual AssetRef<T> Resolve() const = 0;
	};

	template<typename TAsset>
	concept AssetType = std::derived_from<TAsset, Asset>;

	template<typename TAsset>
	concept SourceAssetType = std::derived_from<TAsset, SourceAsset>;

	template<typename TAsset, typename T>
	concept ResolvableAssetType = std::derived_from<TAsset, IResolvableAsset<T>>;

	// Not sure if I like this or not, also tried std::variant but it was 6 times slower. I am not sure exactly how to hold both Ex: the Mesh and the MeshAsset
	template<typename T, ResolvableAssetType<T> TAsset>
	class AssetStorage
	{
	public:
		AssetRef<T> ResolveCopy() const;
		const AssetRef<T>& Resolve() const;

		std::shared_ptr<TAsset> GetAsset() const;

		AssetStorage& operator=(AssetRef<T> value)
		{
			if (m_Asset && value == m_Asset->Resolve()) return *this;
			
			m_Asset = nullptr;
			m_Value = std::move(value);
			return *this;
		}

		AssetStorage& operator=(std::shared_ptr<TAsset> asset)
		{
			m_Asset = std::move(asset);
			m_Value = std::move(m_Asset->Resolve());
			return *this;
		}

		AssetStorage& operator=(std::nullptr_t)
		{
			m_Asset = nullptr;
			m_Value = nullptr;
			return *this;
		}

	private:
		std::shared_ptr<TAsset> m_Asset;
		AssetRef<T> m_Value;
	};
	
	template<typename T, ResolvableAssetType<T> TAsset>
	inline AssetRef<T> AssetStorage<T, TAsset>::ResolveCopy() const
	{
		return m_Value;
	}

	template<typename T, ResolvableAssetType<T> TAsset>
	inline const AssetRef<T>& AssetStorage<T, TAsset>::Resolve() const
	{
		return m_Value;
	}

	template<typename T, ResolvableAssetType<T> TAsset>
	inline std::shared_ptr<TAsset> AssetStorage<T, TAsset>::GetAsset() const
	{
		return m_Asset;
	}
}