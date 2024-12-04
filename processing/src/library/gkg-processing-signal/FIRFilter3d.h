#ifndef _gkg_processing_signal_FIRFilter3d_h_
#define _gkg_processing_signal_FIRFilter3d_h_


#include <vector>
#include <string>


namespace gkg
{


template < class T > class Volume;


template < class T >
class FIRFilter3d
{

  public:

    enum Type
    {

      LowPass,
      HighPass,
      BandPass,
      BandStop

    };

    enum Axis
    {

      X,
      Y,
      Z,

    };

    FIRFilter3d( Type type,
                 int32_t filterSize,
                 const std::vector< double >& frequencies,
                 const std::string& apodizationName,
                 bool verbose = false );
    virtual ~FIRFilter3d();        

    void filter( const Volume< T >& in,
                 Volume< T >& out,
                 Axis axis ) const;

  protected:

    std::vector< double > _h;    
    bool _verbose;

};


}


#endif
