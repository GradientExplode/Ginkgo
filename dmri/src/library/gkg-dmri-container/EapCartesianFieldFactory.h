#ifndef _gkg_dmri_container_EapCartesianFieldFactory_h_
#define _gkg_dmri_container_EapCartesianFieldFactory_h_


#include <gkg-core-pattern/Singleton.h>
#include <gkg-dmri-container/EapCartesianField.h>
#include <gkg-processing-coordinates/OrientationSet.h>
#include <vector>


namespace gkg
{


class EapCartesianFieldFactory : public Singleton< EapCartesianFieldFactory >
{

  public:

    typedef EapCartesianField* ( *Creator )(
                    RCPointer< Volume< float > > t2,
                    RCPointer< Volume< float > > dw,
                    RCPointer< Volume< int16_t > > mask,
                    const std::vector< int32_t >& iBox,
                    const std::vector< float >& fBox,
                    const std::vector< float >& superResolution,
                    const OrientationSet* outputOrientationSet,
                    const std::vector< double >& specificScalarParameters,
                    const std::vector< std::string >& specificStringParameters,
                    bool verbose );
    typedef void ( *ParameterChecker )(
                         std::vector< double >& specificScalarParameters,
                         std::vector< std::string >& specificStringParameters );

    ~EapCartesianFieldFactory();

    void registerCreator( const std::string& name, Creator creator );
    void registerParameterChecker( const std::string& name,
                                   ParameterChecker parameterChecker );
    std::list< std::string > getNames() const;

    EapCartesianField* create(
                    const std::string& name,
                    RCPointer< Volume< float > > t2,
                    RCPointer< Volume< float > > dw,
                    RCPointer< Volume< int16_t > > mask,
                    const std::vector< int32_t >& iBox,
                    const std::vector< float >& fBox,
                    const std::vector< float >& superResolution,
                    const OrientationSet* outputOrientationSet,
                    const std::vector< double >& specificScalarParameters,
                    const std::vector< std::string >& specificStringParameters,
                    bool verbose ) const;

    void checkOrInitializeDefaultParameters(
                         const std::string& name,
                         std::vector< double >& specificScalarParameters,
                         std::vector< std::string >& specificStringParameters );

    template < class F >
    static EapCartesianField* createEapCartesianField( 
                    RCPointer< Volume< float > > t2,
                    RCPointer< Volume< float > > dw,
                    RCPointer< Volume< int16_t > > mask,
                    const std::vector< int32_t >& iBox,
                    const std::vector< float >& fBox,
                    const std::vector< float >& superResolution,
                    const OrientationSet* outputOrientationSet,
                    const std::vector< double >& specificScalarParameters,
                    const std::vector< std::string >& specificStringParameters,
                    bool verbose );

  protected:

    friend class Singleton< EapCartesianFieldFactory >;

    EapCartesianFieldFactory();

    std::map< std::string, Creator > _creators;
    std::map< std::string, ParameterChecker > _parameterCheckers;

};


}


#endif
