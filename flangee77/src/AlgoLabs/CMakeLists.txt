
target_sources(flangee77
    PRIVATE

    math/find_cubic_roots.cpp
    math/find_quadratic_roots.cpp
    math/merge_nearby.cpp

    packing/AbstractFreeRectPacker.cpp
    packing/AbstractRectPacker.cpp
    packing/GuillotinePacker.cpp
    packing/MaxRectsPacker.cpp
    packing/NaiveRowPacker.cpp
    packing/Rect.cpp
    packing/ShelfPacker.cpp
    packing/SkylinePacker.cpp
)
