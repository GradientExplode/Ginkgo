#include <gkg-dmri-io/PdfFunctorWriter.h>
#include <gkg-dmri-pdf-functor/PdfFunctorFactory_i.h>
#include <gkg-dmri-io/DefaultAsciiItemWriterOfPdf_i.h>
#include <gkg-dmri-io/DefaultBinaryItemWriterOfPdf_i.h>
#include <gkg-dmri-io/DefaultBSwapItemWriterOfPdf_i.h>
#include <gkg-dmri-io/DefaultAsciiItemReaderOfPdf_i.h>
#include <gkg-dmri-io/DefaultBinaryItemReaderOfPdf_i.h>
#include <gkg-dmri-io/DefaultBSwapItemReaderOfPdf_i.h>
#include <gkg-dmri-io/DefaultAsciiItemWriterOfOdf_i.h>
#include <gkg-dmri-io/DefaultBinaryItemWriterOfOdf_i.h>
#include <gkg-dmri-io/DefaultBSwapItemWriterOfOdf_i.h>
#include <gkg-dmri-io/DefaultAsciiItemReaderOfOdf_i.h>
#include <gkg-dmri-io/DefaultBinaryItemReaderOfOdf_i.h>
#include <gkg-dmri-io/DefaultBSwapItemReaderOfOdf_i.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-container/MeshMap_i.h>
#include <gkg-processing-container/TextureMap_i.h>
#include <gkg-processing-io/TextureMapAnalyzer_i.h>
#include <gkg-processing-io/TextureMapDiskFormat_i.h>
#include <gkg-processing-container/SiteMap_i.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-processing-colorimetry/RGBComponent.h>
#include <gkg-dmri-microstructure-plot/MeasurementVsModelPlotter.h>
#include <gkg-dmri-pdf/ProbabilityDensityFunction.h>
#include <gkg-dmri-odf/OrientationDistributionFunction.h>
#include <gkg-core-io/TypeOf.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-exception/Exception.h>
#include <iostream>
#include <utility>


////////////////////////////////////////////////////////////////////////////////
// creating disk format for TextureMap< ProbabilityDensityFunction >
////////////////////////////////////////////////////////////////////////////////

template class gkg::TextureMapAnalyzer< gkg::ProbabilityDensityFunction >;
template class gkg::DiskFormatFactory<
                     gkg::TextureMap< gkg::ProbabilityDensityFunction > >;
template class gkg::TextureMapDiskFormat<
                                        gkg::ProbabilityDensityFunction >;

RegisterTextureAnalyzer( gkg::ProbabilityDensityFunction, gkg_pdf );
RegisterTextureDiskFormat( gkg::ProbabilityDensityFunction, gkg_pdf );


template class gkg::TextureMapAnalyzer< gkg::OrientationDistributionFunction >;
template class gkg::DiskFormatFactory<
                     gkg::TextureMap< gkg::OrientationDistributionFunction > >;
template class gkg::TextureMapDiskFormat<
                                        gkg::OrientationDistributionFunction >;

RegisterTextureAnalyzer( gkg::OrientationDistributionFunction, gkg_odf );
RegisterTextureDiskFormat( gkg::OrientationDistributionFunction, gkg_odf );


gkg::PdfFunctorWriter::PdfFunctorWriter(
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
  GKG_CATCH( "gkg::PdfFunctorWriter::PdfFunctorWriter( float rgbScale, "
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


gkg::PdfFunctorWriter::~PdfFunctorWriter()
{
}


void gkg::PdfFunctorWriter::write(
                             const std::list< std::string >& pdfFunctorNames,
                             const std::list< std::string >& fileNames,
                             gkg::PdfCartesianField& pdfCartesianField )
{

  try
  {


    // splitting PDF functor name(s) between those not requiring to be in the
    // standard basis and those requiring to be in the standard basis
    std::list< std::pair< std::string, std::string > > 
      namesNotRequiringStandardBasis;
    std::list< std::pair< std::string, std::string > > 
      namesRequiringStandardBasis;
    splitNames( pdfFunctorNames, fileNames,
                namesNotRequiringStandardBasis, namesRequiringStandardBasis );

    // first, processing map(s) that do not require converting the PDF
    // map to the standard basis
    write( namesNotRequiringStandardBasis, pdfCartesianField );

    if ( !namesRequiringStandardBasis.empty() )
    {

      // transforming PDF texture map to standard basis
      if ( pdfCartesianField.getBasisType() != 
           gkg::ProbabilityDensityFunction::Standard )
      {

        pdfCartesianField.setStandardBasis( _verbose );

      }

      // second, processing map(s) that require converting the PDF
      // map to the standard basis
      write( namesRequiringStandardBasis, pdfCartesianField );

    }

  }
  GKG_CATCH( "void gkg::PdfFunctorWriter::write( "
             "const std::list< std::string >& pdfFunctorNames, "
             "const std::list< std::string >& fileNames, "
             "gkg::PdfCartesianField& pdfCartesianField ) const" );

}


std::list< std::string > gkg::PdfFunctorWriter::getNames()
{

  try
  {

    std::list< std::string > functorNames;

    // C = gkg::Volume< float >, I = float
    std::list< std::string > otherFunctorNames =
      gkg::PdfFunctorFactory<
        gkg::Volume< float >,
        float >::getInstance().getNames();
    functorNames.merge( otherFunctorNames );

    // C = gkg::Volume< gkg::Vector3d< float > >, I = gkg::Vector3d< float >
    otherFunctorNames =
      gkg::PdfFunctorFactory<
        gkg::Volume< gkg::Vector3d< float > >,
        gkg::Vector3d< float > >::getInstance().getNames();
    functorNames.merge( otherFunctorNames );

    // C = gkg::Volume< float >, I = std::vector< float >
    otherFunctorNames =
      gkg::PdfFunctorFactory<
        gkg::Volume< float >,
        std::vector< float > >::getInstance().getNames();
    functorNames.merge( otherFunctorNames );

    // C = gkg::Volume< gkg::RGBComponent >, I = gkg::RGBComponent
    otherFunctorNames =
      gkg::PdfFunctorFactory<
        gkg::Volume< gkg::RGBComponent >,
        gkg::RGBComponent >::getInstance().getNames();
    functorNames.merge( otherFunctorNames );

    // C = gkg::MeshMap< int32_t, float, 3U >, I = std::vector< float >
    otherFunctorNames =
      gkg::PdfFunctorFactory<
        gkg::MeshMap< int32_t, float, 3U >,
        std::vector< float > >::getInstance().getNames();
    functorNames.merge( otherFunctorNames );

    // C = gkg::MeshMap< int32_t, float, 3U >,
    // I = std::vector< std::vector< float > >
    otherFunctorNames =
      gkg::PdfFunctorFactory<
        gkg::MeshMap< int32_t, float, 3U >,
        std::vector< std::vector< float > > >::getInstance().getNames();
    functorNames.merge( otherFunctorNames );

    // C = gkg::TextureMap< gkg::ProbabilityDensityFunction >,
    // I = gkg::ProbabilityDensityFunction
    otherFunctorNames =
      gkg::PdfFunctorFactory<
        gkg::TextureMap< gkg::ProbabilityDensityFunction >,
        gkg::ProbabilityDensityFunction >::getInstance().getNames();
    functorNames.merge( otherFunctorNames );

    // C = gkg::TextureMap< gkg::OrientationDistributionFunction >,
    // I = gkg::OrientationDistributionFunction
    otherFunctorNames =
      gkg::PdfFunctorFactory<
        gkg::TextureMap< gkg::OrientationDistributionFunction >,
        gkg::OrientationDistributionFunction >::getInstance().getNames();
    functorNames.merge( otherFunctorNames );

    // C = SiteMap< int32_t, int32_t >, I = gkg::Vector3d< int32_t >
    otherFunctorNames =
      gkg::PdfFunctorFactory<
        gkg::SiteMap< int32_t, int32_t >,
        gkg::Vector3d< int32_t > >::getInstance().getNames();
    functorNames.merge( otherFunctorNames );

    // C = MeasurementVsModelPlotter, I = MeasurementVsModelAttenuation
    otherFunctorNames =
      gkg::PdfFunctorFactory<
        gkg::MeasurementVsModelPlotter,
        gkg::MeasurementVsModelAttenuation >::getInstance().getNames();
    functorNames.merge( otherFunctorNames );


    return functorNames;

  }
  GKG_CATCH( "std::list< std::string > "
             "gkg::PdfFunctorWriter::getNames()" );

}

std::list< std::string > 
gkg::PdfFunctorWriter::getNamesForSiteTextureFunctorGeneration()
{

  try
  {

    std::list< std::string > functorNames;

    // C = gkg::Volume< float >, I = float
    std::list< std::string > otherFunctorNames =
      gkg::PdfFunctorFactory<
        gkg::Volume< float >,
       float >::getInstance().getNames();
    functorNames.merge( otherFunctorNames );

    // C = gkg::Volume< gkg::Vector3d< float > >, I = gkg::Vector3d< float >
    otherFunctorNames =
      gkg::PdfFunctorFactory<
        gkg::Volume< gkg::Vector3d< float > >,
        gkg::Vector3d< float > >::getInstance().getNames();
    functorNames.merge( otherFunctorNames );

    // C = gkg::Volume< float >, I = std::vector< float >
    otherFunctorNames =
      gkg::PdfFunctorFactory<
        gkg::Volume< float >,
        std::vector< float > >::getInstance().getNames();
    functorNames.merge( otherFunctorNames );

    // C = gkg::Volume< gkg::RGBComponent >, I = gkg::RGBComponent
    otherFunctorNames =
      gkg::PdfFunctorFactory<
        gkg::Volume< gkg::RGBComponent >,
        gkg::RGBComponent >::getInstance().getNames();
    functorNames.merge( otherFunctorNames );

    // C = gkg::MeshMap< int32_t, float, 3U >, I = std::vector< float >
    otherFunctorNames =
      gkg::PdfFunctorFactory<
        gkg::MeshMap< int32_t, float, 3U >,
        std::vector< float > >::getInstance().getNames();
    functorNames.merge( otherFunctorNames );

    // C = gkg::MeshMap< int32_t, float, 3U >,
    // I = std::vector< std::vector< float > >
    otherFunctorNames =
      gkg::PdfFunctorFactory<
        gkg::MeshMap< int32_t, float, 3U >,
        std::vector< std::vector< float > > >::getInstance().getNames();
    functorNames.merge( otherFunctorNames );

    return functorNames;

  }
  GKG_CATCH( "std::list< std::string > "
             "gkg::PdfFunctorWriter::getNames()" );

}


void gkg::PdfFunctorWriter::write( 
                      const std::list< std::pair< std::string, std::string > >&
                                                        pdfFunctorAndFileNames,
                      gkg::PdfCartesianField& pdfCartesianField )
{

  try
  {

    std::list< std::pair< std::string,
                          std::string > >::const_iterator
      n = pdfFunctorAndFileNames.begin(),
      ne = pdfFunctorAndFileNames.end();
    while ( n != ne )
    {

      // C = gkg::Volume< float >, I = float
      if ( gkg::PdfFunctorFactory< gkg::Volume< float >,
                             float >::getInstance().hasPdfFunctor( n->first ) )
      {

        _stringParameters[ 3 ] = n->second;
        gkg::Volume< float > output;
        gkg::PdfFunctorFactory< gkg::Volume< float >, float >::getInstance().
          getPdfFunctor( n->first ).process( output,
                                             pdfCartesianField,
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
      else if ( gkg::PdfFunctorFactory< gkg::Volume< gkg::Vector3d< float > >,
            gkg::Vector3d< float > >::getInstance().hasPdfFunctor( n->first ) )
      {

        _stringParameters[ 3 ] = n->second;
        gkg::Volume< gkg::Vector3d< float > > output;
        gkg::PdfFunctorFactory< gkg::Volume< gkg::Vector3d< float > >,
                                       gkg::Vector3d< float > >::getInstance().
          getPdfFunctor( n->first ).process( output,
                                             pdfCartesianField,
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
      else if ( gkg::PdfFunctorFactory< gkg::Volume< float >,
              std::vector< float > >::getInstance().hasPdfFunctor( n->first ) )
      {

        _stringParameters[ 3 ] = n->second;
        gkg::Volume< float > output;
        gkg::PdfFunctorFactory< gkg::Volume< float >,
                                         std::vector< float > >::getInstance().
          getPdfFunctor( n->first ).process( output,
                                             pdfCartesianField,
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
      else if ( gkg::PdfFunctorFactory< gkg::Volume< gkg::RGBComponent >,
                 gkg::RGBComponent >::getInstance().hasPdfFunctor( n->first ) )
      {

        _stringParameters[ 3 ] = n->second;
        gkg::Volume< gkg::RGBComponent > output;
        gkg::PdfFunctorFactory< gkg::Volume< gkg::RGBComponent >,
                                            gkg::RGBComponent >::getInstance().
          getPdfFunctor( n->first ).process( output,
                                             pdfCartesianField,
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
      else if ( gkg::PdfFunctorFactory< gkg::MeshMap< int32_t, float, 3U >,
              std::vector< float > >::getInstance().hasPdfFunctor( n->first ) )
      {

        _stringParameters[ 3 ] = n->second;
        gkg::MeshMap< int32_t, float, 3U > output;
        gkg::PdfFunctorFactory< gkg::MeshMap< int32_t, float, 3U >,
                                         std::vector< float > >::getInstance().
          getPdfFunctor( n->first ).process( output,
                                             pdfCartesianField,
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
      else if ( gkg::PdfFunctorFactory< gkg::MeshMap< int32_t, float, 3U >,
                      std::vector< std::vector< float > > >::getInstance().
                                                hasPdfFunctor( n->first ) )
      {

        _stringParameters[ 3 ] = n->second;
        gkg::MeshMap< int32_t, float, 3U > output;
        gkg::PdfFunctorFactory< gkg::MeshMap< int32_t, float, 3U >,
                      std::vector< std::vector< float > > >::getInstance().
          getPdfFunctor( n->first ).process( output,
                                             pdfCartesianField,
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
      // C = gkg::TextureMap< gkg::ProbabilityDensityFunction >,
      // I = gkg::ProbabilityDensityFunction
      else if ( gkg::PdfFunctorFactory< gkg::TextureMap<
                                gkg::ProbabilityDensityFunction >,
                         gkg::ProbabilityDensityFunction >::getInstance().
                                                    hasPdfFunctor( n->first ) )
      {

        _stringParameters[ 3 ] = n->second;
        gkg::TextureMap< gkg::ProbabilityDensityFunction > output;
        gkg::PdfFunctorFactory< gkg::TextureMap<
                                gkg::ProbabilityDensityFunction >,
                         gkg::ProbabilityDensityFunction >::getInstance().
          getPdfFunctor( n->first ).process( output,
                                             pdfCartesianField,
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
      // C = gkg::TextureMap< gkg::OrientationDistributionFunction >,
      // I = gkg::OrientationDistributionFunction
      else if ( gkg::PdfFunctorFactory< gkg::TextureMap<
                                gkg::OrientationDistributionFunction >,
                         gkg::OrientationDistributionFunction >::getInstance().
                                                    hasPdfFunctor( n->first ) )
      {

        _stringParameters[ 3 ] = n->second;
        gkg::TextureMap< gkg::OrientationDistributionFunction > output;
        gkg::PdfFunctorFactory< gkg::TextureMap<
                                gkg::OrientationDistributionFunction >,
                         gkg::OrientationDistributionFunction >::getInstance().
          getPdfFunctor( n->first ).process( output,
                                             pdfCartesianField,
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
      else if ( gkg::PdfFunctorFactory< gkg::SiteMap< int32_t, int32_t >,
                                     gkg::Vector3d< int32_t > >::getInstance().
                                                    hasPdfFunctor( n->first ) )
      {

        _stringParameters[ 3 ] = n->second;
        gkg::SiteMap< int32_t, int32_t > output;
        gkg::PdfFunctorFactory< gkg::SiteMap< int32_t, int32_t >,
                                      gkg::Vector3d< int32_t > >::getInstance().
          getPdfFunctor( n->first ).process( output,
                                             pdfCartesianField,
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
      else if ( gkg::PdfFunctorFactory<
                            gkg::MeasurementVsModelPlotter,
                            gkg::MeasurementVsModelAttenuation >::getInstance().
                                                    hasPdfFunctor( n->first ) )
      {

        _stringParameters[ 3 ] = n->second;
        gkg::MeasurementVsModelPlotter output;
        gkg::PdfFunctorFactory<
                            gkg::MeasurementVsModelPlotter,
                            gkg::MeasurementVsModelAttenuation >::getInstance().
          getPdfFunctor( n->first ).process( output,
                                             pdfCartesianField,
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

      ++ n;

    }

  }
  GKG_CATCH( "void gkg::PdfFunctorWriter::write( "
             "const std::list< std::pair< std::string, std::string > >& "
             "pdfFunctorAndFileNames ) const" );

}


void gkg::PdfFunctorWriter::splitNames(
    const std::list< std::string >& pdfFunctorNames,
    const std::list< std::string >& fileNames,
    std::list< std::pair< std::string,
                          std::string > >& namesNotRequiringStandardBasis,
    std::list< std::pair< std::string,
                          std::string > >& namesRequiringStandardBasis )
{

  try
  {

    std::list< std::string >::const_iterator
      n = pdfFunctorNames.begin(),
      ne = pdfFunctorNames.end();
    std::list< std::string >::const_iterator
      f = fileNames.begin();
    while ( n != ne )
    {

      // C = gkg::Volume< float >, I = float
      if ( gkg::PdfFunctorFactory< gkg::Volume< float >,
                                   float >::getInstance().hasPdfFunctor( *n ) )
      {

        if ( gkg::PdfFunctorFactory< gkg::Volume< float >,
                                   float >::getInstance().getPdfFunctor( *n ).
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
      else if ( gkg::PdfFunctorFactory< gkg::Volume< gkg::Vector3d< float > >,
                  gkg::Vector3d< float > >::getInstance().hasPdfFunctor( *n ) )
      {

        if ( gkg::PdfFunctorFactory< gkg::Volume< gkg::Vector3d< float > >,
                  gkg::Vector3d< float > >::getInstance().getPdfFunctor( *n ).
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
      else if ( gkg::PdfFunctorFactory< gkg::Volume< float >,
                    std::vector< float > >::getInstance().hasPdfFunctor( *n ) )
      {

        if ( gkg::PdfFunctorFactory< gkg::Volume< float >,
                    std::vector< float > >::getInstance().getPdfFunctor( *n ).
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
      else if ( gkg::PdfFunctorFactory< gkg::Volume< gkg::RGBComponent >,
                       gkg::RGBComponent >::getInstance().hasPdfFunctor( *n ) )
      {

        if ( gkg::PdfFunctorFactory< gkg::Volume< gkg::RGBComponent >,
                       gkg::RGBComponent >::getInstance().getPdfFunctor( *n ).
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
      else if ( gkg::PdfFunctorFactory< gkg::MeshMap< int32_t, float, 3U >,
                    std::vector< float > >::getInstance().hasPdfFunctor( *n ) )
      {

        if ( gkg::PdfFunctorFactory< gkg::MeshMap< int32_t, float, 3U >,
                    std::vector< float > >::getInstance().getPdfFunctor( *n ).
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
      else if ( gkg::PdfFunctorFactory< gkg::MeshMap< int32_t, float, 3U >,
                      std::vector< std::vector< float > > >::getInstance().
                                                      hasPdfFunctor( *n ) )
      {

        if ( gkg::PdfFunctorFactory< gkg::MeshMap< int32_t, float, 3U >,
                    std::vector< std::vector< float > > >::getInstance().
                                                     getPdfFunctor( *n ).
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
      // C = gkg::TextureMap< gkg::ProbabilityDensityFunction >,
      // I = gkg::ProbabilityDensityFunction
      else if ( gkg::PdfFunctorFactory<
                  gkg::TextureMap< gkg::ProbabilityDensityFunction >,
                  gkg::ProbabilityDensityFunction >::getInstance().
                                                          hasPdfFunctor( *n ) )
      {

        if ( gkg::PdfFunctorFactory< gkg::TextureMap<
                                       gkg::ProbabilityDensityFunction >,
                                       gkg::ProbabilityDensityFunction >::
                                          getInstance().getPdfFunctor( *n ).
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
      else if ( gkg::PdfFunctorFactory<
                  gkg::TextureMap< gkg::OrientationDistributionFunction >,
                  gkg::OrientationDistributionFunction >::getInstance().
                                                          hasPdfFunctor( *n ) )
      {

        if ( gkg::PdfFunctorFactory< gkg::TextureMap<
                                       gkg::OrientationDistributionFunction >,
                                       gkg::OrientationDistributionFunction >::
                                          getInstance().getPdfFunctor( *n ).
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
      else if ( gkg::PdfFunctorFactory<
                  gkg::SiteMap< int32_t, int32_t >,
                  gkg::Vector3d< int32_t > >::getInstance().
                                                          hasPdfFunctor( *n ) )
      {

        if ( gkg::PdfFunctorFactory< gkg::SiteMap< int32_t, int32_t >,
                                     gkg::Vector3d< int32_t > >::
                                          getInstance().getPdfFunctor( *n ).
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
      else if ( gkg::PdfFunctorFactory<
                  gkg::MeasurementVsModelPlotter,
                  gkg::MeasurementVsModelAttenuation >::getInstance().
                                                          hasPdfFunctor( *n ) )
      {

        if ( gkg::PdfFunctorFactory< gkg::MeasurementVsModelPlotter,
                                     gkg::MeasurementVsModelAttenuation >::
                                          getInstance().getPdfFunctor( *n ).
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
  GKG_CATCH( "void gkg::PdfFunctorWriter::splitNames( "
             "const std::list< std::string >& pdfFunctorNames, "
             "const std::list< std::string >& fileNames, "
             "std::list< std::pair< std::string, "
             "std::string > >& namesNotRequiringStandardBasis, "
             "std::list< std::pair< std::string, "
             "std::string > >& namesRequiringStandardBasis )" );

}


