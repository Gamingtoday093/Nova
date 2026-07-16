#pragma once
#include <Nova/ImGui/IImGuiLayer.hpp>

class AssetsTab : public Nova::IImGuiLayer
{
public:
	AssetsTab();

	void Render() override;

private:
	using ImGuiTreeNodeFlags = int32_t;
	using ImGuiID = uint32_t;

	void FileBrowserTreeNode(const std::filesystem::path& path, bool expandTree, bool defaultOpen = false);
	void FileBrowserPanel();
	void FileBrowserRecursive(const std::filesystem::path& path, bool expandTree);
	void FileBrowserForceCloseRecursive(const std::filesystem::path& path, ImGuiID seed);

	void AssetFilesPanel();

	std::filesystem::path m_OpenFolder;

	using ImTextureID = void*;
	ImTextureID m_FolderEmptyIcon = nullptr;
	ImTextureID m_FolderOpenIcon = nullptr;
	ImTextureID m_FolderClosedIcon = nullptr;
};
