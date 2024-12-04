#ifndef _gkg_processing_roi_DataAttachmentMeasure_h_
#define _gkg_processing_roi_DataAttachmentMeasure_h_


#include <gkg-processing-container/RoiMeasure.h>
#include <gkg-processing-numericalanalysis/PhiFunction.h>


namespace gkg
{


template < class T, class S >
class DataAttachmentMeasure : public RoiMeasure< T > 
{

  public:

    DataAttachmentMeasure( RCPointer< Roi< T > > roi,
                           RCPointer< Volume< S > > mriVolume,
                           RCPointer< PhiFunction > phiFunction );
    virtual ~DataAttachmentMeasure();

    void initialize();
    void processNewValue( const Vector3d< int32_t >& site,
                          bool addOrRemove );

  protected:

    RCPointer< Volume< S > > _mriVolume;
    RCPointer< PhiFunction > _phiFunction;
    double _strideFactor;

};


}


#endif
