#ifndef DL7_CONFIG_BINDING_H
#define DL7_CONFIG_BINDING_H

#include "./types.h"

#include <type_traits>



namespace dl7::config {



/**
 * One field of a structure: the key it goes by in a configuration file, and the
 * member it is.
 */
template <typename Tstruct, typename Tmember>
struct Field
{
    cl7::u8string_view key;
    Tmember Tstruct::* member;
};

/**
 * Describes one field of a structure, for a `Binding`'s field tuple.
 */
template <typename Tstruct, typename Tmember>
constexpr Field<Tstruct, Tmember> field(cl7::u8string_view key, Tmember Tstruct::* member)
{
    return {key, member};
}



/**
 * Describes how a structure maps onto a configuration value. Specialize it for the
 * structure in question and give it a `fields` tuple built from `field()`:
 *
 *     namespace dl7::config {
 *         template <>
 *         struct Binding<xl7::Config::Video>
 *         {
 *             static constexpr auto fields = std::make_tuple(
 *                 field(u8"driver_name", &xl7::Config::Video::driver_name),
 *                 field(u8"vsync_enabled", &xl7::Config::Video::vsync_enabled));
 *         };
 *     }
 *
 * Deliberately non-intrusive: the structure itself needs to know nothing about any
 * of this, which keeps it free of the dependency and works for structures that
 * cannot be changed at all. The same description drives reading and writing, so
 * there is only ever one list of fields to keep up to date.
 */
template <typename Tstruct>
struct Binding;



/**
 * One value of an enumeration: the name it goes by in a configuration file, and the
 * value it is.
 */
template <typename Tenum>
struct EnumValue
{
    cl7::u8string_view name;
    Tenum value;
};

/**
 * Describes one value of an enumeration, for an `EnumNames`' name tuple.
 */
template <typename Tenum>
constexpr EnumValue<Tenum> enum_value(cl7::u8string_view name, Tenum value)
{
    return {name, value};
}



/**
 * Maps an enumeration's values onto the names they go by in a configuration file.
 * Specialize it and give it a `names` tuple built from `enum_value()`. Without such
 * a specialization an enumeration cannot be bound at all, there being no sensible
 * way to guess what its values should be called.
 */
template <typename Tenum>
struct EnumNames;



/**
 * Satisfied by structures that have a `Binding` specialization, i.e., the ones that
 * can be read and written as a mapping.
 */
template <typename T>
concept bound_struct = requires { Binding<T>::fields; };

/**
 * Satisfied by enumerations that have an `EnumNames` specialization.
 */
template <typename T>
concept named_enum = std::is_enum_v<T> && requires { EnumNames<T>::names; };



} // namespace dl7::config

#endif // DL7_CONFIG_BINDING_H
