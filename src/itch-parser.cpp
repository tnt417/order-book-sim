#include <cstdio>
#include <iostream>
#include <ostream>

#include <arpa/inet.h>
#include <cstdint>
#include <vector>

#include "byte-utils.cpp"
#include "messages.hpp"

#define DO_PRINT false

uint64_t read6ByteBigEndian(const char *bytes) {
    uint64_t value = 0;
    for (int i = 0; i < 6; ++i) {
        value <<= 8;
        value |= static_cast<unsigned char>(bytes[i]);
    }
    return value;
}

void printNHex(int n, FILE *file) {
    for (int i = 0; i < n; i++) {
        char hex;

        fread(&hex, sizeof(hex), 1, file);

        printf("%X\n", hex);
    }
}

SystemEventMessage parseSystemEventMessage(FILE *binary_read,
                                           int messageLength) {
    unsigned char buffer[messageLength - 1];
    fread(buffer, sizeof(buffer), 1, binary_read);

    SystemEventMessage message;

    // 2-byte StockLocate
    message.stockLocate = read_u16_be(buffer);

    // 2-byte TrackingNumber
    message.trackingNumber = read_u16_be(buffer + 2);

    // 6-byte Timestamp
    message.timestamp = read_u48_be(buffer + 4);

    // 1-byte EventCode
    message.eventCode = buffer[10];

#if DO_PRINT
    std::cout << "Stock Locate: " << message.stockLocate << std::endl;
    std::cout << "Tracking Number: " << message.trackingNumber << std::endl;
    std::cout << "Event Code: " << message.eventCode << std::endl;
    std::cout << "Timestamp: " << message.timestamp << std::endl;
#endif

    return message;
}

StockDirectoryMessage parseStockDirectoryMessage(FILE *binary_read,
                                                 int messageLength) {
    unsigned char buffer[messageLength - 1];
    fread(buffer, sizeof(buffer), 1, binary_read);

    StockDirectoryMessage message;
    message.stockLocate = read_u16_be(buffer);
    message.trackingNumber = read_u16_be(buffer + 2);
    message.timestamp = read_u48_be(buffer + 4);
    memcpy(message.stock, buffer + 10, 8);
    message.stock[7] = '\0';
    message.marketCategory = buffer[18];
    message.financialStatusIndicator = buffer[19];
    message.roundLotSize = read_u32_be(buffer + 20);
    message.roundLotsOnly = buffer[24];
    message.issueClassification = buffer[25];
    memcpy(message.issueSubType, buffer + 26, 2);
    message.authenticity = buffer[28];
    message.shortSaleThresholdIndicator = buffer[29];
    message.ipoFlag = buffer[30];
    message.luldReferencePriceTier = buffer[31];
    message.etpFlag = buffer[32];
    message.etpLeverageFactor = read_u32_be(buffer + 33);
    message.inverseIndicator = buffer[37];

    return message;
}

StockTradingActionMessage parseStockTradingActionMessage(FILE *binary_read,
                                                         int messageLength) {
    unsigned char buffer[messageLength - 1];
    fread(buffer, sizeof(buffer), 1, binary_read);

    StockTradingActionMessage message;
    message.stockLocate = read_u16_be(buffer);
    message.trackingNumber = read_u16_be(buffer + 2);
    message.timestamp = read_u48_be(buffer + 4);
    memcpy(message.stock, buffer + 10, 8);
    message.tradingState = buffer[18];
    message.reserved = buffer[19];
    memcpy(message.reason, buffer + 20, 4);

    return message;
}

ShortSaleMessage parseShortSaleMessage(FILE *binary_read, int messageLength) {
    unsigned char buffer[messageLength - 1];
    fread(buffer, sizeof(buffer), 1, binary_read);

    ShortSaleMessage message;
    message.locateCode = read_u16_be(buffer);
    message.trackingNumber = read_u16_be(buffer + 2);
    message.timestamp = read_u48_be(buffer + 4);
    memcpy(message.stock, buffer + 10, 8);
    message.regShoAction = buffer[18];

    return message;
}

MarketParticipantPositionMessage
parseMarketParticipantPositionMessage(FILE *binary_read, int messageLength) {
    unsigned char buffer[messageLength - 1];
    fread(buffer, sizeof(buffer), 1, binary_read);

    MarketParticipantPositionMessage message;
    message.stockLocate = read_u16_be(buffer);
    message.trackingNumber = read_u16_be(buffer + 2);
    message.timestamp = read_u48_be(buffer + 4);
    memcpy(message.mpid, buffer + 10, 4);
    memcpy(message.stock, buffer + 14, 8);
    message.primaryMarketMaker = buffer[22];
    message.marketMakerMode = buffer[23];
    message.marketParticipantState = buffer[24];

    std::string mpid_str(message.mpid, 4);

    // std::cout << "Found market maker " << mpid_str << std::endl;

    return message;
}

static uint64_t last_timestamp;

AddOrderMessage parseAddOrderMessage(FILE *binary_read, int messageLength) {
    unsigned char buffer[messageLength - 1];
    fread(buffer, sizeof(buffer), 1, binary_read);

    AddOrderMessage message;
    message.stockLocate = read_u16_be(buffer);
    message.trackingNumber = read_u16_be(buffer + 2);
    message.timestamp = read_u48_be(buffer + 4);

    // std::cout << "Last:" << last_timestamp << std::endl;
    // std::cout << "Cur:" << message.timestamp << std::endl;
    // if (last_timestamp > message.timestamp) {
    //     std::cout << "Arrived out of order!" << std::endl;
    // }

    last_timestamp = message.timestamp;

    message.orderReferenceNumber = read_u64_be(buffer + 10);
    message.buySellIndicator = buffer[18];
    message.shares = read_u32_be(buffer + 19);
    memcpy(message.stock, buffer + 23, 8);
    message.price = read_u32_be(buffer + 31);

    return message;
}

AddOrderAttribMessage parseAddOrderAttribMessage(FILE *binary_read,
                                                 int messageLength) {
    unsigned char buffer[messageLength - 1];
    fread(buffer, sizeof(buffer), 1, binary_read);

    AddOrderAttribMessage message;
    message.stockLocate = read_u16_be(buffer);
    message.trackingNumber = read_u16_be(buffer + 2);
    message.timestamp = read_u48_be(buffer + 4);
    message.orderReferenceNumber = read_u64_be(buffer + 10);
    message.buySellIndicator = buffer[18];
    message.shares = read_u32_be(buffer + 19);
    memcpy(message.stock, buffer + 23, 8);
    message.price = read_u32_be(buffer + 31);
    memcpy(message.attribution, buffer + 35, 4);

    return message;
}

OrderExecutedMessage parseOrderExecutedMessage(FILE *binary_read,
                                               int messageLength) {
    unsigned char buffer[messageLength - 1];
    fread(buffer, sizeof(buffer), 1, binary_read);

    OrderExecutedMessage message;
    message.stockLocate = read_u16_be(buffer);
    message.trackingNumber = read_u16_be(buffer + 2);
    message.timestamp = read_u48_be(buffer + 4);
    message.orderReferenceNumber = read_u64_be(buffer + 10);
    message.executedShares = read_u32_be(buffer + 18);
    message.matchNumber = read_u64_be(buffer + 22);

    return message;
}

OrderDeleteMessage parseOrderDeleteMessage(FILE *binary_read,
                                           int messageLength) {
    unsigned char buffer[messageLength - 1];
    fread(buffer, sizeof(buffer), 1, binary_read);

    OrderDeleteMessage message;
    message.stockLocate = read_u16_be(buffer);
    message.trackingNumber = read_u16_be(buffer + 2);
    message.timestamp = read_u48_be(buffer + 4);
    message.orderReferenceNumber = read_u64_be(buffer + 10);

    return message;
}

OrderCancelMessage parseOrderCancelMessage(FILE *binary_read,
                                           int messageLength) {
    unsigned char buffer[messageLength - 1];
    fread(buffer, sizeof(buffer), 1, binary_read);

    OrderCancelMessage message;
    message.stockLocate = read_u16_be(buffer);
    message.trackingNumber = read_u16_be(buffer + 2);
    message.timestamp = read_u48_be(buffer + 4);
    message.orderReferenceNumber = read_u64_be(buffer + 10);
    message.cancelledShares = read_u32_be(buffer + 18);

    return message;
}

OrderReplaceMessage parseOrderReplaceMessage(FILE *binary_read,
                                             int messageLength) {
    unsigned char buffer[messageLength - 1];
    fread(buffer, sizeof(buffer), 1, binary_read);

    OrderReplaceMessage message;
    message.stockLocate = read_u16_be(buffer);
    message.trackingNumber = read_u16_be(buffer + 2);
    message.timestamp = read_u48_be(buffer + 4);
    message.originalOrderReferenceNumber = read_u64_be(buffer + 10);
    message.newOrderReferenceNumber = read_u64_be(buffer + 18);
    message.shares = read_u32_be(buffer + 26);
    message.price = read_u32_be(buffer + 30);

    return message;
}

TradeMessage parseTradeMessage(FILE *binary_read, int messageLength) {
    unsigned char buffer[messageLength - 1];
    fread(buffer, sizeof(buffer), 1, binary_read);

    TradeMessage message;
    message.stockLocate = read_u16_be(buffer);
    message.trackingNumber = read_u16_be(buffer + 2);
    message.timestamp = read_u48_be(buffer + 4);
    message.orderReferenceNumber = read_u64_be(buffer + 10);
    message.buySellIndicator = buffer[18];
    message.shares = read_u32_be(buffer + 19);
    memcpy(message.stock, buffer + 23, 8);
    message.price = read_u32_be(buffer + 31);
    message.matchNumber = read_u64_be(buffer + 35);

    return message;
}

OrderExecutedWithPriceMessage
parseOrderExecutedWithPriceMessage(FILE *binary_read, int messageLength) {
    unsigned char buffer[messageLength - 1];
    fread(buffer, sizeof(buffer), 1, binary_read);

    OrderExecutedWithPriceMessage message;
    message.stockLocate = read_u16_be(buffer);
    message.trackingNumber = read_u16_be(buffer + 2);
    message.timestamp = read_u48_be(buffer + 4);
    message.orderReferenceNumber = read_u64_be(buffer + 10);
    message.executedShares = read_u32_be(buffer + 18);
    message.matchNumber = read_u64_be(buffer + 22);
    message.printable = buffer[30];
    message.executionPrice = read_u32_be(buffer + 31);

    return message;
}

CrossTradeMessage parseCrossTradeMessage(FILE *binary_read, int messageLength) {
    unsigned char buffer[messageLength - 1];
    fread(buffer, sizeof(buffer), 1, binary_read);

    CrossTradeMessage message;
    message.stockLocate = read_u16_be(buffer);
    message.trackingNumber = read_u16_be(buffer + 2);
    message.timestamp = read_u48_be(buffer + 4);
    message.shares = read_u64_be(buffer + 10);
    memcpy(message.stock, buffer + 18, 8);
    message.crossPrice = read_u32_be(buffer + 26);
    message.matchNumber = read_u64_be(buffer + 30);
    message.crossType = buffer[38];

    return message;
}

NetOrderImbalanceIndicatorMessage
parseNetOrderImbalanceIndicatorMessage(FILE *binary_read, int messageLength) {
    unsigned char buffer[messageLength - 1];
    fread(buffer, sizeof(buffer), 1, binary_read);

    NetOrderImbalanceIndicatorMessage message;
    message.stockLocate = read_u16_be(buffer);
    message.trackingNumber = read_u16_be(buffer + 2);
    message.timestamp = read_u48_be(buffer + 4);
    message.pairedShares = read_u64_be(buffer + 10);
    message.imbalanceShares = read_u64_be(buffer + 18);
    memcpy(message.stock, buffer + 27, 8);
    message.farPrice = read_u32_be(buffer + 35);
    message.nearPrice = read_u32_be(buffer + 39);
    message.currentReferencePrice = read_u32_be(buffer + 43);
    message.crossType = buffer[47];
    message.priceVariationIndicator = buffer[48];

    return message;
}

MwcbDeclineLevelMessage parseMwcbDeclineLevelMessage(FILE *binary_read,
                                                     int messageLength) {
    unsigned char buffer[messageLength - 1];
    fread(buffer, sizeof(buffer), 1, binary_read);

    MwcbDeclineLevelMessage message;
    message.stockLocate = read_u16_be(buffer);
    message.trackingNumber = read_u16_be(buffer + 2);
    message.timestamp = read_u48_be(buffer + 4);
    message.level1 = read_u64_be(buffer + 10);
    message.level2 = read_u64_be(buffer + 18);
    message.level3 = read_u64_be(buffer + 26);

    return message;
}

QuotingPeriodUpdateMessage parseQuotingPeriodUpdateMessage(FILE *binary_read,
                                                           int messageLength) {
    unsigned char buffer[messageLength - 1];
    fread(buffer, sizeof(buffer), 1, binary_read);

    QuotingPeriodUpdateMessage message;
    message.stockLocate = read_u16_be(buffer);
    message.trackingNumber = read_u16_be(buffer + 2);
    message.timestamp = read_u48_be(buffer + 4);
    memcpy(message.stock, buffer + 10, 8);
    message.ipoQuotationReleaseTime = read_u32_be(buffer + 18);
    message.ipoQuotationReleaseQualifier = buffer[22];
    message.ipoPrice = read_u32_be(buffer + 23);

    return message;
}

LULDAuctionCollarMessage parseLULDAuctionCollarMessage(FILE *binary_read,
                                                       int messageLength) {
    unsigned char buffer[messageLength - 1];
    fread(buffer, sizeof(buffer), 1, binary_read);

    LULDAuctionCollarMessage message;
    message.stockLocate = read_u16_be(buffer);
    message.trackingNumber = read_u16_be(buffer + 2);
    message.timestamp = read_u48_be(buffer + 4);
    memcpy(message.stock, buffer + 10, 8);
    message.auctionCollarReferencePrice = read_u32_be(buffer + 18);
    message.upperAuctionCollarPrice = read_u32_be(buffer + 22);
    message.lowerAuctionCollarPrice = read_u32_be(buffer + 26);
    message.auctionCollarExtension = read_u32_be(buffer + 30);

    return message;
}

void readNextMessage(FILE *binaryRead, int messageLength) {
    char messageType;

    fread(&messageType, 1, 1, binaryRead);

    // std::cout << "Message type: " << messageType << std::endl;

    switch (messageType) {
        case 'S':
            parseSystemEventMessage(binaryRead, messageLength);
            break;
        case 'R':
            parseStockDirectoryMessage(binaryRead, messageLength);
            break;
        case 'H':
            parseStockTradingActionMessage(binaryRead, messageLength);
            break;
        case 'Y':
            parseShortSaleMessage(binaryRead, messageLength);
            break;
        case 'A':
            parseAddOrderMessage(binaryRead, messageLength);
            break;
        case 'F':
            parseAddOrderAttribMessage(binaryRead, messageLength);
            break;
        case 'E':
            parseOrderExecutedMessage(binaryRead, messageLength);
            break;
        case 'L':
            parseMarketParticipantPositionMessage(binaryRead, messageLength);
            break;
        case 'D':
            parseOrderDeleteMessage(binaryRead, messageLength);
            break;
        case 'X':
            parseOrderCancelMessage(binaryRead, messageLength);
            break;
        case 'U':
            parseOrderReplaceMessage(binaryRead, messageLength);
            break;
        case 'P':
            parseTradeMessage(binaryRead, messageLength);
            break;
        case 'C':
            parseOrderExecutedWithPriceMessage(binaryRead, messageLength);
            break;
        case 'Q':
            parseCrossTradeMessage(binaryRead, messageLength);
            break;
        case 'I':
            parseNetOrderImbalanceIndicatorMessage(binaryRead, messageLength);
            break;
        case 'V':
            parseMwcbDeclineLevelMessage(binaryRead, messageLength);
            break;
        case 'K':
            parseQuotingPeriodUpdateMessage(binaryRead, messageLength);
            break;
        case 'J':
            parseLULDAuctionCollarMessage(binaryRead, messageLength);
            break;
        default:
            std::cout << "Unknown message type " << messageType << std::endl;
            fseek(binaryRead, messageLength - 1, SEEK_CUR);
    }
}

int main() {
    FILE *binary_read = fopen("../12302019.NASDAQ_ITCH50", "rb");

    if (binary_read == NULL) {
        return 1;
    }

    long numMessages = 0;

    while(true) {
        short messageLengthBE;

        int bytes_read;
        if((bytes_read = fread(&messageLengthBE, sizeof(messageLengthBE), 1, binary_read)) < 1) {
            break;
        }

        numMessages++;

        if (numMessages % 1000000 == 0) {
            printf("%ld\n", numMessages);
        }

        short messageLength = ntohs(messageLengthBE);

        // std::cout << "Message Length: " << messageLength << std::endl;

        readNextMessage(binary_read, messageLength);
    }

    printf("%ld\n", numMessages);

    return 0;
}
