#ifndef Vec3_H_INCLUDED
#define Vec3_H_INCLUDED

template <class T>
class Vec3 : public Shape<T>
{
public:
    T x, y, z;

    Vec3() = default;
    Vec3(T _x, T _y, T _z);
    Vec3(const std::initializer_list<T>& list);

    Vec3(const Vec3<T>& v) = default;
    Vec3(Vec3<T>&& v) = default;
    Vec3<T>& operator=(const Vec3<T>& v1) = default;
    Vec3<T>& operator=(Vec3<T> && v1) = default;

    template <class X>
    Vec3(const Vec3<X>& v);
    template <class X>
    Vec3<T>& operator=(const Vec3<X>& v1);

    Vec3<T> operator+(const Vec3<T>& v1) const;
    Vec3<T> operator-(const Vec3<T>& v1) const;
    Vec3<T> operator*(const T scalar) const;
    Vec3<T> operator*(const Vec3<T>& v1) const;
    Vec3<T> operator/(const T scalar) const;

    Vec3<T>& operator+=(const Vec3<T>& v1);
    Vec3<T>& operator-=(const Vec3<T>& v1);
    Vec3<T>& operator*=(const T scalar);
    Vec3<T>& operator*=(const Vec3<T>& v1);
    Vec3<T>& operator/=(const T scalar);

    T magnitude();
    T distanceTo(const Vec3<T>& v);
    void normalize();
    Vec3<T> normal();

    template <class Transformation>
    void transform(Transformation&& func);

    template<class S>
    bool intersects(const S& b) const;

    static const Vec3<T> unitX;
    static const Vec3<T> unitY;
    static const Vec3<T> unitZ;

    /* swizzling */
    Vec2<T&> xx(){return Vec2<T&>(x,x);}
    Vec2<T&> xy(){return Vec2<T&>(x,y);}
    Vec2<T&> xz(){return Vec2<T&>(x,z);}
    Vec2<T&> yx(){return Vec2<T&>(y,x);}
    Vec2<T&> yy(){return Vec2<T&>(y,y);}
    Vec2<T&> yz(){return Vec2<T&>(y,z);}
    Vec2<T&> zx(){return Vec2<T&>(z,x);}
    Vec2<T&> zy(){return Vec2<T&>(z,y);}
    Vec2<T&> zz(){return Vec2<T&>(z,z);}

    Vec3<T&> xxx(){return Vec3<T&>(x,x,x);}
    Vec3<T&> xxy(){return Vec3<T&>(x,x,y);}
    Vec3<T&> xxz(){return Vec3<T&>(x,x,z);}
    Vec3<T&> xyx(){return Vec3<T&>(x,y,x);}
    Vec3<T&> xyy(){return Vec3<T&>(x,y,y);}
    Vec3<T&> xyz(){return Vec3<T&>(x,y,z);}
    Vec3<T&> xzx(){return Vec3<T&>(x,z,x);}
    Vec3<T&> xzy(){return Vec3<T&>(x,z,y);}
    Vec3<T&> xzz(){return Vec3<T&>(x,z,z);}
    Vec3<T&> yxx(){return Vec3<T&>(y,x,x);}
    Vec3<T&> yxy(){return Vec3<T&>(y,x,y);}
    Vec3<T&> yxz(){return Vec3<T&>(y,x,z);}
    Vec3<T&> yyx(){return Vec3<T&>(y,y,x);}
    Vec3<T&> yyy(){return Vec3<T&>(y,y,y);}
    Vec3<T&> yyz(){return Vec3<T&>(y,y,z);}
    Vec3<T&> yzx(){return Vec3<T&>(y,z,x);}
    Vec3<T&> yzy(){return Vec3<T&>(y,z,y);}
    Vec3<T&> yzz(){return Vec3<T&>(y,z,z);}
    Vec3<T&> zxx(){return Vec3<T&>(z,x,x);}
    Vec3<T&> zxy(){return Vec3<T&>(z,x,y);}
    Vec3<T&> zxz(){return Vec3<T&>(z,x,z);}
    Vec3<T&> zyx(){return Vec3<T&>(z,y,x);}
    Vec3<T&> zyy(){return Vec3<T&>(z,y,y);}
    Vec3<T&> zyz(){return Vec3<T&>(z,y,z);}
    Vec3<T&> zzx(){return Vec3<T&>(z,z,x);}
    Vec3<T&> zzy(){return Vec3<T&>(z,z,y);}
    Vec3<T&> zzz(){return Vec3<T&>(z,z,z);}
    /*
    Vec4<T&> xxxx(){return Vec4<T&>(x,x,x,x);}
    Vec4<T&> xxxy(){return Vec4<T&>(x,x,x,y);}
    Vec4<T&> xxxz(){return Vec4<T&>(x,x,x,z);}
    Vec4<T&> xxyx(){return Vec4<T&>(x,x,y,z);}
    Vec4<T&> xxyy(){return Vec4<T&>(x,x,y,y);}
    Vec4<T&> xxyz(){return Vec4<T&>(x,x,y,z);}
    Vec4<T&> xxzx(){return Vec4<T&>(x,x,z,x);}
    Vec4<T&> xxzy(){return Vec4<T&>(x,x,z,y);}
    Vec4<T&> xxzz(){return Vec4<T&>(x,x,z,z);}
    Vec4<T&> xyxx(){return Vec4<T&>(x,y,x,x);}
    Vec4<T&> xyxy(){return Vec4<T&>(x,y,x,y);}
    Vec4<T&> xyxz(){return Vec4<T&>(x,y,x,z);}
    Vec4<T&> xyyx(){return Vec4<T&>(x,y,y,x);}
    Vec4<T&> xyyy(){return Vec4<T&>(x,y,y,y);}
    Vec4<T&> xyyz(){return Vec4<T&>(x,y,y,z);}
    Vec4<T&> xyzx(){return Vec4<T&>(x,y,z,x);}
    Vec4<T&> xyzy(){return Vec4<T&>(x,y,z,y);}
    Vec4<T&> xyzz(){return Vec4<T&>(x,y,z,z);}
    Vec4<T&> xzxx(){return Vec4<T&>(x,z,x,x);}
    Vec4<T&> xzxy(){return Vec4<T&>(x,z,x,y);}
    Vec4<T&> xzxz(){return Vec4<T&>(x,z,x,z);}
    Vec4<T&> xzyx(){return Vec4<T&>(x,z,y,x);}
    Vec4<T&> xzyy(){return Vec4<T&>(x,z,y,y);}
    Vec4<T&> xzyz(){return Vec4<T&>(x,z,y,z);}
    Vec4<T&> xzzx(){return Vec4<T&>(x,z,z,x);}
    Vec4<T&> xzzy(){return Vec4<T&>(x,z,z,y);}
    Vec4<T&> xzzz(){return Vec4<T&>(x,z,z,z);}
    Vec4<T&> yxxx(){return Vec4<T&>(y,x,x,x);}
    Vec4<T&> yxxy(){return Vec4<T&>(y,x,x,y);}
    Vec4<T&> yxxz(){return Vec4<T&>(y,x,x,z);}
    Vec4<T&> yxyx(){return Vec4<T&>(y,x,y,x);}
    Vec4<T&> yxyy(){return Vec4<T&>(y,x,y,y);}
    Vec4<T&> yxyz(){return Vec4<T&>(y,x,y,z);}
    Vec4<T&> yxzx(){return Vec4<T&>(y,x,z,x);}
    Vec4<T&> yxzy(){return Vec4<T&>(y,x,z,y);}
    Vec4<T&> yxzz(){return Vec4<T&>(y,x,z,z);}
    Vec4<T&> yyxx(){return Vec4<T&>(y,y,x,x);}
    Vec4<T&> yyxy(){return Vec4<T&>(y,y,x,y);}
    Vec4<T&> yyxz(){return Vec4<T&>(y,y,x,z);}
    Vec4<T&> yyyx(){return Vec4<T&>(y,y,y,x);}
    Vec4<T&> yyyy(){return Vec4<T&>(y,y,y,y);}
    Vec4<T&> yyyz(){return Vec4<T&>(y,y,y,z);}
    Vec4<T&> yyzx(){return Vec4<T&>(y,y,z,x);}
    Vec4<T&> yyzy(){return Vec4<T&>(y,y,z,y);}
    Vec4<T&> yyzz(){return Vec4<T&>(y,y,z,z);}
    Vec4<T&> yzxx(){return Vec4<T&>(y,z,x,x);}
    Vec4<T&> yzxy(){return Vec4<T&>(y,z,x,y);}
    Vec4<T&> yzxz(){return Vec4<T&>(y,z,x,z);}
    Vec4<T&> yzyx(){return Vec4<T&>(y,z,y,x);}
    Vec4<T&> yzyy(){return Vec4<T&>(y,z,y,y);}
    Vec4<T&> yzyz(){return Vec4<T&>(y,z,y,z);}
    Vec4<T&> yzzx(){return Vec4<T&>(y,z,z,x);}
    Vec4<T&> yzzy(){return Vec4<T&>(y,z,z,y);}
    Vec4<T&> yzzz(){return Vec4<T&>(y,z,z,z);}
    Vec4<T&> zxxx(){return Vec4<T&>(z,x,x,x);}
    Vec4<T&> zxxy(){return Vec4<T&>(z,x,x,y);}
    Vec4<T&> zxxz(){return Vec4<T&>(z,x,x,z);}
    Vec4<T&> zxyx(){return Vec4<T&>(z,x,y,x);}
    Vec4<T&> zxyy(){return Vec4<T&>(z,x,y,y);}
    Vec4<T&> zxyz(){return Vec4<T&>(z,x,y,z);}
    Vec4<T&> zxzx(){return Vec4<T&>(z,x,z,x);}
    Vec4<T&> zxzy(){return Vec4<T&>(z,x,z,y);}
    Vec4<T&> zxzz(){return Vec4<T&>(z,x,z,z);}
    Vec4<T&> zyxx(){return Vec4<T&>(z,y,x,x);}
    Vec4<T&> zyxy(){return Vec4<T&>(z,y,x,y);}
    Vec4<T&> zyxz(){return Vec4<T&>(z,y,x,z);}
    Vec4<T&> zyyx(){return Vec4<T&>(z,y,y,x);}
    Vec4<T&> zyyy(){return Vec4<T&>(z,y,y,y);}
    Vec4<T&> zyyz(){return Vec4<T&>(z,y,y,z);}
    Vec4<T&> zyzx(){return Vec4<T&>(z,y,z,x);}
    Vec4<T&> zyzy(){return Vec4<T&>(z,y,z,y);}
    Vec4<T&> zyzz(){return Vec4<T&>(z,y,z,z);}
    Vec4<T&> zzxx(){return Vec4<T&>(z,z,x,x);}
    Vec4<T&> zzxy(){return Vec4<T&>(z,z,x,y);}
    Vec4<T&> zzxz(){return Vec4<T&>(z,z,x,z);}
    Vec4<T&> zzyx(){return Vec4<T&>(z,z,y,x);}
    Vec4<T&> zzyy(){return Vec4<T&>(z,z,y,y);}
    Vec4<T&> zzyz(){return Vec4<T&>(z,z,y,z);}
    Vec4<T&> zzzx(){return Vec4<T&>(z,z,z,x);}
    Vec4<T&> zzzy(){return Vec4<T&>(z,z,z,y);}
    Vec4<T&> zzzz(){return Vec4<T&>(z,z,z,z);}
    */
};

template <class T>
const Vec3<T> Vec3<T>::unitX = Vec3<T> {1, 0, 0};
template <class T>
const Vec3<T> Vec3<T>::unitY = Vec3<T> {0, 1, 0};
template <class T>
const Vec3<T> Vec3<T>::unitZ = Vec3<T> {0, 0, 1};

typedef Vec3<double> Vec3D;
typedef Vec3<float> Vec3F;
typedef Vec3<int> Vec3I;

template <class T>
class Vec3<T&> : public Shape<T>
{
public:
    T& x;
    T& y;
    T& z;

    Vec3(T& _x, T& _y, T& _z);

    Vec3(const Vec3<T>& v);
    Vec3(Vec3<T>&& v);
    Vec3<T&>& operator=(const Vec3<T>& v1);
    Vec3<T&>& operator=(Vec3<T> && v1);

    template <class X>
    Vec3(const Vec3<X>& v);
    template <class X>
    Vec3<T&>& operator=(const Vec3<X>& v1);

    Vec3<T> operator+(const Vec3<T>& v1) const;
    Vec3<T> operator-(const Vec3<T>& v1) const;
    Vec3<T> operator*(const T scalar) const;
    Vec3<T> operator*(const Vec3<T>& v1) const;
    Vec3<T> operator/(const T scalar) const;

    Vec3<T&>& operator+=(const Vec3<T>& v1);
    Vec3<T&>& operator-=(const Vec3<T>& v1);
    Vec3<T&>& operator*=(const T scalar);
    Vec3<T&>& operator*=(const Vec3<T>& v1);
    Vec3<T&>& operator/=(const T scalar);

    T magnitude();
    T distanceTo(const Vec3<T>& v);
    void normalize();
    Vec3<T> normal();

    template <class Transformation>
    void transform(Transformation&& func);

    template<class S>
    bool intersects(const S& b) const;
};

#include "../src/Vec3.cpp"
#endif // Vec3_H_INCLUDED
