#include "errors.h"

#include <CoreLabs/sstream.h>

#include <d3d9.h>



namespace xl7 {
namespace graphics {
namespace impl {
namespace direct3d9 {
namespace errors {



    cl7::string d3d9_result(long error_code)
    {
        const char* message = nullptr;

        switch ( error_code )
        {
        case D3D_OK:
            message = "D3D_OK: No error occurred.";
            break;

        case D3DERR_WRONGTEXTUREFORMAT:
            message = "D3DERR_WRONGTEXTUREFORMAT: The pixel format of the texture surface is not valid.";
            break;
        case D3DERR_UNSUPPORTEDCOLOROPERATION:
            message = "D3DERR_UNSUPPORTEDCOLOROPERATION: The device does not support a specified texture-blending operation for color values.";
            break;
        case D3DERR_UNSUPPORTEDCOLORARG:
            message = "D3DERR_UNSUPPORTEDCOLORARG: The device does not support a specified texture-blending argument for color values.";
            break;
        case D3DERR_UNSUPPORTEDALPHAOPERATION:
            message = "D3DERR_UNSUPPORTEDALPHAOPERATION: The device does not support a specified texture-blending operation for the alpha channel.";
            break;
        case D3DERR_UNSUPPORTEDALPHAARG:
            message = "D3DERR_UNSUPPORTEDALPHAARG: The device does not support a specified texture-blending argument for the alpha channel.";
            break;
        case D3DERR_TOOMANYOPERATIONS:
            message = "D3DERR_TOOMANYOPERATIONS: The application is requesting more texture-filtering operations than the device supports.";
            break;
        case D3DERR_CONFLICTINGTEXTUREFILTER:
            message = "D3DERR_CONFLICTINGTEXTUREFILTER: The current texture filters cannot be used together.";
            break;
        case D3DERR_UNSUPPORTEDFACTORVALUE:
            message = "D3DERR_UNSUPPORTEDFACTORVALUE: The device does not support the specified texture factor value.";
            break;
        case D3DERR_CONFLICTINGRENDERSTATE:
            message = "D3DERR_CONFLICTINGRENDERSTATE: The currently set render states cannot be used together.";
            break;
        case D3DERR_UNSUPPORTEDTEXTUREFILTER:
            message = "D3DERR_UNSUPPORTEDTEXTUREFILTER: The device does not support the specified texture filter.";
            break;
        case D3DERR_CONFLICTINGTEXTUREPALETTE:
            message = "D3DERR_CONFLICTINGTEXTUREPALETTE: The current textures cannot be used simultaneously.";
            break;
        case D3DERR_DRIVERINTERNALERROR:
            message = "D3DERR_DRIVERINTERNALERROR: Internal driver error. Applications should destroy and recreate the device when receiving this error.";
            break;

        case D3DERR_NOTFOUND:
            message = "D3DERR_NOTFOUND: The requested item was not found.";
            break;
        case D3DERR_MOREDATA:
            message = "D3DERR_MOREDATA: There is more data available than the specified buffer size can hold.";
            break;
        case D3DERR_DEVICELOST:
            message = "D3DERR_DEVICELOST: The device has been lost but cannot be reset at this time. Therefore, rendering is not possible. A Direct3D device object other than the one that returned this code caused the hardware adapter to be reset by the OS. Delete all video memory objects (surfaces, textures, state blocks) and call Reset() to return the device to a default state. If the application continues rendering without a reset, the rendering calls will succeed.";
            break;
        case D3DERR_DEVICENOTRESET:
            message = "D3DERR_DEVICENOTRESET: The device has been lost but can be reset at this time.";
            break;
        case D3DERR_NOTAVAILABLE:
            message = "D3DERR_NOTAVAILABLE: This device does not support the queried technique.";
            break;
        case D3DERR_OUTOFVIDEOMEMORY:
            message = "D3DERR_OUTOFVIDEOMEMORY: Direct3D does not have enough display memory to perform the operation. The device is using more resources in a single scene than can fit simultaneously into video memory. Present, PresentEx, or CheckDeviceState can return this error. Recovery is similar to D3DERR_DEVICEHUNG, though the application may want to reduce its per-frame memory usage as well to avoid having the error recur.";
            break;
        case D3DERR_INVALIDDEVICE:
            message = "D3DERR_INVALIDDEVICE: The requested device type is not valid.";
            break;
        case D3DERR_INVALIDCALL:
            message = "D3DERR_INVALIDCALL: The method call is invalid. For example, a method's parameter may not be a valid pointer.";
            break;
        case D3DERR_DRIVERINVALIDCALL:
            message = "D3DERR_DRIVERINVALIDCALL: Not used.";
            break;
        case D3DERR_WASSTILLDRAWING:
            message = "D3DERR_WASSTILLDRAWING: The previous blit operation that is transferring information to or from this surface is incomplete.";
            break;
        case D3DOK_NOAUTOGEN:
            message = "D3DOK_NOAUTOGEN: The autogeneration of mipmaps is not supported for this format. This means that resource creation will succeed but the mipmap levels will not be automatically generated.";
            break;

#if !defined(D3D_DISABLE_9EX)
        case D3DERR_DEVICEREMOVED:
            message = "D3DERR_DEVICEREMOVED: The hardware adapter has been removed. Application must destroy the device, do enumeration of adapters and create another Direct3D device. If application continues rendering without calling Reset, the rendering calls will succeed.";
            break;
        case S_NOT_RESIDENT:
            message = "S_NOT_RESIDENT: At least one allocation that comprises the resources is on disk.";
            break;
        case S_RESIDENT_IN_SHARED_MEMORY:
            message = "S_RESIDENT_IN_SHARED_MEMORY: No allocations that comprise the resources are on disk. However, at least one allocation is not in GPU-accessible memory.";
            break;
        case S_PRESENT_MODE_CHANGED:
            message = "S_PRESENT_MODE_CHANGED: The desktop display mode has been changed. The application can continue rendering, but there might be color conversion/stretching. Pick a back buffer format similar to the current display mode, and call Reset to recreate the swap chains. The device will leave this state after a Reset is called.";
            break;
        case S_PRESENT_OCCLUDED:
            message = "S_PRESENT_OCCLUDED: The presentation area is occluded. Occlusion means that the presentation window is minimized or another device entered the fullscreen mode on the same monitor as the presentation window and the presentation window is completely on that monitor. Occlusion will not occur if the client area is covered by another Window. Occluded applications can continue rendering and all calls will succeed, but the occluded presentation window will not be updated. Preferably the application should stop rendering to the presentation window using the device and keep calling CheckDeviceState until S_OK or S_PRESENT_MODE_CHANGED returns.";
            break;
        case D3DERR_DEVICEHUNG:
            message = "D3DERR_DEVICEHUNG: The device that returned this code caused the hardware adapter to be reset by the OS. Most applications should destroy the device and quit. Applications that must continue should destroy all video memory objects (surfaces, textures, state blocks etc) and call Reset() to put the device in a default state. If the application then continues rendering in the same way, the device will return to this state.";
            break;
        case D3DERR_UNSUPPORTEDOVERLAY:
            message = "D3DERR_UNSUPPORTEDOVERLAY: The device does not support overlay for the specified size or display mode.";
            break;
        case D3DERR_UNSUPPORTEDOVERLAYFORMAT:
            message = "D3DERR_UNSUPPORTEDOVERLAYFORMAT: The device does not support overlay for the specified surface format.";
            break;
        case D3DERR_CANNOTPROTECTCONTENT:
            message = "D3DERR_CANNOTPROTECTCONTENT: The specified content cannot be protected.";
            break;
        case D3DERR_UNSUPPORTEDCRYPTO:
            message = "D3DERR_UNSUPPORTEDCRYPTO: The specified cryptographic algorithm is not supported.";
            break;
        case D3DERR_PRESENT_STATISTICS_DISJOINT:
            message = "D3DERR_PRESENT_STATISTICS_DISJOINT: The present statistics have no orderly sequence.";
            break;
#endif
        } // switch error code

        if ( !message )
            return cl7::errors::system_result( static_cast<unsigned long>( error_code ) );

        cl7::osstream oss;
        oss << std::hex << TEXT("0x") << error_code;
        oss << TEXT(" ");
        oss << message;

        return oss.str();
    }

    cl7::string d3d9_result(long error_code, const cl7::string_view& context)
    {
        return cl7::errors::with_context( d3d9_result( error_code ), context );
    }



} // namespace errors
} // namespace direct3d9
} // namespace impl
} // namespace graphics
} // namespace xl7
