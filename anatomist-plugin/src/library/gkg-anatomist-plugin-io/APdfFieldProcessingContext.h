#ifndef _gkg_anatomist_plugin_io_APdfFieldProcessingContext_h_
#define _gkg_anatomist_plugin_io_APdfFieldProcessingContext_h_


#include <gkg-communication-thread/LoopContext.h>
#include <gkg-dmri-container/PdfCartesianField.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-processing-transform/Translation3d.h>
#include <vector>

namespace gkg
{


class APdfFieldProcessingContext : public LoopContext< int32_t >
{

  public:

    APdfFieldProcessingContext(
                 PdfCartesianField* pdfCartesianField,
                 std::vector< std::vector< float > >& transformations,
                 std::vector< Translation3d< float > >& translations,
                 float meshScale );
    virtual ~APdfFieldProcessingContext();

    void doIt( int32_t startIndex, int32_t countIndex  );

  private:

    PdfCartesianField* _pdfCartesianField;
    std::vector< std::vector< float > >& _transformations;
    std::vector< Translation3d< float > >& _translations;
    float _meshScale; 
    Vector3d< double > _resolution;

};



}


#endif
