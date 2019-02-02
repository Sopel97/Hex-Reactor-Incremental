template <class T>
Circle<T>::Circle(const Vec2<T>& p1, T r) : center(p1), radius(r)
{
}
template <class T>
template <class X>
Circle<T>::Circle(const Circle<X>& c) : center(c.center), radius(c.radius)
{
}
template <class T>
template <class X>
Circle<T>& Circle<T>::operator =(const Circle<X>& c)
{
    center = c.center;
    radius = c.radius;
    return *this;
}
template <class T>
Circle<T> Circle<T>::operator +(const Vec2<T>& v) const
{
    return Circle<T>(center + v, radius);
}
template <class T>
Circle<T>& Circle<T>::operator +=(const Vec2<T>& v)
{
    center += v;
    return *this;
}
template <class T>
Circle<T> Circle<T>::operator -(const Vec2<T>& v) const
{
    return Circle<T>(center - v, radius);
}
template <class T>
Circle<T>& Circle<T>::operator -=(const Vec2<T>& v)
{
    center -= v;
    return *this;
}

template <class T>
void Circle<T>::translate(const Vec2<T>& v)
{

}
template <class T>
void Circle<T>::scale(const Vec2<T>& c, const Vec2<T>& s)
{

}
template <class T>
void Circle<T>::scale(const Vec2<T>& c, const T s)
{

}
template <class T>
void Circle<T>::scale(const Vec2<T>& s)
{

}
template <class T>
template <class Transformation>
void Circle<T>::transform(Transformation&& func)
{
    func(*this);
}
template <class T>
void Circle<T>::scale(const T s)
{

}

template <class T>
Mesh<LineSegment<T>> Circle<T>::outline() const
{
    const int segmentCount = 64;
    Mesh<LineSegment<T>> out;
    float angle = 0;
    float angleDiff = 2 * PI / (float)segmentCount;
    Vec2<T> prev(cos(angle)*radius + center.x, sin(angle)*radius + center.y);
    for(int i = 0; i < segmentCount; ++i)
    {
        angle += angleDiff;
        Vec2<T> next(cos(angle)*radius + center.x, sin(angle)*radius + center.y);
        out.add(LineSegment<T>(prev, next));
        prev = next;
    }
    return out;
}

/* INTERSECTIONS */
template <class T>
template <class S>
bool Circle<T>::intersects(const S& b) const
{
    return Intersections::intersection(*this, b);
}
