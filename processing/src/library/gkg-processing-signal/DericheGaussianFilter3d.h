#ifndef _gkg_processing_signal_DericheGaussianFilter3d_h_
#define _gkg_processing_signal_DericheGaussianFilter3d_h_


#include <gkg-core-pattern/Singleton.h>
#include <gkg-processing-signal/DericheGaussianCoefficients.h>


namespace gkg
{


template < class U > class Volume;
class Matrix;


template < class T >
class DericheGaussianFilter3d : public Singleton< DericheGaussianFilter3d< T > >
{

  public:

    enum Axis
    {

      X_AXIS,
      Y_AXIS,
      Z_AXIS

    };

    ~DericheGaussianFilter3d();

    template < class U >
    void filter( const Volume< U >& inVolume,
                 const DericheGaussianCoefficients< T >& coefficients,
                 Axis axis,
                 Volume< U >& outVolume ) const;
    template < class U >
    void floatingFilter( const Volume< U >& inVolume,
                         const DericheGaussianCoefficients< T >& coefficients,
                         Axis axis,
                         Volume< T >& outVolume ) const;
    void filter( const Matrix& inMatrix,
                 const DericheGaussianCoefficients< T >& coefficients,
                 Axis axis,
                 Matrix& outMatrix ) const;


  protected:

    friend class Singleton< DericheGaussianFilter3d< T > >;

    DericheGaussianFilter3d();


};


}


#endif
