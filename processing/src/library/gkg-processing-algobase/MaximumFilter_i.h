#ifndef _gkg_processing_algobase_MaximumFilter_i_h_
#define _gkg_processing_algobase_MaximumFilter_i_h_


#include <gkg-processing-algobase/MaximumFilter.h>
#include <gkg-processing-algobase/VolumeMaximumFilterContext_i.h>
#include <gkg-processing-algobase/FilterFunction_i.h>
#include <gkg-processing-algobase/Converter_i.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-communication-thread/ThreadedLoop.h>
#include <gkg-core-exception/Exception.h>


//
// class MaximumFilter< IN,
//                      OUT >
//

template < class IN, class OUT >
inline
void gkg::MaximumFilter< IN,
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
             "void gkg::MaximumFilter< IN, OUT >::filter( "
             "const IN& in, OUT& out ) const" );

}


//
// class MaximumFilter< std::vector< IN >,
//                      OUT >
//

template < class IN, class OUT >
inline
void gkg::MaximumFilter< std::vector< IN >,
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
    while ( i != ie )
    {

      if ( *i > maximum )
      {

        maximum = *i;

      }
      ++ i;

    }
    gkg::Converter< IN, OUT > converter;
    converter.convert( maximum, out );

  }
  GKG_CATCH( "template < class IN, class OUT > "
             "inline "
             "void gkg::MaximumFilter< std::vector< IN >, OUT >::filter( "
             "const std::vector< IN >& in, OUT& out ) const" );

}


//
// class MaximumFilter< Vector,
//                      OUT >
//

template < class OUT >
inline
void gkg::MaximumFilter< gkg::Vector,
                         OUT >::filter( const gkg::Vector& in,
                                        OUT& out ) const
{

  try
  {

    int32_t i = 0;
    int32_t ie = in.getSize();
    double maximum = in( 0 );
    while ( i != ie )
    {

      if ( in( i ) > maximum )
      {

        maximum = in( i );

      }
      ++ i;

    }
    gkg::Converter< double, OUT > converter;
    converter.convert( maximum, out );

  }
  GKG_CATCH( "template < class OUT > "
             "inline "
             "void gkg::MaximumFilter< gkg::Vector, OUT >::filter( "
             "const gkg::Vector& in, OUT& out ) const" );

}


//
// class MaximumFilter< Volume< IN >,
//                      OUT >
//

template < class IN, class OUT >
inline
void gkg::MaximumFilter< gkg::Volume< IN >,
                         OUT >::filter( const gkg::Volume< IN >& in,
                                        OUT& out ) const
{

  try
  {

    int32_t sizeZ = in.getSizeZ();
    IN maximum = in( 0 );

    gkg::VolumeMaximumFilterContext< IN > context( in, maximum );
    gkg::ThreadedLoop< int32_t >
      threadedLoop( &context, 
                    0,
                    sizeZ,
                    in.isMemoryMapped() ? 1 : 0 );

    threadedLoop.launch( true, true );

    gkg::Converter< IN, OUT > converter;
    converter.convert( maximum, out );

  }
  GKG_CATCH( "template < class IN, class OUT > "
             "inline "
             "void gkg::MaximumFilter< gkg::Volume< IN >, OUT >::filter( "
             "const gkg::Volume< IN >& in, OUT& out ) const" );

}


//
// class MaximumFilter< Matrix,
//                      OUT >
//

template < class OUT >
inline
void gkg::MaximumFilter< gkg::Matrix,
                         OUT >::filter( const gkg::Matrix& in,
                                        OUT& out ) const
{

  try
  {

    int32_t size1 = in.getSize1();
    int32_t size2 = in.getSize2();
    int32_t i1 = 0;
    int32_t i2 = 0;

    double maximum = in( i1, i2 );
    double value = 0.0;
    for ( i1 = 0; i1 < size1; ++ i1 )
    {

      for ( i2 = 0; i2 < size2; ++ i2 )
      {

        value = in( i1, i2 );
        if ( value > maximum )
        {

          maximum = value;

        }

      }

    }

    gkg::Converter< double, OUT > converter;
    converter.convert( maximum, out );


  }
  GKG_CATCH( "template < class OUT > "
             "inline "
             "void gkg::MaximumFilter< gkg::Matrix, OUT >::filter( "
             "const gkg::Matrix& in, OUT& out ) const" );

}


//
// class MaximumFilter< SparseMatrix,
//                      OUT >
//

template < class OUT >
inline
void gkg::MaximumFilter< gkg::SparseMatrix,
                         OUT >::filter( const gkg::SparseMatrix& in,
                                        OUT& out ) const
{

  try
  {

    const std::map< size_t, double >& data = in.getData();
    std::map< size_t, double >::const_iterator
      i = data.begin(),
      ie = data.end();

    double maximum = i->second;

    while ( i != ie )
    {

      if ( i->second > maximum )
      {

        maximum = i->second;

      }
      ++ i;

    }

    gkg::Converter< double, OUT > converter;
    converter.convert( maximum, out );


  }
  GKG_CATCH( "template < class OUT > "
             "inline "
             "void gkg::MaximumFilter< gkg::SparseMatrix, OUT >::filter( "
             "const gkg::SparseMatrix& in, OUT& out ) const" );

}


#endif
