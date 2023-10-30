#include "ImageSharpening.h"
#include "Convolution.h"

const double blurring_kernel_value = 1.0 / 9.0;
const int kernel_size = 3;
double min_val = 0.0;
double max_val = 255.0;

// Default constructor
ImageSharpening::ImageSharpening() {
    blurring_kernel = new double*[kernel_size];
    for (int i = 0; i < kernel_size; ++i) {
        blurring_kernel[i] = new double[kernel_size]{blurring_kernel_value, blurring_kernel_value, blurring_kernel_value};
    }

    kernel_height = kernel_size;
    kernel_width = kernel_size;

}

// Default destructor
ImageSharpening::~ImageSharpening(){
    for (int i = 0; i < kernel_height; ++i) {
        delete[] blurring_kernel[i];
    }
    delete[] blurring_kernel;
}

ImageMatrix ImageSharpening::sharpen(const ImageMatrix& input_image, double k) {

    int height = input_image.get_height();
    int width = input_image.get_width();

    ImageMatrix result(height, width);

    // Blurring image

    Convolution convolution(blurring_kernel, kernel_height, kernel_width, 1, true);

    ImageMatrix blur_image = convolution.convolve(input_image);

    // Sharpening image

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            double originalValue = input_image.get_data(i, j);
            double blurValue = blur_image.get_data(i, j);

            double sharpValue = originalValue + k * (originalValue - blurValue);

            // Clipping

            if (sharpValue < min_val) sharpValue = min_val;
            else if (sharpValue > max_val) sharpValue = max_val;

            result.set_data(i, j, sharpValue);
        }
    }

    return result;
}