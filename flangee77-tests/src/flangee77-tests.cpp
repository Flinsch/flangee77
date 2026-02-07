
#include <TestLabs/TestSuite.h>
#include <CoreLabs/logging.h>

#include "./AlgoLabs/math.h"
#include "./AlgoLabs/packing/Rect.h"
#include "./AlgoLabs/packing/AbstractRectPacker.h"
#include "./AlgoLabs/packing/NaiveRowPacker.h"
#include "./AlgoLabs/packing/MaxRectsPacker.h"
#include "./AlgoLabs/packing/ShelfPacker.h"
#include "./AlgoLabs/packing/SkylinePacker.h"
#include "./AlgoLabs/packing/GuillotinePacker.h"

#include "./CoreLabs/bits.h"
#include "./CoreLabs/bytes.h"
#include "./CoreLabs/memory.h"
#include "./CoreLabs/ordered_map.h"
#include "./CoreLabs/static_vector.h"
#include "./CoreLabs/utilities.h"
#include "./CoreLabs/Guid.h"
#include "./CoreLabs/Version.h"

#include "./CoreLabs/io/ByteReader.h"
#include "./CoreLabs/io/ByteWriter.h"
#include "./CoreLabs/io/AsciiReader.h"
#include "./CoreLabs/io/AsciiWriter.h"
#include "./CoreLabs/io/Utf8Reader.h"
#include "./CoreLabs/io/Utf8Writer.h"
#include "./CoreLabs/text/codec.h"
#include "./CoreLabs/text/inspect.h"
#include "./CoreLabs/text/transform.h"
#include "./CoreLabs/text/format.h"
#include "./CoreLabs/text/metrics.h"

#include "./DataLabs/Base64.h"
#include "./DataLabs/Buffer1d.h"
#include "./DataLabs/Buffer2d.h"
#include "./DataLabs/Buffer3d.h"
#include "./DataLabs/compression.h"
#include "./DataLabs/syntax.h"
#include "./DataLabs/json.h"
//#include "./DataLabs/yaml.h"
//#include "./DataLabs/toml.h"
#include "./DataLabs/ini.h"
#include "./DataLabs/xml.h"

#include "./MathLabs/functions.h"
#include "./MathLabs/ops.h"
#include "./MathLabs/Angle.h"
#include "./MathLabs/Vector2.h"
#include "./MathLabs/Vector3.h"
#include "./MathLabs/Vector4.h"
#include "./MathLabs/Matrix2x2.h"
#include "./MathLabs/Matrix2x3.h"
#include "./MathLabs/Matrix3x3.h"
#include "./MathLabs/Matrix3x4.h"
#include "./MathLabs/Matrix4x4.h"
#include "./MathLabs/Quaternion.h"
#include "./MathLabs/Line2.h"
#include "./MathLabs/Line3.h"
#include "./MathLabs/LineSegment2.h"
#include "./MathLabs/LineSegment3.h"
#include "./MathLabs/Ray2.h"
#include "./MathLabs/Ray3.h"
#include "./MathLabs/QuadraticBezier2.h"
#include "./MathLabs/QuadraticBezier3.h"
#include "./MathLabs/CubicBezier2.h"
#include "./MathLabs/CubicBezier3.h"
#include "./MathLabs/distance2.h"
#include "./MathLabs/distance3.h"

#include "./ProgLabs/ArgumentBag.h"

#include "./XiaoLabs/general.h"

#include "./XiaoLabs/graphics/Color.h"
#include "./XiaoLabs/graphics/PixelLayout.h"
#include "./XiaoLabs/graphics/images/ImageProcessor.h"
#include "./XiaoLabs/graphics/images/ImageConverter.h"
#include "./XiaoLabs/graphics/images/ImageResizer.h"
#include "./XiaoLabs/graphics/shaders/reflection.h"



int main()
{
    cl7::logging::StandardLogger::instance().clear_log_handlers();

    tl7::TestSuite::Settings settings;
    settings.max_check_fail_count = 0;
    tl7::TestSuite::instance().set_settings(settings);

    tl7::TestSuite::instance().run_tests();

    return 0;
}
