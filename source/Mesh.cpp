#include "pch.h"
#include "Mesh.h"
#include "Effect.h"
#include "EffectShaded.h"
#include "EffectTransparent.h"
#include "Texture.h"

namespace dae
{
	dae::Mesh::Mesh(ID3D11Device* pDevice, const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, EffectType type)
	{
		switch (type)
		{
		case EffectType::Shaded:
			m_pEffect = new EffectShaded{ pDevice, L"./Resources/PosTex3D.fx" };
			break;
		case EffectType::Transparent:
			m_pEffect = new EffectTransparent{ pDevice, L"./Resources/PosTransparent3D.fx" };
			break;
		default:
			break;
		}

		//Create Vertex Layout
		static constexpr uint32_t numElements{ 4 };
		D3D11_INPUT_ELEMENT_DESC vertexDesc[numElements]{};

		vertexDesc[0].SemanticName = "POSITION";
		vertexDesc[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		vertexDesc[0].AlignedByteOffset = 0;
		vertexDesc[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;

		vertexDesc[1].SemanticName = "NORMAL";
		vertexDesc[1].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		vertexDesc[1].AlignedByteOffset = 12;
		vertexDesc[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;

		vertexDesc[2].SemanticName = "TANGENT";
		vertexDesc[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		vertexDesc[2].AlignedByteOffset = 24;
		vertexDesc[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;

		vertexDesc[3].SemanticName = "TEXCOORD";
		vertexDesc[3].Format = DXGI_FORMAT_R32G32_FLOAT;
		vertexDesc[3].AlignedByteOffset = 36;
		vertexDesc[3].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		
		//Create Input Layout
		D3DX11_PASS_DESC passDesc{};
		m_pEffect->GetTechnique()->GetPassByIndex(0)->GetDesc(&passDesc);

		HRESULT result{ pDevice->CreateInputLayout
		(
			vertexDesc,
			numElements,
			passDesc.pIAInputSignature,
			passDesc.IAInputSignatureSize,
			&m_pInputLayout
		) };
		if (FAILED(result))return;

		//Create vertex buffer
		D3D11_BUFFER_DESC bd{};
		bd.Usage = D3D11_USAGE_IMMUTABLE;
		bd.ByteWidth = sizeof(Vertex) * static_cast<uint32_t>(vertices.size());
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA initData{};
		initData.pSysMem = vertices.data();

		result = pDevice->CreateBuffer(&bd, &initData, &m_pVertexBuffer);
		if (FAILED(result)) return;

		//Create index buffer
		m_NumIndices = static_cast<uint32_t>(indices.size());
		bd.Usage = D3D11_USAGE_IMMUTABLE;
		bd.ByteWidth = sizeof(uint32_t) * m_NumIndices;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		initData.pSysMem = indices.data();

		result = pDevice->CreateBuffer(&bd, &initData, &m_pIndexBuffer);
		if (FAILED(result)) return;
	}

	dae::Mesh::~Mesh()
	{
		delete m_pEffect;

		if (m_pIndexBuffer)m_pIndexBuffer->Release();
		if (m_pVertexBuffer)m_pVertexBuffer->Release();
		if (m_pInputLayout)m_pInputLayout->Release();
	}

	void Mesh::Render(ID3D11DeviceContext* pDeviceContext) const
	{
		//1. Set Primitive Topology
		pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//2. Set Input Layout
		pDeviceContext->IASetInputLayout(m_pInputLayout);

		//3. Set vertex buffer
		constexpr UINT stride = sizeof(Vertex);
		constexpr UINT offset = 0;
		pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

		//4. Set IndexBuffer
		pDeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

		//5. Draw
		D3DX11_TECHNIQUE_DESC techDesc{};
		m_pEffect->GetTechnique()->GetDesc(&techDesc);
		for (UINT p = 0; p < techDesc.Passes; p++)
		{
			m_pEffect->GetTechnique()->GetPassByIndex(p)->Apply(0, pDeviceContext);
			pDeviceContext->DrawIndexed(m_NumIndices, 0, 0);
		}
	}

	void Mesh::SetMatrices(const Matrix& wvpmatrix, const Matrix& worldMatrix, const Matrix& viewinverse)
	{
		m_pEffect->SetMatrices(wvpmatrix, worldMatrix, viewinverse);
	}

	void Mesh::SetDiffuseMap(Texture* pDiffuseTexture)
	{
		m_pEffect->SetDiffuseMap(pDiffuseTexture);
	}

	void Mesh::SetGlossmap(Texture* pGlossMap)
	{
		m_pEffect->SetGlossmap(pGlossMap);
	}

	void Mesh::SetNormalMap(Texture* pNormalMap)
	{
		m_pEffect->SetNormalMap(pNormalMap);
	}

	void Mesh::SetSpecularMap(Texture* pSpecularMap)
	{
		m_pEffect->SetSpecularMap(pSpecularMap);
	}

	void Mesh::ToggleFilter(FilterState filter)
	{
		m_pEffect->ToggleFilter(filter);
	}
}