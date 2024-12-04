from AdvancedParameter import *


class RegistrationParameter( AdvancedParameter ):

  def __init__( self,
                theName,
                setParameters ):

    registerParameter = \
                    { 'similarityMeasureName':
                      { 'widget': 'comboBox_SimilarityMeasureName',
                        'widgetType': 'comboBox',
                        'defaultValue': 1,
                        'choices': [ 'correlation-coefficient',
                                     'mutual-information',
                                     'normalized-mutual-information',
                                     'correlation-ratio' ]
                      },
                        'referenceLowerThreshold':
                      { 'widget': 'doubleSpinBox_ReferenceLowerThreshold',
                        'widgetType': 'spinBox',
                        'defaultValue': 0.0,
                        'minimumValue': 0.0,
                        'maximumValue': 10000,
                        'incrementValue': 0.01
                      },
                        'floatingLowerThreshold':
                      { 'widget': 'doubleSpinBox_FloatingLowerThreshold',
                        'widgetType': 'spinBox',
                        'defaultValue': 0.0,
                        'minimumValue': 0.0,
                        'maximumValue': 10000,
                        'incrementValue': 0.01
                      },
                        'resamplingOrder':
                      { 'widget': 'spinBox_ResamplingOrder',
                        'widgetType': 'spinBox',
                        'defaultValue': 1,
                        'minimumValue': 0,
                        'maximumValue': 7,
                        'incrementValue': 1
                      },
                        'subSamplingMaximumSizes':
                      { 'widget': 'lineEdit_SubSamplingMaximumSizes',
                        'widgetType': 'lineEdit',
                        'defaultValue': '64'
                      },
                        'levelCount':
                      { 'widget': 'spinBox_LevelCount',
                        'widgetType': 'spinBox',
                        'defaultValue': 32,
                        'minimumValue': 0,
                        'maximumValue': 1000,
                        'incrementValue': 1,
                        'dependency': [ 'similarityMeasureName' ],
                        'dependencyValues': [ { 0: False,
                                                1: True,
                                                2: True,
                                                3: True } ]
                      },
                        'applySmoothing':
                      { 'widget': 'checkBox_ApplySmoothing',
                        'widgetType': 'checkBox',
                        'defaultValue': 1,
                        'dependency': [ 'similarityMeasureName' ],
                        'dependencyValues': [ { 0: False,
                                                1: True,
                                                2: True,
                                                3: True  } ]
                      },
                        'maximumIterationCount':
                      { 'widget': 'spinBox_MaximumIterationCount',
                        'widgetType': 'spinBox',
                        'defaultValue': 1000,
                        'minimumValue': 0,
                        'maximumValue': 1000,
                        'incrementValue': 1
                      },
                        'transform3DType':
                      { 'widget': 'comboBox_Transform3DType',
                        'widgetType': 'comboBox',
                        'defaultValue': 0,
                        'choices': [ 'rigid',
                                     'affine_wo_shearing',
                                     'affine' ]
                      },
                        'initialParametersTranslationX':
                      { 'widget': 'doubleSpinBox_InitialParametersTranslationX',
                        'widgetType': 'spinBox',
                        'defaultValue': 0,
                        'minimumValue': -10000,
                        'maximumValue': 10000,
                        'incrementValue': 0.01,
                        'dependency': \
                               [ 'initializeCoefficientsUsingCenterOfGravity' ],
                        'dependencyValues': [ { True: False,
                                                False: True } ]
                      },
                        'initialParametersTranslationY':
                      { 'widget': 'doubleSpinBox_InitialParametersTranslationY',
                        'widgetType': 'spinBox',
                        'defaultValue': 0,
                        'minimumValue': -10000,
                        'maximumValue': 10000,
                        'incrementValue': 0.01,
                        'dependency': \
                               [ 'initializeCoefficientsUsingCenterOfGravity' ],
                        'dependencyValues': [ { True: False,
                                                False: True } ]
                      },
                        'initialParametersTranslationZ':
                      { 'widget': 'doubleSpinBox_InitialParametersTranslationZ',
                        'widgetType': 'spinBox',
                        'defaultValue': 0,
                        'minimumValue': -10000,
                        'maximumValue': 10000,
                        'incrementValue': 0.01,
                        'dependency': \
                               [ 'initializeCoefficientsUsingCenterOfGravity' ],
                        'dependencyValues': [ { True: False,
                                                False: True } ]
                      },
                        'initializeCoefficientsUsingCenterOfGravity':
                      { 'widget': \
                           'checkBox_InitializeCoefficientsUsingCenterOGravity',
                        'widgetType': 'checkBox',
                        'defaultValue': True
                      },
                        'initialParametersRotationX':
                      { 'widget': 'doubleSpinBox_InitialParametersRotationX',
                        'widgetType': 'spinBox',
                        'defaultValue': 0,
                        'minimumValue': 0,
                        'maximumValue': 360,
                        'incrementValue': 0.01,
                        'dependency': \
                               [ 'initializeCoefficientsUsingCenterOfGravity' ],
                        'dependencyValues': [ { True: False,
                                                False: True } ]
                      },
                        'initialParametersRotationY':
                      { 'widget': 'doubleSpinBox_InitialParametersRotationY',
                        'widgetType': 'spinBox',
                        'defaultValue': 0,
                        'minimumValue': 0,
                        'maximumValue': 360,
                        'incrementValue': 0.01,
                        'dependency': \
                               [ 'initializeCoefficientsUsingCenterOfGravity' ],
                        'dependencyValues': [ { True: False,
                                                False: True } ]
                      },
                        'initialParametersRotationZ':
                      { 'widget': 'doubleSpinBox_InitialParametersRotationZ',
                        'widgetType': 'spinBox',
                        'defaultValue': 0,
                        'minimumValue': 0,
                        'maximumValue': 360,
                        'incrementValue': 0.01,
                        'dependency': \
                               [ 'initializeCoefficientsUsingCenterOfGravity' ],
                        'dependencyValues': [ { True: False,
                                                False: True } ]
                      },
                        'initialParametersScalingX':
                      { 'widget': 'doubleSpinBox_InitialParametersScalingX',
                        'widgetType': 'spinBox',
                        'defaultValue': 1.0,
                        'dependency': \
                                 [ 'initializeCoefficientsUsingCenterOfGravity',
                                   'transform3DType' ],
                        'dependencyValues': [ { True: False,
                                                False: True },
                                              { 0: False,
                                                1: True,
                                                2: True } ]
                      },
                        'initialParametersScalingY':
                      { 'widget': 'doubleSpinBox_InitialParametersScalingY',
                        'widgetType': 'spinBox',
                        'defaultValue': 1.0,
                        'minimumValue': 0.0,
                        'maximumValue': 10000,
                        'incrementValue': 0.01,
                        'dependency': \
                                 [ 'initializeCoefficientsUsingCenterOfGravity',
                                   'transform3DType' ],
                        'dependencyValues': [ { True: False,
                                                False: True },
                                              { 0: False,
                                                1: True,
                                                2: True } ]
                      },
                        'initialParametersScalingZ':
                      { 'widget': 'doubleSpinBox_InitialParametersScalingZ',
                        'widgetType': 'spinBox',
                        'defaultValue': 1.0,
                        'minimumValue': 0.0,
                        'maximumValue': 10000,
                        'incrementValue': 0.01,
                        'dependency': \
                                 [ 'initializeCoefficientsUsingCenterOfGravity',
                                   'transform3DType' ],
                        'dependencyValues': [ { True: False,
                                                False: True },
                                              { 0: False,
                                                1: True,
                                                2: True } ]
                      },
                        'initialParametersShearingXY':
                      { 'widget': 'doubleSpinBox_InitialParametersShearingXY',
                        'widgetType': 'spinBox',
                        'defaultValue': 0.0,
                        'minimumValue': 0.0,
                        'maximumValue': 10000,
                        'incrementValue': 0.01,
                        'dependency': \
                                 [ 'initializeCoefficientsUsingCenterOfGravity',
                                   'transform3DType' ],
                        'dependencyValues': [ { True: False,
                                                False: True },
                                              { 0: False,
                                                1: False,
                                                2: True } ]
                      },
                        'initialParametersShearingXZ':
                      { 'widget': 'doubleSpinBox_InitialParametersShearingXZ',
                        'widgetType': 'spinBox',
                        'defaultValue': 0.0,
                        'minimumValue': 0.0,
                        'maximumValue': 10000,
                        'incrementValue': 0.01,
                        'dependency': \
                                 [ 'initializeCoefficientsUsingCenterOfGravity',
                                   'transform3DType' ],
                        'dependencyValues': [ { True: False,
                                                False: True },
                                              { 0: False,
                                                1: False,
                                                2: True } ]
                      },
                        'initialParametersShearingYZ':
                      { 'widget': 'doubleSpinBox_InitialParametersShearingYZ',
                        'widgetType': 'spinBox',
                        'defaultValue': 0.0,
                        'minimumValue': 0.0,
                        'maximumValue': 10000,
                        'incrementValue': 0.01,
                        'dependency': \
                                 [ 'initializeCoefficientsUsingCenterOfGravity',
                                   'transform3DType' ],
                        'dependencyValues': [ { True: False,
                                                False: True },
                                              { 0: False,
                                                1: False,
                                                2: True } ]
                      },
                        'optimizerName':
                      { 'widget': 'comboBox_OptimizerName',
                        'widgetType': 'comboBox',
                        'defaultValue': 0,
                        'choices': [ 'nelder-mead',
                                     'fletcher-reeves' ]
                      },
                        'stoppingCriterionError':
                      { 'widget': 'doubleSpinBox_StoppingCriterionError',
                        'widgetType': 'spinBox',
                        'defaultValue': 0.01,
                        'minimumValue': 0.00001,
                        'maximumValue': 10000,
                        'incrementValue': 0.00001,
                        'dependency': [ 'optimizerName' ],
                        'dependencyValues': [ { 0: True,
                                                1: False } ]
                      },
                        'stepSize':
                      { 'widget': 'doubleSpinBox_StepSize',
                        'widgetType': 'spinBox',
                        'defaultValue': 1e-1,
                        'minimumValue': 1e-10,
                        'maximumValue': 100,
                        'incrementValue': 1e-10,
                        'dependency': [ 'optimizerName' ],
                        'dependencyValues': [ { 0: False,
                                                1: True } ]
                      },
                        'maximumTestGradient':
                      { 'widget': 'doubleSpinBox_MaximumTestGradient',
                        'widgetType': 'spinBox',
                        'defaultValue': 1000.0,
                        'minimumValue': 0.0,
                        'maximumValue': 1000000.0,
                        'incrementValue': 1.0,
                        'dependency': [ 'optimizerName' ],
                        'dependencyValues': [ { 0: False,
                                                1: True } ]
                      },
                        'maximumTolerance':
                      { 'widget': 'doubleSpinBox_MaximumTolerance',
                        'widgetType': 'spinBox',
                        'defaultValue': 1e-2,
                        'minimumValue': 1e-10,
                        'maximumValue': 100,
                        'incrementValue': 1e-10,
                        'dependency': [ 'optimizerName' ],
                        'dependencyValues': [ { 0: False,
                                                1: True } ]
                      },
                        'optimizerParametersTranslationX':
                      { 'widget': 'doubleSpinBox_OptimizerParametersTranslationX',
                        'widgetType': 'spinBox',
                        'defaultValue': 10,
                        'minimumValue': -10000,
                        'maximumValue': 10000,
                        'incrementValue': 0.01,
                        'dependency': [ 'optimizerName' ],
                        'dependencyValues': [ { 0: True,
                                                1: False } ]
                      },
                        'optimizerParametersTranslationY':
                      { 'widget': 'doubleSpinBox_OptimizerParametersTranslationY',
                        'widgetType': 'spinBox',
                        'defaultValue': 10,
                        'minimumValue': -10000,
                        'maximumValue': 10000,
                        'incrementValue': 0.01,
                        'dependency': [ 'optimizerName' ],
                        'dependencyValues': [ { 0: True,
                                                1: False } ]
                      },
                        'optimizerParametersTranslationZ':
                      { 'widget': 'doubleSpinBox_OptimizerParametersTranslationZ',
                        'widgetType': 'spinBox',
                        'defaultValue': 10,
                        'minimumValue': -10000,
                        'maximumValue': 10000,
                        'incrementValue': 0.01,
                        'dependency': [ 'optimizerName' ],
                        'dependencyValues': [ { 0: True,
                                                1: False } ]
                      },
                        'optimizerParametersRotationX':
                      { 'widget': 'doubleSpinBox_OptimizerParametersRotationX',
                        'widgetType': 'spinBox',
                        'defaultValue': 10,
                        'minimumValue': 0,
                        'maximumValue': 360,
                        'incrementValue': 0.01,
                        'dependency': [ 'optimizerName' ],
                        'dependencyValues': [ { 0: True,
                                                1: False } ]
                      },
                        'optimizerParametersRotationY':
                      { 'widget': 'doubleSpinBox_OptimizerParametersRotationY',
                        'widgetType': 'spinBox',
                        'defaultValue': 10,
                        'minimumValue': 0,
                        'maximumValue': 360,
                        'incrementValue': 0.01,
                        'dependency': [ 'optimizerName' ],
                        'dependencyValues': [ { 0: True,
                                                1: False } ]
                      },
                        'optimizerParametersRotationZ':
                      { 'widget': 'doubleSpinBox_OptimizerParametersRotationZ',
                        'widgetType': 'spinBox',
                        'defaultValue': 10,
                        'minimumValue': 0,
                        'maximumValue': 360,
                        'incrementValue': 0.01,
                        'dependency': [ 'optimizerName' ],
                        'dependencyValues': [ { 0: True,
                                                1: False } ]
                      },
                        'optimizerParametersScalingX':
                      { 'widget': 'doubleSpinBox_OptimizerParametersScalingX',
                        'widgetType': 'spinBox',
                        'defaultValue': 0.05,
                        'minimumValue': 0.0,
                        'maximumValue': 10000,
                        'incrementValue': 0.01,
                        'dependency': [ 'optimizerName', 'transform3DType' ],
                        'dependencyValues': [ { 0: True,
                                                1: False },
                                              { 0: False,
                                                1: True,
                                                2: True } ]
                      },
                        'optimizerParametersScalingY':
                      { 'widget': 'doubleSpinBox_OptimizerParametersScalingY',
                        'widgetType': 'spinBox',
                        'defaultValue': 0.05,
                        'minimumValue': 0.0,
                        'maximumValue': 10000,
                        'incrementValue': 0.01,
                        'dependency': [ 'optimizerName', 'transform3DType' ],
                        'dependencyValues': [ { 0: True,
                                                1: False },
                                              { 0: False,
                                                1: True,
                                                2: True } ]
                      },
                        'optimizerParametersScalingZ':
                      { 'widget': 'doubleSpinBox_OptimizerParametersScalingZ',
                        'widgetType': 'spinBox',
                        'defaultValue': 0.05,
                        'minimumValue': 0.0,
                        'maximumValue': 10000,
                        'incrementValue': 0.01,
                        'dependency': [ 'optimizerName', 'transform3DType' ],
                        'dependencyValues': [ { 0: True,
                                                1: False },
                                              { 0: False,
                                                1: True,
                                                2: True } ]
                      },
                        'optimizerParametersShearingXY':
                      { 'widget': 'doubleSpinBox_OptimizerParametersShearingXY',
                        'widgetType': 'spinBox',
                        'defaultValue': 0.05,
                        'minimumValue': 0.0,
                        'maximumValue': 10000,
                        'incrementValue': 0.01,
                        'dependency': [ 'optimizerName', 'transform3DType' ],
                        'dependencyValues': [ { 0: True,
                                                1: False },
                                              { 0: False,
                                                1: False,
                                                2: True } ]
                      },
                        'optimizerParametersShearingXZ':
                      { 'widget': 'doubleSpinBox_OptimizerParametersShearingXZ',
                        'widgetType': 'spinBox',
                        'defaultValue': 0.05,
                        'minimumValue': 0.0,
                        'maximumValue': 10000,
                        'incrementValue': 0.01,
                        'dependency': [ 'optimizerName', 'transform3DType' ],
                        'dependencyValues': [ { 0: True,
                                                1: False },
                                              { 0: False,
                                                1: False,
                                                2: True } ]
                      },
                        'optimizerParametersShearingYZ':
                      { 'widget': 'doubleSpinBox_OptimizerParametersShearingYZ',
                        'widgetType': 'spinBox',
                        'defaultValue': 0.05,
                        'minimumValue': 0.0,
                        'maximumValue': 10000,
                        'incrementValue': 0.01,
                        'dependency': [ 'optimizerName', 'transform3DType' ],
                        'dependencyValues': [ { 0: True,
                                                1: False },
                                              { 0: False,
                                                1: False,
                                                2: True } ]
                      }

                    }

    for parameterKey in setParameters.keys():

      for key in setParameters[ parameterKey ].keys():

        registerParameter[ parameterKey ][ key ] = \
                                            setParameters[ parameterKey ][ key ]
    AdvancedParameter.__init__( \
                    self,
                    theName,
                    registerParameter )



