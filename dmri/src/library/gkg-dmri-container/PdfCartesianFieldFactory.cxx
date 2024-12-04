#include <gkg-dmri-container/PdfCartesianFieldFactory.h>
#include <gkg-core-object/Dictionary.h>
#include <gkg-core-object/GenericObject_i.h>
#include <gkg-dmri-container/DiffusionCartesianField_i.h>
#include <gkg-dmri-pdf-shore/ShorePdfCartesianField.h>
#include <gkg-core-exception/Exception.h>


gkg::PdfCartesianFieldFactory::PdfCartesianFieldFactory()
{
}


gkg::PdfCartesianFieldFactory::~PdfCartesianFieldFactory()
{
}


void gkg::PdfCartesianFieldFactory::registerCreators(
                              const std::string& name,
                              gkg::PdfCartesianFieldFactory::Creator1 creator1,
                              gkg::PdfCartesianFieldFactory::Creator2 creator2 )
{

  try
  {

    std::map< std::string,
              gkg::PdfCartesianFieldFactory::Creator1 >::const_iterator
      c1 = _creator1s.find( name );

    if ( c1 == _creator1s.end() )
    {

      _creator1s[ name ] = creator1;
      _creator2s[ name ] = creator2;

    }
    else
    {

      std::cerr << "gkg::PdfCartesianFieldFactory::"
                << "registerCreators: " << name
                << " already exists"
                << std::endl;

    }

  }
  GKG_CATCH( "void gkg::PdfCartesianFieldFactory::registerCreators( "
             "const std::string& name, "
             "gkg::PdfCartesianFieldFactory::Creator1 creator1, "
             "gkg::PdfCartesianFieldFactory::Creator2 creator2 )" );

}


void gkg::PdfCartesianFieldFactory::registerParameterChecker(
              const std::string& name,
              gkg::PdfCartesianFieldFactory::ParameterChecker parameterChecker )
{

  try
  {

    std::map< std::string,
              gkg::PdfCartesianFieldFactory::ParameterChecker >::const_iterator
      c = _parameterCheckers.find( name );

    if ( c == _parameterCheckers.end() )
    {

      _parameterCheckers[ name ] = parameterChecker;

    }
    else
    {

      std::cerr << "gkg::PdfCartesianFieldFactory::"
                << "registerParameterChecker: " << name
                << " already exists"
                << std::endl;

    }

  }
  GKG_CATCH( "void gkg::PdfCartesianFieldFactory::registerParameterChecker( "
             "const std::string& name, "
             "gkg::PdfCartesianFieldFactory::ParameterChecker "
             "parameterChecker )" );

}


std::list< std::string > gkg::PdfCartesianFieldFactory::getNames() const
{

  try
  {

    std::list< std::string > names;
    std::map< std::string,
              gkg::PdfCartesianFieldFactory::Creator1 >::const_iterator
      c1 = _creator1s.begin(),
      c1e = _creator1s.end();

    while ( c1 != c1e )
    {

      names.push_back( c1->first );
      ++ c1;

    }
    return names;

  }
  GKG_CATCH( "std::list< std::string > "
             "gkg::PdfCartesianFieldFactory::getNames() const" );

}


gkg::PdfCartesianField* gkg::PdfCartesianFieldFactory::create(
                    const std::string& name,
                    gkg::RCPointer< gkg::Volume< float > > t2,
                    gkg::RCPointer< gkg::Volume< float > > dw,
                    gkg::RCPointer< gkg::Volume< int16_t > > mask,
                    const std::vector< int32_t >& iBox,
                    const std::vector< float >& fBox,
                    const std::vector< float >& superResolution,
                    const std::map< float, gkg::OrientationSet >*
                                                    outputMultipleShellSampling,
                    const std::vector< double >& specificScalarParameters,
                    const std::vector< std::string >& specificStringParameters,
                    bool verbose ) const
{

  try
  {

    std::map< std::string,
              gkg::PdfCartesianFieldFactory::Creator1 >::const_iterator
      c1 = _creator1s.find( name );
    if ( c1 != _creator1s.end() )
    {

      return ( c1->second )( t2,
                             dw,
                             mask,
                             iBox,
                             fBox,
                             superResolution,
                             outputMultipleShellSampling,
                             specificScalarParameters,
                             specificStringParameters,
                             verbose );

    }
    else
    {

      throw std::runtime_error( std::string( "'"  ) + name + "' not found" );

    }

  }
  GKG_CATCH( "gkg::PdfCartesianField* "
             "gkg::PdfCartesianFieldFactory::create( "
             "const std::string& name, "
             "gkg::RCPointer< gkg::Volume< float > > t2, "
             "gkg::RCPointer< gkg::Volume< float > > dw, "
             "gkg::RCPointer< gkg::Volume< int16_t > > mask, "
             "const std::vector< int32_t >& iBox, "
             "const std::vector< float >& fBox, "
             "const std::vector< float >& superResolution, "
             "const std::map< float, gkg::OrientationSet >* "
             "outputMultipleShellSampling, "
             "const std::vector< double >& specificScalarParameters, "
             "const std::vector< std::string >& specificStringParameters, "
             "bool verbose ) const" );

}


gkg::PdfCartesianField* gkg::PdfCartesianFieldFactory::create(
                   const std::string& name,
                   gkg::RCPointer< gkg::Volume< float > > t2,
                   gkg::RCPointer< gkg::Volume< float > > dw,
                   gkg::RCPointer< gkg::Volume< int16_t > > mask,
                   const std::vector< int32_t >& iBox,
                   const std::vector< float >& fBox,
                   const std::vector< float >& superResolution,
                   const gkg::Vector3d< int32_t >* outputCartesianGridSize,
                   const gkg::Vector3d< double >* outputCartesianGridResolution,
                   const std::vector< double >& specificScalarParameters,
                   const std::vector< std::string >& specificStringParameters,
                   bool verbose ) const
{

  try
  {

    std::map< std::string,
              gkg::PdfCartesianFieldFactory::Creator2 >::const_iterator
      c2 = _creator2s.find( name );
    if ( c2 != _creator2s.end() )
    {

      return ( c2->second )( t2,
                             dw,
                             mask,
                             iBox,
                             fBox,
                             superResolution,
                             outputCartesianGridSize,
                             outputCartesianGridResolution,
                             specificScalarParameters,
                             specificStringParameters,
                             verbose );

    }
    else
    {

      throw std::runtime_error( std::string( "'"  ) + name + "' not found" );

    }

  }
  GKG_CATCH( "gkg::PdfCartesianField* "
             "gkg::PdfCartesianFieldFactory::create( "
             "const std::string& name, "
             "gkg::RCPointer< gkg::Volume< float > > t2, "
             "gkg::RCPointer< gkg::Volume< float > > dw, "
             "gkg::RCPointer< gkg::Volume< int16_t > > mask, "
             "const std::vector< int32_t >& iBox, "
             "const std::vector< float >& fBox, "
             "const std::vector< float >& superResolution, "
             "const gkg::Vector3d< int32_t >* outputCartesianGridSize, "
             "const gkg::Vector3d< double >* outputCartesianGridResolution, "
             "const std::vector< double >& specificScalarParameters, "
             "const std::vector< std::string >& specificStringParameters, "
             "bool verbose ) const" );

}


void gkg::PdfCartesianFieldFactory::checkOrInitializeDefaultParameters(
                         const std::string& name,
                         std::vector< double >& specificScalarParameters,
                         std::vector< std::string >& specificStringParameters )
{

  try
  {

    std::map< std::string,
              gkg::PdfCartesianFieldFactory::ParameterChecker >::const_iterator
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
  GKG_CATCH( "void gkg::PdfCartesianFieldFactory::"
             "checkOrInitializeDefaultParameters( "
             "const std::string& name, "
             "std::vector< double >& specificScalarParameters, "
             "std::vector< std::string >& specificStringParameters )" );

}


template < class F >
gkg::PdfCartesianField* 
gkg::PdfCartesianFieldFactory::createPdfCartesianField1( 
                    gkg::RCPointer< gkg::Volume< float > > t2,
                    gkg::RCPointer< gkg::Volume< float > > dw,
                    gkg::RCPointer< gkg::Volume< int16_t > > mask,
                    const std::vector< int32_t >& iBox,
                    const std::vector< float >& fBox,
                    const std::vector< float >& superResolution,
                    const std::map< float, gkg::OrientationSet >*
                                                    outputMultipleShellSampling,
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
    // building PDF cartesian field
    ///////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "building PDF cartesian field : " << std::flush;

    }

    gkg::PdfCartesianField* pdfCartesianField = 0;

    if ( iBox.empty() && fBox.empty() )
    {

      pdfCartesianField = new F( t2,
                                 dw,
                                 mask,
                                 outputMultipleShellSampling,
                                 specificScalarParameters,
                                 specificStringParameters,
                                 verbose );

    }
    else if ( !iBox.empty() )
    {

      gkg::BoundingBox< int32_t > boundingBox( iBox[ 0 ], iBox[ 3 ],
                                               iBox[ 1 ], iBox[ 4 ],
                                               iBox[ 2 ], iBox[ 5 ] );
      pdfCartesianField = new F( t2,
                                 dw,
                                 mask,
                                 boundingBox,
                                 outputMultipleShellSampling,
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
      pdfCartesianField = new F( t2,
                                 dw,
                                 mask,
                                 boundingBox,
                                 sResolution,
                                 outputMultipleShellSampling,
                                 specificScalarParameters,
                                 specificStringParameters,
                                 verbose );

    }

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    return pdfCartesianField;

  }
  GKG_CATCH( "template < class F > "
             "gkg::PdfCartesianField* "
             "gkg::PdfCartesianFieldFactory::createPdfCartesianField( "
             "gkg::RCPointer< gkg::Volume< float > > t2, "
             "gkg::RCPointer< gkg::Volume< float > > dw, "
             "gkg::RCPointer< gkg::Volume< int16_t > > mask, "
             "const std::vector< int32_t >& iBox, "
             "const std::vector< float >& fBox, "
             "const std::vector< float >& superResolution, "
             "const std::map< float, gkg::OrientationSet >* "
             "outputMultipleShellSampling, "
             "const std::vector< double >& specificScalarParameters, "
             "const std::vector< std::string >& specificStringParameters, "
             "bool verbose ) const" );

}


template < class F >
gkg::PdfCartesianField* 
gkg::PdfCartesianFieldFactory::createPdfCartesianField2( 
                   gkg::RCPointer< gkg::Volume< float > > t2,
                   gkg::RCPointer< gkg::Volume< float > > dw,
                   gkg::RCPointer< gkg::Volume< int16_t > > mask,
                   const std::vector< int32_t >& iBox,
                   const std::vector< float >& fBox,
                   const std::vector< float >& superResolution,
                   const gkg::Vector3d< int32_t >* outputCartesianGridSize,
                   const gkg::Vector3d< double >* outputCartesianGridResolution,
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
    // building PDF cartesian field
    ///////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "building PDF cartesian field : " << std::flush;

    }

    gkg::PdfCartesianField* pdfCartesianField = 0;

    if ( iBox.empty() && fBox.empty() )
    {

      pdfCartesianField = new F( t2,
                                 dw,
                                 mask,
                                 outputCartesianGridSize,
                                 outputCartesianGridResolution,
                                 specificScalarParameters,
                                 specificStringParameters,
                                 verbose );

    }
    else if ( !iBox.empty() )
    {

      gkg::BoundingBox< int32_t > boundingBox( iBox[ 0 ], iBox[ 3 ],
                                               iBox[ 1 ], iBox[ 4 ],
                                               iBox[ 2 ], iBox[ 5 ] );
      pdfCartesianField = new F( t2,
                                 dw,
                                 mask,
                                 boundingBox,
                                 outputCartesianGridSize,
                                 outputCartesianGridResolution,
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
      pdfCartesianField = new F( t2,
                                 dw,
                                 mask,
                                 boundingBox,
                                 sResolution,
                                 outputCartesianGridSize,
                                 outputCartesianGridResolution,
                                 specificScalarParameters,
                                 specificStringParameters,
                                 verbose );

    }

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    return pdfCartesianField;

  }
  GKG_CATCH( "template < class F > "
             "gkg::PdfCartesianField* "
             "gkg::PdfCartesianFieldFactory::createPdfCartesianField( "
             "gkg::RCPointer< gkg::Volume< float > > t2, "
             "gkg::RCPointer< gkg::Volume< float > > dw, "
             "gkg::RCPointer< gkg::Volume< int16_t > > mask, "
             "const std::vector< int32_t >& iBox, "
             "const std::vector< float >& fBox, "
             "const std::vector< float >& superResolution, "
             "const gkg::Vector3d< int32_t >* outputCartesianGridSize, "
             "const gkg::Vector3d< double >* outputCartesianGridResolution, "
             "const std::vector< double >& specificScalarParameters, "
             "const std::vector< std::string >& specificStringParameters, "
             "bool verbose ) const" );

}


//
// PdfCartesianField creator registration
//

static bool registerCreators()
{

  try
  {

    gkg::PdfCartesianFieldFactory::getInstance().registerCreators(
      "shore_pdf_cartesian_field",
       gkg::PdfCartesianFieldFactory::createPdfCartesianField1<
                                             gkg::ShorePdfCartesianField >,
       gkg::PdfCartesianFieldFactory::createPdfCartesianField2<
                                             gkg::ShorePdfCartesianField > );

    return true;

  }
  GKG_CATCH( "static bool registerCreators()" );


}

static bool registeredCreators __attribute__((unused)) = registerCreators();


//
// PdfCartesianField parameter checker registration
//

static bool registerParameterCheckers()
{

  try
  {

    gkg::PdfCartesianFieldFactory::getInstance().registerParameterChecker(
      "shore_pdf_cartesian_field",
       gkg::ShorePdfCartesianField::checkOrInitializeDefaultParameters );

    return true;

  }
  GKG_CATCH( "static bool registerParameterCheckers()" );


}

static bool registeredParameterCheckers __attribute__((unused)) =
                                                    registerParameterCheckers();
