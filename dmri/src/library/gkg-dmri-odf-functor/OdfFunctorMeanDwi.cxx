#include <gkg-dmri-odf-functor/OdfFunctorMeanDwi.h>
#include <gkg-dmri-odf-functor/OdfFunctor_i.h>
#include <gkg-dmri-odf-functor/OdfFunctorFactory_i.h>
#include <gkg-core-exception/Exception.h>
#include <cmath>


gkg::OdfFunctorMeanDwi::OdfFunctorMeanDwi()
                               : gkg::OdfFunctor< gkg::Volume< float > ,
                                                  float >()
{
}


gkg::OdfFunctorMeanDwi::~OdfFunctorMeanDwi()
{
}


std::string gkg::OdfFunctorMeanDwi::getName() const
{

  return "mean_dwi";

}


bool 
gkg::OdfFunctorMeanDwi::isStandardBasisRequired() const
{

  return false;

}


void gkg::OdfFunctorMeanDwi::process(
                             gkg::Volume< float >& output,
                             gkg::OdfCartesianField& odfCartesianField,
                             const std::vector< double >& scalarParameters,
                             const std::vector< std::string >& stringParameters,
                             bool verbose ) const
{

  try
  {

    if ( verbose )
    {

      std::cout << "processing " << this->getName() << " : " << std::flush;

    }

    // getting site count
    int32_t siteCount = odfCartesianField.getSiteMap().getSiteCount( 0 );

    // getting site(s)
    const std::list< gkg::Vector3d< int32_t > >&
      sites = odfCartesianField.getSiteMap().getSites( 0 );

    // getting offset
    gkg::Vector3d< int32_t > 
      offset = odfCartesianField.getOffset();

    // getting pointer to DW image
    const gkg::Volume< float >& dwVolume = odfCartesianField.getDWVolume();
    int32_t inputOrientationCount = dwVolume.getSizeT();

    // preparing output volume
    int32_t sizeX = odfCartesianField.getSizeX();
    int32_t sizeY = odfCartesianField.getSizeY();
    int32_t sizeZ = odfCartesianField.getSizeZ();
    gkg::Vector3d< double > resolution;
    resolution.x = odfCartesianField.getResolutionX();
    resolution.y = odfCartesianField.getResolutionY();
    resolution.z = odfCartesianField.getResolutionZ();

    output.reallocate( sizeX, sizeY, sizeZ );
    output.getHeader().addAttribute( "resolutionX", resolution.x );
    output.getHeader().addAttribute( "resolutionY", resolution.y );
    output.getHeader().addAttribute( "resolutionZ", resolution.z );

    output.fill( 0.0f );

    // allocating specific scalar parameter vector
    // the required parameters are organized as:
    // <DW_count > <DW(0)> .... <DW(N-1)>
    std::vector< double > specificScalarParameters( 5 +
                                                    1 /*for DW count*/ +
                                                    inputOrientationCount, 0 );
    specificScalarParameters[ 0 ] = scalarParameters[ 0 ];
    specificScalarParameters[ 1 ] = scalarParameters[ 1 ];
    specificScalarParameters[ 2 ] = scalarParameters[ 2 ];
    specificScalarParameters[ 3 ] = scalarParameters[ 3 ];
    specificScalarParameters[ 4 ] = scalarParameters[ 4 ];
    specificScalarParameters[ 5 ] = inputOrientationCount;

    // looping over site(s) and processing output item(s)
    int32_t o = 0;
    int32_t count = 0;
    std::list< Vector3d< int32_t > >::const_iterator
      s = sites.begin(),
      se = sites.end();
    while ( s != se )
    {

      if ( verbose )
      {

        if ( ( ( count + 1 ) % 100 == 0 ) ||
             ( count == 0 ) ||
             ( count == siteCount - 1 ) )
        {

          if ( count )
          {

            std::cout << Eraser( 22 );

          }
          std::cout << " [ " << std::setw( 7 ) << count + 1 
                    << " / " << std::setw( 7 ) << siteCount
                    << " ]" << std::flush;

        }

      }
      for ( o = 0; o  < inputOrientationCount; o++ )
      {

        specificScalarParameters[ 6 + o ] = ( double )dwVolume( *s, o );

      }
      this->processItem( output( *s - offset ),
                         *odfCartesianField.getItem( *s ),
                         specificScalarParameters,
                         stringParameters );

      ++ s;
      ++ count;

    }

    if ( verbose )
    {

      std::cout << Eraser( 22 ) << "done" << std::endl;

    }

  }
  GKG_CATCH( "void gkg::OdfFunctorMeanDwi::process( "
             "gkg::Volume< float >& output, "
             "gkg::OdfCartesianField& odfCartesianField, "
             "const std::vector< double >& scalarParameters, "
             "const std::vector< std::string >& stringParameters, "
             "bool verbose ) const" );


}


void gkg::OdfFunctorMeanDwi::processItem(
                 float& item,
                 const gkg::OrientationDistributionFunction& /*odf*/,
                 const std::vector< double >& scalarParameters,
                 const std::vector< std::string >& /*stringParameters*/ ) const
{

  try
  {

    int32_t inputOrientationCount = ( int32_t )( scalarParameters[ 5 ] +
                                                   0.5 );

    int32_t o = 0;
    double dwi = 0.0;
    double meanDwi = 1.0;
    for ( o = 0; o < inputOrientationCount; o++ )
    {

      if ( scalarParameters[ 6 + o ] > 0 )
      {

        dwi = std::pow( scalarParameters[ 6 + o ], 1 / ( double )inputOrientationCount );
        meanDwi *= dwi;
//          dwi = scalarParameters[ 6 + o ] / ( double )inputOrientationCount;
//          meanDwi += dwi;

      }

    }
    item = ( float )meanDwi;

  }
  GKG_CATCH( "void gkg::OdfFunctorMeanDwi::processItem( "
             "float& item, "
             "const gkg::OrientationDistributionFunction& odf, "
             "const std::vector< double >& scalarParameters, "
             "const std::vector< std::string >& /*stringParameters*/ ) "
             "const" );

}


////////////////////////////////////////////////////////////////////////////////
// ODF functor registration
////////////////////////////////////////////////////////////////////////////////


static bool register_gkg_OdfFunctorMeanDwi __attribute__((unused)) =
gkg::OdfFunctorFactory< gkg::Volume< float > , float >::getInstance().
                                                           registerOdfFunctor(
                 gkg::OdfFunctorMeanDwi::getInstance() );
