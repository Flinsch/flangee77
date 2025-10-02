#include "Singleton.h"

#include <algorithm>



namespace cl7::creational {



    std::unique_ptr<std::vector<SingletonBase*>> SingletonBase::_stack;



    void SingletonBase::_register(SingletonBase* singleton)
    {
        assert(singleton);
        if (!_stack)
            _stack = std::make_unique<std::vector<SingletonBase*>>();

        assert(_find(singleton) == _stack->end());

        _stack->push_back(singleton);
    }

    void SingletonBase::_unregister(SingletonBase* singleton, bool keep_stack)
    {
        assert(singleton);
        assert(_stack);

        auto it = _find(singleton);
        assert(it != _stack->end());
        _stack->erase(it);

        if (_stack->empty() && !keep_stack)
            _stack.reset();
    }

    void SingletonBase::_reregister(SingletonBase* singleton)
    {
        assert(singleton);
        assert(_stack);

        _unregister(singleton, true);
        _register(singleton);
    }

    void SingletonBase::_before_destroy(SingletonBase* singleton)
    {
        assert(singleton);

        singleton->_before_destroy();
    }

    std::vector<SingletonBase*>::iterator SingletonBase::_find(SingletonBase* singleton)
    {
        assert(singleton);
        assert(_stack);

        return std::find(_stack->begin(), _stack->end(), singleton);
    }

    void SingletonManager::destroy_all()
    {
        while (SingletonBase::_stack && !SingletonBase::_stack->empty())
        {
            SingletonBase* singleton = SingletonBase::_stack->back();
            singleton->_invoke_destroy();

            assert(!SingletonBase::_stack || SingletonBase::_find(singleton) == SingletonBase::_stack->end());
        }

        assert(!SingletonBase::_stack);
        SingletonBase::_stack.reset(); // Actually not necessary.
    }



} // namespace cl7::creational
