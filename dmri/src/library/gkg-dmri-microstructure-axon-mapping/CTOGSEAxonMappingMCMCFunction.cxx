#include <gkg-dmri-microstructure-axon-mapping/CTOGSEAxonMappingMCMCFunction.h>
#include <gkg-dmri-microstructure-axon-mapping/CTOGSEIntracellularContributionCache.h>
#include <gkg-dmri-microstructure-axon-mapping/CTOGSEDefines.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-transform/Rotation3dFunctions.h>
#include <gkg-processing-numericalanalysis/Matrix.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-processing-algobase/Math.h>
#include <gkg-core-object/GenericObject_i.h>
#include <gkg-core-exception/Exception.h>

#include <cmath>
#include <cstring>




gkg::CTOGSEAxonMappingMCMCFunction::CTOGSEAxonMappingMCMCFunction( 
                                gkg::RCPointer< gkg::Volume< float > > dw,
                                const gkg::OrientationSet& outputOrientationSet,
                                double noiseStandardDeviation )
                                   : gkg::AxonMappingMCMCFunction(
                                                        dw,
                                                        outputOrientationSet,
                                                        noiseStandardDeviation )
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // allocating structure to speed-up computation
    ////////////////////////////////////////////////////////////////////////////

    _squareAndCrossInputOrientations = new double[ 6 * 
                                                this->_inputOrientationCount ];

    if ( !_squareAndCrossInputOrientations )
    {

      throw std::runtime_error( "Cannot allocate input orientations buffer" );

    }
    std::vector< gkg::Vector3d< float > >::const_iterator
      io = this->_inputOrientations.begin(),
      ioe = this->_inputOrientations.end();
    double* iPtr = _squareAndCrossInputOrientations;
    
    while ( io != ioe )
    {

      //  ix^2
      *iPtr++ = ( double )io->x * ( double )io->x;
      //  iy^2
      *iPtr++ = ( double )io->y * ( double )io->y;
      //  iz^2
      *iPtr++ = ( double )io->z * ( double )io->z;
      //  2*ix*iy
      *iPtr++ = 2.0 * ( double )io->x * ( double )io->y;
      //  2*ix*iz
      *iPtr++ = 2.0 * ( double )io->x * ( double )io->z;
      //  2*iy*iz
      *iPtr++ = 2.0 * ( double )io->y * ( double )io->z;
      ++io;
      
    }

  }
  GKG_CATCH( "gkg::CTOGSEAxonMappingMCMCFunction::"
             "CTOGSEAxonMappingMCMCFunction( "
             "gkg::RCPointer< gkg::Volume< float > > dw, "
             "const gkg::OrientationSet& outputOrientationSet, "
             "double noiseStandardDeviation )" );

}


gkg::CTOGSEAxonMappingMCMCFunction::~CTOGSEAxonMappingMCMCFunction()
{

  if ( _squareAndCrossInputOrientations )
  {

    delete [] _squareAndCrossInputOrientations;

  }

}


void gkg::CTOGSEAxonMappingMCMCFunction::getValuesAt( 
                                                  const gkg::Vector& parameters,
                                                  gkg::Vector& values ) const
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // pointing to parameters using references
    ////////////////////////////////////////////////////////////////////////////

    const double& isotropicFraction =
      parameters( CTOGSE_IsotropicFraction );
    const double& isotropicDiffusivity = 
      parameters( CTOGSE_IsotropicDiffusivity );
    const double& extraPerpendicularDiffusivity = 
      parameters( CTOGSE_ExtraPerpendicularDiffusivity );
    const double& stationaryFraction =
      parameters( CTOGSE_StationaryFraction );
    const double& intracellularFraction =
      parameters( CTOGSE_IntracellularFraction );
    const double& kappa =
      parameters( CTOGSE_Kappa );
    const double& parallelDiffusivity =
      parameters( CTOGSE_ParallelDiffusivity );
    const double& axonDiameter =
      parameters( CTOGSE_AxonDiameter );
    gkg::Vector3d< double > principalOrientation(
                                parameters( CTOGSE_PrincipalOrientation ),
                                parameters( CTOGSE_PrincipalOrientation + 1 ),
                                parameters( CTOGSE_PrincipalOrientation + 2 ) );

    double oneMinusIsotropicFraction = 1.0 - isotropicFraction;
    double oneMinusStationaryFraction = 1.0 - stationaryFraction;
    double oneMinusIntracellularFraction = 1.0 - intracellularFraction;


    ////////////////////////////////////////////////////////////////////////////
    // computing extra-cellular tensor
    ////////////////////////////////////////////////////////////////////////////

    // computing parallelDiffusivityPrime and perpendicularDiffusivityPrime
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
    double perpendicularDiffusivityPrime = extraPerpendicularDiffusivity;

    // computing the apparent extracellular diffusion tensor
    double apparentExtracellularTensor[ 6 ];
    
    principalOrientation.normalize();
    this->getApparentExtracellularTensor( principalOrientation,
                                          parallelDiffusivityPrime,
                                          perpendicularDiffusivityPrime,
                                          &apparentExtracellularTensor[ 0 ] );


    ////////////////////////////////////////////////////////////////////////////
    // looping over measurements
    ////////////////////////////////////////////////////////////////////////////

    double isotropicContribution = 0.0;
    double extracellularContribution = 0.0;
    double stationaryContribution = 0.0;
    double intracellularContribution = 0.0;
    int32_t i = 0;
    int32_t o = 0;

    // Use pointers to speed up computations
    const gkg::CTOGSEIntracellularContributionCache& 
      ctogseIntracellularContributionCache =
        gkg::CTOGSEIntracellularContributionCache::getInstance();

    std::vector< double >::const_iterator 
      bPtr = ctogseIntracellularContributionCache.getBValues().begin();

    double* inputPtr = _squareAndCrossInputOrientations;

    gkg::Vector3d< double > diffusionOrientation;
    for ( i = 0; i < this->_inputOrientationCount; i++, inputPtr += 6, bPtr++ )
    {

      //////////////////////////////////////////////////////////////////////////
      // computing dot product between input diffusion direction and principal
      // direction
      //////////////////////////////////////////////////////////////////////////

      const gkg::Vector3d< float >& 
       inputOrientation = this->_inputOrientations[ i ];
      diffusionOrientation.x = ( double )inputOrientation.x;
      diffusionOrientation.y = ( double )inputOrientation.y;
      diffusionOrientation.z = ( double )inputOrientation.z;
      double dotProduct = diffusionOrientation.dot( principalOrientation );
      if ( dotProduct < -1.0 )
      {

        dotProduct = -1.0;

      }
      else if ( dotProduct > +1.0 )
      {

        dotProduct = +1.0;

      }

      //////////////////////////////////////////////////////////////////////////
      // contribution of isotropic compartment
      //////////////////////////////////////////////////////////////////////////

      isotropicContribution = std::exp( -*bPtr * isotropicDiffusivity );

      //////////////////////////////////////////////////////////////////////////
      // contribution of extracellular compartment
      //////////////////////////////////////////////////////////////////////////

      // coef = i^T*D*i
      //      = Dxx*ix^2 + Dyy*iy^2 + Dzz*iz^2 +
      //        2*Dxy*ix*iy + 2*Dxz*ix*iz + 2*Dyz*iy*iz
      double diffusionCoefficient = 0.0;
      double* t = apparentExtracellularTensor;
      const double* inPtr = inputPtr;
      
      for ( o = 0; o < 6; o++ )
      {
      
        diffusionCoefficient += *t++ * *inPtr++;
      
      }
      extracellularContribution = std::exp( -diffusionCoefficient * *bPtr );

      //////////////////////////////////////////////////////////////////////////
      // contribution of stationary compartment
      //////////////////////////////////////////////////////////////////////////

      stationaryContribution = 1.0;

      //////////////////////////////////////////////////////////////////////////
      // contribution of intracellular compartment
      //////////////////////////////////////////////////////////////////////////

      intracellularContribution = ctogseIntracellularContributionCache.
                              getIntracellularContribution( i,
                                                            kappa,
                                                            dotProduct,
                                                            parallelDiffusivity,
                                                            axonDiameter );

      //////////////////////////////////////////////////////////////////////////
      // composing multicompartmental signal
      //////////////////////////////////////////////////////////////////////////

      values( i ) = oneMinusIsotropicFraction *
                    ( oneMinusStationaryFraction *
                      ( intracellularFraction * 
                        intracellularContribution +
                        oneMinusIntracellularFraction * 
                        extracellularContribution ) +
                      stationaryFraction * stationaryContribution ) +
                    isotropicFraction * isotropicContribution;

    }

  }
  GKG_CATCH( "void gkg::CTOGSEAxonMappingMCMCFunction::getValuesAt( "
             "const gkg::Vector& parameters, "
             "gkg::Vector& values ) const" );

}


void gkg::CTOGSEAxonMappingMCMCFunction::getModelAt(
       const gkg::Vector& parameters,
       const std::vector< float >& multipleShellBValues,
       const std::vector< std::vector< float > >& /* gradientCharacteristics */,
       int32_t inputOrientationCount,
       std::vector< std::vector< float > >& values,
       std::vector< float >& coordinates ) const
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // pointing to parameters using references
    ////////////////////////////////////////////////////////////////////////////

    const double& isotropicFraction =
      parameters( CTOGSE_IsotropicFraction );
    const double& isotropicDiffusivity = 
      parameters( CTOGSE_IsotropicDiffusivity );
    const double& extraPerpendicularDiffusivity = 
      parameters( CTOGSE_ExtraPerpendicularDiffusivity );
    const double& intracellularFraction =
      parameters( CTOGSE_IntracellularFraction );
    const double& kappa =
      parameters( CTOGSE_Kappa );
    const double& parallelDiffusivity =
      parameters( CTOGSE_ParallelDiffusivity );
    const double& axonDiameter =
      parameters( CTOGSE_AxonDiameter );
    gkg::Vector3d< float > principalOrientation(
                       ( float )parameters( CTOGSE_PrincipalOrientation ),
                       ( float )parameters( CTOGSE_PrincipalOrientation + 1 ),
                       ( float )parameters( CTOGSE_PrincipalOrientation + 2 ) );
    gkg::Vector3d< double > principalOrientationDouble(
                                parameters( CTOGSE_PrincipalOrientation ),
                                parameters( CTOGSE_PrincipalOrientation + 1 ),
                                parameters( CTOGSE_PrincipalOrientation + 2 ) );

    double oneMinusIsotropicFraction = 1.0 - isotropicFraction;
    double oneMinusIntracellularFraction = 1.0 - intracellularFraction;


    ////////////////////////////////////////////////////////////////////////////
    // computing extra-cellular tensor
    ////////////////////////////////////////////////////////////////////////////

    // computing parallelDiffusivityPrime and perpendicularDiffusivityPrime
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
    double perpendicularDiffusivityPrime = extraPerpendicularDiffusivity;

    // computing the apparent extracellular diffusion tensor
    double apparentExtracellularTensor[ 6 ];
    
    principalOrientation.normalize();
    this->getApparentExtracellularTensor( principalOrientationDouble,
                                          parallelDiffusivityPrime,
                                          perpendicularDiffusivityPrime,
                                          &apparentExtracellularTensor[ 0 ] );



    ////////////////////////////////////////////////////////////////////////////
    // creating a trieder from principal orientation to get at least a
    // perpendicular axis to the principal axis
    ////////////////////////////////////////////////////////////////////////////

    gkg::Vector3d< float > secondAxis;
    gkg::Vector3d< float > thirdAxis;
    gkg::getRandomOrthonormalTriederFromFirstAxis( principalOrientation,
                                                   secondAxis,
                                                   thirdAxis );


    ////////////////////////////////////////////////////////////////////////////
    // reallocating values and coordinates
    ////////////////////////////////////////////////////////////////////////////

    int32_t bValueCount = ( int32_t )multipleShellBValues.size();
    values.resize( bValueCount );
    std::vector< std::vector< float > >::iterator
      v = values.begin(),
      ve = values.end();
    while ( v != ve )
    {

      v->resize( inputOrientationCount );
      ++ v;

    }
    coordinates.resize( inputOrientationCount );


    ////////////////////////////////////////////////////////////////////////////
    // filling input orientations
    ////////////////////////////////////////////////////////////////////////////

    int32_t b = 0;
    int32_t i = 0;

    float step = 1.0f / ( float )( inputOrientationCount - 1 );

    std::vector< gkg::Vector3d< float > >
      inputOrientations( inputOrientationCount * bValueCount );

    float cosTheta = 0.0f;
    for ( i = 0; i < inputOrientationCount; i++ )
    {

      cosTheta = ( float )i * step;
      coordinates[ i ] = cosTheta;

      for ( b = 0; b < bValueCount; b++ )
      {

        inputOrientations[ b * inputOrientationCount + i ] = 
            principalOrientation * cosTheta +
            secondAxis * ( float )std::sqrt( 1.0 - cosTheta * cosTheta );

      }

    }


    ////////////////////////////////////////////////////////////////////////////
    // allocating and filling bValues
    ////////////////////////////////////////////////////////////////////////////

    std::vector< float > bValues( inputOrientationCount * bValueCount );
    for ( b = 0; b < bValueCount; b++ )
    {

      for ( i = 0; i < inputOrientationCount; i++ )
      {

        bValues[ b * inputOrientationCount + i ] = multipleShellBValues[ b ] *
                                                   1e6;

      }

    }


    ////////////////////////////////////////////////////////////////////////////
    // allocating and filling squareAndCrossInputOrientations
    ////////////////////////////////////////////////////////////////////////////

    std::vector< float >
      squareAndCrossInputOrientations( 6 * inputOrientationCount * 
                                                                  bValueCount );
    std::vector< gkg::Vector3d< float > >::const_iterator
      cio = inputOrientations.begin(),
      cioe = inputOrientations.end();
    std::vector< float >::iterator
      iPtr = squareAndCrossInputOrientations.begin();
    
    while ( cio != cioe )
    {

      //  ix^2
      *iPtr++ = cio->x * cio->x;
      //  iy^2
      *iPtr++ = cio->y * cio->y;
      //  iz^2
      *iPtr++ = cio->z * cio->z;
      //  2*ix*iy
      *iPtr++ = 2.0f * cio->x * cio->y;
      //  2*ix*iz
      *iPtr++ = 2.0f * cio->x * cio->z;
      //  2*iy*iz
      *iPtr++ = 2.0f * cio->y * cio->z;

      ++ cio;
      
    }
  

    ////////////////////////////////////////////////////////////////////////////
    // allocating and filling minusBiTimesSquareOfDiDotDo
    ////////////////////////////////////////////////////////////////////////////
    std::vector< float >
      minusBiTimesSquareOfDiDotDo( inputOrientationCount * bValueCount *
                                   _outputOrientationCount );


    cio = inputOrientations.begin();
    std::vector< gkg::Vector3d< float > >::const_iterator
      oo, ooe = _outputOrientations.end();
    std::vector< float >::const_iterator bi = bValues.begin();

    int32_t j = 0;
    while ( cio != cioe )
    {

      oo = _outputOrientations.begin();

      while ( oo != ooe )
      {
      
        float dotProd = cio->x * oo->x +  // ix*ox
	                cio->y * oo->y +  // iy*oy
			cio->z * oo->z;   // iz*oz
	minusBiTimesSquareOfDiDotDo[ j ] = -dotProd * dotProd * *bi;

        ++oo;
        j++;
      
      }

      ++ cio;
      ++ bi;

    }

    ////////////////////////////////////////////////////////////////////////////
    // looping over measurements
    ////////////////////////////////////////////////////////////////////////////

    double isotropicContribution = 0.0;
    double extracellularContribution = 0.0;
    double intracellularContribution = 0.0;
    int32_t o = 0;

    // Use pointers to speed up computations
    std::vector< float >::const_iterator bPtr = bValues.begin();
    std::vector< float >::const_iterator
                             inputPtr = squareAndCrossInputOrientations.begin();

    for ( b = 0; b < bValueCount; b++ )
    {

      for ( i = 0; i < inputOrientationCount; i++, inputPtr += 6, bPtr++ )
      {

        ////////////////////////////////////////////////////////////////////////
        // computing dot product between input diffusion direction and principal
        // direction
        ////////////////////////////////////////////////////////////////////////
 
        const gkg::Vector3d< float >&
          diffusionOrientation = inputOrientations[ i ];
        double dotProduct = diffusionOrientation.dot( principalOrientation );
        if ( dotProduct < -1.0 )
        {

          dotProduct = -1.0f;

        }
        else if ( dotProduct > +1.0 )
        {

          dotProduct = +1.0f;

        }


        ////////////////////////////////////////////////////////////////////////
        // contribution of isotropic compartment
        ////////////////////////////////////////////////////////////////////////

        isotropicContribution = std::exp( -isotropicDiffusivity * *bPtr );


        ////////////////////////////////////////////////////////////////////////
        // contribution of extracellular compartment
        ////////////////////////////////////////////////////////////////////////

        // coef = i^T*D*i
        //      = Dxx*ix^2 + Dyy*iy^2 + Dzz*iz^2 +
        //        2*Dxy*ix*iy + 2*Dxz*ix*iz + 2*Dyz*iy*iz
        float diffusionCoefficient = 0.0f;
        double* t = apparentExtracellularTensor;
        std::vector< float >::const_iterator inPtr = inputPtr;
        for ( o = 0; o < 6; o++ )
        {
    
          diffusionCoefficient += ( float )*t++ * *inPtr++;
    
        }
        extracellularContribution = std::exp( -diffusionCoefficient * *bPtr );


        ////////////////////////////////////////////////////////////////////////
        // contribution of intracellular compartment
        ////////////////////////////////////////////////////////////////////////

        intracellularContribution = 
                       gkg::CTOGSEIntracellularContributionCache::getInstance().
                              getIntracellularContribution( b, i,
                                                            kappa,
                                                            dotProduct,
                                                            parallelDiffusivity,
                                                            axonDiameter );

        ////////////////////////////////////////////////////////////////////////
        // computing mixture of compartments
        ////////////////////////////////////////////////////////////////////////

          values[ b ][ i ] = oneMinusIsotropicFraction *
                             ( intracellularFraction * 
                               intracellularContribution +
                               oneMinusIntracellularFraction * 
                               extracellularContribution ) +
                               isotropicFraction * isotropicContribution;

      }

    }

  }
  GKG_CATCH( "void gkg::CTOGSEAxonMappingMCMCFunction::getModelAt( "
             "const gkg::Vector& parameters, "
             "const std::vector< float >& multipleShellBValues, "
             "const std::vector< std::vector< float > >& "
             "gradientCharacteristics, "
             "int32_t inputOrientationCount, "
             "std::vector< std::vector< float > >& values, "
             "std::vector< float >& coordinates ) const" );

}


gkg::OrientationDistributionFunction::BasisType 
gkg::CTOGSEAxonMappingMCMCFunction::getType() const
{

  try
  {

    return gkg::OrientationDistributionFunction::CTOGSEAxonMapping;

  }
  GKG_CATCH( "gkg::OrientationDistributionFunction::BasisType "
             "gkg::CTOGSEAxonMappingMCMCFunction::getType() const" );


}


int32_t gkg::CTOGSEAxonMappingMCMCFunction::getParameterCount() const
{

  try
  {

    return 11;

  }
  GKG_CATCH( "int32_t "
             "gkg::CTOGSEAxonMappingMCMCFunction::getParameterCount() const" );

}


void gkg::CTOGSEAxonMappingMCMCFunction::getApparentExtracellularTensor(
                            const gkg::Vector3d< double >& firstAxis,
                            double parallelDiffusivityPrime,
                            double perpendicularDiffusivityPrime,
                            double* apparentExtracellularTensor ) const
{

  try
  {

    std::memset( apparentExtracellularTensor, 0, 6 * sizeof( double ) );

    if ( std::fabs( firstAxis.x - 1.0 ) < 1e-4 )
    {

      apparentExtracellularTensor[ 0 ] = parallelDiffusivityPrime;
      apparentExtracellularTensor[ 1 ] = perpendicularDiffusivityPrime;
      apparentExtracellularTensor[ 2 ] = perpendicularDiffusivityPrime;

    }
    else if ( std::fabs( firstAxis.y - 1.0 ) <  1e-4 )
    {

      apparentExtracellularTensor[ 0 ] = perpendicularDiffusivityPrime;
      apparentExtracellularTensor[ 1 ] = parallelDiffusivityPrime;
      apparentExtracellularTensor[ 2 ] = perpendicularDiffusivityPrime;

    }
    else if ( std::fabs( firstAxis.z - 1.0 ) < 1e-4 )
    {

      apparentExtracellularTensor[ 0 ] = perpendicularDiffusivityPrime;
      apparentExtracellularTensor[ 1 ] = perpendicularDiffusivityPrime;
      apparentExtracellularTensor[ 2 ] = parallelDiffusivityPrime;

    }
    else
    {
      
      // e1 = firstAxis
      gkg::Vector3d< double > secondAxis( 0.0, 0.0, 0.0 );
      gkg::Vector3d< double > thirdAxis( 0.0, 0.0, 0.0 );
      
      gkg::getRandomOrthonormalTriederFromFirstAxis( firstAxis,
                                                     secondAxis,
                                                     thirdAxis );
      apparentExtracellularTensor[ 0 ] =
        parallelDiffusivityPrime * firstAxis.x * firstAxis.x +
	perpendicularDiffusivityPrime * ( secondAxis.x * secondAxis.x +
	                                  thirdAxis.x * thirdAxis.x );
      apparentExtracellularTensor[ 1 ] =
        parallelDiffusivityPrime * firstAxis.y * firstAxis.y +
	perpendicularDiffusivityPrime * ( secondAxis.y * secondAxis.y +
	                                  thirdAxis.y * thirdAxis.y );
      apparentExtracellularTensor[ 2 ] =
        parallelDiffusivityPrime * firstAxis.z * firstAxis.z +
	perpendicularDiffusivityPrime * ( secondAxis.z * secondAxis.z +
	                                  thirdAxis.z * thirdAxis.z );
      apparentExtracellularTensor[ 3 ] =
        parallelDiffusivityPrime * firstAxis.x * firstAxis.y +
	perpendicularDiffusivityPrime * ( secondAxis.x * secondAxis.y +
	                                  thirdAxis.x * thirdAxis.y );
      apparentExtracellularTensor[ 4 ] =
        parallelDiffusivityPrime * firstAxis.x * firstAxis.z +
	perpendicularDiffusivityPrime * ( secondAxis.x * secondAxis.z +
	                                  thirdAxis.x * thirdAxis.z );
      apparentExtracellularTensor[ 5 ] =
        parallelDiffusivityPrime * firstAxis.y * firstAxis.z +
	perpendicularDiffusivityPrime * ( secondAxis.y * secondAxis.z +
	                                  thirdAxis.y * thirdAxis.z );

    }

  }
  GKG_CATCH( "void gkg::CTOGSEAxonMappingMCMCFunction::"
             "getApparentExtracellularTensor( "
             "const gkg::Vector3d< double >& principalOrientation, "
             "double parallelDiffusivityPrime, "
             "double perpendicularDiffusivityPrime, "
             "double* apparentExtracellularTensor ) const" );

}
