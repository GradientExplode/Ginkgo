#ifndef _gkg_graphics_QtGL_plugin_functors_Voi_LoadWindow_h_
#define _gkg_graphics_QtGL_plugin_functors_Voi_LoadWindow_h_


#include <gkg-graphics-QtGL-plugin-functors/Voi/Data.h>

#include <QDialog>
#include <QLineEdit>

 #include <string>


namespace gkg
{


class VoiLoadWindow : public QDialog
{

  Q_OBJECT

  public:

    VoiLoadWindow( VoiData& data, QWidget* parent = 0 );
    virtual ~VoiLoadWindow();
 
  private slots:

    void loadVolT1();
    void loadVolB();
    void loadTransfo();
    void loadLabel();
    void accept();
    void clear();

  protected:

    QLineEdit* pathT1;
    QLineEdit* pathVolB;
    QLineEdit* pathTransfo;
    QLineEdit* pathLabel;

    VoiData& _data;

};


} 


#endif
