#ifndef _gkg_processing_algobase_MinMaxIndexFilter_i_h_
#define _gkg_processing_algobase_MinMaxIndexFilter_i_h_


#include <gkg-processing-algobase/MinMaxIndexFilter.h>
#include <gkg-processing-algobase/FilterFunction_i.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-algobase/Converter_i.h>
#include <gkg-core-exception/Exception.h>


//
// class MinMaxIndexFilter< IN,
//                          OUT >
//

template < class IN, class OUT >
inline
void gkg::MinMaxIndexFilter< IN,
                             OUT >::filter( const IN& /* in */, 
                                            std::pair< OUT,
                                                       OUT >& out ) const
{

  try
  {

    OUT value;
    gkg::Converter< uint32_t, OUT > converter;

    converter.convert( 0U, value );

    out.first = value;
    out.second = value;

  }
  GKG_CATCH( "template < class IN, class OUT > "
             "inline "
             "void gkg::MinMaxIndexFilter< IN, "
             "OUT >::filter( const IN& /* in */, "
             "std::pair< OUT, "
             "OUT >& out ) const" );

}


//
// class MinMaxIndexFilter< std::vector< IN >,
//                          OUT >
//

template < class IN, class OUT >
inline
void gkg::MinMaxIndexFilter< std::vector< IN >,
                             OUT >::filter( const std::vector< IN >& in, 
                                            std::pair< OUT,
                                                       OUT >& out ) const
{

  try
  {

    if ( in.empty() )
    {

      throw std::runtime_error( "empty vector" );

    }

    typename std::vector< IN >::const_iterator i = in.begin(),
                                               ie = in.end();
    IN minimum = *i;
    IN maximum = *i;
    uint32_t minimumIndex = 0U;
    uint32_t maximumIndex = 0U;
    uint32_t index = 0U;
    while ( i != ie )
    {

      if ( *i < minimum )
      {

        minimum = *i;
        minimumIndex = index;

      }
      else if ( *i > maximum )
      {

        maximum = *i;
        maximumIndex = index;

      }
      ++ i;
      ++ index;

    }
    gkg::Converter< uint32_t, OUT > converter;
    converter.convert( minimumIndex, out.first );
    converter.convert( maximumIndex, out.second );

  }
  GKG_CATCH( "template < class IN, class OUT > "
             "inline "
             "void gkg::MinMaxIndexFilter< std::vector< IN >, OUT >::filter( "
             "const std::vector< IN >& in, "
             "std::pair< OUT, OUT >& out ) const" );

}


//
// class MinMaxIndexFilter< Vector,
//                          OUT >
//

template < class OUT >
inline
void gkg::MinMaxIndexFilter< gkg::Vector,
                             OUT >::filter( const gkg::Vector& in, 
                                            std::pair< OUT,
                                                       OUT >& out ) const
{

  try
  {

    int32_t i = 0,
            ie = in.getSize();
    double minimum = in( 0 );
    double maximum = in( 0 );
    uint32_t minimumIndex = 0U;
    uint32_t maximumIndex = 0U;
    uint32_t index = 0U;
    while ( i != ie )
    {

      if ( in( i ) < minimum )
      {

        minimum = in( i );
        minimumIndex = index;

      }
      else if ( in( i ) > maximum )
      {

        maximum = in( i );
        maximumIndex = index;

      }
      ++ i;
      ++ index;

    }
    gkg::Converter< uint32_t, OUT > converter;
    converter.convert( minimumIndex, out.first );
    converter.convert( maximumIndex, out.second );

  }
  GKG_CATCH( "template < class OUT > "
             "inline "
             "void gkg::MinMaxIndexFilter< gkg::Vector, OUT >::filter( "
             "const gkg::Vector& in, "
             "std::pair< OUT, OUT >& out ) const" );

}


//
// class MinMaxIndexFilter< Volume< IN >,
//                          OUT >
//

template < class IN, class OUT >
inline
void gkg::MinMaxIndexFilter< gkg::Volume< IN >,
                             OUT >::filter( const gkg::Volume< IN >& in, 
                                            std::pair< OUT,
                                                       OUT >& out ) const
{

  try
  {

    typename gkg::Volume< IN >::const_iterator i = in.begin(),
                                               ie = in.end();
    IN minimum = *i;
    IN maximum = *i;
    uint32_t minimumIndex = 0U;
    uint32_t maximumIndex = 0U;
    uint32_t index = 0U;
    while ( i != ie )
    {

      if ( *i < minimum )
      {

        minimum = *i;
        minimumIndex = index;

      }
      else if ( *i > maximum )
      {

        maximum = *i;
        maximumIndex = index;

      }
      ++ i;
      ++ index;

    }
    gkg::Converter< uint32_t, OUT > converter;
    converter.convert( minimumIndex, out.first );
    converter.convert( maximumIndex, out.second );

  }
  GKG_CATCH( "template < class IN, class OUT > "
             "inline "
             "void gkg::MinMaxIndexFilter< std::vector< IN >, OUT >::filter( "
             "const std::vector< IN >& in, "
             "std::pair< OUT, OUT >& out ) const" );

}


#endif
