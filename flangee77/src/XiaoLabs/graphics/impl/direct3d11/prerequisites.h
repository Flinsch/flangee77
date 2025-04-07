#ifndef XL7_GRAPHICS_IMPL_D3D11_PREREQUISITES_H
#define XL7_GRAPHICS_IMPL_D3D11_PREREQUISITES_H

#include <CoreLabs/root.h>

#include <dxgi1_5.h>
#include <d3d11_1.h>

#include <wrl/client.h>
namespace wrl = Microsoft::WRL;



// The following typedefs assume that we are compiling for Windows 7 (or later),
// more specifically, for the Platform Update for Windows 7, which enables improved
// features and performance on Windows 7 SP1 (and Windows Server 2008 R2 SP1).
//
// Update: We want some interfaces that are only available with Windows 10 or later.
// And besides, Steam will soon/now only support Windows 10 and higher anyway. So
// let's go the "right" way right away.
//
// The interface versions are primarily determined by the operating system, not any
// hardware capabilities. However, hardware capabilities may affect the performance
// or level of support for certain features. Just for info at this point.
static_assert(_WIN32_WINNT >= _WIN32_WINNT_WIN10);

// D3D11 interfaces (targeting Direct3D 11.1+)
using ID3D11DeviceN             = ID3D11Device1;        // Has Create*1 methods, required for feature levels > 11.
using ID3D11DeviceContextN      = ID3D11DeviceContext1; // Adds more fine-grained context control.

// DXGI interfaces (modern Windows 10 stack)
using IDXGIFactoryN             = IDXGIFactory5;        // For checking tearing support, adapter memory segments, etc.
using IDXGIAdapterN             = IDXGIAdapter3;        // Adds memory usage info: QueryVideoMemoryInfo().
using IDXGIDeviceN              = IDXGIDevice3;         // For Trim() and extended resource residency control.
using IDXGISwapChainN           = IDXGISwapChain3;      // Flip model, Present1(), and GetCurrentBackBufferIndex().

using DXGI_SWAP_CHAIN_DESCn     = DXGI_SWAP_CHAIN_DESC1;// Needed for CreateSwapChainForHwnd() etc.



#endif // XL7_GRAPHICS_IMPL_D3D11_PREREQUISITES_H
