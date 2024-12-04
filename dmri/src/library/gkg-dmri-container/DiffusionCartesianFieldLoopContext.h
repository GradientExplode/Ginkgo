#ifndef _gkg_dmri_container_DiffusionCartesianFieldLoopContext_h_
#define _gkg_dmri_container_DiffusionCartesianFieldLoopContext_h_


#include <gkg-communication-thread/LoopContext.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-processing-container/BoundingBox.h>
#include <vector>


namespace gkg
{


template < class T > class DiffusionCartesianField;
class DiffusionCartesianFieldGauge;


template < class T >
class DiffusionCartesianFieldLoopContext : public LoopContext< int32_t >
{

  public:

    enum Type
    {

      CalculateOnGrid,
      CalculateOnSubGrid

    };

    DiffusionCartesianFieldLoopContext(
                  DiffusionCartesianFieldGauge& gauge,
                  const std::vector< Vector3d< int32_t > >& sites,
                  const BoundingBox< int32_t >& boundingBox,
                  DiffusionCartesianField< T >& diffusionCartesianField,
                  bool& verbose );
    DiffusionCartesianFieldLoopContext(
                  DiffusionCartesianFieldGauge& gauge,
                  const std::vector< Vector3d< int32_t > >& sites,
                  const BoundingBox< float >& boundingBox,
                  const Vector3d< float >& superResolution,
                  DiffusionCartesianField< T >& diffusionCartesianField,
                  bool& verbose );
    virtual ~DiffusionCartesianFieldLoopContext();

    void doIt( int32_t startIndex, int32_t count );

  private:

    const std::vector< Vector3d< int32_t > >& _sites;
    BoundingBox< int32_t > _boundingBoxInt32;
    BoundingBox< float > _boundingBoxFloat;
    Vector3d< float > _superResolution;
    DiffusionCartesianField< T >& _diffusionCartesianField;
    bool& _verbose;
    Type _type;

};


}


#endif
