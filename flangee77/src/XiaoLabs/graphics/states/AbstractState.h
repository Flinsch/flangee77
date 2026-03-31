#ifndef XL7_GRAPHICS_STATES_ABSTRACTSTATE_H
#define XL7_GRAPHICS_STATES_ABSTRACTSTATE_H
#include "../../resources/ResourceBase.h"



namespace xl7::graphics::states {



class StateManager;



class AbstractState
    : public resources::ResourceBase<AbstractState>
{

public:
    AbstractState() = delete;

    AbstractState(const AbstractState&) = delete;
    AbstractState& operator=(const AbstractState&) = delete;
    AbstractState(AbstractState&&) = delete;
    AbstractState& operator=(AbstractState&&) = delete;



protected:

    explicit AbstractState(const CreateContext& ctx);

    ~AbstractState() override = default;



private:
    /**
     * Checks whether the given data provider complies with the specific properties
     * of the resource to (re)populate it, taking into account the current state of
     * the resource if necessary.
     */
    bool _check_data_impl(const resources::DataProvider& data_provider) override;

}; // class AbstractState



} // namespace xl7::graphics::states

#endif // XL7_GRAPHICS_STATES_ABSTRACTSTATE_H
