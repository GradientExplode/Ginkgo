#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-io/TypedVolumeReaderProcess.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-io/TypedVolumeReaderProcess.h>
#include <gkg-processing-process/Process.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-io/StringConverter.h>
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


#include "DiffeomorphicTransformations.h"
#include "MinimumSpanningTree.h"
#include "DataCostD.h"
#include "Regularization.h"
#include "SymmetricDiffeomorphism.h"

#define INF = 1e20
#define RAND_SAMPLES = 64
   

int main( int argc, char* argv[] )
{

  int result = EXIT_SUCCESS;

  try
  {

    std::string fileNameReference;
    std::string fileNameFloating;
    std::string fileNameDeformedFloating; 
    float regularizationWeighting = 0.0f;
    std::vector< int32_t > halfWidthSearchSpace;
    std::vector< int32_t > controlPointsSpacing;
    std::vector< float > searchSpaceQuantisation;
    bool verbose = false;



    gkg::Application application( argc, argv,
                                  "Diffeomorphic registration"
                                  "between scalar images" );
    application.addSingleOption( "-reference",
                                 "Reference file name",
                                 fileNameReference );
    application.addSingleOption( "-floating",
                                 "Floating file name",
                                 fileNameFloating );
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
    application.addSingleOption( "-output",
                                 "Deformed floating file name",
                                 fileNameDeformedFloating );

    application.initialize();

    ////////////////////////////////////////////////////////////////////////////
    //Initialise random variable
    ////////////////////////////////////////////////////////////////////////////
    std::srand( time( 0 ) );
    float randv= ( ( float ) std::rand() / float( RAND_MAX ) );
    timeval time1, time2, time1a, time2a;

    ////////////////////////////////////////////////////////////////////////////
    // reading reference object
    ////////////////////////////////////////////////////////////////////////////
    gkg::Volume< float > reference;

    gkg::TypedVolumeReaderProcess< float >
      typedReferenceVolumeReader( reference );
    typedReferenceVolumeReader.execute( fileNameReference );

    ////////////////////////////////////////////////////////////////////////////
    // reading floating object
    ////////////////////////////////////////////////////////////////////////////
    gkg::Volume< float > floating;
    gkg::TypedVolumeReaderProcess< float >
      typedFloatingVolumeReader( floating );
    typedFloatingVolumeReader.execute( fileNameFloating );

    ////////////////////////////////////////////////////////////////////////////
    // collecting size and resolution of input volumes
    ////////////////////////////////////////////////////////////////////////////
    int32_t sizeX = reference.getSizeX();
    int32_t sizeY = reference.getSizeY();
    int32_t sizeZ = reference.getSizeZ();
    gkg::Vector3d< int32_t > sizeVector( sizeX, sizeY, sizeZ );

    gkg::Vector3d< double > resolution( 0.0, 0.0, 0.0 );
    reference.getResolution( resolution );    
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

    gettimeofday(&time1a, NULL);

    ////////////////////////////////////////////////////////////////////////////
    // initialize warped images
    ////////////////////////////////////////////////////////////////////////////
    gkg::Volume< float > warpedVolume1( sizeVector );
    warpedVolume1.fill( 0.0f );
    warpedVolume1.setResolution( resolution );
    gkg::Volume< float > warpedVolume2( sizeVector );
    warpedVolume2.fill( 0.0f );
    warpedVolume2.setResolution( resolution );


    ////////////////////////////////////////////////////////////////////////////
    // matching process
    ////////////////////////////////////////////////////////////////////////////
    float similarityEnergyBefore = 0.0f;
    float similarityEnergyAfter = 0.0f;

    int32_t currentSizeX = 0;
    int32_t currentSizeY = 0;
    int32_t currentSizeZ = 0;
    int32_t currentSize = 0;

    int32_t levelCount = halfWidthSearchSpace.size() - 1;


    int32_t level = 0;
    for ( level = 0; level < levelCount; ++level ) 
    {

      //warp both high-resolution images according
      warpVolume( warpedVolume1, 
                  reference, 
                  floating, 
                  sizeVector, 
                  directFlowFieldX, 
                  directFlowFieldY, 
                  directFlowFieldZ, 
                  similarityEnergyBefore, 
                  similarityEnergyAfter );

      gkg::Writer::getInstance().write(
              std::string( "Kevin2/warpedVolume1_" ) + 
              gkg::StringConverter::toString( level ) + ".ima",
              warpedVolume1 );

      warpVolume( warpedVolume2, 
                  floating, 
                  reference, 
                  sizeVector, 
                  inverseFlowFieldX, 
                  inverseFlowFieldY, 
                  inverseFlowFieldZ, 
                  similarityEnergyBefore, 
                  similarityEnergyAfter );

      gkg::Writer::getInstance().write(
              std::string( "Kevin2/warpedVolume2_" ) + 
              gkg::StringConverter::toString( level ) + ".ima",
              warpedVolume2 );

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


      std::ofstream os( std::string( "Kevin2/1_" ) +
                        gkg::StringConverter::toString( level ) + ".txt" );
      os << "currentHalfWidthSearchSpace=" << currentHalfWidthSearchSpace
         << std::endl;
      os << "currentControlPointSpacing=" << currentControlPointsSpacing
         << std::endl;
      os << "currentSearchSpaceQuantisation=" << currentSearchSpaceQuantisation
         << std::endl;
      os << "currentControlPointVolumeSize=" << currentSizeVector
         << std::endl;
      os << "scaledSizeVector=" << scaledSizeVector
         << std::endl;
      os.close();


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

      gkg::Writer::getInstance().write(
              std::string( "Kevin2/coarserDirectFlowFieldAlongX" ) + 
              gkg::StringConverter::toString( level ) + ".ima",
              scaledDirectFlowFieldX );
      gkg::Writer::getInstance().write(
              std::string( "Kevin2/coarserDirectFlowFieldAlongY" ) + 
              gkg::StringConverter::toString( level ) + ".ima",
              scaledDirectFlowFieldY );
      gkg::Writer::getInstance().write(
              std::string( "Kevin2/coarserDirectFlowFieldAlongZ" ) + 
              gkg::StringConverter::toString( level ) + ".ima",
              scaledDirectFlowFieldZ );

      gkg::Writer::getInstance().write(
              std::string( "Kevin2/currentDirectFlowFieldAlongX" ) + 
              gkg::StringConverter::toString( level ) + ".ima",
              currentDirectFlowFieldX );
      gkg::Writer::getInstance().write(
              std::string( "Kevin2/currentDirectFlowFieldAlongY" ) + 
              gkg::StringConverter::toString( level ) + ".ima",
              currentDirectFlowFieldY );
      gkg::Writer::getInstance().write(
              std::string( "Kevin2/currentDirecteFlowFieldAlongZ" ) + 
              gkg::StringConverter::toString( level ) + ".ima",
              currentDirectFlowFieldZ );

      gkg::Writer::getInstance().write(
              std::string( "Kevin2/coarserInverseFlowFieldAlongX" ) + 
              gkg::StringConverter::toString( level ) + ".ima",
              scaledInverseFlowFieldX );
      gkg::Writer::getInstance().write(
              std::string( "Kevin2/coarserInverseFlowFieldAlongY" ) + 
              gkg::StringConverter::toString( level ) + ".ima",
              scaledInverseFlowFieldY );
      gkg::Writer::getInstance().write(
              std::string( "Kevin2/coarserInverseFlowFieldAlongZ" ) + 
              gkg::StringConverter::toString( level ) + ".ima",
              scaledInverseFlowFieldZ );

      gkg::Writer::getInstance().write(
              std::string( "Kevin2/currentInverseFlowFieldAlongX" ) + 
              gkg::StringConverter::toString( level ) + ".ima",
              currentInverseFlowFieldX );
      gkg::Writer::getInstance().write(
              std::string( "Kevin2/currentInverseFlowFieldAlongY" ) + 
              gkg::StringConverter::toString( level ) + ".ima",
              currentInverseFlowFieldY );
      gkg::Writer::getInstance().write(
              std::string( "Kevin2/currentInverseFlowFieldAlongZ" ) + 
              gkg::StringConverter::toString( level ) + ".ima",
              currentInverseFlowFieldZ );

      std::cout<<"============================================\n" << std::endl;
      std::cout<<"Level "<< level <<" grid=" << currentControlPointsSpacing
               <<" with sizes: "<< currentSizeX << "x"
                                << currentSizeY << "x"
                                << currentSizeZ
               <<" hw="<< currentHalfWidthSearchSpace
               <<" quant="<< currentSearchSpaceQuantisation << "\n";
      std::cout<<"============================================\n" << std::endl;

      // minimum spanning trees
      std::vector< int32_t > referenceOrderedMST( currentSize, 0 );
      std::vector< int32_t > referenceParentsMST( currentSize, 0 );

      primsGraph( reference, 
                  sizeVector,
                  referenceOrderedMST, 
                  referenceParentsMST, 
                  currentControlPointsSpacing );

      os.open( std::string( "Kevin2/2_" ) +
                        gkg::StringConverter::toString( level ) + ".txt" );
      for ( int32_t ii = 0; ii < currentSize; ii++ )
      {

        os << referenceOrderedMST[ ii ] << " "
           << referenceParentsMST[ ii ] << std::endl;

      }
      os.close();

      std::vector< int32_t > floatingOrderedMST( currentSize, 0 );
      std::vector< int32_t > floatingParentsMST( currentSize, 0 );

      primsGraph( floating, 
                  sizeVector,
                  floatingOrderedMST, 
                  floatingParentsMST, 
                  currentControlPointsSpacing );


      os.open( std::string( "Kevin2/3_" ) +
                        gkg::StringConverter::toString( level ) + ".txt" );
      for ( int32_t ii = 0; ii < currentSize; ii++ )
      {

        os << floatingOrderedMST[ ii ] << " "
           << floatingParentsMST[ ii ] << std::endl;

      }
      os.close();


      std::cout<<"============================================\n" << std::endl;
      std::cout << "Starting similarity computation\n";
      std::cout<<"============================================\n" << std::endl;

      // data-cost / similarity computation
      std::vector< float > totalCost1( cubedLength * currentSize );
      std::vector< float > totalCost2( cubedLength * currentSize );

      dataCost( reference, 
                warpedVolume2,
                sizeVector,
                totalCost2,
                regularizationWeighting,
                currentHalfWidthSearchSpace,
                currentControlPointsSpacing,
                currentSearchSpaceQuantisation );

      gkg::Writer::getInstance().write(
        std::string( "Kevin2/4_source_" ) +
        gkg::StringConverter::toString( level ) + ".ima",
        reference );
      gkg::Writer::getInstance().write(
        std::string( "Kevin2/4_destination_" ) +
        gkg::StringConverter::toString( level ) + ".ima",
        warpedVolume2 );

      os.open( std::string( "Kevin2/4_" ) +
                        gkg::StringConverter::toString( level ) + ".txt" );
      for ( int32_t ii = 0; ii < cubedLength * currentSize;
            ii++ )
      {

        os << ii << " : " << totalCost1[ ii ]
           << std::endl;

      }
      os.close();

      dataCost( floating, 
                warpedVolume1,
                sizeVector,
                totalCost1,
                regularizationWeighting,
                currentHalfWidthSearchSpace,
                currentControlPointsSpacing,
                currentSearchSpaceQuantisation );

      gkg::Writer::getInstance().write(
        std::string( "Kevin2/5_source_" ) +
        gkg::StringConverter::toString( level ) + ".ima",
        floating );
      gkg::Writer::getInstance().write(
        std::string( "Kevin2/5_destination_" ) +
        gkg::StringConverter::toString( level ) + ".ima",
        warpedVolume1 );

      os.open( std::string( "Kevin2/5_" ) +
                        gkg::StringConverter::toString( level ) + ".txt" );
      for ( int32_t ii = 0; ii < cubedLength * currentSize;
            ii++ )
      {

        os << ii << " : " << totalCost1[ ii ]
           << std::endl;

      }
      os.close();


      std::cout<<"============================================\n" << std::endl;
      std::cout<<"Start regularisation on MST\n";
      std::cout<<"============================================\n" << std::endl;


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
                      referenceOrderedMST,
                      referenceParentsMST,
                      sizeVector );

      gkg::Writer::getInstance().write(
        std::string( "Kevin2/10_coarserDirectFlowFieldAlongX_" ) +
        gkg::StringConverter::toString( level ) + ".ima",
        scaledInverseFlowFieldX );
      gkg::Writer::getInstance().write(
        std::string( "Kevin2/10_coarserDirectFlowFieldAlongY_" ) +
        gkg::StringConverter::toString( level ) + ".ima",
        scaledInverseFlowFieldY );
      gkg::Writer::getInstance().write(
        std::string( "Kevin2/10_coarserDirectFlowFieldAlongZ_" ) +
        gkg::StringConverter::toString( level ) + ".ima",
        scaledInverseFlowFieldZ );

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
                      floatingOrderedMST,
                      floatingParentsMST,
                      sizeVector );


      gkg::Writer::getInstance().write(
        std::string( "Kevin2/11_coarserInverseFlowFieldAlongX_" ) +
        gkg::StringConverter::toString( level ) + ".ima",
        scaledDirectFlowFieldX );
      gkg::Writer::getInstance().write( 
        std::string( "Kevin2/11_coarserInverseFlowFieldAlongY_" ) +
        gkg::StringConverter::toString( level ) + ".ima",
        scaledDirectFlowFieldY );
      gkg::Writer::getInstance().write( 
        std::string( "Kevin2/11_coarserInverseFlowFieldAlongZ_" ) +
        gkg::StringConverter::toString( level ) + ".ima",
        scaledDirectFlowFieldZ );

      //make transformations diffeomorphic using scaling-and-squaring

      diffeomorphic( scaledInverseFlowFieldX,
                     scaledInverseFlowFieldY,
                     scaledInverseFlowFieldZ,
                     currentSizeVector,
                     4,
                     currentControlPointsSpacing );

      gkg::Writer::getInstance().write(
        std::string( "Kevin2/12_coarserDirectFlowFieldAlongX_" ) +
        gkg::StringConverter::toString( level ) + ".ima",
        scaledInverseFlowFieldX );
      gkg::Writer::getInstance().write(
        std::string( "Kevin2/12_coarserDirectFlowFieldAlongY_" ) +
        gkg::StringConverter::toString( level ) + ".ima",
        scaledInverseFlowFieldY );
      gkg::Writer::getInstance().write(
        std::string( "Kevin2/12_coarserDirectFlowFieldAlongZ_" ) +
        gkg::StringConverter::toString( level ) + ".ima",
        scaledInverseFlowFieldZ );

      diffeomorphic( scaledDirectFlowFieldX,
                     scaledDirectFlowFieldY,
                     scaledDirectFlowFieldZ,
                     currentSizeVector,
                     4,
                     currentControlPointsSpacing );


     gkg::Writer::getInstance().write(
        std::string( "Kevin2/13_coarserInverseFlowFieldAlongX_" ) +
        gkg::StringConverter::toString( level ) + ".ima",
        scaledDirectFlowFieldX );
      gkg::Writer::getInstance().write( 
        std::string( "Kevin2/13_coarserInverseFlowFieldAlongY_" ) +
        gkg::StringConverter::toString( level ) + ".ima",
        scaledDirectFlowFieldY );
      gkg::Writer::getInstance().write( 
        std::string( "Kevin2/13_coarserInverseFlowFieldAlongZ_" ) +
        gkg::StringConverter::toString( level ) + ".ima",
        scaledDirectFlowFieldZ );


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

      gkg::Writer::getInstance().write(
        std::string( "Kevin2/14_coarserDirectFlowFieldAlongX_" ) +
        gkg::StringConverter::toString( level ) + ".ima",
        scaledInverseFlowFieldX );
      gkg::Writer::getInstance().write(
        std::string( "Kevin2/14_coarserDirectFlowFieldAlongY_" ) +
        gkg::StringConverter::toString( level ) + ".ima",
        scaledInverseFlowFieldY );
      gkg::Writer::getInstance().write(
        std::string( "Kevin2/14_coarserDirectFlowFieldAlongZ_" ) +
        gkg::StringConverter::toString( level ) + ".ima",
        scaledInverseFlowFieldZ );

      gkg::Writer::getInstance().write(
        std::string( "Kevin2/15_coarserInverseFlowFieldAlongX_" ) +
        gkg::StringConverter::toString( level ) + ".ima",
        scaledDirectFlowFieldX );
      gkg::Writer::getInstance().write(
        std::string( "Kevin2/15_coarserInverseFlowFieldAlongY_" ) +
        gkg::StringConverter::toString( level ) + ".ima",
        scaledDirectFlowFieldY );
      gkg::Writer::getInstance().write(
        std::string( "Kevin2/15_coarserInverseFlowFieldAlongZ_" ) +
        gkg::StringConverter::toString( level ) + ".ima",
        scaledDirectFlowFieldZ );


      //calculate Jacobian determinant
      jacobian( scaledDirectFlowFieldX,
                scaledDirectFlowFieldY,
                scaledDirectFlowFieldZ, 
                currentSizeVector,
                currentControlPointsSpacing );

      //upsample deformations from grid-resolution to high-resolution (trilinear=1st-order spline)
      upsampleDeformations( inverseFlowFieldX,
                            inverseFlowFieldY,
                            inverseFlowFieldZ,
                            scaledInverseFlowFieldX,
                            scaledInverseFlowFieldY,
                            scaledInverseFlowFieldZ,
                            sizeVector,
                            currentSizeVector ); 

      gkg::Writer::getInstance().write(
        std::string( "Kevin2/16_directFlowFieldAlongX_" ) +
        gkg::StringConverter::toString( level ) + ".ima",
        inverseFlowFieldX );
      gkg::Writer::getInstance().write(
        std::string( "Kevin2/16_dDirectFlowFieldAlongY_" ) +
        gkg::StringConverter::toString( level ) + ".ima",
        inverseFlowFieldY );
      gkg::Writer::getInstance().write(
        std::string( "Kevin2/16_directFlowFieldAlongZ_" ) +
        gkg::StringConverter::toString( level ) + ".ima",
        inverseFlowFieldZ );

      upsampleDeformations( directFlowFieldX,
                            directFlowFieldY,
                            directFlowFieldZ,
                            scaledDirectFlowFieldX,
                            scaledDirectFlowFieldY,
                            scaledDirectFlowFieldZ,
                            sizeVector,
                            currentSizeVector );   


      gkg::Writer::getInstance().write(
        std::string( "Kevin2/17_inverseFlowFieldAlongX_" ) +
        gkg::StringConverter::toString( level ) + ".ima",
        directFlowFieldX );
      gkg::Writer::getInstance().write(
        std::string( "Kevin2/17_iInverseFlowFieldAlongY_" ) +
        gkg::StringConverter::toString( level ) + ".ima",
        directFlowFieldY );
      gkg::Writer::getInstance().write(
        std::string( "Kevin2/17_inverseFlowFieldAlongZ_" ) +
        gkg::StringConverter::toString( level ) + ".ima",
        directFlowFieldZ );


      float energy = harmonicEnergy( directFlowFieldX, 
                                     directFlowFieldY,
                                     directFlowFieldZ, 
                                     sizeVector );
      std::cout << "Harmonic energy of deformation field: " << energy << "\n";
		
      warpVolume( warpedVolume1, 
                  reference, 
                  floating, 
                  sizeVector, 
                  directFlowFieldX, 
                  directFlowFieldY, 
                  directFlowFieldZ, 
                  similarityEnergyBefore, 
                  similarityEnergyAfter );

      std::cout << "Similarity energy before registration: " 
                << similarityEnergyBefore 
                <<" and after " 
                << similarityEnergyAfter << "\n";

      scaledSizeVector.x = currentSizeVector.x;
      scaledSizeVector.y = currentSizeVector.y;
      scaledSizeVector.z = currentSizeVector.z;

      std::cout<<"\n";

    }

    gkg::Volume< float > flowField( currentSizeX,
                                    currentSizeY,
                                    currentSizeZ, 3 );

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

	}	
	
      }

    }

    std::cout << "Deformable Registration finished " << std::endl;

    std::cout << "Writing flow field ...\n" << std::endl;

    flowField.setResolution( resolution );
    gkg::Writer::getInstance().write( "Kevin2/flowField",
                                      flowField,
                                      false,
                                      "ima" );
    std::cout << "done" << std::endl;

    std::cout << "Writing warped image ...\n" << std::endl;

    warpedVolume1.setResolution( resolution );
    gkg::Writer::getInstance().write( fileNameDeformedFloating,
                                      warpedVolume1,
                                      false,
                                      "ima" );

    std::cout << "done" << std::endl;

  }
  GKG_CATCH_COMMAND( result );

  return result;

}

