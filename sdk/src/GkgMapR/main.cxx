#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-io/TypedVolumeReaderProcess.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-io/TypedVolumeReaderProcess.h>
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
    gkg::Volume< float > warpedVolume2( sizeVector );
    warpedVolume2.fill( 0.0f );


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


      warpVolume( warpedVolume2, 
                  floating, 
                  reference, 
                  sizeVector, 
                  inverseFlowFieldX, 
                  inverseFlowFieldY, 
                  inverseFlowFieldZ, 
                  similarityEnergyBefore, 
                  similarityEnergyAfter );

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

      std::cout<<"============================================\n" << std::endl;
      std::cout<<"Level "<< level <<" grid=" << currentControlPointsSpacing
               <<" with sizes: "<< currentSizeX << "x"
                                << currentSizeY << "x"
                                << currentSizeZ
               <<" hw="<< currentHalfWidthSearchSpace
               <<" quant="<< currentSearchSpaceQuantisation << "\n";
      std::cout<<"============================================\n" << std::endl;

      // minimum spanning trees
      std::vector< int32_t > floatingOrderedMST( currentSize, 0 );
      std::vector< int32_t > floatingParentsMST( currentSize, 0 );

      primsGraph( floating, 
                  sizeVector,
                  floatingOrderedMST, 
                  floatingParentsMST, 
                  currentControlPointsSpacing );

      std::vector< int32_t > referenceOrderedMST( currentSize, 0 );
      std::vector< int32_t > referenceParentsMST( currentSize, 0 );

      primsGraph( reference, 
                  sizeVector,
                  referenceOrderedMST, 
                  referenceParentsMST, 
                  currentControlPointsSpacing );

      std::cout<<"============================================\n" << std::endl;
      std::cout << "Starting similarity computation\n";
      std::cout<<"============================================\n" << std::endl;

      // data-cost / similarity computation
      std::vector< float > totalCost1( cubedLength * currentSize );
      std::vector< float > totalCost2( cubedLength * currentSize );

      dataCost( floating, 
                warpedVolume1,
                sizeVector,
                totalCost1,
                regularizationWeighting,
                currentHalfWidthSearchSpace,
                currentControlPointsSpacing,
                currentSearchSpaceQuantisation );

      dataCost( reference, 
                warpedVolume2,
                sizeVector,
                totalCost2,
                regularizationWeighting,
                currentHalfWidthSearchSpace,
                currentControlPointsSpacing,
                currentSearchSpaceQuantisation );

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
      jacobian( scaledDirectFlowFieldX,
                scaledDirectFlowFieldY,
                scaledDirectFlowFieldZ, 
                currentSizeVector,
                currentControlPointsSpacing );

      //upsample deformations from grid-resolution to high-resolution (trilinear=1st-order spline)
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
    gkg::Writer::getInstance().write( "flowField",
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

