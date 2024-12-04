#include <gkg-processing-algobase/RandomIndexVector.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-numericalanalysis/RandomGenerator.h>
#include <gkg-core-exception/Exception.h>
#include <set>


template < class T >
void gkg::getRandomIndexVector( std::vector< T >& data )
{

  try
  {

    // getting numerical analysis implementation factory
    static gkg::NumericalAnalysisImplementationFactory*
      factory = gkg::NumericalAnalysisSelector::getInstance().
                                                     getImplementationFactory();
    static gkg::RandomGenerator randomGenerator( gkg::RandomGenerator::Taus );

    // building data
    size_t size = data.size();

    std::set< T > stack;
    T index = 0;

    while ( stack.size() < size )
    {

      index = ( T )factory->getUniformRandomNumber( randomGenerator,
                                                  0.0,
                                                  ( float )2 * ( float )size ) %
            ( T )size;
      if ( stack.find( index ) == stack.end() )
      {

        data[ stack.size() ] = index;
        stack.insert( index ); 

      }

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::getRandomIndexVector( std::vector< T >& data )" );

}


template < class T >
void gkg::getRandomIndexVector( std::vector< T >& data, const T& maximumIndex )
{

  try
  {

    // getting numerical analysis implementation factory
    static gkg::NumericalAnalysisImplementationFactory*
      factory = gkg::NumericalAnalysisSelector::getInstance().
                                                     getImplementationFactory();
    static gkg::RandomGenerator randomGenerator( gkg::RandomGenerator::Taus );


    // sanity checks
    size_t size = data.size();
    if ( maximumIndex < ( T )size )
    {

       throw std::runtime_error(
                   "maximum index must be at least equal to the size of data" );

    }

    // building data
    std::set< T > stack;
    T index = 0;

    while ( stack.size() < size )
    {

      index = ( T )factory->getUniformRandomNumber( randomGenerator,
                                                    0.0,
                                                    ( float )2 *
                                                    ( float )maximumIndex ) %
              ( T )maximumIndex;
      if ( stack.find( index ) == stack.end() )
      {

        data[ stack.size() ] = index;
        stack.insert( index ); 

      }

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::getRandomIndexVector( "
             "std::vector< T >& data, const T& maximumIndex )" );

}


void gkg::getRandomSelection( std::vector< bool >& selection,
                              int32_t selectedCount )
{

  try
  {

    // getting numerical analysis implementation factory
    static gkg::NumericalAnalysisImplementationFactory*
      factory = gkg::NumericalAnalysisSelector::getInstance().
                                                     getImplementationFactory();
    static gkg::RandomGenerator randomGenerator( gkg::RandomGenerator::Taus );

    // sanity checks    
    int32_t size = ( int32_t )selection.size();
    if ( selectedCount > size )
    {

       throw std::runtime_error(
               "selected count cannot be greater than selection size" );

    }
    if ( selectedCount < 0 )
    {

       throw std::runtime_error( "selected count must be positive" );

    }

    // initializing selection to false
    std::vector< bool >::iterator
      s = selection.begin(),
      se = selection.end();
    while ( s != se )
    {

      *s = false;
      ++ s;

    }

    // buidling selection
    std::set< int32_t > stack;
    int32_t index = 0;

    while ( ( int32_t )stack.size() < selectedCount )
    {

      index = ( int32_t )factory->getUniformRandomNumber(
                                         randomGenerator,
                                         0.0,
                                         ( float )2 * ( float )size ) %
                                         size;
      if ( stack.find( index ) == stack.end() )
      {

        selection[ index ] = true;
        stack.insert( index ); 

      }

    }

  }
  GKG_CATCH( "void gkg::getRandomIndexVector( "
             "std::vector< bool >& selection, int32_t selectedCount )" );

}


namespace gkg
{


template void getRandomIndexVector< int8_t >(
                                     std::vector< int8_t >& data );
template void getRandomIndexVector< uint8_t >(
                                     std::vector< uint8_t >& data );

template void getRandomIndexVector< int16_t >(
                                     std::vector< int16_t >& data );
template void getRandomIndexVector< uint16_t >(
                                     std::vector< uint16_t >& data );

template void getRandomIndexVector< int32_t >(
                                     std::vector< int32_t >& data );
template void getRandomIndexVector< uint32_t >(
                                     std::vector< uint32_t >& data );

template void getRandomIndexVector< int64_t >(
                                     std::vector< int64_t >& data );
template void getRandomIndexVector< uint64_t >(
                                     std::vector< uint64_t >& data );


template void getRandomIndexVector< int8_t >(
                                     std::vector< int8_t >& data,
                                     const int8_t& maximumIndex );
template void getRandomIndexVector< uint8_t >(
                                     std::vector< uint8_t >& data,
                                     const uint8_t& maximumIndex );

template void getRandomIndexVector< int16_t >(
                                     std::vector< int16_t >& data,
                                     const int16_t& maximumIndex );
template void getRandomIndexVector< uint16_t >(
                                     std::vector< uint16_t >& data,
                                     const uint16_t& maximumIndex );

template void getRandomIndexVector< int32_t >(
                                     std::vector< int32_t >& data,
                                     const int32_t& maximumIndex );
template void getRandomIndexVector< uint32_t >(
                                     std::vector< uint32_t >& data,
                                     const uint32_t& maximumIndex );

template void getRandomIndexVector< int64_t >(
                                     std::vector< int64_t >& data,
                                     const int64_t& maximumIndex );
template void getRandomIndexVector< uint64_t >(
                                     std::vector< uint64_t >& data,
                                     const uint64_t& maximumIndex );


}
