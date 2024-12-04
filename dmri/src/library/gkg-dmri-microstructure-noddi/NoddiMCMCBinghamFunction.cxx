#include <gkg-dmri-microstructure-noddi/NoddiMCMCBinghamFunction.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-numericalanalysis/Matrix.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-processing-algobase/Math.h>
#include <gkg-core-exception/Exception.h>


gkg::NoddiMCMCBinghamFunction::NoddiMCMCBinghamFunction( 
                                gkg::RCPointer< gkg::Volume< float > > dw,
                                const gkg::OrientationSet& outputOrientationSet,
                                double noiseStandardDeviation )
                              : gkg::NoddiMCMCFunction( dw,
                                                        outputOrientationSet,
                                                        noiseStandardDeviation )
{
}


gkg::NoddiMCMCBinghamFunction::~NoddiMCMCBinghamFunction()
{
}


void gkg::NoddiMCMCBinghamFunction::getValuesAt( const gkg::Vector& parameters,
                                                 gkg::Vector& values ) const
{

  try
  {

    // pointing to parameters using references
    const double& isotropicFraction = parameters( 0 );
    const double& intracellularFraction = parameters( 1 );
    const double& kappa1 = parameters( 2 );
    const double& kappa2 = parameters( 3 );
    gkg::Vector3d< float > principalOrientation( ( double )parameters( 4 ),
                                                 ( double )parameters( 5 ),
                                                 ( double )parameters( 6 ) );
    gkg::Vector3d< float > secondaryOrientation( ( double )parameters( 7 ),
                                                 ( double )parameters( 8 ),
                                                 ( double )parameters( 9 ) );
    const double& parallelDiffusivity = parameters( 10 );
    const double& isotropicDiffusivity = parameters( 11 );
    // exvivo parameter for water trapped in glial cells; as in Alexander 2010,
    // stationary contribution can be considered as a pool of water molecules
    // trapped in a dot sphere (sphere of nul radius); and in this case, 
    // the diffusion attenuation si simply equal to A = 1
    const double& stationaryFraction = parameters( 12 );
    const double oneMinusStationaryFraction = 1.0 - stationaryFraction;

    // computing parallelDiffusivityPrime
    double tau1 = 1.0 / 3.0;
    if ( kappa1 > 0 )
    {

      tau1 = -1.0 / ( 2.0 * kappa1 ) +
             1.0 / ( 2.0 * this->_factory->getDawsonIntegral( 
                                                         std::sqrt( kappa1 ) ) *
                                                         std::sqrt( kappa1 ) );

    }
    //double tau2 = 1.0 / 3.0;
    //if ( kappa2 > 0 )
    //{

    //  tau2 = -1.0 / ( 2.0 * kappa2 ) +
    //         1.0 / ( 2.0 * this->_factory->getDawsonIntegral( 
    //                                                   std::sqrt( kappa2 ) ) *
    //                                                   std::sqrt( kappa2 ) );

    //}
    double parallelDiffusivityPrime = parallelDiffusivity *
                                      ( 1.0 - intracellularFraction *
                                        ( 1.0 - tau1 ) );
    double secondaryDiffusivityPrime = parallelDiffusivity *
                                           ( 1.0 - intracellularFraction *
                                             ( ( 1.0 + tau1 ) / 2.0 ) );
    double tertiaryDiffusivityPrime = parallelDiffusivity *
                                           ( 1.0 - intracellularFraction *
                                             ( ( 1.0 + tau1 ) / 2.0 ) );

    // computing the apparent extracellular diffusion tensor
    gkg::Matrix apparentExtracellularTensor( 3, 3 );
    this->getApparentExtracellularTensor( principalOrientation,
                                          secondaryOrientation,
                                          parallelDiffusivityPrime,
                                          secondaryDiffusivityPrime,
                                          tertiaryDiffusivityPrime,
                                          apparentExtracellularTensor );

    // looping over measures
    int32_t i = 0;
    int32_t o = 0;

    double intracellularContribution = 0.0;
    double extracellularContribution = 0.0;
    double csfContribution = 0.0;
    float* bDiDo = (float*)&_minusBiTimesSquareOfDiDotDo[ 0 ];

    for ( i = 0; i < this->_inputOrientationCount; i++ )
    {

      // creating input direction as a Vector
      gkg::Vector inputOrientation( this->_inputOrientations[ i ] );

      // computing restricted intracellular contribution
      intracellularContribution = 0.0;
      for ( o = 0; o < this->_outputOrientationCount; o++ )
      {

        intracellularContribution += 
          this->_factory->getBinghamDistribution(
                        kappa1,
                        kappa2,
                        principalOrientation,
                        secondaryOrientation,
                        this->_outputOrientations[ o ] ) *
          std::exp( *bDiDo++ * parallelDiffusivity );
                                           

      }
      intracellularContribution /= ( double )this->_outputOrientationCount;

      // computing hindered extracellular contribution
      extracellularContribution = 
          std::exp( -this->_bValues[ i ] * 
                    inputOrientation.getComposition(  
                      apparentExtracellularTensor.getComposition(
                                                         inputOrientation ) ) );


      // computing CSF contribution due to partial volume effect
      csfContribution = std::exp( -this->_bValues[ i ] * isotropicDiffusivity );

      values( i ) = ( 1.0 - isotropicFraction ) *
                    ( oneMinusStationaryFraction *
                    ( intracellularFraction * intracellularContribution +
                      ( 1.0 - intracellularFraction ) * 
                      extracellularContribution ) +
                      stationaryFraction ) +
                    isotropicFraction * csfContribution;

    }

  }
  GKG_CATCH( "void gkg::NoddiMCMCBinghamFunction::getValuesAt( "
             "const gkg::Vector& parameters, "
             "gkg::Vector& values ) const" );

}


void gkg::NoddiMCMCBinghamFunction::getModelAt(
         const gkg::Vector& /* parameters */,
         const std::vector< float >& /* bValues */,
         const std::vector< std::vector< float > >& /*gradientCharacteristics*/,
         int32_t /* inputOrientationCount */,
         std::vector< std::vector< float > >& /* values */,
         std::vector< float >& /* coordinates */ ) const
{

  try
  {

    throw std::runtime_error( "should not pass through this method" );

  }
  GKG_CATCH( "void gkg::NoddiMCMCBinghamFunction::getModelAt( "
             "const gkg::Vector& parameters, "
             "const std::vector< float >& bValues, "
             "int32_t inputOrientationCount, "
             "std::vector< std::vector< float > >& values, "
             "std::vector< float >& coordinates ) const" );

}


gkg::OrientationDistributionFunction::BasisType 
gkg::NoddiMCMCBinghamFunction::getType() const
{

  try
  {

    return gkg::OrientationDistributionFunction::NoddiBingham;

  }
  GKG_CATCH( "gkg::OrientationDistributionFunction::BasisType "
             "gkg::NoddiMCMCBinghamFunction::getType() const" );


}


int32_t gkg::NoddiMCMCBinghamFunction::getParameterCount() const
{

  try
  {

    return 13;

  }
  GKG_CATCH( "int32_t "
             "gkg::NoddiMCMCWatsonFunction::getParameterCount() const" );

}


void gkg::NoddiMCMCBinghamFunction::getApparentExtracellularTensor(
                            const gkg::Vector3d< float >& principalOrientation,
                            const gkg::Vector3d< float >& secondaryOrientation,
                            double parallelDiffusivityPrime,
                            double secondaryDiffusivityPrime,
                            double tertiaryDiffusivityPrime,
                            gkg::Matrix& apparentExtracellularTensor ) const
{

  try
  {

    if ( gkg::equal( principalOrientation.z, 1.0f, 1e-4f ) )
    {

      apparentExtracellularTensor.setZero();
      apparentExtracellularTensor( 0,0 ) = secondaryDiffusivityPrime;
      apparentExtracellularTensor( 1,1 ) = tertiaryDiffusivityPrime;
      apparentExtracellularTensor( 2,2 ) = parallelDiffusivityPrime;

    }
    else
    {

      // e1
      gkg::Vector e1( 3 );
      e1( 0 ) = ( double )principalOrientation.x;
      e1( 1 ) = ( double )principalOrientation.y;
      e1( 2 ) = ( double )principalOrientation.z;

      // e2
      gkg::Vector e2( 3 );
      e2( 0 ) = ( double )secondaryOrientation.x;
      e2( 1 ) = ( double )secondaryOrientation.y;
      e2( 2 ) = ( double )secondaryOrientation.z;

      // e3
      gkg::Vector e3 = e1.cross( e2 );

      // computing rotation matrix
      gkg::Matrix rotationMatrix( 3, 3 );
      rotationMatrix.setColumn( 0, e1 );
      rotationMatrix.setColumn( 1, e2 );
      rotationMatrix.setColumn( 2, e3 );

      // eigenvalue matrix
      gkg::Vector eigenValues( 3 );
      eigenValues( 0 ) = parallelDiffusivityPrime;
      eigenValues( 1 ) = secondaryDiffusivityPrime;
      eigenValues( 2 ) = tertiaryDiffusivityPrime;

      apparentExtracellularTensor = rotationMatrix.
                                    getComposition( eigenValues.getDiagonal() ).
                                    getComposition( 
                                            rotationMatrix.getTransposition() );

    }

  }
  GKG_CATCH( "void gkg::NoddiMCMCBinghamFunction::"
             "getApparentExtracellularTensor( "
             "const gkg::Vector3d< float >& principalOrientation, "
             "const gkg::Vector3d< float >& secondaruOrientation, "
             "double parallelDiffusivityPrime, "
             "double perpendicularDiffusivityPrime, "
             "gkg::Matrix& apparentExtracellularTensor ) const" );

}
   
