#ifndef _gkg_processing_algobase_RescalerWithSlopeAndIntercept_h_
#define _gkg_processing_algobase_RescalerWithSlopeAndIntercept_h_


#include <gkg-core-cppext/Limits.h>
#include <vector>


namespace gkg
{


template < class T > class Volume;


//
// class RescalerWithSlopeAndIntercept< IN, OUT >
//

template < class IN, class OUT >
class RescalerWithSlopeAndIntercept
{

  public:

    RescalerWithSlopeAndIntercept( double slope  = 1.0,
                                   double intercept = 0.0 );
    RescalerWithSlopeAndIntercept(
                        const RescalerWithSlopeAndIntercept< IN, OUT >& other );

    RescalerWithSlopeAndIntercept< IN, OUT >& 
      operator=( const RescalerWithSlopeAndIntercept< IN, OUT >& other );

    void rescale( const IN& in, OUT& out ) const;

  private:

    double _slope;
    double _intercept;

};


//
// class RescalerWithSlopeAndIntercept< std::vector< IN >, std::vector< OUT > >
//

template < class IN, class OUT >
class RescalerWithSlopeAndIntercept< std::vector< IN >, std::vector< OUT > >
{

  public:

    RescalerWithSlopeAndIntercept( double slope, double intercept );

    void rescale( const std::vector< IN >& in, std::vector< OUT >& out ) const;

  private:

    RescalerWithSlopeAndIntercept< IN, OUT > _itemRescalerWithSlopeAndIntercept;

};


//
// class RescalerWithSlopeAndIntercept< Volume< IN >, Volume< OUT > >
//

template < class IN, class OUT >
class RescalerWithSlopeAndIntercept< Volume< IN >, Volume< OUT > >
{

  public:

    RescalerWithSlopeAndIntercept( double slope, double intercept );

    void rescale( const Volume< IN >& in, Volume< OUT >& out ) const;

  private:

    RescalerWithSlopeAndIntercept< IN, OUT > _itemRescalerWithSlopeAndIntercept;

};


}


#endif
