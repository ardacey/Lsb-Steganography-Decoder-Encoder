#include "ImageProcessor.h"
#include "ImageSharpening.h"
#include "EdgeDetector.h"
#include "DecodeMessage.h"
#include "EncodeMessage.h"

ImageSharpening imagesharpening;
EdgeDetector edgeDetector;
DecodeMessage decodeMessage;
EncodeMessage encodeMessage;

ImageProcessor::ImageProcessor() {

}

ImageProcessor::~ImageProcessor() {

}


std::string ImageProcessor::decodeHiddenMessage(const ImageMatrix &img) {

    ImageMatrix sharpImg = imagesharpening.sharpen(img, 2);
    std::vector<std::pair<int, int>> edgeImg = edgeDetector.detectEdges(sharpImg);
    std::string message = decodeMessage.decodeFromImage(sharpImg, edgeImg);

    return message;
}

ImageMatrix ImageProcessor::encodeHiddenMessage(const ImageMatrix &img, const std::string &message) {

    std::string decodedMessage = message; // If I use the message argument directly my test score drops... just... why...

    ImageMatrix sharpImage = imagesharpening.sharpen(img, 2);
    std::vector<std::pair<int, int>> edgePosition = edgeDetector.detectEdges(sharpImage);
    ImageMatrix encodedImage = encodeMessage.encodeMessageToImage(img, decodedMessage, edgePosition);

    return encodedImage;
}
