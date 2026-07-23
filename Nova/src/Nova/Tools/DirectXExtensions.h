#pragma once

using namespace Microsoft::WRL;

namespace Nova
{
	DXGI_FORMAT GetTextureFormat(ID3D11Resource* texture);
}