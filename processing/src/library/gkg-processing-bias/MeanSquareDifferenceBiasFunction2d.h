#ifndef _gkg_processing_bias_MeanSquareDifferenceBiasFunction2d_h_
#define _gkg_processing_bias_MeanSquareDifferenceBiasFunction2d_h_


#include <gkg-processing-bias/BiasNelderMeadFunction2d.h>


namespace gkg
{


template < class T >
class MeanSquareDifferenceBiasFunction2d : public BiasNelderMeadFunction2d< T >
{

  public:

    MeanSquareDifferenceBiasFunction2d(
                                      RCPointer< BiasFunctor< T > > biasFunctor,
                                      const Volume< T >& in,
                                      const Volume< int16_t >& mask,
                                      const Volume< float >& weight,
                                      Volume< T >& out,
                                      Volume< float >& bias,
                                      const std::string& planeAxis );
    ~MeanSquareDifferenceBiasFunction2d();

    double getValueAt( const Vector& parameters ) const;

};


}


#endif

