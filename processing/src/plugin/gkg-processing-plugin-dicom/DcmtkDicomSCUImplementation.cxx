#include <gkg-processing-plugin-dicom/DcmtkDicomSCUImplementation.h>


// SCU stands for Service Class User


gkg::DcmtkDicomSCUImplementation::DcmtkDicomSCUImplementation(
                           DicomServiceClassUser* dicomServiceClassUser,
	                   const std::string& callingTitle,
	                   const std::string& calledTitle,
			   const std::string& peerHostName,
	                   int32_t port,
	                   DicomServiceClassUser::TransferSyntax transferSyntax,
	                   int32_t acseTimeout )
			         : gkg::DicomServiceClassUserImplementation(
				                          dicomServiceClassUser,
					                  callingTitle,
					                  calledTitle,
							  peerHostName,
					                  port,
					                  transferSyntax,
					                  acseTimeout ),
				   _associationNetwork( 0 ),
				   _currentPresentationContextID( 0 ),
				   _currentMessageID( 0 )
{

  initializeTransferSyntaxes();

}


gkg::DcmtkDicomSCUImplementation::~DcmtkDicomSCUImplementation()
{
}


void gkg::DcmtkDicomSCUImplementation::initializeNetwork( 
                                DicomServiceClassUser::NetworkRole networkRole )
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

    T_ASC_NetworkRole ascNetworkRole = 
                        ( networkRole == DicomServiceClassUser::NetRequestor ) ?
	                NET_REQUESTOR : NET_ACCEPTORREQUESTOR;

    // initialize network, i.e. create an instance of T_ASC_Network
    OFCondition condition = ASC_initializeNetwork( ascNetworkRole,
                                                   _port,
                                                   _acseTimeout,
                                                   &_associationNetwork );
    if ( condition.bad() )
    {

      DimseCondition::dump( condition );
      throw std::runtime_error( "unable to initialize network" );

    }

  }
  GKG_CATCH( "void gkg::DcmtkDicomSCUImplementation::initializeNetwork( "
             "DicomServiceClassUser::NetworkRole networkRole )" );

}


void gkg::DcmtkDicomSCUImplementation::store( 
                                     std::list< std::string >& /*fileNameList*/, 
                                     bool /*verbose*/ )
{

  throw std::runtime_error( "store not yet implemented" );

}


void gkg::DcmtkDicomSCUImplementation::get( 
                                     std::list< std::string >& /*fileNameList*/, 
                                     bool /*verbose*/ )
{

  throw std::runtime_error( "get not yet implemented" );

}


void gkg::DcmtkDicomSCUImplementation::find( 
                                     std::list< std::string >& /*fileNameList*/, 
                                     bool /*verbose*/ )
{

  throw std::runtime_error( "find not yet implemented" );

}


void gkg::DcmtkDicomSCUImplementation::move( 
                                     std::list< std::string >& /*fileNameList*/, 
                                     bool /*verbose*/ )
{

  throw std::runtime_error( "move not yet implemented" );

}


void gkg::DcmtkDicomSCUImplementation::echo( uint32_t /*repeatCount*/, 
                                             bool /*verbose*/ )
{

  throw std::runtime_error( "echo not yet implemented" );

}


void gkg::DcmtkDicomSCUImplementation::cancel( bool verbose )
{

  try
  {
  
    if ( _associationNetwork && _currentPresentationContextID &&
         _currentMessageID )
    {
    
      OFCondition condition = DIMSE_sendCancelRequest( 
                                                 _association,
                                                 _currentPresentationContextID, 
                                                 _currentMessageID );
						  
      if ( condition.bad() )
      {
      
        if ( verbose )
	{
	
	  DimseCondition::dump( condition );
	
	}
	
      }
    
    }
  
  }
  GKG_CATCH( "gkg::DcmtkDicomSCUImplementation::cancel( bool verbose )" );

}


void gkg::DcmtkDicomSCUImplementation::shutdown( bool verbose )
{

  try
  {
  
    OFCondition condition = initializeAssociation( verbose );
    if ( condition.bad() )
    {
    
      throw std::runtime_error( "unable to initialize association" );
    
    }
    
    // set the presentation context which will be negotiated when the
    // network connection will be established
    condition = ASC_addPresentationContext( _associationParameters,
                                            1,
					    UID_PrivateShutdownSOPClass,
					    _transferSyntaxes,
					    _transferSyntaxesCount );
    if ( condition.bad() )
    {
    
      if ( verbose )
      {

        DimseCondition::dump( condition );
	
      }
    
      throw std::runtime_error( "unable set the presentation context" );
    
    }
    
    if ( verbose )
    {
    
      std::cout << "Request parameters" << std::endl;
      ASC_dumpParameters( _associationParameters, COUT );
    
    }
    
    if ( requestAssociation( verbose ) )
    {
    
      if ( !releaseAssociation( EC_Normal, verbose ) )
      {
    
        throw std::runtime_error( "unable to release association" );

      }
   
    }

  }
  GKG_CATCH( "void gkg::DcmtkDicomSCUImplementation::shutdown( "
             "bool verbose )" );
  
}


void gkg::DcmtkDicomSCUImplementation::dropNetwork()
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
  GKG_CATCH( "void gkg::DcmtkDicomSCUImplementation::dropNetwork()" );

}


gkg::DcmtkDicomSCUImplementation* 
gkg::DcmtkDicomSCUImplementation::getImplementation(
                                  DicomServiceClassUser* dicomServiceClassUser )
{

  return static_cast< gkg::DcmtkDicomSCUImplementation* >(
                                   dicomServiceClassUser->getImplementation() );

}


void gkg::DcmtkDicomSCUImplementation::initializeTransferSyntaxes()
{

  try
  {
  
    // we prefer explicit transfer syntaxes; if we are running on a Little 
    // Endian machine, we prefer LittleEndianExplicitTransferSyntax to 
    // BigEndianTransferSyntax
    if ( _transferSyntax == gkg::DicomServiceClassUser::Uncompressed )
    {

      if ( gLocalByteOrder == EBO_LittleEndian )
      {

        _transferSyntaxes[ 0 ] = UID_LittleEndianExplicitTransferSyntax;
        _transferSyntaxes[ 1 ] = UID_BigEndianExplicitTransferSyntax;

      }
      else
      {

        _transferSyntaxes[ 0 ] = UID_BigEndianExplicitTransferSyntax;
        _transferSyntaxes[ 1 ] = UID_LittleEndianExplicitTransferSyntax;

      }
      _transferSyntaxes[ 2 ] = UID_LittleEndianImplicitTransferSyntax;
      _transferSyntaxesCount = 3;

    }
    else if ( _transferSyntax == gkg::DicomServiceClassUser::Implicit )
    {

      // we only support Little Endian Implicit
      _transferSyntaxes[ 0 ]  = UID_LittleEndianImplicitTransferSyntax;
      _transferSyntaxesCount = 1;

    }
    else if ( _transferSyntax == gkg::DicomServiceClassUser::Little )
    {

      // we prefer Little Endian Explicit
      _transferSyntaxes[ 0 ] = UID_LittleEndianExplicitTransferSyntax;
      _transferSyntaxes[ 1 ] = UID_BigEndianExplicitTransferSyntax;
      _transferSyntaxes[ 2 ]  = UID_LittleEndianImplicitTransferSyntax;
      _transferSyntaxesCount = 3;

    }
    else if ( _transferSyntax == gkg::DicomServiceClassUser::Big )
    {

      // we prefer Big Endian Explicit
      _transferSyntaxes[ 0 ] = UID_BigEndianExplicitTransferSyntax;
      _transferSyntaxes[ 1 ] = UID_LittleEndianExplicitTransferSyntax;
      _transferSyntaxes[ 2 ]  = UID_LittleEndianImplicitTransferSyntax;
      _transferSyntaxesCount = 3;

    }
    else if ( _transferSyntax == gkg::DicomServiceClassUser::Lossless )
    {

      // we prefer JPEGLossless:Hierarchical-1stOrderPrediction 
      // (default lossless)
      _transferSyntaxes[ 0 ] = UID_JPEGProcess14SV1TransferSyntax;
      _transferSyntaxes[ 1 ] = UID_LittleEndianExplicitTransferSyntax;
      _transferSyntaxes[ 2 ] = UID_BigEndianExplicitTransferSyntax;
      _transferSyntaxes[ 3 ] = UID_LittleEndianImplicitTransferSyntax;
      _transferSyntaxesCount = 4;

    }
    else if ( _transferSyntax == gkg::DicomServiceClassUser::Jpeg8 )
    {

      // we prefer JPEGBaseline (default lossy for 8 bit images)
      _transferSyntaxes[ 0 ] = UID_JPEGProcess1TransferSyntax;
      _transferSyntaxes[ 1 ] = UID_LittleEndianExplicitTransferSyntax;
      _transferSyntaxes[ 2 ] = UID_BigEndianExplicitTransferSyntax;
      _transferSyntaxes[ 3 ] = UID_LittleEndianImplicitTransferSyntax;
      _transferSyntaxesCount = 4;

    }
    else if ( _transferSyntax == gkg::DicomServiceClassUser::Jpeg12 )
    {

      // we prefer JPEGExtended (default lossy for 12 bit images)
      _transferSyntaxes[ 0 ] = UID_JPEGProcess2_4TransferSyntax;
      _transferSyntaxes[ 1 ] = UID_LittleEndianExplicitTransferSyntax;
      _transferSyntaxes[ 2 ] = UID_BigEndianExplicitTransferSyntax;
      _transferSyntaxes[ 3 ] = UID_LittleEndianImplicitTransferSyntax;
      _transferSyntaxesCount = 4;

    }
    else if ( _transferSyntax == gkg::DicomServiceClassUser::J2kLossless )
    {

      // we prefer JPEG2000 Lossless
      _transferSyntaxes[ 0 ] = UID_JPEG2000LosslessOnlyTransferSyntax;
      _transferSyntaxes[ 1 ] = UID_LittleEndianExplicitTransferSyntax;
      _transferSyntaxes[ 2 ] = UID_BigEndianExplicitTransferSyntax;
      _transferSyntaxes[ 3 ] = UID_LittleEndianImplicitTransferSyntax;
      _transferSyntaxesCount = 4;

    }
    else if ( _transferSyntax == gkg::DicomServiceClassUser::J2kLossy )
    {

      // we prefer JPEG2000 Lossy
      _transferSyntaxes[ 0 ] = UID_JPEG2000TransferSyntax;
      _transferSyntaxes[ 1 ] = UID_LittleEndianExplicitTransferSyntax;
      _transferSyntaxes[ 2 ] = UID_BigEndianExplicitTransferSyntax;
      _transferSyntaxes[ 3 ] = UID_LittleEndianImplicitTransferSyntax;
      _transferSyntaxesCount = 4;

    }
#if OFFIS_DCMTK_VERSION_NUMBER >= 354

    else if ( _transferSyntax == gkg::DicomServiceClassUser::Rle )
    {

      // we prefer RLE Lossless
      _transferSyntaxes[ 0 ] = UID_RLELosslessTransferSyntax;
      _transferSyntaxes[ 1 ] = UID_LittleEndianExplicitTransferSyntax;
      _transferSyntaxes[ 2 ] = UID_BigEndianExplicitTransferSyntax;
      _transferSyntaxes[ 3 ] = UID_LittleEndianImplicitTransferSyntax;
      _transferSyntaxesCount = 4;

    }

#endif
    else if ( _transferSyntax == gkg::DicomServiceClassUser::Deflated )
    {

      // we prefer Deflated Explicit VR Little Endian
      _transferSyntaxes[ 0 ] = UID_DeflatedExplicitVRLittleEndianTransferSyntax;
      _transferSyntaxes[ 1 ] = UID_LittleEndianExplicitTransferSyntax;
      _transferSyntaxes[ 2 ] = UID_BigEndianExplicitTransferSyntax;
      _transferSyntaxes[ 3 ] = UID_LittleEndianImplicitTransferSyntax;
      _transferSyntaxesCount = 4;

    }
    
  }
  GKG_CATCH( "void "
             "gkg::DcmtkDicomSCUImplementation::initializeTransferSyntaxes()" );

}


OFCondition 
gkg::DcmtkDicomSCUImplementation::initializeAssociation( bool verbose )
{

  try
  {
  
    // initialize association parameters
    OFCondition condition = ASC_createAssociationParameters( 
                                                        &_associationParameters,
			                                ASC_DEFAULTMAXPDU );
    if ( condition.bad() )
    {
    
      if ( verbose )
      {
      
        DimseCondition::dump( condition );
      
      }
      
      return condition;
    
    }
    
    // set this application's title and the called application's title in
    // the parameter structure
    condition = ASC_setAPTitles( _associationParameters, 
                                 _callingTitle.c_str(), 
				 _calledTitle.c_str(), 
				 NULL );
    if ( condition.bad() )
    {
    
      if ( verbose )
      {
      
        DimseCondition::dump( condition );
      
      }
      
      return condition;
    
    }
    
    // set the transport layer type (i.e. type of network connection) in
    // the parameter structure
    condition = ASC_setTransportLayerType( _associationParameters, OFFalse );
    if ( condition.bad() )
    {
    
      if ( verbose )
      {
      
        DimseCondition::dump( condition );
      
      }
      
      return condition;
    
    }
    
    // figure out the presentation addresses and copy the corresponding
    // values into the association parameters
    DIC_NODENAME localHost, peerHost;
    gethostname( localHost, sizeof( localHost ) - 1 );
    sprintf( peerHost, "%s:%d", _peerHostName.c_str(), _port );
    condition = ASC_setPresentationAddresses( _associationParameters, 
                                              localHost, 
					      peerHost );
    if ( condition.bad() )
    {
    
      if ( verbose )
      {
      
        DimseCondition::dump( condition );
      
      }
          
    }
    
    return condition;
  
  }
  GKG_CATCH( "OFCondition "
             "gkg::DcmtkDicomSCUImplementation::initializeAssociation( "
	     "bool verbose )" );

}


bool gkg::DcmtkDicomSCUImplementation::requestAssociation( bool verbose )
{

  try
  {
  
    // request association
    OFCondition condition = ASC_requestAssociation( _associationNetwork,
                                                    _associationParameters,
						    &_association );
    if ( condition.bad() )
    {
    
      if ( verbose )
      {
      
        if ( condition == DUL_ASSOCIATIONREJECTED )
	{
	
	  T_ASC_RejectParameters rejectParameters;
	  ASC_getRejectParameters( _associationParameters, &rejectParameters );
	  std::cout << "association rejected" << std::endl;
	  ASC_printRejectParameters( stderr, &rejectParameters );
	
	}
	else
	{
      
          std::cout << "association request failed" << std::endl;
          DimseCondition::dump( condition );
	  
	}
      
      }
    
      return false;    

    }
    
    if ( verbose )
    {
    
      std::cout << "association parameters negotiated" << std::endl;
      ASC_dumpParameters( _associationParameters, COUT );
    
    }
    
    // count the presentation contexts which have been accepted by the
    // Service Class Provider.
    // If there are none, finish the execution
    if ( ASC_countAcceptedPresentationContexts( _associationParameters ) == 0 )
    {
    
      if ( verbose )
      {
    
        std::cout << "no acceptable presentation contexts" << std::endl;

      }
      
      return false;
    
    }
    
    return true;
  
  }
  GKG_CATCH( "OFCondition "
             "gkg::DcmtkDicomSCUImplementation::requestAssociation( "
	     "bool verbose )" );

}


bool gkg::DcmtkDicomSCUImplementation::releaseAssociation( OFCondition cond,
                                                           bool verbose )
{

  try
  {
  
    OFCondition condition = EC_Normal;

    if ( cond == EC_Normal )
    {
  
      condition = ASC_releaseAssociation( _association );
      if ( condition.bad() )
      {
    
        if ( verbose )
        {
      
          std::cout << "association release failed" << std::endl;
	  DimseCondition::dump( condition );
      
        }
      
        return false;
     
      }
  
    }
    else if ( cond == DUL_PEERREQUESTEDRELEASE )
    {
  
      condition = ASC_abortAssociation( _association );
      if ( condition.bad() )
      {
    
        if ( verbose )
        {
      
          std::cout << "association abort failed" << std::endl;
	  DimseCondition::dump( condition );
      
        }
      
        return false;
    
      }
  
    }
    else if ( cond == DUL_PEERABORTEDASSOCIATION )
    {
  
      if ( verbose )
      {
    
        std::cout << "peer aborted association" << std::endl;
    
      }
  
    }
    else
    {
  
      if ( verbose )
      {
    
        std::cout << "Dicom Service Class User failed" << std::endl;
        DimseCondition::dump( condition );
    
      }
    
      condition = ASC_abortAssociation( _association );
      if ( condition.bad() )
      {
    
        if ( verbose )
        {
      
          std::cout << "association abort failed" << std::endl;
  	  DimseCondition::dump( condition );
      
        }
      
        return false;
    
      }
  
    }
  
    condition = ASC_destroyAssociation( &_association );
    if ( condition.bad() )
    {
  
      if ( verbose )
      {
    
        std::cout << "association destroy failed" << std::endl;
        DimseCondition::dump( condition );
    
      }
    
      return false;
  
    }

    return true;
    
  }
  GKG_CATCH( "bool gkg::DcmtkDicomSCUImplementation::releaseAssociation( "
             "OFCondition cond, bool verbose )" );

}
