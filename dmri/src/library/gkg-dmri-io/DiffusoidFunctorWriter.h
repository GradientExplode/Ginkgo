#ifndef _gkg_dmri_io_DiffusoidFunctorWriter_h_
#define _gkg_dmri_io_DiffusoidFunctorWriter_h_


#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-dmri-container/DiffusoidCartesianField.h>
#include <string>
#include <list>
#include <vector>


namespace gkg
{


class DiffusoidFunctorWriter
{

  public:

    DiffusoidFunctorWriter( float rgbScale,
                            float meshScale,
                            const std::string& volumeFormat,
                            const std::string& meshMapFormat,
                            const std::string& textureMapFormat,
                            bool ascii,
                            bool verbose );
    virtual ~DiffusoidFunctorWriter();



    void write( const std::list< std::string >& odfFunctorNames,
                const std::list< std::string >& fileNames,
                DiffusoidCartesianField& diffusoidCartesianField );

    static std::list< std::string > getNames();

  protected:

    void write( const std::list< std::pair< std::string,
                                            std::string > >&
                  diffusoidFunctorAndFileNames,
                DiffusoidCartesianField& diffusoidCartesianField );

    static void splitNames(
                   const std::list< std::string >& diffusoidFunctorNames,
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
