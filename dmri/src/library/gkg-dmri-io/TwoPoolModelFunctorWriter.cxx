#include <gkg-dmri-io/TwoPoolModelFunctorWriter.h>
#include <gkg-dmri-pdf-two-pool-model/TwoPoolModelFunctorFactory_i.h>
#include <gkg-dmri-io/DefaultAsciiItemWriterOfTwoPoolModel_i.h>
#include <gkg-dmri-io/DefaultBinaryItemWriterOfTwoPoolModel_i.h>
#include <gkg-dmri-io/DefaultBSwapItemWriterOfTwoPoolModel_i.h>
#include <gkg-dmri-io/DefaultAsciiItemReaderOfTwoPoolModel_i.h>
#include <gkg-dmri-io/DefaultBinaryItemReaderOfTwoPoolModel_i.h>
#include <gkg-dmri-io/DefaultBSwapItemReaderOfTwoPoolModel_i.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-container/MeshMap_i.h>
#include <gkg-processing-container/TextureMap_i.h>
#include <gkg-processing-io/TextureMapAnalyzer_i.h>
#include <gkg-processing-io/TextureMapDiskFormat_i.h>
#include <gkg-processing-container/SiteMap_i.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-processing-colorimetry/RGBComponent.h>
#include <gkg-dmri-pdf-two-pool-model/TwoPoolModel.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-exception/Exception.h>
#include <iostream>
#include <utility>


////////////////////////////////////////////////////////////////////////////////
// creating disk format for TextureMap< TwoPoolModel >
////////////////////////////////////////////////////////////////////////////////

template class gkg::TextureMapAnalyzer< gkg::TwoPoolModel >;
template class gkg::DiskFormatFactory< gkg::TextureMap< gkg::TwoPoolModel > >;
template class gkg::TextureMapDiskFormat< gkg::TwoPoolModel >;

RegisterTextureAnalyzer( gkg::TwoPoolModel, gkg_two_pool_model );
RegisterTextureDiskFormat( gkg::TwoPoolModel, gkg_two_pool_model );


gkg::TwoPoolModelFunctorWriter::TwoPoolModelFunctorWriter(
                                         float rgbScale,
                                         float meshScale,
                                         float lowerFAThreshold,
                                         float upperFAThreshold,
                                         const std::string& volumeFormat,
                                         const std::string& meshMapFormat,
                                         const std::string& textureMapFormat,
                                         bool ascii,
                                         bool verbose )
                               : _scalarParameters( 4 ),
                                 _stringParameters( 1 ),
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
    if ( lowerFAThreshold < 0.0 )
    {

      throw std::runtime_error( "lower FA threshold must be positive" );

    }
    if ( upperFAThreshold < 0.0 )
    {

      throw std::runtime_error( "upper FA threshold must be positive" );

    }
    if ( lowerFAThreshold > 1 )
    {

      throw std::runtime_error(
                   "lower FA threshold must be lower or equal to 1.0" );

    }
    if ( upperFAThreshold > 1.0 )
    {

      throw std::runtime_error(
                   "lower FA threshold must be lower or equal to 1.0" );

    }

    // filling scalar and string parameter(s)
    _scalarParameters[ 0 ] = rgbScale;
    _scalarParameters[ 1 ] = meshScale;
    _scalarParameters[ 2 ] = lowerFAThreshold;
    _scalarParameters[ 3 ] = upperFAThreshold;


  }
  GKG_CATCH( "gkg::TwoPoolModelFunctorWriter::TwoPoolModelFunctorWriter( "
             "float rgbScale, "
             "float meshScale, "
             "float lowerFAThreshold, "
             "float upperFAThreshold, "
             "const std::string& volumeFormat, "
             "const std::string& meshMapFormat, "
             "const std::string& textureMapFormat, "
             "bool ascii, "
             "bool verbose )" );

}


gkg::TwoPoolModelFunctorWriter::~TwoPoolModelFunctorWriter()
{
}


void gkg::TwoPoolModelFunctorWriter::write(
                   const std::list< std::string >& twoPoolModelFunctorNames,
                   const std::list< std::string >& fileNames,
                   gkg::TwoPoolModelCartesianField& twoPoolModelCartesianField )
{

  try
  {

    std::list< std::string >::const_iterator
      n = twoPoolModelFunctorNames.begin(),
      ne = twoPoolModelFunctorNames.end();
    std::list< std::string >::const_iterator
      f = fileNames.begin();
    while ( n != ne )
    {

      // C = gkg::Volume< float >, I = float
      if ( gkg::TwoPoolModelFunctorFactory< gkg::Volume< float >,
                           float >::getInstance().hasTwoPoolModelFunctor( *n ) )
      {

        _stringParameters[ 0 ] = *f;
        gkg::Volume< float > output;
        gkg::TwoPoolModelFunctorFactory< gkg::Volume< float >,
                                         float >::getInstance().
          getTwoPoolModelFunctor( *n ).process( output,
                                                twoPoolModelCartesianField,
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
      else if ( gkg::TwoPoolModelFunctorFactory<
                                        gkg::Volume< gkg::Vector3d< float > >,
                                        gkg::Vector3d< float > >::getInstance().
                                        hasTwoPoolModelFunctor( *n ) )
      {

        _stringParameters[ 0 ] = *f;
        gkg::Volume< gkg::Vector3d< float > > output;
        gkg::TwoPoolModelFunctorFactory< gkg::Volume< gkg::Vector3d< float > >,
                                       gkg::Vector3d< float > >::getInstance().
          getTwoPoolModelFunctor( *n ).process( output,
                                                twoPoolModelCartesianField,
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
      else if ( gkg::TwoPoolModelFunctorFactory< gkg::Volume< float >,
                                          std::vector< float > >::getInstance().
                                                  hasTwoPoolModelFunctor( *n ) )
      {

        _stringParameters[ 0 ] = *f;
        gkg::Volume< float > output;
        gkg::TwoPoolModelFunctorFactory< gkg::Volume< float >,
                                         std::vector< float > >::getInstance().
          getTwoPoolModelFunctor( *n ).process( output,
                                                twoPoolModelCartesianField,
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
      else if ( gkg::TwoPoolModelFunctorFactory< 
                                     gkg::Volume< gkg::RGBComponent >,
                                     gkg::RGBComponent >::getInstance().
                                                  hasTwoPoolModelFunctor( *n ) )
      {

        _stringParameters[ 0 ] = *f;
        gkg::Volume< gkg::RGBComponent > output;
        gkg::TwoPoolModelFunctorFactory< gkg::Volume< gkg::RGBComponent >,
                                            gkg::RGBComponent >::getInstance().
          getTwoPoolModelFunctor( *n ).process( output,
                                                twoPoolModelCartesianField,
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
      // C = gkg::TextureMap< gkg::TwoPoolModel >,
      // I = gkg::TwoPoolModel
      else if ( gkg::TwoPoolModelFunctorFactory<
                                           gkg::TextureMap< gkg::TwoPoolModel >,
                                           gkg::TwoPoolModel >::getInstance().
                                                  hasTwoPoolModelFunctor( *n ) )
      {

        _stringParameters[ 0 ] = *f;
        gkg::TextureMap< gkg::TwoPoolModel > output;
        gkg::TwoPoolModelFunctorFactory< gkg::TextureMap< gkg::TwoPoolModel >,
                                   gkg::TwoPoolModel >::getInstance().
          getTwoPoolModelFunctor( *n ).process( output,
                                                twoPoolModelCartesianField,
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
      else if ( gkg::TwoPoolModelFunctorFactory< 
                                     gkg::SiteMap< int32_t, int32_t >,
                                     gkg::Vector3d< int32_t > >::getInstance().
                                                  hasTwoPoolModelFunctor( *n ) )
      {

        _stringParameters[ 0 ] = *f;
        gkg::SiteMap< int32_t, int32_t > output;
        gkg::TwoPoolModelFunctorFactory< gkg::SiteMap< int32_t, int32_t >,
                                      gkg::Vector3d< int32_t > >::getInstance().
          getTwoPoolModelFunctor( *n ).process( output,
                                          twoPoolModelCartesianField,
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
  GKG_CATCH( "void gkg::TwoPoolModelFunctorWriter::write( "
             "const std::list< std::string >& twoPoolModelFunctorNames, "
             "const std::list< std::string >& fileNames, "
             "gkg::TwoPoolModelCartesianField& "
             "twoPoolModelCartesianField ) const" );

}


std::list< std::string > gkg::TwoPoolModelFunctorWriter::getNames()
{

  try
  {

    std::list< std::string > functorNames;

    // C = gkg::Volume< float >, I = float
    std::list< std::string > otherFunctorNames =
      gkg::TwoPoolModelFunctorFactory< gkg::Volume< float >,
                                        float >::getInstance().getNames();
    functorNames.merge( otherFunctorNames );

    // C = gkg::Volume< gkg::Vector3d< float > >, I = gkg::Vector3d< float >
    otherFunctorNames =
      gkg::TwoPoolModelFunctorFactory< gkg::Volume< gkg::Vector3d< float > >,
                       gkg::Vector3d< float > >::getInstance().getNames();
    functorNames.merge( otherFunctorNames );

    // C = gkg::Volume< float >, I = std::vector< float >
    otherFunctorNames =
      gkg::TwoPoolModelFunctorFactory< gkg::Volume< float >,
                         std::vector< float > >::getInstance().getNames();
    functorNames.merge( otherFunctorNames );

    // C = gkg::Volume< gkg::RGBComponent >, I = gkg::RGBComponent
    otherFunctorNames =
      gkg::TwoPoolModelFunctorFactory< gkg::Volume< gkg::RGBComponent >,
                            gkg::RGBComponent >::getInstance().getNames();
    functorNames.merge( otherFunctorNames );

    // C = gkg::TextureMap< gkg::TwoPoolModel >,
    // I = gkg::TwoPoolModel
    otherFunctorNames =
      gkg::TwoPoolModelFunctorFactory< gkg::TextureMap< gkg::TwoPoolModel >,
                                  gkg::TwoPoolModel >::getInstance().getNames();
    functorNames.merge( otherFunctorNames );

    // C = SiteMap< int32_t, int32_t >, I = gkg::Vector3d< int32_t >
    otherFunctorNames =
      gkg::TwoPoolModelFunctorFactory< gkg::SiteMap< int32_t, int32_t >,
                     gkg::Vector3d< int32_t > >::getInstance().getNames();
    functorNames.merge( otherFunctorNames );

    return functorNames;

  }
  GKG_CATCH( "std::list< std::string > "
             "gkg::TwoPoolModelFunctorWriter::getNames()" );

}


