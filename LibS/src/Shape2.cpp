template <class T>
Mesh<LineSegment<T>> Shape2<T>::outline() const
{
    return Mesh<LineSegment<T>>();
}

template <class T>
Vec2<T> Shape2<T>::project(const Vec2<T>& b) const
{
    //not specified
    return Vec2<T>();
}

template <class T>
std::pair<T,T> Shape2<T>::projectMinMax(const Vec2<T>& b) const
{
    //not specified
    return std::pair<T,T>(T(), T());
}

