#ifndef RAY_H
#define RAY_H

template <class T>
class Ray
{
public:
    Vec2<T> origin;
    Vec2<T> direction; //must be normalized

    Ray() = default;
    Ray(const Vec2<T>& o, const Vec2<T>& d);

    Ray(const Ray<T>& r) = default;
    Ray(Ray<T>&& r) = default;

    Ray<T>& operator=(const Ray<T>& r) = default;
    Ray<T>& operator=(Ray<T>&& r) = default;

    template <class X>
    Ray(const Ray<X>& r);
    template <class X>
    Ray<T>& operator=(const Ray<X>& r);

    Ray<T> operator+(const Vec2<T>& v) const;
    Ray<T>& operator+=(const Vec2<T>& v);
    Ray<T> operator-(const Vec2<T>& v) const;
    Ray<T>& operator-=(const Vec2<T>& v);

    void setDirection(const Vec2<T>& d);

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
};

typedef Ray<double> RayD;
typedef Ray<float> RayF;
typedef Ray<int> RayI;

#include "../src/Ray.cpp"

#endif // RAY_H
