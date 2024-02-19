#pragma once
#ifndef CL7_IO_FILE_H
#define CL7_IO_FILE_H
#include "./ifile.h"

#include "./open_mode.h"

#include <CoreLabs/fstream.h>
#include <CoreLabs/string.h>



namespace cl7 {
namespace io {



class file
    : public ifile
{

    // #############################################################################
    // Construction / Destruction
    // #############################################################################
public:
    /**
     * Default constructor.
     */
    file();

    /**
     * Explicit constructor.
     */
    file(cl7::string_view path, open_mode open_mode = open_mode::read | open_mode::write);

    /**
     * Destructor.
     */
    virtual ~file() = default;

private:
    /** Copy constructor. */
    file(const file&) = delete;
    /** Copy assignment operator. */
    file& operator = (const file&) = delete;



    // #############################################################################
    // Attributes
    // #############################################################################
private:
    cl7::afstream _fstream;
    cl7::string _path;
    open_mode _open_mode;
    size_t _size;
    size_t _position;



    // #############################################################################
    // Methods
    // #############################################################################
public:
    /**
     * Opens the specified file for reading and/or writing. The seek mode specifies
     * whether to truncate
     */
    bool open(cl7::string_view path, open_mode open_mode = open_mode::read | open_mode::write);

    /**
     * Closes the file (if opened before).
     */
    void close();



    // #############################################################################
    // Implementations
    // #############################################################################
public:
    /**
     * Checks whether the file is "open" and can be used.
     */
    virtual bool is_good() const override;

    /**
     * Returns the size of the file, in bytes.
     */
    virtual size_t get_size() const override { return _size; }

    /**
     * Returns the current byte position within the file (relative to the beginning).
     */
    virtual size_t get_position() const override { return _position; }

    /**
     * (Re)sets the current byte position within the file (relative to the beginning).
     * Returns the new/current byte position.
     */
    virtual size_t set_position(size_t position) override;

    /**
     * (Re)sets the current byte position within the file by moving it by the
     * specified bytes relative to the file's beginning, end, or current position,
     * as specified.
     * Returns the new/current byte position.
     */
    virtual size_t seek(ptrdiff_t relative, const seek_mode seek_mode = seek_mode::current) override;

    /**
     * Reads data from the file (at the current position) into the specified buffer
     * and returns the number of bytes transferred.
     */
    virtual size_t read(cl7::byte_span buffer) override;

    /**
     * Writes data to the file (at the current position) from the specified buffer
     * and returns the number of bytes transferred.
     */
    virtual size_t write(cl7::byte_view buffer) override;

    /**
     * (Re)sets the size of the file, in bytes, and truncates or appends the
     * difference in bytes accordingly.
     * Returns the new/current file size.
     */
    virtual size_t set_size(size_t size) override;

    /**
     * Checks whether the file is "open" and can be read.
     */
    virtual bool is_readable() const override;

    /**
     * Checks whether the file is "open" and can be written.
     */
    virtual bool is_writable() const override;



    // #############################################################################
    // Helpers
    // #############################################################################
private:
    size_t _tell_position();
    void _seek_position(size_t position);
    void _seek_position(ptrdiff_t relative, const seek_mode seek_mode);

}; // class file



} // namespace io
} // namespace cl7

#endif // CL7_IO_FILE_H
