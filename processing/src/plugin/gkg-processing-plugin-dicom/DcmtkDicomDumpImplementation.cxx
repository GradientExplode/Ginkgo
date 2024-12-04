#include <gkg-processing-plugin-dicom/DcmtkDicomDumpImplementation.h>
#include <gkg-core-io/DicomDump.h>
#include <gkg-core-exception/Exception.h>
#include <sstream>


gkg::DcmtkDicomDumpImplementation::DcmtkDicomDumpImplementation(
                                                   gkg::DicomDump* dicomDump,
                                                   const std::string& fileName )
                                 : gkg::DicomDumpImplementation( dicomDump,
                                                                 fileName )
{

  try
  {

    ::DcmFileFormat dicomFile;

    dcmAcceptUnexpectedImplicitEncoding.set(OFFalse);

    ::OFCondition cond = dicomFile.loadFile( fileName.c_str() );

    if ( !cond.good() )
    {

      throw std::runtime_error( "Cannot open dicom file" );

    }

    ::DcmMetaInfo* metaInfo = dicomFile.getMetaInfo();

    if ( metaInfo )
    {

      gkg::DicomTagInfo metaInfoList( -1, 
                                      "Meta Information", 
                                      "", 
                                      -1, 
                                      -1, 
                                      "", 
                                      "" );

      processGroup( metaInfo, metaInfoList.tagList );
      _dicomTagInfo.tagList.push_back( metaInfoList );

    }

    ::DcmDataset* dataset = dicomFile.getDataset();

    if ( dataset )
    {

      gkg::DicomTagInfo dataSetList( -1, "Dataset", "", -1, -1, "", "" );

      processGroup( dataset, dataSetList.tagList );
      _dicomTagInfo.tagList.push_back( dataSetList );

    }

  }
  GKG_CATCH( "gkg::DcmtkDicomDumpImplementation::DcmtkDicomDumpImplementation( "
             "gkg::DicomDump* dicomDump, const std::string& fileName )" );

}


gkg::DicomTagInfo& gkg::DcmtkDicomDumpImplementation::getDicomTagInfo()
{

  try
  {

    return _dicomTagInfo;

  }
  GKG_CATCH( "gkg::DicomTagInfo& "
             "gkg::DcmtkDicomDumpImplementation::getDicomTagInfo()" );

}


void gkg::DcmtkDicomDumpImplementation::processGroup( 
                                       ::DcmItem* item, 
                                       std::list< gkg::DicomTagInfo >& tagList )
{

  ::DcmElement* element = item->getElement( 0 );
  int32_t currentGroup = element->getTag().getGTag();
  gkg::DicomTagInfo* groupInfoList = new gkg::DicomTagInfo( currentGroup, 
                                                            "", 
                                                            "", 
                                                            -1, 
                                                            -1, 
                                                            "", 
                                                            "" );

  if ( groupInfoList )
  {

    int32_t group;
    unsigned long i, n = item->card();

    for ( i = 0; i < n; i++ )
    {

      element = item->getElement( i );
      group = element->getTag().getGTag();

      if ( group != currentGroup )
      {

        tagList.push_back( *groupInfoList );
        delete groupInfoList;
        currentGroup = group;
        groupInfoList = new gkg::DicomTagInfo( currentGroup, 
                                               "", 
                                               "", 
                                               -1, 
                                               -1, 
                                               "", 
                                               "" );

        if ( !groupInfoList )
        {

          return;

        }

      }

      processElement( element, groupInfoList->tagList );

    }

    tagList.push_back( *groupInfoList );
    delete groupInfoList;

  }

}


void gkg::DcmtkDicomDumpImplementation::processElement( 
                                       ::DcmElement* element, 
                                       std::list< gkg::DicomTagInfo >& tagList )
{

  ::OFString txt;
  ::DcmPixelSequence* pixelSeq = NULL;
  bool pixelSequence = false;
  ::DcmTag tag = element->getTag();
  unsigned long length = element->getLength();
  std::string vrName = tag.getVRName();
  std::string tagName = tag.getTagName();
  element->getOFStringArray( txt );

  std::string txtStr = txt.c_str();

  if ( txtStr.length() > 64 )
  {

    txtStr = txtStr.substr( 0, 64 );
    txtStr += "...";

  }

  if ( vrName == "UI" )
  {

    const char* uiName = ::dcmFindNameOfUID( txt.c_str() );

    if ( uiName )
    {

      std::string uiStr( uiName );

      if ( uiStr.length() )
      {

        txtStr += std::string( " (=" ) + uiStr + std::string( ")" );

      }

    }

  }

  if ( !length && ( tagName == "PixelData" ) )
  {

    pixelSequence = true;
    ::DcmPixelData* pixelData = static_cast< ::DcmPixelData* >( element );
    
    if ( pixelData )
    {
    
      E_TransferSyntax repType;
      const ::DcmRepresentationParameter* repParam = NULL;
      
      pixelData->getCurrentRepresentationKey( repType, repParam );
      pixelData->getEncapsulatedRepresentation( repType, repParam, pixelSeq );
      
      if ( pixelSeq )
      {
      
        length = pixelSeq->card();
	tagName = "Pixel Sequence";
	pixelSequence = true;
    
      }
      
    }
    
  }

  gkg::DicomTagInfo elementInfo( tag.getGTag(),
                                 tag.getXTag().toString().c_str(),
                                 vrName,
                                 element->getVM(),
                                 length,
                                 tagName,
                                 txtStr );

  if ( vrName == "SQ" )
  {

    ::DcmSequenceOfItems* seq = static_cast< ::DcmSequenceOfItems* >( element );

    if ( seq )
    {

      unsigned long i, n = seq->card();

      for ( i = 0; i < n; i++ )
      {

        processItem( seq->getItem( i ), elementInfo.tagList );

      }

    }

  }
  
  if ( pixelSequence && pixelSeq )
  {

    unsigned long i;

    for ( i = 0; i < length; i++ )
    {

      ::DcmPixelItem* pixelItem;

      pixelSeq->getItem( pixelItem, i );
      processPixelItem( pixelItem, elementInfo.tagList );

    }
  
  }

  tagList.push_back( elementInfo );

}


void gkg::DcmtkDicomDumpImplementation::processItem( 
                                       ::DcmItem* item, 
                                       std::list< gkg::DicomTagInfo >& tagList )
{

  ::DcmTag tag = item->getTag();

  gkg::DicomTagInfo itemInfo( tag.getGTag(),
                              tag.getXTag().toString().c_str(),
                              tag.getVRName(),
                              item->getVM(),
                              item->getLength(),
                              tag.getTagName(),
                              "" );

  unsigned long n = item->card();

  if ( n )
  {

    unsigned long i;

    for ( i = 0; i < n; i++ )
    {

      processElement( item->getElement( i ), itemInfo.tagList );

    }

  }

  tagList.push_back( itemInfo );

}


void gkg::DcmtkDicomDumpImplementation::processPixelItem( 
                                       ::DcmPixelItem* item, 
                                       std::list< gkg::DicomTagInfo >& tagList )
{

  ::OFString txt;
  ::DcmTag tag = item->getTag();
  item->getOFStringArray( txt );

  std::string txtStr = txt.c_str();

  if ( txtStr.length() > 64 )
  {

    txtStr = txtStr.substr( 0, 64 );
    txtStr += "...";

  }

  gkg::DicomTagInfo itemInfo( tag.getGTag(),
                              tag.getXTag().toString().c_str(),
                              tag.getVRName(),
                              item->getVM(),
                              item->getLength(),
                              tag.getTagName(),
                              txtStr );

  tagList.push_back( itemInfo );

}


gkg::DcmtkDicomDumpImplementation* 
gkg::DcmtkDicomDumpImplementation::getImplementation(
                                                     gkg::DicomDump* dicomDump )
{

  return static_cast< gkg::DcmtkDicomDumpImplementation* >(
                                               dicomDump->getImplementation() );

}
