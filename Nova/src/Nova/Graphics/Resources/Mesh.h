#pragma once
#include "Nova/Graphics/RenderData/VertexFormats.h"
#include "Nova/Graphics/Bindables/Mesh/VertexBuffer.h"
#include "Nova/Graphics/Bindables/Mesh/IndexBuffer.h"
#include "Nova/Tools/dynamic_array.hpp"
#include <span>

namespace Nova::Graphics
{
	struct SubMesh
	{
		SubMesh() = default;
		SubMesh(uint32_t IndexOffset, uint32_t IndexLength, uint32_t VertexOffset, uint32_t MaterialIndex)
			: IndexOffset(IndexOffset), IndexLength(IndexLength), VertexOffset(VertexOffset), MaterialIndex(MaterialIndex) { }

		uint32_t IndexOffset = 0;
		uint32_t IndexLength = 0;
		uint32_t VertexOffset = 0;
		uint32_t MaterialIndex = 0;
	};

	struct MeshData
	{
		MeshData() = default;
		template<VertexFormat TVertex>
		MeshData(const std::vector<TVertex>& vertices, const std::vector<uint16_t>& indices, const std::vector<SubMesh>& subMeshes);

		const void* VertexData = nullptr;
		uint32_t VertexDataSize = 0;
		uint32_t VertexElementSize = 0;
		std::vector<Vertex> Vertices;
		std::vector<uint16_t> Indices;
		std::vector<SubMesh> SubMeshes;
	};

	template<VertexFormat TVertex>
	MeshData::MeshData(const std::vector<TVertex>& vertices, const std::vector<uint16_t>& indices, const std::vector<SubMesh>& subMeshes)
	{
		VertexElementSize = sizeof(TVertex);
		VertexData = std::data(vertices);
		VertexDataSize = VertexElementSize * uint32_t(vertices.size());

		Vertices = vertices;
		Indices = indices;
		SubMeshes = subMeshes;
	}

	class Mesh
	{
	public:
		Mesh();
		Mesh(const MeshData& meshData, bool readWriteable = false);

		void Bind() const;
		uint32_t GetIndexLength() const;
		std::span<const SubMesh> GetSubMeshes() const;

		bool ReadWriteable() const;
		MeshData* GetReadWriteData() const;
		void Apply(bool readWriteable);

	private:
		VertexBuffer m_VertexBuffer;
		IndexBuffer m_IndexBuffer;
		dynamic_array<SubMesh> m_SubMeshes;

		// TODO: m_Bounds
		std::unique_ptr<MeshData> m_MeshData;
	};
}