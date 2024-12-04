#ifndef _gkg_dmri_bundlemap_operator_BundleMapOperatorBetweenRoiSelection_i_h_
#define _gkg_dmri_bundlemap_operator_BundleMapOperatorBetweenRoiSelection_i_h_


#include <gkg-dmri-bundlemap-operator/BundleMapOperatorBetweenRoiSelection.h>
#include <gkg-dmri-bundlemap-operator/BundleMapOperator_i.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-container/BoundingBox.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-processing-transform/RigidTransform3d.h>
#include <gkg-processing-transform/AffineTransform3d.h>
#include <gkg-processing-transform/IdentityTransform3d.h>
#include <gkg-processing-transform/Scaling3d.h>
#include <gkg-processing-transform/CompositeTransform3d.h>
#include <gkg-dmri-container/BundleMap_i.h>
#include <gkg-core-exception/Exception.h>




template < class L, class Compare >
inline
gkg::BundleMapOperatorBetweenRoiSelection< L, Compare >::
                                           BundleMapOperatorBetweenRoiSelection(
                             const std::vector< std::string >& stringParameters,
                             const std::vector< double >& scalarParameters,
                             bool verbose )
                               : gkg::BundleMapOperator< L, Compare >( verbose )
{

  try
  {

    // sanity checks for string parameters
    if ( ( stringParameters.size() != 1U ) &&
         ( stringParameters.size() != 2U ) )
    {

      throw std::runtime_error(
                               "string parameters must have 1 or 2 arguments" );

    }

    _fileNameRois = stringParameters[ 0 ];
    if ( stringParameters.size() == 2U )
    {

      _fileNameTransform3d = stringParameters[ 1 ];

    }

    // sanity checks for scalar parameters
    if ( gkg::TypeOf< L >::getName() == gkg::TypeOf< std::string >::getName() )
    {

      if ( scalarParameters.size() != 2U &&
           scalarParameters.size() != 3U  )
      {

        throw std::runtime_error( 
                      "scalar parameters must have at least 2 or 3 arguments" );

      }
      _minimumIntersectionLength = ( float )scalarParameters[ 0 ];
      _noInterRoiFibers = ( ( int32_t )( scalarParameters[ 1 ] + 0.5 ) == 1 ?
                            true : false );

    }
    else if ( gkg::TypeOf< L >::getName() ==
              gkg::TypeOf< int16_t >::getName() )
    {

      if ( scalarParameters.size() != 3U )
      {

        throw std::runtime_error( "scalar parameters must have 3 arguments" );

      }
      _minimumIntersectionLength = ( float )scalarParameters[ 0 ];
      _noInterRoiFibers = ( ( int32_t )( scalarParameters[ 1 ] + 0.5 ) == 1 ?
                            true : false );
      _offset = ( int16_t )( scalarParameters[ 2 ] + 0.5 );

    }

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "gkg::BundleMapOperatorBetweenRoiSelection< L, Compare >::"
             "BundleMapOperatorBetweenRoiSelection( "
             "const std::vector< std::string >& stringParameters, "
             "const std::vector< double >& scalarParameters, "
             "bool verbose )" );

}


template < class L, class Compare >
inline
gkg::BundleMapOperatorBetweenRoiSelection< L, Compare >::
                                         ~BundleMapOperatorBetweenRoiSelection()
{
}



template < class L, class Compare >
inline
void gkg::BundleMapOperatorBetweenRoiSelection< L, Compare >::startContribution(
                gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap,
                gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap )
{

  try
  {

    // allocating the ROI volume
    _rois.reset( new gkg::Volume< int16_t > );

    // reading the ROI from file
    gkg::Reader::getInstance().read( _fileNameRois, *_rois );
    gkg::Vector3d< double > resolution;
    _rois->getResolution( resolution );

    // reading or building the 3D transform from ROI to DWI frame
    gkg::RCPointer< gkg::Transform3d< float > >
                                             transform3dFromRoisRealToFiberReal;
    if ( !_fileNameTransform3d.empty() )
    {

     try
      {

        gkg::RCPointer< gkg::RigidTransform3d< float > >
          rigidTransform3d( new gkg::RigidTransform3d< float > );
        std::ifstream is( _fileNameTransform3d.c_str() );
        rigidTransform3d->readTrm( is );
        is.close();
        transform3dFromRoisRealToFiberReal = rigidTransform3d;

      }
      catch( std::exception& )
      {

        try
        {

          gkg::RCPointer< gkg::AffineTransform3d< float > >
            affineTransform3d( new gkg::AffineTransform3d< float > );
          std::ifstream is( _fileNameTransform3d.c_str() );
          affineTransform3d->readTrm( is );
          is.close();
          transform3dFromRoisRealToFiberReal = affineTransform3d;

        }
        catch( std::exception& )
        {

          throw std::runtime_error( "invalid 3D ROIs->fibers transform" );

        }

      }

    }
    else
    {

      transform3dFromRoisRealToFiberReal.reset(
                                        new gkg::IdentityTransform3d< float > );

    }

    // building the 3D transform from ROI voxel coordinates to DWI real 
    // coordinate
    gkg::CompositeTransform3d< float >* transform3dFromRoisVoxelToFiberReal = 
      new gkg::CompositeTransform3d< float >;

    transform3dFromRoisVoxelToFiberReal->compose(
                             gkg::Scaling3d< float >( ( float )resolution.x,
                                                      ( float )resolution.y,
                                                      ( float )resolution.z ) );
    transform3dFromRoisVoxelToFiberReal->compose(
                                          *transform3dFromRoisRealToFiberReal );

    _transform3dFromRoisVoxelToFiberReal.reset( 
                                          transform3dFromRoisVoxelToFiberReal );

    // adding contribution of the input bundle map to the output bundle map
    this->addContribution( inputBundleMap, outputBundleMap );

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void gkg::BundleMapOperatorBetweenRoiSelection< L, Compare >::"
             "startContribution( "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap, "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap "
             ") const" );

}


template < class L, class Compare >
inline
void gkg::BundleMapOperatorBetweenRoiSelection< L, Compare >::addContribution(
                gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap,
                gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap )
{

  try
  {

    // sanity checks
    if ( inputBundleMap.isNull() )
    {

      throw std::runtime_error( "input pointer is empty" );

    }
    if ( outputBundleMap.isNull() )
    {

      throw std::runtime_error( "output pointer is empty" );

    }

    // building the bounding box of the ROI mask
    gkg::BoundingBox< int32_t > boundingBox( *_rois );

    // temporary map of list of fibers
    std::map< L, std::list< gkg::LightCurve3d< float > > > labelledFibers;

    // looping over input bundles
    gkg::Vector3d< int32_t > siteInRoiFrame;
    int16_t label = 0;
    float length = 0.0f;

    typename gkg::BundleMap< L, Compare >::const_iterator
      b = inputBundleMap->begin(),
      be = inputBundleMap->end();
    while ( b != be )
    {

      // looping over fibers of the current bundle
      typename gkg::BundleMap< L, Compare >::Bundle::const_iterator
        f = b->second.begin(),
        fe = b->second.end();
      while ( f != fe )
      {
 
        // collecting point count
        int32_t pointCount = f->getPointCount();

        // initializing label vector
        std::vector< int16_t > labels;
        labels.reserve( pointCount );

        // looping over points of the current fiber and processing the vector of
        // labels
        typename gkg::BundleMap< L, Compare >::Fiber::const_iterator
          p = f->begin(),
          pe = f->end();
        while ( p != pe )
        {

          _transform3dFromRoisVoxelToFiberReal->getIntegerInverse(
                                                               *p,
                                                               siteInRoiFrame );
          if( boundingBox.contains( siteInRoiFrame ) )
          {

            labels.push_back( ( *_rois )( siteInRoiFrame ) );

          }
          else
          {

            labels.push_back( 0 );

          }

          ++ p;

        }

        // removing short range from the vector of labels
        int32_t i = 0;
        int32_t j = 0;
        std::pair< int32_t, int32_t > range;
        std::vector< std::pair< int32_t, int32_t > > ranges;
        for ( i = 0; i < pointCount; )
        {

          label = getRange( labels, i, range );
          length = f->getLength( range.first, range.second );

          // if a range is found
          if ( label != -1 )
          {

            // if the range is too small, just delete it
            if ( length < _minimumIntersectionLength )
            {

              for ( j = range.first; j <= range.second; j++ )
              {

                labels[ j ] = 0;

              }

            }
            else
            {

              ranges.push_back( range );

            }
            i = range.second + 1;

          }
          // else if no range is found
          else
          {

            break;

          }

        }

        // splitting the current fiber into smallest fibers and storing them 
        // into the labelled fiber map
        int32_t rangeCount = ( int32_t )ranges.size();
        int32_t r1 = -1;
        int32_t r2 = 0;
        int32_t index1 = 0;
        int32_t index2 = 0;
        int16_t label1 = 0;
        int16_t label2 = 0;

        for ( ; r1 < rangeCount; r1++, r2++ )
        {

          if ( r1 == -1 )
          {

            index1 = 0;
            label1 = -1;

          }
          else
          {

            index1 = ranges[ r1 ].first;
            label1 = labels[ ranges[ r1 ].first ];

          }
          if ( r2 == rangeCount )
          {

            index2 = pointCount - 1;
            label2 = -1;

          }
          else
          {

            index2 = ranges[ r2 ].second;
            label2 = labels[ ranges[ r2 ].second ];

          }

          if ( !_noInterRoiFibers ||
               ( _noInterRoiFibers &&
                 ( ( label1 == -1 ) || ( label2 == -1 ) ) ) )
          {

            std::list< gkg::Vector3d< float > > points;
            for ( i = index1; i <= index2; i++ )
            {

              points.push_back( f->getPoint( i ) );

            }
            if ( !points.empty() )
            {

              L newLabel = getLabel( label1, label2 );
              labelledFibers[ newLabel ].push_back(
                                         gkg::LightCurve3d< float >( points ) );

            }

          }

        }

        ++ f;

      }
      
      ++ b;

    }

    // filling output bundle map
    typename 
    std::map< L, std::list< gkg::LightCurve3d< float > > >::const_iterator
      l = labelledFibers.begin(),
      le = labelledFibers.end();
    while ( l != le )
    {

      outputBundleMap->addCurve3ds( l->first, l->second );
      ++ l;

    }

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void gkg::BundleMapOperatorBetweenRoiSelection< L, Compare >::"
             "addContribution( "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap, "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap "
             ") const" );

}


template < class L, class Compare >
inline
void gkg::BundleMapOperatorBetweenRoiSelection< L, Compare >::endContribution(
                gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap,
                gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap )
{

  try
  {

    this->addContribution( inputBundleMap, outputBundleMap );

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void gkg::BundleMapOperatorBetweenRoiSelection< L, Compare >::"
             "endContribution( "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap, "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap "
             ") const" );

}


template < class L, class Compare >
inline
std::string 
gkg::BundleMapOperatorBetweenRoiSelection< L, Compare >::getName() const
{

  try
  {

    return 
      gkg::BundleMapOperatorBetweenRoiSelection< L, Compare >::getStaticName();

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "std::string "
             "gkg::BundleMapOperatorBetweenRoiSelection< L, Compare >::"
             "getName() const" );

}


template < class L, class Compare >
inline
std::string 
gkg::BundleMapOperatorBetweenRoiSelection< L, Compare >::getStaticName()
{

  try
  {

    return "between-roi-selection";

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "std::string "
             "gkg::BundleMapOperatorBetweenRoiSelection< L, Compare >::"
             "getStaticName()" );

}


template < class L, class Compare >
inline
std::string 
gkg::BundleMapOperatorBetweenRoiSelection< L, Compare >::
                                                        getStringParameterHelp()
{

  try
  {

    return std::string(
      ".  <P1>: ROI mask file name\n"
      ".  <P2>: ROI to DWI frame transform3d filename (default=id)\n" );

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "std::string "
             "gkg::BundleMapOperatorBetweenRoiSelection< L, Compare >::"
             "getStringParameterHelp()" );

}


template < class L, class Compare >
inline
std::string 
gkg::BundleMapOperatorBetweenRoiSelection< L, Compare >::
                                                        getScalarParameterHelp()
{

  try
  {

    return std::string(
      ".  <P1>: minimum intersection length in mm\n"
      ".  <P2>: do not process inter-ROI bundles if set to 1 (0 instead)\n"
      ".  <P3>: in case of int16_t labels, offset to create a composite label\n"
      ".        from l1 and l2 (l = l1 + offset * l2)\n" );

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "std::string "
             "gkg::BundleMapOperatorBetweenRoiSelection< L, Compare >::"
             "getStringParameterHelp()" );

}


template < class L, class Compare >
inline
int16_t gkg::BundleMapOperatorBetweenRoiSelection< L, Compare >::getRange(
                                    const std::vector< int16_t >& labels,
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
      int16_t label = labels[ startIndex ];
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
    return -1;

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "int16_t gkg::BundleMapOperatorBetweenRoiSelection< L, Compare >::"
             "getRange( "
             "const std::vector< int16_t >& labels, "
             "int32_t index, "
             "std::pair< int32_t, int32_t >& range ) const" );

}


template < class L, class Compare >
inline
L gkg::BundleMapOperatorBetweenRoiSelection< L, Compare >::getLabel(
                                                          int16_t label1,
                                                          int16_t label2 ) const
{

  try
  {

    throw std::runtime_error( "should not enter this code" );

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "L gkg::BundleMapOperatorBetweenRoiSelection< L, Compare >::"
             "getLabel( "
             "int16_t label1, "
             "int16_t label2 ) const" );

}


namespace gkg
{


template <>
inline
std::string 
BundleMapOperatorBetweenRoiSelection< std::string,
                                    std::less< std::string > >::getLabel(
                                                          int16_t label1,
                                                          int16_t label2 ) const
{

  try
  {

    std::string label;
    if ( label1 == -1 )
    {

      label = gkg::StringConverter::toString( label2 );

    }
    else if ( label2 == -1 )
    {

      label = gkg::StringConverter::toString( label1 );

    }
    else
    {

      label = gkg::StringConverter::toString( label1 ) + '_' +
             gkg::StringConverter::toString( label2 );

    }
    return label;

  }
  GKG_CATCH( "inline "
             "std::string  "
             "BundleMapOperatorBetweenRoiSelection< std::string, "
             "std::less< std::string > >::getLabel( "
             "int16_t label1, "
             "int16_t label2 ) const" );

}


template <>
inline
int16_t 
BundleMapOperatorBetweenRoiSelection< int16_t,
                                    std::less< int16_t > >::getLabel(
                                                          int16_t label1,
                                                          int16_t label2 ) const
{

  try
  {

    int16_t label = 0;
    if ( label1 == -1 )
    {

      label = label2;

    }
    else if ( label2 == -1 )
    {

      label = label1;

    }
    else
    {

      label = label1 + _offset * label2;

    }
    return label;

  }
  GKG_CATCH( "inline "
             "int16_t  "
             "BundleMapOperatorBetweenRoiSelection< int16_t, "
             "std::less< int16_t > >::getLabel( "
             "int16_t label1, "
             "int16_t label2 ) const" );

}


}


#endif
