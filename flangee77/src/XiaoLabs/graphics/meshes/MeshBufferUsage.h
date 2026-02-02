#ifndef XL7_GRAPHICS_MESHES_MESHBUFFERUSAGE_H
#define XL7_GRAPHICS_MESHES_MESHBUFFERUSAGE_H

#include <CoreLabs/root.h>



namespace xl7::graphics::meshes {



/**
 * Describes the expected lifetime and update frequency of a mesh buffer (vertex or
 * index buffer).
 *
 * The selected usage guides how the backend manages memory placement, update paths,
 * and synchronization. Choosing a usage that does not match the actual update
 * behavior may result in reduced performance.
 *
 * A good read for this: John McDonald, "Don’t Throw it all Away: Efficient Buffer Management", 2012.
 * https://developer.nvidia.com/sites/default/files/akamai/gamedev/files/gdc12/Efficient_Buffer_Management_McDonald.pdf
 */
enum struct MeshBufferUsage
{
    /**
     * The buffer is initialized once and never modified afterward.
     *
     * Intended for fully static mesh data (e.g., static world geometry). Provides
     * the strongest guarantee and allows maximum optimization.
     */
    Immutable,

    /**
     * The buffer is long-lived and usually static, but its contents may be replaced
     * or rebuilt occasionally.
     *
     * This is the default choice for most mesh buffers that are not strictly
     * immutable (e.g., procedurally generated meshes, editor-driven terrain, etc.).
     * Compared to immutable, this usage sacrifices some optimization potential in
     * exchange for (limited) mutability.
     */
    Default,

    /**
     * The buffer contents are updated frequently (e.g., once per frame) while the
     * buffer object itself remains stable.
     *
     * Intended for per-frame or near-per-frame updates of mesh data (e.g., skinned
     * or animated vertex data, per-frame instance data, particles, UI, text, etc.).
     * Dynamic buffers favor update efficiency over maximal GPU locality.
     */
    Dynamic,

    /**
     * The buffer is short-lived and used for write-only, throwaway data.
     *
     * Contents are considered irrelevant after use, intended for streaming or
     * frame-local mesh data (e.g., particle spawning, UI, text, debug visuals, etc.).
     * Transient buffers are optimized for minimal synchronization and maximal write
     * throughput, not for data preservation or reuse.
     *
     * This usage expresses a streaming intent at the application level. Internally,
     * transient buffers are typically implemented using dynamic buffers with
     * sub-allocation and no-overwrite semantics.
     */
    Transient,
};



} // namespace xl7::graphics::meshes

#endif // XL7_GRAPHICS_MESHES_MESHBUFFERUSAGE_H
