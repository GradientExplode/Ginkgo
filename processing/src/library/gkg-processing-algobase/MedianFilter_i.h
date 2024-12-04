#ifndef _gkg_processing_algobase_MedianFilter_i_h_
#define _gkg_processing_algobase_MedianFilter_i_h_


#include <gkg-processing-algobase/MedianFilter.h>
#include <gkg-processing-algobase/FilterFunction_i.h>
#include <gkg-processing-algobase/Converter_i.h>
#include <gkg-processing-algobase/MagnitudeConverter_i.h>
#include <gkg-processing-algobase/PhaseConverter_i.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-communication-thread/ThreadedLoop.h>
#include <gkg-core-exception/Exception.h>


//
// class MedianFilter< IN,
//                     OUT >
//

template < class IN, class OUT >
inline
void gkg::MedianFilter< IN,
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
             "void gkg::MedianFilter< IN, OUT >::filter( "
             "const IN& in, OUT& out ) const" );

}


//
// class MedianFilter< std::vector< IN >,
//                     OUT >
//

template < class IN, class OUT >
inline
void gkg::MedianFilter< std::vector< IN >,
                        OUT >::filter( const std::vector< IN >& in,
                                       OUT& out ) const
{

  try
  {

    if ( in.empty() )
    {

      throw std::runtime_error( "empty vector" );

    }

    std::vector< IN > copy( in );
    std::sort( copy.begin(), copy.end() );
    gkg::Converter< IN, OUT > converter;
    converter.convert( copy[ copy.size() / 2 - ( copy.size() % 2 ?  0U : 1U ) ],
                       out );

  }
  GKG_CATCH( "template < class IN, class OUT > "
             "inline "
             "void gkg::MedianFilter< std::vector< IN >, OUT >::filter( "
             "const std::vector< IN >& in, OUT& out ) const" );

}


//
// class MedianFilter< std::vector< std::complex< IN > >,
//                     std::complex< OUT > >
//

template < class IN, class OUT >
inline
void gkg::MedianFilter< std::vector< std::complex< IN > >,
                        std::complex< OUT > >::filter(
                                    const std::vector< std::complex< IN > >& in,
                                    std::complex< OUT >& out ) const
{

  try
  {

    if ( in.empty() )
    {

      throw std::runtime_error( "empty vector" );

    }

    // taking magnitude and phase
    std::vector< double > magnitude;
    std::vector< double > phase;

    gkg::MagnitudeConverter< std::vector< std::complex< IN > >,
                             std::vector< double > > magnitudeConverter;
    gkg::PhaseConverter< std::vector< std::complex< IN > >,
                             std::vector< double > > phaseConverter;

    magnitudeConverter.convert( in, magnitude );
    phaseConverter.convert( in, phase );

    double medianMagnitude;
    double medianPhase;

    gkg::MedianFilter< std::vector< double >, double > medianFilter;
    medianFilter.filter( magnitude, medianMagnitude );
    medianFilter.filter( phase, medianPhase );

    std::complex< double > doubleOut = std::polar( medianMagnitude,
                                                   medianPhase );

    out = std::complex< OUT >( ( OUT )std::real( doubleOut ),
                               ( OUT )std::imag( doubleOut ) );

  }
  GKG_CATCH( "template < class IN, class OUT > "
             "inline "
             "void gkg::MedianFilter< std::vector< std::complex< IN > >, "
             " std::complex< OUT > >::filter( "
             "const std::vector< std::complex< IN > >& in, "
             " std::complex< OUT >& out ) const" );

}


//
// class MedianFilter< Vector,
//                     OUT >
//

template < class OUT >
inline
void gkg::MedianFilter< gkg::Vector,
                        OUT >::filter( const gkg::Vector& in,
                                       OUT& out ) const
{

  try
  {

    if ( in.getSize() == 0 )
    {

      throw std::runtime_error( "empty vector" );

    }

    std::vector< double > copy( in.getSize() );

    int32_t index = 0;
    std::vector< double >::iterator
      c = copy.begin(),
      ce = copy.end();
    while ( c != ce )
    {

      *c = in( index );
      ++ index;
      ++ c;

    }

    std::sort( copy.begin(), copy.end() );
    gkg::Converter< double, OUT > converter;
    converter.convert( copy[ copy.size() / 2 - ( copy.size() % 2 ?  0U : 1U ) ],
                       out );

  }
  GKG_CATCH( "template < class OUT > "
             "inline "
             "void gkg::MedianFilter< gkg::Vector, OUT >::filter( "
             "const gkg::Vector& in, OUT& out ) const" );

}


//
// class MedianFilter< Volume< IN >,
//                     OUT >
//

template < class IN, class OUT >
inline
void gkg::MedianFilter< gkg::Volume< IN >,
                        OUT >::filter( const gkg::Volume< IN >& in,
                                       OUT& out ) const
{

  try
  {

    std::vector< IN > copy( in.getSizeXYZT() );
    typename gkg::Volume< IN >::const_iterator i = in.begin(),
                                               ie = in.end();
    int32_t count = 0;
    while ( i != ie )
    {

      copy[ count ] = *i;
      ++ count;
      ++ i;

    }
    std::sort( copy.begin(), copy.end() );
    gkg::Converter< IN, OUT > converter;
    converter.convert( copy[ copy.size() / 2 - ( copy.size() % 2 ?  0U : 1U ) ],
                       out );

  }
  GKG_CATCH( "template < class IN, class OUT > "
             "inline "
             "void gkg::MedianFilter< gkg::Volume< IN >, OUT >::filter( "
             "const gkg::Volume< IN >& in, OUT& out ) const" );

}


//
// class MedianFilter< Matrix,
//                     OUT >
//

template < class OUT >
inline
void gkg::MedianFilter< gkg::Matrix,
                        OUT >::filter( const gkg::Matrix& in,
                                       OUT& out ) const
{

  try
  {

    int32_t size1 = in.getSize1();
    int32_t size2 = in.getSize2();
    std::vector< double > copy( size1 * size2 );

    int32_t i1 = 0;
    int32_t i2 = 0;

    int32_t index = 0;
    for ( i1 = 0; i1 < size1; ++ i1 )
    {

      for ( i2 = 0; i2 < size2; ++ i2 )
      {

        copy[ index ] = in( i1, i2 );
        ++ index;

      }

    }
    std::sort( copy.begin(), copy.end() );
    gkg::Converter< double, OUT > converter;
    converter.convert( copy[ copy.size() / 2 - ( copy.size() % 2 ?  0U : 1U ) ],
                       out );

  }
  GKG_CATCH( "template < class OUT > "
             "inline "
             "void gkg::MedianFilter< gkg::Matrix, OUT >::filter( "
             "const gkg::Matrix& in, OUT& out ) const" );

}


//
// class MedianFilter< SparseMatrix,
//                     OUT >
//

template < class OUT >
inline
void gkg::MedianFilter< gkg::SparseMatrix,
                        OUT >::filter( const gkg::SparseMatrix& in,
                                       OUT& out ) const
{

  try
  {

    const std::map< size_t, double >& data = in.getData();
    std::vector< double > copy( data.size() );

    std::map< size_t, double >::const_iterator
      i = data.begin(),
      ie = data.end();
    std::vector< double >::iterator c = copy.begin();
    while ( i != ie )
    {

      *c = i->second;
      ++ i;
      ++ c;

    }

    std::sort( copy.begin(), copy.end() );
    gkg::Converter< double, OUT > converter;
    converter.convert( copy[ copy.size() / 2 - ( copy.size() % 2 ?  0U : 1U ) ],
                       out );

  }
  GKG_CATCH( "template < class OUT > "
             "inline "
             "void gkg::MedianFilter< gkg::SparseMatrix, OUT >::filter( "
             "const gkg::SparseMatrix& in, OUT& out ) const" );

}


#endif
