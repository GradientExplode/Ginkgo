#ifndef _gkg_graphics_QtGL_slicing_Slicing_h_
#define _gkg_graphics_QtGL_slicing_Slicing_h_


#include <gkg-graphics-QtGL-slicing/CubeZBuffer.h>
#include <gkg-core-pattern/Singleton.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-core-cppext/StdInt.h>
#include <map>
#include <list>


namespace gkg
{


template < class T >
class Slicing : public Singleton< Slicing< T > >
{

  public:

    void setMaxSlices( int32_t nbSlices );
    std::list< Vector3d< T > > getSlice( const T* m, T z );
    std::map< T, std::list< Vector3d< T > > >& getSlices( const T* m );
    std::map< T, std::list< Vector3d< T > > >& getSlab( const T* m, 
                                                        T z,
                                                        int32_t nbSlices );

  protected:

    friend class Singleton< Slicing< T > >;

    Slicing();

  private:

    void getOneSlice( T z );

    static T _vertex[ 8 ][ 3 ];
    static int32_t _edge[ 12 ][ 2 ];
    static int32_t _intersection[ 256 ][ 8 ];

    int32_t _maxNbSlices;
    CubeZBuffer< T > _cubeZBuffer;
    std::map< T, std::list< Vector3d< T > > > _slices;

};


}


#endif
