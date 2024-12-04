#ifndef _gkg_graphics_QtGL_plugin_functors_Voi_LabelInfoW_h_
#define _gkg_graphics_QtGL_plugin_functors_Voi_LabelInfoW_h_


#include <gkg-graphics-QtGL-plugin-functors/Voi/Data.h>

#include <string>

#include <QTableWidget>
#include <QWidget>
#include <QTableWidget>

#include <set>


namespace gkg
{


class VoiLabelInfoW : public QWidget
{

  Q_OBJECT

  public:

    VoiLabelInfoW( VoiObservable *, QWidget *p = NULL );
    virtual ~VoiLabelInfoW();

  private slots:

    void selectionLabel( const QModelIndex& );
    void labelChanged( int row, int column );
    void chooseColor();
    void setSelection( int );
    void showSelection();
    void hideSelection();
    void resetList();
    void addLabel();
    void removeLabel();
    void loadLabel();
    void saveLabel();
    void buttonClicked( int );
    void checkChanged( int, int );
    void headerChecked( bool );

  private:

    void applyLabel( const std::string&, int32_t, int32_t, int32_t );
    void applySelection( bool state );
    int32_t addRow( int32_t index, int32_t label, LabelInformation& lInfo );
    void updateRows();

    VoiData* data;
    QTableWidget* lview;
    int currentItem;
    std::set< int32_t > selectedROI;

};


}


#endif
