#ifndef PNGIMAGE_H
#define PNGIMAGE_H

#include <vector>
#include <string>
#include <iostream>
#include "lodepng.h" // PNG manipulation library

struct Color {
    unsigned char r, g, b, a;
    Color(unsigned char r_, unsigned char g_, unsigned char b_, unsigned char a_ = 255)
        : r(r_), g(g_), b(b_), a(a_) {}
    Color() : r(0), g(0), b(0), a(255) {}
    bool operator==(const Color &c) const {
        return r == c.r && g == c.g && b == c.b && a == c.a;
    }
    bool operator!=(const Color &c) const {
        return !(*this == c);
    }
};

class PNGImage {
public:
    PNGImage() : width(0), height(0) {}
    PNGImage(unsigned width, unsigned height) { init(width, height); }

    bool loadFromFile(const std::string &filename);
    bool saveToFile(const std::string &filename) const;

    void set(unsigned x, unsigned y, const Color &c) { image[y][x] = c; }
    Color get(unsigned x, unsigned y) const { return image[y][x]; }

    unsigned getWidth() const { return width; }
    unsigned getHeight() const { return height; }

    bool isUniform(unsigned x, unsigned y, unsigned size, Color &outColor, int tolerance = 10) const;

private:
    void init(unsigned width_, unsigned height_);
    unsigned width, height;
    std::vector<std::vector<Color>> image;
};

#endif // PNGIMAGE_H