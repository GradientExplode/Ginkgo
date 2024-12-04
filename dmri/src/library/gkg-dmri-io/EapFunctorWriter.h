#ifndef _gkg_dmri_io_EapFunctorWriter_h_
#define _gkg_dmri_io_EapFunctorWriter_h_


#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-dmri-container/EapCartesianField.h>
#include <string>
#include <list>
#include <vector>

namespace gkg
{


class EapFunctorWriter
{

  public:

    EapFunctorWriter( float rgbScale,
		      float meshScale,
		      std::vector< float > eapMeshRadii,
		      bool meshOpacity,
		      bool marginalOdf,
		      std::vector< float > odfIntegralLowerUpperLimits,
                      const std::string& volumeFormat,
                      const std::string& meshMapFormat,
                      const std::string& textureMapFormat,
                      bool ascii,
                      bool verbose );
    virtual ~EapFunctorWriter();

    void write( const std::list< std::string >& eapFunctorNames,
                const std::list< std::string >& fileNames,
                EapCartesianField& eapCartesianField );

    static std::list< std::string > getNames();

  protected:

    void write( const std::list< std::pair< std::string,
                                            std::string > >&
		eapFunctorAndFileNames,
                EapCartesianField& eapCartesianField );

    static void splitNames(
		   const std::list< std::string >& eapFunctorNames,
                   const std::list< std::string >& fileNames,
                   std::list< std::pair< std::string,
		   std::string > >& namesNotRequiringStandardBasis,
                   std::list< std::pair< std::string,
                      std::string > >& namesRequiringStandardBasis );


    std::vector< double > _scalarParameters;
    std::vector< std::string > _stringParameters;
    std::string _volumeFormat;
    std::string _meshMapFormat;
    std::string _textureMapFormat;
    bool _ascii;
    bool _verbose;

};


}


#endif
