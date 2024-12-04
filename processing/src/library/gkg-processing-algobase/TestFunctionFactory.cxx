#include <gkg-processing-algobase/TestFunctionFactory.h>
#include <gkg-processing-algobase/LowerThanFunction_i.h>
#include <gkg-processing-algobase/LowerOrEqualToFunction_i.h>
#include <gkg-processing-algobase/EqualToFunction_i.h>
#include <gkg-processing-algobase/DifferentFromFunction_i.h>
#include <gkg-processing-algobase/GreaterOrEqualToFunction_i.h>
#include <gkg-processing-algobase/GreaterThanFunction_i.h>
#include <gkg-processing-algobase/BetweenOrEqualToFunction_i.h>
#include <gkg-processing-algobase/BetweenFunction_i.h>
#include <gkg-processing-algobase/OutsideOrEqualToFunction_i.h>
#include <gkg-processing-algobase/OutsideFunction_i.h>
#include <gkg-core-cppext/StdInt.h>
#include <gkg-core-exception/Exception.h>


template < class T >
gkg::TestFunctionFactory< T >::TestFunctionFactory()
{
}


template < class T >
gkg::TestFunctionFactory< T >::~TestFunctionFactory()
{
}


template < class T >
gkg::TestFunction< T >* 
gkg::TestFunctionFactory< T >::createTestFunction( const std::string& name,
                                                   T threshold1,
					           T threshold2 ) const
{

  try
  {
  
    if ( name == "lt" )
    {

      return new gkg::LowerThanFunction< T >( threshold1 );

    }
    else if ( name == "le" )
    {

      return new gkg::LowerOrEqualToFunction< T >( threshold1 );

    }
    else if ( name == "eq" )
    {

      return new gkg::EqualToFunction< T >( threshold1 );

    }
    else if ( name == "di" )
    {

      return new gkg::DifferentFromFunction< T >( threshold1 );

    }
    else if ( name == "ge" )
    {

      return new gkg::GreaterOrEqualToFunction< T >( threshold1 );

    }
    else if ( name == "gt" )
    {

      return new gkg::GreaterThanFunction< T >( threshold1 );

    }
    else if ( name == "be" )
    {

      return new gkg::BetweenOrEqualToFunction< T >( threshold1, threshold2 );

    }
    else if ( name == "bt" )
    {

      return new gkg::BetweenFunction< T >( threshold1, threshold2 );

    }
    else if ( name == "oe" )
    {

      return new gkg::OutsideOrEqualToFunction< T >( threshold1, threshold2 );

    }
    else if ( name == "ou" )
    {

      return new gkg::OutsideFunction< T >( threshold1, threshold2 );

    }
    else
    {

      throw std::runtime_error( std::string( "'"  ) + name + "' not found" );

    }
    
  }
  GKG_CATCH( "template < class T > "
             "gkg::TestFunction< T >* "
             "gkg::TestFunctionFactory< T >::createTestFunction( "
             "const std::string& name, "
             "T threshold1, "
             "T threshold2 ) const" );

}


template class gkg::TestFunctionFactory< int8_t >;
template class gkg::TestFunctionFactory< uint8_t >;
template class gkg::TestFunctionFactory< int16_t >;
template class gkg::TestFunctionFactory< uint16_t >;
template class gkg::TestFunctionFactory< int32_t >;
template class gkg::TestFunctionFactory< uint32_t >;
template class gkg::TestFunctionFactory< int64_t >;
template class gkg::TestFunctionFactory< uint64_t >;
template class gkg::TestFunctionFactory< float >;
template class gkg::TestFunctionFactory< double >;
