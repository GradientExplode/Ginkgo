#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-io/TypedVolumeReaderProcess.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-process/Process.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-exception/Exception.h>
#include <string>
#include <iostream>
#include <fstream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <vector>
#include <algorithm>
#include <functional>
#include <unistd.h>
#include <string.h>
#include <sstream>
#include <stdarg.h>
#include <unistd.h>


#include "ProbabilityDensityFunction.h"

#include <gkg-processing-coordinates/ElectrostaticOrientationSet.h>
#include <gkg-processing-algobase/Convolution3d_i.h>
#include <gkg-dmri-container/PdfCartesianField.h>
#include <gkg-processing-container/SiteMap_i.h>
#include <gkg-processing-container/TextureMap_i.h>
#include <gkg-processing-container/CartesianField_i.h>
#include <gkg-communication-thread/ThreadedLoop.h>


#include "DiffeomorphicTransformations.h"
#include "MinimumSpanningTree.h"
#include "DataCostD.h"
#include "Regularization.h"
#include "SymmetricDiffeomorphism.h"
#include "PolarHarmonicsCache.h"
#include "SetStandardBasisTypeContext.h"
#include "PdfFieldInterpolation3DContext.h"
#include "DissimilarityEnergyComputationContext.h"
#include "ReferenceHasSiteBoolVectorContext.h"
#include "ReorientationContext.h"

#define INF = 1e20
#define RAND_SAMPLES = 64
   

int main( int argc, char* argv[] )
{

  int result = EXIT_SUCCESS;

  try
  {

    std::string fileNameSiteMapReference;
    std::string fileNameSiteMapFloating;
    std::string fileNameTextureMapReference;
    std::string fileNameTextureMapFloating;
    std::string fileNameSiteMapDeformedFloating; 
    std::string fileNameTextureMapDeformedFloating; 
    std::string fileNameFlowField; 
    std::string fileNameJacobianVolume; 
    std::string fileNameJacobian; 
    float regularizationWeighting = 0.0f;
    std::vector< int32_t > halfWidthSearchSpace;
    std::vector< int32_t > controlPointsSpacing;
    std::vector< float > searchSpaceQuantisation;
    std::vector< int32_t > displacementMagnitudes( 3, 0 );
    std::vector< int32_t > outputOrientationCounts( 3, 0 );
    bool convertToStandardBasisType = false;
    std::string reorientationType = "finite-strain";
    bool verbose = false;



    gkg::Application application( argc, argv,
                                  "Diffeomorphic registration"
                                  "between diffusion fields" );
    application.addSingleOption( "-referenceSiteMap",
                                 "Reference site map file name",
                                 fileNameSiteMapReference );
    application.addSingleOption( "-floatingSiteMap",
                                 "Floating site map file name",
                                 fileNameSiteMapFloating );
    application.addSingleOption( "-deformedFloatingSiteMap",
                                 "Deformed floating site map file name",
                                 fileNameSiteMapDeformedFloating );
    application.addSingleOption( "-referenceTextureMap",
                                 "Reference texture map file name",
                                 fileNameTextureMapReference );
    application.addSingleOption( "-floatingTextureMap",
                                 "Floating texture map file name",
                                 fileNameTextureMapFloating );
    application.addSingleOption( "-deformedFloatingTextureMap",
                                 "Deformed floating texture map file name",
                                 fileNameTextureMapDeformedFloating );
    application.addSingleOption( "-flowField",
                                 "Output direct/inverse flow field file name",
                                 fileNameFlowField );
    application.addSingleOption( "-jacobianVolume",
                                 "Output Jacobian volume file name",
                                 fileNameJacobianVolume );
    application.addSingleOption( "-jacobian",
                                 "Output Jacobian file name",
                                 fileNameJacobian );
				 
				 
    application.addSingleOption( "-convertToStandardBasisType",
                                 "Convert to standard basis (default=false)",
                                 convertToStandardBasisType,
                                 false );
    application.addSingleOption( "-reorientationType",
                                 "Reorientation type :"
                                 "affine, finite-strain or none"
                                 "(default=finie-strain)",
                                 reorientationType );
    application.addSingleOption( "-regularizationWeighting",
                                 "Regularization weighting (default=5.0)",
                                 regularizationWeighting,
                                 5.0f );
    application.addSeriesOption( "-halfWidthSearchSpace",
                                 "Half width search space ",
                                 halfWidthSearchSpace );
    application.addSeriesOption( "-controlPointsSpacing",
                                 "Control points spacing ",
                                 controlPointsSpacing );
    application.addSeriesOption( "-searchSpaceQuantisation",
                                 "Search space quantisation ",
                                 searchSpaceQuantisation );
    application.addSeriesOption( "-outputOrientationCounts",
                                 " Output orientation counts ",
                                 outputOrientationCounts );
    application.addSeriesOption( "-displacementMagnitudes",
                                 "Displacement magnitudes",
                                 displacementMagnitudes );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose, 
                                 true ); 

    application.initialize();


    ////////////////////////////////////////////////////////////////////////////
    // reading and building reference and floating
    ////////////////////////////////////////////////////////////////////////////

    // reading reference and floating site/texture maps

    gkg::SiteMap< int32_t, int32_t > referenceSiteMap;
    gkg::SiteMap< int32_t, int32_t > formerFloatingSiteMap;
    gkg::TextureMap< gkg::ProbabilityDensityFunction > referencePdfs;
    gkg::TextureMap< gkg::ProbabilityDensityFunction > formerFloatingPdfs;
    gkg::Reader::getInstance().read( fileNameSiteMapReference, 
                                     referenceSiteMap );
    gkg::Reader::getInstance().read( fileNameTextureMapReference, 
                                     referencePdfs );
    gkg::Reader::getInstance().read( fileNameSiteMapFloating, 
                                     formerFloatingSiteMap );
    gkg::Reader::getInstance().read( fileNameTextureMapFloating, 
                                     formerFloatingPdfs );

    // building reference and floating cartesian fields

    // getting sites list and vector
    std::list< gkg::Vector3d< int32_t > > floatingSitesList =
                                            formerFloatingSiteMap.getSites( 0 );
    std::set< gkg::Vector3d< int32_t >, 
              gkg::Vector3dCompare< int32_t > > hasSiteFormerFloating;
    hasSiteFormerFloating.insert( floatingSitesList.begin(),
                                  floatingSitesList.end() );



    const std::list< gkg::Vector3d< int32_t > > referenceSitesList =
                                                 referenceSiteMap.getSites( 0 );

    std::set< gkg::Vector3d< int32_t >, 
              gkg::Vector3dCompare< int32_t > > hasSiteReference;
    hasSiteReference.insert( referenceSitesList.begin(),
                             referenceSitesList.end() );
    std::vector< gkg::Vector3d< int32_t > > referenceSites( 
                                                     referenceSitesList.begin(),
                                                     referenceSitesList.end() );
    int32_t referenceSitesSize = referenceSites.size();

    // creating output sampling
    std::map< float, gkg::OrientationSet > outputMultipleShellSampling;
    if( outputOrientationCounts.size() ==  displacementMagnitudes.size() )
    {

      std::vector< int32_t >::iterator
      o = outputOrientationCounts.begin(),
      oe = outputOrientationCounts.end();

      std::vector< int32_t >::iterator
      d = displacementMagnitudes.begin();


      while( o != oe )
      {

        outputMultipleShellSampling.insert( std::make_pair(
                         *d, 
                         gkg::OrientationSet( gkg::ElectrostaticOrientationSet(
                                    *o / 2 ).getSymmetricalOrientations() ) ) );
        ++o;  
        ++d;
   
      }      

    }
    else
    {

      throw std::runtime_error(" Not the same number of output orientation "
                               " counts and displacement magnitudes ");

    }

    gkg::PdfCartesianField rawReference( referenceSiteMap,
                                         referencePdfs,
                                         outputMultipleShellSampling );

    gkg::PdfCartesianField formerFloating( formerFloatingSiteMap,
                                           formerFloatingPdfs,
                                           outputMultipleShellSampling );

    // adding missing reference sites to floating sitemap

    gkg::ProbabilityDensityFunction nullPdf( 
                            *( rawReference.getItem( referenceSites[ 0 ] ) ) );

    int32_t valueIndex = 0;
    for ( valueIndex = 1; valueIndex < ( int32_t )nullPdf.getValues().size(); 
          ++ valueIndex )
    {

      nullPdf.setValue( valueIndex, 0.0f );

    }    

    std::list< gkg::ProbabilityDensityFunction > floatingPdfsList;


    std::list< gkg::Vector3d< int32_t > >::const_iterator
       f = floatingSitesList.begin(),
       fe = floatingSitesList.end();
    while ( f != fe )
    {

      const gkg::Vector3d< int32_t >& floatingSite = *f;

      floatingPdfsList.push_back( *( formerFloating.getItem( floatingSite ) ) );

      ++ f;  

    }

    std::set< gkg::Vector3d< int32_t >, 
              gkg::Vector3dCompare< int32_t > >::iterator h;

    std::list< gkg::Vector3d< int32_t > >::const_iterator
       r = referenceSitesList.begin(),
       re = referenceSitesList.end();
    while ( r != re )
    {

      gkg::Vector3d< int32_t > referenceSite( *r );

      h = hasSiteFormerFloating.find( referenceSite );

      if ( h == hasSiteFormerFloating.end() )
      {

        floatingSitesList.push_back( referenceSite );
        floatingPdfsList.push_back( nullPdf );

      }

      ++ r;  

    }

    std::vector< gkg::Vector3d< int32_t > > floatingSites( 
                                                      floatingSitesList.begin(),
                                                      floatingSitesList.end() );
    int32_t floatingSitesSize = floatingSites.size();

    std::cout << "floatingSitesSize = " << floatingSitesSize << std::endl;

    std::set< gkg::Vector3d< int32_t >, 
              gkg::Vector3dCompare< int32_t > > hasSiteFloating;
    hasSiteFloating.insert( floatingSitesList.begin(),
                            floatingSitesList.end() );

    gkg::SiteMap< int32_t, int32_t > floatingSiteMap;
    gkg::TextureMap< gkg::ProbabilityDensityFunction > floatingPdfs;
    floatingSiteMap.addSites( 0, floatingSitesList );
    floatingPdfs.addTextures( floatingPdfsList );

    // creating new floating fields 
    gkg::PdfCartesianField rawFloating( formerFloating.getSizeX(),
                                        formerFloating.getSizeY(),
                                        formerFloating.getSizeZ(),
                                        formerFloating.getResolution(),
                                        floatingSiteMap,
                                        floatingPdfs,
                                        formerFloating.getOffsetX(),
                                        formerFloating.getOffsetY(),
                                        formerFloating.getOffsetZ(),
                                        outputMultipleShellSampling );
 
    ////////////////////////////////////////////////////////////////////////////
    // apply mask and affine registration on PDF fields ( TO BE DONE )
    ////////////////////////////////////////////////////////////////////////////

    // ( ... )

    ////////////////////////////////////////////////////////////////////////////
    // creating PDF fields before diffeomorphic registration
    ////////////////////////////////////////////////////////////////////////////

    gkg::PdfCartesianField reference( rawReference.getSizeX(),
                                      rawReference.getSizeY(),
                                      rawReference.getSizeZ(),
                                      rawReference.getResolution(),
                                      referenceSiteMap,
                                      referencePdfs,
                                      rawReference.getOffsetX(),
                                      rawReference.getOffsetY(),
                                      rawReference.getOffsetZ(),
                                      outputMultipleShellSampling );

    gkg::PdfCartesianField floating( rawFloating.getSizeX(),
                                     rawFloating.getSizeY(),
                                     rawFloating.getSizeZ(),
                                     rawFloating.getResolution(),
                                     floatingSiteMap,
                                     floatingPdfs,
                                     rawFloating.getOffsetX(),
                                     rawFloating.getOffsetY(),
                                     rawFloating.getOffsetZ(),
                                     outputMultipleShellSampling );

    // creating 2 copies of volumes to apply final transformation to ph coeffts

    gkg::PdfCartesianField floatingPhBasis1( rawFloating.getSizeX(),
                                             rawFloating.getSizeY(),
                                             rawFloating.getSizeZ(),
                                             rawFloating.getResolution(),
                                             floatingSiteMap,
                                             floatingPdfs,
                                             rawFloating.getOffsetX(),
                                             rawFloating.getOffsetY(),
                                             rawFloating.getOffsetZ(),
                                             outputMultipleShellSampling );

    gkg::PdfCartesianField floatingPhBasis2( rawFloating.getSizeX(),
                                             rawFloating.getSizeY(),
                                             rawFloating.getSizeZ(),
                                             rawFloating.getResolution(),
                                             floatingSiteMap,
                                             floatingPdfs,
                                             rawFloating.getOffsetX(),
                                             rawFloating.getOffsetY(),
                                             rawFloating.getOffsetZ(),
                                             outputMultipleShellSampling );


    ////////////////////////////////////////////////////////////////////////////
    // which kind of PDF are we dealing with ?
    ////////////////////////////////////////////////////////////////////////////

    gkg::ProbabilityDensityFunction& pdfTest = 
                             *( reference.getItemFast( referenceSites[ 0 ] ) );

    int32_t basisOrder = pdfTest.getValue( 0 );
    std::string basisTypeName = pdfTest.getBasisTypeName();
    std::cout << "Basis type name : " << basisTypeName << std::endl;

    // getting access to the polar harmonics cache
    gkg::PolarHarmonicsCache& polarHarmonicsCache = 
                                       gkg::PolarHarmonicsCache::getInstance();

    // updating the polar harmonics cache (computation of tuple (n1,n2,n3), ..
    polarHarmonicsCache.update( basisOrder );

    //pdfTest.setOutputSampling( &outputMultipleShellSampling );
    //pdfTest.setStandardBasisType();

    std::vector< float > values = pdfTest.getValues();

    int32_t v = 0;
    int32_t c = 0;
    for ( v = 0; v < ( int32_t )values.size(); ++v )
    {

      if ( values[ v ] > 0 )
        c++;

      std::cout << "values[ v ] = " << values[ v ] << std::endl;  

    }

    std::cout << "Number of values = " << c << std::endl;  

    ////////////////////////////////////////////////////////////////////////////
    // computing a boolean vector to speed up computation
    //////////////////////////////////////////////////////////////////////////// 

      
    std::cout << "Computing bool vector to speed up computation" << std::endl;

    std::vector< bool > referenceHasSiteBoolVector( floatingSitesSize, false );

    gkg::ReferenceHasSiteBoolVectorContext referenceHasSiteBoolVectorContext( 
                                           hasSiteReference,
                                           referenceHasSiteBoolVector,
                                           floatingSites );

    gkg::ThreadedLoop< int32_t >
     threadedLoop4( &referenceHasSiteBoolVectorContext,
                    0,
                    floatingSitesSize );

    threadedLoop4.launch();

    std::cout << "Done." << std::endl;

    ////////////////////////////////////////////////////////////////////////////
    // getting a mask of reference volume
    ////////////////////////////////////////////////////////////////////////////        


    ////////////////////////////////////////////////////////////////////////////
    // collecting size and resolution of input volumes
    ////////////////////////////////////////////////////////////////////////////
    int32_t sizeX = reference.getSizeX();
    int32_t sizeY = reference.getSizeY();
    int32_t sizeZ = reference.getSizeZ();
    gkg::Vector3d< int32_t > sizeVector( sizeX, sizeY, sizeZ );
    int32_t size = sizeX * sizeY * sizeZ;

    gkg::Vector3d< double > resolution = reference.getResolution();   
    gkg::Vector3d< int32_t > volumeSize( sizeX, sizeY, sizeZ );

    ////////////////////////////////////////////////////////////////////////////
    // initializing control points spacing and label space (default 3 levels)
    ////////////////////////////////////////////////////////////////////////////
    int32_t currentHalfWidthSearchSpace = 0;
    int32_t currentControlPointsSpacing = 0;
    float currentSearchSpaceQuantisation = 0.0f;
    
    ////////////////////////////////////////////////////////////////////////////
    // initialize full resolution direct and inverse flow fields
    ////////////////////////////////////////////////////////////////////////////
    gkg::Volume< float > directFlowFieldX( sizeVector );
    directFlowFieldX.setResolution( resolution );
    directFlowFieldX.fill( 0.0f );
    gkg::Volume< float > directFlowFieldY( sizeVector );
    directFlowFieldY.setResolution( resolution );
    directFlowFieldY.fill( 0.0f );
    gkg::Volume< float > directFlowFieldZ( sizeVector );
    directFlowFieldZ.setResolution( resolution );
    directFlowFieldZ.fill( 0.0f );

    gkg::Volume< float > inverseFlowFieldX( sizeVector );
    inverseFlowFieldX.setResolution( resolution );
    inverseFlowFieldX.fill( 0.0f );
    gkg::Volume< float > inverseFlowFieldY( sizeVector );
    inverseFlowFieldY.setResolution( resolution );
    inverseFlowFieldY.fill( 0.0f );
    gkg::Volume< float > inverseFlowFieldZ( sizeVector );
    inverseFlowFieldZ.setResolution( resolution );
    inverseFlowFieldZ.fill( 0.0f );
    ////////////////////////////////////////////////////////////////////////////
    // initialize direct and inverse flow fields at coarsest resolution
    ////////////////////////////////////////////////////////////////////////////
    int32_t resolutionFactor = controlPointsSpacing[ 0 ];

    int32_t scaledSizeX = sizeX / resolutionFactor;
    int32_t scaledSizeY = sizeY / resolutionFactor;
    int32_t scaledSizeZ = sizeZ / resolutionFactor;
    gkg::Vector3d< int32_t > scaledSizeVector( scaledSizeX,
                                               scaledSizeY,
                                               scaledSizeZ );

    gkg::Vector3d< double > scaledResolution = gkg::Vector3d< double >( 
                                              resolution.x * resolutionFactor,
                                              resolution.y * resolutionFactor,
                                              resolution.z * resolutionFactor );

    gkg::Volume< float > scaledDirectFlowFieldX( scaledSizeVector );
    scaledDirectFlowFieldX.setResolution( scaledResolution );
    scaledDirectFlowFieldX.fill( 0.0f );

    gkg::Volume< float > scaledDirectFlowFieldY( scaledSizeVector );
    scaledDirectFlowFieldY.setResolution( scaledResolution );
    scaledDirectFlowFieldY.fill( 0.0f );

    gkg::Volume< float > scaledDirectFlowFieldZ( scaledSizeVector );
    scaledDirectFlowFieldZ.setResolution( scaledResolution );
    scaledDirectFlowFieldZ.fill( 0.0f );

    gkg::Volume< float > scaledInverseFlowFieldX( scaledSizeVector );
    scaledInverseFlowFieldX.setResolution( scaledResolution );
    scaledInverseFlowFieldX.fill( 0.0f );

    gkg::Volume< float > scaledInverseFlowFieldY( scaledSizeVector );
    scaledInverseFlowFieldY.setResolution( scaledResolution );
    scaledInverseFlowFieldY.fill( 0.0f );

    gkg::Volume< float > scaledInverseFlowFieldZ( scaledSizeVector );
    scaledInverseFlowFieldZ.setResolution( scaledResolution );
    scaledInverseFlowFieldZ.fill( 0.0f );


    gkg::Volume< float > J11( scaledSizeVector );
      J11.fill( 0.0f );
    gkg::Volume< float > J12( scaledSizeVector );
      J12.fill( 0.0f );
    gkg::Volume< float > J13( scaledSizeVector );
      J13.fill( 0.0f );
    gkg::Volume< float > J21( scaledSizeVector );
      J21.fill( 0.0f );
    gkg::Volume< float > J22( scaledSizeVector );
      J22.fill( 0.0f );
    gkg::Volume< float > J23( scaledSizeVector );
      J23.fill( 0.0f );
    gkg::Volume< float > J31( scaledSizeVector );
      J31.fill( 0.0f );
    gkg::Volume< float > J32( scaledSizeVector );
      J32.fill( 0.0f );
    gkg::Volume< float > J33( scaledSizeVector );
      J33.fill( 0.0f );


    gkg::Volume< float > J11regularized( scaledSizeVector );
      J11regularized.fill( 0.0f );
    gkg::Volume< float > J12regularized( scaledSizeVector );
      J12regularized.fill( 0.0f );
    gkg::Volume< float > J13regularized( scaledSizeVector );
      J13regularized.fill( 0.0f );
    gkg::Volume< float > J21regularized( scaledSizeVector );
      J21regularized.fill( 0.0f );
    gkg::Volume< float > J22regularized( scaledSizeVector );
      J22regularized.fill( 0.0f );
    gkg::Volume< float > J23regularized( scaledSizeVector );
      J23regularized.fill( 0.0f );
    gkg::Volume< float > J31regularized( scaledSizeVector );
      J31regularized.fill( 0.0f );
    gkg::Volume< float > J32regularized( scaledSizeVector );
      J32regularized.fill( 0.0f );
    gkg::Volume< float > J33regularized( scaledSizeVector );
      J33regularized.fill( 0.0f );

    ////////////////////////////////////////////////////////////////////////////
    // converting PDF fields to standard basis type (if necessary)
    ////////////////////////////////////////////////////////////////////////////
    
    bool hasConvertedToStandardBasisType = false;
 
    if ( basisTypeName == "ph" && convertToStandardBasisType )
    {

      hasConvertedToStandardBasisType = true; 

      std::cout << "Converting reference to standard basis type" << std::endl;

      // vector set representation context reference
      gkg::SetStandardBasisTypeContext setStandardBasisTypeContext1(
                                                 referenceSites,
                                                 reference,
                                                 outputMultipleShellSampling );

      gkg::ThreadedLoop< int32_t >
        threadedLoop1( &setStandardBasisTypeContext1,
                       0,
                       referenceSitesSize );

      threadedLoop1.launch();

      std::cout << "Done." << std::endl;

      std::cout << "Converting floating to standard basis type" << std::endl;

      // vector set representation context floating
      gkg::SetStandardBasisTypeContext setStandardBasisTypeContext2( 
                                                 floatingSites,
                                                 floating,
                                                 outputMultipleShellSampling );

      gkg::ThreadedLoop< int32_t >
        threadedLoop2( &setStandardBasisTypeContext2,
                       0,
                       floatingSitesSize );

      threadedLoop2.launch();

      std::cout << "Done." << std::endl;

    }


    values = pdfTest.getValues();

    v = 0;
    c = 0;
    for ( v = 0; v < ( int32_t )values.size(); ++v )
    {

      if ( values[ v ] > 0 )
        c++;

      std::cout << "std values[ v ] = " << values[ v ] << std::endl;  

    }

    std::cout << "Number of values = " << c << std::endl;  

    /*
    std::cout << "Writing std reference ...\n" << std::endl;

    gkg::Writer::getInstance().write( "reference_std",
                                      reference.getSiteMap(),
                                      false,
                                      "sitemap" );
    gkg::Writer::getInstance().write( "reference_std",
                                      reference.getItems(),
                                      false,
                                      "texturemap" );

    std::cout << "done" << std::endl;


    std::cout << "Writing std floating ...\n" << std::endl;

    gkg::Writer::getInstance().write( "floating_std",
                                      floating.getSiteMap(),
                                      false,
                                      "sitemap" );
    gkg::Writer::getInstance().write( "floating_std",
                                      floating.getItems(),
                                      false,
                                      "texturemap" );

    std::cout << "done" << std::endl;
    */

    ////////////////////////////////////////////////////////////////////////////
    // initialize warped images
    ////////////////////////////////////////////////////////////////////////////


    gkg::PdfCartesianField warpedField1( floating.getSizeX(),
                                         floating.getSizeY(),
                                         floating.getSizeZ(),
                                         floating.getResolution(),
                                         floating.getSiteMap(),
                                         floating.getItems(),
                                         floating.getOffsetX(),
                                         floating.getOffsetY(),
                                         floating.getOffsetZ(),
                                         outputMultipleShellSampling );


    gkg::PdfCartesianField warpedField2( reference.getSizeX(),
                                         reference.getSizeY(),
                                         reference.getSizeZ(),
                                         reference.getResolution(),
                                         reference.getSiteMap(),
                                         reference.getItems(),
                                         reference.getOffsetX(),
                                         reference.getOffsetY(),
                                         reference.getOffsetZ(),
                                         outputMultipleShellSampling );

    ////////////////////////////////////////////////////////////////////////////
    // matching process
    ////////////////////////////////////////////////////////////////////////////
    float dissimilarityEnergyBefore = 0.0f;
    float dissimilarityEnergyAfter = 0.0f;

    int32_t currentSizeX = 0;
    int32_t currentSizeY = 0;
    int32_t currentSizeZ = 0;
    int32_t currentSize = 0;

    int32_t levelCount = halfWidthSearchSpace.size();
    std::cout << "levelCount = " << levelCount  << std::endl;

    bool addIntegerCoordinates = true;
    int32_t index = 0;

    if ( basisTypeName == "ph" && !hasConvertedToStandardBasisType )
    {

      index = 1;   
 
    }

    int32_t level = 0;
    for ( level = 0; level < levelCount; ++level ) 
    {

      //warp both high-resolution images according
      /*
      std::cout << "Warping both high-resolution images" << std::endl;

      gkg::PdfFieldInterpolation3DContext pdfFieldInterpolation3DContext1( 
                                                        warpedField1,
                                                        floating,
                                                        hasSiteFloating,
                                                        hasSiteFloating,
                                                        floatingSites,
                                                        directFlowFieldX, 
                                                        directFlowFieldY, 
                                                        directFlowFieldZ, 
                                                        sizeVector,
                                                        addIntegerCoordinates,
                                                        index );

      gkg::ThreadedLoop< int32_t >
        threadedLoop3DInterpolation1( &pdfFieldInterpolation3DContext1,
                                      0,
                                      floatingSitesSize );

      threadedLoop3DInterpolation1.launch();



      gkg::PdfFieldInterpolation3DContext pdfFieldInterpolation3DContext2( 
                                                        warpedField2,
                                                        reference,
                                                        hasSiteReference,
                                                        hasSiteReference,
                                                        referenceSites,
                                                        inverseFlowFieldX, 
                                                        inverseFlowFieldY, 
                                                        inverseFlowFieldZ, 
                                                        sizeVector,
                                                        addIntegerCoordinates,
                                                        index );

      gkg::ThreadedLoop< int32_t >
        threadedLoop3DInterpolation2( &pdfFieldInterpolation3DContext2,
                                      0,
                                      referenceSitesSize );

      threadedLoop3DInterpolation2.launch();

      std::cout << "Done." << std::endl;
      */

      currentHalfWidthSearchSpace = halfWidthSearchSpace[ level ];
      currentControlPointsSpacing = controlPointsSpacing[ level ];
      currentSearchSpaceQuantisation = searchSpaceQuantisation[ level ];

      int32_t cubedLength = std::pow( currentHalfWidthSearchSpace * 2 + 1, 3 );
      currentSizeX = sizeX / currentControlPointsSpacing;
      currentSizeY = sizeY / currentControlPointsSpacing; 
      currentSizeZ = sizeZ / currentControlPointsSpacing; 
      gkg::Vector3d< int32_t > currentSizeVector( currentSizeX,
                                                  currentSizeY,
                                                  currentSizeZ ); 
      currentSize = currentSizeX * currentSizeY * currentSizeZ;

      //resize flow to current scale (grid spacing)
      gkg::Volume< float > currentDirectFlowFieldX( currentSizeVector );
      currentDirectFlowFieldX.fill( 0.0f );

      gkg::Volume< float > currentDirectFlowFieldY( currentSizeVector );
      currentDirectFlowFieldY.fill( 0.0f );

      gkg::Volume< float > currentDirectFlowFieldZ( currentSizeVector );
      currentDirectFlowFieldZ.fill( 0.0f );

      gkg::Volume< float > currentInverseFlowFieldX( currentSizeVector );
      currentInverseFlowFieldX.fill( 0.0f );

      gkg::Volume< float > currentInverseFlowFieldY( currentSizeVector );
      currentInverseFlowFieldY.fill( 0.0f );

      gkg::Volume< float > currentInverseFlowFieldZ( currentSizeVector );
      currentInverseFlowFieldZ.fill( 0.0f );

      upsampleDeformations( currentDirectFlowFieldX,
                            currentDirectFlowFieldY,  
                            currentDirectFlowFieldZ,  
                            scaledDirectFlowFieldX, 
                            scaledDirectFlowFieldY, 
                            scaledDirectFlowFieldZ, 
                            currentSizeVector, 
                            scaledSizeVector ); 

      upsampleDeformations( currentInverseFlowFieldX,
                            currentInverseFlowFieldY,  
                            currentInverseFlowFieldZ,  
                            scaledInverseFlowFieldX, 
                            scaledInverseFlowFieldY, 
                            scaledInverseFlowFieldZ, 
                            currentSizeVector, 
                            scaledSizeVector ); 

      std::cout<<"==========================================================\n";
      std::cout<<"Level "<< level <<" grid=" << currentControlPointsSpacing
               <<" with sizes: "<< currentSizeX << "x"
                                << currentSizeY << "x"
                                << currentSizeZ
               <<" hw="<< currentHalfWidthSearchSpace
               <<" quant="<< currentSearchSpaceQuantisation << "\n";
      std::cout<<"==========================================================\n"; 

      std::cout << "Computing first Prim's graph " << std::endl;

      std::vector< int32_t > referenceOrderedMST( currentSize, 0 );
      std::vector< int32_t > referenceParentsMST( currentSize, 0 );

      primsGraph( reference, 
                  hasSiteReference,
                  sizeVector,
                  referenceOrderedMST, 
                  referenceParentsMST, 
                  currentControlPointsSpacing );

      std::cout << "Done. " << std::endl;

      std::cout << "Computing second Prim's graph " << std::endl;

      // minimum spanning trees
      std::vector< int32_t > floatingOrderedMST( currentSize, 0 );
      std::vector< int32_t > floatingParentsMST( currentSize, 0 );

      primsGraph( floating, 
                  hasSiteFloating,
                  sizeVector,
                  floatingOrderedMST, 
                  floatingParentsMST, 
                  currentControlPointsSpacing );

      std::cout << "Done. " << std::endl;

      std::cout << "Starting dissimilarity computation\n";
      std::cout << "==================================================\n";

      // data-cost / dissimilarity computation
      std::vector< float > totalCost1( cubedLength * currentSize );
      std::vector< float > totalCost2( cubedLength * currentSize );

      dataCost( reference, 
                warpedField1,
                hasSiteReference,
                hasSiteFloating,
                sizeVector,
                totalCost1,
                regularizationWeighting,
                currentHalfWidthSearchSpace,
                currentControlPointsSpacing,
                currentSearchSpaceQuantisation );

      dataCost( floating, 
                warpedField2,
                hasSiteFloating,
                hasSiteReference,
                sizeVector,
                totalCost2,
                regularizationWeighting,
                currentHalfWidthSearchSpace,
                currentControlPointsSpacing,
                currentSearchSpaceQuantisation );



      std::cout << "Done. " << std::endl;

      std::cout<<"Start regularisation on MST\n";
      std::cout<<"==================================================\n";


      scaledDirectFlowFieldX.reallocate( currentSizeVector );
      scaledDirectFlowFieldX.fill( 0.0f ); 
      scaledDirectFlowFieldY.reallocate( currentSizeVector );
      scaledDirectFlowFieldY.fill( 0.0f ); 
      scaledDirectFlowFieldZ.reallocate( currentSizeVector );
      scaledDirectFlowFieldZ.fill( 0.0f ); 

      scaledInverseFlowFieldX.reallocate( currentSizeVector );
      scaledInverseFlowFieldX.fill( 0.0f ); 
      scaledInverseFlowFieldY.reallocate( currentSizeVector );
      scaledInverseFlowFieldY.fill( 0.0f ); 
      scaledInverseFlowFieldZ.reallocate( currentSizeVector );
      scaledInverseFlowFieldZ.fill( 0.0f ); 

      regularisation( scaledDirectFlowFieldX,
                      scaledDirectFlowFieldY,
                      scaledDirectFlowFieldZ,
                      currentDirectFlowFieldX,
                      currentDirectFlowFieldY,
                      currentDirectFlowFieldZ,
                      totalCost1,
                      regularizationWeighting,
                      currentHalfWidthSearchSpace,
                      currentControlPointsSpacing,
                      currentSearchSpaceQuantisation,  
                      referenceOrderedMST,
                      referenceParentsMST,
                      sizeVector,
                      hasSiteFloating );

      regularisation( scaledInverseFlowFieldX,
                      scaledInverseFlowFieldY,
                      scaledInverseFlowFieldZ,
                      currentInverseFlowFieldX,
                      currentInverseFlowFieldY,
                      currentInverseFlowFieldZ,
                      totalCost2,
                      regularizationWeighting,
                      currentHalfWidthSearchSpace,
                      currentControlPointsSpacing,
                      currentSearchSpaceQuantisation,  
                      floatingOrderedMST,
                      floatingParentsMST,
                      sizeVector,
                      hasSiteFloating );

      std::cout << "Done. " << std::endl;

      std::cout << "Making transformations diffeomorphic " << std::endl;

      
      //make transformations diffeomorphic using scaling-and-squaring
      diffeomorphic( scaledDirectFlowFieldX,
                     scaledDirectFlowFieldY,
                     scaledDirectFlowFieldZ,
                     currentSizeVector,
                     4,
                     currentControlPointsSpacing );

      diffeomorphic( scaledInverseFlowFieldX,
                     scaledInverseFlowFieldY,
                     scaledInverseFlowFieldZ,
                     currentSizeVector,
                     4,
                     currentControlPointsSpacing );
      


      //calculate inverses and compose new symmetric mapping 
      // (forward and backward)
      symmetricMapping( scaledDirectFlowFieldX,
                        scaledDirectFlowFieldY,
                        scaledDirectFlowFieldZ,
                        scaledInverseFlowFieldX,
                        scaledInverseFlowFieldY,
                        scaledInverseFlowFieldZ,
                        currentSizeVector,
                        currentControlPointsSpacing );


      //calculate Jacobian determinant
      J11.reallocate( currentSizeVector );
        J11.fill( 0.0f );
      J12.reallocate( currentSizeVector );
        J12.fill( 0.0f );
      J13.reallocate( currentSizeVector );
        J13.fill( 0.0f );
      J21.reallocate( currentSizeVector );
        J21.fill( 0.0f );
      J22.reallocate( currentSizeVector );
        J22.fill( 0.0f );
      J23.reallocate( currentSizeVector );
        J23.fill( 0.0f );
      J31.reallocate( currentSizeVector );
        J31.fill( 0.0f );
      J32.reallocate( currentSizeVector );
        J32.fill( 0.0f );
      J33.reallocate( currentSizeVector );
        J33.fill( 0.0f );

      float jac = jacobian( scaledDirectFlowFieldX,
                scaledDirectFlowFieldY,
                scaledDirectFlowFieldZ, 
                currentSizeVector,
                currentControlPointsSpacing,
                J11, J12, J13,
                J21, J22, J23,
                J31, J32, J33 );

      if ( !fileNameJacobian.empty() )
      {
      
        std::ofstream os( fileNameJacobian.c_str() );
        os << jac << std::endl;  
        os.close();
	
      }

      //upsample deformations from grid-resolution to high-resolution 
      // (trilinear=1st-order spline)
      upsampleDeformations( directFlowFieldX,
                            directFlowFieldY,
                            directFlowFieldZ,
                            scaledDirectFlowFieldX,
                            scaledDirectFlowFieldY,
                            scaledDirectFlowFieldZ,
                            sizeVector,
                            currentSizeVector );   

      upsampleDeformations( inverseFlowFieldX,
                            inverseFlowFieldY,
                            inverseFlowFieldZ,
                            scaledInverseFlowFieldX,
                            scaledInverseFlowFieldY,
                            scaledInverseFlowFieldZ,
                            sizeVector,
                            currentSizeVector ); 

      std::cout << "Done. " << std::endl;

      float energy = harmonicEnergy( directFlowFieldX, 
                                     directFlowFieldY,
                                     directFlowFieldZ, 
                                     sizeVector );
      std::cout << "Harmonic energy of deformation field: " << energy << "\n";

      gkg::DissimilarityEnergyComputationContext 
                          dissimilarityEnergyComputationContextBefore( 
                                                    reference,
                                                    warpedField1,
                                                    referenceHasSiteBoolVector,
                                                    floatingSites,
                                                    dissimilarityEnergyBefore,
                                                    size );


      gkg::ThreadedLoop< int32_t >
        threadedLoopDissimilarityEnergyBefore( 
                                    &dissimilarityEnergyComputationContextBefore,
                                    0,
                                    floatingSitesSize );

      threadedLoopDissimilarityEnergyBefore.launch();		



      gkg::PdfFieldInterpolation3DContext pdfFieldInterpolation3DContext3( 
                                                        warpedField1,
                                                        floating,
                                                        hasSiteFloating,
                                                        hasSiteFloating,
                                                        floatingSites,
                                                        directFlowFieldX, 
                                                        directFlowFieldY, 
                                                        directFlowFieldZ, 
                                                        sizeVector,
                                                        addIntegerCoordinates,
                                                        index );

      gkg::ThreadedLoop< int32_t >
        threadedLoop3DInterpolation3( &pdfFieldInterpolation3DContext3,
                                      0,
                                      floatingSitesSize );

      threadedLoop3DInterpolation3.launch();

      /*
      if ( !( reorientationType == "none" ) )
      {
 
        std::cout << "Performing reorientation of dMRI data" << std::endl;

        gkg::ReorientationContext reorientationContext(
           reorientationType, 
           floatingSites,
           warpedField1,
           reference,
           referenceHasSiteBoolVector,
           J11, J12, J13,
           J21, J22, J23,
           J31, J32, J33 );
      

        gkg::ThreadedLoop< int32_t >
          threadedLoopReorientation( &reorientationContext,
                                     0,
                                     floatingSitesSize );

        threadedLoopReorientation.launch();
      
        std::cout << "Done." << std::endl;

      }
      */

      gkg::DissimilarityEnergyComputationContext 
                          dissimilarityEnergyComputationContextAfter( 
                                                    reference,
                                                    warpedField1,
                                                    referenceHasSiteBoolVector,
                                                    floatingSites,
                                                    dissimilarityEnergyAfter,
                                                    size );


      gkg::ThreadedLoop< int32_t >
        threadedLoopDissimilarityEnergyAfter( 
                                    &dissimilarityEnergyComputationContextAfter,
                                    0,
                                    floatingSitesSize );

      threadedLoopDissimilarityEnergyAfter.launch();

      std::cout << "Dissimilarity energy before registration: " 
                << dissimilarityEnergyBefore 
                <<" and after " 
                << dissimilarityEnergyAfter << "\n";

      scaledSizeVector.x = currentSizeVector.x;
      scaledSizeVector.y = currentSizeVector.y;
      scaledSizeVector.z = currentSizeVector.z;

      std::cout<<"\n";

    }

    // applying final transformation to pdfs in ph basis
    index = 1;
    gkg::PdfFieldInterpolation3DContext pdfFieldInterpolation3DContext( 
                                                        floatingPhBasis2,
                                                        floatingPhBasis1,
                                                        hasSiteFloating,
                                                        hasSiteFloating,
                                                        floatingSites,
                                                        directFlowFieldX, 
                                                        directFlowFieldY, 
                                                        directFlowFieldZ, 
                                                        sizeVector,
                                                        addIntegerCoordinates,
                                                        index );

    gkg::ThreadedLoop< int32_t >
      threadedLoop3DInterpolation( &pdfFieldInterpolation3DContext,
                                   0,
                                   floatingSitesSize );

    threadedLoop3DInterpolation.launch();

    // applying reference mask on registered floating
    std::cout << "Applying reference mask on floating ...\n" << std::endl;

    std::list< gkg::Vector3d< int32_t > > maskedFloatingSitesList;
    std::list< gkg::ProbabilityDensityFunction > maskedFloatingPdfsList;

    std::set< gkg::Vector3d< int32_t >, 
              gkg::Vector3dCompare< int32_t > >::iterator nh;

    std::list< gkg::Vector3d< int32_t > >::const_iterator
       nf = floatingSitesList.begin(),
       nfe = floatingSitesList.end();
    while ( nf != nfe )
    {

      const gkg::Vector3d< int32_t >& floatingSite = *nf;
      nh = hasSiteReference.find( floatingSite );

      if ( nh != hasSiteReference.end() )
      {

        maskedFloatingSitesList.push_back( floatingSite );
        maskedFloatingPdfsList.push_back( *( floatingPhBasis2.getItem( 
                                                           floatingSite ) ) );

      }

      ++ nf;  

    }

    std::vector< gkg::Vector3d< int32_t > > maskedFloatingSites( 
                                               maskedFloatingSitesList.begin(),
                                               maskedFloatingSitesList.end() );

    int32_t maskedFloatingSitesSize = maskedFloatingSites.size();

    gkg::SiteMap< int32_t, int32_t > maskedFloatingSiteMap;
    gkg::TextureMap< gkg::ProbabilityDensityFunction > maskedFloatingPdfs;
    maskedFloatingSiteMap.addSites( 0, maskedFloatingSitesList );
    maskedFloatingPdfs.addTextures( maskedFloatingPdfsList );

    // creating new floating fields 
    gkg::PdfCartesianField maskedFloating( floatingPhBasis2.getSizeX(),
                                           floatingPhBasis2.getSizeY(),
                                           floatingPhBasis2.getSizeZ(),
                                           floatingPhBasis2.getResolution(),
                                           maskedFloatingSiteMap,
                                           maskedFloatingPdfs,
                                           floatingPhBasis2.getOffsetX(),
                                           floatingPhBasis2.getOffsetY(),
                                           floatingPhBasis2.getOffsetZ(),
                                           outputMultipleShellSampling );


    std::cout << "Done." << std::endl;


    std::cout << "Writing not reoriented ph warped image ...\n" << std::endl;

    gkg::Writer::getInstance().write( fileNameSiteMapDeformedFloating,
                                      maskedFloating.getSiteMap(),
                                      false,
                                      "sitemap" );
    gkg::Writer::getInstance().write( fileNameTextureMapDeformedFloating,
                                      maskedFloating.getItems(),
                                      false,
                                      "texturemap" );

    std::cout << "done" << std::endl;


    if ( !( reorientationType == "none" ) )
    {
 
      std::cout << "Gaussian smoothing of jacobian " << std::endl;

      float jacobianRegularizationFactor = 3.0f;
      float twiceSigmaSquare = 2.0f * jacobianRegularizationFactor * 
                                      jacobianRegularizationFactor;
      int32_t radius = (int32_t)( jacobianRegularizationFactor );

      if ( radius%2 == 0 )
        radius++;

      float sum = 0.0f;
      gkg::Volume< float > kernel( radius, radius, radius );

      int32_t i = 0;
      int32_t j = 0;
      int32_t k = 0;
      for( k = 0; k < radius; ++k )
      {

        for( j = 0; j < radius; ++j )
        {
        
          for( i = 0; i < radius; ++i )
          {

            kernel( i, j, k ) = std::exp( - ( i*i + j*j + k*k ) /
                                          twiceSigmaSquare );
            sum += kernel( i, j, k );

          }

        }

      }


      for( k = 0; k < radius; ++k )
      {

        for( j = 0; j < radius; ++j )
        {
        
          for( i = 0; i < radius; ++i )
          {

            kernel( i, j, k ) /= sum;

          }

        }

      }


      gkg::Convolution3d< float, float, float > convolution3d;   
      convolution3d.convolve( J11, kernel, J11regularized );
      convolution3d.convolve( J12, kernel, J12regularized );
      convolution3d.convolve( J13, kernel, J13regularized );
      convolution3d.convolve( J21, kernel, J21regularized );
      convolution3d.convolve( J22, kernel, J22regularized );
      convolution3d.convolve( J23, kernel, J23regularized );
      convolution3d.convolve( J31, kernel, J31regularized );
      convolution3d.convolve( J32, kernel, J32regularized );
      convolution3d.convolve( J33, kernel, J33regularized );
      

      std::cout << "Done." << std::endl;

      std::cout << "Performing reorientation of dMRI data" << std::endl;

      gkg::ReorientationContext reorientationContext(
           reorientationType, 
           maskedFloatingSites,
           maskedFloating,
           reference,
           referenceHasSiteBoolVector,
           J11regularized, J12regularized, J13regularized,
           J21regularized, J22regularized, J23regularized,
           J31regularized, J32regularized, J33regularized );
      

      gkg::ThreadedLoop< int32_t > threadedLoopReorientation( 
                                    &reorientationContext,
                                    0,
                                    maskedFloatingSitesSize );

      threadedLoopReorientation.launch();
      
      std::cout << "Done." << std::endl;

    }

    /*
    std::cout << "Writing reoriented ph warped image ...\n" << std::endl;

    gkg::Writer::getInstance().write( fileNameSiteMapDeformedFloating,
                                      maskedFloating.getSiteMap(),
                                      false,
                                      "sitemap" );
    gkg::Writer::getInstance().write( fileNameTextureMapDeformedFloating,
                                      maskedFloating.getItems(),
                                      false,
                                      "texturemap" );

    std::cout << "Done." << std::endl;
    */

    gkg::Volume< float > flowField( currentSizeX,
                                    currentSizeY,
                                    currentSizeZ, 6 );

    int32_t i = 0;
    int32_t j = 0;
    int32_t k = 0;
    for( k = 0; k < currentSizeZ; ++k )
    {
	
      for ( j = 0; j < currentSizeY; ++j )
      {

        for ( i = 0; i < currentSizeX; ++i )
        {

          flowField( i, j, k, 0 ) =  scaledDirectFlowFieldX( i, j, k );
          flowField( i, j, k, 1 ) =  scaledDirectFlowFieldY( i, j, k );
          flowField( i, j, k, 2 ) =  scaledDirectFlowFieldZ( i, j, k );
          flowField( i, j, k, 3 ) =  scaledInverseFlowFieldX( i, j, k );
          flowField( i, j, k, 4 ) =  scaledInverseFlowFieldY( i, j, k );
          flowField( i, j, k, 5 ) =  scaledInverseFlowFieldZ( i, j, k );

	}	
	
      }

    }



    gkg::Volume< float > jacobianVolume( currentSizeX,
                                         currentSizeY,
                                         currentSizeZ, 9 );

    i = 0;
    j = 0;
    k = 0;
    for( k = 0; k < currentSizeZ; ++k )
    {
	
      for ( j = 0; j < currentSizeY; ++j )
      {

        for ( i = 0; i < currentSizeX; ++i )
        {

          jacobianVolume( i, j, k, 0 ) =  J11regularized( i, j, k );
          jacobianVolume( i, j, k, 1 ) =  J12regularized( i, j, k );
          jacobianVolume( i, j, k, 2 ) =  J13regularized( i, j, k );
          jacobianVolume( i, j, k, 3 ) =  J21regularized( i, j, k );
          jacobianVolume( i, j, k, 4 ) =  J22regularized( i, j, k );
          jacobianVolume( i, j, k, 5 ) =  J23regularized( i, j, k );
          jacobianVolume( i, j, k, 6 ) =  J31regularized( i, j, k );
          jacobianVolume( i, j, k, 7 ) =  J32regularized( i, j, k );
          jacobianVolume( i, j, k, 8 ) =  J33regularized( i, j, k );

	}	
	
      }

    }



    std::cout << "Deformable Registration finished " << std::endl;

    std::cout << "Writing flow field ...\n" << std::endl;

    flowField.setResolution( resolution );
    gkg::Writer::getInstance().write( fileNameFlowField,
                                      flowField,
                                      false,
                                      "gis" );
    std::cout << "done" << std::endl;
    

    std::cout << "Writing jacobian volume ...\n" << std::endl;

    jacobianVolume.setResolution( resolution );
    gkg::Writer::getInstance().write( fileNameJacobianVolume,
                                      jacobianVolume,
                                      false,
                                      "gis" );
    std::cout << "done" << std::endl;

    /*
    std::cout << "Writing std warped image ...\n" << std::endl;

    gkg::Writer::getInstance().write( fileNameSiteMapDeformedFloating,
                                      warpedField1.getSiteMap(),
                                      false,
                                      "sitemap" );
    gkg::Writer::getInstance().write( fileNameTextureMapDeformedFloating,
                                      warpedField1.getItems(),
                                      false,
                                      "texturemap" );

    std::cout << "done" << std::endl;
    */

  }
  GKG_CATCH_COMMAND( result );

  return result;

}

