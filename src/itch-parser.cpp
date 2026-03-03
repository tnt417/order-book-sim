#include <cstdio>
#include <iostream>
#include <ostream>

#include <cstdint>
#include <vector>
#include <arpa/inet.h>

uint64_t read6ByteBigEndian(const char *bytes) {
    uint64_t value = 0;
    for (int i = 0; i < 6; ++i) {
        value <<= 8;
        value |= static_cast<unsigned char>(bytes[i]);
    }
    return value;
}

// Structs found at https://nasdaqtrader.com/content/technicalsupport/specifications/dataproducts/NQTVITCHSpecification.pdf
struct SystemEventMessage {
    short stockLocate;
    short trackingNumber;
    char timestamp[6];
    char eventCode;
};

struct StockDirectoryMessage {
    short stockLocate;
    short trackingNumber;
    char timestamp[6];
    char stock[8];
    char marketCategory;
    char financialStatusIndicator;
    int roundLotSize;
    char roundLotsOnly;
    char issueClassification;
    char issueSubType[2];
    char authenticity;
    char shortSaleThresholdIndicator;
    char ipoFlag;
    char luldReferencePriceTier;
    char etpFlag;
    int etpLeverageFactor;
    char inverseIndicator;
};

void printNHex(int n, FILE *file) {
    for (int i = 0; i < n; i++) {
        char hex;

        fread(&hex, sizeof(hex), 1, file);

        printf("%X\n", hex);
    }
}

SystemEventMessage readSystemEventMessage(FILE *binary_read, int messageLength) {

    std::vector<char> buffer(messageLength - 1);
    fread(buffer.data(), buffer.size(), 1, binary_read);

    return {};

    SystemEventMessage message{};
    message.stockLocate = ntohs(message.stockLocate);
    message.trackingNumber = ntohs(message.trackingNumber);

    std::cout << "Stock Locate: " << message.stockLocate << std::endl;

    std::cout << "Tracking Number: " << message.trackingNumber << std::endl;

    std::cout << "Event Code: " << message.eventCode << std::endl;

    long timestamp;
    timestamp = read6ByteBigEndian(message.timestamp);

    std::cout << "Timestamp: " << timestamp << std::endl;

    return message;
}

StockDirectoryMessage readStockDirectoryMessage(FILE *binary_read, int messageLength) {
    std::vector<char> buffer(messageLength - 1);
    fread(buffer.data(), buffer.size(), 1, binary_read);

    return {};

    StockDirectoryMessage message{};

    message.stockLocate = ntohs(message.stockLocate);
    message.trackingNumber = ntohs(message.trackingNumber);
    message.roundLotSize = ntohl(message.roundLotSize);

    std::cout << "Stock Locate: " << message.stockLocate << std::endl;

    std::cout << "Tracking Number: " << message.trackingNumber << std::endl;

    long timestamp;
    timestamp = read6ByteBigEndian(message.timestamp);

    std::cout << "Timestamp: " << timestamp << std::endl;

    return message;
}

void readMessageOfType(char messageType, FILE *binaryRead, int messageLength) {
    switch (messageType) {
        case 'S':
            readSystemEventMessage(binaryRead, messageLength);
            break;
        case 'R':
            readStockDirectoryMessage(binaryRead, messageLength);
            break;
    }
}

int main() {
    FILE *binary_read = fopen("../12302019.NASDAQ_ITCH50", "rb");

    if (binary_read == NULL) {
        return 1;
    }

    std::cout << "Struct size: " << sizeof(StockDirectoryMessage) << std::endl;

    for (int i = 0; i < 10000; i++) {
        short messageLengthBE;

        fread(&messageLengthBE, sizeof(messageLengthBE), 1, binary_read);

        short messageLength = ntohs(messageLengthBE);

        std::cout << "Message Length: " << messageLength << std::endl;

        char messageType[1];

        fread(&messageType, sizeof(messageType), 1, binary_read);

        std::cout << "Message type: " << messageType << std::endl;

        readMessageOfType(*messageType, binary_read, messageLength);

        // printNHex(10, binary_read);
    }

    return 0;
}
