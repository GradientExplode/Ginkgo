#ifndef _gkg_anatomist_plugin_GkgDwiGlobalTractographyViewer_AnatomistViewer_h_
#define _gkg_anatomist_plugin_GkgDwiGlobalTractographyViewer_AnatomistViewer_h_


#include "AGlobalTractographyObject.h"

#include <gkg-core-cppext/StdInt.h>

#include <QWidget>
#include <QLayout>
#include <QEvent>

#include <string>


namespace gkg
{


class AnatomistViewer : public QWidget
{


  public:

    enum Type
    {

      Axial,
      Sagittal,
      Coronal,
      ThreeD

    };

    AnatomistViewer( int32_t windowWidth, 
                     Type type,
                     const std::string& name,
                     QWidget* parent = 0 );
    virtual ~AnatomistViewer();

    void reset();
  
    void addGlobalTractographyObject(
                         AGlobalTractographyObject* aGlobalTractographyObject );

  private:

    int32_t _windowWidth;
    QGridLayout* _gridLayout;

};


}


#endif

