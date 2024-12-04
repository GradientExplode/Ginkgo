#ifndef _gkg_graphics_QtGL_plugin_functors_Voi_MainWindow_h_
#define _gkg_graphics_QtGL_plugin_functors_Voi_MainWindow_h_


#include <gkg-graphics-QtGL-plugin-functors/Voi/Data.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/ModeStrategy.h>

#include <QWidget>

#include <string>


class QPushButton;


namespace gkg
{


class VoiMainWindow : public QWidget
{

  Q_OBJECT

  public:

    VoiMainWindow();
    virtual ~VoiMainWindow();

   void initializeArg( const std::string& anatName,
                       const std::string& fusionName,
                       const std::string& labelName,
                       const std::string& transfoName );

  private slots:

    void loadVolumes();
    void saveLabel();
    void saveStatistics();
    void toggleContour();

    void clearSelection();
    void clearAll();

    void setModeStrategy( int );

  private:

    VoiData data;
    QPushButton* b22;

    void createWorkArea();
  
};


}


#endif
