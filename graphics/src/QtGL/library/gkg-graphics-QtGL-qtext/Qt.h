#ifndef _gkg_graphics_QtGL_qtext_Qt_h_
#define _gkg_graphics_QtGL_qtext_Qt_h_


#ifndef QT_CLEAN_NAMESPACE
#define QT_CLEAN_NAMESPACE 1
#endif


#include <qglobal.h>


#if QT_VERSION >= 0x040000


#include <QApplication>
#include <QBitmap>
#include <QPixmap>
#include <QColorDialog>
#include <QComboBox>
#include <QDesktopWidget>
#include <QEvent>
#include <QFileDialog>
#include <QtOpenGL>
#include <QImage>
#include <QLabel>
#include <QLCDNumber>
#include <QLineEdit>
#include <QMenuBar>
#include <QMessageBox>
#include <QPushButton>
#include <QRadioButton>
#include <QSlider>
#include <QSpinBox>
#include <QStatusBar>
#include <QTableWidget>
#include <QToolButton>

#include <QButtonGroup>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QTreeWidgetItem>
#include <QListView>
#include <QScrollBar>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QTransform>

#if QT_VERSION >= 0x050000
#include <QStyleFactory>
#else
#include <QWindowsStyle>
#endif

#else


#include <qaccel.h>
#include <qapplication.h>
#include <qbitmap.h>
#include <qbrush.h>
#include <qbuttongroup.h>
#include <qcolordialog.h>
#include <qcombobox.h>
#include <qcursor.h>
#include <qevent.h>
#include <qfile.h>
#include <qfiledialog.h>
#include <qframe.h>
#include <qgl.h>
#include <qgrid.h>
#include <qgroupbox.h>
#include <qhbox.h>
#include <qhbuttongroup.h>
#include <qhgroupbox.h>
#include <qiconset.h>
#include <qimage.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qlcdnumber.h>
#include <qlineedit.h>
#include <qlistview.h>
#include <qmainwindow.h>
#include <qmenubar.h>
#include <qmessagebox.h>
#include <qobject.h>
#include <qpainter.h>
#include <qpixmap.h>
#include <qpoint.h>
#include <qpopupmenu.h>
#include <qpushbutton.h>
#include <qradiobutton.h>
#include <qslider.h>
#include <qspinbox.h>
#include <qstatusbar.h>
#include <qstring.h>
#include <qtabbar.h>
#include <qtablewidget.h>
#include <qtabwidget.h>
#include <qtoolbar.h>
#include <qtoolbutton.h>
#include <qtreewidget.h>
#include <qvalidator.h>
#include <qvbox.h>
#include <qvbuttongroup.h>
#include <qvgroupbox.h>
#include <qwidget.h>
#include <qwindowsstyle.h>

typedef QListViewItem Q3ListViewItem;


#endif


#endif
