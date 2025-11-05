
target_sources(flangee77
    PRIVATE
    Config.cpp
    ConfigProvider.cpp
    DriverResolver.cpp
    MainWindow.cpp

    graphics/ChannelOrder.cpp
    graphics/Color.cpp
    graphics/GraphicsSystem.cpp
    graphics/PixelBitKit.cpp
    graphics/PixelFormat.cpp
    graphics/RenderingContext.cpp
    graphics/RenderingDevice.cpp

    graphics/images/Image.cpp
    graphics/images/ImageConverter.cpp
    graphics/images/ImageHandler.cpp
    graphics/images/ImageProcessor.cpp
    graphics/images/ImageResizer.cpp
    graphics/images/ImageStack.cpp
    graphics/images/PngImageHandler.cpp
    graphics/images/TargaImageHandler.cpp

    graphics/meshes/IndexBuffer.cpp
    graphics/meshes/MeshBuffer.cpp
    graphics/meshes/MeshManager.cpp
    graphics/meshes/MeshUtil.cpp
    graphics/meshes/VertexBuffer.cpp
    graphics/meshes/VertexLayout.cpp

    graphics/shaders/CodeDataProvider.cpp
    graphics/shaders/ConstantBuffer.cpp
    graphics/shaders/ConstantBufferLayout.cpp
    graphics/shaders/ConstantBufferMapping.cpp
    graphics/shaders/PixelShader.cpp
    graphics/shaders/Shader.cpp
    graphics/shaders/ShaderCode.cpp
    graphics/shaders/ShaderManager.cpp
    graphics/shaders/VertexShader.cpp

    graphics/states/AbstractState.cpp
    graphics/states/BlendState.cpp
    graphics/states/DepthStencilState.cpp
    graphics/states/RasterizerState.cpp
    graphics/states/SamplerState.cpp
    graphics/states/StateManager.cpp

    graphics/surfaces/DepthStencilSurface.cpp
    graphics/surfaces/FramebufferSurface.cpp
    graphics/surfaces/RenderTargetSurface.cpp
    graphics/surfaces/Surface.cpp
    graphics/surfaces/SurfaceManager.cpp

    graphics/textures/Cubemap.cpp
    graphics/textures/ImageDataProvider.cpp
    graphics/textures/Texture.cpp
    graphics/textures/Texture2D.cpp
    graphics/textures/Texture2DArray.cpp
    graphics/textures/Texture3D.cpp
    graphics/textures/TextureManager.cpp

    resources/DataProvider.cpp
    resources/DefaultDataProvider.cpp
    resources/FakeDataProvider.cpp
    resources/Resource.cpp
    resources/ResourceManager.cpp
)

target_sources(flangee77
    PRIVATE
    graphics/impl/shared/errors.cpp

    graphics/impl/shared/meshes/ComposedVertexLayout.cpp
    graphics/impl/shared/meshes/VertexBufferBinding.cpp

    graphics/impl/shared/shaders/D3DShaderCompiler.cpp
)

target_sources(flangee77
    PRIVATE
    graphics/impl/direct3d9/errors.cpp
    graphics/impl/direct3d9/GraphicsSystemImpl.cpp
    graphics/impl/direct3d9/mappings.cpp
    graphics/impl/direct3d9/RenderingContextImpl.cpp
    graphics/impl/direct3d9/RenderingDeviceImpl.cpp
    graphics/impl/direct3d9/ResourceFactoryImpl.cpp

    graphics/impl/direct3d9/meshes/IndexBufferImpl.cpp
    graphics/impl/direct3d9/meshes/VertexBufferImpl.cpp

    graphics/impl/direct3d9/shaders/ConstantBufferImpl.cpp
    graphics/impl/direct3d9/shaders/D3DShaderReflection.cpp
    graphics/impl/direct3d9/shaders/PixelShaderImpl.cpp
    graphics/impl/direct3d9/shaders/VertexShaderImpl.cpp

    graphics/impl/direct3d9/states/BlendStateImpl.cpp
    graphics/impl/direct3d9/states/DepthStencilStateImpl.cpp
    graphics/impl/direct3d9/states/RasterizerStateImpl.cpp
    graphics/impl/direct3d9/states/SamplerStateImpl.cpp

    #graphics/impl/direct3d9/surfaces/

    graphics/impl/direct3d9/textures/Texture2DImpl.cpp
)

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_sources(flangee77
        PRIVATE
        graphics/impl/direct3d11/errors.cpp
        graphics/impl/direct3d11/GraphicsSystemImpl.cpp
        graphics/impl/direct3d11/mappings.cpp
        graphics/impl/direct3d11/RenderingContextImpl.cpp
        graphics/impl/direct3d11/RenderingDeviceImpl.cpp
        graphics/impl/direct3d11/ResourceFactoryImpl.cpp

        graphics/impl/direct3d11/meshes/IndexBufferImpl.cpp
        graphics/impl/direct3d11/meshes/VertexBufferImpl.cpp

        graphics/impl/direct3d11/shaders/ConstantBufferImpl.cpp
        graphics/impl/direct3d11/shaders/D3DConstantBufferWrapper.cpp
        graphics/impl/direct3d11/shaders/D3DShaderReflection.cpp
        graphics/impl/direct3d11/shaders/PixelShaderImpl.cpp
        graphics/impl/direct3d11/shaders/VertexShaderImpl.cpp

        graphics/impl/direct3d11/states/BlendStateImpl.cpp
        graphics/impl/direct3d11/states/DepthStencilStateImpl.cpp
        graphics/impl/direct3d11/states/RasterizerStateImpl.cpp
        graphics/impl/direct3d11/states/SamplerStateImpl.cpp

        #graphics/impl/direct3d11/surfaces/

        graphics/impl/direct3d11/textures/Texture2DImpl.cpp
    )
endif()
