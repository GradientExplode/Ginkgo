#include <gkg-processing-histogram/HistogramAnalyzer.h>
#include <gkg-processing-histogram/HistogramContext_i.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-algobase/MinMaxFilter_i.h>
#include <gkg-core-exception/Exception.h>
#include <set>
#include <string>


gkg::HistogramAnalyzer::HistogramAnalyzer()
{
}


gkg::HistogramAnalyzer::~HistogramAnalyzer()
{
}


//
// methods without mask for volumes
//

template < class T >
void gkg::HistogramAnalyzer::getBoundaries( const gkg::Volume< T >& volume,
                                            T& lowerBoundary,
                                            T& upperBoundary ) const
{

  try
  {

    gkg::MinMaxFilter< gkg::Volume< T >, T > minMaxFilter;
    std::pair< T, T > minMax;

    minMaxFilter.filter( volume, minMax );

    lowerBoundary = minMax.first;
    upperBoundary = minMax.second;

  }
  GKG_CATCH( "template < class T > "
             "void gkg::HistogramAnalyzer::getBoundaries( "
             "const gkg::Volume< T >& volume, "
             "T& lowerBoundary, "
             "T& upperBoundary ) const" );

}


template < class T >
void 
gkg::HistogramAnalyzer::getHistogram( const gkg::Volume< T >& volume,
                                      std::vector< double >& histogram,
                                      T& lowerBoundary,
                                      T& upperBoundary,
                                      double kernelWidth,
                                      bool normalize ) const
{

  try
  {

    // sanity checks
    if ( histogram.empty() )
    {

      throw std::runtime_error( "level count should be strictly positive" );

    }

    // collecting level count
    int32_t levelCount = ( int32_t )histogram.size();

    // computing boundaries
    getBoundaries( volume, lowerBoundary, upperBoundary );

    if ( lowerBoundary == upperBoundary )
    {

      throw std::runtime_error( "lower boundary is equal to upper boundary" );

    }
    double increment =  ( ( double )upperBoundary - 
			  ( double )lowerBoundary ) /
                        ( double )( levelCount - 1 );

    // computing histogram values
    if ( volume.isMemoryMapped() )
    {

      int32_t sizeX = volume.getSizeX();
      int32_t sizeY = volume.getSizeY();
      int32_t sizeZ = volume.getSizeZ();
      int32_t sizeT = volume.getSizeT();
      int32_t x, y, z, t;

      for ( t = 0; t < sizeT; t++ )
      {

        for ( z = 0; z < sizeZ; z++ )
        {

          for ( y = 0; y < sizeY; y++ )
          {

            for ( x = 0; x < sizeX; x++ )
            {

              int32_t level = ( int32_t )( ( volume( x, y, z, t ) - 
                                             ( double )lowerBoundary ) / 
                                           increment );
              histogram[ level ] ++;

            }

          }

        }

      }

    }
    else
    {

      gkg::HistogramContext< T > context( volume, 
                                          histogram,
                                          (double)lowerBoundary,
                                          increment );
      gkg::ThreadedLoop< int32_t >
        threadedLoop( &context, 0, volume.getSizeXYZT() );

      threadedLoop.launch();

    }

    // resampling histogram
    if ( kernelWidth > 0.0 )
    {

      int32_t level = 0;
      std::vector< double > xs;
      std::vector< double > ys;
      std::vector< double >::iterator 
        h = histogram.begin(),
        he = histogram.end();

      while ( h != he )
      {

        if ( *h )
        {

          xs.push_back( ( double )lowerBoundary + ( double )level * increment );
          ys.push_back( *h );

        }

        ++level;
        ++h;

      }

      // getting numerical analysis implementation factory
      gkg::NumericalAnalysisImplementationFactory*
      factory = gkg::NumericalAnalysisSelector::getInstance().
                                                  getImplementationFactory();
      

      int32_t index = 0;
      for ( index = 0; index < levelCount; index++ )
      {

        histogram[ index ] = factory->getParzenWindowInterpolation(
                           ( double )lowerBoundary +
                           ( double )index * increment,
                           xs,
                           ys,
                           kernelWidth );

      }

    }

    // normalizing pdf
    if ( normalize )
    {

      double sum = 0;
      std::vector< double >::iterator h = histogram.begin(),
                                      he = histogram.end();
      while ( h != he )
      {

        sum += *h;
        ++ h;

      }

      if ( sum > 0.0 )
      {

        h = histogram.begin();
        while ( h != he )
        {

          *h /= sum;
          ++ h;

        }

      }

    }

  }
  GKG_CATCH( "void gkg::HistogramAnalyzer::getHistogram( const "
             "gkg::Volume< T >& volume, "
             "std::vector< double >& histogram, "
             "T& lowerBoundary, "
             "T& upperBoundary, "
             "double kernelWidth, "
             "bool normalize ) const" );

}


template < class T >
void gkg::HistogramAnalyzer::getCumulatedHistogram(
                                      const gkg::Volume< T >& volume,
                                      std::vector< double >& cumulatedHistogram,
                                      T& lowerBoundary,
                                      T& upperBoundary,
                                      double kernelWidth,
                                      bool normalize ) const
{

  try
  {

    getHistogram( volume, 
                  cumulatedHistogram, 
                  lowerBoundary, 
                  upperBoundary,
                  kernelWidth,
                  normalize );
    for ( size_t h = 1; h < cumulatedHistogram.size(); h++ )
    {

      cumulatedHistogram[ h ] += cumulatedHistogram[ h - 1 ];

    }

  }
  GKG_CATCH( "template < class T > void gkg::HistogramAnalyzer::"
             "getCumulatedHistogram( "
             "const gkg::Volume< T >& volume, "
             "std::vector< double >& cumulatedHistogram, "
             "T& lowerBoundary,"
             "T& upperBoundary, "
             "double kernelWidth, "
             "bool normalize ) const" );

}


template < class T >
void gkg::HistogramAnalyzer::getPercentileUpperBoundaries(
                                                 const gkg::Volume< T >& volume,
                                                 int32_t levelCount,
                                                 double percent,
                                                 T& lowerBoundary,
                                                 T& upperBoundary ) const
{

  try
  {

    std::vector< double > cumulatedHistogram( levelCount, 0.0 );
    getCumulatedHistogram( volume,
                           cumulatedHistogram,
                           lowerBoundary,
                           upperBoundary );

    int32_t level = ( int32_t )cumulatedHistogram.size() - 1;
    while ( ( level >= 0 ) && ( cumulatedHistogram[ level ] > 0.01 * percent ) )
    {

      -- level;

    }

    upperBoundary = lowerBoundary + ( T )( ( double )level * 
                    ( ( double )upperBoundary - ( double )lowerBoundary ) /
                    ( double )( levelCount - 1 ) );

  }
  GKG_CATCH( "template < class T > void gkg::HistogramAnalyzer::"
             "getPercentileUpperBoundaries( "
             "const gkg::Volume< T >& volume, "
             "int levelCount, "
             "double percent, "
             "T& lowerBoundary, "
             "T& upperBoundary ) const" );

}


template < class T >
void gkg::HistogramAnalyzer::getPercentileLowerBoundaries(
                                                 const gkg::Volume< T >& volume,
                                                 int32_t levelCount,
                                                 double percent,
                                                 T& lowerBoundary,
                                                 T& upperBoundary ) const
{

  try
  {

    std::vector< double > cumulatedHistogram( levelCount, 0.0 );
    getCumulatedHistogram( volume,
                           cumulatedHistogram,
                           lowerBoundary,
                           upperBoundary );

    int32_t levelMax = ( int32_t )cumulatedHistogram.size() - 1;
    int32_t level = 0;
    while ( ( level < levelMax ) && 
            ( cumulatedHistogram[ level ] < 0.01 * percent ) )
    {

      ++ level;

    }

    lowerBoundary += ( T )( ( double )level * 
                     ( ( double )upperBoundary - ( double )lowerBoundary ) /
                     ( double )( levelCount - 1 ) );

  }
  GKG_CATCH( "template < class T > void gkg::HistogramAnalyzer::"
             "getPercentileLowerBoundaries( "
             "const gkg::Volume< T >& volume, "
             "int levelCount, "
             "double percent, "
             "T& lowerBoundary, "
             "T& upperBoundary ) const" );

}


template < class T >
void gkg::HistogramAnalyzer::getPercentileBoundaries(
                                                 const gkg::Volume< T >& volume,
                                                 int32_t levelCount,
                                                 double percent,
                                                 T& lowerBoundary,
                                                 T& upperBoundary ) const
{

  try
  {

    std::vector< double > cumulatedHistogram( levelCount, 0.0 );
    getCumulatedHistogram( volume,
                           cumulatedHistogram,
                           lowerBoundary,
                           upperBoundary );

    int32_t levelMax = ( int32_t )cumulatedHistogram.size() - 1;
    int32_t level = 0;
    while ( ( level < levelMax ) && 
            ( cumulatedHistogram[ level ] < 0.5 - 0.005 * percent ) )
    {

      ++ level;

    }

    T lowerB = lowerBoundary + ( T )( ( double )level * 
               ( ( double )upperBoundary - ( double )lowerBoundary ) /
               ( double )( levelCount - 1 ) );

    level = levelMax;
    while ( ( level >= 0 ) && 
            ( cumulatedHistogram[ level ] > 0.5 + 0.005 * percent ) )
    {

      -- level;

    }

    upperBoundary = lowerBoundary + ( T )( ( double )level * 
                    ( ( double )upperBoundary - ( double )lowerBoundary ) /
                    ( double )( levelCount - 1 ) );
    lowerBoundary = lowerB;

  }
  GKG_CATCH( "template < class T > void gkg::HistogramAnalyzer::"
             "getPercentileBoundaries( "
             "const gkg::Volume< T >& volume, "
             "int levelCount, "
             "double percent, "
             "T& lowerBoundary, "
             "T& upperBoundary ) const" );

}


template < class T >
void gkg::HistogramAnalyzer::getPercentileBoundariesAndMinMax(
                                                 const gkg::Volume< T >& volume,
                                                 int32_t levelCount,
                                                 double percent,
                                                 T& lowerBoundary,
                                                 T& upperBoundary,
                                                 T& minimum,
                                                 T& maximum ) const
{

  try
  {

    std::vector< double > cumulatedHistogram( levelCount, 0.0 );
    getCumulatedHistogram( volume,
                           cumulatedHistogram,
                           minimum,
                           maximum );

    int32_t levelMax = ( int32_t )cumulatedHistogram.size() - 1;
    int32_t level = 0;
    while ( ( level < levelMax ) && 
            ( cumulatedHistogram[ level ] < 0.5 - 0.005 * percent ) )
    {

      ++ level;

    }

    lowerBoundary = minimum + ( T )( ( double )level * 
                    ( ( double )maximum - ( double )minimum ) /
                    ( double )( levelCount - 1 ) );

    level = levelMax;
    while ( ( level >= 0 ) && 
            ( cumulatedHistogram[ level ] > 0.5 + 0.005 * percent ) )
    {

      -- level;

    }

    upperBoundary = minimum + ( T )( ( double )level * 
                    ( ( double )maximum - ( double )minimum ) /
                    ( double )( levelCount - 1 ) );

  }
  GKG_CATCH( "template < class T > void gkg::HistogramAnalyzer::"
             "getPercentileBoundaries( "
             "const gkg::Volume< T >& volume, "
             "int levelCount, double percent, "
             "T& lowerBoundary, T& upperBoundary, "
             "T& minimum, T& maximum ) const" );

}


//
// methods with mask for volume(s)
//

template < class T >
void gkg::HistogramAnalyzer::getBoundaries( const gkg::Volume< T >& volume,
                                            const gkg::Volume< int16_t >& mask,
                                            T& lowerBoundary,
                                            T& upperBoundary ) const
{

  try
  {


    int32_t sizeX = volume.getSizeX();
    int32_t sizeY = volume.getSizeY();
    int32_t sizeZ = volume.getSizeZ();
    int32_t sizeT = volume.getSizeT();

    int32_t x, y, z, t;
    bool initialized = false;
    T value;

    for ( t = 0; t < sizeT; t++ )
    {

      std::cout << t << std::endl;

      for ( z = 0; z < sizeZ; z++ )
      {

        for ( y = 0; y < sizeY; y++ )
        {

          for ( x = 0; x < sizeX; x++ )
          {

            if ( mask( x, y, z ) )
            {

              value = volume( x, y, z, t );
              if ( !initialized )
              {

                lowerBoundary = upperBoundary = value;
                initialized = true;

              }
              else
              {

                if ( value > upperBoundary )
                {

                  upperBoundary = value;

                }
                if ( value < lowerBoundary )
                {

                  lowerBoundary = value;

                }

              }

            }

          }

        }

      }

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::HistogramAnalyzer::getBoundaries( "
             "const gkg::Volume< T >& volume, "
             "const gkg::Volume< int16_t >& mask, "
             "T& lowerBoundary, "
             "T& upperBoundary ) const" );

}


template < class T >
void 
gkg::HistogramAnalyzer::getHistogram( const gkg::Volume< T >& volume,
                                      const gkg::Volume< int16_t >& mask,
                                      std::vector< double >& histogram,
                                      T& lowerBoundary,
                                      T& upperBoundary,
                                      double kernelWidth,
                                      bool normalize ) const
{

  try
  {

    // sanity checks
    if ( histogram.empty() )
    {

      throw std::runtime_error( "level count should be strictly positive" );

    }

    // collecting level count
    int32_t levelCount = ( int32_t )histogram.size();

    // computing boundaries
    getBoundaries( volume, mask, lowerBoundary, upperBoundary );

    if ( lowerBoundary == upperBoundary )
    {

      throw std::runtime_error( "lower boundary is equal to upper boundary" );

    }
    double increment =  ( ( double )upperBoundary - 
			  ( double )lowerBoundary ) /
                        ( double )( levelCount - 1 );

    // computing histogram values
    int32_t level = 0;
    std::set< int32_t > levels;
    int32_t sizeX = volume.getSizeX();
    int32_t sizeY = volume.getSizeY();
    int32_t sizeZ = volume.getSizeZ();
    int32_t sizeT = volume.getSizeT();

    int32_t x, y, z, t;
    for ( t = 0; t < sizeT; t++ )
    {

      for ( z = 0; z < sizeZ; z++ )
      {

        for ( y = 0; y < sizeY; y++ )
        {

          for ( x = 0; x < sizeX; x++ )
          {

            if ( mask( x, y, z ) )
            {

              level = ( int32_t )( ( ( double )volume( x, y, z, t ) -
                                     ( double )lowerBoundary ) / increment );
              histogram[ level ] ++;
              levels.insert( level );

            }

          }

        }

      }

    }

    // resampling histogram
    if ( kernelWidth > 0.0 )
    {

      int32_t sampleCount = ( int32_t )levels.size();
      std::vector< double > xs( sampleCount );
      std::vector< double > ys( sampleCount );

      int32_t count = 0;
      std::set< int32_t >::const_iterator l = levels.begin(),
                                          le = levels.end();
      while ( l != le )
      {

        xs[ count ] = ( double )lowerBoundary + ( double )*l * increment;
        ys[ count ] = histogram[ *l ];
        ++ count;
        ++ l;

      }

      // getting numerical analysis implementation factory
      gkg::NumericalAnalysisImplementationFactory*
      factory = gkg::NumericalAnalysisSelector::getInstance().
                                                  getImplementationFactory();

      int32_t index = 0;
      for ( index = 0; index < levelCount; index++ )
      {

        histogram[ index ] = factory->getParzenWindowInterpolation(
                           ( double )lowerBoundary +
                           ( double )index * increment,
                           xs,
                           ys,
                           kernelWidth );

      }

    }

    // normalizing pdf
    if ( normalize )
    {

      double sum = 0;
      int32_t index = 0;
      for ( index = 0; index < levelCount; index++ )
      {

        sum += histogram[ index ];

      }

      if ( sum > 0.0 )
      {

        std::vector< double >::iterator h = histogram.begin(),
                                        he = histogram.end();
        while ( h != he )
        {

          *h /= sum;
          ++ h;

        }

      }

    }

  }
  GKG_CATCH( "void gkg::HistogramAnalyzer::getHistogram( "
             "const gkg::Volume< T >& volume, "
             "const gkg::Volume< int16_t >& mask, "
             "std::vector< double >& histogram, "
             "T& lowerBoundary, "
             "T& upperBoundary, "
             "double kernelWidth, "
             "bool normalize ) const" );

}


template < class T >
void gkg::HistogramAnalyzer::getCumulatedHistogram(
                                      const gkg::Volume< T >& volume,
                                      const gkg::Volume< int16_t >& mask,
                                      std::vector< double >& cumulatedHistogram,
                                      T& lowerBoundary,
                                      T& upperBoundary,
                                      double kernelWidth,
                                      bool normalize ) const
{

  try
  {

    getHistogram( volume,
                  mask,
                  cumulatedHistogram, 
                  lowerBoundary, 
                  upperBoundary,
                  kernelWidth,
                  normalize );
    for ( size_t h = 1; h < cumulatedHistogram.size(); h++ )
    {

      cumulatedHistogram[ h ] += cumulatedHistogram[ h - 1 ];

    }

  }
  GKG_CATCH( "template < class T > void gkg::HistogramAnalyzer::"
             "getCumulatedHistogram( "
             "const gkg::Volume< T >& volume, "
             "const gkg::Volume< int16_t >& mask, "
             "std::vector< double >& cumulatedHistogram, "
             "T& lowerBoundary,"
             "T& upperBoundary, "
             "double kernelWidth, "
             "bool normalize ) const" );

}


template < class T >
void gkg::HistogramAnalyzer::getPercentileUpperBoundaries(
                                                 const gkg::Volume< T >& volume,
                                                 const gkg::Volume< int16_t >& mask,
                                                 int32_t levelCount,
                                                 double percent,
                                                 T& lowerBoundary,
                                                 T& upperBoundary ) const
{

  try
  {

    std::vector< double > cumulatedHistogram( levelCount );
    getCumulatedHistogram( volume,
                           mask,
                           cumulatedHistogram,
                           lowerBoundary,
                           upperBoundary );

    int32_t level = ( int32_t )cumulatedHistogram.size() - 1;
    while ( ( level >= 0 ) && ( cumulatedHistogram[ level ] > 0.01 * percent ) )
    {

      -- level;

    }

    upperBoundary = lowerBoundary + ( T )( ( double )level * 
                    ( ( double )upperBoundary - ( double )lowerBoundary ) /
                    ( double )( levelCount - 1 ) );

  }
  GKG_CATCH( "template < class T > void gkg::HistogramAnalyzer::"
             "getPercentileUpperBoundaries( "
             "const gkg::Volume< T >& volume, "
             "const gkg::Volume< int16_t >& mask, "
             "int levelCount, "
             "double percent, "
             "T& lowerBoundary, "
             "T& upperBoundary ) const" );

}


template < class T >
void gkg::HistogramAnalyzer::getPercentileLowerBoundaries(
                                                 const gkg::Volume< T >& volume,
                                                 const gkg::Volume< int16_t >& mask,
                                                 int32_t levelCount,
                                                 double percent,
                                                 T& lowerBoundary,
                                                 T& upperBoundary ) const
{

  try
  {

    std::vector< double > cumulatedHistogram( levelCount );
    getCumulatedHistogram( volume,
                           mask,
                           cumulatedHistogram,
                           lowerBoundary,
                           upperBoundary );

    int32_t levelMax = ( int32_t )cumulatedHistogram.size() - 1;
    int32_t level = 0;
    while ( ( level < levelMax ) && 
            ( cumulatedHistogram[ level ] < 0.01 * percent ) )
    {

      ++ level;

    }

    lowerBoundary += ( T )( ( double )level * 
                     ( ( double )upperBoundary - ( double )lowerBoundary ) /
                     ( double )( levelCount - 1 ) );

  }
  GKG_CATCH( "template < class T > void gkg::HistogramAnalyzer::"
             "getPercentileLowerBoundaries( "
             "const gkg::Volume< T >& volume, "
             "const gkg::Volume< int16_t >& mask, "
             "int levelCount, "
             "double percent, "
             "T& lowerBoundary, "
             "T& upperBoundary ) const" );

}


template < class T >
void gkg::HistogramAnalyzer::getPercentileBoundaries(
                                                 const gkg::Volume< T >& volume,
                                                 const gkg::Volume< int16_t >& mask,
                                                 int32_t levelCount,
                                                 double percent,
                                                 T& lowerBoundary,
                                                 T& upperBoundary ) const
{

  try
  {

    std::vector< double > cumulatedHistogram( levelCount );
    getCumulatedHistogram( volume,
                           mask,
                           cumulatedHistogram,
                           lowerBoundary,
                           upperBoundary );

    int32_t levelMax = ( int32_t )cumulatedHistogram.size() - 1;
    int32_t level = 0;
    while ( ( level < levelMax ) && 
            ( cumulatedHistogram[ level ] < 0.5 - 0.005 * percent ) )
    {

      ++ level;

    }

    T lowerB = lowerBoundary + ( T )( ( double )level * 
               ( ( double )upperBoundary - ( double )lowerBoundary ) /
               ( double )( levelCount - 1 ) );

    level = levelMax;
    while ( ( level >= 0 ) && 
            ( cumulatedHistogram[ level ] > 0.5 + 0.005 * percent ) )
    {

      -- level;

    }

    upperBoundary = lowerBoundary + ( T )( ( double )level * 
                    ( ( double )upperBoundary - ( double )lowerBoundary ) /
                    ( double )( levelCount - 1 ) );
    lowerBoundary = lowerB;

  }
  GKG_CATCH( "template < class T > void gkg::HistogramAnalyzer::"
             "getPercentileBoundaries( "
             "const gkg::Volume< T >& volume, "
             "const gkg::Volume< int16_t >& mask, "
             "int levelCount, "
             "double percent, "
             "T& lowerBoundary, "
             "T& upperBoundary ) const" );

}


//
// methods for vector(s)
//

template < class T >
void gkg::HistogramAnalyzer::getBoundaries( const std::vector< T >& v,
                                            T& lowerBoundary,
                                            T& upperBoundary ) const
{

  try
  {

    typename std::vector< T >::const_iterator i = v.begin(),
                                              ie = v.end();
    lowerBoundary = upperBoundary = *i;
    ++ i;
    while ( i != ie )
    {

      if ( *i > upperBoundary )
      {

        upperBoundary = *i;

      }
      if ( *i < lowerBoundary )
      {

        lowerBoundary = *i;

      }
      ++ i;

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::HistogramAnalyzer::getBoundaries( "
             "const std::vector< T >& v, "
             "T& lowerBoundary, "
             "T& upperBoundary ) const" );

}


template < class T >
void 
gkg::HistogramAnalyzer::getHistogram( const std::vector< T >& v,
                                      std::vector< double >& histogram,
                                      T& lowerBoundary,
                                      T& upperBoundary,
                                      double kernelWidth,
                                      bool normalize ) const
{

  try
  {

    // sanity checks
    if ( histogram.empty() )
    {

      throw std::runtime_error( "level count should be strictly positive" );

    }

    // collecting level count
    int32_t levelCount = ( int32_t )histogram.size();

    // computing boundaries
    getBoundaries( v, lowerBoundary, upperBoundary );

    if ( lowerBoundary == upperBoundary )
    {

      throw std::runtime_error( "lower boundary is equal to upper boundary" );

    }
    double increment =  ( ( double )upperBoundary - 
			  ( double )lowerBoundary ) /
                        ( double )( levelCount - 1 );

    // computing histogram values
    int32_t level = 0;
    std::set< int32_t > levels;
    typename std::vector< T >::const_iterator i = v.begin(),
                                              ie = v.end();
    while ( i != ie )
    {

      level = ( int32_t )( ( *i - ( double )lowerBoundary ) / increment );
      histogram[ level ] ++;
      levels.insert( level );
      ++ i;

    }

    // resampling histogram
    if ( kernelWidth > 0.0 )
    {

      int32_t sampleCount = ( int32_t )levels.size();
      std::vector< double > xs( sampleCount );
      std::vector< double > ys( sampleCount );

      int32_t count = 0;
      std::set< int32_t >::const_iterator l = levels.begin(),
                                          le = levels.end();
      while ( l != le )
      {

        xs[ count ] = ( double )lowerBoundary + ( double )*l * increment;
        ys[ count ] = histogram[ *l ];
        ++ count;
        ++ l;

      }

      // getting numerical analysis implementation factory
      gkg::NumericalAnalysisImplementationFactory*
      factory = gkg::NumericalAnalysisSelector::getInstance().
                                                  getImplementationFactory();
      

      int32_t index = 0;
      for ( index = 0; index < levelCount; index++ )
      {

        histogram[ index ] = factory->getParzenWindowInterpolation(
                           ( double )lowerBoundary +
                           ( double )index * increment,
                           xs,
                           ys,
                           kernelWidth );

      }

    }

    // normalizing pdf
    if ( normalize )
    {

      double sum = 0;
      int32_t index = 0;
      for ( index = 0; index < levelCount; index++ )
      {

        sum += histogram[ index ];

      }

      if ( sum > 0.0 )
      {

        std::vector< double >::iterator h = histogram.begin(),
                                        he = histogram.end();
        while ( h != he )
        {

          *h /= sum;
          ++ h;

        }

      }

    }

  }
  GKG_CATCH( "void gkg::HistogramAnalyzer::getHistogram( const "
             "std::vector< T >& v, "
             "std::vector< double >& histogram, "
             "T& lowerBoundary, "
             "T& upperBoundary, "
             "double kernelWidth, "
             "bool normalize ) const" );

}


template < class T >
void gkg::HistogramAnalyzer::getCumulatedHistogram(
                                      const std::vector< T >& v,
                                      std::vector< double >& cumulatedHistogram,
                                      T& lowerBoundary,
                                      T& upperBoundary,
                                      double kernelWidth,
                                      bool normalize ) const
{

  try
  {

    getHistogram( v, 
                  cumulatedHistogram, 
                  lowerBoundary, 
                  upperBoundary,
                  kernelWidth,
                  normalize );
    for ( size_t h = 1; h < cumulatedHistogram.size(); h++ )
    {

      cumulatedHistogram[ h ] += cumulatedHistogram[ h - 1 ];

    }

  }
  GKG_CATCH( "template < class T > void gkg::HistogramAnalyzer::"
             "getCumulatedHistogram( "
             "const std::vector< T >& v, "
             "std::vector< double >& cumulatedHistogram, "
             "T& lowerBoundary,"
             "T& upperBoundary, "
             "double kernelWidth, "
             "bool normalize ) const" );

}


template < class T >
void gkg::HistogramAnalyzer::getPercentileUpperBoundaries(
                                                 const std::vector< T >& v,
                                                 int32_t levelCount,
                                                 double percent,
                                                 T& lowerBoundary,
                                                 T& upperBoundary ) const
{

  try
  {

    std::vector< double > cumulatedHistogram( levelCount );
    getCumulatedHistogram( v,
                           cumulatedHistogram,
                           lowerBoundary,
                           upperBoundary );

    int32_t level = ( int32_t )cumulatedHistogram.size() - 1;
    while ( ( level >= 0 ) && ( cumulatedHistogram[ level ] > 0.01 * percent ) )
    {

      -- level;

    }

    upperBoundary = lowerBoundary + ( T )( ( double )level * 
                    ( ( double )upperBoundary - ( double )lowerBoundary ) /
                    ( double )( levelCount - 1 ) );

  }
  GKG_CATCH( "template < class T > void gkg::HistogramAnalyzer::"
             "getPercentileUpperBoundaries( "
             "const std::vector< T >& v, "
             "int levelCount, "
             "double percent, "
             "T& lowerBoundary, "
             "T& upperBoundary ) const" );

}


template < class T >
void gkg::HistogramAnalyzer::getPercentileLowerBoundaries(
                                                 const std::vector< T >& v,
                                                 int32_t levelCount,
                                                 double percent,
                                                 T& lowerBoundary,
                                                 T& upperBoundary ) const
{

  try
  {

    std::vector< double > cumulatedHistogram( levelCount );
    getCumulatedHistogram( v,
                           cumulatedHistogram,
                           lowerBoundary,
                           upperBoundary );

    int32_t levelMax = ( int32_t )cumulatedHistogram.size() - 1;
    int32_t level = 0;
    while ( ( level < levelMax ) && 
            ( cumulatedHistogram[ level ] < 0.01 * percent ) )
    {

      ++ level;

    }

    lowerBoundary += ( T )( ( double )level * 
                     ( ( double )upperBoundary - ( double )lowerBoundary ) /
                     ( double )( levelCount - 1 ) );

  }
  GKG_CATCH( "template < class T > void gkg::HistogramAnalyzer::"
             "getPercentileLowerBoundaries( "
             "const std::vector< T >& v, "
             "int levelCount, "
             "double percent, "
             "T& lowerBoundary, "
             "T& upperBoundary ) const" );

}


template < class T >
void gkg::HistogramAnalyzer::getPercentileBoundaries(
                                                 const std::vector< T >& v,
                                                 int32_t levelCount,
                                                 double percent,
                                                 T& lowerBoundary,
                                                 T& upperBoundary ) const
{

  try
  {

    std::vector< double > cumulatedHistogram( levelCount );
    getCumulatedHistogram( v,
                           cumulatedHistogram,
                           lowerBoundary,
                           upperBoundary );

    int32_t levelMax = ( int32_t )cumulatedHistogram.size() - 1;
    int32_t level = 0;
    while ( ( level < levelMax ) && 
            ( cumulatedHistogram[ level ] < 0.5 - 0.005 * percent ) )
    {

      ++ level;

    }

    T lowerB = lowerBoundary + ( T )( ( double )level * 
               ( ( double )upperBoundary - ( double )lowerBoundary ) /
               ( double )( levelCount - 1 ) );

    level = levelMax;
    while ( ( level >= 0 ) && 
            ( cumulatedHistogram[ level ] > 0.5 + 0.005 * percent ) )
    {

      -- level;

    }

    upperBoundary = lowerBoundary + ( T )( ( double )level * 
                    ( ( double )upperBoundary - ( double )lowerBoundary ) /
                    ( double )( levelCount - 1 ) );
    lowerBoundary = lowerB;

  }
  GKG_CATCH( "template < class T > void gkg::HistogramAnalyzer::"
             "getPercentileBoundaries( "
             "const std::vector< T >& v, "
             "int levelCount, "
             "double percent, "
             "T& lowerBoundary, "
             "T& upperBoundary ) const" );

}


//
// methods for list(s)
//

template < class T >
void gkg::HistogramAnalyzer::getBoundaries( const std::list< T >& v,
                                            T& lowerBoundary,
                                            T& upperBoundary ) const
{

  try
  {

    typename std::list< T >::const_iterator i = v.begin(),
                                            ie = v.end();
    lowerBoundary = upperBoundary = *i;
    ++ i;
    while ( i != ie )
    {

      if ( *i > upperBoundary )
      {

        upperBoundary = *i;

      }
      if ( *i < lowerBoundary )
      {

        lowerBoundary = *i;

      }
      ++ i;

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::HistogramAnalyzer::getBoundaries( "
             "const std::list< T >& v, "
             "T& lowerBoundary, "
             "T& upperBoundary ) const" );

}


template < class T >
void 
gkg::HistogramAnalyzer::getHistogram( const std::list< T >& v,
                                      std::vector< double >& histogram,
                                      T& lowerBoundary,
                                      T& upperBoundary,
                                      double kernelWidth,
                                      bool normalize ) const
{

  try
  {

    // sanity checks
    if ( histogram.empty() )
    {

      throw std::runtime_error( "level count should be strictly positive" );

    }

    // collecting level count
    int32_t levelCount = ( int32_t )histogram.size();

    // computing boundaries
    getBoundaries( v, lowerBoundary, upperBoundary );

    if ( lowerBoundary == upperBoundary )
    {

      throw std::runtime_error( "lower boundary is equal to upper boundary" );

    }
    double increment =  ( ( double )upperBoundary - 
			  ( double )lowerBoundary ) /
                        ( double )( levelCount - 1 );

    // computing histogram values
    int32_t level = 0;
    std::set< int32_t > levels;
    typename std::list< T >::const_iterator i = v.begin(),
                                            ie = v.end();
    while ( i != ie )
    {

      level = ( int32_t )( ( *i - ( double )lowerBoundary ) / increment );
      histogram[ level ] ++;
      levels.insert( level );
      ++ i;

    }

    // resampling histogram
    if ( kernelWidth > 0.0 )
    {

      int32_t sampleCount = ( int32_t )levels.size();
      std::vector< double > xs( sampleCount );
      std::vector< double > ys( sampleCount );

      int32_t count = 0;
      std::set< int32_t >::const_iterator l = levels.begin(),
                                          le = levels.end();
      while ( l != le )
      {

        xs[ count ] = ( double )lowerBoundary + ( double )*l * increment;
        ys[ count ] = histogram[ *l ];
        ++ count;
        ++ l;

      }

      // getting numerical analysis implementation factory
      gkg::NumericalAnalysisImplementationFactory*
      factory = gkg::NumericalAnalysisSelector::getInstance().
                                                  getImplementationFactory();
      

      int32_t index = 0;
      for ( index = 0; index < levelCount; index++ )
      {

        histogram[ index ] = factory->getParzenWindowInterpolation(
                           ( double )lowerBoundary +
                           ( double )index * increment,
                           xs,
                           ys,
                           kernelWidth );

      }

    }

    // normalizing pdf
    if ( normalize )
    {

      double sum = 0;
      int32_t index = 0;
      for ( index = 0; index < levelCount; index++ )
      {

        sum += histogram[ index ];

      }

      if ( sum > 0.0 )
      {

        std::vector< double >::iterator h = histogram.begin(),
                                        he = histogram.end();
        while ( h != he )
        {

          *h /= sum;
          ++ h;

        }

      }

    }

  }
  GKG_CATCH( "void gkg::HistogramAnalyzer::getHistogram( const "
             "std::list< T >& v, "
             "std::vector< double >& histogram, "
             "T& lowerBoundary, "
             "T& upperBoundary, "
             "double kernelWidth, "
             "bool normalize ) const" );

}


template < class T >
void gkg::HistogramAnalyzer::getCumulatedHistogram(
                                      const std::list< T >& v,
                                      std::vector< double >& cumulatedHistogram,
                                      T& lowerBoundary,
                                      T& upperBoundary,
                                      double kernelWidth,
                                      bool normalize ) const
{

  try
  {

    getHistogram( v, 
                  cumulatedHistogram, 
                  lowerBoundary, 
                  upperBoundary,
                  kernelWidth,
                  normalize );
    for ( size_t h = 1; h < cumulatedHistogram.size(); h++ )
    {

      cumulatedHistogram[ h ] += cumulatedHistogram[ h - 1 ];

    }

  }
  GKG_CATCH( "template < class T > void gkg::HistogramAnalyzer::"
             "getCumulatedHistogram( "
             "const std::list< T >& v, "
             "std::vector< double >& cumulatedHistogram, "
             "T& lowerBoundary,"
             "T& upperBoundary, "
             "double kernelWidth, "
             "bool normalize ) const" );

}


template < class T >
void gkg::HistogramAnalyzer::getPercentileUpperBoundaries(
                                                 const std::list< T >& v,
                                                 int32_t levelCount,
                                                 double percent,
                                                 T& lowerBoundary,
                                                 T& upperBoundary ) const
{

  try
  {

    std::vector< double > cumulatedHistogram( levelCount );
    getCumulatedHistogram( v,
                           cumulatedHistogram,
                           lowerBoundary,
                           upperBoundary );

    int32_t level = ( int32_t )cumulatedHistogram.size() - 1;
    while ( ( level >= 0 ) && ( cumulatedHistogram[ level ] > 0.01 * percent ) )
    {

      -- level;

    }

    upperBoundary = lowerBoundary + ( T )( ( double )level * 
                    ( ( double )upperBoundary - ( double )lowerBoundary ) /
                    ( double )( levelCount - 1 ) );

  }
  GKG_CATCH( "template < class T > void gkg::HistogramAnalyzer::"
             "getPercentileUpperBoundaries( "
             "const std::list< T >& v, "
             "int levelCount, "
             "double percent, "
             "T& lowerBoundary, "
             "T& upperBoundary ) const" );

}


template < class T >
void gkg::HistogramAnalyzer::getPercentileLowerBoundaries(
                                                 const std::list< T >& v,
                                                 int32_t levelCount,
                                                 double percent,
                                                 T& lowerBoundary,
                                                 T& upperBoundary ) const
{

  try
  {

    std::vector< double > cumulatedHistogram( levelCount );
    getCumulatedHistogram( v,
                           cumulatedHistogram,
                           lowerBoundary,
                           upperBoundary );

    int32_t levelMax = ( int32_t )cumulatedHistogram.size() - 1;
    int32_t level = 0;
    while ( ( level < levelMax ) && 
            ( cumulatedHistogram[ level ] < 0.01 * percent ) )
    {

      ++ level;

    }

    lowerBoundary += ( T )( ( double )level * 
                     ( ( double )upperBoundary - ( double )lowerBoundary ) /
                     ( double )( levelCount - 1 ) );

  }
  GKG_CATCH( "template < class T > void gkg::HistogramAnalyzer::"
             "getPercentileLowerBoundaries( "
             "const std::list< T >& v, "
             "int levelCount, "
             "double percent, "
             "T& lowerBoundary, "
             "T& upperBoundary ) const" );

}


template < class T >
void gkg::HistogramAnalyzer::getPercentileBoundaries(
                                                 const std::list< T >& v,
                                                 int32_t levelCount,
                                                 double percent,
                                                 T& lowerBoundary,
                                                 T& upperBoundary ) const
{

  try
  {

    std::vector< double > cumulatedHistogram( levelCount );
    getCumulatedHistogram( v,
                           cumulatedHistogram,
                           lowerBoundary,
                           upperBoundary );

    int32_t levelMax = ( int32_t )cumulatedHistogram.size() - 1;
    int32_t level = 0;
    while ( ( level < levelMax ) && 
            ( cumulatedHistogram[ level ] < 0.5 - 0.005 * percent ) )
    {

      ++ level;

    }

    T lowerB = lowerBoundary + ( T )( ( double )level * 
               ( ( double )upperBoundary - ( double )lowerBoundary ) /
               ( double )( levelCount - 1 ) );

    level = levelMax;
    while ( ( level >= 0 ) && 
            ( cumulatedHistogram[ level ] > 0.5 + 0.005 * percent ) )
    {

      -- level;

    }

    upperBoundary = lowerBoundary + ( T )( ( double )level * 
                    ( ( double )upperBoundary - ( double )lowerBoundary ) /
                    ( double )( levelCount - 1 ) );
    lowerBoundary = lowerB;

  }
  GKG_CATCH( "template < class T > void gkg::HistogramAnalyzer::"
             "getPercentileBoundaries( "
             "const std::list< T >& v, "
             "int levelCount, "
             "double percent, "
             "T& lowerBoundary, "
             "T& upperBoundary ) const" );

}


//
// methods for Vector
//

void gkg::HistogramAnalyzer::getBoundaries( const gkg::Vector& v,
                                            double& lowerBoundary,
                                            double& upperBoundary ) const
{

  try
  {

    int32_t i = 0,
            ie = v.getSize();
    lowerBoundary = upperBoundary = v( i );
    ++ i;
    while ( i != ie )
    {

      if ( v( i ) > upperBoundary )
      {

        upperBoundary = v( i );

      }
      if ( v( i ) < lowerBoundary )
      {

        lowerBoundary = v( i );

      }
      ++ i;

    }

  }
  GKG_CATCH( "void gkg::HistogramAnalyzer::getBoundaries( "
             "const gkg::Vector& v, "
             "double& lowerBoundary, "
             "double& upperBoundary ) const" );

}


void 
gkg::HistogramAnalyzer::getHistogram( const gkg::Vector& v,
                                      std::vector< double >& histogram,
                                      double& lowerBoundary,
                                      double& upperBoundary,
                                      double kernelWidth,
                                      bool normalize ) const
{

  try
  {

    // sanity checks
    if ( histogram.empty() )
    {

      throw std::runtime_error( "level count should be strictly positive" );

    }

    // collecting level count
    int32_t levelCount = ( int32_t )histogram.size();

    // computing boundaries
    getBoundaries( v, lowerBoundary, upperBoundary );

    if ( lowerBoundary == upperBoundary )
    {

      throw std::runtime_error( "lower boundary is equal to upper boundary" );

    }
    double increment =  ( ( double )upperBoundary - 
			  ( double )lowerBoundary ) /
                        ( double )( levelCount - 1 );

    // computing histogram values
    int32_t level = 0;
    std::set< int32_t > levels;
    int32_t i = 0,
            ie = v.getSize();
    while ( i != ie )
    {

      level = ( int32_t )( ( v( i ) - ( double )lowerBoundary ) / increment );
      histogram[ level ] ++;
      levels.insert( level );
      ++ i;

    }

    // resampling histogram
    if ( kernelWidth > 0.0 )
    {

      int32_t sampleCount = ( int32_t )levels.size();
      std::vector< double > xs( sampleCount );
      std::vector< double > ys( sampleCount );

      int32_t count = 0;
      std::set< int32_t >::const_iterator l = levels.begin(),
                                          le = levels.end();
      while ( l != le )
      {

        xs[ count ] = ( double )lowerBoundary + ( double )*l * increment;
        ys[ count ] = histogram[ *l ];
        ++ count;
        ++ l;

      }

      // getting numerical analysis implementation factory
      gkg::NumericalAnalysisImplementationFactory*
      factory = gkg::NumericalAnalysisSelector::getInstance().
                                                  getImplementationFactory();
      

      int32_t index = 0;
      for ( index = 0; index < levelCount; index++ )
      {

        histogram[ index ] = factory->getParzenWindowInterpolation(
                           ( double )lowerBoundary +
                           ( double )index * increment,
                           xs,
                           ys,
                           kernelWidth );

      }

    }

    // normalizing pdf
    if ( normalize )
    {

      double sum = 0;
      int32_t index = 0;
      for ( index = 0; index < levelCount; index++ )
      {

        sum += histogram[ index ];

      }

      if ( sum > 0.0 )
      {

        std::vector< double >::iterator h = histogram.begin(),
                                        he = histogram.end();
        while ( h != he )
        {

          *h /= sum;
          ++ h;

        }

      }

    }

  }
  GKG_CATCH( "void gkg::HistogramAnalyzer::getHistogram( const "
             "gkg::Vector& v, "
             "std::vector< double >& histogram, "
             "double& lowerBoundary, "
             "double& upperBoundary, "
             "double kernelWidth, "
             "bool normalize ) const" );

}


void gkg::HistogramAnalyzer::getCumulatedHistogram(
                                      const gkg::Vector& v,
                                      std::vector< double >& cumulatedHistogram,
                                      double& lowerBoundary,
                                      double& upperBoundary,
                                      double kernelWidth,
                                      bool normalize ) const
{

  try
  {

    getHistogram( v, 
                  cumulatedHistogram, 
                  lowerBoundary, 
                  upperBoundary,
                  kernelWidth,
                  normalize );
    for ( size_t h = 1; h < cumulatedHistogram.size(); h++ )
    {

      cumulatedHistogram[ h ] += cumulatedHistogram[ h - 1 ];

    }

  }
  GKG_CATCH( "void gkg::HistogramAnalyzer::"
             "getCumulatedHistogram( "
             "const gkg::Vector& v, "
             "std::vector< double >& cumulatedHistogram, "
             "double& lowerBoundary,"
             "double& upperBoundary, "
             "double kernelWidth, "
             "bool normalize ) const" );

}


void gkg::HistogramAnalyzer::getPercentileUpperBoundaries(
                                                 const gkg::Vector& v,
                                                 int32_t levelCount,
                                                 double percent,
                                                 double& lowerBoundary,
                                                 double& upperBoundary ) const
{

  try
  {

    std::vector< double > cumulatedHistogram( levelCount );
    getCumulatedHistogram( v,
                           cumulatedHistogram,
                           lowerBoundary,
                           upperBoundary );

    int32_t level = ( int32_t )cumulatedHistogram.size() - 1;
    while ( ( level >= 0 ) && ( cumulatedHistogram[ level ] > 0.01 * percent ) )
    {

      -- level;

    }

    upperBoundary = lowerBoundary + ( double )( ( double )level * 
                    ( ( double )upperBoundary - ( double )lowerBoundary ) /
                    ( double )( levelCount - 1 ) );

  }
  GKG_CATCH( "void gkg::HistogramAnalyzer::"
             "getPercentileUpperBoundaries( "
             "const gkg::Vector& v, "
             "int levelCount, "
             "double percent, "
             "double& lowerBoundary, "
             "double& upperBoundary ) const" );

}


void gkg::HistogramAnalyzer::getPercentileLowerBoundaries(
                                                 const gkg::Vector& v,
                                                 int32_t levelCount,
                                                 double percent,
                                                 double& lowerBoundary,
                                                 double& upperBoundary ) const
{

  try
  {

    std::vector< double > cumulatedHistogram( levelCount );
    getCumulatedHistogram( v,
                           cumulatedHistogram,
                           lowerBoundary,
                           upperBoundary );

    int32_t levelMax = ( int32_t )cumulatedHistogram.size() - 1;
    int32_t level = 0;
    while ( ( level < levelMax ) && 
            ( cumulatedHistogram[ level ] < 0.01 * percent ) )
    {

      ++ level;

    }

    lowerBoundary += ( double )( ( double )level * 
                     ( ( double )upperBoundary - ( double )lowerBoundary ) /
                     ( double )( levelCount - 1 ) );

  }
  GKG_CATCH( "void gkg::HistogramAnalyzer::"
             "getPercentileLowerBoundaries( "
             "const gkg::Vector& v, "
             "int levelCount, "
             "double percent, "
             "double& lowerBoundary, "
             "double& upperBoundary ) const" );

}


void gkg::HistogramAnalyzer::getPercentileBoundaries(
                                                 const gkg::Vector& v,
                                                 int32_t levelCount,
                                                 double percent,
                                                 double& lowerBoundary,
                                                 double& upperBoundary ) const
{

  try
  {

    std::vector< double > cumulatedHistogram( levelCount );
    getCumulatedHistogram( v,
                           cumulatedHistogram,
                           lowerBoundary,
                           upperBoundary );

    int32_t levelMax = ( int32_t )cumulatedHistogram.size() - 1;
    int32_t level = 0;
    while ( ( level < levelMax ) && 
            ( cumulatedHistogram[ level ] < 0.5 - 0.005 * percent ) )
    {

      ++ level;

    }

    double lowerB = lowerBoundary + ( double )( ( double )level * 
                    ( ( double )upperBoundary - ( double )lowerBoundary ) /
                    ( double )( levelCount - 1 ) );

    level = levelMax;
    while ( ( level >= 0 ) && 
            ( cumulatedHistogram[ level ] > 0.5 + 0.005 * percent ) )
    {

      -- level;

    }

    upperBoundary = lowerBoundary + ( double )( ( double )level * 
                    ( ( double )upperBoundary - ( double )lowerBoundary ) /
                    ( double )( levelCount - 1 ) );
    lowerBoundary = lowerB;

  }
  GKG_CATCH( "void gkg::HistogramAnalyzer::"
             "getPercentileBoundaries( "
             "const gkg::Vector& v, "
             "int levelCount, "
             "double percent, "
             "double& lowerBoundary, "
             "double& upperBoundary ) const" );

}


//
// methods without mask for volume(s)
//


// force instanciations of getBoundaries for current types
template
void gkg::HistogramAnalyzer::getBoundaries(
                                const gkg::Volume< int8_t >& volume,
                                int8_t& lowerBoundary,
                                int8_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getBoundaries(
                                const gkg::Volume< uint8_t >& volume,
                                uint8_t& lowerBoundary,
                                uint8_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getBoundaries(
                                const gkg::Volume< int16_t >& volume,
                                int16_t& lowerBoundary,
                                int16_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getBoundaries(
                                const gkg::Volume< uint16_t >& volume,
                                uint16_t& lowerBoundary,
                                uint16_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getBoundaries(
                                const gkg::Volume< int32_t >& volume,
                                int32_t& lowerBoundary,
                                int32_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getBoundaries(
                                const gkg::Volume< uint32_t >& volume,
                                uint32_t& lowerBoundary,
                                uint32_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getBoundaries(
                                const gkg::Volume< int64_t >& volume,
                                int64_t& lowerBoundary,
                                int64_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getBoundaries(
                                const gkg::Volume< uint64_t >& volume,
                                uint64_t& lowerBoundary,
                                uint64_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getBoundaries(
                                const gkg::Volume< float >& volume,
                                float& lowerBoundary,
                                float& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getBoundaries(
                                const gkg::Volume< double >& volume,
                                double& lowerBoundary,
                                double& upperBoundary ) const;


// force instanciations of getHistogram for current types
template
void gkg::HistogramAnalyzer::getHistogram(
                                const gkg::Volume< int8_t >& volume,
                                std::vector< double >& histogram,
                                int8_t& lowerBoundary,
                                int8_t& upperBoundary,
                                double kernelWidth = -1.0,
                                bool normalize = true ) const;
template
void gkg::HistogramAnalyzer::getHistogram(
                                const gkg::Volume< uint8_t >& volume,
                                std::vector< double >& histogram,
                                uint8_t& lowerBoundary,
                                uint8_t& upperBoundary,
                                double kernelWidth = -1.0,
                                bool normalize = true ) const;
template
void gkg::HistogramAnalyzer::getHistogram(
                                const gkg::Volume< int16_t >& volume,
                                std::vector< double >& histogram,
                                int16_t& lowerBoundary,
                                int16_t& upperBoundary,
                                double kernelWidth = -1.0,
                                bool normalize = true ) const;
template
void gkg::HistogramAnalyzer::getHistogram(
                                const gkg::Volume< uint16_t >& volume,
                                std::vector< double >& histogram,
                                uint16_t& lowerBoundary,
                                uint16_t& upperBoundary,
                                double kernelWidth = -1.0,
                                bool normalize = true ) const;
template
void gkg::HistogramAnalyzer::getHistogram(
                                const gkg::Volume< int32_t >& volume,
                                std::vector< double >& histogram,
                                int32_t& lowerBoundary,
                                int32_t& upperBoundary,
                                double kernelWidth = -1.0,
                                bool normalize = true ) const;
template
void gkg::HistogramAnalyzer::getHistogram(
                                const gkg::Volume< uint32_t >& volume,
                                std::vector< double >& histogram,
                                uint32_t& lowerBoundary,
                                uint32_t& upperBoundary,
                                double kernelWidth = -1.0,
                                bool normalize = true ) const;
template
void gkg::HistogramAnalyzer::getHistogram(
                                const gkg::Volume< int64_t >& volume,
                                std::vector< double >& histogram,
                                int64_t& lowerBoundary,
                                int64_t& upperBoundary,
                                double kernelWidth = -1.0,
                                bool normalize = true ) const;
template
void gkg::HistogramAnalyzer::getHistogram(
                                const gkg::Volume< uint64_t >& volume,
                                std::vector< double >& histogram,
                                uint64_t& lowerBoundary,
                                uint64_t& upperBoundary,
                                double kernelWidth = -1.0,
                                bool normalize = true ) const;
template
void gkg::HistogramAnalyzer::getHistogram(
                                const gkg::Volume< float >& volume,
                                std::vector< double >& histogram,
                                float& lowerBoundary,
                                float& upperBoundary,
                                double kernelWidth = -1.0,
                                bool normalize ) const;
template
void gkg::HistogramAnalyzer::getHistogram(
                                const gkg::Volume< double >& volume,
                                std::vector< double >& histogram,
                                double& lowerBoundary,
                                double& upperBoundary,
                                double kernelWidth = -1.0,
                                bool normalize = true ) const;


// force instanciations of getCumulatedHistogram for current types
template
void gkg::HistogramAnalyzer::getCumulatedHistogram(
                                const gkg::Volume< int8_t >& volume,
                                std::vector< double >& cumulatedHistogram,
                                int8_t& lowerBoundary,
                                int8_t& upperBoundary,
                                double kernelWidth = -1.0,
                                bool normalize = true ) const;
template
void gkg::HistogramAnalyzer::getCumulatedHistogram(
                                const gkg::Volume< uint8_t >& volume,
                                std::vector< double >& cumulatedHistogram,
                                uint8_t& lowerBoundary,
                                uint8_t& upperBoundary,
                                double kernelWidth = -1.0,
                                bool normalize = true ) const;
template
void gkg::HistogramAnalyzer::getCumulatedHistogram(
                                const gkg::Volume< int16_t >& volume,
                                std::vector< double >& cumulatedHistogram,
                                int16_t& lowerBoundary,
                                int16_t& upperBoundary,
                                double kernelWidth = -1.0,
                                bool normalize = true ) const;
template
void gkg::HistogramAnalyzer::getCumulatedHistogram(
                                const gkg::Volume< uint16_t >& volume,
                                std::vector< double >& cumulatedHistogram,
                                uint16_t& lowerBoundary,
                                uint16_t& upperBoundary,
                                double kernelWidth = -1.0,
                                bool normalize = true ) const;
template
void gkg::HistogramAnalyzer::getCumulatedHistogram(
                                const gkg::Volume< int32_t >& volume,
                                std::vector< double >& cumulatedHistogram,
                                int32_t& lowerBoundary,
                                int32_t& upperBoundary,
                                double kernelWidth = -1.0,
                                bool normalize = true ) const;
template
void gkg::HistogramAnalyzer::getCumulatedHistogram(
                                const gkg::Volume< uint32_t >& volume,
                                std::vector< double >& cumulatedHistogram,
                                uint32_t& lowerBoundary,
                                uint32_t& upperBoundary,
                                double kernelWidth = -1.0,
                                bool normalize = true ) const;
template
void gkg::HistogramAnalyzer::getCumulatedHistogram(
                                const gkg::Volume< int64_t >& volume,
                                std::vector< double >& cumulatedHistogram,
                                int64_t& lowerBoundary,
                                int64_t& upperBoundary,
                                double kernelWidth = -1.0,
                                bool normalize = true ) const;
template
void gkg::HistogramAnalyzer::getCumulatedHistogram(
                                const gkg::Volume< uint64_t >& volume,
                                std::vector< double >& cumulatedHistogram,
                                uint64_t& lowerBoundary,
                                uint64_t& upperBoundary,
                                double kernelWidth = -1.0,
                                bool normalize = true ) const;
template
void gkg::HistogramAnalyzer::getCumulatedHistogram(
                                const gkg::Volume< float >& volume,
                                std::vector< double >& cumulatedHistogram,
                                float& lowerBoundary,
                                float& upperBoundary,
                                double kernelWidth = -1.0,
                                bool normalize = true ) const;
template
void gkg::HistogramAnalyzer::getCumulatedHistogram(
                                const gkg::Volume< double >& volume,
                                std::vector< double >& cumulatedHistogram,
                                double& lowerBoundary,
                                double& upperBoundary,
                                double kernelWidth = -1.0,
                                bool normalize = true ) const;


// force instanciations of getPercentileUpperBoundary for current types
template
void gkg::HistogramAnalyzer::getPercentileUpperBoundaries(
                                const gkg::Volume< int8_t >& volume,
                                int32_t levelCount,
                                double percent,
                                int8_t& lowerBoundary,
                                int8_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getPercentileUpperBoundaries(
                                const gkg::Volume< uint8_t >& volume,
                                int32_t levelCount,
                                double percent,
                                uint8_t& lowerBoundary,
                                uint8_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getPercentileUpperBoundaries(
                                const gkg::Volume< int16_t >& volume,
                                int32_t levelCount,
                                double percent,
                                int16_t& lowerBoundary,
                                int16_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getPercentileUpperBoundaries(
                                const gkg::Volume< uint16_t >& volume,
                                int32_t levelCount,
                                double percent,
                                uint16_t& lowerBoundary,
                                uint16_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getPercentileUpperBoundaries(
                                const gkg::Volume< int32_t >& volume,
                                int32_t levelCount,
                                double percent,
                                int32_t& lowerBoundary,
                                int32_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getPercentileUpperBoundaries(
                                const gkg::Volume< uint32_t >& volume,
                                int32_t levelCount,
                                double percent,
                                uint32_t& lowerBoundary,
                                uint32_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getPercentileUpperBoundaries(
                                const gkg::Volume< int64_t >& volume,
                                int32_t levelCount,
                                double percent,
                                int64_t& lowerBoundary,
                                int64_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getPercentileUpperBoundaries(
                                const gkg::Volume< uint64_t >& volume,
                                int32_t levelCount,
                                double percent,
                                uint64_t& lowerBoundary,
                                uint64_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getPercentileUpperBoundaries(
                                const gkg::Volume< float >& volume,
                                int32_t levelCount,
                                double percent,
                                float& lowerBoundary,
                                float& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getPercentileUpperBoundaries(
                                const gkg::Volume< double >& volume,
                                int32_t levelCount,
                                double percent,
                                double& lowerBoundary,
                                double& upperBoundary ) const;


// force instanciations of getPercentileLowerBoundary for current types
template
void gkg::HistogramAnalyzer::getPercentileLowerBoundaries(
                                const gkg::Volume< int8_t >& volume,
                                int32_t levelCount,
                                double percent,
                                int8_t& lowerBoundary,
                                int8_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getPercentileLowerBoundaries(
                                const gkg::Volume< uint8_t >& volume,
                                int32_t levelCount,
                                double percent,
                                uint8_t& lowerBoundary,
                                uint8_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getPercentileLowerBoundaries(
                                const gkg::Volume< int16_t >& volume,
                                int32_t levelCount,
                                double percent,
                                int16_t& lowerBoundary,
                                int16_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getPercentileLowerBoundaries(
                                const gkg::Volume< uint16_t >& volume,
                                int32_t levelCount,
                                double percent,
                                uint16_t& lowerBoundary,
                                uint16_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getPercentileLowerBoundaries(
                                const gkg::Volume< int32_t >& volume,
                                int32_t levelCount,
                                double percent,
                                int32_t& lowerBoundary,
                                int32_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getPercentileLowerBoundaries(
                                const gkg::Volume< uint32_t >& volume,
                                int32_t levelCount,
                                double percent,
                                uint32_t& lowerBoundary,
                                uint32_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getPercentileLowerBoundaries(
                                const gkg::Volume< int64_t >& volume,
                                int32_t levelCount,
                                double percent,
                                int64_t& lowerBoundary,
                                int64_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getPercentileLowerBoundaries(
                                const gkg::Volume< uint64_t >& volume,
                                int32_t levelCount,
                                double percent,
                                uint64_t& lowerBoundary,
                                uint64_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getPercentileLowerBoundaries(
                                const gkg::Volume< float >& volume,
                                int32_t levelCount,
                                double percent,
                                float& lowerBoundary,
                                float& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getPercentileLowerBoundaries(
                                const gkg::Volume< double >& volume,
                                int32_t levelCount,
                                double percent,
                                double& lowerBoundary,
                                double& upperBoundary ) const;


// force instanciations of getPercentileBoundaries for current types
template
void gkg::HistogramAnalyzer::getPercentileBoundaries(
                                const gkg::Volume< int8_t >& volume,
                                int32_t levelCount,
                                double percent,
                                int8_t& lowerBoundary,
                                int8_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getPercentileBoundaries(
                                const gkg::Volume< uint8_t >& volume,
                                int32_t levelCount,
                                double percent,
                                uint8_t& lowerBoundary,
                                uint8_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getPercentileBoundaries(
                                const gkg::Volume< int16_t >& volume,
                                int32_t levelCount,
                                double percent,
                                int16_t& lowerBoundary,
                                int16_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getPercentileBoundaries(
                                const gkg::Volume< uint16_t >& volume,
                                int32_t levelCount,
                                double percent,
                                uint16_t& lowerBoundary,
                                uint16_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getPercentileBoundaries(
                                const gkg::Volume< int32_t >& volume,
                                int32_t levelCount,
                                double percent,
                                int32_t& lowerBoundary,
                                int32_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getPercentileBoundaries(
                                const gkg::Volume< uint32_t >& volume,
                                int32_t levelCount,
                                double percent,
                                uint32_t& lowerBoundary,
                                uint32_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getPercentileBoundaries(
                                const gkg::Volume< int64_t >& volume,
                                int32_t levelCount,
                                double percent,
                                int64_t& lowerBoundary,
                                int64_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getPercentileBoundaries(
                                const gkg::Volume< uint64_t >& volume,
                                int32_t levelCount,
                                double percent,
                                uint64_t& lowerBoundary,
                                uint64_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getPercentileBoundaries(
                                const gkg::Volume< float >& volume,
                                int32_t levelCount,
                                double percent,
                                float& lowerBoundary,
                                float& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getPercentileBoundaries(
                                const gkg::Volume< double >& volume,
                                int32_t levelCount,
                                double percent,
                                double& lowerBoundary,
                                double& upperBoundary ) const;


// force instanciations of getPercentileBoundariesAndMinMax for current types
template
void gkg::HistogramAnalyzer::getPercentileBoundariesAndMinMax(
                                const gkg::Volume< int8_t >& volume,
                                int32_t levelCount,
                                double percent,
                                int8_t& lowerBoundary,
                                int8_t& upperBoundary,
                                int8_t& minimum,
                                int8_t& maximum ) const;
template
void gkg::HistogramAnalyzer::getPercentileBoundariesAndMinMax(
                                const gkg::Volume< uint8_t >& volume,
                                int32_t levelCount,
                                double percent,
                                uint8_t& lowerBoundary,
                                uint8_t& upperBoundary,
                                uint8_t& minimum,
                                uint8_t& maximum ) const;
template
void gkg::HistogramAnalyzer::getPercentileBoundariesAndMinMax(
                                const gkg::Volume< int16_t >& volume,
                                int32_t levelCount,
                                double percent,
                                int16_t& lowerBoundary,
                                int16_t& upperBoundary,
                                int16_t& minimum,
                                int16_t& maximum ) const;
template
void gkg::HistogramAnalyzer::getPercentileBoundariesAndMinMax(
                                const gkg::Volume< uint16_t >& volume,
                                int32_t levelCount,
                                double percent,
                                uint16_t& lowerBoundary,
                                uint16_t& upperBoundary,
                                uint16_t& minimum,
                                uint16_t& maximum ) const;
template
void gkg::HistogramAnalyzer::getPercentileBoundariesAndMinMax(
                                const gkg::Volume< int32_t >& volume,
                                int32_t levelCount,
                                double percent,
                                int32_t& lowerBoundary,
                                int32_t& upperBoundary,
                                int32_t& minimum,
                                int32_t& maximum ) const;
template
void gkg::HistogramAnalyzer::getPercentileBoundariesAndMinMax(
                                const gkg::Volume< uint32_t >& volume,
                                int32_t levelCount,
                                double percent,
                                uint32_t& lowerBoundary,
                                uint32_t& upperBoundary,
                                uint32_t& minimum,
                                uint32_t& maximum ) const;
template
void gkg::HistogramAnalyzer::getPercentileBoundariesAndMinMax(
                                const gkg::Volume< int64_t >& volume,
                                int32_t levelCount,
                                double percent,
                                int64_t& lowerBoundary,
                                int64_t& upperBoundary,
                                int64_t& minimum,
                                int64_t& maximum ) const;
template
void gkg::HistogramAnalyzer::getPercentileBoundariesAndMinMax(
                                const gkg::Volume< uint64_t >& volume,
                                int32_t levelCount,
                                double percent,
                                uint64_t& lowerBoundary,
                                uint64_t& upperBoundary,
                                uint64_t& minimum,
                                uint64_t& maximum ) const;
template
void gkg::HistogramAnalyzer::getPercentileBoundariesAndMinMax(
                                const gkg::Volume< float >& volume,
                                int32_t levelCount,
                                double percent,
                                float& lowerBoundary,
                                float& upperBoundary,
                                float& minimum,
                                float& maximum ) const;
template
void gkg::HistogramAnalyzer::getPercentileBoundariesAndMinMax(
                                const gkg::Volume< double >& volume,
                                int32_t levelCount,
                                double percent,
                                double& lowerBoundary,
                                double& upperBoundary,
                                double& minimum,
                                double& maximum ) const;


//
// methods with mask for volume(s)
//


// force instanciations of getBoundaries for current types
template
void gkg::HistogramAnalyzer::getBoundaries(
                                const gkg::Volume< int8_t >& volume,
                                const gkg::Volume< int16_t >& mask,
                                int8_t& lowerBoundary,
                                int8_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getBoundaries(
                                const gkg::Volume< uint8_t >& volume,
                                const gkg::Volume< int16_t >& mask,
                                uint8_t& lowerBoundary,
                                uint8_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getBoundaries(
                                const gkg::Volume< int16_t >& volume,
                                const gkg::Volume< int16_t >& mask,
                                int16_t& lowerBoundary,
                                int16_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getBoundaries(
                                const gkg::Volume< uint16_t >& volume,
                                const gkg::Volume< int16_t >& mask,
                                uint16_t& lowerBoundary,
                                uint16_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getBoundaries(
                                const gkg::Volume< int32_t >& volume,
                                const gkg::Volume< int16_t >& mask,
                                int32_t& lowerBoundary,
                                int32_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getBoundaries(
                                const gkg::Volume< uint32_t >& volume,
                                const gkg::Volume< int16_t >& mask,
                                uint32_t& lowerBoundary,
                                uint32_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getBoundaries(
                                const gkg::Volume< int64_t >& volume,
                                const gkg::Volume< int16_t >& mask,
                                int64_t& lowerBoundary,
                                int64_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getBoundaries(
                                const gkg::Volume< uint64_t >& volume,
                                const gkg::Volume< int16_t >& mask,
                                uint64_t& lowerBoundary,
                                uint64_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getBoundaries(
                                const gkg::Volume< float >& volume,
                                const gkg::Volume< int16_t >& mask,
                                float& lowerBoundary,
                                float& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getBoundaries(
                                const gkg::Volume< double >& volume,
                                const gkg::Volume< int16_t >& mask,
                                double& lowerBoundary,
                                double& upperBoundary ) const;


// force instanciations of getHistogram for current types
template
void gkg::HistogramAnalyzer::getHistogram(
                                const gkg::Volume< int8_t >& volume,
                                const gkg::Volume< int16_t >& mask,
                                std::vector< double >& histogram,
                                int8_t& lowerBoundary,
                                int8_t& upperBoundary,
                                double kernelWidth = -1.0,
                                bool normalize = true ) const;
template
void gkg::HistogramAnalyzer::getHistogram(
                                const gkg::Volume< uint8_t >& volume,
                                const gkg::Volume< int16_t >& mask,
                                std::vector< double >& histogram,
                                uint8_t& lowerBoundary,
                                uint8_t& upperBoundary,
                                double kernelWidth = -1.0,
                                bool normalize = true ) const;
template
void gkg::HistogramAnalyzer::getHistogram(
                                const gkg::Volume< int16_t >& volume,
                                const gkg::Volume< int16_t >& mask,
                                std::vector< double >& histogram,
                                int16_t& lowerBoundary,
                                int16_t& upperBoundary,
                                double kernelWidth = -1.0,
                                bool normalize = true ) const;
template
void gkg::HistogramAnalyzer::getHistogram(
                                const gkg::Volume< uint16_t >& volume,
                                const gkg::Volume< int16_t >& mask,
                                std::vector< double >& histogram,
                                uint16_t& lowerBoundary,
                                uint16_t& upperBoundary,
                                double kernelWidth = -1.0,
                                bool normalize = true ) const;
template
void gkg::HistogramAnalyzer::getHistogram(
                                const gkg::Volume< int32_t >& volume,
                                const gkg::Volume< int16_t >& mask,
                                std::vector< double >& histogram,
                                int32_t& lowerBoundary,
                                int32_t& upperBoundary,
                                double kernelWidth = -1.0,
                                bool normalize = true ) const;
template
void gkg::HistogramAnalyzer::getHistogram(
                                const gkg::Volume< uint32_t >& volume,
                                const gkg::Volume< int16_t >& mask,
                                std::vector< double >& histogram,
                                uint32_t& lowerBoundary,
                                uint32_t& upperBoundary,
                                double kernelWidth = -1.0,
                                bool normalize = true ) const;
template
void gkg::HistogramAnalyzer::getHistogram(
                                const gkg::Volume< int64_t >& volume,
                                const gkg::Volume< int16_t >& mask,
                                std::vector< double >& histogram,
                                int64_t& lowerBoundary,
                                int64_t& upperBoundary,
                                double kernelWidth = -1.0,
                                bool normalize = true ) const;
template
void gkg::HistogramAnalyzer::getHistogram(
                                const gkg::Volume< uint64_t >& volume,
                                const gkg::Volume< int16_t >& mask,
                                std::vector< double >& histogram,
                                uint64_t& lowerBoundary,
                                uint64_t& upperBoundary,
                                double kernelWidth = -1.0,
                                bool normalize = true ) const;
template
void gkg::HistogramAnalyzer::getHistogram(
                                const gkg::Volume< float >& volume,
                                const gkg::Volume< int16_t >& mask,
                                std::vector< double >& histogram,
                                float& lowerBoundary,
                                float& upperBoundary,
                                double kernelWidth = -1.0,
                                bool normalize ) const;
template
void gkg::HistogramAnalyzer::getHistogram(
                                const gkg::Volume< double >& volume,
                                const gkg::Volume< int16_t >& mask,
                                std::vector< double >& histogram,
                                double& lowerBoundary,
                                double& upperBoundary,
                                double kernelWidth = -1.0,
                                bool normalize = true ) const;


// force instanciations of getCumulatedHistogram for current types
template
void gkg::HistogramAnalyzer::getCumulatedHistogram(
                                const gkg::Volume< int8_t >& volume,
                                const gkg::Volume< int16_t >& mask,
                                std::vector< double >& cumulatedHistogram,
                                int8_t& lowerBoundary,
                                int8_t& upperBoundary,
                                double kernelWidth = -1.0,
                                bool normalize = true ) const;
template
void gkg::HistogramAnalyzer::getCumulatedHistogram(
                                const gkg::Volume< uint8_t >& volume,
                                const gkg::Volume< int16_t >& mask,
                                std::vector< double >& cumulatedHistogram,
                                uint8_t& lowerBoundary,
                                uint8_t& upperBoundary,
                                double kernelWidth = -1.0,
                                bool normalize = true ) const;
template
void gkg::HistogramAnalyzer::getCumulatedHistogram(
                                const gkg::Volume< int16_t >& volume,
                                const gkg::Volume< int16_t >& mask,
                                std::vector< double >& cumulatedHistogram,
                                int16_t& lowerBoundary,
                                int16_t& upperBoundary,
                                double kernelWidth = -1.0,
                                bool normalize = true ) const;
template
void gkg::HistogramAnalyzer::getCumulatedHistogram(
                                const gkg::Volume< uint16_t >& volume,
                                const gkg::Volume< int16_t >& mask,
                                std::vector< double >& cumulatedHistogram,
                                uint16_t& lowerBoundary,
                                uint16_t& upperBoundary,
                                double kernelWidth = -1.0,
                                bool normalize = true ) const;
template
void gkg::HistogramAnalyzer::getCumulatedHistogram(
                                const gkg::Volume< int32_t >& volume,
                                const gkg::Volume< int16_t >& mask,
                                std::vector< double >& cumulatedHistogram,
                                int32_t& lowerBoundary,
                                int32_t& upperBoundary,
                                double kernelWidth = -1.0,
                                bool normalize = true ) const;
template
void gkg::HistogramAnalyzer::getCumulatedHistogram(
                                const gkg::Volume< uint32_t >& volume,
                                const gkg::Volume< int16_t >& mask,
                                std::vector< double >& cumulatedHistogram,
                                uint32_t& lowerBoundary,
                                uint32_t& upperBoundary,
                                double kernelWidth = -1.0,
                                bool normalize = true ) const;
template
void gkg::HistogramAnalyzer::getCumulatedHistogram(
                                const gkg::Volume< int64_t >& volume,
                                const gkg::Volume< int16_t >& mask,
                                std::vector< double >& cumulatedHistogram,
                                int64_t& lowerBoundary,
                                int64_t& upperBoundary,
                                double kernelWidth = -1.0,
                                bool normalize = true ) const;
template
void gkg::HistogramAnalyzer::getCumulatedHistogram(
                                const gkg::Volume< uint64_t >& volume,
                                const gkg::Volume< int16_t >& mask,
                                std::vector< double >& cumulatedHistogram,
                                uint64_t& lowerBoundary,
                                uint64_t& upperBoundary,
                                double kernelWidth = -1.0,
                                bool normalize = true ) const;
template
void gkg::HistogramAnalyzer::getCumulatedHistogram(
                                const gkg::Volume< float >& volume,
                                const gkg::Volume< int16_t >& mask,
                                std::vector< double >& cumulatedHistogram,
                                float& lowerBoundary,
                                float& upperBoundary,
                                double kernelWidth = -1.0,
                                bool normalize = true ) const;
template
void gkg::HistogramAnalyzer::getCumulatedHistogram(
                                const gkg::Volume< double >& volume,
                                const gkg::Volume< int16_t >& mask,
                                std::vector< double >& cumulatedHistogram,
                                double& lowerBoundary,
                                double& upperBoundary,
                                double kernelWidth = -1.0,
                                bool normalize = true ) const;


// force instanciations of getPercentileUpperBoundary for current types
template
void gkg::HistogramAnalyzer::getPercentileUpperBoundaries(
                                const gkg::Volume< int8_t >& volume,
                                const gkg::Volume< int16_t >& mask,
                                int32_t levelCount,
                                double percent,
                                int8_t& lowerBoundary,
                                int8_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getPercentileUpperBoundaries(
                                const gkg::Volume< uint8_t >& volume,
                                const gkg::Volume< int16_t >& mask,
                                int32_t levelCount,
                                double percent,
                                uint8_t& lowerBoundary,
                                uint8_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getPercentileUpperBoundaries(
                                const gkg::Volume< int16_t >& volume,
                                const gkg::Volume< int16_t >& mask,
                                int32_t levelCount,
                                double percent,
                                int16_t& lowerBoundary,
                                int16_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getPercentileUpperBoundaries(
                                const gkg::Volume< uint16_t >& volume,
                                const gkg::Volume< int16_t >& mask,
                                int32_t levelCount,
                                double percent,
                                uint16_t& lowerBoundary,
                                uint16_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getPercentileUpperBoundaries(
                                const gkg::Volume< int32_t >& volume,
                                const gkg::Volume< int16_t >& mask,
                                int32_t levelCount,
                                double percent,
                                int32_t& lowerBoundary,
                                int32_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getPercentileUpperBoundaries(
                                const gkg::Volume< uint32_t >& volume,
                                const gkg::Volume< int16_t >& mask,
                                int32_t levelCount,
                                double percent,
                                uint32_t& lowerBoundary,
                                uint32_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getPercentileUpperBoundaries(
                                const gkg::Volume< int64_t >& volume,
                                const gkg::Volume< int16_t >& mask,
                                int32_t levelCount,
                                double percent,
                                int64_t& lowerBoundary,
                                int64_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getPercentileUpperBoundaries(
                                const gkg::Volume< uint64_t >& volume,
                                const gkg::Volume< int16_t >& mask,
                                int32_t levelCount,
                                double percent,
                                uint64_t& lowerBoundary,
                                uint64_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getPercentileUpperBoundaries(
                                const gkg::Volume< float >& volume,
                                const gkg::Volume< int16_t >& mask,
                                int32_t levelCount,
                                double percent,
                                float& lowerBoundary,
                                float& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getPercentileUpperBoundaries(
                                const gkg::Volume< double >& volume,
                                const gkg::Volume< int16_t >& mask,
                                int32_t levelCount,
                                double percent,
                                double& lowerBoundary,
                                double& upperBoundary ) const;


// force instanciations of getPercentileLowerBoundary for current types
template
void gkg::HistogramAnalyzer::getPercentileLowerBoundaries(
                                const gkg::Volume< int8_t >& volume,
                                const gkg::Volume< int16_t >& mask,
                                int32_t levelCount,
                                double percent,
                                int8_t& lowerBoundary,
                                int8_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getPercentileLowerBoundaries(
                                const gkg::Volume< uint8_t >& volume,
                                const gkg::Volume< int16_t >& mask,
                                int32_t levelCount,
                                double percent,
                                uint8_t& lowerBoundary,
                                uint8_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getPercentileLowerBoundaries(
                                const gkg::Volume< int16_t >& volume,
                                const gkg::Volume< int16_t >& mask,
                                int32_t levelCount,
                                double percent,
                                int16_t& lowerBoundary,
                                int16_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getPercentileLowerBoundaries(
                                const gkg::Volume< uint16_t >& volume,
                                const gkg::Volume< int16_t >& mask,
                                int32_t levelCount,
                                double percent,
                                uint16_t& lowerBoundary,
                                uint16_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getPercentileLowerBoundaries(
                                const gkg::Volume< int32_t >& volume,
                                const gkg::Volume< int16_t >& mask,
                                int32_t levelCount,
                                double percent,
                                int32_t& lowerBoundary,
                                int32_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getPercentileLowerBoundaries(
                                const gkg::Volume< uint32_t >& volume,
                                const gkg::Volume< int16_t >& mask,
                                int32_t levelCount,
                                double percent,
                                uint32_t& lowerBoundary,
                                uint32_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getPercentileLowerBoundaries(
                                const gkg::Volume< int64_t >& volume,
                                const gkg::Volume< int16_t >& mask,
                                int32_t levelCount,
                                double percent,
                                int64_t& lowerBoundary,
                                int64_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getPercentileLowerBoundaries(
                                const gkg::Volume< uint64_t >& volume,
                                const gkg::Volume< int16_t >& mask,
                                int32_t levelCount,
                                double percent,
                                uint64_t& lowerBoundary,
                                uint64_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getPercentileLowerBoundaries(
                                const gkg::Volume< float >& volume,
                                const gkg::Volume< int16_t >& mask,
                                int32_t levelCount,
                                double percent,
                                float& lowerBoundary,
                                float& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getPercentileLowerBoundaries(
                                const gkg::Volume< double >& volume,
                                const gkg::Volume< int16_t >& mask,
                                int32_t levelCount,
                                double percent,
                                double& lowerBoundary,
                                double& upperBoundary ) const;


// force instanciations of getPercentileBoundary for current types
template
void gkg::HistogramAnalyzer::getPercentileBoundaries(
                                const gkg::Volume< int8_t >& volume,
                                const gkg::Volume< int16_t >& mask,
                                int32_t levelCount,
                                double percent,
                                int8_t& lowerBoundary,
                                int8_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getPercentileBoundaries(
                                const gkg::Volume< uint8_t >& volume,
                                const gkg::Volume< int16_t >& mask,
                                int32_t levelCount,
                                double percent,
                                uint8_t& lowerBoundary,
                                uint8_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getPercentileBoundaries(
                                const gkg::Volume< int16_t >& volume,
                                const gkg::Volume< int16_t >& mask,
                                int32_t levelCount,
                                double percent,
                                int16_t& lowerBoundary,
                                int16_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getPercentileBoundaries(
                                const gkg::Volume< uint16_t >& volume,
                                const gkg::Volume< int16_t >& mask,
                                int32_t levelCount,
                                double percent,
                                uint16_t& lowerBoundary,
                                uint16_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getPercentileBoundaries(
                                const gkg::Volume< int32_t >& volume,
                                const gkg::Volume< int16_t >& mask,
                                int32_t levelCount,
                                double percent,
                                int32_t& lowerBoundary,
                                int32_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getPercentileBoundaries(
                                const gkg::Volume< uint32_t >& volume,
                                const gkg::Volume< int16_t >& mask,
                                int32_t levelCount,
                                double percent,
                                uint32_t& lowerBoundary,
                                uint32_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getPercentileBoundaries(
                                const gkg::Volume< int64_t >& volume,
                                const gkg::Volume< int16_t >& mask,
                                int32_t levelCount,
                                double percent,
                                int64_t& lowerBoundary,
                                int64_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getPercentileBoundaries(
                                const gkg::Volume< uint64_t >& volume,
                                const gkg::Volume< int16_t >& mask,
                                int32_t levelCount,
                                double percent,
                                uint64_t& lowerBoundary,
                                uint64_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getPercentileBoundaries(
                                const gkg::Volume< float >& volume,
                                const gkg::Volume< int16_t >& mask,
                                int32_t levelCount,
                                double percent,
                                float& lowerBoundary,
                                float& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getPercentileBoundaries(
                                const gkg::Volume< double >& volume,
                                const gkg::Volume< int16_t >& mask,
                                int32_t levelCount,
                                double percent,
                                double& lowerBoundary,
                                double& upperBoundary ) const;


//
// methods without mask for vector(s)
//


// force instanciations of getBoundaries for current types
template
void gkg::HistogramAnalyzer::getBoundaries(
                                const std::vector< int8_t >& v,
                                int8_t& lowerBoundary,
                                int8_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getBoundaries(
                                const std::vector< uint8_t >& v,
                                uint8_t& lowerBoundary,
                                uint8_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getBoundaries(
                                const std::vector< int16_t >& v,
                                int16_t& lowerBoundary,
                                int16_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getBoundaries(
                                const std::vector< uint16_t >& v,
                                uint16_t& lowerBoundary,
                                uint16_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getBoundaries(
                                const std::vector< int32_t >& v,
                                int32_t& lowerBoundary,
                                int32_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getBoundaries(
                                const std::vector< uint32_t >& v,
                                uint32_t& lowerBoundary,
                                uint32_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getBoundaries(
                                const std::vector< int64_t >& v,
                                int64_t& lowerBoundary,
                                int64_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getBoundaries(
                                const std::vector< uint64_t >& v,
                                uint64_t& lowerBoundary,
                                uint64_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getBoundaries(
                                const std::vector< float >& v,
                                float& lowerBoundary,
                                float& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getBoundaries(
                                const std::vector< double >& v,
                                double& lowerBoundary,
                                double& upperBoundary ) const;


// force instanciations of getHistogram for current types
template
void gkg::HistogramAnalyzer::getHistogram(
                                const std::vector< int8_t >& v,
                                std::vector< double >& histogram,
                                int8_t& lowerBoundary,
                                int8_t& upperBoundary,
                                double kernelWidth = -1.0,
                                bool normalize = true ) const;
template
void gkg::HistogramAnalyzer::getHistogram(
                                const std::vector< uint8_t >& v,
                                std::vector< double >& histogram,
                                uint8_t& lowerBoundary,
                                uint8_t& upperBoundary,
                                double kernelWidth = -1.0,
                                bool normalize = true ) const;
template
void gkg::HistogramAnalyzer::getHistogram(
                                const std::vector< int16_t >& v,
                                std::vector< double >& histogram,
                                int16_t& lowerBoundary,
                                int16_t& upperBoundary,
                                double kernelWidth = -1.0,
                                bool normalize = true ) const;
template
void gkg::HistogramAnalyzer::getHistogram(
                                const std::vector< uint16_t >& v,
                                std::vector< double >& histogram,
                                uint16_t& lowerBoundary,
                                uint16_t& upperBoundary,
                                double kernelWidth = -1.0,
                                bool normalize = true ) const;
template
void gkg::HistogramAnalyzer::getHistogram(
                                const std::vector< int32_t >& v,
                                std::vector< double >& histogram,
                                int32_t& lowerBoundary,
                                int32_t& upperBoundary,
                                double kernelWidth = -1.0,
                                bool normalize = true ) const;
template
void gkg::HistogramAnalyzer::getHistogram(
                                const std::vector< uint32_t >& v,
                                std::vector< double >& histogram,
                                uint32_t& lowerBoundary,
                                uint32_t& upperBoundary,
                                double kernelWidth = -1.0,
                                bool normalize = true ) const;
template
void gkg::HistogramAnalyzer::getHistogram(
                                const std::vector< int64_t >& v,
                                std::vector< double >& histogram,
                                int64_t& lowerBoundary,
                                int64_t& upperBoundary,
                                double kernelWidth = -1.0,
                                bool normalize = true ) const;
template
void gkg::HistogramAnalyzer::getHistogram(
                                const std::vector< uint64_t >& v,
                                std::vector< double >& histogram,
                                uint64_t& lowerBoundary,
                                uint64_t& upperBoundary,
                                double kernelWidth = -1.0,
                                bool normalize = true ) const;
template
void gkg::HistogramAnalyzer::getHistogram(
                                const std::vector< float >& v,
                                std::vector< double >& histogram,
                                float& lowerBoundary,
                                float& upperBoundary,
                                double kernelWidth = -1.0,
                                bool normalize ) const;
template
void gkg::HistogramAnalyzer::getHistogram(
                                const std::vector< double >& v,
                                std::vector< double >& histogram,
                                double& lowerBoundary,
                                double& upperBoundary,
                                double kernelWidth = -1.0,
                                bool normalize = true ) const;


// force instanciations of getCumulatedHistogram for current types
template
void gkg::HistogramAnalyzer::getCumulatedHistogram(
                                const std::vector< int8_t >& v,
                                std::vector< double >& cumulatedHistogram,
                                int8_t& lowerBoundary,
                                int8_t& upperBoundary,
                                double kernelWidth = -1.0,
                                bool normalize = true ) const;
template
void gkg::HistogramAnalyzer::getCumulatedHistogram(
                                const std::vector< uint8_t >& v,
                                std::vector< double >& cumulatedHistogram,
                                uint8_t& lowerBoundary,
                                uint8_t& upperBoundary,
                                double kernelWidth = -1.0,
                                bool normalize = true ) const;
template
void gkg::HistogramAnalyzer::getCumulatedHistogram(
                                const std::vector< int16_t >& v,
                                std::vector< double >& cumulatedHistogram,
                                int16_t& lowerBoundary,
                                int16_t& upperBoundary,
                                double kernelWidth = -1.0,
                                bool normalize = true ) const;
template
void gkg::HistogramAnalyzer::getCumulatedHistogram(
                                const std::vector< uint16_t >& v,
                                std::vector< double >& cumulatedHistogram,
                                uint16_t& lowerBoundary,
                                uint16_t& upperBoundary,
                                double kernelWidth = -1.0,
                                bool normalize = true ) const;
template
void gkg::HistogramAnalyzer::getCumulatedHistogram(
                                const std::vector< int32_t >& v,
                                std::vector< double >& cumulatedHistogram,
                                int32_t& lowerBoundary,
                                int32_t& upperBoundary,
                                double kernelWidth = -1.0,
                                bool normalize = true ) const;
template
void gkg::HistogramAnalyzer::getCumulatedHistogram(
                                const std::vector< uint32_t >& v,
                                std::vector< double >& cumulatedHistogram,
                                uint32_t& lowerBoundary,
                                uint32_t& upperBoundary,
                                double kernelWidth = -1.0,
                                bool normalize = true ) const;
template
void gkg::HistogramAnalyzer::getCumulatedHistogram(
                                const std::vector< int64_t >& v,
                                std::vector< double >& cumulatedHistogram,
                                int64_t& lowerBoundary,
                                int64_t& upperBoundary,
                                double kernelWidth = -1.0,
                                bool normalize = true ) const;
template
void gkg::HistogramAnalyzer::getCumulatedHistogram(
                                const std::vector< uint64_t >& v,
                                std::vector< double >& cumulatedHistogram,
                                uint64_t& lowerBoundary,
                                uint64_t& upperBoundary,
                                double kernelWidth = -1.0,
                                bool normalize = true ) const;
template
void gkg::HistogramAnalyzer::getCumulatedHistogram(
                                const std::vector< float >& v,
                                std::vector< double >& cumulatedHistogram,
                                float& lowerBoundary,
                                float& upperBoundary,
                                double kernelWidth = -1.0,
                                bool normalize = true ) const;
template
void gkg::HistogramAnalyzer::getCumulatedHistogram(
                                const std::vector< double >& v,
                                std::vector< double >& cumulatedHistogram,
                                double& lowerBoundary,
                                double& upperBoundary,
                                double kernelWidth = -1.0,
                                bool normalize = true ) const;


// force instanciations of getPercentileUpperBoundary for current types
template
void gkg::HistogramAnalyzer::getPercentileUpperBoundaries(
                                const std::vector< int8_t >& v,
                                int32_t levelCount,
                                double percent,
                                int8_t& lowerBoundary,
                                int8_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getPercentileUpperBoundaries(
                                const std::vector< uint8_t >& v,
                                int32_t levelCount,
                                double percent,
                                uint8_t& lowerBoundary,
                                uint8_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getPercentileUpperBoundaries(
                                const std::vector< int16_t >& v,
                                int32_t levelCount,
                                double percent,
                                int16_t& lowerBoundary,
                                int16_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getPercentileUpperBoundaries(
                                const std::vector< uint16_t >& v,
                                int32_t levelCount,
                                double percent,
                                uint16_t& lowerBoundary,
                                uint16_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getPercentileUpperBoundaries(
                                const std::vector< int32_t >& v,
                                int32_t levelCount,
                                double percent,
                                int32_t& lowerBoundary,
                                int32_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getPercentileUpperBoundaries(
                                const std::vector< uint32_t >& v,
                                int32_t levelCount,
                                double percent,
                                uint32_t& lowerBoundary,
                                uint32_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getPercentileUpperBoundaries(
                                const std::vector< int64_t >& v,
                                int32_t levelCount,
                                double percent,
                                int64_t& lowerBoundary,
                                int64_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getPercentileUpperBoundaries(
                                const std::vector< uint64_t >& v,
                                int32_t levelCount,
                                double percent,
                                uint64_t& lowerBoundary,
                                uint64_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getPercentileUpperBoundaries(
                                const std::vector< float >& v,
                                int32_t levelCount,
                                double percent,
                                float& lowerBoundary,
                                float& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getPercentileUpperBoundaries(
                                const std::vector< double >& v,
                                int32_t levelCount,
                                double percent,
                                double& lowerBoundary,
                                double& upperBoundary ) const;


// force instanciations of getPercentileLowerBoundary for current types
template
void gkg::HistogramAnalyzer::getPercentileLowerBoundaries(
                                const std::vector< int8_t >& v,
                                int32_t levelCount,
                                double percent,
                                int8_t& lowerBoundary,
                                int8_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getPercentileLowerBoundaries(
                                const std::vector< uint8_t >& v,
                                int32_t levelCount,
                                double percent,
                                uint8_t& lowerBoundary,
                                uint8_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getPercentileLowerBoundaries(
                                const std::vector< int16_t >& v,
                                int32_t levelCount,
                                double percent,
                                int16_t& lowerBoundary,
                                int16_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getPercentileLowerBoundaries(
                                const std::vector< uint16_t >& v,
                                int32_t levelCount,
                                double percent,
                                uint16_t& lowerBoundary,
                                uint16_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getPercentileLowerBoundaries(
                                const std::vector< int32_t >& v,
                                int32_t levelCount,
                                double percent,
                                int32_t& lowerBoundary,
                                int32_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getPercentileLowerBoundaries(
                                const std::vector< uint32_t >& v,
                                int32_t levelCount,
                                double percent,
                                uint32_t& lowerBoundary,
                                uint32_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getPercentileLowerBoundaries(
                                const std::vector< int64_t >& v,
                                int32_t levelCount,
                                double percent,
                                int64_t& lowerBoundary,
                                int64_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getPercentileLowerBoundaries(
                                const std::vector< uint64_t >& v,
                                int32_t levelCount,
                                double percent,
                                uint64_t& lowerBoundary,
                                uint64_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getPercentileLowerBoundaries(
                                const std::vector< float >& v,
                                int32_t levelCount,
                                double percent,
                                float& lowerBoundary,
                                float& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getPercentileLowerBoundaries(
                                const std::vector< double >& v,
                                int32_t levelCount,
                                double percent,
                                double& lowerBoundary,
                                double& upperBoundary ) const;


// force instanciations of getPercentileBoundary for current types
template
void gkg::HistogramAnalyzer::getPercentileBoundaries(
                                const std::vector< int8_t >& v,
                                int32_t levelCount,
                                double percent,
                                int8_t& lowerBoundary,
                                int8_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getPercentileBoundaries(
                                const std::vector< uint8_t >& v,
                                int32_t levelCount,
                                double percent,
                                uint8_t& lowerBoundary,
                                uint8_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getPercentileBoundaries(
                                const std::vector< int16_t >& v,
                                int32_t levelCount,
                                double percent,
                                int16_t& lowerBoundary,
                                int16_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getPercentileBoundaries(
                                const std::vector< uint16_t >& v,
                                int32_t levelCount,
                                double percent,
                                uint16_t& lowerBoundary,
                                uint16_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getPercentileBoundaries(
                                const std::vector< int32_t >& v,
                                int32_t levelCount,
                                double percent,
                                int32_t& lowerBoundary,
                                int32_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getPercentileBoundaries(
                                const std::vector< uint32_t >& v,
                                int32_t levelCount,
                                double percent,
                                uint32_t& lowerBoundary,
                                uint32_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getPercentileBoundaries(
                                const std::vector< int64_t >& v,
                                int32_t levelCount,
                                double percent,
                                int64_t& lowerBoundary,
                                int64_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getPercentileBoundaries(
                                const std::vector< uint64_t >& v,
                                int32_t levelCount,
                                double percent,
                                uint64_t& lowerBoundary,
                                uint64_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getPercentileBoundaries(
                                const std::vector< float >& v,
                                int32_t levelCount,
                                double percent,
                                float& lowerBoundary,
                                float& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getPercentileBoundaries(
                                const std::vector< double >& v,
                                int32_t levelCount,
                                double percent,
                                double& lowerBoundary,
                                double& upperBoundary ) const;


//
// methods without mask for list(s)
//


// force instanciations of getBoundaries for current types
template
void gkg::HistogramAnalyzer::getBoundaries(
                                const std::list< int8_t >& v,
                                int8_t& lowerBoundary,
                                int8_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getBoundaries(
                                const std::list< uint8_t >& v,
                                uint8_t& lowerBoundary,
                                uint8_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getBoundaries(
                                const std::list< int16_t >& v,
                                int16_t& lowerBoundary,
                                int16_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getBoundaries(
                                const std::list< uint16_t >& v,
                                uint16_t& lowerBoundary,
                                uint16_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getBoundaries(
                                const std::list< int32_t >& v,
                                int32_t& lowerBoundary,
                                int32_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getBoundaries(
                                const std::list< uint32_t >& v,
                                uint32_t& lowerBoundary,
                                uint32_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getBoundaries(
                                const std::list< int64_t >& v,
                                int64_t& lowerBoundary,
                                int64_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getBoundaries(
                                const std::list< uint64_t >& v,
                                uint64_t& lowerBoundary,
                                uint64_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getBoundaries(
                                const std::list< float >& v,
                                float& lowerBoundary,
                                float& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getBoundaries(
                                const std::list< double >& v,
                                double& lowerBoundary,
                                double& upperBoundary ) const;


// force instanciations of getHistogram for current types
template
void gkg::HistogramAnalyzer::getHistogram(
                                const std::list< int8_t >& v,
                                std::vector< double >& histogram,
                                int8_t& lowerBoundary,
                                int8_t& upperBoundary,
                                double kernelWidth = -1.0,
                                bool normalize = true ) const;
template
void gkg::HistogramAnalyzer::getHistogram(
                                const std::list< uint8_t >& v,
                                std::vector< double >& histogram,
                                uint8_t& lowerBoundary,
                                uint8_t& upperBoundary,
                                double kernelWidth = -1.0,
                                bool normalize = true ) const;
template
void gkg::HistogramAnalyzer::getHistogram(
                                const std::list< int16_t >& v,
                                std::vector< double >& histogram,
                                int16_t& lowerBoundary,
                                int16_t& upperBoundary,
                                double kernelWidth = -1.0,
                                bool normalize = true ) const;
template
void gkg::HistogramAnalyzer::getHistogram(
                                const std::list< uint16_t >& v,
                                std::vector< double >& histogram,
                                uint16_t& lowerBoundary,
                                uint16_t& upperBoundary,
                                double kernelWidth = -1.0,
                                bool normalize = true ) const;
template
void gkg::HistogramAnalyzer::getHistogram(
                                const std::list< int32_t >& v,
                                std::vector< double >& histogram,
                                int32_t& lowerBoundary,
                                int32_t& upperBoundary,
                                double kernelWidth = -1.0,
                                bool normalize = true ) const;
template
void gkg::HistogramAnalyzer::getHistogram(
                                const std::list< uint32_t >& v,
                                std::vector< double >& histogram,
                                uint32_t& lowerBoundary,
                                uint32_t& upperBoundary,
                                double kernelWidth = -1.0,
                                bool normalize = true ) const;
template
void gkg::HistogramAnalyzer::getHistogram(
                                const std::list< int64_t >& v,
                                std::vector< double >& histogram,
                                int64_t& lowerBoundary,
                                int64_t& upperBoundary,
                                double kernelWidth = -1.0,
                                bool normalize = true ) const;
template
void gkg::HistogramAnalyzer::getHistogram(
                                const std::list< uint64_t >& v,
                                std::vector< double >& histogram,
                                uint64_t& lowerBoundary,
                                uint64_t& upperBoundary,
                                double kernelWidth = -1.0,
                                bool normalize = true ) const;
template
void gkg::HistogramAnalyzer::getHistogram(
                                const std::list< float >& v,
                                std::vector< double >& histogram,
                                float& lowerBoundary,
                                float& upperBoundary,
                                double kernelWidth = -1.0,
                                bool normalize ) const;
template
void gkg::HistogramAnalyzer::getHistogram(
                                const std::list< double >& v,
                                std::vector< double >& histogram,
                                double& lowerBoundary,
                                double& upperBoundary,
                                double kernelWidth = -1.0,
                                bool normalize = true ) const;


// force instanciations of getCumulatedHistogram for current types
template
void gkg::HistogramAnalyzer::getCumulatedHistogram(
                                const std::list< int8_t >& v,
                                std::vector< double >& cumulatedHistogram,
                                int8_t& lowerBoundary,
                                int8_t& upperBoundary,
                                double kernelWidth = -1.0,
                                bool normalize = true ) const;
template
void gkg::HistogramAnalyzer::getCumulatedHistogram(
                                const std::list< uint8_t >& v,
                                std::vector< double >& cumulatedHistogram,
                                uint8_t& lowerBoundary,
                                uint8_t& upperBoundary,
                                double kernelWidth = -1.0,
                                bool normalize = true ) const;
template
void gkg::HistogramAnalyzer::getCumulatedHistogram(
                                const std::list< int16_t >& v,
                                std::vector< double >& cumulatedHistogram,
                                int16_t& lowerBoundary,
                                int16_t& upperBoundary,
                                double kernelWidth = -1.0,
                                bool normalize = true ) const;
template
void gkg::HistogramAnalyzer::getCumulatedHistogram(
                                const std::list< uint16_t >& v,
                                std::vector< double >& cumulatedHistogram,
                                uint16_t& lowerBoundary,
                                uint16_t& upperBoundary,
                                double kernelWidth = -1.0,
                                bool normalize = true ) const;
template
void gkg::HistogramAnalyzer::getCumulatedHistogram(
                                const std::list< int32_t >& v,
                                std::vector< double >& cumulatedHistogram,
                                int32_t& lowerBoundary,
                                int32_t& upperBoundary,
                                double kernelWidth = -1.0,
                                bool normalize = true ) const;
template
void gkg::HistogramAnalyzer::getCumulatedHistogram(
                                const std::list< uint32_t >& v,
                                std::vector< double >& cumulatedHistogram,
                                uint32_t& lowerBoundary,
                                uint32_t& upperBoundary,
                                double kernelWidth = -1.0,
                                bool normalize = true ) const;
template
void gkg::HistogramAnalyzer::getCumulatedHistogram(
                                const std::list< int64_t >& v,
                                std::vector< double >& cumulatedHistogram,
                                int64_t& lowerBoundary,
                                int64_t& upperBoundary,
                                double kernelWidth = -1.0,
                                bool normalize = true ) const;
template
void gkg::HistogramAnalyzer::getCumulatedHistogram(
                                const std::list< uint64_t >& v,
                                std::vector< double >& cumulatedHistogram,
                                uint64_t& lowerBoundary,
                                uint64_t& upperBoundary,
                                double kernelWidth = -1.0,
                                bool normalize = true ) const;
template
void gkg::HistogramAnalyzer::getCumulatedHistogram(
                                const std::list< float >& v,
                                std::vector< double >& cumulatedHistogram,
                                float& lowerBoundary,
                                float& upperBoundary,
                                double kernelWidth = -1.0,
                                bool normalize = true ) const;
template
void gkg::HistogramAnalyzer::getCumulatedHistogram(
                                const std::list< double >& v,
                                std::vector< double >& cumulatedHistogram,
                                double& lowerBoundary,
                                double& upperBoundary,
                                double kernelWidth = -1.0,
                                bool normalize = true ) const;


// force instanciations of getPercentileUpperBoundary for current types
template
void gkg::HistogramAnalyzer::getPercentileUpperBoundaries(
                                const std::list< int8_t >& v,
                                int32_t levelCount,
                                double percent,
                                int8_t& lowerBoundary,
                                int8_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getPercentileUpperBoundaries(
                                const std::list< uint8_t >& v,
                                int32_t levelCount,
                                double percent,
                                uint8_t& lowerBoundary,
                                uint8_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getPercentileUpperBoundaries(
                                const std::list< int16_t >& v,
                                int32_t levelCount,
                                double percent,
                                int16_t& lowerBoundary,
                                int16_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getPercentileUpperBoundaries(
                                const std::list< uint16_t >& v,
                                int32_t levelCount,
                                double percent,
                                uint16_t& lowerBoundary,
                                uint16_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getPercentileUpperBoundaries(
                                const std::list< int32_t >& v,
                                int32_t levelCount,
                                double percent,
                                int32_t& lowerBoundary,
                                int32_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getPercentileUpperBoundaries(
                                const std::list< uint32_t >& v,
                                int32_t levelCount,
                                double percent,
                                uint32_t& lowerBoundary,
                                uint32_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getPercentileUpperBoundaries(
                                const std::list< int64_t >& v,
                                int32_t levelCount,
                                double percent,
                                int64_t& lowerBoundary,
                                int64_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getPercentileUpperBoundaries(
                                const std::list< uint64_t >& v,
                                int32_t levelCount,
                                double percent,
                                uint64_t& lowerBoundary,
                                uint64_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getPercentileUpperBoundaries(
                                const std::list< float >& v,
                                int32_t levelCount,
                                double percent,
                                float& lowerBoundary,
                                float& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getPercentileUpperBoundaries(
                                const std::list< double >& v,
                                int32_t levelCount,
                                double percent,
                                double& lowerBoundary,
                                double& upperBoundary ) const;


// force instanciations of getPercentileLowerBoundary for current types
template
void gkg::HistogramAnalyzer::getPercentileLowerBoundaries(
                                const std::list< int8_t >& v,
                                int32_t levelCount,
                                double percent,
                                int8_t& lowerBoundary,
                                int8_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getPercentileLowerBoundaries(
                                const std::list< uint8_t >& v,
                                int32_t levelCount,
                                double percent,
                                uint8_t& lowerBoundary,
                                uint8_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getPercentileLowerBoundaries(
                                const std::list< int16_t >& v,
                                int32_t levelCount,
                                double percent,
                                int16_t& lowerBoundary,
                                int16_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getPercentileLowerBoundaries(
                                const std::list< uint16_t >& v,
                                int32_t levelCount,
                                double percent,
                                uint16_t& lowerBoundary,
                                uint16_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getPercentileLowerBoundaries(
                                const std::list< int32_t >& v,
                                int32_t levelCount,
                                double percent,
                                int32_t& lowerBoundary,
                                int32_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getPercentileLowerBoundaries(
                                const std::list< uint32_t >& v,
                                int32_t levelCount,
                                double percent,
                                uint32_t& lowerBoundary,
                                uint32_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getPercentileLowerBoundaries(
                                const std::list< int64_t >& v,
                                int32_t levelCount,
                                double percent,
                                int64_t& lowerBoundary,
                                int64_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getPercentileLowerBoundaries(
                                const std::list< uint64_t >& v,
                                int32_t levelCount,
                                double percent,
                                uint64_t& lowerBoundary,
                                uint64_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getPercentileLowerBoundaries(
                                const std::list< float >& v,
                                int32_t levelCount,
                                double percent,
                                float& lowerBoundary,
                                float& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getPercentileLowerBoundaries(
                                const std::list< double >& v,
                                int32_t levelCount,
                                double percent,
                                double& lowerBoundary,
                                double& upperBoundary ) const;


// force instanciations of getPercentileBoundary for current types
template
void gkg::HistogramAnalyzer::getPercentileBoundaries(
                                const std::list< int8_t >& v,
                                int32_t levelCount,
                                double percent,
                                int8_t& lowerBoundary,
                                int8_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getPercentileBoundaries(
                                const std::list< uint8_t >& v,
                                int32_t levelCount,
                                double percent,
                                uint8_t& lowerBoundary,
                                uint8_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getPercentileBoundaries(
                                const std::list< int16_t >& v,
                                int32_t levelCount,
                                double percent,
                                int16_t& lowerBoundary,
                                int16_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getPercentileBoundaries(
                                const std::list< uint16_t >& v,
                                int32_t levelCount,
                                double percent,
                                uint16_t& lowerBoundary,
                                uint16_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getPercentileBoundaries(
                                const std::list< int32_t >& v,
                                int32_t levelCount,
                                double percent,
                                int32_t& lowerBoundary,
                                int32_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getPercentileBoundaries(
                                const std::list< uint32_t >& v,
                                int32_t levelCount,
                                double percent,
                                uint32_t& lowerBoundary,
                                uint32_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getPercentileBoundaries(
                                const std::list< int64_t >& v,
                                int32_t levelCount,
                                double percent,
                                int64_t& lowerBoundary,
                                int64_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getPercentileBoundaries(
                                const std::list< uint64_t >& v,
                                int32_t levelCount,
                                double percent,
                                uint64_t& lowerBoundary,
                                uint64_t& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getPercentileBoundaries(
                                const std::list< float >& v,
                                int32_t levelCount,
                                double percent,
                                float& lowerBoundary,
                                float& upperBoundary ) const;
template
void gkg::HistogramAnalyzer::getPercentileBoundaries(
                                const std::list< double >& v,
                                int32_t levelCount,
                                double percent,
                                double& lowerBoundary,
                                double& upperBoundary ) const;


