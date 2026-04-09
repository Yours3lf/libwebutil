#ifndef WEBUTIL_H
#define WEBUTIL_H

#include <vector>
#include <algorithm>

static uint16_t swapEndianness(uint16_t x)
{
    uint16_t converted = 0;
    converted |= (0x00ffu & x) << 8u;
    converted |= (0xff00u & x) >> 8u;
    return converted;
}

static uint32_t swapEndianness(uint32_t x)
{
    uint32_t converted = 0;
    converted |= (0x000000ffu & x) << 24u;
    converted |= (0x0000ff00u & x) << 8u;
    converted |= (0x00ff0000u & x) >> 8u;
    converted |= (0xff000000u & x) >> 24u;
    return converted;
}

static uint64_t swapEndianness(uint64_t x)
{
    uint64_t converted = 0;
    converted |= (0x00000000000000ffull & x) << 56ull;
    converted |= (0x000000000000ff00ull & x) << 40ull;
    converted |= (0x0000000000ff0000ull & x) << 24ull;
    converted |= (0x00000000ff000000ull & x) << 8ull;
    converted |= (0x000000ff00000000ull & x) >> 8ull;
    converted |= (0x0000ff0000000000ull & x) >> 24ull;
    converted |= (0x00ff000000000000ull & x) >> 40ull;
    converted |= (0xff00000000000000ull & x) >> 56ull;
    return converted;
}

//extract bits from a 32bit value
// first/last inclusive
//first bit 0..31
//last  bit 0..31
static uint32_t extractBitRange(uint32_t data, uint8_t firstBit, uint8_t lastBit)
{
    assert(lastBit >= firstBit);
    assert(firstBit <= 31);
    assert(lastBit <= 31);
    return (data >> firstBit) & ~(~0u << (lastBit - firstBit + 1u));
}

static void printRawData(const char* raw, int len)
{
    for (int c = 0; c < len; ++c)
    {
        printf("%02x ", raw[c]);
    }
    printf("\n");
}

static void printRawData(const std::vector<char>& raw)
{
    for (uint c = 0; c < raw.size(); ++c)
    {
        printf("%02x ", raw[c]);
    }
    printf("\n");
}

static void printRawData(const std::vector<uint8_t>& raw)
{
    for (uint c = 0; c < raw.size(); ++c)
    {
        printf("%02x ", raw[c]);
    }
    printf("\n");
}

template<typename t>
static const t* getRawData(const std::vector<char>& data, uint32_t& payloadByteOffset, uint32_t length = 0)
{
    const t* d = (const t*)(data.data() + payloadByteOffset);

    uint32_t l = 0;

    if (length != 0)
    {
        l = length;
    }
    else 
    {
        l = sizeof(t);
    }

    //make sure we don't overrun our buffer
    if (payloadByteOffset + l > data.size()) { return nullptr;  }

    payloadByteOffset += l;

    return d;
}

template<typename t>
static void derefRawData(const t* raw, t& candidate)
{
    assert(raw);

    if (raw) 
    {
        candidate = *raw;
    }
}

template<typename t>
static void setRawData(std::vector<char>& buf, const t* data, uint32_t length = 0)
{
    assert(data);

    uint32_t curSize = buf.size();
    uint32_t payloadSize = (length != 0 ? length : sizeof(t));

    buf.resize(buf.size() + payloadSize);
    memcpy(buf.data() + curSize, data, payloadSize);
}

template<class T>
static std::string bytesToString(const T& bytes)
{
    std::string res;
    res.resize(bytes.size() * 2);
    for(size_t c = 0; c < bytes.size(); ++c)
    {
        snprintf(res.data() + (c * 2), 3, "%02x", bytes[c]);
    }
    return res;
}

static std::string normalizeBase64(const std::string& input) {
    std::string out = input;

    bool isUrl = (out.find('-') != std::string::npos ||
                   out.find('_') != std::string::npos);

    if (isUrl) {
        std::replace(out.begin(), out.end(), '-', '+');
        std::replace(out.begin(), out.end(), '_', '/');
    }

    while (out.size() % 4 != 0) {
        out += '=';
    }

    return out;
}

static std::string urlSafeBase64(const std::string& input) {
    std::string out = input;

    bool isUrlSafe = (out.find('+') == std::string::npos &&
                   out.find('/') == std::string::npos);

    if (!isUrlSafe) {
        std::replace(out.begin(), out.end(), '+', '-');
        std::replace(out.begin(), out.end(), '/', '_');
    }

    while (out.back() == '=') {
        out.pop_back();
    }

    return out;
}

#endif