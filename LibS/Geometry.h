#ifndef GEOMETRY_H_INCLUDED
#define GEOMETRY_H_INCLUDED

#include <vector>
#include <cmath>
#include <utility>
#include <functional>
#include <algorithm>
#include "Array2.h"

//maybe will be renamed to Shapes and some functions will be extracted to other files
namespace Geo
{
/*   UTILITY   */
constexpr double PI = 3.14159265359;

inline double radiansToDegrees(const double radians)
{
    return radians * (180.0 / PI);
}
inline double degreesToRadians(const double degrees)
{
    return degrees * (PI / 180.0);
}
template<class T>
T min(const T& a, const T& b)
{
    if(a < b) return a;
    return b;
}
template<class T>
T max(const T& a, const T& b)
{
    if(a > b) return a;
    return b;
}
inline double clamp(int a, int mi, int ma)
{
    return max(min(a, ma), mi);
}
/*   UTILITY END  */
template <class T> class Angle;
template <class T> class Shape;
template <class T> class Shape2;
template <class T> class Vec2;
template <class T> class Vec3;
template <class T> class Rectangle;
template <class T> class Cuboid;
template <class T> class Circle;
template <class T> class Cylinder;
template <class T> class LineSegment;
template <class T> class Ray;
template <class T> class Triangle;
template <class T> class Polygon;
template <class T> class Mesh;

class Intersections;
template <class T> class Raycast;
class Raycaster;

template <class T> class ConvexHull;
template <class T> class AffineTransformation;
template <class T> class BezierCurve;
template <class T> class BezierPath;
template <class T> class Matrix3x3;

/*         HEADERS         */

#include "include/Angle.h"
#include "include/Shape.h"
#include "include/Shape2.h"
#include "include/Vec2.h"
#include "include/Vec3.h"
#include "include/Rectangle.h"
#include "include/Cuboid.h"
#include "include/Circle.h"
#include "include/Cylinder.h"
#include "include/LineSegment.h"
#include "include/Ray.h"
#include "include/Triangle.h"
#include "include/Polygon.h"
#include "include/Mesh.h"

#include "include/Intersections.h"
#include "include/Raycast.h"
#include "include/Raycaster.h"

#include "include/ConvexHull.h"
#include "include/AffineTransformation.h"
#include "include/BezierCurve.h"
#include "include/BezierPath.h"
#include "include/Matrix3x3.h"

}
#endif // GEOMETRY_H_INCLUDED
