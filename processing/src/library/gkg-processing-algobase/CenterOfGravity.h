#ifndef _gkg_processing_algobase_CenterOfGravity_h_
#define _gkg_processing_algobase_CenterOfGravity_h_


#include <gkg-processing-coordinates/Vector3d.h>


namespace gkg
{


template < class T > class Volume;


template < class T >
class CenterOfGravity
{

  public:

    Vector3d< float > compute( const Volume< T >& volume ) const;

};


}


#endif
