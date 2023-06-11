#include <iostream>
#include <cmath>

struct float2
{
    float x;
    float y;

    float length()
    {
        return sqrtf((x * x) + (y * y));
    }

    void normalize()
    {
        const float len = length();

        x /= len;
        y /= len;
    }

    Vector2 V()
    {
        return (Vector2) { x, y};
    }

    float2 operator+(const float2 &v)const
    {
        return (float2) { x + v.x, y + v.y };
    }

    float2 operator-(const float2 &v)const
    {
        return (float2) { x - v.x, y - v.y };
    }

    float2 operator*(const float2 &v)const
    {
        return (float2) { x * v.x, y * v.y };
    }

    float2 operator/(const float2 &v)const
    {
        return (float2) { x / v.x, y / v.y };
    }

    float2 operator+(const float &v)const
    {
        return (float2) { x + v, y + v };
    }

    float2 operator-(const float &v)const
    {
        return (float2) { x - v, y - v };
    }

    float2 operator*(const float &v)const
    {
        return (float2) { x * v, y * v };
    }

    float2 operator/(const float &v)const
    {
        return (float2) { x / v, y / v };
    }

    void operator-=(const float2 &v)
    {
        x -= v.x;
        y -= v.y;
    }

    void operator+=(const float2 &v)
    {
        x += v.x;
        y += v.y;
    }

    void operator*=(const float2 &v)
    {
        x *= v.x;
        y *= v.y;
    }

    void operator/=(const float2 &v)
    {
        x /= v.x;
        y /= v.y;
    }

    void operator-=(const float &v)
    {
        x -= v;
        y -= v;
    }

    void operator+=(const float &v)
    {
        x += v;
        y += v;
    }

    void operator*=(const float &v)
    {
        x *= v;
        y *= v;
    }

    void operator/=(const float &v)
    {
        x /= v;
        y /= v;
    }
};

struct float3
{
    float x;
    float y;
    float z;

    float length()
    {
        return sqrtf((x * x) + (y * y) + (z * z));
    }

    void normalize()
    {
        const float len = length();

        x /= len;
        y /= len;
        z /= len;
    }

    Vector3 V()
    {
        return (Vector3) { x, y, z};
    }

    float3 operator+(const float3 &v)const
    {
        return (float3) { x + v.x, y + v.y, z + v.z };
    }

    float3 operator-(const float3 &v)const
    {
        return (float3) { x - v.x, y - v.y, z - v.z };
    }

    float3 operator*(const float3 &v)const
    {
        return (float3) { x * v.x, y * v.y, z * v.z };
    }

    float3 operator/(const float3 &v)const
    {
        return (float3) { x / v.x, y / v.y, z / v.z };
    }

    float3 operator+(const float &v)const
    {
        return (float3) { x + v, y + v, z + v };
    }

    float3 operator-(const float &v)const
    {
        return (float3) { x - v, y - v, z - v };
    }

    float3 operator*(const float &v)const
    {
        return (float3) { x * v, y * v, z * v };
    }

    float3 operator/(const float &v)const
    {
        return (float3) { x / v, y / v, z / v };
    }

    void operator-=(const float3 &v)
    {
        x -= v.x;
        y -= v.y;
        z -= v.z;
    }

    void operator+=(const float3 &v)
    {
        x += v.x;
        y += v.y;
        z += v.z;
    }

    void operator*=(const float3 &v)
    {
        x *= v.x;
        y *= v.y;
        z *= v.z;
    }

    void operator/=(const float3 &v)
    {
        x /= v.x;
        y /= v.y;
        z /= v.z;
    }

    void operator-=(const float &v)
    {
        x -= v;
        y -= v;
        z -= v;
    }

    void operator+=(const float &v)
    {
        x += v;
        y += v;
        z += v;
    }

    void operator*=(const float &v)
    {
        x *= v;
        y *= v;
        z *= v;
    }

    void operator/=(const float &v)
    {
        x /= v;
        y /= v;
        z /= v;
    }
};


float dot2(float2 a, float2 b)
{
    return ((a.x * b.x) + (a.y * b.y));
}

float distance2(float2 a, float2 b)
{
    const float dx = a.x - b.x;
    const float dy = a.y - b.y;

    return sqrtf((dx * dx) + (dy * dy));
}

float length2(float2 a)
{
    return sqrtf((a.x * a.x) + (a.y * a.y));
}

float2 normalized2(float2 a)
{
    const float len = length2(a);

    return (float2) { a.x / len, a.y / len };
}

float dot3(float3 a, float3 b)
{
    return ((a.x * b.x) + (a.y * b.y) + (a.z * b.z));
}

float distance3(float3 a, float3 b)
{
    const float dx = a.x - b.x;
    const float dy = a.y - b.y;
    const float dz = a.z - b.z;

    return sqrtf((dx * dx) + (dy * dy) + (dz * dz));
}


float length3(float3 a)
{
    return sqrtf((a.x * a.x) + (a.y * a.y) + (a.z * a.z));
}

float3 normalized3(float3 a)
{
    const float len = length3(a);

    return (float3) { a.x / len, a.y / len, a.z / len };
}

float snap(float x, float size)
{
    return floorf(x / size) * size;
}

float2 snap2(float2 x, float2 size)
{
    return (float2) { snap(x.x, size.x), snap(x.y, size.y)};
}

float RNG()
{
    return ((float)(GetRandomValue(0, 1000))) / 1000.0f;
}

float3 snap3(float3 x, float3 size)
{
    return (float3) { snap(x.x, size.x), snap(x.y, size.y), snap(x.z, size.z) };
}

float2 rotateVector(float2 x, float angle)
{
    float xangle = atan2f(x.y, x.x) + angle;

    return (float2) {cosf(xangle), sinf(xangle)};
}

float getAngle(float2 x)
{
    return atan2f(x.x, x.y);
}

float2 RVEC2(Vector2 v)
{
    return (float2) { v.x, v.y };
}

float3 RVEC3(Vector3 v)
{
    return (float3) { v.x, v.y, v.z };
}

const float2 resolution = (float2) { 1000, 700 };
const float2 center = resolution / 2;
