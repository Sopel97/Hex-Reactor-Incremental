#ifndef TRIANGLE_H_INCLUDED
#define TRIANGLE_H_INCLUDED

template <class T>
class Triangle : public Shape2<T>
{
public:
    Vec2<T> vertices[3];
    Triangle() = default;
    Triangle(const Vec2<T>& p1, const Vec2<T>& p2, const Vec2<T>& p3);
    Triangle(const Vec2<T>* p);

    Triangle(const Triangle<T>& t) = default;
    Triangle(Triangle<T>&& t) = default;
    Triangle<T>& operator =(const Triangle<T>& t) = default;
    Triangle<T>& operator =(Triangle<T> && t) = default;

    template <class X>
    Triangle(const Triangle<X>& t);
    template <class X>
    Triangle<T>& operator =(const Triangle<X>& t);

    Triangle<T> operator +(const Vec2<T>& p) const;
    Triangle<T>& operator +=(const Vec2<T>& p);
    Triangle<T> operator -(const Vec2<T>& p) const;
    Triangle<T>& operator -=(const Vec2<T>& p);

    void translate(const Vec2<T>& v);
    void scale(const Vec2<T>& c, const Vec2<T>& s);
    void scale(const Vec2<T>& c, const T s);
    void scale(const Vec2<T>& s);
    void scale(const T s);

    template <class Transformation>
    void transform(Transformation&& func);

    Mesh<LineSegment<T>> outline() const;

    template<class S>
    bool intersects(const S& b) const;

    static Triangle<T> equilateral(const Vec2D& center, const T base);
    static Triangle<T> isosceles(const Vec2D& center, const T base, const T height);
    static Triangle<T> rightTriangle(const Vec2D& rightAngledVertex, const T width, const T height); //with 90 degree angle on the left
};

typedef Triangle<double> TriangleD;
typedef Triangle<float> TriangleF;
typedef Triangle<int> TriangleI;

#include "../src/Triangle.cpp"
#endif // TRIANGLE_H_INCLUDED
