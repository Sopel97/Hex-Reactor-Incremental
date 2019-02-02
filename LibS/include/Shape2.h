#ifndef Shape2_H_INCLUDED
#define Shape2_H_INCLUDED

template <class T>
class Shape2 : public Shape<T>
{
public:
    Shape2() = default;
    Shape2(const Shape2&) = default;
    Shape2(Shape2&&) = default;

    Shape2& operator = (const Shape2&) = default;
    Shape2& operator = (Shape2&&) = default;

    virtual ~Shape2(){};

    virtual Mesh<LineSegment<T>> outline() const;

    virtual void translate(const Vec2<T>& v) = 0;
    virtual void scale(const Vec2<T>& c, const Vec2<T>& s) = 0;
    virtual void scale(const Vec2<T>& c, const T s) = 0;
    virtual void scale(const Vec2<T>& s) = 0;
    virtual void scale(const T s) = 0;

    virtual Vec2<T> project(const Vec2<T>& b) const;
    virtual std::pair<T, T> projectMinMax(const Vec2<T>& b) const; //min and max values on line

};

typedef Shape2<double> Shape2D;
typedef Shape2<float> Shape2F;
typedef Shape2<int> Shape2I;

#include "../src/Shape2.cpp"
#endif // Shape2_H_INCLUDED
