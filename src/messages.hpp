#include <cstdint>

// Structs found at https://nasdaqtrader.com/content/technicalsupport/specifications/dataproducts/NQTVITCHSpecification.pdf
struct SystemEventMessage {
    uint16_t stockLocate;
    uint16_t trackingNumber;
    uint64_t timestamp;
    char eventCode;
};

struct StockDirectoryMessage {
    uint16_t stockLocate;
    uint16_t trackingNumber;
    uint64_t timestamp;
    char stock[8];
    char marketCategory;
    char financialStatusIndicator;
    uint32_t roundLotSize;
    char roundLotsOnly;
    char issueClassification;
    char issueSubType[2];
    char authenticity;
    char shortSaleThresholdIndicator;
    char ipoFlag;
    char luldReferencePriceTier;
    char etpFlag;
    uint32_t etpLeverageFactor;
    char inverseIndicator;
};

struct StockTradingActionMessage {
    uint16_t stockLocate;
    uint16_t trackingNumber;
    uint64_t timestamp;
    char stock[8];
    char tradingState;
    char reserved;
    char reason[4];
};

struct ShortSaleMessage {
    uint16_t locateCode;
    uint16_t trackingNumber;
    uint64_t timestamp;
    char stock[8];
    char regShoAction;
};

struct MarketParticipantPositionMessage {
    uint16_t stockLocate;
    uint16_t trackingNumber;
    uint64_t timestamp;
    char mpid[4];
    char stock[8];
    char primaryMarketMaker;
    char marketMakerMode;
    char marketParticipantState;
};

struct AddOrderMessage {
    uint16_t stockLocate;
    uint16_t trackingNumber;
    uint64_t timestamp;
    uint64_t orderReferenceNumber;
    char buySellIndicator;
    uint32_t shares;
    char stock[8];
    uint32_t price;
};

struct AddOrderAttribMessage {
    uint16_t stockLocate;
    uint16_t trackingNumber;
    uint64_t timestamp;
    uint64_t orderReferenceNumber;
    char buySellIndicator;
    uint32_t shares;
    char stock[8];
    uint32_t price;
    char attribution[4];
};

struct OrderExecutedMessage {
    uint16_t stockLocate;
    uint16_t trackingNumber;
    uint64_t timestamp;
    uint64_t orderReferenceNumber;
    uint32_t executedShares;
    uint64_t matchNumber;
};

struct OrderDeleteMessage {
    uint16_t stockLocate;
    uint16_t trackingNumber;
    uint64_t timestamp;
    uint64_t orderReferenceNumber;
};

struct OrderCancelMessage {
    uint16_t stockLocate;
    uint16_t trackingNumber;
    uint64_t timestamp;
    uint64_t orderReferenceNumber;
    uint32_t cancelledShares;
};

struct OrderReplaceMessage {
    uint16_t stockLocate;
    uint16_t trackingNumber;
    uint64_t timestamp;
    uint64_t originalOrderReferenceNumber;
    uint64_t newOrderReferenceNumber;
    uint32_t shares;
    uint32_t price;
};

struct TradeMessage {
    uint16_t stockLocate;
    uint16_t trackingNumber;
    uint64_t timestamp;
    uint64_t orderReferenceNumber;
    char buySellIndicator;
    uint32_t shares;
    char stock[8];
    uint32_t price;
    uint64_t matchNumber;
};

struct OrderExecutedWithPriceMessage {
    uint16_t stockLocate;
    uint16_t trackingNumber;
    uint64_t timestamp;
    uint64_t orderReferenceNumber;
    uint32_t executedShares;
    uint64_t matchNumber;
    char printable;
    uint32_t executionPrice;
};

struct CrossTradeMessage {
    uint16_t stockLocate;
    uint16_t trackingNumber;
    uint64_t timestamp;
    uint32_t shares;
    char stock[8];
    uint32_t crossPrice;
    uint64_t matchNumber;
    char crossType;
};

struct NetOrderImbalanceIndicatorMessage {
    uint16_t stockLocate;
    uint16_t trackingNumber;
    uint64_t timestamp;
    uint64_t pairedShares;
    uint64_t imbalanceShares;
    char imbalanceDirection;
    char stock[8];
    uint32_t farPrice;
    uint32_t nearPrice;
    uint32_t currentReferencePrice;
    char crossType;
    char priceVariationIndicator;
};

struct MwcbDeclineLevelMessage {
    uint16_t stockLocate;
    uint16_t trackingNumber;
    uint64_t timestamp;
    uint64_t level1;
    uint64_t level2;
    uint64_t level3;
};

struct QuotingPeriodUpdateMessage {
    uint16_t stockLocate;
    uint16_t trackingNumber;
    uint64_t timestamp;
    char stock[8];
    uint32_t ipoQuotationReleaseTime;
    char ipoQuotationReleaseQualifier;
    uint32_t ipoPrice;
};

struct LULDAuctionCollarMessage {

    uint16_t stockLocate;
    uint16_t trackingNumber;
    uint64_t timestamp;
    char stock[8];
    uint32_t auctionCollarReferencePrice;
    uint32_t upperAuctionCollarPrice;
    uint32_t lowerAuctionCollarPrice;
    uint32_t auctionCollarExtension;
};
