#ifndef _gkg_graphics_QtGL_widget_LabelledComboBox_h_
#define _gkg_graphics_QtGL_widget_LabelledComboBox_h_


#include <gkg-graphics-QtGL-qtext/Qt.h>
#include <gkg-core-cppext/StdInt.h>
#include <string>


namespace gkg
{


class LabelledComboBox : public QWidget
{

  Q_OBJECT

  public:

    LabelledComboBox( QWidget* parent = 0, const std::string& name = "" );

    void insertItem( const std::string& text );

    void clear();

    int32_t count();

    std::string currentText() const;

    int32_t currentIndex() const;
    void setCurrentIndex( uint32_t index );
    void setCurrentText( const QString& text );

    QComboBox* getComboBox() const;

  public slots:

    void activate( const QString& text );

  signals:

    void activated( const QString& text );

  private:

    QComboBox* _combobox;

};


}


#endif
