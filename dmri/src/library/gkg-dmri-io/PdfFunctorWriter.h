#ifndef _gkg_dmri_io_PdfFunctorWriter_h_
#define _gkg_dmri_io_PdfFunctorWriter_h_


#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-dmri-container/PdfCartesianField.h>
#include <string>
#include <list>
#include <vector>


namespace gkg
{


class PdfFunctorWriter
{

  public:

    PdfFunctorWriter( float rgbScale,
                      float meshScale,
                      float lowerGFAThreshold,
                      float upperGFAThreshold,
                      const std::string& volumeFormat,
                      const std::string& meshMapFormat,
                      const std::string& textureMapFormat,
                      bool ascii,
                      const std::vector< double >& furtherScalarParameters,
                      const std::vector< std::string >& furtherStringParameters,
                      bool verbose );
    virtual ~PdfFunctorWriter();



    void write( const std::list< std::string >& pdfFunctorNames,
                const std::list< std::string >& fileNames,
                PdfCartesianField& pdfCartesianField );

    static std::list< std::string > getNames();
    static std::list< std::string > getNamesForSiteTextureFunctorGeneration();

  protected:

    void write( const std::list< std::pair< std::string,
                                            std::string > >&
                  pdfFunctorAndFileNames,
                PdfCartesianField& pdfCartesianField );

    static void splitNames(
                   const std::list< std::string >& pdfFunctorNames,
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
