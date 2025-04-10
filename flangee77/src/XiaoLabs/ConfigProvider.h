#ifndef XL7_CONFIGPROVIDER_H
#define XL7_CONFIGPROVIDER_H
#include <CoreLabs/creational/Singleton.h>

#include "./Config.h"



namespace xl7 {



class ConfigProvider
    : public cl7::creational::Singleton<ConfigProvider>
{

public:

    // #############################################################################
    // Properties
    // #############################################################################

    /**
     * Returns the "X" pre-config structure.
     */
    const Config& get_config() const { return _config; }

    /**
     * Sets the "X" pre-config structure.
     */
    void set_config(const Config& config) { _config = config; }



private:

    // #############################################################################
    // Attributes
    // #############################################################################

    /**
     * The "X" pre-config structure.
     */
    Config _config;

}; // class ConfigProvider



    inline ConfigProvider& config_provider() { return ConfigProvider::instance(); }



} // namespace xl7

#endif // XL7_CONFIGPROVIDER_H
