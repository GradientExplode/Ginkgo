#ifndef _gkg_processing_roi_SurfaceMeasure_h_
#define _gkg_processing_roi_SurfaceMeasure_h_


#include <gkg-processing-container/RoiMeasure.h>
#include <gkg-processing-container/BoundingBox.h>
#include <gkg-processing-morphology/Neighborhood3d.h>


namespace gkg
{


template < class T >
class SurfaceMeasure : public RoiMeasure< T >
{

  public:

    SurfaceMeasure( RCPointer< Roi< T > > roi );
    virtual ~SurfaceMeasure();

    void initialize();
    void processNewValue( const Vector3d< int32_t >& site,
                          bool addOrRemove );

  protected:

    BoundingBox< int32_t > _boundingBox;
    std::list< gkg::Vector3d< int32_t > > _neighbors;
    double _surfaceXY;
    double _surfaceXZ;
    double _surfaceYZ;

};


}


#endif
