#include <gkg-dmri-odf-functor/OdfFunctorMeasurementVsModelPlot.h>
#include <gkg-dmri-odf-functor/OdfFunctor_i.h>
#include <gkg-dmri-odf-functor/OdfFunctorFactory_i.h>
#include <gkg-dmri-microstructure-noddi/NoddiMCMCWatsonFunction.h>
#include <gkg-dmri-microstructure-noddi/NoddiMCMCBinghamFunction.h>
#include <gkg-dmri-microstructure-axon-mapping/PGSEAxonMappingMCMCFunction.h>
#include <gkg-dmri-microstructure-axon-mapping/CTOGSEAxonMappingMCMCFunction.h>
#include <gkg-processing-algobase/Converter_i.h>
#include <gkg-processing-transform/IdentityTransform3d.h>
#include <gkg-processing-transform/RigidTransform3d.h>
#include <gkg-processing-transform/AffineTransform3d.h>
#include <gkg-processing-coordinates/ElectrostaticOrientationSet.h>
#include <gkg-dmri-microstructure-model/MicrostructureMCMCFunction.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-pattern/RCPointer.h>
#include <gkg-core-exception/Exception.h>
#include <cmath>
#include <vector>


gkg::OdfFunctorMeasurementVsModelPlot::OdfFunctorMeasurementVsModelPlot()
                       : gkg::OdfFunctor< gkg::MeasurementVsModelPlotter,
                                          gkg::MeasurementVsModelAttenuation >()
{
}


gkg::OdfFunctorMeasurementVsModelPlot::~OdfFunctorMeasurementVsModelPlot()
{
}


std::string gkg::OdfFunctorMeasurementVsModelPlot::getName() const
{

  try
  {

    return "measurement_versus_model_plot";

  }
  GKG_CATCH( "std::string "
             "gkg::OdfFunctorMeasurementVsModelPlot::getName() const" );

}


bool gkg::OdfFunctorMeasurementVsModelPlot::isStandardBasisRequired() const
{

  try
  {

    return false;

  }
  GKG_CATCH( "bool "
             "gkg::OdfFunctorMeasurementVsModelPlot::"
             "isStandardBasisRequired() const" );

}


void gkg::OdfFunctorMeasurementVsModelPlot::process(
                             gkg::MeasurementVsModelPlotter& output,
                             gkg::OdfCartesianField& odfCartesianField,
                             const std::vector< double >& scalarParameters,
                             const std::vector< std::string >& stringParameters,
                             bool verbose ) const
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // sanity check(s)
    ////////////////////////////////////////////////////////////////////////////

    if ( scalarParameters.size() != 8U )
    {

      std::string message = std::string(
                              "missing further scalar parameters; should " ) +
                            "be added:\n" +
                            "- the output orientation count\n" +
                            "- the delta variation of b-value in s/mm2\n" +
                            "- the model orientation count";
      throw std::runtime_error( message );

    }

    if ( stringParameters.size() != 6U )
    {

      std::string message = std::string(
                              "missing further string parameters; should " ) +
                            "be added:\n" +
                            "- the volume mask filename onto which\n" +
                            "  plots will be done\n" +
                            "- the 3D transformation filename from the mask\n" +
                            "  to the ODF field (*.trm)";
      throw std::runtime_error( message );

    }


    ////////////////////////////////////////////////////////////////////////////
    // skipping or applying according to the ODF type
    ////////////////////////////////////////////////////////////////////////////

    if ( ( odfCartesianField.getBasisType() !=
           gkg::OrientationDistributionFunction::NoddiWatson ) &&
         ( odfCartesianField.getBasisType() !=
           gkg::OrientationDistributionFunction::NoddiBingham ) &&
         ( odfCartesianField.getBasisType() !=
           gkg::OrientationDistributionFunction::PGSEAxonMapping ) &&
         ( odfCartesianField.getBasisType() !=
           gkg::OrientationDistributionFunction::CTOGSEAxonMapping ) )
    {

      if ( verbose )
      {

        std::cout << "skipping " << this->getName() << " : " << std::flush;

      }
      return;

    }

    if ( verbose )
    {

      std::cout << "processing " << this->getName() << " : " << std::flush;

    }

    ////////////////////////////////////////////////////////////////////////////
    // reading the mask
    ////////////////////////////////////////////////////////////////////////////

    if ( stringParameters[ 4 ].empty() )
    {

      throw std::runtime_error( "empty mask filename" );

    }
    gkg::Volume< int16_t > volumeMaskInt16;
    try
    {

      gkg::Reader::getInstance().read( stringParameters[ 4 ], volumeMaskInt16 );

    }
    catch( std::exception& )
    {

      try
      {

        gkg::Volume< uint8_t > volumeMaskUInt8;
        gkg::Reader::getInstance().read( stringParameters[ 4 ],
                                         volumeMaskUInt8 );

        gkg::Converter< gkg::Volume< uint8_t >, gkg::Volume< int16_t > >
          converter;
        converter.convert( volumeMaskUInt8, volumeMaskInt16 );

      }
       catch ( std::exception& )
      {

        throw std::runtime_error( "unable to read mask" );

      }

    }
    gkg::Vector3d< double > maskResolution;
    volumeMaskInt16.getResolution( maskResolution );
    gkg::BoundingBox< int32_t > maskBoundingBox( volumeMaskInt16 );


    ////////////////////////////////////////////////////////////////////////////
    // reading the 3D transform from mask to ODF Cartesian field
    ////////////////////////////////////////////////////////////////////////////
    
    if ( stringParameters[ 5 ].empty() )
    {

      throw std::runtime_error( "empty 3D transformation filename" );

    }

    gkg::RCPointer< gkg::Transform3d< float > > transform3dFromMaskToOdf;
    if ( stringParameters[ 5 ] == "id" )
    {

      transform3dFromMaskToOdf.reset( new gkg::IdentityTransform3d< float > );

    }
    else
    {

      try
      {

        gkg::RCPointer< gkg::RigidTransform3d< float > >
          rigidTransform3d( new gkg::RigidTransform3d< float > );
        std::ifstream is( stringParameters[ 5 ].c_str() );
        rigidTransform3d->readTrm( is );
        is.close();
        transform3dFromMaskToOdf = rigidTransform3d;

      }
      catch( std::exception& )
      {

        try
        {

          gkg::RCPointer< gkg::AffineTransform3d< float > >
            affineTransform3d( new gkg::AffineTransform3d< float > );
          std::ifstream is( stringParameters[ 5 ].c_str() );
          affineTransform3d->readTrm( is );
          is.close();
          transform3dFromMaskToOdf = affineTransform3d;

        }
        catch( std::exception& )
        {

          throw std::runtime_error( "invalid 3D mask->ODFs transform" );

        }

      }

    }


    ////////////////////////////////////////////////////////////////////////////
    // getting ODF Cartesian field information(s)
    ////////////////////////////////////////////////////////////////////////////

    // getting site(s)
    const std::list< gkg::Vector3d< int32_t > >&
      sites = odfCartesianField.getSiteMap().getSites( 0 );

    // getting offset
    gkg::Vector3d< int32_t > 
      offset = odfCartesianField.getOffset();

    // collecting input orientation information
    const gkg::OrientationSet&
      inputOrientationSet = odfCartesianField.getInputOrientationSet();
    int32_t inputOrientationCount = inputOrientationSet.getCount();

    // getting pointer to DW image
    const gkg::Volume< float >&
      t2Volume = odfCartesianField.getT2Volume();
    const gkg::Volume< float >&
      dwVolume = odfCartesianField.getDWVolume();

    // getting the Cartesian field size(s)
    int32_t sizeX = odfCartesianField.getSizeX();
    int32_t sizeY = odfCartesianField.getSizeY();
    int32_t sizeZ = odfCartesianField.getSizeZ();

    // getting the Cartesian field resolution
    gkg::Vector3d< double > resolution = odfCartesianField.getResolution();


    ////////////////////////////////////////////////////////////////////////////
    // creating the output orientation set
    ////////////////////////////////////////////////////////////////////////////

    int32_t outputOrientationCount = ( int32_t )( scalarParameters[ 5 ] + 0.5 );
    if ( outputOrientationCount <= 6 )
    {

      throw std::runtime_error(
                            "output orientation count must be greater than 6" );

    }

    gkg::OrientationSet
        outputOrientationSet( gkg::ElectrostaticOrientationSet( 
                    outputOrientationCount / 2 ).getSymmetricalOrientations() );


    ////////////////////////////////////////////////////////////////////////////
    // getting the b-value variation
    ////////////////////////////////////////////////////////////////////////////
    float deltaBValue = ( float )scalarParameters[ 6 ];


    ////////////////////////////////////////////////////////////////////////////
    // getting the input model orientation count
    ////////////////////////////////////////////////////////////////////////////
    int32_t inputModelOrientationCount =
                                     ( int32_t )( scalarParameters[ 7 ] + 0.5 );


    ////////////////////////////////////////////////////////////////////////////
    // allocating the adequate MCMC function
    ////////////////////////////////////////////////////////////////////////////

    gkg::RCPointer< gkg::MicrostructureMCMCFunction >
      microstructureMCMCFunction;

    if ( odfCartesianField.getBasisType() ==
         gkg::OrientationDistributionFunction::NoddiWatson )
    {

      // std deviation of noise only used for likelyhood so set to zero here
      microstructureMCMCFunction.reset(
        new gkg::NoddiMCMCWatsonFunction( 
                                     odfCartesianField.getRCPointerOfDWVolume(),
                                     outputOrientationSet,
                                     0.0 ) );

    }
    else if ( odfCartesianField.getBasisType() ==
              gkg::OrientationDistributionFunction::NoddiBingham )
    {

      // std deviation of noise only used for likelyhood so set to zero here
      microstructureMCMCFunction.reset(
        new gkg::NoddiMCMCBinghamFunction( 
                                     odfCartesianField.getRCPointerOfDWVolume(),
                                     outputOrientationSet,
                                     0.0 ) );

    }
    else if ( odfCartesianField.getBasisType() ==
              gkg::OrientationDistributionFunction::PGSEAxonMapping )
    {

      // std deviation of noise only used for likelyhood so set to zero here
      microstructureMCMCFunction.reset(
        new gkg::PGSEAxonMappingMCMCFunction( 
                                     odfCartesianField.getRCPointerOfDWVolume(),
                                     outputOrientationSet,
                                     0.0 ) );

    }
    else if ( odfCartesianField.getBasisType() ==
              gkg::OrientationDistributionFunction::CTOGSEAxonMapping )
    {

      // std deviation of noise only used for likelyhood so set to zero here
      microstructureMCMCFunction.reset(
        new gkg::CTOGSEAxonMappingMCMCFunction( 
                                     odfCartesianField.getRCPointerOfDWVolume(),
                                     outputOrientationSet,
                                     0.0 ) );

    }


    ////////////////////////////////////////////////////////////////////////////
    // computing site map of site(s) included in both the ODF cartesian field
    // and the mask
    ////////////////////////////////////////////////////////////////////////////

    std::list< gkg::Vector3d< int32_t > > sitesInMask;
    gkg::Vector3d< float > siteInOdfFrame;
    gkg::Vector3d< float > siteInMaskFrame;
    gkg::Vector3d< int32_t > siteInMask;
    std::list< Vector3d< int32_t > >::const_iterator
      s = sites.begin(),
      se = sites.end();
    while ( s != se )
    {

      siteInOdfFrame.x = ( float )s->x * ( float )resolution.x;
      siteInOdfFrame.y = ( float )s->y * ( float )resolution.y;
      siteInOdfFrame.z = ( float )s->z * ( float )resolution.z;

      transform3dFromMaskToOdf->getInverse( siteInOdfFrame, siteInMaskFrame );

      siteInMask.x = ( int32_t )( siteInMaskFrame.x / maskResolution.x );
      siteInMask.y = ( int32_t )( siteInMaskFrame.y / maskResolution.y );
      siteInMask.z = ( int32_t )( siteInMaskFrame.z / maskResolution.z );

      if ( maskBoundingBox.contains( siteInMask ) )
      {

        if ( volumeMaskInt16( siteInMask ) )
        {

          sitesInMask.push_back( *s );

        }

      }

      ++ s;

    }
    gkg::SiteMap< int32_t, int32_t > siteMapInMask;
    siteMapInMask.addSites( 0, sitesInMask );
    int32_t sitesInMaskCount = ( int32_t )sitesInMask.size();


    ////////////////////////////////////////////////////////////////////////////
    // preparing output container
    ////////////////////////////////////////////////////////////////////////////


    output.allocate( sizeX, sizeY, sizeZ,
                     resolution,
                     offset.x, offset.y, offset.z );
    output.setSiteMap( siteMapInMask );


    ////////////////////////////////////////////////////////////////////////////
    // extracting shell b-values
    ////////////////////////////////////////////////////////////////////////////


    std::vector< float > multipleShellBValues;
    std::vector< std::vector< int32_t > > multipleShellOrientationIndexSets;
    std::vector< std::vector< float > > gradientCharacteristics;
    std::string sequenceType;
    microstructureMCMCFunction->getMultipleShellBValues(
                                              sequenceType,
                                              gradientCharacteristics,
                                              multipleShellBValues,
                                              multipleShellOrientationIndexSets,
                                              deltaBValue );


    ////////////////////////////////////////////////////////////////////////////
    // looping over site(s) and processing output item(s)
    ////////////////////////////////////////////////////////////////////////////

    int32_t count = 0;
    int32_t o = 0;
    s = sitesInMask.begin(),
    se = sitesInMask.end();
    float t2Value = 0.0;
    std::vector< float > measuredAttenuationValues( inputOrientationCount );
    std::vector< float > modeledAttenuationValues( inputOrientationCount );
    while ( s != se )
    {

      if ( verbose )
      {

        if ( ( ( count + 1 ) % 100 == 0 ) ||
             ( count == 0 ) ||
             ( count == sitesInMaskCount - 1 ) )
        {

          if ( count )
          {

            std::cout << Eraser( 22 );

          }
          std::cout << " [ " << std::setw( 7 ) << count + 1 
                    << " / " << std::setw( 7 ) << sitesInMaskCount
                    << " ]" << std::flush;

        }

      }
      t2Value = t2Volume( *s );
      for ( o = 0; o  < inputOrientationCount; o++ )
      {

        measuredAttenuationValues[ o ] = dwVolume( *s, o ) / t2Value;

      }

      gkg::OrientationDistributionFunction*
        odf = odfCartesianField.getItem( *s );

      std::vector< std::vector< float > > modeledAttenuationValues;
      std::vector< float > coordinates;
      microstructureMCMCFunction->getModelAt( odf->getValues(),
                                              multipleShellBValues,
                                              gradientCharacteristics,
                                              inputModelOrientationCount,
                                              modeledAttenuationValues,
                                              coordinates );

      gkg::MeasurementVsModelAttenuation
        measurementVsModelAttenuation( measuredAttenuationValues,
                                       inputOrientationSet,
                                       odf->getPrincipalOrientation(),
                                       modeledAttenuationValues,
                                       multipleShellBValues,
                                       multipleShellOrientationIndexSets,
                                       coordinates );

      output.setItem( *s, measurementVsModelAttenuation );

      ++ s;
      ++ count;

    }

    if ( verbose )
    {

      std::cout << Eraser( 22 ) << "done" << std::endl;

    }

  }
  GKG_CATCH( "void gkg::OdfFunctorMeasurementVsModelPlot::process( "
             "gkg::MeasurementVsModelPlotter& output, "
             "gkg::OdfCartesianField& odfCartesianField, "
             "const std::vector< double >& scalarParameters, "
             "const std::vector< std::string >& stringParameters, "
             "bool verbose ) const" );


}


////////////////////////////////////////////////////////////////////////////////
// ODF functor registration
////////////////////////////////////////////////////////////////////////////////


static bool register_gkg_OdfFunctorMeasurementVsModelPlot 
                                                       __attribute__((unused)) =
gkg::OdfFunctorFactory< gkg::MeasurementVsModelPlotter ,
                        gkg::MeasurementVsModelAttenuation >::getInstance().
                                                           registerOdfFunctor(
                 gkg::OdfFunctorMeasurementVsModelPlot::getInstance() );


