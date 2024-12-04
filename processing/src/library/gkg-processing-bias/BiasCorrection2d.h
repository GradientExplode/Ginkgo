#ifndef _gkg_processing_bias_BiasCorrection2d_h_
#define _gkg_processing_bias_BiasCorrection2d_h_


#include <gkg-processing-bias/BiasCorrection.h>
#include <gkg-processing-bias/BiasNelderMeadFunction2d.h>
#include <gkg-core-pattern/RCPointer.h>
#include <gkg-core-pattern/Creator.h>
#include <vector>


namespace gkg
{


template < class T >
class BiasCorrection2d : public BiasCorrection< T >,
                         public Creator2Arg< BiasCorrection2d< T >,
                                             BiasCorrection< T >,
                                             const std::vector< double >&,
                                             const std::vector< std::string >& >
{

  public:

    virtual ~BiasCorrection2d();

    std::string getName() const;

    static std::string getStaticName();

    void correct( const Volume< T >& in,
                  const Volume< int16_t >& mask,
                  const Volume< float >& weight,
                  Volume< T >& out,
                  Volume< float >& bias,
                  bool verbose ) const;

  protected:

    friend class Creator2Arg< BiasCorrection2d< T >,
                              BiasCorrection< T >,
                              const std::vector< double >&,
                              const std::vector< std::string >& >;

    std::string getCode( int32_t time, int32_t slice1, int32_t slice2 ) const;

    BiasCorrection2d( const std::vector< double >& scalarParameters,
                      const std::vector< std::string >& stringParameters );

    int32_t _referenceSlice;
    double _initialParameter;
    double _deltaParameter;
    int32_t _maximumIterationCount;
    double _maximumTestSize;
    bool _verboseNelderMead;
    int32_t _entropyLevelCount;
    bool _entropyIsSmoothingApplied;

    std::string _measureType;
    RCPointer< BiasFunctor< T > > _biasFunctor;
    std::string _planeAxis;

};


}


#endif
