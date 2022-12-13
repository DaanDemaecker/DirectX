#pragma once

namespace dae
{
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

	private:
		ID3DX11Effect* m_pEffect{};
		ID3DX11EffectTechnique* m_pTechnique{};

		ID3DX11Effect* LoadEffect(ID3D11Device* pDevice, const std::wstring& assetFile);
	};
}

