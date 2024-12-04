#ifndef _gkg_processing_coordinates_AntipodallySymmetricUniformOrientationSet_h_
#define _gkg_processing_coordinates_AntipodallySymmetricUniformOrientationSet_h_


#include <gkg-processing-coordinates/OrientationSet.h>


namespace gkg
{


//
// class AntipodallySymmetricUniformOrientationSet
//
//
// algorithm developed from the following publication:
// A simple scheme for generating nearly uniform distribution of
// antipodally symmetric points on the unit sphere
// Cheng Guan Koay, J ut Sci 11. December. ): 377-381
// doi:10.1016/j.jocs.2011.06.007
//

class AntipodallySymmetricUniformOrientationSet : public OrientationSet
{

  public:

    AntipodallySymmetricUniformOrientationSet( int32_t count );
    virtual ~AntipodallySymmetricUniformOrientationSet();


};


}


#endif
