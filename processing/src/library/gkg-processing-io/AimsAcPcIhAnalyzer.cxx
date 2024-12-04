#include <gkg-processing-io/AimsAcPcIhAnalyzer.h>
#include <gkg-processing-io/AimsAcPcIhDiskFormat.h>
#include <gkg-core-exception/Exception.h>




gkg::AimsAcPcIhAnalyzer::AimsAcPcIhAnalyzer()
                        : gkg::BaseDiskFormatAnalyzer()
{
}



gkg::AimsAcPcIhAnalyzer::~AimsAcPcIhAnalyzer()
{
}



gkg::DiskFormat& gkg::AimsAcPcIhAnalyzer::getDiskFormat() const
{

  return gkg::AimsAcPcIhDiskFormat::getInstance();

}



void gkg::AimsAcPcIhAnalyzer::analyze(
                                     const std::string& name,
                                     std::string& format,
                                     gkg::AnalyzedObject& analyzedObject ) const
{

  try
  {

    getDiskFormat().readHeader( name, analyzedObject );
    format = getDiskFormat().getName();

  }
  GKG_CATCH( "void gkg::AimsAcPcIhAnalyzer::analyze( "
                                "const std::string& name, "
                                "std::string& format, "
                                "gkg::AnalyzedObject& analyzedObject ) const" );

}
