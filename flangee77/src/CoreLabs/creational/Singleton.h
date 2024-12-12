#pragma once
#ifndef CL7_CREATIONAL_SINGLETON_H
#define CL7_CREATIONAL_SINGLETON_H

#include <CoreLabs/root.h>

#include <vector>



namespace cl7::creational {



class SingletonBase // NOLINT(cppcoreguidelines-virtual-class-destructor)
{
    friend class SingletonManager;

public:
    SingletonBase(const SingletonBase&) = delete;
    SingletonBase& operator = (const SingletonBase&) = delete;
    SingletonBase(SingletonBase&&) = delete;
    SingletonBase& operator = (SingletonBase&&) = delete;



protected:
    SingletonBase() = default;
    virtual ~SingletonBase() = default;


    static void _register(SingletonBase* singleton);
    static void _unregister(SingletonBase* singleton, bool keep_stack = false);
    static void _reregister(SingletonBase* singleton);
    static void _before_destroy(SingletonBase* singleton);



private:
    /**
     * This is called just before the singleton object is destroyed.
     * If the singleton object has cleanup functions that are virtual and therefore
     * cannot be called from the destructor, then they should be called here.
     */
    virtual void _before_destroy() {}

    virtual void _invoke_destroy() = 0;


    static std::vector<SingletonBase*>::iterator _find(SingletonBase* singleton);


    static std::unique_ptr<std::vector<SingletonBase*>> _stack;

}; // class SingletonBase



class SingletonManager final
    : public SingletonBase
{

public:
    SingletonManager() = delete;


    /**
     * Destroys all singleton objects in the reverse order in which they were
     * registered.
     */
    static void destroy_all();

}; // class SingletonManager



template <class TSingleton>
class Singleton
    : public SingletonBase
{

public:
    Singleton(const Singleton&) = delete;
    Singleton& operator = (const Singleton&) = delete;
    Singleton(Singleton&&) = delete;
    Singleton& operator = (Singleton&&) = delete;


    /**
     * Returns the actual singleton object instance.
     * If it does not exist yet, it will be created.
     */
    static TSingleton& instance()
    {
        if (!_instance)
            return _create();
        return *_instance;
    }

    /**
     * Destroys the actual singleton object instance.
     * The instance is recreated the next time it is accessed.
     */
    static void destroy()
    {
        if (!_instance)
            return;
        _before_destroy(_instance);
        _destroy();
    }



protected:
    Singleton() = default;
    ~Singleton() override = default;


    void _reregister()  { assert(_instance); _reregister(_instance); }



private:
    void _invoke_destroy() final { assert(_instance); destroy(); }


    static TSingleton* factory_func()
    {
        return new TSingleton();
    }

    static TSingleton& _create()
    {
        if (!_instance)
        {
            static_assert(std::derived_from<TSingleton, Singleton<TSingleton>>);
            _instance = TSingleton::factory_func();
            _register(_instance);
        }
        return *_instance;
    }

    static void _destroy()
    {
        _unregister(_instance);
        delete _instance;
        _instance = nullptr;
    }


    static TSingleton* _instance;

}; // class Singleton



    template <class TSingleton>
    TSingleton* Singleton<TSingleton>::_instance = nullptr;



} // namespace cl7::creational

#endif // CL7_CREATIONAL_SINGLETON_H
