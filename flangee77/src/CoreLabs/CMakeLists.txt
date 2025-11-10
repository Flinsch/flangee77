
target_sources(flangee77
    PRIVATE
    bits.cpp
    Guid.cpp
    logging.cpp
    memory.cpp
    root.cpp
    sstream.cpp
    string.cpp
    Version.cpp

    creational/Singleton.cpp

    io/ByteReader.cpp
    io/ByteWriter.cpp
    io/File.cpp
    io/ReadableMemory.cpp
    io/Utf8Reader.cpp
    io/Utf8Writer.cpp
    io/WritableMemory.cpp

    logging/AbstractLogHandler.cpp
    logging/CoutLogHandler.cpp
    logging/FileLogHandler.cpp
    logging/HtmlLogHandler.cpp
    logging/LogContext.cpp
    logging/Logger.cpp
    logging/StandardLogger.cpp

    platform/CPUID.cpp
    platform/errors.cpp
    platform/filesystem.cpp
    platform/memory.cpp
    platform/user.cpp

    platform/detail/linux/memory.cpp

    platform/detail/windows/errors.cpp
    platform/detail/windows/filesystem.cpp
    platform/detail/windows/memory.cpp
    platform/detail/windows/user.cpp

    profiling/Profiler.cpp
    profiling/Registry.cpp
    profiling/SampleHandler.cpp
    profiling/StandardRegistry.cpp
    profiling/Stopwatch.cpp

    text/codec.cpp
    text/format.cpp
    text/inspect.cpp
    text/metrics.cpp
    text/transform.cpp

    text/codec/AsciiCodec.cpp
    text/codec/codepoint.cpp
    text/codec/ErrorHandler.cpp
    text/codec/LoggingErrorHandler.cpp
    text/codec/Utf8Codec.cpp
    text/codec/Utf16Codec.cpp
    text/codec/Utf32Codec.cpp

    system/datetime.cpp
)
