#pragma once

#include <cstdint>

class RateLimiter
{
public:
    virtual ~RateLimiter() = default;

    virtual bool consume( const uint64_t tokens = 1 ) = 0;
};
