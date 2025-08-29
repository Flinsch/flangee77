
target_sources(flangee77-tests
    PRIVATE
    flangee77-tests.cpp
    shared.h

    CoreLabs/bits.h
    CoreLabs/Guid.h
    CoreLabs/ordered_map.h
    CoreLabs/static_vector.h
    CoreLabs/utilities.h
    CoreLabs/Version.h

    CoreLabs/strings/codec.h
    CoreLabs/strings/format.h
    CoreLabs/strings/inspect.h
    CoreLabs/strings/metrics.h
    CoreLabs/strings/transform.h

    DataLabs/Base64.h
    DataLabs/compression.h
    DataLabs/ini.h
    DataLabs/json.h
    DataLabs/syntax.h

    MathLabs/algorithm.h
    MathLabs/Angle.h
    MathLabs/distance2.h
    MathLabs/distance3.h
    MathLabs/functions.h
    MathLabs/Line2.h
    MathLabs/Line3.h
    MathLabs/LineSegment2.h
    MathLabs/LineSegment3.h
    MathLabs/Matrix2x2.h
    MathLabs/Matrix2x3.h
    MathLabs/Matrix3x3.h
    MathLabs/Matrix3x4.h
    MathLabs/Matrix4x4.h
    MathLabs/ops.h
    MathLabs/Quaternion.h
    MathLabs/Ray2.h
    MathLabs/Ray3.h
    MathLabs/shared.h
    MathLabs/Vector2.h
    MathLabs/Vector3.h
    MathLabs/Vector4.h

    ProgLabs/ArgumentBag.h

    XiaoLabs/general.h
    XiaoLabs/graphics.h
    XiaoLabs/shared.h
)
