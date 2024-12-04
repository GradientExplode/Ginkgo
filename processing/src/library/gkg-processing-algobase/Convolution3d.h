#ifndef _gkg_processing_algobase_Convolution3d_h_
#define _gkg_processing_algobase_Convolution3d_h_


namespace gkg
{


template < class T > class Volume;


template < class IN, class K, class OUT >
class Convolution3d
{

  public:

    void convolve( const Volume< IN >& in,
                   const Volume< K >& kernel,
                   Volume< OUT >& out ) const;

};


}


#endif
