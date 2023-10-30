#include <iostream>

#include "Convolution.h"

const double paddingValue = 0.0;

// Default constructor 
Convolution::Convolution() {
    kernel = nullptr;
    kernelHeight = 0;
    kernelWidth = 0;
    stride = 1;
    padding = false;
}

// Parametrized constructor for custom kernel and other parameters
Convolution::Convolution(double** customKernel, int kh, int kw, int stride_val, bool pad){
    kernelHeight = kh;
    kernelWidth = kw;
    stride = stride_val;
    padding = pad;

    kernel = new double*[kernelHeight];
    for (int i = 0; i < kernelHeight; i++) {
        kernel[i] = new double[kernelWidth];
        for (int j = 0; j < kernelWidth; j++) {
            kernel[i][j] = customKernel[i][j];
        }
    }
}

// Destructor
Convolution::~Convolution() {
    if (kernel != nullptr) {
        for (int i = 0; i < kernelHeight; i++) {
            delete[] kernel[i];
        }
        delete[] kernel;
        kernel = nullptr;
    }
}

// Copy constructor
Convolution::Convolution(const Convolution &other){    
    kernel = new double*[other.kernelHeight];
    // Copy kernel from other to kernel
    for (int i = 0; i < other.kernelHeight; i++) {
        kernel[i] = new double[other.kernelWidth];
        for (int j = 0; j < other.kernelWidth; j++) {
            kernel[i][j] = other.kernel[i][j];
        }
    }
}

// Copy assignment operator
Convolution& Convolution::operator=(const Convolution &other) {
    if (this == &other) {
        return *this; // self-assignment check
    }
    
    // Deallocate old memory
    if (kernel != nullptr) {
        for (int i = 0; i < kernelHeight; i++) {
            delete[] kernel[i];
        }
        delete[] kernel;
    }

    // Copy from other
    kernel = new double*[other.kernelHeight];

    for (int i = 0; i < other.kernelHeight; i++) {
        kernel[i] = new double[other.kernelWidth];
        for (int j = 0; j < other.kernelWidth; j++) {
            kernel[i][j] = other.kernel[i][j];
        }
    }

    return *this;
}


// Convolve Function: Responsible for convolving the input image with a kernel and return the convolved image.
ImageMatrix Convolution::convolve(const ImageMatrix& input_image) const {

    const ImageMatrix* inputData = nullptr;
    ImageMatrix result;
    ImageMatrix* paddedImage = nullptr;

    if (padding) {
        int newHeight = input_image.get_height()+2;
        int newWidth = input_image.get_width()+2;
        paddedImage = new ImageMatrix(newHeight, newWidth);

        // If padding is true add paddingValue around the image.

        for (int i = 0; i < newHeight; i++) {
            for (int j = 0; j < newWidth; j++) {
                if (i == 0 || i == newHeight - 1 || j == 0 || j == newWidth - 1) {
                    paddedImage->set_data(i, j, paddingValue);
                } else {
                    paddedImage->set_data(i, j, input_image.get_data(i - 1, j - 1));
                }
            }
        }
    
    // result ImageMatrix constructor

        inputData = paddedImage;
        result = ImageMatrix(((newHeight - kernelHeight)/stride) + 1, ((newWidth - kernelWidth)/stride) + 1);
    } else {
        inputData = &input_image;
        result = ImageMatrix(((input_image.get_height() - kernelHeight)/stride) + 1, ((input_image.get_width() - kernelWidth)/stride) + 1);
    }

    // Convolution formula (technically cross-correlation)

    for (int i = 0; i < result.get_height(); i++){
        for (int j = 0; j < result.get_width(); j++){
            double value = 0.0;
            for (int m = 0; m < kernelHeight; m++){
                for (int n = 0; n < kernelWidth; n++){
                    value += kernel[m][n] * inputData->get_data(m + i * stride, n + j * stride);
                }
            }
            result.set_data(i, j, value);
        }
    }

    if (padding) {
        delete paddedImage;
    }

    return result;
}