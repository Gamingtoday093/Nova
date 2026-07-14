#include "pch.h"
#include "AssetsTab.h"
#include <ImGui/ImGui.h>
#include <Nova/Assets/AssetManager.h>
#include <Nova/Assets/AssetFormats/Texture2DAsset.h>
#include <d3d11.h>
#include <Nova/Graphics/Bindables/Texture/Texture2D.h>

AssetsTab::AssetsTab()
{
	m_OpenFolder = Nova::AssetManager::GetAssetsFullPath();

	auto folderEmpty = Nova::AssetManager::GetAsset<Nova::Texture2DAsset>("Assets/Icons/FolderEmpty.png");
	if (folderEmpty)
		m_FolderEmptyIcon = static_cast<ImTextureID>(folderEmpty->GetTexture()->GetTexture());

	auto folderOpen = Nova::AssetManager::GetAsset<Nova::Texture2DAsset>("Assets/Icons/FolderOpen.png");
	if (folderOpen)
		m_FolderOpenIcon = static_cast<ImTextureID>(folderOpen->GetTexture()->GetTexture());

	auto folderClosed = Nova::AssetManager::GetAsset<Nova::Texture2DAsset>("Assets/Icons/FolderClosed.png");
	if (folderClosed)
		m_FolderClosedIcon = static_cast<ImTextureID>(folderClosed->GetTexture()->GetTexture());
}

void AssetsTab::Render()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });
	ImGui::Begin("Assets");

	FileBrowserPanel();

	ImGui::SameLine(0, 0);
	
	AssetFilesPanel();

	ImGui::End();
	ImGui::PopStyleVar();
}

bool AssetsTab::FolderTreeNode(const std::filesystem::path& path, ImGuiTreeNodeFlags flags)
{
	std::string label = path.filename().string();
	bool open = ImGui::TreeNodeEx(("##" + label).c_str(),
		(path == m_OpenFolder ? ImGuiTreeNodeFlags_Selected : ImGuiTreeNodeFlags_None) |
		ImGuiTreeNodeFlags_SpanAvailWidth | flags);

	if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
	{
		m_OpenFolder = path;
	}

	ImGui::SameLine(0, 8);
	ImTextureID image = std::filesystem::is_empty(path) ? m_FolderEmptyIcon : open ? m_FolderOpenIcon : m_FolderClosedIcon;
	if (image)
	{
		ImGui::Image(image, { 13.f, 13.f });
		ImGui::SameLine();
	}

	ImGui::Text(label.c_str());

	return open;
}

void AssetsTab::FileBrowserPanel()
{
	ImGui::SetNextWindowSizeConstraints({ 0, -1 }, { ImGui::GetWindowWidth() - ImGui::GetStyle().WindowMinSize.x, -1});
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 4.f, 4.f });
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 5.f, 4.f });
	ImGui::BeginChild("FileBrowser", { 0.f, 0.f }, ImGuiChildFlags_AlwaysUseWindowPadding | ImGuiChildFlags_ResizeX);
	
	if (FolderTreeNode(Nova::AssetManager::GetAssetsFullPath(), ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_DefaultOpen))
	{
		FileBrowserRecursive(Nova::AssetManager::GetAssetsFullPath());
		ImGui::TreePop();
	}

	ImGui::EndChild();
	ImGui::PopStyleVar(2);

	ImGui::SameLine(0, 0);
	
	// For some reason ImGuiChildFlags_ResizeX doesn't render the Handle/Border when not Hovered or Active so Manually drawing it
	ImVec2 pos = ImGui::GetCursorPos();
	ImVec2 min = ImGui::GetWindowPos();
	ImVec2 size = ImGui::GetWindowSize();
	ImVec2 max = { min.x + size.x, min.y + size.y };

	// ForegroundDrawlist because Scrollbar loves to Render on-top
	ImGui::GetForegroundDrawList()->AddRectFilled(
		ImVec2(min.x + pos.x - 1.f, min.y + ImGui::GetFrameHeight()),
		ImVec2(min.x + pos.x + 0.5f, max.y),
		ImGui::GetColorU32(ImGuiCol_Separator));
}

void AssetsTab::FileBrowserRecursive(const std::filesystem::path& path)
{
	for (auto& entry : std::filesystem::directory_iterator(path))
	{
		if (!entry.is_directory())
		{
			ImGui::Text(entry.path().filename().string().c_str());
			continue;
		}
		
		bool empty = std::filesystem::is_empty(entry.path());
		if (FolderTreeNode(entry.path(), empty ? ImGuiTreeNodeFlags_Leaf : ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick))
		{
			if (!empty) FileBrowserRecursive(entry.path());
			ImGui::TreePop();
		}
	}
}

void AssetsTab::AssetFilesPanel()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 16.f, 16.f });
	ImGui::BeginChild("AssetFiles", { 0.f, 0.f }, ImGuiChildFlags_AlwaysUseWindowPadding);

	for (auto& entry : std::filesystem::directory_iterator(m_OpenFolder))
	{
		ImGui::Text(entry.path().filename().string().c_str());
	}

	ImGui::EndChild();
	ImGui::PopStyleVar();
}
