#ifndef _gkg_processing_algobase_MinimumIndexFilter_i_h_
#define _gkg_processing_algobase_MinimumIndexFilter_i_h_


#include <gkg-processing-algobase/MinimumIndexFilter.h>
#include <gkg-processing-algobase/FilterFunction_i.h>
#include <gkg-processing-algobase/Converter_i.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-core-exception/Exception.h>


//
// class MinimumIndexFilter< IN,
//                           OUT >
//

template < class IN, class OUT >
inline
void gkg::MinimumIndexFilter< IN,
                              OUT >::filter( const IN& in,
                                             OUT& out ) const
{

  try
  {

    gkg::Converter< uint32_t, OUT > converter;
    converter.convert( 0U, out );

  }
  GKG_CATCH( "template < class IN, class OUT > "
             "inline "
             "void gkg::MinimumIndexFilter< IN, "
             "OUT >::filter( const IN& in, "
             "OUT& out ) const" );

}


//
// class MinimumIndexFilter< std::vector< IN >,
//                           OUT >
//

template < class IN, class OUT >
inline
void gkg::MinimumIndexFilter< std::vector< IN >,
                              OUT >::filter( const std::vector< IN >& in,
                                             OUT& out ) const
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
    uint32_t minimumIndex = 0U;
    uint32_t index = 0U;
    while ( i != ie )
    {

      if ( *i < minimum )
      {

        minimum = *i;
        minimumIndex = index;

      }
      ++ i;
      ++ index;

    }
    gkg::Converter< uint32_t, OUT > converter;
    converter.convert( minimumIndex, out );

  }
  GKG_CATCH( "template < class IN, class OUT > "
             "inline "
             "void gkg::MinimumIndexFilter< std::vector< IN >, OUT >::filter( "
             "const std::vector< IN >& in, OUT& out ) const" );

}


//
// class MinimumIndexFilter< Vector,
//                           OUT >
//

template < class OUT >
inline
void gkg::MinimumIndexFilter< gkg::Vector,
                              OUT >::filter( const gkg::Vector& in,
                                             OUT& out ) const
{

  try
  {

    int32_t i = 0,
            ie = in.getSize();
    double minimum = in( 0 );
    uint32_t minimumIndex = 0U;
    uint32_t index = 0U;
    while ( i != ie )
    {

      if ( in( i ) < minimum )
      {

        minimum = in( i );
        minimumIndex = index;

      }
      ++ i;
      ++ index;

    }
    gkg::Converter< uint32_t, OUT > converter;
    converter.convert( minimumIndex, out );

  }
  GKG_CATCH( "template < class OUT > "
             "inline "
             "void gkg::MinimumIndexFilter< gkg::Vector, OUT >::filter( "
             "const gkg::Vector& in, OUT& out ) const" );

}


//
// class MinimumIndexFilter< Volume< IN >,
//                           OUT >
//

template < class IN, class OUT >
inline
void gkg::MinimumIndexFilter< gkg::Volume< IN >,
                              OUT >::filter( const gkg::Volume< IN >& in,
                                             OUT& out ) const
{

  try
  {

    typename gkg::Volume< IN >::const_iterator i = in.begin(),
                                               ie = in.end();
    IN minimum = *i;
    uint32_t minimumIndex = 0U;
    uint32_t index = 0U;
    while ( i != ie )
    {

      if ( *i < minimum )
      {

        minimum = *i;
        minimumIndex = index;

      }
      ++ i;
      ++ index;

    }
    gkg::Converter< uint32_t, OUT > converter;
    converter.convert( minimumIndex, out );

  }
  GKG_CATCH( "template < class IN, class OUT > "
             "inline "
             "void gkg::MinimumIndexFilter< std::vector< IN >, OUT >::filter( "
             "const std::vector< IN >& in, OUT& out ) const" );

}


#endif
