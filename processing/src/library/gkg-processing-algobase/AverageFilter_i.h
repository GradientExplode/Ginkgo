#ifndef _gkg_processing_algobase_AverageFilter_i_h_
#define _gkg_processing_algobase_AverageFilter_i_h_


#include <gkg-processing-algobase/AverageFilter.h>
#include <gkg-processing-algobase/FilterFunction_i.h>
#include <gkg-processing-algobase/Converter_i.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-communication-thread/ThreadedLoop.h>
#include <gkg-core-exception/Exception.h>


//
// class AverageFilter< IN,
//                      OUT >
//

template < class IN, class OUT >
inline
void gkg::AverageFilter< IN,
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
             "void gkg::AverageFilter< IN, OUT >::filter( "
             "const IN& in, OUT& out ) const" );

}


//
// class AverageFilter< std::vector< IN >,
//                      OUT >
//

template < class IN, class OUT >
inline
void gkg::AverageFilter< std::vector< IN >,
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
    double sum = 0.0;
    while ( i != ie )
    {

      sum += ( double )*i;
      ++ i;

    }
    sum /= ( double )in.size();

    gkg::Converter< double, OUT > converter;
    converter.convert( sum, out );

  }
  GKG_CATCH( "template < class IN, class OUT > "
             "inline "
             "void gkg::AverageFilter< std::vector< IN >, OUT >::filter( "
             "const std::vector< IN >& in, OUT& out ) const" );

}


//
// class AverageFilter< Vector,
//                      OUT >
//

template < class OUT >
inline
void gkg::AverageFilter< gkg::Vector,
                         OUT >::filter( const gkg::Vector& in,
                                        OUT& out ) const
{

  try
  {

    int32_t i = 0,
            ie = in.getSize();
    double sum = 0.0;
    while ( i != ie )
    {

      sum += ( double )in( i );
      ++ i;

    }
    sum /= ( double )in.getSize();

    gkg::Converter< double, OUT > converter;
    converter.convert( sum, out );

  }
  GKG_CATCH( "template < class OUT > "
             "inline "
             "void gkg::AverageFilter< gkg::Vector, OUT >::filter( "
             "const gkg::Vector& in, OUT& out ) const" );

}


//
// class AverageFilter< Volume< IN >,
//                      OUT >
//

template < class IN, class OUT >
inline
void gkg::AverageFilter< gkg::Volume< IN >,
                         OUT >::filter( const gkg::Volume< IN >& in,
                                        OUT& out ) const
{

  try
  {

    typename gkg::Volume< IN >::const_iterator i = in.begin(),
                                               ie = in.end();
    double sum = 0.0;
    while ( i != ie )
    {

      sum += ( double )*i;
      ++ i;

    }
    sum /= ( double )in.getSizeXYZT();

    gkg::Converter< double, OUT > converter;
    converter.convert( sum, out );

  }
  GKG_CATCH( "template < class IN, class OUT > "
             "inline "
             "void gkg::AverageFilter< gkg::Volume< IN >, OUT >::filter( "
             "const gkg::Volume< IN >& in, OUT& out ) const" );

}


//
// class AverageFilter< Matrix,
//                      OUT >
//

template < class OUT >
inline
void gkg::AverageFilter< gkg::Matrix,
                         OUT >::filter( const gkg::Matrix& in,
                                        OUT& out ) const
{

  try
  {

    int32_t size1 = in.getSize1();
    int32_t size2 = in.getSize2();
    int32_t i1 = 0;
    int32_t i2 = 0;

    double sum = 0.0;
    for ( i1 = 0; i1 < size1; ++ i1 )
    {

      for ( i2 = 0; i2 < size2; ++ i2 )
      {

        sum += ( double )in( i1, i2 );

      }

    }
    sum /= ( ( double )size1 * ( double )size2 );

    gkg::Converter< double, OUT > converter;
    converter.convert( sum, out );

  }
  GKG_CATCH( "template < class OUT > "
             "inline "
             "void gkg::AverageFilter< gkg::Matrix, OUT >::filter( "
             "const gkg::Matrix& in, OUT& out ) const" );

}


//
// class AverageFilter< SparseMatrix,
//                      OUT >
//

template < class OUT >
inline
void gkg::AverageFilter< gkg::SparseMatrix,
                         OUT >::filter( const gkg::SparseMatrix& in,
                                        OUT& out ) const
{

  try
  {

    const std::map< size_t, double >& data = in.getData();
    std::map< size_t, double >::const_iterator
      i = data.begin(),
      ie = data.end();

    double sum = 0.0;
    int32_t count = 0;

    while ( i != ie )
    {

      sum += i->second;
      ++ i;
      ++ count;

    }
    sum /= ( double )count;

    gkg::Converter< double, OUT > converter;
    converter.convert( sum, out );

  }
  GKG_CATCH( "template < class OUT > "
             "inline "
             "void gkg::AverageFilter< gkg::SparseMatrix, OUT >::filter( "
             "const gkg::SparseMatrix& in, OUT& out ) const" );

}


#endif
