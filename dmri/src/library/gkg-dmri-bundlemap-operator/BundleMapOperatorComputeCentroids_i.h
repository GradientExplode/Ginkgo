#ifndef _gkg_dmri_bundlemap_operator_BundleMapOperatorComputeCentroids_i_h_
#define _gkg_dmri_bundlemap_operator_BundleMapOperatorComputeCentroids_i_h_


#include <gkg-dmri-bundlemap-operator/BundleMapOperatorComputeCentroids.h>
#include <gkg-dmri-bundlemap-operator/BundleMapOperator_i.h>
#include <gkg-dmri-container/BundleMap_i.h>
#include <gkg-processing-algobase/RandomIndexVector.h>
#include <gkg-processing-numericalanalysis/Matrix.h>
#include <gkg-processing-algobase/MinimumIndexFilter_i.h>
#include <gkg-core-exception/Exception.h>


template < class L, class Compare >
inline
gkg::BundleMapOperatorComputeCentroids< L, Compare >::
                                              BundleMapOperatorComputeCentroids(
                             const std::vector< std::string >& stringParameters,
                             const std::vector< double >& scalarParameters,
                             bool verbose )
                               : gkg::BundleMapOperator< L, Compare >( verbose )
{

  try
  {

    // sanity checks for string parameters
    if ( stringParameters.size() != 3U  )
    {

      throw std::runtime_error( "3 string parameter required" );

    }

    // collecting centroid type
    if ( stringParameters[ 0 ] == "closest-fiber" )
    {

      _centroidType =
             gkg::BundleMapOperatorComputeCentroids< L, Compare >::ClosestFiber;

    }
    else if ( stringParameters[ 0 ] == "average-fiber" )
    {

      _centroidType =
             gkg::BundleMapOperatorComputeCentroids< L, Compare >::AverageFiber;

    }
    else
    {

      throw std::runtime_error(
                 "centroid type should be 'closest-fiber' or 'average-fiber'" );

    }

    // collecting selection mode
    if ( stringParameters[ 1 ] == "random" )
    {

      _selectionMode =
                   gkg::BundleMapOperatorComputeCentroids< L, Compare >::Random;

    } 
    else if ( stringParameters[ 1 ] == "all" )
    {

      _selectionMode =
                gkg::BundleMapOperatorComputeCentroids< L, Compare >::All;

    }
    else
    {

      throw std::runtime_error( "selection mode should be 'random' or 'all'" );

    }

    // collecting distance type
     if ( stringParameters[ 2 ] ==
                               "symmetric-mean-of-mean-closest-point-distance" )
    {

      _distanceType =
            gkg::LightCurve3d< float >::SymmetricMeanOfMeanClosestPointDistance;

    } 
    else if ( stringParameters[ 2 ] ==
                            "symmetric-maximum-of-mean-closest-point-distance" )
    {

      _distanceType =
         gkg::LightCurve3d< float >::SymmetricMaximumOfMeanClosestPointDistance;

    }
    else
    {

      throw std::runtime_error(
      "distance type should be 'symmetric-mean-of-mean-closest-point-distance' "
      "or 'symmetric-maximum-of-mean-closest-point-distance'" );

    }
   

    // sanity checks for scalar parameters
    if ( scalarParameters.empty()  )
    {

      _maximumFiberCount = 10000;
      _centroidPointCount = 100;
      _verbose = false;

    }
    else
    {

      if ( scalarParameters.size() != 3U  )
      {

        throw std::runtime_error( "3 scalar parameter required" );

      }
      else
      {

        _maximumFiberCount = ( int32_t )( scalarParameters[ 0 ] + 0.5 );
        _centroidPointCount = ( int32_t )( scalarParameters[ 1 ] + 0.5 );
        _verbose = ( ( ( int32_t )( scalarParameters[ 2 ] + 0.5 ) ) ?
                     true : false );

      }

    }

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "gkg::BundleMapOperatorComputeCentroids< L, Compare >::"
             "BundleMapOperatorComputeCentroids( "
             "const std::vector< std::string >& stringParameters, "
             "const std::vector< double >& scalarParameters, "
             "bool verbose )" );

}


template < class L, class Compare >
inline
gkg::BundleMapOperatorComputeCentroids< L, Compare >::
                                            ~BundleMapOperatorComputeCentroids()
{
}



template < class L, class Compare >
inline
void gkg::BundleMapOperatorComputeCentroids< L, Compare >::startContribution(
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

    // allocating output labels and bundles to collect centroid(s)
    std::list< L > outputLabels;
    std::list< typename gkg::BundleMap< L, Compare >::Bundle > outputBundles;

    ////////////////////////////////////////////////////////////////////////////
    // average fiber centroid
    ////////////////////////////////////////////////////////////////////////////
    if ( _centroidType ==
         gkg::BundleMapOperatorComputeCentroids< L, Compare >::AverageFiber )
    {

      // case of random selection //////////////////////////////////////////////
      if ( _selectionMode ==
           gkg::BundleMapOperatorComputeCentroids< L, Compare >::Random )
      {

        // looping over input bundles
        typename gkg::BundleMap< L, Compare >::iterator
          ib = inputBundleMap->begin(),
          ibe = inputBundleMap->end();
        while ( ib != ibe )
        {

          // getting label of current bundle
          const L& currentLabel = ib->first;

          if ( _verbose )
          {

            std::cout << currentLabel << " " << std::flush;

          }

          // getting reference to current bundle
          typename gkg::BundleMap< L, Compare >::Bundle&
            currentBundle = ib->second;

          if ( currentBundle.getCurve3dCount() != 0 )
          {

            // adding label to output label list
            outputLabels.push_back( currentLabel );

            // creating an output bundle
            typename gkg::BundleMap< L, Compare >::Bundle outputBundle;

            // getting fiber count
            int32_t fiberCount = currentBundle.getCurve3dCount();

            // if the fiber count remains lower than the maximum fiber count
            if ( fiberCount < _maximumFiberCount )
            {

              // setting the same number of points for all the fibers
              currentBundle.setEquidistant( _centroidPointCount );

              // computing the average fiber
              outputBundle.addCurve3d( currentBundle.getAverage() );

            }
            // else if the fiber count exceeds the maximum fiber count
            else
            {

              // creating a vector of random indices of same size as the
              // input bundle
              std::vector< int32_t >
                randomIndices( currentBundle.getCurve3dCount() );
              gkg::getRandomIndexVector( randomIndices );

              // collecting the randomly selected fibers
              std::list< gkg::LightCurve3d< float > > randomlySelectedFibers;
              int32_t i = 0;
              for ( i = 0; i < _maximumFiberCount; i++ )
              {
 
                randomlySelectedFibers.push_back( currentBundle.getCurve3d(
                                                         randomIndices[ i ] ) );

              }  

              // instanciating a randomly sampled bundle
              typename 
              gkg::BundleMap< L, Compare >::Bundle randomlySampledBundle;
              randomlySampledBundle.addCurve3ds( randomlySelectedFibers );

              // setting the same number of points for all the fibers
              randomlySampledBundle.setEquidistant( _centroidPointCount );

              // computing the average fiber
              outputBundle.addCurve3d( randomlySampledBundle.getAverage() );

            }

            // adding centroid bundle to output bundle list
            outputBundles.push_back( outputBundle );

          }

          ++ ib;

        }

      }
      // case of all selection /////////////////////////////////////////////////
      else if ( _selectionMode ==
              gkg::BundleMapOperatorComputeCentroids< L, Compare >::All )
      {

        // looping over input bundles
        typename gkg::BundleMap< L, Compare >::iterator
          ib = inputBundleMap->begin(),
          ibe = inputBundleMap->end();
        while ( ib != ibe )
        {

          // getting label of current bundle
          const L& currentLabel = ib->first;

          if ( _verbose )
          {

            std::cout << currentLabel << " " << std::flush;

          }

          // getting reference to current bundle
          typename gkg::BundleMap< L, Compare >::Bundle&
            currentBundle = ib->second;

          if ( currentBundle.getCurve3dCount() != 0 )
          {

            // adding label to output label list
            outputLabels.push_back( currentLabel );

            // creating an output bundle
            typename gkg::BundleMap< L, Compare >::Bundle outputBundle;

            // setting the same number of points for all the fibers
            currentBundle.setEquidistant( _centroidPointCount );

            // computing the average fiber
            outputBundle.addCurve3d( currentBundle.getAverage() );

            // adding centroid bundle to output bundle list
            outputBundles.push_back( outputBundle );

          }

          ++ ib;

        }

      }

    }
    ////////////////////////////////////////////////////////////////////////////
    // closest fiber centroid
    ////////////////////////////////////////////////////////////////////////////
    else if ( _centroidType ==
            gkg::BundleMapOperatorComputeCentroids< L, Compare >::ClosestFiber )
    {

      // case of random selection //////////////////////////////////////////////
      if ( _selectionMode ==
           gkg::BundleMapOperatorComputeCentroids< L, Compare >::Random )
      {

        // looping over input bundles
        typename gkg::BundleMap< L, Compare >::const_iterator
          ib = inputBundleMap->begin(),
          ibe = inputBundleMap->end();
        while ( ib != ibe )
        {

          // getting label of current bundle
          const L& currentLabel = ib->first;

          if ( _verbose )
          {

            std::cout << currentLabel << " " << std::flush;

          }

          // getting reference to current bundle
          const typename gkg::BundleMap< L, Compare >::Bundle&
            currentBundle = ib->second;

          if ( currentBundle.getCurve3dCount() != 0 )
          {

            // adding label to output label list
            outputLabels.push_back( currentLabel );

            // creating an output bundle
            typename gkg::BundleMap< L, Compare >::Bundle outputBundle;

            // getting fiber count
            int32_t fiberCount = currentBundle.getCurve3dCount();

            // if the fiber count remains lower than the maximum fiber count
            if ( fiberCount < _maximumFiberCount )
            {

              // getting index of centroid
              int32_t centroidIndex = getFastCentroidIndex( currentBundle );

              // ading centroid to outputBundle
              outputBundle.addCurve3d(
                                    currentBundle.getCurve3d( centroidIndex ) );

            }
            // else if the fiber count exceeds the maximum fiber count
            else
            {

              // creating a vector of random indices of same size as the
              // input bundle
              std::vector< int32_t >
                randomIndices( currentBundle.getCurve3dCount() );
              gkg::getRandomIndexVector( randomIndices );

              // collecting the randomly selected fibers
              std::list< gkg::LightCurve3d< float > > randomlySelectedFibers;
              int32_t i = 0;
              for ( i = 0; i < _maximumFiberCount; i++ )
              {
 
                randomlySelectedFibers.push_back( currentBundle.getCurve3d(
                                                         randomIndices[ i ] ) );

              }  

              // instanciating a randomly sampled bundle
              typename 
              gkg::BundleMap< L, Compare >::Bundle randomlySampledBundle;
              randomlySampledBundle.addCurve3ds( randomlySelectedFibers );

              // getting index of centroid
              int32_t centroidIndex = getFastCentroidIndex(
                                                        randomlySampledBundle );

              // ading centroid to outputBundle
              outputBundle.addCurve3d(
                            randomlySampledBundle.getCurve3d( centroidIndex ) );

            }

            // adding centroid bundle to output bundle list
            outputBundles.push_back( outputBundle );

          }

          ++ ib;

        }

      }
      // case of all selection /////////////////////////////////////////////////
      else if ( _selectionMode ==
              gkg::BundleMapOperatorComputeCentroids< L, Compare >::All )
      {

        // looping over input bundles
        typename gkg::BundleMap< L, Compare >::const_iterator
          ib = inputBundleMap->begin(),
          ibe = inputBundleMap->end();
        while ( ib != ibe )
        {

          // getting label of current bundle
          const L& currentLabel = ib->first;

          if ( _verbose )
          {

            std::cout << currentLabel << " " << std::flush;

          }

          // getting reference to current bundle
          const typename gkg::BundleMap< L, Compare >::Bundle&
            currentBundle = ib->second;

          if ( currentBundle.getCurve3dCount() != 0 )
          {

            // adding label to output label list
            outputLabels.push_back( currentLabel );

            // creating an output bundle
            typename gkg::BundleMap< L, Compare >::Bundle outputBundle;

            // getting fiber count
            int32_t fiberCount = currentBundle.getCurve3dCount();

            // if the fiber count remains lower than the maximum fiber count
            if ( fiberCount < _maximumFiberCount )
            {

              // getting index of centroid
              int32_t centroidIndex = getFastCentroidIndex( currentBundle );

              // ading centroid to outputBundle
              outputBundle.addCurve3d(
                                    currentBundle.getCurve3d( centroidIndex ) );

            }
            // else if the fiber count exceeds the maximum fiber count
            else
            {

              // getting index of centroid
              int32_t centroidIndex = getSlowCentroidIndex( currentBundle );

              // ading centroid to outputBundle
              outputBundle.addCurve3d(
                                    currentBundle.getCurve3d( centroidIndex ) );

            }

            // adding centroid bundle to output bundle list
            outputBundles.push_back( outputBundle );

          }

          ++ ib;

        }

      }

    }

    // adding output labels/centroids to the output bundle map
    outputBundleMap->addBundles( outputLabels, outputBundles );


  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void gkg::BundleMapOperatorComputeCentroids< L, Compare >::"
             "startContribution( "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap, "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap "
             ") const" );

}


template < class L, class Compare >
inline
void gkg::BundleMapOperatorComputeCentroids< L, Compare >::addContribution(
                gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap,
                gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap )
{

  try
  {

     this->startContribution( inputBundleMap, outputBundleMap );

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void gkg::BundleMapOperatorComputeCentroids< L, Compare >::"
             "addContribution( "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap, "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap "
             ") const" );

}


template < class L, class Compare >
inline
void gkg::BundleMapOperatorComputeCentroids< L, Compare >::endContribution(
                gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap,
                gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap )
{

  try
  {

     this->startContribution( inputBundleMap, outputBundleMap );

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void gkg::BundleMapOperatorComputeCentroids< L, Compare >::"
             "endContribution( "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap, "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap "
             ") const" );

}


template < class L, class Compare >
inline
std::string 
gkg::BundleMapOperatorComputeCentroids< L, Compare >::getName() const
{

  try
  {

    return 
      gkg::BundleMapOperatorComputeCentroids< L, Compare >::getStaticName();

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "std::string "
             "gkg::BundleMapOperatorComputeCentroids< L, Compare >::"
             "getName() const" );

}


template < class L, class Compare >
inline
std::string 
gkg::BundleMapOperatorComputeCentroids< L, Compare >::getStaticName()
{

  try
  {

    return "compute-centroids";

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "std::string "
             "gkg::BundleMapOperatorComputeCentroids< L, Compare >::"
             "getStaticName()" );

}


template < class L, class Compare >
inline
std::string 
gkg::BundleMapOperatorComputeCentroids< L, Compare >::getStringParameterHelp()
{

  try
  {

    return std::string(
      ".  <P1>: centroid type, one of:\n"
      ".        - closest-fiber to the other(s) based on the definition of a\n"
      ".          distance type (see <P3>)\n"
      ".        - average-fiber of the other(s)\n"
      ".  <P2>: sampling mode of fibers, one of:\n"
      ".        - random -> random selection of fibers\n"
      ".        - all -> all fibers are selected\n"
      ".  <P3>: distance type required if 'closest-fiber' centroid chosen,\n"
      ".        one of:\n"
      ".        - symmetric-mean-of-mean-closest-point-distance\n"
      ".        - symmetric-maximum-of-mean-closest-point-distance\n" );

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "std::string "
             "gkg::BundleMapOperatorComputeCentroids< L, Compare >::"
             "getStringParameterHelp()" );

}


template < class L, class Compare >
inline
std::string 
gkg::BundleMapOperatorComputeCentroids< L, Compare >::getScalarParameterHelp()
{

  try
  {

    return std::string(   
      ".  <P1>: fiber count to be used in random mode or from which\n"
      ".        the 'all' selection mode switch from a fast to a slow\n"
      ".        algorithm to avoid running out of memory (default=10000)\n"
      ".  <P2>: centroid point count for 'average-fiber' centroid type\n"
      ".        (default=100)\n"
      ".  <P3>: verbosity (default=false)\n" );

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "std::string "
             "gkg::BundleMapOperatorComputeCentroids< L, Compare >::"
             "getStringParameterHelp()" );

}


template < class L, class Compare >
inline
int32_t 
gkg::BundleMapOperatorComputeCentroids< L, Compare >::getFastCentroidIndex(
             const typename gkg::BundleMap< L, Compare >::Bundle& bundle ) const
{

  try
  {

    // collecting fiber count
    int32_t fiberCount = bundle.getCurve3dCount();

    // allocating distance matrix and setting its diagonal to nul
    gkg::Matrix distanceMatrix( fiberCount, fiberCount );
    distanceMatrix.setDiagonal( 0.0 );

    // computing the distance matrix
    int32_t f1 = 0;
    int32_t f2 = 0;
    for ( f1 = 0; f1 < fiberCount; f1++ )
    {

      for ( f2 = f1 + 1; f2 < fiberCount; f2++ )
      {

        distanceMatrix( f1, f2 ) = 
        distanceMatrix( f2, f1 ) = 
                 ( double )bundle.getCurve3d( f1 ).getDistance( f2,
                                                                _distanceType );

      }

    }

    // computing sum of distances for each row
    gkg::Vector sumOfDistances( fiberCount );
    int32_t f = 0;
    for ( f = 0; f < fiberCount; f++ )
    {
      
      sumOfDistances( f ) = distanceMatrix.getRow( f ).getSumOfValues();
      
    }

    // the centroid index corresponds to the index of the row with the minimum
    // sum of values 
    int32_t centroidIndex = 0;
    _minimumIndexFilter.filter( sumOfDistances, centroidIndex );

    return centroidIndex;

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "int32_t gkg::BundleMapOperatorComputeCentroids< L, Compare >::"
             "getFastCentroidIndex( "
             "const typename gkg::BundleMap< L, Compare >::Bundle& bundle ) "
             "const" );

}


template < class L, class Compare >
inline
int32_t
gkg::BundleMapOperatorComputeCentroids< L, Compare >::getSlowCentroidIndex(
             const typename gkg::BundleMap< L, Compare >::Bundle& bundle ) const
{

  try
  {

    // collecting fiber count
    int32_t fiberCount = bundle.getCurve3dCount();

    // allocating vector for sum of distances to other fiber(s) for every fiber
    gkg::Vector sumOfDistances( fiberCount );
    sumOfDistances.setZero();

    // computing the sum of distance(s) for each fiber
    int32_t f1 = 0;
    int32_t f2 = 0;
    for ( f1 = 0; f1 < fiberCount; f1++ )
    {

      double& sumOfDistancesForF1 = sumOfDistances( f1 );

      for ( f2 = 0; f2 < fiberCount; f2++ )
      {

        if ( f2 != f1 )
        {

          sumOfDistancesForF1 += bundle.getCurve3d( f1 ).getDistance(
                                                                f2,
                                                                _distanceType );

        }

      }

    }

    // the centroid index corresponds to the index of the row with the minimum
    // sum of distances 
    int32_t centroidIndex = 0;
    _minimumIndexFilter.filter( sumOfDistances, centroidIndex );

    return centroidIndex;

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "int32_t gkg::BundleMapOperatorComputeCentroids< L, Compare >::"
             "getSlowCentroidIndex( "
             "const typename gkg::BundleMap< L, Compare >::Bundle& bundle ) "
             "const" );

}


#endif
