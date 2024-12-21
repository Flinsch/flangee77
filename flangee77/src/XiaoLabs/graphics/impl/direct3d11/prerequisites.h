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
static_assert(_WIN32_WINNT >= _WIN32_WINNT_WIN7);

using ID3D11DeviceN             = ID3D11Device1;
using ID3D11DeviceContextN      = ID3D11DeviceContext1;

using IDXGIFactoryN             = IDXGIFactory2;
//using IDXGIAdapterN             = IDXGIAdapter2;
using IDXGIAdapterN             = IDXGIAdapter;
using IDXGIDeviceN              = IDXGIDevice2;
//using IDXGISwapChainN           = IDXGISwapChain1;
using IDXGISwapChainN           = IDXGISwapChain;

//using DXGI_SWAP_CHAIN_DESCn     = DXGI_SWAP_CHAIN_DESC1;
using DXGI_SWAP_CHAIN_DESCn     = DXGI_SWAP_CHAIN_DESC;



#endif // XL7_GRAPHICS_IMPL_D3D11_PREREQUISITES_H
