#include "PNGImage.h"

void PNGImage::init(unsigned width_, unsigned height_) {
    width = width_;
    height = height_;
    image.assign(height, std::vector<Color>(width));
}

bool PNGImage::loadFromFile(const std::string &filename) {
    std::vector<unsigned char> rawImage;
    unsigned error = lodepng::decode(rawImage, width, height, filename);
    if (error) {
        std::cerr << "Erro when loading PNG file: " << lodepng_error_text(error) << std::endl;
        return false;
    }

    init(width, height);

    for (unsigned y = 0; y < height; ++y) {
        for (unsigned x = 0; x < width; ++x) {
            unsigned index = (y * width + x) * 4;
            set(x, y, Color(rawImage[index], rawImage[index + 1], rawImage[index + 2], rawImage[index + 3]));
        }
    }

    return true;
}

bool PNGImage::saveToFile(const std::string &filename) const {
    std::vector<unsigned char> rawImage(width * height * 4);

    for (unsigned y = 0; y < height; ++y) {
        for (unsigned x = 0; x < width; ++x) {
            unsigned index = (y * width + x) * 4;
            Color c = get(x, y);
            rawImage[index] = c.r;
            rawImage[index + 1] = c.g;
            rawImage[index + 2] = c.b;
            rawImage[index + 3] = c.a;
        }
    }

    unsigned error = lodepng::encode(filename, rawImage, width, height);
    if (error) {
        std::cerr << "Error when saving PNG file: " << lodepng_error_text(error) << std::endl;
        return false;
    }

    return true;
}

bool PNGImage::isUniform(unsigned x, unsigned y, unsigned size, Color &outColor, int tolerance) const {
    if (x + size > width || y + size > height) {
        return false;
    }

    Color firstColor = get(x, y);

    for (unsigned i = y; i < y + size; ++i) {
        for (unsigned j = x; j < x + size; ++j) {
            Color currentColor = get(j, i);
            if (abs(currentColor.r - firstColor.r) > tolerance ||
                abs(currentColor.g - firstColor.g) > tolerance ||
                abs(currentColor.b - firstColor.b) > tolerance) {
                return false;
            }
        }
    }

    outColor = firstColor;
    return true;
}