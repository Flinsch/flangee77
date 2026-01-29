#ifndef CL7_IO_FILE_H
#define CL7_IO_FILE_H
#include "./IReadWritable.h"
#include "./IResizable.h"

#include "./OpenMode.h"

#include <CoreLabs/string.h>

#include <fstream>



namespace cl7::io {



class File
    : public IReadWritable, public IResizable
{

public:
    File() = default;
    File(cl7::u8string_view path, OpenMode open_mode = OpenMode::Read | OpenMode::Write);

    File(const File&) = delete;
    File& operator=(const File&) = delete;
    File(File&&) = delete;
    File& operator=(File&&) = delete;

    ~File() override = default;


    /**
     * Opens the specified file for reading and/or writing.
     */
    bool open(cl7::u8string_view path, OpenMode open_mode = OpenMode::Read | OpenMode::Write);

    /**
     * Closes the file (if opened before).
     */
    void close();


    /**
     * Checks whether the file is readable, i.e. whether it has been (successfully)
     * opened for reading and no previous read operation caused an error, e.g., due
     * to I/O failures at the OS level or loss of stream buffer integrity.
     */
    bool is_readable() const override;

    /**
     * Checks whether EOF has been reached, so that there is no more data to read at
     * the current byte position.
     */
    bool is_eof() const override;

    /**
     * Returns the total readable size of the file, in bytes.
     */
    size_t get_total_readable_bytes() const override;

    /**
     * Returns the number of bytes that can be read from the file at the current
     * read position.
     */
    size_t get_readable_bytes_remaining() const override;

    /**
     * Returns the current read position within the file (in bytes, relative to the
     * beginning).
     */
    size_t get_read_position() const override;

    /**
     * (Re)sets the current read position within the file (in bytes, relative to the
     * beginning). Returns the new/current byte position.
     */
    size_t set_read_position(size_t position) override;

    /**
     * (Re)sets the current read position within the file by moving it by the
     * specified bytes relative to the beginning, end, or current position, as
     * specified. Returns the new/current byte position.
     */
    size_t seek_read(ptrdiff_t relative, SeekMode seek_mode = SeekMode::Current) override;

    /**
     * Reads data from the file (at the current position) into the specified buffer
     * and returns the number of bytes actually read.
     */
    size_t read(cl7::byte_span buffer) override;

    /**
     * Reads a single byte from the file (at the current position) and returns the
     * number of bytes actually read (i.e. 0 or 1).
     */
    size_t read(std::byte& byte) override;

    /**
     * "Peeks" a single byte from the file (at the current position) without
     * extracting it. Returns the number of bytes that would have been extracted if
     * actually read (i.e. 0 or 1).
     */
    size_t peek(std::byte& byte) override;


    /**
     * Checks whether the file is writable, i.e. whether it has been (successfully)
     * opened for writing and no previous read operation caused an error, e.g., due
     * to I/O failures at the OS level or loss of stream buffer integrity.
     */
    bool is_writable() const override;

    /**
     * Returns the total writable size of the file, in bytes.
     */
    size_t get_total_writable_bytes() const override;

    /**
     * Returns the number of bytes that can be written to the file at the current
     * write position.
     */
    size_t get_writable_bytes_remaining() const override;

    /**
     * Returns the current write position within the file (in bytes, relative to the
     * beginning).
     */
    size_t get_write_position() const override;

    /**
     * (Re)sets the current write position within the file (in bytes, relative to
     * the beginning). Returns the new/current byte position.
     */
    size_t set_write_position(size_t position) override;

    /**
     * (Re)sets the current write position within the file by moving it by the
     * specified bytes relative to the beginning, end, or current position, as
     * specified. Returns the new/current byte position.
     */
    size_t seek_write(ptrdiff_t relative, SeekMode seek_mode = SeekMode::Current) override;

    /**
     * Writes data to the file (at the current position) from the specified buffer
     * and returns the number of bytes actually written.
     */
    size_t write(cl7::byte_view buffer) override;

    /**
     * Writes a single byte to the file (at the current position) and returns the
     * number of bytes actually written (i.e. 0 or 1).
     */
    size_t write(std::byte byte) override;


    /**
     * Returns the current size of the file, in bytes.
     */
    size_t get_size() const override;

    /**
     * (Re)sets the size of the file, in bytes, and truncates or appends the
     * difference in bytes accordingly. Returns the new/current size.
     */
    size_t set_size(size_t size) override;



private:
    std::fstream _fstream;
    cl7::u8string _path;
    OpenMode _open_mode;
    size_t _size = 0;
    size_t _read_position = 0;
    size_t _write_position = 0;

}; // class File



} // namespace cl7::io

#endif // CL7_IO_FILE_H
