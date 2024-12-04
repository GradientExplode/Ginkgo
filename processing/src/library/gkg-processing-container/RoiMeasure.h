#ifndef _gkg_processing_container_RoiMeasure_h_
#define _gkg_processing_container_RoiMeasure_h_

#include <gkg-processing-container/Roi.h>
#include <gkg-core-pattern/RCPointer.h>
#include <gkg-processing-coordinates/Vector3d.h>


namespace gkg
{


template < class T >
class RoiMeasure
{

  public:

    RoiMeasure( RCPointer< Roi< T > > roi );
    RoiMeasure( const RoiMeasure< T >& other );
    virtual ~RoiMeasure();

    const double& getValue() const;
    const double& getNewValue() const;
    RCPointer< Roi< T > > getRoi() const;

    virtual void initialize() = 0;
    virtual void processNewValue( const Vector3d< int32_t >& /*site*/,
                                  bool /*addOrRemove*/ ) = 0;
    virtual void applyNewValue();
    virtual void resetNewValue();

  protected:

    RCPointer< Roi< T > > _roi;
    double _value;
    double _newValue;

};

}

#endif
