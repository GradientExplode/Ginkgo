#ifndef _gkg_dmri_pdf_two_pool_model_TwoPoolModelFunctor_i_h_
#define _gkg_dmri_pdf_two_pool_model_TwoPoolModelFunctor_i_h_


#include <gkg-dmri-pdf-two-pool-model/TwoPoolModelFunctor.h>
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
gkg::TwoPoolModelFunctor< C, I >::TwoPoolModelFunctor()
{
}


template < class C, class I >
inline
gkg::TwoPoolModelFunctor< C, I >::~TwoPoolModelFunctor()
{
}


template < class C, class I >
inline
void gkg::TwoPoolModelFunctor< C, I >::process(
                C& /*output*/,
                gkg::TwoPoolModelCartesianField& /*twoPoolModelCartesianField*/,
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
             "void gkg::TwoPoolModelFunctor< C, I >::process( "
             "C& output, "
             "gkg::TwoPoolModelCartesianField& twoPoolModelCartesianField, "
             "const std::vector< double >& scalarParameters, "
             "const std::vector< std::string >& stringParameters, "
             "bool verbose ) const" );

}


template < class C, class I >
inline
void gkg::TwoPoolModelFunctor< C, I >::processItem(
                 I& /*item*/,
                 const gkg::TwoPoolModel& /*twoPoolModel*/,
                 const std::vector< double >& /*scalarParameters*/,
                 const std::vector< std::string >& /*stringParameters*/ ) const
{

  try
  {

    // does nothing by default

  }
  GKG_CATCH( "template < class C, class I > "
             "inline "
             "void gkg::TwoPoolModelFunctor< C, I >::processItem( "
             "I& /*item*/, "
             "const gkg::TwoPoolModel& /*twoPoolModel*/, "
             "const std::vector< double >& /*scalarParameters*/, "
             "const std::vector< std::string >& /*stringParameters*/ ) const" );

}


namespace gkg
{


template <>
inline
void TwoPoolModelFunctor< Volume< float >, float >::process(
                         Volume< float >& output,
                         TwoPoolModelCartesianField& twoPoolModelCartesianField,
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
    int32_t 
      siteCount = twoPoolModelCartesianField.getSiteMap().getSiteCount( 0 );

    // getting site(s)
    const std::list< Vector3d< int32_t > >&
      sites = twoPoolModelCartesianField.getSiteMap().getSites( 0 );

    // getting offset
    Vector3d< int32_t > 
      offset = twoPoolModelCartesianField.getOffset();

    // preparing ouput volume
    int32_t sizeX = twoPoolModelCartesianField.getSizeX();
    int32_t sizeY = twoPoolModelCartesianField.getSizeY();
    int32_t sizeZ = twoPoolModelCartesianField.getSizeZ();
    gkg::Vector3d< double > resolution;
    resolution.x = twoPoolModelCartesianField.getResolutionX();
    resolution.y = twoPoolModelCartesianField.getResolutionY();
    resolution.z = twoPoolModelCartesianField.getResolutionZ();

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
                         *twoPoolModelCartesianField.getItem( *s ),
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
             "void TwoPoolModelFunctor< Volume< float >, float >::process( "
             "Volume< float >& output, "
             "TwoPoolModelCartesianField& twoPoolModelCartesianField, "
             "const std::vector< double >& scalarParameters, "
             "const std::vector< std::string >& stringParameters, "
             "bool verbose ) const" );

}


template <>
inline
void 
TwoPoolModelFunctor< Volume< Vector3d< float > >, Vector3d< float > >::process(
                         Volume< Vector3d< float > >& output,
                         TwoPoolModelCartesianField& twoPoolModelCartesianField,
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
    int32_t 
      siteCount = twoPoolModelCartesianField.getSiteMap().getSiteCount( 0 );

    // getting site(s)
    const std::list< Vector3d< int32_t > >&
      sites = twoPoolModelCartesianField.getSiteMap().getSites( 0 );

    // getting offset
    Vector3d< int32_t > 
      offset = twoPoolModelCartesianField.getOffset();

    // preparing ouput volume
    int32_t sizeX = twoPoolModelCartesianField.getSizeX();
    int32_t sizeY = twoPoolModelCartesianField.getSizeY();
    int32_t sizeZ = twoPoolModelCartesianField.getSizeZ();
    gkg::Vector3d< double > resolution;
    resolution.x = twoPoolModelCartesianField.getResolutionX();
    resolution.y = twoPoolModelCartesianField.getResolutionY();
    resolution.z = twoPoolModelCartesianField.getResolutionZ();

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
                         *twoPoolModelCartesianField.getItem( *s ),
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
             "void TwoPoolModelFunctor< "
             "Volume< Vector3d< float > >, Vector3d< float > >::process( "
             "Volume< float >& output, "
             "TwoPoolModelCartesianField& twoPoolModelCartesianField, "
             "const std::vector< double >& scalarParameters, "
             "const std::vector< std::string >& stringParameters, "
             "bool verbose ) const" );

}


template <>
inline
void TwoPoolModelFunctor< Volume< float >, std::vector< float > >::process(
                         Volume< float >& output,
                         TwoPoolModelCartesianField& twoPoolModelCartesianField,
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
    int32_t 
      siteCount = twoPoolModelCartesianField.getSiteMap().getSiteCount( 0 );

    // getting site(s)
    const std::list< Vector3d< int32_t > >&
      sites = twoPoolModelCartesianField.getSiteMap().getSites( 0 );

    // getting offset
    Vector3d< int32_t > 
      offset = twoPoolModelCartesianField.getOffset();

    // preparing ouput volume
    int32_t sizeX = twoPoolModelCartesianField.getSizeX();
    int32_t sizeY = twoPoolModelCartesianField.getSizeY();
    int32_t sizeZ = twoPoolModelCartesianField.getSizeZ();
    gkg::Vector3d< double > resolution;
    resolution.x = twoPoolModelCartesianField.getResolutionX();
    resolution.y = twoPoolModelCartesianField.getResolutionY();
    resolution.z = twoPoolModelCartesianField.getResolutionZ();

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
                         *twoPoolModelCartesianField.getItem( *s ),
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
             "void TwoPoolModelFunctor< Volume< float >, "
             "std::vector< float > >::process( "
             "Volume< float >& output, "
             "TwoPoolModelCartesianField& twoPoolModelCartesianField, "
             "const std::vector< double >& scalarParameters, "
             "const std::vector< std::string >& stringParameters, "
             "bool verbose ) const" );

}


template <>
inline
void TwoPoolModelFunctor< Volume< RGBComponent >, RGBComponent >::process(
                         Volume< RGBComponent >& output,
                         TwoPoolModelCartesianField& twoPoolModelCartesianField,
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
    int32_t 
      siteCount = twoPoolModelCartesianField.getSiteMap().getSiteCount( 0 );

    // getting site(s)
    const std::list< Vector3d< int32_t > >&
      sites = twoPoolModelCartesianField.getSiteMap().getSites( 0 );

    // getting offset
    Vector3d< int32_t > 
      offset = twoPoolModelCartesianField.getOffset();

    // preparing ouput volume
    int32_t sizeX = twoPoolModelCartesianField.getSizeX();
    int32_t sizeY = twoPoolModelCartesianField.getSizeY();
    int32_t sizeZ = twoPoolModelCartesianField.getSizeZ();
    gkg::Vector3d< double > resolution;
    resolution.x = twoPoolModelCartesianField.getResolutionX();
    resolution.y = twoPoolModelCartesianField.getResolutionY();
    resolution.z = twoPoolModelCartesianField.getResolutionZ();

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
                         *twoPoolModelCartesianField.getItem( *s ),
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
             "void TwoPoolModelFunctor< "
             "Volume< RGBComponent >, RGBComponent >::process( "
             "Volume< float >& output, "
             "TwoPoolModelCartesianField& twoPoolModelCartesianField, "
             "const std::vector< double >& scalarParameters, "
             "const std::vector< std::string >& stringParameters, "
             "bool verbose ) const" );

}


}


#endif
