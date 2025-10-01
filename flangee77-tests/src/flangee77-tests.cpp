
#include <TestLabs/TestSuite.h>
#include <CoreLabs/logging.h>

#include "./CoreLabs/bits.h"
#include "./CoreLabs/ordered_map.h"
#include "./CoreLabs/static_vector.h"
#include "./CoreLabs/utilities.h"
#include "./CoreLabs/Guid.h"
#include "./CoreLabs/Version.h"

#include "./CoreLabs/text/codec.h"
#include "./CoreLabs/text/inspect.h"
#include "./CoreLabs/text/transform.h"
#include "./CoreLabs/text/format.h"
#include "./CoreLabs/text/metrics.h"

#include "./DataLabs/Base64.h"
#include "./DataLabs/compression.h"
#include "./DataLabs/syntax.h"
#include "./DataLabs/json.h"
#include "./DataLabs/ini.h"

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
#include "./MathLabs/algorithm.h"
#include "./MathLabs/distance2.h"
#include "./MathLabs/distance3.h"

#include "./ProgLabs/ArgumentBag.h"

#include "./XiaoLabs/general.h"
#include "./XiaoLabs/graphics.h"



int main()
{
    cl7::logging::StandardLogger::instance().clear_log_handlers();

    tl7::TestSuite::Settings settings;
    settings.max_check_fail_count = 0;
    tl7::TestSuite::instance().set_settings(settings);

    tl7::TestSuite::instance().run_tests();

    return 0;
}
