#ifndef _gkg_processing_roi_ProbabilisticAtlasMeasure_h_
#define _gkg_processing_roi_ProbabilisticAtlasMeasure_h_


#include <gkg-processing-container/RoiMeasure.h>
#include <gkg-processing-roi/AtlasInformation.h>
#include <gkg-processing-container/BoundingBox.h>
#include <gkg-processing-transform/Transform3d.h>
#include <string>


namespace gkg
{


template < class T, class A >
class ProbabilisticAtlasMeasure : public RoiMeasure< T >
{

  public:

    ProbabilisticAtlasMeasure(
                     RCPointer< Roi< T > > roi,
                     RCPointer< Volume< A > > atlas,
                     const std::string& structureDictionaryName,
                     const std::string& statisticalDictionaryName,
                     int32_t dataBaseSize,
                     RCPointer< Transform3d< float > > labelToAtlasTranform3d );
    virtual ~ProbabilisticAtlasMeasure();

    void initialize();
    void processNewValue( const Vector3d< int32_t >& site,
                          bool addOrRemove );

  protected:

    RCPointer< Volume< A > > _atlas;
    AtlasInformation< std::string > _atlasInformation;
    BoundingBox< int32_t > _atlasBoundingBox;
    int32_t _dataBaseSize;
    RCPointer< Transform3d< float > > _labelToAtlasTransform3d;
    double _strideFactor;

    // for optimization
    int32_t _stride;
    int32_t _dataBaseSizePlusOne;
    double _ultraSmallContribution;
    double _verySmallContribution;
    std::vector< double > _contributions;

};


}


#endif
