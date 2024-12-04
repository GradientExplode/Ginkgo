#ifndef _gkg_anatomist_plugin_io_ATwoPoolModelFieldProcessingContext_h_
#define _gkg_anatomist_plugin_io_ATwoPoolModelFieldProcessingContext_h_


#include <gkg-communication-thread/LoopContext.h>
#include <gkg-dmri-pdf-two-pool-model/TwoPoolModelCartesianField.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-processing-transform/Translation3d.h>
#include <vector>

namespace gkg
{


class ATwoPoolModelFieldProcessingContext : public LoopContext< int32_t >
{

  public:

    ATwoPoolModelFieldProcessingContext(
                 TwoPoolModelCartesianField* twoPoolModelCartesianField,
                 std::vector< std::vector< float > >& fastTransformations,
                 std::vector< std::vector< float > >& slowTransformations,
                 std::vector< Translation3d< float > >& translations,
                 float meshScale );
    virtual ~ATwoPoolModelFieldProcessingContext();

    void doIt( int32_t startIndex, int32_t countIndex  );

  private:

    TwoPoolModelCartesianField* _twoPoolModelCartesianField;
    std::vector< std::vector< float > >& _fastTransformations;
    std::vector< std::vector< float > >& _slowTransformations;
    std::vector< Translation3d< float > >& _translations;
    float _meshScale; 
    Vector3d< double > _resolution;

};



}


#endif
