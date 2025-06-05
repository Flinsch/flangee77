#ifndef CL7_IO_READABLEMEMORY_H
#define CL7_IO_READABLEMEMORY_H
#include "./IReadable.h"

#include <CoreLabs/byte_vector.h>
#include <CoreLabs/byte_view.h>



namespace cl7::io {



class ReadableMemory
    : public IReadable
{

public:
    ReadableMemory() = default;
    ReadableMemory(cl7::byte_vector&& source_data);
    ReadableMemory(cl7::byte_view source_data);

    ReadableMemory(const ReadableMemory&) = default;
    ReadableMemory& operator=(const ReadableMemory&) = default;
    ReadableMemory(ReadableMemory&&) noexcept = default;
    ReadableMemory& operator=(ReadableMemory&&) noexcept = default;

    ~ReadableMemory() noexcept override = default;

    void swap(ReadableMemory& other) noexcept;


    /**
     * Checks whether the memory is readable. Always returns true.
     */
    bool is_readable() const override { return true; }

    /**
     * Checks whether EOF has been reached, so that there is no more data to read at
     * the current byte position.
     */
    bool is_eof() const override;

    /**
     * Returns the total readable size of the source data, in bytes.
     */
    size_t get_total_readable_bytes() const override;

    /**
     * Returns the number of bytes that can be read from the source data at the
     * current read position.
     */
    size_t get_readable_bytes_remaining() const override;

    /**
     * Returns the current read position within the source data (in bytes, relative
     * to the beginning).
     */
    size_t get_read_position() const override;

    /**
     * (Re)sets the current read position within the source data (in bytes, relative
     * to the beginning). Returns the new/current byte position.
     */
    size_t set_read_position(size_t position) override;

    /**
     * (Re)sets the current read position within the source data by moving it by the
     * specified bytes relative to the beginning, end, or current position, as
     * specified. Returns the new/current byte position.
     */
    size_t seek_read(ptrdiff_t relative, SeekMode seek_mode = SeekMode::Current) override;

    /**
     * Reads data from the source data (at the current position) into the specified
     * buffer and returns the number of bytes actually read.
     */
    size_t read(cl7::byte_span buffer) override;

    /**
     * Reads a single byte from the source data (at the current position) and
     * returns the number of bytes actually read (i.e. 0 or 1).
     */
    size_t read(std::byte& byte) override;

    /**
     * "Peeks" a single byte from the source data (at the current position) without
     * extracting it. Returns the number of bytes that would have been extracted if
     * actually read (i.e. 0 or 1).
     */
    size_t peek(std::byte& byte) override;



private:
    cl7::byte_vector _data;
    size_t _read_position = 0;

}; // class ReadableMemory



} // namespace cl7::io

#endif // CL7_IO_READABLEMEMORY_H
