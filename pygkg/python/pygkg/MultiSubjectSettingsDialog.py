import re
import os
from PyQt5 import QtCore, QtWidgets, uic
from ResourceManager import *

import ConfigParser


class MultiSubjectSettingsDialog( QtWidgets.QDialog ):

  def __init__( self, applicationName, parent = None ):

    QtWidgets.QDialog.__init__( self, parent )

    self._root = uic.loadUi( ResourceManager().getUIFileName(
                                             'core',
                                             'MultiSubjectSettingsDialog.ui' ) )
    self._findChild = lambda x, y: QtCore.QObject.findChild( x, QtCore.QObject,
                                                             y )
    self._layout = QtWidgets.QVBoxLayout( self )
    self._layout.addWidget( self._root )

    settings = ResourceManager().readSettings( applicationName )
    
    self._distributionStrategy = settings[ 'distribution_strategy' ]
    self._maximumThreadCount = 1
    
    if ( self._distributionStrategy == 'multiple-threading' ):

      if ( settings.has_key( 'thread_count' ) ):

        self._maximumThreadCount = settings[ 'thread_count' ]

    else:

      self._distributionStrategy = 'single-threading'

    self.setWindowTitle( 'Multi-subject distribution settings' )
    self.setWindowModality( QtCore.Qt.WindowModal )

    # ok and cancel buttons
    self._buttonBoxOkCancel = self._findChild( self._root,
                                               'buttonBox_OkCancel' )
    self._buttonBoxOkCancel.button( QtWidgets.QDialogButtonBox.Ok
                                   ).clicked.connect( self.accept )
    self._buttonBoxOkCancel.button( QtWidgets.QDialogButtonBox.Cancel
                                   ).clicked.connect( self.reject )

    # distribution strategy buttons
    self._radioButtonSingleThreading = self._findChild( self._root,
                                                 'radioButton_SingleThreading' )
    self._radioButtonSingleThreading.toggled.connect(
                        self.setDistributionStrategyAsSingleThreading )

    self._radioButtonMultipleThreading = self._findChild( self._root,
                                               'radioButton_MultipleThreading' )
    self._radioButtonMultipleThreading.toggled.connect(
                        self.setDistributionStrategyAsMultipleThreading )

    # multiple threading buttons
    self._spinBoxMaximumThreadCount = self._findChild( self._root,
                                                  'spinBox_MaximumThreadCount' )
    self._spinBoxMaximumThreadCount.valueChanged.connect(
                                                    self.setMaximumThreadCount )

    # getting pointer to distribution strategy stacked widget
    self._stackedWidgetDistributionStrategy = self._findChild( self._root,
                                          'stackedWidget_DistributionStrategy' )

    # setting default values
    self.setDefaultValues()

    self._labelCPUCount = self._findChild( self._root, 'label_CPUCount' )
    self._labelCPUCount.setText( str( os.sysconf( 'SC_NPROCESSORS_CONF' ) ) )



  def setDefaultValues( self ):

    if ( self._distributionStrategy == 'single-threading' ):

      self._radioButtonSingleThreading.setChecked( True )

    elif ( self._distributionStrategy == 'multiple-threading' ):

      self._radioButtonMultipleThreading.setChecked( True )
      self._spinBoxMaximumThreadCount.setValue( self._maximumThreadCount )


  def setDistributionStrategyAsSingleThreading( self, value ):

    if value:

      self._distributionStrategy = 'single-threading'
      self._stackedWidgetDistributionStrategy.setCurrentIndex( 0 )


  def setDistributionStrategyAsMultipleThreading( self, value ):

    if value:

      self._distributionStrategy = 'multiple-threading'
      self._stackedWidgetDistributionStrategy.setCurrentIndex( 1 )


  def setMaximumThreadCount( self, value ):

    self._maximumThreadCount = value


  def getSettings( self ):

    settings = {}

    settings[ 'distribution_strategy' ] = self._distributionStrategy
    if ( self._distributionStrategy == 'multiple-threading' ):

      settings[ 'thread_count' ] = self._maximumThreadCount

    return settings


  def exec_( self ):

    self.show()
    self.raise_()
    return QtWidgets.QDialog.exec_( self )
