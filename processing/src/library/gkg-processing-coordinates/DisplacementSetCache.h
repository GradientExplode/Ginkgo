#ifndef _gkg_processing_coordinates_DisplacementSetCache_h_
#define _gkg_processing_coordinates_DisplacementSetCache_h_


#include <gkg-core-pattern/Singleton.h>
#include <gkg-processing-coordinates/OrientationSet.h>
#include <gkg-communication-thread/Mutex.h>
#include <vector>
#include <map>


namespace gkg
{


class DisplacementSet;


class DisplacementSetCache : public Singleton< DisplacementSetCache >
{

  public:

    ~DisplacementSetCache();

    void update(
            const std::map< float, OrientationSet >& multipleShellSampling );
    void update( const Vector3d< int32_t >& cartesianGridSize,
                 const Vector3d< double >& cartesianGridResolution );

    const std::map< float, OrientationSet >* getMultiShellSampling() const;
    const Vector3d< int32_t >* getCartesianGridSize() const;
    const Vector3d< double >* getCartesianGridResolution() const;

    int32_t getCount() const;
    const Vector3d< float >& getDisplacement( int32_t index ) const;
    const std::vector< Vector3d< float > >& getDisplacements() const;
    int32_t getNearestDisplacementIndex(
                                  const Vector3d< float >& displacement ) const;

  protected:

    friend class Singleton< DisplacementSetCache >;

    DisplacementSetCache();

    const std::map< float, OrientationSet >* _multipleShellSampling;
    const Vector3d< int32_t >* _cartesianGridSize;
    const Vector3d< double >* _cartesianGridResolution;

    std::vector< gkg::Vector3d< float > > _displacements;
    Mutex _mutex;

};


}



#endif
