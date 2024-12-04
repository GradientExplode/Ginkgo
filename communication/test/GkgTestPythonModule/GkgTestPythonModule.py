import gkg


gkg.executeCommand( { 'algorithm' : 'Thresholder',
                      'parameters' : { 'fileNameIn' : '001.ima',
                                       'fileNameOut' : 'output1.ima',
                                       'mode' : 'be',
                                       'threshold1' : 0.0,
                                       'threshold2' : 10.0,
                                       'background' : 0,
                                       'ascii' : False,
                                       'format' : 'gis',
                                       'verbose' : True
                                     },
                       'verbose' : True
                    } )

gkg.executeCommand( { 'algorithm' : 'SubVolume',
                      'parameters' : { 'fileNameIn' : 'output1.ima',
                                       'fileNameOut' : 'output2.ima',
                                       'sx' : 0,
                                       'ex' : 159,
                                       'sy' : 0,
                                       'ey' : 239,
                                       'sz' : 100,
                                       'ez' : 156,
                                       'st' : 0,
                                       'et' : 0,
                                       'ascii' : False,
                                       'format' : 'gis',
                                       'verbose' : True
                                     },
                       'verbose' : False
                    } )
