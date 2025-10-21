#include "gekko_types.h"
#include "net.h"

#include <cassert>
#include <iostream>

u32 Gekko::NetAddress::GetSize()
{
    return _size;
}

Gekko::NetAddress::NetAddress(void* data, u32 size)
{
    _size = size;
    _data = std::make_unique<u8[]>(_size);
    // copy address data
    std::memcpy(_data.get(), data, _size);
}

Gekko::NetAddress::NetAddress()
{
    _size = 0;
    _data = nullptr;
}

void Gekko::NetAddress::Copy(NetAddress* other)
{
    if (!other) {
        return;
    }

    _size = other->_size;

    if (_data) {
        _data.reset();
    }

    _data = std::make_unique<u8[]>(_size);
    // copy address data
    std::memcpy(_data.get(), other->GetAddress(), _size);
}

bool Gekko::NetAddress::Equals(NetAddress& other)
{
    return _size == other._size && std::memcmp(_data.get(), other._data.get(), _size) == 0;
}

u8* Gekko::NetAddress::GetAddress()
{
    return _data.get();
}

float Gekko::NetStats::CalculateJitter()
{
    if (rtt.size() < 2) {
        return 0.f;
    }

    float sum = 0.f;
    for (i32 i = 1; i < rtt.size(); ++i) {
        sum += std::abs((float)rtt[i] - (float)rtt[i - 1]);
    }

    float jitter = sum / (rtt.size() - 1);
    return jitter;
}

float Gekko::NetStats::CalculateAvgRTT()
{
    if (rtt.empty()) {
        return 0.f;
    }

    float sum = 0.f;
    for (i32 i = 0; i < rtt.size(); i++) {
        sum += rtt[i];
    }

    float avg_rtt = sum / rtt.size();
    return avg_rtt;
}

u32 Gekko::NetStats::LastRTT()
{
    if (rtt.empty()) {
        return 0;
    }

    return rtt.at(0);
}
