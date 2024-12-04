#include <gkg-dmri-container/DiffusoidCartesianFieldFactory.h>
#include <gkg-core-object/Dictionary.h>
#include <gkg-core-object/GenericObject_i.h>
#include <gkg-dmri-container/DiffusionCartesianField_i.h>
#include <gkg-dmri-diffusoid-acquisition/AcquisitionDiffusoidCartesianField.h>
#include <gkg-core-exception/Exception.h>


gkg::DiffusoidCartesianFieldFactory::DiffusoidCartesianFieldFactory()
{
}


gkg::DiffusoidCartesianFieldFactory::~DiffusoidCartesianFieldFactory()
{
}


void gkg::DiffusoidCartesianFieldFactory::registerCreator(
                         const std::string& name,
                         gkg::DiffusoidCartesianFieldFactory::Creator creator )
{

  try
  {

    std::map< std::string,
              gkg::DiffusoidCartesianFieldFactory::Creator >::const_iterator
      c = _creators.find( name );

    if ( c == _creators.end() )
    {

      _creators[ name ] = creator;

    }
    else
    {

      std::cerr << "gkg::DiffusoidCartesianFieldFactory::"
                << "registerCreator: " << name
                << " already exists"
                << std::endl;

    }

  }
  GKG_CATCH( "void gkg::DiffusoidCartesianFieldFactory::registerCreator( "
             "const std::string& name, "
             "gkg::DiffusoidCartesianFieldFactory::Creator creator )" );

}


void gkg::DiffusoidCartesianFieldFactory::registerParameterChecker(
       const std::string& name,
       gkg::DiffusoidCartesianFieldFactory::ParameterChecker parameterChecker )
{

  try
  {

    std::map<
        std::string,
        gkg::DiffusoidCartesianFieldFactory::ParameterChecker >::const_iterator
      c = _parameterCheckers.find( name );

    if ( c == _parameterCheckers.end() )
    {

      _parameterCheckers[ name ] = parameterChecker;

    }
    else
    {

      std::cerr << "gkg::DiffusoidCartesianFieldFactory::"
                << "registerParameterChecker: " << name
                << " already exists"
                << std::endl;

    }

  }
  GKG_CATCH( "void gkg::DiffusoidCartesianFieldFactory::registerParameterChecker( "
             "const std::string& name, "
             "gkg::DiffusoidCartesianFieldFactory::ParameterChecker "
             "parameterChecker )" );

}


std::list< std::string > gkg::DiffusoidCartesianFieldFactory::getNames() const
{

  try
  {

    std::list< std::string > names;
    std::map< std::string,
              gkg::DiffusoidCartesianFieldFactory::Creator >::const_iterator
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
             "gkg::DiffusoidCartesianFieldFactory::getNames() const" );

}


gkg::DiffusoidCartesianField* gkg::DiffusoidCartesianFieldFactory::create(
                    const std::string& name,
                    gkg::RCPointer< gkg::Volume< float > > t2,
                    gkg::RCPointer< gkg::Volume< float > > dw,
                    gkg::RCPointer< gkg::Volume< int16_t > > mask,
                    const std::vector< int32_t >& iBox,
                    const std::vector< float >& fBox,
                    const std::vector< float >& superResolution,
                    const gkg::OrientationSet* outputOrientationSet,
                    const gkg::SO3Interpolator* so3Interpolator,
                    const std::vector< double >& specificScalarParameters,
                    const std::vector< std::string >& specificStringParameters,
                    bool verbose ) const
{

  try
  {

    std::map< std::string,
              gkg::DiffusoidCartesianFieldFactory::Creator >::const_iterator
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
                            so3Interpolator,
                            specificScalarParameters,
                            specificStringParameters,
                            verbose );

    }
    else
    {

      throw std::runtime_error( std::string( "'"  ) + name + "' not found" );

    }

  }
  GKG_CATCH( "gkg::DiffusoidCartesianField* "
             "gkg::DiffusoidCartesianFieldFactory::create( "
             "const std::string& name, "
             "gkg::RCPointer< gkg::Volume< float > > t2, "
             "gkg::RCPointer< gkg::Volume< float > > dw, "
             "gkg::RCPointer< gkg::Volume< int16_t > > mask, "
             "const std::vector< int32_t >& iBox, "
             "const std::vector< float >& fBox, "
             "const std::vector< float >& superResolution, "
             "const gkg::OrientationSet* outputOrientationSet, "
             "const gkg::SO3Interpolator* so3Interpolator, "
             "const std::vector< double >& specificScalarParameters, "
             "const std::vector< std::string >& specificStringParameters, "
             "bool verbose ) const" );

}


void gkg::DiffusoidCartesianFieldFactory::checkOrInitializeDefaultParameters(
                         const std::string& name,
                         std::vector< double >& specificScalarParameters,
                         std::vector< std::string >& specificStringParameters )
{

  try
  {

    std::map<
        std::string,
        gkg::DiffusoidCartesianFieldFactory::ParameterChecker >::const_iterator
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
  GKG_CATCH( "void gkg::DiffusoidCartesianFieldFactory::"
             "checkOrInitializeDefaultParameters( "
             "const std::string& name, "
             "std::vector< double >& specificScalarParameters, "
             "std::vector< std::string >& specificStringParameters )" );

}


template < class F >
gkg::DiffusoidCartesianField* 
gkg::DiffusoidCartesianFieldFactory::createDiffusoidCartesianField( 
                    gkg::RCPointer< gkg::Volume< float > > t2,
                    gkg::RCPointer< gkg::Volume< float > > dw,
                    gkg::RCPointer< gkg::Volume< int16_t > > mask,
                    const std::vector< int32_t >& iBox,
                    const std::vector< float >& fBox,
                    const std::vector< float >& superResolution,
                    const gkg::OrientationSet* outputOrientationSet,
                    const gkg::SO3Interpolator* so3Interpolator,
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

      std::cout << "building Diffusoid cartesian field : " << std::flush;

    }

    gkg::DiffusoidCartesianField* diffusoidCartesianField = 0;

    if ( iBox.empty() && fBox.empty() )
    {

      diffusoidCartesianField = new F( t2,
                                       dw,
                                       mask,
                                       outputOrientationSet,
                                       so3Interpolator,
                                       specificScalarParameters,
                                       specificStringParameters,
                                       verbose );

    }
    else if ( !iBox.empty() )
    {

      gkg::BoundingBox< int32_t > boundingBox( iBox[ 0 ], iBox[ 3 ],
                                               iBox[ 1 ], iBox[ 4 ],
                                               iBox[ 2 ], iBox[ 5 ] );
      diffusoidCartesianField = new F( t2,
                                       dw,
                                       mask,
                                       boundingBox,
                                       outputOrientationSet,
                                       so3Interpolator,
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
      diffusoidCartesianField = new F( t2,
                                       dw,
                                       mask,
                                       boundingBox,
                                       sResolution,
                                       outputOrientationSet,
                                       so3Interpolator,
                                       specificScalarParameters,
                                       specificStringParameters,
                                       verbose );

    }

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    return diffusoidCartesianField;

  }
  GKG_CATCH( "template < class F > "
             "gkg::DiffusoidCartesianField* "
             "gkg::DiffusoidCartesianFieldFactory::"
             "createDiffusoidCartesianField( "
             "gkg::RCPointer< gkg::Volume< float > > t2, "
             "gkg::RCPointer< gkg::Volume< float > > dw, "
             "gkg::RCPointer< gkg::Volume< int16_t > > mask, "
             "const std::vector< int32_t >& iBox, "
             "const std::vector< float >& fBox, "
             "const std::vector< float >& superResolution, "
             "const gkg::OrientationSet* outputOrientationSet, "
             "const gkg::SO3Interpolator* so3Interpolator, "
             "const std::vector< double >& specificScalarParameters, "
             "const std::vector< std::string >& specificStringParameters, "
             "bool verbose ) const" );

}


//
// DiffusoidCartesianField creator registration
//

static bool registerCreators()
{

  try
  {

    gkg::DiffusoidCartesianFieldFactory::getInstance().registerCreator(
      "acquisition_diffusoid_cartesian_field",
       gkg::DiffusoidCartesianFieldFactory::createDiffusoidCartesianField<
                                   gkg::AcquisitionDiffusoidCartesianField > );

    return true;

  }
  GKG_CATCH( "static bool registerCreators()" );


}

static bool registeredCreators __attribute__((unused)) = registerCreators();


//
// DiffusoidCartesianField parameter checker registration
//

static bool registerParameterCheckers()
{

  try
  {

    gkg::DiffusoidCartesianFieldFactory::getInstance().
      registerParameterChecker(
        "acquisition_diffusoid_cartesian_field",
         gkg::AcquisitionDiffusoidCartesianField::
                                          checkOrInitializeDefaultParameters );

    return true;

  }
  GKG_CATCH( "static bool registerParameterCheckers()" );


}

static bool registeredParameterCheckers __attribute__((unused)) =
                                                    registerParameterCheckers();
