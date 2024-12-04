#include <gkg-processing-io/HistogramAnalysisAnalyzer.h>
#include <gkg-processing-io/HistogramAnalysisDiskFormat.h>
#include <gkg-core-exception/Exception.h>




gkg::HistogramAnalysisAnalyzer::HistogramAnalysisAnalyzer()
                               : gkg::BaseDiskFormatAnalyzer()
{
}



gkg::HistogramAnalysisAnalyzer::~HistogramAnalysisAnalyzer()
{
}



gkg::DiskFormat& gkg::HistogramAnalysisAnalyzer::getDiskFormat() const
{

  return gkg::HistogramAnalysisDiskFormat::getInstance();

}



void gkg::HistogramAnalysisAnalyzer::analyze(
                                     const std::string& name,
                                     std::string& format,
                                     gkg::AnalyzedObject& analyzedObject ) const
{

  try
  {

    getDiskFormat().readHeader( name, analyzedObject );
    format = getDiskFormat().getName();

  }
  GKG_CATCH( "void gkg::HistogramAnalysisAnalyzer::analyze("
                                "const std::string& name,"
                                "std::string& format,"
                                "gkg::AnalyzedObject& analyzedObject ) const" );

}
