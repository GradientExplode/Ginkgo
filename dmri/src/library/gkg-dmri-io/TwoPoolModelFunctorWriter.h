#ifndef _gkg_dmri_io_TwoPoolModelFunctorWriter_h_
#define _gkg_dmri_io_TwoPoolModelFunctorWriter_h_


#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-dmri-pdf-two-pool-model/TwoPoolModelCartesianField.h>
#include <string>
#include <list>
#include <vector>


namespace gkg
{


class TwoPoolModelFunctorWriter
{

  public:

    TwoPoolModelFunctorWriter( float rgbScale,
                               float meshScale,
                               float lowerFAThreshold,
                               float upperFAThreshold,
                               const std::string& volumeFormat,
                               const std::string& meshMapFormat,
                               const std::string& textureMapFormat,
                               bool ascii,
                               bool verbose );
    virtual ~TwoPoolModelFunctorWriter();



    void write( const std::list< std::string >& twoPoolModelFunctorNames,
                const std::list< std::string >& fileNames,
                TwoPoolModelCartesianField& twoPoolModelCartesianField );

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
