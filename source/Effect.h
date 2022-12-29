#pragma once

namespace dae
{
	class Texture;

	class Effect final
	{
	public:
		Effect() = delete;
		explicit Effect(ID3D11Device* pDevice, const std::wstring& assetFile);

		~Effect();
		
		Effect(Effect& rhs) = delete;
		Effect(Effect&& rhs) = delete;
		Effect& operator=(Effect& rhs) = delete;
		Effect& operator=(Effect&& rhs) = delete;

		ID3DX11Effect* GetEffect() const { return m_pEffect; };
		ID3DX11EffectTechnique* GetTechnique() const { return m_pTechnique; };

		void SetMatrices(const Matrix& wvpmatrix, const Matrix& worldMatrix, const Matrix& viewinverse);
		void SetDiffuseMap(Texture* pDiffuseTexture);
		void SetGlossmap(Texture* pGlossMap);
		void SetNormalMap(Texture* pNormalMap);
		void SetSpecularMap(Texture* pSpecularMap);

		void ToggleFilter(FilterState filter);

	private:
		ID3DX11Effect* m_pEffect{};
		ID3DX11EffectTechnique* m_pTechnique{};

		ID3D11SamplerState* m_pPoint{};
		ID3D11SamplerState* m_pLinear{};
		ID3D11SamplerState* m_pAnisotropic{};

		ID3DX11EffectSamplerVariable* m_pSamplerVariable{};

		ID3DX11EffectMatrixVariable* m_pMatWorldViewProjVariable{};
		ID3DX11EffectMatrixVariable* m_pMatWorldVariable{};
		ID3DX11EffectMatrixVariable* m_pMatInvViewVariable{};


		ID3DX11EffectShaderResourceVariable* m_pDiffuseMapVariable;
		ID3DX11EffectShaderResourceVariable* m_pGlossMapVariable;
		ID3DX11EffectShaderResourceVariable* m_pNormalMapVariable;
		ID3DX11EffectShaderResourceVariable* m_pSpecularMapVariable;

		ID3DX11Effect* LoadEffect(ID3D11Device* pDevice, const std::wstring& assetFile);
	};
}

