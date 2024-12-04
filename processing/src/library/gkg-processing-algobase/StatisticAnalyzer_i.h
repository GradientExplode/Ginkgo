#ifndef _gkg_processing_algobase_StatisticAnalyzer_i_h_
#define _gkg_processing_algobase_StatisticAnalyzer_i_h_


#include <gkg-processing-algobase/StatisticAnalyzer.h>
#include <gkg-processing-algobase/StatisticAnalyzerMinimumContext_i.h>
#include <gkg-processing-algobase/StatisticAnalyzerMaximumContext_i.h>
#include <gkg-processing-algobase/StatisticAnalyzerMinMaxContext_i.h>
#include <gkg-communication-thread/ThreadedLoop.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-core-exception/Exception.h>
#include <cmath>
#include <algorithm>


template < class T >
inline
double gkg::StatisticAnalyzer< gkg::Volume< T > >::getAverage(
                                              const gkg::Volume< T >& in ) const
{

  try
  {

    typename gkg::Volume< T >::const_iterator i = in.begin(), ie = in.end();
    double accumulator = 0.0;
    while ( i != ie )
    {

      accumulator += ( double )( *i );
      ++ i;

    }
    return accumulator / ( double )in.getSizeXYZT();

  }
  GKG_CATCH( "template < class T >"
             "inline"
             "double gkg::StatisticAnalyzer< gkg::Volume< T > >::getAverage( "
             "const gkg::Volume< T >& in ) const" );

}


template < class T >
inline
double gkg::StatisticAnalyzer< gkg::Volume< T > >::getStdDev(
                                              const gkg::Volume< T >& in ) const
{

  try
  {

    double average = getAverage( in );
    typename gkg::Volume< T >::const_iterator i = in.begin(), ie = in.end();
    double accumulator = 0.0;
    double error;
    while ( i != ie )
    {

      error = ( double )( *i ) - average;
      accumulator += error * error;
      ++ i;

    }
    return std::sqrt( accumulator / ( double )( in.getSizeXYZT() - 1 ) );

  }
  GKG_CATCH( "template < class T >"
             "inline"
             "double gkg::StatisticAnalyzer< gkg::Volume< T > >::getStdDev( "
             "const gkg::Volume< T >& in ) const" );

}


template < class T >
inline
T gkg::StatisticAnalyzer< gkg::Volume< T > >::getMinimum(
                                              const gkg::Volume< T >& in ) const
{

  try
  {

    int32_t sizeZ = in.getSizeZ();
    T minimum = in( 0 );

    gkg::StatisticAnalyzerMinimumContext< T > context( in, minimum );
    gkg::ThreadedLoop< int32_t >
      threadedLoop( &context,
                    0,
                    sizeZ,
                    in.isMemoryMapped() ? 1 : 0 );

    threadedLoop.launch( true, true );

    return minimum;

  }
  GKG_CATCH( "template < class T >"
             "inline"
             "T gkg::StatisticAnalyzer< gkg::Volume< T > >::getMinimum( "
             "const gkg::Volume< T >& in ) const" );

}


template < class T >
inline
T gkg::StatisticAnalyzer< gkg::Volume< T > >::getMaximum(
                                              const gkg::Volume< T >& in ) const
{

  try
  {

    int32_t sizeZ = in.getSizeZ();
    T maximum = in( 0 );

    gkg::StatisticAnalyzerMaximumContext< T > context( in, maximum );
    gkg::ThreadedLoop< int32_t >
      threadedLoop( &context,
                    0,
                    sizeZ,
                    in.isMemoryMapped() ? 1 : 0 );

    threadedLoop.launch( true, true );

    return maximum;

  }
  GKG_CATCH( "template < class T >"
             "inline"
             "T gkg::StatisticAnalyzer< gkg::Volume< T > >::getMaximum( "
             "const gkg::Volume< T >& in ) const" );

}


template < class T >
inline
void gkg::StatisticAnalyzer< gkg::Volume< T > >::getMinMax(
                                                     const gkg::Volume< T >& in,
                                                     T& minimum,
                                                     T& maximum ) const
{

  try
  {

    int32_t sizeZ = in.getSizeZ();
    
    minimum = in( 0 );
    maximum = minimum;

    gkg::StatisticAnalyzerMinMaxContext< T > context( in, minimum, maximum );
    gkg::ThreadedLoop< int32_t >
      threadedLoop( &context,
                    0,
                    sizeZ,
                    in.isMemoryMapped() ? 1 : 0 );

    threadedLoop.launch( true, true );

  }
  GKG_CATCH( "template < class T >"
             "inline"
             "void gkg::StatisticAnalyzer< gkg::Volume< T > >::getMinMax( "
             "const gkg::Volume< T >& in, T& minimum, T& maximum ) const" );

}


template < class T >
inline
T gkg::StatisticAnalyzer< gkg::Volume< T > >::getCorrectedMaximum(
                                            const gkg::Volume< T >& in,
                                            int32_t thrown, int32_t kept ) const
{

  try
  {

    return ( double )getDoubleCorrectedMaximum( in, thrown, kept );

  }
  GKG_CATCH( "template < class T >"
             "inline"
             "double "
             "gkg::StatisticAnalyzer< gkg::Volume< T > >::getCorrectedMaximum( "
             "const gkg::Volume< T >& in, "
             "int thrown, int kept ) const" );

}



template < class T >
inline
double gkg::StatisticAnalyzer< gkg::Volume< T > >::getDoubleMinimum(
                                              const gkg::Volume< T >& in ) const
{

  try
  {

    return ( double )getMinimum( in );

  }
  GKG_CATCH( "template < class T >"
             "inline"
             "double "
             "gkg::StatisticAnalyzer< gkg::Volume< T > >::getDoubleMinimum( "
             "const gkg::Volume< T >& in ) const" );

}


template < class T >
inline
double gkg::StatisticAnalyzer< gkg::Volume< T > >::getDoubleMaximum(
                                              const gkg::Volume< T >& in ) const
{

  try
  {

    return ( double )getMaximum( in );

  }
  GKG_CATCH( "template < class T >"
             "inline"
             "double "
             "gkg::StatisticAnalyzer< gkg::Volume< T > >::getDoubleMaximum( "
             "const gkg::Volume< T >& in ) const" );

}


template < class T >
inline
double 
gkg::StatisticAnalyzer< gkg::Volume< T > >::getDoubleCorrectedMaximum(
                                            const gkg::Volume< T >& in,
                                            int32_t thrown, int32_t kept ) const
{

  try
  {

    // thresholds : max , 95%, 90%, 85%, 80% (first attempt)
    double thresholds[ 5 ];  
    thresholds[ 0 ] = getDoubleMaximum( in );

    float percentageStep = 0.05; 
    float firstPercentage = 1.00;

    bool ok = false;

    std::vector< std::vector< double > > storage( 4 );
    int32_t s;
    for ( s = 0; s < 4; s++ )
    {

      storage[ s ] = std::vector< double >( 10 * kept, 0.0 );

    }

    int32_t pointCount[ 4 ];

    int32_t t;
    int32_t firstNotFull;
    bool flag;
    int32_t goodIndex;
    double correctedMaximum = 0.0;
    while ( !ok ) 
    {

      pointCount[ 0 ] =
      pointCount[ 1 ] =
      pointCount[ 2 ] =
      pointCount[ 3 ] = 0;

      // setting the thresholds
      for ( t = 1; t < 5; t++ )
      {
      
        thresholds[ t ] = ( firstPercentage - percentageStep * t ) *
                          thresholds[ 0 ];

      }

      if ( percentageStep < 0.001 ) 
      {

        return thresholds[ 0 ];

      }

      firstNotFull = 4;
      typename gkg::Volume< T >::const_iterator i = in.begin(), ie = in.end();
      while ( i != ie )
      { 

        flag = true;	  
        for ( t = firstNotFull; ( ( flag == true ) && ( t > 0 ) ); t-- ) 
        {

          if ( pointCount[ t - 1 ] < 10 * kept ) 
          {

            if ( *i > thresholds[ t - 1 ] ) 
            {

              storage[ t - 1 ][ pointCount[ t - 1 ] ] = ( double )*i;
              ++ pointCount[ t - 1 ];

            } 
            else 
            {

              flag = false;

            }
	       
          }
          else 
          {

            -- firstNotFull;

          }

        }
        ++ i;      

      }

      // looking for a storage vect with more than KEPT values and not FULL 
      if ( firstNotFull == 0 ) 
      { 

        // all the storage vectors are full
        percentageStep /= 4.0;

      } 
      else 
      {
    	
        goodIndex = 0;
        while ( ( pointCount[ goodIndex ] < kept ) && ( goodIndex < 4 ) )
        {
        
    	  ++ goodIndex;
    	
        }
       
        if ( goodIndex == 4 )
        {

          // all the storage vectors have less than KEPT values 
          firstPercentage -= 4.0 * percentageStep;

        } 
        else
        {

          if ( pointCount[ goodIndex ] == ( 10 * kept ) ) 
    	  {

    	    // the percentage step is too large 
            firstPercentage -= goodIndex * percentageStep;
            percentageStep /= 4.0;

    	  } 
    	  else 
          {

    	    std::sort( storage[ goodIndex ].begin(),
                       storage[ goodIndex ].end() );

            correctedMaximum = 0.0;
            int32_t count = 0;
            for ( s = 9 * kept; s < 10 * kept - thrown; s++ )
            {
            
              correctedMaximum += storage[ goodIndex ][ s ];
              ++ count;
            
            }
    	    if ( count > 0 )
            {
          
    	      correctedMaximum /= ( double )count;
              ok = true;
    
    	    }	 

          }

        }

      }

    }
    return correctedMaximum;

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "double gkg::StatisticAnalyzer< gkg::Volume< T > >::"
             "getCorrectedMaximum( const gkg::Volume< T >& in, "
             "int thrown, int kept ) const" );

}


template < class T >
inline
double gkg::StatisticAnalyzer< gkg::Volume< T > >::getNoiseAverage(
                                                    const gkg::Volume< T >& in,
                                                    double noisePosition ) const
{

  try
  {

    int32_t x, y, z;
    double accumulator = 0.0;
    int32_t count = 0;
    for ( z = 0; z < in.getSizeZ(); z++ )
    {

      for ( y = 0; y < in.getSizeY(); y++ )
      {

        for ( x = ( int32_t )( noisePosition * in.getSizeX() );
              x < in.getSizeX(); x++ )
        {

          accumulator += ( double )in( x, y, z );
          ++ count;

        }

      }

    }
    return accumulator / ( double )count;

  }
  GKG_CATCH( "template < class T >"
             "inline double"
             "gkg::StatisticAnalyzer< gkg::Volume< T > >::getNoiseAverage( "
             "const gkg::Volume< T >& in, "
             "double noisePosition ) const" );

}


template < class T >
inline
double gkg::StatisticAnalyzer< gkg::Volume< T > >::getNoiseStdDev(
                                                    const gkg::Volume< T >& in,
                                                    double noisePosition ) const
{

  try
  {

    double average = getNoiseAverage( in, noisePosition );

    int32_t x, y, z;
    double accumulator = 0.0;
    double error;
    int32_t count = 0;
    for ( z = 0; z < in.getSizeZ(); z++ )
    {

      for ( y = 0; y < in.getSizeY(); y++ )
      {

        for ( x = ( int32_t )( noisePosition * in.getSizeX() ); 
              x < in.getSizeX(); x++ )
        {

          error = ( double )in( x, y, z ) - average;
          accumulator += error * error;
          ++ count;

        }

      }

    }
    return std::sqrt( accumulator / ( double )( count - 1 ) );

  }
  GKG_CATCH( "template < class T >"
             "inline double"
             "gkg::StatisticAnalyzer< gkg::Volume< T > >::getNoiseStdDev( "
             "const gkg::Volume< T >& in, "
             "double noisePosition ) const" );

}


template < class T >
inline
double 
gkg::StatisticAnalyzer< gkg::Volume< T > >::getDoubleAutomaticThreshold(
                                            const gkg::Volume< T >& in,
                                            double noisePosition,
                                            double noiseRatio,
                                            int32_t thrown, int32_t kept ) const
{

  try
  {

    double correctedMaximum = getDoubleCorrectedMaximum( in, thrown, kept );
    double noiseAverage = getNoiseAverage( in, noisePosition );
    double noiseStdDev = getNoiseStdDev( in, noisePosition );
    double minimum = noiseAverage + 4.0 * noiseStdDev;
    return ( minimum + noiseRatio * ( correctedMaximum - minimum ) );

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "double gkg::StatisticAnalyzer< gkg::Volume< T > >::"
             "getDoubleAutomaticThreshold( const gkg::Volume< T >& in, "
             "double noisePosition, "
             "double noiseRatio, "
             "int thrown, int kept ) const" );

}


template < class T >
inline
T 
gkg::StatisticAnalyzer< gkg::Volume< T > >::getAutomaticThreshold(
                                            const gkg::Volume< T >& in,
                                            double noisePosition,
                                            double noiseRatio,
                                            int32_t thrown, int32_t kept ) const
{

  try
  {

    return ( T )getDoubleAutomaticThreshold( in,
                                             noisePosition,
                                             noiseRatio,
                                             thrown,
                                             kept );

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "double gkg::StatisticAnalyzer< gkg::Volume< T > >::"
             "getAutomaticThreshold( const gkg::Volume< T >& in, "
             "double noisePosition, "
             "double noiseRatio, "
             "int thrown, int kept ) const" );

}




#endif
