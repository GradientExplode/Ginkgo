#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-io/TypedVolumeReaderProcess.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-io/TypedVolumeReaderProcess.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-registration/SimilarityMeasureFactory_i.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-transform/InverseTransform3d.h>
#include <gkg-processing-transform/ParameterizedRigidTransform3d.h>
#include <gkg-processing-transform/VectorFieldTransform3d.h>
#include <gkg-processing-transform/NonLinearTransform3d.h>
#include <gkg-processing-algobase/MinimumIndexFilter_i.h>
#include <gkg-processing-algobase/Scaler_i.h>
#include <gkg-processing-algobase/SumOfSquareFilter_i.h>
#include <gkg-processing-resampling/ResamplerFactory.h>
#include <gkg-core-io/StringConverter.h>
#include <gkg-core-cppext/Limits.h>
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


#include "MinimumSpanningTree_i.h"

#define FAST_INVERSE_TRANSFORM_MAXIMUM_ITERATION_COUNT 10

void computingDisplacementCosts(
              const gkg::Volume< uint8_t >& sourceVolume,
              const gkg::Volume< uint8_t >& transformedVolume,
              int32_t currentHalfWidthSearchSpace,
              int32_t currentControlPointSpacing,
              int32_t currentSearchSpaceQuantisation,
              float currentRegularizationWeight,
              std::vector< float >& displacementCosts );
void attributeOptimalDisplacements(
                 const gkg::Volume< float >& currentFlowFieldAlongX,
                 const gkg::Volume< float >& currentFlowFieldAlongY,
                 const gkg::Volume< float >& currentFlowFieldAlongZ,
                 std::vector< float >& displacementCosts,
                 int32_t currentHalfWidthSearchSpace,
                 int32_t currentSearchSpaceQuantisation,
                 const gkg::MinimumSpanningTree< uint8_t >& minimumSpanningTree,
                 gkg::Volume< float >& coarserFlowFieldAlongX,
                 gkg::Volume< float >& coarserFlowFieldAlongY,
                 gkg::Volume< float >& coarserFlowFieldAlongZ );
void getDisplacements(  
                    int32_t currentSearchSpaceSize,
                    int32_t currentSearchSpaceVoxelCount,
                    int32_t currentHalfWidthSearchSpace,
                    int32_t currentSearchSpaceQuantisation,
                    std::vector< gkg::Vector3d< float > >& displacements );
void fastDistanceTransform( int32_t currentSearchSpaceSize,
                            const gkg::Vector3d< float >& deltaDisplacement,
                            std::vector< float >& costs,
                            std::vector< int32_t >& displacementIndices );
void fastDistanceTransformSquare(
                             int32_t index,
                             int32_t length,
                             float displacement,
                             int32_t offsetToNextElement,
                             std::vector< int32_t >& v,
                             std::vector< float >& z,
                             std::vector< float >& f,
                             std::vector< int32_t >& ind1,
                             std::vector< float >& costs,
                             std::vector< int32_t >& displacementIndices );
void getInterpolation3d( const gkg::Volume< float >& flowField,
                         const gkg::Volume< float >& flowFieldAlongX,
                         const gkg::Volume< float >& flowFieldAlongY,
                         const gkg::Volume< float >& flowFieldAlongZ,
                         gkg::Volume< float >& flowFieldPrime );
void combineDeformations3d( gkg::Volume< float >& flowFieldAlongX,
                            gkg::Volume< float >& flowFieldAlongY,
                            gkg::Volume< float >& flowFieldAlongZ,
                            gkg::Volume< float >& flowFieldAlongXPrime,
                            gkg::Volume< float >& flowFieldAlongYPrime,
                            gkg::Volume< float >& flowFieldAlongZPrime );
void fastInverseTransform( const gkg::Volume< float >& directFlowFieldAlongX,
                           const gkg::Volume< float >& directFlowFieldAlongY,
                           const gkg::Volume< float >& directFlowFieldAlongZ,
                           gkg::Volume< float >& inverseFlowFieldAlongX,
                           gkg::Volume< float >& inverseFlowFieldAlongY,
                           gkg::Volume< float >& inverseFlowFieldAlongZ );
void computeGradientAlongX( const gkg::Volume< float >& flowField,
                            gkg::Volume< float >& flowFieldGradientAlongX );
void computeGradientAlongY( const gkg::Volume< float >& flowField,
                            gkg::Volume< float >& flowFieldGradientAlongY );
void computeGradientAlongZ( const gkg::Volume< float >& flowField,
                            gkg::Volume< float >& flowFieldGradientAlongZ );
void scaleAndSquareTransformation( gkg::Volume< float >& flowFieldAlongX,
                                   gkg::Volume< float >& flowFieldAlongY,
                                   gkg::Volume< float >& flowFieldAlongZ,
                                   int32_t currentControlPointSpacing,
                                   int32_t exponentiationStepCount );
void symmetrizeDirectAndInverseTransformations(
                                   gkg::Volume< float >& directFlowFieldAlongX,
                                   gkg::Volume< float >& directFlowFieldAlongY,
                                   gkg::Volume< float >& directFlowFieldAlongZ,
                                   gkg::Volume< float >& inverseFlowFieldAlongX,
                                   gkg::Volume< float >& inverseFlowFieldAlongY,
                                   gkg::Volume< float >& inverseFlowFieldAlongZ,
                                   int32_t currentControlPointSpacing );
void computeJacobianDeterminant( const gkg::Volume< float >& flowFieldAlongX,
                                 const gkg::Volume< float >& flowFieldAlongY,
                                 const gkg::Volume< float >& flowFieldAlongZ,
                                 int32_t currentControlPointSpacing,
                                 float& jacobianMean,
                                 float& jacobianStandardDeviation,
                                 float& jacobianMinimum,
                                 float& jacobianMaximum,
                                 float& jacobianNegativeRatio );
float computeHarmonicEnergy( const gkg::Volume< float >& flowFieldAlongX,
                             const gkg::Volume< float >& flowFieldAlongY,
                             const gkg::Volume< float >& flowFieldAlongZ );

static int32_t computingDisplacementCostsIndex = 6;
void computingDisplacementCosts(
              const gkg::Volume< uint8_t >& sourceVolume,
              const gkg::Volume< uint8_t >& transformedVolume,
              int32_t currentHalfWidthSearchSpace,
              int32_t currentControlPointSpacing,
              int32_t currentSearchSpaceQuantisation,
              float currentRegularizationWeight,
              std::vector< float >& displacementCosts )
{

  try
  {



    gkg::Vector3d< int32_t > sourceVolumeSize;
    sourceVolume.getSize( sourceVolumeSize );

    gkg::Vector3d< double > sourceVolumeResolution;
    sourceVolume.getResolution( sourceVolumeResolution );

    gkg::BoundingBox< int32_t > sourceBoundingBox( sourceVolume );

    gkg::Vector3d< int32_t > 
      currentControlPointVolumeSize = sourceVolumeSize /
                                      currentControlPointSpacing;

    int32_t currentControlPointCount = currentControlPointVolumeSize.x *
                                       currentControlPointVolumeSize.y *
                                       currentControlPointVolumeSize.z;

    int32_t currentSearchSpaceSize = 2 * currentHalfWidthSearchSpace + 1;
    int32_t currentSearchSpaceVoxelCount = currentSearchSpaceSize *
                                           currentSearchSpaceSize *
                                           currentSearchSpaceSize;

    std::vector< gkg::Vector3d< float > > displacements;
    getDisplacements( currentSearchSpaceSize,
                            currentSearchSpaceVoxelCount,
                            currentHalfWidthSearchSpace,
                            currentSearchSpaceQuantisation,
                            displacements );

    std::ofstream os( std::string( "Cyril/" ) +
                      gkg::StringConverter::toString(
                                             computingDisplacementCostsIndex ) +
                      "_1.txt" );

    std::vector< gkg::Vector3d< float > >::const_iterator
      dd = displacements.begin(),
      dde = displacements.end();
    while ( dd != dde )
    {

      os << dd->x << " "
         << dd->y << " "
         << dd->z << std::endl;
         
      ++ dd;

    }

    os.close();


    os.open( std::string( "Cyril/" ) +
                      gkg::StringConverter::toString(
                                             computingDisplacementCostsIndex ) +
                      "_2.txt" );

    int32_t currentHalfWidthSearchSpaceBis = currentHalfWidthSearchSpace *
                                             currentSearchSpaceQuantisation;
    os << "currentHalfWidthSearchSpaceBis=" << currentHalfWidthSearchSpaceBis << std::endl;

    bool useRandomMode = false;
    int32_t maximumSampleCount = currentControlPointSpacing *
                                 currentControlPointSpacing *
                                 currentControlPointSpacing;
    if ( maximumSampleCount > 64 )
    {

      useRandomMode = true;
      maximumSampleCount = 64;

    } 


    os << "useRandomMode=" << useRandomMode << std::endl;
    os << "maximumSampleCount=" << maximumSampleCount << std::endl;

    float totalRegularizationWeight = 
                               ( ( float )currentControlPointSpacing /
                                 ( currentRegularizationWeight *
                                   ( float )currentSearchSpaceQuantisation ) ) /
                               ( float )maximumSampleCount;
    os << "totalRegularizationWeight=" << totalRegularizationWeight << std::endl;
    std::vector< float > unregularizedDisplacementCosts(
                                                   currentSearchSpaceVoxelCount,
                                                   0.0f );


    os.close();


    int32_t cx = 0;
    int32_t cy = 0;
    int32_t cz = 0;
    int32_t x = 0;
    int32_t y = 0;
    int32_t z = 0;
    bool isWithinBoundaries = true;
    int32_t s = 0;

    int32_t spx = 0;
    int32_t spy = 0;
    int32_t spz = 0;

    int32_t nx = 0;
    int32_t ny = 0;
    int32_t nz = 0;

    int32_t tx = 0;
    int32_t ty = 0;
    int32_t tz = 0;

    int32_t d = 0;

    int32_t currentControlPointIndex = 0;


    std::cout << std::endl;
    for ( cz = 0; cz < currentControlPointVolumeSize.z; cz++ )
    {

      for ( cy = 0; cy < currentControlPointVolumeSize.y; cy++ )
      {

        for ( cx = 0; cx < currentControlPointVolumeSize.x; cx++ )
        {

          // computing coordinates of point in the source image
          x = cx * currentControlPointSpacing;
          y = cy * currentControlPointSpacing;
          z = cz * currentControlPointSpacing;



          // checking whether the control point plus its search space lie
          // within the source bounding box
          isWithinBoundaries = true;
          if ( !sourceBoundingBox.contains( x + currentControlPointSpacing - 1 +
                                            currentHalfWidthSearchSpaceBis,
                                            y + currentControlPointSpacing - 1 +
                                            currentHalfWidthSearchSpaceBis,
                                            z + currentControlPointSpacing - 1 +
                                            currentHalfWidthSearchSpaceBis ) )
          {

            isWithinBoundaries = false;

          }
          if ( !sourceBoundingBox.contains( x - currentHalfWidthSearchSpaceBis,
                                            y - currentHalfWidthSearchSpaceBis,
                                            z - currentHalfWidthSearchSpaceBis ) )
          {

            isWithinBoundaries = false;

          }


          // resetting the displacement costs to nul
          for ( s = 0; s < currentSearchSpaceVoxelCount; s++ )
          {

            unregularizedDisplacementCosts[ s ] = 0.0f;

          }

          // looping over samples
          // in case of active random mode, randomly picking up samples 
          if ( useRandomMode )
          {

            for ( s = 0; s < maximumSampleCount; s++ )
            {

              // randomly selecting and building the neighbor voxel
              spx = ( int32_t )( ( float )std::rand() *
                                ( float )currentControlPointSpacing /
                                ( float )RAND_MAX );
              spy = ( int32_t )( ( float )std::rand() *
                                ( float )currentControlPointSpacing /
                                ( float )RAND_MAX );
              spz = ( int32_t )( ( float )std::rand() *
                                ( float )currentControlPointSpacing /
                                ( float )RAND_MAX );

              nx = x + spx;
              ny = y + spy;
              nz = z + spz;


              // computing the unregularized energy cost for all the possible
              // displacements 
              for ( d = 0; d < currentSearchSpaceVoxelCount; d++ )
              {

                tx = nx + ( int32_t )displacements[ d ].x;
                ty = ny + ( int32_t )displacements[ d ].y;
                tz = nz + ( int32_t )displacements[ d ].z;


                if ( !isWithinBoundaries )
                {

                  tx = std::max( std::min( tx, sourceVolumeSize.x - 1 ), 0 );
                  ty = std::max( std::min( ty, sourceVolumeSize.y - 1 ), 0 );
                  tz = std::max( std::min( tz, sourceVolumeSize.z - 1 ), 0 );

                }

                unregularizedDisplacementCosts[ d ] += 
                                    std::abs( ( float )sourceVolume( nx, ny, nz ) -
                                              ( float )transformedVolume( tx, ty, tz ) );

              }

            }

          }
          // else going through all samples
          else
          {

            for ( spz = 0; spz < currentControlPointSpacing; spz++ )
            {

              for ( spy = 0; spy < currentControlPointSpacing; spy++ )
              {

                for ( spx = 0; spx < currentControlPointSpacing; spx++ )
                {

                  nx = x + spx;
                  ny = y + spy;
                  nz = z + spz;

                  // computing the unregularized energy cost for all the 
                  // possible displacements 
                  for ( d = 0; d < currentSearchSpaceVoxelCount; d++ )
                  {

                    tx = nx + ( int32_t )displacements[ d ].x;
                    ty = ny + ( int32_t )displacements[ d ].y;
                    tz = nz + ( int32_t )displacements[ d ].z;

                    if ( !isWithinBoundaries )
                    {

                      tx = std::max( std::min( tx, sourceVolumeSize.x - 1 ),
                                     0 );
                      ty = std::max( std::min( ty, sourceVolumeSize.y - 1 ),
                                     0 );
                      tz = std::max( std::min( tz, sourceVolumeSize.z - 1 ),
                                     0 );

                    }

                    unregularizedDisplacementCosts[ d ] += 
                           std::abs( ( float )sourceVolume( nx, ny, nz ) -
                                     ( float )transformedVolume( tx, ty, tz ) );

                  }

                }

              }

            }

          }

          // regularizing and storing the displacement costs in the output 
          // container for the current control point
          for ( d = 0; d < currentSearchSpaceVoxelCount; d++ )
          {

            displacementCosts[ currentControlPointIndex + 
                               currentControlPointCount * d ] =
              totalRegularizationWeight * unregularizedDisplacementCosts[ d ];

          }

          ++ currentControlPointIndex;

        }

      }

    }


    gkg::Volume< float >
      displacementCostsVolume( currentControlPointVolumeSize,
                               currentSearchSpaceVoxelCount );
    gkg::Volume< float >::iterator dout = displacementCostsVolume.begin();
    std::vector< float >::const_iterator din = displacementCosts.begin(),
                                         dine = displacementCosts.end();
    while ( din != dine )
    {

      *dout = *din;
      ++ dout;
      ++ din;

    }
    gkg::Writer::getInstance().write(

                      std::string( "Cyril/" ) +
                      gkg::StringConverter::toString(
                                             computingDisplacementCostsIndex ) +
                      "_3.ima",
                      displacementCostsVolume );

++computingDisplacementCostsIndex;
  }
  GKG_CATCH( "void computingDisplacementCosts( "
             "const gkg::RCPointer< gkg::Volume< uint8_t > >& sourceVolume, "
             "const gkg::RCPointer< gkg::Volume< uint8_t > >& "
             "transformedVolume, "
             "int32_t currentHalfWidthSearchSpace, "
             "int32_t currentControlPointSpacing, "
             "int32_t currentSearchSpaceQuantisation, "
             "float currentRegularizationWeight, "
             "std::vector< float >& displacementCosts )" );


}


static int32_t attributeOptimalDisplacementsIndex = 8;
void attributeOptimalDisplacements(
                 const gkg::Volume< float >& currentFlowFieldAlongX,
                 const gkg::Volume< float >& currentFlowFieldAlongY,
                 const gkg::Volume< float >& currentFlowFieldAlongZ,
                 std::vector< float >& displacementCosts,
                 int32_t currentHalfWidthSearchSpace,
                 int32_t currentSearchSpaceQuantisation,
                 const gkg::MinimumSpanningTree< uint8_t >& minimumSpanningTree,
                 gkg::Volume< float >& coarserFlowFieldAlongX,
                 gkg::Volume< float >& coarserFlowFieldAlongY,
                 gkg::Volume< float >& coarserFlowFieldAlongZ )
{

  try
  {

    gkg::Vector3d< int32_t > currentFlowFieldSize;
    currentFlowFieldAlongX.getSize( currentFlowFieldSize );

    int32_t currentFlowFieldPointCount = currentFlowFieldSize.x *
                                         currentFlowFieldSize.y *
                                         currentFlowFieldSize.z;

    int32_t currentSearchSpaceSize = 2 * currentHalfWidthSearchSpace + 1;
    int32_t currentSearchSpaceVoxelCount = currentSearchSpaceSize *
                                           currentSearchSpaceSize *
                                           currentSearchSpaceSize;

    std::vector< gkg::Vector3d< float > > displacements;
    getDisplacements( currentSearchSpaceSize,
                      currentSearchSpaceVoxelCount,
                      currentHalfWidthSearchSpace,
                      currentSearchSpaceQuantisation,
                      displacements );


    std::ofstream os( std::string( "Cyril/" ) +
                      gkg::StringConverter::toString(
                                         attributeOptimalDisplacementsIndex ) +
                      "_1.txt" );

                                             currentSearchSpaceQuantisation;
    os << "currentFlowFieldSize=" << currentFlowFieldSize << std::endl;
    os << "currentFlowFieldPointCount=" << currentFlowFieldPointCount << std::endl;
    os << "currentSearchSpaceSize=" << currentSearchSpaceSize << std::endl;
    os << "currentSearchSpaceVoxelCount=" << currentSearchSpaceVoxelCount << std::endl;
    os << "displacements=" << std::endl;
    for ( int32_t dd = 0; dd < ( int32_t )displacements.size(); dd++ )
    {

      os << displacements[ dd ] << std::endl;

    }
    os.close();


    std::vector< float > costs( currentSearchSpaceVoxelCount, 0.0f );
    std::vector< float >::iterator
      cc = costs.begin(),
      cce = costs.end();
    while ( cc != cce )
    {

      *cc = 0.0f;
      ++cc;

    }
    std::vector< int32_t > displacementIndices( currentSearchSpaceVoxelCount,
                                                0 );
    std::vector< int32_t >::iterator
      dd = displacementIndices.begin(),
      dde = displacementIndices.end();
    while ( dd != dde )
    {

      *dd = 0;
      ++dd;

    }
    std::vector< int32_t > allDisplacementIndices( currentFlowFieldPointCount *
                                                   currentSearchSpaceVoxelCount,
                                                   0 );
    std::vector< int32_t >::iterator
      ad = allDisplacementIndices.begin(),
      ade = allDisplacementIndices.end();
    while ( ad != ade )
    {

      *ad = 0;
      ++ad;

    }

    os.open( std::string( "Cyril/" ) +
                      gkg::StringConverter::toString(
                                         attributeOptimalDisplacementsIndex ) +
                      "_2.txt" );

    int32_t d = 0;
    int32_t index = currentFlowFieldPointCount - 1;
    int32_t childNodeIndex = 0;
    int32_t parentNodeIndex = 0;
    gkg::Vector3d< float > deltaDisplacement;
    while ( index )
    {

      // collecting the child node index
      childNodeIndex = minimumSpanningTree.getNodeIndex( index );

      // collecting the voxel corresponding to the current node index
      const gkg::Vector3d< int32_t >& 
        childVoxel = minimumSpanningTree.getVoxelFromIndex( childNodeIndex );

      // collecting the parent node index of the child
      parentNodeIndex = minimumSpanningTree.getParentNodeIndex( childNodeIndex );

      // collecting the voxel corresponding to the current node index
      const gkg::Vector3d< int32_t >& 
        parentVoxel = minimumSpanningTree.getVoxelFromIndex( parentNodeIndex );

      os << "childNodeIndex=" << childNodeIndex << std::endl;
      os << "childVoxel=" << childVoxel << std::endl;
      os << "parentNodeIndex=" << parentNodeIndex << std::endl;
      os << "parentVoxel=" << parentVoxel << std::endl;

      os << "costs=" << std::flush;

      // initializing the cost from the previous cost
      for ( d = 0; d < currentSearchSpaceVoxelCount; d++ )
      {

        costs[ d ] = displacementCosts[ childNodeIndex +
                                        d * currentFlowFieldPointCount ];
        os << costs[ d ] << " ";

      }
      os << std::endl;

      // computing the displacement residual beetwen parent and child voxel(s)
      deltaDisplacement.x = ( currentFlowFieldAlongX( parentVoxel ) -
                              currentFlowFieldAlongX( childVoxel ) ) /
                            ( float )currentSearchSpaceQuantisation;
      deltaDisplacement.y = ( currentFlowFieldAlongY( parentVoxel ) -
                              currentFlowFieldAlongY( childVoxel ) ) /
                            ( float )currentSearchSpaceQuantisation;
      deltaDisplacement.z = ( currentFlowFieldAlongZ( parentVoxel ) -
                              currentFlowFieldAlongZ( childVoxel ) ) /
                            ( float )currentSearchSpaceQuantisation;

      os << "deltaDisplacement=" << deltaDisplacement << std::endl;


      // fast distance transform
      fastDistanceTransform( currentSearchSpaceSize,
                             deltaDisplacement,
                             costs,
                             displacementIndices );

      os << "displacementIndices=" << std::flush;
      for (int32_t dd = 0; dd < ( int32_t )displacementIndices.size(); dd++ )
      {

        os << displacementIndices[ dd ] << " ";

      }
      os << std::endl;


      // computing the argmin for each displacement (=label)
      for ( d = 0; d < currentSearchSpaceVoxelCount; d++ )
      {

        allDisplacementIndices[ childNodeIndex +
                                d * currentFlowFieldPointCount ] =
          displacementIndices[ d ];

      }

      for ( d = 0; d < currentSearchSpaceVoxelCount; d++ )
      {

        // adding mincost to the parent node
        displacementCosts[ parentNodeIndex +
                           d * currentFlowFieldPointCount ] +=
          costs[ d ];

      }

      -- index;

    }

    os.close();


    os.open( std::string( "Cyril/" ) +
                      gkg::StringConverter::toString(
                                         attributeOptimalDisplacementsIndex ) +
                      "_3.txt" );

    // computing the MST cost and selecting the displacement for the root node
    int32_t rootNodeIndex = minimumSpanningTree.getNodeIndex( 0 );
    const gkg::Vector3d< int32_t >& 
      rootVoxel = minimumSpanningTree.getVoxelFromIndex( rootNodeIndex );

    os << "rootNodeIndex=" << rootNodeIndex << std::endl;
    os << "rootVoxel=" << rootVoxel << std::endl;

    os << "costs=" << std::flush; 
    for ( d = 0; d < currentSearchSpaceVoxelCount; d++ )
    {

      costs[ d ] = displacementCosts[ rootNodeIndex +
                                      d * currentFlowFieldPointCount ];
      os << costs[ d ] << " ";
      allDisplacementIndices[ rootNodeIndex +
                              d * currentFlowFieldPointCount ] = d;

    }
    os << std::endl;

    // looking for the index corresponding to the minimum value
    int32_t minimumDisplacementIndex = 0;
    gkg::MinimumIndexFilter< std::vector< float >, int32_t >
      minimumIndexFilter;
    minimumIndexFilter.filter( costs, minimumDisplacementIndex );

    os << "minimumDisplacementIndex=" << minimumDisplacementIndex << std::endl; 

    std::vector< int32_t > 
                        selectedDisplacementIndices( 
                                             currentFlowFieldPointCount, 0 );
    std::vector< int32_t >::iterator
      sd = selectedDisplacementIndices.begin(),
      sde = selectedDisplacementIndices.end();
    while ( sd != sde )
    {

      *sd = 0;
      ++sd;

    }

    selectedDisplacementIndices[ rootNodeIndex ] = minimumDisplacementIndex;

    coarserFlowFieldAlongX( rootVoxel ) =
                                    currentFlowFieldAlongX( rootVoxel ) +
                                    displacements[ minimumDisplacementIndex ].x;
    coarserFlowFieldAlongY( rootVoxel ) =
                                    currentFlowFieldAlongY( rootVoxel ) +
                                    displacements[ minimumDisplacementIndex ].y;
    coarserFlowFieldAlongZ( rootVoxel ) =
                                    currentFlowFieldAlongZ( rootVoxel ) +
                                    displacements[ minimumDisplacementIndex ].z;

    os.close();

    os.open( std::string( "Cyril/" ) +
                      gkg::StringConverter::toString(
                                         attributeOptimalDisplacementsIndex ) +
                      "_4.txt" );

    os << "currentFlowFieldPointCount=" << currentFlowFieldPointCount << std::endl;
    for ( index = 1; index < currentFlowFieldPointCount; index++ )
    {

      //os << "index=" << index << std::endl;
      // collecting the child node index
      childNodeIndex = minimumSpanningTree.getNodeIndex( index );

      os << "childNodeIndex=" << childNodeIndex << std::endl;

      // collecting the parent node index of the child
      parentNodeIndex = minimumSpanningTree.getParentNodeIndex( childNodeIndex );

      os << "parentNodeIndex=" << parentNodeIndex << std::endl;

      os << "selectedDisplacementIndices[ parentNodeIndex ]="
         << selectedDisplacementIndices[ parentNodeIndex ] <<std::endl;

      // computing the displacement index corresponding to the minimum cost
      minimumDisplacementIndex =
       allDisplacementIndices[ childNodeIndex +
                               selectedDisplacementIndices[ parentNodeIndex ] * 
                               currentFlowFieldPointCount ];

      os << "minimumDisplacementIndex=" << minimumDisplacementIndex << std::endl;

      // storing the displacement index corresponding to the minimum cost
      selectedDisplacementIndices[ childNodeIndex ] = minimumDisplacementIndex;

      // collecting the voxel corresponding to the current node index
      const gkg::Vector3d< int32_t >& 
        childVoxel = minimumSpanningTree.getVoxelFromIndex( childNodeIndex );

      os << "childVoxel=" << childVoxel << std::endl;

      os << "displacements[ minimumDisplacementIndex ]="
         << displacements[ minimumDisplacementIndex ] << std::endl;

      // updating the coarser flow field
      coarserFlowFieldAlongX( childVoxel ) =
                                    currentFlowFieldAlongX( childVoxel ) +
                                    displacements[ minimumDisplacementIndex ].x;
      coarserFlowFieldAlongY( childVoxel ) =
                                    currentFlowFieldAlongY( childVoxel ) +
                                    displacements[ minimumDisplacementIndex ].y;
      coarserFlowFieldAlongZ( childVoxel ) =
                                    currentFlowFieldAlongZ( childVoxel ) +
                                    displacements[ minimumDisplacementIndex ].z;

    }
    os.close();
    std::cout << "a la  fin" << std::endl;
++attributeOptimalDisplacementsIndex;
  }
  GKG_CATCH( "void attributeOptimalDisplacements( "
             "const gkg::Volume< float >& currentFlowFieldAlongX, "
             "const gkg::Volume< float >& currentFlowFieldAlongY, "
             "const gkg::Volume< float >& currentFlowFieldAlongZ, "
             "const std::vector< float >& displacementCosts, "
             "int32_t currentHalfWidthSearchSpace, "
             "int32_t currentSearchSpaceQuantisation, "
             "const gkg::MinimumSpanningTree< uint8_t >& minimumSpanningTree, "
             "gkg::Volume< float >& coarserFlowFieldAlongX, "
             "gkg::Volume< float >& coarserFlowFieldAlongY, "
             "gkg::Volume< float >& coarserFlowFieldAlongZ )" );

}


void getDisplacements(  
                    int32_t currentSearchSpaceSize,
                    int32_t currentSearchSpaceVoxelCount,
                    int32_t currentHalfWidthSearchSpace,
                    int32_t currentSearchSpaceQuantisation,
                    std::vector< gkg::Vector3d< float > >& displacements ) 
{

  try
  {

    displacements.resize( currentSearchSpaceVoxelCount );

    int32_t sx = 0;
    int32_t sy = 0;
    int32_t sz = 0;
    int32_t displacementIndex = 0;
    for ( sz = 0; sz < currentSearchSpaceSize; sz++ )
    {

      for ( sy = 0; sy < currentSearchSpaceSize; sy++ )
      {

        for ( sx = 0; sx < currentSearchSpaceSize; sx++ )
        {

          displacements[ displacementIndex ].x =
                                 ( float )( sx - currentHalfWidthSearchSpace ) *
                                 currentSearchSpaceQuantisation;
          displacements[ displacementIndex ].y =
                                 ( float )( sy - currentHalfWidthSearchSpace ) *
                                 currentSearchSpaceQuantisation;
          displacements[ displacementIndex ].z =
                                 ( float )( sz - currentHalfWidthSearchSpace ) *
                                 currentSearchSpaceQuantisation;

          ++ displacementIndex;

        }

      }

    }

  }
  GKG_CATCH( "void getDisplacements( "
             "int32_t currentSearchSpaceSize, "
             "int32_t currentSearchSpaceVoxelCount, "
             "int32_t currentHalfWidthSearchSpace, "
             "int32_t currentSearchSpaceQuantisation, "
             "std::vector< gkg::Vector3d< float > >& displacements )" );

}


void fastDistanceTransform( int32_t currentSearchSpaceSize,
                            const gkg::Vector3d< float >& deltaDisplacement,
                            std::vector< float >& costs,
                            std::vector< int32_t >& displacementIndices )
{

  try
  {

    int32_t currentSearchSpaceVoxelCount = currentSearchSpaceSize *
                                           currentSearchSpaceSize *
                                           currentSearchSpaceSize;
    int32_t currentSearchSpacePlaneVoxelCount = currentSearchSpaceSize *
                                                currentSearchSpaceSize;

    int32_t spi = 0;
    for( spi = 0; spi < currentSearchSpaceVoxelCount; spi++ )
    {
	
      displacementIndices[ spi ] = spi;
	
    }

    std::vector< int32_t > v( currentSearchSpaceSize, 0 );
    std::vector< int32_t >::iterator
     iv = v.begin(),
     ive = v.end();
    while ( iv != ive )
    {

      *iv = 0;
      ++ iv;

    }

    std::vector< float > z( currentSearchSpaceSize + 1, 0.0f );
    std::vector< float >::iterator
     iz = z.begin(),
     ize = z.end();
    while ( iz != ize )
    {

      *iz = 0.0f;
      ++ iz;

    }

    std::vector< float > f( currentSearchSpaceSize, 0.0f );
    std::vector< float >::iterator
     itf = f.begin(),
     itfe = f.end();
    while ( itf != itfe )
    {

      *itf = 0.0f;
      ++ itf;

    }

    std::vector< int32_t > i1( currentSearchSpaceSize, 0 ); 
    std::vector< int32_t >::iterator
     ii1 = i1.begin(),
     ii1e = i1.end();
    while ( ii1 != ii1e )
    {

      *ii1 = 0;
      ++ ii1;

    }

    int32_t i = 0;
    int32_t j = 0;
    int32_t k = 0;
    for( k = 0; k < currentSearchSpaceSize; k++ )
    {

      for( i = 0; i < currentSearchSpaceSize; i++ )
      {
	
        fastDistanceTransformSquare( i + k * currentSearchSpacePlaneVoxelCount,
                                     currentSearchSpaceSize,
                                     -deltaDisplacement.x,
                                     currentSearchSpaceSize,
                                     v,
                                     z,
                                     f,
                                     i1,
                                     costs,
                                     displacementIndices );

      }

    }

    for( k = 0; k < currentSearchSpaceSize; k++ )
    {

      for( j = 0; j < currentSearchSpaceSize; j++ )
      {
	
        fastDistanceTransformSquare( j * currentSearchSpaceSize +
                                     k * currentSearchSpacePlaneVoxelCount,
                                     currentSearchSpaceSize,
                                     -deltaDisplacement.y,
                                     1,
                                     v,
                                     z,
                                     f,
                                     i1,
                                     costs,
                                     displacementIndices );

      }

    }

    for( j = 0; j < currentSearchSpaceSize; j++ )
    {

      for( i = 0; i < currentSearchSpaceSize; i++ )
      {
	
        fastDistanceTransformSquare( i + j * currentSearchSpaceSize,
                                     currentSearchSpaceSize,
                                     -deltaDisplacement.z,
                                     currentSearchSpacePlaneVoxelCount,
                                     v,
                                     z,
                                     f,
                                     i1,
                                     costs,
                                     displacementIndices );

      }

    }

  }
  GKG_CATCH( "void fastDistanceTransform( "
             "int32_t currentSearchSpaceSize, "
             "const gkg::Vector3d< float >& deltaDisplacement, "
             "std::vector< float >& costs, "
             "std::vector< int32_t >& displacementIndices )" );

}


void fastDistanceTransformSquare(
                             int32_t index,
                             int32_t length,
                             float displacement,
                             int32_t offsetToNextElement,
                             std::vector< int32_t >& v,
                             std::vector< float >& z,
                             std::vector< float >& f,
                             std::vector< int32_t >& ind1,
                             std::vector< float >& costs,
                             std::vector< int32_t >& displacementIndices )
{

  try
  {

    float infinity = std::numeric_limits< float >::max();

    int32_t j = 0;
    z[ 0 ] = -infinity;
    z[ 1 ] = infinity;
    v[ 0 ] = 0;

    int32_t q = 0;
    for ( q = 1; q < length; q++ )
    {
	
      float s = ( ( costs[ index + q * offsetToNextElement ] + 
                    ( ( float )q + displacement ) *
                    ( ( float )q + displacement ) ) -
                    ( costs[ index + v[ j ] * offsetToNextElement ] + 
                    ( ( float )v[ j ] + displacement ) *
                    ( ( float )v[ j ] + displacement ) ) ) / 
                ( 2.0 * ( float )( q - v[ j ] ) );
		
      while ( s <= z[ j ] )
      {
	
        --j;
		
	s = ( ( costs[ index + q * offsetToNextElement ] + 
                ( ( float )q + displacement ) *
                ( ( float )q + displacement ) ) -
              ( costs[ index + v[ j ] * offsetToNextElement ] + 
                ( ( float )v[ j ] + displacement ) *
                ( ( float )v[ j ] + displacement ) ) ) / 
            ( 2.0 * ( float )( q - v[ j ] ) );

      }
		
      ++j;
      v[ j ] = q;
      z[ j ] = s;
      z[ j + 1 ] = infinity;
	
    }

    j = 0;
    for ( q = 0; q < length; q++ )
    {
	
      f[ q ] = costs[ index + q * offsetToNextElement ];
      ind1[ q ] = displacementIndices[ index + q * offsetToNextElement ];
	
    } 

    for ( q = 0; q < length; q++ )
    {
	
      while ( z[ j + 1 ] < q )
      {
		
	++j;

      }
		
      displacementIndices[ index + q * offsetToNextElement ] = ind1[ v[ j ] ];
      costs[ index + q * offsetToNextElement ] =
        ( ( float )q - ( ( float )v[ j ] + displacement ) ) *
        ( ( float )q - ( ( float )v[ j ] + displacement ) ) + f[ v[ j ] ];
	
    }

  }
  GKG_CATCH( "void fastDistanceTransformSquare( "
             "int32_t index, "
             "int32_t length, "
             "float displacement, "
             "int32_t offsetToNextElement, "
             "std::vector< int32_t >& v, "
             "std::vector< float >& z, "
             "std::vector< float >& f, "
             "std::vector< int32_t >& ind1, "
             "std::vector< float >& costs, "
             "std::vector< int32_t >& displacementIndices )" );

}


void getInterpolation3d( const gkg::Volume< float >& flowField,
                         const gkg::Volume< float >& flowFieldAlongX,
                         const gkg::Volume< float >& flowFieldAlongY,
                         const gkg::Volume< float >& flowFieldAlongZ,
                         gkg::Volume< float >& flowFieldPrime )
{

  try
  {

    // collecting flow field size
    gkg::Vector3d< int32_t > flowFieldSize;
    flowField.getSize( flowFieldSize );

    // collecting flow field resolution
    gkg::Vector3d< double > flowFieldResolution;
    flowField.getResolution( flowFieldResolution );

    float flowFieldValueAlongX = 0.0f;
    float flowFieldValueAlongY = 0.0f; 
    float flowFieldValueAlongZ = 0.0f; 

    int32_t floorFlowFieldValueAlongX = 0.0f; 
    int32_t floorFlowFieldValueAlongY = 0.0f; 
    int32_t floorFlowFieldValueAlongZ = 0.0f;

    float dx = 0.0f;
    float dy = 0.0f;
    float dz = 0.0f;

    int32_t i = 0;
    int32_t j = 0;
    int32_t k = 0;
    for( k = 0; k < flowFieldSize.z; ++k )
    {
		
      for( j = 0; j < flowFieldSize.y; ++j )
      {
			
        for( i = 0; i < flowFieldSize.x; ++i )
        {

          flowFieldValueAlongX = flowFieldAlongX( i, j, k );
          flowFieldValueAlongY = flowFieldAlongY( i, j, k );
          flowFieldValueAlongZ = flowFieldAlongZ( i, j, k );

          floorFlowFieldValueAlongX = std::floor( flowFieldValueAlongX );
          floorFlowFieldValueAlongY = std::floor( flowFieldValueAlongY );
          floorFlowFieldValueAlongZ = std::floor( flowFieldValueAlongZ );
  
          dx = flowFieldValueAlongX - floorFlowFieldValueAlongX;
          dy = flowFieldValueAlongY - floorFlowFieldValueAlongY;
          dz = flowFieldValueAlongZ - floorFlowFieldValueAlongZ;

          flowFieldPrime( i, j, k ) =
            ( 1.0f - dx ) * ( 1.0f - dy ) * ( 1.0f - dz ) * 
            flowField(
              std::min( std::max( i + floorFlowFieldValueAlongX, 0 ),
                        flowFieldSize.x - 1 ),
              std::min( std::max( j + floorFlowFieldValueAlongY, 0 ),
                        flowFieldSize.y - 1 ),
              std::min( std::max( k + floorFlowFieldValueAlongZ, 0 ),
                        flowFieldSize.z - 1 ) ) +
 
            ( 1.0f - dx ) * dy * ( 1.0f - dz ) * 
            flowField(
              std::min( std::max( i + floorFlowFieldValueAlongX, 0 ),
                        flowFieldSize.x - 1 ),
              std::min( std::max( j + floorFlowFieldValueAlongY + 1, 0 ),
                        flowFieldSize.y - 1 ),
              std::min( std::max( k + floorFlowFieldValueAlongZ, 0 ),
                        flowFieldSize.z - 1 ) ) +

            dx * ( 1.0f - dy ) * ( 1.0f - dz ) * 
            flowField(
              std::min( std::max( i + floorFlowFieldValueAlongX + 1, 0 ),
                        flowFieldSize.x - 1 ),
              std::min( std::max( j + floorFlowFieldValueAlongY, 0 ),
                        flowFieldSize.y - 1 ),
              std::min( std::max( k + floorFlowFieldValueAlongZ, 0 ),
                        flowFieldSize.z - 1 ) ) +

            ( 1.0f - dx ) * ( 1.0f - dy ) * dz * 
            flowField(
              std::min( std::max( i + floorFlowFieldValueAlongX, 0 ),
                        flowFieldSize.x - 1 ),
              std::min( std::max( j + floorFlowFieldValueAlongY, 0 ),
                        flowFieldSize.y - 1 ),
              std::min( std::max( k + floorFlowFieldValueAlongZ + 1, 0 ),
                        flowFieldSize.z - 1 ) ) +

            dx * dy * ( 1.0f - dz ) * 
            flowField(
              std::min( std::max( i + floorFlowFieldValueAlongX + 1, 0 ),
                        flowFieldSize.x - 1 ),
              std::min( std::max( j + floorFlowFieldValueAlongY + 1, 0 ),
                        flowFieldSize.y - 1 ),
              std::min( std::max( k + floorFlowFieldValueAlongZ, 0 ),
                        flowFieldSize.z - 1 ) ) +

            ( 1.0f - dx ) * dy * dz * 
            flowField(
              std::min( std::max( i + floorFlowFieldValueAlongX, 0 ),
                        flowFieldSize.x - 1 ),
              std::min( std::max( j + floorFlowFieldValueAlongY + 1, 0 ),
                        flowFieldSize.y - 1 ),
              std::min( std::max( k + floorFlowFieldValueAlongZ + 1, 0 ),
                        flowFieldSize.z - 1 ) ) +

            dx * ( 1.0f - dy ) * dz * 
            flowField(
              std::min( std::max( i + floorFlowFieldValueAlongX + 1, 0 ),
                        flowFieldSize.x - 1 ),
              std::min( std::max( j + floorFlowFieldValueAlongY, 0 ),
                        flowFieldSize.y - 1 ),
              std::min( std::max( k + floorFlowFieldValueAlongZ + 1, 0 ),
                        flowFieldSize.z - 1 ) ) +

            dx * dy * dz * 
            flowField(
              std::min( std::max( i + floorFlowFieldValueAlongX + 1, 0 ),
                        flowFieldSize.x - 1 ),
              std::min( std::max( j + floorFlowFieldValueAlongY + 1, 0 ),
                        flowFieldSize.y - 1 ),
              std::min( std::max( k + floorFlowFieldValueAlongZ + 1, 0 ),
                        flowFieldSize.z - 1 ) );

        }

      }

    }
    
  }
  GKG_CATCH( "void getInterpolation3d( "
             "const gkg::Volume< float >& flowField, "
             "const gkg::Volume< float >& flowFieldAlongX, "
             "const gkg::Volume< float >& flowFieldAlongY, "
             "const gkg::Volume< float >& flowFieldAlongZ, "
             "const gkg::Volume< float >& flowFieldPrime )" );


}


void combineDeformations3d( const gkg::Volume< float >& flowFieldAlongX1,
                            const gkg::Volume< float >& flowFieldAlongY1,
                            const gkg::Volume< float >& flowFieldAlongZ1,
                            const gkg::Volume< float >& flowFieldAlongX2,
                            const gkg::Volume< float >& flowFieldAlongY2,
                            const gkg::Volume< float >& flowFieldAlongZ2,
                            gkg::Volume< float >& combinedFlowFieldAlongX,
                            gkg::Volume< float >& combinedFlowFieldAlongY,
                            gkg::Volume< float >& combinedFlowFieldAlongZ )
{

  try
  {


    // combining deformations
    getInterpolation3d( flowFieldAlongX1,
                        flowFieldAlongX2,
                        flowFieldAlongY2,
                        flowFieldAlongZ2,
                        combinedFlowFieldAlongX );

    getInterpolation3d( flowFieldAlongY1,
                        flowFieldAlongX2,
                        flowFieldAlongY2,
                        flowFieldAlongZ2,
                        combinedFlowFieldAlongY );

    getInterpolation3d( flowFieldAlongZ1,
                        flowFieldAlongX2,
                        flowFieldAlongY2,
                        flowFieldAlongZ2,
                        combinedFlowFieldAlongZ );

    gkg::Volume< float >::const_iterator 
      fX2 = flowFieldAlongX2.begin(),
      fX2e = flowFieldAlongX2.end(),
      fY2 = flowFieldAlongY2.begin(),
      fZ2 = flowFieldAlongZ2.begin();
    gkg::Volume< float >::iterator 
      fXp = combinedFlowFieldAlongX.begin(),
      fYp = combinedFlowFieldAlongY.begin(),
      fZp = combinedFlowFieldAlongZ.begin();
    while ( fX2 != fX2e )
    {

      *fXp += *fX2;
      *fYp += *fY2;
      *fZp += *fZ2;

      ++ fX2;
      ++ fY2;
      ++ fZ2;
      ++ fXp;
      ++ fYp;
      ++ fZp;

    }

  }
  GKG_CATCH( "void combineDeformations3d( "
             "const gkg::Volume< float >& flowFieldAlongX1, "
             "const gkg::Volume< float >& flowFieldAlongY1, "
             "const gkg::Volume< float >& flowFieldAlongZ1, "
             "const gkg::Volume< float >& flowFieldAlongX2, "
             "const gkg::Volume< float >& flowFieldAlongY2, "
             "const gkg::Volume< float >& flowFieldAlongZ2, "
             "gkg::Volume< float >& combinedFlowFieldAlongX, "
             "gkg::Volume< float >& combinedFlowFieldAlongY, "
             "gkg::Volume< float >& combinedFlowFieldAlongZ )" );

}



void fastInverseTransform( const gkg::Volume< float >& directFlowFieldAlongX,
                           const gkg::Volume< float >& directFlowFieldAlongY,
                           const gkg::Volume< float >& directFlowFieldAlongZ,
                           gkg::Volume< float >& inverseFlowFieldAlongX,
                           gkg::Volume< float >& inverseFlowFieldAlongY,
                           gkg::Volume< float >& inverseFlowFieldAlongZ )
{

  try
  {

    // collecting flow field size
    gkg::Vector3d< int32_t > flowFieldSize;
    directFlowFieldAlongX.getSize( flowFieldSize );

    // collecting flow field resolution
    gkg::Vector3d< double > flowFieldResolution;
    directFlowFieldAlongX.getResolution( flowFieldResolution );

    // resetting inverse flow field(s) to zero
    inverseFlowFieldAlongX.fill( 0.0f );
    inverseFlowFieldAlongY.fill( 0.0f );
    inverseFlowFieldAlongZ.fill( 0.0f );

    // allocating temporary volume(s)
    gkg::Volume< float > newDirectFlowFieldAlongX( flowFieldSize );
    gkg::Volume< float > newDirectFlowFieldAlongY( flowFieldSize );
    gkg::Volume< float > newDirectFlowFieldAlongZ( flowFieldSize );
    newDirectFlowFieldAlongX.setResolution( flowFieldResolution );
    newDirectFlowFieldAlongY.setResolution( flowFieldResolution );
    newDirectFlowFieldAlongZ.setResolution( flowFieldResolution );

    gkg::Volume< float > newInverseFlowFieldAlongX( flowFieldSize );
    gkg::Volume< float > newInverseFlowFieldAlongY( flowFieldSize );
    gkg::Volume< float > newInverseFlowFieldAlongZ( flowFieldSize );
    newInverseFlowFieldAlongX.setResolution( flowFieldResolution );
    newInverseFlowFieldAlongY.setResolution( flowFieldResolution );
    newInverseFlowFieldAlongZ.setResolution( flowFieldResolution );


    // initializing the temporary inverse flow field by taking the opposit of
    // the direct flow field
    gkg::Volume< float >::const_iterator 
      dfX = directFlowFieldAlongX.begin(),
      dfXe = directFlowFieldAlongX.end(),
      dfY = directFlowFieldAlongY.begin(),
      dfZ = directFlowFieldAlongZ.begin();
    gkg::Volume< float >::iterator 
      ifX = newInverseFlowFieldAlongX.begin(),
      ifY = newInverseFlowFieldAlongY.begin(),
      ifZ = newInverseFlowFieldAlongZ.begin();
    while ( dfX != dfXe )
    {

      *ifX = -*dfX;
      *ifY = -*dfY;
      *ifZ = -*dfZ;

      ++ dfX;
      ++ dfY;
      ++ dfZ;
      ++ ifX;
      ++ ifY;
      ++ ifZ;

    }

    int32_t iteration = 0;
    for ( iteration = 0;
          iteration < FAST_INVERSE_TRANSFORM_MAXIMUM_ITERATION_COUNT;
          iteration++ )
    {

      getInterpolation3d( newInverseFlowFieldAlongX,
                          inverseFlowFieldAlongX,
                          inverseFlowFieldAlongY,
                          inverseFlowFieldAlongZ,
                          newDirectFlowFieldAlongX );
      getInterpolation3d( newInverseFlowFieldAlongY,
                          inverseFlowFieldAlongX,
                          inverseFlowFieldAlongY,
                          inverseFlowFieldAlongZ,
                          newDirectFlowFieldAlongY );
      getInterpolation3d( newInverseFlowFieldAlongZ,
                          inverseFlowFieldAlongX,
                          inverseFlowFieldAlongY,
                          inverseFlowFieldAlongZ,
                          newDirectFlowFieldAlongZ );

      inverseFlowFieldAlongX = newDirectFlowFieldAlongX;
      inverseFlowFieldAlongY = newDirectFlowFieldAlongY;
      inverseFlowFieldAlongZ = newDirectFlowFieldAlongZ;

    }

  }
  GKG_CATCH( "void fastInverseTransform( "
             "const gkg::Volume< float >& directFlowFieldAlongX, "
             "const gkg::Volume< float >& directFlowFieldAlongY, "
             "const gkg::Volume< float >& directFlowFieldAlongZ, "
             "gkg::Volume< float >& inverseFlowFieldAlongX, "
             "gkg::Volume< float >& inverseFlowFieldAlongY, "
             "gkg::Volume< float >& inverseFlowFieldAlongZ )" );

}


void computeGradientAlongX( const gkg::Volume< float >& flowField,
                            gkg::Volume< float >& flowFieldGradientAlongX )
{

  try
  {

    // collecting flow field size
    gkg::Vector3d< int32_t > flowFieldSize;
    flowField.getSize( flowFieldSize );

    flowFieldGradientAlongX.fill( 0.0f);

    int32_t x = 0;
    int32_t y = 0;
    int32_t z = 0;
    for ( z = 0; z < flowFieldSize.z; z++ )
    {

      for ( y = 0; y < flowFieldSize.y; y++ )
      {

        for ( x = 0; x < flowFieldSize.x; x++ )
        {

          flowFieldGradientAlongX( x, y, z ) =
          ( flowField( std::max( std::min( x + 1, flowFieldSize.x - 1  ), 0 ),
                       y,
                       z ) -
            flowField( std::max( std::min( x - 1, flowFieldSize.x - 1  ), 0 ),
                       y,
                       z ) ) / 2.0f;

        }

      }

    }

  }
  GKG_CATCH( "void computeGradientAlongX( "
             "const gkg::Volume< float >& flowField, "
             "gkg::Volume< float >& flowFieldGradientAlongX )" );

}


void computeGradientAlongY( const gkg::Volume< float >& flowField,
                            gkg::Volume< float >& flowFieldGradientAlongY )
{

  try
  {

    // collecting flow field size
    gkg::Vector3d< int32_t > flowFieldSize;
    flowField.getSize( flowFieldSize );

    flowFieldGradientAlongY.fill( 0.0f);

    int32_t x = 0;
    int32_t y = 0;
    int32_t z = 0;
    for ( z = 0; z < flowFieldSize.z; z++ )
    {

      for ( y = 0; y < flowFieldSize.y; y++ )
      {

        for ( x = 0; x < flowFieldSize.x; x++ )
        {

          flowFieldGradientAlongY( x, y, z ) =
          ( flowField( x,
                       std::max( std::min( y + 1, flowFieldSize.y - 1  ), 0 ),
                       z ) -
            flowField( x,
                       std::max( std::min( y - 1, flowFieldSize.y - 1  ), 0 ),
                       z ) ) / 2.0f;

        }

      }

    }

  }
  GKG_CATCH( "void computeGradientAlongY( "
             "const gkg::Volume< float >& flowField, "
             "gkg::Volume< float >& flowFieldGradientAlongY )" );

}


void computeGradientAlongZ( const gkg::Volume< float >& flowField,
                            gkg::Volume< float >& flowFieldGradientAlongZ )
{

  try
  {

    // collecting flow field size
    gkg::Vector3d< int32_t > flowFieldSize;
    flowField.getSize( flowFieldSize );

    flowFieldGradientAlongZ.fill( 0.0f);

    int32_t x = 0;
    int32_t y = 0;
    int32_t z = 0;
    for ( z = 0; z < flowFieldSize.z; z++ )
    {

      for ( y = 0; y < flowFieldSize.y; y++ )
      {

        for ( x = 0; x < flowFieldSize.x; x++ )
        {

          flowFieldGradientAlongZ( x, y, z ) =
          ( flowField( x,
                       y,
                       std::max( std::min( z + 1, flowFieldSize.z - 1  ), 0 ) )-
            flowField( x,
                       y,
                       std::max( std::min( z - 1, flowFieldSize.z - 1  ), 0 ) ) ) / 2.0f;

        }

      }

    }

  }
  GKG_CATCH( "void computeGradientAlongZ( "
             "const gkg::Volume< float >& flowField, "
             "gkg::Volume< float >& flowFieldGradientAlongZ )" );

}


void scaleAndSquareTransformation( gkg::Volume< float >& flowFieldAlongX,
                                   gkg::Volume< float >& flowFieldAlongY,
                                   gkg::Volume< float >& flowFieldAlongZ,
                                   int32_t currentControlPointSpacing,
                                   int32_t exponentiationStepCount )
{

  try
  {

    // collecting flow field size
    gkg::Vector3d< int32_t > flowFieldSize;
    flowFieldAlongX.getSize( flowFieldSize );

    // collecting flow field resolution
    gkg::Vector3d< double > flowFieldResolution;
    flowFieldAlongX.getResolution( flowFieldResolution );


    // allocating temporary volumes
    gkg::Volume< float > flowFieldAlongX1( flowFieldAlongX );
    gkg::Volume< float > flowFieldAlongY1( flowFieldAlongY );
    gkg::Volume< float > flowFieldAlongZ1( flowFieldAlongZ );

    gkg::Volume< float > flowFieldAlongX2( flowFieldSize );
    flowFieldAlongX2.setResolution( flowFieldResolution );
    gkg::Volume< float > flowFieldAlongY2( flowFieldSize );
    flowFieldAlongY2.setResolution( flowFieldResolution );
    gkg::Volume< float > flowFieldAlongZ2( flowFieldSize );
    flowFieldAlongZ2.setResolution( flowFieldResolution );

    // scaling the flow field(s) according to the exponentiation step count
    // and the current control point spacing
    gkg::Scaler< gkg::Volume< float > > 
      scaler1( 1.0f / ( ( float )currentControlPointSpacing * 
                        ( float )std::pow(2.0, exponentiationStepCount ) ) );

    scaler1.scale( flowFieldAlongX1 );
    scaler1.scale( flowFieldAlongY1 );
    scaler1.scale( flowFieldAlongZ1 );


    int32_t exponentiationStepIndex = 0;
    for ( exponentiationStepIndex = 0;
          exponentiationStepIndex < exponentiationStepCount;
          exponentiationStepIndex++ )
    {

      // combining deformations
      combineDeformations3d( flowFieldAlongX1,
                             flowFieldAlongY1,
                             flowFieldAlongZ1,
                             flowFieldAlongX1,
                             flowFieldAlongY1,
                             flowFieldAlongZ1,
                             flowFieldAlongX2,
                             flowFieldAlongY2,
                             flowFieldAlongZ2 );

      // copying back result to input
      flowFieldAlongX1 = flowFieldAlongX2;
      flowFieldAlongY1 = flowFieldAlongY2;
      flowFieldAlongZ1 = flowFieldAlongZ2;

    }


    flowFieldAlongX = flowFieldAlongX2;
    flowFieldAlongY = flowFieldAlongY2;
    flowFieldAlongZ = flowFieldAlongZ2;

    gkg::Scaler< gkg::Volume< float > >
      scaler2( ( float )currentControlPointSpacing ); 

    scaler2.scale( flowFieldAlongX );
    scaler2.scale( flowFieldAlongY );
    scaler2.scale( flowFieldAlongZ );

  }
  GKG_CATCH( "void scaleAndSquareTransformation( "
             "const gkg::Volume< float >& coarserDirectFlowFieldAlongX, "
             "const gkg::Volume< float >& coarserDirectFlowFieldAlongY, "
             "const gkg::Volume< float >& coarserDirectFlowFieldAlongZ, "
             "int32_t currentControlPointSpacing, "
             "int32_t exponentiationStepCount )" );

}


void symmetrizeDirectAndInverseTransformations(
                                   gkg::Volume< float >& directFlowFieldAlongX,
                                   gkg::Volume< float >& directFlowFieldAlongY,
                                   gkg::Volume< float >& directFlowFieldAlongZ,
                                   gkg::Volume< float >& inverseFlowFieldAlongX,
                                   gkg::Volume< float >& inverseFlowFieldAlongY,
                                   gkg::Volume< float >& inverseFlowFieldAlongZ,
                                   int32_t currentControlPointSpacing )
{

  try
  {

    // collecting flow field size
    gkg::Vector3d< int32_t > flowFieldSize;
    directFlowFieldAlongX.getSize( flowFieldSize );

    // collecting flow field resolution
    gkg::Vector3d< double > flowFieldResolution;
    directFlowFieldAlongX.getResolution( flowFieldResolution );


    // allocating temporary volume(s)
    gkg::Volume< float > symDirectFlowFieldAlongX( directFlowFieldAlongX );
    gkg::Volume< float > symDirectFlowFieldAlongY( directFlowFieldAlongY );
    gkg::Volume< float > symDirectFlowFieldAlongZ( directFlowFieldAlongZ );
    gkg::Volume< float > symInverseFlowFieldAlongX( inverseFlowFieldAlongX );
    gkg::Volume< float > symInverseFlowFieldAlongY( inverseFlowFieldAlongY );
    gkg::Volume< float > symInverseFlowFieldAlongZ( inverseFlowFieldAlongZ );

    // scaling the flow field(s) according to the current control point spacing
    gkg::Scaler< gkg::Volume< float > > 
      scaler1( 0.5f / ( ( float )currentControlPointSpacing ) );

    scaler1.scale( symDirectFlowFieldAlongX );
    scaler1.scale( symDirectFlowFieldAlongY );
    scaler1.scale( symDirectFlowFieldAlongZ );

    scaler1.scale( symInverseFlowFieldAlongX );
    scaler1.scale( symInverseFlowFieldAlongY );
    scaler1.scale( symInverseFlowFieldAlongZ );

    // computing the symmetric direct and inverse flow field(s)
    gkg::Volume< float > tmpInverseFlowFieldAlongX1( flowFieldSize );
    gkg::Volume< float > tmpInverseFlowFieldAlongY1( flowFieldSize );
    gkg::Volume< float > tmpInverseFlowFieldAlongZ1( flowFieldSize );
    gkg::Volume< float > tmpInverseFlowFieldAlongX2( flowFieldSize );
    gkg::Volume< float > tmpInverseFlowFieldAlongY2( flowFieldSize );
    gkg::Volume< float > tmpInverseFlowFieldAlongZ2( flowFieldSize );

    tmpInverseFlowFieldAlongX1.setResolution( flowFieldResolution );
    tmpInverseFlowFieldAlongY1.setResolution( flowFieldResolution );
    tmpInverseFlowFieldAlongZ1.setResolution( flowFieldResolution );
    tmpInverseFlowFieldAlongX2.setResolution( flowFieldResolution );
    tmpInverseFlowFieldAlongY2.setResolution( flowFieldResolution );
    tmpInverseFlowFieldAlongZ2.setResolution( flowFieldResolution );

    fastInverseTransform( symDirectFlowFieldAlongX,
                          symDirectFlowFieldAlongY,
                          symDirectFlowFieldAlongZ,
                          tmpInverseFlowFieldAlongX1,
                          tmpInverseFlowFieldAlongY1,
                          tmpInverseFlowFieldAlongZ1 );
                          

    fastInverseTransform( symInverseFlowFieldAlongX,
                          symInverseFlowFieldAlongY,
                          symInverseFlowFieldAlongZ,
                          tmpInverseFlowFieldAlongX2,
                          tmpInverseFlowFieldAlongY2,
                          tmpInverseFlowFieldAlongZ2 );

    // combining deformations to symmetrize them
    combineDeformations3d( tmpInverseFlowFieldAlongX2,
                           tmpInverseFlowFieldAlongY2,
                           tmpInverseFlowFieldAlongZ2,
                           symDirectFlowFieldAlongX,
                           symDirectFlowFieldAlongY,
                           symDirectFlowFieldAlongZ,
                           directFlowFieldAlongX,
                           directFlowFieldAlongY,
                           directFlowFieldAlongZ );

    combineDeformations3d( tmpInverseFlowFieldAlongX1,
                           tmpInverseFlowFieldAlongY1,
                           tmpInverseFlowFieldAlongZ1,
                           symInverseFlowFieldAlongX,
                           symInverseFlowFieldAlongY,
                           symInverseFlowFieldAlongZ,
                           inverseFlowFieldAlongX,
                           inverseFlowFieldAlongY,
                           inverseFlowFieldAlongZ );


    // scaling the flow field(s) according to the current control point spacing
    gkg::Scaler< gkg::Volume< float > > 
      scaler2( ( float )currentControlPointSpacing );

    scaler2.scale( directFlowFieldAlongX );
    scaler2.scale( directFlowFieldAlongY );
    scaler2.scale( directFlowFieldAlongZ );

    scaler2.scale( inverseFlowFieldAlongX );
    scaler2.scale( inverseFlowFieldAlongY );
    scaler2.scale( inverseFlowFieldAlongZ );

  }
  GKG_CATCH( "void symmetrizeDirectAndInverseTransformations( "
             "gkg::Volume< float >& directFlowFieldAlongX, "
             "gkg::Volume< float >& directFlowFieldAlongY, "
             "gkg::Volume< float >& directFlowFieldAlongZ, "
             "gkg::Volume< float >& inverseFlowFieldAlongX, "
             "gkg::Volume< float >& inverseFlowFieldAlongY, "
             "gkg::Volume< float >& inverseFlowFieldAlongZ, "
             "int32_t currentControlPointSpacing )" );

}


void computeJacobianDeterminant( const gkg::Volume< float >& flowFieldAlongX,
                                 const gkg::Volume< float >& flowFieldAlongY,
                                 const gkg::Volume< float >& flowFieldAlongZ,
                                 int32_t currentControlPointSpacing,
                                 float& jacobianMean,
                                 float& jacobianStandardDeviation,
                                 float& jacobianMinimum,
                                 float& jacobianMaximum,
                                 float& jacobianNegativeRatio )
{

  try
  {

    // collecting flow field size
    gkg::Vector3d< int32_t > flowFieldSize;
    flowFieldAlongX.getSize( flowFieldSize );
    float sizeXYZ = ( float )flowFieldSize.x *
                    ( float )flowFieldSize.y *
                    ( float )flowFieldSize.z;

    // computing the gradient volumes along X, Y and Z
    gkg::Volume< float > J11( flowFieldSize );
    computeGradientAlongX( flowFieldAlongX, J11 );
    gkg::Volume< float > J12( flowFieldSize );
    computeGradientAlongY( flowFieldAlongX, J12 );
    gkg::Volume< float > J13( flowFieldSize );
    computeGradientAlongZ( flowFieldAlongX, J13 );

    gkg::Volume< float > J21( flowFieldSize );
    computeGradientAlongX( flowFieldAlongY, J21 );
    gkg::Volume< float > J22( flowFieldSize );
    computeGradientAlongY( flowFieldAlongY, J22 );
    gkg::Volume< float > J23( flowFieldSize );
    computeGradientAlongZ( flowFieldAlongY, J23 );

    gkg::Volume< float > J31( flowFieldSize );
    computeGradientAlongX( flowFieldAlongZ, J31 );
    gkg::Volume< float > J32( flowFieldSize );
    computeGradientAlongY( flowFieldAlongZ, J32 );
    gkg::Volume< float > J33( flowFieldSize );
    computeGradientAlongZ( flowFieldAlongZ, J33 );


    int32_t x = 0;
    int32_t y = 0;
    int32_t z = 0;
    for ( z = 0; z < flowFieldSize.z; z++ )
    {

      for ( y = 0; y < flowFieldSize.y; y++ )
      {

        for ( x = 0; x < flowFieldSize.x; x++ )
        {

          J11( x, y, z ) /= ( float )currentControlPointSpacing;
          J11( x, y, z ) += 1.0f;
          J12( x, y, z ) /= ( float )currentControlPointSpacing;
          J13( x, y, z ) /= ( float )currentControlPointSpacing;
          J21( x, y, z ) /= ( float )currentControlPointSpacing;
          J22( x, y, z ) /= ( float )currentControlPointSpacing;
          J22( x, y, z ) += 1.0f;
          J23( x, y, z ) /= ( float )currentControlPointSpacing;
          J31( x, y, z ) /= ( float )currentControlPointSpacing;
          J32( x, y, z ) /= ( float )currentControlPointSpacing;
          J33( x, y, z ) /= ( float )currentControlPointSpacing;
          J33( x, y, z ) += 1.0f;

        }

      }

    }


    // creating an output volume for the Jacobian and filling it with zeros
    gkg::Volume< float > Jacobian( flowFieldSize );
    Jacobian.fill( 0.0f );

    // computing the Jacobian from the flow field gradient(s)
    float J = 0.0f;
    jacobianMean = 0.0f;
    jacobianMinimum = 1.0f;
    jacobianMaximum = 1.0f;
    jacobianNegativeRatio = 0;
    for ( z = 0; z < flowFieldSize.z; z++ )
    {

      for ( y = 0; y < flowFieldSize.y; y++ )
      {

        for ( x = 0; x < flowFieldSize.x; x++ )
        {

          J = J11( x, y, z ) * ( J22( x, y, z ) * J33( x, y, z ) - 
                                 J23( x, y, z ) * J32( x, y, z ) ) -
              J21( x, y, z ) * ( J12( x, y, z ) * J33( x, y, z ) - 
                                 J13( x, y, z ) * J32( x, y, z ) ) +
              J31( x, y, z ) * ( J12( x, y, z ) * J23( x, y, z ) - 
                                 J13( x, y, z ) * J22( x, y, z ) );
          jacobianMean += J;
          if ( J > jacobianMaximum )
          {

            jacobianMaximum = J;

          }
          else if ( J < jacobianMinimum )
          {

            jacobianMinimum = J;

          }
          if ( J < 0.0f )
          {

            ++ jacobianNegativeRatio;

          }
          Jacobian( x, y, z ) = J;

        }

      }

    }

    jacobianMean /= sizeXYZ;

    float jacobianDifference = 0.0f;
    float jacobianVariance = 0.0f;
    for ( z = 0; z < flowFieldSize.z; z++ )
    {

      for ( y = 0; y < flowFieldSize.y; y++ )
      {

        for ( x = 0; x < flowFieldSize.x; x++ )
        {

           jacobianDifference = Jacobian( x, y, z ) - jacobianMean;
           jacobianVariance += jacobianDifference * jacobianDifference;
          

        }

      }

    }
    jacobianStandardDeviation = std::sqrt( jacobianVariance / 
                                           ( sizeXYZ - 1.0f ) );
    
    jacobianNegativeRatio /= sizeXYZ;

  }
  GKG_CATCH( "void computeJacobianDeterminant( "
             "const gkg::Volume< float >& coarserDirectFlowFieldAlongX, "
             "const gkg::Volume< float >& coarserDirectFlowFieldAlongY, "
             "const gkg::Volume< float >& coarserDirectFlowFieldAlongZ, "
             "int32_t currentControlPointSpacing, "
             "float& jacobianMean, "
             "float& jacobianStandardDeviation, "
             "float& jacobianMinimum, "
             "float& jacobianMaximum, "
             "float& jacobianNegativeRatio )" );

}


float computeHarmonicEnergy( const gkg::Volume< float >& flowFieldAlongX,
                             const gkg::Volume< float >& flowFieldAlongY,
                             const gkg::Volume< float >& flowFieldAlongZ ) 
{

  try
  {

    // collecting flow field size
    gkg::Vector3d< int32_t > flowFieldSize;
    flowFieldAlongX.getSize( flowFieldSize );
    float sizeXYZ = ( float )flowFieldSize.x *
                    ( float )flowFieldSize.y *
                    ( float )flowFieldSize.z;

    float harmonicEnergy = 0.0f;
    float sumOfSquare = 0.0f;
    gkg::Volume< float > flowFieldGradient( flowFieldSize );
    gkg::SumOfSquareFilter< gkg::Volume< float >, float > sumOfSquareFilter;

    ////////////////////////////////////////////////////////////////////////////
    // gradients of flowFieldAlongX
    ////////////////////////////////////////////////////////////////////////////

    // flow field gradient of flowFieldAlongX along X
    computeGradientAlongX( flowFieldAlongX, flowFieldGradient );
    sumOfSquareFilter.filter( flowFieldGradient, sumOfSquare );
    harmonicEnergy += sumOfSquare;

    // flow field gradient of flowFieldAlongX along Y
    computeGradientAlongY( flowFieldAlongX, flowFieldGradient );
    sumOfSquareFilter.filter( flowFieldGradient, sumOfSquare );
    harmonicEnergy += sumOfSquare;

    // flow field gradient of flowFieldAlongX along Z
    computeGradientAlongZ( flowFieldAlongX, flowFieldGradient );
    sumOfSquareFilter.filter( flowFieldGradient, sumOfSquare );
    harmonicEnergy += sumOfSquare;

    ////////////////////////////////////////////////////////////////////////////
    // gradients of flowFieldAlongY
    ////////////////////////////////////////////////////////////////////////////

    // flow field gradient of flowFieldAlongY along X
    computeGradientAlongX( flowFieldAlongY, flowFieldGradient );
    sumOfSquareFilter.filter( flowFieldGradient, sumOfSquare );
    harmonicEnergy += sumOfSquare;

    // flow field gradient of flowFieldAlongY along Y
    computeGradientAlongY( flowFieldAlongY, flowFieldGradient );
    sumOfSquareFilter.filter( flowFieldGradient, sumOfSquare );
    harmonicEnergy += sumOfSquare;

    // flow field gradient of flowFieldAlongY along Z
    computeGradientAlongZ( flowFieldAlongY, flowFieldGradient );
    sumOfSquareFilter.filter( flowFieldGradient, sumOfSquare );
    harmonicEnergy += sumOfSquare;

    ////////////////////////////////////////////////////////////////////////////
    // gradients of flowFieldAlongZ
    ////////////////////////////////////////////////////////////////////////////

    // flow field gradient of flowFieldAlongZ along X
    computeGradientAlongX( flowFieldAlongZ, flowFieldGradient );
    sumOfSquareFilter.filter( flowFieldGradient, sumOfSquare );
    harmonicEnergy += sumOfSquare;

    // flow field gradient of flowFieldAlongZ along Y
    computeGradientAlongY( flowFieldAlongZ, flowFieldGradient );
    sumOfSquareFilter.filter( flowFieldGradient, sumOfSquare );
    harmonicEnergy += sumOfSquare;

    // flow field gradient of flowFieldAlongZ along Z
    computeGradientAlongZ( flowFieldAlongZ, flowFieldGradient );
    sumOfSquareFilter.filter( flowFieldGradient, sumOfSquare );
    harmonicEnergy += sumOfSquare;

    return harmonicEnergy / sizeXYZ;

  }
  GKG_CATCH( "float computeHarmonicEnergy( "
             "const gkg::Volume< float >& directFlowFieldAlongX, "
             "const gkg::Volume< float >& directFlowFieldAlongY, "
             "const gkg::Volume< float >& directFlowFieldAlongZ )" );

}




int main( int argc, char* argv[] )
{

  int result = EXIT_SUCCESS;

  try
  {

    std::string fileNameReference;
    std::string fileNameFloating;
    std::string fileNameDeformedFloating; 
    std::vector< int32_t > halfWidthSearchSpaces;
    std::vector< int32_t > controlPointSpacings;
    std::vector< int32_t > searchSpaceQuantisations;
    std::vector< float > regularizationWeights;
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
    application.addSeriesOption( "-regularizationWeighting",
                                 "Regularization weights",
                                 regularizationWeights );
    application.addSeriesOption( "-halfWidthSearchSpace",
                                 "Half width search spaces ",
                                 halfWidthSearchSpaces );
    application.addSeriesOption( "-controlPointsSpacing",
                                 "Control point spacings ",
                                 controlPointSpacings );
    application.addSeriesOption( "-searchSpaceQuantisation",
                                 "Search space quantisations ",
                                 searchSpaceQuantisations );
    application.addSingleOption( "-output",
                                 "Deformed floating file name",
                                 fileNameDeformedFloating );
    application.addSingleOption( "-verbose",
                                 "Verbosity on",
                                 verbose );

    application.initialize();


    gkg::RCPointer< gkg::ParameterizedRigidTransform3d< float > >
      rigidTransform3d;
    rigidTransform3d.reset( new gkg::ParameterizedRigidTransform3d< float > );

    gkg::RCPointer< gkg::VectorFieldTransform3d< float > >
      vectorFieldTransform3d;
    vectorFieldTransform3d.reset( new gkg::VectorFieldTransform3d< float >(
                                         gkg::Volume< float >( 1, 1, 1, 6 ) ) );

    gkg::RCPointer< gkg::NonLinearTransform3d< float > > transform3d;
    transform3d.reset(
      new gkg::NonLinearTransform3d< float >( rigidTransform3d,
                                              vectorFieldTransform3d ) );

    ////////////////////////////////////////////////////////////////////////////
    //Initialise random variable
    ////////////////////////////////////////////////////////////////////////////
    std::srand( time( 0 ) );

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

    // collecting the reference volume size
    gkg::Vector3d< int32_t > referenceSize;
    reference.getSize( referenceSize );

    gkg::Vector3d< double > referenceResolution;
    reference.getResolution( referenceResolution );

    // first tuning the direct and inverse vector fields of the diffeomorphic
    // transformation according to the floating size(s) and resolution(s)

    gkg::Vector3d< int32_t > floatingSize;
    floating.getSize( floatingSize );

    gkg::Vector3d< double > floatingResolution;
    floating.getResolution( floatingResolution );


    // computing the resampling resolution and size required to compute
    // the diffeomorphism
    gkg::Vector3d< double > 
      diffeomorphismResolution( std::min( referenceResolution.x,
                                          floatingResolution.x ),
                                std::min( referenceResolution.y,
                                          floatingResolution.y ),
                                std::min( referenceResolution.z,
                                          floatingResolution.z ) );

    gkg::Vector3d< int32_t >
      diffeomorphismSize( std::max( ( int32_t )( referenceSize.x *
                                                 referenceResolution.x /
                                                 diffeomorphismResolution.x +
                                                 0.5 ),
                                    ( int32_t )( floatingSize.x *
                                                 floatingResolution.x /
                                                 diffeomorphismResolution.x +
                                                 0.5 ) ),
                          std::max( ( int32_t )( referenceSize.y *
                                                 referenceResolution.y /
                                                 diffeomorphismResolution.y +
                                                 0.5 ),
                                    ( int32_t )( floatingSize.y *
                                                 floatingResolution.y /
                                                 diffeomorphismResolution.y +
                                                 0.5 ) ),
                          std::max( ( int32_t )( referenceSize.z *
                                                 referenceResolution.z /
                                                 diffeomorphismResolution.z +
                                                 0.5 ),
                                    ( int32_t )( floatingSize.z *
                                                 floatingResolution.z /
                                                 diffeomorphismResolution.z +
                                                 0.5 ) ) );

    gkg::RCPointer< gkg::Volume< float > >
      diffeomorphismReference( new gkg::Volume< float >( diffeomorphismSize ) );
    diffeomorphismReference->setResolution( diffeomorphismResolution );
                          
    gkg::RCPointer< gkg::Volume< float > >
      diffeomorphismFloating( new gkg::Volume< float >( diffeomorphismSize ) );
    diffeomorphismFloating->setResolution( diffeomorphismResolution );
                                    
    gkg::Resampler< float >* referenceResampler = 
      gkg::ResamplerFactory< float >::getInstance().getResampler( 3 );
                                          
    gkg::Resampler< float >* floatingResampler = 
      gkg::ResamplerFactory< float >::getInstance().getResampler( 3 );

    gkg::IdentityTransform3d< float > identityTransform3d;
    referenceResampler->resample( reference,
                                  identityTransform3d,
                                  0.0f,
                                  *diffeomorphismReference );

    floatingResampler->resample( floating,
                                 identityTransform3d,
                                 0.0f,
                                 *diffeomorphismFloating );

      gkg::Writer::getInstance().write(
              "Cyril/diffeomorphismReference.ima",
              *diffeomorphismReference );

      gkg::Writer::getInstance().write(
              "Cyril/diffeomorphismFloating.ima",
              *diffeomorphismFloating );

    int32_t subSamplingSize = std::max( diffeomorphismSize.x,
                                std::max( diffeomorphismSize.y,
                                          diffeomorphismSize.z ) );

    gkg::Volume< float >
      directAndInverseVectorFields( diffeomorphismSize, 6 );
    directAndInverseVectorFields.setResolution( diffeomorphismResolution );
    directAndInverseVectorFields.fill( 0.0f );

    gkg::Volume< float > directFlowFieldAlongX( diffeomorphismSize );
    directFlowFieldAlongX.setResolution( diffeomorphismResolution );
    gkg::Volume< float > directFlowFieldAlongY( diffeomorphismSize );
    directFlowFieldAlongY.setResolution( diffeomorphismResolution );
    gkg::Volume< float > directFlowFieldAlongZ( diffeomorphismSize );
    directFlowFieldAlongZ.setResolution( diffeomorphismResolution );

    gkg::Volume< float > inverseFlowFieldAlongX( diffeomorphismSize );
    inverseFlowFieldAlongX.setResolution( diffeomorphismResolution );
    gkg::Volume< float > inverseFlowFieldAlongY( diffeomorphismSize );
    inverseFlowFieldAlongY.setResolution( diffeomorphismResolution );
    gkg::Volume< float > inverseFlowFieldAlongZ( diffeomorphismSize );
    inverseFlowFieldAlongZ.setResolution( diffeomorphismResolution );

    vectorFieldTransform3d->setDirectAndInverseVectorFields( 
                                               directAndInverseVectorFields );


    int32_t c = 0;
    int32_t optimizerConfigurationCount =
                                        ( int32_t )halfWidthSearchSpaces.size();

    // allocating the similarity measure
    std::vector< double > similarityMeasureParameters( 2U );
    similarityMeasureParameters[ 0 ] = 64;
    similarityMeasureParameters[ 1 ] = 1;

    gkg::RCPointer< gkg::SimilarityMeasure< float, float > >
      directSimilarityMeasure =
        gkg::SimilarityMeasureFactory< float, float >::getInstance().create(
          "mutual-information",
          diffeomorphismReference,
          diffeomorphismFloating,
          0,
          0,
          1,
          subSamplingSize,
          similarityMeasureParameters,
          verbose );


    gkg::RCPointer< gkg::SimilarityMeasure< float, float > >
      inverseSimilarityMeasure =
        gkg::SimilarityMeasureFactory< float, float >::getInstance().create(
          "mutual-information",
          diffeomorphismFloating,
          diffeomorphismReference,
          0,
          0,
          1,
          subSamplingSize,
          similarityMeasureParameters,
          verbose );

    gkg::Resampler< float >* deformationResampler = 
    gkg::ResamplerFactory< float >::getInstance().getResampler( 1 );

    gkg::RCPointer< gkg::InverseTransform3d< float > >
      inverseTransform3d( new gkg::InverseTransform3d< float >(
                                                              transform3d ) );

    gkg::Vector3d< double > 
      controlPointVolumeResolution( diffeomorphismResolution ); //1.0, 1.0, 1.0 );

    gkg::Vector3d< int32_t > 
      coarserControlPointVolumeSize = diffeomorphismSize /
                                      controlPointSpacings[ 0 ];
    gkg::Vector3d< double > 
    coarserControlPointVolumeResolution = controlPointVolumeResolution *
                                          ( double )controlPointSpacings[ 0 ];

    gkg::Volume< float > 
      coarserDirectFlowFieldAlongX( coarserControlPointVolumeSize );
    coarserDirectFlowFieldAlongX.setResolution( 
                                        coarserControlPointVolumeResolution );
    gkg::Volume< float > 
      coarserDirectFlowFieldAlongY( coarserControlPointVolumeSize );
    coarserDirectFlowFieldAlongY.setResolution( 
                                        coarserControlPointVolumeResolution );
    gkg::Volume< float > 
      coarserDirectFlowFieldAlongZ( coarserControlPointVolumeSize );
    coarserDirectFlowFieldAlongZ.setResolution( 
                                        coarserControlPointVolumeResolution );

    gkg::Volume< float > 
      coarserInverseFlowFieldAlongX( coarserControlPointVolumeSize );
    coarserInverseFlowFieldAlongX.setResolution( 
                                        coarserControlPointVolumeResolution );
    gkg::Volume< float > 
      coarserInverseFlowFieldAlongY( coarserControlPointVolumeSize );
    coarserInverseFlowFieldAlongY.setResolution( 
                                        coarserControlPointVolumeResolution );
    gkg::Volume< float > 
      coarserInverseFlowFieldAlongZ( coarserControlPointVolumeSize );
    coarserInverseFlowFieldAlongZ.setResolution( 
                                        coarserControlPointVolumeResolution );

    coarserDirectFlowFieldAlongX.fill( 0.0f );
    coarserDirectFlowFieldAlongY.fill( 0.0f );
    coarserDirectFlowFieldAlongZ.fill( 0.0f );

    coarserInverseFlowFieldAlongX.fill( 0.0f );
    coarserInverseFlowFieldAlongY.fill( 0.0f );
    coarserInverseFlowFieldAlongZ.fill( 0.0f );

    gkg::Volume< float > currentDirectFlowFieldAlongX;
    gkg::Volume< float > currentDirectFlowFieldAlongY;
    gkg::Volume< float > currentDirectFlowFieldAlongZ;

    gkg::Volume< float > currentInverseFlowFieldAlongX;
    gkg::Volume< float > currentInverseFlowFieldAlongY;
    gkg::Volume< float > currentInverseFlowFieldAlongZ;

    gkg::Vector3d< int32_t > currentControlPointVolumeSize;
    gkg::Vector3d< double > currentControlPointVolumeResolution;

    double directSimilarityValueBefore = 0.0;
    double directSimilarityValueAfter = 0.0;
    int32_t currentSearchSpaceSize = 0;
    int32_t currentSearchSpaceVoxelCount = 0;
    int32_t currentControlPointCount = 0;

    gkg::Volume< uint8_t > 
      directTransformedFloatingU8( diffeomorphismSize );
    directTransformedFloatingU8.setResolution( diffeomorphismResolution );
    gkg::Volume< uint8_t > 
      inverseTransformedReferenceU8( diffeomorphismSize );
    inverseTransformedReferenceU8.setResolution( diffeomorphismResolution );

    ///////////////////////////// looping over configuration(s) //////////////
    for ( c = 0; c < optimizerConfigurationCount; c++ )
    {

      // compute direct similarity measure before deformation
      directSimilarityValueBefore = directSimilarityMeasure->getValueAt(
                                                               *transform3d );

      // computing resampled floating volume with direct transformation
      directSimilarityMeasure->computeTransformedFloatingU8(
                                              *transform3d,
                                              directTransformedFloatingU8 );

      // computing resampled reference volume with inverse transformation
      inverseSimilarityMeasure->computeTransformedFloatingU8(
                                              *inverseTransform3d,
                                              inverseTransformedReferenceU8 );


      gkg::Writer::getInstance().write(
              std::string( "Cyril/warpedVolume1_" ) + 
              gkg::StringConverter::toString( c ) + ".ima",
              directTransformedFloatingU8 );

      gkg::Writer::getInstance().write(
              std::string( "Cyril/warpedVolume2_" ) + 
              gkg::StringConverter::toString( c ) + ".ima",
              inverseTransformedReferenceU8 );


      // computing the current 
      const int32_t& currentHalfWidthSearchSpace = halfWidthSearchSpaces[ c ];
      const int32_t& currentControlPointSpacing = controlPointSpacings[ c ];
      const int32_t& currentSearchSpaceQuantisation = 
                                                searchSpaceQuantisations[ c ];
      const float& currentRegularizationWeight = regularizationWeights[ c ];


      currentSearchSpaceSize = 2 * currentHalfWidthSearchSpace + 1;
      currentSearchSpaceVoxelCount = currentSearchSpaceSize *
                                     currentSearchSpaceSize *
                                     currentSearchSpaceSize;
      currentControlPointVolumeSize = diffeomorphismSize /
                                      currentControlPointSpacing;
      currentControlPointCount = currentControlPointVolumeSize.x *
                                 currentControlPointVolumeSize.y *
                                 currentControlPointVolumeSize.z;
      currentControlPointVolumeResolution =
                                         controlPointVolumeResolution *
                                         ( double )currentControlPointSpacing;
      std::ofstream os( std::string( "Cyril/1_" ) +
                        gkg::StringConverter::toString( c ) + ".txt" );
      os << "currentHalfWidthSearchSpace=" << currentHalfWidthSearchSpace
         << std::endl;
      os << "currentControlPointSpacing=" << currentControlPointSpacing
         << std::endl;
      os << "currentSearchSpaceQuantisation=" << currentSearchSpaceQuantisation
         << std::endl;
      os << "currentControlPointVolumeSize=" << currentControlPointVolumeSize
         << std::endl;
      os.close();


      if ( verbose )
      {

        std::cout << "======================================================="
                  << std::endl;
        std::cout << "configuration index: " << c + 1
                  << " grid size: " << currentControlPointVolumeSize
                  << " half width search space: "
                  << currentHalfWidthSearchSpace
                  << std::endl;

      }

      currentDirectFlowFieldAlongX.reallocate( currentControlPointVolumeSize,
                                               1,
                                               false );
      currentDirectFlowFieldAlongX.setResolution( 
                                        currentControlPointVolumeResolution );
      currentDirectFlowFieldAlongY.reallocate( currentControlPointVolumeSize,
                                               1,
                                               false );
      currentDirectFlowFieldAlongY.setResolution( 
                                        currentControlPointVolumeResolution );
      currentDirectFlowFieldAlongZ.reallocate( currentControlPointVolumeSize,
                                               1,
                                               false );
      currentDirectFlowFieldAlongZ.setResolution( 
                                        currentControlPointVolumeResolution );

      currentInverseFlowFieldAlongX.reallocate( currentControlPointVolumeSize,
                                                1,
                                                false );
      currentInverseFlowFieldAlongX.setResolution( 
                                        currentControlPointVolumeResolution );
      currentInverseFlowFieldAlongY.reallocate( currentControlPointVolumeSize,
                                                1,
                                                false );
      currentInverseFlowFieldAlongY.setResolution( 
                                        currentControlPointVolumeResolution );
      currentInverseFlowFieldAlongZ.reallocate( currentControlPointVolumeSize,
                                                1,
                                                false );
      currentInverseFlowFieldAlongZ.setResolution( 
                                        currentControlPointVolumeResolution );

      deformationResampler->resample( coarserDirectFlowFieldAlongX,
                                      identityTransform3d,
                                      0.0f,
                                      currentDirectFlowFieldAlongX );
      deformationResampler->resample( coarserDirectFlowFieldAlongY,
                                      identityTransform3d,
                                      0.0f,
                                      currentDirectFlowFieldAlongY );
      deformationResampler->resample( coarserDirectFlowFieldAlongZ,
                                      identityTransform3d,
                                      0.0f,
                                      currentDirectFlowFieldAlongZ );

      deformationResampler->resample( coarserInverseFlowFieldAlongX,
                                      identityTransform3d,
                                      0.0f,
                                      currentInverseFlowFieldAlongX );
      deformationResampler->resample( coarserInverseFlowFieldAlongY,
                                      identityTransform3d,
                                      0.0f,
                                      currentInverseFlowFieldAlongY );
      deformationResampler->resample( coarserInverseFlowFieldAlongZ,
                                      identityTransform3d,
                                      0.0f,
                                      currentInverseFlowFieldAlongZ );

      gkg::Writer::getInstance().write(
              std::string( "Cyril/coarserDirectFlowFieldAlongX" ) + 
              gkg::StringConverter::toString( c ) + ".ima",
              coarserDirectFlowFieldAlongX );
      gkg::Writer::getInstance().write(
              std::string( "Cyril/coarserDirectFlowFieldAlongY" ) + 
              gkg::StringConverter::toString( c ) + ".ima",
              coarserDirectFlowFieldAlongY );
      gkg::Writer::getInstance().write(
              std::string( "Cyril/coarserDirectFlowFieldAlongZ" ) + 
              gkg::StringConverter::toString( c ) + ".ima",
              coarserDirectFlowFieldAlongZ );

      gkg::Writer::getInstance().write(
              std::string( "Cyril/currentDirectFlowFieldAlongX" ) + 
              gkg::StringConverter::toString( c ) + ".ima",
              currentDirectFlowFieldAlongX );
      gkg::Writer::getInstance().write(
              std::string( "Cyril/currentDirectFlowFieldAlongY" ) + 
              gkg::StringConverter::toString( c ) + ".ima",
              currentDirectFlowFieldAlongY );
      gkg::Writer::getInstance().write(
              std::string( "Cyril/currentDirecteFlowFieldAlongZ" ) + 
              gkg::StringConverter::toString( c ) + ".ima",
              currentDirectFlowFieldAlongZ );


      gkg::Writer::getInstance().write(
              std::string( "Cyril/coarserInverseFlowFieldAlongX" ) + 
              gkg::StringConverter::toString( c ) + ".ima",
              coarserInverseFlowFieldAlongX );
      gkg::Writer::getInstance().write(
              std::string( "Cyril/coarserInverseFlowFieldAlongY" ) + 
              gkg::StringConverter::toString( c ) + ".ima",
              coarserInverseFlowFieldAlongY );
      gkg::Writer::getInstance().write(
              std::string( "Cyril/coarserInverseFlowFieldAlongZ" ) + 
              gkg::StringConverter::toString( c ) + ".ima",
              coarserInverseFlowFieldAlongZ );

      gkg::Writer::getInstance().write(
              std::string( "Cyril/currentInverseFlowFieldAlongX" ) + 
              gkg::StringConverter::toString( c ) + ".ima",
              currentInverseFlowFieldAlongX );
      gkg::Writer::getInstance().write(
              std::string( "Cyril/currentInverseFlowFieldAlongY" ) + 
              gkg::StringConverter::toString( c ) + ".ima",
              currentInverseFlowFieldAlongY );
      gkg::Writer::getInstance().write(
              std::string( "Cyril/currentInverseFlowFieldAlongZ" ) + 
              gkg::StringConverter::toString( c ) + ".ima",
              currentInverseFlowFieldAlongZ );


      // computing minimum spanning trees of floating and reference volumes
      gkg::MinimumSpanningTree< uint8_t >
        referenceMinimumSpanningTree(
          *directSimilarityMeasure->getResampledReferenceU8(),
          gkg::Neighborhood3d::Neighborhood3d_6,
          currentControlPointSpacing );

      gkg::Writer::getInstance().write(
              std::string( "Cyril/resampledReferenceU8_" ) + 
              gkg::StringConverter::toString( c ) + ".ima",
              *directSimilarityMeasure->getResampledReferenceU8() );

      os.open( std::string( "Cyril/2_" ) +
                        gkg::StringConverter::toString( c ) + ".txt" );
      for ( int32_t ii = 0; ii < referenceMinimumSpanningTree.getNodeCount();
            ii++ )
      {

        os << referenceMinimumSpanningTree.getNodeIndex( ii ) << " "
           << referenceMinimumSpanningTree.getParentNodeIndex( ii )
           << std::endl;

      }
      os.close();

      gkg::MinimumSpanningTree< uint8_t >
        floatingMinimumSpanningTree(
          *directSimilarityMeasure->getResampledFloatingU8(),
          gkg::Neighborhood3d::Neighborhood3d_6,
          currentControlPointSpacing );

      gkg::Writer::getInstance().write(
              std::string( "Cyril/resampledFloatingU8_" ) + 
              gkg::StringConverter::toString( c ) + ".ima",
              *directSimilarityMeasure->getResampledFloatingU8() );

      os.open( std::string( "Cyril/3_" ) +
                        gkg::StringConverter::toString( c ) + ".txt" );
      for ( int32_t ii = 0; ii < floatingMinimumSpanningTree.getNodeCount();
            ii++ )
      {

        os << floatingMinimumSpanningTree.getNodeIndex( ii ) << " "
           << floatingMinimumSpanningTree.getParentNodeIndex( ii )
           << std::endl;

      }
      os.close();

      // computing all the direct displacement cost(s)
      std::vector< float > 
        directDisplacementCosts( currentControlPointCount * 
                                 currentSearchSpaceVoxelCount, 0.0f );
      computingDisplacementCosts(
                       *( directSimilarityMeasure->getResampledReferenceU8() ),
                       directTransformedFloatingU8,
                       currentHalfWidthSearchSpace,
                       currentControlPointSpacing,
                       currentSearchSpaceQuantisation,
                       currentRegularizationWeight,
                       directDisplacementCosts );

      gkg::Writer::getInstance().write(
        std::string( "Cyril/4_source_" ) +
        gkg::StringConverter::toString( c ) + ".ima",
        *( directSimilarityMeasure->getResampledReferenceU8() ) );
      gkg::Writer::getInstance().write(
        std::string( "Cyril/4_destination_" ) +
        gkg::StringConverter::toString( c ) + ".ima",
        directTransformedFloatingU8 );

      os.open( std::string( "Cyril/4_" ) +
                        gkg::StringConverter::toString( c ) + ".txt" );
      for ( int32_t ii = 0; ii < currentControlPointCount * 
                                 currentSearchSpaceVoxelCount;
            ii++ )
      {

        os << ii<< " : " << directDisplacementCosts[ ii ]
           << std::endl;

      }
      os.close();


      // computing all the inverse displacement cost(s)
      std::vector< float > 
        inverseDisplacementCosts( currentControlPointCount * 
                                  currentSearchSpaceVoxelCount, 0.0f );
      computingDisplacementCosts(
                      *( directSimilarityMeasure->getResampledFloatingU8() ),
                      inverseTransformedReferenceU8,
                      currentHalfWidthSearchSpace,
                      currentControlPointSpacing,
                      currentSearchSpaceQuantisation,
                      currentRegularizationWeight,
                      inverseDisplacementCosts );
      gkg::Writer::getInstance().write(
        std::string( "Cyril/5_source_" ) +
        gkg::StringConverter::toString( c ) + ".ima",
        *( directSimilarityMeasure->getResampledFloatingU8() ) );
      gkg::Writer::getInstance().write(
        std::string( "Cyril/5_destination_" ) +
        gkg::StringConverter::toString( c ) + ".ima",
        inverseTransformedReferenceU8 );

      os.open( std::string( "Cyril/5_" ) +
                        gkg::StringConverter::toString( c ) + ".txt" );
      for ( int32_t ii = 0; ii < currentControlPointCount * 
                                 currentSearchSpaceVoxelCount;
            ii++ )
      {

        os << ii<< " : " << inverseDisplacementCosts[ ii ]
           << std::endl;

      }
      os.close();

      // reallocating coarser direct and inverse flow fields
      coarserDirectFlowFieldAlongX.reallocate( currentControlPointVolumeSize,
                                               1,
                                               false );
      coarserDirectFlowFieldAlongX.fill( 0.0f );
      coarserDirectFlowFieldAlongY.reallocate( currentControlPointVolumeSize,
                                               1,
                                               false );
      coarserDirectFlowFieldAlongY.fill( 0.0f );
      coarserDirectFlowFieldAlongZ.reallocate( currentControlPointVolumeSize,
                                               1,
                                               false );
      coarserDirectFlowFieldAlongZ.fill( 0.0f );
 
      coarserInverseFlowFieldAlongX.reallocate( currentControlPointVolumeSize,
                                                1,
                                                false );
      coarserInverseFlowFieldAlongX.fill( 0.0f );
      coarserInverseFlowFieldAlongY.reallocate( currentControlPointVolumeSize,
                                                1,
                                                false );
      coarserInverseFlowFieldAlongY.fill( 0.0f );
      coarserInverseFlowFieldAlongZ.reallocate( currentControlPointVolumeSize,
                                                1,
                                                false );
      coarserInverseFlowFieldAlongZ.fill( 0.0f );


      // attributing optimal displacement(s) to each node of the direct MST
      attributeOptimalDisplacements( currentDirectFlowFieldAlongX,
                                     currentDirectFlowFieldAlongY,
                                     currentDirectFlowFieldAlongZ,
                                     directDisplacementCosts,
                                     currentHalfWidthSearchSpace,
                                     currentSearchSpaceQuantisation,
                                     referenceMinimumSpanningTree,
                                     coarserDirectFlowFieldAlongX,
                                     coarserDirectFlowFieldAlongY,
                                     coarserDirectFlowFieldAlongZ );

      gkg::Writer::getInstance().write(
        std::string( "Cyril/10_coarserDirectFlowFieldAlongX_" ) +
        gkg::StringConverter::toString( c ) + ".ima",
        coarserDirectFlowFieldAlongX );
      gkg::Writer::getInstance().write(
        std::string( "Cyril/10_coarserDirectFlowFieldAlongY_" ) +
        gkg::StringConverter::toString( c ) + ".ima",
        coarserDirectFlowFieldAlongY );
      gkg::Writer::getInstance().write(
        std::string( "Cyril/10_coarserDirectFlowFieldAlongZ_" ) +
        gkg::StringConverter::toString( c ) + ".ima",
        coarserDirectFlowFieldAlongZ );

      std::cout << "attributeOptimalDisplacements direct flow" << std::endl;

      // attributing optimal displacement(s) to each node of the inverse MST
      attributeOptimalDisplacements( currentInverseFlowFieldAlongX,
                                     currentInverseFlowFieldAlongY,
                                     currentInverseFlowFieldAlongZ,
                                     inverseDisplacementCosts,
                                     currentHalfWidthSearchSpace,
                                     currentSearchSpaceQuantisation,
                                     floatingMinimumSpanningTree,
                                     coarserInverseFlowFieldAlongX,
                                     coarserInverseFlowFieldAlongY,
                                     coarserInverseFlowFieldAlongZ );

      gkg::Writer::getInstance().write(
        std::string( "Cyril/11_coarserInverseFlowFieldAlongX_" ) +
        gkg::StringConverter::toString( c ) + ".ima",
        coarserInverseFlowFieldAlongX );
      gkg::Writer::getInstance().write(
        std::string( "Cyril/11_coarserInverseFlowFieldAlongY_" ) +
        gkg::StringConverter::toString( c ) + ".ima",
        coarserInverseFlowFieldAlongY );
      gkg::Writer::getInstance().write(
        std::string( "Cyril/11_coarserInverseFlowFieldAlongZ_" ) +
        gkg::StringConverter::toString( c ) + ".ima",
        coarserInverseFlowFieldAlongZ );
      std::cout << "attributeOptimalDisplacements inverse flow" << std::endl;

      // scaling and squaring the direct transformation
      scaleAndSquareTransformation( coarserDirectFlowFieldAlongX,
                                          coarserDirectFlowFieldAlongY,
                                          coarserDirectFlowFieldAlongZ,
                                          currentControlPointSpacing,
                                          4 );
      gkg::Writer::getInstance().write(
        std::string( "Cyril/12_coarserDirectFlowFieldAlongX_" ) +
        gkg::StringConverter::toString( c ) + ".ima",
        coarserDirectFlowFieldAlongX );
      gkg::Writer::getInstance().write(
        std::string( "Cyril/12_coarserDirectFlowFieldAlongY_" ) +
        gkg::StringConverter::toString( c ) + ".ima",
        coarserDirectFlowFieldAlongY );
      gkg::Writer::getInstance().write(
        std::string( "Cyril/12_coarserDirectFlowFieldAlongZ_" ) +
        gkg::StringConverter::toString( c ) + ".ima",
        coarserDirectFlowFieldAlongZ );
      std::cout << "scaleAndSquareTransformation direct flow" << std::endl;


      // scaling and squaring the inverse transformation
      scaleAndSquareTransformation( coarserInverseFlowFieldAlongX,
                                          coarserInverseFlowFieldAlongY,
                                          coarserInverseFlowFieldAlongZ,
                                          currentControlPointSpacing,
                                          4 );
      gkg::Writer::getInstance().write(
        std::string( "Cyril/13_coarserInverseFlowFieldAlongX_" ) +
        gkg::StringConverter::toString( c ) + ".ima",
        coarserInverseFlowFieldAlongX );
      gkg::Writer::getInstance().write(
        std::string( "Cyril/13_coarserInverseFlowFieldAlongY_" ) +
        gkg::StringConverter::toString( c ) + ".ima",
        coarserInverseFlowFieldAlongY );
      gkg::Writer::getInstance().write(
        std::string( "Cyril/13_coarserInverseFlowFieldAlongZ_" ) +
        gkg::StringConverter::toString( c ) + ".ima",
        coarserInverseFlowFieldAlongZ );
      std::cout << "scaleAndSquareTransformation invertse flow" << std::endl;



      // making direct and inverse transformations symmetric
      symmetrizeDirectAndInverseTransformations(
                                          coarserInverseFlowFieldAlongX,
                                          coarserInverseFlowFieldAlongY,
                                          coarserInverseFlowFieldAlongZ,
                                          coarserDirectFlowFieldAlongX,
                                          coarserDirectFlowFieldAlongY,
                                          coarserDirectFlowFieldAlongZ,
                                          currentControlPointSpacing );

      gkg::Writer::getInstance().write(
        std::string( "Cyril/14_coarserDirectFlowFieldAlongX_" ) +
        gkg::StringConverter::toString( c ) + ".ima",
        coarserDirectFlowFieldAlongX );
      gkg::Writer::getInstance().write(
        std::string( "Cyril/14_coarserDirectFlowFieldAlongY_" ) +
        gkg::StringConverter::toString( c ) + ".ima",
        coarserDirectFlowFieldAlongY );
      gkg::Writer::getInstance().write(
        std::string( "Cyril/14_coarserDirectFlowFieldAlongZ_" ) +
        gkg::StringConverter::toString( c ) + ".ima",
        coarserDirectFlowFieldAlongZ );

      gkg::Writer::getInstance().write(
        std::string( "Cyril/15_coarserInverseFlowFieldAlongX_" ) +
        gkg::StringConverter::toString( c ) + ".ima",
        coarserInverseFlowFieldAlongX );
      gkg::Writer::getInstance().write(
        std::string( "Cyril/15_coarserInverseFlowFieldAlongY_" ) +
        gkg::StringConverter::toString( c ) + ".ima",
        coarserInverseFlowFieldAlongY );
      gkg::Writer::getInstance().write(
        std::string( "Cyril/15_coarserInverseFlowFieldAlongZ_" ) +
        gkg::StringConverter::toString( c ) + ".ima",
        coarserInverseFlowFieldAlongZ );

      std::cout << "symmetrizeDirectAndInverseTransformations" << std::endl;

      // computing Jacobian determinant
      float jacobianMean = 0.0f;
      float jacobianStandardDeviation = 0.0f;
      float jacobianMinimum = 0.0f;
      float jacobianMaximum = 0.0f;
      float jacobianNegativeRatio = 0.0f;
      computeJacobianDeterminant( coarserDirectFlowFieldAlongX,
                                        coarserDirectFlowFieldAlongY,
                                        coarserDirectFlowFieldAlongZ,
                                        currentControlPointSpacing,
                                        jacobianMean,
                                        jacobianStandardDeviation,
                                        jacobianMinimum,
                                        jacobianMaximum,
                                        jacobianNegativeRatio );
      std::cout << "computeJacobianDeterminant" << std::endl;
      if ( verbose )
      {

         std::cout << "Jacobian of deformations :"
                   << " mean=" << jacobianMean
                   << " std=" << jacobianStandardDeviation
                   << " min=" << jacobianMinimum
                   << " max=" << jacobianMaximum
                   << " %neg=" << jacobianNegativeRatio * 100.0f
                   << std::endl;

      }

      // upsampling direct transformation to full-resolution grid
      deformationResampler->resample( coarserDirectFlowFieldAlongX,
                                      identityTransform3d,
                                      0.0f,
                                      directFlowFieldAlongX );
      deformationResampler->resample( coarserDirectFlowFieldAlongY,
                                      identityTransform3d,
                                      0.0f,
                                      directFlowFieldAlongY );
      deformationResampler->resample( coarserDirectFlowFieldAlongZ,
                                      identityTransform3d,
                                      0.0f,
                                      directFlowFieldAlongZ );

      gkg::Writer::getInstance().write(
        std::string( "Cyril/16_directFlowFieldAlongX_" ) +
        gkg::StringConverter::toString( c ) + ".ima",
        directFlowFieldAlongX );
      gkg::Writer::getInstance().write(
        std::string( "Cyril/16_directFlowFieldAlongY_" ) +
        gkg::StringConverter::toString( c ) + ".ima",
        directFlowFieldAlongY );
      gkg::Writer::getInstance().write(
        std::string( "Cyril/16_directFlowFieldAlongZ_" ) +
        gkg::StringConverter::toString( c ) + ".ima",
        directFlowFieldAlongZ );


      std::cout << "upsampling direct transformation to full-resolution grid" << std::endl;

      // upsampling inverse transformation to full-resolution grid
      deformationResampler->resample( coarserInverseFlowFieldAlongX,
                                      identityTransform3d,
                                      0.0f,
                                      inverseFlowFieldAlongX );
      deformationResampler->resample( coarserInverseFlowFieldAlongY,
                                      identityTransform3d,
                                      0.0f,
                                      inverseFlowFieldAlongY );
      deformationResampler->resample( coarserInverseFlowFieldAlongZ,
                                      identityTransform3d,
                                      0.0f,
                                      inverseFlowFieldAlongZ );

      gkg::Writer::getInstance().write(
        std::string( "Cyril/17_inverseFlowFieldAlongX_" ) +
        gkg::StringConverter::toString( c ) + ".ima",
        inverseFlowFieldAlongX );
      gkg::Writer::getInstance().write(
        std::string( "Cyril/17_inverseFlowFieldAlongY_" ) +
        gkg::StringConverter::toString( c ) + ".ima",
        inverseFlowFieldAlongY );
      gkg::Writer::getInstance().write(
        std::string( "Cyril/17_inverseFlowFieldAlongZ_" ) +
        gkg::StringConverter::toString( c ) + ".ima",
        inverseFlowFieldAlongZ );
      std::cout << "upsampling inverse transformation to full-resolution grid" << std::endl;

      // computing the harmonic energy of the direct transformation to check
      // whether the transformation can be inverted
      float energy = computeHarmonicEnergy( directFlowFieldAlongX,
                                            directFlowFieldAlongY,
                                            directFlowFieldAlongZ );

      if ( verbose )
      {

        std::cout << "Harmonic energy of deformation field : "
                  << energy
                  << std::endl;

      }

      // now copying back the full-resolution direct and inverse flow fields 
      // along X/Y/Z to the vector field
      vectorFieldTransform3d->setDirectAndInverseVectorFields( 
                                                     directFlowFieldAlongX,
                                                     directFlowFieldAlongY,
                                                     directFlowFieldAlongZ,
                                                     inverseFlowFieldAlongX,
                                                     inverseFlowFieldAlongY,
                                                     inverseFlowFieldAlongZ );
                                   
      // compute direct similarity measure after deformation
      directSimilarityValueAfter = directSimilarityMeasure->getValueAt(
                                                               *transform3d );

      gkg::Volume< uint8_t > 
       intermediateDirectTransformedFloatingU8( diffeomorphismSize );
      intermediateDirectTransformedFloatingU8.setResolution(
                                                     diffeomorphismResolution );
      directSimilarityMeasure->computeTransformedFloatingU8(
                                      *transform3d,
                                      intermediateDirectTransformedFloatingU8 );

      gkg::Writer::getInstance().write(
        std::string( "Cyril/18_intermediateDirectTransformedFloatingU8_" ) +
        gkg::StringConverter::toString( c ) + ".ima",
        intermediateDirectTransformedFloatingU8 );


      if ( verbose )
      {

        std::cout << "Similarity energy before registration : "
                  << directSimilarityValueBefore
                  << " and after : "
                  << directSimilarityValueAfter
                  << std::endl;

      }

      // now, the coarser becomes the current
      coarserControlPointVolumeSize = currentControlPointVolumeSize;
      coarserControlPointVolumeResolution =
                                          currentControlPointVolumeResolution;

    }

  }
  GKG_CATCH_COMMAND( result );

  return result;

}

