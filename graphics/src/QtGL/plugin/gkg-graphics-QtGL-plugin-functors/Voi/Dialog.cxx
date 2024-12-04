#include <gkg-graphics-QtGL-plugin-functors/Voi/Dialog.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/LabelInfoW.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/FusionSliders.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/CoordinateSpinBoxes.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/ModeStrategy.h>

#include <QVBoxLayout>


gkg::VoiDialog::VoiDialog( gkg::VoiObservable* d, QWidget* parent )
               : QWidget( parent )
{

  gkg::VoiLabelInfoW* linfo = new gkg::VoiLabelInfoW( d, this );
  gkg::FusionSliders* finfo = new gkg::FusionSliders( d, this );
  gkg::CoordinateSpinBoxes* pinfo = new gkg::CoordinateSpinBoxes( d, this );
  binfo = new gkg::VoiBrushSizeW( d, this );
  einfo = new gkg::VoiEvolutionInfoW( d, this );
  minfo = new gkg::MorphologyW( d, this );
  cinfo = new gkg::CylinderW( d, this );

  QVBoxLayout *layout = new QVBoxLayout();
  layout->setContentsMargins( 0, 0, 0, 0 );
  layout->setSpacing( 0 );

  layout->addWidget( linfo );
  layout->addWidget( finfo );
  layout->addWidget( pinfo );
  layout->addWidget( binfo );
  layout->addWidget( einfo );
  layout->addWidget( minfo );
  layout->addWidget( cinfo );

  binfo->hide();
  einfo->hide();
  minfo->hide();
  cinfo->hide();

  setLayout( layout );

}


void gkg::VoiDialog::show( int button )
{

  if ( button == int( gkg::VoiModeStrategy::PaintMode ) )
  {

    binfo->show();

  }
  else
  {

    binfo->hide();

  }
  if ( button == int( gkg::VoiModeStrategy::EvolutionMode ) )
  {

    einfo->show();

  }
  else
  {

    einfo->hide();

  }
  if ( button == int( gkg::VoiModeStrategy::MorphologyMode ) )
  {

    minfo->show();

  }
  else
  {

    minfo->hide();

  }
  if ( button == int( gkg::VoiModeStrategy::CylinderMode ) )
  {

    cinfo->show();

  }
  else
  {

    cinfo->hide();

  }

}
