#ifndef _gkg_dmri_container_DiffusionCartesianFieldFunctor_h_
#define _gkg_dmri_container_DiffusionCartesianFieldFunctor_h_


#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-communication-thread/Mutex.h>


namespace gkg
{


template < class T >
class DiffusionCartesianFieldFunctor
{

  public:
  
    virtual ~DiffusionCartesianFieldFunctor();
    
    virtual T&
      apply( const Vector3d< int32_t >& site, T& t ) = 0;

  protected:

    DiffusionCartesianFieldFunctor();

    Mutex _mutex;

};



}


#endif
