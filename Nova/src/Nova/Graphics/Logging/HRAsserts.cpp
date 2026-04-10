#include "novapch.h"
#include "HRAsserts.h"
#include <dxgi1_3.h>
#include <comdef.h>
#include <dxgidebug.h>

using namespace Microsoft::WRL;

bool Nova::Graphics::HRAsserts::WriteErrors(HRESULT hr)
{
	if (SUCCEEDED(hr)) return false;

	NOVA_CORE_ERROR(_com_error(hr).ErrorMessage());

	//ComPtr<IDXGIInfoQueue> infoQueue;
	//if (FAILED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(infoQueue.GetAddressOf()))))
	//{
	//	NOVA_CORE_ERROR("Failed to get Debug IDXGIInfoQueue");
	//	return;
	//}
	//
	//UINT64 messagesCount = infoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);
	//for (size_t msg = 0; msg < messagesCount; msg++)
	//{
	//	DXGI_INFO_QUEUE_MESSAGE message;
	//	SIZE_T messageLength;
	//	if (FAILED(infoQueue->GetMessage(DXGI_DEBUG_ALL, msg, &message, &messageLength)))
	//	{
	//		NOVA_CORE_ERROR("Failed to get DXGI Debug Message");
	//		continue;
	//	}
	//
	//	std::string message2(message.pDescription, message.DescriptionByteLength);
	//	NOVA_CORE_INFO(message2);
	//}

	return true;
}
