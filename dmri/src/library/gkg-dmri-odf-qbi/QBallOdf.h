#ifndef _gkg_dmri_odf_qbi_QBallOdf_h_
#define _gkg_dmri_odf_qbi_QBallOdf_h_


#include <gkg-dmri-odf/OrientationDistributionFunction.h>
#include <gkg-core-io/TypeOf.h>


namespace gkg
{


template < class T > class Volume;
class HeaderedObject;
template < class T > class Vector3d;
class Matrix;
class PhiFunction;


class QBallOdf : public OrientationDistributionFunction
{

  public:

    template < class S >
    QBallOdf( const OrientationSet* orientationSet,
              const Volume< float >& t2,
              const Volume< float >& dw,
              const Vector3d< S >& site,
              int32_t equatorPointCount,
              const PhiFunction& phiFunction,
              const OrientationSet* basisFunctionCenterSet = 0 );

    template < class S >
    QBallOdf( const OrientationSet* orientationSet,
              const Volume< float >& t2,
              const Volume< float >& dw,
              const Vector3d< S >& site,
              const Matrix& reconstructionMatrix );

    virtual ~QBallOdf();

  protected:

    void getReconstructionMatrix( const HeaderedObject& headeredObject,
                                  const OrientationSet& outputOrientationSet,
                                  int32_t equatorPointCount,
                                  const PhiFunction& phiFunction,
                                  const OrientationSet& basisFunctionCenterSet,
                                  bool identicalBFCSetAndOOSet,
                                  Matrix& reconstructionMatrix );

    template < class S >
    void reconstruct( const OrientationSet* orientationSet,
                      const Volume< float >& t2,
                      const Volume< float >& dw,
                      const Vector3d< S >& site,
                      const Matrix& reconstructionMatrix );

};


}


///////////////////////////////////////////////////////////////////////////////
// creating type name for QBallOdf
///////////////////////////////////////////////////////////////////////////////

RegisterBaseType( gkg::QBallOdf, gkg_QBallOdf );


#endif
