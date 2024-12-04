#ifndef _gkg_processing_roi_FrontPottsMeasure_h_
#define _gkg_processing_roi_FrontPottsMeasure_h_

#include <gkg-processing-container/RoiMeasure.h>
#include <gkg-processing-container/FrontEvolutionRoi.h>
#include <gkg-processing-roi/AtlasInformation.h>
#include <gkg-processing-container/BoundingBox.h>
#include <gkg-processing-numericalanalysis/Matrix.h>
#include <list>
#include <map>


namespace gkg
{


template < class T >
class FrontPottsMeasure : public RoiMeasure< T >
{

  public:

    FrontPottsMeasure(
       RCPointer< FrontEvolutionRoi< T > > frontEvolutionRoi,
       RCPointer< AtlasInformation< std::string > > atlasInformation,
       std::map< T, RCPointer< FrontEvolutionRoi< T > > >& frontEvolutionRois );
    virtual ~FrontPottsMeasure();

    void initialize();
    void processNewValue( const Vector3d< int32_t >& site,
                          bool addOrRemove );
    void setNewValue( double value );

  protected:

    RCPointer< FrontEvolutionRoi< T > > _frontEvolutionRoi;
    RCPointer< AtlasInformation< std::string > > _atlasInformation;
    BoundingBox< int32_t > _boundingBox;
    gkg::Matrix _KPotts;
    std::list< gkg::Vector3d< int32_t > > _neighbors;
    std::map< T, RCPointer< FrontEvolutionRoi< T > > >& _frontEvolutionRois;
    double _strideFactor;

};


}


#endif
