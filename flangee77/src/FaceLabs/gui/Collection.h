#ifndef FL7_GUI_COLLECTION_H
#define FL7_GUI_COLLECTION_H
#include "./Face.h"

#include <concepts>
#include <memory>
#include <vector>



namespace fl7::gui {



/**
 * A face that owns children: adds child ownership and the recursive hit-test
 * traversal on top of face's leaf behavior. Children are exposed read-only for
 * external tree traversal (e.g., by a renderer), but a collection itself knows
 * nothing about drawing/rendering. Child ownership stops here deliberately. See
 * `Container` (arbitrary, publicly addable children) and `Compound` (fixed,
 * internally managed children) for the two ways of actually exposing this.
 */
class Collection
    : public Face
{

public:
    Collection() = default;

    Collection(const Collection&) = delete;
    Collection& operator=(const Collection&) = delete;
    Collection(Collection&&) = delete;
    Collection& operator=(Collection&&) = delete;

    ~Collection() override = default;



    // #############################################################################
    // Properties
    // #############################################################################

    /**
     * Returns this collection's children, in insertion order. Exposed publicly
     * (read-only) so external code (e.g., a renderer traversing the face tree) can
     * walk into a collection without needing any special access.
     */
    const std::vector<std::unique_ptr<Face>>& get_children() const { return _children; }



protected:

    // #############################################################################
    // Methods
    // #############################################################################

    /**
     * Constructs a new child Face of the specified type in place, adds it,
     * and returns a reference to it. Protected: whether/how this is exposed
     * publicly is up to the concrete subclass (see Container/Compound).
     */
    template <class TFace, class... Args>
        requires(std::derived_from<TFace, Face>)
    TFace& add_child(Args&&... args)
    {
        auto child = std::make_unique<TFace>(std::forward<Args>(args)...);
        TFace& ref = *child;
        _add_child(std::move(child));
        return ref;
    }



    // #############################################################################
    // Face Implementations
    // #############################################################################

    /**
     * Finds the topmost face containing the given absolute (screen) point:
     * children first (most-recently-added/topmost first), falling back to
     * this Collection itself if none of them are hit.
     */
    Face* _find_hit_face(ml7::Vector2f screen_point, ml7::Vector2f parent_absolute_position) override;



private:

    // #############################################################################
    // Helpers
    // #############################################################################

    /**
     * Adds the specified face as a child of this Collection.
     */
    void _add_child(std::unique_ptr<Face> child);



    // #############################################################################
    // Attributes
    // #############################################################################

    std::vector<std::unique_ptr<Face>> _children;

}; // class Collection



} // namespace fl7::gui

#endif // FL7_GUI_COLLECTION_H
