#include <gkg-dmri-container/MicrostructureCartesianFieldFactory.h>
#include <gkg-core-object/Dictionary.h>
#include <gkg-core-object/GenericObject_i.h>
#include <gkg-dmri-container/DiffusionCartesianField_i.h>
#include <gkg-dmri-microstructure-noddi/NoddiOdfCartesianField.h>
#include <gkg-dmri-microstructure-axcaliber/AxCaliberOdfCartesianField.h>
#include <gkg-dmri-microstructure-axon-mapping/AxonMappingOdfCartesianField.h>
#include <gkg-dmri-microstructure-ivim/IvimOdfCartesianField.h>
#include <gkg-core-exception/Exception.h>


gkg::MicrostructureCartesianFieldFactory::MicrostructureCartesianFieldFactory()
{
}


gkg::MicrostructureCartesianFieldFactory::~MicrostructureCartesianFieldFactory()
{
}


void gkg::MicrostructureCartesianFieldFactory::registerCreator(
                     const std::string& name,
                     gkg::MicrostructureCartesianFieldFactory::Creator creator )
{

  try
  {

    std::map< std::string,
             gkg::MicrostructureCartesianFieldFactory::Creator >::const_iterator
      c = _creators.find( name );

    if ( c == _creators.end() )
    {

      _creators[ name ] = creator;

    }
    else
    {

      std::cerr << "gkg::MicrostructureCartesianFieldFactory::"
                << "registerCreator: " << name
                << " already exists"
                << std::endl;

    }

  }
  GKG_CATCH( "void gkg::MicrostructureCartesianFieldFactory::registerCreator( "
             "const std::string& name, "
             "gkg::MicrostructureCartesianFieldFactory::Creator creator )" );

}


void gkg::MicrostructureCartesianFieldFactory::registerParameterChecker(
   const std::string& name,
   gkg::MicrostructureCartesianFieldFactory::ParameterChecker parameterChecker )
{

  try
  {

    std::map< std::string,
    gkg::MicrostructureCartesianFieldFactory::ParameterChecker >::const_iterator
      c = _parameterCheckers.find( name );

    if ( c == _parameterCheckers.end() )
    {

      _parameterCheckers[ name ] = parameterChecker;

    }
    else
    {

      std::cerr << "gkg::MicrostructureCartesianFieldFactory::"
                << "registerParameterChecker: " << name
                << " already exists"
                << std::endl;

    }

  }
  GKG_CATCH( "void gkg::MicrostructureCartesianFieldFactory::"
             "registerParameterChecker( "
             "const std::string& name, "
             "gkg::MicrostructureCartesianFieldFactory::ParameterChecker "
             "parameterChecker )" );

}


std::list< std::string > 
gkg::MicrostructureCartesianFieldFactory::getNames() const
{

  try
  {

    std::list< std::string > names;
    std::map< std::string,
             gkg::MicrostructureCartesianFieldFactory::Creator >::const_iterator
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
             "gkg::MicrostructureCartesianFieldFactory::getNames() const" );

}


gkg::OdfCartesianField* gkg::MicrostructureCartesianFieldFactory::create(
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
             gkg::MicrostructureCartesianFieldFactory::Creator >::const_iterator
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
  GKG_CATCH( "gkg::OdfCartesianField* "
             "gkg::MicrostructureCartesianFieldFactory::create( "
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


void 
gkg::MicrostructureCartesianFieldFactory::checkOrInitializeDefaultParameters(
                         const std::string& name,
                         std::vector< double >& specificScalarParameters,
                         std::vector< std::string >& specificStringParameters )
{

  try
  {

    std::map< std::string,
    gkg::MicrostructureCartesianFieldFactory::ParameterChecker >::const_iterator
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
  GKG_CATCH( "void gkg::MicrostructureCartesianFieldFactory::"
             "checkOrInitializeDefaultParameters( "
             "const std::string& name, "
             "std::vector< double >& specificScalarParameters, "
             "std::vector< std::string >& specificStringParameters )" );

}


template < class F >
gkg::OdfCartesianField* 
gkg::MicrostructureCartesianFieldFactory::createMicrostructureCartesianField( 
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
    // building ODF cartesian field
    ///////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "building microstructure cartesian field : " << std::flush;

    }

    gkg::OdfCartesianField* odfCartesianField = 0;

    if ( iBox.empty() && fBox.empty() )
    {

      odfCartesianField = new F( t2,
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
      odfCartesianField = new F( t2,
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
      odfCartesianField = new F( t2,
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

    return odfCartesianField;

  }
  GKG_CATCH( "template < class F > "
             "gkg::OdfCartesianField* "
             "gkg::MicrostructureCartesianFieldFactory::"
             "createOdfCartesianField( "
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
// OdfCartesianField creator registration
//

static bool registerCreators()
{

  try
  {

    gkg::MicrostructureCartesianFieldFactory::getInstance().registerCreator(
      "noddi_microstructure_cartesian_field",
       gkg::MicrostructureCartesianFieldFactory::
         createMicrostructureCartesianField< gkg::NoddiOdfCartesianField > );

    gkg::MicrostructureCartesianFieldFactory::getInstance().registerCreator(
      "axon_mapping_microstructure_cartesian_field",
       gkg::MicrostructureCartesianFieldFactory::
         createMicrostructureCartesianField< 
                                          gkg::AxonMappingOdfCartesianField > );

    gkg::MicrostructureCartesianFieldFactory::getInstance().registerCreator(
      "ivim_microstructure_cartesian_field",
       gkg::MicrostructureCartesianFieldFactory::
         createMicrostructureCartesianField< gkg::IvimOdfCartesianField > );

    gkg::MicrostructureCartesianFieldFactory::getInstance().registerCreator(
      "ax_caliber_microstructure_cartesian_field",
       gkg::MicrostructureCartesianFieldFactory::
         createMicrostructureCartesianField< gkg::AxCaliberOdfCartesianField > );

    return true;

  }
  GKG_CATCH( "static bool registerCreators()" );


}

static bool registeredCreators __attribute__((unused)) = registerCreators();


//
// OdfCartesianField parameter checker registration
//

static bool registerParameterCheckers()
{

  try
  {

    gkg::MicrostructureCartesianFieldFactory::getInstance().
                                                       registerParameterChecker(
      "noddi_microstructure_cartesian_field",
      gkg::NoddiOdfCartesianField::checkOrInitializeDefaultParameters );

    gkg::MicrostructureCartesianFieldFactory::getInstance().
                                                       registerParameterChecker(
      "axon_mapping_microstructure_cartesian_field",
      gkg::AxonMappingOdfCartesianField::checkOrInitializeDefaultParameters );

    gkg::MicrostructureCartesianFieldFactory::getInstance().
                                                       registerParameterChecker(
      "ivim_microstructure_cartesian_field",
      gkg::IvimOdfCartesianField::checkOrInitializeDefaultParameters );

    gkg::MicrostructureCartesianFieldFactory::getInstance().
                                                       registerParameterChecker(
      "ax_caliber_microstructure_cartesian_field",
      gkg::AxCaliberOdfCartesianField::checkOrInitializeDefaultParameters );

    return true;

  }
  GKG_CATCH( "static bool registerParameterCheckers()" );


}

static bool registeredParameterCheckers __attribute__((unused)) =
                                                    registerParameterCheckers();
