#ifndef _gkg_processing_algobase_Flipper_h_
#define _gkg_processing_algobase_Flipper_h_


#include <list>


namespace gkg
{


template < class T > class Volume;


//
// class Flipper< T >
//

template < class T >
class Flipper
{

  public:

    enum Type
    {

      SymmetryAlongX,
      SymmetryAlongY,
      SymmetryAlongZ,
      SymmetryAlongT,
      FlippingXY,
      FlippingXZ,
      FlippingXT,
      FlippingYZ,
      FlippingYT,
      FlippingZT,

    };

    Flipper( const std::list< Type >& types );
    virtual ~Flipper();

    void flip( const T& in, T& out ) const;

  private:

    std::list< Type > _types;

};




//
// class Flipper< Volume< T > >
//

template < class T >
class Flipper< Volume< T > >
{

  public:

    enum Type
    {

      SymmetryAlongX,
      SymmetryAlongY,
      SymmetryAlongZ,
      SymmetryAlongT,
      FlippingXY,
      FlippingXZ,
      FlippingXT,
      FlippingYZ,
      FlippingYT,
      FlippingZT,

    };

    Flipper( const std::list< Type >& types );
    virtual ~Flipper();

    void flip( const Volume< T >& in, Volume< T >& out ) const;

  private:

    void symmetryAlongX( const Volume< T >& in, Volume< T >& out ) const;
    void symmetryAlongY( const Volume< T >& in, Volume< T >& out ) const;
    void symmetryAlongZ( const Volume< T >& in, Volume< T >& out ) const;
    void symmetryAlongT( const Volume< T >& in, Volume< T >& out ) const;
    void flippingXY( const Volume< T >& in, Volume< T >& out ) const;
    void flippingXZ( const Volume< T >& in, Volume< T >& out ) const;
    void flippingXT( const Volume< T >& in, Volume< T >& out ) const;
    void flippingYZ( const Volume< T >& in, Volume< T >& out ) const;
    void flippingYT( const Volume< T >& in, Volume< T >& out ) const;
    void flippingZT( const Volume< T >& in, Volume< T >& out ) const;

    std::list< Type > _types;

};


}


#endif
