#ifndef _gkg_dmri_connectivity_matrix_VolumeToMeshConnectivityMatrixFunctor_i_h_
#define _gkg_dmri_connectivity_matrix_VolumeToMeshConnectivityMatrixFunctor_i_h_

#include <gkg-dmri-connectivity-matrix/VolumeToMeshConnectivityMatrixFunctor.h>
#include <gkg-dmri-connectivity-matrix/ConnectivityMatrixFunctor_i.h>
#include <gkg-dmri-connectivity-matrix/ConnectivityMatrixFactory_i.h>
#include <gkg-dmri-connectivity-matrix/ConnectivityMatrixGauge.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-container/MeshMap_i.h>
#include <gkg-processing-container/TextureMap_i.h>
#include <gkg-processing-transform/IdentityTransform3d.h>
#include <gkg-processing-transform/AffineTransform3d.h>
#include <gkg-processing-transform/RigidTransform3d.h>
#include <gkg-processing-transform/Scaling3d.h>
#include <gkg-processing-transform/CompositeTransform3d.h>
#include <gkg-processing-transform/InverseTransform3d.h>
#include <gkg-processing-mesh/StaticVertexEvolutionFunction.h>
#include <gkg-processing-algobase/Math.h>
#include <gkg-dmri-container/BundleMap_i.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-core-io/StringConverter.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-processing-io/TypedVolumeReaderProcess.h>
#include <gkg-processing-io/SparseMatrixItemIOFactory.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-communication-thread/ThreadedLoop.h>
#include <gkg-core-exception/Exception.h>
#include <fstream>
#include <vector>
#include <iomanip>


#define LABEL_FOR_MESH        -1
#define LABEL_FOR_BACKGROUND   0


//
// class VolumeToMeshLoopContext< L, Compare >
//

template < class L, class Compare >
inline
gkg::VolumeToMeshLoopContext< L, Compare >::VolumeToMeshLoopContext(
     gkg::ConnectivityMatrixGauge& gauge,
     const gkg::VolumeToMeshConnectivityMatrixFunctor< L, Compare >& 
                                          volumeToMeshConnectivityMatrixFunctor,
     const typename gkg::BundleMap< L, Compare >::Bundle& bundle,
     const gkg::Volume< int32_t >& rois1,
     const gkg::BoundingBox< int32_t >& rois1BoundingBox,
     gkg::Transform3d< float >* transform3dFromBundleRealFrameToROIs1VoxelFrame,
     const gkg::RCPointer< gkg::SceneModeler >& sceneModeler,
     const gkg::BoundingBox< float >& rois2BoundingBox,
     const gkg::Volume< uint8_t >& rois2Mask,
     const gkg::RCPointer< gkg::EvolvingMesh >& evolvingMesh,
     gkg::Transform3d< float >* transform3dFromBundleRealFrameToROIs2RealFrame,
     gkg::SparseMatrix& connectivityMatrix,
     gkg::RCPointer< gkg::SparseMatrix >& averageLengthMatrix,
     gkg::RCPointer< gkg::BundleMap< L, Compare > >& roiToMeshBundleMap,
     const bool& verbose )
                         : gkg::LoopContext< int32_t >( &gauge ),
                           _volumeToMeshConnectivityMatrixFunctor(
                                        volumeToMeshConnectivityMatrixFunctor ),
                           _bundle( bundle ),
                           _rois1( rois1 ),
                           _rois1BoundingBox( rois1BoundingBox ),
                           _transform3dFromBundleRealFrameToROIs1VoxelFrame(
                              transform3dFromBundleRealFrameToROIs1VoxelFrame ),
                           _sceneModeler( sceneModeler ),
                           _rois2BoundingBox( rois2BoundingBox ),
                           _rois2Mask( rois2Mask ),
                           _evolvingMesh( evolvingMesh ),
                           _transform3dFromBundleRealFrameToROIs2RealFrame(
                               transform3dFromBundleRealFrameToROIs2RealFrame ),
                           _connectivityMatrix( connectivityMatrix ),
                           _averageLengthMatrix( averageLengthMatrix ),
                           _roiToMeshBundleMap( roiToMeshBundleMap ),
                           _verbose( verbose ),
                           _fiberIndexAlreadyDisplayed( false )
{
}


template < class L, class Compare >
inline
gkg::VolumeToMeshLoopContext< L, Compare >::~VolumeToMeshLoopContext()
{
}


template < class L, class Compare >
inline
void gkg::VolumeToMeshLoopContext< L, Compare >::doIt( int32_t startIndex,
                                                       int32_t count )
{

  try
  {

    int32_t stopIndex = startIndex + count;
    int32_t s, g;

    for ( s = startIndex, g = 0; s < stopIndex; g++, s++ )
    {

      if ( _verbose && _gauge && g && !( g % 100 ) )
      {

        lock();
        _gauge->add( 100 );
        unlock();

      }

      //////////////////////////////////////////////////////////////////////////
      // resampling the fiber at the input resolution
      //////////////////////////////////////////////////////////////////////////

      typename gkg::BundleMap< L, Compare >::Fiber resampledFiber;
      int32_t resampledPointCount = resampleFiber( _bundle.getCurve3d( s ),
                                                   resampledFiber );

      //////////////////////////////////////////////////////////////////////////
      // allocating the label, polygon, distance and project point vectors
      //////////////////////////////////////////////////////////////////////////

      std::vector< int32_t > labels( resampledPointCount,
                                     LABEL_FOR_BACKGROUND );
      std::vector< gkg::Polygon< 3U > > polygons( resampledPointCount );
      std::vector< float > distances( resampledPointCount, 1e38 );
      std::vector< gkg::Vector3d< float > > 
        projectionPoints( resampledPointCount );

      //////////////////////////////////////////////////////////////////////////
      // looping over points of the fiber and determining their label
      //////////////////////////////////////////////////////////////////////////

      findLabels( resampledFiber,
                  labels,
                  polygons,
                  distances,
                  projectionPoints );

      //////////////////////////////////////////////////////////////////////////
      // removing short ranges from the vector of labels
      //////////////////////////////////////////////////////////////////////////

      std::vector< std::pair< int32_t, int32_t > > ranges;
      getRangesAndRemovingShortOnes( resampledFiber, labels, ranges );


      //////////////////////////////////////////////////////////////////////////
      // splitting the current fiber into smallest fibers and storing them into
      // the labelled fiber map; plus storing the polygons and projection point
      // maps required to fill the connectivity matrix
      //////////////////////////////////////////////////////////////////////////

      std::map< L, std::list< gkg::LightCurve3d< float > > > labelledFibers;
      std::map< int32_t, std::list< gkg::Polygon< 3U > > > polygonMap;
      std::map< int32_t, std::list< gkg::Vector3d< float > > > 
                                                             projectionPointMap;
      std::map< int32_t, L > correspondingRois1Label;

      splitFiberAndSortFiberPieces( resampledFiber,
                                    labels,
                                    ranges,
                                    polygons,
                                    distances,
                                    projectionPoints,
                                    labelledFibers,
                                    polygonMap,
                                    projectionPointMap,
                                    correspondingRois1Label );

      //////////////////////////////////////////////////////////////////////////
      // filling :
      // - the connectivity matrix
      // - the average length matrix
      // - the ROI to mesh bundle map
      //////////////////////////////////////////////////////////////////////////

      fillOutputs( labelledFibers,
                   polygonMap,
                   projectionPointMap,
                   correspondingRois1Label );

    }

    if ( _verbose && _gauge )
    {

      lock();
      _gauge->add( count % 100 );
      unlock();

    }

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void gkg::VolumeToMeshLoopContext< L, Compare >::doIt( "
             "int32_t startIndex, int32_t count )" );

}


template < class L, class Compare >
inline
int32_t 
gkg::VolumeToMeshLoopContext< L, Compare >::resampleFiber(
      const typename gkg::BundleMap< L, Compare >::Fiber& fiber,
      typename gkg::BundleMap< L, Compare >::Fiber& resampledFiber ) const
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // resampling the fiber at the input resolution
    ////////////////////////////////////////////////////////////////////////////

    float fiberLength = fiber.getLength();

    int32_t resampledPointCount = std::max( 2, ( int32_t )( fiberLength /
     _volumeToMeshConnectivityMatrixFunctor._inputFiberResamplingStep + 0.5 ) );

    resampledFiber = fiber.getEquidistantCurve( resampledPointCount );

    return resampledPointCount;

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "int32_t  "
             "gkg::VolumeToMeshLoopContext< L, Compare >::"
             "resampleFiber( "
             "const typename gkg::BundleMap< L, Compare >::Fiber& fiber, "
             "const typename gkg::BundleMap< L, Compare >::Fiber& "
             "resampledFiber ) const" );

}


template < class L, class Compare >
inline
void 
gkg::VolumeToMeshLoopContext< L, Compare >::findLabels(
             const typename gkg::BundleMap< L, Compare >::Fiber& resampledFiber,
             std::vector< int32_t >& labels,
             std::vector< gkg::Polygon< 3U > >& polygons,
             std::vector< float >& distances,
             std::vector< gkg::Vector3d< float > >& projectionPoints ) const
{

  try
  {

    float maximumFiberTangentToMeshNormalAngleInRadians =
      ( _volumeToMeshConnectivityMatrixFunctor.
        _maximumFiberTangentToMeshNormalAngleInDegrees * M_PI ) / 180.0;

    gkg::Vector3d< int32_t > voxelInRois1VoxelFrame;
    gkg::Vector3d< int32_t > voxelInRois2VoxelFrame;

    gkg::Vector3d< float > pointInRois2RealFrame;
    gkg::Vector3d< float > nextPointInRois2RealFrame;
    gkg::Vector3d< float > tangent;
    gkg::Vector3d< float > normal;

    gkg::LightCurve3d< float >::const_iterator p = resampledFiber.begin(),
                                               pe = resampledFiber.end();
    gkg::LightCurve3d< float >::const_iterator nextP;
    std::vector< int32_t >::iterator l = labels.begin();
    std::vector< gkg::Polygon< 3U > >::iterator po = polygons.begin();
    std::vector< float >::iterator d = distances.begin();
    std::vector< gkg::Vector3d< float > >::iterator 
                                                  pr = projectionPoints.begin();

    while ( p != pe )
    {

      // processing the voxel coordinates in the ROIs1 voxel frame
      _transform3dFromBundleRealFrameToROIs1VoxelFrame->getIntegerDirect(
                                                   *p, voxelInRois1VoxelFrame );

      // processing the real coordinates in the ROIs2 real frame
      _transform3dFromBundleRealFrameToROIs2RealFrame->getDirect(
                                                    *p, pointInRois2RealFrame );

      // processing the voxel coordinates in the ROIs2 voxel frame
      _sceneModeler->getCacheVoxel( pointInRois2RealFrame,  
                                    voxelInRois2VoxelFrame );


      // first, we check if the voxel belongs to a ROI, and if so,
      // collecting its label
      if ( _rois1BoundingBox.contains( voxelInRois1VoxelFrame ) )
      {

        *l = _rois1( voxelInRois1VoxelFrame );

      }

      // if the voxel does not belong to a ROI
      if ( *l == LABEL_FOR_BACKGROUND )
      {

        // if we are inside the bounding box of the ROIs2 mesh
        if ( _rois2BoundingBox.contains( pointInRois2RealFrame ) )
        {

          // if we are close to the ROIs2 mesh
          if ( _rois2Mask( voxelInRois2VoxelFrame ) )
          {

            if ( _volumeToMeshConnectivityMatrixFunctor.
                          _maximumFiberTangentToMeshNormalAngleInDegrees > 0.0 )


            {

              _evolvingMesh->
                   getFastClosestPolygonAndDistanceAndProjectionPointAndNormal(
                                                         pointInRois2RealFrame,
                                                         voxelInRois2VoxelFrame,
                                                         *po,
                                                         *d,
                                                         *pr,
                                                         normal );
              // when the polygon cache is created, the distance threshold 
              // consists of the minimum distance to the closest vertex of the
              // triangle, and not to the distance to the triangle globally 
              // so, it may produce distances bigger than the one asked; as a
              // a consequence, we need to perform a further test on the 
              // distance here; in addition, a condition about the angle between
              // the fiber tangent and the normal to the surface is added

              if ( *d <= _volumeToMeshConnectivityMatrixFunctor.
                                                        _maximumDistanceToMesh )
              {

                nextP = p + 1;
                if ( nextP != pe )
                {
 
                  _transform3dFromBundleRealFrameToROIs2RealFrame->getDirect(
                                            *nextP, nextPointInRois2RealFrame );
 
                }
                else
                {
 
                  nextP = p - 1;
                  _transform3dFromBundleRealFrameToROIs2RealFrame->getDirect(
                                            *nextP, nextPointInRois2RealFrame );

                }
                tangent = nextPointInRois2RealFrame - pointInRois2RealFrame;
 
                if ( gkg::getLineAngles( tangent, normal ) <=
                                maximumFiberTangentToMeshNormalAngleInRadians )
                {

                  *l = LABEL_FOR_MESH;

                }

              }

            }
            else
            {

              _evolvingMesh->getFastClosestPolygonAndDistanceAndProjectionPoint(
                                                         pointInRois2RealFrame,
                                                         voxelInRois2VoxelFrame,
                                                         *po,
                                                         *d,
                                                         *pr );
              // when the polygon cache is created, the distance threshold 
              // consists of the minimum distance to the closest vertex of the
              // triangle, and not to the distance to the triangle globally 
              // so, it may produce distances bigger than the one asked; as a
              // a consequence, we need to perform a further test on the 
              // distance here

              if ( *d <= _volumeToMeshConnectivityMatrixFunctor.
                                                        _maximumDistanceToMesh )
              {

                *l = LABEL_FOR_MESH;

              }

            }

          }

        }

      }

      ++ p;
      ++ l;
      ++ po;
      ++ d;
      ++ pr;

    }

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void  "
             "gkg::VolumeToMeshLoopContext< L, Compare >::"
             "findLabels( "
             "const typename gkg::BundleMap< L, Compare >::Fiber& "
             "resampledFiber, "
             "std::vector< int32_t >& labels, "
             "std::vector< gkg::Polygon< 3U > >& polygons, "
             "std::vector< float >& distances, "
             "std::vector< gkg::Vector3d< float > >& projectionPoints ) "
             "const" );

}


template < class L, class Compare >
inline
void 
gkg::VolumeToMeshLoopContext< L, Compare >::getRangesAndRemovingShortOnes(
             const typename gkg::BundleMap< L, Compare >::Fiber& resampledFiber,
             std::vector< int32_t >& labels,
             std::vector< std::pair< int32_t, int32_t > >& ranges ) const
{

  try
  {

    int32_t resampledPointCount = ( int32_t )labels.size();
    int32_t i = 0;
    int32_t j = 0;
    std::pair< int32_t, int32_t > range;
    int32_t label = LABEL_FOR_BACKGROUND;
    float length = 0.0f;
    for ( i = 0; i < resampledPointCount; i++ )
    {

      label = getRange( labels, i, range );
      length = resampledFiber.getLength( range.first, range.second );

      // if a range is found
      if ( label )
      {

        // we forbid small ranges only for label stemming from ROIs1
        if ( label > LABEL_FOR_BACKGROUND )
        {

          // if the range is too small inside ROIs1, just delete it
          if ( length < 
               _volumeToMeshConnectivityMatrixFunctor.
                                                 _minimumRoiIntersectionLength )
          {

            for ( j = range.first; j <= range.second; j++ )
            {

              labels[ j ] = LABEL_FOR_BACKGROUND;

            }

          }
          else
          {

            ranges.push_back( range );

          }
          i = range.second + 1;

        }
        else
        {
        
          ranges.push_back( range );
          i = range.second + 1;

        }

      }
      // else if no range is found
      else
      {

        break;

      }

    }

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void  "
             "gkg::VolumeToMeshLoopContext< L, Compare >::"
             "getRangesAndRemovingShortOnes( "
             "const typename gkg::BundleMap< L, Compare >::Fiber& "
             "resampledFiber, "
             "std::vector< int32_t >& labels, "
             "std::vector< std::pair< int32_t, int32_t > >& ranges ) const" );

}


template < class L, class Compare >
inline
int32_t gkg::VolumeToMeshLoopContext< L, Compare >::getRange(
                                    const std::vector< int32_t >& labels,
                                    int32_t index,
                                    std::pair< int32_t, int32_t >& range ) const
{

  try
  {

    int32_t startIndex = index;
    int32_t pointCount = ( int32_t )labels.size();

    if ( ( index < 0 ) || ( index >= pointCount ) )
    {

      throw std::runtime_error( "bad index" );

    }

    // looking for the first point of the range
    while ( startIndex < pointCount )
    {

      if ( labels[ startIndex ] != 0 )
      {

        break;

      }

      ++ startIndex;

    }

    // if found
    if ( startIndex < pointCount )
    {

      // collecting the corresponding label
      int32_t label = labels[ startIndex ];
      int32_t endIndex = startIndex;

      // looking for the end point of the range
      while ( endIndex < pointCount )
      {

        if ( labels[ endIndex ] != label )
        {

          break;

        }

        ++ endIndex;

      }

      // filling the range information
      range.first = startIndex;
      range.second = endIndex - 1;

      return label;

    }
    return 0;

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "int32_t "
             "gkg::VolumeToMeshLoopContext< L, Compare >::"
             "getRange( "
             "const std::vector< int32_t >& labels, "
             "int32_t index, "
             "std::pair< int32_t, int32_t >& range ) const" );

}


template < class L, class Compare >
inline
void gkg::VolumeToMeshLoopContext< L, Compare >::splitFiberAndSortFiberPieces(
   const typename gkg::BundleMap< L, Compare >::Fiber& resampledFiber,
   const std::vector< int32_t >& labels,
   const std::vector< std::pair< int32_t, int32_t > >& ranges,
   const std::vector< gkg::Polygon< 3U > >& polygons,
   const std::vector< float >& distances,
   const std::vector< gkg::Vector3d< float > >& projectionPoints,
   std::map< L, std::list< gkg::LightCurve3d< float > > >& labelledFibers,
   std::map< int32_t, std::list< gkg::Polygon< 3U > > >& polygonMap,
   std::map< int32_t, std::list< gkg::Vector3d< float > > >& projectionPointMap,
  std::map< int32_t, L >& correspondingRois1Label ) const
{

  try
  {

    int32_t resampledPointCount = resampledFiber.getPointCount();
    int32_t rangeCount = ( int32_t )ranges.size();
    int32_t r1 = -1;
    int32_t r2 = 0;
    int32_t index = 0;
    int32_t index1 = 0;
    int32_t index2 = 0;
    int32_t indexOfMinimumDistance = 0;
    float minimumDistance;
    int32_t label1 = LABEL_FOR_BACKGROUND;
    int32_t label2 = LABEL_FOR_BACKGROUND;
    float distance = 0.0;

    for ( ; r1 < rangeCount; r1++, r2++ )
    {

      if ( r1 == -1 )
      {

        index1 = 0;
        label1 = LABEL_FOR_BACKGROUND;

      }
      else
      {

        index1 = ranges[ r1 ].first;
        label1 = labels[ ranges[ r1 ].first ];

      }
      if ( r2 == rangeCount )
      {

        index2 = resampledPointCount - 1;
        label2 = LABEL_FOR_BACKGROUND;

      }
      else
      {

        index2 = ranges[ r2 ].second;
        label2 = labels[ ranges[ r2 ].second ];

      }

      if ( ( label1 == LABEL_FOR_MESH ) &&
           ( label2 > LABEL_FOR_BACKGROUND ) )
      {

        indexOfMinimumDistance = index1;
        minimumDistance = distances[ index1 ];
        index = index1;
        do
        {

          distance = distances[ index ];
          if ( distance < minimumDistance )
          {

            indexOfMinimumDistance = index;
            minimumDistance = distance;

          }
          ++ index;

        }
        while ( labels[ index ] == LABEL_FOR_MESH );

        L newLabel = getLabel( label2 );
        labelledFibers[ newLabel ].push_back(
          resampledFiber.getEquidistantCurvePart(
          index2,
          indexOfMinimumDistance,
          _volumeToMeshConnectivityMatrixFunctor._outputFiberResamplingStep ) );

        polygonMap[ label2 ].push_back( polygons[ indexOfMinimumDistance ] );
        projectionPointMap[ label2 ].push_back( 
                                   projectionPoints[ indexOfMinimumDistance ] );
        correspondingRois1Label[ label2 ] = newLabel;

      }
      else if ( ( label1 > LABEL_FOR_BACKGROUND ) &&
                ( label2 == LABEL_FOR_MESH ) )
      {

        indexOfMinimumDistance = index2;
        minimumDistance = distances[ index2 ];
        index = index2;
        do
        {

          distance = distances[ index ];
          if ( distance < minimumDistance )
          {

            indexOfMinimumDistance = index;
            minimumDistance = distance;

          }
          -- index;

        }
        while ( labels[ index ] == LABEL_FOR_MESH );

        L newLabel = getLabel( label1 );
        labelledFibers[ newLabel ].push_back(
          resampledFiber.getEquidistantCurvePart(
          index1,
          indexOfMinimumDistance,
          _volumeToMeshConnectivityMatrixFunctor._outputFiberResamplingStep ) );
        polygonMap[ label1 ].push_back( polygons[ indexOfMinimumDistance ] );
        projectionPointMap[ label1 ].push_back( 
                                   projectionPoints[ indexOfMinimumDistance ] );
        correspondingRois1Label[ label1 ] = newLabel;

      }

    }

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void "
             "gkg::VolumeToMeshLoopContext< L, Compare >::"
             "splitFiberAndSortFiberPieces( "
             "const typename gkg::BundleMap< L, Compare >::Fiber& "
             "resampledFiber, "
             "const std::vector< int32_t >& labels, "
             "const std::vector< std::pair< int32_t, int32_t > >& ranges, "
             "const std::vector< gkg::Polygon< 3U > >& polygons, "
             "const std::vector< float >& distances, "
             "const std::vector< gkg::Vector3d< float > >& projectionPoints, "
             "std::map< L, std::list< gkg::LightCurve3d< float > > >& "
             "labelledFibers, "
             "std::map< int32_t, std::list< gkg::Polygon< 3U > > >& "
             "polygonMap, "
             "std::map< int32_t, std::list< gkg::Vector3d< float > > >& "
             "projectionPointMap,"
             "std::map< int32_t, L >& correspondingRois1Label ) const" );

}


template < class L, class Compare >
inline
void gkg::VolumeToMeshLoopContext< L, Compare >::fillOutputs(
  const std::map< L, std::list< gkg::LightCurve3d< float > > >&  labelledFibers,
  const std::map< int32_t, std::list< gkg::Polygon< 3U > > >&  polygonMap,
  const std::map< int32_t, std::list< gkg::Vector3d< float > > >& 
                                                             projectionPointMap,
  const std::map< int32_t, L >& correspondingRois1Label )
{

  try
  {

    std::map< int32_t, std::list< gkg::Polygon< 3U > > >::const_iterator
      pm = polygonMap.begin(),
      pme = polygonMap.end();
      
    std::map< int32_t, std::list< gkg::Vector3d< float > > >::const_iterator
      ppm = projectionPointMap.begin();

    typename std::map< L, 
                       std::list< gkg::LightCurve3d< float > > >::const_iterator
      b;

    typename std::map< int32_t, L >::const_iterator
      c = correspondingRois1Label.begin();

    float weight1 = 0.0;
    float weight2 = 0.0;
    float weight3 = 0.0;
    float fiberLength = 0.0;

    while( pm != pme )
    {

      const int32_t& label = pm->first;
      const L& newLabel = c->second;
      const std::list< gkg::Polygon< 3U > >& polygons = pm->second;
      const std::list< gkg::Vector3d< float > >& projectionPoints = ppm->second;
      b = labelledFibers.find( newLabel );

      std::list< gkg::Polygon< 3U > >::const_iterator
        po = polygons.begin(),
	poe = polygons.end();

      std::list< gkg::Vector3d< float > >::const_iterator
        pp = projectionPoints.begin();

      std::list< gkg::LightCurve3d< float > >::const_iterator
        f = b->second.begin();

      while( po != poe )
      {
      
        const int32_t& index1 = po->indices[ 0 ];
        const int32_t& index2 = po->indices[ 1 ];
        const int32_t& index3 = po->indices[ 2 ];
        _evolvingMesh->getPartialTriangleAreaWeighting( *po,
                                                        *pp,
                                                        weight1,
                                                        weight2,
                                                        weight3 );
        fiberLength =  f->getLength();   

        if ( !_connectivityMatrix.hasElement( label, index1 ) )
        {

          lock();
          _connectivityMatrix( label, index1 ) = ( double )weight1;
          unlock();

        }
        else
        {

          lock();
          _connectivityMatrix( label, index1 ) += ( double )weight1;
          unlock();

        }
        if ( !_connectivityMatrix.hasElement( label, index2 ) )
        {

          lock();
          _connectivityMatrix( label, index2 ) = ( double )weight2;
          unlock();

        }
        else
        {

          lock();
          _connectivityMatrix( label, index2 ) += ( double )weight2;
          unlock();

        }
        if ( !_connectivityMatrix.hasElement( label, index3 ) )
        {

          lock();
          _connectivityMatrix( label, index3 ) = ( double )weight3;
          unlock();

        }
        else
        {

          lock();
          _connectivityMatrix( label, index3 ) += ( double )weight3;
          unlock();

        }
        
        if ( !_averageLengthMatrix.isNull() )
        {

          if ( !_averageLengthMatrix->hasElement( label, index1 ) )
          {

            lock();
            ( *_averageLengthMatrix )( label, index1 ) =  fiberLength * weight1;
            unlock();

          }
          else
          {

            lock();
            ( *_averageLengthMatrix )( label, index1 ) += fiberLength * weight1;
            unlock();

          }
          if ( !_averageLengthMatrix->hasElement( label, index2 ) )
          {

            lock();
            ( *_averageLengthMatrix )( label, index2 ) = fiberLength * weight2;
            unlock();

          }
          else
          {

            lock();
            ( *_averageLengthMatrix )( label, index2 ) += fiberLength * weight2;
            unlock();

          }
          if ( !_averageLengthMatrix->hasElement( label, index3 ) )
          {

            lock();
            ( *_averageLengthMatrix )( label, index3 ) = fiberLength * weight3;
            unlock();

          }
          else
          {

            lock();
            ( *_averageLengthMatrix )( label, index3 ) += fiberLength * weight3;
            unlock();

          }

        }
     
        ++ po;
        ++ pp;
	++ f;
      
      }

      if ( !_roiToMeshBundleMap.isNull() )
      {

       lock();
        _roiToMeshBundleMap->addCurve3ds( b->first, b->second );
       unlock();

      }
    
      ++ pm;
      ++ ppm;
      ++c;
    
    }

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void gkg::VolumeToMeshLoopContext< L, Compare >::"
             "fillOutputs( "
             "const std::map< L, std::list< gkg::LightCurve3d< float > > >&  "
             "labelledFibers, "
             "const std::map< int32_t, std::list< gkg::Polygon< 3U > > >& "
             "polygonMap, "
             "const std::map< int32_t, std::list< gkg::Vector3d< float > > >& "
             "projectionPointMap ) const" );

}


template < class L, class Compare >
inline
L gkg::VolumeToMeshLoopContext< L, Compare >::getLabel( int32_t label ) const
{

  try
  {

    throw std::runtime_error( "should not enter this code" );

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "L gkg::VolumeToMeshLoopContext< L, Compare >::"
             "getLabel( "
             "int16_t label ) const" );

}


namespace gkg
{


template <>
inline
std::string 
VolumeToMeshLoopContext< std::string, std::less< std::string > >::getLabel(
                                                           int32_t label ) const
{

  try
  {

    return gkg::StringConverter::toString( label );

  }
  GKG_CATCH( "inline "
             "std::string  "
             "VolumeToMeshLoopContext< std::string, "
             "std::less< std::string > >::getLabel( "
             "int32_t label ) const" );

}


template <>
inline
int16_t 
VolumeToMeshLoopContext< int16_t,std::less< int16_t > >::getLabel(
                                                           int32_t label ) const
{

  try
  {

    return ( int16_t )label;

  }
  GKG_CATCH( "inline "
             "int16_t  "
             "VolumeToMeshLoopContext< int16_t, "
             "std::less< int16_t > >::getLabel( "
             "int32_t label ) const" );

}


}


//
// class VolumeToMeshConnectivityMatrixFunctor< L, Compare >
//

template < class L, class Compare >
inline
gkg::VolumeToMeshConnectivityMatrixFunctor< L, Compare >::
                                         VolumeToMeshConnectivityMatrixFunctor(
                        const std::vector< double >& scalarParameters,
                        const std::vector< std::string >& /*stringParameters*/ )
                                : gkg::ConnectivityMatrixFunctor< L, Compare >()
{

  try
  {

    if ( scalarParameters.empty() )
    {

      _minimumRoiIntersectionLength = 1;
      _inputFiberResamplingStep = 0.1;
      _maximumDistanceToMesh = 1.0;
      _outputFiberResamplingStep = 5;
      _maximumFiberTangentToMeshNormalAngleInDegrees = 0.0;
      _smoothConnectivityMap = false;
      _smoothingStandardDeviation = 1.0;
      _cacheSize.x = 100;
      _cacheSize.y = 100;
      _cacheSize.z = 100;

    }
    else
    {

      if ( scalarParameters.size() != 10U )
      {

        throw std::runtime_error( "10 scalar parameters required" );

      }
      _minimumRoiIntersectionLength = ( float )scalarParameters[ 0 ];
      _inputFiberResamplingStep = ( float )scalarParameters[ 1 ];
      _maximumDistanceToMesh = ( float )scalarParameters[ 2 ];
      _outputFiberResamplingStep = ( float )scalarParameters[ 3 ];
      _maximumFiberTangentToMeshNormalAngleInDegrees =
                                                 ( float )scalarParameters[ 4 ];
      _smoothConnectivityMap = ( int32_t )( scalarParameters[ 5 ] + 0.5 )
                                     ? true : false;
      _smoothingStandardDeviation = ( float )scalarParameters[ 6 ];
      _cacheSize.x = ( int32_t )( scalarParameters[ 7 ] + 0.5 );
      _cacheSize.y = ( int32_t )( scalarParameters[ 8 ] + 0.5 );
      _cacheSize.z = ( int32_t )( scalarParameters[ 9 ] + 0.5 );

      // sanity checks
      if ( _minimumRoiIntersectionLength <= 0.0 )
      {

        throw std::runtime_error(
                  "minimum ROI intersection length must be strictly positive" );

      }

      if ( _inputFiberResamplingStep <= 0 )
      {

        throw std::runtime_error(
                      "input fiber resampling step must be strictly positive" );

      }

      if ( _maximumDistanceToMesh <= 0 )
      {

        throw std::runtime_error(
                         "maximum distance to mesh must be strictly positive" );

      }

      if ( _outputFiberResamplingStep <= 0 )
      {

        throw std::runtime_error(
                     "output fiber resampling step must be strictly positive" );

      }

      if ( _maximumFiberTangentToMeshNormalAngleInDegrees < 0 )
      {

        throw std::runtime_error(  "maximum fiber tangent to mesh normal "
                                   "angle must be strictly positive" );

      }

      if ( _smoothingStandardDeviation <= 0 )
      {

        throw std::runtime_error(
                     "smoothing standard deviation must be strictly positive" );

      }

      if ( _cacheSize.x <= 0 )
      {

        throw std::runtime_error( "cache X size must be strictly positive" );

      }

      if ( _cacheSize.y <= 0 )
      {

        throw std::runtime_error( "cache Y size must be strictly positive" );

      }

      if ( _cacheSize.z <= 0 )
      {

        throw std::runtime_error( "cache Z size must be strictly positive" );

      }

    }

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "gkg::VolumeToMeshConnectivityMatrixFunctor< L, Compare >::"
             "VolumeToMeshConnectivityMatrixFunctor( "
             "const std::vector< double >& scalarParameters, "
             "const std::vector< std::string >& /*stringParameters*/ )" );

}


template < class L, class Compare >
inline
gkg::VolumeToMeshConnectivityMatrixFunctor< L, Compare >::
                                        ~VolumeToMeshConnectivityMatrixFunctor()
{
}


template < class L, class Compare >
inline
void gkg::VolumeToMeshConnectivityMatrixFunctor< L, Compare >::apply(
                const std::list< std::string >& fileNameBundleMaps,
                const std::list< std::string >& fileNameListOfROIs1,
                const std::string& fileNameROIs1ToBundleMapTransform3d,
                const std::list< std::string >& fileNameListOfROIs2,
                const std::string& fileNameROIs2ToBundleMapTransform3d,
                const std::list< std::string >& functorNames,
                const std::list< std::string >& outputFileNames,
                const std::string& theOutputBundleMapFormat,
                const std::string& /* theOutputTextureMapFormat */,
                bool ascii,
                bool verbose ) const
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // sanity checks
    ////////////////////////////////////////////////////////////////////////////

    if ( fileNameListOfROIs1.size() != 1U )
    {

      throw std::runtime_error( "only one input volume filename required" );

    }
    if ( fileNameListOfROIs2.size() != 1U )
    {

      throw std::runtime_error( "only one input mesh filename required" );

    }
    if ( functorNames.size() != outputFileNames.size() )
    {

      throw std::runtime_error(
                  "there should be as many output filenames as functor names" );

    }

    ////////////////////////////////////////////////////////////////////////////
    // building functor name map
    ////////////////////////////////////////////////////////////////////////////

    std::map< std::string, std::string > outputFileNameMap;
    std::list< std::string >::const_iterator fn = functorNames.begin(),
                                             fne = functorNames.end();
    std::list< std::string >::const_iterator ofn = outputFileNames.begin();
    while ( fn != fne )
    {


      outputFileNameMap[ *fn ] = *ofn;
      ++ fn;
      ++ ofn;

    }


    ////////////////////////////////////////////////////////////////////////////
    // creating a set of functor for quick search
    ////////////////////////////////////////////////////////////////////////////

    std::set< std::string > setOfFunctorNames( functorNames.begin(),
                                               functorNames.end() );


    ////////////////////////////////////////////////////////////////////////////
    // reading input volume for ROIs1
    ////////////////////////////////////////////////////////////////////////////

    std::string fileNameROIs1 = fileNameListOfROIs1.front();
    if ( verbose )
    {

      std::cout << "reading ROIs1 volume '" << fileNameROIs1 << "'"
                << std::flush;

    }

    gkg::Volume< int32_t > rois1;
    gkg::TypedVolumeReaderProcess< int32_t > typedVolumeReaderProcess( rois1 );
    typedVolumeReaderProcess.execute( fileNameROIs1 );
    gkg::BoundingBox< int32_t > rois1BoundingBox( rois1 );
    gkg::Vector3d< double > resolution;
    rois1.getResolution( resolution );
    
    if ( verbose )
    {

      std::cout << gkg::Eraser( 23 + ( int32_t )fileNameROIs1.size() )
                << std::flush;

    }

    ////////////////////////////////////////////////////////////////////////////
    // reading input mesh for ROIs2
    ////////////////////////////////////////////////////////////////////////////

    std::string fileNameROIs2 = fileNameListOfROIs2.front();
    if ( verbose )
    {

      std::cout << "reading ROIs2 mesh '" << fileNameROIs2 << "'"
                << std::flush;

    }

    gkg::RCPointer< gkg::MeshMap< int32_t, float, 3U > > rois2(
                                       new gkg::MeshMap< int32_t, float, 3U > );
    gkg::Reader::getInstance().read( fileNameROIs2, *rois2 );
    if ( rois2->vertices.getRankCount() != 1 )
    {

      std::cerr << "rois2->vertices.getRankCount()="
                << rois2->vertices.getRankCount()
                << std::endl;
      throw std::runtime_error( "mesh ROI2 must have a unique rank" );

    }
    int32_t vertexCount = rois2->vertices.getSiteCount( 0 );

    if ( verbose )
    {

      std::cout << gkg::Eraser( 21 + ( int32_t )fileNameROIs2.size() )
                << std::flush;

    }

    ////////////////////////////////////////////////////////////////////////////
    // reading 3D transform for ROIs1
    ////////////////////////////////////////////////////////////////////////////

    gkg::RCPointer< gkg::Transform3d< float > >
      transform3dFromROIs1RealToBundleReal;
    if ( fileNameROIs1ToBundleMapTransform3d.empty() )
    {

      if ( verbose )
      {

       std::cout << "initializing 3D ROIs1->Bundles transform to identity"
                 << std::flush;

      }
      // by default, initializaing to inverse scaling
      transform3dFromROIs1RealToBundleReal.reset(
                                        new gkg::IdentityTransform3d< float > );

      if ( verbose )
      {

        std::cout << gkg::Eraser( 52 )
                  << std::flush;

      }

    }
    else
    {

      if ( verbose )
      {

       std::cout << "reading rigid or affine 3D ROIs1->Bundles transform"
                 << std::flush;

      }


      // Read transformation file, first try rigid, then affine
      try
      {

        gkg::RCPointer< gkg::RigidTransform3d< float > > rigidTransform3d;
        rigidTransform3d.reset( new gkg::RigidTransform3d< float > );

        std::ifstream is( fileNameROIs1ToBundleMapTransform3d.c_str() );
        rigidTransform3d->readTrm( is );
        is.close();
        transform3dFromROIs1RealToBundleReal = rigidTransform3d;

      }
      catch ( std::exception& )
      {

        gkg::RCPointer< gkg::AffineTransform3d< float > > affineTransform3d;
        affineTransform3d.reset( new gkg::AffineTransform3d< float > );

        std::ifstream is( fileNameROIs1ToBundleMapTransform3d.c_str() );
        affineTransform3d->readTrm( is );
        is.close();
        transform3dFromROIs1RealToBundleReal = affineTransform3d;

      }

      if ( verbose )
      {

        std::cout << gkg::Eraser( 41 )
                  << std::flush;

      }

    }

    ////////////////////////////////////////////////////////////////////////////
    // reading 3D transform for ROIs2
    ////////////////////////////////////////////////////////////////////////////

    gkg::RCPointer< gkg::Transform3d< float > >
      transform3dFromROIs2RealToBundleReal;
    if ( fileNameROIs2ToBundleMapTransform3d.empty() )
    {

      if ( verbose )
      {

       std::cout << "initializing 3D ROIs2->Bundles transform to identity"
                 << std::flush;

      }
      // by default, initializaing to inverse scaling
      transform3dFromROIs2RealToBundleReal.reset(
                                        new gkg::IdentityTransform3d< float > );

      if ( verbose )
      {

        std::cout << gkg::Eraser( 52 )
                  << std::flush;

      }

    }
    else
    {

      if ( verbose )
      {

       std::cout << "reading rigid or affine 3D ROIs2->Bundles transform"
                 << std::flush;

      }


      // Read transformation file, first try rigid, then affine
      try
      {

        gkg::RCPointer< gkg::RigidTransform3d< float > > rigidTransform3d;
        rigidTransform3d.reset( new gkg::RigidTransform3d< float > );

        std::ifstream is( fileNameROIs2ToBundleMapTransform3d.c_str() );
        rigidTransform3d->readTrm( is );
        is.close();
        transform3dFromROIs2RealToBundleReal = rigidTransform3d;

      }
      catch ( std::exception& )
      {

        gkg::RCPointer< gkg::AffineTransform3d< float > > affineTransform3d;
        affineTransform3d.reset( new gkg::AffineTransform3d< float > );

        std::ifstream is( fileNameROIs2ToBundleMapTransform3d.c_str() );
        affineTransform3d->readTrm( is );
        is.close();
        transform3dFromROIs2RealToBundleReal = affineTransform3d;

      }

      if ( verbose )
      {

        std::cout << gkg::Eraser( 41 )
                  << std::flush;

      }

    }

    ////////////////////////////////////////////////////////////////////////////
    // computing the number of ROIs1 labels
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

     std::cout << "computing labels of ROIs1" << std::flush;

    }
    std::set< int32_t > rois1Labels;
    gkg::Volume< int32_t >::const_iterator
      v = rois1.begin(),
      ve = rois1.end();
    while ( v != ve )
    {

      if ( *v > 0 )
      {

        rois1Labels.insert( *v );

      }
      ++ v;

    }
    int32_t rois1LabelCount = ( int32_t )rois1Labels.size();
    int32_t maximumLabel = ( int32_t )*rois1Labels.rbegin();
    if ( verbose )
    {

      std::cout << gkg::Eraser( 25 )
                << std::flush;

    }


    ////////////////////////////////////////////////////////////////////////////
    // building the mesh cache for ROIs2
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

     std::cout << "computing mesh cache for ROIs2" << std::flush;

    }

    gkg::BoundingBox< float >
      boundingBox( rois2->vertices.getBoundingBox( 0 ) );
    boundingBox.setLowerX( boundingBox.getLowerX() - _maximumDistanceToMesh );
    boundingBox.setLowerY( boundingBox.getLowerY() - _maximumDistanceToMesh );
    boundingBox.setLowerZ( boundingBox.getLowerZ() - _maximumDistanceToMesh );

    //boundingBox.setUpperX( boundingBox.getUpperX() + _maximumDistanceToMesh );
    //boundingBox.setUpperY( boundingBox.getUpperY() + _maximumDistanceToMesh );
    //boundingBox.setUpperZ( boundingBox.getUpperZ() + _maximumDistanceToMesh );

    boundingBox.setLowerX( 0.0 );
    boundingBox.setLowerY( 0.0 );
    boundingBox.setLowerZ( 0.0 );

    //boundingBox.setLowerX( - _maximumDistanceToMesh );
    //boundingBox.setLowerY( - _maximumDistanceToMesh );
    //boundingBox.setLowerZ( - _maximumDistanceToMesh );

    gkg::RCPointer< gkg::SceneModeler > sceneModeler( 
                     new gkg::SceneModeler( boundingBox,
                                            1.0,
                                            1,
                                            1,
                                            _cacheSize,
                                            verbose ) );
    gkg::RCPointer< gkg::VertexEvolutionFunction >
      staticVertexEvolutionFunction( new gkg::StaticVertexEvolutionFunction() );
    gkg::RCPointer< gkg::EvolvingMesh > evolvingMesh(
                           new gkg::EvolvingMesh( sceneModeler,
                                                  rois2,
                                                  staticVertexEvolutionFunction,
                                                  _maximumDistanceToMesh,
                                                  false ) );
    sceneModeler->addEvolvingMesh( "ROIs2", evolvingMesh );
    gkg::Volume< uint8_t > rois2Mask;
    sceneModeler->getEvolvingMeshCache().getBinaryMask( rois2Mask );
    gkg::BoundingBox< float > rois2BoundingBox = sceneModeler->getBoundingBox();

    //gkg::Writer::getInstance().write( "rois2mask.ima", rois2Mask );

    if ( verbose )
    {

      std::cout << gkg::Eraser( 30 )
                << std::flush;

    }

    ////////////////////////////////////////////////////////////////////////////
    // allocating a sparse matrix
    ////////////////////////////////////////////////////////////////////////////

    gkg::SparseMatrix connectivityMatrix( maximumLabel + 1, vertexCount );

    ////////////////////////////////////////////////////////////////////////////
    // allocating average length sparse matrix
    ////////////////////////////////////////////////////////////////////////////

    gkg::RCPointer< gkg::SparseMatrix > averageLengthMatrix;
    if ( setOfFunctorNames.find( "average-length-matrix" ) !=
         setOfFunctorNames.end() )
    {

      averageLengthMatrix.reset( new gkg::SparseMatrix( maximumLabel + 1,
                                                        vertexCount ) );

    }

    ////////////////////////////////////////////////////////////////////////////
    // allocating the output bundle map
    ////////////////////////////////////////////////////////////////////////////

    gkg::RCPointer< gkg::BundleMap< L, Compare > > roiToMeshBundleMap;
    if ( setOfFunctorNames.find( "roi-to-mesh-bundle-map" ) !=
         setOfFunctorNames.end() )
    {

      roiToMeshBundleMap.reset( new gkg::BundleMap< L, Compare > );

    }

    ////////////////////////////////////////////////////////////////////////////
    // computing 3D transforms:
    // - from bundle real frame to ROIs1 voxel frame
    // - from bundle real frame to ROIs2 real frame
    ////////////////////////////////////////////////////////////////////////////

    gkg::RCPointer< gkg::CompositeTransform3d< float > >
                                transform3dFromROIs1VoxelFrameToBundleRealFrame(
                                       new gkg::CompositeTransform3d< float > );
    
    transform3dFromROIs1VoxelFrameToBundleRealFrame->compose(
                            gkg::Scaling3d< float >( ( float ) resolution.x,
                                                     ( float ) resolution.y, 
                                                     ( float ) resolution.z ) );
    transform3dFromROIs1VoxelFrameToBundleRealFrame->compose(
                                         transform3dFromROIs1RealToBundleReal );

    gkg::RCPointer< gkg::Transform3d< float > >
      transform3dFromBundleRealFrameToROIs1VoxelFrame(
        new gkg::InverseTransform3d< float >( 
                            transform3dFromROIs1VoxelFrameToBundleRealFrame ) );

    gkg::RCPointer< gkg::Transform3d< float > >
      transform3dFromBundleRealFrameToROIs2RealFrame(
        new gkg::InverseTransform3d< float >( 
                                       transform3dFromROIs2RealToBundleReal ) );
 
    ////////////////////////////////////////////////////////////////////////////
    // computing the connectivity matrix
    ////////////////////////////////////////////////////////////////////////////

    std::list< std::string >::const_iterator
      b = fileNameBundleMaps.begin(),
      be = fileNameBundleMaps.end();
    int32_t bundleMapIndex = 0;
    int32_t bundleMapCount = ( int32_t )fileNameBundleMaps.size();
    while ( b != be )
    {

      if ( verbose )
      {

        if ( bundleMapIndex )
        {

          std::cout << Eraser( 30 );

        }
        std::cout << "bundle map [ " << std::setw( 3 ) << bundleMapIndex + 1 
                  << " / " << std::setw( 3 ) << bundleMapCount
                  << " ]" << std::flush;

      }

      gkg::BundleMap< L, std::less< L > > inputBundleMap;
      gkg::Reader::getInstance().read( *b, inputBundleMap );

      typename gkg::BundleMap< L, Compare >::const_iterator
        l = inputBundleMap.begin(),
        le = inputBundleMap.end();
      int32_t bundleIndex = 0;
      int32_t bundleCount = inputBundleMap.getBundleCount();
      while ( l != le )
      {

        const typename gkg::BundleMap< L, Compare >::Bundle& bundle = l->second;

        if ( verbose )
        {

          if ( bundleIndex )
          {

            std::cout << Eraser( 21 );

          }
          std::cout << " bundle [ " << std::setw( 3 ) << bundleIndex + 1 
                    << " / " << std::setw( 3 ) << bundleCount
                    << " ]" << std::flush;

        }

        // allocating a thread context
        gkg::ConnectivityMatrixGauge gauge(
                                          ( int32_t )bundle.getCurve3dCount() );
        gkg::VolumeToMeshLoopContext< L, Compare >
          context( gauge,
                   *this,
                   bundle,
                   rois1,
                   rois1BoundingBox,
                   transform3dFromBundleRealFrameToROIs1VoxelFrame.get(),
                   sceneModeler,
                   rois2BoundingBox,
                   rois2Mask,
                   evolvingMesh,
                   transform3dFromBundleRealFrameToROIs2RealFrame.get(),
                   connectivityMatrix,
                   averageLengthMatrix,
                   roiToMeshBundleMap,
                   verbose );

        // creating a threaded loop
        gkg::ThreadedLoop< int32_t >
          threadedLoop( &context,
                        0,                      // start index
                        ( int32_t )bundle.getCurve3dCount()
                                                // count
                      );

        // launching the threaded loop
        threadedLoop.launch();

        if ( verbose )
        {

          std::cout << gkg::Eraser( 28 )
                    << std::flush;

        }

        ++ l;
        ++ bundleIndex;

      }
      if ( verbose )
      {

        std::cout << gkg::Eraser( 21 )
                  << std::flush;

      }


      ++ b;
      ++ bundleMapIndex;

    }

    if ( verbose )
    {

      std::cout << gkg::Eraser( 30 )
                << std::flush;

    }

    ////////////////////////////////////////////////////////////////////////////
    // normalizing the average length matrix
    ////////////////////////////////////////////////////////////////////////////

    if ( !averageLengthMatrix.isNull() )
    {


      const std::map< size_t, double >&
        connectivityMatrixData = connectivityMatrix.getData();
      std::map< size_t, double >&
        averageLengthMatrixData = averageLengthMatrix->getData();

      std::map< size_t, double >::const_iterator
        cm = connectivityMatrixData.begin(),
        cme = connectivityMatrixData.end();
      std::map< size_t, double >::iterator
        am = averageLengthMatrixData.begin();
      while ( cm != cme )
      {

       am->second /= cm->second;
       ++ cm;
       ++ am;

      }

    }


    ////////////////////////////////////////////////////////////////////////////
    // filling the 4D texture
    ////////////////////////////////////////////////////////////////////////////

    gkg::RCPointer< gkg::TextureMap< float > > roiToRoiConnectivityTextureMap;
    if ( setOfFunctorNames.find( "roi-to-roi-connectivity-texture-map" ) !=
         setOfFunctorNames.end() )
    {

      roiToRoiConnectivityTextureMap.reset( new gkg::TextureMap< float > );
      std::set< int32_t >::const_iterator r1 = rois1Labels.begin(),
                                          r1e = rois1Labels.end();

      int32_t s2 = 0;
      std::vector< float > connectivities( vertexCount );
      for ( ; r1 != r1e; r1++ )
      {

        connectivities.assign( vertexCount, 0.0 );
        for ( s2 = 0; s2 < vertexCount; s2++ )
        {

          if ( connectivityMatrix.hasElement( *r1, s2 ) )
          {
 
            connectivities[ s2 ] = connectivityMatrix( *r1, s2 );

          }

        }
        roiToRoiConnectivityTextureMap->addTextures( connectivities );

      }

      // adding rank information to the header
      std::vector< int32_t > ranks( rois1LabelCount );
      r1 = rois1Labels.begin();
      int32_t rank = 0;
      for ( rank = 0; rank < rois1LabelCount; rank++ )
      {

        ranks[ rank ] = rank; 

      }

      std::vector< int32_t > vertexCounts( rois1LabelCount, vertexCount );      
      roiToRoiConnectivityTextureMap->getHeader().addAttribute(
                                                              "aims_rank_count",
                                                              rois1LabelCount );
      roiToRoiConnectivityTextureMap->getHeader().addAttribute( "aims_rank",
                                                                ranks );
      roiToRoiConnectivityTextureMap->getHeader().addAttribute( 
                                                              "aims_item_count",
                                                              vertexCounts );

    }

    ////////////////////////////////////////////////////////////////////////////
    // saving connectivity matrix
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

     std::cout << "saving connectivity matrix" << std::flush;

    }

    gkg::ItemWriter< gkg::SparseMatrix >& sparseMatrixW =
      gkg::SparseMatrixItemIOFactory::getInstance().getWriter( true, false );
    std::ofstream os( outputFileNameMap[ "connectivity-matrix" ].c_str() );
    sparseMatrixW.write( os, connectivityMatrix );
    os.close();

    if ( verbose )
    {

      std::cout << gkg::Eraser( 26 ) << std::flush;

    }


    ////////////////////////////////////////////////////////////////////////////
    // saving average length matrix
    ////////////////////////////////////////////////////////////////////////////

    if ( setOfFunctorNames.find( "average-length-matrix" ) !=
         setOfFunctorNames.end() )
    {

      if ( verbose )
      {

       std::cout << "saving average length matrix" << std::flush;

      }

      os.open( outputFileNameMap[ "average-length-matrix" ].c_str() );
      sparseMatrixW.write( os, *averageLengthMatrix );
      os.close();

      if ( verbose )
      {

        std::cout << gkg::Eraser( 28 ) << std::flush;

      }

    }


    ////////////////////////////////////////////////////////////////////////////
    // saving ROIs1 to mesh bundle map
    ////////////////////////////////////////////////////////////////////////////

    if ( setOfFunctorNames.find( "roi-to-mesh-bundle-map" ) !=
         setOfFunctorNames.end() )
    {

      if ( verbose )
      {

       std::cout << "saving ROIs1 to mesh bundle map" << std::flush;

      }


      gkg::Writer::getInstance().write(
                                  outputFileNameMap[ "roi-to-mesh-bundle-map" ],
                                  *roiToMeshBundleMap,
                                  ascii,
                                  theOutputBundleMapFormat );

      if ( verbose )
      {

        std::cout << gkg::Eraser( 31 ) << std::flush;

      }

    }


    ////////////////////////////////////////////////////////////////////////////
    // saving ROIs1 to ROIs2 connectivity texture map
    ////////////////////////////////////////////////////////////////////////////

    if ( setOfFunctorNames.find( "roi-to-roi-connectivity-texture-map" ) !=
         setOfFunctorNames.end() )
    {

      if ( verbose )
      {

       std::cout << "saving ROIs1 to ROIs2 connectivity texture map"
                 << std::flush;

      }


      gkg::Writer::getInstance().write(
                     outputFileNameMap[ "roi-to-roi-connectivity-texture-map" ],
                     *roiToRoiConnectivityTextureMap,
                     ascii,
                     theOutputBundleMapFormat );

      if ( verbose )
      {

        std::cout << gkg::Eraser( 46 ) << std::flush;

      }

    }

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void gkg::VolumeToMeshConnectivityMatrixFunctor< L, Compare >::"
             "apply( "
             "const std::list< std::string >& fileNameBundleMaps, "
             "const std::list< std::string >& fileNameListOfROIs1, "
             "const std::string& fileNameROIs1ToBundleMapTransform3d, "
             "const std::list< std::string >& fileNameListOfROIs2, "
             "const std::string& fileNameROIs2ToBundleMapTransform3d, "
             "const std::list< std::string >& functorNames, "
             "const std::list< std::string >& outputFileNames, "
             "const std::string& theOutputBundleMapFormat, "
             "const std::string& theOutputTextureMapFormat, "
             "bool ascii, "
             "bool verbose ) const" );

}


template < class L, class Compare >
inline
std::string 
gkg::VolumeToMeshConnectivityMatrixFunctor< L,
                                            Compare >::getScalarParameterHelp()
{

  try
  {

    std::string help;
    help += ".  <P1>: minimum ROI intersection length in mm (default=1mm)\n";
    help += ".  <P2>: input fiber resampling step in mm (default=0.1)\n";
    help += ".  <P3>: maximum distance to mesh in mm (default=1.0)\n";
    help += ".  <P4>: output fiber resampling step in mm (default=4.0)\n";
    help += ".  <P5>: maximum angle between fiber tangent and mesh normal\n "
            ".        in degrees (default=0->tangent not taken into account)\n";
    help += ".  <P6>: smooth connectivity map (default=0)\n";
    help += ".  <P7>: standard deviation of smoothing (default=1.0)\n";
    help += ".  <P8>: cache X size (default=100)\n";
    help += ".  <P9>: cache Y size (default=100)\n";
    help += ".  <P10>: cache Z size (default=100)\n\n";

    return help;

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "std::string "
             "gkg::VolumeToMeshConnectivityMatrixFunctor< L, "
             "Compare >::getScalarParameterHelp()" );

}


template < class L, class Compare >
inline
std::string 
gkg::VolumeToMeshConnectivityMatrixFunctor< L,
                                            Compare >::getStringParameterHelp()
{

  try
  {

    std::string help;
    help += ".  N/A\n\n";

    return help;

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "std::string "
             "gkg::VolumeToMeshConnectivityMatrixFunctor< L, "
             "Compare >::getStringParameterHelp()" );

}


template < class L, class Compare >
inline
std::string 
gkg::VolumeToMeshConnectivityMatrixFunctor< L,
                                            Compare >::getFunctorHelp()
{

  try
  {

    std::string help;
    help += ".  connectivity-matrix\n";
    help += ".  average-length-matrix\n";
    help += ".  roi-to-mesh-bundle-map\n";
    help += ".  roi-to-roi-connectivity-texture-map\n\n";

    return help;

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "std::string "
             "gkg::VolumeToMeshConnectivityMatrixFunctor< L, "
             "Compare >::getFunctorHelp()" );

}


#undef LABEL_FOR_MESH
#undef LABEL_FOR_BACKGROUND


#endif
