#include <gkg-dmri-odf-msmt-csd/MSMTConstrainedSphericalDeconvolutionFiberOdf.h>
#include <gkg-dmri-odf/OrientationDistributionFunction_i.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-numericalanalysis/Matrix.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-core-exception/Exception.h>
#include <cmath>


template < class S >
gkg::MSMTConstrainedSphericalDeconvolutionFiberOdf::
                                  MSMTConstrainedSphericalDeconvolutionFiberOdf(
                              const gkg::OrientationSet* orientationSet,
                              const gkg::Volume< float >& t2,
                              const gkg::Volume< float >& dw,
                              const gkg::Vector3d< S >& site,
                              int32_t tissueCount,
                              const std::vector< int32_t >& shCoefficientCounts,
                              const gkg::Matrix& H,
                              const gkg::Matrix& minusCTransposed,
                              const gkg::Matrix& A,
                              int32_t maximumIterationCount,
                              double epsilonGap,
                              double epsilonResiduals,
                              double epsilonInfeasible )
  : gkg::OrientationDistributionFunction(     
      gkg::OrientationDistributionFunction::MultiTissueSphericalHarmonics,
      orientationSet )
{

  try
  {

    if ( orientationSet )
    {

      // computing average T2 value and normalized signal vector
      float averageT2 = 0.0;
      gkg::Vector normalizedSignal;
      bool isOk = this->computeNormalizedSignal( t2,
                                                 dw,
                                                 site,
                                                 averageT2,
                                                 normalizedSignal );

      // resizing ODF vector to SH coefficient count
      int32_t allSHCoefficientCount = minusCTransposed.getSize1();
      _values.resize( tissueCount + allSHCoefficientCount + 1 );

      if ( !isOk )
      {

        std::vector< float >::iterator v = _values.begin();
        *v = ( float )tissueCount;
        ++ v;

        int32_t t = 0;
        int32_t s = 0;
        for ( t = 0; t < tissueCount; t++ )
        {

          const int32_t& shCoefficientCount = shCoefficientCounts[ t ];
          *v = ( float )shCoefficientCount;
          ++ v;

          // setting ODF SH coefficients to nul
          for ( s = 0; s < shCoefficientCount; s++ )
          {

            *v = 0.0f;
            ++ v;

          }

        }

        std::cout << "voxel " << site << " -> " << "notOk" << std::endl;
        std::cout << "========================================" << std::endl;

      }
      else
      {

        // getting a pointer to the numerical analysis factory
        gkg::NumericalAnalysisImplementationFactory* factory =
        gkg::NumericalAnalysisSelector::getInstance().getImplementationFactory();

        // processing ODF spherical harmonics coefficient(s)
        gkg::Vector odfSphericalHarmonicsCoefficients;

        gkg::Vector f = minusCTransposed.getComposition( normalizedSignal );

        std::cout << "f=" << f << std::endl;

        gkg::Vector nullVector( A.getSize1() );
        nullVector.setZero();

        std::cout << "nullVector=" << nullVector << std::endl;

        factory->getConvexQuadraticProgrammingSolution(
                                              H,
                                              f,
                                              A,
                                              nullVector,
                                              odfSphericalHarmonicsCoefficients,
                                              maximumIterationCount,
                                              epsilonGap,
                                              epsilonResiduals,
                                              epsilonInfeasible,
                                              true );

        // getting iterator to _values
        std::vector< float >::iterator v = _values.begin();

        // filling tissue count information
        *v = ( float )tissueCount;

        // copying SH coefficients
        int32_t t = 0;
        int32_t s = 0;
        int32_t sPrime = 0;
        for ( t = 0; t < tissueCount; t++ )
        {

          const int32_t& shCoefficientCount = shCoefficientCounts[ t ];
          *v = ( float )shCoefficientCount;
          ++ v;

          for ( s = 0; s < shCoefficientCount; s++ )
          {

            *v = ( float )odfSphericalHarmonicsCoefficients( sPrime );
            ++v;
            ++ sPrime;

          }

        }

      }

    }

  }
  GKG_CATCH( "template < class S > "
             "gkg::MSMTConstrainedSphericalDeconvolutionFiberOdf::"
             "MSMTConstrainedSphericalDeconvolutionFiberOdf( "
             "const gkg::OrientationSet* orientationSet, "
             "const gkg::Volume< float >& t2, "
             "const gkg::Volume< float >& dw, "
             "const gkg::Vector3d< S >& site, "
             "const gkg::Matrix& H, "
             "const gkg::Matrix& minusC )" );

}


gkg::MSMTConstrainedSphericalDeconvolutionFiberOdf::
                                ~MSMTConstrainedSphericalDeconvolutionFiberOdf()
{
}


template
gkg::MSMTConstrainedSphericalDeconvolutionFiberOdf::
                                  MSMTConstrainedSphericalDeconvolutionFiberOdf(
                             const gkg::OrientationSet* orientationSet,
                             const gkg::Volume< float >& t2,
                             const gkg::Volume< float >& dw,
                             const gkg::Vector3d< int32_t >& site,
                             int32_t tissueCount,
                             const std::vector< int32_t >& shCoefficientCounts,
                             const gkg::Matrix& H,
                             const gkg::Matrix& minusC,
                             const Matrix& A,
                             int32_t maximumIterationCount,
                             double epsilonGap,
                             double epsilonResiduals,
                             double epsilonInfeasible );

template
gkg::MSMTConstrainedSphericalDeconvolutionFiberOdf::
                                  MSMTConstrainedSphericalDeconvolutionFiberOdf(
                             const gkg::OrientationSet* orientationSet,
                             const gkg::Volume< float >& t2,
                             const gkg::Volume< float >& dw,
                             const gkg::Vector3d< float >& site,
                             int32_t tissueCount,
                             const std::vector< int32_t >& shCoefficientCounts,
                             const gkg::Matrix& H,
                             const gkg::Matrix& minusC,
                             const Matrix& A,
                             int32_t maximumIterationCount,
                             double epsilonGap,
                             double epsilonResiduals,
                             double epsilonInfeasible );
