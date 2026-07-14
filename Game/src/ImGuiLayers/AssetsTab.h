#pragma once
#include <Nova/ImGui/IImGuiLayer.hpp>

class AssetsTab : public Nova::IImGuiLayer
{
public:
	AssetsTab();

	void Render() override;

private:
	using ImGuiTreeNodeFlags = int32_t;

	bool FolderTreeNode(const std::filesystem::path& path, ImGuiTreeNodeFlags flags = 0);
	void FileBrowserPanel();
	void FileBrowserRecursive(const std::filesystem::path& path);

	void AssetFilesPanel();

	std::filesystem::path m_OpenFolder;

	using ImTextureID = void*;
	ImTextureID m_FolderEmptyIcon = nullptr;
	ImTextureID m_FolderOpenIcon = nullptr;
	ImTextureID m_FolderClosedIcon = nullptr;
};
