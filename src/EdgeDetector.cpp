// EdgeDetector.cpp

#include "EdgeDetector.h"
#include <cmath>

#include <iostream>

// Default constructor
EdgeDetector::EdgeDetector() {

}

// Destructor
EdgeDetector::~EdgeDetector() {

}

// Detect Edges using the given algorithm
std::vector<std::pair<int, int>> EdgeDetector::detectEdges(const ImageMatrix& input_image) {

    // kernel matrix initalization

    double** gxKernel = new double*[3];

    gxKernel[0] = new double[3]{-1.0, 0.0, 1.0};
    gxKernel[1] = new double[3]{-2.0, 0.0, 2.0};
    gxKernel[2] = new double[3]{-1.0, 0.0, 1.0};

    Convolution gxconvolution(gxKernel, 3, 3, 1, true);

    for (int i = 0; i < 3; ++i) {
        delete[] gxKernel[i];
    }
    delete[] gxKernel;

    double** gyKernel = new double*[3];

    gyKernel[0] = new double[3]{-1.0, -2.0, -1.0};
    gyKernel[1] = new double[3]{0.0, 0.0, 0.0};
    gyKernel[2] = new double[3]{1.0, 2.0, 1.0};

    Convolution gyconvolution(gyKernel, 3, 3, 1, true);

    for (int i = 0; i < 3; ++i) {
        delete[] gyKernel[i];
    }
    delete[] gyKernel;

    ImageMatrix ixmatrix = gxconvolution.convolve(input_image);
    ImageMatrix iymatrix = gyconvolution.convolve(input_image);

    // kernel matrix initialization end

    ImageMatrix gradientMagnitudeMatrix(input_image.get_height(), input_image.get_width());
    double threshold = 0.0;

    for (int i = 0; i < ixmatrix.get_height(); i++) {
        for (int j = 0; j < iymatrix.get_width(); j++) {
            double ix = ixmatrix.get_data(i,j);
            double iy = iymatrix.get_data(i,j);

            double gradientMagnitude = std::sqrt(ix*ix + iy*iy);
            gradientMagnitudeMatrix.set_data(i,j,gradientMagnitude);

            threshold += gradientMagnitude;
        }
    }

    threshold /= (gradientMagnitudeMatrix.get_height() * gradientMagnitudeMatrix.get_width());

    std::vector<std::pair<int, int>> edgePixels;

    for (int i = 0; i < gradientMagnitudeMatrix.get_height(); i++) {
        for (int j = 0; j < gradientMagnitudeMatrix.get_width(); j++) {
            if (gradientMagnitudeMatrix.get_data(i,j) > threshold) {
                edgePixels.push_back(std::make_pair(i, j));
            }
        }
    }
    
    return edgePixels;
}