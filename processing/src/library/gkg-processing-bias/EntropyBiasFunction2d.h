#ifndef _gkg_processing_bias_EntropyBiasFunction2d_h_
#define _gkg_processing_bias_EntropyBiasFunction2d_h_


#include <gkg-processing-bias/BiasNelderMeadFunction2d.h>
#include <gkg-processing-signal/DericheGaussianFilter3d.h>


namespace gkg
{


template < class T >
class EntropyBiasFunction2d : public BiasNelderMeadFunction2d< T >
{

  public:

    EntropyBiasFunction2d( RCPointer< BiasFunctor< T > > biasFunctor,
                           const Volume< T >& in,
                           const Volume< int16_t >& mask,
                           const Volume< float >& weight,
                           Volume< T >& out,
                           Volume< float >& bias,
                           const std::string& planeAxis,
                           int32_t levelCount,
                           bool isSmoothingApplied );
    ~EntropyBiasFunction2d();

    double getValueAt( const Vector& parameters ) const;

  protected:

    int32_t _levelCount;
    bool _isSmoothingApplied;
    DericheGaussianCoefficients< double >* _dericheGaussianCoefficients;

};


}


#endif

