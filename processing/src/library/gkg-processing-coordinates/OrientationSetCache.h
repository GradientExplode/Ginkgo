#ifndef _gkg_processing_coordinates_OrientationSetCache_h_
#define _gkg_processing_coordinates_OrientationSetCache_h_


#include <gkg-communication-thread/LoopContext.h>
#include <gkg-core-pattern/Singleton.h>
#include <gkg-core-cppext/StdInt.h>
#include <vector>


namespace gkg
{


class OrientationSet;


class OrientationSetCacheContext : public LoopContext< int32_t >
{

  public:

    OrientationSetCacheContext(
      const OrientationSet& orientationSet,
      std::vector< std::vector< int32_t > >& neighborCounts,
      std::vector< std::vector< std::vector< int32_t > > >& neighborIndices,
      std::vector< std::vector< int32_t > >& solidAngleOrientationCounts,
      std::vector< std::vector< std::vector< int32_t > > >&
                                                  solidAngleOrientationIndices,
      std::vector< std::vector< float > >& dotProducts,
      std::vector< std::vector< float > >& vectorAngles,
      std::vector< std::vector< float > >& lineAngles );

    void doIt( int32_t startIndex, int32_t count );

  private:

    const OrientationSet& _orientationSet;
    int32_t _orientationCount;

    std::vector< std::vector< int32_t > >& _neighborCounts;
    std::vector< std::vector< std::vector< int32_t > > >& _neighborIndices;
    std::vector< std::vector< int32_t > >& _solidAngleOrientationCounts;
    std::vector< std::vector< std::vector< int32_t > > >&
                                                  _solidAngleOrientationIndices;

    std::vector< std::vector< float > >& _dotProducts;
    std::vector< std::vector< float > >& _vectorAngles;
    std::vector< std::vector< float > >& _lineAngles;

};


class OrientationSetCache : public Singleton< OrientationSetCache >
{

  public:

    ~OrientationSetCache();

    void update( const OrientationSet& orientationSet,
                  float apertureAngle,
                  bool verbose = false );

    float getApertureAngle() const;

    const OrientationSet* getOrientationSet() const;

    int32_t getNeighborCount( int32_t orientationIndex ) const;
    const std::vector< int32_t >& 
      getNeighborIndices( int32_t orientationIndex ) const;
    int32_t getNeighborIndex( int32_t orientationIndex,
                              int32_t neighborIndex ) const;

    int32_t getSolidAngleOrientationCount( int32_t orientationIndex ) const;
    const std::vector< int32_t >& 
      getSolidAngleOrientationIndices( int32_t orientationIndex ) const;
    int32_t getSolidAngleOrientationIndex(
                                     int32_t orientationIndex,
                                     int32_t solidAngleOrientationIndex ) const;

    float getDotProduct( int32_t orientationIndex1,
                         int32_t orientationIndex2 ) const;
    float getVectorAngle( int32_t orientationIndex1,
                          int32_t orientationIndex2 ) const;
    float getLineAngle( int32_t orientationIndex1,
                        int32_t orientationIndex2 ) const;

  protected:

    friend class Singleton< OrientationSetCache >;

    OrientationSetCache();

    const OrientationSet* _orientationSet;
    int32_t _apertureAngleIndex;

    std::vector< std::vector< int32_t > > _neighborCounts;
    std::vector< std::vector< std::vector< int32_t > > > _neighborIndices;
    std::vector< std::vector< int32_t > > _solidAngleOrientationCounts;
    std::vector< std::vector< std::vector< int32_t > > >
                                                  _solidAngleOrientationIndices;

    std::vector< std::vector< float > > _dotProducts;
    std::vector< std::vector< float > > _vectorAngles;
    std::vector< std::vector< float > > _lineAngles;

};


}



#endif
