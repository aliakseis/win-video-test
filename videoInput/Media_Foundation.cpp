
#include <mfapi.h>
#include <mfplay.h>


#include "Media_Foundation.h"
#include "videoDevices.h"
#include "DebugPrintOut.h"
#include "Common.h"

#include <dshow.h>

static HRESULT EnumerateDevices(REFGUID category, IEnumMoniker **ppEnum)
{
    // Create the System Device Enumerator.
    ICreateDevEnum *pDevEnum;
    HRESULT hr = CoCreateInstance(CLSID_SystemDeviceEnum, nullptr,
        CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pDevEnum));

    if (SUCCEEDED(hr))
    {
        // Create an enumerator for the category.
        hr = pDevEnum->CreateClassEnumerator(category, ppEnum, 0);
        if (hr == S_FALSE)
        {
            hr = VFW_E_NOT_FOUND;  // The category is empty. Treat as an error.
        }
        pDevEnum->Release();
    }
    return hr;
}


Media_Foundation::Media_Foundation(void)
{
	HRESULT hr = MFStartup(MF_VERSION);

	if(!SUCCEEDED(hr))
	{
		DebugPrintOut *DPO = &DebugPrintOut::getInstance();

		DPO->printOut(L"MEDIA FOUNDATION: It cannot be created!!!\n");
	}
}

Media_Foundation::~Media_Foundation(void)
{
	HRESULT hr = MFShutdown();  
	
	if(!SUCCEEDED(hr))
	{
		DebugPrintOut *DPO = &DebugPrintOut::getInstance();

		DPO->printOut(L"MEDIA FOUNDATION: Resources cannot be released\n");
	}
}

bool Media_Foundation::buildListOfDevices()
{	
    /*
	HRESULT hr = S_OK;
	
	IMFAttributes *pAttributes = NULL;

    CoInitialize(NULL);
	
    hr = MFCreateAttributes(&pAttributes, 1);
   
	if (SUCCEEDED(hr))
    {
        hr = pAttributes->SetGUID(
            MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE,
            MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE_VIDCAP_GUID
            );
    }	
	
	if (SUCCEEDED(hr))
    {
		videoDevices *vDs = &videoDevices::getInstance();

		hr = vDs->initDevices(pAttributes);
    }	
	else
	{
		DebugPrintOut *DPO = &DebugPrintOut::getInstance();

		DPO->printOut(L"MEDIA FOUNDATION: The access to the video cameras denied\n");
	
	}

	
	SafeReleaseAllCount(&pAttributes);

	return (SUCCEEDED(hr));
    */

    CoInitializeEx(nullptr, COINIT_MULTITHREADED);

    IEnumMoniker *pEnum;

    HRESULT hr = EnumerateDevices(CLSID_VideoInputDeviceCategory, &pEnum);
    if (SUCCEEDED(hr))
    {
        //DisplayDeviceInformation(pEnum);

        hr = videoDevices::getInstance().initDevices(pEnum);

        pEnum->Release();
    }


    return (SUCCEEDED(hr));
}


Media_Foundation& Media_Foundation::getInstance() 
{
	static Media_Foundation instance;

	return instance;
}