#ifndef _gkg_processing_algobase_MaximumIndexFilter_i_h_
#define _gkg_processing_algobase_MaximumIndexFilter_i_h_


#include <gkg-processing-algobase/MaximumIndexFilter.h>
#include <gkg-processing-algobase/FilterFunction_i.h>
#include <gkg-processing-algobase/Converter_i.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-core-exception/Exception.h>


//
// class MaximumIndexFilter< IN,
//                           OUT >
//

template < class IN, class OUT >
inline
void gkg::MaximumIndexFilter< IN,
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
             "void gkg::MaximumIndexFilter< IN, "
             "OUT >::filter( const IN& in, "
             "OUT& out ) const" );

}


//
// class MaximumIndexFilter< std::vector< IN >,
//                           OUT >
//

template < class IN, class OUT >
inline
void gkg::MaximumIndexFilter< std::vector< IN >,
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
    IN maximum = *i;
    uint32_t maximumIndex = 0U;
    uint32_t index = 0U;
    while ( i != ie )
    {

      if ( *i > maximum )
      {

        maximum = *i;
        maximumIndex = index;

      }
      ++ i;
      ++ index;

    }
    gkg::Converter< uint32_t, OUT > converter;
    converter.convert( maximumIndex, out );

  }
  GKG_CATCH( "template < class IN, class OUT > "
             "inline "
             "void gkg::MaximumIndexFilter< std::vector< IN >, OUT >::filter( "
             "const std::vector< IN >& in, OUT& out ) const" );

}


//
// class MaximumIndexFilter< Vector,
//                           OUT >
//

template < class OUT >
inline
void gkg::MaximumIndexFilter< gkg::Vector,
                              OUT >::filter( const gkg::Vector& in,
                                             OUT& out ) const
{

  try
  {

    int32_t i = 0,
            ie = in.getSize();
    double maximum = in( 0 );
    uint32_t maximumIndex = 0U;
    uint32_t index = 0U;
    while ( i != ie )
    {

      if ( in( i ) > maximum )
      {

        maximum = in( i );
        maximumIndex = index;

      }
      ++ i;
      ++ index;

    }
    gkg::Converter< uint32_t, OUT > converter;
    converter.convert( maximumIndex, out );

  }
  GKG_CATCH( "template < class OUT > "
             "inline "
             "void gkg::MaximumIndexFilter< gkg::Vector, OUT >::filter( "
             "const gkg::Vector& in, OUT& out ) const" );

}


//
// class MaximumIndexFilter< Volume< IN >,
//                           OUT >
//

template < class IN, class OUT >
inline
void gkg::MaximumIndexFilter< gkg::Volume< IN >,
                              OUT >::filter( const gkg::Volume< IN >& in,
                                             OUT& out ) const
{

  try
  {

    typename gkg::Volume< IN >::const_iterator i = in.begin(),
                                               ie = in.end();
    IN maximum = *i;
    uint32_t maximumIndex = 0U;
    uint32_t index = 0U;
    while ( i != ie )
    {

      if ( *i > maximum )
      {

        maximum = *i;
        maximumIndex = index;

      }
      ++ i;
      ++ index;

    }
    gkg::Converter< uint32_t, OUT > converter;
    converter.convert( maximumIndex, out );

  }
  GKG_CATCH( "template < class IN, class OUT > "
             "inline "
             "void gkg::MaximumIndexFilter< std::vector< IN >, OUT >::filter( "
             "const std::vector< IN >& in, OUT& out ) const" );

}


#endif
