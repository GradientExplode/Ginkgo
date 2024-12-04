#ifndef _gkg_processing_numericalanalysis_PhiFunction_h_
#define _gkg_processing_numericalanalysis_PhiFunction_h_


#include <functional>

namespace gkg
{


class PhiFunction
{

  public:

    virtual ~PhiFunction();

    virtual double getValue( double x ) const = 0;

  protected:

    PhiFunction();

};


class IdentityPhiFunction : public PhiFunction
{

  public:

    IdentityPhiFunction();
    virtual ~IdentityPhiFunction();

    double getValue( double x ) const;

};


class NormalizedSquaredCenterPhiFunction : public PhiFunction
{

  public:

    NormalizedSquaredCenterPhiFunction( double mean,
                                        double standardDeviation,
                                        double factor = 1.0 );
    virtual ~NormalizedSquaredCenterPhiFunction();

    double getValue( double x ) const;

  protected:

    double _mean;
    double _standardDeviation;
    double _factorOverTwo;

};


class GaussianPhiFunction : public PhiFunction
{

  public:

    GaussianPhiFunction( double sigma, double maximumX );
    virtual ~GaussianPhiFunction();

    double getValue( double x ) const;

  protected:

    double _twoTimesSigma2;
    double _maximumX;

};


class MultiquadricPhiFunction : public PhiFunction
{

  public:

    MultiquadricPhiFunction( double sigma, double maximumX );
    virtual ~MultiquadricPhiFunction();

    double getValue( double x ) const;

  protected:

    double _sigma2;
    double _maximumX;

};


class ThinPlateSplinePhiFunction : public PhiFunction
{

  public:

    ThinPlateSplinePhiFunction( double sigma, double maximumX );
    virtual ~ThinPlateSplinePhiFunction();

    double getValue( double x ) const;

  protected:

    double _sigmaTimesTwo;
    double _maximumX;

};


/*
class UltraSphericalPolynomialPhiFunction : public PhiFunction
{

  public:

    UltraSphericalPolynomialPhiFunction( int32_t n, int32_t beta );
    virtual ~UltraSphericalPolynomialPhiFunction();

    double getValue( double x ) const;

  protected:

    double _n;
    double _beta;

};
*/


}


#endif
