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