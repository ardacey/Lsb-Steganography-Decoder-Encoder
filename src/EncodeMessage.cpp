#include "EncodeMessage.h"
#include <cmath>

// Default Constructor
EncodeMessage::EncodeMessage() {

}

// Destructor
EncodeMessage::~EncodeMessage() {
    
}

// Function to encode a message into an image matrix
ImageMatrix EncodeMessage::encodeMessageToImage(const ImageMatrix &img, const std::string &message, const std::vector<std::pair<int, int>>& positions) {

    ImageMatrix encodedImage = img;
    std::string& copyMessage = const_cast<std::string&>(message);
    std::vector<std::pair<int, int>>& edgePositions = const_cast<std::vector<std::pair<int, int>>&>(positions);
    int messageLength = copyMessage.length();

    // Character Transformation

    for (int i = 0; i < messageLength; ++i) {
        if(isPrime(i)){
            int value = static_cast<int>(copyMessage[i]) + fibonacci(i);
            if (value <= 32) {
                value += 33;
            } else if (value >= 127) {
                value = 126;
            }

            copyMessage[i] = static_cast<char>(value);
        }
    }

    // Right Circular Shifting

    int shift = messageLength / 2;
    std::string shiftedMessage = copyMessage.substr(messageLength - shift) + copyMessage.substr(0, messageLength - shift);
    std::string shiftedBinaryMessage = stringToBinary(shiftedMessage);

    // Message Embedding
    
    int index = 0;

    for (const auto &pos : edgePositions) {
        int x = pos.first;
        int y = pos.second;

        int imageValue = encodedImage.get_data(x,y);

        char shiftedMessageValue = shiftedBinaryMessage[index];

        imageValue = (imageValue & 0xFE) | (shiftedMessageValue - '0');

        encodedImage.set_data(x,y,imageValue);

        ++index;

        if (index >= shiftedBinaryMessage.length()) {
            break;
        }
    }

    return encodedImage;
}

int EncodeMessage::fibonacci(int n) {
    if (n <= 1) return n;

    int a = 0, b = 1, temp;
    for (int i = 2; i <= n; ++i) {
        temp = a + b;
        a = b;
        b = temp;
    }
    return b;
}

bool EncodeMessage::isPrime(int n) {
    if (n <= 1)
        return false;
    
    for (int i = 2; i <= std::sqrt(n); i++)
        if (n % i == 0)
            return false;
    return true;
}

std::string EncodeMessage::stringToBinary(const std::string& text) {
    std::string binaryString;
    binaryString.reserve(text.length() * 7); // Reserve space for efficiency
    for (char c : text) {
        for (int i = 0; i < 7; ++i) {
            binaryString += ((c & (1 << i)) ? '1' : '0');
        }
    }
    return binaryString;
}