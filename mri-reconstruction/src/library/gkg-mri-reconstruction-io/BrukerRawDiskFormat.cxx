#include <gkg-mri-reconstruction-io/BrukerRawDiskFormat.h>
#include <gkg-core-io/DiskFormatFactory_i.h>
#include <gkg-core-io/TypedDiskFormat_i.h>
#include <gkg-core-io/ByteOrder.h>
#include <gkg-core-io/IOMode.h>
#include <gkg-core-io/DefaultItemIOFactory_i.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-core-object/RegionInformation.h>
#include <gkg-processing-container/BoundingBox.h>
#include <gkg-core-io/TypeOf.h>
#include <gkg-core-io/StringConverter.h>
#include <gkg-processing-algobase/PowerOfTwo.h>
#include <gkg-core-cppext/LargeFileStream_i.h>
#include <gkg-core-exception/FileNotFoundException.h>
#include <gkg-core-exception/Exception.h>
#include <gkg-core-exception/ItemTypeException.h>
#include <complex>
#include <vector>
#include <fstream>


//#define BRUKER_DEBUG


//
// Bruker parameters
//


namespace gkg
{


enum BrukerParameterType
{

  BRUKER_INTEGER = 0,
  BRUKER_DOUBLE,
  BRUKER_STRING,
  BRUKER_ENUM,
  BRUKER_STRUCTURE,

  BRUKER_ARRAY_INTEGER,
  BRUKER_ARRAY_DOUBLE,
  BRUKER_ARRAY_STRING,
  BRUKER_ARRAY_ENUM,
  BRUKER_ARRAY_STRUCTURE,

  BRUKER_UNKNOWN,

};


struct BrukerParameter
{

  const char* name;
  BrukerParameterType type;

};


}


static gkg::BrukerParameter _acqpParameterDictionary[] =
{

  /* basic ACQP parameters */
  { "ACQ_experiment_mode", gkg::BRUKER_ENUM },
  { "ACQ_n_experiments", gkg::BRUKER_INTEGER },
  { "ACQ_experiments", gkg::BRUKER_ARRAY_STRING },   // not sure it is a string array
  { "NUC1", gkg::BRUKER_STRING },
  { "NUC2", gkg::BRUKER_STRING },
  { "NUC3", gkg::BRUKER_STRING },
  { "NUC4", gkg::BRUKER_STRING },
  { "NUC5", gkg::BRUKER_STRING },
  { "NUC6", gkg::BRUKER_STRING },
  { "NUC7", gkg::BRUKER_STRING },
  { "NUC8", gkg::BRUKER_STRING },
  { "ACQ_user_filter", gkg::BRUKER_ENUM },
  { "ACQ_user_filter_name", gkg::BRUKER_STRING },
  { "ACQ_user_filter_memory", gkg::BRUKER_ENUM },
  { "ACQ_user_filter_mode", gkg::BRUKER_ENUM },
  { "ACQ_user_filter_size", gkg::BRUKER_INTEGER },
  { "ACQ_user_filter_setup_size", gkg::BRUKER_INTEGER },
  { "ACQ_user_filter_setup_name", gkg::BRUKER_STRING },
  { "ACQ_DS_enabled", gkg::BRUKER_ENUM },
  { "ACQ_dim", gkg::BRUKER_INTEGER },
  { "ACQ_dim_desc", gkg::BRUKER_ARRAY_ENUM },
  { "ACQ_size", gkg::BRUKER_ARRAY_INTEGER },
  { "ACQ_ns_list_size", gkg::BRUKER_INTEGER },
  { "ACQ_ns", gkg::BRUKER_INTEGER },
  { "ACQ_ns_list", gkg::BRUKER_ARRAY_INTEGER },
  { "ACQ_phase_factor", gkg::BRUKER_INTEGER },
  { "ACQ_scan_size", gkg::BRUKER_ENUM },
  { "NI", gkg::BRUKER_INTEGER },
  { "NA", gkg::BRUKER_INTEGER },
  { "NAE", gkg::BRUKER_INTEGER },
  { "NR", gkg::BRUKER_INTEGER },
  { "DS", gkg::BRUKER_INTEGER },
  /* ACQP parameters to control the Pulse Program */
  { "PULPROG", gkg::BRUKER_STRING },
  { "L", gkg::BRUKER_ARRAY_INTEGER },
  { "D",gkg::BRUKER_ARRAY_DOUBLE },
  { "P",gkg::BRUKER_ARRAY_DOUBLE },
  { "DE", gkg::BRUKER_DOUBLE },
  { "IN",gkg::BRUKER_ARRAY_DOUBLE },
  { "INP",gkg::BRUKER_ARRAY_DOUBLE },
  { "ACQ_vd_list_size", gkg::BRUKER_INTEGER },
  { "ACQ_vd_list", gkg::BRUKER_ARRAY_DOUBLE },
  { "TPQQ", gkg::BRUKER_ARRAY_STRUCTURE },
  { "DPQQ", gkg::BRUKER_ARRAY_STRUCTURE },
  { "DBPQQ", gkg::BRUKER_ARRAY_STRUCTURE },
  /* ACQP parameters to control the Spectrometer settings */
  { "AQ_mod", gkg::BRUKER_ENUM },
  { "SW", gkg::BRUKER_DOUBLE },
  { "SW_h", gkg::BRUKER_DOUBLE },
  { "FW", gkg::BRUKER_DOUBLE },
  { "ACQ_BF_enable", gkg::BRUKER_ENUM },
  { "BF1", gkg::BRUKER_DOUBLE },
  { "BF2", gkg::BRUKER_DOUBLE },
  { "BF3", gkg::BRUKER_DOUBLE },
  { "BF4", gkg::BRUKER_DOUBLE },
  { "BF5", gkg::BRUKER_DOUBLE },
  { "BF6", gkg::BRUKER_DOUBLE },
  { "BF7", gkg::BRUKER_DOUBLE },
  { "BF8", gkg::BRUKER_DOUBLE },
  { "01", gkg::BRUKER_DOUBLE },
  { "02", gkg::BRUKER_DOUBLE },
  { "03", gkg::BRUKER_DOUBLE },
  { "04", gkg::BRUKER_DOUBLE },
  { "05", gkg::BRUKER_DOUBLE },
  { "06", gkg::BRUKER_DOUBLE },
  { "07", gkg::BRUKER_DOUBLE },
  { "O8", gkg::BRUKER_DOUBLE },
  { "SFO1", gkg::BRUKER_DOUBLE },
  { "SFO2", gkg::BRUKER_DOUBLE },
  { "SFO34", gkg::BRUKER_DOUBLE },
  { "SFO4", gkg::BRUKER_DOUBLE },
  { "SFO5", gkg::BRUKER_DOUBLE },
  { "SFO6", gkg::BRUKER_DOUBLE },
  { "SFO7", gkg::BRUKER_DOUBLE },
  { "SFO8", gkg::BRUKER_DOUBLE },
  { "ACQ_O1_mode", gkg::BRUKER_ENUM },
  { "ACQ_O1_list_size", gkg::BRUKER_INTEGER },
  { "ACQ_O1_list", gkg::BRUKER_ARRAY_DOUBLE },
  { "ACQ_O2_mode", gkg::BRUKER_ENUM },
  { "ACQ_O2_list_size", gkg::BRUKER_INTEGER },
  { "ACQ_O2_list", gkg::BRUKER_ARRAY_DOUBLE },
  { "ACQ_O3_mode", gkg::BRUKER_ENUM },
  { "ACQ_O3_list_size", gkg::BRUKER_INTEGER },
  { "ACQ_O3_list", gkg::BRUKER_ARRAY_DOUBLE },
  { "DR", gkg::BRUKER_INTEGER },
  { "RG", gkg::BRUKER_DOUBLE },
  { "PH_ref", gkg::BRUKER_DOUBLE },
  { "Coil_operation", gkg::BRUKER_ENUM },
  /* ACQ_INFO parameter */
  { "ACQ_status", gkg::BRUKER_STRING },
  { "ACQ_scan_type", gkg::BRUKER_STRING },
  { "ACQ_protocol_name", gkg::BRUKER_STRING },
  { "ACQ_method", gkg::BRUKER_STRING },
  { "ACQ_completed", gkg::BRUKER_ENUM },
  { "ACQ_scans_completed", gkg::BRUKER_INTEGER },
  { "ACQ_nr_completed", gkg::BRUKER_INTEGER },
  { "ACQ_total_completed", gkg::BRUKER_INTEGER },
  { "NECHOES", gkg::BRUKER_INTEGER },
  { "ACQ_fov", gkg::BRUKER_ARRAY_DOUBLE },
  { "ACQ_read_ext", gkg::BRUKER_INTEGER },
  { "ACQ_slice_orient", gkg::BRUKER_ENUM },
  { "ACQ_slice_angle", gkg::BRUKER_ARRAY_DOUBLE },
  { "ACQ_slice_sepn", gkg::BRUKER_ARRAY_DOUBLE },
  { "ACQ_slice_thick", gkg::BRUKER_DOUBLE },
  { "ACQ_phase1_offset", gkg::BRUKER_ARRAY_DOUBLE },
  { "ACQ_phase2_offset", gkg::BRUKER_ARRAY_DOUBLE },
  { "ACQ_slice_offset", gkg::BRUKER_ARRAY_DOUBLE },
  { "ACQ_obj_order", gkg::BRUKER_ARRAY_INTEGER },
  { "ACQ_flip_angle", gkg::BRUKER_DOUBLE },
  { "ACQ_flipback", gkg::BRUKER_ENUM },
  { "ACQ_echo_time", gkg::BRUKER_ARRAY_DOUBLE },
  { "ACQ_inter_echo_time", gkg::BRUKER_ARRAY_DOUBLE },
  { "ACQ_recov_time", gkg::BRUKER_ARRAY_DOUBLE },
  { "ACQ_repetition_time", gkg::BRUKER_ARRAY_DOUBLE },
  { "ACQ_scan_time", gkg::BRUKER_DOUBLE },
  { "ACQ_inversion_time", gkg::BRUKER_ARRAY_DOUBLE },
  { "ACQ_temporal_delay", gkg::BRUKER_ARRAY_DOUBLE },
  { "ACQ_time", gkg::BRUKER_STRING },
  { "ACQ_operator", gkg::BRUKER_STRING },
  { "ACQ_comment", gkg::BRUKER_STRING },
  { "ACQ_RF_power", gkg::BRUKER_DOUBLE },
  { "ACQ_transmitter_coil", gkg::BRUKER_STRING },
  { "ACQ_receiver_coil", gkg::BRUKER_STRING },
  { "ACQ_contrast_agent", gkg::BRUKER_STRING },
  { "ACQ_contrast", gkg::BRUKER_DOUBLE },
  { "ACQ_trigger_enable", gkg::BRUKER_ENUM },
  { "ACQ_trigger_reference", gkg::BRUKER_DOUBLE },
  { "ACQ_trigger_delay", gkg::BRUKER_DOUBLE },
  { "ACQ_institution", gkg::BRUKER_STRING },
  { "ACQ_station", gkg::BRUKER_STRING },
  { "ACQ_sw_version", gkg::BRUKER_STRING },
  { "ACQ_calib_date", gkg::BRUKER_STRING },
  /* image loop structure */
  { "ACQ_n_echo_images", gkg::BRUKER_INTEGER },
  { "ACQ_n_movie_frames", gkg::BRUKER_INTEGER },
  { "ACQ_echo_descr", gkg::BRUKER_ARRAY_STRING },
  { "ACQ_movie_descr", gkg::BRUKER_ARRAY_STRING },
  /* ACQP parameters describing experiments with volume selection */
  { "ACQ_grad_str_X", gkg::BRUKER_DOUBLE },
  { "ACQ_grad_str_Y", gkg::BRUKER_DOUBLE },
  { "ACQ_grad_str_Z", gkg::BRUKER_DOUBLE },
  { "ACQ_position_X", gkg::BRUKER_DOUBLE },
  { "ACQ_position_Y", gkg::BRUKER_DOUBLE },
  { "ACQ_position_Z", gkg::BRUKER_DOUBLE },
  { "BYTORDA", gkg::BRUKER_ENUM },
  { "INSTRUM", gkg::BRUKER_STRING },
  { "Probehead", gkg::BRUKER_INTEGER },
  /* subclass HIRES parameter */
  { "CPDPROG1", gkg::BRUKER_STRING },
  { "CPDPROG2", gkg::BRUKER_STRING },
  { "CPDPROG3", gkg::BRUKER_STRING },
  { "CPDPROG4", gkg::BRUKER_STRING },
  { "CPDPROG5", gkg::BRUKER_STRING },
  { "CPDPROG6", gkg::BRUKER_STRING },
  { "CPDPROG7", gkg::BRUKER_STRING },
  { "CPDPROG8", gkg::BRUKER_STRING },
  { "FQ1LIST", gkg::BRUKER_STRING },
  { "FQ2LIST", gkg::BRUKER_STRING },
  { "FQ3LIST", gkg::BRUKER_STRING },
  { "FQ4LIST", gkg::BRUKER_STRING },
  { "FQ5LIST", gkg::BRUKER_STRING },
  { "FQ6LIST", gkg::BRUKER_STRING },
  { "FQ7LIST", gkg::BRUKER_STRING },
  { "FQ8LIST", gkg::BRUKER_STRING },
  { "SPNAM0", gkg::BRUKER_STRING },
  { "SPNAM1", gkg::BRUKER_STRING },
  { "SPNAM2", gkg::BRUKER_STRING },
  { "SPNAM3", gkg::BRUKER_STRING },
  { "SPNAM4", gkg::BRUKER_STRING },
  { "SPNAM5", gkg::BRUKER_STRING },
  { "SPNAM6", gkg::BRUKER_STRING },
  { "SPNAM7", gkg::BRUKER_STRING },
  { "SPNAM8", gkg::BRUKER_STRING },
  { "SPNAM9", gkg::BRUKER_STRING },
  { "SPNAM10", gkg::BRUKER_STRING },
  { "SPNAM11", gkg::BRUKER_STRING },
  { "SPNAM12", gkg::BRUKER_STRING },
  { "SPNAM13", gkg::BRUKER_STRING },
  { "SPNAM14", gkg::BRUKER_STRING },
  { "SPNAM15", gkg::BRUKER_STRING },
  { "SP", gkg::BRUKER_ARRAY_DOUBLE },
  { "SPOFFS", gkg::BRUKER_ARRAY_DOUBLE },
  { "HOLDER", gkg::BRUKER_STRING },
  { "HGAIN", gkg::BRUKER_ENUM },
  { "HPPRGAIN", gkg::BRUKER_ENUM },
  { "POWMOD", gkg::BRUKER_ENUM },
  { "PRGAIN", gkg::BRUKER_DOUBLE },
  { "XGAIN", gkg::BRUKER_DOUBLE },
  { "NC", gkg::BRUKER_DOUBLE },
  { "LOCNUC", gkg::BRUKER_STRING },
  { "PHP", gkg::BRUKER_INTEGER },
  { "QNP", gkg::BRUKER_ENUM },
  { "RO", gkg::BRUKER_DOUBLE },
  { "SOLVENT", gkg::BRUKER_STRING },
  { "TE", gkg::BRUKER_DOUBLE },
  { "V9\%", gkg::BRUKER_DOUBLE },
  { "VCLIST", gkg::BRUKER_STRING },
  { "VDLIST", gkg::BRUKER_STRING },
  { "VPLIST", gkg::BRUKER_STRING },
  { "VTLIST", gkg::BRUKER_STRING },
  { "XL", gkg::BRUKER_STRING },
  { "YL", gkg::BRUKER_STRING },
  { "ZL1", gkg::BRUKER_DOUBLE },
  { "ZL2", gkg::BRUKER_DOUBLE },
  { "ZL3", gkg::BRUKER_DOUBLE },
  { "ZL4", gkg::BRUKER_DOUBLE },
  /* subclass AVANCE parameters - parameters to control digitization */
  { "DIGMOD", gkg::BRUKER_ENUM },
  { "DIGTYP", gkg::BRUKER_ENUM },
  { "DQDMODE", gkg::BRUKER_ENUM },
  { "DSPFIRM", gkg::BRUKER_ENUM },
  { "DECIM", gkg::BRUKER_INTEGER },
  { "DSPVS", gkg::BRUKER_INTEGER },
  /* subclass AVANCE parameters - other parameters */
  { "ACQ_scan_shift", gkg::BRUKER_INTEGER },
  { "DEOSC", gkg::BRUKER_DOUBLE },
  { "DE", gkg::BRUKER_DOUBLE },
  /* subclass AVANCE parameters -parameters to control the Routing */
  { "ACQ_Routing", gkg::BRUKER_ENUM },
  { "FCUCHAN", gkg::BRUKER_ARRAY_INTEGER },
  { "RSEL", gkg::BRUKER_ARRAY_INTEGER },
  { "SWIBOX", gkg::BRUKER_ARRAY_INTEGER },
  { "PRECHAN", gkg::BRUKER_ARRAY_INTEGER },
  { "OBSCHAN", gkg::BRUKER_ARRAY_INTEGER },
  { "ACQ_2nd_preamp", gkg::BRUKER_ENUM },
  /* ACQ_trim_values and GRAD_PARS */
  { "GRDPROG", gkg::BRUKER_STRING },
  { "ACQ_n_trim", gkg::BRUKER_INTEGER },
  { "ACQ_trim", gkg::BRUKER_ARRAY_DOUBLE },
  { "ACQ_patient_pos", gkg::BRUKER_ENUM },
  { "ACQ_scaling_read", gkg::BRUKER_DOUBLE },
  { "ACQ_scaling_phase", gkg::BRUKER_DOUBLE },
  { "ACQ_scaling_slice", gkg::BRUKER_DOUBLE },
  { "ACQ_phase_encoding_mode", gkg::BRUKER_ARRAY_ENUM },
  { "ACQ_phase_enc_start", gkg::BRUKER_ARRAY_DOUBLE },
  { "ACQ_spatial_phase_0", gkg::BRUKER_ARRAY_DOUBLE },
  { "ACQ_spatial_phase_1", gkg::BRUKER_ARRAY_DOUBLE },
  { "ACQ_spatial_phase_2", gkg::BRUKER_ARRAY_DOUBLE },
  { "ACQ_rare_factor", gkg::BRUKER_INTEGER },
  { "ACQ_slice_sepn_mode", gkg::BRUKER_ENUM },
  { "NSLICES", gkg::BRUKER_INTEGER },
  { "ACQ_grad_matrix", gkg::BRUKER_ARRAY_DOUBLE },
  /* subclass GS parameters to control adjustment pipelines */
  { "GS_dim", gkg::BRUKER_INTEGER },
  { "GS_disp_update", gkg::BRUKER_ENUM },
  { "GS_online_reco", gkg::BRUKER_ENUM },
  { "GS_reco_display", gkg::BRUKER_ENUM },
  { "GS_pc_mode", gkg::BRUKER_ARRAY_ENUM },
  { "GS_image_type", gkg::BRUKER_ENUM },
  { "GS_typ", gkg::BRUKER_ENUM },
  { "GS_auto_name", gkg::BRUKER_STRING },
  { "GS_info_receiver_offset", gkg::BRUKER_ENUM },
  { "GS_info_dig_filling", gkg::BRUKER_ENUM },
  { "GS_info_normalized_area", gkg::BRUKER_ENUM },
  { "GS_info_max_point", gkg::BRUKER_ENUM },
  { "GS_get_info_points", gkg::BRUKER_ENUM },
  { "GS_info_points", gkg::BRUKER_INTEGER },
  { "GS_info_offset", gkg::BRUKER_ARRAY_INTEGER },
  { "GS_continue", gkg::BRUKER_ENUM },
  { "GS_steady_state", gkg::BRUKER_ENUM },
  /* subclass GO_parameters */
  { "GO_init_files", gkg::BRUKER_ENUM },
  { "GO_data_save", gkg::BRUKER_ENUM },
  { "GO_disp_update", gkg::BRUKER_ENUM },
  { "GO_macro", gkg::BRUKER_STRING },
  { "GO_use_macro", gkg::BRUKER_ENUM },
  { "GO_online_reco", gkg::BRUKER_ENUM },
  { "GO_reco_display", gkg::BRUKER_ENUM },
  { "GO_time_est", gkg::BRUKER_ENUM },
  { "GO_reco_each_nr", gkg::BRUKER_ENUM },
  { 0, gkg::BRUKER_UNKNOWN },

};


static gkg::BrukerParameter _imndParameterDictionary[] =
{

  { "IMND_scan_name", gkg::BRUKER_STRING },
  { "IMND_method", gkg::BRUKER_STRING },
  { "IMND_nucleus", gkg::BRUKER_ENUM },
  { "IMND_matrix_eq", gkg::BRUKER_INTEGER },
  { "IMND_matrix", gkg::BRUKER_ARRAY_INTEGER },
  { "IMND_sw_h", gkg::BRUKER_DOUBLE },
  { "IMND_scout_orient_matrix_curr", gkg::BRUKER_ARRAY_DOUBLE },
  { "IMND_scout_orient_matrix", gkg::BRUKER_ARRAY_DOUBLE },
  { "IMND_recov_time", gkg::BRUKER_DOUBLE },
  { "IMND_echo_time", gkg::BRUKER_DOUBLE },
  { "IMND_pulse_angle", gkg::BRUKER_DOUBLE },
  { "IMND_total_time", gkg::BRUKER_STRING },
  { "IMND_pulse_length", gkg::BRUKER_DOUBLE },
  { "IMND_n_averages", gkg::BRUKER_INTEGER },
  { "IMND_n_echoes", gkg::BRUKER_INTEGER },
  { "IMND_rare_factor", gkg::BRUKER_INTEGER },
  { "IMND_inter_package_delay", gkg::BRUKER_DOUBLE },
  { "IMND_n_interleaves", gkg::BRUKER_INTEGER },
  { "IMND_altint", gkg::BRUKER_ENUM },
  { "IMND_TE_eff", gkg::BRUKER_DOUBLE },
  { "IMND_max_grad", gkg::BRUKER_DOUBLE },
  { "IMND_epi_mode", gkg::BRUKER_ENUM },
  { "IMND_epi_phase_start", gkg::BRUKER_DOUBLE },
  { "IMND_epi_rampsamp", gkg::BRUKER_ENUM },
  { "IMND_epi_corr", gkg::BRUKER_DOUBLE },
  { "IMND_epi_ong", gkg::BRUKER_DOUBLE },
  { "IMND_epi_ets", gkg::BRUKER_ENUM },
  { "IMND_epi_dim3", gkg::BRUKER_INTEGER },
  { "IMND_kspc_calib", gkg::BRUKER_ENUM },
  { "IMND_regrid_mode", gkg::BRUKER_ENUM },
  { "IMND_regrid_traj", gkg::BRUKER_ARRAY_DOUBLE },
  { "IMND_inv_delay_storage", gkg::BRUKER_DOUBLE },
  { "IMND_inv_delay", gkg::BRUKER_DOUBLE },
  { "IMND_patient_pos", gkg::BRUKER_ENUM },
  { "IMND_square_fov_matrix", gkg::BRUKER_ENUM },
  { "IMND_isotropic", gkg::BRUKER_ENUM },
  { "IMND_fov_eq", gkg::BRUKER_DOUBLE },
  { "IMND_fov", gkg::BRUKER_ARRAY_DOUBLE },
  { "IMND_slice_orient", gkg::BRUKER_ENUM },
  { "IMND_n_slices", gkg::BRUKER_INTEGER },
  { "IMND_slice_offset", gkg::BRUKER_DOUBLE },
  { "IMND_slice_sepn_mode", gkg::BRUKER_ENUM },
  { "IMND_slice_sepn", gkg::BRUKER_ARRAY_DOUBLE },
  { "IMND_slice_thick", gkg::BRUKER_DOUBLE },
  { "IMND_read_offset", gkg::BRUKER_ARRAY_DOUBLE },
  { "IMND_read_offset_eq", gkg::BRUKER_DOUBLE },
  { "IMND_slice_scheme", gkg::BRUKER_ENUM },
  { "IMND_n_slicepacks", gkg::BRUKER_INTEGER },
  { "IMND_slicepack_n_slices", gkg::BRUKER_ARRAY_INTEGER },
  { "$IMND_slicepack_vector", gkg::BRUKER_ARRAY_DOUBLE },
  { "IMND_slicepack_position", gkg::BRUKER_ARRAY_DOUBLE },
  { "IMND_slicepack_gap", gkg::BRUKER_ARRAY_DOUBLE },
  { "IMND_read_vector", gkg::BRUKER_ARRAY_DOUBLE },
  { "IMND_trigger_enable", gkg::BRUKER_ENUM },
  { "IMND_fat_mode", gkg::BRUKER_ENUM },
  { "IMND_sat_mode", gkg::BRUKER_ENUM },
  { "IMND_contrast_agent", gkg::BRUKER_ENUM },
  { "IMND_imag_shape_enum", gkg::BRUKER_ENUM },
  { "IMND_imag_shape", gkg::BRUKER_STRING },
  { "IMND_sl_thick_hz", gkg::BRUKER_DOUBLE },
  { "IMND_acq_time", gkg::BRUKER_DOUBLE },
  { "IMND_DW_time", gkg::BRUKER_DOUBLE },
  { "IMND_rep_delay", gkg::BRUKER_DOUBLE },
  { "IMND_bf1", gkg::BRUKER_DOUBLE },
  { "IMND_rcvr_offset_bc", gkg::BRUKER_ENUM },
  { "IMND_isotropic_reco", gkg::BRUKER_ENUM },
  { "IMND_zf", gkg::BRUKER_DOUBLE },
  { "IMND_dscan_time", gkg::BRUKER_DOUBLE },
  { "IMND_dscans", gkg::BRUKER_INTEGER },
  { "IMND_derive_gains", gkg::BRUKER_ENUM },
  { "IMND_reference_gain", gkg::BRUKER_DOUBLE },
  { "IMND_ref_gain_state", gkg::BRUKER_ENUM },
  { "IMND_rg_defined", gkg::BRUKER_ENUM },
  { "FH_nr", gkg::BRUKER_INTEGER },
  { "FH_silent_volumes", gkg::BRUKER_ENUM },
  { "FH_ttl_frq", gkg::BRUKER_ENUM },
  { "FH_ttl_order", gkg::BRUKER_ENUM },
  { "FH_tagging", gkg::BRUKER_ENUM },
  { "EPI_ramp_time", gkg::BRUKER_DOUBLE },
  { "EPI_ramp_gap", gkg::BRUKER_DOUBLE },
  { 0, gkg::BRUKER_UNKNOWN },

};


static gkg::BrukerParameter _methodParameterDictionary[] =
{

  { "Method", gkg::BRUKER_ENUM },
  { "PVM_NRepetitions", gkg::BRUKER_INTEGER },
  { "PVM_RepetitionTime", gkg::BRUKER_DOUBLE },
  { "PVM_ScanTimeStr", gkg::BRUKER_STRING },
  { "PartialFourierAcq", gkg::BRUKER_ENUM },
  { "PE_reco_dim", gkg::BRUKER_INTEGER },
  { "N_overscans", gkg::BRUKER_INTEGER },
  { "ExcPulseEnum", gkg::BRUKER_ENUM },
  { "$ExcPulse", gkg::BRUKER_STRUCTURE },
  { "RefPulseEnum", gkg::BRUKER_ENUM },
  { "RefPulse", gkg::BRUKER_STRUCTURE },
  { "$EchoTime", gkg::BRUKER_DOUBLE },
  { "PVM_EffSWh", gkg::BRUKER_DOUBLE },
  { "PVM_EpiRampMode", gkg::BRUKER_ENUM },
  { "PVM_EpiRampForm", gkg::BRUKER_ENUM },
  { "PVM_EpiNShots", gkg::BRUKER_INTEGER },
  { "PVM_EpiEchoPosition", gkg::BRUKER_DOUBLE },
  { "PVM_EpiRampTime", gkg::BRUKER_DOUBLE },
  { "PVM_EpiSlope", gkg::BRUKER_DOUBLE },
  { "PVM_EpiBlipTime", gkg::BRUKER_DOUBLE },
  { "PVM_EpiSwitchTime", gkg::BRUKER_DOUBLE },
  { "PVM_EpiEchoDelay", gkg::BRUKER_DOUBLE },
  { "PVM_EpiModuleTime", gkg::BRUKER_DOUBLE },
  { "PVM_EpiGradDwellTime", gkg::BRUKER_DOUBLE },
  { "PVM_EpiAcqDelayTrim", gkg::BRUKER_DOUBLE },
  { "PVM_EpiBlipAsym", gkg::BRUKER_DOUBLE },
  { "PVM_EpiReadAsym", gkg::BRUKER_DOUBLE },
  { "PVM_EpiReadDephTrim", gkg::BRUKER_INTEGER },
  { "PVM_EpiEchoTimeShifting", gkg::BRUKER_ENUM },
  { "PVM_EpiEchoShiftA", gkg::BRUKER_ARRAY_DOUBLE },
  { "PVM_EpiEchoShiftB", gkg::BRUKER_ARRAY_DOUBLE },
  { "PVM_TrajectoryMeasurement", gkg::BRUKER_ENUM },
  { "PVM_Trajectory", gkg::BRUKER_ARRAY_DOUBLE },
  { "PVM_PhaseError", gkg::BRUKER_ARRAY_DOUBLE },
  { "PVM_UseTrajectory", gkg::BRUKER_ENUM },
  { "PVM_RefAtt", gkg::BRUKER_ARRAY_STRUCTURE },
  { "PVM_ExSliceRephaseTime", gkg::BRUKER_DOUBLE },
  { "SliceSpoilerDuration", gkg::BRUKER_DOUBLE },
  { "SliceSpoilerStrength", gkg::BRUKER_DOUBLE },
  { "PVM_GeoMode", gkg::BRUKER_ENUM },
  { "PVM_SpatDimEnum", gkg::BRUKER_ENUM },
  { "PVM_Isotropic", gkg::BRUKER_ENUM },
  { "PVM_Fov", gkg::BRUKER_ARRAY_DOUBLE },
  { "PVM_FovCm", gkg::BRUKER_ARRAY_DOUBLE },
  { "PVM_SpatResol", gkg::BRUKER_ARRAY_DOUBLE },
  { "PVM_Matrix", gkg::BRUKER_ARRAY_INTEGER },
  { "PVM_MinMatrix", gkg::BRUKER_ARRAY_INTEGER },
  { "PVM_MaxMatrix", gkg::BRUKER_ARRAY_INTEGER },
  { "PVM_AntiAlias", gkg::BRUKER_ARRAY_DOUBLE },
  { "PVM_MaxAntiAlias", gkg::BRUKER_ARRAY_DOUBLE },
  { "PVM_SliceThick", gkg::BRUKER_DOUBLE },
  { "PVM_ObjOrderScheme", gkg::BRUKER_ENUM },
  { "PVM_ObjOrderList", gkg::BRUKER_ARRAY_INTEGER },
  { "PVM_NSPacks", gkg::BRUKER_INTEGER },
  { "PVM_SPackArrNSlices", gkg::BRUKER_ARRAY_INTEGER },
  { "PVM_SPackArrSliceOrient", gkg::BRUKER_ARRAY_ENUM },
  { "PVM_SPackArrReadOrient", gkg::BRUKER_ARRAY_ENUM },
  { "PVM_SPackArrReadOffset", gkg::BRUKER_ARRAY_DOUBLE },
  { "PVM_SPackArrPhase1Offset", gkg::BRUKER_ARRAY_DOUBLE },
  { "PVM_SPackArrPhase2Offset", gkg::BRUKER_ARRAY_DOUBLE },
  { "PVM_SPackArrSliceOffset", gkg::BRUKER_ARRAY_DOUBLE },
  { "PVM_SPackArrSliceGapMode", gkg::BRUKER_ARRAY_ENUM },
  { "PVM_SPackArrSliceGap", gkg::BRUKER_ARRAY_DOUBLE },
  { "PVM_SPackArrSliceDistance", gkg::BRUKER_ARRAY_DOUBLE },
  { "PVM_SPackArrGradOrient", gkg::BRUKER_ARRAY_DOUBLE },
  { "PackDel", gkg::BRUKER_DOUBLE },
  { "TTL_Order", gkg::BRUKER_ENUM },
  { "N_sil_vol", gkg::BRUKER_INTEGER },
  { "N_noisy_vol", gkg::BRUKER_INTEGER },
  { "Reco_mode", gkg::BRUKER_ENUM },
  { "Reco_macro", gkg::BRUKER_STRING },
  { "PVM_PreemphasisSpecial", gkg::BRUKER_ENUM },
  { "PVM_PreemphasisFileEnum", gkg::BRUKER_ENUM },
  { "NDummyScans", gkg::BRUKER_INTEGER },
  { "PVM_TriggerModule", gkg::BRUKER_ENUM },
  { "PVM_FatSupOnOff", gkg::BRUKER_ENUM },
  { "PVM_MagTransOnOff", gkg::BRUKER_ENUM },
  { "PVM_FovSatOnOff", gkg::BRUKER_ENUM },
  { "PVM_TriggerOutOnOff", gkg::BRUKER_ENUM },
  { "PVM_TriggerOutMode", gkg::BRUKER_ENUM },
  { "PVM_TriggerOutDelay", gkg::BRUKER_DOUBLE },
  { "SignalType", gkg::BRUKER_ENUM },
  { "PVM_DeriveGains", gkg::BRUKER_ENUM },
  { "PVM_NAverages", gkg::BRUKER_INTEGER },
  { "PVM_EchoTime1", gkg::BRUKER_DOUBLE },
  { "PVM_EchoTime", gkg::BRUKER_DOUBLE },
  { "PVM_NEchoImages", gkg::BRUKER_INTEGER },
  { 0, gkg::BRUKER_UNKNOWN },

};


//
// class BrukerRawDiskFormat
//


template < class T >
gkg::BrukerRawDiskFormat< T >::BrukerRawDiskFormat()
                   : gkg::TypedDiskFormat< gkg::Volume< std::complex< T > > >()
{

  // because Bruker raw data stream is a directory, one could give it
  // with a '/' character at the end, so take it as an extension
  this->_extensions.insert( "/" );

}


template < class T >
gkg::BrukerRawDiskFormat< T >::~BrukerRawDiskFormat()
{
}


template < class T >
std::string gkg::BrukerRawDiskFormat< T >::getName() const
{

  return "bruker-raw";

}


template < class T >
void gkg::BrukerRawDiskFormat< T >::readHeader(
                                             const std::string& name,
                                             gkg::HeaderedObject& object ) const
{

  try
  {

    std::ifstream is;

    // checking item type compatibility
    if ( object.getHeader().hasAttribute( "item_type" ) )
    {

      std::string itemType;
      object.getHeader().getAttribute( "item_type", itemType );
      if ( ! gkg::TypeOf< std::complex< T > >::isSameType( itemType ) ) 
      {

        throw gkg::ItemTypeException( this->getNameWithoutExtension( name ) );

      }

    }

    // reading the acqp header
    is.open( ( this->getNameWithoutExtension( name ) + "/acqp" ).c_str() );
    if ( !is )
    {

      throw gkg::FileNotFoundException( this->getNameWithoutExtension( name ) + 
                                        "/acqp" );

    }
    readAcqpHeader( is, object );
    is.close();

    // reading one of imnd or method header
    std::string paravisionVersion;
    is.open( ( this->getNameWithoutExtension( name ) + "/imnd" ).c_str() );
    if ( !is )
    {

      std::ifstream isprime( ( this->getNameWithoutExtension( name ) +
                               "/method" ).c_str() );
      if ( !isprime )
      {

        throw gkg::FileNotFoundException(
                           this->getNameWithoutExtension( name ) + 
                           "/imnd or " +
                           this->getNameWithoutExtension( name ) + 
                           "/method" );

      }
      readMethodHeader( isprime, object );
      isprime.close();

      paravisionVersion = "PV3";
      object.addSemantic( "__bruker_raw__",
                          "paravision_version",
                       gkg::Semantic( gkg::TypeOf< std::string >::getName() ) );
      object.getHeader()[ "paravision_version" ] = paravisionVersion;

    }
    else
    {

      readImndHeader( is, object );
      is.close();

      paravisionVersion = "PV1";
      object.addSemantic( "__bruker_raw__",
                          "paravision_version",
                       gkg::Semantic( gkg::TypeOf< std::string >::getName() ) );
      object.getHeader()[ "paravision_version" ] = paravisionVersion;

    }
    
    // adding size(s)
    int32_t sizeX, sizeY, acquisitionDimension;

    // acquisition dimension
    object.getHeader().getAttribute( "ACQ_dim", acquisitionDimension );
    object.addSemantic( "__bruker_raw__",
                        "acquisition_dimension",
                        gkg::Semantic( gkg::TypeOf< int32_t >::getName() ) );
    object.getHeader().addAttribute( "acquisition_dimension",
                                     acquisitionDimension );

    std::vector< int32_t > size( acquisitionDimension );
    object.getHeader().getAttribute( "ACQ_size", size );

    object.getHeader()[ "sizeX" ] = sizeX = size[ 0 ] / 2;
    object.getHeader()[ "sizeY" ] = sizeY = size[ 1 ];


    // adding lines acquired by echo
    int32_t linesPerEcho;
    object.getHeader().getAttribute( "ACQ_phase_factor", linesPerEcho );
    object.addSemantic( "__bruker_raw__",
                        "lines_per_echo",
                        gkg::Semantic( gkg::TypeOf< int32_t >::getName() ) );
    object.getHeader().addAttribute( "lines_per_echo", linesPerEcho );


    // managing echo count
    int32_t echoCount;
    object.getHeader().getAttribute( "NECHOES", echoCount );

    if ( paravisionVersion == "PV1" )
    {

      echoCount /= linesPerEcho;

    }
    object.addSemantic( "__bruker_raw__",
                        "echo_count",
                        gkg::Semantic( gkg::TypeOf< int32_t >::getName() ) );
    object.getHeader().addAttribute( "echo_count", echoCount );

    if ( acquisitionDimension > 2 )  // 3D acquisition
    {

      object.getHeader()[ "sizeZ" ] = size[ 2 ];

    }
    else  // 2D acquisition
    {

      int32_t imageCount;
      int32_t sliceCount;
      object.getHeader().getAttribute( "NI", imageCount );
      sliceCount = imageCount / echoCount;
      object.getHeader()[ "sizeZ" ] = ( int32_t )sliceCount;

    }

    int32_t repetitionCount;
    object.getHeader().getAttribute( "NR", repetitionCount );
    object.getHeader()[ "sizeT" ] = repetitionCount * echoCount;

    // notifying observers
    object.getHeader().setChanged();
    object.getHeader().notifyObservers();

  }
  GKG_CATCH( "template < class T > "
             "void gkg::BrukerRawDiskFormat< T >::readHeader( "
             "const std::string& name, "
             "gkg::HeaderedObject& object ) const" );

}


template < class T >
void gkg::BrukerRawDiskFormat< T >::read(
                                const std::string& name,
                                gkg::Volume< std::complex< T > >& object ) const
{

  try
  {

    // preparing partial binary read
    gkg::VolumeProxy< std::complex< T > > proxy;
    preparePartialBinaryRead( name, proxy );

    // building bounding box
    gkg::BoundingBox< int32_t > boundingBox( 0, proxy.getSizeX() - 1,
                                             0, proxy.getSizeY() - 1,
                                             0, proxy.getSizeZ() - 1,
                                             0, proxy.getSizeT() - 1 );

    // reading whole volume
    partialBinaryRead( proxy, object, boundingBox );

  }
  GKG_CATCH( "template < class T > "
             "void gkg::BrukerRawDiskFormat< T >::read( "
             "const std::string& name, "
             "gkg::Volume< std::complex< T > >& object ) const" );

}


template < class T >
inline
void gkg::BrukerRawDiskFormat< T >::preparePartialBinaryRead(
                                             const std::string& name,
                                             gkg::HeaderedObject& proxy ) const
{

  try
  {

    if ( &dynamic_cast< gkg::VolumeProxy< std::complex< T > >& >( proxy ) )
    {

      gkg::VolumeProxy< std::complex< T > >& volumeProxy =
        dynamic_cast< gkg::VolumeProxy< std::complex< T > >& >( proxy );

      // reading header(s) acq + imnd or method header
      readHeader( name, volumeProxy );

      // adding proxy file name
      if ( volumeProxy.getHeader().hasAttribute( "proxyFileName" ) )
      {

        volumeProxy.getHeader().removeAttribute( "proxyFileName" );

      }
      volumeProxy.getHeader().addAttribute( "proxyFileName", name );


      // getting byte order
      if ( volumeProxy.getHeader().hasAttribute( "BYTORDA" ) )
      {

        std::string byteOrderName;
        volumeProxy.getHeader().getAttribute( "BYTORDA", byteOrderName );

        gkg::ByteOrder::Type byteOrderType = ( byteOrderName == "little" ?
                                               gkg::ByteOrder::LittleEndian:
                                               gkg::ByteOrder::BigEndian );
        int32_t proxyBSwap = 
          ( gkg::ByteOrder::getInstance().getCurrentType() !=
            byteOrderType ? 1 : 0 );
        volumeProxy.getHeader().addAttribute( "proxyBSwap", proxyBSwap );
      
      }

    }
    else
    {

      throw std::runtime_error( "not a volume proxy" );

    }

  }
  GKG_CATCH( "template < class T > void gkg::BrukerRawDiskFormat< T >::"
             "preparePartialBinaryRead( "
             "const std::string& name, "
             "gkg::HeaderedObject& proxy ) const" );

}


template < class T >
void gkg::BrukerRawDiskFormat< T >::partialBinaryRead(
                         const gkg::HeaderedObject& proxy,
                         gkg::GenericObject& subpart,
                         const gkg::RegionInformation& regionInformation ) const
{

  try
  {

    if ( &dynamic_cast< const gkg::VolumeProxy< std::complex< T > >& >( proxy ) )
    {

      const gkg::VolumeProxy< std::complex< T > >& volumeProxy =
        dynamic_cast< const gkg::VolumeProxy< std::complex< T > >& >( proxy );

      if ( &dynamic_cast< gkg::Volume< std::complex< T > >& >( subpart ) )
      {

        gkg::Volume< std::complex< T > >&
          subVolume = dynamic_cast< gkg::Volume< std::complex< T > >& >(
                                                                subpart );

        if ( &dynamic_cast< const gkg::BoundingBox< int32_t >& >(
                                           regionInformation ) )
        {

          const gkg::BoundingBox< int32_t >& boundingBox = 
            dynamic_cast< const gkg::BoundingBox< int32_t >& >( 
                                                            regionInformation );

          try
          {

            // getting byte swap info and proxy file name
            if ( !volumeProxy.getHeader().hasAttribute( "proxyBSwap" ) )
            {

               throw std::runtime_error( "cannot find 'proxyBSwap' attribute" );

            }
            if ( !volumeProxy.getHeader().hasAttribute( "proxyFileName" ) )
            {

               throw std::runtime_error(
                                      "cannot find 'proxyFileName' attribute" );

            }
            int32_t proxyBSwap = 0;
            volumeProxy.getHeader().getAttribute( "proxyBSwap",
                                                  proxyBSwap );
            std::string proxyFileName;
            volumeProxy.getHeader().getAttribute( "proxyFileName",
                                                  proxyFileName );

            // checking bounding box
            gkg::BoundingBox< int32_t >
              onDiskBoundingBox( 0, volumeProxy.getSizeX() - 1,
                                 0, volumeProxy.getSizeY() - 1,
                                 0, volumeProxy.getSizeZ() - 1,
                                 0, volumeProxy.getSizeT() - 1 );
            if ( !onDiskBoundingBox.contains( boundingBox ) )
            {

              throw std::runtime_error(
                                "bounding box is not contained inside volume" );

            }

            int32_t subSizeX = boundingBox.getUpperX() -
                               boundingBox.getLowerX() + 1;
            int32_t subSizeY = boundingBox.getUpperY() -
                               boundingBox.getLowerY() + 1;
            int32_t subSizeZ = boundingBox.getUpperZ() -
                               boundingBox.getLowerZ() + 1;
            int32_t subSizeT = boundingBox.getUpperT() -
                               boundingBox.getLowerT() + 1;

            // reallocating the sub-part
            subVolume.getHeader() = volumeProxy.getHeader();
            subVolume.reallocate( subSizeX, subSizeY, subSizeZ, subSizeT );

            // reading from core data <*/fid>
            gkg::largefile_ifstream is( proxyFileName + "/fid" );
            if ( !is.isOpened() )
            {

              throw gkg::FileNotFoundException( proxyFileName + "/fid" );

            }

            gkg::ItemReader< int32_t >& itemR =
              gkg::DefaultItemIOFactory< int32_t >::getInstance().getReader(
                                         false, ( proxyBSwap ? true : false ) );
            readPartialBinaryCore( is,
                                   itemR,
                                   volumeProxy,
                                   subVolume,
                                   boundingBox );
            is.close();

          }
          catch ( std::exception& e )
          {

            throw std::runtime_error( std::string( "volume sub-part: " ) +
                                      e.what() );

          }

        }

      }
      else
      {

        throw std::runtime_error( "not a valid sub-part" );

      }

    }
    else
    {

      throw std::runtime_error( "not a volume proxy" );

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::BrukerRawDiskFormat< T >::partialBinaryRead( "
             "const gkg::HeaderedObject& proxy, "
             "gkg::GenericObject& subpart, "
             "const gkg::RegionInformation& regionInformation ) const" );

}


template < class T >
bool gkg::BrukerRawDiskFormat< T >::hasReader() const
{

  return true;

}


template < class T >
void gkg::BrukerRawDiskFormat< T >::readPartialBinaryCore(
                       gkg::largefile_ifstream& is,
                       const gkg::ItemReader< int32_t >& itemR,
                       const gkg::VolumeProxy< std::complex< T > >& volumeProxy,
                       gkg::Volume< std::complex< T > >& subVolume,
                       const gkg::BoundingBox< int32_t >& boundingBox ) const
{

  try
  {

    typename gkg::Volume< std::complex< T > >::iterator s = subVolume.begin();
    int32_t x, y, z, t;
    int32_t subSizeX = subVolume.getSizeX();
    int32_t subSizeY = subVolume.getSizeY();
    int32_t subSizeZ = subVolume.getSizeZ();
    int32_t subSizeT = subVolume.getSizeT();

    int32_t onDiskSizeX = volumeProxy.getSizeX();
    int32_t onDiskSizeY = volumeProxy.getSizeY();
    int32_t onDiskSizeZ = volumeProxy.getSizeZ();

    int32_t bboxLowerX = boundingBox.getLowerX();
    int32_t bboxLowerY = boundingBox.getLowerY();
    int32_t bboxLowerZ = boundingBox.getLowerZ();
    int32_t bboxLowerT = boundingBox.getLowerT();

    // collecting acquisition dimension (2D or 3D)
    int32_t acquisitionDimension;
    volumeProxy.getHeader().getAttribute( "acquisition_dimension", 
                                          acquisitionDimension );

    // collecting echo count
    int32_t echoCount;
    volumeProxy.getHeader().getAttribute( "echo_count", echoCount );

    // collecting slice order
    std::vector< int32_t > sliceOrder( onDiskSizeZ );
    if ( acquisitionDimension == 2 )
    {
  
      // need for slice reversal
      std::vector< double > sliceOffset( onDiskSizeZ );
      volumeProxy.getHeader().getAttribute( "ACQ_slice_offset", sliceOffset );
      bool reverse = ( sliceOffset[ onDiskSizeZ - 1 ] > sliceOffset[ 0 ] );
                     
      // carreful: here we loop through the destination volume, so we
      // must reverse the slice order
      std::vector< int32_t > objOrder( onDiskSizeZ * echoCount );
      volumeProxy.getHeader().getAttribute( "ACQ_obj_order", objOrder );
      for ( z = 0; z < onDiskSizeZ; z++ )
      {

        if ( reverse )
        {

          sliceOrder[ objOrder[ z * echoCount ] / echoCount ] =
                                                            onDiskSizeZ - 1 - z;

        }
        else
        {

          sliceOrder[ objOrder[ z * echoCount ] / echoCount ] = z;

        }

      }

    }
    else
    {

      for ( z = 0; z < onDiskSizeZ; z++ )
      {

        sliceOrder[ z ] = z;

      }

    }

    int64_t rowLength = onDiskSizeX;

    // Bruker's line were at least 1kBytes long in PV1 !!! Clumsy !
    // collecting echo count
    std::string paravisionVersion;
    volumeProxy.getHeader().getAttribute( "paravision_version",
                                          paravisionVersion );
    if ( paravisionVersion == "PV1" )
    {

      rowLength = ( int64_t )( std::ceil( onDiskSizeX / 128.0 ) + 0.5 ) * 128;

    }

    // collecting line count acquired per signal echo
    int32_t linesPerEcho;
    volumeProxy.getHeader().getAttribute( "lines_per_echo", linesPerEcho );

    // reading data: for a given (y,z,t) coordinate, the given line to be read
    // is given by the following equation:
    // line = ( y % linesPerEcho ) + sliceOrder[ z ] . linesPerEcho +
    //        linesPerEcho . sizeZ . ( y % ( sizeY / linesPerEcho ) ) +
    //        t . ( sizeY . sizeZ )
    std::vector< int32_t > bufferS32( 2 * rowLength );
    int32_t zprime, tprime;

    int32_t sizeZprime = onDiskSizeZ * echoCount;
    int64_t line;

    if ( acquisitionDimension == 3 )
    {

      for ( t = 0; t < subSizeT; t++ )
      {

        for ( z = 0; z < subSizeZ; z++ )
        {

          for ( y = 0; y < subSizeY; y++ )
          {

            line = ( int64_t )( bboxLowerT + t ) +
                   ( int64_t )y * ( int64_t )echoCount +
                   ( int64_t )z * ( int64_t )echoCount * ( int64_t )onDiskSizeY;
            is.seek( ( gkg::largefile_ifstream::off_type )(
                       ( line * rowLength ) *
                       ( int64_t )sizeof( int32_t ) * 2 ),
                     gkg::IosBase::beg );
            itemR.read( is, &bufferS32[ 0 ], 2 * rowLength );
            for ( x = 0; x < subSizeX; x++ )
            {

              *s = std::complex< T >( ( T )bufferS32[ 2 * x ],
                                    ( T )bufferS32[ 2 * x + 1 ] );
              ++ s;

            }

          }

        }

      }

    }
    else
    {

      for ( t = 0; t < subSizeT; t++ )
      {

        for ( z = 0; z < subSizeZ; z++ )
        {

          for ( y = 0; y < subSizeY; y++ )
          {

            zprime = sliceOrder[ z + bboxLowerZ ] * echoCount +
                     ( ( t + bboxLowerT ) % echoCount );
            tprime = ( t + bboxLowerT ) / echoCount;

            line = ( ( int64_t )( y + bboxLowerY ) %
                     ( int64_t )linesPerEcho ) +
                   ( int64_t )zprime * ( int64_t )linesPerEcho +
                   ( int64_t )( linesPerEcho * sizeZprime ) *
                   ( ( int64_t )( y + bboxLowerY ) %
                     ( int64_t )( onDiskSizeY / linesPerEcho ) ) +
                   ( int64_t )tprime * ( int64_t )onDiskSizeY *
                   ( int64_t )sizeZprime;

            is.seek( ( gkg::largefile_ifstream::off_type )( (
                      ( int64_t )bboxLowerX +
                      line * rowLength ) * ( int64_t )sizeof( int32_t ) * 2 ),
                      gkg::IosBase::beg );
            itemR.read( is, &bufferS32[ 0 ], 2 * ( size_t )rowLength );
            for ( x = 0; x < subSizeX; x++ )
            {

              *s = std::complex< T >( ( T )bufferS32[ 2 * x ],
                                      ( T )bufferS32[ 2 * x + 1 ] );
              ++ s;

            }

          }

        }

      }

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::BrukerRawDiskFormat< T >::readPartialBinaryCore( "
             "gkg::largefile_ifstream& is, "
             "const gkg::ItemReader< int >& itemR, "
             "const gkg::VolumeProxy< std::complex< T > >& volumeProxy, "
             "gkg::Volume< std::complex< T > >& subVolume, "
             "const gkg::BoundingBox< int >& boundingBox ) const" );

}


template < class T >
void gkg::BrukerRawDiskFormat< T >::readAcqpHeader(
                                             std::istream& is, 
                                             gkg::HeaderedObject& object ) const
{

  try
  {

    gkg::BrukerParameter* parameters = _acqpParameterDictionary;
    while ( parameters->name != 0 )
    {

      switch ( parameters->type )
      {

        case gkg::BRUKER_INTEGER:

          readInteger( is, parameters->name, object );
          break;

        case gkg::BRUKER_DOUBLE:

          readDouble( is, parameters->name, object );
          break;

        case gkg::BRUKER_STRING:

          readString( is, parameters->name, object );
          break;

        case gkg::BRUKER_ENUM:

          readEnum( is, parameters->name, object );
          break;

        case gkg::BRUKER_STRUCTURE:

          readStructure( is, parameters->name, object );
          break;

        case gkg::BRUKER_ARRAY_INTEGER:

          readArrayInteger( is, parameters->name, object );
          break;

        case gkg::BRUKER_ARRAY_DOUBLE:

          readArrayDouble( is, parameters->name, object );
          break;

        case gkg::BRUKER_ARRAY_STRING:

          readArrayString( is, parameters->name, object );
          break;

        case gkg::BRUKER_ARRAY_ENUM:

          readArrayEnum( is, parameters->name, object );
          break;

        case gkg::BRUKER_ARRAY_STRUCTURE:

          readArrayStructure( is, parameters->name, object );
          break;

        case gkg::BRUKER_UNKNOWN:

          throw std::runtime_error( "not a valid tag" );
          break;


      }
      ++ parameters;

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::BrukerRawDiskFormat< T >::readAcqpHeader( "
             "std::istream& is, "
             "gkg::HeaderedObject& object ) const" );

}


template < class T >
void gkg::BrukerRawDiskFormat< T >::readImndHeader(
                                             std::istream& is, 
                                             gkg::HeaderedObject& object ) const
{

  try
  {

    gkg::BrukerParameter* parameters = _imndParameterDictionary;
    while ( parameters->name != 0 )
    {

      switch ( parameters->type )
      {

        case gkg::BRUKER_INTEGER:

          readInteger( is, parameters->name, object );
          break;

        case gkg::BRUKER_DOUBLE:

          readDouble( is, parameters->name, object );
          break;

        case gkg::BRUKER_STRING:

          readString( is, parameters->name, object );
          break;

        case gkg::BRUKER_ENUM:

          readEnum( is, parameters->name, object );
          break;

        case gkg::BRUKER_STRUCTURE:

          readStructure( is, parameters->name, object );
          break;

        case gkg::BRUKER_ARRAY_INTEGER:

          readArrayInteger( is, parameters->name, object );
          break;

        case gkg::BRUKER_ARRAY_DOUBLE:

          readArrayDouble( is, parameters->name, object );
          break;

        case gkg::BRUKER_ARRAY_STRING:

          readArrayString( is, parameters->name, object );
          break;

        case gkg::BRUKER_ARRAY_ENUM:

          readArrayEnum( is, parameters->name, object );
          break;

        case gkg::BRUKER_ARRAY_STRUCTURE:

          readArrayStructure( is, parameters->name, object );
          break;

        case gkg::BRUKER_UNKNOWN:

          throw std::runtime_error( "not a valid tag" );
          break;

      }
      ++ parameters;

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::BrukerRawDiskFormat< T >::readImndHeader( "
             "std::istream& is, "
             "gkg::HeaderedObject& object ) const" );

}


template < class T >
void gkg::BrukerRawDiskFormat< T >::readMethodHeader(
                                             std::istream& is, 
                                             gkg::HeaderedObject& object ) const
{

  try
  {

    gkg::BrukerParameter* parameters = _methodParameterDictionary;
    while ( parameters->name != 0 )
    {

      switch ( parameters->type )
      {

        case gkg::BRUKER_INTEGER:

          readInteger( is, parameters->name, object );
          break;

        case gkg::BRUKER_DOUBLE:

          readDouble( is, parameters->name, object );
          break;

        case gkg::BRUKER_STRING:

          readString( is, parameters->name, object );
          break;

        case gkg::BRUKER_ENUM:

          readEnum( is, parameters->name, object );
          break;

        case gkg::BRUKER_STRUCTURE:

          readStructure( is, parameters->name, object );
          break;

        case gkg::BRUKER_ARRAY_INTEGER:

          readArrayInteger( is, parameters->name, object );
          break;

        case gkg::BRUKER_ARRAY_DOUBLE:

          readArrayDouble( is, parameters->name, object );
          break;

        case gkg::BRUKER_ARRAY_STRING:

          readArrayString( is, parameters->name, object );
          break;

        case gkg::BRUKER_ARRAY_ENUM:

          readArrayEnum( is, parameters->name, object );
          break;

        case gkg::BRUKER_ARRAY_STRUCTURE:

          readArrayStructure( is, parameters->name, object );
          break;

        case gkg::BRUKER_UNKNOWN:

          throw std::runtime_error( "not a valid tag" );
          break;

      }
      ++ parameters;

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::BrukerRawDiskFormat< T >::readMethodHeader( "
             "std::istream& is, "
             "gkg::HeaderedObject& object ) const" );

}


template < class T >
void gkg::BrukerRawDiskFormat< T >::replaceDiezeWithUnderscore(
                                                       std::string& data ) const
{

  try
  {

    // removing # from strings as this character is used for comment start
    // in meta-header
    std::string::size_type i = 0;
    for ( i = 0; i < data.length(); i++ )
    {

      if ( data[ i ] == '#' )
      {

        data[ i ] = '_';

      }

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::BrukerRawDiskFormat< T >::replaceDiezeWithUnderscore( "
             "std::string& data ) const" );

}


template < class T >
void gkg::BrukerRawDiskFormat< T >::readInteger(
                                             std::istream& is,
                                             const std::string& attributeName,
                                             gkg::HeaderedObject& object ) const
{

  try
  {

    // reading name
    if ( findPosition( is, attributeName ) )
    {

      // reading data
      int32_t data;
      is >> data;

#ifdef BRUKER_DEBUG

      std::cout << attributeName << " : [" << data << "]" << std::endl;

#endif

      // adding object to header
      object.addSemantic( "__bruker_raw__",
                          attributeName,
                          gkg::Semantic( gkg::TypeOf< int32_t >::getName() ) );
      object.getHeader().addAttribute( attributeName, data );

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::BrukerRawDiskFormat< T >::readInteger( "
             "std::istream& is, "
             "const std::string& attributeName, "
             "gkg::HeaderedObject& object ) const" );

}


template < class T >
void gkg::BrukerRawDiskFormat< T >::readDouble(
                                             std::istream& is,
                                             const std::string& attributeName,
                                             gkg::HeaderedObject& object ) const
{

  try
  {

    // reading name
    if ( findPosition( is, attributeName ) )
    {

      // reading data
      double data;
      is >> data;

#ifdef BRUKER_DEBUG

      std::cout << attributeName << " : [" << data << "]" << std::endl;

#endif

      // adding object to header
      object.addSemantic( "__bruker_raw__",
                          attributeName,
                          gkg::Semantic( gkg::TypeOf< double >::getName() ) );
      object.getHeader().addAttribute( attributeName, data );

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::BrukerRawDiskFormat< T >::readDouble( "
             "std::istream& is, "
             "const std::string& attributeName, "
             "gkg::HeaderedObject& object ) const" );

}


template < class T >
void gkg::BrukerRawDiskFormat< T >::readString(
                                             std::istream& is,
                                             const std::string& attributeName,
                                             gkg::HeaderedObject& object ) const
{

  try
  {

    // reading name
    if ( findPosition( is, attributeName ) )
    {

      // skipping string size ( '( xxx )' )
      char character;
      is >> character;
      while ( character != '<' )
      {

        is >> character;

      }

      // reading data
      std::string data = "";
      is >> character;
      while ( character != '>' )
      {

        data += character;
        is >> character;

      }
      if ( data.empty() )
      {

        data = "_empty_string_";

      }

      // replacing dieze character with underscore
      replaceDiezeWithUnderscore( data );

#ifdef BRUKER_DEBUG

      std::cout << attributeName << " : [" << data << "]" << std::endl;

#endif

      // adding object to header
      object.addSemantic(
                       "__bruker_raw__",
                       attributeName,
                       gkg::Semantic( gkg::TypeOf< std::string >::getName() ) );
      object.getHeader().addAttribute( attributeName, data );

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::BrukerRawDiskFormat< T >::readString( "
             "std::istream& is, "
             "const std::string& attributeName, "
             "gkg::HeaderedObject& object ) const" );

}


template < class T >
void gkg::BrukerRawDiskFormat< T >::readEnum(
                                             std::istream& is,
                                             const std::string& attributeName,
                                             gkg::HeaderedObject& object ) const
{

  try
  {

    // reading name
    if ( findPosition( is, attributeName ) )
    {

      // reading data
      std::string data = "";
      is >> data;

      // replacing dieze character with underscore
      replaceDiezeWithUnderscore( data );

#ifdef BRUKER_DEBUG

      std::cout << attributeName << " : [" << data << "]" << std::endl;

#endif

      // adding object to header
      object.addSemantic(
                       "__bruker_raw__",
                       attributeName,
                       gkg::Semantic( gkg::TypeOf< std::string >::getName() ) );
      object.getHeader().addAttribute( attributeName, data );

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::BrukerRawDiskFormat< T >::readEnum( "
             "std::istream& is, "
             "const std::string& attributeName, "
             "gkg::HeaderedObject& object ) const" );

}


template < class T >
void gkg::BrukerRawDiskFormat< T >::readStructure(
                                             std::istream& is,
                                             const std::string& attributeName,
                                             gkg::HeaderedObject& object ) const
{

  try
  {

    // reading name
    if ( findPosition( is, attributeName ) )
    {

      // reading data
      std::vector< std::string > structureItems = readStructureItems( is );

#ifdef BRUKER_DEBUG

      std::cout << attributeName << " : ";
      for ( size_t ii = 0; ii <  structureItems.size(); ii++ )
        std::cout << structureItems[ ii ] << " ";
      std::cout << std::endl;

#endif      

      // adding object to header
      object.addSemantic(
        "__bruker_raw__",
        attributeName,
        gkg::Semantic( gkg::TypeOf< std::vector< std::string > >::getName() ) );
      object.getHeader().addAttribute( attributeName, structureItems );

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::BrukerRawDiskFormat< T >::readStructure( "
             "std::istream& is, "
             "const std::string& attributeName, "
             "gkg::HeaderedObject& object ) const" );

}


template < class T >
void gkg::BrukerRawDiskFormat< T >::readArrayInteger(
                                             std::istream& is,
                                             const std::string& attributeName,
                                             gkg::HeaderedObject& object ) const
{

  try
  {

    // reading name
    if ( findPosition( is, attributeName ) )
    {


      // reading dimension(s)
      std::vector< int32_t > arraySize = readArraySize( is );

      // calculating global item count
      int32_t s, arrayItemCount = 1;
      for ( s = 0; s < ( int32_t )arraySize.size(); s++ )
      {

        arrayItemCount *= arraySize[ s ];

      }

      // reading data
      int32_t a;
      std::vector< int32_t > data( arrayItemCount );
      for ( a = 0; a < arrayItemCount; a++ )
      {

        is >> data[ a ];

      }

#ifdef BRUKER_DEBUG

      std::cout << attributeName << " : ";
      for ( size_t ii = 0; ii <  data.size(); ii++ )
        std::cout << data[ ii ] << " ";
      std::cout << std::endl;

#endif

      // adding object to header
      object.addSemantic(
            "__bruker_raw__",
            attributeName + "_array_size",
            gkg::Semantic( gkg::TypeOf< std::vector< int32_t > >::getName() ) );
      object.getHeader().addAttribute( attributeName + "_array_size",
                                       arraySize );

      object.addSemantic(
            "__bruker_raw__",
            attributeName,
            gkg::Semantic( gkg::TypeOf< std::vector< int32_t > >::getName() ) );
      object.getHeader().addAttribute( attributeName, data );

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::BrukerRawDiskFormat< T >::readArrayInteger( "
             "std::istream& is, "
             "const std::string& attributeName, "
             "gkg::HeaderedObject& object ) const" );

}


template < class T >
void gkg::BrukerRawDiskFormat< T >::readArrayDouble(
                                             std::istream& is,
                                             const std::string& attributeName,
                                             gkg::HeaderedObject& object ) const
{

  try
  {

    // reading name
    if ( findPosition( is, attributeName ) )
    {


      // reading dimension(s)
      std::vector< int32_t > arraySize = readArraySize( is );

      // calculating global item count
      int32_t s, arrayItemCount = 1;
      for ( s = 0; s < ( int32_t )arraySize.size(); s++ )
      {

        arrayItemCount *= arraySize[ s ];

      }

      // reading data
      int32_t a;
      std::vector< double > data( arrayItemCount );
      for ( a = 0; a < arrayItemCount; a++ )
      {

        is >> data[ a ];

      }

#ifdef BRUKER_DEBUG

      std::cout << attributeName << " : ";
      for ( size_t ii = 0; ii <  data.size(); ii++ )
        std::cout << data[ ii ] << " ";
      std::cout << std::endl;

#endif

      // adding object to header
      object.addSemantic(
            "__bruker_raw__",
            attributeName + "_array_size",
            gkg::Semantic( gkg::TypeOf< std::vector< int32_t > >::getName() ) );
      object.getHeader().addAttribute( attributeName + "_array_size",
                                       arraySize );

      object.addSemantic(
             "__bruker_raw__",
             attributeName,
             gkg::Semantic( gkg::TypeOf< std::vector< double > >::getName() ) );
      object.getHeader().addAttribute( attributeName, data );

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::BrukerRawDiskFormat< T >::readArrayDouble( "
             "std::istream& is, "
             "const std::string& attributeName, "
             "gkg::HeaderedObject& object ) const" );

}




template < class T >
void gkg::BrukerRawDiskFormat< T >::readArrayString(
                                             std::istream& is,
                                             const std::string& attributeName,
                                             gkg::HeaderedObject& object ) const
{

  try
  {

    // reading name
    if ( findPosition( is, attributeName ) )
    {


      // reading dimension(s)
      std::vector< int32_t > arraySize = readArraySize( is );
      arraySize.pop_back(); // the last dimension is the number
                            // of characters per string, so
                            // useless!

      // calculating global item count
      int32_t s, arrayItemCount = 1;
      for ( s = 0; s < ( int32_t )arraySize.size(); s++ )
      {

        arrayItemCount *= arraySize[ s ];

      }

      // reading data
      int32_t a;
      std::vector< std::string > data( arrayItemCount );
      for ( a = 0; a < arrayItemCount; a++ )
      {

        is >> data[ a ];

        // replacing dieze character with underscore
        replaceDiezeWithUnderscore( data[ a ] );

        // removing '<' and '>' characters
        if ( data[ a].length() > 2 )
        {

          data[ a ] = data[ a ].substr( 1, data[ a ].length() - 2 );

        }
        else
        {

          data[ a ] = "_empty_string_";

        }

      }

#ifdef BRUKER_DEBUG

      std::cout << attributeName << " : ";
      for ( size_t ii = 0; ii <  data.size(); ii++ )
        std::cout << data[ ii ] << " ";
      std::cout << std::endl;

#endif

      // adding object to header
      object.addSemantic(
            "__bruker_raw__",
            attributeName + "_array_size",
            gkg::Semantic( gkg::TypeOf< std::vector< int32_t > >::getName() ) );
      object.getHeader().addAttribute( attributeName + "_array_size",
                                       arraySize );

      object.addSemantic(
        "__bruker_raw__",
        attributeName,
        gkg::Semantic( gkg::TypeOf< std::vector< std::string > >::getName() ) );
      object.getHeader().addAttribute( attributeName, data );

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::BrukerRawDiskFormat< T >::readArrayString( "
             "std::istream& is, "
             "const std::string& attributeName, "
             "gkg::HeaderedObject& object ) const" );

}


template < class T >
void gkg::BrukerRawDiskFormat< T >::readArrayEnum(
                                             std::istream& is,
                                             const std::string& attributeName,
                                             gkg::HeaderedObject& object ) const
{

  try
  {

    // reading name
    if ( findPosition( is, attributeName ) )
    {


      // reading dimension(s)
      std::vector< int32_t > arraySize = readArraySize( is );

      // calculating global item count
      int32_t s, arrayItemCount = 1;
      for ( s = 0; s < ( int32_t )arraySize.size(); s++ )
      {

        arrayItemCount *= arraySize[ s ];

      }

      // reading data
      int32_t a;
      std::vector< std::string > data( arrayItemCount );
      for ( a = 0; a < arrayItemCount; a++ )
      {

        is >> data[ a ];

      }

#ifdef BRUKER_DEBUG

      std::cout << attributeName << " : ";
      for ( size_t ii = 0; ii <  data.size(); ii++ )
        std::cout << data[ ii ] << " ";
      std::cout << std::endl;

#endif

      // adding object to header
      object.addSemantic(
            "__bruker_raw__",
            attributeName + "_array_size",
            gkg::Semantic( gkg::TypeOf< std::vector< int32_t > >::getName() ) );
      object.getHeader().addAttribute( attributeName + "_array_size",
                                       arraySize );

      object.addSemantic(
        "__bruker_raw__",
        attributeName,
        gkg::Semantic( gkg::TypeOf< std::vector< std::string > >::getName() ) );
      object.getHeader().addAttribute( attributeName, data );

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::BrukerRawDiskFormat< T >::readArrayEnum( "
             "std::istream& is, "
             "const std::string& attributeName, "
             "gkg::HeaderedObject& object ) const" );

}


template < class T >
void gkg::BrukerRawDiskFormat< T >::readArrayStructure(
                                             std::istream& is,
                                             const std::string& attributeName,
                                             gkg::HeaderedObject& object ) const
{

  try
  {

    // reading name
    if ( findPosition( is, attributeName ) )
    {


      // reading dimension(s)
      std::vector< int32_t > arraySize = readArraySize( is );

      // calculating global item count
      int32_t s, arrayItemCount = 1;
      for ( s = 0; s < ( int32_t )arraySize.size(); s++ )
      {

        arrayItemCount *= arraySize[ s ];

      }

      // reading data
      int32_t a, i;
      std::vector< int32_t > structureSize( arrayItemCount );
      std::vector< std::string > data;
      for ( a = 0; a < arrayItemCount; a++ )
      {

        std::vector< std::string > structureItems = readStructureItems( is );
        structureSize[ a ] = ( int32_t )structureItems.size();
        for ( i = 0; i < ( int32_t )structureItems.size(); i++ )
        {

          data.push_back( structureItems[ i ] );

        }

      }

#ifdef BRUKER_DEBUG

      std::cout << attributeName << " : ";
      for ( size_t ii = 0; ii <  data.size(); ii++ )
        std::cout << data[ ii ] << " ";
      std::cout << std::endl;

#endif

      // adding object to header
      object.addSemantic(
            "__bruker_raw__",
            attributeName + "_array_size",
            gkg::Semantic( gkg::TypeOf< std::vector< int32_t > >::getName() ) );
      object.getHeader().addAttribute( attributeName + "_array_size",
                                       arraySize );

      object.addSemantic(
            "__bruker_raw__",
            attributeName + "_structure_size",
            gkg::Semantic( gkg::TypeOf< std::vector< int32_t > >::getName() ) );
      object.getHeader().addAttribute( attributeName + "_structure_size",
                                       structureSize );

      object.addSemantic(
        "__bruker_raw__",
        attributeName,
        gkg::Semantic( gkg::TypeOf< std::vector< std::string > >::getName() ) );
      object.getHeader().addAttribute( attributeName, data );

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::BrukerRawDiskFormat< T >::readArrayStructure( "
             "std::istream& is, "
             "const std::string& attributeName, "
             "gkg::HeaderedObject& object ) const" );

}


template < class T >
std::vector< int >
gkg::BrukerRawDiskFormat< T >::readArraySize( std::istream& is ) const
{

  try
  {

    char dummy;
    std::vector< int32_t > arraySize;
    is >> dummy;   // skipping '('
    while ( dummy != ')' )
    {

      int32_t size;
      // dummy is a ',' except for the last size where it is a ')'
      is >> size >> dummy;
      arraySize.push_back( size );

    }
    return arraySize;

  }
  GKG_CATCH( "template < class T > "
             "std::vector< int > "
             "gkg::BrukerRawDiskFormat< T >::readArraySize( "
             "std::istream& is) const " );

}


template < class T >
std::vector< std::string >
gkg::BrukerRawDiskFormat< T >::readStructureItems( std::istream& is ) const
{

  try
  {

    char character;
    std::vector< std::string > structureItems;

    skipWhiteSpace( is );
     // skipping '('
    is >> character;

    while ( character != ')' )
    {

      // reading raw item
      std::string structureItem = "";
      is >> character;
      while ( character != ',' && character != ')' )
      {

        structureItem += character;
        is >> character;

      }
      // removing spaces and '<' at the beginning of the item
      while ( structureItem[ 0 ] == ' ' || structureItem[ 0 ] == '<' )
      {

        structureItem = ( structureItem.length() > 1 ?
                          structureItem.substr( 1,
                                                structureItem.length() - 1 ) :
                          "" );

      }
      // removing spaces and '>' at the end of the item
      while ( !structureItem.empty() && 
              ( structureItem[ structureItem.length() - 1 ] == ' ' ||
                structureItem[ structureItem.length() - 1 ] == '>' ) )
      {

        structureItem = ( structureItem.length() > 1 ?
                          structureItem.substr( 0,
                                                structureItem.length() - 1 ) :
                          "" );

      }

      // manage case of empty string
      if ( structureItem.empty() )
      {

        structureItem = "_empty_string_";

      }

      // adding item to item vector
      structureItems.push_back( structureItem );

    }
    return structureItems;

  }
  GKG_CATCH( "template < class T > "
             "std::vector< std::string > "
             "gkg::BrukerRawDiskFormat< T >::readStructureItems( "
             "std::istream& is) const " );

}


template < class T >
bool gkg::BrukerRawDiskFormat< T >::findPosition(
                                        std::istream& is,
                                        const std::string& attributeName ) const
{

  try
  {

    // rewinding stream
    is.clear();
    is.seekg( 0, gkg::IosBase::beg );

    // building pattern to be searched
    std::string pattern = std::string( "##$" ) + attributeName + "=";

    std::string line;
    while ( getline( is, line ) )
    {

      std::string::size_type position = line.find( pattern );
      if ( position != std::string::npos )
      {

#if defined( __GNUC__ ) && (__GNUC__-0 < 3)

        is.seekg( - ( ( streamoff )line.length() -
                      ( streamoff )pattern.length() + 1 ),
                  gkg::IosBase::cur );

#else

        is.seekg( - ( ( std::ifstream::off_type )line.length() -
                      ( std::ifstream::off_type )pattern.length() + 1 ),
                  gkg::IosBase::cur );

#endif

        return true;

      }

    }
    is.clear();
    is.seekg( 0, gkg::IosBase::beg );
    return false;

  }
  GKG_CATCH( "bool gkg::BrukerRawDiskFormat< T >::findPosition( "
             "std::istream& is, "
             "const std::string& attributeName ) const" );

}

template < class T >
void gkg::BrukerRawDiskFormat< T >::skipWhiteSpace( std::istream& is ) const
{

  char character;
  is >> character;
  while ( character == ' ' || character == '\t' )
  {

    is >> character;

  }
  is.putback( character );

}


template class gkg::BrukerRawDiskFormat< int16_t >;
template class gkg::BrukerRawDiskFormat< float >;
template class gkg::BrukerRawDiskFormat< double >;


//
// registrating standard Bruker Raw disk format(s) for "Volume" object 
// factory
//

static bool initialize()
{

  try
  {

    gkg::DiskFormatFactory< gkg::Volume< std::complex< int16_t > > 
                           >::getInstance().registerDiskFormat(
         gkg::BrukerRawDiskFormat< int16_t >::getInstance().getName(),
         &gkg::BrukerRawDiskFormat< int16_t >::getInstance() );
    gkg::DiskFormatFactory< gkg::Volume< std::complex< float > > 
                           >::getInstance().registerDiskFormat(
         gkg::BrukerRawDiskFormat< float >::getInstance().getName(),
         &gkg::BrukerRawDiskFormat< float >::getInstance() );
    gkg::DiskFormatFactory< gkg::Volume< std::complex< double > > 
                           >::getInstance().registerDiskFormat(
         gkg::BrukerRawDiskFormat< double >::getInstance().getName(),
         &gkg::BrukerRawDiskFormat< double >::getInstance() );

    return true;

  }
  GKG_CATCH( "Bruker Raw disk format registration" );

}

static bool initialized __attribute__((unused)) = initialize();

