
target_sources(flangee77
    PRIVATE
    bits.cpp
    errors.cpp
    filesystem.cpp
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

    logging/AbstractLogHandler.cpp
    logging/CoutLogHandler.cpp
    logging/FileLogHandler.cpp
    logging/HtmlLogHandler.cpp
    logging/LogContext.cpp
    logging/Logger.cpp
    logging/StandardLogger.cpp

    profiling/Profiler.cpp
    profiling/Registry.cpp
    profiling/SampleHandler.cpp
    profiling/StandardRegistry.cpp
    profiling/Stopwatch.cpp

    strings/codec.cpp
    strings/format.cpp
    strings/inspect.cpp
    strings/metrics.cpp
    strings/transform.cpp

    strings/codec/AsciiCodec.cpp
    strings/codec/codepoint.cpp
    strings/codec/ErrorHandler.cpp
    strings/codec/LoggingErrorHandler.cpp
    strings/codec/Utf8Codec.cpp
    strings/codec/Utf16Codec.cpp
    strings/codec/Utf32Codec.cpp

    system/CPUID.cpp
    system/datetime.cpp
    system/MemoryStatus.cpp
    system/user.cpp
)
