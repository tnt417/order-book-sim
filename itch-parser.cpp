#include <cstdio>
#include <iostream>
#include <ostream>

#include <cstdint>

uint64_t read6ByteBigEndian(const char* bytes) {
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

void printNHex(int n, FILE* file) {
    for (int i = 0; i < n; i++) {
        char hex;

        fread(&hex, sizeof(hex), 1, file);

        printf("%X\n", hex);
    }
}

int main() {
    FILE* binary_read = fopen("12302019.NASDAQ_ITCH50", "rb");

    if (binary_read == NULL) {
        return 1;
    }

    short messageLengthBE;

    fread(&messageLengthBE, sizeof(messageLengthBE), 1, binary_read);

    short messageLength = ntohs(messageLengthBE);

    std::cout << "Message Length: " << messageLength << std::endl;

    char messageType[1];

    fread(&messageType, sizeof(messageType), 1, binary_read);

    std::cout << "Message type: " << messageType << std::endl;

    SystemEventMessage message;

    fread(&message, sizeof(message), 1, binary_read);

    message.stockLocate = ntohl(message.stockLocate);
    message.trackingNumber = ntohl(message.trackingNumber);

    std::cout << "Stock Locate: " << message.stockLocate << std::endl;

    std::cout << "Tracking Number: " << message.trackingNumber << std::endl;

    std::cout << "Event Code: " << message.eventCode << std::endl;

    long timestamp = read6ByteBigEndian(message.timestamp);

    std::cout << "Timestamp: " << timestamp << std::endl;


    printNHex(10, binary_read);
// TODO: Next message is of type 'R'

    short nextMessageLengthBE;

    fread(&nextMessageLengthBE, sizeof(nextMessageLengthBE), 1, binary_read);

    short nextMessageLength = ntohs(nextMessageLengthBE);

    std::cout << "Message Length: " << nextMessageLength << std::endl;

    return 0;
}
