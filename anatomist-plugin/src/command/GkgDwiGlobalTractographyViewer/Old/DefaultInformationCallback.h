#ifndef _gkg_dmri_GkgDwiInformationViewer_DefaultInformationCallback_h_
#define _gkg_dmri_GkgDwiInformationViewer_DefaultInformationCallback_h_


#include "GlobalTractographyService.h"


namespace gkg
{


class DefaultInformationCallback
{

  public:

    DefaultInformationCallback( bool verbose );
    virtual ~DefaultInformationCallback();

    const RCPointer< GlobalTractographyOptimizationInformation >&
      duplicateLastInformation(
             const GlobalTractographyService& globalTractographyService ) const;

  private:

    bool _verbose;

};


}


#endif

