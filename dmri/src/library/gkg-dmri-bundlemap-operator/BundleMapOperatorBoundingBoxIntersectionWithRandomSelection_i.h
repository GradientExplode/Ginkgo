#ifndef _gkg_dmri_bundlemap_operator_BundleMapOperatorBoundingBoxIntersectionWithRandomSelection_i_h_
#define _gkg_dmri_bundlemap_operator_BundleMapOperatorBoundingBoxIntersectionWithRandomSelection_i_h_


#include <gkg-dmri-bundlemap-operator/BundleMapOperatorBoundingBoxIntersectionWithRandomSelection.h>
#include <gkg-dmri-bundlemap-operator/BundleMapOperator_i.h>
#include <gkg-dmri-container/BundleMap_i.h>
#include <gkg-processing-algobase/RandomIndexVector.h>
#include <gkg-core-exception/Exception.h>


template < class L, class Compare >
inline
gkg::BundleMapOperatorBoundingBoxIntersectionWithRandomSelection< L, Compare >::
                    BundleMapOperatorBoundingBoxIntersectionWithRandomSelection(
                             const std::vector< std::string >& stringParameters,
                             const std::vector< double >& scalarParameters,
                             bool verbose )
                               : gkg::BundleMapOperator< L, Compare >( verbose )
{

  try
  {

    // sanity checks for string parameters
    if ( !stringParameters.empty()  )
    {

      throw std::runtime_error( "no string string parameter required" );

    }

    // sanity checks for scalar parameters
    if ( ( scalarParameters.size() != 6U ) &&
         ( scalarParameters.size() != 7U ) )
    {

      throw std::runtime_error(
                               "scalar parameters must have 6 or 7 arguments" );

    }

    // collecting the bounding box for selection
    _boundingBox.setLowerX( ( float )scalarParameters[ 0 ] );
    _boundingBox.setLowerY( ( float )scalarParameters[ 1 ] );
    _boundingBox.setLowerZ( ( float )scalarParameters[ 2 ] );
    _boundingBox.setUpperX( ( float )scalarParameters[ 3 ] );
    _boundingBox.setUpperY( ( float )scalarParameters[ 4 ] );
    _boundingBox.setUpperZ( ( float )scalarParameters[ 5 ] );

    //  by default do not randomly select fibers
    _keptFiberPercentage = 100.0;
 
    // but it case it is wanted, collect the percentage of kept fibers
    if ( scalarParameters.size() == 7U )
    {

      if ( ( scalarParameters[ 6 ] < 0.0 ) ||
           ( scalarParameters[ 6 ] > 100.0 ) )
      {

        throw std::runtime_error(
                        "kept fiber percentage must be between 0.0 and 100.0" );

      }
      _keptFiberPercentage = scalarParameters[ 6 ];

    }

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "gkg::BundleMapOperatorBoundingBoxIntersectionWithRandomSelection"
             "< L, Compare >::"
             "BundleMapOperatorBoundingBoxIntersectionWithRandomSelection( "
             "const std::vector< std::string >& stringParameters, "
             "const std::vector< double >& scalarParameters, "
             "bool verbose )" );

}


template < class L, class Compare >
inline
gkg::BundleMapOperatorBoundingBoxIntersectionWithRandomSelection< L, Compare >::
                  ~BundleMapOperatorBoundingBoxIntersectionWithRandomSelection()
{
}



template < class L, class Compare >
inline
void 
gkg::BundleMapOperatorBoundingBoxIntersectionWithRandomSelection< L, Compare >::
  startContribution(
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


    // if no random sampling is required
    if ( _keptFiberPercentage == 100.0 )
    {

      // looping over input bundles
      typename gkg::BundleMap< L, Compare >::const_iterator
        ib = inputBundleMap->begin(),
        ibe = inputBundleMap->end();
      while ( ib != ibe )
      {

        std::list< gkg::LightCurve3d< float > > fibers;
        std::vector< gkg::LightCurve3d< float > >::const_iterator
          f = ib->second.begin(),
          fe = ib->second.end();
        while ( f != fe )
        {

          this->addToSelection( fibers, *f );
          ++ f;

        }

        // adding the kept pieces of fibers to the output bundle map
        if ( !fibers.empty() )
        {

          outputBundleMap->addCurve3ds( ib->first, fibers );

        }

        ++ ib;

      }

    }
    else
    {

      // looping over input bundles
      typename gkg::BundleMap< L, Compare >::const_iterator
        ib = inputBundleMap->begin(),
        ibe = inputBundleMap->end();
      while ( ib != ibe )
      {

        // computing the number of kept curves (at least 1 per bundle)
        int32_t keptCurve3dCount = std::max(
                                 1,
                                 ( int32_t )( ib->second.getCurve3dCount() *
                                              _keptFiberPercentage / 100.0 ) );

        // computing the selection
        std::vector< bool > selection( ib->second.getCurve3dCount() );
        if( ib->second.getCurve3dCount() > 0 )
        {

          gkg::getRandomSelection( selection, keptCurve3dCount );

        }

        // collecting the selected fibers
        std::list< gkg::LightCurve3d< float > > fibers;
        std::vector< gkg::LightCurve3d< float > >::const_iterator
          f = ib->second.begin(),
          fe = ib->second.end();
        std::vector< bool >::const_iterator s = selection.begin();
        while ( f != fe )
        {

          if ( *s )
          {

            this->addToSelection( fibers, *f );

          }
          ++ f;
          ++ s;

        }

        // adding the kept fibers to the output bundle map
        if ( !fibers.empty() )
        {

          outputBundleMap->addCurve3ds( ib->first, fibers );

        }

        ++ ib;

      }

    }

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void "
             "gkg::BundleMapOperatorBoundingBoxIntersectionWithRandomSelection"
             "< L, Compare >::"
             "startContribution( "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap, "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap "
             ") const" );

}


template < class L, class Compare >
inline
void 
gkg::BundleMapOperatorBoundingBoxIntersectionWithRandomSelection< L, Compare >::
  addContribution(
                gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap,
                gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap )
{

  try
  {

     this->startContribution( inputBundleMap, outputBundleMap );

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void "
             "gkg::BundleMapOperatorBoundingBoxIntersectionWithRandomSelection"
             "< L, Compare >::"
             "addContribution( "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap, "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap "
             ") const" );

}


template < class L, class Compare >
inline
void 
gkg::BundleMapOperatorBoundingBoxIntersectionWithRandomSelection< L, Compare >::
  endContribution(
                gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap,
                gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap )
{

  try
  {

     this->startContribution( inputBundleMap, outputBundleMap );

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void "
             "gkg::BundleMapOperatorBoundingBoxIntersectionWithRandomSelection"
             "< L, Compare >::"
             "endContribution( "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap, "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap "
             ") const" );

}


template < class L, class Compare >
inline
std::string 
gkg::BundleMapOperatorBoundingBoxIntersectionWithRandomSelection< L, Compare >::
                                                                 getName() const
{

  try
  {

    return 
      gkg::BundleMapOperatorBoundingBoxIntersectionWithRandomSelection<
                                                     L,
                                                     Compare >::getStaticName();

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "std::string "
             "gkg::BundleMapOperatorBoundingBoxIntersectionWithRandomSelection"
             "< L, Compare >::"
             "getName() const" );

}


template < class L, class Compare >
inline
std::string 
gkg::BundleMapOperatorBoundingBoxIntersectionWithRandomSelection< L, Compare >::
                                                                 getStaticName()
{

  try
  {

    return "bounding-box-intersection-with-random-selection";

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "std::string "
             "gkg::BundleMapOperatorBoundingBoxIntersectionWithRandomSelection"
             "< L, Compare >::"
             "getStaticName()" );

}


template < class L, class Compare >
inline
std::string 
gkg::BundleMapOperatorBoundingBoxIntersectionWithRandomSelection< L, Compare >::
                                                        getStringParameterHelp()
{

  try
  {

    return std::string( ".  N/A\n" );

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "std::string "
             "gkg::BundleMapOperatorBoundingBoxIntersectionWithRandomSelection"
             "< L, Compare >::"
             "getStringParameterHelp()" );

}


template < class L, class Compare >
inline
std::string 
gkg::BundleMapOperatorBoundingBoxIntersectionWithRandomSelection< L, Compare >::
                                                        getScalarParameterHelp()
{

  try
  {

    return std::string(
      ".  <P1>: bounding box lower X\n"
      ".  <P2>: bounding box lower Y\n"
      ".  <P3>: bounding box lower Z\n"
      ".  <P4>: bounding box upper X\n"
      ".  <P5>: bounding box upper Y\n"
      ".  <P6>: bounding box upper Z\n"
      ".  <P7>: kept fiber percentage between 0.0 and 100.0 "
      "(default=100.0)\n" );

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "std::string "
             "gkg::BundleMapOperatorBoundingBoxIntersectionWithRandomSelection"
             "< L, Compare >::"
             "getStringParameterHelp()" );

}


template < class L, class Compare >
void 
gkg::BundleMapOperatorBoundingBoxIntersectionWithRandomSelection< L, Compare >::
  addToSelection( std::list< LightCurve3d< float > >& fibers,
                  const LightCurve3d< float >& fiber ) const
{

  try
  {

    bool isANewFiber = true;
    gkg::LightCurve3d< float > newFiber;


    std::list< gkg::Vector3d< float > > points;

    gkg::LightCurve3d< float >::const_iterator p = fiber.begin(),
                                               pe = fiber.end();
    while ( p != pe )
    {

      if ( _boundingBox.contains( *p ) )
      {

        if ( isANewFiber == true )
        {

          isANewFiber = false;

        }
        points.push_back( *p );

      }
      else
      {

        if ( points.size() >= 2U )
        {

          fibers.push_back( gkg::LightCurve3d< float >( points ) );

        }
        points.clear();
        isANewFiber = true;

      }
      ++ p;

    }

  }
  GKG_CATCH( "template < class L, class Compare > "
             "void "
             "gkg::BundleMapOperatorBoundingBoxIntersectionWithRandomSelection"
             "< L, Compare >::"
             "addToSelection( std::list< LightCurve3d< float > > fibers, "
             "const LightCurve3d< float >& fiber ) const" );

}


#endif
