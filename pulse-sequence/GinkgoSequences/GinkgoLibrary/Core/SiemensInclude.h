#ifndef _gkg_pulse_sequence_GinkgoLibrary_SiemensInclude_h_
#define _gkg_pulse_sequence_GinkgoLibrary_SiemensInclude_h_


// For GinkgoSpinEcho2d.h
#include "MrServers/MrImaging/libSBB/StdSeqIF.h"
#include "MrServers/MrImaging/libSeqUtil/libSeqUtil.h"
#include "MrServers/MrMeasSrv/SeqFW/libGSL/libGSL.h"
#include "MrServers/MrMeasSrv/SeqFW/libSSL/libSSL.h"
#include "MrServers/MrImaging/libSBB/libSBBmsg.h"
#include "MrServers/MrProtSrv/MrProt/KSpace/MrKSpace.h"
#include "MrServers/MrProtSrv/MrProt/MeasParameter/MrSysSpec.h"
#include "MrServers/MrProtSrv/MrProt/MeasParameter/MrRXSpec.h"
#include "MrServers/MrProtSrv/MrProt/MrProt.h"
#include "ProtBasic/Interfaces/SeqLim.h"
#include "MrServers/MrProtSrv/MrProt/SeqIF/SeqExpo.h"
#include "MrServers/MrMeasSrv/SeqIF/libRT/sREADOUT.h"
#include "MrServers/MrMeasSrv/SeqIF/libRT/sGRAD_PULSE.h"
#include "MrServers/MrMeasSrv/SeqIF/libRT/sRF_PULSE.h"
#include "MrServers/MrMeasSrv/SeqIF/libRT/sFREQ_PHASE.h"
#include "MrCommon/MrGlobalDefinitions/MrResult.h"
#include "MrServers/MrMeasSrv/SeqIF/Sequence/sequmsg.h"
#include "MrServers/MrProtSrv/MrProt/MrSliceGroup.h"
#include "MrServers/MrProtSrv/MrProt/MrProtSliceSeries.h"

// iPAT
#include  "MrServers/MrImaging/seq/common/iPAT/iPAT.h"

#ifdef WIN32

#include "TCHAR.h"

#endif

// For text file parameters
#include "MrServers/MrImaging/libINIAccess/INIAccess.h"
#include "MrServers/MrVista/Ice/IceApplicationFunctors/IceTMapping/IniFile.h"

// For GinkgoSpinEcho2d.cpp
#include "MrServers/MrMeasSrv/SeqIF/csequence.h"
#include "MrServers/MrImaging/libSeqSysProp/SysProperties.h"
#include "MrServers/MrImaging/ut/libsequt.h"     

// For GinkgoSpinEcho2d_UI.h
#include "MrServers/MrProtSrv/MrProtocol/libUICtrl/UICtrl.h"
#include "MrServers/MrImaging/seq/common/WIPParameterTool/WIPParameterTool.h"
#include "MrServers/MrImaging/libKSpace/KSpaceException.h"
#include "MrServers/MrImaging/libMath/MrException.h"

#ifdef WIN32

#include "MrServers/MrProtSrv/MrProtocol/libUILink/UILinkLimited.h"
#include "MrServers/MrProtSrv/MrProtocol/libUILink/UILinkSelection.h"

#endif

// For GinkgoSpinEcho2d_UI.cpp
#include "MrServers/MrMeasSrv/SeqIF/Sequence/Sequence.h"

#ifdef WIN32

#include "MrServers/MrProtSrv/MrProtocol/UILink/MrStdNameTags.h"

#endif


#endif
