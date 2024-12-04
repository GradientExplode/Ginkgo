#include <gkg-graphics-QtGL-widget/NetworkSetting.h>
#include <gkg-core-exception/Exception.h>

#include <QGridLayout>


gkg::NetworkSetting::NetworkSetting( QWidget* parent,
                                     const std::string& ipAddress,
                                     int32_t port )
                   : QWidget( parent ),
                     _ipAddress( 0 ),
                     _port( 0 )
{

  try
  {

    QGridLayout* grid = new QGridLayout();
    int32_t row = 0;

    if ( ipAddress.length() )
    {

      QLabel* labelIP = new QLabel( "IP Address:" );
      _ipAddress = new QLineEdit( ipAddress.c_str() );    
      _ipAddress->setInputMask( "000.000.000.000" );
      grid->addWidget( labelIP, row, 0 );
      grid->addWidget( _ipAddress, row++, 1 );
      connect( _ipAddress, SIGNAL( textEdited( const QString& ) ),
               this, SLOT( setIpAddress( const QString& ) ) );

    }

    if ( port > 0 )
    {

      QString portTxt;
      QLabel* labelPort = new QLabel( "Port:" );
      portTxt.setNum( port );
      _port = new QLineEdit( portTxt );
      _port->setInputMask( "00000" );
      grid->addWidget( labelPort, row, 0 );
      grid->addWidget( _port, row, 1 );
      connect( _port, SIGNAL( textEdited( const QString& ) ),
               this, SLOT( setPort( const QString& ) ) );

    }

    setLayout( grid );

  }
  GKG_CATCH( "gkg::NetworkSetting::NetworkSetting( QWidget* parent, "
             "const std::string& ipAddress, int32_t port )" );

}


void gkg::NetworkSetting::setNetworkInformation( const std::string& ipAddress,
                                                 int32_t port )
{

  try
  {

    if ( _ipAddress && ipAddress.length() )
    {

      _ipAddress->setText( ipAddress.c_str() );

    }

    if ( _port && ( port > 0 ) )
    {

      QString txt;

      txt.setNum( port );
      _port->setText( txt );

    }

  }
  GKG_CATCH( "void gkg::NetworkSetting::setNetworkInformation( "
             "const std::string& ipAdress, int32_t port )" );

}


void gkg::NetworkSetting::setIpAddress( const QString& ip )
{

  emit ipChanged( ip );

}


void gkg::NetworkSetting::setPort( const QString& port )
{

  emit portChanged( port.toInt() );

}

