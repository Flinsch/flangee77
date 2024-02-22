#pragma once
#ifndef TL7_SUBCASEBRANCHSWITCH_H
#define TL7_SUBCASEBRANCHSWITCH_H

#include "./Context.h"



namespace tl7 {



class SubcaseBranchSwitch
{

    // #############################################################################
    // Construction / Destruction
    // #############################################################################
public:
    /**
     * Explicit constructor.
     */
    SubcaseBranchSwitch(Context& ctx, cl7::string_view name, const cl7::char_type* file_path, unsigned line_number, signed iteration_number = -1);

    /**
     * Destructor.
     */
    ~SubcaseBranchSwitch();

private:
    /** Default constructor. */
    SubcaseBranchSwitch() = delete;
    /** Copy constructor. */
    SubcaseBranchSwitch(const SubcaseBranchSwitch&) = delete;
    /** Copy assignment operator. */
    SubcaseBranchSwitch& operator = (const SubcaseBranchSwitch&) = delete;



    // #############################################################################
    // Attributes
    // #############################################################################
private:
    /**
     * The current context within which the subcase is executed.
     */
    Context& _ctx;

    /**
     * The flag indicating whether the corresponding subcase branch has been entered
     * or not.
     */
    bool _has_been_entered;

public:
    /**
     * The meta description of where the subtest has been defined and registered.
     */
    const Meta meta;



    // #############################################################################
    // Properties
    // #############################################################################
public:
    /**
     * Returns the flag indicating whether the corresponding subcase branch has
     * been entered or not.
     */
    bool has_been_entered() const { return _has_been_entered; }



    // #############################################################################
    // Boolean Conversion Operator
    // #############################################################################
public:
    /**
     * Returns the flag indicating whether the corresponding subcase branch has
     * been entered or not.
     */
    operator bool () const { return _has_been_entered; }

}; // class SubcaseBranchSwitch



} // namespace tl7

#endif // TL7_SUBCASEBRANCHSWITCH_H
