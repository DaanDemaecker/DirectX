#pragma once

namespace dae
{
	class Effect;

	struct Vertex
	{
		Vector3 position;
		ColorRGB color;
	};

	class Mesh final
	{
	public:
		Mesh() = delete;
		Mesh(ID3D11Device* pDevice, const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indicess);

		~Mesh();

		Mesh& operator=(Mesh& rhs) = delete;
		Mesh& operator=(Mesh&& rhs) = delete;
		Mesh(Mesh& rhs) = delete;
		Mesh(Mesh&& rhs) = delete;

		void Render(ID3D11DeviceContext* pDeviceContext) const;

	private:
		Effect* m_pEffect;

		ID3D11InputLayout* m_pInputLayout{};

		ID3D11Buffer* m_pVertexBuffer{};

		uint32_t m_NumIndices{};
		ID3D11Buffer* m_pIndexBuffer{};
	};
}

