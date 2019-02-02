template <class T>
Rectangle<T>::Rectangle(const Vec2<T>& p1, const Vec2<T>& p2) : min(p1), max(p2)
{
}
template <class T>
Rectangle<T>::Rectangle(const Vec2<T>& p1, const T width, const T height) : min(p1), max(p1 + Vec2<T>(width, height))
{
}
template <class T>
template <class X>
Rectangle<T>::Rectangle(const Rectangle<X>& r) : min(r.min), max(r.max)
{
}
template <class T>
template <class X>
Rectangle<T>& Rectangle<T>::operator =(const Rectangle<X>& r)
{
    min = r.min;
    max = r.max;
    return *this;
}

template <class T>
Rectangle<T> Rectangle<T>::operator +(const Vec2<T>& v) const
{
    return Rectangle<T>(min + v, max + v);
}
template <class T>
Rectangle<T>& Rectangle<T>::operator +=(const Vec2<T>& v)
{
    min += v;
    max += v;
    return *this;
}
template <class T>
Rectangle<T> Rectangle<T>::operator -(const Vec2<T>& v) const
{
    return Rectangle<T>(min - v, max - v);
}
template <class T>
Rectangle<T>& Rectangle<T>::operator -=(const Vec2<T>& v)
{
    min -= v;
    max -= v;
    return *this;
}
template <class T>
T Rectangle<T>::width() const
{
    return max.x - min.x;
}
template <class T>
T Rectangle<T>::height() const
{
    return max.y - min.y;
}

template <class T>
void Rectangle<T>::translate(const Vec2<T>& v)
{

}
template <class T>
void Rectangle<T>::scale(const Vec2<T>& c, const Vec2<T>& s)
{

}
template <class T>
void Rectangle<T>::scale(const Vec2<T>& c, const T s)
{

}
template <class T>
void Rectangle<T>::scale(const Vec2<T>& s)
{

}
template <class T>
void Rectangle<T>::scale(const T s)
{

}
template <class T>
template <class Transformation>
void Rectangle<T>::transform(Transformation&& func)
{
    func(*this);
}

template <class T>
Mesh<LineSegment<T>> Rectangle<T>::outline() const
{
    return Mesh<LineSegment<T>>(
    {
        LineSegment<T>(Vec2<T>(min), Vec2<T>(max.x, min.y)),
        LineSegment<T>(Vec2<T>(min), Vec2<T>(min.x, max.y)),
        LineSegment<T>(Vec2<T>(max), Vec2<T>(max.x, min.y)),
        LineSegment<T>(Vec2<T>(max), Vec2<T>(min.x, max.y))
    });
}

/* INTERSECTIONS */

template <class T>
template <class S>
bool Rectangle<T>::intersects(const S& b) const
{
    return Intersections::intersection(*this, b);
}

template <class T>
Rectangle<T> Rectangle<T>::unitRectangle()
{
    return Rectangle<T>(Vec2<T>{0.0,0.0}, 1.0, 1.0);
}
