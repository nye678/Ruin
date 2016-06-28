#ifndef __RUIN_VEC_
#define __RUIN_VEC_

#include "ruin_int.h"
#include "ruin_math_basic.h"

union float2
{
    float d[2];
    struct { float x, y; };
};

union float3
{
    float d[3];
    struct { float x, y, z; };
    struct { float r, g, b; };
};

__declspec(align(16))
union float4
{
    __m128 v;
    float d[4];
    struct { float x, y, z, w; };
    struct { float r, g, b, a; };
};

union int2
{
    int32_t d[2];
    struct { int32_t x, y; };
};

inline float2 Float2(float x, float y) { return float2 { x, y }; }
inline float3 Float3(float x, float y, float z) { return float3 { x, y, z }; }
inline float4 Float4(float x, float y, float z, float w) { return float4 { x, y, z, w }; }
inline int2 Int2(int32_t x, int32_t y) { return int2 { x, y }; }

inline bool operator== (const float2 &lhs, const float2 &rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

inline bool operator== (const float3 &lhs, const float3 &rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
}

inline bool operator== (const float4 &lhs, const float4 &rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z && lhs.w == rhs.w;
}

inline bool operator!= (const float2 &lhs, const float2 &rhs)
{
    return !(lhs == rhs);
}

inline bool operator!= (const float3 &lhs, const float3 &rhs)
{
    return !(lhs == rhs);
}

inline bool operator!= (const float4 &lhs, const float4 &rhs)
{
    return !(lhs == rhs);
}

inline float2 operator+ (const float2 &lhs, const float2 &rhs)
{
    return float2 { lhs.x + rhs.x, lhs.y + rhs.y };
}

inline float3 operator+ (const float3 &lhs, const float3 &rhs)
{
    return float3 { lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z };
}

inline float4 operator+ (const float4 &lhs, const float4 &rhs)
{
    return float4 { lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w };
}

inline float2 operator- (const float2 &lhs, const float2 &rhs)
{
    return float2 { lhs.x - rhs.x, lhs.y - rhs.y };
}

inline float3 operator- (const float3 &lhs, const float3 &rhs)
{
    return float3 { lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z };
}

inline float4 operator- (const float4 &lhs, const float4 &rhs)
{
    return float4 { lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w };
}

inline float2 operator* (const float2 &lhs, const float2 &rhs)
{
    return float2 { lhs.x * rhs.x, lhs.y * rhs.y };
}

inline float2 operator* (const float2 &lhs, const int2 &rhs)
{
    return float2 { lhs.x * rhs.x, lhs.y * rhs.y };
}

inline float3 operator* (const float3 &lhs, const float3 &rhs)
{
    return float3 { lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z };
}

inline float4 operator* (const float4 &lhs, const float4 &rhs)
{
    return float4 { lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z, lhs.w * rhs.w };
}

inline int2 operator* (const int2 &lhs, const int2 &rhs)
{
    return int2 { lhs.x * rhs.x, lhs.y * rhs.y };
}

inline float4 operator/ (const float4 &lhs, float rhs)
{
    return float4 { lhs.x / rhs, lhs.y / rhs, lhs.z / rhs, lhs.w / rhs };
}

inline float LengthSquare(const float2 &vec)
{
    return vec.x * vec.x + vec.y * vec.y;
}

inline float LengthSquare(const float3 &vec)
{
    return vec.x * vec.x + vec.y * vec.y + vec.z * vec.z;
}

inline float LengthSquare(const float4 &vec)
{
    return vec.x * vec.x + vec.y * vec.y + vec.z * vec.z + vec.w * vec.w;
}

inline float Length(const float4 &vec)
{
    return _mm_cvtss_f32(_mm_sqrt_ss(_mm_dp_ps(vec.v, vec.v, 0xF1)));
}

inline float4 Normalize(const float4 &vec)
{
    return vec / Length(vec);
}

#endif