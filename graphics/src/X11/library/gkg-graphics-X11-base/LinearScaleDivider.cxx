#include <gkg-graphics-X11-base/LinearScaleDivider.h>
#include <gkg-processing-algobase/Math.h>
#include <algorithm>


template < class T >
gkg::LinearScaleDivider< T >::LinearScaleDivider( const T& lowerBound,
                                                  const T& upperBound,
                                                  int32_t majorCount,
                                                  int32_t minorCount, 
                                                  const T& step,
                                                  bool ascend )
                             : gkg::ScaleDivider< T >( lowerBound,
                                                       upperBound,
                                                       step )
{

  int32_t i, k;

  majorCount = std::max( 1, majorCount );
  minorCount = std::max( 0, minorCount );

  if ( lowerBound != upperBound )
  {

    // major divisions
    if ( std::fabs( step ) == 0.0 )
    {

      this->_majorStep = gkg::ceil125( 
                              ( this->_upperBound - this->_lowerBound ) *
                              0.999999 / majorCount );

    }

    T firstTick = ( T )std::ceil( ( this->_lowerBound -
                                   gkg::ScaleDivider< T >::StepEpsilon *
                                   this->_majorStep ) / this->_majorStep ) *
                                   this->_majorStep;
    T lastTick = ( T )std::floor( ( this->_upperBound +
                                   gkg::ScaleDivider< T >::StepEpsilon *
                                   this->_majorStep ) / this->_majorStep ) *
                                   this->_majorStep;
    int32_t nMaj = std::min( 10000,
                         ( int32_t )( std::floor( ( lastTick - firstTick ) /
                                              this->_majorStep + 0.5 ) ) + 1 );
    this->_majorMarks.resize( nMaj );
    this->_majorMarks[ 0 ] = firstTick;
    for ( i = 1; i < nMaj - 1; i++ )
    {

      this->_majorMarks[ i ] = firstTick +
                               ( float )i * ( ( lastTick - firstTick ) /
                                              ( float )( nMaj - 1 ) );

    }
    this->_majorMarks[ nMaj - 1 ] = lastTick;

    // minor divisions
    if ( minorCount >= 1 )
    {

      T minorStep = ( T )gkg::ceil125( this->_majorStep / minorCount );
      int32_t nMin = std::abs( ( int32_t )std::floor( this->_majorStep /
                                                      minorStep + 0.5 ) ) - 1;
      // do the minor steps fit into the interval ?
      if ( ( T )std::fabs( ( nMin + 1 ) * minorStep - this->_majorStep ) >
           gkg::ScaleDivider< T >::StepEpsilon * this->_majorStep )
      {

        nMin = 1;
        minorStep = this->_majorStep * 0.5;

      }
      // are there minor ticks below the first major tick ?
      int32_t i0 = 0;
      if ( this->_majorMarks[ 0 ] > this->_lowerBound )
      {

        i0 = -1;

      }

      T val, mval;
      for ( i = i0; i < ( int32_t )this->_majorMarks.size(); i++ )
      {

        if ( i >= 0 )
        {

          val = this->_majorMarks[ i ];

        }
        else
        {

          val = this->_majorMarks[ 0 ] - this->_majorStep;

        }
        for ( k = 0; k < nMin; k++ )
        {

          val += minorStep;
          mval = val;
          if ( this->limitRange( mval, this->_lowerBound, this->_upperBound,
                                 gkg::ScaleDivider< T >::BorderEpsilon ) )
          {

            this->_minorMarks.push_back( mval );

          }

        }

      }

    }

    // ascent or decrease order
    if ( ( !ascend ) && ( upperBound < lowerBound ) )
    {

      std::swap( this->_lowerBound, this->_upperBound );
      std::reverse( this->_majorMarks.begin(), this->_majorMarks.end() );
      std::reverse( this->_minorMarks.begin(), this->_minorMarks.end() );

    }

  }

}


template < class T >
gkg::LinearScaleDivider< T >::~LinearScaleDivider()
{
}


template class gkg::LinearScaleDivider< float >;
template class gkg::LinearScaleDivider< double >;
