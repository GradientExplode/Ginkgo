#ifndef _gkg_anatomist_plugin_GkgDwiGlobalTractographyViewer_GlobalTractographyViewer_h_
#define _gkg_anatomist_plugin_GkgDwiGlobalTractographyViewer_GlobalTractographyViewer_h_


#include <gkg-core-pattern/RCPointer.h>

#include <QWidget>


class QDoubleSpinBox;
class QComboBox;


namespace gkg
{


class AnatomistViewer;
class GlobalTractographyService;
class AGlobalTractographyObject;


class GlobalTractographyViewer : public QWidget
{

  Q_OBJECT
  
  public:

    GlobalTractographyViewer( QWidget* parent = 0,
                              int32_t openGLRefreshPeriod = 1000,
                              bool verbose = false );
    virtual ~GlobalTractographyViewer();

    int32_t getOpenGLRefreshPeriod() const;
    bool isVerbose() const;

    void customEvent( QEvent* event );

  public slots:

    void setClippingDistance( double value );
    void setViewType( int viewType );

  protected:

    int32_t _openGLRefreshPeriod;
    bool _verbose;


    QDoubleSpinBox* _doubleSpinBoxClippingDistance;
    QComboBox* _comboBoxViewType;
    QWidget* _viewer;
    AnatomistViewer* _anatomistViewer;


    RCPointer< GlobalTractographyService > _globalTractographyService;

    // this object cannot be encapsulated within a RCPointer because it
    // inheritates an anatomist::SliceableObject that is already inheriting from
    // a soma::rc_pointer causing an ambiguity
    AGlobalTractographyObject* _aGlobalTractographyObject;

};



}


#endif
