#include <gkg-graphics-X11-base/LogScaleDivider.h>
#include <gkg-graphics-X11-base/LinearScaleDivider.h>
#include <gkg-processing-algobase/Math.h>
#include <algorithm>
#include <cstdio>


template < class T >
const T gkg::LogScaleDivider< T >::LogMin = ( T )1.0e-38;
template < class T >
const T gkg::LogScaleDivider< T >::LogMax = ( T )1.0e+38;



template < class T >
gkg::LogScaleDivider< T >::LogScaleDivider( const T& lowerBound,
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

  // boundary check
  this->limitRange( this->_lowerBound,
                    gkg::LogScaleDivider< T >::LogMin,
                    gkg::LogScaleDivider< T >::LogMax );
  this->limitRange( this->_upperBound,
                    gkg::LogScaleDivider< T >::LogMin,
                    gkg::LogScaleDivider< T >::LogMax );

  if ( this->_lowerBound != this->_upperBound )
  {

    T width = ( T )( std::log10( this->_upperBound ) -
                     std::log10( this->_lowerBound ) );

    // scale width is less than one decade -> build linear scale
    if ( width < ( T )1.0 )
    {

      gkg::LinearScaleDivider< T >*
        sd = new gkg::LinearScaleDivider< T >( this->_lowerBound,
                                               this->_upperBound,
                                               majorCount,
                                               minorCount,
                                               ( T )0 );
      if ( sd->getMajorStep() > 0.0f )
      {

        this->_majorStep = ( T )std::log10( sd->getMajorStep() );

      }
      for ( i = 0; i < sd->getMajorCount(); i++ )
      {

        this->_majorMarks.push_back( sd->getMajorMark( i ) );

      }
      for ( i = 0; i < sd->getMinorCount(); i++ )
      {

        this->_minorMarks.push_back( sd->getMinorMark( i ) );

      }
      delete sd;

    }
    else
    {

      // major divisions
      if ( this->_majorStep == 0.0f )
      {

        this->_majorStep = ( T )gkg::ceil125( width * 0.999999 / majorCount );

      }
      this->_majorStep = ( T )std::max( this->_majorStep, ( T )1.0 );

      T first = ( T )std::ceil( ( std::log10( this->_lowerBound ) -
                                 gkg::ScaleDivider< T >::StepEpsilon *
                                 this->_majorStep ) / this->_majorStep ) *
                                 this->_majorStep;
      T last = ( T )std::floor( ( std::log10( this->_upperBound ) +
                                 gkg::ScaleDivider< T >::StepEpsilon *
                                 this->_majorStep ) / this->_majorStep ) *
                                 this->_majorStep;

      T firstTick = ( T )std::pow( ( T )10.0, ( T )first );
      T lastTick = ( T )std::pow( ( T )10.0, last );

      int32_t nMaj = std::min( 10000,
                               ( int32_t )( std::floor( ( last - first ) /
                                               this->_majorStep + 0.5 ) ) + 1 );
      this->_majorMarks.resize( nMaj );
      this->_majorMarks[ 0 ] = firstTick;
      for ( i = 1; i < nMaj - 1; i++ )
      {

        this->_majorMarks[ i ] = ( T )std::exp( std::log( firstTick ) +
                                         ( float )i *
                                         ( ( std::log( lastTick ) -
                                             std::log( firstTick ) ) /
                                           ( float )( nMaj - 1 ) ) );

      }
      this->_majorMarks[ nMaj - 1 ] = lastTick;

      // minor divisions
      if ( ( this->_majorMarks.size() >= 1 ) && ( minorCount >= 1 ) )
      {

        if ( this->_majorStep < ( T )1.1 )   // major step width is one decade
        {

          int32_t k0, kmax, kstep;
          if ( minorCount >= 8 )
          {

            k0 = 2;
            kmax = 9;
            kstep = 1;

          }
          else if ( minorCount >= 4 )
          {

            k0 = 2;
            kmax = 8;
            kstep = 2;

          }
          else if ( minorCount >= 2 )
          {

            k0 = 2;
            kmax = 5;
            kstep = 3;

          }
          else
          {

            k0 = 5;
            kmax = 5;
            kstep = 1;

          }
          // are there minor ticks below the first major tick ?
          int32_t i0 = 0;
          if ( firstTick > this->_lowerBound )
          i0 = -1;

          T val, mval;
          for ( i = i0; i < ( int32_t )this->_majorMarks.size(); i++ )
          {

            if ( i >= 0 )
            {

              val = this->_majorMarks[ i ];
 
            }
            else
            {

              val = this->_majorMarks[ 0 ] / ( T )std::pow( ( T )10.0,
                                                      ( T )this->_majorStep );

            }
            for ( k = k0; k <= kmax; k += kstep )
            {

              mval = val * ( float )k;
              if ( this->limitRange( mval, this->_lowerBound, this->_upperBound,
                                     gkg::ScaleDivider< T >::BorderEpsilon ) )
              {

                this->_minorMarks.push_back( mval );

              }

            }

          }

        }
        else      // major step > one decade
        {

          // substep width in decades, at least one decade
          T minorStep = ( T )gkg::ceil125( ( this->_majorStep - 
                            gkg::ScaleDivider< T >::StepEpsilon *
                             ( this->_majorStep / minorCount ) ) / minorCount );
          minorStep = ( T )std::max( ( T )1.0, minorStep );
          int32_t nMin = std::abs( ( int32_t )floor( this->_majorStep /
                                                     minorStep + 0.5 ) ) - 1;
          // do the minor steps fit into the interval ?
          if ( ( T )std::fabs( ( nMin + 1 ) * minorStep - this->_majorStep ) >
               gkg::ScaleDivider< T >::StepEpsilon * this->_majorStep )
          {

            nMin = 0;

          }
          if ( nMin >= 1 )
          {

            // substep factor = 10^substeps
            T minorFactor = std::max( ( T )std::pow( 10, minorStep ),
                                      ( T )10.0 );

            // are there minor ticks below the first major tick ?
            int32_t i0 = 0;
            if ( firstTick > this->_lowerBound )
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

                val = firstTick / ( T )std::pow( ( T )10.0, this->_majorStep );

              }
              for ( k = 0; k < nMin; k++ )
              {

                val *= minorFactor;
                mval = val;
                if ( this->limitRange( mval, this->_lowerBound,
                                       this->_upperBound,
                                       gkg::ScaleDivider< T >::BorderEpsilon ) )
                {

                  this->_minorMarks.push_back( mval );

                }

              }

            }

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
gkg::LogScaleDivider< T >::~LogScaleDivider()
{
}


template class gkg::LogScaleDivider< float >;
template class gkg::LogScaleDivider< double >;
