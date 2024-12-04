#include <gkg-processing-io/LabelledRoiDictionaryAnalyzer.h>
#include <gkg-processing-io/LabelledRoiDictionaryDiskFormat.h>
#include <gkg-core-exception/Exception.h>




gkg::LabelledRoiDictionaryAnalyzer::LabelledRoiDictionaryAnalyzer()
                                   : gkg::BaseDiskFormatAnalyzer()
{
}



gkg::LabelledRoiDictionaryAnalyzer::~LabelledRoiDictionaryAnalyzer()
{
}



gkg::DiskFormat& gkg::LabelledRoiDictionaryAnalyzer::getDiskFormat() const
{

  return gkg::LabelledRoiDictionaryDiskFormat::getInstance();

}



void gkg::LabelledRoiDictionaryAnalyzer::analyze(
                                     const std::string& name,
                                     std::string& format,
                                     gkg::AnalyzedObject& analyzedObject ) const
{

  try
  {

    getDiskFormat().readHeader( name, analyzedObject );
    format = getDiskFormat().getName();

  }
  GKG_CATCH( "void gkg::LabelledRoiDictionaryAnalyzer::analyze( "
             "const std::string& name, "
             "std::string& format, "
             "gkg::AnalyzedObject& analyzedObject ) const" );

}
