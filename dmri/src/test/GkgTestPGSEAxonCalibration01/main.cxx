#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-io/TypedVolumeReaderProcess.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-numericalanalysis/Matrix.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-processing-coordinates/ElectrostaticOrientationSet.h>
#include <gkg-processing-numericalanalysis/PowellNewUOAFunction.h>
#include <gkg-processing-numericalanalysis/NelderMeadSimplexFunction.h>
#include <gkg-processing-numericalanalysis/NonLinearProgrammingFunction.h>
#include <gkg-processing-numericalanalysis/OptimizerConstraint.h>
#include <gkg-processing-algobase/Rescaler_i.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-exception/Exception.h>
#include <iostream>
#include <cmath>
#include <cstdio>
#include <fstream>
#include <vector>

#define GAMMA 2.675221900e8
#define PARAMETER_COUNT 8

#define ALPHA_MIN 0.0
#define ALPHA_MAX 1.0
#define ALPHA 0.2


class AxonMappingFunction : public gkg::PowellNewUOAFunction,
                            public gkg::NelderMeadSimplexFunction,
                            public gkg::NonLinearProgrammingFunction
{

  public:

    AxonMappingFunction(
      const std::vector< double >& realMeasurements,
      const std::vector< double >& littleDeltas,
      const std::vector< double >& bigDeltas,
      const std::vector< double >& riseTimes,
      const std::vector< double >& gradientMagnitudes,
      const std::vector< double >& bValues,
      const std::vector< gkg::Vector3d< float > >& inputOrientations,
      const std::vector< gkg::Vector3d< float > >& outputOrientations,
      double noiseVariance,
      int32_t maximumGPDCylinderSumIndex );
    virtual ~AxonMappingFunction();

    double getValueAt( const double* parameters, int32_t count ) const;
    double getValueAt( const gkg::Vector& parameters ) const;

    void activateConstraints();
    void deactivateConstraints();

    void activateDiameterCorrection();
    void deactivateDiameterCorrection();

  protected:

    double getLogLikelihood(
                       const std::vector< double >& currentMeasurements ) const;

    void computeCanonicalWatsonDecomposition(
                              double kappa,
                              int32_t maximumSphericalHarmonicsOrder,
                              gkg::Vector& canonicalWatsonDecomposition ) const;
  
    double computeIntracellularContribution(
                             const gkg::Vector& canonicalWatsonDecomposition,
                             int32_t maximumSphericalHarmonicsOrder,
                             double parallelDiffusivity,
                             double axonDiameter,
                             const gkg::Vector3d< float >& principalOrientation,
                             int32_t i ) const;
    double computePerpendicularAttenuationInCylinder(
                                                     double parallelDiffusivity,
                                                     double axonDiameter,
                                                     int32_t i ) const;
    double computeExtracellularContribution(
                         double bValue,
                         const double* apparentExtracellularTensor,
                         const gkg::Vector3d< float >& inputOrientation ) const;
    double computeC2n( int32_t n, double x ) const;

    gkg::NumericalAnalysisImplementationFactory* _factory;

    std::vector< double > _realMeasurements;
    std::vector< double > _littleDeltas;
    std::vector< double > _bigDeltas;
    std::vector< double > _riseTimes;
    std::vector< double > _gradientMagnitudes;
    std::vector< double > _bValues;
    std::vector< gkg::Vector3d< float > > _inputOrientations;
    std::vector< gkg::Vector3d< float > > _outputOrientations;
    double _noiseVariance;
    int32_t _maximumGPDCylinderSumIndex;

    double _twiceNoiseVariance;
    double _twoPi;
    int32_t _measurementCount;
    mutable std::vector< double > _currentMeasurements;

    bool _hasConstraints;
    bool _hasDiameterCorrection;

};


AxonMappingFunction::AxonMappingFunction( 
                const std::vector< double >& realMeasurements,
                const std::vector< double >& littleDeltas,
                const std::vector< double >& bigDeltas,
                const std::vector< double >& riseTimes,
                const std::vector< double >& gradientMagnitudes,
                const std::vector< double >& bValues,
                const std::vector< gkg::Vector3d< float > >& inputOrientations,
                const std::vector< gkg::Vector3d< float > >& outputOrientations,
                double noiseVariance,
                int32_t maximumGPDCylinderSumIndex )
                    : gkg::PowellNewUOAFunction( PARAMETER_COUNT ),
                      gkg::NelderMeadSimplexFunction( PARAMETER_COUNT ),
                      gkg::NonLinearProgrammingFunction( PARAMETER_COUNT,
                                                         true ),
                      _factory( 
                         gkg::NumericalAnalysisSelector::getInstance().
                                                   getImplementationFactory() ),
                      _realMeasurements( realMeasurements ),
                      _littleDeltas( littleDeltas ),
                      _bigDeltas( bigDeltas ),
                      _riseTimes( riseTimes ),
                      _gradientMagnitudes( gradientMagnitudes ),
                      _bValues( bValues ),
                      _inputOrientations( inputOrientations ),
                      _outputOrientations( outputOrientations ),
                      _noiseVariance( noiseVariance ),
                      _maximumGPDCylinderSumIndex( maximumGPDCylinderSumIndex ),
                      _twiceNoiseVariance( noiseVariance * 2.0 ),
                      _twoPi( 2.0 * M_PI ),
                      _measurementCount( ( int32_t )realMeasurements.size() ),
                      _currentMeasurements( realMeasurements.size() ),
                      _hasConstraints( false ),
                      _hasDiameterCorrection( false )
{
}


AxonMappingFunction::~AxonMappingFunction()
{
}


double AxonMappingFunction::getValueAt( const gkg::Vector& parameters ) const
{

  try
  {

    return this->getValueAt( &parameters( 0 ), parameters.getSize() );

  }
  GKG_CATCH( "double AxonMappingFunction::getValueAt( "
             "const gkg::Vector& parameters ) const" );

}


double AxonMappingFunction::getValueAt( const double* parameters,
                                        int32_t /* count */ ) const
{

  try
  {

    //  0     1      2         3     4      5       6         7
    // fiso Diso Dextra_perp fstat fintra kappa Dparallel axonDiameter

    static double constrainedParameters[ PARAMETER_COUNT ];

    double isotropicFraction = 0.0;
    double isotropicDiffusivity = 0.0;
    double extraPerpendicularDiffusivityCorrection = 0.0;
    double stationaryFraction = 0.0;
    double intracellularFraction = 0.0;
    double kappa = 0.0;
    double parallelDiffusivity = 0.0;
    double axonDiameter = 0.0;

    if ( _hasConstraints )
    {

      this->applyConstraints( parameters, PARAMETER_COUNT,
                              constrainedParameters );

      gkg::Rescaler< double, double >
        rescalerExtraPerpendicularDiffusivityCorrection( -1.0, 1.0,
                                                         ALPHA_MIN, ALPHA_MAX );

      gkg::Rescaler< double, double >
        rescalerFintra( -1.0, 1.0, 0.0, 1.0 );
      gkg::Rescaler< double, double >
        rescalerKappa( -1.0, 1.0, 1.0, 200.0 );
      gkg::Rescaler< double, double >
        rescalerDParallel( -1.0, 1.0, 1.0e-9, 3.0e-9 );
      gkg::Rescaler< double, double >
        rescalerDiameter( -1.0, 1.0, 0.1e-6, 10.0e-6 );

      isotropicFraction = 0.0;
      isotropicDiffusivity = 0.0;
      extraPerpendicularDiffusivityCorrection = 0.0;
      stationaryFraction = 0.0;

      if ( _hasDiameterCorrection )
      {

        rescalerExtraPerpendicularDiffusivityCorrection.rescale(
                                  constrainedParameters[ 2 ],
                                  extraPerpendicularDiffusivityCorrection );

      }

      rescalerFintra.rescale( constrainedParameters[ 4 ],
                              intracellularFraction );
      rescalerKappa.rescale( constrainedParameters[ 5 ],
                             kappa );
      rescalerDParallel.rescale( constrainedParameters[ 6 ],
                                 parallelDiffusivity );
      rescalerDiameter.rescale( constrainedParameters[ 7 ],
                                axonDiameter );

    }
    else
    {

      isotropicFraction = parameters[ 0 ];
      isotropicDiffusivity = parameters[ 1 ];
      extraPerpendicularDiffusivityCorrection = parameters[ 2 ];
      stationaryFraction = parameters[ 3 ];
      intracellularFraction = parameters[ 4 ];
      kappa = parameters[ 5 ];
      parallelDiffusivity = parameters[ 6 ];
      axonDiameter = parameters[ 7 ];

    }

    std::cout << "fiso=" << isotropicFraction << " ";
    std::cout << "Diso=" << isotropicDiffusivity << " ";
    std::cout << "corr_Dextra_perp=" << extraPerpendicularDiffusivityCorrection
              << " ";
    std::cout << "fstat=" << stationaryFraction << " ";
    std::cout << "fintra=" << intracellularFraction << " ";
    std::cout << "kappa=" << kappa << " ";
    std::cout << "D//=" << parallelDiffusivity << " ";
    std::cout << "dia=" << axonDiameter << std::endl;


    gkg::Vector3d< float > principalOrientation( 0.0, 0.0, 1.0 );

    double isotropicContribution = 0.0;
    double intracellularContribution = 0.0;
    double extracellularContribution = 0.0;
    double stationaryContribution = 1.0;

    // computing extra tensor
    double tau1 = 1.0 / 3.0;
    if ( kappa > 0.0 )
    {

      double sqrtKappa = std::sqrt( kappa );
      
      tau1 = -0.5 / kappa +
             0.5 / ( this->_factory->getDawsonIntegral( sqrtKappa ) *
                     sqrtKappa );

    }

    double parallelDiffusivityPrime = parallelDiffusivity *
                                      ( 1.0 - intracellularFraction *
                                        ( 1.0 - tau1 ) );
    double perpendicularDiffusivityPrime = parallelDiffusivity *
                                          ( 1.0f - intracellularFraction *
                                            ( ( 1.0f + tau1 ) / 2.0f ) );


    gkg::Vector canonicalWatsonDecomposition;
    this->computeCanonicalWatsonDecomposition( kappa,
                                               12, // maximum sph. harm order
                                               canonicalWatsonDecomposition );
                                               

    int32_t i = 0;
    for ( i = 0; i < _measurementCount; i++ )
    {


      // computing the apparent extracellular diffusion tensor

      double perpendicularDiffusivityPrimeCorrection =  
                             extraPerpendicularDiffusivityCorrection * 
                             ( 0.37 * axonDiameter ) *
                             ( 0.37 * axonDiameter ) *
                             ( std::log( _bigDeltas[ i ] / 
                                         _littleDeltas[ i ] ) + 3.0 / 2.0 ) / 
                             ( _bigDeltas[ i ] - _littleDeltas[ i ] / 3.0 );

      double apparentExtracellularTensor[ 6 ];
      apparentExtracellularTensor[ 0 ] = perpendicularDiffusivityPrime +
                                        perpendicularDiffusivityPrimeCorrection;
      apparentExtracellularTensor[ 1 ] = 0.0;
      apparentExtracellularTensor[ 2 ] = 0.0;
      apparentExtracellularTensor[ 3 ] = perpendicularDiffusivityPrime +
                                        perpendicularDiffusivityPrimeCorrection;
      apparentExtracellularTensor[ 4 ] = 0.0;
      apparentExtracellularTensor[ 5 ] = parallelDiffusivityPrime;


      isotropicContribution = std::exp( -_bValues[ i ] * isotropicDiffusivity );
      intracellularContribution = this->computeIntracellularContribution(
                                    canonicalWatsonDecomposition,
                                    12, // maximum sph. harmonics order
                                    parallelDiffusivity,
                                    axonDiameter,
                                    principalOrientation,
                                    i
                                    );
      extracellularContribution = this->computeExtracellularContribution(
                                      _bValues[ i ],
                                      apparentExtracellularTensor,
                                      _inputOrientations[ i ] );

      stationaryContribution = 1.0;

      _currentMeasurements[ i ] =
                    ( 1.0 - isotropicFraction ) *
                    ( ( 1.0 - stationaryFraction ) *
                      ( intracellularFraction *
                        intracellularContribution +
                        ( 1.0 - intracellularFraction ) * 
                        extracellularContribution ) +
                      stationaryFraction * stationaryContribution ) +
                    isotropicFraction * isotropicContribution;


    }

    double minusLogLikelihood = -this->getLogLikelihood( _currentMeasurements );
    std::cout << "minusLogLikelihood=" << minusLogLikelihood << std::endl;
    return minusLogLikelihood;

  }
  GKG_CATCH( "double gkg::AxonMappingFunction::getValueAt( "
             "const double* parameters, "
             "int32_t count ) const" );


}


void AxonMappingFunction::activateConstraints()
{

  try
  {

    _hasConstraints = true;

  }
  GKG_CATCH( "bool AxonMappingFunction::activateConstraints()" );

}


void AxonMappingFunction::deactivateConstraints()
{

  try
  {

    _hasConstraints = false;

  }
  GKG_CATCH( "bool AxonMappingFunction::deactivateConstraints()" );

}


void AxonMappingFunction::activateDiameterCorrection()
{

  try
  {

    _hasDiameterCorrection = true;

  }
  GKG_CATCH( "bool AxonMappingFunction::activateDiameterCorrection()" );

}


void AxonMappingFunction::deactivateDiameterCorrection()
{

  try
  {

    _hasDiameterCorrection = false;

  }
  GKG_CATCH( "bool AxonMappingFunction::deactivateDiameterCorrection()" );

}


double AxonMappingFunction::getLogLikelihood(
                  const std::vector< double >& /* currentMeasurements */ ) const
{

  try
  {
/*
    double logLikelihood = 0.0;
    double a = 0.0;

    int32_t m = 0;
    for ( m = 0; m < _measurementCount; m++ )
    {

      a = ( _realMeasurements[ m ] * _currentMeasurements[ m ] ) /
            _noiseVariance;

      if ( a < 700.0 )
      {

        logLikelihood += 
                     std::log( _currentMeasurements[ m ] *
		     _factory->getRegularBesselFunctionI0( a ) / 
		     _noiseVariance ) -
                     ( _currentMeasurements[ m ] * _currentMeasurements[ m ] +
                       _realMeasurements[ m ] * _realMeasurements[ m ] ) /
                     _twiceNoiseVariance;

      }
      else
      {

       logLikelihood += 
                     std::log( _currentMeasurements[ m ] / _noiseVariance /
		     std::sqrt( _twoPi * a ) ) -
                     ( _currentMeasurements[ m ] * _currentMeasurements[ m ] +
                       _realMeasurements[ m ] * _realMeasurements[ m ] ) /
                     _twiceNoiseVariance + a;

      }

    }
*/

    double logLikelihood = 0.0;
    int32_t m = 0;
    for ( m = 0; m < _measurementCount; m++ )
    {

      logLikelihood += -std::log( std::sqrt( _noiseVariance ) *
                                   std::sqrt( _twoPi ) ) -
                        ( ( _currentMeasurements[ m ] -
                            _realMeasurements[ m ] ) *
                          ( _currentMeasurements[ m ] -
                            _realMeasurements[ m ] ) ) / 
                        ( 2.0 * _noiseVariance );
    
    }

    return logLikelihood;

  }
  GKG_CATCH( "double gkg::AxonMappingFunction::getLogLikelihood( "
             "const std::vector< double >& currentMeasurements ) const" );

}


void AxonMappingFunction::computeCanonicalWatsonDecomposition(
                             double kappa,
                             int32_t maximumSphericalHarmonicsOrder,
                             gkg::Vector& canonicalWatsonDecomposition ) const
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // computing canonical Watson spherical decomposition
    ////////////////////////////////////////////////////////////////////////////

    int32_t outputOrientationCount = ( int32_t )_outputOrientations.size();

    // allocating real spherical harmonics B matrix
    gkg::Matrix B( outputOrientationCount, maximumSphericalHarmonicsOrder + 1 );

    // looping over orientation(s)
    int32_t o = 0;
    int32_t s = 0;
    double radial = 0.0;
    double theta = 0.0;
    double phi = 0.0;
    for ( o = 0; o < outputOrientationCount; o++ )
    {

      // obtaining a reference to the current orientation
      const gkg::Vector3d< float >& 
        orientation = _outputOrientations[ o ];

      // converting cartesian coordinate(s) to spherical coordinate(s)
      _factory->getCartesianToSphericalCoordinates( ( double )orientation.x,
                                                    ( double )orientation.y,
                                                    ( double )orientation.z,
                                                    radial,
                                                    theta,
                                                    phi );

      // looping over spherical harmonic(s)
      for ( s = 0; s <= maximumSphericalHarmonicsOrder; s++ )
      {

        B( o, s ) = 
                std::real( _factory->getSphericalHarmonic( s, 0, phi, theta ) );

      }

    }

    // computing the matrix for spherical harmonics decomposition
    gkg::Matrix Bplus;
    _factory->getMoorePenrosePseudoInverse( B, Bplus );

    // allocating the watson distribution vector
    gkg::Vector watsonDistribution( outputOrientationCount );

    // filling principal orientation vector
    gkg::Vector3d< float > principalOrientation( 0.0, 0.0, 1.0 );

    //  computing values of canonical Watson distribution along provided
    // orientations for current kappa
    double sum = 0.0;
    for ( o = 0; o < outputOrientationCount; o++ )
    {

      watsonDistribution( o ) = _factory->getWatsonDistribution(
                                     kappa,
                                     principalOrientation,
                                     _outputOrientations[ o ] );
      sum += watsonDistribution( o );

    }
    /*for ( o = 0; o < outputOrientationCount; o++ )
    {

      watsonDistribution( o ) /= sum;

    }*/
    canonicalWatsonDecomposition = Bplus.getComposition( watsonDistribution );

  }
  GKG_CATCH( "void AxonMappingFunction::computeCanonicalWatsonDecomposition( "
             "double kappa, "
             "int32_t maximumSphericalHarmonicsOrder, "
             "gkg::Vector& canonicalWatsonDecomposition ) const" );

}


double AxonMappingFunction::computeIntracellularContribution(
                             const gkg::Vector& canonicalWatsonDecomposition,
                             int32_t maximumSphericalHarmonicsOrder,
                             double parallelDiffusivity,
                             double axonDiameter,
                             const gkg::Vector3d< float >& principalOrientation,
                             int32_t i ) const
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // computing parallel L
    ////////////////////////////////////////////////////////////////////////////

    double parallelL = _bValues[ i ] * parallelDiffusivity;

    ////////////////////////////////////////////////////////////////////////////
    // computing perpendicular L
    ////////////////////////////////////////////////////////////////////////////

    double perpendicularL =
                   - _gradientMagnitudes[ i ] * _gradientMagnitudes[ i ] *
                     this->computePerpendicularAttenuationInCylinder(
                                                         parallelDiffusivity,
                                                         axonDiameter,
                                                         i );

    double dotProduct = ( double )principalOrientation.x *
                        ( double )_inputOrientations[ i ].x +
                        ( double )principalOrientation.y *
                        ( double )_inputOrientations[ i ].y +
                        ( double )principalOrientation.z *
                        ( double )_inputOrientations[ i ].z;

    double intracellularContribution = 0.0;
    int32_t l = 0;
    for ( l = 0; l <= maximumSphericalHarmonicsOrder; l += 2 )
    {

      intracellularContribution += 
        canonicalWatsonDecomposition( l ) * 
        std::sqrt( ( 2.0 * l + 1.0 ) / ( 4.0 * M_PI ) ) *
        this->_factory->getLegendrePolynomial(
                                 l,
                                 dotProduct ) *
        this->computeC2n( l / 2, parallelL - perpendicularL );

    }

    intracellularContribution *= 0.5 * std::exp( -perpendicularL );

    return intracellularContribution;

  }
  GKG_CATCH( "double AxonMappingFunction::computeIntracellularContribution( "
             "const gkg::Vector& canonicalWatsonDecomposition, "
             "int32_t maximumSphericalHarmonicsOrder, "
             "double parallelDiffusivity, "
             "double axonDiameter, "
             "const gkg::Vector3d< float >& principalOrientation, "
             "int32_t i ) const" );

}


double AxonMappingFunction::computePerpendicularAttenuationInCylinder(
                                                     double parallelDiffusivity,
                                                     double axonDiameter,
                                                     int32_t i ) const
{

  try
  {

    double sum = 0.0;
    double axonRadius = axonDiameter / 2.0;
    double squareOfAxonRadius = axonRadius * axonRadius;
    double squareOfParallelDiffusivity = parallelDiffusivity * 
                                         parallelDiffusivity;

    int32_t m = 0;

    // _maximumGPDCylinderSumIndex = 20
    for ( m = 1; m <= _maximumGPDCylinderSumIndex; m++ )
    {

      // collecting the mth zero 'alpha_m' of the derivative of J1 by solving
      //     alpha_m x J1( alpha_m * axon_diameter )' = 0
      double besselRoot = 
                         this->_factory->getZeroBesselDerivativeFunctionJ1( m );

      double squareOfBesselRoot = besselRoot * besselRoot;

      double B_n = 2.0 * ( squareOfAxonRadius / squareOfBesselRoot ) /
                   ( squareOfBesselRoot  - 1.0 );

      double lambda_n = squareOfBesselRoot / squareOfAxonRadius;

      double squareOfLambdaN = lambda_n * lambda_n;

      double littleDelta_n_tilde = parallelDiffusivity * 
                                   lambda_n * 
                                   _littleDeltas[ i ];

      double bigDelta_n_tilde = parallelDiffusivity * 
                                lambda_n * 
                                _bigDeltas[ i ]; 

      double gamma_n = lambda_n * parallelDiffusivity *
                       _littleDeltas[ i ] - 1.0 +
                       std::exp( - littleDelta_n_tilde ) +
                       std::exp( -bigDelta_n_tilde ) -
                       0.5 * std::exp( 
                                 littleDelta_n_tilde - bigDelta_n_tilde ) -
                       0.5 * std::exp( 
                                -littleDelta_n_tilde - bigDelta_n_tilde );  

      sum += B_n * gamma_n / squareOfLambdaN;

    }

    return -2.0 * GAMMA * GAMMA * sum / squareOfParallelDiffusivity;

  }
  GKG_CATCH( "double AxonMappingFunction::"
             "computePerpendicularAttenuationInCylinder( "
             "double parallelDiffusivity, "
             "double axonDiameter, "
             "int32_t i ) const" );

}



double AxonMappingFunction::computeC2n( int32_t n, double x ) const
{

  try
  {

    double a = ( double )n + 0.5;
    double b = 2.0 * ( double )n + 1.5;
    return std::pow( -x, n ) *
           ( _factory->getGammaFunction( a ) /
             _factory->getGammaFunction( b ) ) *
           _factory->getHypergeometricFunction1F1( a, b, -x );

  }
  GKG_CATCH( "double AxonMappingFunction::computeC2n( "
             "int32_t n, double x ) const" );

}


double AxonMappingFunction::computeExtracellularContribution(
                          double bValue,
                          const double* apparentExtracellularTensor,
                          const gkg::Vector3d< float >& inputOrientation ) const
{

  try
  {

    return std::exp( -bValue * ( inputOrientation.x * inputOrientation.x * 
                                 apparentExtracellularTensor[ 0 ] +
                                 2.0 * inputOrientation.x * inputOrientation.y *
                                 apparentExtracellularTensor[ 1 ] +
                                 2.0 * inputOrientation.x * inputOrientation.z *
                                 apparentExtracellularTensor[ 2 ] +
                                 inputOrientation.y * inputOrientation.y * 
                                 apparentExtracellularTensor[ 3 ] +
                                 2.0 * inputOrientation.y * inputOrientation.z *
                                 apparentExtracellularTensor[ 4 ] +
                                 inputOrientation.z * inputOrientation.z * 
                                 apparentExtracellularTensor[ 5 ] ) );

  }
  GKG_CATCH( "double AxonMappingFunction::"
             "computeExtracellularContribution( "
             "double bValue, "
             "const double* apparentExtracellularTensor, "
             "const gkg::Vector3d< float >& inputOrientation) const" );

}


int main( int32_t argc, char* argv[] )
{


  int result = EXIT_SUCCESS;

  try
  {


    ////////////////////////////////////////////////////////////////////////////
    // creating application to load plugins
    ////////////////////////////////////////////////////////////////////////////

    gkg::Application application( argc, argv, "Test axonal calibration" );
    application.initialize();


    ////////////////////////////////////////////////////////////////////////////
    // reading T2 and DW(s)
    ////////////////////////////////////////////////////////////////////////////

    gkg::Volume< float > t2;
    gkg::TypedVolumeReaderProcess< float > t2VolumeReaderProcess( t2 );
    t2VolumeReaderProcess.execute( "t2.ima" );
    std::cout << "T2 read" << std::endl;

    int32_t shellCount = 4;
    std::vector< gkg::Volume< float > > dws( shellCount );

    int32_t s = 0;
    for ( s = 0; s < shellCount; s++ )
    {

      gkg::TypedVolumeReaderProcess< float > dwVolumeReaderProcess( dws[ s ] );
      dwVolumeReaderProcess.execute( std::string( "dw" ) +
                                     gkg::StringConverter::toString( s + 1 ) +
                                     ".ima" );
      std::cout << "DW Shell "
                << gkg::StringConverter::toString( s + 1 )
                << " read" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // computing b-values
    ////////////////////////////////////////////////////////////////////////////

    std::vector< double > littleDeltas( shellCount );
    std::vector< double > bigDeltas( shellCount );
    std::vector< double > gradientMagnitudes( shellCount );
    std::vector< double > riseTimes( shellCount );
    std::vector< double > bValues( shellCount );

    littleDeltas[ 0 ] = 0.012;   // s
    littleDeltas[ 1 ] = 0.015;
    littleDeltas[ 2 ] = 0.005;
    littleDeltas[ 3 ] = 0.013;

    bigDeltas[ 0 ] = 0.080;    // s
    bigDeltas[ 1 ] = 0.077;
    bigDeltas[ 2 ] = 0.087;
    bigDeltas[ 3 ] = 0.020;

    gradientMagnitudes[ 0 ] = 0.058;  // T
    gradientMagnitudes[ 1 ] = 0.046;
    gradientMagnitudes[ 2 ] = 0.057;
    gradientMagnitudes[ 3 ] = 0.060;


    riseTimes[ 0 ] = 0.0; // s
    riseTimes[ 1 ] = 0.0;
    riseTimes[ 2 ] = 0.0;
    riseTimes[ 3 ] = 0.0;

    for ( s = 0; s < shellCount; s++ )
    {

      bValues[ s ] = GAMMA * GAMMA *
                        gradientMagnitudes[ s ] * gradientMagnitudes[ s ] *
                        ( ( littleDeltas[ s ] - riseTimes[ s ] ) *
                          ( littleDeltas[ s ] - riseTimes[ s ] ) *
                          ( bigDeltas[ s ] - ( 1.0 / 3.0 ) * 
                            ( littleDeltas[ s ] - riseTimes[ s ] ) ) -
                          ( 1.0 / 6.0 ) *  
                          ( littleDeltas[ s ] - riseTimes[ s ] ) *
                          riseTimes[ s ] * riseTimes[ s ] +
                          ( 1.0 / 30.0 ) *
                          riseTimes[ s ] * riseTimes[ s ] * riseTimes[ s ] );

      std::cout << "bValues[ " << s << " ] = "
                << bValues[ s ]
                << " s/m^2"
                << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // creating input orientation count
    ////////////////////////////////////////////////////////////////////////////

    int32_t inputOrientationCount = 400;
    gkg::ElectrostaticOrientationSet
      inputOrientationSet( inputOrientationCount );
    std::cout << "input orientation set created: "
              << inputOrientationSet.getOrientation( 0 )
              << std::endl;

    ////////////////////////////////////////////////////////////////////////////
    // creating output orientation count
    ////////////////////////////////////////////////////////////////////////////


    gkg::OrientationSet 
      outputOrientationSet( gkg::ElectrostaticOrientationSet( 
                 2 * inputOrientationCount / 2 ).getSymmetricalOrientations() );
    std::cout << "input orientation set created: "
              << outputOrientationSet.getOrientation( 0 ) << " / "
              << outputOrientationSet.getOrientation( inputOrientationCount )
              << " / "
              << std::endl;

    ////////////////////////////////////////////////////////////////////////////
    // creating signal attenuation
    ////////////////////////////////////////////////////////////////////////////

    std::vector< double > 
      allSignalAttenuations( shellCount * inputOrientationCount );
    std::vector< double > 
      allLittleDeltas( shellCount * inputOrientationCount );
    std::vector< double > 
      allBigDeltas( shellCount * inputOrientationCount );
    std::vector< double > 
      allRiseTimes( shellCount * inputOrientationCount );
    std::vector< double > 
      allGradientMagnitudes( shellCount * inputOrientationCount );
    std::vector< double > 
      allBValues( shellCount * inputOrientationCount );
    std::vector< gkg::Vector3d< float > > 
      allInputOrientations( shellCount * inputOrientationCount );

    std::cout << "creation of signal attenuation" << std::endl;
    int32_t o = 0;
    for ( s = 0; s < shellCount; s++ )
    {

      for ( o = 0; o < inputOrientationCount; o++ )
      {

        allSignalAttenuations[ s * inputOrientationCount + o ] =
          dws[ s ]( 0, 0, 0, o ) / t2( 0, 0, 0 );

        allLittleDeltas[ s * inputOrientationCount + o ] = littleDeltas[ s ];
        allBigDeltas[ s * inputOrientationCount + o ] = bigDeltas[ s ];
        allRiseTimes[ s * inputOrientationCount + o ] = riseTimes[ s ];
        allGradientMagnitudes[ s * inputOrientationCount + o ] = 
                                                        gradientMagnitudes[ s ];
        allBValues[ s * inputOrientationCount + o ] =  bValues[ s ];
        allInputOrientations[ s * inputOrientationCount + o ] = 
                                        inputOrientationSet.getOrientation( o );

        /*
        std::cout << "allSignalAttenuations[ " << s * inputOrientationCount + o 
                  << " ] = "
                  << allSignalAttenuations[ s * inputOrientationCount + o ]
                  << " "
                  << "allLittleDeltas[ " << s * inputOrientationCount + o 
                  << " ] = "
                  << allLittleDeltas[ s * inputOrientationCount + o ]
                  << " "
                  << "allBigDeltas[ " << s * inputOrientationCount + o 
                  << " ] = "
                  << allBigDeltas[ s * inputOrientationCount + o ]
                  << " "
                  << "allRiseTimes[ " << s * inputOrientationCount + o 
                  << " ] = "
                  << allRiseTimes[ s * inputOrientationCount + o ]
                  << " "
                  << "allGradientMagnitudes[ " << s * inputOrientationCount + o 
                  << " ] = "
                  << allGradientMagnitudes[ s * inputOrientationCount + o ]
                  << " "
                  << "allBValues[ " << s * inputOrientationCount + o 
                  << " ] = "
                  << allBValues[ s * inputOrientationCount + o ]
                  << " "
                  << "allInputOrientations[ " << s * inputOrientationCount + o 
                  << " ] = "
                  << allInputOrientations[ s * inputOrientationCount + o ]
                  << std::endl;*/

      }

    }


    ////////////////////////////////////////////////////////////////////////////
    // creating axon mapping NewUOA function
    ////////////////////////////////////////////////////////////////////////////

    double noiseVariance = 0.001;
    AxonMappingFunction axonMappingFunction( 
                                         allSignalAttenuations,
                                         allLittleDeltas,
                                         allBigDeltas,
                                         allRiseTimes,
                                         allGradientMagnitudes,
                                         allBValues,
                                         allInputOrientations,
                                         outputOrientationSet.getOrientations(),
                                         noiseVariance,
                                         20 );
    std::cout << "axon mapping function created" << std::endl;


    ////////////////////////////////////////////////////////////////////////////
    // optimizing axon mapping parameters
    ////////////////////////////////////////////////////////////////////////////

    // getting numerical analysis implementation factory
    gkg::NumericalAnalysisImplementationFactory*
      factory = gkg::NumericalAnalysisSelector::getInstance().
                                                     getImplementationFactory();

    ////////////////////////////////////////////////////////////////////////////
    // QUICK SEARCH
    ////////////////////////////////////////////////////////////////////////////

    std::cout << "======================================================"
              << std::endl
              << "Quick Search"
              << std::endl
              << "======================================================"
              << std::endl;

    axonMappingFunction.deactivateConstraints();

    gkg::Vector lowerParameters( 8 );
    lowerParameters( 0 ) = 0.0;
    lowerParameters( 1 ) = 0.0;
    lowerParameters( 2 ) = 0.0;
    lowerParameters( 3 ) = 0.0;
    lowerParameters( 4 ) = 0.0;
    lowerParameters( 5 ) = 200;
    lowerParameters( 6 ) = 1.0e-9;
    lowerParameters( 7 ) = 5.0e-6;


    gkg::Vector upperParameters( 8 );
    upperParameters( 0 ) = 0.0;
    upperParameters( 1 ) = 0.0;
    upperParameters( 2 ) = 0.0;
    upperParameters( 3 ) = 0.0;
    upperParameters( 4 ) = 1.0;
    upperParameters( 5 ) = 200;
    upperParameters( 6 ) = 3.0e-9;
    upperParameters( 7 ) = 5.0e-6;


    int32_t quicksearchStepCount1 = 0;  //steps for perp diffusivity
    int32_t quicksearchStepCount2 = 10;  //steps for volume fraction 
    int32_t quicksearchStepCount3 = 10;  //steps for par diffusivity
    int32_t quicksearchStepCount4 = 0;  //steps for diameter

    gkg::Vector QSparameters( PARAMETER_COUNT );
    double optimalP2 = 0.0;
    double optimalP3 = 0.0;
    double optimalP4 = 0.0;
    double optimalMinusLogLikelihood = 1e38;
    bool applyQuickSearchOptimizer = true;
    if ( applyQuickSearchOptimizer )
    {

      int32_t p1 = 0;
      int32_t p2 = 0;
      int32_t p3 = 0;
      int32_t p4 = 0;
      double minusLogLikelihood = 0.0;

      double delta0 = 0.0;
      double delta1 = 0.0;
      double delta2 = 0.0;
      double delta3 = 0.0;

      if ( quicksearchStepCount1 > 0 )
      {

        delta0 = ( upperParameters( 2 ) - lowerParameters( 2 ) ) /
                              ( double )quicksearchStepCount1;

      }

      if ( quicksearchStepCount2 > 0 )
      {

        delta1 = ( upperParameters( 4 ) - lowerParameters( 4 ) ) /
                            ( double )quicksearchStepCount2;

      }

      if ( quicksearchStepCount3 > 0 )
      {

        delta2 = ( upperParameters( 6 ) - lowerParameters( 6 ) ) /
                            ( double )quicksearchStepCount3;

      }

      if ( quicksearchStepCount4 > 0 )
      {

        delta3 = ( upperParameters( 7 ) - lowerParameters( 7 ) ) /
                            ( double )quicksearchStepCount4;

      }

      QSparameters( 5 ) = lowerParameters( 5 );

      for ( p4 = 0; p4 <= quicksearchStepCount4; p4++ )
      {

        QSparameters( 7 ) = lowerParameters( 7 ) + p4 * delta3; 

        for ( p3 = 0; p3 <= quicksearchStepCount3; p3++ )
        {

          QSparameters( 6 ) = lowerParameters( 6 ) + p3 * delta2;

          for ( p2 = 0; p2 <= quicksearchStepCount2; p2++ )
          {

            QSparameters( 4 ) = lowerParameters( 4 ) + p2 * delta1;

            for ( p1 = 0; p1 <= quicksearchStepCount1; p1++ )
            {

              QSparameters( 2 ) = lowerParameters( 2 ) + p1 * delta0;

              minusLogLikelihood = axonMappingFunction.getValueAt( 
                                                                QSparameters );

              std::cout << "/////////////////////////////////////////////\n" 
                        << std::endl;

              if ( minusLogLikelihood < optimalMinusLogLikelihood )
              {

                optimalP2 = QSparameters( 4 );
                optimalP3 = QSparameters( 6 );
                optimalP4 = QSparameters( 7 );
                optimalMinusLogLikelihood = minusLogLikelihood;

              }

            }

          }

        }

      }

    }

    std::cout << "optimalMinusLogLikelihood: " 
              << optimalMinusLogLikelihood << std::endl;

    std::cout << "optimum parameters: " 
              << optimalP2 
              << " " 
              << optimalP3 
              << " "
              << optimalP4 
              << " "                            
              << std::endl;

    ////////////////////////////////////////////////////////////////////////////
    // POWELL NEWUOA
    ////////////////////////////////////////////////////////////////////////////

    std::cout << "======================================================"
              << std::endl
              << "Constrained Powell NewUOA"
              << std::endl
              << "======================================================"
              << std::endl;

    axonMappingFunction.activateConstraints();

    std::vector< gkg::RCPointer< gkg::OptimizerConstraint > > 
      constraints( PARAMETER_COUNT );

    ////////////////////////////////////////////////////////////////////////////
    // FIRST STEP
    ////////////////////////////////////////////////////////////////////////////

    std::cout << "First step" << std::endl;
    std::cout << "------------------------------------------------------"
              << std::endl;

    gkg::Rescaler< double, double >
      rescalerFintraConstraint( 0.0, 1.0, -1.0, 1.0 );
    gkg::Rescaler< double, double >
      rescalerKappaConstraint( 1.0, 200.0, -1.0, 1.0 );
    gkg::Rescaler< double, double >
      rescalerDParallelConstraint( 1.0e-9, 3.0e-9, -1.0, 1.0 );
    gkg::Rescaler< double, double >
      rescalerDiameterConstraint( 0.1e-6, 10.0e-6, -1.0, 1.0 );

    lowerParameters( 0 ) = 0.0;
    lowerParameters( 1 ) = 0.0;
    lowerParameters( 2 ) = 0.0;
    lowerParameters( 3 ) = 0.0;
    lowerParameters( 4 ) = optimalP2 - 0.1;
    lowerParameters( 5 ) = 1;
    lowerParameters( 6 ) = optimalP3 - 0.5e-9;
    lowerParameters( 7 ) = 5.0e-6;

    upperParameters( 0 ) = 0.0;
    upperParameters( 1 ) = 0.0;
    upperParameters( 2 ) = 0.0;
    upperParameters( 3 ) = 0.0;
    upperParameters( 4 ) = optimalP2 + 0.1;
    upperParameters( 5 ) = 200;
    upperParameters( 6 ) = optimalP3 + 0.5e-9;
    upperParameters( 7 ) = 5.0e-6;

    rescalerFintraConstraint.rescale( 
      lowerParameters( 4 ), lowerParameters( 4 ) );
    rescalerKappaConstraint.rescale( 
      lowerParameters( 5 ), lowerParameters( 5 ) );
    rescalerDParallelConstraint.rescale( 
      lowerParameters( 6 ), lowerParameters( 6 ) );
    rescalerDiameterConstraint.rescale( 
      lowerParameters( 7 ), lowerParameters( 7 ) );

    rescalerFintraConstraint.rescale( 
      upperParameters( 4 ), upperParameters( 4 ) );
    rescalerKappaConstraint.rescale( 
      upperParameters( 5 ), upperParameters( 5 ) );
    rescalerDParallelConstraint.rescale( 
      upperParameters( 6 ), upperParameters( 6 ) );
    rescalerDiameterConstraint.rescale( 
      upperParameters( 7 ), upperParameters( 7 ) );


    std::cout << "lowerParameters( 4 ) (fintra) = " 
              << lowerParameters( 4 ) << std::endl;     
    std::cout << "lowerParameters( 5 ) (kappa) = " 
              << lowerParameters( 5 ) << std::endl;     
    std::cout << "lowerParameters( 6 ) (Dparallel) = " 
              << lowerParameters( 6 ) << std::endl;                          
    std::cout << "lowerParameters( 7 ) (diameter) = " 
              << lowerParameters( 7 ) << std::endl;
  
    std::cout << "upperParameters( 4 ) (fintra) = " 
              << upperParameters( 4 ) << std::endl;     
    std::cout << "upperParameters( 5 ) (kappa) = " 
              << upperParameters( 5 ) << std::endl;     
    std::cout << "upperParameters( 6 ) (Dparallel) = " 
              << upperParameters( 6 ) << std::endl;                          
    std::cout << "upperParameters( 7 ) (diameter) = " 
              << upperParameters( 7 ) << std::endl;

    std::cout << "\n" << std::endl;                     
  

    // fiso
    constraints[ 0 ].reset( new gkg::ClosedIntervalOptimizerConstraint(
                                  0.0, 0.0 ) );
    // Diso
    constraints[ 1 ].reset( new gkg::ClosedIntervalOptimizerConstraint(
                                  0.0, 0.0 ) );
    // Dextra_perp
    constraints[ 2 ].reset( new gkg::ClosedIntervalOptimizerConstraint(
                                  0.0, 0.0 ) );
    // fstat
    constraints[ 3 ].reset( new gkg::ClosedIntervalOptimizerConstraint(
                                  0.0, 0.0 ) );
    // fintra
    constraints[ 4 ].reset( new gkg::ClosedIntervalOptimizerConstraint(
                                  lowerParameters( 4 ), 
                                  upperParameters( 4 ) ) );
    // kappa
    constraints[ 5 ].reset( new gkg::ClosedIntervalOptimizerConstraint(
                                  lowerParameters( 5 ), 
                                  upperParameters( 5 ) ) ); 
    // Dparallel
    constraints[ 6 ].reset( new gkg::ClosedIntervalOptimizerConstraint(
                                  lowerParameters( 6 ), 
                                  upperParameters( 6 ) ) );    
    // diameter
    constraints[ 7 ].reset( new gkg::ClosedIntervalOptimizerConstraint(
                                  lowerParameters( 7 ), 
                                  upperParameters( 7 ) ) );
    axonMappingFunction.addConstraints( constraints );

    gkg::Rescaler< double, double >
      rescalerFintra( 0.0, 1.0, -M_PI / 2, +M_PI / 2 );
    gkg::Rescaler< double, double >
      rescalerKappa( 1.0, 200.0, -M_PI / 2, +M_PI / 2 );
    gkg::Rescaler< double, double >
      rescalerDParallel( 1.0e-9, 3.0e-9, -M_PI / 2, +M_PI / 2 );
    gkg::Rescaler< double, double >
      rescalerDiameter( 0.1e-6, 10.0e-6, -M_PI / 2, +M_PI / 2 );

    gkg::Vector parameters( 8 );
    parameters( 0 ) = 0.0;
    parameters( 1 ) = 0.0;
    parameters( 2 ) = 0.0;
    parameters( 3 ) = 0.0;

    rescalerFintra.rescale( optimalP2, parameters( 4 ) );
    rescalerKappa.rescale( 200.0, parameters( 5 ) );
    rescalerDParallel.rescale( optimalP3, parameters( 6 ) );
    rescalerDiameter.rescale( optimalP4, parameters( 7 ) );

    /* double resultOfNewUOA = */ factory->getPowellNewUOAOptimumParameters(
                                      parameters,
                                      axonMappingFunction,
                                      0.2,
                                      2000,
                                      1e-2,
                                      true );

    gkg::Vector constrainedParameters( PARAMETER_COUNT );
    axonMappingFunction.getConstrainedParameters( parameters,
                                                  constrainedParameters );

    gkg::Rescaler< double, double >
      inverseRescalerFintra( -1.0, +1.0, 0.0, 1.0 );
    gkg::Rescaler< double, double >
      inverseRescalerKappa( -1.0, +1.0, 1.0, 200.0 );
    gkg::Rescaler< double, double >
      inverseRescalerDParallel( -1.0, +1.0, 1.0e-9, 3.0e-9 );
    gkg::Rescaler< double, double >
      inverseRescalerDiameter( -1.0, +1.0, 0.1e-6, 10.0e-6 );

    inverseRescalerFintra.rescale( constrainedParameters( 4 ),
                                   parameters( 4 ) );
    inverseRescalerKappa.rescale( constrainedParameters( 5 ),
                                 parameters( 5 ) );
    inverseRescalerDParallel.rescale( constrainedParameters( 6 ),
                                      parameters( 6 ) );
    inverseRescalerDiameter.rescale( constrainedParameters( 7 ),
                                     parameters( 7 ) );

    std::cout << "optimum parameters (first step): "
              << parameters << std::endl;


    ////////////////////////////////////////////////////////////////////////////
    // SECOND STEP
    ////////////////////////////////////////////////////////////////////////////

    std::cout << "Second step" << std::endl;
    std::cout << "------------------------------------------------------"
              << std::endl;

    lowerParameters( 0 ) = 0.0;
    lowerParameters( 1 ) = 0.0;
    lowerParameters( 2 ) = 0.0;
    lowerParameters( 3 ) = 0.0;
    lowerParameters( 4 ) = parameters( 4 );
    lowerParameters( 5 ) = parameters( 5 );
    lowerParameters( 6 ) = parameters( 6 );
    lowerParameters( 7 ) = 0.1e-6;

    upperParameters( 0 ) = 0.0;
    upperParameters( 1 ) = 0.0;
    upperParameters( 2 ) = 0.0;
    upperParameters( 3 ) = 0.0;
    upperParameters( 4 ) = parameters( 4 );
    upperParameters( 5 ) = parameters( 5 );
    upperParameters( 6 ) = parameters( 6 );
    upperParameters( 7 ) = 10.0e-6;

    rescalerFintraConstraint.rescale( 
      lowerParameters( 4 ), lowerParameters( 4 ) );
    rescalerKappaConstraint.rescale( 
      lowerParameters( 5 ), lowerParameters( 5 ) );
    rescalerDParallelConstraint.rescale( 
      lowerParameters( 6 ), lowerParameters( 6 ) );
    rescalerDiameterConstraint.rescale( 
      lowerParameters( 7 ), lowerParameters( 7 ) );

    rescalerFintraConstraint.rescale( 
      upperParameters( 4 ), upperParameters( 4 ) );
    rescalerKappaConstraint.rescale( 
      upperParameters( 5 ), upperParameters( 5 ) );
    rescalerDParallelConstraint.rescale( 
      upperParameters( 6 ), upperParameters( 6 ) );
    rescalerDiameterConstraint.rescale( 
      upperParameters( 7 ), upperParameters( 7 ) );


    std::cout << "lowerParameters( 4 ) (fintra) = " 
              << lowerParameters( 4 ) << std::endl;     
    std::cout << "lowerParameters( 5 ) (kappa) = " 
              << lowerParameters( 5 ) << std::endl;     
    std::cout << "lowerParameters( 6 ) (Dparallel) = " 
              << lowerParameters( 6 ) << std::endl;                          
    std::cout << "lowerParameters( 7 ) (diameter) = " 
              << lowerParameters( 7 ) << std::endl;
  
    std::cout << "upperParameters( 4 ) (fintra) = " 
              << upperParameters( 4 ) << std::endl;     
    std::cout << "upperParameters( 5 ) (kappa) = " 
              << upperParameters( 5 ) << std::endl;     
    std::cout << "upperParameters( 6 ) (Dparallel) = " 
              << upperParameters( 6 ) << std::endl;                          
    std::cout << "upperParameters( 7 ) (diameter) = " 
              << upperParameters( 7 ) << std::endl;

    std::cout << "\n" << std::endl;                     
  

    // fiso
    constraints[ 0 ].reset( new gkg::ClosedIntervalOptimizerConstraint(
                                  0.0, 0.0 ) );
    // Diso
    constraints[ 1 ].reset( new gkg::ClosedIntervalOptimizerConstraint(
                                  0.0, 0.0 ) );
    // Dextra_perp
    constraints[ 2 ].reset( new gkg::ClosedIntervalOptimizerConstraint(
                                  0.0, 0.0 ) );
    // fstat
    constraints[ 3 ].reset( new gkg::ClosedIntervalOptimizerConstraint(
                                  0.0, 0.0 ) );
    // fintra
    constraints[ 4 ].reset( new gkg::ClosedIntervalOptimizerConstraint(
                                  lowerParameters( 4 ), 
                                  upperParameters( 4 ) ) );
    // kappa
    constraints[ 5 ].reset( new gkg::ClosedIntervalOptimizerConstraint(
                                  lowerParameters( 5 ), 
                                  upperParameters( 5 ) ) ); 
    // Dparallel
    constraints[ 6 ].reset( new gkg::ClosedIntervalOptimizerConstraint(
                                  lowerParameters( 6 ), 
                                  upperParameters( 6 ) ) );    
    // diameter
    constraints[ 7 ].reset( new gkg::ClosedIntervalOptimizerConstraint(
                                  lowerParameters( 7 ), 
                                  upperParameters( 7 ) ) );
    axonMappingFunction.addConstraints( constraints );

    parameters( 0 ) = 0.0;
    parameters( 1 ) = 0.0;
    parameters( 2 ) = 0.0;
    parameters( 3 ) = 0.0;

//     rescalerFintra.rescale( optimalP2, parameters( 4 ) );
//     rescalerKappa.rescale( 200.0, parameters( 5 ) );
//     rescalerDParallel.rescale( optimalP3, parameters( 6 ) );
//     rescalerDiameter.rescale( optimalP4, parameters( 7 ) );

    /* resultOfNewUOA = */ factory->getPowellNewUOAOptimumParameters(
                                      parameters,
                                      axonMappingFunction,
                                      0.1,
                                      2000,
                                      1e-6,
                                      true );

    axonMappingFunction.getConstrainedParameters( parameters,
                                                  constrainedParameters );

    inverseRescalerFintra.rescale( constrainedParameters( 4 ),
                                   constrainedParameters( 4 ) );
    inverseRescalerKappa.rescale( constrainedParameters( 5 ),
                                 constrainedParameters( 5 ) );
    inverseRescalerDParallel.rescale( constrainedParameters( 6 ),
                                      constrainedParameters( 6 ) );
    inverseRescalerDiameter.rescale( constrainedParameters( 7 ),
                                     constrainedParameters( 7 ) );

    std::cout << "optimum parameters (second step): "
              << constrainedParameters << std::endl;


  }
  GKG_CATCH_COMMAND( result );

  return result;

  
}
