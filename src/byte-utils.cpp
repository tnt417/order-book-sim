#include <cstdint>

uint16_t read_u16_be(const unsigned char* p) {
    return static_cast<uint16_t>(p[0]) << 8 | p[1];
}

uint32_t read_u32_be(const unsigned char* p) {
    return static_cast<uint32_t>(p[0]) << 24 |
           static_cast<uint32_t>(p[1]) << 16 |
           static_cast<uint32_t>(p[2]) << 8  |
           p[3];
}

uint64_t read_u48_be(const unsigned char* p) {
    return static_cast<uint64_t>(p[0]) << 40 |
           static_cast<uint64_t>(p[1]) << 32 |
           static_cast<uint64_t>(p[2]) << 24 |
           static_cast<uint64_t>(p[3]) << 16 |
           static_cast<uint64_t>(p[4]) << 8  |
           p[5];
}

uint64_t read_u64_be(const unsigned char* p) {
    return static_cast<uint64_t>(p[0]) << 56 |
           static_cast<uint64_t>(p[1]) << 48 |
           static_cast<uint64_t>(p[2]) << 40 |
           static_cast<uint64_t>(p[3]) << 32 |
           static_cast<uint64_t>(p[4]) << 24 |
           static_cast<uint64_t>(p[5]) << 16 |
           static_cast<uint64_t>(p[6]) << 8  |
           p[7];
}
