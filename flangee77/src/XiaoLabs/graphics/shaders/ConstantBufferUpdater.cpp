#include "ConstantBufferUpdater.h"



namespace xl7::graphics::shaders {



    /**
     * Updates the entire constant buffer and returns true if the update succeeded,
     * false otherwise.
     *
     * The provided data must exactly match the size of the constant buffer; partial
     * updates are not supported.
     *
     * On success, the buffer is marked as "fully dirty".
     */
    bool ConstantBufferUpdater::write(const ConstantBufferWrite& write)
    {
        return ResourceUpdaterBase::write_full(write.data);
    }



} // namespace xl7::graphics::shaders
