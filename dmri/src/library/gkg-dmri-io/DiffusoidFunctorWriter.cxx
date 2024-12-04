#include <gkg-dmri-io/DiffusoidFunctorWriter.h>
#include <gkg-dmri-diffusoid-functor/DiffusoidFunctorFactory_i.h>
#include <gkg-dmri-io/DefaultAsciiItemWriterOfDiffusoid_i.h>
#include <gkg-dmri-io/DefaultBinaryItemWriterOfDiffusoid_i.h>
#include <gkg-dmri-io/DefaultBSwapItemWriterOfDiffusoid_i.h>
#include <gkg-dmri-io/DefaultAsciiItemReaderOfDiffusoid_i.h>
#include <gkg-dmri-io/DefaultBinaryItemReaderOfDiffusoid_i.h>
#include <gkg-dmri-io/DefaultBSwapItemReaderOfDiffusoid_i.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-container/MeshMap_i.h>
#include <gkg-processing-container/TextureMap_i.h>
#include <gkg-processing-io/TextureMapAnalyzer_i.h>
#include <gkg-processing-io/TextureMapDiskFormat_i.h>
#include <gkg-processing-container/SiteMap_i.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-processing-colorimetry/RGBComponent.h>
#include <gkg-dmri-diffusoid/Diffusoid_i.h>
#include <gkg-core-io/TypeOf.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-exception/Exception.h>
#include <iostream>
#include <utility>


////////////////////////////////////////////////////////////////////////////////
// creating disk format for TextureMap< Diffusoid >
////////////////////////////////////////////////////////////////////////////////

template class gkg::TextureMapAnalyzer< gkg::Diffusoid >;
template class gkg::DiskFormatFactory< gkg::TextureMap< gkg::Diffusoid > >;
template class gkg::TextureMapDiskFormat< gkg::Diffusoid >;

RegisterTextureAnalyzer( gkg::Diffusoid, gkg_diffusoid );
RegisterTextureDiskFormat( gkg::Diffusoid, gkg_diffusoid );


gkg::DiffusoidFunctorWriter::DiffusoidFunctorWriter(
                                           float rgbScale,
                                           float meshScale,
                                           const std::string& volumeFormat,
                                           const std::string& meshMapFormat,
                                           const std::string& textureMapFormat,
                                           bool ascii,
                                           bool verbose )
                      : _scalarParameters( 3 ),
                        _stringParameters( 4 ),
                        _volumeFormat( volumeFormat ),
                        _meshMapFormat( meshMapFormat ),
                        _textureMapFormat( textureMapFormat ),
                        _ascii( ascii ),
                        _verbose( verbose )
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // sanity checks
    ////////////////////////////////////////////////////////////////////////////

    if ( rgbScale <= 0 )
    {

      throw std::runtime_error( "RGB scale must be positive" );

    }
    if ( meshScale <= 0 )
    {

      throw std::runtime_error( "mesh scale must be positive" );

    }

    // filling scalar and string parameter(s)
    _scalarParameters[ 0 ] = rgbScale;
    _scalarParameters[ 1 ] = meshScale;
    _scalarParameters[ 2 ] = ( ascii ? 1.0 : 0.0 );

    _stringParameters[ 0 ] = volumeFormat;
    _stringParameters[ 1 ] = meshMapFormat;
    _stringParameters[ 2 ] = textureMapFormat;

  }
  GKG_CATCH( "gkg::DiffusoidFunctorWriter::DiffusoidFunctorWriter( "
             "float rgbScale, "
             "float meshScale, "
             "const std::string& volumeFormat, "
             "const std::string& meshMapFormat, "
             "const std::string& textureMapFormat, "
             "bool ascii, "
             "bool verbose )" );

}


gkg::DiffusoidFunctorWriter::~DiffusoidFunctorWriter()
{
}


void gkg::DiffusoidFunctorWriter::write(
                         const std::list< std::string >& diffusoidFunctorNames,
                         const std::list< std::string >& fileNames,
                         gkg::DiffusoidCartesianField& diffusoidCartesianField )
{

  try
  {

    std::list< std::string >::const_iterator
      n = diffusoidFunctorNames.begin(),
      ne = diffusoidFunctorNames.end();
    std::list< std::string >::const_iterator
      f = fileNames.begin();
    while ( n != ne )
    {

      // C = gkg::Volume< float >, I = float
      if ( gkg::DiffusoidFunctorFactory< gkg::Volume< float >,
                             float >::getInstance().hasDiffusoidFunctor( *n ) )
      {

        _stringParameters[ 3 ] = *f;
        gkg::Volume< float > output;
        gkg::DiffusoidFunctorFactory< gkg::Volume< float >, float >::getInstance().
          getDiffusoidFunctor( *n ).process( output,
                                             diffusoidCartesianField,
                                             _scalarParameters,
                                             _stringParameters,
                                             _verbose );
        if ( _verbose )
        {

          std::cout << "writing " << *n << " '" << *f << "' : "
                    << std::flush;

        }
        gkg::Writer::getInstance().write( *f,
                                          output,
                                          _ascii,
                                          _volumeFormat );
        if ( _verbose )
        {

          std::cout << "done" << std::endl;

        }

      }
      // C = gkg::Volume< gkg::Vector3d< float > >, I = gkg::Vector3d< float >
      else if ( gkg::DiffusoidFunctorFactory< gkg::Volume< gkg::Vector3d< float > >,
            gkg::Vector3d< float > >::getInstance().hasDiffusoidFunctor( *n ) )
      {

        _stringParameters[ 3 ] = *f;
        gkg::Volume< gkg::Vector3d< float > > output;
        gkg::DiffusoidFunctorFactory< gkg::Volume< gkg::Vector3d< float > >,
                                       gkg::Vector3d< float > >::getInstance().
          getDiffusoidFunctor( *n ).process( output,
                                             diffusoidCartesianField,
                                             _scalarParameters,
                                             _stringParameters,
                                             _verbose );
        if ( _verbose )
        {

          std::cout << "writing " << *n << " '" << *f << "' : "
                    << std::flush;

        }
        gkg::Writer::getInstance().write( *f,
                                          output,
                                          _ascii,
                                          _volumeFormat );
        if ( _verbose )
        {

          std::cout << "done" << std::endl;

        }

      }
      // C = gkg::Volume< float >, I = std::vector< float >
      else if ( gkg::DiffusoidFunctorFactory< gkg::Volume< float >,
              std::vector< float > >::getInstance().hasDiffusoidFunctor( *n ) )
      {

        _stringParameters[ 3 ] = *f;
        gkg::Volume< float > output;
        gkg::DiffusoidFunctorFactory< gkg::Volume< float >,
                                         std::vector< float > >::getInstance().
          getDiffusoidFunctor( *n ).process( output,
                                             diffusoidCartesianField,
                                             _scalarParameters,
                                             _stringParameters,
                                             _verbose );
        if ( _verbose )
        {

          std::cout << "writing " << *n << " '" << *f << "' : "
                    << std::flush;

        }
        gkg::Writer::getInstance().write( *f,
                                          output,
                                          _ascii,
                                          _volumeFormat );
        if ( _verbose )
        {

          std::cout << "done" << std::endl;

        }

      }
      // C = gkg::Volume< gkg::RGBComponent >, I = gkg::RGBComponent
      else if ( gkg::DiffusoidFunctorFactory< gkg::Volume< gkg::RGBComponent >,
                 gkg::RGBComponent >::getInstance().hasDiffusoidFunctor( *n ) )
      {

        _stringParameters[ 3 ] = *f;
        gkg::Volume< gkg::RGBComponent > output;
        gkg::DiffusoidFunctorFactory< gkg::Volume< gkg::RGBComponent >,
                                            gkg::RGBComponent >::getInstance().
          getDiffusoidFunctor( *n ).process( output,
                                             diffusoidCartesianField,
                                             _scalarParameters,
                                             _stringParameters,
                                             _verbose );
        if ( _verbose )
        {

          std::cout << "writing " << *n << " '" << *f << "' : "
                    << std::flush;

        }
        gkg::Writer::getInstance().write( *f,
                                          output,
                                          _ascii,
                                          _volumeFormat );
        if ( _verbose )
        {

          std::cout << "done" << std::endl;

        }

      }
      // C = gkg::MeshMap< int32_t, float, 3U >, I = std::vector< float >
      else if ( gkg::DiffusoidFunctorFactory< gkg::MeshMap< int32_t, float, 3U >,
              std::vector< float > >::getInstance().hasDiffusoidFunctor( *n ) )
      {

        _stringParameters[ 3 ] = *f;
        gkg::MeshMap< int32_t, float, 3U > output;
        gkg::DiffusoidFunctorFactory< gkg::MeshMap< int32_t, float, 3U >,
                                         std::vector< float > >::getInstance().
          getDiffusoidFunctor( *n ).process( output,
                                             diffusoidCartesianField,
                                             _scalarParameters,
                                             _stringParameters,
                                             _verbose );
        if ( _verbose )
        {

          std::cout << "writing " << *n << " '" << *f << "' : "
                    << std::flush;

        }
        gkg::Writer::getInstance().write( *f,
                                          output,
                                          _ascii,
                                          _meshMapFormat );
        if ( _verbose )
        {

          std::cout << "done" << std::endl;

        }

      }
      // C = gkg::MeshMap< int32_t, float, 3U >,
      // I = std::vector< std::vector< float > >
      else if ( gkg::DiffusoidFunctorFactory< gkg::MeshMap< int32_t, float, 3U >,
                      std::vector< std::vector< float > > >::getInstance().
                                                hasDiffusoidFunctor( *n ) )
      {

        _stringParameters[ 3 ] = *f;
        gkg::MeshMap< int32_t, float, 3U > output;
        gkg::DiffusoidFunctorFactory< gkg::MeshMap< int32_t, float, 3U >,
                      std::vector< std::vector< float > > >::getInstance().
          getDiffusoidFunctor( *n ).process( output,
                                             diffusoidCartesianField,
                                             _scalarParameters,
                                             _stringParameters,
                                             _verbose );
        if ( _verbose )
        {

          std::cout << "writing " << *n << " '" << *f << "' : "
                    << std::flush;

        }
        gkg::Writer::getInstance().write( *f,
                                          output,
                                          _ascii,
                                          _meshMapFormat );
        if ( _verbose )
        {

          std::cout << "done" << std::endl;

        }

      }
      // C = gkg::TextureMap< gkg::Diffusoid >,
      // I = gkg::Diffusoid
      else if ( gkg::DiffusoidFunctorFactory<
                         gkg::TextureMap< gkg::Diffusoid >,
                         gkg::Diffusoid >::getInstance().
                                                    hasDiffusoidFunctor( *n ) )
      {

        _stringParameters[ 3 ] = *f;
        gkg::TextureMap< gkg::Diffusoid > output;
        gkg::DiffusoidFunctorFactory<
                         gkg::TextureMap< gkg::Diffusoid >,
                         gkg::Diffusoid >::getInstance().
          getDiffusoidFunctor( *n ).process( output,
                                             diffusoidCartesianField,
                                             _scalarParameters,
                                             _stringParameters,
                                             _verbose );
        if ( _verbose )
        {

          std::cout << "writing " << *n << " '" << *f << "' : "
                    << std::flush;

        }
        gkg::Writer::getInstance().write( *f,
                                          output,
                                          _ascii,
                                          _volumeFormat );
        if ( _verbose )
        {

          std::cout << "done" << std::endl;

        }

      }
      // C = SiteMap< int32_t, int32_t >, I = gkg::Vector3d< int32_t >
      else if ( gkg::DiffusoidFunctorFactory< gkg::SiteMap< int32_t, int32_t >,
                                     gkg::Vector3d< int32_t > >::getInstance().
                                                    hasDiffusoidFunctor( *n ) )
      {

        _stringParameters[ 3 ] = *f;
        gkg::SiteMap< int32_t, int32_t > output;
        gkg::DiffusoidFunctorFactory< gkg::SiteMap< int32_t, int32_t >,
                                      gkg::Vector3d< int32_t > >::getInstance().
          getDiffusoidFunctor( *n ).process( output,
                                             diffusoidCartesianField,
                                             _scalarParameters,
                                             _stringParameters,
                                             _verbose );
        if ( _verbose )
        {

          std::cout << "writing " << *n << " '" << *f << "' : "
                    << std::flush;

        }
        gkg::Writer::getInstance().write( *f,
                                          output,
                                          _ascii,
                                          _volumeFormat );
        if ( _verbose )
        {

          std::cout << "done" << std::endl;

        }

      }

      ++ n;
      ++ f;

    }

  }
  GKG_CATCH( "void gkg::DiffusoidFunctorWriter::write( "
             "const std::list< std::string >& diffusoidFunctorNames, "
             "const std::list< std::string >& fileNames, "
             "gkg::DiffusoidCartesianField& diffusoidCartesianField ) const" );

}


std::list< std::string > gkg::DiffusoidFunctorWriter::getNames()
{

  try
  {

    std::list< std::string > functorNames;

    // C = gkg::Volume< float >, I = float
    std::list< std::string > otherFunctorNames =
      gkg::DiffusoidFunctorFactory< gkg::Volume< float >,
                                        float >::getInstance().getNames();
    functorNames.merge( otherFunctorNames );

    // C = gkg::Volume< gkg::Vector3d< float > >, I = gkg::Vector3d< float >
    otherFunctorNames =
      gkg::DiffusoidFunctorFactory< gkg::Volume< gkg::Vector3d< float > >,
                       gkg::Vector3d< float > >::getInstance().getNames();
    functorNames.merge( otherFunctorNames );

    // C = gkg::Volume< float >, I = std::vector< float >
    otherFunctorNames =
      gkg::DiffusoidFunctorFactory< gkg::Volume< float >,
                         std::vector< float > >::getInstance().getNames();
    functorNames.merge( otherFunctorNames );

    // C = gkg::Volume< gkg::RGBComponent >, I = gkg::RGBComponent
    otherFunctorNames =
      gkg::DiffusoidFunctorFactory< gkg::Volume< gkg::RGBComponent >,
                            gkg::RGBComponent >::getInstance().getNames();
    functorNames.merge( otherFunctorNames );

    // C = gkg::MeshMap< int32_t, float, 3U >, I = std::vector< float >
    otherFunctorNames =
      gkg::DiffusoidFunctorFactory< gkg::MeshMap< int32_t, float, 3U >,
                         std::vector< float > >::getInstance().getNames();
    functorNames.merge( otherFunctorNames );

    // C = gkg::MeshMap< int32_t, float, 3U >,
    // I = std::vector< std::vector< float > >
    otherFunctorNames =
      gkg::DiffusoidFunctorFactory< gkg::MeshMap< int32_t, float, 3U >,
          std::vector< std::vector< float > > >::getInstance().getNames();
    functorNames.merge( otherFunctorNames );

    // C = gkg::TextureMap< gkg::OrientationDistributionFunction >,
    // I = gkg::OrientationDistributionFunction
    otherFunctorNames =
      gkg::DiffusoidFunctorFactory< gkg::TextureMap<
                                gkg::Diffusoid >,
         gkg::Diffusoid >::getInstance().getNames();
    functorNames.merge( otherFunctorNames );

    // C = SiteMap< int32_t, int32_t >, I = gkg::Vector3d< int32_t >
    otherFunctorNames =
      gkg::DiffusoidFunctorFactory< gkg::SiteMap< int32_t, int32_t >,
                     gkg::Vector3d< int32_t > >::getInstance().getNames();
    functorNames.merge( otherFunctorNames );

    return functorNames;

  }
  GKG_CATCH( "std::list< std::string > "
             "gkg::DiffusoidFunctorWriter::getNames()" );

}

