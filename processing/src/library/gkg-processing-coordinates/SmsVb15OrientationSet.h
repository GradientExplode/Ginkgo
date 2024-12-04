#ifndef _gkg_processing_coordinates_SmsVb15OrientationSet_h_
#define _gkg_processing_coordinates_SmsVb15OrientationSet_h_


#include <gkg-processing-coordinates/OrientationSet.h>


namespace gkg
{


//
// quick C function
//

float SmsVb15GetUniformOrientation( int32_t orientationCount,
                                    int32_t index, 
                                    int32_t coordinate );

//
// class SmsVb15OrientationSet
//

class SmsVb15OrientationSet : public OrientationSet
{

  public:

    SmsVb15OrientationSet( int32_t count );
    virtual ~SmsVb15OrientationSet();


};


}


#endif
