#pragma once
#ifndef TL7_SUBCASEBRANCHSWITCH_H
#define TL7_SUBCASEBRANCHSWITCH_H

#include "./Context.h"



namespace tl7 {



class SubcaseBranchSwitch
{

public:

    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    SubcaseBranchSwitch() = delete;

    SubcaseBranchSwitch(Context& ctx, cl7::string_view name, const cl7::char_type* file_path, unsigned line_number, signed iteration_number = -1);

    SubcaseBranchSwitch(const SubcaseBranchSwitch&) = delete;
    SubcaseBranchSwitch& operator=(const SubcaseBranchSwitch&) = delete;
    SubcaseBranchSwitch(SubcaseBranchSwitch&&) = delete;
    SubcaseBranchSwitch& operator=(SubcaseBranchSwitch&&) = delete;

    ~SubcaseBranchSwitch();



    // #############################################################################
    // Methods
    // #############################################################################

    /**
     * Returns the meta description of where the subtest has been defined and registered.
     */
    const Meta& meta() const { return _meta; }

    /**
     * Returns the flag indicating whether the corresponding subcase branch has
     * been entered or not.
     */
    bool has_been_entered() const { return _has_been_entered; }



    // #############################################################################
    // Boolean Conversion Operator
    // #############################################################################

    /**
     * Returns the flag indicating whether the corresponding subcase branch has
     * been entered or not.
     */
    operator bool () const { return _has_been_entered; }



private:

    // #############################################################################
    // Attributes
    // #############################################################################

    /**
     * The meta description of where the subtest has been defined and registered.
     */
    const Meta _meta;

    /**
     * The current context within which the subcase is executed.
     */
    Context& _ctx;

    /**
     * The flag indicating whether the corresponding subcase branch has been entered
     * or not.
     */
    bool _has_been_entered;

}; // class SubcaseBranchSwitch



} // namespace tl7

#endif // TL7_SUBCASEBRANCHSWITCH_H
