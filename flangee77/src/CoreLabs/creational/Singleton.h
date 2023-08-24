#pragma once
#ifndef CL7_CREATIONAL_SINGLETON_H
#define CL7_CREATIONAL_SINGLETON_H

#include <CoreLabs/root.h>

#include <vector>



namespace cl7 {
namespace creational {



class SingletonManager;



class SingletonBase
{
    friend SingletonManager;

    // #############################################################################
    // Construction / Destruction
    // #############################################################################
protected:
    /** Default constructor. */
    SingletonBase() = default;

    /** Destructor. */
    virtual ~SingletonBase() = default;

private:
    /** Copy constructor. */
    SingletonBase(const SingletonBase&) = delete;
    /** Copy assignment operator. */
    SingletonBase& operator = (const SingletonBase&) = delete;



    // #############################################################################
    // Singleton Management
    // #############################################################################
private:
    static std::unique_ptr<std::vector<SingletonBase*>> _stack;

private:
    static std::vector<SingletonBase*>::iterator _find(SingletonBase* singleton);

protected:
    static void _register(SingletonBase* singleton);
    static void _unregister(SingletonBase* singleton);
    static void _reregister(SingletonBase* singleton);
    static void _before_destroy(SingletonBase* singleton);

private:
    virtual void _invoke_destroy() = 0;



    // #############################################################################
    // Prototypes
    // #############################################################################
private:
    /**
     * This is called just before the singleton object is destroyed.
     * If the singleton object has cleanup functions that are virtual and therefore
     * cannot be called from the destructor, then they should be called here.
     */
    virtual void _before_destroy() {}

}; // class SingletonBase



class SingletonManager final
    : public SingletonBase
{

    // #############################################################################
    // Construction / Destruction
    // #############################################################################
private:
    /** Default constructor. */
    SingletonManager() = delete;
    /** Copy constructor. */
    SingletonManager(const SingletonManager&) = delete;
    /** Copy assignment operator. */
    SingletonManager& operator = (const SingletonManager&) = delete;



    // #############################################################################
    // Singleton Management
    // #############################################################################
public:
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

    // #############################################################################
    // Default Factory Function
    // #############################################################################
private:
    static TSingleton* factory_func()
    {
        return new TSingleton();
    }



    // #############################################################################
    // Construction / Destruction
    // #############################################################################
protected:
    /** Default constructor. */
    Singleton() = default;

    /** Destructor. */
    virtual ~Singleton() = default;

private:
    /** Copy constructor. */
    Singleton(const Singleton&) = delete;
    /** Copy assignment operator. */
    Singleton& operator = (const Singleton&) = delete;



    // #############################################################################
    // Singleton Object
    // #############################################################################
private:
    static TSingleton* _instance;



    // #############################################################################
    // Singleton Management
    // #############################################################################
private:
    static TSingleton& _create()
    {
        if ( !_instance )
        {
            static_assert( std::derived_from<TSingleton, Singleton<TSingleton>> );
            _instance = TSingleton::factory_func();
            _register( _instance );
        }
        return *_instance;
    }

public:
    /**
     * Destroys the actual singleton object instance.
     * The instance is recreated the next time it is accessed.
     */
    static void destroy()
    {
        if ( !_instance )
            return;
        _before_destroy( _instance );
        _unregister( _instance );
        delete _instance;
        _instance = nullptr;
    }

private:
    void _invoke_destroy() final { assert( _instance ); destroy(); }

protected:
    void _reregister()  { assert( _instance ); _reregister( _instance ); }



    // #############################################################################
    // Singleton Access
    // #############################################################################
public:
    /**
     * Returns the actual singleton object instance.
     * If it does not exist yet, it will be created.
     */
    static TSingleton& instance()
    {
        if ( !_instance )
            return _create();
        return *_instance;
    }

}; // class Singleton



    template <class TSingleton>
    TSingleton* Singleton<TSingleton>::_instance = nullptr;



} // namespace creational
} // namespace cl7

#endif // CL7_CREATIONAL_SINGLETON_H
