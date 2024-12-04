#ifndef _gkg_graphics_QtGL_plugin_functors_Voi_ComboBox_h_
#define _gkg_graphics_QtGL_plugin_functors_Voi_ComboBox_h_


#include <QWidget>
#include <QComboBox>

#include <string>


namespace gkg
{


class VoiComboBox : public QWidget
{

  Q_OBJECT

  public:

    VoiComboBox( bool, QWidget *, const std::string& );

    void insertStrList( const char **, int32_t index=-1 );

  public slots:

    void activate( int );
    void activate( const QString& );

  signals:

    void activated( int );
    void activated( const QString& );

  private:

    QComboBox *cbox;

};


}


#endif
