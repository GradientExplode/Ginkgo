#ifndef _gkg_processing_coordinates_GehcNewOrientationSet_h_
#define _gkg_processing_coordinates_GehcNewOrientationSet_h_


#include <gkg-processing-coordinates/OrientationSet.h>


namespace gkg
{


//
// quick C function
//

float GehcNewGetUniformOrientation( int32_t orientationCount,
                                    int32_t index, 
                                    int32_t coordinate );

//
// class GehcOrientationSet
//

class GehcNewOrientationSet : public OrientationSet
{

  public:

    GehcNewOrientationSet( int32_t count );
    virtual ~GehcNewOrientationSet();


};


}


#endif
