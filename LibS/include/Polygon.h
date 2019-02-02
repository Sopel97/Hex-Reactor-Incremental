#ifndef POLYGON_H_INCLUDED
#define POLYGON_H_INCLUDED

template <class T>
class Polygon : public Shape2<T>
{
public:
    std::vector<Vec2<T>> vertices;
    Polygon() = default;
    Polygon(const std::initializer_list<Vec2<T>>& list);
    Polygon(const std::vector<Vec2<T>>& v);
    Polygon(std::vector<Vec2<T>>&& v);
    Polygon(Vec2<T>* v, size_t count);
    Polygon(const Polygon<T>& p) = default;
    Polygon(Polygon<T>&& p) = default;

    Polygon<T>& operator=(const Polygon<T>& p) = default;
    Polygon<T>& operator=(Polygon<T> && p) = default;

    template <class X>
    Polygon(const Polygon<X>& p);
    template <class X>
    Polygon<T>& operator=(const Polygon<X>& p);

    Polygon<T> operator+(const Vec2<T>& v) const;
    Polygon<T>& operator+=(const Vec2<T>& v);
    Polygon<T> operator-(const Vec2<T>& v) const;
    Polygon<T>& operator-=(const Vec2<T>& v);

    void add(const Vec2<T>& v);
    void add(const std::vector<T>& v);
    void add(const std::initializer_list<Vec2<T>>& list);

    void add(Vec2<T>&& v);
    void add(std::vector<T>&& v);

    void translate(const Vec2<T>& v);
    void scale(const Vec2<T>& c, const Vec2<T>& s);
    void scale(const Vec2<T>& c, const T s);
    void scale(const Vec2<T>& s);
    void scale(const T s);

    Vec2<T> project(const Vec2<T>& b) const;
    std::pair<T, T> projectMinMax(const Vec2<T>& b) const;

    template <class Transformation>
    void transform(Transformation&& func);

    Mesh<LineSegment<T>> outline() const;

    size_t size() const;

    template <class S>
    bool intersects(const S& shape) const;

    //static Polygon<T> randomInscribedInCircle(const Circle<T>& circle, int edges); later, when using Random.h will be somehow standarized
    static Polygon<T> regular(const Vec2D& center, int sides, int radius);
    static Polygon<T> fromRectangle(const Rectangle<T>& rectangle);
    static Polygon<T> fromTriangle(const Triangle<T>& triangle);
};

typedef Polygon<double> PolygonD;
typedef Polygon<float> PolygonF;
typedef Polygon<int> PolygonI;

#include "../src/Polygon.cpp"
#endif // POLYGON_H_INCLUDED
