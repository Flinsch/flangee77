#ifndef CL7_IO_WRITABLEMEMORY_H
#define CL7_IO_WRITABLEMEMORY_H
#include "./IWritable.h"

#include <CoreLabs/byte_vector.h>
#include <CoreLabs/byte_view.h>



namespace cl7::io {



class WritableMemory
    : public IWritable
{

public:
    WritableMemory() = default;

    WritableMemory(const WritableMemory&) = default;
    WritableMemory& operator=(const WritableMemory&) = default;
    WritableMemory(WritableMemory&&) noexcept = default;
    WritableMemory& operator=(WritableMemory&&) noexcept = default;

    ~WritableMemory() noexcept override = default;

    void swap(WritableMemory& other) noexcept;

    /**
     * Special swap operation. The data is essentially "exported" and replaced with
     * any other data. The write position is adjusted if necessary.
     */
    void swap_data(cl7::byte_vector& data) noexcept;


    /**
     * Checks whether the target is writable. Always returns true.
     */
    bool is_writable() const override { return true; }

    /**
     * Returns the total writable size of the target, in bytes.
     */
    size_t get_total_writable_bytes() const override;

    /**
     * Returns the number of bytes that can be written to the target at the current
     * write position.
     */
    size_t get_writable_bytes_remaining() const override;

    /**
     * Returns the current write position within the target (in bytes, relative to
     * the beginning).
     */
    size_t get_write_position() const override;

    /**
     * (Re)sets the current write position within the target (in bytes, relative to
     * the beginning). Returns the new/current byte position.
     */
    size_t set_write_position(size_t position) override;

    /**
     * (Re)sets the current write position within the target by moving it by the
     * specified bytes relative to the beginning, end, or current position, as
     * specified. Returns the new/current byte position.
     */
    size_t seek_write(ptrdiff_t relative, SeekMode seek_mode = SeekMode::Current) override;

    /**
     * Writes data to the target (at the current position) from the specified buffer
     * and returns the number of bytes actually written.
     */
    size_t write(cl7::byte_view buffer) override;

    /**
     * Writes a single byte to the target (at the current position) and returns the
     * number of bytes actually written (i.e. 0 or 1).
     */
    size_t write(std::byte byte) override;


    /**
     * Returns a random-access view over the data written so far.
     */
    cl7::byte_view get_data() const { return _data; }



private:
    cl7::byte_vector _data;
    size_t _write_position = 0;

}; // class WritableMemory



} // namespace cl7::io

#endif // CL7_IO_WRITABLEMEMORY_H
