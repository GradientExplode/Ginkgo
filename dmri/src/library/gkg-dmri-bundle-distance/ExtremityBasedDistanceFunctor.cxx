#include <gkg-dmri-bundle-distance/ExtremityBasedDistanceFunctor.h>
#include <gkg-dmri-bundle-distance/BundleDistanceFunctorFactory.h>
#include <gkg-core-exception/Exception.h>
#include <algorithm>



gkg::ExtremityBasedDistanceFunctor::
                                 ExtremityBasedDistanceFunctor(
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
  GKG_CATCH( "gkg::ExtremityBasedDistanceFunctor::"
             "ExtremityBasedDistanceFunctor( "
             "const std::vector< double >& scalarParameters )" );

}


gkg::ExtremityBasedDistanceFunctor::
                               ~ExtremityBasedDistanceFunctor()
{
}


float gkg::ExtremityBasedDistanceFunctor::getDistance(
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

        const gkg::Vector3d< float >&
          fiber1Extremity1 = *f1->begin();
        const gkg::Vector3d< float >&
          fiber1Extremity2 = *f1->rbegin();

        const gkg::Vector3d< float >&
          fiber2Extremity1 = *f2->begin();
        const gkg::Vector3d< float >&
          fiber2Extremity2 = *f2->rbegin();

        currentFiberDistance = 0.5 * std::min( 
                            ( fiber2Extremity1 - fiber1Extremity1 ).getNorm() +
                            ( fiber2Extremity2 - fiber1Extremity2 ).getNorm(),
                            ( fiber2Extremity1 - fiber1Extremity2 ).getNorm() +
                            ( fiber2Extremity2 - fiber1Extremity1 ).getNorm() );
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

        const gkg::Vector3d< float >&
          fiber1Extremity1 = *fp1->begin();
        const gkg::Vector3d< float >&
          fiber1Extremity2 = *fp1->rbegin();

        const gkg::Vector3d< float >&
          fiber2Extremity1 = *f2->begin();
        const gkg::Vector3d< float >&
          fiber2Extremity2 = *f2->rbegin();

        currentFiberDistance = 0.5 * std::min( 
                            ( fiber2Extremity1 - fiber1Extremity1 ).getNorm() +
                            ( fiber2Extremity2 - fiber1Extremity2 ).getNorm(),
                            ( fiber2Extremity1 - fiber1Extremity2 ).getNorm() +
                            ( fiber2Extremity2 - fiber1Extremity1 ).getNorm() );


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
  GKG_CATCH( "float gkg::ExtremityBasedDistanceFunctor::"
             "getDistance( "
             "const gkg::Curve3dMap< float >& bundle1, "
             "const gkg::Curve3dMap< float >& bundle2 ) const" );

}


std::string gkg::ExtremityBasedDistanceFunctor::getStaticName()
{

  try
  {

    return "extremity-based-distance";

  }
  GKG_CATCH( "std::string gkg::ExtremityBasedDistanceFunctor::"
             "getStaticName()" );

}


std::string 
gkg::ExtremityBasedDistanceFunctor::getScalarParameterHelp()
{

  try
  {

    return std::string(
      ".    no parameter expected\n" );

  }
  GKG_CATCH( "std::string "
             "gkg::ExtremityBasedDistanceFunctor::"
             "getStringParameterHelp()" );

}


static bool init_ExtremityBasedDistanceFunctorr
                                                       __attribute__((unused)) =
gkg::BundleDistanceFunctorFactory::getInstance().registerBundleDistanceFunctor(
gkg::ExtremityBasedDistanceFunctor::getStaticName(),
&gkg::ExtremityBasedDistanceFunctor::createInstance,
gkg::ExtremityBasedDistanceFunctor::getScalarParameterHelp() );
