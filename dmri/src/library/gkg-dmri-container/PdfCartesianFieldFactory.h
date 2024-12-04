#ifndef _gkg_dmri_container_PdfCartesianFieldFactory_h_
#define _gkg_dmri_container_PdfCartesianFieldFactory_h_


#include <gkg-core-pattern/Singleton.h>
#include <gkg-dmri-container/PdfCartesianField.h>
#include <gkg-processing-coordinates/OrientationSet.h>
#include <vector>
#include <map>


namespace gkg
{


class PdfCartesianFieldFactory : public Singleton< PdfCartesianFieldFactory >
{

  public:

    typedef PdfCartesianField* ( *Creator1 )(
                    RCPointer< Volume< float > > t2,
                    RCPointer< Volume< float > > dw,
                    RCPointer< Volume< int16_t > > mask,
                    const std::vector< int32_t >& iBox,
                    const std::vector< float >& fBox,
                    const std::vector< float >& superResolution,
                    const std::map< float, OrientationSet >*
                                                    outputMultipleShellSampling,
                    const std::vector< double >& specificScalarParameters,
                    const std::vector< std::string >& specificStringParameters,
                    bool verbose );

    typedef PdfCartesianField* ( *Creator2 )(
                    RCPointer< Volume< float > > t2,
                    RCPointer< Volume< float > > dw,
                    RCPointer< Volume< int16_t > > mask,
                    const std::vector< int32_t >& iBox,
                    const std::vector< float >& fBox,
                    const std::vector< float >& superResolution,
                    const Vector3d< int32_t >* outputCartesianGridSize,
                    const Vector3d< double >* outputCartesianGridResolution,
                    const std::vector< double >& specificScalarParameters,
                    const std::vector< std::string >& specificStringParameters,
                    bool verbose );

    typedef void ( *ParameterChecker )(
                         std::vector< double >& specificScalarParameters,
                         std::vector< std::string >& specificStringParameters );

    ~PdfCartesianFieldFactory();

    void registerCreators( const std::string& name,
                           Creator1 creator1,
                           Creator2 creator2 );
    void registerParameterChecker( const std::string& name,
                                   ParameterChecker parameterChecker );
    std::list< std::string > getNames() const;

    PdfCartesianField* create(
                    const std::string& name,
                    RCPointer< Volume< float > > t2,
                    RCPointer< Volume< float > > dw,
                    RCPointer< Volume< int16_t > > mask,
                    const std::vector< int32_t >& iBox,
                    const std::vector< float >& fBox,
                    const std::vector< float >& superResolution,
                    const std::map< float, OrientationSet >*
                                                    outputMultipleShellSampling,
                    const std::vector< double >& specificScalarParameters,
                    const std::vector< std::string >& specificStringParameters,
                    bool verbose ) const;

    PdfCartesianField* create(
                    const std::string& name,
                    RCPointer< Volume< float > > t2,
                    RCPointer< Volume< float > > dw,
                    RCPointer< Volume< int16_t > > mask,
                    const std::vector< int32_t >& iBox,
                    const std::vector< float >& fBox,
                    const std::vector< float >& superResolution,
                    const Vector3d< int32_t >* outputCartesianGridSize,
                    const Vector3d< double >* outputCartesianGridResolution,
                    const std::vector< double >& specificScalarParameters,
                    const std::vector< std::string >& specificStringParameters,
                    bool verbose ) const;

    void checkOrInitializeDefaultParameters(
                         const std::string& name,
                         std::vector< double >& specificScalarParameters,
                         std::vector< std::string >& specificStringParameters );

    template < class F >
    static PdfCartesianField* createPdfCartesianField1( 
                    RCPointer< Volume< float > > t2,
                    RCPointer< Volume< float > > dw,
                    RCPointer< Volume< int16_t > > mask,
                    const std::vector< int32_t >& iBox,
                    const std::vector< float >& fBox,
                    const std::vector< float >& superResolution,
                    const std::map< float, OrientationSet >*
                                                    outputMultipleShellSampling,
                    const std::vector< double >& specificScalarParameters,
                    const std::vector< std::string >& specificStringParameters,
                    bool verbose );

    template < class F >
    static PdfCartesianField* createPdfCartesianField2( 
                    RCPointer< Volume< float > > t2,
                    RCPointer< Volume< float > > dw,
                    RCPointer< Volume< int16_t > > mask,
                    const std::vector< int32_t >& iBox,
                    const std::vector< float >& fBox,
                    const std::vector< float >& superResolution,
                    const Vector3d< int32_t >* outputCartesianGridSize,
                    const Vector3d< double >* outputCartesianGridResolution,
                    const std::vector< double >& specificScalarParameters,
                    const std::vector< std::string >& specificStringParameters,
                    bool verbose );

  protected:

    friend class Singleton< PdfCartesianFieldFactory >;

    PdfCartesianFieldFactory();

    std::map< std::string, Creator1 > _creator1s;
    std::map< std::string, Creator2 > _creator2s;
    std::map< std::string, ParameterChecker > _parameterCheckers;

};


}


#endif
