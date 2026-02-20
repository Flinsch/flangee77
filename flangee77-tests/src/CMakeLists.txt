
target_sources(flangee77-tests
    PRIVATE
    flangee77-tests.cpp


    AlgoLabs/math.cpp
    AlgoLabs/packing/Rect.cpp
    AlgoLabs/packing/AbstractRectPacker.cpp
    AlgoLabs/packing/NaiveRowPacker.cpp
    AlgoLabs/packing/MaxRectsPacker.cpp
    AlgoLabs/packing/ShelfPacker.cpp
    AlgoLabs/packing/SkylinePacker.cpp
    AlgoLabs/packing/GuillotinePacker.cpp


    CoreLabs/bits.cpp
    CoreLabs/bytes.cpp
    CoreLabs/memory.cpp
    CoreLabs/ordered_map.cpp
    CoreLabs/static_vector.cpp
    CoreLabs/utilities.cpp
    CoreLabs/Guid.cpp
    CoreLabs/Version.cpp

    CoreLabs/io/ByteReader.cpp
    CoreLabs/io/ByteWriter.cpp
    CoreLabs/io/AsciiReader.cpp
    CoreLabs/io/AsciiWriter.cpp
    CoreLabs/io/Utf8Reader.cpp
    CoreLabs/io/Utf8Writer.cpp
    CoreLabs/text/codec.cpp
    CoreLabs/text/inspect.cpp
    CoreLabs/text/transform.cpp
    CoreLabs/text/format.cpp
    CoreLabs/text/metrics.cpp


    DataLabs/Base64.cpp
    DataLabs/Buffer1d.cpp
    DataLabs/Buffer2d.cpp
    DataLabs/Buffer3d.cpp
    DataLabs/compression.cpp
    DataLabs/syntax.cpp
    DataLabs/json.cpp
    #DataLabs/yaml.cpp
    #DataLabs/toml.cpp
    DataLabs/ini.cpp
    DataLabs/xml.cpp


    MathLabs/functions.cpp
    MathLabs/ops.cpp
    MathLabs/Angle.cpp
    MathLabs/Vector2.cpp
    MathLabs/Vector3.cpp
    MathLabs/Vector4.cpp
    MathLabs/Matrix2x2.cpp
    MathLabs/Matrix2x3.cpp
    MathLabs/Matrix3x3.cpp
    MathLabs/Matrix3x4.cpp
    MathLabs/Matrix4x4.cpp
    MathLabs/Quaternion.cpp
    MathLabs/Line2.cpp
    MathLabs/Line3.cpp
    MathLabs/LineSegment2.cpp
    MathLabs/LineSegment3.cpp
    MathLabs/Ray2.cpp
    MathLabs/Ray3.cpp
    MathLabs/QuadraticBezier2.cpp
    MathLabs/QuadraticBezier3.cpp
    MathLabs/CubicBezier2.cpp
    MathLabs/CubicBezier3.cpp
    MathLabs/distance2.cpp
    MathLabs/distance3.cpp


    ProgLabs/ArgumentBag.cpp


    XiaoLabs/general.cpp

    XiaoLabs/graphics/Color.cpp
    XiaoLabs/graphics/PixelLayout.cpp
    XiaoLabs/graphics/images/ImageProcessor.cpp
    XiaoLabs/graphics/images/ImageConverter.cpp
    XiaoLabs/graphics/images/ImageResizer.cpp
    XiaoLabs/graphics/images/NetpbmImageReader.cpp
    XiaoLabs/graphics/images/NetpbmImageWriter.cpp
    XiaoLabs/graphics/images/PngImageReader.cpp
    XiaoLabs/graphics/shaders/reflection.cpp
)
