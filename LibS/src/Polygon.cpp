template <class T>
Polygon<T>::Polygon(const std::initializer_list<Vec2<T>>& list) : vertices(list)
{
}
template <class T>
Polygon<T>::Polygon(const std::vector<Vec2<T>>& v) : vertices(v)
{
}
template <class T>
Polygon<T>::Polygon(std::vector<Vec2<T>>&& v) : vertices(std::move(v))
{
}
template <class T>
Polygon<T>::Polygon(Vec2<T>* v, size_t count)
{
    vertices.assign(v, v + count);
}
template <class T>
template <class X>
Polygon<T>::Polygon(const Polygon<X>& p) : vertices(p.vertives)
{
}
template <class T>
template <class X>
Polygon<T>& Polygon<T>::operator=(const Polygon<X>& p)
{
    vertices = p.vertices;
    return *this;
}

template <class T>
Polygon<T> Polygon<T>::operator+(const Vec2<T>& v) const
{
    Polygon newPolygon(*this);
    newPolygon += v;
    return newPolygon;
}
template <class T>
Polygon<T>& Polygon<T>::operator+=(const Vec2<T>& v)
{
    for(Vec2<T>& vertex : vertices)
    {
        vertex += v;
    }
    return *this;
}
template <class T>
Polygon<T> Polygon<T>::operator-(const Vec2<T>& v) const
{
    Polygon newPolygon(*this);
    newPolygon -= v;
    return newPolygon;
}
template <class T>
Polygon<T>& Polygon<T>::operator-=(const Vec2<T>& v)
{
    for(Vec2<T>& vertex : vertices)
    {
        vertex -= v;
    }
    return *this;
}

template <class T>
void Polygon<T>::add(const Vec2<T>& v)
{
    vertices.push_back(v);
}
template <class T>
void Polygon<T>::add(const std::vector<T>& v)
{
    vertices.insert(vertices.end(), v.begin(), v.end());
}
template <class T>
void Polygon<T>::add(const std::initializer_list<Vec2<T>>& list)
{
    vertices.insert(vertices.end(), list.begin(), list.end());
}

template <class T>
void Polygon<T>::add(Vec2<T>&& v)
{
    vertices.push_back(v);
}
template <class T>
void Polygon<T>::add(std::vector<T>&& v)
{
    vertices.insert(vertices.end(), v.begin(), v.end());
}

template <class T>
void Polygon<T>::translate(const Vec2<T>& v)
{

}
template <class T>
void Polygon<T>::scale(const Vec2<T>& c, const Vec2<T>& s)
{

}
template <class T>
void Polygon<T>::scale(const Vec2<T>& c, const T s)
{

}
template <class T>
void Polygon<T>::scale(const Vec2<T>& s)
{

}
template <class T>
void Polygon<T>::scale(const T s)
{

}
template <class T>
template <class Transformation>
void Polygon<T>::transform(Transformation&& func)
{
    func(*this);
}

template <class T>
Vec2<T> Polygon<T>::project(const Vec2<T>& b) const
{
    Vec2<T> projection;

    return projection;
}

template <class T>
std::pair<T, T> Polygon<T>::projectMinMax(const Vec2<T>& b) const
{
    T minProjection;
    T maxProjection;
    minProjection = maxProjection = vertices[0].dot(b);
    for(const Vec2<T>& vertex : vertices)
    {
        T dotProduct = vertex.dot(b);
        if(dotProduct < minProjection) minProjection = dotProduct;
        else if(dotProduct > maxProjection) maxProjection = dotProduct;
    }
    return std::pair<T, T>(minProjection, maxProjection);
}
template <class T>
Mesh<LineSegment<T>> Polygon<T>::outline() const
{
    Mesh<LineSegment<T>> outline;
    int vertexCount = vertices.size() - 1;
    for(int i = 0; i < vertexCount; ++i)
    {
        outline.add(LineSegment<T>({vertices[i]}, {vertices[i + 1]}));
    }
    outline.add(LineSegment<T>(vertices[vertexCount], vertices[0]));
    return outline;
}

template <class T>
size_t Polygon<T>::size() const
{
    return vertices.size();
}

template <class T>
template <class S>
bool Polygon<T>::intersects(const S& shape) const
{
    return Intersections::intersection(*this, shape);
}

template <class T>
Polygon<T> Polygon<T>::regular(const Vec2D& center, int sides, int radius)
{
    Polygon<T> polygon;
    polygon.vertices.reserve(sides);
    for(int i = 0; i < sides; ++i)
    {
        polygon.vertices.emplace_back(radius * std::cos(2.0 * PI * i / sides), radius * std::sin(2.0 * PI * i / sides));
    }
    return polygon;
}
template <class T>
Polygon<T> Polygon<T>::fromRectangle(const Rectangle<T>& rectangle)
{
    return Polygon<T>{Vec2<T>{rectangle.min.x, rectangle.min.y},
                      Vec2<T>{rectangle.max.x, rectangle.min.y},
                      Vec2<T>{rectangle.max.x, rectangle.max.y},
                      Vec2<T>{rectangle.min.x, rectangle.max.y}};
}
template <class T>
Polygon<T> Polygon<T>::fromTriangle(const Triangle<T>& triangle)
{
    return Polygon<T>{triangle.vertices[0],triangle.vertices[1],triangle.vertices[2]};
}
