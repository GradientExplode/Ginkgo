#ifndef _gkg_dmri_bundlemap_operator_BundleMapOperatorMaskIntersection_i_h_
#define _gkg_dmri_bundlemap_operator_BundleMapOperatorMaskIntersection_i_h_


#include <gkg-dmri-bundlemap-operator/BundleMapOperatorMaskIntersection.h>
#include <gkg-dmri-bundlemap-operator/BundleMapOperator_i.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-container/BoundingBox.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-processing-transform/RigidTransform3d.h>
#include <gkg-processing-transform/AffineTransform3d.h>
#include <gkg-processing-transform/IdentityTransform3d.h>
#include <gkg-processing-transform/Scaling3d.h>
#include <gkg-processing-transform/CompositeTransform3d.h>
#include <gkg-processing-io/TypedVolumeReaderProcess.h>
#include <gkg-dmri-container/BundleMap_i.h>
#include <gkg-processing-algobase/TestFunction_i.h>
#include <gkg-processing-algobase/TestFunctionFactory.h>
#include <gkg-core-exception/Exception.h>




template < class L, class Compare >
inline
gkg::BundleMapOperatorMaskIntersection< L, Compare >::
                                             BundleMapOperatorMaskIntersection(
                             const std::vector< std::string >& stringParameters,
                             const std::vector< double >& scalarParameters,
                             bool verbose )
                               : gkg::BundleMapOperator< L, Compare >( verbose )
{

  try
  {

    // sanity checks for string parameters
    if ( ( stringParameters.size() != 3U ) )
    {

      throw std::runtime_error( "string parameters must have 3 arguments" );

    }

    // sanity checks for scalar parameters
    if ( scalarParameters.size() != 4U  )
    {

      throw std::runtime_error( "scalar parameters must have 4 arguments" );

    }


    _fileNameRois = stringParameters[ 0 ];
    _fileNameTransform3d = stringParameters[ 1 ];


    // setting the test function
    _testFunction.reset( gkg::TestFunctionFactory< int32_t >::getInstance().
                             createTestFunction( stringParameters[ 2 ],
                                ( int32_t )( scalarParameters[ 0 ] + 0.5 ),
                                ( int32_t )( scalarParameters[ 1 ] + 0.5 ) ) );

    _minimumPortionLength = scalarParameters[ 2 ];
    _inputFiberResamplingStep = scalarParameters[ 3 ];

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "gkg::BundleMapOperatorMaskIntersection< L, Compare >::"
             "BundleMapOperatorMaskIntersection( "
             "const std::vector< std::string >& stringParameters, "
             "const std::vector< double >& scalarParameters, "
             "bool verbose )" );

}


template < class L, class Compare >
inline
gkg::BundleMapOperatorMaskIntersection< L, Compare >::
                                           ~BundleMapOperatorMaskIntersection()
{
}



template < class L, class Compare >
inline
void gkg::BundleMapOperatorMaskIntersection< L, Compare >::startContribution(
                gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap,
                gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap )
{

  try
  {

    // allocating the ROI volume
    _rois.reset( new gkg::Volume< int32_t > );

    // reading the ROI from file
    gkg::TypedVolumeReaderProcess< int32_t > typedVolumeReaderProcess( *_rois );
    typedVolumeReaderProcess.execute( _fileNameRois );

    gkg::Vector3d< double > resolution;
    _rois->getResolution( resolution );

    // reading or building the 3D transform from ROI to DWI frame
    if ( _fileNameTransform3d != "id" )
    {

     try
      {

        gkg::RCPointer< gkg::RigidTransform3d< float > >
          rigidTransform3d( new gkg::RigidTransform3d< float > );
        std::ifstream is( _fileNameTransform3d.c_str() );
        rigidTransform3d->readTrm( is );
        is.close();
        _transform3dFromRoisRealToFiberReal = rigidTransform3d;

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
          _transform3dFromRoisRealToFiberReal = affineTransform3d;

        }
        catch( std::exception& )
        {

          throw std::runtime_error( "invalid 3D ROIs->fibers transform" );

        }

      }

    }
    else
    {

      _transform3dFromRoisRealToFiberReal.reset(
                                        new gkg::IdentityTransform3d< float > );

    }

    // adding contribution of the input bundle map to the output bundle map
    this->addContribution( inputBundleMap, outputBundleMap );

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void gkg::BundleMapOperatorMaskIntersection< L, Compare >::"
             "startContribution( "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap, "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap "
             ") const" );

}


template < class L, class Compare >
inline
void gkg::BundleMapOperatorMaskIntersection< L, Compare >::addContribution(
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


    // looping over input bundles
    int32_t resampledPointCount = 0;
    typename gkg::BundleMap< L, Compare >::Fiber resampledFiber;
    int32_t index = 0;
    int32_t lastIndex = 0;
    int32_t startingPortionIndex = 0;
    int32_t endingPortionIndex = 0;

    typename gkg::BundleMap< L, Compare >::const_iterator
      b = inputBundleMap->begin(),
      be = inputBundleMap->end();
    while ( b != be )
    {

      // allocating the list of kept fiber portions
      std::list< gkg::LightCurve3d< float > > keptFiberPortions;

      // looping over fibers of the current bundle
      typename gkg::BundleMap< L, Compare >::Bundle::const_iterator
        f = b->second.begin(),
        fe = b->second.end();
      while ( f != fe )
      {


        // resampling the fiber at the input resolution
        resampledPointCount = std::max( 2, ( int32_t )( f->getLength() /
                                            _inputFiberResamplingStep + 0.5 ) );
        resampledFiber = f->getEquidistantCurve( resampledPointCount );

        // computing the profile of labels crossed by the fiber
        std::vector< int32_t > labels;
        resampledFiber.getScalarVolumeProfile(
                                           *_rois,
                                           *_transform3dFromRoisRealToFiberReal,
                                           labels,
                                           -1,
                                           0,
                                           false );


        // the selecting the fiber portions intersection the mask
        index = 0;
        lastIndex = resampledFiber.getPointCount();


        startingPortionIndex = 0;
        endingPortionIndex = lastIndex - 1;

        for ( index = 0; index < lastIndex; index ++ )
        {

          while ( !_testFunction->getState( labels[ index ] ) &&
                  ( index < lastIndex ) )
          {

            ++ index;

          }
          
          if ( index < lastIndex )
          {

            startingPortionIndex = index;
            do
            {

              ++ index;

            }
            while ( ( index < lastIndex ) &&
                     _testFunction->getState( labels[ index ] ) );

            if ( index <= lastIndex )
            {

              endingPortionIndex = index - 1;
              // keepinp a fober portion only if its length is sufficient
              if ( resampledFiber.getLength( startingPortionIndex,
                                             endingPortionIndex ) >=
                   _minimumPortionLength )
              {

                keptFiberPortions.push_back(
                  resampledFiber.getCurvePart( startingPortionIndex,
                                               endingPortionIndex ) );

              }

            }

          }

        }


        ++ f;

      }

      // adding the kept fiber portions to the current output bundle
      outputBundleMap->addCurve3ds( b->first, keptFiberPortions );
      
      ++ b;

    }

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void gkg::BundleMapOperatorMaskIntersection< L, Compare >::"
             "addContribution( "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap, "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap "
             ") const" );

}


template < class L, class Compare >
inline
void gkg::BundleMapOperatorMaskIntersection< L, Compare >::endContribution(
                gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap,
                gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap )
{

  try
  {

    this->addContribution( inputBundleMap, outputBundleMap );

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void gkg::BundleMapOperatorMaskIntersection< L, Compare >::"
             "endContribution( "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap, "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap "
             ") const" );

}


template < class L, class Compare >
inline
std::string 
gkg::BundleMapOperatorMaskIntersection< L, Compare >::getName() const
{

  try
  {

    return 
      gkg::BundleMapOperatorMaskIntersection< L, Compare >::getStaticName();

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "std::string "
             "gkg::BundleMapOperatorMaskIntersection< L, Compare >::"
             "getName() const" );

}


template < class L, class Compare >
inline
std::string 
gkg::BundleMapOperatorMaskIntersection< L, Compare >::getStaticName()
{

  try
  {

    return "mask-intersection";

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "std::string "
             "gkg::BundleMapOperatorMaskIntersection< L, Compare >::"
             "getStaticName()" );

}


template < class L, class Compare >
inline
std::string 
gkg::BundleMapOperatorMaskIntersection< L, Compare >::getStringParameterHelp()
{

  try
  {

    return std::string(
      ".  <P1>: ROI mask file name\n"
      ".  <P2>: ROI to DWI frame transform3d filename (default=id)\n"
      ".  <P3>: test mode among\n"
      ".        - lt -> lower than\n"
      ".        - le -> lower or equal to\n"
      ".        - eq -> equal to\n"
      ".        - di -> different from\n"
      ".        - ge -> greater or equal to\n"
      ".        - gt -> greater than\n"
      ".        - be -> between or equal\n"
      ".        - bt -> between\n"
      ".        - oe -> outside or equal\n"
      ".        - ou -> outside\n" );

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "std::string "
             "gkg::BundleMapOperatorMaskIntersection< L, Compare >::"
             "getStringParameterHelp()" );

}


template < class L, class Compare >
inline
std::string 
gkg::BundleMapOperatorMaskIntersection< L, Compare >::getScalarParameterHelp()
{

  try
  {

    return std::string(
      ".  <P1>: first threshold\n"
      ".  <P2>: second threshold (set 0.0 when useless)\n"
      ".  <P3>: minimum length in mm of fiber portions to be kept\n"
      ".  <P4>: input fiber resampling step in mm (default=0.1mm)\n" );

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "std::string "
             "gkg::BundleMapOperatorMaskIntersection< L, Compare >::"
             "getStringParameterHelp()" );

}


#endif
