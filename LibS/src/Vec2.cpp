#include "../include/Intersections.h"
template <class T>
Vec2<T>::Vec2(T _x, T _y) : x(_x), y(_y)
{
}
template <class T>
Vec2<T>::Vec2(const std::initializer_list<T>& list)
{
    typename std::initializer_list<T>::iterator it = list.begin();
    x = *it;
    y = *(it + 1);
}
template <class T>
template <class X>
Vec2<T>::Vec2(const Vec2<X>& v) : x(v.x), y(v.y)
{
}
template <class T>
template <class X>
Vec2<T>& Vec2<T>::operator=(const Vec2<X>& v1)
{
    x = v1.x;
    y = v1.y;
    return *this;
}

template <class T>
Vec2<T> Vec2<T>::operator+(const Vec2<T>& v1) const
{
    return Vec2<T>(x + v1.x, y + v1.y);
}
template <class T>
Vec2<T> Vec2<T>::operator-(const Vec2<T>& v1) const
{
    return Vec2<T>(x - v1.x, y - v1.y);
}
template <class T>
Vec2<T> Vec2<T>::operator*(const T scalar) const
{
    return Vec2<T>(x * scalar, y * scalar);
}
template <class T>
Vec2<T> Vec2<T>::operator*(const Vec2<T>& v1) const
{
    return Vec2<T>(x * v1.x, y * v1.y);
}
template <class T>
Vec2<T> Vec2<T>::operator/(const T scalar) const
{
    return Vec2<T>(x / scalar, y / scalar);
}
template <class T>
Vec2<T> Vec2<T>::operator/(const Vec2<T>& v1) const
{
    return Vec2<T>(x / v1.x, y / v1.y);
}

template <class T>
Vec2<T>& Vec2<T>::operator+=(const Vec2<T>& v1)
{
    x += v1.x;
    y += v1.y;
    return *this;
}
template <class T>
Vec2<T>& Vec2<T>::operator-=(const Vec2<T>& v1)
{
    x -= v1.x;
    y -= v1.y;
    return *this;
}
template <class T>
Vec2<T>& Vec2<T>::operator*=(const T scalar)
{
    x *= scalar;
    y *= scalar;
    return *this;
}
template <class T>
Vec2<T>& Vec2<T>::operator*=(const Vec2<T>& v1)
{
    x *= v1.x;
    y *= v1.y;
    return *this;
}
template <class T>
Vec2<T>& Vec2<T>::operator/=(const T scalar)
{
    x /= scalar;
    y /= scalar;
    return *this;
}
template <class T>
Vec2<T>& Vec2<T>::operator/=(const Vec2<T>& v1)
{
    x /= v1.x;
    y /= v1.y;
    return *this;
}


template <class T>
T Vec2<T>::magnitude() const
{
    return std::sqrt(x * x + y * y);
}
template <class T>
T Vec2<T>::distanceTo(const Vec2<T>& v1) const
{
    T dx = x - v1.x;
    T dy = y - v1.y;
    return std::sqrt(dx * dx + dy * dy);
}
template <class T>
void Vec2<T>::normalize()
{
    T invertedSquareRoot = 1. / std::sqrt(x * x + y * y);
    x *= invertedSquareRoot;
    y *= invertedSquareRoot;
}
template <class T>
Vec2<T> Vec2<T>::normalized() const
{
    T invertedSquareRoot = 1. / std::sqrt(x * x + y * y);
    return Vec2<T>(x * invertedSquareRoot, y * invertedSquareRoot);
}
template <class T>
T Vec2<T>::dot(const Vec2<T>& b) const
{
    return x * b.x + y * b.y;
}
template <class T>
T Vec2<T>::cross(const Vec2<T>& b) const
{
    return x * b.y - y * b.x;
}
template <class T>
Vec2<T> Vec2<T>::project(const Vec2<T>& b) const
{
    Vec2<T> projection;
    T firstPart = dot(b) / (b.x * b.x + b.y * b.y);
    projection.x = firstPart * b.x;
    projection.y = firstPart * b.y;
    return projection;
}
template <class T>
Vec2<T> Vec2<T>::normalLeft() const
{
    return Vec2<T>(-y, x).normalized();
}
template <class T>
Vec2<T> Vec2<T>::normalRight() const
{
    return Vec2<T>(y, -x).normalized();
}
template <class T>
Vec2<T> Vec2<T>::normal() const //right one
{
    return Vec2<T>(y, -x).normalized();
}
template <class T>
Angle<T> Vec2<T>::angle() const
{
    return Angle<T>::radians(atan2(y, x));
}
template <class T>
Angle<T> Vec2<T>::angle(const Vec2<T>& other) const
{
    return Angle<T>::radians(atan2(cross(other), dot(other)));
}

template <class T>
void Vec2<T>::translate(const Vec2<T>& v)
{

}
template <class T>
void Vec2<T>::scale(const Vec2<T>& c, const Vec2<T>& s)
{

}
template <class T>
void Vec2<T>::scale(const Vec2<T>& c, const T s)
{

}
template <class T>
void Vec2<T>::scale(const Vec2<T>& s)
{

}
template <class T>
void Vec2<T>::scale(const T s)
{

}
template <class T>
template <class Transformation>
void Vec2<T>::transform(Transformation&& func)
{
    func(*this);
}

template <class T>
Mesh<LineSegment<T>> Vec2<T>::outline() const
{
    return Mesh<LineSegment<T>>();
}

template <class T>
template <class S>
bool Vec2<T>::intersects(const S& b) const
{
    return Intersections::intersection(*this, b);
}


template <class T>
Vec2<T> directionRadians(T radians)
{
    return Vec2<T> {cos(radians), sin(radians)};
}
template <class T>
Vec2<T> directionDegrees(T degrees)
{
    return Vec2<T> {cos(degreesToRadians(degrees)), sin(degreesToRadians(degrees))};
}

/*
    Version with references
*/
template <class T>
Vec2<T&>::Vec2(T& _x, T& _y) : x(_x), y(_y)
{
}
template <class T>
Vec2<T&>::Vec2(const Vec2<T>& v) : x(v.x), y(v.y)
{

}
template <class T>
Vec2<T&>::Vec2(Vec2<T>&& v) : x(v.x), y(v.y)
{

}

template <class T>
Vec2<T&>& Vec2<T&>::operator=(const Vec2<T>& v1)
{
    x = v1.x;
    y = v1.y;
}
template <class T>
Vec2<T&>& Vec2<T&>::operator=(Vec2<T> && v1)
{
    x = v1.x;
    y = v1.y;
}
template <class T>
template <class X>
Vec2<T&>::Vec2(const Vec2<X>& v) : x(v.x), y(v.y)
{
}
template <class T>
template <class X>
Vec2<T&>& Vec2<T&>::operator=(const Vec2<X>& v1)
{
    x = v1.x;
    y = v1.y;
    return *this;
}

template <class T>
Vec2<T> Vec2<T&>::operator+(const Vec2<T>& v1) const
{
    return Vec2<T>(x + v1.x, y + v1.y);
}
template <class T>
Vec2<T> Vec2<T&>::operator-(const Vec2<T>& v1) const
{
    return Vec2<T>(x - v1.x, y - v1.y);
}
template <class T>
Vec2<T> Vec2<T&>::operator*(const T scalar) const
{
    return Vec2<T>(x * scalar, y * scalar);
}
template <class T>
Vec2<T> Vec2<T&>::operator*(const Vec2<T>& v1) const
{
    return Vec2<T>(x * v1.x, y * v1.y);
}
template <class T>
Vec2<T> Vec2<T&>::operator/(const T scalar) const
{
    return Vec2<T>(x / scalar, y / scalar);
}
template <class T>
Vec2<T> Vec2<T&>::operator/(const Vec2<T>& v1) const
{
    return Vec2<T>(x / v1.x, y / v1.y);
}

template <class T>
Vec2<T&>& Vec2<T&>::operator+=(const Vec2<T>& v1)
{
    x += v1.x;
    y += v1.y;
    return *this;
}
template <class T>
Vec2<T&>& Vec2<T&>::operator-=(const Vec2<T>& v1)
{
    x -= v1.x;
    y -= v1.y;
    return *this;
}
template <class T>
Vec2<T&>& Vec2<T&>::operator*=(const T scalar)
{
    x *= scalar;
    y *= scalar;
    return *this;
}
template <class T>
Vec2<T&>& Vec2<T&>::operator*=(const Vec2<T>& v1)
{
    x *= v1.x;
    y *= v1.y;
    return *this;
}
template <class T>
Vec2<T&>& Vec2<T&>::operator/=(const T scalar)
{
    x /= scalar;
    y /= scalar;
    return *this;
}
template <class T>
Vec2<T&>& Vec2<T&>::operator/=(const Vec2<T>& v1)
{
    x /= v1.x;
    y /= v1.y;
    return *this;
}


template <class T>
T Vec2<T&>::magnitude() const
{
    return std::sqrt(x * x + y * y);
}
template <class T>
T Vec2<T&>::distanceTo(const Vec2<T>& v1) const
{
    T dx = x - v1.x;
    T dy = y - v1.y;
    return std::sqrt(dx * dx + dy * dy);
}
template <class T>
void Vec2<T&>::normalize()
{
    T invertedSquareRoot = 1. / std::sqrt(x * x + y * y);
    x *= invertedSquareRoot;
    y *= invertedSquareRoot;
}
template <class T>
Vec2<T> Vec2<T&>::normalized() const
{
    T invertedSquareRoot = 1. / std::sqrt(x * x + y * y);
    return Vec2<T>(x * invertedSquareRoot, y * invertedSquareRoot);
}
template <class T>
T Vec2<T&>::dot(const Vec2<T>& b) const
{
    return x * b.x + y * b.y;
}
template <class T>
T Vec2<T&>::cross(const Vec2<T>& b) const
{
    return x * b.y - y * b.x;
}
template <class T>
Vec2<T> Vec2<T&>::project(const Vec2<T>& b) const
{
    Vec2<T> projection;
    T firstPart = dot(b) / (b.x * b.x + b.y * b.y);
    projection.x = firstPart * b.x;
    projection.y = firstPart * b.y;
    return projection;
}
template <class T>
Vec2<T> Vec2<T&>::normalLeft() const
{
    return Vec2<T>(-y, x).normalized();
}
template <class T>
Vec2<T> Vec2<T&>::normalRight() const
{
    return Vec2<T>(y, -x).normalized();
}
template <class T>
Vec2<T> Vec2<T&>::normal() const //right one
{
    return Vec2<T>(y, -x).normalized();
}
template <class T>
Angle<T> Vec2<T&>::angle() const
{
    return Angle<T>::radians(atan2(y, x));
}
template <class T>
Angle<T> Vec2<T&>::angle(const Vec2<T>& other) const
{
    return Angle<T>::radians(atan2(cross(other), dot(other)));
}

template <class T>
void Vec2<T&>::translate(const Vec2<T>& v)
{

}
template <class T>
void Vec2<T&>::scale(const Vec2<T>& c, const Vec2<T>& s)
{

}
template <class T>
void Vec2<T&>::scale(const Vec2<T>& c, const T s)
{

}
template <class T>
void Vec2<T&>::scale(const Vec2<T>& s)
{

}
template <class T>
void Vec2<T&>::scale(const T s)
{

}
template <class T>
template <class Transformation>
void Vec2<T&>::transform(Transformation&& func)
{
    func(*this);
}

template <class T>
Mesh<LineSegment<T>> Vec2<T&>::outline() const
{
    return Mesh<LineSegment<T>>();
}

template <class T>
template <class S>
bool Vec2<T&>::intersects(const S& b) const
{
    return Intersections::intersection(*this, b);
}
