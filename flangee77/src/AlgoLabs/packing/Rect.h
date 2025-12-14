#ifndef AL7_PACKING_RECT_H
#define AL7_PACKING_RECT_H

#include <CoreLabs/root.h>



namespace al7::packing {



struct Position
{
    unsigned x;
    unsigned y;

    Position() noexcept = default;
    Position(unsigned x, unsigned y) noexcept : x{x}, y{y} {}

    bool operator==(const Position& other) const noexcept = default;
};

struct Size
{
    unsigned width;
    unsigned height;

    Size() noexcept = default;
    Size(unsigned width, unsigned height) noexcept : width{width}, height{height} {}

    bool covers(Size other) const noexcept { return width >= other.width && height >= other.height; }
    Size flipped() const noexcept { return {height, width}; }
    size_t area() const noexcept { return static_cast<size_t>(width) * static_cast<size_t>(height); }

    bool operator==(const Size& other) const noexcept = default;
};

struct Rect
{
    Position position;
    Size size;

    Rect() noexcept = default;
    Rect(Position position, Size size) noexcept : position{position}, size{size} {}
    Rect(Position position, unsigned width, unsigned height) noexcept : position{position}, size{width, height} {}
    Rect(unsigned x, unsigned y, Size size) noexcept : position{x, y}, size{size} {}
    Rect(unsigned x, unsigned y, unsigned width, unsigned height) noexcept : position{x, y}, size{width, height} {}

    bool overlaps(const Rect& other) const noexcept;
    bool contains(const Rect& other) const noexcept;

    bool operator==(const Rect& other) const noexcept = default;
};



} // namespace al7::packing

#endif // AL7_PACKING_RECT_H
