#ifndef _gkg_processing_numericalanalysis_Polynomial2dFitter_h_
#define _gkg_processing_numericalanalysis_Polynomial2dFitter_h_


#include <gkg-processing-numericalanalysis/Vector.h>


namespace gkg
{


class Polynomial2dFitter
{

  public:

    // when set to -1 the maximum degree of the polynomial is automatically
    // calculated from the size of the input x, y, f vectors
    Polynomial2dFitter( const Vector& x,
                        const Vector& y,
                        const Vector& w,
                        const Vector& f,
                        int32_t maximumDegree = -1 );
    Polynomial2dFitter( const Vector& x,
                        const Vector& y,
                        const Vector& f,
                        int32_t maximumDegree = -1 );
    virtual ~Polynomial2dFitter();

    int32_t getMaximumDegree() const;
    const Vector& getCoefficients() const;
    double getValueAt( double x, double y, int32_t maximumOrder = -1 ) const;

  protected:

    void fit( const Vector& x,
              const Vector& y,
              const Vector& w,
              const Vector& f,
              int32_t maximumDegree );
    int32_t triang( int32_t i ) const;
    int32_t strIndx( int32_t i, int32_t j ) const;
    void strIndxInv( int32_t n, int32_t& i, int32_t& j ) const;

    int32_t _maximumDegree;
    Vector _coefficients;

};


}


#endif

