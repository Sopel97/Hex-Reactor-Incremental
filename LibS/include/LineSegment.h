#ifndef LINESEGMENT_H_INCLUDED
#define LINESEGMENT_H_INCLUDED

template <class T>
class LineSegment : public Shape2<T>
{
public:
    Vec2<T> begin;
    Vec2<T> end;

    LineSegment() = default;
    LineSegment(const Vec2<T>& _begin, const Vec2<T>& _end);

    LineSegment(const LineSegment<T>& s) = default;
    LineSegment(LineSegment<T>&& s) = default;

    LineSegment<T>& operator=(const LineSegment<T>& s) = default;
    LineSegment<T>& operator=(LineSegment<T>&& s) = default;

    template <class X>
    LineSegment(const LineSegment<X>& s);
    template <class X>
    LineSegment<T>& operator=(const LineSegment<X>& s);

    LineSegment<T> operator+(const Vec2<T>& v) const;
    LineSegment<T>& operator+=(const Vec2<T>& v);
    LineSegment<T> operator-(const Vec2<T>& v) const;
    LineSegment<T>& operator-=(const Vec2<T>& v);

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

    bool intersects(const LineSegment<T>& lineSegment, Vec2<T>& intersectionPoint) const;
};

typedef LineSegment<double> LineSegmentD;
typedef LineSegment<float> LineSegmentF;
typedef LineSegment<int> LineSegmentI;

#include "../src/LineSegment.cpp"
#endif // LINESEGMENT_H_INCLUDED
