#include <gkg-dmri-odf-functor/SphericalHarmonicsOdfPeakFinder.h>
#include <gkg-processing-algobase/SumOfSquareFilter_i.h>
#include <gkg-processing-algobase/Scaler_i.h>
#include <gkg-processing-numericalanalysis/NumAnalysisImplementationFactory.h>
#include <gkg-processing-numericalanalysis/CVector.h>
#include <gkg-processing-algobase/Math.h>
#include <gkg-core-exception/Exception.h>
#include <cmath>
#include <algorithm>


#define INITIAL_PHI_INCREMENT 0.01


gkg::SphericalHarmonicsOdfPeakFinder::SphericalHarmonicsOdfPeakFinder(
                                     int32_t peakCount,
                                     float colatitudeDerivativeThreshold,
                                     float resolution,
                                     float clusteringThreshold,
                                     float angularThreshold,
                                     float minimumIncrement,
                                     float maximumIncrement )
                                     : _peakCount( peakCount ),
                                       _colatitudeDerivativeThreshold( 
                                                colatitudeDerivativeThreshold ),
                                       _resolution( resolution ),
                                       _clusteringThreshold( 
                                                          clusteringThreshold ),
                                       _angularThreshold( angularThreshold ),
                                       _minimumIncrement( minimumIncrement ),
                                       _maximumIncrement( maximumIncrement )                                       
{

  try
  {

    _factory = 
       gkg::NumericalAnalysisSelector::getInstance().getImplementationFactory();

  }
  GKG_CATCH( "gkg::SphericalHarmonicsOdfPeakFinder::"
             "SphericalHarmonicsOdfPeakFinder( "
             "int32_t peakCount, "
             "float colatitudeDerivativeThreshold, "
             "float resolution, "
             "float clusteringThreshold, "
             "float angularThreshold, "
             "float minimumIncrement, "
             "float maximumIncrement )" );

}


gkg::SphericalHarmonicsOdfPeakFinder::~SphericalHarmonicsOdfPeakFinder()
{
}


void gkg::SphericalHarmonicsOdfPeakFinder::getPeaks(
                          const gkg::OrientationDistributionFunction& odf,
                          std::vector< gkg::Vector3d< float > >& peakDirections,
                          std::vector< float >& peakMagnitudes ) const
{

  try
  {

    if ( odf.getBasisType() == 
         gkg::OrientationDistributionFunction::SphericalHarmonics )
    {


      //////////////////////////////////////////////////////////////////////////
      // getting access to the SH coefficients
      //////////////////////////////////////////////////////////////////////////

      int32_t shCoefficientCount = 15; 
      std::vector< float > shCoefficients( shCoefficientCount, 0.0f );
      const std::vector< float >& shValues = odf.getValues();
      int32_t shValueSize = ( int32_t )shValues.size();

      int32_t c = 0;
      while ( ( c < 15 ) && ( c < shValueSize ) )
      {

        shCoefficients[ c ] = shValues[ c ];
        ++ c;

      }


      //////////////////////////////////////////////////////////////////////////
      // allocating the vector of peak directions to the adequate size
      //////////////////////////////////////////////////////////////////////////

      peakDirections.resize( _peakCount );
      peakMagnitudes.resize( _peakCount );


      //////////////////////////////////////////////////////////////////////////
      // computing normalization factor
      //////////////////////////////////////////////////////////////////////////

      float normalizationFactor = 0.0f;
      gkg::SumOfSquareFilter< std::vector< float >, float > sumOfSquareFilter;
      sumOfSquareFilter.filter( shCoefficients, normalizationFactor );


      //////////////////////////////////////////////////////////////////////////
      // scaling SH coefficients by the normalization factor
      //////////////////////////////////////////////////////////////////////////

      gkg::Scaler< std::vector< float > > scaler( 1.0f / normalizationFactor );
      scaler.scale( shCoefficients );
 

      //////////////////////////////////////////////////////////////////////////
      // looping to find peak directions
      //////////////////////////////////////////////////////////////////////////

      gkg::Vector polynomialCoefficients( 4 );
      gkg::CVector polynomialRoots( 3 );
      gkg::CVector storedPolynomialRoot( 3 );
      storedPolynomialRoot.fill( std::complex< double >( 1e38, 1e38 ) );

      std::list< gkg::SphericalHarmonicsOdfPeakFinder::Peak > odfPeaks;
      float phiIncrement = INITIAL_PHI_INCREMENT;
      float phi = 0.0f;
      float storedPhi = 0.0f;
      while ( phi < M_PI )
      {

        this->getTanThetaPolynomialCoefficients( shCoefficients,
                                                 phi,
                                                 polynomialCoefficients );
        _factory->getPolynomialRoots( polynomialCoefficients,
                                      polynomialRoots );

        this->findExtremaAmongPolynomialRoots( shCoefficients,
                                               phi,
                                               polynomialRoots,
                                               odfPeaks );

        this->tunePhiIncrement( shCoefficients,
                                phi,
                                polynomialRoots,
                                storedPolynomialRoot,
                                storedPhi,
                                phiIncrement );
        phi += phiIncrement;

      }

      std::list< gkg::SphericalHarmonicsOdfPeakFinder::Peak > odfPeakClusters;
      this->computeClusters( shCoefficients,
                             odfPeaks,
                             odfPeakClusters );

      this->pickBiggestPeaks( shCoefficients,
                              odfPeakClusters,
                              peakDirections,
                              peakMagnitudes ); 

    }
    else
    {

      throw std::runtime_error( 
                   "not implemented for other basis that spherical harmonics" );

    }

  }
  GKG_CATCH( "void gkg::SphericalHarmonicsOdfPeakFinder::getPeakDirection( "
             "const gkg::OrientationDistributionFunction& odf, "
             "std::vector< Vector3d< float > >& peakDirections ) const" );

}


void gkg::SphericalHarmonicsOdfPeakFinder::getTanThetaPolynomialCoefficients(
                               const std::vector< float >& shCoefficients,
                               float phi,
                               gkg::Vector& polynomialCoefficients ) const
{

  try
  {

    double sinPhi = std::sin( phi );
    double cosPhi = std::cos( phi );
    double sin2Phi = std::sin( 2.0 * phi );
    double cos2Phi = std::cos( 2.0 * phi );
    double sin3Phi = std::sin( 3.0 * phi );
    double cos3Phi = std::cos( 3.0 * phi );
    double sin4Phi = std::sin( 4.0 * phi );
    double cos4Phi = std::cos( 4.0 * phi );
    double tmp1 = std::sqrt( 15.0 / ( 4.0 * M_PI ) );

    double BdPhi = ( -shCoefficients[ 2 ] * sinPhi - 
                    shCoefficients[ 4 ] * cosPhi ) * tmp1;

    double CdPhi = ( -shCoefficients[ 1 ] * sin2Phi +
                    shCoefficients[ 5 ] * cos2Phi ) * tmp1;

    double EdPhi = ( -shCoefficients[ 9 ] * sinPhi -
                    shCoefficients[ 11 ] * cosPhi ) *
                  ( float )std::sqrt( 45.0 / ( 32.0 * M_PI ) );

    double FdPhi = ( -shCoefficients[ 8 ] * sin2Phi +
                    shCoefficients[ 12 ] * cos2Phi ) *
                  ( float )std::sqrt( 5.0 / M_PI ) * 0.75;

    double GdPhi = ( -shCoefficients[ 7 ] * sin3Phi -
                    shCoefficients[ 13 ] * cos3Phi ) *
                  ( float )std::sqrt( 35.0 / ( 2.0 * M_PI ) ) * 2.25;

    double HdPhi = ( -shCoefficients[ 6 ] * sin4Phi +
                    shCoefficients[ 14 ] * cos4Phi ) *
                  ( float )std::sqrt( 35.0 / M_PI ) * 0.75;

    polynomialCoefficients( 0 ) = HdPhi + CdPhi - FdPhi ;
    polynomialCoefficients( 1 ) = GdPhi + BdPhi - 3.0 * EdPhi ;
    polynomialCoefficients( 2 ) = 6.0 * FdPhi + CdPhi ;
    polynomialCoefficients( 3 ) = BdPhi + 4.0 * EdPhi ;

  }
  GKG_CATCH( "void gkg::SphericalHarmonicsOdfPeakFinder::"
             "getTanThetaPolynomialCoefficients("
             "float phi, "
             "gkg::Vector& polynomialCoefficients ) const");
}


void gkg::SphericalHarmonicsOdfPeakFinder::findExtremaAmongPolynomialRoots(
       const std::vector< float >& shCoefficients,
       float phi,
       const gkg::CVector& polynomialRoots,
       std::list< gkg::SphericalHarmonicsOdfPeakFinder::Peak >& odfPeaks ) const
{

  try
  {

    int32_t rootIndex = 0;
    for ( rootIndex = 0; rootIndex < 3; rootIndex++ )
    {

      const std::complex< double >& root = polynomialRoots( rootIndex );

      if ( std::fabs( root.imag() ) < 1e-10 )
      {

        float theta = ( float )std::atan( root.real() );

        if ( theta > -50.0f )
        {

          float psidTheta2 = 0.0f;
          float psidThetadPhi = 0.0f;
          float psidPhi2 = 0.0f;
          float psidTheta = 0.0f;
          this->computeHessian( shCoefficients,
                                phi,
                                theta,
                                psidTheta2, 
                                psidThetadPhi, 
                                psidPhi2, 
                                psidTheta);

          float  determinant = psidTheta2 * psidPhi2 -
                               psidThetadPhi * psidThetadPhi;
          if ( ( determinant > 0.0f ) &&
               ( std::fabs( psidTheta ) < _colatitudeDerivativeThreshold ) &&
               ( psidTheta > 0.0f ) )
          {

            gkg::Vector3d< float > odfPeakCartesianCoordinate;
            float radial = 0.0f;
            this->getPeakCartesianCoordinates( shCoefficients,
                                               phi,
                                               theta,
                                               odfPeakCartesianCoordinate,
                                               radial );
                                                             
            odfPeaks.push_back( gkg::SphericalHarmonicsOdfPeakFinder::Peak(
                                                                     phi,
                                                                     theta,
                                                                     radial ) );

          }

        }

      }

    }

  }
  GKG_CATCH( "void gkg::SphericalHarmonicsOdfPeakFinder::"
             "findExtremaAmongPolynomialRoots( "
             "const std::vector< float >& shCoefficients, "
             "float phi, "
             "const gkg::CVector& polynomialRoots, "
             "std::list< gkg::SphericalHarmonicsOdfPeakFinder::Peak >& "
             "odfPeaks ) const" );

}



void gkg::SphericalHarmonicsOdfPeakFinder::computeHessian(
                                     const std::vector< float >& shCoefficients,
                                     float phi,
                                     float theta,
                                     float& psidTheta2,
                                     float& psidThetadPhi,
                                     float& psidPhi2, 
                                     float& psidTheta ) const
{

  try
  {

    float sinPhi = ( float )std::sin( phi );
    float cosPhi = ( float )std::cos( phi );
    float sin2Phi = ( float )std::sin( 2.0 * phi );
    float cos2Phi = ( float )std::cos( 2.0 * phi );
    float sin3Phi = ( float )std::sin( 3.0 * phi );
    float cos3Phi = ( float )std::cos( 3.0 * phi );
    float sin4Phi = ( float )std::sin( 4.0 * phi );
    float cos4Phi = ( float )std::cos( 4.0 * phi );
    float sinTheta = ( float )std::sin( theta );
    float cosTheta = ( float )std::cos( theta );
    float squareSinTheta = sinTheta * sinTheta;
    float squareCosTheta = cosTheta * cosTheta;

    float A = ( float )std::sqrt( 5.0 / ( 16.0 * M_PI ) ) *
              shCoefficients[ 3 ];

    float B = ( float )std::sqrt( 15.0 / ( 4.0 * M_PI ) ) *
              ( shCoefficients[ 2 ] * cosPhi -
                shCoefficients[ 4 ] * sinPhi );

    float C = ( float )std::sqrt( 15.0 / ( 16.0 * M_PI ) ) *
              ( shCoefficients[ 1 ] * cos2Phi +
                shCoefficients[ 5 ] * sin2Phi );

    float D = ( float )std::sqrt( 1.0 / M_PI ) * ( 3.0f / 16.0f ) *
              shCoefficients[ 10 ];

    float E = ( float )std::sqrt( 45.0 / ( 32.0 * M_PI ) ) *
              ( shCoefficients[ 9 ] * cosPhi -
                shCoefficients[ 11 ] * sinPhi );
  
    float F = ( float )std::sqrt( 5.0 / M_PI ) * ( 3.0f / 8.0f ) * 
              ( shCoefficients[ 8 ] * cos2Phi +
                shCoefficients[ 12 ] * sin2Phi );
  
    float G = ( float )std::sqrt( 35.0 / ( 2.0 * M_PI ) ) * ( 3.0f / 4.0f ) * 
              ( shCoefficients[ 7 ] * cos3Phi -
                shCoefficients[ 13 ] * sin3Phi );
  
    float H = ( float )std::sqrt( 35.0 / M_PI ) * ( 3.0f / 16.0f ) * 
              ( shCoefficients[ 14 ] * sin4Phi +
                shCoefficients[ 6 ] * cos4Phi );

    psidTheta2 = - 6.0f * A * ( squareCosTheta - squareSinTheta ) -
                 4.0f * B * sinTheta * cosTheta +
                 2.0f * C * ( squareCosTheta - squareSinTheta ) +
                 D * ( 420.0f * squareCosTheta * squareSinTheta -
                       140.0f * squareCosTheta * squareCosTheta  +
                       60.0f * squareCosTheta - 60.0f * squareSinTheta ) +
                 E * ( -49.0f * sinTheta * squareCosTheta * cosTheta +
                       21.0f * squareSinTheta * sinTheta  +
                       12.0f * cosTheta * sinTheta ) +
                 F * ( -84.0f * squareSinTheta * squareCosTheta  +
                       14.0f * squareSinTheta * squareSinTheta +
                       14.0f * squareCosTheta * squareCosTheta +
                       2.0f * squareSinTheta - 2 * squareCosTheta ) +
                 G * ( -10.0f * squareSinTheta * sinTheta * cosTheta +
                 6.0f * squareCosTheta * cosTheta * sinTheta ) +
                 H * ( 12.0f * squareCosTheta * squareSinTheta -
                       4.0f * squareSinTheta * squareSinTheta );

    float BdPhi = ( float )std::sqrt( 15.0 / ( 4.0 * M_PI ) ) * 
                  ( -shCoefficients[ 2 ] * sinPhi -
                    shCoefficients[ 4 ] * cosPhi);

    float CdPhi = ( float )std::sqrt( 15.0 / ( 4.0 * M_PI ) ) * 
                  ( -shCoefficients[ 1 ] * sin2Phi +
                    shCoefficients[ 5 ] * cos2Phi );

    float EdPhi = ( float )std::sqrt( 45.0/ ( 32.0 * M_PI ) ) * 
                  ( -shCoefficients[ 9 ] * sinPhi -
                    shCoefficients[ 11 ] * cosPhi );
  
    float FdPhi = ( float )std::sqrt( 5.0 / M_PI ) * ( 3.0f / 4.0f ) * 
                  ( -shCoefficients[ 8 ] * sin2Phi +
                    shCoefficients[ 12 ] * cos2Phi );
  
    float GdPhi = ( float )std::sqrt( 35.0 / ( 2.0 * M_PI ) ) *
                  ( 9.0f / 4.0f ) * 
                  ( -shCoefficients[ 7 ] * sin3Phi -
                    shCoefficients[ 13 ] * cos3Phi );
  
    float HdPhi = ( float )std::sqrt( 35.0 / M_PI ) * ( 3.0f / 4.0f ) * 
                  ( shCoefficients[ 14 ] * cos4Phi -
                    shCoefficients[ 6 ] * sin4Phi );

    psidThetadPhi = BdPhi * ( squareCosTheta - squareSinTheta ) + 
                    CdPhi * ( 2.0f * cosTheta * sinTheta ) + 
                    EdPhi * ( -21.0f * squareSinTheta*squareCosTheta +
                              7.0f * squareCosTheta * squareCosTheta  -
                              3.0f * squareCosTheta + 3.0f * squareSinTheta ) + 
                    FdPhi * ( -14.0f * squareSinTheta * sinTheta * cosTheta +
                              14.0f * sinTheta * squareCosTheta * cosTheta -
                              2.0f * sinTheta * cosTheta ) +
                    GdPhi * ( -squareSinTheta * squareSinTheta +
                              3.0f * squareCosTheta * squareSinTheta )+ 
                    HdPhi * ( 4.0f * cosTheta * squareSinTheta * sinTheta );

    float BdPhi2 = ( float )std::sqrt( 15.0 / ( 4.0 * M_PI ) ) * 
                   ( -shCoefficients[ 2 ] * cosPhi + 
                     shCoefficients[ 4 ] * sinPhi );

    float CdPhi2 = ( float )std::sqrt( 15.0 / M_PI ) *
                   ( -shCoefficients[ 1 ] * cos2Phi -
                     shCoefficients[ 5 ] * sin2Phi );

    float EdPhi2 = ( float )std::sqrt( 5.0 / ( 2.0 * M_PI ) ) *
                   ( 3.0f / 4.0f ) *
                   ( -shCoefficients[ 9 ] * cosPhi +
                     shCoefficients[ 11 ] * sinPhi );

    float FdPhi2 = ( float )std::sqrt( 5.0 / M_PI ) * ( 3.0f / 2.0f ) *
                   ( -shCoefficients[ 8 ] * cos2Phi -
                     shCoefficients[ 12 ] * sin2Phi );

    float GdPhi2 = ( float )std::sqrt( 35.0 / ( 2.0 * M_PI ) ) *
                   ( 27.0f / 4.0f ) *
                   ( -shCoefficients[ 7 ] * cos3Phi +
                     shCoefficients[13] * sin3Phi );

    float HdPhi2 = ( float )std::sqrt( 35.0 / M_PI ) * 3.0f *
                   ( -shCoefficients[ 14 ] * sin4Phi -
                     shCoefficients[ 6 ] * cos4Phi );

    psidPhi2 = BdPhi2 * sinTheta * cosTheta + 
               CdPhi2 * squareSinTheta + 
               EdPhi2 * ( 7.0f * squareCosTheta - 3.0f ) * sinTheta * cosTheta + 
               FdPhi2 * ( 7.0f * squareCosTheta - 1.0f ) * squareSinTheta + 
               GdPhi2 * ( cosTheta * squareSinTheta * sinTheta ) + 
               HdPhi2 * squareSinTheta * squareSinTheta; 

    psidTheta = A * ( -6.0f *  cosTheta * sinTheta ) +
                B * ( squareCosTheta - squareSinTheta ) +
                C * ( 2.0f * cosTheta * sinTheta ) +
                D * ( -140.0f * squareCosTheta * cosTheta * sinTheta +
                      60.0f * sinTheta * cosTheta ) +
                E * ( -21.0f * squareSinTheta * squareCosTheta +
                      7.0f * squareCosTheta * squareCosTheta -
                      3.0f * squareCosTheta + 3.0f * squareSinTheta ) +
                F * ( -14.0f * squareSinTheta * sinTheta * cosTheta +
                      14.0f * sinTheta * squareCosTheta * cosTheta -
                      2.0f * sinTheta * cosTheta ) +
                G * ( -squareSinTheta * squareSinTheta +
                      3.0f * squareCosTheta * squareSinTheta ) +
                H * ( 4.0f * cosTheta * squareSinTheta * sinTheta );

  }
  GKG_CATCH( "void gkg::SphericalHarmonicsOdfPeakFinder::computeHessian( "
             "const std::vector< float >& shCoefficients, "
             "float phi, "
             "float theta, "
             "float& psidTheta2, "
             "float& psidThetadPhi, "
             "float& psidPhi2, "
             "float& psidTheta ) const" );

}


void gkg::SphericalHarmonicsOdfPeakFinder::getPeakCartesianCoordinates(
                      const std::vector< float >& shCoefficients,
                      float phi,
                      float theta,
                      gkg::Vector3d< float >& odfPeakCartesianCoordinates,
                      float& radial ) const
{

  try
  {

    float sinPhi = ( float )std::sin( phi );
    float cosPhi = ( float )std::cos( phi );
    float sin2Phi = ( float )std::sin( 2.0 * phi );
    float cos2Phi = ( float )std::cos( 2.0 * phi );
    float sin3Phi = ( float )std::sin( 3.0 * phi );
    float cos3Phi = ( float )std::cos( 3.0 * phi );
    float sin4Phi = ( float )std::sin( 4.0 * phi );
    float cos4Phi = ( float )std::cos( 4.0 * phi );
    float sinTheta = ( float )std::sin( theta );
    float cosTheta = ( float )std::cos( theta );
    float squareSinTheta = sinTheta * sinTheta;
    float squareCosTheta = cosTheta * cosTheta;

    float A = ( float )std::sqrt( 5.0 / ( 16.0 * M_PI ) ) *
              shCoefficients[ 3 ];

    float B = ( float )std::sqrt( 15.0 / ( 4.0 * M_PI ) ) * 
              ( shCoefficients[ 2 ] * cosPhi - 
                shCoefficients[ 4 ] * sinPhi );

    float C = ( float )std::sqrt( 15.0 / ( 16.0 * M_PI ) ) * 
              ( shCoefficients[ 1 ] * cos2Phi +
                shCoefficients[ 5 ] * sin2Phi );
    
    float D = ( float )std::sqrt( 1.0 / M_PI ) * ( 3.0f / 16.0f ) *
              shCoefficients[ 10 ];

    float E = ( float )std::sqrt( 45.0 / ( 32.0 * M_PI ) ) * 
              ( shCoefficients[ 9 ] * cosPhi -
                shCoefficients[ 11 ] * sinPhi );

    float F = ( float )std::sqrt( 5.0 / M_PI ) * ( 3.0f / 8.0f ) * 
              ( shCoefficients[ 8 ] * cos2Phi +
                shCoefficients[ 12 ] * sin2Phi );

    float G = ( float )std::sqrt( 35.0 / ( 2.0 * M_PI ) ) * ( 3.0f / 4.0f ) * 
              ( shCoefficients[ 7 ] * cos3Phi -
                shCoefficients[ 13 ] * sin3Phi );

    float H = ( float )std::sqrt( 35.0 / M_PI ) * ( 3.0f / 16.0f ) *
              ( shCoefficients[ 14 ] * sin4Phi +
                shCoefficients[ 6 ] * cos4Phi );

    radial = shCoefficients[ 0 ] / ( 2.0f * ( float )std::sqrt( M_PI ) ) + 
             A * ( 3.0f * squareCosTheta - 1 ) +
             B * sinTheta * cosTheta + 
             C * sinTheta * sinTheta + 
             D * ( 35.0f * squareCosTheta * squareCosTheta - 
                   30.0f * squareCosTheta + 3.0f ) + 
             E * ( 7.0f * squareCosTheta - 3.0f ) * sinTheta * cosTheta +
             F * ( 7.0f * squareCosTheta - 1.0f ) * squareSinTheta + 
             G * cosTheta * squareSinTheta * sinTheta +
             H * squareSinTheta * squareSinTheta;

    odfPeakCartesianCoordinates.x =  sinTheta * cosPhi * radial;
    odfPeakCartesianCoordinates.y =  sinTheta * sinPhi * radial;
    odfPeakCartesianCoordinates.z =  cosTheta * radial;


  }
  GKG_CATCH( "void gkg::SphericalHarmonicsOdfPeakFinder::"
             "getPeakCartesianCoordinates( "
             "const std::vector< float >& shCoefficients, "
             "float phi, "
             "float theta, "
             "gkg::Vector3d< float >& odfPeakCartesianCoordinates, "
             "float& radial ) const" );

}


void gkg::SphericalHarmonicsOdfPeakFinder::tunePhiIncrement(
                                     const std::vector< float >& shCoefficients,
                                     float phi,
                                     const gkg::CVector& polynomialRoots,
                                     gkg::CVector& storedPolynomialRoots,
                                     float& storedPhi,
                                     float& phiIncrement ) const
{

  try
  {

    float oldPhi = storedPhi;
    float currentPhi = phi;

    float oldTheta = 0.0f;
    float currentTheta = 0.0f;

    std::vector< int32_t > resolutionIncreaseChoices( 3, -1 );

    int32_t rootIndex = 0;
    for ( rootIndex = 0; rootIndex < 3; rootIndex++ )
    {

      const std::complex< double >& 
        storedRoot = storedPolynomialRoots( rootIndex );
      const std::complex< double >&
        root = polynomialRoots( rootIndex );

      if ( ( std::fabs( storedRoot.imag() ) < 1e-10 ) &&
           ( std::fabs( root.imag() ) < 1e-10 ) )
      {

        oldTheta = ( float )std::atan( storedRoot.real() );
        currentTheta = ( float )std::atan( root.real() );

        float distanceBetweenOdfPeaks = this->getDistanceBeetweenOdfPeaks(
                                                                 shCoefficients,
                                                                 oldPhi,
                                                                 oldTheta,
                                                                 currentPhi,
                                                                 currentTheta );

        if ( ( distanceBetweenOdfPeaks > _resolution ) &&
             ( phiIncrement > _minimumIncrement ) )
        { 

          resolutionIncreaseChoices[ rootIndex ] = 0;

        }
        else if ( ( distanceBetweenOdfPeaks < _resolution / 4.0f ) &&
                  ( phiIncrement < _maximumIncrement ) )
        {

          resolutionIncreaseChoices[ rootIndex ] = 2;

        }
        else
        {

          resolutionIncreaseChoices[ rootIndex ] = 1; 

        }

      }
      else
      {

        resolutionIncreaseChoices[ rootIndex ] = -1;

      }

    }

    if ( ( resolutionIncreaseChoices[ 0 ] == 0 ) ||
         ( resolutionIncreaseChoices[ 1 ] == 0 ) ||
         ( resolutionIncreaseChoices[ 2 ] == 0 ) )
    {

      phiIncrement /= 8.0f;

    }
    else if ( ( resolutionIncreaseChoices[ 0 ] == 2 ) ||
              ( resolutionIncreaseChoices[ 1 ] == 2 ) ||
               ( resolutionIncreaseChoices[ 2 ] == 2 ) )
    {

      phiIncrement *= 2.0f; 

    }

    storedPolynomialRoots( 0 ) = polynomialRoots( 0 );
    storedPolynomialRoots( 1 ) = polynomialRoots( 1 );
    storedPolynomialRoots( 2 ) = polynomialRoots( 2 );

    storedPhi = phi;

  }
  GKG_CATCH( "void gkg::SphericalHarmonicsOdfPeakFinder::tunePhiIncrement( "
             "const std::vector< float >& shCoefficients, "
             "float phi, "
             "const gkg::CVector& polynomialRoots, "
             "gkg::CVector& storedPolynomialRoots, "
             "float& storedPhi, "
             "float& phiIncrement ) const" );

}


float gkg::SphericalHarmonicsOdfPeakFinder::getDistanceBeetweenOdfPeaks(
                                     const std::vector< float >& shCoefficients,
                                     float oldPhi,
                                     float oldTheta,
                                     float currentPhi,
                                     float currentTheta ) const
{

  try
  {

    gkg::Vector3d< float > oldOdfPeakCartesianCoordinates;
    gkg::Vector3d< float > currentOdfPeakCartesianCoordinates;

    float oldRadial = 0.0f;
    float currentRadial = 0.0f;

    this->getPeakCartesianCoordinates( shCoefficients,
                                       oldPhi,
                                       oldTheta,
                                       oldOdfPeakCartesianCoordinates,
                                       oldRadial );

    this->getPeakCartesianCoordinates( shCoefficients,
                                       currentPhi,
                                       currentTheta,
                                       currentOdfPeakCartesianCoordinates,
                                       currentRadial );

    return ( oldOdfPeakCartesianCoordinates -
             currentOdfPeakCartesianCoordinates ).getNorm();

  }
  GKG_CATCH( "float gkg::SphericalHarmonicsOdfPeakFinder::"
             "getDistanceBeetweenOdfPeaks( "
             "const std::vector< float >& shCoefficients, "
             "float oldPhi, "
             "float oldTheta, "
             "float currentPhi, "
             "float currentTheta ) const" );

}


float gkg::SphericalHarmonicsOdfPeakFinder::getAngleBeetweenOdfPeaks(
                                     const std::vector< float >& shCoefficients,
                                     float oldPhi,
                                     float oldTheta,
                                     float currentPhi,
                                     float currentTheta ) const
{

  try
  {

    gkg::Vector3d< float > oldOdfPeakCartesianCoordinates;
    gkg::Vector3d< float > currentOdfPeakCartesianCoordinates;

    float oldRadial = 0.0f;
    float currentRadial = 0.0f;

    this->getPeakCartesianCoordinates( shCoefficients,
                                       oldPhi,
                                       oldTheta,
                                       oldOdfPeakCartesianCoordinates,
                                       oldRadial );

    this->getPeakCartesianCoordinates( shCoefficients,
                                       currentPhi,
                                       currentTheta,
                                       currentOdfPeakCartesianCoordinates,
                                       currentRadial );

    return gkg::getVectorAngles( oldOdfPeakCartesianCoordinates,
                                 currentOdfPeakCartesianCoordinates );

  }
  GKG_CATCH( "float gkg::SphericalHarmonicsOdfPeakFinder::"
             "getAngleBeetweenOdfPeaks( "
             "const std::vector< float >& shCoefficients, "
             "float oldPhi, "
             "float oldTheta, "
             "float currentPhi, "
             "float currentTheta ) const" );

}


void gkg::SphericalHarmonicsOdfPeakFinder::computeClusters( 
const std::vector< float >& shCoefficients,
const std::list< gkg::SphericalHarmonicsOdfPeakFinder::Peak >& odfPeaks,
std::list< gkg::SphericalHarmonicsOdfPeakFinder::Peak >& odfPeakClusters ) const

{

  try
  {

    std::list< gkg::SphericalHarmonicsOdfPeakFinder::Peak > referenceOdfPeaks;

    std::list< gkg::SphericalHarmonicsOdfPeakFinder::Peak >::const_iterator
      p = odfPeaks.begin(),
      pe = odfPeaks.end();

    referenceOdfPeaks.push_back( *p );
    odfPeakClusters.push_back( *p );
    ++ p;

    // looping over peaks
    bool joinedCluster = false;
    while ( p != pe )
    {

      // looping over other peaks
      std::list< gkg::SphericalHarmonicsOdfPeakFinder::Peak >::iterator
        c = odfPeakClusters.begin(),
        ce = odfPeakClusters.end();
      std::list< gkg::SphericalHarmonicsOdfPeakFinder::Peak >::iterator
        r = referenceOdfPeaks.begin();
      while ( c != ce )
      {

        // if the Cartesian distance between peaks is too small, 
        // then merging peak to peak cluster
        if ( ( this->getDistanceBeetweenOdfPeaks( shCoefficients,
                                                  c->phi,
                                                  c->theta,
                                                  p->phi,
                                                  p->theta ) <
               _clusteringThreshold ) && !joinedCluster )
        {

          if ( c->height < p->height )
          {

            *c = *p;

          }
          *r = *p;
          joinedCluster = true;

        }
        ++ c;
        ++ r;

      }

      // looping over other peaks
      c = odfPeakClusters.begin();
      r = referenceOdfPeaks.begin();
      while ( c != ce )
      {

        // if the angle between peaks is too small, 
        // then merging peak to peak cluster
        if ( ( this->getAngleBeetweenOdfPeaks( shCoefficients,
                                                  c->phi,
                                                  c->theta,
                                                  p->phi,
                                                  p->theta ) <
               _angularThreshold ) && !joinedCluster )
        {

          if ( c->height < p->height )
          {

            *c = *p;

          }
          *r = *p;
          joinedCluster = true;          

        }
        ++ c;
        ++ r;

      }

      if ( !joinedCluster )
      {
      
        referenceOdfPeaks.push_back( *p );
        odfPeakClusters.push_back( *p );

      }

      ++ p;

    }


  }
  GKG_CATCH( "void gkg::SphericalHarmonicsOdfPeakFinder::computeClusters( "
             "const std::vector< float >& shCoefficients, "
             "const std::list< gkg::SphericalHarmonicsOdfPeakFinder::Peak >& "
             "odfPeaks, "
             "std::list< gkg::SphericalHarmonicsOdfPeakFinder::Peak >& "
             "odfPeakClusters ) const" );

}


struct IndexAndOdfPeakPointer
{

  int32_t index;
  const gkg::SphericalHarmonicsOdfPeakFinder::Peak* odfPeakPointer;

};


static int32_t compareDistance( const void *element1, const void *element2 )
{

  return ( ( ( IndexAndOdfPeakPointer* )element1 )->odfPeakPointer->height <=
           ( ( IndexAndOdfPeakPointer* )element2 )->odfPeakPointer->height ) ?
         +1 : -1;

}

void gkg::SphericalHarmonicsOdfPeakFinder::pickBiggestPeaks(
 const std::vector< float >& shCoefficients,
 const std::list< gkg::SphericalHarmonicsOdfPeakFinder::Peak >& odfPeakClusters,
 std::vector< gkg::Vector3d< float > >& peakDirections,
 std::vector< float >& peakMagnitudes ) const
{

  try
  {

    // preparing the adequate structure to sort peak clusters according to
    // their magnitude
    std::vector< IndexAndOdfPeakPointer >
      indexAndOdfPeakPointers( odfPeakClusters.size() );

    int32_t index = 0;
    std::list< gkg::SphericalHarmonicsOdfPeakFinder::Peak >::const_iterator
      c = odfPeakClusters.begin(),
      ce = odfPeakClusters.end();
    std::vector< IndexAndOdfPeakPointer >::iterator
      iop = indexAndOdfPeakPointers.begin();
    while ( c != ce )
    {

      iop->index = index;
      iop->odfPeakPointer = &( *c );

      ++ c;
      ++ iop;
      ++ index;

    }

    // sorting the ODF peak according to their magnitude
    qsort( ( void* )&indexAndOdfPeakPointers[ 0 ],
           odfPeakClusters.size(),
           sizeof( IndexAndOdfPeakPointer ),
           compareDistance ); 

    // selecting the adequate number of peak according to the wanted peak count
    std::vector< IndexAndOdfPeakPointer >::const_iterator
      iopPrime = indexAndOdfPeakPointers.begin(),
      iopPrimeE = indexAndOdfPeakPointers.end();
    std::vector< gkg::Vector3d< float > >::iterator 
      pd = peakDirections.begin();
    std::vector< float >::iterator 
      pm = peakMagnitudes.begin();
    int32_t odfPeakIndex = 0;
    while ( ( iopPrime != iopPrimeE ) && ( odfPeakIndex < _peakCount ) )
    {

      this->getPeakCartesianCoordinates( shCoefficients,
                                         iopPrime->odfPeakPointer->phi,
                                         iopPrime->odfPeakPointer->theta,
                                         *pd,
                                         *pm );

      ++ iopPrime;
      ++ pd;
      ++ pm;
      ++ odfPeakIndex;

    }

  }
  GKG_CATCH( "void gkg::SphericalHarmonicsOdfPeakFinder::pickBiggestPeaks( "
             "const std::vector< float >& shCoefficients, "
             "const std::list< gkg::SphericalHarmonicsOdfPeakFinder::Peak >& "
             "odfPeakClusters, "
             "std::vector< gkg::Vector3d< float > >& peakDirections, "
             "std::vector< float >& peakMagnitudes ) const" );

}

