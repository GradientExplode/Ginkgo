#ifndef _gkg_dmri_pdf_dti_TensorFunctor_i_h_
#define _gkg_dmri_pdf_dti_TensorFunctor_i_h_


#include <gkg-dmri-pdf-dti/TensorFunctor.h>
#include <gkg-dmri-container/DiffusionCartesianField_i.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-processing-colorimetry/RGBComponent.h>
#include <gkg-core-exception/Exception.h>
#include <gkg-core-io/Eraser.h>
#include <iostream>
#include <iomanip>


template < class C, class I >
inline
gkg::TensorFunctor< C, I >::TensorFunctor()
{
}


template < class C, class I >
inline
gkg::TensorFunctor< C, I >::~TensorFunctor()
{
}


template < class C, class I >
inline
void gkg::TensorFunctor< C, I >::process(
                        C& /*output*/,
                        gkg::TensorCartesianField& /*tensorCartesianField*/,
                        const std::vector< double >& /*scalarParameters*/,
                        const std::vector< std::string >& /*stringParameters*/,
                        bool /*verbose*/ ) const
{

  try
  {

    throw std::runtime_error( "not a managed output container type" );

  }
  GKG_CATCH( "template < class C, class I > "
             "inline "
             "void gkg::TensorFunctor< C, I >::process( "
             "C& output, "
             "gkg::TensorCartesianField& tensorCartesianField, "
             "const std::vector< double >& scalarParameters, "
             "const std::vector< std::string >& stringParameters, "
             "bool verbose ) const" );

}


template < class C, class I >
inline
void gkg::TensorFunctor< C, I >::processItem(
                 I& /*item*/,
                 const gkg::Tensor& /*tensor*/,
                 const std::vector< double >& /*scalarParameters*/,
                 const std::vector< std::string >& /*stringParameters*/ ) const
{

  try
  {

    // does nothing by default

  }
  GKG_CATCH( "template < class C, class I > "
             "inline "
             "void gkg::TensorFunctor< C, I >::processItem( "
             "I& /*item*/, "
             "const gkg::Tensor& /*tensor*/, "
             "const std::vector< double >& /*scalarParameters*/, "
             "const std::vector< std::string >& /*stringParameters*/ ) const" );

}


namespace gkg
{


template <>
inline
void TensorFunctor< Volume< float >, float >::process(
                             Volume< float >& output,
                             TensorCartesianField& tensorCartesianField,
                             const std::vector< double >& scalarParameters,
                             const std::vector< std::string >& stringParameters,
                             bool verbose ) const
{

  try
  {

    if ( verbose )
    {

      std::cout << "processing " << this->getName() << " : " << std::flush;

    }

    // getting site count
    int32_t siteCount = tensorCartesianField.getSiteMap().getSiteCount( 0 );

    // getting site(s)
    const std::list< Vector3d< int32_t > >&
      sites = tensorCartesianField.getSiteMap().getSites( 0 );

    // getting offset
    Vector3d< int32_t > 
      offset = tensorCartesianField.getOffset();

    // preparing ouput volume
    int32_t sizeX = tensorCartesianField.getSizeX();
    int32_t sizeY = tensorCartesianField.getSizeY();
    int32_t sizeZ = tensorCartesianField.getSizeZ();
    gkg::Vector3d< double > resolution;
    resolution.x = tensorCartesianField.getResolutionX();
    resolution.y = tensorCartesianField.getResolutionY();
    resolution.z = tensorCartesianField.getResolutionZ();

    output.reallocate( sizeX, sizeY, sizeZ );
    output.getHeader().addAttribute( "resolutionX", resolution.x );
    output.getHeader().addAttribute( "resolutionY", resolution.y );
    output.getHeader().addAttribute( "resolutionZ", resolution.z );

    output.fill( 0.0f );

    // looping over site(s) and processing output item(s)
    int32_t count = 0;
    std::list< Vector3d< int32_t > >::const_iterator
      s = sites.begin(),
      se = sites.end();
    while ( s != se )
    {

      if ( verbose )
      {

        if ( ( ( count + 1 ) % 100 == 0 ) ||
             ( count == 0 ) ||
             ( count == siteCount - 1 ) )
        {

          if ( count )
          {

            std::cout << Eraser( 22 );

          }
          std::cout << " [ " << std::setw( 7 ) << count + 1 
                    << " / " << std::setw( 7 ) << siteCount
                    << " ]" << std::flush;

        }

      }
      this->processItem( output( *s - offset ),
                         *tensorCartesianField.getItem( *s ),
                         scalarParameters,
                         stringParameters );

      ++ s;
      ++ count;

    }
    if ( verbose )
    {

      std::cout << Eraser( 22 ) << "done" << std::endl;

    }

  }
  GKG_CATCH( "template <> "
             "inline "
             "void TensorFunctor< Volume< float >, float >::process( "
             "Volume< float >& output, "
             "TensorCartesianField& tensorCartesianField, "
             "const std::vector< double >& scalarParameters, "
             "const std::vector< std::string >& stringParameters, "
             "bool verbose ) const" );

}


template <>
inline
void TensorFunctor< Volume< Vector3d< float > >, Vector3d< float > >::process(
                             Volume< Vector3d< float > >& output,
                             TensorCartesianField& tensorCartesianField,
                             const std::vector< double >& scalarParameters,
                             const std::vector< std::string >& stringParameters,
                             bool verbose ) const
{

  try
  {

    if ( verbose )
    {

      std::cout << "processing " << this->getName() << " : " << std::flush;

    }

    // getting site count
    int32_t siteCount = tensorCartesianField.getSiteMap().getSiteCount( 0 );

    // getting site(s)
    const std::list< Vector3d< int32_t > >&
      sites = tensorCartesianField.getSiteMap().getSites( 0 );

    // getting offset
    Vector3d< int32_t > 
      offset = tensorCartesianField.getOffset();

    // preparing ouput volume
    int32_t sizeX = tensorCartesianField.getSizeX();
    int32_t sizeY = tensorCartesianField.getSizeY();
    int32_t sizeZ = tensorCartesianField.getSizeZ();
    gkg::Vector3d< double > resolution;
    resolution.x = tensorCartesianField.getResolutionX();
    resolution.y = tensorCartesianField.getResolutionY();
    resolution.z = tensorCartesianField.getResolutionZ();

    output.reallocate( sizeX, sizeY, sizeZ );
    output.getHeader().addAttribute( "resolutionX", resolution.x );
    output.getHeader().addAttribute( "resolutionY", resolution.y );
    output.getHeader().addAttribute( "resolutionZ", resolution.z );

    output.fill( Vector3d< float >( 0, 0, 0 ) );

    // looping over site(s) and processing output item(s)
    int32_t count = 0;
    std::list< Vector3d< int32_t > >::const_iterator
      s = sites.begin(),
      se = sites.end();
    while ( s != se )
    {

      if ( verbose )
      {

        if ( ( ( count + 1 ) % 100 == 0 ) ||
             ( count == 0 ) ||
             ( count == siteCount - 1 ) )
        {

          if ( count )
          {

            std::cout << Eraser( 22 );

          }
          std::cout << " [ " << std::setw( 7 ) << count + 1 
                    << " / " << std::setw( 7 ) << siteCount
                    << " ]" << std::flush;

        }

      }
      this->processItem( output( *s - offset ),
                         *tensorCartesianField.getItem( *s ),
                         scalarParameters,
                         stringParameters );

      ++ s;
      ++ count;

    }
    if ( verbose )
    {

      std::cout << Eraser( 22 ) << "done" << std::endl;

    }

  }
  GKG_CATCH( "template <> "
             "inline "
             "void TensorFunctor< "
             "Volume< Vector3d< float > >, Vector3d< float > >::process( "
             "Volume< float >& output, "
             "TensorCartesianField& tensorCartesianField, "
             "const std::vector< double >& scalarParameters, "
             "const std::vector< std::string >& stringParameters, "
             "bool verbose ) const" );

}


template <>
inline
void TensorFunctor< Volume< float >, std::vector< float > >::process(
                             Volume< float >& output,
                             TensorCartesianField& tensorCartesianField,
                             const std::vector< double >& scalarParameters,
                             const std::vector< std::string >& stringParameters,
                             bool verbose ) const
{

  try
  {

    if ( verbose )
    {

      std::cout << "processing " << this->getName() << " : " << std::flush;

    }

    // getting site count
    int32_t siteCount = tensorCartesianField.getSiteMap().getSiteCount( 0 );

    // getting site(s)
    const std::list< Vector3d< int32_t > >&
      sites = tensorCartesianField.getSiteMap().getSites( 0 );

    // getting offset
    Vector3d< int32_t > 
      offset = tensorCartesianField.getOffset();

    // preparing ouput volume
    int32_t sizeX = tensorCartesianField.getSizeX();
    int32_t sizeY = tensorCartesianField.getSizeY();
    int32_t sizeZ = tensorCartesianField.getSizeZ();
    gkg::Vector3d< double > resolution;
    resolution.x = tensorCartesianField.getResolutionX();
    resolution.y = tensorCartesianField.getResolutionY();
    resolution.z = tensorCartesianField.getResolutionZ();

    // looping over site(s) and processing output item(s)
    int32_t count = 0;
    int32_t itemCount = 0;
    int32_t i = 0;
    std::vector< float > item;
    std::list< Vector3d< int32_t > >::const_iterator
      s = sites.begin(),
      se = sites.end();
    while ( s != se )
    {

      if ( verbose )
      {

        if ( ( ( count + 1 ) % 100 == 0 ) ||
             ( count == 0 ) ||
             ( count == siteCount - 1 ) )
        {

          if ( count )
          {

            std::cout << Eraser( 22 );

          }
          std::cout << " [ " << std::setw( 7 ) << count + 1 
                    << " / " << std::setw( 7 ) << siteCount
                    << " ]" << std::flush;

        }

      }
      this->processItem( item,
                         *tensorCartesianField.getItem( *s ),
                         scalarParameters,
                         stringParameters );
      if ( s == sites.begin() )
      {

        itemCount = ( int32_t )item.size();
        output.reallocate( sizeX, sizeY, sizeZ, itemCount );
        output.getHeader().addAttribute( "resolutionX", resolution.x );
        output.getHeader().addAttribute( "resolutionY", resolution.y );
        output.getHeader().addAttribute( "resolutionZ", resolution.z );
        output.fill( 0.0f );

      }
      for ( i = 0; i < itemCount; i++ )
      {

        output( *s - offset, i ) = item[ i ];

      }

      ++ s;
      ++ count;

    }
    if ( verbose )
    {

      std::cout << Eraser( 22 ) << "done" << std::endl;

    }

  }
  GKG_CATCH( "template <> "
             "inline "
             "void TensorFunctor< Volume< float >, "
             "std::vector< float > >::process( "
             "Volume< float >& output, "
             "TensorCartesianField& tensorCartesianField, "
             "const std::vector< double >& scalarParameters, "
             "const std::vector< std::string >& stringParameters, "
             "bool verbose ) const" );

}


template <>
inline
void TensorFunctor< Volume< RGBComponent >, RGBComponent >::process(
                             Volume< RGBComponent >& output,
                             TensorCartesianField& tensorCartesianField,
                             const std::vector< double >& scalarParameters,
                             const std::vector< std::string >& stringParameters,
                             bool verbose ) const
{

  try
  {

    if ( verbose )
    {

      std::cout << "processing " << this->getName() << " : " << std::flush;

    }

    // getting site count
    int32_t siteCount = tensorCartesianField.getSiteMap().getSiteCount( 0 );

    // getting site(s)
    const std::list< Vector3d< int32_t > >&
      sites = tensorCartesianField.getSiteMap().getSites( 0 );

    // getting offset
    Vector3d< int32_t > 
      offset = tensorCartesianField.getOffset();

    // preparing ouput volume
    int32_t sizeX = tensorCartesianField.getSizeX();
    int32_t sizeY = tensorCartesianField.getSizeY();
    int32_t sizeZ = tensorCartesianField.getSizeZ();
    gkg::Vector3d< double > resolution;
    resolution.x = tensorCartesianField.getResolutionX();
    resolution.y = tensorCartesianField.getResolutionY();
    resolution.z = tensorCartesianField.getResolutionZ();

    output.reallocate( sizeX, sizeY, sizeZ );
    output.getHeader().addAttribute( "resolutionX", resolution.x );
    output.getHeader().addAttribute( "resolutionY", resolution.y );
    output.getHeader().addAttribute( "resolutionZ", resolution.z );

    output.fill( RGBComponent( 0, 0, 0 ) );

    // looping over site(s) and processing output item(s)
    int32_t count = 0;
    std::list< Vector3d< int32_t > >::const_iterator
      s = sites.begin(),
      se = sites.end();
    while ( s != se )
    {

      if ( verbose )
      {

        if ( ( ( count + 1 ) % 100 == 0 ) ||
             ( count == 0 ) ||
             ( count == siteCount - 1 ) )
        {

          if ( count )
          {

            std::cout << Eraser( 22 );

          }
          std::cout << " [ " << std::setw( 7 ) << count + 1 
                    << " / " << std::setw( 7 ) << siteCount
                    << " ]" << std::flush;

        }

      }
      this->processItem( output( *s - offset ),
                         *tensorCartesianField.getItem( *s ),
                         scalarParameters,
                         stringParameters );

      ++ s;
      ++ count;

    }
    if ( verbose )
    {

      std::cout << Eraser( 22 ) << "done" << std::endl;

    }

  }
  GKG_CATCH( "template <> "
             "inline "
             "void TensorFunctor< "
             "Volume< RGBComponent >, RGBComponent >::process( "
             "Volume< float >& output, "
             "TensorCartesianField& tensorCartesianField, "
             "const std::vector< double >& scalarParameters, "
             "const std::vector< std::string >& stringParameters, "
             "bool verbose ) const" );

}


}


#endif
