#pragma once
#ifndef XL7_RESOURCEMANAGER_H
#define XL7_RESOURCEMANAGER_H

#include <CoreLabs/string.h>



namespace xl7 {



class ResourceManager
{

    // #############################################################################
    // Construction / Destruction
    // #############################################################################
protected:
public:
    /**
     * Default constructor.
     */
    ResourceManager() = default;

    /**
     * Destructor.
     */
    virtual ~ResourceManager() = default;

private:
    /** Copy constructor. */
    ResourceManager(const ResourceManager&) = delete;
    /** Copy assignment operator. */
    ResourceManager& operator = (const ResourceManager&) = delete;

}; // class ResourceManager



} // namespace xl7

#endif // XL7_RESOURCEMANAGER_H
