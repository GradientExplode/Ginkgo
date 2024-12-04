#include <gkg-dmri-container/EapCartesianFieldFactory.h>
#include <gkg-core-object/Dictionary.h>
#include <gkg-core-object/GenericObject_i.h>
#include <gkg-dmri-container/DiffusionCartesianField_i.h>
#include <gkg-dmri-pdf-dpi/DpiEapCartesianField.h>
#include <gkg-core-exception/Exception.h>


gkg::EapCartesianFieldFactory::EapCartesianFieldFactory()
{
}


gkg::EapCartesianFieldFactory::~EapCartesianFieldFactory()
{
}


void gkg::EapCartesianFieldFactory::registerCreator(
                               const std::string& name,
                               gkg::EapCartesianFieldFactory::Creator creator )
{

  try
  {

    std::map< std::string,
              gkg::EapCartesianFieldFactory::Creator >::const_iterator
      c = _creators.find( name );

    if ( c == _creators.end() )
    {

      _creators[ name ] = creator;

    }
    else
    {

      std::cerr << "gkg::EapCartesianFieldFactory::"
                << "registerCreator: " << name
                << " already exists"
                << std::endl;

    }

  }
  GKG_CATCH( "void gkg::EapCartesianFieldFactory::registerCreator( "
             "const std::string& name, "
             "gkg::EapCartesianFieldFactory::Creator creator )" );

}


void gkg::EapCartesianFieldFactory::registerParameterChecker(
              const std::string& name,
              gkg::EapCartesianFieldFactory::ParameterChecker parameterChecker )
{

  try
  {

    std::map< std::string,
              gkg::EapCartesianFieldFactory::ParameterChecker >::const_iterator
      c = _parameterCheckers.find( name );

    if ( c == _parameterCheckers.end() )
    {

      _parameterCheckers[ name ] = parameterChecker;

    }
    else
    {

      std::cerr << "gkg::EapCartesianFieldFactory::"
                << "registerParameterChecker: " << name
                << " already exists"
                << std::endl;

    }

  }
  GKG_CATCH( "void gkg::EapCartesianFieldFactory::registerParameterChecker( "
             "const std::string& name, "
             "gkg::EapCartesianFieldFactory::ParameterChecker "
             "parameterChecker )" );

}


std::list< std::string > gkg::EapCartesianFieldFactory::getNames() const
{

  try
  {

    std::list< std::string > names;
    std::map< std::string,
              gkg::EapCartesianFieldFactory::Creator >::const_iterator
      c = _creators.begin(),
      ce = _creators.end();

    while ( c != ce )
    {

      names.push_back( c->first );
      ++ c;

    }
    return names;

  }
  GKG_CATCH( "std::list< std::string > "
             "gkg::EapCartesianFieldFactory::getNames() const" );

}


gkg::EapCartesianField* gkg::EapCartesianFieldFactory::create(
                    const std::string& name,
                    gkg::RCPointer< gkg::Volume< float > > t2,
                    gkg::RCPointer< gkg::Volume< float > > dw,
                    gkg::RCPointer< gkg::Volume< int16_t > > mask,
                    const std::vector< int32_t >& iBox,
                    const std::vector< float >& fBox,
                    const std::vector< float >& superResolution,
                    const gkg::OrientationSet* outputOrientationSet,
                    const std::vector< double >& specificScalarParameters,
                    const std::vector< std::string >& specificStringParameters,
                    bool verbose ) const
{

  try
  {

    std::map< std::string,
              gkg::EapCartesianFieldFactory::Creator >::const_iterator
      c = _creators.find( name );

    if ( c != _creators.end() )
    {

      return ( c->second )( t2,
                            dw,
                            mask,
                            iBox,
                            fBox,
                            superResolution,
                            outputOrientationSet,
                            specificScalarParameters,
                            specificStringParameters,
                            verbose );

    }
    else
    {

      throw std::runtime_error( std::string( "'"  ) + name + "' not found" );

    }

  }
  GKG_CATCH( "gkg::EapCartesianField* "
             "gkg::EapCartesianFieldFactory::create( "
             "const std::string& name, "
             "gkg::RCPointer< gkg::Volume< float > > t2, "
             "gkg::RCPointer< gkg::Volume< float > > dw, "
             "gkg::RCPointer< gkg::Volume< int16_t > > mask, "
             "const std::vector< int32_t >& iBox, "
             "const std::vector< float >& fBox, "
             "const std::vector< float >& superResolution, "
             "const gkg::OrientationSet* outputOrientationSet, "
             "const std::vector< double >& specificScalarParameters, "
             "const std::vector< std::string >& specificStringParameters, "
             "bool verbose ) const" );

}


void gkg::EapCartesianFieldFactory::checkOrInitializeDefaultParameters(
                         const std::string& name,
                         std::vector< double >& specificScalarParameters,
                         std::vector< std::string >& specificStringParameters )
{

  try
  {

    std::map< std::string,
              gkg::EapCartesianFieldFactory::ParameterChecker >::const_iterator
      c = _parameterCheckers.find( name );
    if ( c != _parameterCheckers.end() )
    {

      return ( c->second )( specificScalarParameters,
                            specificStringParameters );

    }
    else
    {

      throw std::runtime_error( std::string( "'"  ) + name + "' not found" );

    }

  }
  GKG_CATCH( "void gkg::EapCartesianFieldFactory::"
             "checkOrInitializeDefaultParameters( "
             "const std::string& name, "
             "std::vector< double >& specificScalarParameters, "
             "std::vector< std::string >& specificStringParameters )" );

}


template < class F >
gkg::EapCartesianField* 
gkg::EapCartesianFieldFactory::createEapCartesianField( 
                    gkg::RCPointer< gkg::Volume< float > > t2,
                    gkg::RCPointer< gkg::Volume< float > > dw,
                    gkg::RCPointer< gkg::Volume< int16_t > > mask,
                    const std::vector< int32_t >& iBox,
                    const std::vector< float >& fBox,
                    const std::vector< float >& superResolution,
                    const gkg::OrientationSet* outputOrientationSet,
                    const std::vector< double >& specificScalarParameters,
                    const std::vector< std::string >& specificStringParameters,
                    bool verbose )
{

  try
  {

    ///////////////////////////////////////////////////////////////////////////
    // sanity checks
    ///////////////////////////////////////////////////////////////////////////

    if ( !iBox.empty() )
    {

      if ( !fBox.empty() || !superResolution.empty() )
      {

        throw std::runtime_error( "-ibox option not compatible with -fbox and "
                                  "-superResolution options" );

      }
      if ( iBox.size() != 6U )
      {

        throw std::runtime_error( "-iBox argument must be of type"
                                  " xinf yinf zinf xsup ysup zsup" );

      }

    }
    else if ( !fBox.empty() )
    {

      if ( fBox.size() != 6U )
      {

        throw std::runtime_error( "-fBox argument must be of type"
                                  " xinf yinf zinf xsup ysup zsup" );

      }

      if ( !superResolution.empty() && ( superResolution.size() != 3U ) )
      {

        throw std::runtime_error( "-superResolution option must be of the form"
                                  "xres yres zres" );

      }

    }


    ///////////////////////////////////////////////////////////////////////////
    // estimating input orientation set statistics
    ///////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "input orientation set statistics : " << std::flush;

    }

    float minimumAngle = 0.0;
    float maximumAngle = 0.0;
    float averageAngle = 0.0;
    float angleStdDev = 0.0;

    gkg::GenericObjectList genericObjectListOrientations( dw->getSizeT() );
    if ( dw->getHeader().hasAttribute( "diffusion_gradient_orientations" ) )
    {

      dw->getHeader().getAttribute( "diffusion_gradient_orientations",
                                    genericObjectListOrientations );

    }
    else
    {

      throw std::runtime_error(
            "missing attribute 'diffusion_gradient_orientations' in DW data" );

    }
    std::vector< gkg::Vector3d< float > > orientations( dw->getSizeT() );
    int32_t o;
    for ( o = 0; o < dw->getSizeT(); o++ )
    {

      gkg::GenericObjectList tmp =
      genericObjectListOrientations[ o ]->getValue< gkg::GenericObjectList >();
      orientations[ o ].x = ( float )tmp[ 0 ]->getScalar();
      orientations[ o ].y = ( float )tmp[ 1 ]->getScalar();
      orientations[ o ].z = ( float )tmp[ 2 ]->getScalar();

    }
    gkg::OrientationSet 
      inputOrientationSet = gkg::OrientationSet( orientations );
    inputOrientationSet.getStatistics( &minimumAngle,
                                       &maximumAngle,
                                       &averageAngle,
                                       &angleStdDev );
    minimumAngle *= 180.0 / M_PI;
    maximumAngle *= 180.0 / M_PI;
    averageAngle *= 180.0 / M_PI;
    angleStdDev *= 180.0 / M_PI;
    if ( verbose )
    {

      std::cout << "cnt=" << dw->getSizeT() << " " << std::flush;
      std::cout << "min=" << minimumAngle << "deg " << std::flush;
      std::cout << "max=" << maximumAngle << "deg " << std::flush;
      std::cout << "avg=" << averageAngle << "deg " << std::flush;
      std::cout << "std=" << angleStdDev << "deg" << std::endl;

    }


    ///////////////////////////////////////////////////////////////////////////
    // building EAP cartesian field
    ///////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "building EAP cartesian field : " << std::flush;

    }

    gkg::EapCartesianField* eapCartesianField = 0;

    if ( iBox.empty() && fBox.empty() )
    {

      eapCartesianField = new F( t2,
                                 dw,
                                 mask,
                                 outputOrientationSet,
                                 specificScalarParameters,
                                 specificStringParameters,
                                 verbose );

    }
    else if ( !iBox.empty() )
    {

      gkg::BoundingBox< int32_t > boundingBox( iBox[ 0 ], iBox[ 3 ],
                                               iBox[ 1 ], iBox[ 4 ],
                                               iBox[ 2 ], iBox[ 5 ] );

      std::cout << "building DPI Cartesian field with ibox\n";
      eapCartesianField = new F( t2,
                                 dw,
                                 mask,
                                 boundingBox,
                                 outputOrientationSet,
                                 specificScalarParameters,
                                 specificStringParameters,
                                 verbose );

    }
    else if ( !fBox.empty() )
    {

      gkg::BoundingBox< float > boundingBox( fBox[ 0 ], fBox[ 3 ],
                                             fBox[ 1 ], fBox[ 4 ],
                                             fBox[ 2 ], fBox[ 5 ] );

      gkg::Vector3d< float > sResolution;
      if ( !superResolution.empty() )
      {

        sResolution.x = superResolution[ 0 ];
        sResolution.y = superResolution[ 1 ];
        sResolution.z = superResolution[ 2 ];

      }
      else
      {

        double resolutionX = 0.0;
        double resolutionY = 0.0;
        double resolutionZ = 0.0;
        t2->getHeader().getAttribute( "resolutionX", resolutionX );
        t2->getHeader().getAttribute( "resolutionY", resolutionY );
        t2->getHeader().getAttribute( "resolutionZ", resolutionZ );
        sResolution.x = ( float )resolutionX;
        sResolution.y = ( float )resolutionY;
        sResolution.z = ( float )resolutionZ;

      }

      eapCartesianField = new F( t2,
                                 dw,
                                 mask,
                                 boundingBox,
                                 sResolution,
                                 outputOrientationSet,
                                 specificScalarParameters,
                                 specificStringParameters,
                                 verbose );

    }

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    return eapCartesianField;

  }
  GKG_CATCH( "template < class F > "
             "gkg::EapCartesianField* "
             "gkg::EapCartesianFieldFactory::createEapCartesianField( "
             "gkg::RCPointer< gkg::Volume< float > > t2, "
             "gkg::RCPointer< gkg::Volume< float > > dw, "
             "gkg::RCPointer< gkg::Volume< int16_t > > mask, "
             "const std::vector< int32_t >& iBox, "
             "const std::vector< float >& fBox, "
             "const std::vector< float >& superResolution, "
             "const gkg::OrientationSet* outputOrientationSet, "
             "const std::vector< double >& specificScalarParameters, "
             "const std::vector< std::string >& specificStringParameters, "
             "bool verbose ) const" );

}


//
// EapCartesianField creator registration
//

static bool registerCreators()
{

  try
  {

//     gkg::EapCartesianFieldFactory::getInstance().registerCreator(
//       "charmed_eap_cartesian_field",
//       gkg::EapCartesianFieldFactory::createEapCartesianField<
//                                              gkg::CharmedEapCartesianField > );

//     gkg::EapCartesianFieldFactory::getInstance().registerCreator(
//       "dot_eap_cartesian_field",
//       gkg::EapCartesianFieldFactory::createEapCartesianField<
//                                             gkg::DotEapCartesianField > );

    gkg::EapCartesianFieldFactory::getInstance().registerCreator(
      "dpi_eap_cartesian_field",
       gkg::EapCartesianFieldFactory::createEapCartesianField<
                                            gkg::DpiEapCartesianField > );

//     gkg::EapCartesianFieldFactory::getInstance().registerCreator(
//       "dsi_eap_cartesian_field",
//        gkg::EapCartesianFieldFactory::createEapCartesianField<
//                                             gkg::DsiEapCartesianField > );

//     gkg::EapCartesianFieldFactory::getInstance().registerCreator(
//       "tomographic_eap_cartesian_field",
//        gkg::EapCartesianFieldFactory::createEapCartesianField<
//                                             gkg::TomographicEapCartesianField > );

    return true;

  }
  GKG_CATCH( "static bool registerCreators()" );


}

static bool registeredCreators __attribute__((unused)) = registerCreators();


//
// EapCartesianField parameter checker registration
//

static bool registerParameterCheckers()
{

  try
  {

//     gkg::EapCartesianFieldFactory::getInstance().registerParameterChecker(
//       "charmed_eap_cartesian_field",
//        gkg::CharmedEapCartesianField::checkOrInitializeDefaultParameters );

//     gkg::EapCartesianFieldFactory::getInstance().registerParameterChecker(
//       "dot_eap_cartesian_field",
//       gkg::DotEapCartesianField::checkOrInitializeDefaultParameters );

    gkg::EapCartesianFieldFactory::getInstance().registerParameterChecker(
      "dpi_eap_cartesian_field",
      gkg::DpiEapCartesianField::checkOrInitializeDefaultParameters );

//     gkg::EapCartesianFieldFactory::getInstance().registerParameterChecker(
//       "dsi_eap_cartesian_field",
//       gkg::DsiEapCartesianField::checkOrInitializeDefaultParameters );

//     gkg::EapCartesianFieldFactory::getInstance().registerParameterChecker(
//       "tomographic_eap_cartesian_field",
//       gkg::TomographicEapCartesianField::checkOrInitializeDefaultParameters );

    return true;

  }
  GKG_CATCH( "static bool registerParameterCheckers()" );


}

static bool registeredParameterCheckers __attribute__((unused)) =
                                                    registerParameterCheckers();
