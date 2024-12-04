#ifndef _gkg_processing_coordinates_GehcOrientationSet_h_
#define _gkg_processing_coordinates_GehcOrientationSet_h_


#include <gkg-processing-coordinates/OrientationSet.h>


namespace gkg
{


//
// quick C function
//

float GehcGetUniformOrientation( int32_t orientationCount,
                                 int32_t index, 
                                 int32_t coordinate );

//
// class GehcOrientationSet
//

class GehcOrientationSet : public OrientationSet
{

  public:

    GehcOrientationSet( int32_t count );
    virtual ~GehcOrientationSet();


};


}


#endif
