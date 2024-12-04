#ifndef _gkg_processing_algobase_Scaler_h_
#define _gkg_processing_algobase_Scaler_h_


#include <vector>
#include <gkg-processing-numericalanalysis/Vector.h>


namespace gkg
{


template < class T > class Volume;


//
// class Scaler< T >
//

template < class T >
class Scaler
{

  public:

    Scaler( const T& scale );

    void scale( T& in ) const;

  private:

    T _scale;

};


//
// class Scaler< std::vector< T > >
//

template < class T >
class Scaler< std::vector< T > >
{

  public:

    Scaler( const T& scale );

    void scale( std::vector< T >& in ) const;

  private:

    Scaler< T > _itemScaler;

};


//
// class Scaler< Vector >
//

template <>
class Scaler< gkg::Vector >
{

  public:

    Scaler( const double& scale );

    void scale( gkg::Vector& in ) const;

  private:

    Scaler< double > _itemScaler;

};


//
// class Scaler< Volume< T > >
//

template < class T >
class Scaler< Volume< T > >
{

  public:

    Scaler( const T& scale );

    void scale( Volume< T >& in ) const;

  private:

    Scaler< T > _itemScaler;

};


}


#endif
