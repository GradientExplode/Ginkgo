#ifndef _gkg_processing_bias_BiasCorrectionPoly2d_h_
#define _gkg_processing_bias_BiasCorrectionPoly2d_h_


#include <gkg-processing-bias/BiasCorrection.h>
#include <gkg-core-pattern/Creator.h>
#include <vector>


namespace gkg
{


template < class T >
class BiasCorrectionPoly2d : public BiasCorrection< T >,
                             public Creator2Arg< BiasCorrectionPoly2d< T >,
                                                 BiasCorrection< T >,
                                             const std::vector< double >&,
                                             const std::vector< std::string >& >
{

  public:

    virtual ~BiasCorrectionPoly2d();

    std::string getName() const;

    static std::string getStaticName();

    void correct( const Volume< T >& in,
                  const Volume< int16_t >& mask,
                  const Volume< float >& weight,
                  Volume< T >& out,
                  Volume< float >& bias,
                  bool verbose ) const;

  protected:

    friend class Creator2Arg< BiasCorrectionPoly2d< T >,
                              BiasCorrection< T >,
                              const std::vector< double >&,
                              const std::vector< std::string >& >;

    BiasCorrectionPoly2d( const std::vector< double >& scalarParameters,
                          const std::vector< std::string >& stringParameters );

    int32_t _maximumFitOrder;
    int32_t _maximumBiasOrder;
    std::string _planeAxis;

};


}


#endif
