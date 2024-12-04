#ifndef _gkg_processing_morphology_Neighborhood3d_h_
#define _gkg_processing_morphology_Neighborhood3d_h_


#include <list>
#include <gkg-core-pattern/Singleton.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-processing-container/Volume.h>


namespace gkg
{


class Neighborhood3d : public Singleton< Neighborhood3d >
{

  public:

    enum Type
    {

      Neighborhood2d_4xy,
      Neighborhood2d_4xz,
      Neighborhood2d_4yz,
      Neighborhood2d_8xy,
      Neighborhood2d_8xz,
      Neighborhood2d_8yz,
      Neighborhood3d_6,
      Neighborhood3d_18,
      Neighborhood3d_26,
      Neighborhood3d_Cube,
      Neighborhood3d_Sphere,
      Neighborhood3d_DistanceFromVoxelBoundaries

    };

    ~Neighborhood3d();

    std::list< Vector3d< int32_t > >
      getNeighborOffsets( Type type,
                          int32_t stride = 1,
                          float scalarParameter = 1.0,
                          Volume< int16_t >* neighborhoodMask = 0 ) const;

  protected:

    friend class Singleton< Neighborhood3d >;

    Neighborhood3d();

};


}


#endif

