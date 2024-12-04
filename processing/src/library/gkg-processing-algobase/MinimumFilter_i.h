#ifndef _gkg_processing_algobase_MinimumFilter_i_h_
#define _gkg_processing_algobase_MinimumFilter_i_h_


#include <gkg-processing-algobase/MinimumFilter.h>
#include <gkg-processing-algobase/VolumeMinimumFilterContext_i.h>
#include <gkg-processing-algobase/FilterFunction_i.h>
#include <gkg-processing-algobase/Converter_i.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-communication-thread/ThreadedLoop.h>
#include <gkg-core-exception/Exception.h>


//
// class MinimumFilter< IN,
//                      OUT >
//

template < class IN, class OUT >
inline
void gkg::MinimumFilter< IN,
                         OUT >::filter( const IN& in,
                                        OUT& out ) const
{

  try
  {

    gkg::Converter< IN, OUT > converter;
    converter.convert( in, out );

  }
  GKG_CATCH( "template < class IN, class OUT > "
             "inline "
             "void gkg::MinimumFilter< IN, OUT >::filter( "
             "const IN& in, OUT& out ) const" );

}


//
// class MinimumFilter< std::vector< IN >,
//                      OUT >
//

template < class IN, class OUT >
inline
void gkg::MinimumFilter< std::vector< IN >,
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
    while ( i != ie )
    {

      if ( *i < minimum )
      {

        minimum = *i;

      }
      ++ i;

    }
    gkg::Converter< IN, OUT > converter;
    converter.convert( minimum, out );

  }
  GKG_CATCH( "template < class IN, class OUT > "
             "inline "
             "void gkg::MinimumFilter< std::vector< IN >, OUT >::filter( "
             "const std::vector< IN >& in, OUT& out ) const" );

}


//
// class MinimumFilter< Vector,
//                      OUT >
//

template < class OUT >
inline
void gkg::MinimumFilter< gkg::Vector,
                         OUT >::filter( const gkg::Vector& in,
                                        OUT& out ) const
{

  try
  {

    int32_t i = 0;
    int32_t ie = in.getSize();
    double minimum = in( 0 );
    while ( i != ie )
    {

      if ( in( i ) < minimum )
      {

        minimum = in( i );

      }
      ++ i;

    }
    gkg::Converter< double, OUT > converter;
    converter.convert( minimum, out );

  }
  GKG_CATCH( "template < class OUT > "
             "inline "
             "void gkg::MinimumFilter< gkg::Vector, OUT >::filter( "
             "const gkg::Vector& in, OUT& out ) const" );

}


//
// class MinimumFilter< Volume< IN >,
//                      OUT >
//

template < class IN, class OUT >
inline
void gkg::MinimumFilter< gkg::Volume< IN >,
                         OUT >::filter( const gkg::Volume< IN >& in,
                                        OUT& out ) const
{

  try
  {

    int32_t sizeZ = in.getSizeZ();
    IN minimum = in( 0 );

    gkg::VolumeMinimumFilterContext< IN > context( in, minimum );
    gkg::ThreadedLoop< int32_t >
      threadedLoop( &context, 
                    0,
                    sizeZ,
                    in.isMemoryMapped() ? 1 : 0 );

    threadedLoop.launch( true, true );

    gkg::Converter< IN, OUT > converter;
    converter.convert( minimum, out );

  }
  GKG_CATCH( "template < class IN, class OUT > "
             "inline "
             "void gkg::MinimumFilter< gkg::Volume< IN >, OUT >::filter( "
             "const gkg::Volume< IN >& in, OUT& out ) const" );

}


//
// class MinimumFilter< Matrix,
//                      OUT >
//

template < class OUT >
inline
void gkg::MinimumFilter< gkg::Matrix,
                         OUT >::filter( const gkg::Matrix& in,
                                        OUT& out ) const
{

  try
  {

    int32_t size1 = in.getSize1();
    int32_t size2 = in.getSize2();
    int32_t i1 = 0;
    int32_t i2 = 0;

    double minimum = in( i1, i2 );
    double value = 0.0;
    for ( i1 = 0; i1 < size1; ++ i1 )
    {

      for ( i2 = 0; i2 < size2; ++ i2 )
      {

        value = in( i1, i2 );
        if ( value < minimum )
        {

          minimum = value;

        }

      }

    }

    gkg::Converter< double, OUT > converter;
    converter.convert( minimum, out );


  }
  GKG_CATCH( "template < class OUT > "
             "inline "
             "void gkg::MinimumFilter< gkg::Matrix, OUT >::filter( "
             "const gkg::Matrix& in, OUT& out ) const" );

}


//
// class MinimumFilter< SparseMatrix,
//                      OUT >
//

template < class OUT >
inline
void gkg::MinimumFilter< gkg::SparseMatrix,
                         OUT >::filter( const gkg::SparseMatrix& in,
                                        OUT& out ) const
{

  try
  {

    const std::map< size_t, double >& data = in.getData();
    std::map< size_t, double >::const_iterator
      i = data.begin(),
      ie = data.end();

    double minimum = i->second;

    while ( i != ie )
    {

      if ( i->second < minimum )
      {

        minimum = i->second;

      }
      ++ i;

    }

    gkg::Converter< double, OUT > converter;
    converter.convert( minimum, out );

  }
  GKG_CATCH( "template < class OUT > "
             "inline "
             "void gkg::MinimumFilter< gkg::SparseMatrix, OUT >::filter( "
             "const gkg::SparseMatrix& in, OUT& out ) const" );

}


#endif
