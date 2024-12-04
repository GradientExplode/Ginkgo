#ifndef _gkg_processing_algobase_MinMaxFilter_i_h_
#define _gkg_processing_algobase_MinMaxFilter_i_h_


#include <gkg-processing-algobase/MinMaxFilter.h>
#include <gkg-processing-algobase/VolumeMinMaxFilterContext_i.h>
#include <gkg-processing-algobase/FilterFunction_i.h>
#include <gkg-processing-algobase/Converter_i.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-communication-thread/ThreadedLoop.h>
#include <gkg-core-exception/Exception.h>


//
// class MinMaxFilter< IN,
//                     OUT >
//

template < class IN, class OUT >
inline
void gkg::MinMaxFilter< IN,
                        OUT >::filter( const IN& in,
                                       std::pair< OUT,
                                                  OUT >& out ) const
{

  try
  {

    gkg::Converter< IN, OUT > converter;
    converter.convert( in, out.first );
    converter.convert( in, out.second );

  }
  GKG_CATCH( "template < class IN, class OUT > "
             "inline "
             "void gkg::MinMaxFilter< IN, OUT >::filter( "
             "const IN& in, "
             "std::pair< OUT, OUT >& out ) const" );

}


//
// class MinMaxFilter< std::vector< IN >,
//                     OUT >
//

template < class IN, class OUT >
inline
void gkg::MinMaxFilter< std::vector< IN >,
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
    while ( i != ie )
    {

      if ( *i < minimum )
      {

        minimum = *i;

      }
      else if ( *i > maximum )
      {

        maximum = *i;

      }
      ++ i;

    }
    gkg::Converter< IN, OUT > converter;
    converter.convert( minimum, out.first );
    converter.convert( maximum, out.second );

  }
  GKG_CATCH( "template < class IN, class OUT > "
             "inline "
             "void gkg::MinMaxFilter< std::vector< IN >, OUT >::filter( "
             "const std::vector< IN >& in, "
             "std::pair< OUT, OUT >& out ) const" );

}


//
// class MinMaxFilter< Vector,
//                     OUT >
//

template < class OUT >
inline
void gkg::MinMaxFilter< gkg::Vector,
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
    while ( i != ie )
    {

      if ( in( i ) < minimum )
      {

        minimum = in( i );

      }
      else if ( in( i ) > maximum )
      {

        maximum = in( i );

      }
      ++ i;

    }
    gkg::Converter< double, OUT > converter;
    converter.convert( minimum, out.first );
    converter.convert( maximum, out.second );

  }
  GKG_CATCH( "template < class OUT > "
             "inline "
             "void gkg::MinMaxFilter< gkg::Vector, OUT >::filter( "
             "const gkg::Vector& in, "
             "std::pair< OUT, OUT >& out ) const" );

}


//
// class MinMaxFilter< Volume< IN >,
//                     OUT >
//

template < class IN, class OUT >
inline
void gkg::MinMaxFilter< gkg::Volume< IN >,
                        OUT >::filter( const gkg::Volume< IN >& in,
                                       std::pair< OUT,
                                                  OUT >& out ) const
{

  try
  {

    IN minimum = in( 0 );
    IN maximum = minimum;

    if ( in.isMemoryMapped() )
    {

      int32_t sizeX = in.getSizeX();
      int32_t sizeY = in.getSizeY();
      int32_t sizeZ = in.getSizeZ();
      int32_t sizeT = in.getSizeT();
      int32_t x, y, z, t;

      for ( t = 0; t < sizeT; t++ )
      {

        for ( z = 0; z < sizeZ; z++ )
        {

          for ( y = 0; y < sizeY; y++ )
          {

            for ( x = 0; x < sizeX; x++ )
            {

              IN value = in( x, y, z, t );

              if ( value < minimum )
              {

                minimum = value;

              }
              else if ( value > maximum )
              {

                maximum = value;

              }

            }

          }

        }

      }

    }
    else
    {

      gkg::VolumeMinMaxFilterContext< IN >
        context( in, minimum, maximum );
      gkg::ThreadedLoop< int32_t >
        threadedLoop( &context, 0, in.getSizeXYZT() );

      threadedLoop.launch();

    }

    gkg::Converter< IN, OUT > converter;
    converter.convert( minimum, out.first );
    converter.convert( maximum, out.second );

  }
  GKG_CATCH( "template < class IN, class OUT > "
             "inline "
             "void gkg::MinMaxFilter< gkg::Volume< IN >, OUT >::filter( "
             "const gkg::Volume< IN >& in, "
             "std::pair< OUT, OUT >& out ) const" );

}


//
// class MinMaxFilter< Matrix,
//                     OUT >
//

template < class OUT >
inline
void gkg::MinMaxFilter< gkg::Matrix,
                        OUT >::filter( const gkg::Matrix& in,
                                       std::pair< OUT,
                                                  OUT >& out ) const
{

  try
  {

    int32_t size1 = in.getSize1();
    int32_t size2 = in.getSize2();
    int32_t i1 = 0;
    int32_t i2 = 0;

    double minimum = in( i1, i2 );
    double maximum = minimum;
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
        else if ( value > maximum )
        {

          maximum = value;

        }

      }

    }

    gkg::Converter< double, OUT > converter;
    converter.convert( minimum, out.first );
    converter.convert( maximum, out.second );


  }
  GKG_CATCH( "template < class OUT > "
             "inline "
             "void gkg::MinMaxFilter< gkg::Matrix, OUT >::filter( "
             "const gkg::Matrix& in, "
             "std::pair< OUT, OUT >& out ) const" );

}


//
// class MinMaxFilter< SparseMatrix,
//                     OUT >
//

template < class OUT >
inline
void gkg::MinMaxFilter< gkg::SparseMatrix,
                        OUT >::filter( const gkg::SparseMatrix& in,
                                       std::pair< OUT,
                                                  OUT >& out ) const
{

  try
  {

    const std::map< size_t, double >& data = in.getData();
    std::map< size_t, double >::const_iterator
      i = data.begin(),
      ie = data.end();

    double minimum = i->second;
    double maximum = i->second;

    while ( i != ie )
    {

      if ( i->second < minimum )
      {

        minimum = i->second;

      }
      else if ( i->second > maximum )
      {

        maximum = i->second;

      }
      ++ i;

    }

    gkg::Converter< double, OUT > converter;
    converter.convert( minimum, out.first );
    converter.convert( maximum, out.second );

  }
  GKG_CATCH( "template < class OUT > "
             "inline "
             "void gkg::MinMaxFilter< gkg::SparseMatrix, OUT >::filter( "
             "const gkg::SparseMatrix& in, "
             "std::pair< OUT, OUT >& out ) const" );

}


#endif
