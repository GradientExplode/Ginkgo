#ifndef _gkg_processing_mesh_GridGenerator_h_
#define _gkg_processing_mesh_GridGenerator_h_


#include <gkg-core-pattern/Singleton.h>
#include <gkg-processing-container/Volume.h>
#include <string>
#include <set>


namespace gkg
{


class GridGenerator : public Singleton< GridGenerator >
{

  public:

    ~GridGenerator();

    std::vector< Vector3d< float > > create( const std::string& gridTypeName,
                                              Vector3d< int32_t > size,
                                              Vector3d< float > spacing,
                                              Vector3d< float > offset ) const;

  protected:

    friend class Singleton< GridGenerator >;

    GridGenerator();

    void getCubicGrid( Vector3d< int32_t > size,
                        Vector3d< float > spacing,
                        Vector3d< float > offset,
                        std::vector< Vector3d< float > >& grid ) const;

    void getHexagonalGrid( Vector3d< int32_t > size,
                            Vector3d< float > spacing,
                            Vector3d< float > offset,
                            std::vector< Vector3d< float > >& grid ) const;

    std::set< std::string > _gridTypeNameList; 

};


}

#endif
