
// TODO: this file's only job is to track the internal state of the orber book
// It will assume its only tracking one stock ticker
// So contract-wise, it just needs to provide methods for adding, updating,
// removing, executing trades We have access to the parser. Where do orders get
// fed to the orderbook? Probably by some outside controller that I'll have to
// make. So maybe we have an event OnEventAdd or something (idk how it works in
// C++) in the parser, then we have a controller file to subscribe to this and
// mediate between these two

// Data-wise an "Order" needs a way to identify it. Since we can do a partial
// cancel as well as a delete This is order reference number I think timestamps
// all come in order from the binary file. For UDP it might arrive out of order.
// Idk how you'd go back and correct if a UDP one came in really late.

// Okay I just confirmed that timestamps come in order. At least for
// AddOrderMessage.

// Requirements: Key of order reference number mapped to value of price.
// Buy/Sell should be two separate dictionaries. Would prefer O(1) lookup of
// order reference number. We actually don't need to have it sorted right now,
// we'll use the standard dictionary

// TODO FOR THIS FILE:
// cur - process AddOrderMessage
// - then work on all the others
//
// For separation of resp I'm going to write this class as if it has no idea
// about the ITCH format. It expects good data coming in.

#include <unordered_map>

struct Order {
  uint32_t price;
  uint32_t shares;
};

class OrderBookEngine {

  std::unordered_map<uint64_t, Order> buyOrders;
  std::unordered_map<uint64_t, Order> sellOrders;

  void addOrder(uint64_t referenceNumber, uint32_t price, uint32_t shares,
                bool isBuy) {
    Order newOrder;
    newOrder.price = price;
    newOrder.shares = shares;
    if (isBuy) {
      buyOrders.emplace(referenceNumber, newOrder);
    } else {
      sellOrders.emplace(referenceNumber, newOrder);
    }
  }
};
