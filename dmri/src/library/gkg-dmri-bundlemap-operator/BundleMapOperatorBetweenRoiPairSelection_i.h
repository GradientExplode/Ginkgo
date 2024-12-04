#ifndef _gkg_dmri_bundlemap_operator_BundleMapOperatorBetweenRoiPairSelection_i_h_
#define _gkg_dmri_bundlemap_operator_BundleMapOperatorBetweenRoiPairSelection_i_h_


#include <gkg-dmri-bundlemap-operator/BundleMapOperatorBetweenRoiPairSelection.h>
#include <gkg-dmri-bundlemap-operator/BundleMapOperator_i.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-container/BoundingBox.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-processing-transform/RigidTransform3d.h>
#include <gkg-processing-transform/IdentityTransform3d.h>
#include <gkg-processing-transform/NonLinearTransform3d.h>
#include <gkg-processing-transform/Scaling3d.h>
#include <gkg-processing-transform/CompositeTransform3d.h>
#include <gkg-processing-io/TypedVolumeReaderProcess.h>
#include <gkg-dmri-container/BundleMap_i.h>
#include <gkg-processing-algobase/TestFunction_i.h>
#include <gkg-processing-algobase/TestFunctionFactory.h>
#include <gkg-processing-colorimetry/RandomColorGenerator.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-core-exception/Exception.h>
#include <fstream>
#include <iomanip>
#include <iostream>


template < class L, class Compare >
inline
gkg::BundleMapOperatorBetweenRoiPairSelection< L, Compare >::
                                       BundleMapOperatorBetweenRoiPairSelection(
                             const std::vector< std::string >& stringParameters,
                             const std::vector< double >& scalarParameters,
                             bool verbose )
                               : gkg::BundleMapOperator< L, Compare >( verbose )
{

  try
  {

    // sanity checks for string parameters
    if ( ( stringParameters.size() != 4U ) &&
         ( stringParameters.size() != 5U ) &&
         ( stringParameters.size() != 7U ) )
    {

      throw std::runtime_error(
                               "string parameters must have 4 or 6 arguments" );

    }

    _fileNameRois = stringParameters[ 0 ];
    _fileNameROILabelToNameLut = stringParameters[ 1 ];
    _fileNameOutputHierarchy = stringParameters[ 2 ];
    _fileNameLabelChanges = stringParameters[ 3 ];


    if ( stringParameters.size() == 5U )
    {

      _fileNameTransform3ds.resize( 1U );
      _fileNameTransform3ds[ 0 ] = stringParameters[ 4 ];

    }
    else if ( stringParameters.size() == 7U )
    {

      _fileNameTransform3ds.resize( 3U );
      _fileNameTransform3ds[ 0 ] = stringParameters[ 4 ];
      _fileNameTransform3ds[ 1 ] = stringParameters[ 5 ];
      _fileNameTransform3ds[ 2 ] = stringParameters[ 6 ];

    }

    // sanity checks for scalar parameters
    if ( scalarParameters.size() != 2U  )
    {

      throw std::runtime_error( "scalar parameters must have 2 arguments" );

    }

    _distanceThresholdFromExtremity = scalarParameters[ 0 ];
    _inputFiberResamplingStep = scalarParameters[ 1 ];

    _roiCount = 0;

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "gkg::BundleMapOperatorBetweenRoiPairSelection< L, Compare >::"
             "BundleMapOperatorBetweenRoiPairSelection( "
             "const std::vector< std::string >& stringParameters, "
             "const std::vector< double >& scalarParameters, "
             "bool verbose )" );

}


template < class L, class Compare >
inline
gkg::BundleMapOperatorBetweenRoiPairSelection< L, Compare >::
                                     ~BundleMapOperatorBetweenRoiPairSelection()
{
}



template < class L, class Compare >
inline
void 
gkg::BundleMapOperatorBetweenRoiPairSelection< L, Compare >::startContribution(
                gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap,
                gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap )
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // allocating the ROI volume
    ////////////////////////////////////////////////////////////////////////////

    _rois.reset( new gkg::Volume< int32_t > );


    ////////////////////////////////////////////////////////////////////////////
    // reading the ROI from file
    ////////////////////////////////////////////////////////////////////////////

    gkg::TypedVolumeReaderProcess< int32_t > typedVolumeReaderProcess( *_rois );
    typedVolumeReaderProcess.execute( _fileNameRois );

    gkg::Vector3d< double > resolution;
    _rois->getResolution( resolution );

    
    ////////////////////////////////////////////////////////////////////////////
    // computing the set of ROI indices
    ////////////////////////////////////////////////////////////////////////////

    std::set< int32_t > roiLabelSet;

    gkg::Volume< int32_t >::const_iterator
      r = _rois->begin(),
      re = _rois->end();
    while ( r != re )
    {

      if ( *r )
      {

        roiLabelSet.insert( *r );

      }
      ++ r;

    }

    _roiCount = ( int32_t )roiLabelSet.size();
    _roiLabels = std::vector< int32_t >( roiLabelSet.begin(),
                                         roiLabelSet.end() );


    ////////////////////////////////////////////////////////////////////////////
    // computing the set of ROI indices
    ////////////////////////////////////////////////////////////////////////////

    std::ifstream is( _fileNameROILabelToNameLut.c_str() );

    int32_t roiLabel = 0;
    std::string name;
    while ( !is.eof() )
    {

      is >> roiLabel >> name;
      _roiLabelToNameLut[ roiLabel ] = name;


    }

    is.close();


    ////////////////////////////////////////////////////////////////////////////
    // reading or building the 3D transform from ROI to DWI frame
    ////////////////////////////////////////////////////////////////////////////

    gkg::RCPointer< gkg::Transform3d< float > >
                                             transform3dFromRoisRealToFiberReal;


    if ( _fileNameTransform3ds.empty() )
    {

      transform3dFromRoisRealToFiberReal.reset(
                                        new gkg::IdentityTransform3d< float > );

    }
    else
    {

      gkg::NonLinearTransform3d< float >*
         nonLinearTransform3d = new gkg::NonLinearTransform3d< float >;

      if ( _fileNameTransform3ds.size() == 1U )
      {

        nonLinearTransform3d->readTrm( _fileNameTransform3ds[ 0 ],
                                       "",
                                       "" );
      }
      else if ( _fileNameTransform3ds.size() == 3U )
      {

        nonLinearTransform3d->readTrm( _fileNameTransform3ds[ 0 ],
                                       _fileNameTransform3ds[ 1 ],
                                       _fileNameTransform3ds[ 2 ] );

      }
      transform3dFromRoisRealToFiberReal.reset( nonLinearTransform3d );


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
             "void "
             "gkg::BundleMapOperatorBetweenRoiPairSelection< L, Compare >::"
             "startContribution( "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap, "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap "
             ") const" );

}


template < class L, class Compare >
inline
void 
gkg::BundleMapOperatorBetweenRoiPairSelection< L, Compare >::addContribution(
                gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap,
                gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap )
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // sanity checks
    ////////////////////////////////////////////////////////////////////////////

    if ( inputBundleMap.isNull() )
    {

      throw std::runtime_error( "input pointer is empty" );

    }
    if ( outputBundleMap.isNull() )
    {

      throw std::runtime_error( "output pointer is empty" );

    }


    ////////////////////////////////////////////////////////////////////////////
    // building the bounding box of the ROI mask
    ////////////////////////////////////////////////////////////////////////////

    gkg::BoundingBox< int32_t > boundingBox( *_rois );



    ////////////////////////////////////////////////////////////////////////////
    // looping over bundles
    ////////////////////////////////////////////////////////////////////////////

    typename gkg::BundleMap< L, Compare >::Fiber resampledFiber;
    int32_t resampledPointCount = 0;
    int32_t toExtremityPointCount = 0;
    int32_t forwardPointCount = 0;
    int32_t backwardPointCount = 0;
    gkg::Vector3d< int32_t > siteInRoiFrame;
    int32_t roiLabel = 0;
    L newLabel;
    typename gkg::BundleMap< L, Compare >::const_iterator
      b = inputBundleMap->begin(),
      be = inputBundleMap->end();
    while ( b != be )
    {

      std::map< int32_t, int32_t > forwardLabelHistogram;
      std::map< int32_t, int32_t > backwardLabelHistogram;

      // looping over fibers of the current bundle
      typename gkg::BundleMap< L, Compare >::Bundle::const_iterator
        f = b->second.begin(),
        fe = b->second.end();
      while ( f != fe )
      {

        // resampling the fiber at the input resolution
        resampledPointCount = std::max( 2, ( int32_t )( f->getLength() /
                                            _inputFiberResamplingStep + 0.5 ) );
        toExtremityPointCount =
                     std::max( 1,
                               ( int32_t )( _distanceThresholdFromExtremity /
                                            _inputFiberResamplingStep + 0.5 ) );

        resampledFiber = f->getEquidistantCurve( resampledPointCount );

        // forward direction
        forwardPointCount = 0;
        typename gkg::BundleMap< L, Compare >::Fiber::const_iterator
          fp = resampledFiber.begin(),
          fpe = resampledFiber.end();
        while ( ( fp != fpe ) &&
                ( forwardPointCount <= toExtremityPointCount ) )
        {

          _transform3dFromRoisVoxelToFiberReal->getIntegerInverse(
                                                               *fp,
                                                               siteInRoiFrame );
          if( boundingBox.contains( siteInRoiFrame ) )
          {

            roiLabel = ( *_rois )( siteInRoiFrame );

            if ( roiLabel )
            {

              forwardLabelHistogram[ roiLabel ] += 1;

            }

          }
          ++ forwardPointCount;
          ++ fp;

        }

        // backward direction
        backwardPointCount = 0;
        typename gkg::BundleMap< L, Compare >::Fiber::const_reverse_iterator
          bp = resampledFiber.rbegin(),
          bpe = resampledFiber.rend();
        while ( ( bp != bpe ) &&
                ( backwardPointCount <= toExtremityPointCount ) )
        {

          _transform3dFromRoisVoxelToFiberReal->getIntegerInverse(
                                                               *bp,
                                                               siteInRoiFrame );
          if( boundingBox.contains( siteInRoiFrame ) )
          {

            roiLabel = ( *_rois )( siteInRoiFrame );
            if ( roiLabel )
            {

              backwardLabelHistogram[ roiLabel ] += 1;

            }

          }
          ++ backwardPointCount;
          ++ bp;

        }

        ++ f;

      }

      // computing the forward likeliest label
      int32_t likeliestForwardCount = 0;
      int32_t likeliestForwardLabel = 0;
      std::map< int32_t, int32_t >::const_iterator
        fh = forwardLabelHistogram.begin(),
        fhe = forwardLabelHistogram.end();
      while ( fh != fhe )
      {

        if ( fh->second > likeliestForwardCount )
        {

          likeliestForwardCount = fh->second;
          likeliestForwardLabel = fh->first;

        }
        ++ fh;

      }


      // computing the backward likeliest label
      int32_t likeliestBackwardCount = 0;
      int32_t likeliestBackwardLabel = 0;
      std::map< int32_t, int32_t >::const_iterator
        bh = backwardLabelHistogram.begin(),
        bhe = backwardLabelHistogram.end();
      while ( bh != bhe )
      {

        if ( bh->second > likeliestBackwardCount )
        {

          likeliestBackwardCount = bh->second;
          likeliestBackwardLabel = bh->first;

        }
        ++ bh;

      }


      if ( likeliestForwardLabel && likeliestBackwardLabel )
      {


        std::pair< int32_t, int32_t > 
          roiForwardLabelAndBackwardLabel( std::min( likeliestForwardLabel,
                                                     likeliestBackwardLabel ),
                                           std::max( likeliestForwardLabel,
                                                     likeliestBackwardLabel ) );
        std::map< std::pair< int32_t, int32_t >, int32_t >::iterator
          flblCount = _roiForwardLabelToRoiBackwardLabelClusterCount.find(
                                              roiForwardLabelAndBackwardLabel );
        int32_t roiFowardLabelToRoiBackwardLabelClusterCount = 1;
        if ( flblCount == _roiForwardLabelToRoiBackwardLabelClusterCount.end() )
        {

          _roiForwardLabelToRoiBackwardLabelClusterCount[
                                          roiForwardLabelAndBackwardLabel ] = 1;

        }
        else
        {

          ++ ( flblCount->second );
          roiFowardLabelToRoiBackwardLabelClusterCount = flblCount->second;

        }

        newLabel = this->getLabel( std::min( likeliestForwardLabel,
                                           likeliestBackwardLabel ),
                                 std::max( likeliestForwardLabel,
                                           likeliestBackwardLabel ),
                                 roiFowardLabelToRoiBackwardLabelClusterCount );

        _labelChanges[ b->first ] = newLabel;
        outputBundleMap->addCurve3ds( newLabel, b->second.getCurve3ds() );

      }
      else
      {

        std::cout << "problem: one of the label is nul "
                  << likeliestForwardLabel << " "
                  << likeliestBackwardLabel << " "
                  << std::endl;

      }

      ++ b;

    }


    ////////////////////////////////////////////////////////////////////////////
    // creating the output hierarchy over ROI pairs
    ////////////////////////////////////////////////////////////////////////////

    this->createHierarchy( *outputBundleMap );
    this->saveLabelChanges();

    if ( this->_verbose )
    {

      std::cout << gkg::Eraser( 30 );

    }

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void "
             "gkg::BundleMapOperatorBetweenRoiPairSelection< L, Compare >::"
             "addContribution( "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap, "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap "
             ") const" );

}


template < class L, class Compare >
inline
void 
gkg::BundleMapOperatorBetweenRoiPairSelection< L, Compare >::endContribution(
                gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap,
                gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap )
{

  try
  {

    this->addContribution( inputBundleMap, outputBundleMap );

    ////////////////////////////////////////////////////////////////////////////
    // creating the output hierarchy over ROI pairs
    ////////////////////////////////////////////////////////////////////////////

    this->createHierarchy( *outputBundleMap );
    this->saveLabelChanges();

    if ( this->_verbose )
    {

      std::cout << gkg::Eraser( 30 );

    }

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void "
             "gkg::BundleMapOperatorBetweenRoiPairSelection< L, Compare >::"
             "endContribution( "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap, "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap "
             ") const" );

}


template < class L, class Compare >
inline
std::string 
gkg::BundleMapOperatorBetweenRoiPairSelection< L, Compare >::getName() const
{

  try
  {

    return gkg::BundleMapOperatorBetweenRoiPairSelection< L, Compare >::
                                                                getStaticName();

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "std::string "
             "gkg::BundleMapOperatorBetweenRoiPairSelection< L, Compare >::"
             "getName() const" );

}


template < class L, class Compare >
inline
std::string 
gkg::BundleMapOperatorBetweenRoiPairSelection< L, Compare >::getStaticName()
{

  try
  {

    return "between-roi-pair-selection";

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "std::string "
             "gkg::BundleMapOperatorBetweenRoiPairSelection< L, Compare >::"
             "getStaticName()" );

}


template < class L, class Compare >
inline
std::string 
gkg::BundleMapOperatorBetweenRoiPairSelection< L, Compare >::
                                                        getStringParameterHelp()
{

  try
  {

    return std::string(
      ".  <P1>: ROI mask filename\n"
      ".  <P2>: ROI Label to Name LUT filename \n"
      ".  <P3>: Output hierarchy filename\n"
      ".  <P4>: ROI to DWI frame linear transform3d filename "
      "(default=optional)\n"
      ".  <P5>: ROI to DWI frame diffeomorphic direct transform3d filename "
      "(optional)\n"
      ".  <P6>: ROI to DWI frame diffeomorphic inverse transform3d filename "
      "(optional)\n" );

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "std::string "
             "gkg::BundleMapOperatorBetweenRoiPairSelection< L, Compare >::"
             "getStringParameterHelp()" );

}


template < class L, class Compare >
inline
std::string 
gkg::BundleMapOperatorBetweenRoiPairSelection< L, Compare >::
                                                        getScalarParameterHelp()
{

  try
  {

    return std::string(
      ".  <P1>: threshold on distance from the extremities in mm (do not"
      ".        forget to reorder extremities before)\n"
      ".  <P2>: input fiber resampling step in mm (default=0.1mm)\n" );

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "std::string "
             "gkg::BundleMapOperatorBetweenRoiPairSelection< L, Compare >::"
             "getStringParameterHelp()" );

}


template < class L, class Compare >
inline
L gkg::BundleMapOperatorBetweenRoiPairSelection< L, Compare >::getLabel(
                                                    int32_t roiLabel1,
                                                    int32_t roiLabel2,
                                                    int32_t clusterIndex ) const
{

  try
  {

    throw std::runtime_error( "should not enter this code" );

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "L gkg::BundleMapOperatorBetweenRoiPairSelection< L, Compare >::"
             "getLabel( "
             "int16_t roiLabel1, "
             "int16_t roiLabel2, "
             "int32_t clusterIndex ) const" );

}


namespace gkg
{


template <>
inline
std::string 
BundleMapOperatorBetweenRoiPairSelection< std::string,
                                          std::less< std::string > >::getLabel(
                                                    int32_t roiLabel1,
                                                    int32_t roiLabel2,
                                                    int32_t clusterIndex ) const
{

  try
  {

    std::map< int32_t, std::string >::const_iterator
      n1 = _roiLabelToNameLut.find( roiLabel1 );
    std::map< int32_t, std::string >::const_iterator
      n2 = _roiLabelToNameLut.find( roiLabel2 );


    std::string prefix = "";
    if ( clusterIndex < 10 )
    {

      prefix = "00";

    }
    else if ( clusterIndex < 100 )
    {

      prefix = "0";

    }

    std::string label = n1->second + "_" +
                        n2->second + "_" +
                        prefix +
                        gkg::StringConverter::toString( clusterIndex );
    return label;

  }
  GKG_CATCH( "inline "
             "std::string  "
             "BundleMapOperatorBetweenRoiPairSelection< std::string, "
             "std::less< std::string > >::getLabel( "
             "int32_t roiLabel1, "
             "int32_t roiLabel2, "
             "int32_t clusterIndex ) const" );

}


template <>
inline
int16_t 
BundleMapOperatorBetweenRoiPairSelection< int16_t,
                                    std::less< int16_t > >::getLabel(
                                                    int32_t roiLabel1,
                                                    int32_t roiLabel2,
                                                    int32_t clusterIndex ) const
{

  try
  {

    int16_t label = ( int16_t )roiLabel1 * 1000 +
                    ( int16_t )roiLabel2 * 10 +
                    ( int16_t )clusterIndex;
    return label;

  }
  GKG_CATCH( "inline "
             "int16_t  "
             "BundleMapOperatorBetweenRoiPairSelection< int16_t, "
             "std::less< int16_t > >::getLabel( "
             "int32_t roiLabel1, "
             "int32_t roiLabel2, "
             "int32_t clusterIndex ) const" );

}


}


template < class L, class Compare >
inline
void 
gkg::BundleMapOperatorBetweenRoiPairSelection< L, Compare >::createHierarchy(
                                const BundleMap< L, Compare >& bundleMap ) const
{

  try
  {

    int32_t colorCount = bundleMap.getBundleCount();

    gkg::RandomColorGenerator randomColorGenerator;

    std::ofstream os( _fileNameOutputHierarchy.c_str() );

    os << "# tree 1.0" << std::endl;
    os << std::endl;

    os << "*BEGIN TREE hierarchy" << std::endl;
    os << "graph_syntax RoiArg" << std::endl;
    os << std::endl;


    os << "*BEGIN TREE fold_name" << std::endl;
    os << "name all" << std::endl;
    os << std::endl;

    gkg::RGBComponent rgbComponent;
    int32_t colorIndex = 0;
    typename gkg::BundleMap< L >::const_iterator
      b = bundleMap.begin(),
      be = bundleMap.end();
    while ( b != be )
    {

      rgbComponent = randomColorGenerator.getRandomRainbowColor( colorIndex,
                                                                 colorCount );
      os << "*BEGIN TREE fold_name" << std::endl;
      os << "name " << b->first << std::endl;
      os << "color "
         << ( int32_t )rgbComponent.r << " "
         << ( int32_t )rgbComponent.g << " "
         << ( int32_t )rgbComponent.b << std::endl;
      os << "*END" << std::endl;
      os << std::endl;

      ++ b;
      ++ colorIndex;

    }

    os << "*END" << std::endl;
    os << std::endl;

    os << "*END" << std::endl;
    os << std::endl;

    os.close();

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void "
             "gkg::BundleMapOperatorBetweenRoiPairSelection< L, Compare >::"
             "createHierarchy( "
             "const BundleMap< L, Compare >& bundleMap ) const" );


}


template < class L, class Compare >
inline
void 
gkg::BundleMapOperatorBetweenRoiPairSelection< L, Compare >::saveLabelChanges(
                                                                         ) const
{

  try
  {

    std::ofstream os( _fileNameLabelChanges );

    if ( !os )
    {

      throw std::runtime_error( std::string( "unable to open file'" ) +
                                _fileNameLabelChanges + "'" );

    }

    typename std::map< L, L >::const_iterator
      lc = _labelChanges.begin(),
      lce = _labelChanges.end();
    while ( lc != lce )
    {

      os << lc->first << "\t" << lc->second << std::endl;
      ++ lc;

    }

    os.close();
  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void "
             "gkg::BundleMapOperatorBetweenRoiPairSelection< L, Compare >::"
             "saveLabelChanges() const" );

}

#endif
