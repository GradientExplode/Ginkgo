#ifndef _gkg_processing_coordinates_ElectrostaticOrientationSet_h_
#define _gkg_processing_coordinates_ElectrostaticOrientationSet_h_


#include <gkg-processing-coordinates/OrientationSet.h>


namespace gkg
{


//
// quick C function
//

float GkgGetUniformOrientation( int32_t orientationCount,
                                int32_t index, 
                                int32_t coordinate );

//
// class ElectrostaticOrientationSet
//

class ElectrostaticOrientationSet : public OrientationSet
{

  public:

    ElectrostaticOrientationSet( int32_t count,
                                 bool avoidSymmetricOrientations = true,
                                 int32_t probabilityFactor = 2,
                                 float multiplicator = 1.0f,
                                 float initialStep = 0.01f,
                                 float minimumStep = 1e-8,
                                 int32_t globalStepCount = 10000,
                                 int32_t internalStepCount = 10000
                                  );
    virtual ~ElectrostaticOrientationSet();


};


}


#endif
