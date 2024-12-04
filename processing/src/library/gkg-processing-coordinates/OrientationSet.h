#ifndef _gkg_processing_coordinates_OrientationSet_h_
#define _gkg_processing_coordinates_OrientationSet_h_


#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-processing-numericalanalysis/Matrix.h>
#include <vector>


namespace gkg
{


template < class R, class S, uint32_t D, class Compare > class MeshMap;
template < class T > class TextureMap;
template < class T > class Transform3d;


class OrientationSet
{

  public:

    typedef std::vector< Vector3d< float > >::iterator iterator;
    typedef std::vector< Vector3d< float > >::const_iterator const_iterator;
    typedef std::vector< Vector3d< float > >::reverse_iterator reverse_iterator;
    typedef std::vector< Vector3d< float > >::const_reverse_iterator 
                                                         const_reverse_iterator;

    OrientationSet();
    OrientationSet( const std::string& fileName );
    OrientationSet( const std::vector< Vector3d< float > >& orientations );
    OrientationSet( const OrientationSet& other );
    virtual ~OrientationSet();

    OrientationSet& operator=( const OrientationSet& other );

    iterator begin();
    iterator end();

    const_iterator begin() const;
    const_iterator end() const;

    reverse_iterator rbegin();
    reverse_iterator rend();

    const_reverse_iterator rbegin() const;
    const_reverse_iterator rend() const;

    int32_t getCount() const;
    const Vector3d< float >& getOrientation( int32_t index ) const;
    Vector3d< float > getSymmetricalOrientation( int32_t index ) const;
    int32_t getNearestOrientationIndex(
                            const Vector3d< float >& unitaryOrientation ) const;
    std::vector< int32_t > getNearestOrientationIndices(
                                    const Vector3d< float >& unitaryOrientation,
                                    int32_t neighborCount ) const;
    const Vector3d< float >& getNearestSymmetricalOrientation(
                            const Vector3d< float >& unitaryOrientation ) const;
    int32_t getNearestSymmetricalOrientationIndex( int32_t index ) const;

    const std::vector< Vector3d< float > >& getOrientations() const;
    std::vector< Vector3d< float > > getSymmetricalOrientations() const;

    void getStatistics( float* minimumAngle,
                        float* maximumAngle,
                        float* averageAngle,
                        float* angleStdDev ) const;

    void getPerpendicularOrientationIndices( 
                                  int32_t index,
                                  float angleError,
                                  std::vector< int32_t >& indices ) const;

    void getSHToCartesianBasisTransform(
                                       int32_t maximumSphericalHarmonicOrder,
                                       Matrix& shToCartesianBasisMatrix ) const;
    void getCartesianToSHBasisTransform(
                                       int32_t maximumSphericalHarmonicOrder,
                                       Matrix& cartesianToSHBasisMatrix ) const;

    template < class R, class Compare >
    void addMesh( const Transform3d< float >& transform,
                  const R& rank,
                  MeshMap< R, float, 3U, Compare >& meshMap ) const;

    template < class R, class Compare >
    void addMesh( const Transform3d< float >& transform,
                  const R& rank,
                  const TextureMap< float >& scaling,
                  MeshMap< R, float, 3U, Compare >& meshMap ) const;
    template < class R, class Compare >
    void addMesh( const Transform3d< float >& transform,
                  const R& rank,
                  const std::vector< float >& scaling,
                  MeshMap< R, float, 3U, Compare >& meshMap ) const;

    template < class R, class Compare >
    void addSymmetricalMesh( const Transform3d< float >& transform,
                             const R& rank,
                             MeshMap< R, float, 3U, Compare >& meshMap ) const;

    template < class R, class Compare >
    void addSymmetricalMesh( const Transform3d< float >& transform,
                             const R& rank,
                             const TextureMap< float >& scaling,
                             MeshMap< R, float, 3U, Compare >& meshMap ) const;
    template < class R, class Compare >
    void addSymmetricalMesh( const Transform3d< float >& transform,
                             const R& rank,
                             const std::vector< float >& scaling,
                             MeshMap< R, float, 3U, Compare >& meshMap ) const;

  protected:

    OrientationSet( int32_t count );

    std::vector< Vector3d< float > > _orientations;
    std::vector< int32_t > _nearestSymmetricalOrientationIndices;

};


}


#endif
