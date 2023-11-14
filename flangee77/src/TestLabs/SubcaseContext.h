#pragma once
#ifndef TL7_SUBCASECONTEXT_H
#define TL7_SUBCASECONTEXT_H

#include "./Meta.h"
#include "./Signature.h"

#include <vector>
#include <unordered_set>



namespace tl7 {



class SubcaseContext
{

    // #############################################################################
    // Attributes
    // #############################################################################
private:
    bool _leaf_reached = false;
    size_t _current_depth = 0;
    std::vector<Signature> _current_stack;
    std::vector<Signature> _next_stack;
    std::unordered_set<Signature, Signature::hash, Signature::equal_to> _completed_nodes;



    // #############################################################################
    // Methods
    // #############################################################################
public:
    void start_run();
    size_t get_current_depth() const { return _current_depth; }
    bool try_push(const Meta& meta);
    bool try_pop(const Meta& meta);
    bool has_next() const { return !_next_stack.empty(); }

}; // class SubcaseContext



} // namespace tl7

#endif // TL7_SUBCASECONTEXT_H
