// DecodeMessage.cpp

#include "DecodeMessage.h"

// Default constructor
DecodeMessage::DecodeMessage() {
    // Nothing specific to initialize here
}

// Destructor
DecodeMessage::~DecodeMessage() {
    // Nothing specific to clean up
}


std::string DecodeMessage::decodeFromImage(const ImageMatrix& image, const std::vector<std::pair<int, int>>& edgePixels) {
    std::vector<int> lsbValues;
    std::string lsbString;

    // lsb process

    for (const auto& pixel : edgePixels) {
        int lsb = static_cast<int>(image.get_data(pixel.first, pixel.second)) & 1;
        lsbValues.push_back(lsb);
    }

    for (int lsb : lsbValues) {
        lsbString += std::to_string(lsb);
    }

    int padSize = (7 - lsbValues.size() % 7) % 7;
    std::string paddedLsbValues = std::string(padSize, '0') + lsbString;
    std::string result;
    size_t lsbSize = paddedLsbValues.size();

    // Decoding process and ASCII adjustment
    
    for (int i = 0; i < lsbSize; i += 7) {

        int decimal = 0;
        for (int j = 0; j < 7; ++j) {
            decimal = (decimal << 1) | (paddedLsbValues[i + j] - '0');
        }

        if (decimal <= 32) {
            decimal += 33;
        } else if (decimal == 127) {
            decimal = 126;
        }

        result += static_cast<char>(decimal);
    }

    return result;
}

