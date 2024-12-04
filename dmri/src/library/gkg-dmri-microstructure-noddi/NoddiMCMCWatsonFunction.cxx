#include <gkg-dmri-microstructure-noddi/NoddiMCMCWatsonFunction.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-transform/Rotation3dFunctions.h>
#include <gkg-processing-numericalanalysis/Matrix.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-processing-algobase/Math.h>
#include <gkg-core-exception/Exception.h>

#include <cmath>
#include <cstring>
#include <fstream>


gkg::NoddiMCMCWatsonFunction::NoddiMCMCWatsonFunction( 
                   gkg::RCPointer< gkg::Volume< float > > dw,
                   const gkg::OrientationSet& outputOrientationSet,
                   double noiseStandardDeviation )
                             : gkg::NoddiMCMCFunction( dw,
                                                       outputOrientationSet,
                                                       noiseStandardDeviation )
{

  try
  {

    _squareAndCrossInputOrientations = new float[ 6 * _inputOrientationCount ];

    if ( !_squareAndCrossInputOrientations )
    {

      throw std::runtime_error( "Cannot allocate input orientations buffer" );

    }

    std::vector< gkg::Vector3d< float > >::const_iterator
      io = _inputOrientations.begin(),
      ioe = _inputOrientations.end();
    float* iPtr = _squareAndCrossInputOrientations;
    
    while ( io != ioe )
    {

      //  ix^2
      *iPtr++ = io->x * io->x;
      //  iy^2
      *iPtr++ = io->y * io->y;
      //  iz^2
      *iPtr++ = io->z * io->z;
      //  2*ix*iy
      *iPtr++ = 2.0f * io->x * io->y;
      //  2*ix*iz
      *iPtr++ = 2.0f * io->x * io->z;
      //  2*iy*iz
      *iPtr++ = 2.0f * io->y * io->z;
      ++io;
      
    }
  
  }
  GKG_CATCH( "gkg::NoddiMCMCWatsonFunction::NoddiMCMCWatsonFunctionCuda( "
             "gkg::RCPointer< gkg::Volume< float > > dw, "
             "const gkg::OrientationSet& outputOrientationSet, "
             "double noiseStandardDeviation )" );

}


gkg::NoddiMCMCWatsonFunction::~NoddiMCMCWatsonFunction()
{

  delete [] _squareAndCrossInputOrientations;

}


void gkg::NoddiMCMCWatsonFunction::getValuesAt( const gkg::Vector& parameters,
                                                gkg::Vector& values ) const
{

  try
  {

    // pointing to parameters using references
    const double& isotropicFraction = parameters( 0 );
    const double oneMinusIsotropicFraction = 1.0 - isotropicFraction;
    const double& intracellularFraction = parameters( 1 );
    const double oneMinusIntracellularFraction = 1.0 - intracellularFraction;
    const double& kappa = parameters( 2 );
    gkg::Vector3d< float > principalOrientation( ( float )parameters( 3 ),
                                                 ( float )parameters( 4 ),
                                                 ( float )parameters( 5 ) );
    const double& parallelDiffusivity = parameters( 6 );
    const double& isotropicDiffusivity = parameters( 7 );
    // exvivo parameter for water trapped in glial cells; as in Alexander 2010,
    // stationary contribution can be considered as a pool of water molecules
    // trapped in a dot sphere (sphere of nul radius); and in this case, 
    // the diffusion attenuation si simply equal to A = 1
    const double& stationaryFraction = parameters( 8 );
    const double oneMinusStationaryFraction = 1.0 - stationaryFraction;

    // computing parallelDiffusivityPrime
    float tau1 = 1.0f / 3.0f;
    if ( kappa > 0.0 )
    {

#ifdef NODDI_NOT_OPTIMIZED
      tau1 = -1.0 / ( 2.0 * kappa ) +
             1.0 / ( 2.0 * this->_factory->getDawsonIntegral( 
                                                          std::sqrt( kappa ) ) *
                                                          std::sqrt( kappa ) );
#endif

#ifdef NODDI_OPTIMIZED
      double sqrtKappa = std::sqrt( kappa );
      tau1 = -0.5 / kappa +
             0.5 / ( this->_factory->getDawsonIntegral( sqrtKappa ) *
                     sqrtKappa );
#endif

    }

    // Optimization : since kappa is constant in this function,
    // 1F1(a,b,c) can be computed outside of the loop
    double inv1F1 = 0.0;
    double function1F1 = this->_factory->getHypergeometricFunction1F1( 0.5,
                                                                       1.5,
                                                                       kappa );

    if ( function1F1 )
    {
    
#ifdef NODDI_NOT_OPTIMIZED
      inv1F1 = 1.0 / function1F1;
#endif

#ifdef NODDI_OPTIMIZED
      inv1F1 = intracellularFraction / function1F1 / 
               double( _outputOrientationCount );
#endif

    }

    float parallelDiffusivityPrime = parallelDiffusivity *
                                     ( 1.0f - intracellularFraction *
                                       ( 1.0f - tau1 ) );
    float perpendicularDiffusivityPrime = parallelDiffusivity *
                                          ( 1.0f - intracellularFraction *
                                            ( ( 1.0f + tau1 ) / 2.0f ) );

    // computing the apparent extracellular diffusion tensor
#ifdef NODDI_NOT_OPTIMIZED
    gkg::Matrix apparentExtracellularTensor( 3, 3 );
    this->getApparentExtracellularTensor( principalOrientation,
                                          parallelDiffusivityPrime,
                                          perpendicularDiffusivityPrime,
                                          apparentExtracellularTensor );
#endif

#ifdef NODDI_OPTIMIZED
    float apparentExtracellularTensor[ 6 ];
    
    principalOrientation.normalize();
    this->getApparentExtracellularTensor( principalOrientation,
                                          parallelDiffusivityPrime,
                                          perpendicularDiffusivityPrime,
                                          &apparentExtracellularTensor[ 0 ] );

#endif

    // looping over measurements
    int32_t i = 0;
    int32_t o = 0;

    double intracellularContribution = 0.0;
    float extracellularContribution = 0.0;
    float csfContribution = 0.0;

#ifdef NODDI_OPTIMIZED
    // Use pointers to speed up computations
    std::vector< float >::const_iterator bPtr = _bValues.begin();
    float* inputPtr = _squareAndCrossInputOrientations;

    std::vector< float >::const_iterator
      bDiDo = _minusBiTimesSquareOfDiDotDo.begin();
    std::vector< gkg::Vector3d< float > >::const_iterator
      outputPtr = _outputOrientations.begin();
#endif

#ifdef NODDI_NOT_OPTIMIZED
    for ( i = 0; i < this->_inputOrientationCount; i++ )
#endif

#ifdef NODDI_OPTIMIZED
    for ( i = 0; i < this->_inputOrientationCount; i++, inputPtr += 6, bPtr++ )
#endif
    {

#ifdef NODDI_NOT_OPTIMIZED
      // creating input direction as a Vector
      gkg::Vector inputOrientation( this->_inputOrientations[ i ] );

      // computing restricted intracellular contribution
      intracellularContribution = 0.0f;
      for ( o = 0; o < this->_outputOrientationCount; o++ )
      {

        intracellularContribution += 
          this->_factory->getWatsonDistribution(
                        kappa,
                        principalOrientation,
                        this->_outputOrientations[ o ] ) *
          std::exp( this->_minusBiTimesSquareOfDiDotDo[
                                       i * this->_outputOrientationCount + o ] *
                    parallelDiffusivity );
                                           

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

      // computing mixture of compartments
      values( i ) = ( 1.0 - isotropicFraction ) *
                    ( ( 1.0 - stationaryFraction ) *
                    ( intracellularFraction * intracellularContribution +
                      ( 1.0 - intracellularFraction ) * 
                      extracellularContribution ) +
                      stationaryFraction ) +
                    isotropicFraction * csfContribution;
#endif

#ifdef NODDI_OPTIMIZED

      // computing restricted intracellular contribution
      intracellularContribution = 0.0;
      if ( inv1F1 )
      {

        std::vector< gkg::Vector3d< float > >::const_iterator      
          outPtr = outputPtr;
      
        for ( o = 0; o < this->_outputOrientationCount; o++ )
        {

          float dotProd = principalOrientation.x * outPtr->x +
	                           principalOrientation.y * outPtr->y + 
		                   principalOrientation.z * outPtr->z;
          intracellularContribution += std::exp( kappa * dotProd * dotProd + 
	        			         *bDiDo * parallelDiffusivity );

          ++outPtr;
          ++bDiDo;

        }
      
        intracellularContribution *= inv1F1;
      
      }
      else
      {
      
        bDiDo += _outputOrientationCount;
      
      }

      // computing hindered extracellular contribution
      float coef = 0.0f;
      float* t = apparentExtracellularTensor;
      const float* inPtr = inputPtr;
      
      // coef = i^T*D*i
      //      = Dxx*ix^2 + Dyy*iy^2 + Dzz*iz^2 +
      //        2*Dxy*ix*iy + 2*Dxz*ix*iz + 2*Dyz*iy*iz
      for ( o = 0; o < 6; o++ )
      {
      
        coef += *t++ * *inPtr++;
      
      }
      extracellularContribution = std::exp( -coef * *bPtr );

      // computing CSF contribution due to partial volume effect
      csfContribution = std::exp( -isotropicDiffusivity * *bPtr );

      // computing mixture of compartments
      values( i ) = oneMinusIsotropicFraction *
                    ( oneMinusStationaryFraction *
                      ( intracellularContribution +
                        oneMinusIntracellularFraction * 
                        extracellularContribution ) +
                      stationaryFraction ) +
                    isotropicFraction * csfContribution;

#endif

    }

  }
  GKG_CATCH( "void gkg::NoddiMCMCWatsonFunction::getValuesAt( "
             "const gkg::Vector& parameters, "
             "gkg::Vector& values ) const" );

}


void gkg::NoddiMCMCWatsonFunction::getModelAt(
         const gkg::Vector& parameters,
         const std::vector< float >& multipleShellBValues,
         const std::vector< std::vector< float > >& /*gradientCharacteristics*/,
         int32_t inputOrientationCount,
         std::vector< std::vector< float > >& values,
         std::vector< float >& coordinates ) const
{

  try
  {

#ifdef NODDI_OPTIMIZED
    // pointing to parameters using references
    const double& isotropicFraction = parameters( 0 );
    const double oneMinusIsotropicFraction = 1.0 - isotropicFraction;
    const double& intracellularFraction = parameters( 1 );
    const double oneMinusIntracellularFraction = 1.0 - intracellularFraction;
    const double& kappa = parameters( 2 );
    gkg::Vector3d< float > principalOrientation( ( float )parameters( 3 ),
                                                 ( float )parameters( 4 ),
                                                 ( float )parameters( 5 ) );
    const double& parallelDiffusivity = parameters( 6 );
    const double& isotropicDiffusivity = parameters( 7 );
    // exvivo parameter for water trapped in glial cells; as in Alexander 2010,
    // stationary contribution can be considered as a pool of water molecules
    // trapped in a dot sphere (sphere of nul radius); and in this case, 
    // the diffusion attenuation si simply equal to A = 1
    const double& stationaryFraction = parameters( 8 );
    const double oneMinusStationaryFraction = 1.0 - stationaryFraction;

    // computing parallelDiffusivityPrime
    float tau1 = 1.0f / 3.0f;
    if ( kappa > 0.0 )
    {

      double sqrtKappa = std::sqrt( kappa );
      tau1 = -0.5 / kappa +
             0.5 / ( this->_factory->getDawsonIntegral( sqrtKappa ) *
                     sqrtKappa );

    }

    // Optimization : since kappa is constant in this function,
    // 1F1(a,b,c) can be computed outside of the loop
    double inv1F1 = 0.0;
    double function1F1 = this->_factory->getHypergeometricFunction1F1( 0.5,
                                                                       1.5,
                                                                       kappa );

    if ( function1F1 )
    {
    
      inv1F1 = intracellularFraction / function1F1 / 
               double( _outputOrientationCount );
	       
    }

    float parallelDiffusivityPrime = parallelDiffusivity *
                                     ( 1.0f - intracellularFraction *
                                       ( 1.0f - tau1 ) );
    float perpendicularDiffusivityPrime = parallelDiffusivity *
                                          ( 1.0f - intracellularFraction *
                                            ( ( 1.0f + tau1 ) / 2.0f ) );

    // computing the apparent extracellular diffusion tensor
    float apparentExtracellularTensor[ 6 ];

    principalOrientation.normalize();
    this->getApparentExtracellularTensor( principalOrientation,
                                          parallelDiffusivityPrime,
                                          perpendicularDiffusivityPrime,
                                          &apparentExtracellularTensor[ 0 ] );

    // creating a trieder from principal orientation to get at least a
    // perpendicular axis to the principal axis
    gkg::Vector3d< float > secondAxis;
    gkg::Vector3d< float > thirdAxis;
    gkg::getRandomOrthonormalTriederFromFirstAxis( principalOrientation,
                                                   secondAxis,
                                                   thirdAxis );

    // reallocating values and coordinates
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

    // filling input orientations
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

    // allocating and filling bValues
    std::vector< float > bValues( inputOrientationCount * bValueCount );
    for ( b = 0; b < bValueCount; b++ )
    {

      for ( i = 0; i < inputOrientationCount; i++ )
      {

        bValues[ b * inputOrientationCount + i ] = 
                                                multipleShellBValues[ b ] * 1e6;

      }

    }

    // allocating and filling squareAndCrossInputOrientations
    std::vector< float >
      squareAndCrossInputOrientations( 6 * inputOrientationCount * bValueCount );
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

    // allocating and filling minusBiTimesSquareOfDiDotDo
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


    // looping over measurements
    int32_t o = 0;

    double intracellularContribution = 0.0;
    float extracellularContribution = 0.0;
    float csfContribution = 0.0;

    // Use pointers to speed up computations
    std::vector< float >::const_iterator bPtr = bValues.begin();
    std::vector< float >::const_iterator
                             inputPtr = squareAndCrossInputOrientations.begin();

    std::vector< float >::const_iterator
      bDiDo = minusBiTimesSquareOfDiDotDo.begin();
    std::vector< gkg::Vector3d< float > >::const_iterator
      outputPtr = _outputOrientations.begin();

    for ( b = 0; b < bValueCount; b++ )
    {

      for ( i = 0; i < inputOrientationCount; i++, inputPtr += 6, bPtr++ )
      {

        // computing restricted intracellular contribution
        intracellularContribution = 0.0f;
      
        if ( inv1F1 )
        {

          std::vector< gkg::Vector3d< float > >::const_iterator      
            outPtr = outputPtr;
      
          for ( o = 0; o < this->_outputOrientationCount; o++ )
          {

            float dotProd = principalOrientation.x * outPtr->x +
	                    principalOrientation.y * outPtr->y + 
		            principalOrientation.z * outPtr->z;
            intracellularContribution += std::exp( kappa * dotProd * dotProd + 
	        			         *bDiDo * parallelDiffusivity );

            ++outPtr;
            ++bDiDo;

          }
      
          intracellularContribution *= inv1F1;
      
        }
        else
        {
      
          bDiDo += _outputOrientationCount;
      
        }

        // computing hindered extracellular contribution
        float coef = 0.0f;
        float* t = apparentExtracellularTensor;
        std::vector< float >::const_iterator inPtr = inputPtr;
      
        // coef = i^T*D*i
        //      = Dxx*ix^2 + Dyy*iy^2 + Dzz*iz^2 +
        //        2*Dxy*ix*iy + 2*Dxz*ix*iz + 2*Dyz*iy*iz
        for ( o = 0; o < 6; o++ )
        {
      
          coef += *t++ * *inPtr++;
      
        }
        extracellularContribution = std::exp( -coef * *bPtr );

        // computing CSF contribution due to partial volume effect
        csfContribution = std::exp( -isotropicDiffusivity * *bPtr );

        // computing mixture of compartments
        values[ b ][ i ] = oneMinusIsotropicFraction *
                           ( oneMinusStationaryFraction *
                             ( intracellularContribution +
                               oneMinusIntracellularFraction * 
                               extracellularContribution ) +
                             stationaryFraction ) +
                           isotropicFraction * csfContribution;

      }

    }
#endif

  }
  GKG_CATCH( "void gkg::NoddiMCMCWatsonFunction::getModelAt( "
             "const gkg::Vector& parameters, "
             "const std::vector< float >& bValues, "
             "int32_t inputOrientationCount, "
             "std::vector< std::vector< float > >& values, "
             "std::vector< float >& coordinates ) const" );

}


gkg::OrientationDistributionFunction::BasisType 
gkg::NoddiMCMCWatsonFunction::getType() const
{

  try
  {

    return gkg::OrientationDistributionFunction::NoddiWatson;

  }
  GKG_CATCH( "gkg::OrientationDistributionFunction::BasisType "
             "gkg::NoddiMCMCWatsonFunction::getType() const" );


}


int32_t gkg::NoddiMCMCWatsonFunction::getParameterCount() const
{

  try
  {

    return 9;

  }
  GKG_CATCH( "int32_t "
             "gkg::NoddiMCMCWatsonFunction::getParameterCount() const" );

}


#ifdef NODDI_NOT_OPTIMIZED
void gkg::NoddiMCMCWatsonFunction::getApparentExtracellularTensor(
                            const gkg::Vector3d< float >& firstAxis,
                            float parallelDiffusivityPrime,
                            float perpendicularDiffusivityPrime,
                            Matrix& apparentExtracellularTensor ) const
{

  try
  {

    apparentExtracellularTensor.setZero();

    if ( std::fabs( firstAxis.x - 1.0f ) < 1e-4f )
    {

      apparentExtracellularTensor( 0, 0 ) = parallelDiffusivityPrime;
      apparentExtracellularTensor( 1, 1 ) = perpendicularDiffusivityPrime;
      apparentExtracellularTensor( 2, 2 ) = perpendicularDiffusivityPrime;

    }
    else if ( std::fabs( firstAxis.y - 1.0f ) <  1e-4f )
    {

      apparentExtracellularTensor( 0, 0 ) = perpendicularDiffusivityPrime;
      apparentExtracellularTensor( 1, 1 ) = parallelDiffusivityPrime;
      apparentExtracellularTensor( 2, 2 ) = perpendicularDiffusivityPrime;

    }
    else if ( std::fabs( firstAxis.z - 1.0f ) < 1e-4f )
    {

      apparentExtracellularTensor( 0, 0 ) = perpendicularDiffusivityPrime;
      apparentExtracellularTensor( 1, 1 ) = perpendicularDiffusivityPrime;
      apparentExtracellularTensor( 2, 2 ) = parallelDiffusivityPrime;

    }
    else
    {
      
      // e1 = firstAxis
      gkg::Vector3d< float > secondAxis( 0.0, 0.0, 0.0 );
      gkg::Vector3d< float > thirdAxis( 0.0, 0.0, 0.0 );
      
      gkg::getRandomOrthonormalTriederFromFirstAxis( firstAxis,
                                                     secondAxis,
                                                     thirdAxis );

      // computing rotation matrix
      gkg::Matrix rotationMatrix( 3, 3 );
      rotationMatrix.setColumn( 0, firstAxis );
      rotationMatrix.setColumn( 1, secondAxis );
      rotationMatrix.setColumn( 2, thirdAxis );

      // eigenvalue matrix
      gkg::Vector eigenValues( 3 );
      eigenValues( 0 ) = parallelDiffusivityPrime;
      eigenValues( 1 ) = perpendicularDiffusivityPrime;
      eigenValues( 2 ) = perpendicularDiffusivityPrime;

      apparentExtracellularTensor = rotationMatrix.
                                    getComposition( eigenValues.getDiagonal() ).
                                    getComposition(
                                            rotationMatrix.getTransposition() );

    }

  }
  GKG_CATCH( "void gkg::NoddiMCMCWatsonFunction::"
             "getApparentExtracellularTensor( "
             "const gkg::Vector3d< float >& principalOrientation, "
             "float parallelDiffusivityPrime, "
             "float perpendicularDiffusivityPrime, "
             "float* apparentExtracellularTensor ) const" );

}
   
#endif

#ifdef NODDI_OPTIMIZED
void gkg::NoddiMCMCWatsonFunction::getApparentExtracellularTensor(
                            const gkg::Vector3d< float >& firstAxis,
                            float parallelDiffusivityPrime,
                            float perpendicularDiffusivityPrime,
                            float* apparentExtracellularTensor ) const
{

  try
  {

    std::memset( apparentExtracellularTensor, 0, 6 * sizeof( float ) );

    if ( std::fabs( firstAxis.x - 1.0f ) < 1e-4f )
    {

      apparentExtracellularTensor[ 0 ] = parallelDiffusivityPrime;
      apparentExtracellularTensor[ 1 ] = perpendicularDiffusivityPrime;
      apparentExtracellularTensor[ 2 ] = perpendicularDiffusivityPrime;

    }
    else if ( std::fabs( firstAxis.y - 1.0f ) <  1e-4f )
    {

      apparentExtracellularTensor[ 0 ] = perpendicularDiffusivityPrime;
      apparentExtracellularTensor[ 1 ] = parallelDiffusivityPrime;
      apparentExtracellularTensor[ 2 ] = perpendicularDiffusivityPrime;

    }
    else if ( std::fabs( firstAxis.z - 1.0f ) < 1e-4f )
    {

      apparentExtracellularTensor[ 0 ] = perpendicularDiffusivityPrime;
      apparentExtracellularTensor[ 1 ] = perpendicularDiffusivityPrime;
      apparentExtracellularTensor[ 2 ] = parallelDiffusivityPrime;

    }
    else
    {
      
      // e1 = firstAxis
      gkg::Vector3d< float > secondAxis( 0.0, 0.0, 0.0 );
      gkg::Vector3d< float > thirdAxis( 0.0, 0.0, 0.0 );
      
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
  GKG_CATCH( "void gkg::NoddiMCMCWatsonFunction::"
             "getApparentExtracellularTensor( "
             "const gkg::Vector3d< float >& principalOrientation, "
             "float parallelDiffusivityPrime, "
             "float perpendicularDiffusivityPrime, "
             "float* apparentExtracellularTensor ) const" );

}
#endif
