#pragma once
#ifndef XL7_GRAPHICS_IMPL_D3D11_PREREQUISITES_H
#define XL7_GRAPHICS_IMPL_D3D11_PREREQUISITES_H

#include <CoreLabs/root.h>

#include <d3d11_1.h>

#include <wrl/client.h>
namespace wrl = Microsoft::WRL;



// The following typedefs assume that we are compiling for Windows 7 (or later),
// more specifically, for the Platform Update for Windows 7, which enables improved
// features and performance on Windows 7 SP1 (and Windows Server 2008 R2 SP1). The
// interface versions are primarily determined by the operating system, not any
// hardware capabilities. However, hardware capabilities may affect the performance
// or level of support for certain features. Just for info at this point.
static_assert( _WIN32_WINNT >= _WIN32_WINNT_WIN7 );

typedef ID3D11Device1           ID3D11DeviceN;
typedef ID3D11DeviceContext1    ID3D11DeviceContextN;

typedef IDXGIFactory2           IDXGIFactoryN;
//typedef IDXGIAdapter2           IDXGIAdapterN;
typedef IDXGIAdapter            IDXGIAdapterN;
typedef IDXGIDevice2            IDXGIDeviceN;
//typedef IDXGISwapChain1         IDXGISwapChainN;
typedef IDXGISwapChain          IDXGISwapChainN;

//typedef DXGI_SWAP_CHAIN_DESC1   DXGI_SWAP_CHAIN_DESCn;
typedef DXGI_SWAP_CHAIN_DESC    DXGI_SWAP_CHAIN_DESCn;



#endif // XL7_GRAPHICS_IMPL_D3D11_PREREQUISITES_H
