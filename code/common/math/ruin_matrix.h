#ifndef __RUIN_MATRIX_
#define __RUIN_MATRIX_

#include "ruin_vec.h"

__declspec(align(16))
union mat4
{
    float m[16];
    float4 column[4];
};

union mat3
{
    float m[9];
    float3 column[3];
};

mat3 Mat3();
mat3 Mat3(float);

mat4 Mat4();
mat4 Mat4(float);

mat4 identity();
mat4 transpose(const mat4 &mat);
mat4 translation(float x, float y, float z);
mat4 rotation(float angle, float x, float y, float z);
mat4 scale(float x, float y, float z);
mat4 orthoProjection(float left, float right, float top, float bottom, float znear, float zfar);

mat4 operator* (const mat4 &lhs, const mat4 &rhs);
mat4 &operator*= (mat4 &lhs, const mat4 &rhs);

#endif