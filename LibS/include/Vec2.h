#ifndef Vec2_H_INCLUDED
#define Vec2_H_INCLUDED

template <class T>
class Vec2 : public Shape2<T>
{
public:
    T x, y;

    Vec2() = default;
    Vec2(T _x, T _y);
    Vec2(const std::initializer_list<T>& list);

    Vec2(const Vec2<T>& v) = default;
    Vec2(Vec2<T>&& v) = default;
    Vec2<T>& operator=(const Vec2<T>& v1) = default;
    Vec2<T>& operator=(Vec2<T> && v1) = default;

    template <class X>
    Vec2(const Vec2<X>& v);
    template <class X>
    Vec2<T>& operator=(const Vec2<X>& v1);

    Vec2<T> operator+(const Vec2<T>& v1) const;
    Vec2<T> operator-(const Vec2<T>& v1) const;
    Vec2<T> operator*(const T scalar) const;
    Vec2<T> operator*(const Vec2<T>& v1) const;
    Vec2<T> operator/(const T scalar) const;
    Vec2<T> operator/(const Vec2<T>& v1) const;

    Vec2<T>& operator+=(const Vec2<T>& v1);
    Vec2<T>& operator-=(const Vec2<T>& v1);
    Vec2<T>& operator*=(const T scalar);
    Vec2<T>& operator*=(const Vec2<T>& v1);
    Vec2<T>& operator/=(const T scalar);
    Vec2<T>& operator/=(const Vec2<T>& v1);

    T magnitude() const;
    T distanceTo(const Vec2<T>& v1) const;
    void normalize();
    Vec2<T> normalized() const;
    Vec2<T> normalLeft() const;
    Vec2<T> normalRight() const;
    Vec2<T> normal() const; //one of above two. Should be used only when it makes no difference which one is used.
    T dot(const Vec2<T>& b) const;
    T cross(const Vec2<T>& b) const;
    Vec2<T> project(const Vec2<T>& b) const;
    Angle<T> angle() const;
    Angle<T> angle(const Vec2<T>& other) const;

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

    static const Vec2<T> unitX;
    static const Vec2<T> unitY;
    static Vec2<T> directionFromRadians(T radians);
    static Vec2<T> directionFromDegrees(T degrees);

    /* swizzling */
    Vec2<T&> xx(){return Vec2<T&>(x,x);}
    Vec2<T&> xy(){return Vec2<T&>(x,y);}
    Vec2<T&> yx(){return Vec2<T&>(y,x);}
    Vec2<T&> yy(){return Vec2<T&>(y,y);}

    Vec3<T&> xxx(){return Vec3<T&>(x,x,x);}
    Vec3<T&> xxy(){return Vec3<T&>(x,x,y);}
    Vec3<T&> xyx(){return Vec3<T&>(x,y,x);}
    Vec3<T&> xyy(){return Vec3<T&>(x,y,y);}
    Vec3<T&> yxx(){return Vec3<T&>(y,x,x);}
    Vec3<T&> yxy(){return Vec3<T&>(y,x,y);}
    Vec3<T&> yyx(){return Vec3<T&>(y,y,x);}
    Vec3<T&> yyy(){return Vec3<T&>(y,y,y);}

    /* vec classes not prepared yet
    Vec4<T&> xxxx(){return Vec4<T&>(x,x,x,x);}
    Vec4<T&> xxxy(){return Vec4<T&>(x,x,x,y);}
    Vec4<T&> xxyx(){return Vec4<T&>(x,x,y,x);}
    Vec4<T&> xxyy(){return Vec4<T&>(x,x,y,y);}
    Vec4<T&> xyxx(){return Vec4<T&>(x,y,x,x);}
    Vec4<T&> xyxy(){return Vec4<T&>(x,y,x,y);}
    Vec4<T&> xyyx(){return Vec4<T&>(x,y,y,x);}
    Vec4<T&> xyyy(){return Vec4<T&>(x,y,y,y);}
    Vec4<T&> yxxx(){return Vec4<T&>(y,x,x,x);}
    Vec4<T&> yxxy(){return Vec4<T&>(y,x,x,y);}
    Vec4<T&> yxyx(){return Vec4<T&>(y,x,y,x);}
    Vec4<T&> yxyy(){return Vec4<T&>(y,x,y,y);}
    Vec4<T&> yyxx(){return Vec4<T&>(y,y,x,x);}
    Vec4<T&> yyxy(){return Vec4<T&>(y,y,x,y);}
    Vec4<T&> yyyx(){return Vec4<T&>(y,y,y,x);}
    Vec4<T&> yyyy(){return Vec4<T&>(y,y,y,y);}
    */

    #ifdef SFML_VECTOR2_HPP
    //conversions to sf::Vector2<T>
    operator sf::Vector2<T>() const
    {
        return sf::Vector2<T>(x,y);
    }
    #endif // SFML_VECTOR2_HPP
};

template <class T>
const Vec2<T> Vec2<T>::unitX = Vec2<T> {1, 0};
template <class T>
const Vec2<T> Vec2<T>::unitY = Vec2<T> {0, 1};

typedef Vec2<double> Vec2D;
typedef Vec2<float> Vec2F;
typedef Vec2<int> Vec2I;

/* do swizzling with usage of specialized vec classes for references like below. These classes should have every feature possible (all may be possible to implement) */
/* swizzling will only be in the origincal class because chaining swizzling would be redundant */

template <class T>
class Vec2<T&> : public Shape2<T>
{
public:
    T& x;
    T& y;

    Vec2(T& _x, T& _y);

    Vec2(const Vec2<T>& v);
    Vec2(Vec2<T>&& v);

    Vec2<T&>& operator=(const Vec2<T>& v1);
    Vec2<T&>& operator=(Vec2<T> && v1);

    template <class X>
    Vec2(const Vec2<X>& v);
    template <class X>
    Vec2<T&>& operator=(const Vec2<X>& v1);

    Vec2<T> operator+(const Vec2<T>& v1) const;
    Vec2<T> operator-(const Vec2<T>& v1) const;
    Vec2<T> operator*(const T scalar) const;
    Vec2<T> operator*(const Vec2<T>& v1) const;
    Vec2<T> operator/(const T scalar) const;
    Vec2<T> operator/(const Vec2<T>& v1) const;

    Vec2<T&>& operator+=(const Vec2<T>& v1);
    Vec2<T&>& operator-=(const Vec2<T>& v1);
    Vec2<T&>& operator*=(const T scalar);
    Vec2<T&>& operator*=(const Vec2<T>& v1);
    Vec2<T&>& operator/=(const T scalar);
    Vec2<T&>& operator/=(const Vec2<T>& v1);

    T magnitude() const;
    T distanceTo(const Vec2<T>& v1) const;
    void normalize();
    Vec2<T> normalized() const;
    Vec2<T> normalLeft() const;
    Vec2<T> normalRight() const;
    Vec2<T> normal() const; //one of above two. Should be used only when it makes no difference which one is used.
    T dot(const Vec2<T>& b) const;
    T cross(const Vec2<T>& b) const;
    Vec2<T> project(const Vec2<T>& b) const;
    Angle<T> angle() const;
    Angle<T> angle(const Vec2<T>& other) const;

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



#include "../src/Vec2.cpp"
#endif // Vec2_H_INCLUDED
