#include "SubcaseContext.h"



namespace tl7 {



    // #############################################################################
    // Methods
    // #############################################################################

    void SubcaseContext::start_run()
    {
        _data_string.reset();

        _leaf_reached = false;
        _current_depth = 0;
        _current_stack.clear();
    }

    bool SubcaseContext::try_push(const Meta& meta)
    {
        if (_leaf_reached)
        {
            assert(_current_depth < _current_stack.size());

            if (Signature::equal_to{}(meta, _current_stack[_current_depth]))
            {
                // Re-enter (via client's control flow).
                _current_stack[_current_depth++] = meta;
                return true;
            }

            if (_next_stack.size() <= _current_depth && !_completed_nodes.contains(meta))
            {
                // Don't enter/push, but remember for next run.
                _next_stack.clear();
                _next_stack.insert(_next_stack.end(), _current_stack.begin(), _current_stack.begin() + static_cast<std::ptrdiff_t>(_current_depth));
                _next_stack.push_back(meta);
                return false;
            }
        }
        else if (_next_stack.size() <= _current_stack.size() || Signature::equal_to{}(meta, _next_stack[_current_stack.size()]))
        {
            // Push/enter "regularly".
            assert(_current_depth == _current_stack.size());
            _current_stack.push_back(meta);
            ++_current_depth;
            return true;
        }

        return false;
    }

    bool SubcaseContext::try_pop(const Meta& meta)
    {
        assert(_current_depth <= _current_stack.size());
        if (_current_depth == 0)
            return false;

        if (!Signature::equal_to{}(_current_stack[_current_depth - 1], meta))
            return false;

        _data_string.reset();

        if (!_leaf_reached)
        {
            // No leaf node reached yet?
            // Then we are the (executed) leaf node.
            _leaf_reached = true;
            _next_stack.clear();
            _completed_nodes.insert(meta);
        }
        else if (_next_stack.empty())
        {
            // No further execution reserved?
            // Then we're done at this level.
            _completed_nodes.insert(meta);
        }

        --_current_depth;
        return true;
    }

    signed SubcaseContext::get_current_iteration_number() const
    {
        const Meta* meta = get_current_meta();
        return meta ? meta->iteration_number : -1;
    }

    const Meta* SubcaseContext::get_current_meta() const
    {
        return get_meta_at(_current_depth);
    }

    const Meta* SubcaseContext::get_meta_at(size_t depth) const
    {
        assert(depth <= _current_stack.size());
        if (depth == 0 || depth > _current_stack.size())
            return nullptr;

        return &_current_stack[depth - 1];
    }



} // namespace tl7
