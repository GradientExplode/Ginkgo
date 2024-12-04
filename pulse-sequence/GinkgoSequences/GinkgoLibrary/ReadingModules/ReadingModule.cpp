#include <ReadingModules/ReadingModule.h>


gkg::ReadingModule::ReadingModule(
                      const gkg::ReadingModule::Type& type,
                      gkg::RCPointer< gkg::ExcitationModule > excitationModule )
                   : gkg::SequenceModule(),
                     _type( type ),
                     _excitationModule( excitationModule ),
                     _defaultTE( 0 ),
                     _defaultBWPerPixel( 0 ),
                     _defaultBaseResolution( 0 ),
                     _grappaFlag( false )
{
}


gkg::ReadingModule::~ReadingModule()
{
}

void gkg::ReadingModule::initialize( SeqLim& seqLim )
{
  
  seqLim.setTE( 0, 
                100,  
                10000000,
                100,
                _defaultTE );
                
  seqLim.setBandWidthPerPixel( 0,
                               80,
                               10000,
                               10,
                               _defaultBWPerPixel );
                               
  seqLim.setBaseResolution( 32,
                            512,
                            SEQ::INC_16,
                            _defaultBaseResolution );
                            
  seqLim.setContrasts( 1,
                       1,
                       0,
                       1 );
                       
  seqLim.setPhasePartialFourierFactor( SEQ::PF_OFF,
                                       SEQ::PF_7_8,
                                       SEQ::PF_6_8,
                                       SEQ::PF_5_8 );
                                       
  //~ seqLim.setPATMode( SEQ::PAT_MODE_NONE, 
                     //~ SEQ::PAT_MODE_GRAPPA );
    
}

NLSStatus gkg::ReadingModule::prepare( MrProt& mrProt,
                                       SeqLim& seqLim,
                                       MrProtocolData::SeqExpo& seqExpo )
{

  static const char *ptModule = { "gkg::ReadingModule::prepare" };
  
  NLS_STATUS status = SEQU__NORMAL;
  
  //////////////////////////////////////////////////////////////////////////////
  // calling the method of the mother class to initialize maximum gradient
  // magnitude and minimum rise time
  //////////////////////////////////////////////////////////////////////////////

  this->gkg::SequenceModule::prepare( mrProt, 
                                      seqLim, 
                                      seqExpo );
    
  return status;


}

void 
gkg::ReadingModule::setGrappaFlag( bool grappaFlag )
{

  _grappaFlag = grappaFlag;

}


bool gkg::ReadingModule::getGrappaFlag()
{
  
  return _grappaFlag;

}

gkg::ReadingModule::Type gkg::ReadingModule::getType() const
{

  return _type;

}


const gkg::RCPointer< gkg::ExcitationModule >&
gkg::ReadingModule::getExcitationModule() const
{

  return _excitationModule;

}

