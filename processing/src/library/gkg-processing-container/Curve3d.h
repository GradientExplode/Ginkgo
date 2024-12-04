#ifndef _gkg_processing_container_Curve3d_h_
#define _gkg_processing_container_Curve3d_h_


#include <gkg-core-object/HeaderedObject.h>
#include <gkg-processing-container/LightCurve3d.h>


namespace gkg
{


template < class T >
class Curve3d : public HeaderedObject, public LightCurve3d< T >
{

  public:

    typedef typename std::vector< Vector3d< T > >::iterator iterator;
    typedef typename std::vector< Vector3d< T > >::const_iterator
                                                                 const_iterator;
    typedef typename std::vector< Vector3d< T > >::reverse_iterator
                                                               reverse_iterator;
    typedef typename std::vector< Vector3d< T > >::const_reverse_iterator 
                                                         const_reverse_iterator;

    Curve3d();
    Curve3d( int32_t pointCount );
    Curve3d( const std::list< Vector3d< T > >& points, bool closed = false );
    Curve3d( const std::vector< Vector3d< T > >& points, bool closed = false );
    Curve3d( const Matrix& matrix, bool closed = false );
    Curve3d( const Curve3d< T >& other );
    Curve3d( const LightCurve3d< T >& other );
    virtual ~Curve3d();

    gkg::Curve3d< T >& operator=( const gkg::Curve3d< T >& other );
    gkg::Curve3d< T >& operator=( const gkg::LightCurve3d< T >& other );

    void addPoint( const Vector3d< T >& point );
    void addPoints( const std::list< Vector3d< T > >& points );
    void addPoints( const std::vector< Vector3d< T > >& points );
    void addPoints( const Matrix& matrix );

    void setEquidistantCurve( int32_t pointCount );

    void removePoint( const int32_t& index );
    void removePoints( const std::set< int32_t >& indices );
    void clear();

    void setClosed();

    Curve3d< T > getEquidistantCurve( int32_t pointCount ) const;

  protected:

    void updateHeader();

};


}


#endif
