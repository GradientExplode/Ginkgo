#ifndef _gkg_dmri_pdf_dti_TensorCartesianFieldFactory_h_
#define _gkg_dmri_pdf_dti_TensorCartesianFieldFactory_h_


#include <gkg-core-pattern/Singleton.h>
#include <gkg-dmri-pdf-dti/TensorCartesianField.h>
#include <gkg-processing-coordinates/OrientationSet.h>
#include <vector>


namespace gkg
{


class TensorCartesianFieldFactory :
                                 public Singleton< TensorCartesianFieldFactory >
{

  public:

    ~TensorCartesianFieldFactory();

    TensorCartesianField* create(
                    RCPointer< Volume< float > > t2,
                    RCPointer< Volume< float > > dw,
                    RCPointer< Volume< int16_t > > mask,
                    const std::vector< int32_t >& iBox,
                    const std::vector< float >& fBox,
                    const std::vector< float >& superResolution,
                    const std::vector< double >& specificScalarParameters,
                    const std::vector< std::string >& specificStringParameters,
                    bool verbose ) const;

    void checkOrInitializeDefaultParameters(
                         std::vector< double >& specificScalarParameters,
                         std::vector< std::string >& specificStringParameters );

  protected:

    friend class Singleton< TensorCartesianFieldFactory >;

    TensorCartesianFieldFactory();

};


}


#endif
