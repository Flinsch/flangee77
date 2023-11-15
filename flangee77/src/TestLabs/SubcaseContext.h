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
    std::shared_ptr<cl7::string> _data_string;

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
    std::shared_ptr<cl7::string> get_data_string() const { return _data_string; }
    void set_data_string(const std::shared_ptr<cl7::string>& data_string) { _data_string = data_string; }
    void reset_data_string() { _data_string.reset(); }


    // #############################################################################
    // Methods
    // #############################################################################
public:
    void start_run();
    bool try_push(const Meta& meta);
    bool try_pop(const Meta& meta);
    bool has_next() const { return !_next_stack.empty(); }
    size_t get_current_depth() const { return _current_depth; }
    signed get_current_iteration_number() const;

}; // class SubcaseContext



} // namespace tl7

#endif // TL7_SUBCASECONTEXT_H
