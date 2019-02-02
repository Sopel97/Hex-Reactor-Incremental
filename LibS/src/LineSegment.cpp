template <class T>
LineSegment<T>::LineSegment(const Vec2<T>& _begin, const Vec2<T>& _end) : begin(_begin), end(_end)
{
}

template <class T>
template <class X>
LineSegment<T>::LineSegment(const LineSegment<X>& s) : begin(s._begin), end(s._end)
{
}
template <class T>
template <class X>
LineSegment<T>& LineSegment<T>::operator=(const LineSegment<X>& s)
{
    begin = s.begin;
    end = s.end;
    return *this;
}

template <class T>
LineSegment<T> LineSegment<T>::operator+(const Vec2<T>& v) const
{
    return LineSegment<T>(begin + v, end + v);
}
template <class T>
LineSegment<T>& LineSegment<T>::operator+=(const Vec2<T>& v)
{
    begin += v;
    end += v;
    return *this;
}
template <class T>
LineSegment<T> LineSegment<T>::operator-(const Vec2<T>& v) const
{
    return LineSegment<T>(begin - v, end - v);
}
template <class T>
LineSegment<T>& LineSegment<T>::operator-=(const Vec2<T>& v)
{
    begin -= v;
    end -= v;
    return *this;
}

template <class T>
void LineSegment<T>::translate(const Vec2<T>& v)
{

}
template <class T>
void LineSegment<T>::scale(const Vec2<T>& c, const Vec2<T>& s)
{

}
template <class T>
void LineSegment<T>::scale(const Vec2<T>& c, const T s)
{

}
template <class T>
void LineSegment<T>::scale(const Vec2<T>& s)
{

}
template <class T>
void LineSegment<T>::scale(const T s)
{

}
template <class T>
template <class Transformation>
void LineSegment<T>::transform(Transformation&& func)
{
    func(*this);
}

template <class T>
Mesh<LineSegment<T>> LineSegment<T>::outline() const
{
    return Mesh<LineSegment<T>>({*this});
}

template <class T>
bool LineSegment<T>::intersects(const LineSegment<T>& lineSegment, Vec2<T>& intersectionPoint) const
{
    return Intersections::intersection(*this, lineSegment, intersectionPoint);
}

template <class T>
template <class S>
bool LineSegment<T>::intersects(const S& b) const
{
    return Intersections::intersection(*this, b);
}
