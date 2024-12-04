#ifndef _gkg_processing_algobase_Rescaler_h_
#define _gkg_processing_algobase_Rescaler_h_


#include <gkg-core-cppext/Limits.h>
#include <vector>


namespace gkg
{


template < class T > class Volume;


//
// class Rescaler< IN, OUT >
//

template < class IN, class OUT >
class Rescaler
{

  public:

    Rescaler( const IN& imin = std::numeric_limits< IN >::lowest(),
              const IN& imax = std::numeric_limits< IN >::max(),
              const OUT& omin = std::numeric_limits< OUT >::lowest(),
              const OUT& omax = std::numeric_limits< OUT >::max() );

    Rescaler( const Rescaler< IN, OUT >& other );

    Rescaler< IN, OUT >& operator=( const Rescaler< IN, OUT >& other );

    void rescale( const IN& in, OUT& out ) const;

  private:

    IN _imin;
    IN _imax;
    OUT _omin;
    OUT _omax;
    double _scale;

};


//
// class Rescaler< std::vector< IN >, std::vector< OUT > >
//

template < class IN, class OUT >
class Rescaler< std::vector< IN >, std::vector< OUT > >
{

  public:

    Rescaler( const IN& imin = std::numeric_limits< IN >::lowest(),
              const IN& imax = std::numeric_limits< IN >::max(),
              const OUT& omin = std::numeric_limits< OUT >::lowest(),
              const OUT& omax = std::numeric_limits< OUT >::max() );

    void rescale( const std::vector< IN >& in, std::vector< OUT >& out ) const;

  private:

    Rescaler< IN, OUT > _itemRescaler;

};


//
// class Rescaler< Volume< IN >, Volume< OUT > >
//

template < class IN, class OUT >
class Rescaler< Volume< IN >, Volume< OUT > >
{

  public:

    Rescaler( const IN& imin = std::numeric_limits< IN >::lowest(),
              const IN& imax = std::numeric_limits< IN >::max(),
              const OUT& omin = std::numeric_limits< OUT >::lowest(),
              const OUT& omax = std::numeric_limits< OUT >::max() );

    void rescale( const Volume< IN >& in, Volume< OUT >& out ) const;

  private:

    Rescaler< IN, OUT > _itemRescaler;

};


}


#endif
