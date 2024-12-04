#ifndef _gkg_dmri_connectivity_matrix_VolumeConnectivityMatrixFunctor_i_h_
#define _gkg_dmri_connectivity_matrix_VolumeConnectivityMatrixFunctor_i_h_

#include <gkg-dmri-connectivity-matrix/VolumeConnectivityMatrixFunctor.h>
#include <gkg-dmri-connectivity-matrix/ConnectivityMatrixFunctor_i.h>
#include <gkg-dmri-connectivity-matrix/ConnectivityMatrixFactory_i.h>
#include <gkg-dmri-connectivity-matrix/ConnectivityMatrixGauge.h>
#include <gkg-dmri-container/BundleMap_i.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-container/MeshMap_i.h>
#include <gkg-processing-transform/IdentityTransform3d.h>
#include <gkg-processing-transform/AffineTransform3d.h>
#include <gkg-processing-transform/RigidTransform3d.h>
#include <gkg-processing-transform/Scaling3d.h>
#include <gkg-processing-transform/CompositeTransform3d.h>
#include <gkg-processing-transform/InverseTransform3d.h>
#include <gkg-processing-mesh/StaticVertexEvolutionFunction.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-core-io/StringConverter.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-processing-io/TypedVolumeReaderProcess.h>
#include <gkg-processing-io/SparseMatrixItemIOFactory.h>
#include <gkg-communication-thread/ThreadedLoop.h>
#include <gkg-core-exception/Exception.h>
#include <fstream>
#include <vector>
#include <iomanip>

#define LABEL_FOR_MESH        -1
#define LABEL_FOR_BACKGROUND   0


//
// class VolumeLoopContext< L, Compare >
//

template < class L, class Compare >
inline
gkg::VolumeLoopContext< L, Compare >::VolumeLoopContext(
     gkg::ConnectivityMatrixGauge& gauge,
     const gkg::VolumeConnectivityMatrixFunctor< L, Compare >& 
                                                volumeConnectivityMatrixFunctor,
     const typename gkg::BundleMap< L, Compare >::Bundle& bundle,
     const gkg::Volume< int32_t >& rois1,
     const gkg::BoundingBox< int32_t >& rois1BoundingBox,
     gkg::Transform3d< float >* transform3dFromBundleRealFrameToROIs1VoxelFrame,
     const gkg::RCPointer< gkg::SceneModeler >& sceneModeler,
     const gkg::BoundingBox< float >& meshBoundingBox,
     const gkg::Volume< uint8_t >& meshMask,
     gkg::Transform3d< float >* transform3dFromBundleRealFrameToMeshRealFrame,
     gkg::SparseMatrix& connectivityMatrix,
     gkg::RCPointer< gkg::SparseMatrix >& averageLengthMatrix,
     gkg::RCPointer< gkg::BundleMap< L, Compare > >& roiToRoiBundleMap,
     const bool& verbose )
                         : gkg::LoopContext< int32_t >( &gauge ),
                           _volumeConnectivityMatrixFunctor(
                                              volumeConnectivityMatrixFunctor ),
                           _bundle( bundle ),
                           _rois1( rois1 ),
                           _rois1BoundingBox( rois1BoundingBox ),
                           _transform3dFromBundleRealFrameToROIs1VoxelFrame(
                              transform3dFromBundleRealFrameToROIs1VoxelFrame ),
                           _sceneModeler( sceneModeler ),
                           _meshBoundingBox( meshBoundingBox ),
                           _meshMask( meshMask ),
                           _transform3dFromBundleRealFrameToMeshRealFrame(
                               transform3dFromBundleRealFrameToMeshRealFrame ),
                           _connectivityMatrix( connectivityMatrix ),
                           _averageLengthMatrix( averageLengthMatrix ),
                           _roiToRoiBundleMap( roiToRoiBundleMap ),
                           _offset( volumeConnectivityMatrixFunctor._offset ),
                           _verbose( verbose ),
                           _fiberIndexAlreadyDisplayed( false )
{
}


template < class L, class Compare >
inline
gkg::VolumeLoopContext< L, Compare >::~VolumeLoopContext()
{
}


template < class L, class Compare >
inline
void gkg::VolumeLoopContext< L, Compare >::doIt( int32_t startIndex,
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
      // allocating the label vector
      //////////////////////////////////////////////////////////////////////////

      std::vector< int32_t > labels( resampledPointCount,
                                     LABEL_FOR_BACKGROUND );

      //////////////////////////////////////////////////////////////////////////
      // looping over points of the fiber and determining their label
      //////////////////////////////////////////////////////////////////////////

      findLabels( resampledFiber,
                  labels );     

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
      std::map< L, std::pair< int32_t, int32_t > > correspondingRois;

      splitFiberAndSortFiberPieces( resampledFiber,
                                    labels,
                                    ranges,
                                    labelledFibers,
                                    correspondingRois );

      //////////////////////////////////////////////////////////////////////////
      // filling :
      // - the connectivity matrix
      // - the average length matrix
      // - the ROI to ROI bundle map
      //////////////////////////////////////////////////////////////////////////

      fillOutputs( labelledFibers, correspondingRois );

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
             "void gkg::VolumeLoopContext< L, Compare >::doIt( "
             "int32_t startIndex, int32_t count )" );

}


template < class L, class Compare >
inline
int32_t 
gkg::VolumeLoopContext< L, Compare >::resampleFiber(
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
           _volumeConnectivityMatrixFunctor._inputFiberResamplingStep + 0.5 ) );

    resampledFiber = fiber.getEquidistantCurve( resampledPointCount );

    return resampledPointCount;

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "int32_t  "
             "gkg::VolumehLoopContext< L, Compare >::"
             "resampleFiber( "
             "const typename gkg::BundleMap< L, Compare >::Fiber& fiber, "
             "const typename gkg::BundleMap< L, Compare >::Fiber& "
             "resampledFiber ) const" );

}


template < class L, class Compare >
inline
void 
gkg::VolumeLoopContext< L, Compare >::findLabels(
             const typename gkg::BundleMap< L, Compare >::Fiber& resampledFiber,
             std::vector< int32_t >& labels ) const
{

  try
  {

    gkg::Vector3d< int32_t > voxelInRois1VoxelFrame;
    gkg::Vector3d< int32_t > voxelInMeshVoxelFrame;

    gkg::Vector3d< float > pointInMeshRealFrame;

    gkg::LightCurve3d< float >::const_iterator p = resampledFiber.begin(),
                                               pe = resampledFiber.end();
    std::vector< int32_t >::iterator l = labels.begin();

    if ( !_sceneModeler.isNull() )
    {

      while ( p != pe )
      {

        // processing the voxel coordinates in the ROIs1 voxel frame
        _transform3dFromBundleRealFrameToROIs1VoxelFrame->getIntegerDirect(
                                                   *p, voxelInRois1VoxelFrame );

        // processing the real coordinates in the Mesh real frame
        _transform3dFromBundleRealFrameToMeshRealFrame->getDirect(
                                                     *p, pointInMeshRealFrame );

        // processing the voxel coordinates in the Mesh voxel frame
        _sceneModeler->getCacheVoxel( pointInMeshRealFrame,  
                                      voxelInMeshVoxelFrame );


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
          if ( _meshBoundingBox.contains( pointInMeshRealFrame ) )
          {

            // if we are close to the ROIs2 mesh
            if ( _meshMask( voxelInMeshVoxelFrame ) )
            {

              *l = LABEL_FOR_MESH;

            }

          }

        }

        ++ p;
        ++ l;

      }

    }
    else
    {

      while ( p != pe )
      {

        // processing the voxel coordinates in the ROIs1 voxel frame
        _transform3dFromBundleRealFrameToROIs1VoxelFrame->getIntegerDirect(
                                                   *p, voxelInRois1VoxelFrame );



        // first, we check if the voxel belongs to a ROI, and if so,
        // collecting its label
        if ( _rois1BoundingBox.contains( voxelInRois1VoxelFrame ) )
        {

          *l = _rois1( voxelInRois1VoxelFrame );

        }

        ++ p;
        ++ l;

      }

    }

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void  "
             "gkg::VolumeLoopContext< L, Compare >::"
             "findLabels( "
             "const typename gkg::BundleMap< L, Compare >::Fiber& "
             "resampledFiber, "
             "std::vector< int32_t >& labels ) "
             "const" );

}


template < class L, class Compare >
inline
void 
gkg::VolumeLoopContext< L, Compare >::getRangesAndRemovingShortOnes(
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
               _volumeConnectivityMatrixFunctor._minimumRoiIntersectionLength )
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
             "gkg::VolumeLoopContext< L, Compare >::"
             "getRangesAndRemovingShortOnes( "
             "const typename gkg::BundleMap< L, Compare >::Fiber& "
             "resampledFiber, "
             "std::vector< int32_t >& labels, "
             "std::vector< std::pair< int32_t, int32_t > >& ranges ) const" );

}


template < class L, class Compare >
inline
int16_t gkg::VolumeLoopContext< L, Compare >::getRange(
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
             "gkg::VolumeLoopContext< L, Compare >::"
             "getRange( "
             "const std::vector< int32_t >& labels, "
             "int32_t index, "
             "std::pair< int32_t, int32_t >& range ) const" );

}


template < class L, class Compare >
inline
void gkg::VolumeLoopContext< L, Compare >::splitFiberAndSortFiberPieces(
   const typename gkg::BundleMap< L, Compare >::Fiber& resampledFiber,
   const std::vector< int32_t >& labels,
   const std::vector< std::pair< int32_t, int32_t > >& ranges,
   std::map< L, std::list< gkg::LightCurve3d< float > > >& labelledFibers,
   std::map< L, std::pair< int32_t, int32_t > >& correspondingRois ) const
{

  try
  {

    int32_t resampledPointCount = resampledFiber.getPointCount();
    int32_t rangeCount = ( int32_t )ranges.size();
    int32_t r1 = -1;
    int32_t r2 = 0;
    int32_t index1 = 0;
    int32_t index2 = 0;
    int32_t label1 = LABEL_FOR_BACKGROUND;
    int32_t label2 = LABEL_FOR_BACKGROUND;

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

      if ( ( label1 > LABEL_FOR_BACKGROUND ) &&
           ( label2 > LABEL_FOR_BACKGROUND ) && ( label1 != label2 ) )
      {

        L newLabel = getLabel( label1, label2 );
        std::pair< int32_t, int32_t > pairOfLabels;
        labelledFibers[ newLabel ].push_back(
          resampledFiber.getEquidistantCurvePart(
                index1,
                index2,
                _volumeConnectivityMatrixFunctor._outputFiberResamplingStep ) );

        pairOfLabels.first = label1;
        pairOfLabels.second = label2;     
        correspondingRois[ newLabel ] = pairOfLabels;

      }

    }

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void "
             "gkg::VolumeLoopContext< L, Compare >::"
             "splitFiberAndSortFiberPieces( "
             "const typename gkg::BundleMap< L, Compare >::Fiber& "
             "resampledFiber, "
             "const std::vector< int32_t >& labels, "
             "const std::vector< std::pair< int32_t, int32_t > >& ranges, "
             "std::map< L, std::list< gkg::LightCurve3d< float > > >& "
             "labelledFibers, "
             "std::map< L, std::pair< int32_t, int32_t > >& "
             "correspondingRois ) const" );

}


template < class L, class Compare >
inline
void gkg::VolumeLoopContext< L, Compare >::fillOutputs(
  const std::map< L, std::list< gkg::LightCurve3d< float > > >& labelledFibers,
  const std::map< L, std::pair< int32_t, int32_t > >& correspondingRois )
{

  try
  {

    typename std::map< L, 
                       std::list< gkg::LightCurve3d< float > > >::const_iterator
      b = labelledFibers.begin(),
      be = labelledFibers.end();

    float fiberLength = 0.0;

    while( b != be )
    {


      typename std::map< L, std::pair< int32_t, int32_t > >::const_iterator
        c = correspondingRois.find( b->first );

      const int32_t& label1 = c->second.first;
      const int32_t& label2 = c->second.second;

      std::list< gkg::LightCurve3d< float > >::const_iterator
        f = b->second.begin(),
        fe = b->second.end();
      while( f != fe )
      {

        fiberLength =  f->getLength();   
        if( !_connectivityMatrix.hasElement( label1, label2 ) )
        {

          lock();
          _connectivityMatrix( label1, label2 ) = 1.0;
          unlock();

        }
        else
        {

          lock();
          _connectivityMatrix( label1, label2 ) += 1.0;
          unlock();

        }
        if( !_connectivityMatrix.hasElement( label2, label1 ) )
        {

          lock();
          _connectivityMatrix( label2, label1 ) = 1.0;
          unlock();

        }
        else
        {

          lock();
          _connectivityMatrix( label2, label1 ) += 1.0;
          unlock();

        }
        
        if ( !_averageLengthMatrix.isNull() )
        {

          if( !_averageLengthMatrix->hasElement( label1, label2 ) )
          {

            lock();
            ( *_averageLengthMatrix )( label1, label2 ) = fiberLength;
            unlock();

          }
          else
          {

            lock();
            ( *_averageLengthMatrix )( label1, label2 ) += fiberLength;
            unlock();

          }
          if( !_averageLengthMatrix->hasElement( label2, label1 ) )
          {

            lock();
            ( *_averageLengthMatrix )( label2, label1 ) = fiberLength;
            unlock();

          }
          else
          {

            lock();
            ( *_averageLengthMatrix )( label2, label1 ) += fiberLength;
            unlock();

          }

        }
     
        ++ f;
      
      }

      if ( !_roiToRoiBundleMap.isNull() )
      {

       lock();
        _roiToRoiBundleMap->addCurve3ds( b->first, b->second );
       unlock();

      }
    
      ++ b;
    
    }

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void gkg::VolumeLoopContext< L, Compare >::"
             "fillOutputs( "
             "const std::map< L, std::list< gkg::LightCurve3d< float > > >&  "
             "labelledFibers"
             "const std::map< L, std::pair< int32_t, int32_t > >& "
             "correspondingRois ) const" );

}


template < class L, class Compare >
inline
L gkg::VolumeLoopContext< L, Compare >::getLabel(  int32_t label1,
                                                   int32_t label2 ) const
{

  try
  {

    throw std::runtime_error( "should not enter this code" );

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "L gkg::VolumeLoopContext< L, Compare >::"
             "getLabel( "
             "int32_t label1, "
             "int32_t label2 ) const" );

}


namespace gkg
{


template <>
inline
int16_t 
gkg::VolumeLoopContext< int16_t, std::less< int16_t > >::getLabel(
                                                          int32_t label1,
                                                          int32_t label2 ) const
{

  try
  {

    int16_t label = 0;

    if( label1 <= label2 )
    {

      label = ( int16_t )label1 + _offset * ( int16_t )label2;
      
    }
    else
    {
    
      label = ( int16_t )label2 + _offset * ( int16_t )label1;
    
    }

    return label;

  }
  GKG_CATCH( "inline "
             "int16_t  "
             "gkg::VolumeLoopContext< int16_t, "
             "std::less< int16_t > >::getLabel( "
             "int16_t label1, "
             "int16_t label2 ) const" );

}


template <>
inline
std::string 
gkg::VolumeLoopContext< std::string, std::less< std::string > >::getLabel(
                                                          int32_t label1,
                                                          int32_t label2 ) const
{

  try
  {

    std::string label;

    if( label1 <= label2 )
    {
    
      label = gkg::StringConverter::toString( label1 ) + '_' +
              gkg::StringConverter::toString( label2 );

    }
    else
    {
    
      label = gkg::StringConverter::toString( label2 ) + '_' +
              gkg::StringConverter::toString( label1 );
        
    }
    return label;

  }
  GKG_CATCH( "inline "
             "std::string  "
             "gkg::VolumeLoopContext< std::string, "
             "std::less< std::string > >::getLabel( "
             "int32_t label1, "
             "int32_t label2 ) const" );

}


}


//
// class VolumeConnectivityMatrixFunctor< L, Compare >
//

template < class L, class Compare >
inline
gkg::VolumeConnectivityMatrixFunctor< L, Compare >::
                                                VolumeConnectivityMatrixFunctor(
                        const std::vector< double >& scalarParameters,
                        const std::vector< std::string >& stringParameters )
                                : gkg::ConnectivityMatrixFunctor< L, Compare >()
{

  try
  {

    if ( scalarParameters.empty() )
    {

      _minimumRoiIntersectionLength = 1;
      _inputFiberResamplingStep = 0.1;
      _outputFiberResamplingStep = 5;
      _takeIntoAccountInterface = false;
      _offset = 0;
      _maximumDistanceToMesh = 1.0;
      _cacheSize.x = 100;
      _cacheSize.y = 100;
      _cacheSize.z = 100;

    }
    else
    {

      if ( scalarParameters.size() != 9U )
      {

        throw std::runtime_error( "9 scalar parameters required" );

      }

      _minimumRoiIntersectionLength = ( float )scalarParameters[ 0 ];
      _inputFiberResamplingStep = ( float )scalarParameters[ 1 ];
      _outputFiberResamplingStep = ( float )scalarParameters[ 2 ];
      _takeIntoAccountInterface =
                      ( int32_t )( scalarParameters[ 3 ] + 0.5 ) ? true : false;
      _offset = ( int16_t )( scalarParameters[ 4 ] + 0.5 );
      _maximumDistanceToMesh = ( float ) scalarParameters[ 5 ];
      _cacheSize.x = ( int32_t )( scalarParameters[ 6 ] + 0.5 );
      _cacheSize.y = ( int32_t )( scalarParameters[ 7 ] + 0.5 );
      _cacheSize.z = ( int32_t )( scalarParameters[ 8 ] + 0.5 );


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

      if ( _takeIntoAccountInterface )
      {

        if ( stringParameters.empty() || ( stringParameters.size() > 2U ) )
        {

          throw std::runtime_error( "one or two string parameter(s) required" );


        }

        _fileNameInterface = stringParameters[ 0 ];

        if ( stringParameters.size() == 2U )
        {

          _fileNameInterfaceToBundleMapTransform3d = stringParameters[ 1 ];

        }

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
gkg::VolumeConnectivityMatrixFunctor< L, Compare >::
                                      ~VolumeConnectivityMatrixFunctor()
{
}


template < class L, class Compare >
inline
void gkg::VolumeConnectivityMatrixFunctor< L, Compare >::apply(
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
    if ( !fileNameListOfROIs2.empty() )
    {

      throw std::runtime_error( "no ROI2 is required" );

    }
    if ( fileNameROIs2ToBundleMapTransform3d.size() != 0U )
    {

      throw std::runtime_error( "no ROI2 is required and no transform for it" );

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

        std::cout << gkg::Eraser( 42 )
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
    int32_t maximumLabel = *rois1Labels.rbegin();
    if ( verbose )
    {

      std::cout << gkg::Eraser( 25 )
                << std::flush;

    }

    ////////////////////////////////////////////////////////////////////////////
    // adding further interface mesh
    ////////////////////////////////////////////////////////////////////////////

    gkg::RCPointer< gkg::SceneModeler > sceneModeler;
    gkg::BoundingBox< float > meshBoundingBox; 
    gkg::Volume< uint8_t > meshMask;
    gkg::RCPointer< gkg::EvolvingMesh > evolvingMesh;
    gkg::RCPointer< gkg::Transform3d< float > >
                                  transform3dFromBundleRealFrameToMeshRealFrame;
    gkg::RCPointer< gkg::Transform3d< float > >
                                            transform3dFromMeshRealToBundleReal;

    if ( _takeIntoAccountInterface )
    {

      //////////////////////////////////////////////////////////////////////////
      // reading input mesh for mesh
      //////////////////////////////////////////////////////////////////////////

      if ( verbose )
      {

        std::cout << "reading mesh '" << _fileNameInterface << "'"
                  << std::flush;

      }

      gkg::RCPointer< gkg::MeshMap< int32_t, float, 3U > > mesh(
                                       new gkg::MeshMap< int32_t, float, 3U > );
      gkg::Reader::getInstance().read( _fileNameInterface, *mesh );
      if ( mesh->vertices.getRankCount() != 1 )
      {

        throw std::runtime_error( "mesh ROI2 must have a unique rank" );

      }

      if ( verbose )
      {

        std::cout << gkg::Eraser( 15 + ( int32_t )_fileNameInterface.size() )
                  << std::flush;

      }

      //////////////////////////////////////////////////////////////////////////
      // reading 3D transform for mesh
      //////////////////////////////////////////////////////////////////////////


      if ( _fileNameInterfaceToBundleMapTransform3d.empty() )
      {

        if ( verbose )
        {

         std::cout << "initializing 3D Mesh->Bundles transform to identity"
                   << std::flush;

        }
        // by default, initializaing to inverse scaling
        transform3dFromMeshRealToBundleReal.reset(
                                        new gkg::IdentityTransform3d< float > );
        if ( verbose )
        {

          std::cout << gkg::Eraser( 51 )
                    << std::flush;

        }

      }
      else
      {

        if ( verbose )
        {

         std::cout << "reading rigid or affine 3D mesh->Bundles transform"
                   << std::flush;

        }


        // Read transformation file, first try rigid, then affine
        try
        {

          gkg::RCPointer< gkg::RigidTransform3d< float > > rigidTransform3d;
          rigidTransform3d.reset( new gkg::RigidTransform3d< float > );

          std::ifstream is( _fileNameInterfaceToBundleMapTransform3d.c_str() );
          rigidTransform3d->readTrm( is );
          is.close();
          transform3dFromMeshRealToBundleReal = rigidTransform3d;

        }
        catch ( std::exception& )
        {

          gkg::RCPointer< gkg::AffineTransform3d< float > > affineTransform3d;
          affineTransform3d.reset( new gkg::AffineTransform3d< float > );

          std::ifstream is( _fileNameInterfaceToBundleMapTransform3d.c_str() );
          affineTransform3d->readTrm( is );
          is.close();
          transform3dFromMeshRealToBundleReal = affineTransform3d;

        }

        if ( verbose )
        {

          std::cout << gkg::Eraser( 40 )
                    << std::flush;

        }

      }
      
      //////////////////////////////////////////////////////////////////////////
      // computing  3D transform from bundle real frame to mesh real frame
      //////////////////////////////////////////////////////////////////////////
      
      transform3dFromBundleRealFrameToMeshRealFrame.reset(
        new gkg::InverseTransform3d< float >( 
                                        transform3dFromMeshRealToBundleReal ) );

      
      //////////////////////////////////////////////////////////////////////////
      // building the mesh cache for mesh
      //////////////////////////////////////////////////////////////////////////

      if ( verbose )
      {

       std::cout << "computing mesh cache for mesh" << std::flush;

      }

      gkg::BoundingBox< float >
        boundingBox( mesh->vertices.getBoundingBox( 0 ) );
      boundingBox.setLowerX( boundingBox.getLowerX() - _maximumDistanceToMesh );
      boundingBox.setLowerY( boundingBox.getLowerY() - _maximumDistanceToMesh );
      boundingBox.setLowerZ( boundingBox.getLowerZ() - _maximumDistanceToMesh );

      //boundingBox.setUpperX( boundingBox.getUpperX()+_maximumDistanceToMesh );
      //boundingBox.setUpperY( boundingBox.getUpperY()+_maximumDistanceToMesh );
      //boundingBox.setUpperZ( boundingBox.getUpperZ()+_maximumDistanceToMesh );

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
        staticVertexEvolutionFunction( 
                                     new gkg::StaticVertexEvolutionFunction() );
      evolvingMesh.reset( new gkg::EvolvingMesh( sceneModeler,
                                                 mesh,
                                                 staticVertexEvolutionFunction,
                                                 _maximumDistanceToMesh,
                                                 false ) );
      sceneModeler->addEvolvingMesh( "mesh", evolvingMesh );
      sceneModeler->getEvolvingMeshCache().getBinaryMask( meshMask );
      meshBoundingBox = sceneModeler->getBoundingBox();

      //gkg::Writer::getInstance().write( "rois2mask.ima", meshMask );

      if ( verbose )
      {

        std::cout << gkg::Eraser( 30 )
                  << std::flush;

      } 
      
    }


    ////////////////////////////////////////////////////////////////////////////
    // allocating a sparse matrix
    ////////////////////////////////////////////////////////////////////////////

    gkg::SparseMatrix connectivityMatrix( maximumLabel + 1, 
                                          maximumLabel + 1 );


    ////////////////////////////////////////////////////////////////////////////
    // allocating average length sparse matrix
    ////////////////////////////////////////////////////////////////////////////

    gkg::RCPointer< gkg::SparseMatrix > averageLengthMatrix;
    if ( setOfFunctorNames.find( "average-length-matrix" ) !=
         setOfFunctorNames.end() )
    {

      averageLengthMatrix.reset( new gkg::SparseMatrix( maximumLabel + 1, 
                                                        maximumLabel + 1 ) );

    }


    ////////////////////////////////////////////////////////////////////////////
    // allocating the output bundle map
    ////////////////////////////////////////////////////////////////////////////

    gkg::RCPointer< gkg::BundleMap< L, Compare > > roiToRoiBundleMap;
    if ( setOfFunctorNames.find( "roi-to-roi-bundle-map" ) !=
         setOfFunctorNames.end() )
    {

      roiToRoiBundleMap.reset( new gkg::BundleMap< L, Compare > );

    }

    ////////////////////////////////////////////////////////////////////////////
    // computing 3D transforms:
    // - from bundle real frame to ROIs1 voxel frame
    ////////////////////////////////////////////////////////////////////////////

    gkg::RCPointer< gkg::CompositeTransform3d< float > >
                                transform3dFromROIs1VoxelFrameToBundleRealFrame(
                                       new gkg::CompositeTransform3d< float > );

    
    transform3dFromROIs1VoxelFrameToBundleRealFrame->compose(
                            gkg::Scaling3d< float >( ( float ) resolution.x,
                                                     ( float ) resolution.y, 
                                                     ( float ) resolution.z ) );
    transform3dFromROIs1VoxelFrameToBundleRealFrame->compose(
                                        *transform3dFromROIs1RealToBundleReal );

    gkg::RCPointer< gkg::Transform3d< float > >
      transform3dFromBundleRealFrameToROIs1VoxelFrame(
          new gkg::InverseTransform3d< float >(
                            transform3dFromROIs1VoxelFrameToBundleRealFrame ) );

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
        std::cout << "bundle map [ " << std::setw( 6 ) << bundleMapIndex + 1 
                  << " / " << std::setw( 6 ) << bundleMapCount
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
        gkg::VolumeLoopContext< L, Compare >
          context( gauge,
                   *this,
                   bundle,
                   rois1,
                   rois1BoundingBox,
                   transform3dFromBundleRealFrameToROIs1VoxelFrame.get(),
                   sceneModeler,
                   meshBoundingBox,
                   meshMask,
                   transform3dFromBundleRealFrameToMeshRealFrame.get(),
                   connectivityMatrix,
                   averageLengthMatrix,
                   roiToRoiBundleMap,
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
    // saving ROIs1 to ROIs1 bundle map
    ////////////////////////////////////////////////////////////////////////////

    if ( setOfFunctorNames.find( "roi-to-roi-bundle-map" ) !=
         setOfFunctorNames.end() )
    {

      if ( verbose )
      {

       std::cout << "saving ROIs1 to ROIs1 bundle map" << std::flush;

      }


      gkg::Writer::getInstance().write(
                                  outputFileNameMap[ "roi-to-roi-bundle-map" ],
                                  *roiToRoiBundleMap,
                                  ascii,
                                  theOutputBundleMapFormat );

      if ( verbose )
      {

        std::cout << gkg::Eraser( 32 ) << std::flush;

      }

    }

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void gkg::VolumeConnectivityMatrixFunctor< L, Compare >::"
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
gkg::VolumeConnectivityMatrixFunctor< L, Compare >::getScalarParameterHelp()
{

  try
  {

    std::string help;
    help += ".  <P1>: minimum ROI intersection length in mm (default=1mm)\n";
    help += ".  <P2>: input fiber resampling step in mm (default=0.1)\n";
    help += ".  <P3>: output fiber resampling step in mm (default=4.0)\n";
    help += ".  <P4>: take into account a further interface (default=0)\n";
    help += ".  <P5>: offset for output label creation when input bundle map\n";
    help += ".        labels are of type int16_t (default=0)\n";
    help += ".  <P6>: maximum distance to mesh in mm (default=1.0)\n";
    help += ".  <P7>: cache X size (default=100)\n";
    help += ".  <P8>: cache Y size (default=100)\n";
    help += ".  <P9>: cache Z size (default=100)\n\n";

    return help;

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "std::string "
             "gkg::VolumeConnectivityMatrixFunctor< L, "
             "Compare >::getScalarParameterHelp()" );

}


template < class L, class Compare >
inline
std::string 
gkg::VolumeConnectivityMatrixFunctor< L, Compare >::getStringParameterHelp()
{

  try
  {

    std::string help;
    help += ".  <P1>: Further interface file name\n";
    help += ".  <P2>: 3D rigid transform from interface to bundle \n";
    help += ".        map referential (default=identity)\n\n";

    return help;

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "std::string "
             "gkg::VolumeConnectivityMatrixFunctor< L, "
             "Compare >::getStringParameterHelp()" );

}


template < class L, class Compare >
inline
std::string 
gkg::VolumeConnectivityMatrixFunctor< L, Compare >::getFunctorHelp()
{

  try
  {

    std::string help;
    help += ".  connectivity-matrix\n";
    help += ".  average-length-matrix\n";
    help += ".  roi-to-roi-bundle-map\n\n";

    return help;

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "std::string "
             "gkg::VolumeConnectivityMatrixFunctor< L, "
             "Compare >::getFunctorHelp()" );

}


#undef LABEL_FOR_MESH
#undef LABEL_FOR_BACKGROUND



#endif
