#include <gkg-dmri-io/OdfFunctorWriter.h>
#include <gkg-dmri-odf-functor/OdfFunctorFactory_i.h>
#include <gkg-dmri-io/DefaultAsciiItemWriterOfOdf_i.h>
#include <gkg-dmri-io/DefaultBinaryItemWriterOfOdf_i.h>
#include <gkg-dmri-io/DefaultBSwapItemWriterOfOdf_i.h>
#include <gkg-dmri-io/DefaultAsciiItemReaderOfOdf_i.h>
#include <gkg-dmri-io/DefaultBinaryItemReaderOfOdf_i.h>
#include <gkg-core-io/DefaultBSwapItemReaderOfStdVector_i.h>
#include <gkg-core-io/DefaultAsciiItemWriterOfStdVector_i.h>
#include <gkg-core-io/DefaultBinaryItemWriterOfStdVector_i.h>
#include <gkg-core-io/DefaultBSwapItemWriterOfStdVector_i.h>
#include <gkg-core-io/DefaultAsciiItemReaderOfStdVector_i.h>
#include <gkg-core-io/DefaultBinaryItemReaderOfStdVector_i.h>
#include <gkg-core-io/DefaultBSwapItemReaderOfStdVector_i.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-container/MeshMap_i.h>
#include <gkg-processing-container/TextureMap_i.h>
#include <gkg-processing-io/TextureMapAnalyzer_i.h>
#include <gkg-processing-io/TextureMapDiskFormat_i.h>
#include <gkg-processing-container/SiteMap_i.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-processing-colorimetry/RGBComponent.h>
#include <gkg-dmri-microstructure-plot/MeasurementVsModelPlotter.h>
#include <gkg-dmri-odf/OrientationDistributionFunction.h>
#include <gkg-core-io/TypeOf.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-exception/Exception.h>
#include <iostream>
#include <utility>


////////////////////////////////////////////////////////////////////////////////
// creating disk format for TextureMap< OrientationDistributionFunction >
////////////////////////////////////////////////////////////////////////////////

template class gkg::TextureMapAnalyzer< gkg::OrientationDistributionFunction >;
template class gkg::DiskFormatFactory<
                     gkg::TextureMap< gkg::OrientationDistributionFunction > >;
template class gkg::TextureMapDiskFormat<
                                        gkg::OrientationDistributionFunction >;

RegisterTextureAnalyzer( gkg::OrientationDistributionFunction, gkg_odf );
RegisterTextureDiskFormat( gkg::OrientationDistributionFunction, gkg_odf );


template class gkg::TextureMapAnalyzer< std::vector< gkg::Vector3d< float > > >;
template class gkg::DiskFormatFactory<
                     gkg::TextureMap< std::vector< gkg::Vector3d< float > > > >;
template class gkg::TextureMapDiskFormat<
                                        std::vector< gkg::Vector3d< float > > >;

RegisterTextureAnalyzer( std::vector< gkg::Vector3d< float > >,
                         gkg_odf_peak_map );
RegisterTextureDiskFormat( std::vector< gkg::Vector3d< float > >,
                           gkg_odf_peak_map );


gkg::OdfFunctorWriter::OdfFunctorWriter(
                      float rgbScale,
                      float meshScale,
                      float lowerGFAThreshold,
                      float upperGFAThreshold,
                      const std::string& volumeFormat,
                      const std::string& meshMapFormat,
                      const std::string& textureMapFormat,
                      bool ascii,
                      const std::vector< double >& furtherScalarParameters,
                      const std::vector< std::string >& furtherStringParameters,
                      bool verbose )
                      : _scalarParameters( 5 + 
                                    ( int32_t )furtherScalarParameters.size() ),
                        _stringParameters( 4 + 
                                    ( int32_t )furtherStringParameters.size() ),
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
    if ( lowerGFAThreshold < 0.0 )
    {

      throw std::runtime_error( "lower GFA threshold must be positive" );

    }
    if ( upperGFAThreshold < 0.0 )
    {

      throw std::runtime_error( "upper GFA threshold must be positive" );

    }
    if ( lowerGFAThreshold > 1 )
    {

      throw std::runtime_error(
                   "lower GFA threshold must be lower or equal to 1.0" );

    }
    if ( upperGFAThreshold > 1.0 )
    {

      throw std::runtime_error(
                   "lower GFA threshold must be lower or equal to 1.0" );

    }

    // filling scalar and string parameter(s)
    _scalarParameters[ 0 ] = rgbScale;
    _scalarParameters[ 1 ] = meshScale;
    _scalarParameters[ 2 ] = lowerGFAThreshold;
    _scalarParameters[ 3 ] = upperGFAThreshold;
    _scalarParameters[ 4 ] = ( ascii ? 1.0 : 0.0 );

    _stringParameters[ 0 ] = _volumeFormat;
    _stringParameters[ 1 ] = _meshMapFormat;
    _stringParameters[ 2 ] = _textureMapFormat;

    // _stringParameters[ 3 ] is occupied by the output functor filename

    // adding further parameter(s)

    int32_t furtherScalarParameterCount = 
                                      ( int32_t )furtherScalarParameters.size();
    int32_t furtherStringParameterCount = 
                                      ( int32_t )furtherStringParameters.size();

    int32_t i = 0;
    for ( i = 0; i < furtherScalarParameterCount; i++ )
    {

      _scalarParameters[ 5 + i ] = furtherScalarParameters[ i ];


    }

    for ( i = 0; i < furtherStringParameterCount; i++ )
    {

      _stringParameters[ 4 + i ] = furtherStringParameters[ i ];


    }


  }
  GKG_CATCH( "gkg::OdfFunctorWriter::OdfFunctorWriter( float rgbScale, "
             "float meshScale, "
             "float lowerGFAThreshold, "
             "float upperGFAThreshold, "
             "const std::string& volumeFormat, "
             "const std::string& meshMapFormat, "
             "const std::string& textureMapFormat, "
             "bool ascii, "
             "const std::vector< double >& furtherScalarParameters, "
             "const std::vector< std::string >& furtherStringParameters, "
             "bool verbose )" );

}


gkg::OdfFunctorWriter::~OdfFunctorWriter()
{
}


void gkg::OdfFunctorWriter::write(
                             const std::list< std::string >& odfFunctorNames,
                             const std::list< std::string >& fileNames,
                             gkg::OdfCartesianField& odfCartesianField )
{

  try
  {


    // splitting ODF functor name(s) between those not requiring to be in the
    // standard basis and thoese requiring to be in the standard basis
    std::list< std::pair< std::string, std::string > > 
      namesNotRequiringStandardBasis;
    std::list< std::pair< std::string, std::string > > 
      namesRequiringStandardBasis;
    splitNames( odfFunctorNames, fileNames,
                namesNotRequiringStandardBasis, namesRequiringStandardBasis );

    // first, processing map(s) that do not require converting the ODF
    // map to the standard basis
    write( namesNotRequiringStandardBasis, odfCartesianField );

    if ( !namesRequiringStandardBasis.empty() )
    {

      // transforming ODF texture map to standard basis
      if ( odfCartesianField.getBasisType() != 
           gkg::OrientationDistributionFunction::Standard )
      {

        odfCartesianField.setStandardBasis( _verbose );

      }

      // second, processing map(s) that require converting the ODF
      // map to the standard basis
      write( namesRequiringStandardBasis, odfCartesianField );

    }

  }
  GKG_CATCH( "void gkg::OdfFunctorWriter::write( "
             "const std::list< std::string >& odfFunctorNames, "
             "const std::list< std::string >& fileNames, "
             "gkg::OdfCartesianField& odfCartesianField ) const" );

}


std::list< std::string > gkg::OdfFunctorWriter::getNames()
{

  try
  {

    std::list< std::string > functorNames;

    // C = gkg::Volume< float >, I = float
    std::list< std::string > otherFunctorNames =
      gkg::OdfFunctorFactory< gkg::Volume< float >,
                                        float >::getInstance().getNames();
    functorNames.merge( otherFunctorNames );

    // C = gkg::Volume< gkg::Vector3d< float > >, I = gkg::Vector3d< float >
    otherFunctorNames =
      gkg::OdfFunctorFactory< gkg::Volume< gkg::Vector3d< float > >,
                       gkg::Vector3d< float > >::getInstance().getNames();
    functorNames.merge( otherFunctorNames );

    // C = gkg::Volume< float >, I = std::vector< float >
    otherFunctorNames =
      gkg::OdfFunctorFactory< gkg::Volume< float >,
                         std::vector< float > >::getInstance().getNames();
    functorNames.merge( otherFunctorNames );

    // C = gkg::Volume< gkg::RGBComponent >, I = gkg::RGBComponent
    otherFunctorNames =
      gkg::OdfFunctorFactory< gkg::Volume< gkg::RGBComponent >,
                            gkg::RGBComponent >::getInstance().getNames();
    functorNames.merge( otherFunctorNames );

    // C = gkg::MeshMap< int32_t, float, 3U >, I = std::vector< float >
    otherFunctorNames =
      gkg::OdfFunctorFactory< gkg::MeshMap< int32_t, float, 3U >,
                         std::vector< float > >::getInstance().getNames();
    functorNames.merge( otherFunctorNames );

    // C = gkg::MeshMap< int32_t, float, 3U >,
    // I = std::vector< std::vector< float > >
    otherFunctorNames =
      gkg::OdfFunctorFactory< gkg::MeshMap< int32_t, float, 3U >,
          std::vector< std::vector< float > > >::getInstance().getNames();
    functorNames.merge( otherFunctorNames );

    // C = gkg::TextureMap< gkg::OrientationDistributionFunction >,
    // I = gkg::OrientationDistributionFunction
    otherFunctorNames =
      gkg::OdfFunctorFactory< gkg::TextureMap<
                                gkg::OrientationDistributionFunction >,
         gkg::OrientationDistributionFunction >::getInstance().getNames();
    functorNames.merge( otherFunctorNames );

    // C = SiteMap< int32_t, int32_t >, I = gkg::Vector3d< int32_t >
    otherFunctorNames =
      gkg::OdfFunctorFactory< gkg::SiteMap< int32_t, int32_t >,
                     gkg::Vector3d< int32_t > >::getInstance().getNames();
    functorNames.merge( otherFunctorNames );

    // C = MeasurementVsModelPlotter, I = MeasurementVsModelAttenuation
    otherFunctorNames =
      gkg::OdfFunctorFactory<
                 gkg::MeasurementVsModelPlotter,
                 gkg::MeasurementVsModelAttenuation >::getInstance().getNames();
    functorNames.merge( otherFunctorNames );

    // C = gkg::TextureMap< std::vector< gkg::Vector3d< float > > >,
    // I = std::vector< gkg::Vector3d< float > >
    otherFunctorNames =
      gkg::OdfFunctorFactory< gkg::TextureMap<
                                std::vector< gkg::Vector3d< float > > >,
         std::vector< gkg::Vector3d< float > > >::getInstance().getNames();
    functorNames.merge( otherFunctorNames );


    return functorNames;

  }
  GKG_CATCH( "std::list< std::string > "
             "gkg::OdfFunctorWriter::getNames()" );

}

std::list< std::string > 
gkg::OdfFunctorWriter::getNamesForSiteTextureFunctorGeneration()
{

  try
  {

    std::list< std::string > functorNames;

    // C = gkg::Volume< float >, I = float
    std::list< std::string > otherFunctorNames =
      gkg::OdfFunctorFactory< gkg::Volume< float >,
                                        float >::getInstance().getNames();
    functorNames.merge( otherFunctorNames );

    // C = gkg::Volume< gkg::Vector3d< float > >, I = gkg::Vector3d< float >
    otherFunctorNames =
      gkg::OdfFunctorFactory< gkg::Volume< gkg::Vector3d< float > >,
                       gkg::Vector3d< float > >::getInstance().getNames();
    functorNames.merge( otherFunctorNames );

    // C = gkg::Volume< float >, I = std::vector< float >
    otherFunctorNames =
      gkg::OdfFunctorFactory< gkg::Volume< float >,
                         std::vector< float > >::getInstance().getNames();
    functorNames.merge( otherFunctorNames );

    // C = gkg::Volume< gkg::RGBComponent >, I = gkg::RGBComponent
    otherFunctorNames =
      gkg::OdfFunctorFactory< gkg::Volume< gkg::RGBComponent >,
                            gkg::RGBComponent >::getInstance().getNames();
    functorNames.merge( otherFunctorNames );

    // C = gkg::MeshMap< int32_t, float, 3U >, I = std::vector< float >
    otherFunctorNames =
      gkg::OdfFunctorFactory< gkg::MeshMap< int32_t, float, 3U >,
                         std::vector< float > >::getInstance().getNames();
    functorNames.merge( otherFunctorNames );

    // C = gkg::MeshMap< int32_t, float, 3U >,
    // I = std::vector< std::vector< float > >
    otherFunctorNames =
      gkg::OdfFunctorFactory< gkg::MeshMap< int32_t, float, 3U >,
          std::vector< std::vector< float > > >::getInstance().getNames();
    functorNames.merge( otherFunctorNames );

    // C = gkg::TextureMap< std::vector< gkg::Vector3d< float > > >,
    // I = std::vector< gkg::Vector3d< float > >
    otherFunctorNames =
      gkg::OdfFunctorFactory< 
              gkg::TextureMap< std::vector< gkg::Vector3d< float > > >,
              std::vector< gkg::Vector3d< float > > >::getInstance().getNames();
    functorNames.merge( otherFunctorNames );


    return functorNames;

  }
  GKG_CATCH( "std::list< std::string > "
             "gkg::OdfFunctorWriter::getNames()" );

}


void gkg::OdfFunctorWriter::write( 
                      const std::list< std::pair< std::string, std::string > >&
                                                        odfFunctorAndFileNames,
                      gkg::OdfCartesianField& odfCartesianField )
{

  try
  {

    std::list< std::pair< std::string,
                          std::string > >::const_iterator
      n = odfFunctorAndFileNames.begin(),
      ne = odfFunctorAndFileNames.end();
    while ( n != ne )
    {

      // C = gkg::Volume< float >, I = float
      if ( gkg::OdfFunctorFactory< gkg::Volume< float >,
                             float >::getInstance().hasOdfFunctor( n->first ) )
      {

        _stringParameters[ 3 ] = n->second;
        gkg::Volume< float > output;
        gkg::OdfFunctorFactory< gkg::Volume< float >, float >::getInstance().
          getOdfFunctor( n->first ).process( output,
                                             odfCartesianField,
                                             _scalarParameters,
                                             _stringParameters,
                                             _verbose );
        if ( _verbose )
        {

          std::cout << "writing " << n->first << " '" << n->second << "' : "
                    << std::flush;

        }
        gkg::Writer::getInstance().write( n->second,
                                          output,
                                          _ascii,
                                          _volumeFormat );
        if ( _verbose )
        {

          std::cout << "done" << std::endl;

        }

      }
      // C = gkg::Volume< gkg::Vector3d< float > >, I = gkg::Vector3d< float >
      else if ( gkg::OdfFunctorFactory< gkg::Volume< gkg::Vector3d< float > >,
            gkg::Vector3d< float > >::getInstance().hasOdfFunctor( n->first ) )
      {

        _stringParameters[ 3 ] = n->second;
        gkg::Volume< gkg::Vector3d< float > > output;
        gkg::OdfFunctorFactory< gkg::Volume< gkg::Vector3d< float > >,
                                       gkg::Vector3d< float > >::getInstance().
          getOdfFunctor( n->first ).process( output,
                                             odfCartesianField,
                                             _scalarParameters,
                                             _stringParameters,
                                             _verbose );
        if ( _verbose )
        {

          std::cout << "writing " << n->first << " '" << n->second << "' : "
                    << std::flush;

        }
        gkg::Writer::getInstance().write( n->second,
                                          output,
                                          _ascii,
                                          _volumeFormat );
        if ( _verbose )
        {

          std::cout << "done" << std::endl;

        }

      }
      // C = gkg::Volume< float >, I = std::vector< float >
      else if ( gkg::OdfFunctorFactory< gkg::Volume< float >,
              std::vector< float > >::getInstance().hasOdfFunctor( n->first ) )
      {

        _stringParameters[ 3 ] = n->second;
        gkg::Volume< float > output;
        gkg::OdfFunctorFactory< gkg::Volume< float >,
                                         std::vector< float > >::getInstance().
          getOdfFunctor( n->first ).process( output,
                                             odfCartesianField,
                                             _scalarParameters,
                                             _stringParameters,
                                             _verbose );
        if ( _verbose )
        {

          std::cout << "writing " << n->first << " '" << n->second << "' : "
                    << std::flush;

        }
        gkg::Writer::getInstance().write( n->second,
                                          output,
                                          _ascii,
                                          _volumeFormat );
        if ( _verbose )
        {

          std::cout << "done" << std::endl;

        }

      }
      // C = gkg::Volume< gkg::RGBComponent >, I = gkg::RGBComponent
      else if ( gkg::OdfFunctorFactory< gkg::Volume< gkg::RGBComponent >,
                 gkg::RGBComponent >::getInstance().hasOdfFunctor( n->first ) )
      {

        _stringParameters[ 3 ] = n->second;
        gkg::Volume< gkg::RGBComponent > output;
        gkg::OdfFunctorFactory< gkg::Volume< gkg::RGBComponent >,
                                            gkg::RGBComponent >::getInstance().
          getOdfFunctor( n->first ).process( output,
                                             odfCartesianField,
                                             _scalarParameters,
                                             _stringParameters,
                                             _verbose );
        if ( _verbose )
        {

          std::cout << "writing " << n->first << " '" << n->second << "' : "
                    << std::flush;

        }
        gkg::Writer::getInstance().write( n->second,
                                          output,
                                          _ascii,
                                          _volumeFormat );
        if ( _verbose )
        {

          std::cout << "done" << std::endl;

        }

      }
      // C = gkg::MeshMap< int32_t, float, 3U >, I = std::vector< float >
      else if ( gkg::OdfFunctorFactory< gkg::MeshMap< int32_t, float, 3U >,
              std::vector< float > >::getInstance().hasOdfFunctor( n->first ) )
      {

        _stringParameters[ 3 ] = n->second;
        gkg::MeshMap< int32_t, float, 3U > output;
        gkg::OdfFunctorFactory< gkg::MeshMap< int32_t, float, 3U >,
                                         std::vector< float > >::getInstance().
          getOdfFunctor( n->first ).process( output,
                                             odfCartesianField,
                                             _scalarParameters,
                                             _stringParameters,
                                             _verbose );
        if ( _verbose )
        {

          std::cout << "writing " << n->first << " '" << n->second << "' : "
                    << std::flush;

        }
        gkg::Writer::getInstance().write( n->second,
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
      else if ( gkg::OdfFunctorFactory< gkg::MeshMap< int32_t, float, 3U >,
                      std::vector< std::vector< float > > >::getInstance().
                                                hasOdfFunctor( n->first ) )
      {

        _stringParameters[ 3 ] = n->second;
        gkg::MeshMap< int32_t, float, 3U > output;
        gkg::OdfFunctorFactory< gkg::MeshMap< int32_t, float, 3U >,
                      std::vector< std::vector< float > > >::getInstance().
          getOdfFunctor( n->first ).process( output,
                                             odfCartesianField,
                                             _scalarParameters,
                                             _stringParameters,
                                             _verbose );
        if ( _verbose )
        {

          std::cout << "writing " << n->first << " '" << n->second << "' : "
                    << std::flush;

        }
        gkg::Writer::getInstance().write( n->second,
                                          output,
                                          _ascii,
                                          _meshMapFormat );
        if ( _verbose )
        {

          std::cout << "done" << std::endl;

        }

      }
      // C = gkg::TextureMap< gkg::OrientationDistributionFunction >,
      // I = gkg::OrientationDistributionFunction
      else if ( gkg::OdfFunctorFactory< gkg::TextureMap<
                                gkg::OrientationDistributionFunction >,
                         gkg::OrientationDistributionFunction >::getInstance().
                                                    hasOdfFunctor( n->first ) )
      {

        _stringParameters[ 3 ] = n->second;
        gkg::TextureMap< gkg::OrientationDistributionFunction > output;
        gkg::OdfFunctorFactory< gkg::TextureMap<
                                gkg::OrientationDistributionFunction >,
                         gkg::OrientationDistributionFunction >::getInstance().
          getOdfFunctor( n->first ).process( output,
                                             odfCartesianField,
                                             _scalarParameters,
                                             _stringParameters,
                                             _verbose );
        if ( _verbose )
        {

          std::cout << "writing " << n->first << " '" << n->second << "' : "
                    << std::flush;

        }
        gkg::Writer::getInstance().write( n->second,
                                          output,
                                          _ascii,
                                          _textureMapFormat );
        if ( _verbose )
        {

          std::cout << "done" << std::endl;

        }

      }
      // C = SiteMap< int32_t, int32_t >, I = gkg::Vector3d< int32_t >
      else if ( gkg::OdfFunctorFactory< gkg::SiteMap< int32_t, int32_t >,
                                     gkg::Vector3d< int32_t > >::getInstance().
                                                    hasOdfFunctor( n->first ) )
      {

        _stringParameters[ 3 ] = n->second;
        gkg::SiteMap< int32_t, int32_t > output;
        gkg::OdfFunctorFactory< gkg::SiteMap< int32_t, int32_t >,
                                      gkg::Vector3d< int32_t > >::getInstance().
          getOdfFunctor( n->first ).process( output,
                                             odfCartesianField,
                                             _scalarParameters,
                                             _stringParameters,
                                             _verbose );
        if ( _verbose )
        {

          std::cout << "writing " << n->first << " '" << n->second << "' : "
                    << std::flush;

        }
        gkg::Writer::getInstance().write( n->second,
                                          output,
                                          _ascii );
        if ( _verbose )
        {

          std::cout << "done" << std::endl;

        }

      }
      // C = MeasurementVsModelPlotter, I = MeasurementVsModelAttenuation
      else if ( gkg::OdfFunctorFactory<
                            gkg::MeasurementVsModelPlotter,
                            gkg::MeasurementVsModelAttenuation >::getInstance().
                                                    hasOdfFunctor( n->first ) )
      {

        _stringParameters[ 3 ] = n->second;
        gkg::MeasurementVsModelPlotter output;
        gkg::OdfFunctorFactory<
                            gkg::MeasurementVsModelPlotter,
                            gkg::MeasurementVsModelAttenuation >::getInstance().
          getOdfFunctor( n->first ).process( output,
                                             odfCartesianField,
                                             _scalarParameters,
                                             _stringParameters,
                                             _verbose );
        if ( _verbose )
        {

          std::cout << "writing " << n->first << " '" << n->second << "' : "
                    << std::flush;

        }
        output.write( n->second );
        if ( _verbose )
        {

          std::cout << "done" << std::endl;

        }

      }
      // C = gkg::TextureMap< std::vector< gkg::Vector3d< float > > >,
      // I = std::vector< gkg::Vector3d< float > >
      else if ( gkg::OdfFunctorFactory< gkg::TextureMap<
                                std::vector< gkg::Vector3d< float > > >,
                         std::vector< gkg::Vector3d< float > > >::getInstance().
                                                    hasOdfFunctor( n->first ) )
      {

        _stringParameters[ 3 ] = n->second;
        gkg::TextureMap< std::vector< gkg::Vector3d< float > > > output;
        gkg::OdfFunctorFactory< gkg::TextureMap<
                                std::vector< gkg::Vector3d< float > > >,
                         std::vector< gkg::Vector3d< float > > >::getInstance().
          getOdfFunctor( n->first ).process( output,
                                             odfCartesianField,
                                             _scalarParameters,
                                             _stringParameters,
                                             _verbose );
        if ( _verbose )
        {

          std::cout << "writing " << n->first << " '" << n->second << "' : "
                    << std::flush;

        }
        gkg::Writer::getInstance().write( n->second,
                                          output,
                                          _ascii,
                                          _textureMapFormat );
        if ( _verbose )
        {

          std::cout << "done" << std::endl;

        }

      }

      ++ n;

    }

  }
  GKG_CATCH( "void gkg::OdfFunctorWriter::write( "
             "const std::list< std::pair< std::string, std::string > >& "
             "odfFunctorAndFileNames ) const" );

}


void gkg::OdfFunctorWriter::splitNames(
    const std::list< std::string >& odfFunctorNames,
    const std::list< std::string >& fileNames,
    std::list< std::pair< std::string,
                          std::string > >& namesNotRequiringStandardBasis,
    std::list< std::pair< std::string,
                          std::string > >& namesRequiringStandardBasis )
{

  try
  {

    std::list< std::string >::const_iterator
      n = odfFunctorNames.begin(),
      ne = odfFunctorNames.end();
    std::list< std::string >::const_iterator
      f = fileNames.begin();
    while ( n != ne )
    {

      // C = gkg::Volume< float >, I = float
      if ( gkg::OdfFunctorFactory< gkg::Volume< float >,
                                   float >::getInstance().hasOdfFunctor( *n ) )
      {

        if ( gkg::OdfFunctorFactory< gkg::Volume< float >,
                                   float >::getInstance().getOdfFunctor( *n ).
             isStandardBasisRequired() )
        {

          namesRequiringStandardBasis.push_back(
                             std::pair< std::string, std::string >( *n, *f ) );

        }
        else
        {

          namesNotRequiringStandardBasis.push_back(
                             std::pair< std::string, std::string >( *n, *f ) );

        }

      }
      // C = gkg::Volume< gkg::Vector3d< float > >, I = gkg::Vector3d< float >
      else if ( gkg::OdfFunctorFactory< gkg::Volume< gkg::Vector3d< float > >,
                  gkg::Vector3d< float > >::getInstance().hasOdfFunctor( *n ) )
      {

        if ( gkg::OdfFunctorFactory< gkg::Volume< gkg::Vector3d< float > >,
                  gkg::Vector3d< float > >::getInstance().getOdfFunctor( *n ).
             isStandardBasisRequired() )
        {

          namesRequiringStandardBasis.push_back(
                             std::pair< std::string, std::string >( *n, *f ) );

        }
        else
        {

          namesNotRequiringStandardBasis.push_back(
                             std::pair< std::string, std::string >( *n, *f ) );

        }

      }
      // C = gkg::Volume< float >, I = std::vector< float >
      else if ( gkg::OdfFunctorFactory< gkg::Volume< float >,
                    std::vector< float > >::getInstance().hasOdfFunctor( *n ) )
      {

        if ( gkg::OdfFunctorFactory< gkg::Volume< float >,
                    std::vector< float > >::getInstance().getOdfFunctor( *n ).
             isStandardBasisRequired() )
        {

          namesRequiringStandardBasis.push_back(
                             std::pair< std::string, std::string >( *n, *f ) );

        }
        else
        {

          namesNotRequiringStandardBasis.push_back(
                             std::pair< std::string, std::string >( *n, *f ) );

        }

      }
      // C = gkg::Volume< gkg::RGBComponent >, I = gkg::RGBComponent
      else if ( gkg::OdfFunctorFactory< gkg::Volume< gkg::RGBComponent >,
                       gkg::RGBComponent >::getInstance().hasOdfFunctor( *n ) )
      {

        if ( gkg::OdfFunctorFactory< gkg::Volume< gkg::RGBComponent >,
                       gkg::RGBComponent >::getInstance().getOdfFunctor( *n ).
             isStandardBasisRequired() )
        {

          namesRequiringStandardBasis.push_back(
                             std::pair< std::string, std::string >( *n, *f ) );

        }
        else
        {

          namesNotRequiringStandardBasis.push_back(
                             std::pair< std::string, std::string >( *n, *f ) );

        }

      }
      // C = gkg::MeshMap< int32_t, float, 3U >, I = std::vector< float >
      else if ( gkg::OdfFunctorFactory< gkg::MeshMap< int32_t, float, 3U >,
                    std::vector< float > >::getInstance().hasOdfFunctor( *n ) )
      {

        if ( gkg::OdfFunctorFactory< gkg::MeshMap< int32_t, float, 3U >,
                    std::vector< float > >::getInstance().getOdfFunctor( *n ).
             isStandardBasisRequired() )
        {

          namesRequiringStandardBasis.push_back(
                             std::pair< std::string, std::string >( *n, *f ) );

        }
        else
        {

          namesNotRequiringStandardBasis.push_back(
                             std::pair< std::string, std::string >( *n, *f ) );

        }

      }
      // C = gkg::MeshMap< int32_t, float, 3U >,
      // I = std::vector< std::vector< float > >
      else if ( gkg::OdfFunctorFactory< gkg::MeshMap< int32_t, float, 3U >,
                      std::vector< std::vector< float > > >::getInstance().
                                                      hasOdfFunctor( *n ) )
      {

        if ( gkg::OdfFunctorFactory< gkg::MeshMap< int32_t, float, 3U >,
                    std::vector< std::vector< float > > >::getInstance().
                                                     getOdfFunctor( *n ).
             isStandardBasisRequired() )
        {

          namesRequiringStandardBasis.push_back(
                             std::pair< std::string, std::string >( *n, *f ) );

        }
        else
        {

          namesNotRequiringStandardBasis.push_back(
                             std::pair< std::string, std::string >( *n, *f ) );

        }

      }
      // C = gkg::TextureMap< gkg::OrientationDistributionFunction >,
      // I = gkg::OrientationDistributionFunction
      else if ( gkg::OdfFunctorFactory<
                  gkg::TextureMap< gkg::OrientationDistributionFunction >,
                  gkg::OrientationDistributionFunction >::getInstance().
                                                          hasOdfFunctor( *n ) )
      {

        if ( gkg::OdfFunctorFactory< gkg::TextureMap<
                                       gkg::OrientationDistributionFunction >,
                                       gkg::OrientationDistributionFunction >::
                                          getInstance().getOdfFunctor( *n ).
             isStandardBasisRequired() )
        {

          namesRequiringStandardBasis.push_back(
                             std::pair< std::string, std::string >( *n, *f ) );

        }
        else
        {

          namesNotRequiringStandardBasis.push_back(
                             std::pair< std::string, std::string >( *n, *f ) );

        }

      }
      // C = SiteMap< int32_t, int32_t >, I = gkg::Vector3d< int32_t >
      else if ( gkg::OdfFunctorFactory<
                  gkg::SiteMap< int32_t, int32_t >,
                  gkg::Vector3d< int32_t > >::getInstance().
                                                          hasOdfFunctor( *n ) )
      {

        if ( gkg::OdfFunctorFactory< gkg::SiteMap< int32_t, int32_t >,
                                     gkg::Vector3d< int32_t > >::
                                          getInstance().getOdfFunctor( *n ).
             isStandardBasisRequired() )
        {

          namesRequiringStandardBasis.push_back(
                             std::pair< std::string, std::string >( *n, *f ) );

        }
        else
        {

          namesNotRequiringStandardBasis.push_back(
                             std::pair< std::string, std::string >( *n, *f ) );

        }

      }
      // C = MeasurementVsModelPlotter, I = MeasurementVsModelAttenuation
      else if ( gkg::OdfFunctorFactory<
                  gkg::MeasurementVsModelPlotter,
                  gkg::MeasurementVsModelAttenuation >::getInstance().
                                                          hasOdfFunctor( *n ) )
      {

        if ( gkg::OdfFunctorFactory< gkg::MeasurementVsModelPlotter,
                                     gkg::MeasurementVsModelAttenuation >::
                                          getInstance().getOdfFunctor( *n ).
             isStandardBasisRequired() )
        {

          namesRequiringStandardBasis.push_back(
                             std::pair< std::string, std::string >( *n, *f ) );

        }
        else
        {

          namesNotRequiringStandardBasis.push_back(
                             std::pair< std::string, std::string >( *n, *f ) );

        }

      }
      // C = gkg::TextureMap< std::vector< gkg::Vector3d< float > > >,
      // I = std::vector< gkg::Vector3d< float > >
      else if ( gkg::OdfFunctorFactory<
                  gkg::TextureMap< std::vector< gkg::Vector3d< float > > >,
                  std::vector< gkg::Vector3d< float > > >::getInstance().
                                                          hasOdfFunctor( *n ) )
      {

        if ( gkg::OdfFunctorFactory< gkg::TextureMap<
                                       std::vector< gkg::Vector3d< float > > >,
                                       std::vector< gkg::Vector3d< float > > >::
                                          getInstance().getOdfFunctor( *n ).
             isStandardBasisRequired() )
        {

          namesRequiringStandardBasis.push_back(
                             std::pair< std::string, std::string >( *n, *f ) );

        }
        else
        {

          namesNotRequiringStandardBasis.push_back(
                             std::pair< std::string, std::string >( *n, *f ) );

        }

      }

      ++ n;
      ++ f;

    }

  }
  GKG_CATCH( "void gkg::OdfFunctorWriter::splitNames( "
             "const std::list< std::string >& odfFunctorNames, "
             "const std::list< std::string >& fileNames, "
             "std::list< std::pair< std::string, "
             "std::string > >& namesNotRequiringStandardBasis, "
             "std::list< std::pair< std::string, "
             "std::string > >& namesRequiringStandardBasis )" );

}


