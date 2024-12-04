#ifndef _gkg_processing_algobase_ConstantFilter_i_h_
#define _gkg_processing_algobase_ConstantFilter_i_h_


#include <gkg-processing-algobase/ConstantFilter.h>
#include <gkg-processing-algobase/FilterFunction_i.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-core-exception/Exception.h>


//
// class ConstantFilter< IN,
//                       OUT >
//

template < class IN, class OUT >
inline
gkg::ConstantFilter< IN,
                     OUT >::ConstantFilter( const OUT& out )
                           : _out( out )
{
}


template < class IN, class OUT >
inline
void gkg::ConstantFilter< IN,
                          OUT >::filter( const IN& /*in*/,
                                         OUT& out ) const
{

  try
  {

    out = _out;

  }
  GKG_CATCH( "template < class IN, class OUT > "
             "inline "
             "void gkg::ConstantFilter< IN, "
             "OUT >::filter( const IN& /*in*/, "
             "OUT& out ) const" );

}


//
// class ConstantFilter< std::vector< IN >,
//                       OUT >
//

template < class IN, class OUT >
inline
gkg::ConstantFilter< std::vector< IN >,
                      OUT >::ConstantFilter( const OUT& out )
                            : _out( out )
{
}


template < class IN, class OUT >
inline
void gkg::ConstantFilter< std::vector< IN >,
                          OUT >::filter( const std::vector< IN >& in,
                                         OUT& out ) const
{

  try
  {

    if ( in.empty() )
    {

      throw std::runtime_error( "empty vector" );

    }
    out = _out;

  }
  GKG_CATCH( "template < class IN, class OUT > "
             "inline "
             "void gkg::ConstantFilter< std::vector< IN >, OUT >::filter( "
             "const std::vector< IN >& in, OUT& out ) const" );

}


//
// class ConstantFilter< Vector,
//                       OUT >
//

template < class OUT >
inline
gkg::ConstantFilter< gkg::Vector,
                     OUT >::ConstantFilter( const OUT& out )
                           : _out( out )
{
}


template < class OUT >
inline
void gkg::ConstantFilter< gkg::Vector,
                          OUT >::filter( const gkg::Vector& /*in*/,
                                         OUT& out ) const
{

  try
  {

    out = _out;

  }
  GKG_CATCH( "template < class OUT > "
             "inline "
             "void gkg::ConstantFilter< gkg::Vector, OUT >::filter( "
             "const gkg::Vector& in, OUT& out ) const" );

}


//
// class ConstantFilter< Volume< IN >,
//                       OUT >
//

template < class IN, class OUT >
inline
gkg::ConstantFilter< gkg::Volume< IN >,
                     OUT >::ConstantFilter( const OUT& out )
                           : _out( out )
{
}


template < class IN, class OUT >
inline
void gkg::ConstantFilter< gkg::Volume< IN >,
                          OUT >::filter( const gkg::Volume< IN >& /*in*/,
                                         OUT& out ) const
{

  try
  {

    out = _out;

  }
  GKG_CATCH( "template < class IN, class OUT > "
             "inline "
             "void gkg::ConstantFilter< gkg::Volume< IN >, OUT >::filter( "
             "const gkg::Volume< IN >& in, OUT& out ) const" );

}


//
// class ConstantFilter< Matrix,
//                       OUT >
//

template < class OUT >
inline
gkg::ConstantFilter< gkg::Matrix,
                     OUT >::ConstantFilter( const OUT& out )
                           : _out( out )
{
}


template < class OUT >
inline
void gkg::ConstantFilter< gkg::Matrix,
                          OUT >::filter( const gkg::Matrix& /*in*/,
                                         OUT& out ) const
{

  try
  {

    out = _out;

  }
  GKG_CATCH( "template < class OUT > "
             "inline "
             "void gkg::ConstantFilter< gkg::Matrix, OUT >::filter( "
             "const gkg::Matrix& in, OUT& out ) const" );

}


//
// class ConstantFilter< SparseMatrix,
//                       OUT >
//

template < class OUT >
inline
gkg::ConstantFilter< gkg::SparseMatrix,
                     OUT >::ConstantFilter( const OUT& out )
                           : _out( out )
{
}


template < class OUT >
inline
void gkg::ConstantFilter< gkg::SparseMatrix,
                          OUT >::filter( const gkg::SparseMatrix& /*in*/,
                                         OUT& out ) const
{

  try
  {

    out = _out;

  }
  GKG_CATCH( "template < class OUT > "
             "inline "
             "void gkg::ConstantFilter< gkg::SparseMatrix, OUT >::filter( "
             "const gkg::SparseMatrix& in, OUT& out ) const" );

}


#endif
