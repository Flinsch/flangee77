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



    friend SingletonManager;

}; // class SingletonBase



class SingletonManager
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



template <class TSelf>
class Singleton
    : public SingletonBase
{

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
    static std::unique_ptr<TSelf> _obj;



    // #############################################################################
    // Singleton Management
    // #############################################################################
private:
    static TSelf& _create()
    {
        if ( !_obj )
        {
            _obj.reset( new TSelf() );
            _register( _obj.get() );
        }
        return *_obj;
    }

public:
    /**
     * Destroys the actual singleton object instance.
     */
    static void destroy()
    {
        if ( !_obj )
            return;
        _before_destroy( _obj.get() );
        _unregister( _obj.get() );
        _obj.reset();
    }

private:
    void _invoke_destroy() final { assert( _obj ); destroy(); }

protected:
    void _reregister()  { assert( _obj ); _reregister( _obj.get() ); }



    // #############################################################################
    // Singleton Access
    // #############################################################################
public:
    /**
     * Returns the actual singleton object instance.
     */
    static TSelf& obj()
    {
        if ( !_obj )
            return _create();
        return *_obj;
    }

}; // class Singleton



    template <class TSelf>
    std::unique_ptr<TSelf> Singleton<TSelf>::_obj;



} // namespace creational
} // namespace cl7

#endif // CL7_CREATIONAL_SINGLETON_H
