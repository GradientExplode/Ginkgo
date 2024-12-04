#include <gkg-dmri-bundle-distance/SumOfSymmetricSumOfClosestPointDistanceFunctor.h>
#include <gkg-dmri-bundle-distance/BundleDistanceFunctorFactory.h>
#include <gkg-core-exception/Exception.h>
#include <algorithm>



gkg::SumOfSymmetricSumOfClosestPointDistanceFunctor::
                                 SumOfSymmetricSumOfClosestPointDistanceFunctor(
                                 const std::vector< double >& scalarParameters )
                                                  : gkg::BundleDistanceFunctor()
{

  try
  {

    // sanity check(s)
    if ( !scalarParameters.empty() )
    {

      throw std::runtime_error(
                       "no scalar parameter expected for this bundle functor" );

    }

  }
  GKG_CATCH( "gkg::SumOfSymmetricSumOfClosestPointDistanceFunctor::"
             "SumOfSymmetricSumOfClosestPointDistanceFunctor( "
             "const std::vector< double >& scalarParameters )" );

}


gkg::SumOfSymmetricSumOfClosestPointDistanceFunctor::
                               ~SumOfSymmetricSumOfClosestPointDistanceFunctor()
{
}


float gkg::SumOfSymmetricSumOfClosestPointDistanceFunctor::getDistance(
                                 const gkg::Curve3dMap< float >& bundle1,
                                 const gkg::Curve3dMap< float >& bundle2 ) const
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // from bundle1 to bundle2
    ////////////////////////////////////////////////////////////////////////////

    float bundle1ToBundle2Distance = 0.0f;
    float currentFiberDistance = 0.0f;
    float minimumFiberDistance = 0.0f;
    gkg::Curve3dMap< float >::const_iterator
      f1 = bundle1.begin(),
      f1e = bundle1.end();
    while ( f1 != f1e )
    {

      minimumFiberDistance = 1e38;
      gkg::Curve3dMap< float >::const_iterator
        f2 = bundle2.begin(),
        f2e = bundle2.end();
      while ( f2 != f2e )
      {

        currentFiberDistance = f1->getDistance( *f2 );
        if ( currentFiberDistance < minimumFiberDistance )
        {

          minimumFiberDistance = currentFiberDistance;

        }
        ++ f2;

      }

      bundle1ToBundle2Distance += minimumFiberDistance;
      
      ++ f1;

    }

    bundle1ToBundle2Distance /= ( float )bundle1.getCurve3dCount();


    ////////////////////////////////////////////////////////////////////////////
    // from bundle2 to bundle1
    ////////////////////////////////////////////////////////////////////////////


    float bundle2ToBundle1Distance = 0.0f;

    gkg::Curve3dMap< float >::const_iterator
      f2 = bundle2.begin(),
      f2e = bundle2.end();
    while ( f2 != f2e )
    {

      minimumFiberDistance = 1e38;
      gkg::Curve3dMap< float >::const_iterator
        fp1 = bundle1.begin(),
        fp1e = bundle1.end();
      while ( fp1 != fp1e )
      {

        currentFiberDistance = f2->getDistance( *fp1 );
        if ( currentFiberDistance < minimumFiberDistance )
        {

          minimumFiberDistance = currentFiberDistance;

        }

        ++ fp1;

      }

      bundle2ToBundle1Distance += minimumFiberDistance;
      
      ++ f2;

    }

    bundle2ToBundle1Distance /= ( float )bundle2.getCurve3dCount();

    return 0.5f * ( bundle1ToBundle2Distance + bundle2ToBundle1Distance );

  }
  GKG_CATCH( "float gkg::SumOfSymmetricSumOfClosestPointDistanceFunctor::"
             "getDistance( "
             "const gkg::Curve3dMap< float >& bundle1, "
             "const gkg::Curve3dMap< float >& bundle2 ) const" );

}


std::string gkg::SumOfSymmetricSumOfClosestPointDistanceFunctor::getStaticName()
{

  try
  {

    return "sum-of-symmetric-sum-of-closest-point-distance";

  }
  GKG_CATCH( "std::string gkg::SumOfSymmetricSumOfClosestPointDistanceFunctor::"
             "getStaticName()" );

}


std::string 
gkg::SumOfSymmetricSumOfClosestPointDistanceFunctor::getScalarParameterHelp()
{

  try
  {

    return std::string(
      ".    no parameter expected\n" );

  }
  GKG_CATCH( "std::string "
             "gkg::SumOfSymmetricSumOfClosestPointDistanceFunctor::"
             "getStringParameterHelp()" );

}


static bool init_SumOfSymmetricSumOfClosestPointDistanceFunctorr
                                                       __attribute__((unused)) =
gkg::BundleDistanceFunctorFactory::getInstance().registerBundleDistanceFunctor(
gkg::SumOfSymmetricSumOfClosestPointDistanceFunctor::getStaticName(),
&gkg::SumOfSymmetricSumOfClosestPointDistanceFunctor::createInstance,
gkg::SumOfSymmetricSumOfClosestPointDistanceFunctor::getScalarParameterHelp() );
