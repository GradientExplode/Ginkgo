#ifndef _gkg_graphics_QtGL_widget_NetworkSetting_h_
#define _gkg_graphics_QtGL_widget_NetworkSetting_h_


#include <gkg-graphics-QtGL-qtext/Qt.h>
#include <gkg-core-cppext/StdInt.h>

#include <string>



namespace gkg
{


class NetworkSetting : public QWidget
{

  Q_OBJECT

  public:

    NetworkSetting( QWidget* parent = 0, 
                    const std::string& ipAddress = "",
                    int32_t port = 0 );

    void setNetworkInformation( const std::string& ipAdress = "",
                                int32_t port = 0 );

  private slots:

    void setIpAddress( const QString& ip );
    void setPort( const QString& ip );

  signals:

    void ipChanged( const QString& );
    void portChanged( int port );

  private:

    QLineEdit* _ipAddress;
    QLineEdit* _port;

};


}


#endif

