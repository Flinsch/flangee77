#include "Singleton.h"

#include <algorithm>



namespace cl7 {
namespace creational {



    std::unique_ptr<std::vector<SingletonBase*>> SingletonBase::_stack;



    std::vector<SingletonBase*>::iterator SingletonBase::_find(SingletonBase* singleton)
    {
        assert( singleton );
        assert( _stack );

        return std::find( _stack->begin(), _stack->end(), singleton );
    }

    void SingletonBase::_register(SingletonBase* singleton)
    {
        assert( singleton );
        if ( !_stack )
            _stack.reset( new std::vector<SingletonBase*>() );

        assert( _find( singleton ) == _stack->end() );

        _stack->push_back( singleton );
    }

    void SingletonBase::_unregister(SingletonBase* singleton)
    {
        assert( singleton );
        assert( _stack );

        auto it = _find( singleton );
        assert( it != _stack->end() );
        _stack->erase( it );
    }

    void SingletonBase::_reregister(SingletonBase* singleton)
    {
        assert( singleton );
        assert( _stack );

        _unregister( singleton );
        _register( singleton );
    }

    void SingletonBase::_before_destroy(SingletonBase* singleton)
    {
        assert( singleton );

        singleton->_before_destroy();
    }

    void SingletonManager::destroy_all()
    {
        if ( !_stack )
            return;

        while ( !_stack->empty() )
        {
            SingletonBase* singleton = _stack->back();
            singleton->_invoke_destroy();

            assert( _find( singleton ) == _stack->end() );
        }

        _stack.reset();
    }



} // namespace creational
} // namespace cl7
