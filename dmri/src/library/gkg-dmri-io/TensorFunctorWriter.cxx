#include <gkg-dmri-io/TensorFunctorWriter.h>
#include <gkg-dmri-pdf-dti/TensorFunctorFactory_i.h>
#include <gkg-dmri-io/DefaultAsciiItemWriterOfTensor_i.h>
#include <gkg-dmri-io/DefaultBinaryItemWriterOfTensor_i.h>
#include <gkg-dmri-io/DefaultBSwapItemWriterOfTensor_i.h>
#include <gkg-dmri-io/DefaultAsciiItemReaderOfTensor_i.h>
#include <gkg-dmri-io/DefaultBinaryItemReaderOfTensor_i.h>
#include <gkg-dmri-io/DefaultBSwapItemReaderOfTensor_i.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-container/MeshMap_i.h>
#include <gkg-processing-container/TextureMap_i.h>
#include <gkg-processing-io/TextureMapAnalyzer_i.h>
#include <gkg-processing-io/TextureMapDiskFormat_i.h>
#include <gkg-processing-container/SiteMap_i.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-processing-colorimetry/RGBComponent.h>
#include <gkg-dmri-pdf-dti/Tensor.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-exception/Exception.h>
#include <iostream>
#include <utility>


////////////////////////////////////////////////////////////////////////////////
// creating disk format for TextureMap< Tensor >
////////////////////////////////////////////////////////////////////////////////

template class gkg::TextureMapAnalyzer< gkg::Tensor >;
template class gkg::DiskFormatFactory< gkg::TextureMap< gkg::Tensor > >;
template class gkg::TextureMapDiskFormat< gkg::Tensor >;

RegisterTextureAnalyzer( gkg::Tensor, gkg_tensor );
RegisterTextureDiskFormat( gkg::Tensor, gkg_tensor );


gkg::TensorFunctorWriter::TensorFunctorWriter(
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
  GKG_CATCH( "gkg::TensorFunctorWriter::TensorFunctorWriter( float rgbScale, "
             "float meshScale, "
             "float lowerFAThreshold, "
             "float upperFAThreshold, "
             "const std::string& volumeFormat, "
             "const std::string& meshMapFormat, "
             "const std::string& textureMapFormat, "
             "bool ascii, "
             "bool verbose )" );

}


gkg::TensorFunctorWriter::~TensorFunctorWriter()
{
}


void gkg::TensorFunctorWriter::write(
                             const std::list< std::string >& tensorFunctorNames,
                             const std::list< std::string >& fileNames,
                             gkg::TensorCartesianField& tensorCartesianField )
{

  try
  {

    std::list< std::string >::const_iterator
      n = tensorFunctorNames.begin(),
      ne = tensorFunctorNames.end();
    std::list< std::string >::const_iterator
      f = fileNames.begin();
    while ( n != ne )
    {

      // C = gkg::Volume< float >, I = float
      if ( gkg::TensorFunctorFactory< gkg::Volume< float >,
                             float >::getInstance().hasTensorFunctor( *n ) )
      {

        _stringParameters[ 0 ] = *f;
        gkg::Volume< float > output;
        gkg::TensorFunctorFactory< gkg::Volume< float >,
                                   float >::getInstance().
          getTensorFunctor( *n ).process( output,
                                          tensorCartesianField,
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
      else if ( gkg::TensorFunctorFactory< gkg::Volume< 
                                                       gkg::Vector3d< float > >,
            gkg::Vector3d< float > >::getInstance().hasTensorFunctor( *n ) )
      {

        _stringParameters[ 0 ] = *f;
        gkg::Volume< gkg::Vector3d< float > > output;
        gkg::TensorFunctorFactory< gkg::Volume< gkg::Vector3d< float > >,
                                       gkg::Vector3d< float > >::getInstance().
          getTensorFunctor( *n ).process( output,
                                          tensorCartesianField,
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
      else if ( gkg::TensorFunctorFactory< gkg::Volume< float >,
              std::vector< float > >::getInstance().hasTensorFunctor( *n ) )
      {

        _stringParameters[ 0 ] = *f;
        gkg::Volume< float > output;
        gkg::TensorFunctorFactory< gkg::Volume< float >,
                                         std::vector< float > >::getInstance().
          getTensorFunctor( *n ).process( output,
                                          tensorCartesianField,
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
      else if ( gkg::TensorFunctorFactory< gkg::Volume< gkg::RGBComponent >,
                 gkg::RGBComponent >::getInstance().hasTensorFunctor( *n ) )
      {

        _stringParameters[ 0 ] = *f;
        gkg::Volume< gkg::RGBComponent > output;
        gkg::TensorFunctorFactory< gkg::Volume< gkg::RGBComponent >,
                                            gkg::RGBComponent >::getInstance().
          getTensorFunctor( *n ).process( output,
                                          tensorCartesianField,
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
      // C = gkg::TextureMap< gkg::Tensor >,
      // I = gkg::Tensor
      else if ( gkg::TensorFunctorFactory< gkg::TextureMap< gkg::Tensor >,
                                           gkg::Tensor >::getInstance().
                                                    hasTensorFunctor( *n ) )
      {

        _stringParameters[ 0 ] = *f;
        gkg::TextureMap< gkg::Tensor > output;
        gkg::TensorFunctorFactory< gkg::TextureMap< gkg::Tensor >,
                                   gkg::Tensor >::getInstance().
          getTensorFunctor( *n ).process( output,
                                          tensorCartesianField,
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
      else if ( gkg::TensorFunctorFactory< gkg::SiteMap< int32_t, int32_t >,
                                     gkg::Vector3d< int32_t > >::getInstance().
                                                    hasTensorFunctor( *n ) )
      {

        _stringParameters[ 0 ] = *f;
        gkg::SiteMap< int32_t, int32_t > output;
        gkg::TensorFunctorFactory< gkg::SiteMap< int32_t, int32_t >,
                                      gkg::Vector3d< int32_t > >::getInstance().
          getTensorFunctor( *n ).process( output,
                                          tensorCartesianField,
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
  GKG_CATCH( "void gkg::TensorFunctorWriter::write( "
             "const std::list< std::string >& tensorFunctorNames, "
             "const std::list< std::string >& fileNames, "
             "gkg::TensorCartesianField& tensorCartesianField ) const" );

}


std::list< std::string > gkg::TensorFunctorWriter::getNames()
{

  try
  {

    std::list< std::string > functorNames;

    // C = gkg::Volume< float >, I = float
    std::list< std::string > otherFunctorNames =
      gkg::TensorFunctorFactory< gkg::Volume< float >,
                                        float >::getInstance().getNames();
    functorNames.merge( otherFunctorNames );

    // C = gkg::Volume< gkg::Vector3d< float > >, I = gkg::Vector3d< float >
    otherFunctorNames =
      gkg::TensorFunctorFactory< gkg::Volume< gkg::Vector3d< float > >,
                       gkg::Vector3d< float > >::getInstance().getNames();
    functorNames.merge( otherFunctorNames );

    // C = gkg::Volume< float >, I = std::vector< float >
    otherFunctorNames =
      gkg::TensorFunctorFactory< gkg::Volume< float >,
                         std::vector< float > >::getInstance().getNames();
    functorNames.merge( otherFunctorNames );

    // C = gkg::Volume< gkg::RGBComponent >, I = gkg::RGBComponent
    otherFunctorNames =
      gkg::TensorFunctorFactory< gkg::Volume< gkg::RGBComponent >,
                            gkg::RGBComponent >::getInstance().getNames();
    functorNames.merge( otherFunctorNames );

    // C = gkg::TextureMap< gkg::Tensor >,
    // I = gkg::Tensor
    otherFunctorNames =
      gkg::TensorFunctorFactory< gkg::TextureMap< gkg::Tensor >,
                                  gkg::Tensor >::getInstance().getNames();
    functorNames.merge( otherFunctorNames );

    // C = SiteMap< int32_t, int32_t >, I = gkg::Vector3d< int32_t >
    otherFunctorNames =
      gkg::TensorFunctorFactory< gkg::SiteMap< int32_t, int32_t >,
                     gkg::Vector3d< int32_t > >::getInstance().getNames();
    functorNames.merge( otherFunctorNames );

    return functorNames;

  }
  GKG_CATCH( "std::list< std::string > "
             "gkg::TensorFunctorWriter::getNames()" );

}


