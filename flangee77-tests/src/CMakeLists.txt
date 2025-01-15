
target_sources(flangee77-tests
    PRIVATE
    flangee77-tests.cpp
    shared.h

    CoreLabs/bits.h
    CoreLabs/Guid.h
    CoreLabs/ordered_map.h
    CoreLabs/strings.h
    CoreLabs/Version.h

    DataLabs/Base64.h
    DataLabs/compression.h
    DataLabs/json.h
    DataLabs/syntax.h

    MathLabs/Angle.h
    MathLabs/functions.h
    MathLabs/Matrix2x2.h
    MathLabs/Matrix2x3.h
    MathLabs/Matrix3x3.h
    MathLabs/Matrix3x4.h
    MathLabs/Matrix4x4.h
    MathLabs/Quaternion.h
    MathLabs/shared.h
    MathLabs/Vector2.h
    MathLabs/Vector3.h
    MathLabs/Vector4.h

    ProgLabs/ArgumentBag.h

    XiaoLabs/graphics.h
    XiaoLabs/shared.h
)
