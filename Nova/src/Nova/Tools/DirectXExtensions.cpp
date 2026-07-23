#include "novapch.h"
#include "DirectXExtensions.h"
#include "Nova/Graphics/Logging/HRAsserts.h"

DXGI_FORMAT Nova::GetTextureFormat(ID3D11Resource* texture)
{
	ComPtr<ID3D11Texture2D> texture2D;
	NOVA_HRASSERT(texture->QueryInterface(IID_PPV_ARGS(&texture2D)), "As Texture2D");

	D3D11_TEXTURE2D_DESC texture2DDesc;
	texture2D->GetDesc(&texture2DDesc);

	return texture2DDesc.Format;
}
