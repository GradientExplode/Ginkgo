#ifndef _gkg_graphics_QtGL_plugin_functors_Voi_LabelInfo_h_
#define _gkg_graphics_QtGL_plugin_functors_Voi_LabelInfo_h_


#include <gkg-processing-colorimetry/RGBComponent.h>
#include <fstream>
#include <string>
#include <map>
#include <utility>

#include <QObject>


namespace gkg
{


class HeaderedObject;


struct LabelInformation
{

  LabelInformation( const std::string& theName,
                    RGBComponent theRgb = RGBComponent(),
                    bool theShow = true );
  LabelInformation( const LabelInformation& other );

  std::string name;
  RGBComponent rgb;
  bool show;

};


class VoiLabelInfo : public QObject
{

  Q_OBJECT

  public:

    enum LabelNumber
    {

      labelMax = 16384

    };

    VoiLabelInfo();
    virtual ~VoiLabelInfo();

    std::map< int32_t, LabelInformation >& info();
    const std::map< int32_t, LabelInformation >& info() const;
    uint8_t* byteOrder();
    int32_t currentLabel();
    void setCurrent( int );
    int32_t count();
    int32_t getMax();
    int32_t getFirstAvailable();
    void removeLabel( int32_t index );
    std::string getName( int32_t index );
    void toggleShow( int32_t index );
    void setShow( int32_t index, bool state );
    bool getShow( int32_t index );
    bool getColor( int32_t index, RGBComponent& color );
    void update( int32_t index, const std::string& name,
                 int32_t r, int32_t g, int32_t b );
    bool read( const std::string& );
    void write( const std::string& );
    void readHeader( HeaderedObject& header );
    void writeHeader( HeaderedObject& header );

  public slots:

    void clear();

  signals:

    void currentChanged( int );
    void listCleared();

  private:

    void init();
    
    int32_t current;
    std::map< int32_t, LabelInformation > infos;
    uint32_t bOrder;

};


}


#endif
