#ifndef _gkg_processing_plugin_functors_DwiQBallEquator2Curve_DwiQBallEquator2CurveCommand_h_
#define _gkg_processing_plugin_functors_DwiQBallEquator2Curve_DwiQBallEquator2CurveCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>
#include <vector>


namespace gkg
{


class DwiQBallEquator2CurveCommand :
                               public Command,
                               public Creator2Arg< DwiQBallEquator2CurveCommand,
                                         	   Command,
                                         	   int32_t,
                                         	   char** >,
                               public Creator1Arg< DwiQBallEquator2CurveCommand,
                                                   Command,
                                                   const Dictionary& >
{

  public:

    DwiQBallEquator2CurveCommand( int32_t argc,
                  		  char* argv[], 
                  		  bool loadPlugin = false,
                  		  bool removeFirst = true );
    DwiQBallEquator2CurveCommand(
                             const std::string& fileNameIn,
                             const std::vector< double >& normalizedOrientation,
    			     int32_t equatorPointCount,
    			     const std::vector< int32_t >& roiCorners,
    			     const std::string& fileNameOut,
    			     float scaling,
    			     int16_t minimum,
    			     int16_t maximum,
    			     std::vector< double >& positionOffset,
                             int32_t sO3InterpolatorPointCount,
                             float sO3InterpolatorSigma,
    			     bool ascii,
    			     bool verbose );
    DwiQBallEquator2CurveCommand( const Dictionary& parameters );
    virtual ~DwiQBallEquator2CurveCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< DwiQBallEquator2CurveCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< DwiQBallEquator2CurveCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameIn,
                  const std::vector< double >& normalizedOrientation,
    		  int32_t equatorPointCount,
    		  const std::vector< int32_t >& roiCorners,
    		  const std::string& fileNameOut,
    		  float scaling,
    		  int16_t minimum,
    		  int16_t maximum,
    		  std::vector< double >& positionOffset,
                  int32_t sO3InterpolatorPointCount,
                  float sO3InterpolatorSigma,
    		  bool ascii,
    		  bool verbose );

};


}


#endif
