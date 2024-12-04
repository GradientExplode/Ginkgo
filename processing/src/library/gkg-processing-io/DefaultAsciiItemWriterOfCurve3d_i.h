#ifndef _gkg_processing_io_DefaultAsciiItemWriterOfCurve3d_i_h_
#define _gkg_processing_io_DefaultAsciiItemWriterOfCurve3d_i_h_


#include <gkg-core-io/DefaultAsciiItemWriter_i.h>
#include <gkg-processing-container/Curve3d.h>
#include <gkg-core-io/StdVectorAsciiItemWriter.h>


//
// class DefaultAsciiItemWriter< Curve3d< float > >
//

namespace gkg
{


template <>
inline
DefaultAsciiItemWriter< Curve3d< float > >::DefaultAsciiItemWriter()
                                           : ItemWriter< Curve3d< float > >()
{
}


template <>
inline
DefaultAsciiItemWriter< Curve3d< float > >::~DefaultAsciiItemWriter()
{
}


template <>
inline
void DefaultAsciiItemWriter< Curve3d< float > >::write(
                                                  std::ostream & os,
                                                  const Curve3d< float >* pItem,
                                                  size_t count ) const
{

  ItemWriter< std::vector< Vector3d< float > > >& itemW_vector_of_Vector3d =
    StdVectorAsciiItemWriter< Vector3d< float > >::getInstance();
  ItemWriter< bool >& itemW_bool =
    DefaultAsciiItemWriter< bool >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    itemW_vector_of_Vector3d.write( os, pItem[ i ].getPoints() );
    itemW_bool.write( os, pItem[ i ].isClosed() );

  }

}


template <>
inline
void DefaultAsciiItemWriter< Curve3d< float > >::write(
                                                  largefile_ofstream & os,
                                                  const Curve3d< float >* pItem,
                                                  size_t count ) const
{

  ItemWriter< std::vector< Vector3d< float > > >& itemW_vector_of_Vector3d =
    StdVectorAsciiItemWriter< Vector3d< float > >::getInstance();
  ItemWriter< bool >& itemW_bool =
    DefaultAsciiItemWriter< bool >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    itemW_vector_of_Vector3d.write( os, pItem[ i ].getPoints() );
    itemW_bool.write( os, pItem[ i ].isClosed() );

  }

}


template <>
inline
void DefaultAsciiItemWriter< Curve3d< float > >::write(
                                                  largefile_fstream & fs,
                                                  const Curve3d< float >* pItem,
                                                  size_t count ) const
{

  ItemWriter< std::vector< Vector3d< float > > >& itemW_vector_of_Vector3d =
    StdVectorAsciiItemWriter< Vector3d< float > >::getInstance();
  ItemWriter< bool >& itemW_bool =
    DefaultAsciiItemWriter< bool >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    itemW_vector_of_Vector3d.write( fs, pItem[ i ].getPoints() );
    itemW_bool.write( fs, pItem[ i ].isClosed() );

  }

}


}


//
// class DefaultAsciiItemWriter< Curve3d< double > >
//

namespace gkg
{


template <>
inline
DefaultAsciiItemWriter< Curve3d< double > >::DefaultAsciiItemWriter()
                                            : ItemWriter< Curve3d< double > >()
{
}


template <>
inline
DefaultAsciiItemWriter< Curve3d< double > >::~DefaultAsciiItemWriter()
{
}


template <>
inline
void DefaultAsciiItemWriter< Curve3d< double > >::write(
                                                 std::ostream & os,
                                                 const Curve3d< double >* pItem,
                                                 size_t count ) const
{

  ItemWriter< std::vector< Vector3d< double > > >& itemW_vector_of_Vector3d =
    StdVectorAsciiItemWriter< Vector3d< double > >::getInstance();
  ItemWriter< bool >& itemW_bool =
    DefaultAsciiItemWriter< bool >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    itemW_vector_of_Vector3d.write( os, pItem[ i ].getPoints() );
    itemW_bool.write( os, pItem[ i ].isClosed() );

  }

}


template <>
inline
void DefaultAsciiItemWriter< Curve3d< double > >::write(
                                                 largefile_ofstream & os,
                                                 const Curve3d< double >* pItem,
                                                 size_t count ) const
{

  ItemWriter< std::vector< Vector3d< double > > >& itemW_vector_of_Vector3d =
    StdVectorAsciiItemWriter< Vector3d< double > >::getInstance();
  ItemWriter< bool >& itemW_bool =
    DefaultAsciiItemWriter< bool >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    itemW_vector_of_Vector3d.write( os, pItem[ i ].getPoints() );
    itemW_bool.write( os, pItem[ i ].isClosed() );

  }

}


template <>
inline
void DefaultAsciiItemWriter< Curve3d< double > >::write(
                                                 largefile_fstream & fs,
                                                 const Curve3d< double >* pItem,
                                                 size_t count ) const
{

  ItemWriter< std::vector< Vector3d< double > > >& itemW_vector_of_Vector3d =
    StdVectorAsciiItemWriter< Vector3d< double > >::getInstance();
  ItemWriter< bool >& itemW_bool =
    DefaultAsciiItemWriter< bool >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    itemW_vector_of_Vector3d.write( fs, pItem[ i ].getPoints() );
    itemW_bool.write( fs, pItem[ i ].isClosed() );

  }

}


}


#endif
