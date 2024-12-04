#ifndef _gkg_graphics_QtGL_plugin_functors_DicomDump_DicomDumpMainWindow_h_
#define _gkg_graphics_QtGL_plugin_functors_DicomDump_DicomDumpMainWindow_h_


#include <gkg-graphics-QtGL-qtext/Qt.h>
#include <gkg-core-io/DicomTagInfo.h>

#include <QMainWindow>
#include <QMenu>
#include <QTreeWidget>

#include <string>


namespace gkg
{


class DicomDumpMainWindow : public QMainWindow
{

  Q_OBJECT

  public:

    DicomDumpMainWindow();

    void loadFile( const std::string& );

  private slots:

    void open();

  private:

    void createMenu();
    void createTree( DicomTagInfo& );
    void addItem( QList< QTreeWidgetItem* >&, QTreeWidgetItem*, DicomTagInfo& );

    QMenu* _fileMenu;
    QTreeWidget* _treeWidget;

};


}


#endif
