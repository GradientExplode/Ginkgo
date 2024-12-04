#ifndef _gkg_dmri_io_TensorFunctorWriter_h_
#define _gkg_dmri_io_TensorFunctorWriter_h_


#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-dmri-pdf-dti/TensorCartesianField.h>
#include <string>
#include <list>
#include <vector>


namespace gkg
{


class TensorFunctorWriter
{

  public:

    TensorFunctorWriter( float rgbScale,
                         float meshScale,
                         float lowerFAThreshold,
                         float upperFAThreshold,
                         const std::string& volumeFormat,
                         const std::string& meshMapFormat,
                         const std::string& textureMapFormat,
                         bool ascii,
                         bool verbose );
    virtual ~TensorFunctorWriter();



    void write( const std::list< std::string >& tensorFunctorNames,
                const std::list< std::string >& fileNames,
                TensorCartesianField& tensorCartesianField );

    static std::list< std::string > getNames();

  protected:

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
