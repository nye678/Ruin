#include "ruin_matrix.h"
#include "ruin_math_basic.h"

mat3 Mat3()
{
    mat3 mat = { 
        0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f
    };

    return mat;
}

mat3 Mat3(float v)
{
    mat3 mat = { 
        v, 0.0f, 0.0f, 
        0.0f, v, 0.0f, 
        0.0f, 0.0f, v 
    };
    
    return mat;
}

mat4 Mat4()
{
    mat4 mat = { 
        0.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f 
    };

    return mat;
}

mat4 Mat4(float v)
{
    mat4 mat = { 
        v, 0.0f, 0.0f, 0.0f,
        0.0f, v, 0.0f, 0.0f,
        0.0f, 0.0f, v, 0.0f,
        0.0f, 0.0f, 0.0f, v 
    };
    
    return mat;
}

mat4 Mat4(const mat4 &vec)
{
    mat4 mat = vec;
    return mat;
}

mat4 identity()
{
    return Mat4(1.0f);
}

mat4 transpose(const mat4 &mat)
{
    mat4 t = {
        mat.m[0], mat.m[4], mat.m[8], mat.m[12],
        mat.m[1], mat.m[5], mat.m[9], mat.m[13],
        mat.m[2], mat.m[6], mat.m[10], mat.m[14],
        mat.m[3], mat.m[7], mat.m[11], mat.m[15]
    };

    return t;
}

mat4 translation(float x, float y, float z)
{
    mat4 t = {
        1.0f, 0.0f, 0.0f, x,
        0.0f, 1.0f, 0.0f, y,
        0.0f, 0.0f, 1.0f, z,
        0.0f, 0.0f, 0.0f, 1.0f
    };
    return t;
}

mat4 rotation(float angle, float x, float y, float z)
{
    float c = cos(angle);
    float s = sin(angle);
    float onec = 1 - c;

    float4 u = Normalize(Float4(x, y, z, 0.0f));

    mat4 r = Mat4(1.0f);
    r.m[0] = c + u.x * u.x * onec;
    r.m[1] = u.y * u.x * onec + u.z * s;
    r.m[2] = u.z * u.x * onec - u.y * s;

    r.m[4] = u.x * u.y * onec - u.z * s;
    r.m[5] = c + u.y * u.y * onec;
    r.m[6] = u.z * u.y * onec + u.x * s;

    r.m[8] = u.x * u.z * onec + u.y * s;
    r.m[9] = u.y * u.z * onec - u.x * s;
    r.m[10] = c + u.z * u.z * onec;
    return r;
}

mat4 scale(float x, float y, float z)
{
    mat4 s = {
        x, 0.0f, 0.0f, 0.0f,
        0.0f, y, 0.0f, 0.0f,
        0.0f, 0.0f, z, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
    return s;
}

mat4 orthoProjection(float left, float right, float top, float bottom, float znear, float zfar)
{
    mat4 ortho = {0};
    ortho.column[0].x = 2.0f / (right - left);
    ortho.column[1].y = 2.0f / (top - bottom);
    ortho.column[2].z = 2.0f / (znear - zfar);
    ortho.column[3] = Float4(
        (left + right) / (left - right),
        (bottom + top) / (bottom - top),
        (znear + zfar) / (zfar - znear),
        1.0f);
    return ortho;
}

mat4 operator* (const mat4 &lhs, const mat4 &rhs)
{
    mat4 product = Mat4(lhs);
    return product *= rhs;
}

mat4 &operator*= (mat4 &lhs, const mat4 &rhs)
{
    _MM_TRANSPOSE4_PS(lhs.column[0].v, lhs.column[1].v, lhs.column[2].v, lhs.column[3].v);

    __m128 temp = _mm_load_ps(lhs.column[0].d);
    lhs.column[0].v = _mm_dp_ps(temp, rhs.column[0].v, 0xF8);
    lhs.column[0].v = _mm_dp_ps(temp, rhs.column[1].v, 0xF4);
    lhs.column[0].v = _mm_dp_ps(temp, rhs.column[2].v, 0xF2);
    lhs.column[0].v = _mm_dp_ps(temp, rhs.column[3].v, 0xF1);

    temp = _mm_load_ps(lhs.column[1].d);
    lhs.column[1].v = _mm_dp_ps(temp, rhs.column[0].v, 0xF8);
    lhs.column[1].v = _mm_dp_ps(temp, rhs.column[1].v, 0xF4);
    lhs.column[1].v = _mm_dp_ps(temp, rhs.column[2].v, 0xF2);
    lhs.column[1].v = _mm_dp_ps(temp, rhs.column[3].v, 0xF1);

    temp = _mm_load_ps(lhs.column[2].d);
    lhs.column[2].v = _mm_dp_ps(temp, rhs.column[0].v, 0xF8);
    lhs.column[2].v = _mm_dp_ps(temp, rhs.column[1].v, 0xF4);
    lhs.column[2].v = _mm_dp_ps(temp, rhs.column[2].v, 0xF2);
    lhs.column[2].v = _mm_dp_ps(temp, rhs.column[3].v, 0xF1);

    temp = _mm_load_ps(lhs.column[3].d);
    lhs.column[3].v = _mm_dp_ps(temp, rhs.column[0].v, 0xF8);
    lhs.column[3].v = _mm_dp_ps(temp, rhs.column[1].v, 0xF4);
    lhs.column[3].v = _mm_dp_ps(temp, rhs.column[2].v, 0xF2);
    lhs.column[3].v = _mm_dp_ps(temp, rhs.column[3].v, 0xF1);
    
    return lhs;
}