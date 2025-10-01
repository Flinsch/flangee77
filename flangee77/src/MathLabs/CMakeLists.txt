
target_sources(flangee77
    PRIVATE
    functions.cpp
    Matrix3x3.cpp
    Matrix3x4.cpp
    Matrix4x4.cpp
    Quaternion.cpp

    algorithm/find_cubic_roots.cpp
    algorithm/find_quadratic_roots.cpp
    algorithm/merge_nearby.cpp
)
