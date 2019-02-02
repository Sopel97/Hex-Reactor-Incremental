#ifndef CIRCLE_H_INCLUDED
#define CIRCLE_H_INCLUDED

template <class T>
class Circle : public Shape2<T>
{
public:
    Vec2<T> center;
    T radius;

    Circle() = default;
    Circle(const Vec2<T>& p1, T r);

    Circle(const Circle<T>& c) = default;
    Circle(Circle<T>&& c) = default;

    Circle& operator =(Circle<T>&& c) = default;
    Circle& operator =(const Circle<T>& c) = default;

    template<class X>
    Circle(const Circle<X>& c);
    template<class X>
    Circle& operator =(const Circle<X>& c);

    Circle<T> operator +(const Vec2<T>& v) const;
    Circle<T>& operator +=(const Vec2<T>& v);
    Circle<T> operator -(const Vec2<T>& v) const;
    Circle<T>& operator -=(const Vec2<T>& v);

    void translate(const Vec2<T>& v);
    void scale(const Vec2<T>& c, const Vec2<T>& s);
    void scale(const Vec2<T>& c, const T s);
    void scale(const Vec2<T>& s);
    void scale(const T s);

    template <class Transformation>
    void transform(Transformation&& func);

    Mesh<LineSegment<T>> outline() const;

    /* INTERSECTIONS */
    template<class S>
    bool intersects(const S& b) const;
};

typedef Circle<double> CircleD;
typedef Circle<float> CircleF;
typedef Circle<int> CircleI;

#include "../src/Circle.cpp"

#endif // CIRCLE_H_INCLUDED
