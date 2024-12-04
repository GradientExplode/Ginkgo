#ifndef _gkg_dmri_container_DiffusionContinuousField_h_
#define _gkg_dmri_container_DiffusionContinuousField_h_


#include <gkg-processing-container/ContinuousField.h>
#include <gkg-core-pattern/RCPointer.h>
#include <gkg-processing-container/BoundingBox.h>
#include <gkg-processing-coordinates/OrientationSet.h>
#include <vector>


namespace gkg
{


template < class T >
class DiffusionContinuousField : public ContinuousField< T >
{

  public:

    virtual ~DiffusionContinuousField();

    virtual const OrientationSet& getInputOrientationSet() const = 0;
    virtual const std::vector< float > getBValues() const = 0;

  protected:

    DiffusionContinuousField( bool storeItems );

};


}



#endif


