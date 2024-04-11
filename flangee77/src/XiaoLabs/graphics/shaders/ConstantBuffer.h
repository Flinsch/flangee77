#pragma once
#ifndef XL7_GRAPHICS_SHADERS_CONSTANTBUFFER_H
#define XL7_GRAPHICS_SHADERS_CONSTANTBUFFER_H
#include "../../resources/Resource.h"

#include "./ConstantDataProvider.h"



namespace xl7 {
namespace graphics {
namespace shaders {



class ShaderManager;



class ConstantBuffer
    : public resources::Resource
{

public:
    struct Desc
    {
        /** Identifies how the constant buffer is expected to be updated (frequency of update is a key factor). */
        resources::ResourceUsage usage;

        
    };



    // #############################################################################
    // Construction / Destruction
    // #############################################################################
protected:
    /**
     * Explicit constructor.
     */
    ConstantBuffer(const CreateParams<Desc>& params);

    /**
     * Destructor.
     */
    virtual ~ConstantBuffer() = default;

private:
    /** Default constructor. */
    ConstantBuffer() = delete;
    /** Copy constructor. */
    ConstantBuffer(const ConstantBuffer&) = delete;
    /** Copy assignment operator. */
    ConstantBuffer& operator = (const ConstantBuffer&) = delete;



    // #############################################################################
    // Attributes
    // #############################################################################
protected:
    /**
     * The descriptor of the constant buffer.
     */
    const Desc _desc;

private:



    // #############################################################################
    // Properties
    // #############################################################################
public:
    /**
     * Returns the descriptor of the constant buffer.
     */
    const Desc& get_desc() const { return _desc; }

public:



    // #############################################################################
    // Resource Implementations
    // #############################################################################
private:
    /**
     * Checks whether the given data provider complies with the specific properties
     * of the resource to (re)populate it, taking into account the current state of
     * the resource if necessary.
     */
    virtual bool _check_data_impl(const resources::DataProvider& data_provider) override;

public:
    /**
     * Returns the specific type of the resource, as a "human-friendly" string.
     */
    virtual cl7::string_view get_type_string() const override { return TEXT("constant buffer"); }

}; // class ConstantBuffer



} // namespace shaders
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_SHADERS_CONSTANTBUFFER_H
