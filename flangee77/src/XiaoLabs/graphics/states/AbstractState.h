#pragma once
#ifndef XL7_GRAPHICS_STATES_ABSTRACTSTATE_H
#define XL7_GRAPHICS_STATES_ABSTRACTSTATE_H
#include "../../resources/Resource.h"



namespace xl7 {
namespace graphics {
namespace states {



class StateManager;



class AbstractState
    : public resources::Resource
{

    // #############################################################################
    // Construction / Destruction
    // #############################################################################
protected:
    /**
     * Explicit constructor.
     */
    template <class TDesc>
    AbstractState(const CreateParams<TDesc>& params)
        : resources::Resource( params )
    {
    }

    /**
     * Destructor.
     */
    virtual ~AbstractState() = default;

private:
    /** Default constructor. */
    AbstractState() = delete;
    /** Copy constructor. */
    AbstractState(const AbstractState&) = delete;
    /** Copy assignment operator. */
    AbstractState& operator = (const AbstractState&) = delete;



    // #############################################################################
    // Attributes
    // #############################################################################
protected:

private:



    // #############################################################################
    // Properties
    // #############################################################################
public:

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

}; // class AbstractState



} // namespace states
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_STATES_ABSTRACTSTATE_H
