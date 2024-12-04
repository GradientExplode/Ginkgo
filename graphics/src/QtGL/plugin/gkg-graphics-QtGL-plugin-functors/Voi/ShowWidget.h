#ifndef _gkg_graphics_QtGL_plugin_functors_Voi_ShowWidget_h_
#define _gkg_graphics_QtGL_plugin_functors_Voi_ShowWidget_h_


#include <gkg-processing-colorimetry/RGBComponent.h>
#include <gkg-core-cppext/StdInt.h>

#include <QWidget>
#include <QPushButton>
#include <QCheckBox>
#include <QLabel>


namespace gkg
{


class ShowWidget : public QWidget
{

  Q_OBJECT

  public:

    ShowWidget( QWidget* parent = 0 );

    void setRow( int32_t row );
    void setColor( RGBComponent rgb );
    void setShow( bool state );
    void setCheck( bool state );
    bool isChecked();

  private slots:

    void buttonPressed();
    void checkStateChanged( int );

  signals:

    void buttonChange( int row );
    void checkChange( int row, int state );

  private:

    QPushButton* _btn;
    QCheckBox* _chkBox;
    QLabel* _color;
    bool _show;
    int32_t _row;

};


}


#endif
