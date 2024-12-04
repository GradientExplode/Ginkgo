#ifndef _gkg_processing_container_Curve3dMap_h_
#define _gkg_processing_container_Curve3dMap_h_


#include <gkg-core-object/HeaderedObject.h>
#include <gkg-processing-container/Curve3d.h>
#include <gkg-processing-container/BoundingBox.h>
#include <vector>
#include <list>
#include <iostream>


namespace gkg
{


template < class T >
class Curve3dMap : public HeaderedObject
{

  public:

    typedef typename std::vector< LightCurve3d< T > >::iterator iterator;
    typedef typename std::vector< LightCurve3d< T > >::const_iterator
                                                                 const_iterator;
    typedef typename std::vector< LightCurve3d< T > >::reverse_iterator
                                                               reverse_iterator;
    typedef typename std::vector< LightCurve3d< T > >::const_reverse_iterator 
                                                         const_reverse_iterator;

    Curve3dMap();
    Curve3dMap( int32_t curve3dCount );
    Curve3dMap( const std::list< LightCurve3d< T > >& lightCurve3ds );
    Curve3dMap( const std::vector< LightCurve3d< T > >& lightCurve3ds );
    Curve3dMap( const std::list< Curve3d< T > >& curve3ds );
    Curve3dMap( const std::vector< Curve3d< T > >& curve3ds );
    Curve3dMap( const Curve3dMap< T >& other );
    virtual ~Curve3dMap();

    Curve3dMap< T >& operator=( const Curve3dMap< T >& other );

    void reserve( int32_t curve3dCount );

    iterator begin();
    iterator end();

    const_iterator begin() const;
    const_iterator end() const;

    reverse_iterator rbegin();
    reverse_iterator rend();

    const_reverse_iterator rbegin() const;
    const_reverse_iterator rend() const;

    void addCurve3d( const LightCurve3d< T >& lightCurve3d );
    void setCurve3d( int32_t index, const LightCurve3d< T >& lightCurve3d );
    void addCurve3ds( const std::list< LightCurve3d< T > >& lightCurve3ds );
    void addCurve3ds( const std::vector< LightCurve3d< T > >& lightCurve3ds );
    void addCurve3ds( const std::list< Curve3d< T > >& curve3ds );
    void addCurve3ds( const std::vector< Curve3d< T > >& curve3ds );

    void removeCurve3d( const int32_t& index );
    void removeCurve3ds( const std::set< int32_t >& indices );
    void clear();

    LightCurve3d< T >& operator[]( int32_t );
    const LightCurve3d< T >& operator[]( int32_t ) const;
    const LightCurve3d< T >& getCurve3d( int32_t index ) const;
    const std::vector< LightCurve3d< T > >& getCurve3ds() const;
    int32_t getCurve3dCount() const;

    void setEquidistant( int32_t equidistantPointCount );
    T getInnerDistance() const;
    bool allCurvesHaveSamePointCount() const;
    LightCurve3d< T > getAverage( int32_t excludedIndex = - 1 ) const;

    T getCurve3dDistanceToCurve3dMap(
                                  const LightCurve3d< T >& lightCurve3d ) const;
    T getCurve3dDistanceToCurve3dMap(  int32_t excludedIndex  ) const;
    T getCurve3dMaximumDistanceToCurve3dMap(
                                  const LightCurve3d< T >& lightCurve3d ) const;
    T getCurve3dMaximumDistanceToCurve3dMap( int32_t excludedIndex ) const;

    int32_t getClosestCurve3dIndexByMinMaxDistance(
                                  const LightCurve3d< T >& lightCurve3d ) const;
    int32_t getClosestCurve3dIndexByMinMaxDistance(
                                                  int32_t excludedIndex ) const;

    BoundingBox< T > getBoundingBox() const;

  protected:

    void updateHeader();

    std::vector< LightCurve3d< T > > _curve3ds;

};


}


namespace std
{


ostream& operator<<( ostream& os, const gkg::Curve3dMap< float >& thing );
ostream& operator<<( ostream& os, const gkg::Curve3dMap< double >& thing );


}


#endif
