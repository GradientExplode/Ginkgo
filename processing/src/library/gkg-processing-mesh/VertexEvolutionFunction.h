#ifndef _gkg_processing_mesh_VertexEvolutionFunction_h_
#define _gkg_processing_mesh_VertexEvolutionFunction_h_


#include <gkg-core-pattern/RCPointer.h>
#include <gkg-processing-coordinates/Vector3d.h>


namespace gkg
{


class VertexEvolutionFunction : public RCObject
{

  public:

    virtual ~VertexEvolutionFunction();

    int32_t getStep() const;
    void setStep( int32_t iteration );

    virtual void modifyVertex( 
                             Vector3d< float >& currentVertex,
                             const Vector3d< float >& currentNormal ) const = 0;

  protected:

    VertexEvolutionFunction();

    int32_t _step;

};


}


#endif
