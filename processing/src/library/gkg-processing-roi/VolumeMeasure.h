#ifndef _gkg_processing_roi_VolumeMeasure_h_
#define _gkg_processing_roi_VolumeMeasure_h_


#include <gkg-processing-container/RoiMeasure.h>


namespace gkg
{


template < class T >
class VolumeMeasure : public RoiMeasure< T >
{

  public:

    VolumeMeasure( RCPointer< Roi< T > > roi );
    virtual ~VolumeMeasure();

    void initialize();
    void processNewValue( const Vector3d< int32_t >& /*site*/,
                          bool addOrRemove );

  protected:

    double _voxelVolume;

};


}


#endif
