#include <gkg-processing-plugin-dicom/DcmtkDicomSCPImplementation.h>


gkg::DcmtkDicomSCPImplementation::DcmtkDicomSCPImplementation(
                      gkg::DicomServiceClassProvider* dicomServiceClassProvider,
                      const std::string& title,
                      const std::string& incomingDirectory,
                      int32_t port,
                      DicomServiceClassProvider::TransferSyntax transferSyntax,
                      int32_t acseTimeout,
                      int32_t eosTimeout )
                                 : gkg::DicomServiceClassProviderImplementation(
                                                      dicomServiceClassProvider,
                                                      title,
                                                      incomingDirectory,
                                                      port,
                                                      transferSyntax,
                                                      acseTimeout,
                                                      eosTimeout ),
                                   _associationNetwork( 0 ),
                                   _endOfStudyThroughTimeoutEvent( false )
{
}


gkg::DcmtkDicomSCPImplementation::~DcmtkDicomSCPImplementation()
{
}


void gkg::DcmtkDicomSCPImplementation::initializeNetwork()
{

  try
  {

    // checking that DICOM dictionary is loaded
    if ( !dcmDataDict.isDictionaryLoaded() )
    {

      std::cerr << "no data dictionary loaded, check environment variable: "
                << DCM_DICT_ENVIRONMENT_VARIABLE
                << std::endl;

    }

    // initialize network, i.e. create an instance of T_ASC_Network
    OFCondition condition = ASC_initializeNetwork( NET_ACCEPTOR,
                                                   _port,
                                                   _acseTimeout,
                                                   &_associationNetwork );
    if ( condition.bad() )
    {

      DimseCondition::dump( condition );
      throw std::runtime_error( "unable to initialize network" );

    }

  }
  GKG_CATCH( "void "
             "gkg::DcmtkDicomSCPImplementation::initializeNetwork()" );

}


void gkg::DcmtkDicomSCPImplementation::run( bool verbose )
{

  try
  {

    OFCondition condition = EC_Normal;
    while ( condition.good() )
    {

      // receive an association and acknowledge or reject it; if the association 
      // was acknowledged, offer corresponding services and invoke one or more if
      // required
      condition = acceptAssociation( verbose );

    }

  }
  GKG_CATCH( "void gkg::DcmtkDicomSCPImplementation::run( "
             "bool verbose )" );

}


void gkg::DcmtkDicomSCPImplementation::dropNetwork()
{

  try
  {

    // drop the network, i.e. free memory of T_ASC_Network* structure; this call
    // is the counterpart of ASC_initializeNetwork(...) which was called above
    OFCondition condition = ASC_dropNetwork( &_associationNetwork );
    if ( condition.bad() )
    {

      DimseCondition::dump( condition );
      throw std::runtime_error( "unable to drop network" );

    }


  }
  GKG_CATCH( "void gkg::DcmtkDicomSCPImplementation::dropNetwork()" );

}


gkg::DcmtkDicomSCPImplementation* 
gkg::DcmtkDicomSCPImplementation::getImplementation(
                     gkg::DicomServiceClassProvider* dicomServiceClassProvider )
{

  return static_cast< gkg::DcmtkDicomSCPImplementation* >(
                               dicomServiceClassProvider->getImplementation() );

}


OFCondition gkg::DcmtkDicomSCPImplementation::acceptAssociation( bool verbose )
{

  try
  {

    OFBool shutdownAssociation = OFFalse;
    OFCondition condition, shutdownCondition = EC_Normal;
    const char* knownAbstractSyntaxes[] = { UID_VerificationSOPClass,
                                            UID_PrivateShutdownSOPClass };
    const char* transferSyntaxes[] = { NULL, NULL, NULL, NULL };
    int32_t transferSyntaxesCount = 0;

    // try to receive an association
    T_ASC_Association* association;
    condition = ASC_receiveAssociation( _associationNetwork,
                                        &association,
                                        ASC_DEFAULTMAXPDU,
                                        NULL,
                                        NULL,
                                        OFFalse,
                                        DUL_NOBLOCK,
                                        _eosTimeout );

#if OFFIS_DCMTK_VERSION_NUMBER >= 354
					
    if ( condition.code() == DULC_FORKEDCHILD )
    {
 
      goto cleanup;
 
    }

#endif

    // if some kind of error occured, take care of it
    if ( condition.bad() )
    {

      // if no association was received, check if other conditions are met
      if ( condition == DUL_NOASSOCIATIONREQUEST )
      {

        if ( !_lastStudyInstanceUID.empty() )
        {

          // indicatest the end-of-study-event occured through a timeout event
          _endOfStudyThroughTimeoutEvent = true;

          // before we actually execute those optional functions, we need to 
          // determine the path and name of the subdirectory into which the 
          // DICOM files for the last study were written
          _lastStudySubDirectoryPathAndName = _subDirectoryPathAndName;

          // now we can finally handle end-of-study events which might have to 
          // be executed
          executeEndOfStudyEvents( verbose );

          // also, we need to clear lastStudyInstanceUID to indicate
          // that the last study is not considered to be open any more
          _lastStudyInstanceUID.clear();

          // also, we need to clear subdirectoryPathAndName
          _subDirectoryPathAndName.clear();

          // reset the endOfStudyThroughTimeoutEvent variable
          _endOfStudyThroughTimeoutEvent = false;

        }
        else
        {

          if ( verbose )
          {

            DimseCondition::dump( condition );

          }

        }

      }

      goto cleanup;

    }

    if ( verbose )
    {

      std::cout << "association received" << std::endl;
      
      std::cout << "parameters : " << std::endl;
      ASC_dumpParameters( association->params, COUT );
      DIC_AE callingTitle;
      DIC_AE calledTitle;
#if OFFIS_DCMTK_VERSION_NUMBER >= 364
      ASC_getAPTitles( association->params,
                       callingTitle,
                       sizeof( callingTitle ),
                       calledTitle,
                       sizeof( calledTitle ),
                       NULL,
                       0 );
#else
      ASC_getAPTitles( association->params,
                       callingTitle,
                       calledTitle,
                       NULL );
#endif
      CERR << "called AE  : " << calledTitle << std::endl
           << "calling AE : " << callingTitle << std::endl;

    }


    // we prefer explicit transfer syntaxes; if we are running on a Little 
    // Endian machine, we prefer LittleEndianExplicitTransferSyntax to 
    // BigEndianTransferSyntax
    if ( _transferSyntax == gkg::DicomServiceClassProvider::Uncompressed )
    {

      if ( gLocalByteOrder == EBO_LittleEndian )
      {

        transferSyntaxes[ 0 ] = UID_LittleEndianExplicitTransferSyntax;
        transferSyntaxes[ 1 ] = UID_BigEndianExplicitTransferSyntax;

      }
      else
      {

        transferSyntaxes[ 0 ] = UID_BigEndianExplicitTransferSyntax;
        transferSyntaxes[ 1 ] = UID_LittleEndianExplicitTransferSyntax;

      }
      transferSyntaxes[ 2 ] = UID_LittleEndianImplicitTransferSyntax;
      transferSyntaxesCount = 3;

    }
    else if ( _transferSyntax == gkg::DicomServiceClassProvider::Implicit )
    {

      // we only support Little Endian Implicit
      transferSyntaxes[ 0 ]  = UID_LittleEndianImplicitTransferSyntax;
      transferSyntaxesCount = 1;

    }
    else if ( _transferSyntax == gkg::DicomServiceClassProvider::Little )
    {

      // we prefer Little Endian Explicit
      transferSyntaxes[ 0 ] = UID_LittleEndianExplicitTransferSyntax;
      transferSyntaxes[ 1 ] = UID_BigEndianExplicitTransferSyntax;
      transferSyntaxes[ 2 ] = UID_LittleEndianImplicitTransferSyntax;
      transferSyntaxesCount = 3;

    }
    else if ( _transferSyntax == gkg::DicomServiceClassProvider::Big )
    {

      // we prefer Big Endian Explicit
      transferSyntaxes[ 0 ] = UID_BigEndianExplicitTransferSyntax;
      transferSyntaxes[ 1 ] = UID_LittleEndianExplicitTransferSyntax;
      transferSyntaxes[ 2 ] = UID_LittleEndianImplicitTransferSyntax;
      transferSyntaxesCount = 3;

    }
    else if ( _transferSyntax == gkg::DicomServiceClassProvider::Lossless )
    {

      // we prefer JPEGLossless:Hierarchical-1stOrderPrediction 
      // (default lossless)
      transferSyntaxes[ 0 ] = UID_JPEGProcess14SV1TransferSyntax;
      transferSyntaxes[ 1 ] = UID_LittleEndianExplicitTransferSyntax;
      transferSyntaxes[ 2 ] = UID_BigEndianExplicitTransferSyntax;
      transferSyntaxes[ 3 ] = UID_LittleEndianImplicitTransferSyntax;
      transferSyntaxesCount = 4;

    }
    else if ( _transferSyntax == gkg::DicomServiceClassProvider::Jpeg8 )
    {

      // we prefer JPEGBaseline (default lossy for 8 bit images)
      transferSyntaxes[ 0 ] = UID_JPEGProcess1TransferSyntax;
      transferSyntaxes[ 1 ] = UID_LittleEndianExplicitTransferSyntax;
      transferSyntaxes[ 2 ] = UID_BigEndianExplicitTransferSyntax;
      transferSyntaxes[ 3 ] = UID_LittleEndianImplicitTransferSyntax;
      transferSyntaxesCount = 4;

    }
    else if ( _transferSyntax == gkg::DicomServiceClassProvider::Jpeg12 )
    {

      // we prefer JPEGExtended (default lossy for 12 bit images)
      transferSyntaxes[ 0 ] = UID_JPEGProcess2_4TransferSyntax;
      transferSyntaxes[ 1 ] = UID_LittleEndianExplicitTransferSyntax;
      transferSyntaxes[ 2 ] = UID_BigEndianExplicitTransferSyntax;
      transferSyntaxes[ 3 ] = UID_LittleEndianImplicitTransferSyntax;
      transferSyntaxesCount = 4;

    }
    else if ( _transferSyntax == gkg::DicomServiceClassProvider::J2kLossless )
    {

      // we prefer JPEG2000 Lossless
      transferSyntaxes[ 0 ] = UID_JPEG2000LosslessOnlyTransferSyntax;
      transferSyntaxes[ 1 ] = UID_LittleEndianExplicitTransferSyntax;
      transferSyntaxes[ 2 ] = UID_BigEndianExplicitTransferSyntax;
      transferSyntaxes[ 3 ] = UID_LittleEndianImplicitTransferSyntax;
      transferSyntaxesCount = 4;

    }
    else if ( _transferSyntax == gkg::DicomServiceClassProvider::J2kLossy )
    {

      // we prefer JPEG2000 Lossy
      transferSyntaxes[ 0 ] = UID_JPEG2000TransferSyntax;
      transferSyntaxes[ 1 ] = UID_LittleEndianExplicitTransferSyntax;
      transferSyntaxes[ 2 ] = UID_BigEndianExplicitTransferSyntax;
      transferSyntaxes[ 3 ] = UID_LittleEndianImplicitTransferSyntax;
      transferSyntaxesCount = 4;

    }
#if OFFIS_DCMTK_VERSION_NUMBER >= 354

    else if ( _transferSyntax == gkg::DicomServiceClassProvider::Rle )
    {

      // we prefer RLE Lossless
      transferSyntaxes[ 0 ] = UID_RLELosslessTransferSyntax;
      transferSyntaxes[ 1 ] = UID_LittleEndianExplicitTransferSyntax;
      transferSyntaxes[ 2 ] = UID_BigEndianExplicitTransferSyntax;
      transferSyntaxes[ 3 ] = UID_LittleEndianImplicitTransferSyntax;
      transferSyntaxesCount = 4;

    }

#endif
    else if ( _transferSyntax == gkg::DicomServiceClassProvider::Deflated )
    {

      // we prefer Deflated Explicit VR Little Endian
      transferSyntaxes[ 0 ] = UID_DeflatedExplicitVRLittleEndianTransferSyntax;
      transferSyntaxes[ 1 ] = UID_LittleEndianExplicitTransferSyntax;
      transferSyntaxes[ 2 ] = UID_BigEndianExplicitTransferSyntax;
      transferSyntaxes[ 3 ] = UID_LittleEndianImplicitTransferSyntax;
      transferSyntaxesCount = 4;

    }

    // accept the Verification SOP Class if presented
    condition = ASC_acceptContextsWithPreferredTransferSyntaxes(
                                      association->params,
                                      knownAbstractSyntaxes,
                                      DIM_OF( knownAbstractSyntaxes ),
                                      transferSyntaxes, transferSyntaxesCount );
    if ( condition.bad() )
    {

      if ( verbose )
      {

        DimseCondition::dump( condition );

      }
      goto cleanup;

    }

    // the array of Storage SOP Class UIDs comes from dcuid.h

#if OFFIS_DCMTK_VERSION_NUMBER <= 352


    // on RedHat 7.3, dcmtk-3.5.2 is used and some variable names are
    // different:
    //     dcmtk-3.5.2                            dcmtk-3.5.4
    //  dcmStorageSOPClassUIDs          ->   dcmAllStorageSOPClassUIDs
    //  numberOfDcmStorageSOPClassUIDs  ->   numberOfAllDcmStorageSOPClassUIDs


    condition = ASC_acceptContextsWithPreferredTransferSyntaxes(
                                      association->params,
                                      dcmStorageSOPClassUIDs,
                                      numberOfDcmStorageSOPClassUIDs,
                                      transferSyntaxes, transferSyntaxesCount );

#elif OFFIS_DCMTK_VERSION_NUMBER < 362

    condition = ASC_acceptContextsWithPreferredTransferSyntaxes(
                                      association->params,
                                      dcmAllStorageSOPClassUIDs,
                                      numberOfAllDcmStorageSOPClassUIDs,
                                      transferSyntaxes, transferSyntaxesCount );

#else

    condition = ASC_acceptContextsWithPreferredTransferSyntaxes(
                                      association->params,
                                      dcmAllStorageSOPClassUIDs,
                                      numberOfDcmAllStorageSOPClassUIDs,
                                      transferSyntaxes, transferSyntaxesCount );

#endif

    if ( condition.bad() )
    {

      if ( verbose )
      {

        DimseCondition::dump( condition );

      }
      goto cleanup;

    }

    if ( ASC_findAcceptedPresentationContextID( association, 
                                            UID_PrivateShutdownSOPClass ) != 0 )
    {
    
      shutdownAssociation = OFTrue;
      condition = refuseAssociation( association );
      if ( condition.bad() )
      {

        if ( verbose )
        {

          DimseCondition::dump( condition );

        }

      }
      shutdownCondition = makeOFCondition( OFM_dcmnet, 1080, 
                                           OF_error, "Shutdown" );
      goto cleanup;

    }

    // set our app title
    ASC_setAPTitles( association->params, NULL, NULL,
                     _title.c_str() );

    // acknowledge or reject this association
    char buffer[ BUFSIZ ];
#if OFFIS_DCMTK_VERSION_NUMBER >= 364
    condition = ASC_getApplicationContextName( association->params,
                                               buffer,
                                               sizeof( buffer ) );
#else
    condition = ASC_getApplicationContextName( association->params, buffer );
#endif
    if ( ( condition.bad() ) ||
         strcmp( buffer, UID_StandardApplicationContext ) != 0 )
    {

      // reject: the application context name is not supported
      T_ASC_RejectParameters rejection =
      {

        ASC_RESULT_REJECTEDPERMANENT,
        ASC_SOURCE_SERVICEUSER,
        ASC_REASON_SU_APPCONTEXTNAMENOTSUPPORTED

      };

      if ( verbose )
      {

        std::cout << "association rejected: bad application context name: "
                  << buffer
                  << std::endl;
      }
      condition = ASC_rejectAssociation( association, &rejection );
      if ( condition.bad() )
      {

        if ( verbose )
        {

          DimseCondition::dump( condition );

        }

      }
      goto cleanup;

    }
    else
    {

      condition = ASC_acknowledgeAssociation( association );
      if ( condition.bad() )
      {

        if ( verbose )
        {

          DimseCondition::dump( condition );

        }
        goto cleanup;

      }
      if ( verbose )
      {

        std::cout << "association acknowledged (max send PDV: "
                  << association->sendPDVLength << ")"
                  << std::endl;
        if ( ASC_countAcceptedPresentationContexts( association->params ) == 0 )
        {

          std::cout << "    (but no valid presentation contexts)"
                    << std::endl;

        }
        ASC_dumpParameters( association->params, COUT );

      }

    }

    // store calling and called aetitle in global variables to enable using them
    // after; enclose in quotation marks because aetitles may contain space 
    // characters
    DIC_AE callingTitle;
    DIC_AE calledTitle;
#if OFFIS_DCMTK_VERSION_NUMBER >= 364
    if ( ASC_getAPTitles( association->params,
                          callingTitle,
                          sizeof( callingTitle ),
                          calledTitle,
                          sizeof( calledTitle ),
                          NULL,
                          0 ).good() )
#else
    if ( ASC_getAPTitles( association->params,
                          callingTitle,
                          calledTitle,
                          NULL ).good() )
#endif
    {

      _callingAETitle = "\"";
      _callingAETitle += callingTitle;
      _callingAETitle += "\"";
      _calledAETitle = "\"";
      _calledAETitle += calledTitle;
      _calledAETitle += "\"";

    }
    else
    {

      // should never happen
      _callingAETitle.clear();
      _calledAETitle.clear();

    }

    // now do the real work, i.e. receive DIMSE commmands over the network 
    // connection which was established and handle these commands 
    // correspondingly; in case of GkgDicomServiceClassProvider only C-ECHO-RQ and 
    // C-STORE-RQ commands can be processed
    condition = processCommands( association, verbose );

    if ( condition == DUL_PEERREQUESTEDRELEASE )
    {

      if ( verbose )
      {

        std::cout << "association release" << std::endl;

      }
      condition = ASC_acknowledgeRelease( association );

    }
    else if ( condition == DUL_PEERABORTEDASSOCIATION )
    {

      if ( verbose )
      {

        std::cout << "association aborted" << std::endl;

      }

    }
    else
    {

      std::cerr << "gkg::DcmtkDicomSCPImplementation: "
                << "DIMSE failure (aborting association)"
                << std::endl;
      // some kind of error so abort the association
      condition = ASC_abortAssociation( association );

    }

cleanup:

    condition = ASC_dropSCPAssociation( association );
    if ( condition.bad() )
    {

      DimseCondition::dump( condition );
      throw std::runtime_error( "unable to drop association configuration" );

    }
    condition = ASC_destroyAssociation( &association );
    if ( condition.bad() )
    {

      DimseCondition::dump( condition );
      throw std::runtime_error( "unable to destroy association configuration" );

    }

    return shutdownAssociation ? shutdownCondition : condition;

  }
  GKG_CATCH( "OFCondition gkg::DcmtkDicomSCPImplementation::"
             "acceptAssociation( bool verbose )" );

}


OFCondition gkg::DcmtkDicomSCPImplementation::processCommands( 
                                                 T_ASC_Association* association,
                                                 bool verbose )
{

  try
  {

    OFCondition condition = EC_Normal;

    // start a loop to be able to receive more than one DIMSE command
    T_DIMSE_Message message;
    T_ASC_PresentationContextID presentationContextID = 0;
    DcmDataset* statusDetail = NULL;

    while( ( condition == EC_Normal ) ||
           ( condition == DIMSE_NODATAAVAILABLE ) ||
           ( condition == DIMSE_OUTOFRESOURCES ) )
    {

      condition = DIMSE_receiveCommand( association,
                                        DIMSE_NONBLOCKING,
                                        _eosTimeout,
                                        &presentationContextID,
                                        &message,
                                        &statusDetail );

      // check what kind of error occurred; if no data was received, check if 
      // certain other conditions are met
      if ( condition == DIMSE_NODATAAVAILABLE )
      {

        // if in addition to the fact that no data was received also end of 
        // study is reached, if at the same time there is still a study which is
        // considered to be open (i.e. we were actually expecting to receive 
        // more objects that belong to this study) (this is the case if 
        // lastStudyInstanceUID does not equal NULL), we have to consider that
        // all objects for the current study have been received; in such an 
        // "end-of-study" case, we execute EOS function
        if ( !_lastStudyInstanceUID.empty() )
        {

          // indicatest the end-of-study-event occured through a timeout event
          _endOfStudyThroughTimeoutEvent = true;
          
          // before we actually execute those optional functions, we need to 
          // determine the path and name of the subdirectory into which the 
          // DICOM files for the last study were written
          _lastStudySubDirectoryPathAndName = _subDirectoryPathAndName;

          // now we can finally handle end-of-study events which might have to 
          // be executed
          executeEndOfStudyEvents( verbose );

          // also, we need to clear lastStudyInstanceUID to indicate
          // that the last study is not considered to be open any more
          _lastStudyInstanceUID.clear();

          // also, we need to clear subdirectoryPathAndName
          _subDirectoryPathAndName.clear();

          // reset the endOfStudyThroughTimeoutEvent variable
          _endOfStudyThroughTimeoutEvent = false;

        }

      }

      // if the command which was received has extra status detail information, 
      // dump this information
      if ( statusDetail )
      {

        std::cout << "extra status detail:" << std::endl;
        statusDetail->print( COUT );
        delete statusDetail;

      }

      // check if peer did release or abort, or if we have a valid message
      if ( condition == EC_Normal )
      {

        // in case we received a valid message, process this command note that 
        // this command can only process a C-ECHO-RQ and a C-STORE-RQ
        switch ( message.CommandField )
        {

          case DIMSE_C_ECHO_RQ:

            // process C-ECHO-Request
            condition = echoSCP( association,
                                 &message,
                                 presentationContextID,
                                 verbose );
            break;

          case DIMSE_C_STORE_RQ:

            // process C-STORE-Request
            condition = storeSCP( association,
                                  &message,
                                  presentationContextID,
                                  verbose );
            break;

          default:

            // we cannot handle this kind of message
            condition = DIMSE_BADCOMMANDTYPE;
            std::cerr << "gkg::DcmtkDicomSCPImplementation: "
                      << "cannot handle command: "
                      << std::ios::hex
                      << ( uint32_t )message.CommandField
                      << std::endl;
            break;

        }

      }

    }

    return condition;

  }
  GKG_CATCH( "OFCondition gkg::DcmtkDicomSCPImplementation::"
             "processCommands( T_ASC_Association* association, "
             "bool verbose )" );

}


OFCondition gkg::DcmtkDicomSCPImplementation::echoSCP(
                              T_ASC_Association* association,
                              T_DIMSE_Message* message,
                              T_ASC_PresentationContextID presentationContextID,
                              bool verbose )
{

  try
  {

    if ( verbose )
    {

      std::cout << "received " << std::flush;
      DIMSE_printCEchoRQ( stdout, &message->msg.CEchoRQ );

    }

    // the echo succeeded !
    OFCondition condition = DIMSE_sendEchoResponse( association,
                                                    presentationContextID,
                                                    &message->msg.CEchoRQ,
                                                    STATUS_Success,
                                                    NULL );
    if ( condition.bad() )
    {

      std::cerr << "gkg::DcmtkDicomSCPImplementation:: echo SCP failed"
                << std::endl;
      DimseCondition::dump( condition );

    }
    return condition;

  }
  GKG_CATCH( "OFCondition gkg::DcmtkDicomSCPImplementation::"
             "echoSCP( T_ASC_Association* association, "
             "T_DIMSE_Message* message, "
             "T_ASC_PresentationContextID presentationContextID, "
             "bool verbose )" );

}


OFCondition gkg::DcmtkDicomSCPImplementation::storeSCP( 
                              T_ASC_Association* association,
                              T_DIMSE_Message* message,
                              T_ASC_PresentationContextID presentationContextID,
                              bool verbose )
{

  try
  {

    // assign the actual information of the C-STORE-RQ command to a local 
    // variable
    T_DIMSE_C_StoreRQ* request = &message->msg.CStoreRQ;

    // create unique filename by generating a temporary UID and using ".X." as 
    // an infix
    char imageFileName[ 2048 ];
    sprintf( imageFileName, "%s%s%s.X.%s.MRDC",
             _incomingDirectory.c_str(),
             gkg::getDirectorySeparator().c_str(),
             dcmSOPClassUIDToModality( request->AffectedSOPClassUID ),
             request->AffectedSOPInstanceUID );

    // dump some information if required
    if ( verbose )
    {

      std::cout << "received " << std::flush;
      DIMSE_printCStoreRQ( stdout, request );

    }

    // intialize some variables
    gkg::DcmtkDicomSCPImplementation::StoreCallbackData callbackData;
    DcmFileFormat dcmFileFormat;

    callbackData.dcmtkDicomSCPImplementation = this;
    callbackData.association = association;
    callbackData.imageFileName = std::string( imageFileName );
    callbackData.dcmFileFormat = &dcmFileFormat;
    callbackData.verbose = verbose;

    // define an address where the information which will be received over the
    // network will be stored
    DcmDataset* dataSet = dcmFileFormat.getDataset();

    OFCondition condition = DIMSE_storeProvider( association,
                                                 presentationContextID,
                                                 request,
                                                 NULL,
                                                 OFTrue,
                                                 &dataSet,
                                                 storeSCPCallback,
                                                 &callbackData,
                                                 DIMSE_BLOCKING,
                                                 0 );

    // if some error occured, dump corresponding information and remove the 
    // outfile if necessary
    if ( condition.bad() )
    {

      std::cerr << "gkg::DcmtkDicomSCPImplementation:: "
                << " store SCP failed: "
                << std::endl;
      DimseCondition::dump( condition );

      // remove file
      if ( strcmp( imageFileName, NULL_DEVICE_NAME ) != 0 )
      {

        unlink( imageFileName );

      }

    }

    // if everything was successful so far we execute a certain command which 
    // was passed to the application
    if ( condition.good() )
    {

      executeOnReception( imageFileName, verbose );

    }

    // if everything was successful so far, go ahead and handle possible 
    // end-of-study events
    if ( condition.good() )
    {

      executeEndOfStudyEvents( verbose );

    }

    return condition;

  }
  GKG_CATCH( "OFCondition gkg::DcmtkDicomSCPImplementation::"
             "storeSCP( T_ASC_Association* association, "
             "T_DIMSE_Message* message, "
             "T_ASC_PresentationContextID presentationContextID, "
             "bool verbose )" );

}


void gkg::DcmtkDicomSCPImplementation::executeOnReception( 
                                               const std::string& imageFileName,
                                               bool verbose )
{

  try
  {

    std::list<
           gkg::DicomServiceClassProvider::OnReceptionCallback >::const_iterator
      f = _onReceptionCallbacks.begin(),
      fe = _onReceptionCallbacks.end();
    while ( f != fe )
    {

      ( *f )( imageFileName, verbose );
      ++ f;

    }

  }
  GKG_CATCH( "void gkg::DcmtkDicomSCPImplementation::"
             "executeOnReception( const std::string& imageFileName, "
             "bool verbose )" );

}


void gkg::DcmtkDicomSCPImplementation::executeEndOfStudyEvents( bool verbose )
{

  try
  {

    if ( !_lastStudySubDirectoryPathAndName.empty() )
    {

      // this function executes the given command line; the execution will be
      // performed in a new process which can be run in the background so that
      // it does not slow down the execution of GkgDicomServiceClassProvider
      pid_t pid = fork();
      if ( pid < 0 )
      {

        std::cerr << "gkg::DcmtkDicomSCPImplementation: "
                  << "cannot execute EOS events"
                  << std::endl;

      }
      else if ( pid == 0 )
      {

        // in case we are the child process, execute the command etc.
        if ( verbose )
        {

          std::cout << "doing end of study events in directory : "
                    << _lastStudySubDirectoryPathAndName
                    << std::endl;

        }

        std::list< 
          gkg::DicomServiceClassProvider::OnEndOfStudyCallback >::const_iterator
          f = _onEndOfStudyCallbacks.begin(),
          fe = _onEndOfStudyCallbacks.end();
        while ( f != fe )
        {

          ( *f )( _lastStudySubDirectoryPathAndName, verbose );
          ++ f;

        }
        abort();

      }

    }
    _lastStudySubDirectoryPathAndName.clear();

  }
  GKG_CATCH( "void gkg::DcmtkDicomSCPImplementation::"
             "executeEndOfStudyEvents( bool verbose )" );

}


// callbackData  : input data for this callback function
// progress      : the state of progress
//                 (identifies if this is the initial or final call to this 
//                  function, or a call in between these two calls)
// request       : input original store request message
// imageFileName : the path to and name of the file the information shall be 
//                 written to
// imageDataSet  : input data set which shall be stored in the image file
// rsp           : in/out C-STORE-RSP message (will be sent after the call to 
//                 this function)
// statusDetail  : in/out variable that can be used to capture detailed 
//                 information with regard to the status information which is
//                 captured in the status element (0000,0900); note that if this 
//                 function does specify any such information, the pointer will
//                 be set to NULL
void gkg::DcmtkDicomSCPImplementation::storeSCPCallback(
                                                void* cbData,
                                                T_DIMSE_StoreProgress* progress,
                                                T_DIMSE_C_StoreRQ* request,
                                                char* ,
                                                DcmDataset** imageDataSet,
                                                T_DIMSE_C_StoreRSP* rsp,
                                                DcmDataset** statusDetail )
{

  try
  {

    gkg::DcmtkDicomSCPImplementation::StoreCallbackData* callbackData = 
      ( gkg::DcmtkDicomSCPImplementation::StoreCallbackData* )cbData;
    DIC_UI sopClass;
    DIC_UI sopInstance;


    gkg::DcmtkDicomSCPImplementation*
      dcmtkDicomSCPImplementation = callbackData->dcmtkDicomSCPImplementation;

    // dump some information if required (depending on the progress state)
    if ( callbackData->verbose )
    {

      switch ( progress->state )
      {

        case DIMSE_StoreBegin:

          std::cout << "RECV:" << std::flush;
          break;

        case DIMSE_StoreEnd:

          std::cout << std::endl;
          break;

        default:

          std::cout << '.' << std::flush;
          break;

      }
      std::cout << std::flush;

    }

    // if this is the final call of this function, save the data which was 
    // received to a file (note that we could also save the image somewhere 
    // else, put it in database, etc.)
    if ( progress->state == DIMSE_StoreEnd )
    {

      // do not send status detail information
      *statusDetail = 0;

      // we want to write the received information to a file only if this 
      // information is present
      if ( ( imageDataSet ) && ( *imageDataSet ) )
      {

        // determine the study instance UID in the (current) DICOM object that has
        // just been received note that if findAndGetString says the resulting 
        // study instance UID equals NULL, the study instance UID in the (current)
        // DICOM object is an empty string; in general: no matter what happened, 
        // we want to create a new string that will contain a corresponding value
        // for the current study instance UID
        const char* tmpStr1 = 0;
        std::string lastStudyInstanceUID =
          callbackData->dcmtkDicomSCPImplementation->getLastStudyInstanceUID();
        std::string currentStudyInstanceUID;
        DcmTagKey studyInstanceUIDTagKey( DCM_StudyInstanceUID );
        OFCondition ec = ( *imageDataSet )->findAndGetString(
                                     studyInstanceUIDTagKey, tmpStr1, OFFalse );
        if ( ec != EC_Normal )
        {

          std::cerr << "gkg::DcmtkDicomSCPImplementationt: "
                    << "no study instance UID found in data"
                    << std::endl;
          rsp->DimseStatus = STATUS_STORE_Error_CannotUnderstand;
          return;

        }
        if ( tmpStr1 )
        {

          currentStudyInstanceUID = tmpStr1;

        }

        // if this is the first DICOM object that was received or if the study 
        // instance UID in the current DICOM object does not equal the last 
        // object's study instance UID we need to create a new subdirectory in 
        // which the current DICOM object will be stored
        if ( dcmtkDicomSCPImplementation->getLastStudyInstanceUID().empty() ||
             ( dcmtkDicomSCPImplementation->getLastStudyInstanceUID() !=
               currentStudyInstanceUID ) )
        {

          // if lastStudyInstanceUID is non-empty, we have just completed 
          // receiving all objects for one study; in such a case, we need to set
          // a certain indicator variable (lastStudySubDirectoryPathAndName),
          // so we know that executeOnEndOfStudy() might have to be executed
          // later; in detail, this indicator variable will contain the path and
          // name of the last study's subdirectory, so that we can still 
          // remember this directory, when we execute executeOnEndOfStudy(); the
          // memory that is allocated for this variable here will be freed after
          // the execution of executeOnEndOfStudy()
          if ( !dcmtkDicomSCPImplementation->getLastStudyInstanceUID().empty() )
          {

            dcmtkDicomSCPImplementation->
                                        storeLastStudySubDirectoryPathAndName();

          }

          // create the new lastStudyInstanceUID value according to the value in
          // the current DICOM object
          dcmtkDicomSCPImplementation->setLastStudyInstanceUID(
                                                      currentStudyInstanceUID );

          // get the current time (needed for subdirectory name)
          OFDateTime dateTime;
          dateTime.setCurrentDateTime();
          // create a name for the new subdirectory. pattern: 
          // gkg_[YYYYMMDD]_[HHMMSSMMM] (use current datetime)
          char buffer[ 32 ];
          sprintf( buffer, "_%04u%02u%02u_%02u%02u%02u%03u",
                   dateTime.getDate().getYear(),
                   dateTime.getDate().getMonth(),
                   dateTime.getDate().getDay(),
                   dateTime.getTime().getHour(),
                   dateTime.getTime().getMinute(),
                   dateTime.getTime().getIntSecond(),
                   dateTime.getTime().getMilliSecond() );
          std::string subDirectoryName = "gkg";
          subDirectoryName += buffer;

          // create subDirectoryPathAndName (string with full path to new 
          // subdirectory)
          std::string tmpSubDirectoryPathAndName = callbackData->imageFileName;
          size_t position = tmpSubDirectoryPathAndName.rfind(
                                                 gkg::getDirectorySeparator() );
          if ( position != std::string::npos )
          {

            tmpSubDirectoryPathAndName.erase( position + 1 );

          }
          tmpSubDirectoryPathAndName += subDirectoryName;
          dcmtkDicomSCPImplementation->setSubDirectoryPathAndName(
                                                   tmpSubDirectoryPathAndName );

          // check if the subdirectory is already existent
          gkg::Directory directory( tmpSubDirectoryPathAndName );
          if ( directory.isValid() )
          {

            std::cerr << "gkg::DcmtkDicomSCPImplementation: warning: "
                      << "subdirectory '" << tmpSubDirectoryPathAndName 
                      << "'for studies already existent"
                      << std::endl;

          }

          // if it is not existent create it
          directory.mkdir();

        }

        // integrate subdirectory name into file name (note that 
        // cbdata->imageFileName currently contains both path and file name;
        // however, the path refers to the output directory captured in 
        // incomingDirectory)
        size_t pos = callbackData->imageFileName.rfind(
                       gkg::getDirectorySeparator() );
        std::string baseName = callbackData->imageFileName.substr(
                                  pos + 1,
                                  callbackData->imageFileName.length() 
                                  - pos - 1 );
        std::string fileName = 
          dcmtkDicomSCPImplementation->getSubDirectoryPathAndName() +
          gkg::getDirectorySeparator() +
          baseName;

        // update global variable outputFileNameArray
        // (might be used in executeOnReception() and renameOnEndOfStudy)
        //outputFileNameList.push_back( baseName );

        // determine the transfer syntax which shall be used to write the 
        // information to the file
        E_TransferSyntax xfer = ( *imageDataSet )->getOriginalXfer();

        // store file either with meta header or as pure dataset
#if OFFIS_DCMTK_VERSION_NUMBER >= 360
        OFCondition condition = callbackData->dcmFileFormat->saveFile(
                                                             fileName.c_str(),
                                                             xfer,
                                                             EET_ExplicitLength,
                                                             EGL_recalcGL,
                                                             EPD_withoutPadding,
                                                             0U,
                                                             0U,
                                                             EWM_fileformat );
#else
        OFCondition condition = callbackData->dcmFileFormat->saveFile(
                                                             fileName.c_str(),
                                                             xfer,
                                                             EET_ExplicitLength,
                                                             EGL_recalcGL,
                                                             EPD_withoutPadding,
                                                             0U,
                                                             0U,
                                                             OFFalse );
#endif
        if ( condition.bad() )
        {

          std::cerr << "GkgDicomServiceClassProvider: cannot write image file: "
                    << fileName.c_str()
                    << std::endl;
          rsp->DimseStatus = STATUS_STORE_Refused_OutOfResources;

        }

        // check the image to make sure it is consistent, i.e. that its sopClass
        // and sopInstance correspond to those mentioned in the request; if not,
        // set the status in the response message variable
        if ( rsp->DimseStatus == STATUS_Success )
        {

          // which SOP class and SOP instance ?
#if OFFIS_DCMTK_VERSION_NUMBER >= 364
          if ( !DU_findSOPClassAndInstanceInDataSet( *imageDataSet,
                                                     sopClass,
                                                     sizeof( sopClass ),
                                                     sopInstance,
                                                     sizeof( sopInstance ),
                                                     OFFalse ) )
#else
          if ( !DU_findSOPClassAndInstanceInDataSet( *imageDataSet,
                                                     sopClass,
                                                     sopInstance,
                                                     OFFalse ) )
#endif
          {

            std::cerr << "GkgDicomServiceClassProvider: bad image file: "
                      << fileName.c_str()
                      << std::endl;
            rsp->DimseStatus = STATUS_STORE_Error_CannotUnderstand;

          }
          else if ( strcmp( sopClass, request->AffectedSOPClassUID ) != 0 )
          {

            rsp->DimseStatus = STATUS_STORE_Error_DataSetDoesNotMatchSOPClass;

          }
          else if ( strcmp( sopInstance, request->AffectedSOPInstanceUID ) != 0)
          {

            rsp->DimseStatus = STATUS_STORE_Error_DataSetDoesNotMatchSOPClass;

          }

        }

      }

    }

  }
  GKG_CATCH( "void gkg::DcmtkDicomSCPImplementation::storeSCPCallback( "
             "void* cbData, "
             "T_DIMSE_StoreProgress* progress, "
             "T_DIMSE_C_StoreRQ* request, "
             "char* imageFileName, "
             "DcmDataset** imageDataSet, "
             "T_DIMSE_C_StoreRSP* rsp, "
             "DcmDataset** statusDetail )" );

}


const std::string& 
gkg::DcmtkDicomSCPImplementation::getLastStudyInstanceUID() const
{

  try
  {

    return _lastStudyInstanceUID;

  }
  GKG_CATCH( "const std::string& "
             "gkg::DcmtkDicomSCPImplementation::"
             "getLastStudyInstanceUID() const" );

}


void gkg::DcmtkDicomSCPImplementation::setLastStudyInstanceUID(
                                       const std::string& lastStudyInstanceUID )
{

  try
  {

    _lastStudyInstanceUID = lastStudyInstanceUID;

  }
  GKG_CATCH( "void gkg::DcmtkDicomSCPImplementation::"
             "setLastStudyInstanceUID( "
             "const std::string& lastStudyInstanceUID )" );

}


const std::string& 
gkg::DcmtkDicomSCPImplementation::getSubDirectoryPathAndName() const
{

  try
  {

    return _subDirectoryPathAndName;

  }
  GKG_CATCH( "const std::string& "
             "gkg::DcmtkDicomSCPImplementation::"
             "getSubDirectoryPathAndName() const" );

}


void gkg::DcmtkDicomSCPImplementation::setSubDirectoryPathAndName(
                                    const std::string& subDirectoryPathAndName )
{

  try
  {

    _subDirectoryPathAndName = subDirectoryPathAndName;

  }
  GKG_CATCH( "void gkg::DcmtkDicomSCPImplementation::"
             "setSubDirectoryPathAndName( "
             "const std::string& subDirectoryPathAndName )" );

}


void 
gkg::DcmtkDicomSCPImplementation::storeLastStudySubDirectoryPathAndName(
                                                                               )
{

  try
  {

    _lastStudySubDirectoryPathAndName = _subDirectoryPathAndName;

  }
  GKG_CATCH( "void gkg::DcmtkDicomSCPImplementation::"
             "storeLastStudySubDirectoryPathAndName()" );

}


OFCondition
gkg::DcmtkDicomSCPImplementation::refuseAssociation( 
                                                T_ASC_Association* association )
{

  try
  {
  
    T_ASC_RejectParameters rejection;
    
    rejection.result = ASC_RESULT_REJECTEDPERMANENT;
    rejection.source = ASC_SOURCE_SERVICEUSER;
    rejection.reason = ASC_REASON_SU_NOREASON;

    OFCondition condition = ASC_rejectAssociation( association, &rejection );
    
    return condition;

  }
  GKG_CATCH( "OFCondition gkg::DcmtkDicomSCPImplementation::"
             "refuseAssociation()" );

}



