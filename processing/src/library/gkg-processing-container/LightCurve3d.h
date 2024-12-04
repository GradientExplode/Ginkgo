#ifndef _gkg_processing_container_LightCurve3d_h_
#define _gkg_processing_container_LightCurve3d_h_


#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-processing-container/BoundingBox.h>
#include <gkg-processing-container/Volume.h>
#include <gkg-processing-numericalanalysis/Matrix.h>
#include <gkg-processing-transform/Transform3d.h>
#include <vector>
#include <list>
#include <set>
#include <iostream>


namespace gkg
{


template < class T >
class LightCurve3d
{

  public:

    typedef typename std::vector< Vector3d< T > >::iterator iterator;
    typedef typename std::vector< Vector3d< T > >::const_iterator
                                                                 const_iterator;
    typedef typename std::vector< Vector3d< T > >::reverse_iterator
                                                               reverse_iterator;
    typedef typename std::vector< Vector3d< T > >::const_reverse_iterator 
                                                         const_reverse_iterator;

    enum DistanceType
    {

      NonSymmetricMeanClosestPointDistance,
      SymmetricMeanOfMeanClosestPointDistance,
      SymmetricMaximumOfMeanClosestPointDistance,

    };

    LightCurve3d();
    LightCurve3d( int32_t pointCount );
    LightCurve3d( const std::list< Vector3d< T > >& points,
                  bool closed = false );
    LightCurve3d( const std::vector< Vector3d< T > >& points,
                  bool closed = false );
    LightCurve3d( const Matrix& matrix, bool closed = false );
    LightCurve3d( const LightCurve3d< T >& other );
    virtual ~LightCurve3d();

    gkg::LightCurve3d< T >& operator=( const gkg::LightCurve3d< T >& other );

    void reserve( int32_t pointCount );

    iterator begin();
    iterator end();

    const_iterator begin() const;
    const_iterator end() const;

    reverse_iterator rbegin();
    reverse_iterator rend();

    const_reverse_iterator rbegin() const;
    const_reverse_iterator rend() const;

    void addPoint( const Vector3d< T >& point );
    void setPoint( int32_t index, const Vector3d< T >& point );
    void addPoints( const std::list< Vector3d< T > >& points );
    void addPoints( const std::vector< Vector3d< T > >& points );
    void addPoints( const Matrix& matrix );

    void setEquidistantCurve( int32_t pointCount );


    void removePoint( const int32_t& index );
    void removePoints( const std::set< int32_t >& indices );
    void clear();

    const Vector3d< T >& getPoint( int32_t index ) const;
    const std::vector< Vector3d< T > >& getPoints() const;
    int32_t getPointCount() const;

    void setClosed();
    bool isClosed() const;

    T getLength() const;
    T getLength( int32_t fromIndex, int32_t toIndex ) const;

    std::vector< T > getCurvatureProfile() const;
    std::vector< Vector3d< T > > getDirectionProfile() const;
    T getTortuosity() const;
    Vector3d< T > getCenterOfMass() const;
    LightCurve3d< T > getEquidistantCurve( int32_t pointCount ) const;
    LightCurve3d< T > getEquidistantCurvePart(
                                            int32_t fromIndex,
                                            int32_t toIndex,
                                            T maximumResamplingStep ) const;
    LightCurve3d< T > getCurvePart( int32_t fromIndex,
                                    int32_t toIndex ) const;

    T getDistance2( const LightCurve3d< T >& other ) const;
    T getMaximumDistance2( const LightCurve3d< T >& other ) const;

    T getDistance( const LightCurve3d< T >& other,
                   DistanceType distanceType =
                   LightCurve3d< T >::SymmetricMeanOfMeanClosestPointDistance,
                   const std::vector< double >* furtherParameters = 0 ) const;

    Matrix toMatrix() const;
    Matrix toHomogeneousMatrix() const;

    void reverse();

    // return the induced angular dispersion in degrees
    // excludedExtremityPointCount: do not move points outside of the range of
    // indices [  excludedExtremityPointCount; 
    //            pointCount - 1 - excludedExtremityPointCount ]
    T addTortuosity( T magnitude,
                     T angularDispersionInDegrees,
                     T waveLength,
                     const Vector3d< T >& referenceCurveOrientation,
                     const BoundingBox< T >& fieldOfView,
                     int32_t excludedExtremityPointCount = 0,
                     int32_t maximumIterationCount = 100000 );

    
    template < class V >
    void getScalarVolumeProfile(
                     const Volume< V >& scalarVolume,
                     const Transform3d< T >& transform3dFromScalarVolumeToCurve,
                     std::vector< V >& scalarVolumeProfile,
                     const V& backgroundValue,
                     int32_t rank,
                     bool usePartialVolumingInterpolation ) const;

  protected:

    std::vector< Vector3d< T > > _points;
    bool _closed;

};


}


namespace std
{


ostream& operator<<( ostream& os,
                     const gkg::LightCurve3d< float >& thing );
ostream& operator<<( ostream& os,
                     const gkg::LightCurve3d< double >& thing );


}


#endif
