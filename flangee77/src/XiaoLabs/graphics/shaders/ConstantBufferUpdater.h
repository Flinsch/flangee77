#ifndef XL7_GRAPHICS_SHADERS_CONSTANTBUFFERUPDATER_H
#define XL7_GRAPHICS_SHADERS_CONSTANTBUFFERUPDATER_H
#include "../../resources/ResourceUpdaterBase.h"

#include "./ConstantBufferDesc.h"
#include "./ConstantBufferWrite.h"
#include "./DirtyFlag.h"



namespace xl7::graphics::shaders {



class ConstantBufferUpdater
    : public resources::ResourceUpdaterBase<ConstantBufferDesc, DirtyFlag>
{
public:
    using ResourceUpdaterBase::ResourceUpdaterBase;



  /**
     * Updates the entire constant buffer and returns true if the update succeeded,
     * false otherwise.
     *
     * The provided data must exactly match the size of the constant buffer; partial
     * updates are not supported.
     *
     * On success, the buffer is marked as "fully dirty".
     */
    bool write(const ConstantBufferWrite& write);

};



} // namespace xl7::graphics::shaders

#endif // XL7_GRAPHICS_SHADERS_CONSTANTBUFFERUPDATER_H
