#include <gkg-dmri-odf-functor/OdfFunctorMeanDiffusivity.h>
#include <gkg-dmri-odf-functor/OdfFunctor_i.h>
#include <gkg-dmri-odf-functor/OdfFunctorFactory_i.h>
#include <gkg-processing-container/BoundingBox.h>
#include <gkg-processing-morphology/Neighborhood3d.h>
#include <gkg-core-exception/Exception.h>


gkg::OdfFunctorMeanDiffusivity::OdfFunctorMeanDiffusivity()
                               : gkg::OdfFunctor< gkg::Volume< float > ,
                                                  float >()
{
}


gkg::OdfFunctorMeanDiffusivity::~OdfFunctorMeanDiffusivity()
{
}


std::string gkg::OdfFunctorMeanDiffusivity::getName() const
{

  return "mean_diffusivity";

}


bool 
gkg::OdfFunctorMeanDiffusivity::isStandardBasisRequired() const
{

  return false;

}


void gkg::OdfFunctorMeanDiffusivity::process(
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
    const std::list< Vector3d< int32_t > >&
      sites = odfCartesianField.getSiteMap().getSites( 0 );

    // getting offset
    Vector3d< int32_t > 
      offset = odfCartesianField.getOffset();

    // getting pointer to T2 image
    const Volume< float >& t2Volume = odfCartesianField.getT2Volume();
    const Volume< float >& dwVolume = odfCartesianField.getDWVolume();
    int32_t inputOrientationCount = dwVolume.getSizeT();
    const std::vector< float >& bValues = odfCartesianField.getBValues();

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
    // <DW_count > <T2> <DW(0)> <b(0)> .... <DW(N-1)> <b(N-1)>
    std::vector< double > specificScalarParameters( 5 +
                                                    1 /*for DW count*/ +
                                                    1 /*for T2*/ +
                                                    2 * inputOrientationCount, 0 );
    specificScalarParameters[ 0 ] = scalarParameters[ 0 ];
    specificScalarParameters[ 1 ] = scalarParameters[ 1 ];
    specificScalarParameters[ 2 ] = scalarParameters[ 2 ];
    specificScalarParameters[ 3 ] = scalarParameters[ 3 ];
    specificScalarParameters[ 4 ] = scalarParameters[ 4 ];
    specificScalarParameters[ 5 ] = inputOrientationCount;

    int32_t o = 0;
    for ( o = 0; o  < inputOrientationCount; o++ )
    {

      specificScalarParameters[ 7 + 2 * o + 1 ] = bValues[ o ];

    }

    // looping over site(s) and processing output item(s)
    gkg::BoundingBox< int32_t > boundingBox( t2Volume );
    std::list< gkg::Vector3d< int32_t > >
      neighborOffsets = gkg::Neighborhood3d::getInstance().getNeighborOffsets(
                        gkg::Neighborhood3d::Neighborhood3d_26 );

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

      if ( t2Volume( *s ) > 0 )
      {

        specificScalarParameters[ 6 ] = ( double )t2Volume( *s );

      }
      else
      {

        double averageT2 = 0.0;
        std::list< gkg::Vector3d< int32_t > >::const_iterator
          no = neighborOffsets.begin(),
          noe = neighborOffsets.end();
        int32_t neighborCount = 0;
        gkg::Vector3d< int32_t > neighbor;
        while ( no != noe )
        {

          neighbor = *s + *no;
          if ( boundingBox.contains( neighbor ) )
          {

            averageT2 += ( double )t2Volume( neighbor );
            ++ neighborCount;

          }
          ++ no;

        }
        if ( neighborCount )
        {

          averageT2 /= ( double )neighborCount;

        }
        specificScalarParameters[ 6 ] = averageT2;

      }

      for ( o = 0; o  < inputOrientationCount; o++ )
      {

        specificScalarParameters[ 7 + 2 * o ] = ( double )dwVolume( *s, o );

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
  GKG_CATCH( "void gkg::OdfFunctorMeanDiffusivity::process( "
             "gkg::Volume< float >& output, "
             "gkg::OdfCartesianField& odfCartesianField, "
             "const std::vector< double >& scalarParameters, "
             "const std::vector< std::string >& stringParameters, "
             "bool verbose ) const" );


}


void gkg::OdfFunctorMeanDiffusivity::processItem(
                 float& item,
                 const gkg::OrientationDistributionFunction& /*odf*/,
                 const std::vector< double >& scalarParameters,
                 const std::vector< std::string >& /*stringParameters*/ ) const
{

  try
  {

    if ( scalarParameters[ 6 ] == 0.0 )
    {

      item = 0.0;

    }
    else
    {

      int32_t inputOrientationCount = ( int32_t )( scalarParameters[ 5 ] +
                                                   0.5 );

      int32_t o = 0;
      double diffusivity = 0.0;
      double meanDiffusivity = 0;
      for ( o = 0; o < inputOrientationCount; o++ )
      {

        if ( scalarParameters[ 7 + 2 * o ] > 0 )
        {

          diffusivity = -( 1.0 / ( scalarParameters[ 7 + 2 * o + 1 ] * 1e6 ) ) *
                        std::log( scalarParameters[ 7 + 2 * o ] /
                                  scalarParameters[ 6 ] );
          meanDiffusivity += diffusivity;

        }

      }
      item = ( float )meanDiffusivity / ( float )inputOrientationCount;
      if ( item < 0 )
      {

        item = 0;

      }

    }

  }
  GKG_CATCH( "void gkg::OdfFunctorMeanDiffusivity::processItem( "
             "float& item, "
             "const gkg::OrientationDistributionFunction& odf, "
             "const std::vector< double >& scalarParameters, "
             "const std::vector< std::string >& /*stringParameters*/ ) "
             "const" );

}


////////////////////////////////////////////////////////////////////////////////
// ODF functor registration
////////////////////////////////////////////////////////////////////////////////


static bool register_gkg_OdfFunctorMeanDiffusivity __attribute__((unused)) =
gkg::OdfFunctorFactory< gkg::Volume< float > , float >::getInstance().
                                                           registerOdfFunctor(
                 gkg::OdfFunctorMeanDiffusivity::getInstance() );
