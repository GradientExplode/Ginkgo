#ifndef _gkg_processing_numericalanalysis_CubicSpline3d_h_
#define _gkg_processing_numericalanalysis_CubicSpline3d_h_


#include <gkg-processing-container/LightCurve3d.h>
#include <gkg-processing-numericalanalysis/IntegrableFunction.h>
#include <vector>


namespace gkg
{


//
// 3D Cubic Splines (it is creating Hermite splines) relying on the following
// definitions, for the point P(t) at any arclength t:
//   P(t) = a t^3 + b t^2 + c t + d
// a, b, c, d are the coefficient vectors defining the 3D spline segment
// for any segment [ P(i); P(i+1) ] and can be simply calculated using the
// following formulas:
//   a = 2 ( P(0) - P(1) ) + T(0) + T(1)
//   b = 3 ( P(1) - P(0) ) - 2 T(0) - T(1)
//   c = T(0)
//   d = P(0)
// In these formulas, the normalized arclength is used, therefore
//   P(0) = P(i)
//   P(1) = P(i+1)
// The tangent can be defined using the derivative of P(t):
//   T(t) = dP(t)/dt = 3 a t^2 + 2 b t + c
// The curvature can be processeds using the norm of the tangent derivative:
//   C(t) = || dT(t)/dt || = || 6 a t + 2 b ||
// The length of a spline segment is obtained using a numerical integration:
//   length = Integral_0_to_1 of sqrt( ( dP(t)/dt )^2 ) dt
//    ( dP(t)/dt )^2 = T(t)^2 = 9 a^2 t^4 +
//                              12 a.b t^3 +
//                              ( 6 a.c + 4 b^2 ) t^2 +
//                              4 b.c t +
//                              c^2
//


//
// class SplineSegment3dFunction< T >
//

template < class T >
class SplineSegment3dFunction: public IntegrableFunction
{

  public:

    SplineSegment3dFunction( const T& A,
                             const T& B,
                             const T& C,
                             const T& D,
                             const T& E );
    virtual ~SplineSegment3dFunction();

    double getValue( double u ) const;

  private:

    T _A;
    T _B;
    T _C;
    T _D;
    T _E;

};


//
// class SplineSegment3d< T >
//

template < class T >
class SplineSegment3d
{

  public:

    SplineSegment3d();
    SplineSegment3d( const Vector3d< T >& a, 
                     const Vector3d< T >& b,
                     const Vector3d< T >& c,
                     const Vector3d< T >& d );
    SplineSegment3d( const SplineSegment3d< T >& other );
    virtual ~SplineSegment3d();


    SplineSegment3d< T >& operator=( const SplineSegment3d< T >& other );

    const Vector3d< T >& getA() const;
    const Vector3d< T >& getB() const;
    const Vector3d< T >& getC() const;
    const Vector3d< T >& getD() const;

    const T& getLength() const;

    Vector3d< T > getValue( const T& normalizedArcLength ) const;
    Vector3d< T > getTangent( const T& normalizedArcLength ) const;
    T getCurvature( const T& normalizedArcLength ) const;

  protected:

    Vector3d< T > _a;
    Vector3d< T > _b;
    Vector3d< T > _c;
    Vector3d< T > _d;
    T _length;

};


//
// class CubicSpline3d< T >
//

template < class T >
class CubicSpline3d
{

  public:

    CubicSpline3d( const LightCurve3d< T >& lightCurve3d );
    CubicSpline3d( const std::vector< Vector3d< T > >& controlPoints );
    virtual ~CubicSpline3d();

    T getLength() const;

    int32_t getSplineSegment3dCount() const;
    const SplineSegment3d< T >& getSplineSegment3d( int32_t index ) const;
    const SplineSegment3d< T >& getSplineSegment3d(
                                                  const T& arcLength,
                                                  T& currentCurveLength ) const;

    Vector3d< T > getValue( const T& arcLength ) const;
    Vector3d< T > getTangent( const T& arcLength ) const;
    T getCurvature( const T& arcLength ) const;
    void getInterpolatedCurve3d( 
                              std::vector< Vector3d< T > >& interpolatedCurve3d,
                              int32_t pointsPerSegment = 1 ) const;
    void getInterpolatedCurve3d( LightCurve3d< T >& interpolatedCurve3d,
                                 int32_t pointsPerSegment = 1 ) const;
  
  protected:

    CubicSpline3d();

    void processSegments( const std::vector< Vector3d< T > >& controlPoints,
                          const std::vector< Vector3d< T > >& tangents );

    // _spline is a vector of spline segments between control
    // points (P0, P1,..., Pn); spline segment edges Pi and Pi+1,
    // and the associated tangents Ti and Ti+1, define the
    // spline coefficients (a, b, c, and d) on the segment [Pi; Pi+1]

    std::vector< SplineSegment3d< T > > _splineSegment3ds;

};


}


#endif
