#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-processing-machinelearning/SupportVectorMachine.h>
#include <gkg-processing-machinelearning/MachineLearningProblem.h>
#include <gkg-processing-machinelearning/MachineLearningData_i.h>
#include <gkg-processing-machinelearning/TrainScaler.h>
#include <gkg-processing-machinelearning/PredictScaler.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-core-exception/Exception.h>

#include <vector>


int main( int argc, char* argv[] )
{

  int result = EXIT_SUCCESS;

  try
  {

    std::string inputFileName;
    std::string maskFileName;
    std::string labelFileName;
    std::string testFileName;
    bool verbose = false;
    
    ////////////////////////////////////////////////////////////////////////
    // managing command lines argument(s)
    ////////////////////////////////////////////////////////////////////////
    gkg::Application application( argc,
                                  argv, "SVM training and prediction" );
    application.addSingleOption( "-d",
                                 "Data file name (train)",
                                 inputFileName );
    application.addSingleOption( "-m",
                                 "Mask file name (train)",
                                 maskFileName );
    application.addSingleOption( "-l",
                                 "Label file name (train)",
                                 labelFileName );
    application.addSingleOption( "-t",
                                 "Test file name (predict)",
                                 testFileName );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose,
                                 true );
    application.initialize();


    ////////////////////////////////////////////////////////////////////////////
    // reading input data
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

     std::cout << "reading '" << inputFileName << "' : " << std::flush;

    }
    gkg::Volume< double > inputVolume;
    gkg::Reader::getInstance().read( inputFileName, inputVolume );

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    ////////////////////////////////////////////////////////////////////////////
    // reading mask data
    ////////////////////////////////////////////////////////////////////////////


    if ( verbose )
    {

     std::cout << "reading '" << maskFileName << "' : " << std::flush;

    }
    gkg::Volume< uint8_t > maskVolume;
    gkg::Reader::getInstance().read( maskFileName, maskVolume );

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    ////////////////////////////////////////////////////////////////////////////
    // reading label data
    ////////////////////////////////////////////////////////////////////////////


    if ( verbose )
    {

     std::cout << "reading '" << labelFileName << "' : " << std::flush;

    }
    gkg::Volume< double > labelVolume;
    gkg::Reader::getInstance().read( labelFileName, labelVolume );

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    ////////////////////////////////////////////////////////////////////////////
    // reading test data
    ////////////////////////////////////////////////////////////////////////////


    if ( verbose )
    {

     std::cout << "reading '" << testFileName << "' : " << std::flush;

    }
    gkg::Volume< double > testVolume;
    gkg::Reader::getInstance().read( testFileName, testVolume );

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    ////////////////////////////////////////////////////////////////////////////
    // testing support vector machine
    ////////////////////////////////////////////////////////////////////////////

    std::vector< double > vlabels;
    gkg::Volume< double >::iterator
      l = labelVolume.begin(),
      le = labelVolume.end();

    while ( l != le )
    {

      vlabels.push_back( *l );
      ++l;

    }

    int32_t j, N = int32_t( vlabels.size() );
    gkg::Vector labels( N );

    for ( j = 0; j < N; j++ )
    {

      labels( j ) = vlabels[ j ];

    }

    std::vector< uint8_t > selection( vlabels.size(), 1 );
    gkg::SupportVectorMachine svm;
    gkg::TrainScaler trainScaler;
    gkg::RCPointer< gkg::MachineLearningData > mlData( 
                       new gkg::MachineLearningData( inputVolume, maskVolume,
                                                     selection, trainScaler ) );
    gkg::MachineLearningProblem mlProblem( mlData, labels, selection );

    if ( verbose )
    {

     std::cout << "training ... " << std::endl;

    }

    svm.train( mlProblem );

    gkg::Vector predictedLabels;
    gkg::Matrix predictionProba;
    gkg::PredictScaler predictScaler;
    gkg::MachineLearningData testData( testVolume, maskVolume,
                                       selection, predictScaler );

    if ( verbose )
    {

     std::cout << "predicting ... " << std::endl;

    }

    svm.predict( testData, predictedLabels, &predictionProba );

    N = predictedLabels.getSize();
    int32_t i, M = predictionProba.getSize2();

    for ( j = 0; j < N; j++ )
    {

      std::cout << "Label: " << predictedLabels( j ) 
                << " - Proba: " << std::flush;

      for ( i = 0; i < M; i++ )
      {

        std::cout << predictionProba( j, i ) << " ";

      }

      std::cout << std::endl;

    }

  }  
  GKG_CATCH_COMMAND( result );

  return result;

}
