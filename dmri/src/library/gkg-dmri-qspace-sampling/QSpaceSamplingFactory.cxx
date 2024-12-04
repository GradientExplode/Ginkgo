#include <gkg-dmri-qspace-sampling/QSpaceSamplingFactory.h>
#include <gkg-dmri-qspace-sampling/ArbitraryQSpaceSampling.h>
#include <gkg-dmri-qspace-sampling/GkgSingleShellQSpaceSampling.h>
#include <gkg-dmri-qspace-sampling/SmsVb15SingleShellQSpaceSampling.h>
#include <gkg-dmri-qspace-sampling/GehcSingleShellQSpaceSampling.h>
#include <gkg-dmri-qspace-sampling/GehcNewSingleShellQSpaceSampling.h>
#include <gkg-dmri-qspace-sampling/CustomSingleShellQSpaceSampling.h>
#include <gkg-dmri-qspace-sampling/GkgMultipleSameShellQSpaceSampling.h>
#include <gkg-dmri-qspace-sampling/SmsVb15MultipleSameShellQSpaceSampling.h>
#include <gkg-dmri-qspace-sampling/GehcMultipleSameShellQSpaceSampling.h>
#include <gkg-dmri-qspace-sampling/GehcNewMultipleSameShellQSpaceSampling.h>
#include <gkg-dmri-qspace-sampling/GkgMultipleDifferentShellQSpaceSampling.h>
#include <gkg-dmri-qspace-sampling/SmsVb15MultipleDifferentShellQSpaceSampling.h>
#include <gkg-dmri-qspace-sampling/GehcMultipleDifferentShellQSpaceSampling.h>
#include <gkg-dmri-qspace-sampling/GehcNewMultipleDifferentShellQSpaceSampling.h>
#include <gkg-dmri-qspace-sampling/CustomMultipleDifferentShellQSpaceSampling.h>
#include <gkg-dmri-qspace-sampling/CartesianQSpaceSampling.h>
#include <gkg-core-object/GenericObject_i.h>
#include <gkg-core-exception/Exception.h>


gkg::QSpaceSamplingFactory::QSpaceSamplingFactory()
{
}


gkg::QSpaceSamplingFactory::~QSpaceSamplingFactory()
{
}


gkg::QSpaceSampling* 
gkg::QSpaceSamplingFactory::create(
                 const std::vector< std::string >& stringParameters,
                 const std::vector< double >& scalarParameters,
                 const gkg::HomogeneousTransform3d< float >& transform3d,
                 const gkg::Dictionary& gradientCharacteristics ) const
{

  try
  {

    int32_t stringParameterCount = ( int32_t )stringParameters.size();
    int32_t scalarParameterCount = ( int32_t )scalarParameters.size();
    gkg::QSpaceSampling* qSpaceSampling = 0;

    if ( stringParameterCount >= 1 )
    {

      //////////////////////////////////////////////////////////////////////////
      //////////////////////////////////////////////////////////////////////////
      if ( stringParameters[ 0 ] == "spherical" )
      {

        if ( stringParameterCount >= 2 )
        {

          //////////////////////////////////////////////////////////////////////
          if ( stringParameters[ 1 ] == "single-shell" )
          {

            if ( stringParameterCount >= 3 )
            {

              //////////////////////////////////////////////////////////////////
              if ( stringParameters[ 2 ] == "gkg" )
              {

                if ( scalarParameterCount == 2 )
                {

                  qSpaceSampling = new gkg::GkgSingleShellQSpaceSampling(
                                     ( float )scalarParameters[ 0 ],
                                     ( int32_t )( scalarParameters[ 1 ] + 0.5 ),
                                     transform3d,
                                     gradientCharacteristics );

                }
                else
                {

                  throw std::runtime_error(
                                      "invalid vector of scalar parameter(s)" );

                }

              }
              //////////////////////////////////////////////////////////////////
              else if ( stringParameters[ 2 ] == "smsvb15" )
              {

                if ( scalarParameterCount == 2 )
                {

                  qSpaceSampling = new gkg::SmsVb15SingleShellQSpaceSampling(
                                     ( float )scalarParameters[ 0 ],
                                     ( int32_t )( scalarParameters[ 1 ] + 0.5 ),
                                     transform3d,
                                     gradientCharacteristics );

                }
                else
                {

                  throw std::runtime_error(
                                      "invalid vector of scalar parameter(s)" );

                }

              }
              //////////////////////////////////////////////////////////////////
              else if ( stringParameters[ 2 ] == "gehc" )
              {

                if ( scalarParameterCount == 2 )
                {

                  qSpaceSampling = new gkg::GehcSingleShellQSpaceSampling(
                                     ( float )scalarParameters[ 0 ],
                                     ( int32_t )( scalarParameters[ 1 ] + 0.5 ),
                                     transform3d,
                                     gradientCharacteristics );

                }
                else
                {

                  throw std::runtime_error(
                                      "invalid vector of scalar parameter(s)" );

                }

              }
              //////////////////////////////////////////////////////////////////
              else if ( stringParameters[ 2 ] == "gehc-new" )
              {

                if ( scalarParameterCount == 2 )
                {

                  qSpaceSampling = new gkg::GehcNewSingleShellQSpaceSampling(
                                     ( float )scalarParameters[ 0 ],
                                     ( int32_t )( scalarParameters[ 1 ] + 0.5 ),
                                     transform3d,
                                     gradientCharacteristics );

                }
                else
                {

                  throw std::runtime_error(
                                      "invalid vector of scalar parameter(s)" );

                }

              }
              //////////////////////////////////////////////////////////////////
              else if ( stringParameters[ 2 ] == "custom" )
              {

                if ( scalarParameterCount == 0 )
                {

                  qSpaceSampling = new gkg::CustomSingleShellQSpaceSampling(
                                     stringParameters[ 3 ],
                                     stringParameters[ 4 ],
                                     transform3d,
                                     gradientCharacteristics );

                }
                else
                {

                  throw std::runtime_error(
                                      "invalid vector of scalar parameter(s)" );

                }

              }
              else
              {

                throw std::runtime_error( "bad parameter" );

              }

            }
            else
            {

              throw std::runtime_error(
                                      "invalid vector of string parameter(s)" );

            }

          }
          //////////////////////////////////////////////////////////////////////
          else if ( stringParameters[ 1 ] == "multiple-shell" )
          {

            if ( stringParameterCount >= 3 )
            {

              //////////////////////////////////////////////////////////////////
              if ( stringParameters[ 2 ] == "same-orientation-set" )
              {

                if ( stringParameterCount >= 4 )
                {

                  //////////////////////////////////////////////////////////////
                  if ( stringParameters[ 3 ] == "gkg" )
                  {

                    int32_t shellCount = ( int32_t )( scalarParameters[ 0 ] +
                                                      0.5 );
                    std::vector< float > shellBValues( shellCount );
                    int32_t s = 0;
                    for ( s = 0; s < shellCount; s++ )
                    {

                      shellBValues[ s ] = ( float )scalarParameters[ s + 1 ];

                    }

                    if ( scalarParameterCount == shellCount + 2 )
                    {

                      qSpaceSampling =
                        new gkg::GkgMultipleSameShellQSpaceSampling(
                               shellBValues,
                               ( int32_t )( scalarParameters[ shellCount + 1 ] +
                                            0.5 ),
                               transform3d,
                               gradientCharacteristics );

                    }
                    else
                    {

                      throw std::runtime_error(
                                          "invalid vector of scalar parameter(s)" );

                    }

                  }
                  //////////////////////////////////////////////////////////////
                  else if ( stringParameters[ 3 ] == "smsvb15" )
                  {

                    int32_t shellCount = ( int32_t )( scalarParameters[ 0 ] +
                                                      0.5 );
                    std::vector< float > shellBValues( shellCount );
                    int32_t s = 0;
                    for ( s = 0; s < shellCount; s++ )
                    {

                      shellBValues[ s ] = ( float )scalarParameters[ s + 1 ];

                    }

                    if ( scalarParameterCount == shellCount + 2 )
                    {

                      qSpaceSampling =
                        new gkg::SmsVb15MultipleSameShellQSpaceSampling(
                               shellBValues,
                               ( int32_t )( scalarParameters[ shellCount + 1 ] +
                                            0.5 ),
                               transform3d,
                               gradientCharacteristics );

                    }
                    else
                    {

                      throw std::runtime_error(
                                          "invalid vector of scalar parameter(s)" );

                    }

                  }
                  //////////////////////////////////////////////////////////////
                  else if ( stringParameters[ 3 ] == "gehc" )
                  {

                    int32_t shellCount = ( int32_t )( scalarParameters[ 0 ] +
                                                      0.5 );
                    std::vector< float > shellBValues( shellCount );
                    int32_t s = 0;
                    for ( s = 0; s < shellCount; s++ )
                    {

                      shellBValues[ s ] = ( float )scalarParameters[ s + 1 ];

                    }

                    if ( scalarParameterCount == shellCount + 2 )
                    {

                      qSpaceSampling =
                        new gkg::GehcMultipleSameShellQSpaceSampling(
                               shellBValues,
                               ( int32_t )( scalarParameters[ shellCount + 1 ] +
                                            0.5 ),
                               transform3d,
                               gradientCharacteristics );

                    }
                    else
                    {

                      throw std::runtime_error(
                                          "invalid vector of scalar parameter(s)" );

                    }

                  }
                  //////////////////////////////////////////////////////////////
                  else if ( stringParameters[ 3 ] == "gehc-new" )
                  {

                    int32_t shellCount = ( int32_t )( scalarParameters[ 0 ] +
                                                      0.5 );
                    std::vector< float > shellBValues( shellCount );
                    int32_t s = 0;
                    for ( s = 0; s < shellCount; s++ )
                    {

                      shellBValues[ s ] = ( float )scalarParameters[ s + 1 ];

                    }

                    if ( scalarParameterCount == shellCount + 2 )
                    {

                      qSpaceSampling =
                        new gkg::GehcNewMultipleSameShellQSpaceSampling(
                               shellBValues,
                               ( int32_t )( scalarParameters[ shellCount + 1 ] +
                                            0.5 ),
                               transform3d,
                               gradientCharacteristics );

                    }
                    else
                    {

                      throw std::runtime_error(
                                          "invalid vector of scalar parameter(s)" );

                    }

                  }
                  else
                  {

                    throw std::runtime_error( "bad parameter" );

                  }

                }
                else
                {

                  throw std::runtime_error(
                                      "invalid vector of string parameter(s)" );

                }

              }
              //////////////////////////////////////////////////////////////////
              else if ( stringParameters[ 2 ] == "different-orientation-sets" )
              {

                if ( stringParameterCount >= 4 )
                {

                  //////////////////////////////////////////////////////////////
                  if ( stringParameters[ 3 ] == "gkg" )
                  {

                    int32_t shellCount = ( int32_t )( scalarParameters[ 0 ] +
                                                      0.5 );
                    std::vector< float > shellBValues( shellCount );
                    std::vector< int32_t > orientationCounts( shellCount );
                    int32_t s = 0;
                    for ( s = 0; s < shellCount; s++ )
                    {

                      shellBValues[ s ] = ( float )scalarParameters[ s + 1 ];
                      orientationCounts[ s ] =
                           ( int32_t )( scalarParameters[ shellCount + s + 1 ] +
                                        0.5 );

                    }

                    if ( scalarParameterCount == 2 * shellCount + 1 )
                    {

                      qSpaceSampling =
                        new gkg::GkgMultipleDifferentShellQSpaceSampling(
                               shellBValues,
                               orientationCounts,
                               transform3d,
                               gradientCharacteristics );

                    }
                    else
                    {

                      throw std::runtime_error(
                                          "invalid vector of scalar parameter(s)" );

                    }

                  }
                  //////////////////////////////////////////////////////////////
                  else if ( stringParameters[ 3 ] == "smsvb15" )
                  {

                    int32_t shellCount = ( int32_t )( scalarParameters[ 0 ] +
                                                      0.5 );
                    std::vector< float > shellBValues( shellCount );
                    std::vector< int32_t > orientationCounts( shellCount );
                    int32_t s = 0;
                    for ( s = 0; s < shellCount; s++ )
                    {

                      shellBValues[ s ] = ( float )scalarParameters[ s + 1 ];
                      orientationCounts[ s ] =
                           ( int32_t )( scalarParameters[ shellCount + s + 1 ] +
                                        0.5 );

                    }

                    if ( scalarParameterCount == 2 * shellCount + 1 )
                    {

                      qSpaceSampling =
                        new gkg::SmsVb15MultipleDifferentShellQSpaceSampling(
                               shellBValues,
                               orientationCounts,
                               transform3d,
                               gradientCharacteristics );

                    }
                    else
                    {

                      throw std::runtime_error(
                                          "invalid vector of scalar parameter(s)" );

                    }

                  }
                  //////////////////////////////////////////////////////////////
                  else if ( stringParameters[ 3 ] == "gehc" )
                  {

                    int32_t shellCount = ( int32_t )( scalarParameters[ 0 ] +
                                                      0.5 );
                    std::vector< float > shellBValues( shellCount );
                    std::vector< int32_t > orientationCounts( shellCount );
                    int32_t s = 0;
                    for ( s = 0; s < shellCount; s++ )
                    {

                      shellBValues[ s ] = ( float )scalarParameters[ s + 1 ];
                      orientationCounts[ s ] =
                           ( int32_t )( scalarParameters[ shellCount + s + 1 ] +
                                        0.5 );

                    }

                    if ( scalarParameterCount == 2 * shellCount + 1 )
                    {

                      qSpaceSampling =
                        new gkg::GehcMultipleDifferentShellQSpaceSampling(
                               shellBValues,
                               orientationCounts,
                               transform3d,
                               gradientCharacteristics );

                    }
                    else
                    {

                      throw std::runtime_error(
                                          "invalid vector of scalar parameter(s)" );

                    }

                  }
                  //////////////////////////////////////////////////////////////
                  else if ( stringParameters[ 3 ] == "gehc-new" )
                  {

                    int32_t shellCount = ( int32_t )( scalarParameters[ 0 ] +
                                                      0.5 );
                    std::vector< float > shellBValues( shellCount );
                    std::vector< int32_t > orientationCounts( shellCount );
                    int32_t s = 0;
                    for ( s = 0; s < shellCount; s++ )
                    {

                      shellBValues[ s ] = ( float )scalarParameters[ s + 1 ];
                      orientationCounts[ s ] =
                           ( int32_t )( scalarParameters[ shellCount + s + 1 ] +
                                        0.5 );

                    }

                    if ( scalarParameterCount == 2 * shellCount + 1 )
                    {

                      qSpaceSampling =
                        new gkg::GehcNewMultipleDifferentShellQSpaceSampling(
                               shellBValues,
                               orientationCounts,
                               transform3d,
                               gradientCharacteristics );

                    }
                    else
                    {

                      throw std::runtime_error(
                                          "invalid vector of scalar parameter(s)" );

                    }

                  }
                  //////////////////////////////////////////////////////////////
                  else if ( stringParameters[ 3 ] == "custom" )
                  {

                    if ( stringParameterCount != 6 )
                    {

                      throw std::runtime_error(
                                      "invalid vector of string parameter(s)" );

                    }

                    if ( scalarParameterCount == 1 )
                    {

                      qSpaceSampling =
                        new gkg::CustomMultipleDifferentShellQSpaceSampling(
                                         stringParameters[ 4 ],
                                         stringParameters[ 5 ],
                                         ( float )scalarParameters[ 0 ],
                                         transform3d,
                                         gradientCharacteristics );

                    }
                    else
                    {

                      throw std::runtime_error(
                                      "invalid vector of scalar parameter(s)" );

                    }

                  }
                  else
                  {

                    throw std::runtime_error( "bad parameter" );

                  }

                }
                else
                {

                  throw std::runtime_error(
                                      "invalid vector of string parameter(s)" );

                }

              }
              else
              {

                throw std::runtime_error( "bad parameter" );

              }

            }
            else
            {

              throw std::runtime_error(
                                      "invalid vector of string parameter(s)" );

            }

          }
          //////////////////////////////////////////////////////////////////////
          else
          {

            throw std::runtime_error( "bad parameter" );

          }

        }
        else
        {

          throw std::runtime_error( "invalid vector of string parameter(s)" );

        }

      }
      //////////////////////////////////////////////////////////////////////////
      //////////////////////////////////////////////////////////////////////////
      else if ( stringParameters[ 0 ] == "cartesian" )
      {

        if ( scalarParameterCount == 2 )
        {

          qSpaceSampling =
            new gkg::CartesianQSpaceSampling(
                                     ( float )scalarParameters[ 0 ],
                                     ( int32_t )( scalarParameters[ 1 ] + 0.5 ),
                                     transform3d,
                                     gradientCharacteristics );

        }
        else
        {

          throw std::runtime_error(
                              "invalid vector of scalar parameter(s)" );

        }


      }
      //////////////////////////////////////////////////////////////////////////
      //////////////////////////////////////////////////////////////////////////
      else if ( stringParameters[ 0 ] == "arbitrary" )
      {

        if ( stringParameterCount != 3 )
        {

          throw std::runtime_error( "invalid vector of string parameter(s)" );

        }

        if ( scalarParameterCount == 0 )
        {

          qSpaceSampling = new gkg::ArbitraryQSpaceSampling(
                                                      stringParameters[ 1 ],
                                                      stringParameters[ 2 ],
                                                      transform3d,
                                                      gradientCharacteristics );

        }
        else
        {

          throw std::runtime_error( "invalid vector of scalar parameter(s)" );

        }

      }
      //////////////////////////////////////////////////////////////////////////
      //////////////////////////////////////////////////////////////////////////
      else
      {

        throw std::runtime_error( "invalid Q-space sampling type" );

      }

    }
    else
    {

      throw std::runtime_error( "invalid vector of string parameter(s)" );

    }

    return qSpaceSampling;

  }
  GKG_CATCH( "gkg::QSpaceSampling* "
             "gkg::QSpaceSamplingFactory::create( "
             "const std::vector< std::string >& stringParameters, "
             "const std::vector< double >& scalarParameters, "
             "const gkg::Transform3d< float >& transform3d, "
             "const Dictionary& gradientCharacteristics ) const" );

}


gkg::QSpaceSampling* 
gkg::QSpaceSamplingFactory::create( gkg::HeaderedObject& headeredObject ) const
{

  try
  {

    std::string diffusion_sensitization_type;
    headeredObject.getHeader().getAttribute( "diffusion_sensitization_type",
                                             diffusion_sensitization_type );

    gkg::Dictionary diffusion_sensitization_parameters;
    headeredObject.getHeader().getAttribute(
                                           "diffusion_sensitization_parameters",
                                           diffusion_sensitization_parameters );

    gkg::QSpaceSampling* qSpaceSampling = 0;

    gkg::GenericObjectList motion_rotation3ds;


    if ( diffusion_sensitization_type == "spherical_single-shell_gkg" )
    {

      float bValue =
        ( float )diffusion_sensitization_parameters[ "b-value" ]->getScalar();
      int32_t orientationCount =
        ( int32_t )( diffusion_sensitization_parameters[ "orientation-count"
                                                       ]->getScalar() + 0.5 );

      gkg::GenericObjectList homogeneous_transform3d =
        diffusion_sensitization_parameters[ "homogeneous-transform3d"
                                        ]->getValue< gkg::GenericObjectList >();

      motion_rotation3ds =
        diffusion_sensitization_parameters[ "motion-rotation3ds"
                                        ]->getValue< gkg::GenericObjectList >();

      int32_t coefficientCount = ( int32_t )homogeneous_transform3d.size();
      std::vector< float > coefficients( coefficientCount );
      int32_t c;
      for ( c = 0; c < coefficientCount; c++ )
      {

        coefficients[ c ] = ( float )homogeneous_transform3d[ c ]->getScalar();

      }
      gkg::HomogeneousTransform3d< float > transform3d( coefficients );

      gkg::Dictionary gradientCharacteristics;
      if ( diffusion_sensitization_parameters.find( "gradient-characteristics" )
           != diffusion_sensitization_parameters.end() )
      {

        gradientCharacteristics =
          diffusion_sensitization_parameters[ "gradient-characteristics"
                                        ]->getValue< gkg::Dictionary >();

      }
      else
      {

        gradientCharacteristics[ "type" ] = std::string( "unknown" );

      }

      qSpaceSampling = new gkg::GkgSingleShellQSpaceSampling(
                                                     bValue,
                                                     orientationCount,
                                                     transform3d,
                                                     gradientCharacteristics );

    }
    else if ( diffusion_sensitization_type == "spherical_single-shell_smsvb15" )
    {

      float bValue =
        ( float )diffusion_sensitization_parameters[ "b-value" ]->getScalar();
      int32_t orientationCount =
        ( int32_t )( diffusion_sensitization_parameters[ "orientation-count"
                                                       ]->getScalar() + 0.5 );

      gkg::GenericObjectList homogeneous_transform3d =
        diffusion_sensitization_parameters[ "homogeneous-transform3d"
                                        ]->getValue< gkg::GenericObjectList >();

      motion_rotation3ds =
        diffusion_sensitization_parameters[ "motion-rotation3ds"
                                        ]->getValue< gkg::GenericObjectList >();

      int32_t coefficientCount = ( int32_t )homogeneous_transform3d.size();
      std::vector< float > coefficients( coefficientCount );
      int32_t c;
      for ( c = 0; c < coefficientCount; c++ )
      {

        coefficients[ c ] = ( float )homogeneous_transform3d[ c ]->getScalar();

      }
      gkg::HomogeneousTransform3d< float > transform3d( coefficients );

      gkg::Dictionary gradientCharacteristics;
      if ( diffusion_sensitization_parameters.find( "gradient-characteristics" )
           != diffusion_sensitization_parameters.end() )
      {

        gradientCharacteristics =
          diffusion_sensitization_parameters[ "gradient-characteristics"
                                        ]->getValue< gkg::Dictionary >();

      }
      else
      {

        gradientCharacteristics[ "type" ] = std::string( "unknown" );

      }

      qSpaceSampling = new gkg::SmsVb15SingleShellQSpaceSampling(
                                                      bValue,
                                                      orientationCount,
                                                      transform3d,
                                                      gradientCharacteristics );

    }
    else if ( diffusion_sensitization_type == "spherical_single-shell_gehc" )
    {

      float bValue =
        ( float )diffusion_sensitization_parameters[ "b-value" ]->getScalar();
      int32_t orientationCount =
        ( int32_t )( diffusion_sensitization_parameters[ "orientation-count"
                                                       ]->getScalar() + 0.5 );

      gkg::GenericObjectList homogeneous_transform3d =
        diffusion_sensitization_parameters[ "homogeneous-transform3d"
                                        ]->getValue< gkg::GenericObjectList >();

      motion_rotation3ds =
        diffusion_sensitization_parameters[ "motion-rotation3ds"
                                        ]->getValue< gkg::GenericObjectList >();

      int32_t coefficientCount = ( int32_t )homogeneous_transform3d.size();
      std::vector< float > coefficients( coefficientCount );
      int32_t c;
      for ( c = 0; c < coefficientCount; c++ )
      {

        coefficients[ c ] = ( float )homogeneous_transform3d[ c ]->getScalar();

      }
      gkg::HomogeneousTransform3d< float > transform3d( coefficients );

      gkg::Dictionary gradientCharacteristics;
      if ( diffusion_sensitization_parameters.find( "gradient-characteristics" )
           != diffusion_sensitization_parameters.end() )
      {

        gradientCharacteristics =
          diffusion_sensitization_parameters[ "gradient-characteristics"
                                        ]->getValue< gkg::Dictionary >();

      }
      else
      {

        gradientCharacteristics[ "type" ] = std::string( "unknown" );

      }

      qSpaceSampling = new gkg::GehcSingleShellQSpaceSampling(
                                                      bValue,
                                                      orientationCount,
                                                      transform3d,
                                                      gradientCharacteristics );

    }
    else if ( diffusion_sensitization_type == "spherical_single-shell_gehc-new" )
    {

      float bValue =
        ( float )diffusion_sensitization_parameters[ "b-value" ]->getScalar();
      int32_t orientationCount =
        ( int32_t )( diffusion_sensitization_parameters[ "orientation-count"
                                                       ]->getScalar() + 0.5 );

      gkg::GenericObjectList homogeneous_transform3d =
        diffusion_sensitization_parameters[ "homogeneous-transform3d"
                                        ]->getValue< gkg::GenericObjectList >();

      motion_rotation3ds =
        diffusion_sensitization_parameters[ "motion-rotation3ds"
                                        ]->getValue< gkg::GenericObjectList >();

      int32_t coefficientCount = ( int32_t )homogeneous_transform3d.size();
      std::vector< float > coefficients( coefficientCount );
      int32_t c;
      for ( c = 0; c < coefficientCount; c++ )
      {

        coefficients[ c ] = ( float )homogeneous_transform3d[ c ]->getScalar();

      }
      gkg::HomogeneousTransform3d< float > transform3d( coefficients );

      gkg::Dictionary gradientCharacteristics;
      if ( diffusion_sensitization_parameters.find( "gradient-characteristics" )
           != diffusion_sensitization_parameters.end() )
      {

        gradientCharacteristics =
          diffusion_sensitization_parameters[ "gradient-characteristics"
                                        ]->getValue< gkg::Dictionary >();

      }
      else
      {

        gradientCharacteristics[ "type" ] = std::string( "unknown" );

      }

      qSpaceSampling = new gkg::GehcNewSingleShellQSpaceSampling(
                                                      bValue,
                                                      orientationCount,
                                                      transform3d,
                                                      gradientCharacteristics );

    }
    else if ( diffusion_sensitization_type == "spherical_single-shell_custom" )
    {

      int32_t orientationCount =
        ( int32_t )( diffusion_sensitization_parameters[ "orientation-count"
                                                       ]->getScalar() + 0.5 );

      gkg::GenericObjectList homogeneous_transform3d =
        diffusion_sensitization_parameters[ "homogeneous-transform3d"
                                        ]->getValue< gkg::GenericObjectList >();

      motion_rotation3ds =
        diffusion_sensitization_parameters[ "motion-rotation3ds"
                                        ]->getValue< gkg::GenericObjectList >();

      int32_t coefficientCount = ( int32_t )homogeneous_transform3d.size();
      std::vector< float > coefficients( coefficientCount );
      int32_t c;
      for ( c = 0; c < coefficientCount; c++ )
      {

        coefficients[ c ] = ( float )homogeneous_transform3d[ c ]->getScalar();

      }
      gkg::HomogeneousTransform3d< float > transform3d( coefficients );

      gkg::GenericObjectList genericObjectListBValues;
      headeredObject.getHeader().getAttribute( "bvalues",
                                               genericObjectListBValues );

      std::vector<  float > bValues( orientationCount );
      int32_t o;
      for ( o = 0; o < orientationCount; o++ )
      {

        bValues[ o ] = ( float )genericObjectListBValues[ o ]->getScalar();

      }

      gkg::GenericObjectList
        genericObjectListOrientations =
          diffusion_sensitization_parameters[ "orientations"
                                        ]->getValue< gkg::GenericObjectList >();
      std::vector< gkg::Vector3d< float > > orientations( orientationCount );
      gkg::Vector3d< float > orientation;
      for ( o = 0; o < orientationCount; o++ )
      {

        gkg::GenericObjectList tmp =
          genericObjectListOrientations[ o ]->getValue< gkg::GenericObjectList
                                                      >();
        orientation.x = ( float )tmp[ 0 ]->getScalar();
        orientation.y = ( float )tmp[ 1 ]->getScalar();
        orientation.z = ( float )tmp[ 2 ]->getScalar();
        orientations[ o ] = orientation;
        //transform3d.getInverse( orientation, orientations[ o ] );

      }

      gkg::Dictionary gradientCharacteristics;
      if ( diffusion_sensitization_parameters.find( "gradient-characteristics" )
           != diffusion_sensitization_parameters.end() )
      {

        gradientCharacteristics =
          diffusion_sensitization_parameters[ "gradient-characteristics"
                                        ]->getValue< gkg::Dictionary >();

      }
      else
      {

        gradientCharacteristics[ "type" ] = std::string( "unknown" );

      }

      qSpaceSampling = new gkg::CustomSingleShellQSpaceSampling(
                                                      bValues,
                                                      orientations,
                                                      transform3d,
                                                      gradientCharacteristics );

    }
    else if ( diffusion_sensitization_type ==
              "spherical_multiple-shell_same-orientation-set_gkg" )
    {

      gkg::GenericObjectList
        golShellBValues = diffusion_sensitization_parameters[ "b-values" ]->
                            getValue< gkg::GenericObjectList >();
      int32_t shellCount = ( int32_t )golShellBValues.size();
      std::vector< float > shellBValues( shellCount );

      int32_t s = 0;
      for ( s = 0; s < shellCount; s++ )
      {

        shellBValues[ s ] = ( float )golShellBValues[ s ]->getScalar();

      }

      int32_t orientationCount =
        ( int32_t )( diffusion_sensitization_parameters[ "orientation-count"
                                                       ]->getScalar() + 0.5 );

      gkg::GenericObjectList homogeneous_transform3d =
        diffusion_sensitization_parameters[ "homogeneous-transform3d"
                                        ]->getValue< gkg::GenericObjectList >();

      motion_rotation3ds =
        diffusion_sensitization_parameters[ "motion-rotation3ds"
                                        ]->getValue< gkg::GenericObjectList >();

      int32_t coefficientCount = ( int32_t )homogeneous_transform3d.size();
      std::vector< float > coefficients( coefficientCount );
      int32_t c;
      for ( c = 0; c < coefficientCount; c++ )
      {

        coefficients[ c ] = ( float )homogeneous_transform3d[ c ]->getScalar();

      }
      gkg::HomogeneousTransform3d< float > transform3d( coefficients );

      gkg::Dictionary gradientCharacteristics;
      if ( diffusion_sensitization_parameters.find( "gradient-characteristics" )
           != diffusion_sensitization_parameters.end() )
      {

        gradientCharacteristics =
          diffusion_sensitization_parameters[ "gradient-characteristics"
                                        ]->getValue< gkg::Dictionary >();

      }
      else
      {

        gradientCharacteristics[ "type" ] = std::string( "unknown" );

      }

      qSpaceSampling = new gkg::GkgMultipleSameShellQSpaceSampling(
                                                      shellBValues,
                                                      orientationCount,
                                                      transform3d,
                                                      gradientCharacteristics );

    }
    else if ( diffusion_sensitization_type ==
              "spherical_multiple-shell_same-orientation-set_smsvb15" )
    {

      gkg::GenericObjectList
        golShellBValues = diffusion_sensitization_parameters[ "b-values" ]->
                            getValue< gkg::GenericObjectList >();
      int32_t shellCount = ( int32_t )golShellBValues.size();
      std::vector< float > shellBValues( shellCount );

      int32_t s = 0;
      for ( s = 0; s < shellCount; s++ )
      {

        shellBValues[ s ] = ( float )golShellBValues[ s ]->getScalar();

      }

      int32_t orientationCount =
        ( int32_t )( diffusion_sensitization_parameters[ "orientation-count"
                                                       ]->getScalar() + 0.5 );

      gkg::GenericObjectList homogeneous_transform3d =
        diffusion_sensitization_parameters[ "homogeneous-transform3d"
                                        ]->getValue< gkg::GenericObjectList >();

      motion_rotation3ds =
        diffusion_sensitization_parameters[ "motion-rotation3ds"
                                        ]->getValue< gkg::GenericObjectList >();

      int32_t coefficientCount = ( int32_t )homogeneous_transform3d.size();
      std::vector< float > coefficients( coefficientCount );
      int32_t c;
      for ( c = 0; c < coefficientCount; c++ )
      {

        coefficients[ c ] = ( float )homogeneous_transform3d[ c ]->getScalar();

      }
      gkg::HomogeneousTransform3d< float > transform3d( coefficients );

      gkg::Dictionary gradientCharacteristics;
      if ( diffusion_sensitization_parameters.find( "gradient-characteristics" )
           != diffusion_sensitization_parameters.end() )
      {

        gradientCharacteristics =
          diffusion_sensitization_parameters[ "gradient-characteristics"
                                        ]->getValue< gkg::Dictionary >();

      }
      else
      {

        gradientCharacteristics[ "type" ] = std::string( "unknown" );

      }

      qSpaceSampling = new gkg::SmsVb15MultipleSameShellQSpaceSampling(
                                                      shellBValues,
                                                      orientationCount,
                                                      transform3d,
                                                      gradientCharacteristics );

    }
    else if ( diffusion_sensitization_type ==
              "spherical_multiple-shell_same-orientation-set_gehc" )
    {

      gkg::GenericObjectList
        golShellBValues = diffusion_sensitization_parameters[ "b-values" ]->
                            getValue< gkg::GenericObjectList >();
      int32_t shellCount = ( int32_t )golShellBValues.size();
      std::vector< float > shellBValues( shellCount );

      int32_t s = 0;
      for ( s = 0; s < shellCount; s++ )
      {

        shellBValues[ s ] = ( float )golShellBValues[ s ]->getScalar();

      }

      int32_t orientationCount =
        ( int32_t )( diffusion_sensitization_parameters[ "orientation-count"
                                                       ]->getScalar() + 0.5 );

      gkg::GenericObjectList homogeneous_transform3d =
        diffusion_sensitization_parameters[ "homogeneous-transform3d"
                                        ]->getValue< gkg::GenericObjectList >();

      motion_rotation3ds =
        diffusion_sensitization_parameters[ "motion-rotation3ds"
                                        ]->getValue< gkg::GenericObjectList >();

      int32_t coefficientCount = ( int32_t )homogeneous_transform3d.size();
      std::vector< float > coefficients( coefficientCount );
      int32_t c;
      for ( c = 0; c < coefficientCount; c++ )
      {

        coefficients[ c ] = ( float )homogeneous_transform3d[ c ]->getScalar();

      }
      gkg::HomogeneousTransform3d< float > transform3d( coefficients );

      gkg::Dictionary gradientCharacteristics;
      if ( diffusion_sensitization_parameters.find( "gradient-characteristics" )
           != diffusion_sensitization_parameters.end() )
      {

        gradientCharacteristics =
          diffusion_sensitization_parameters[ "gradient-characteristics"
                                        ]->getValue< gkg::Dictionary >();

      }
      else
      {

        gradientCharacteristics[ "type" ] = std::string( "unknown" );

      }

      qSpaceSampling = new gkg::GehcMultipleSameShellQSpaceSampling(
                                                      shellBValues,
                                                      orientationCount,
                                                      transform3d,
                                                      gradientCharacteristics );

    }
    else if ( diffusion_sensitization_type ==
              "spherical_multiple-shell_same-orientation-set_gehc-new" )
    {

      gkg::GenericObjectList
        golShellBValues = diffusion_sensitization_parameters[ "b-values" ]->
                            getValue< gkg::GenericObjectList >();
      int32_t shellCount = ( int32_t )golShellBValues.size();
      std::vector< float > shellBValues( shellCount );

      int32_t s = 0;
      for ( s = 0; s < shellCount; s++ )
      {

        shellBValues[ s ] = ( float )golShellBValues[ s ]->getScalar();

      }

      int32_t orientationCount =
        ( int32_t )( diffusion_sensitization_parameters[ "orientation-count"
                                                       ]->getScalar() + 0.5 );

      gkg::GenericObjectList homogeneous_transform3d =
        diffusion_sensitization_parameters[ "homogeneous-transform3d"
                                        ]->getValue< gkg::GenericObjectList >();

      motion_rotation3ds =
        diffusion_sensitization_parameters[ "motion-rotation3ds"
                                        ]->getValue< gkg::GenericObjectList >();

      int32_t coefficientCount = ( int32_t )homogeneous_transform3d.size();
      std::vector< float > coefficients( coefficientCount );
      int32_t c;
      for ( c = 0; c < coefficientCount; c++ )
      {

        coefficients[ c ] = ( float )homogeneous_transform3d[ c ]->getScalar();

      }
      gkg::HomogeneousTransform3d< float > transform3d( coefficients );

      gkg::Dictionary gradientCharacteristics;
      if ( diffusion_sensitization_parameters.find( "gradient-characteristics" )
           != diffusion_sensitization_parameters.end() )
      {

        gradientCharacteristics =
          diffusion_sensitization_parameters[ "gradient-characteristics"
                                        ]->getValue< gkg::Dictionary >();

      }
      else
      {

        gradientCharacteristics[ "type" ] = std::string( "unknown" );

      }

      qSpaceSampling = new gkg::GehcNewMultipleSameShellQSpaceSampling(
                                                      shellBValues,
                                                      orientationCount,
                                                      transform3d,
                                                      gradientCharacteristics );

    }
    else if ( diffusion_sensitization_type ==
              "spherical_multiple-shell_different-orientation-sets_gkg" )
    {

      gkg::GenericObjectList
        golShellBValues = diffusion_sensitization_parameters[ "b-values" ]->
                            getValue< gkg::GenericObjectList >();
      gkg::GenericObjectList
        golOrientationCounts = diffusion_sensitization_parameters[
                                                        "orientation-counts" ]->
                            getValue< gkg::GenericObjectList >();
      int32_t shellCount = ( int32_t )golShellBValues.size();
      std::vector< float > shellBValues( shellCount );
      std::vector< int32_t > orientationCounts( shellCount );

      int32_t s = 0;
      for ( s = 0; s < shellCount; s++ )
      {

        shellBValues[ s ] = ( float )golShellBValues[ s ]->getScalar();
        orientationCounts[ s ] =
                    ( int32_t )( golOrientationCounts[ s ]->getScalar() + 0.5 );

      }

      gkg::GenericObjectList homogeneous_transform3d =
        diffusion_sensitization_parameters[ "homogeneous-transform3d"
                                        ]->getValue< gkg::GenericObjectList >();

      motion_rotation3ds =
        diffusion_sensitization_parameters[ "motion-rotation3ds"
                                        ]->getValue< gkg::GenericObjectList >();

      int32_t coefficientCount = ( int32_t )homogeneous_transform3d.size();
      std::vector< float > coefficients( coefficientCount );
      int32_t c;
      for ( c = 0; c < coefficientCount; c++ )
      {

        coefficients[ c ] = ( float )homogeneous_transform3d[ c ]->getScalar();

      }
      gkg::HomogeneousTransform3d< float > transform3d( coefficients );

      gkg::Dictionary gradientCharacteristics;
      if ( diffusion_sensitization_parameters.find( "gradient-characteristics" )
           != diffusion_sensitization_parameters.end() )
      {

        gradientCharacteristics =
          diffusion_sensitization_parameters[ "gradient-characteristics"
                                        ]->getValue< gkg::Dictionary >();

      }
      else
      {

        gradientCharacteristics[ "type" ] = std::string( "unknown" );

      }

      qSpaceSampling = new gkg::GkgMultipleDifferentShellQSpaceSampling(
                                                      shellBValues,
                                                      orientationCounts,
                                                      transform3d,
                                                      gradientCharacteristics );

    }
    else if ( diffusion_sensitization_type ==
              "spherical_multiple-shell_different-orientation-sets_smsvb15" )
    {

      gkg::GenericObjectList
        golShellBValues = diffusion_sensitization_parameters[ "b-values" ]->
                            getValue< gkg::GenericObjectList >();
      gkg::GenericObjectList
        golOrientationCounts = diffusion_sensitization_parameters[
                                                        "orientation-counts" ]->
                            getValue< gkg::GenericObjectList >();
      int32_t shellCount = ( int32_t )golShellBValues.size();
      std::vector< float > shellBValues( shellCount );
      std::vector< int32_t > orientationCounts( shellCount );

      int32_t s = 0;
      for ( s = 0; s < shellCount; s++ )
      {

        shellBValues[ s ] = ( float )golShellBValues[ s ]->getScalar();
        orientationCounts[ s ] =
                    ( int32_t )( golOrientationCounts[ s ]->getScalar() + 0.5 );

      }

      gkg::GenericObjectList homogeneous_transform3d =
        diffusion_sensitization_parameters[ "homogeneous-transform3d"
                                        ]->getValue< gkg::GenericObjectList >();

      motion_rotation3ds =
        diffusion_sensitization_parameters[ "motion-rotation3ds"
                                        ]->getValue< gkg::GenericObjectList >();

      int32_t coefficientCount = ( int32_t )homogeneous_transform3d.size();
      std::vector< float > coefficients( coefficientCount );
      int32_t c;
      for ( c = 0; c < coefficientCount; c++ )
      {

        coefficients[ c ] = ( float )homogeneous_transform3d[ c ]->getScalar();

      }
      gkg::HomogeneousTransform3d< float > transform3d( coefficients );

      gkg::Dictionary gradientCharacteristics;
      if ( diffusion_sensitization_parameters.find( "gradient-characteristics" )
           != diffusion_sensitization_parameters.end() )
      {

        gradientCharacteristics =
          diffusion_sensitization_parameters[ "gradient-characteristics"
                                        ]->getValue< gkg::Dictionary >();

      }
      else
      {

        gradientCharacteristics[ "type" ] = std::string( "unknown" );

      }

      qSpaceSampling = new gkg::SmsVb15MultipleDifferentShellQSpaceSampling(
                                                      shellBValues,
                                                      orientationCounts,
                                                      transform3d,
                                                      gradientCharacteristics );

    }
    else if ( diffusion_sensitization_type ==
              "spherical_multiple-shell_different-orientation-sets_gehc" )
    {

      gkg::GenericObjectList
        golShellBValues = diffusion_sensitization_parameters[ "b-values" ]->
                            getValue< gkg::GenericObjectList >();
      gkg::GenericObjectList
        golOrientationCounts = diffusion_sensitization_parameters[
                                                        "orientation-counts" ]->
                            getValue< gkg::GenericObjectList >();
      int32_t shellCount = ( int32_t )golShellBValues.size();
      std::vector< float > shellBValues( shellCount );
      std::vector< int32_t > orientationCounts( shellCount );

      int32_t s = 0;
      for ( s = 0; s < shellCount; s++ )
      {

        shellBValues[ s ] = ( float )golShellBValues[ s ]->getScalar();
        orientationCounts[ s ] =
                    ( int32_t )( golOrientationCounts[ s ]->getScalar() + 0.5 );

      }

      gkg::GenericObjectList homogeneous_transform3d =
        diffusion_sensitization_parameters[ "homogeneous-transform3d"
                                        ]->getValue< gkg::GenericObjectList >();

      motion_rotation3ds =
        diffusion_sensitization_parameters[ "motion-rotation3ds"
                                        ]->getValue< gkg::GenericObjectList >();

      int32_t coefficientCount = ( int32_t )homogeneous_transform3d.size();
      std::vector< float > coefficients( coefficientCount );
      int32_t c;
      for ( c = 0; c < coefficientCount; c++ )
      {

        coefficients[ c ] = ( float )homogeneous_transform3d[ c ]->getScalar();

      }
      gkg::HomogeneousTransform3d< float > transform3d( coefficients );

      gkg::Dictionary gradientCharacteristics;
      if ( diffusion_sensitization_parameters.find( "gradient-characteristics" )
           != diffusion_sensitization_parameters.end() )
      {

        gradientCharacteristics =
          diffusion_sensitization_parameters[ "gradient-characteristics"
                                        ]->getValue< gkg::Dictionary >();

      }
      else
      {

        gradientCharacteristics[ "type" ] = std::string( "unknown" );

      }

      qSpaceSampling = new gkg::GehcMultipleDifferentShellQSpaceSampling(
                                                      shellBValues,
                                                      orientationCounts,
                                                      transform3d,
                                                      gradientCharacteristics );

    }
    else if ( diffusion_sensitization_type ==
              "spherical_multiple-shell_different-orientation-sets_gehc-new" )
    {

      gkg::GenericObjectList
        golShellBValues = diffusion_sensitization_parameters[ "b-values" ]->
                            getValue< gkg::GenericObjectList >();
      gkg::GenericObjectList
        golOrientationCounts = diffusion_sensitization_parameters[
                                                        "orientation-counts" ]->
                            getValue< gkg::GenericObjectList >();
      int32_t shellCount = ( int32_t )golShellBValues.size();
      std::vector< float > shellBValues( shellCount );
      std::vector< int32_t > orientationCounts( shellCount );

      int32_t s = 0;
      for ( s = 0; s < shellCount; s++ )
      {

        shellBValues[ s ] = ( float )golShellBValues[ s ]->getScalar();
        orientationCounts[ s ] =
                    ( int32_t )( golOrientationCounts[ s ]->getScalar() + 0.5 );

      }

      gkg::GenericObjectList homogeneous_transform3d =
        diffusion_sensitization_parameters[ "homogeneous-transform3d"
                                        ]->getValue< gkg::GenericObjectList >();

      motion_rotation3ds =
        diffusion_sensitization_parameters[ "motion-rotation3ds"
                                        ]->getValue< gkg::GenericObjectList >();

      int32_t coefficientCount = ( int32_t )homogeneous_transform3d.size();
      std::vector< float > coefficients( coefficientCount );
      int32_t c;
      for ( c = 0; c < coefficientCount; c++ )
      {

        coefficients[ c ] = ( float )homogeneous_transform3d[ c ]->getScalar();

      }
      gkg::HomogeneousTransform3d< float > transform3d( coefficients );

      gkg::Dictionary gradientCharacteristics;
      if ( diffusion_sensitization_parameters.find( "gradient-characteristics" )
           != diffusion_sensitization_parameters.end() )
      {

        gradientCharacteristics =
          diffusion_sensitization_parameters[ "gradient-characteristics"
                                        ]->getValue< gkg::Dictionary >();

      }
      else
      {

        gradientCharacteristics[ "type" ] = std::string( "unknown" );

      }

      qSpaceSampling = new gkg::GehcNewMultipleDifferentShellQSpaceSampling(
                                                      shellBValues,
                                                      orientationCounts,
                                                      transform3d,
                                                      gradientCharacteristics );

    }
    else if ( diffusion_sensitization_type ==
              "spherical_multiple-shell_different-orientation-sets_custom" )
    {

      gkg::GenericObjectList genericObjectListBValues;
      headeredObject.getHeader().getAttribute( "bvalues",
                                               genericObjectListBValues );

      int32_t orientationCount = ( int32_t )genericObjectListBValues.size();

      std::vector<  float > bValues( orientationCount );
      int32_t o;
      for ( o = 0; o < orientationCount; o++ )
      {

        bValues[ o ] = ( float )genericObjectListBValues[ o ]->getScalar();

      }

      float deltaBValue =
        ( float )diffusion_sensitization_parameters[ "delta-b-value"
                                                   ]->getScalar();

      gkg::GenericObjectList homogeneous_transform3d =
        diffusion_sensitization_parameters[ "homogeneous-transform3d"
                                        ]->getValue< gkg::GenericObjectList >();

      motion_rotation3ds =
        diffusion_sensitization_parameters[ "motion-rotation3ds"
                                        ]->getValue< gkg::GenericObjectList >();

      int32_t coefficientCount = ( int32_t )homogeneous_transform3d.size();
      std::vector< float > coefficients( coefficientCount );
      int32_t c;
      for ( c = 0; c < coefficientCount; c++ )
      {

        coefficients[ c ] = ( float )homogeneous_transform3d[ c ]->getScalar();

      }
      gkg::HomogeneousTransform3d< float > transform3d( coefficients );

      gkg::GenericObjectList genericObjectListOrientations;
      headeredObject.getHeader().getAttribute(
                                              "diffusion_gradient_orientations",
                                              genericObjectListOrientations );
      std::vector< gkg::Vector3d< float > > orientations( orientationCount );
      gkg::Vector3d< float > orientation;
      for ( o = 0; o < orientationCount; o++ )
      {

        gkg::GenericObjectList tmp =
          genericObjectListOrientations[ o ]->getValue< gkg::GenericObjectList
                                                      >();
        orientation.x = ( float )tmp[ 0 ]->getScalar();
        orientation.y = ( float )tmp[ 1 ]->getScalar();
        orientation.z = ( float )tmp[ 2 ]->getScalar();
        transform3d.getInverse( orientation, orientations[ o ] );

      }


      gkg::Dictionary gradientCharacteristics;
      if ( diffusion_sensitization_parameters.find( "gradient-characteristics" )
           != diffusion_sensitization_parameters.end() )
      {

        gradientCharacteristics =
          diffusion_sensitization_parameters[ "gradient-characteristics"
                                        ]->getValue< gkg::Dictionary >();

      }
      else
      {

        gradientCharacteristics[ "type" ] = std::string( "unknown" );

      }

      qSpaceSampling = new gkg::CustomMultipleDifferentShellQSpaceSampling(
                                                      bValues,
                                                      orientations,
                                                      deltaBValue,
                                                      transform3d,
                                                      gradientCharacteristics );

    }
    else if ( diffusion_sensitization_type == "cartesian" )
    {

      float maximumBValue =
        ( float )diffusion_sensitization_parameters[ "maximum-b-value"
                                                   ]->getScalar();
      int32_t qspacePointCount =
        ( int32_t )( diffusion_sensitization_parameters[ "q-space-point-count"
                                                       ]->getScalar() + 0.5 );

      gkg::GenericObjectList homogeneous_transform3d =
        diffusion_sensitization_parameters[ "homogeneous-transform3d"
                                        ]->getValue< gkg::GenericObjectList >();

      motion_rotation3ds =
        diffusion_sensitization_parameters[ "motion-rotation3ds"
                                        ]->getValue< gkg::GenericObjectList >();

      int32_t coefficientCount = ( int32_t )homogeneous_transform3d.size();
      std::vector< float > coefficients( coefficientCount );
      int32_t c;
      for ( c = 0; c < coefficientCount; c++ )
      {

        coefficients[ c ] = ( float )homogeneous_transform3d[ c ]->getScalar();

      }
      gkg::HomogeneousTransform3d< float > transform3d( coefficients );

      gkg::Dictionary gradientCharacteristics;
      if ( diffusion_sensitization_parameters.find( "gradient-characteristics" )
           != diffusion_sensitization_parameters.end() )
      {

        gradientCharacteristics =
          diffusion_sensitization_parameters[ "gradient-characteristics"
                                        ]->getValue< gkg::Dictionary >();

      }
      else
      {

        gradientCharacteristics[ "type" ] = std::string( "unknown" );

      }

      qSpaceSampling = new gkg::CartesianQSpaceSampling(
                                                      maximumBValue,
                                                      qspacePointCount,
                                                      transform3d,
                                                      gradientCharacteristics );

    }
    else if ( diffusion_sensitization_type == "arbitrary" )
    {

      int32_t orientationCount =
        ( int32_t )( diffusion_sensitization_parameters[ "orientation-count"
                                                       ]->getScalar() + 0.5 );

      gkg::GenericObjectList homogeneous_transform3d =
        diffusion_sensitization_parameters[ "homogeneous-transform3d"
                                        ]->getValue< gkg::GenericObjectList >();

      motion_rotation3ds =
        diffusion_sensitization_parameters[ "motion-rotation3ds"
                                        ]->getValue< gkg::GenericObjectList >();

      int32_t coefficientCount = ( int32_t )homogeneous_transform3d.size();
      std::vector< float > coefficients( coefficientCount );
      int32_t c;
      for ( c = 0; c < coefficientCount; c++ )
      {

        coefficients[ c ] = ( float )homogeneous_transform3d[ c ]->getScalar();

      }
      gkg::HomogeneousTransform3d< float > transform3d( coefficients );

      gkg::GenericObjectList genericObjectListBValues;
      headeredObject.getHeader().getAttribute( "bvalues",
                                               genericObjectListBValues );

      std::vector<  float > bValues( orientationCount );
      int32_t o;
      for ( o = 0; o < orientationCount; o++ )
      {

        bValues[ o ] = ( float )genericObjectListBValues[ o ]->getScalar();

      }

      gkg::GenericObjectList
        genericObjectListOrientations =
          diffusion_sensitization_parameters[ "orientations"
                                        ]->getValue< gkg::GenericObjectList >();
      std::vector< gkg::Vector3d< float > > orientations( orientationCount );
      gkg::Vector3d< float > orientation;
      for ( o = 0; o < orientationCount; o++ )
      {

        gkg::GenericObjectList tmp =
          genericObjectListOrientations[ o ]->getValue< gkg::GenericObjectList
                                                      >();
        orientation.x = ( float )tmp[ 0 ]->getScalar();
        orientation.y = ( float )tmp[ 1 ]->getScalar();
        orientation.z = ( float )tmp[ 2 ]->getScalar();
        transform3d.getInverse( orientation, orientations[ o ] );

      }

      gkg::Dictionary gradientCharacteristics;
      if ( diffusion_sensitization_parameters.find( "gradient-characteristics" )
           != diffusion_sensitization_parameters.end() )
      {

        gradientCharacteristics =
          diffusion_sensitization_parameters[ "gradient-characteristics"
                                        ]->getValue< gkg::Dictionary >();

      }
      else
      {

        gradientCharacteristics[ "type" ] = std::string( "unknown" );

      }

      qSpaceSampling = new gkg::ArbitraryQSpaceSampling(
                                                      bValues,
                                                      orientations,
                                                      transform3d,
                                                      gradientCharacteristics );
 

    }
    else
    {

      throw std::runtime_error( "bad sensitization type" );

    }

    // adding motion 3D rotations
    int32_t m;
    gkg::Vector3d< float > angle;
    for ( m = 0; m < ( int32_t )motion_rotation3ds.size(); m++ )
    {

      gkg::GenericObjectList tmp =
        motion_rotation3ds[ m ]->getValue< gkg::GenericObjectList >();
      angle.x = ( float )tmp[ 0 ]->getScalar();
      angle.y = ( float )tmp[ 1 ]->getScalar();
      angle.z = ( float )tmp[ 2 ]->getScalar();
      qSpaceSampling->setMotionRotation3d( m, angle );

    }

    return qSpaceSampling;

  }
  GKG_CATCH( "gkg::QSpaceSampling* "
             "gkg::QSpaceSamplingFactory::create( "
             "gkg::HeaderedObject& headeredObject  ) const" );

}





