#ifndef _gkg_dmri_odf_msmt_csd_MSMTConstrainedSphericalDeconvolutionFiberOdf_h_
#define _gkg_dmri_odf_msmt_csd_MSMTConstrainedSphericalDeconvolutionFiberOdf_h_


#include <gkg-dmri-odf/OrientationDistributionFunction.h>
#include <gkg-processing-numericalanalysis/Matrix.h>
#include <gkg-core-io/TypeOf.h>
#include <vector>


namespace gkg
{

template < class T > class Volume;
class HeaderedObject;
template < class T > class Vector3d;


class MSMTConstrainedSphericalDeconvolutionFiberOdf :
                                         public OrientationDistributionFunction
{

  public:

    template < class S >
    MSMTConstrainedSphericalDeconvolutionFiberOdf(
                              const OrientationSet* orientationSet,
                              const Volume< float >& t2,
                              const Volume< float >& dw,
                              const Vector3d< S >& site,
                              int32_t tissueCount,
                              const std::vector< int32_t >& shCoefficientCounts,
                              const Matrix& H,
                              const Matrix& minusCTransposed,
                              const Matrix& A,
                              int32_t maximumIterationCount,
                              double epsilonGap,
                              double epsilonResiduals,
                              double epsilonInfeasible );

    virtual ~MSMTConstrainedSphericalDeconvolutionFiberOdf();

  protected:


};


}


///////////////////////////////////////////////////////////////////////////////
// creating type name for MSMTConstrainedSphericalDeconvolutionFiberOdf 
///////////////////////////////////////////////////////////////////////////////

RegisterBaseType( gkg::MSMTConstrainedSphericalDeconvolutionFiberOdf,
                  gkg_MSMTConstrainedSphericalDeconvolutionFiberOdf );


#endif

