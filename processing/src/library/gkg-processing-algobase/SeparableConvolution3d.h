#ifndef _gkg_processing_algobase_SeparableConvolution3d_h_
#define _gkg_processing_algobase_SeparableConvolution3d_h_


#include <vector>


namespace gkg
{


template < class T > class Volume;


template < class IN, class K, class OUT >
class SeparableConvolution3d
{

  public:

    void convolve( const Volume< IN >& in,
                   const std::vector< K >& kernelX,
                   const std::vector< K >& kernelY,
                   const std::vector< K >& kernelZ,
                   Volume< OUT >& out ) const;

};


}


#endif
