#ifndef _gkg_mri_reconstruction_io_GehcSignaPFileHeader_h_
#define _gkg_mri_reconstruction_io_GehcSignaPFileHeader_h_


#define RDB_MAX_SLICES               512
#define RDB_MAX_PASSES    RDB_MAX_SLICES
#define RDB_MAX_DABS                   1
#define BLOCK                       char
#define ATOMIC                   int32_t
#define IMATRIXTYPE              int16_t
#define DIMXYTYPE                  float
#define PIXSIZETYPE                float
#define RASPOINT                   float

// specific to RDBM release 8.0
#define SLICE_FACTOR                   2


namespace gkg
{


//
// multiple receiver structure
//
struct RDB_MULTI_RCV_TYPE
{

  int16_t start_rcv;
  int16_t stop_rcv;

};


//
// Header record
//
struct RDB_HEADER_REC
{

  // common to revision 7.x, 8.x and 9.x

  float    rdb_hdr_rdbm_rev;
  int32_t  rdb_hdr_run_int;        // Rdy pkt Run Number
  int16_t  rdb_hdr_scan_seq;       // Rdy pkt Sequence Number
  int8_t   rdb_hdr_run_char[ 6 ];  // Rdy pkt Run no in character
  int8_t   rdb_hdr_scan_date[ 10 ];// scan date in character
  int8_t   rdb_hdr_scan_time[ 8 ]; // scan time
  int8_t   rdb_hdr_logo[ 10 ];     // GEHC rdbm used to verify file

  int16_t  rdb_hdr_file_contents;  // Data type 0=empty 1=nrec 2=raw
  int16_t  rdb_hdr_lock_mode;      // unused
  int16_t  rdb_hdr_dacq_ctrl;      // rhdacqctrl bit mask (15 bits)
  int16_t  rdb_hdr_recon_ctrl;     // rhrcctrl bit mask (15 bits)
  int16_t  rdb_hdr_exec_ctrl;      // rhexecctrl bit mask (15 bits)
  int16_t  rdb_hdr_scan_type;      // bit mask (15 bits)
  int16_t  rdb_hdr_data_collect_type;// rhtype bit mask (15 bits)
  int16_t  rdb_hdr_data_format;    // rhformat bit mask (15 bits)
  int16_t  rdb_hdr_recon;          // rhrecon proc-a-son recon (0 - 100)
  int16_t  rdb_hdr_datacq;         // rhdatacq proc-a-son dacq

  int16_t  rdb_hdr_npasses;        // rhnpasses passes for a scan (0-256)
  int16_t  rdb_hdr_npomp;          // rhnpomp pomp group slices (1,2)
  int16_t  rdb_hdr_nslices;        // rhnslices slices in a pass (0-256)
  int16_t  rdb_hdr_nechoes;        // rhnecho echoes of a slice (1-32)
  int16_t  rdb_hdr_navs;           // rhnavs num of excitations (1-32727)
  int16_t  rdb_hdr_nframes;        // rhnframes yres (0-1024)
  int16_t  rdb_hdr_baseline_views; // rhbline baselines (0-1028)
  int16_t  rdb_hdr_hnover;         // rhhnover overscans (0-1024)
  int16_t  rdb_hdr_frame_size;     // rhfrsize xres (0-1024)
  int16_t  rdb_hdr_point_size;     // rhptsize (2-4)

  int16_t  rdb_hdr_vquant;         // rhvquant 3d volumes 1

  int16_t  rdb_hdr_cheart;         // RX Cine heart phases (1-32)
  float    rdb_hdr_ctr;            // RX Cine TR in sec (0-3.40282e38)
  float    rdb_hdr_ctrr;           // RX Cine RR in sec (0-30.0)

  int16_t  rdb_hdr_initpass;       // rhinitpass allocate passes (0-32767)
  int16_t  rdb_hdr_incrpass;       // rhincrpass tps autopauses (0-32767)

  int16_t  rdb_hdr_method_ctrl;    // rhmethod  0=recon, 1=psd
  int16_t  rdb_hdr_da_xres;        // rhdaxres (0-1024)
  int16_t  rdb_hdr_da_yres;        // rhdayres (0-2049)
  int16_t  rdb_hdr_rc_xres;        // rhrcxres (0-1024)
  int16_t  rdb_hdr_rc_yres;        // rhrcyres (0-1024)
  int16_t  rdb_hdr_im_size;        // rhimsize (0-512)
  int32_t  rdb_hdr_rc_zres;        // power of 2 > rhnslices (0-128)

  int32_t  rdb_hdr_raw_pass_size;  // rhrawsize (0-2147483647)
  int32_t  rdb_hdr_sspsave;        // rhsspsave (0-2147483647)
  int32_t  rdb_hdr_udasave;        // rhudasave (0-2147483647)

  float    rdb_hdr_fermi_radius;   // rhfermr fermi radius (0-3.40282e38)
  float    rdb_hdr_fermi_width;    // rhfermw fermi width (0-3.40282e38)
  float    rdb_hdr_fermi_ecc;      // rhferme fermi excentiricty (0-3.40282e38)
  float    rdb_hdr_clip_min;       // rhclipmin 4x IP limit (+-16383)
  float    rdb_hdr_clip_max;       // rhclipmax 4x IP limit (+-16383)
  float    rdb_hdr_default_offset; // rhdoffset default offset=0 (+-3.40282e38)
  float    rdb_hdr_xoff;           // rhxoff scroll img in x (+-256)
  float    rdb_hdr_yoff;           // rhyoff scroll img in y (+-256)
  float    rdb_hdr_nwin;           // rhnwin hecho window width (0-256)
  float    rdb_hdr_ntran;          // rhntran hecho trans width (0-256)
  float    rdb_hdr_scalei;         // PS rhscalei (+-3.40282e38)
  float    rdb_hdr_scaleq;         // PS rhscaleq def=0 (+-3.40282e38)
  int16_t  rdb_hdr_rotation;       // RX 0 90 180 270 deg (0-3)
  int16_t  rdb_hdr_transpose;      // RX 0, 1 n / y transpose (0-1)
  int16_t  rdb_hdr_kissoff_views;  // rhblank zero image views (0-512)
  int16_t  rdb_hdr_slblank;        // rhslblank  slice blank 3d (0-128)
  int16_t  rdb_hdr_gradcoil;       // RX 0=off 1=Schnk 2=Rmr (0-2)
  int16_t  rdb_hdr_ddaover;        // rhddaover unused

  int16_t  rdb_hdr_sarr;           // SARR bit mask (15 bits)
  int16_t  rdb_hdr_fd_tr;          // SARR feeder timing info
  int16_t  rdb_hdr_fd_te;          // SARR feeder timing info
  int16_t  rdb_hdr_fd_ctrl;        // SARR control of feeder
  int16_t  rdb_hdr_algor_num;      // SARR df decimation ratio
  int16_t  rdb_hdr_fd_df_dec;      // SARR which feeder algor

  RDB_MULTI_RCV_TYPE rdb_hdr_dab[ 4 ];// rhdab0s rhdab0e st, stp rcv (0-15)

  float    rdb_hdr_user0;          // rhuser0   (+-3.40282e38)
  float    rdb_hdr_user1;          // rhuser1   (+-3.40282e38)
  float    rdb_hdr_user2;          // rhuser2   (+-3.40282e38)
  float    rdb_hdr_user3;          // rhuser3   (+-3.40282e38)
  float    rdb_hdr_user4;          // rhuser4   (+-3.40282e38)
  float    rdb_hdr_user5;          // rhuser5   (+-3.40282e38)
  float    rdb_hdr_user6;          // rhuser6   (+-3.40282e38)
  float    rdb_hdr_user7;          // rhuser7   (+-3.40282e38)
  float    rdb_hdr_user8;          // rhuser8   (+-3.40282e38)
  float    rdb_hdr_user9;          // rhuser9   (+-3.40282e38)
  float    rdb_hdr_user10;         // rhuser10  (+-3.40282e38)
  float    rdb_hdr_user11;         // rhuser11  (+-3.40282e38)
  float    rdb_hdr_user12;         // rhuser12  (+-3.40282e38)
  float    rdb_hdr_user13;         // rhuser13  (+-3.40282e38)
  float    rdb_hdr_user14;         // rhuser14  (+-3.40282e38)
  float    rdb_hdr_user15;         // rhuser15  (+-3.40282e38)
  float    rdb_hdr_user16;         // rhuser16  (+-3.40282e38)
  float    rdb_hdr_user17;         // rhuser17  (+-3.40282e38)
  float    rdb_hdr_user18;         // rhuser18  (+-3.40282e38)
  float    rdb_hdr_user19;         // rhuser19  (+-3.40282e38)
  
  int32_t  rdb_hdr_v_type;         // rhvtype bit mask (31 bits)
  float    rdb_hdr_v_coefxa;       // RX x flow direction control (0-4)
  float    rdb_hdr_v_coefxb;       // RX x flow direction control (0-4)
  float    rdb_hdr_v_coefxc;       // RX x flow direction control (0-4)
  float    rdb_hdr_v_coefxd;       // RX x flow direction control (0-4)
  float    rdb_hdr_v_coefya;       // RX y flow direction control (0-4)
  float    rdb_hdr_v_coefyb;       // RX y flow direction control (0-4)
  float    rdb_hdr_v_coefyc;       // RX y flow direction control (0-4)
  float    rdb_hdr_v_coefyd;       // RX y flow direction control (0-4)
  float    rdb_hdr_v_coefza;       // RX z flow direction control (0-4)
  float    rdb_hdr_v_coefzb;       // RX z flow direction control (0-4)
  float    rdb_hdr_v_coefzc;       // RX z flow direction control (0-4)
  float    rdb_hdr_v_coefzd;       // RX z flow direction control (0-4)
  float    rdb_hdr_vm_coef1;       // RX weight for mag image 1 (0-1)
  float    rdb_hdr_vm_coef2;       // RX weight for mag image 2 (0-1)
  float    rdb_hdr_vm_coef3;       // RX weight for mag image 3 (0-1)
  float    rdb_hdr_vm_coef4;       // RX weight for mag image 4 (0-1)
  float    rdb_hdr_v_venc;         // RX vel encoding cm/sec (0.001-5000)
  
  float    spectral_width;         // specwidth  filter width kHz (500-3355432)
  int16_t  csi_dims;               // spectro
  int16_t  xcsi;                   // rhspecrescsix (2-64)
  int16_t  ycsi;                   // rhspecrescsiy (2-64)
  int16_t  zcsi;                   // spectro
  float    roilenx;                // RX x csi volume dimension
  float    roileny;                // RX y csi volume dimension
  float    roilenz;                // RX z csi volume dimension
  float    roilocx;                // RX x csi volume center
  float    roilocy;                // RX y csi volume center
  float    roilocz;                // RX z csi volume center
  float    numdwell;               // specdwells (0-3.40282e38)

  int32_t  rdb_hdr_ps_command;     // PS internal use only
  int32_t  rdb_hdr_ps_mps_r1;      // PS MPS R1 setting (1-7)
  int32_t  rdb_hdr_ps_mps_r2;      // PS MPS R2 setting (1-30)
  int32_t  rdb_hdr_ps_mps_tg;      // PS MPS Transmit gain setting (0-200)
  int32_t  rdb_hdr_ps_mps_freq;    // PS MPS Center frequency Hz (+-3.40282e38)
  int32_t  rdb_hdr_ps_aps_r1;      // PS APS R1 setting (1-7)
  int32_t  rdb_hdr_ps_aps_r2;      // PS APS R2 setting (1-30)
  int32_t  rdb_hdr_ps_aps_tg;      // PS APS Transmit gain setting (0-200)
  int32_t  rdb_hdr_ps_aps_freq;    // PS APS Center frequency Hz (+-3.40282e38)
  float    rdb_hdr_ps_scalei;      // PS rational scaling (+-3.40282e38)
  float    rdb_hdr_ps_scaleq;      // PS unused
  int32_t  rdb_hdr_ps_snr_warning; // PS noise test 0=16 1=32 bits
  int32_t  rdb_hdr_ps_aps_or_mps;  // PS prescan order logic (0-5)
  int32_t  rdb_hdr_ps_mps_bitmap;  // PS bit mask (4 bits)
  int8_t   rdb_hdr_ps_powerspec[ 256 ];
  int32_t  rdb_hdr_ps_filler1;     // PS filler
  int32_t  rdb_hdr_ps_filler2;     // PS filler
  float    rdb_hdr_rec_noise_mean[ 16 ];
                                   // PS mean noise each receiver (+-3.40282e38)
  float    rdb_hdr_rec_noise_std[ 16 ];
                                   // PS noise calc for muti rec (+-3.40282e38)

  int16_t  halfecho;               // spectro 0=full echo, 1=half echo

  // 858 bytes

  int16_t  rdb_hdr_im_size_y;      // rh???? (0-512)
  int32_t  rdb_hdr_data_collect_type1;// rh???? bit mask (31 bits)
  float    rdb_hdr_freq_scale;     // rh???? freq k-space step (+-3.40282e38)
  float    rdb_hdr_phase_scale;    // rh???? freq k-space step (+-3.40282e38)

  // 14 bytes

  int16_t  rdb_hdr_ovl;            // rhovl - overlaps for MOTSA

  // Phase Correction Control Param.
  int16_t  rdb_hdr_pclin;          // Linear Corr. 0=off, 1=linear, 2=polynom.
  int16_t  rdb_hdr_pclinnpts;      // fit number of points
  int16_t  rdb_hdr_pclinorder;     // fit order
  int16_t  rdb_hdr_pclinavg;       // linear phase corr avg 0=off, 1=on
  int16_t  rdb_hdr_pccon;          // Const Corr. 0=off, 1=Ky spec.,
                                   //        2=polyfit(2/ilv), 3:polyfit(1/ilv)
  int16_t  rdb_hdr_pcconnpts;      // fit number of points
  int16_t  rdb_hdr_pcconorder;     // fit order
  int16_t  rdb_hdr_pcextcorr;      // external correction file 0=off 1=on
  int16_t  rdb_hdr_pcgraph;        // Phase Corr. coef. image 0=off 1=lin+cst
  int16_t  rdb_hdr_pcileave;       // Interleaves to use for correction: 0=all,
                                   //                              1=only first
  int16_t  rdb_hdr_hdbestky;       // bestky view for fractional Ky scan
  int16_t  rdb_hdr_pcctrl;         // phase correction research control
  int16_t  rdb_hdr_pcthrespts;     // 2..512 adjacent points
  int16_t  rdb_hdr_pcdiscbeg;      // 0..512 beginning point to discard
  int16_t  rdb_hdr_pcdiscmid;      // 0..512 middle point to discard
  int16_t  rdb_hdr_pcdiscend;      // 0..512 ending point to discard
  int16_t  rdb_hdr_pcthrespct;     // Threshold percentage
  int16_t  rdb_hdr_pcspacial;      // Spacial best ref scan index 0..512
  int16_t  rdb_hdr_pctemporal;     // Temporal best ref scan index 0..512
  int16_t  rdb_hdr_pcspare;        // spare for phase correction
  int16_t  rdb_hdr_ileaves;        // Number of interleaves
  int16_t  rdb_hdr_kydir;          // Ky traversal dircetion 0=top-down, 
                                   //     1=center out 2=bottom-up
  int16_t  rdb_hdr_alt;            // Alt read sign 0=no, 1=odd/even, 2=pairs
  int16_t  rdb_hdr_reps;           // Number of scan repetitions
  int16_t  rdb_hdr_ref;            // Ref Scan 0=off 1=on

  float    rdb_hdr_pcconnorm;      // Constant S term normalization factor
  float    rdb_hdr_pcconfitwt;     // Constant polyfit weighting factor
  float    rdb_hdr_pclinnorm;      // Linear S term normalization factor
  float    rdb_hdr_pclinfitwt;     // Linear polyfit weighting factor

  float    rdb_hdr_pcbestky;       // Best Ky location

  // VRG Filter param
  int32_t  rdb_hdr_vrgf;           // control word for VRG filter
  int32_t  rdb_hdr_vrgfxres;       // control word for VRGF final x resolution


  // Bandpass Asymmetry  Correction Param
  int32_t  rdb_hdr_bp_corr;        // control word for bandpass asymmetry
  float    rdb_hdr_recv_freq_s;    // starting frequency (+62.5)
  float    rdb_hdr_recv_freq_e;    // ending frequency (-62.5)

  int32_t  rdb_hdr_hniter;         // Selects the number of iterations used in
                                   // homodyne processing

  int32_t  rdb_hdr_fast_rec;       // Added for homodyne II, tells if the fast
                                   // receiver is being used and the lpf 
                                   // setting of the fast receiver,
                                   //      0=fast receiver off,
                                   //    1-5=lpf settings

  int32_t  rdb_hdr_refframes;      // total # of frames for ref scan
  int32_t  rdb_hdr_refframep;      // # of frames per pass for a ref scan
  int32_t  rdb_hdr_scnframe;       // total # of frames for a entire scan
  int32_t  rdb_hdr_pasframe;       // # of frames per pass

  uint32_t rdb_hdr_user_usage_tag; // for spectro
  uint32_t rdb_hdr_user_fill_mapMSW;// for spectro
  uint32_t rdb_hdr_user_fill_mapLSW;// for Spectro

  // all following usercv are for spectro
  float    rdb_hdr_user20;
  float    rdb_hdr_user21;
  float    rdb_hdr_user22;
  float    rdb_hdr_user23;
  float    rdb_hdr_user24;
  float    rdb_hdr_user25;
  float    rdb_hdr_user26;
  float    rdb_hdr_user27;
  float    rdb_hdr_user28;
  float    rdb_hdr_user29;
  float    rdb_hdr_user30;
  float    rdb_hdr_user31;
  float    rdb_hdr_user32;
  float    rdb_hdr_user33;
  float    rdb_hdr_user34;
  float    rdb_hdr_user35;
  float    rdb_hdr_user36;
  float    rdb_hdr_user37;
  float    rdb_hdr_user38;
  float    rdb_hdr_user39;
  float    rdb_hdr_user40;
  float    rdb_hdr_user41;
  float    rdb_hdr_user42;
  float    rdb_hdr_user43;
  float    rdb_hdr_user44;
  float    rdb_hdr_user45;
  float    rdb_hdr_user46;
  float    rdb_hdr_user47;
  float    rdb_hdr_user48;

  int16_t  rdb_hdr_pcfitorig;      // Adjust view indexes if bestky view=0
  int16_t  rdb_hdr_pcshotfirst;    // 1st view within an echo group used for fit
  int16_t  rdb_hdr_pcshotlast;     // Last view within an echo grp used for fit
  int16_t  rdb_hdr_pcmultegrp;     // 1=force pts from other egrps to be used
  int16_t  rdb_hdr_pclinfix;       // 2=force slope to be set to pclinslope
                                   // 1=neg readout slope = pos readout slope
  int16_t  rdb_hdr_pcconfix;       // 2=force slope to be set to pcconslope
                                   // 1=neg readout slope = pos readout slope
  float    rdb_hdr_pclinslope;     // Value to set lin slope to if forced
  float    rdb_hdr_pcconslope;     // Value to set con slope to if forced
  int16_t  rdb_hdr_pccoil;         // If 1,2,3,4, use that coil results for all

  // Variable View Sharing
  int16_t  rdb_hdr_vvsmode;        // Variable view sharing mode
  int16_t  rdb_hdr_vvsaimgs;       // number of original images
  int16_t  rdb_hdr_vvstr;          // TR in microseconds
  int16_t  rdb_hdr_vvsgender;      // gender: male or female

  // 3D Slice ZIP
  int16_t  rdb_hdr_zip_factor;     // Slice ZIP factor: 0=OFF, 2, or 4

  // Maxwell Term Correction Coefficients
  float    rdb_hdr_maxcoef1a;      // Coefficient A for flow image 1
  float    rdb_hdr_maxcoef1b;      // Coefficient B for flow image 1
  float    rdb_hdr_maxcoef1c;      // Coefficient C for flow image 1
  float    rdb_hdr_maxcoef1d;      // Coefficient D for flow image 1
  float    rdb_hdr_maxcoef2a;      // Coefficient A for flow image 2
  float    rdb_hdr_maxcoef2b;      // Coefficient B for flow image 2
  float    rdb_hdr_maxcoef2c;      // Coefficient C for flow image 2
  float    rdb_hdr_maxcoef2d;      // Coefficient D for flow image 2
  float    rdb_hdr_maxcoef3a;      // Coefficient A for flow image 3
  float    rdb_hdr_maxcoef3b;      // Coefficient B for flow image 3
  float    rdb_hdr_maxcoef3c;      // Coefficient C for flow image 3
  float    rdb_hdr_maxcoef3d;      // Coefficient D for flow image 3

  int32_t  rdb_hdr_ut_ctrl;        // System utility control variable
  int16_t  rdb_hdr_dp_type;        // EPI II diffusion control cv

  int16_t  rdb_hdr_arw;            // Arrhythmia rejection window
                                   // (percentage:1-100)

  int16_t  rdb_hdr_vps;            // View Per Segment for FastCine

  int16_t  rdb_hdr_mcReconEnable;  // N-Coil recon map


  // common to revisions 8.x and 9.x

  float    rdb_hdr_fov;            // Auto-NCoil

  int32_t  rdb_hdr_te;             // TE for first echo
  int32_t  rdb_hdr_te2;            // TE for second and later echoes
  float    rdb_hdr_dfmrbw;         // BW for navigator frames
  int32_t  rdb_hdr_dfmctrl;        // Control flag for dfm (0=off, 1=on)
  int32_t  rdb_hdr_raw_nex;        // Uncombined NEX at start of recon
  int32_t  rdb_hdr_navs_per_pass;  // Max. navigator frames in a pass
  int32_t  rdb_hdr_dfmxres;        // xres of navigator frames
  int32_t  rdb_hdr_dfmptsize;      // point size of navigator frames
  int32_t  rdb_hdr_navs_per_view;  // Num. navigators per frame (tag table)
  int32_t  rdb_hdr_dfmdebug;       // control flag for dfm debug
  float    rdb_hdr_dfmthreshold;   // threshold for navigator correction

                  // Section added to support gridding
  int16_t  rdb_hdr_grid_control;   // bit settings controlling gridding
  int16_t  rdb_hdr_b0map;          // B0 map enable and map size
  int16_t  rdb_hdr_grid_tediff;    // TE difference between b0 map arms
  int16_t  rdb_hdr_grid_motion_comp;// flag to apply motion compensation
  float    rdb_hdr_grid_radius_a;  // variable density transition
  float    rdb_hdr_grid_radius_b;  // variable density transition
  float    rdb_hdr_grid_max_gradient;// Max gradient amplitude
  float    rdb_hdr_grid_max_slew;  // Max slew rate
  float    rdb_hdr_grid_scan_fov;  // Rx scan field of view
  float    rdb_hdr_grid_a2d_time;  // A to D sample time microsecs
  float    rdb_hdr_grid_density_factor;// change factor for variable density
  float    rdb_hdr_grid_display_fov;// Rx display field of view

  // only in revision 9.x
  int16_t  rdb_hdr_fatwater;       // for Fat and Water Dual Recon
  // placed here in revision 9.x
  // int16_t rdb_hdr_fiestamlf;    // MFO FIESTA recon control (was excess2)

  int16_t  rdb_hdr_app;            // Auto Post-Processing opcode
  int16_t  rdb_hdr_rhncoilsel;     // Auto-Ncoil
  int16_t  rdb_hdr_rhncoillimit;   // Auto-Ncoil
  int16_t  rdb_hdr_app_option;     // Auto Post_processing options
  int16_t  rdb_hdr_grad_mode;      // Gradient mode in Gemini project


  // placed here in revision 8.x
  int16_t  rdb_hdr_fiestamlf;      // MFO FIESTA recon control (was excess2)
  // only in revision 9.x
  int16_t  rdb_hdr_pfile_passes;   // Num passes stored in a multi-pass

  int32_t  rdb_hdr_asset;
  int32_t  rdb_hdr_asset_calthresh;
  float    rdb_hdr_asset_R;
  int32_t  rdb_hdr_coilno;
  int32_t  rdb_hdr_asset_phases;
  float    rdb_hdr_scancent;       // Table position
  int32_t  rdb_hdr_position;       // Patient position
  int32_t  rdb_hdr_entry;          // Patient entry
  float    rdb_hdr_lmhor;          // Landmark


  // only in revision 9.x
  int32_t  rdb_hdr_last_slice_num;   
  float    rdb_hdr_asset_slice_R;  // Slice reduction factor
  float    rdb_hdr_asset_slabwrap;


  float    rdb_hdr_dwnav_coeff;    // Coeff for amount of phase correction
  int16_t  rdb_hdr_dwnav_cor;      // Navigator echo correction
  int16_t  rdb_hdr_dwnav_view;     // Num of views of nav echoes
  int16_t  rdb_hdr_dwnav_corecho;  // Num of nav echoes for actual correction
  int16_t  rdb_hdr_dwnav_sview;    // Start view for phase correction process
  int16_t  rdb_hdr_dwnav_eview;    // End view for phase correction process
  int16_t  rdb_hdr_dwnav_sshot;    // Start shot for delta phase estimation
                                   // in nav echoes
  int16_t  rdb_hdr_dwnav_eshot;    // End shot for delta phase estimation in
                                   // nav echoes

  int16_t  rdb_hdr_3dwin_type;     // 0 = Modified Hanning, 1 = modified Tukey
  float    rdb_hdr_3dwin_apod;     // degree of apodization; 0.0 = boxcar,
                                   //                        1.0 = hanning
  float    rdb_hdr_3dwin_q;        // apodization at ends, 0.0 = max,
                                   //                      1.0 = boxcar
   
  int16_t  rdb_hdr_ime_scic_enable;// Surface Coil Intensity Correction:
                                   // 1 if enabled
  int16_t  rdb_hdr_clariview_type; // Type of Clariview/Name of Filter
  float    rdb_hdr_ime_scic_edge;  // Edge paramaters for Enhanced Recon
  float    rdb_hdr_ime_scic_smooth;// Smooth paramaters for Enhanced Recon
  float    rdb_hdr_ime_scic_focus; // Focus paramaters for Enhanced Recon
  float    rdb_hdr_clariview_edge; // Edge paramaters for clariview
  float    rdb_hdr_clariview_smooth;// Smooth paramaters for clariview
  float    rdb_hdr_clariview_focus;// Focus paramaters for clariview
  float    rdb_hdr_scic_reduction; // Reduction paramater for SCIC
  float    rdb_hdr_scic_gauss;     // Gauss paramater for SCIC
  float    rdb_hdr_scic_threshold; // Threshold paramater for SCIC
   
  int32_t  rdb_hdr_ectricks_no_regions;// Total no of regions acquired by PSD
  int32_t  rdb_hdr_ectricks_input_regions;// Total no of input regions
                                          // for reordering

  int16_t  rdb_hdr_psc_reuse;      // Header field for smart prescan

  int16_t  rdb_hdr_left_blank;     //  K-space blanking fields
  int16_t  rdb_hdr_right_blank;

  int16_t  rdb_hdr_acquire_type;   // Acquire type information from CV

  int16_t  rdb_hdr_retro_control;  // Retrosective FSE phase correction
                                   // control flag
  int16_t  rdb_hdr_etl;            // Added for Retrospective FSE phase
                                   // correction. This variable has the ETL
                                   // value set by the user. This variable has
                                   // a generic name, so that any other PSD who
                                   // wants to send ETL value to Recon can use
                                   // this variable.

  int16_t  rdb_hdr_pcref_start;    // 1st view to use for dynamic EPI phase
                                   // correction
  int16_t  rdb_hdr_pcref_stop;     // Last view to use for dynamic EPI phase
                                   // correction
  int16_t  rdb_hdr_ref_skip;       // Number of passes to skip for dynamic EPI
                                   // phase correction
  int16_t  rdb_hdr_extra_frames_top;// Number of extra frames at top of K-space
  int16_t  rdb_hdr_extra_frames_bot;// Number of extra frames at bottom of 
                                    // K-space

};
 

//
// Per Pass Table and Unlocked RAW Table
//
//	Per Pass DACQ table
//        1. Pointers to data (data, ssp and uda)
//        2. Index by (pass number - 1) in Data Acquisition Table.
//
//	Unlocked RAW table
//        1. pointers to where UNLOCKED RAW DATA is in BAM
//        2. index by (pass number - 1)
//

struct VME_ADDRESS
{

  int32_t bam_modifier;
  int32_t bam_address;

};


struct RDB_PASS_INFO_ENTRY
{

  VME_ADDRESS dab_bam[ RDB_MAX_DABS ];

};


typedef RDB_PASS_INFO_ENTRY RDB_PER_PASS_TAB[ RDB_MAX_PASSES ];
typedef RDB_PASS_INFO_ENTRY RDB_UNLOCK_RAW_TAB[ RDB_MAX_PASSES ];


//
// DATA ACQUISITION TABLE
//   CVs used in setting this table up (by Scan):
//   SI_RDBSLICE RH_SLPASS RH_SLTIME RH_SLLOC
//

struct RDB_SLICE_INFO_ENTRY
{

  int16_t pass_number;             // which pass this slice is in
  int16_t slice_in_pass;           // which slice in this pass
  float   gw_point1[ 3 ];          // corner points of image
  float   gw_point2[ 3 ];
  float   gw_point3[ 3 ];

};


typedef RDB_SLICE_INFO_ENTRY RDB_DATA_ACQ_TAB[ SLICE_FACTOR * RDB_MAX_SLICES ];


//
// NEX Table
//

// gain table structure
struct RDB_GAIN_ENTRY
{

  int16_t range;                   // view range for this gain
  float   gaini;                   // real part of gain
  float   gainq;                   // imaginary part

};


typedef RDB_GAIN_ENTRY  RDB_GAIN_TAB[ 5 ]; // max # of ranges


 // nex table structure
typedef int16_t RDB_NEX_ENTRY;
typedef RDB_NEX_ENTRY RDB_NEX_TAB[ 1026 ]; // max da_yres


struct RDB_NEX_TYPE
{

  int16_t rdb_hdr_nex_size[ 1026 ];

};



//
// TOOLSDATA
//

typedef char TOOLSDATA[ 2048 ];


//
// EXAMDATATYPE
//

struct VARTYPE
{

  uint32_t length;                 // length of the data
  int8_t*  data;                   // pointer to the data

};


struct EXAMDATATYPE
{

  BLOCK ex_suid[ 4 ];          // Suite ID for this Exam
  int16_t ex_uniq;             // The Make-Unique Flag
  char ex_diskid;              // Disk ID for this Exam
  uint16_t ex_no;              // Exam Number

  char    hospname[ 33 ];      // Hospital Name
  int16_t detect;              // Detector Type
  int32_t numcells;            // Number of cells in det
  float   zerocell;            // Cell number at theta
  float    cellspace;          // Cell spacing
  float    srctodet;           // Distance from source to detector
  float    srctoiso;           // Distance from source to iso
  int16_t  tubetyp;            // Tube type
  int16_t  dastyp;             // DAS type
  int16_t  num_dcnk;           // Number of Decon Kernals
  int16_t  dcn_len;            // Number of elements in a Decon Kernal
  int16_t  dcn_density;        // Decon Kernal density
  int16_t  dcn_stepsize;       // Decon Kernal stepsize
  int16_t  dcn_shiftcnt;       // Decon Kernal Shift Count
  int32_t  magstrength;        // Magnet strength (in gauss)
  char     patid[ 13 ];        // Patient ID for this Exam
  char     patname[ 25 ];      // Patient Name
  int16_t  patage;             // Patient Age (years, months or days)
  int16_t  patian;             // Patient Age Notation
  int16_t  patsex;             // Patient Sex
  int32_t  patweight;          // Patient Weight
  int16_t  trauma;             // Trauma Flag
  char     hist[ 61 ];         // Patient History
  char     reqnum[ 13 ];       // Requisition Number
  int32_t  ex_datetime;        // Exam date/time stamp
  char     refphy[ 33 ];       // Referring Physician
  char     diagrad[ 33 ];      // Diagnostician/Radiologist
  char     op[ 4 ];            // Operator
  char     ex_desc[ 23 ];      // Exam Description
  char     ex_typ[ 3 ];        // Exam Type
  int16_t  ex_format;          // Exam Format
  double   firstaxtime;        // Start time(secs) of first axial in exam
  char     ex_sysid[ 9 ];      // Creator Suite and Host
  int32_t  ex_lastmod;         // Date/Time of Last Change
  int16_t  protocolflag;       // Non-Zero indicates Protocol Exam
  char     ex_alloc_key[ 13 ]; // Process that allocated this record
  ATOMIC   ex_delta_cnt;       // Indicates number of updates to header
  BLOCK    ex_verscre[ 2 ];    // Genesis Version - Created
  BLOCK    ex_verscur[ 2 ];    // Genesis Version - Now

  uint32_t ex_checksum;        // Exam Record Checksum
  ATOMIC   ex_complete;        // Exam Complete Flag

  ATOMIC   ex_seriesct;        // Last Series Number Used

  ATOMIC   ex_numarch;         // Number of Series Archived

  ATOMIC   ex_numseries;       // Number of Series Existing
  VARTYPE  ex_series;          // Series Keys for this Exam

  ATOMIC   ex_numunser;        // Number of Unstored Series
  VARTYPE  ex_unseries;        // Unstored Series Keys for this Exam

  ATOMIC   ex_toarchcnt;       // Number of Unarchived Series
  VARTYPE  ex_toarchive;       // Unarchived Series Keys for this Exam

  ATOMIC   ex_prospcnt;        // Number of Prospective/Scout Series
  VARTYPE  ex_prosp;           // Prospective/Scout Series Keys for this Exam


  ATOMIC   ex_modelnum;        // Last Model Number used

  ATOMIC   ex_modelcnt;        // Number of ThreeD Models
  VARTYPE  ex_models;          // ThreeD Model Keys for Exam

  int16_t  ex_stat;            // Patient Status
  BLOCK    uniq_sys_id[ 16 ];  // Unique System ID
  BLOCK    service_id[ 16 ];   // Unique Service ID
  BLOCK    mobile_loc[ 4 ];    // Mobile Location Number
  BLOCK    study_uid[ 32 ];    // Study Entity Unique ID
  int16_t  study_status;       // indicates if study has complete info
                               // (DICOM/genesis)

  BLOCK    refsopcuid[ 32 ];   // Ref SOP Class UID
  BLOCK    refsopiuid[ 32 ];   // Ref SOP Instance UID
  BLOCK    patnameff[ 65 ];    // FF Patient Name
  BLOCK    patidff[ 65 ];      // FF Patient ID
  BLOCK    reqnumff[ 17 ];     // FF Requisition No
  BLOCK    dateofbirth[ 9 ];   // Date of Birth
  BLOCK    mwlstudyuid[ 32 ];  // Genesis Exam UID
  BLOCK    mwlstudyid[ 16 ];   // Genesis Exam No

};


//
// SERIESDATATYPE
//


struct SERIESDATATYPE
{

  BLOCK    se_suid[ 4 ];           // Suite ID for this Series
  int16_t  se_uniq;                // The Make-Unique Flag
  char     se_diskid;              // Disk ID for this Series
  uint16_t se_exno;                // Exam Number
  int16_t  se_no;                  // Series Number
  int32_t  se_datetime;            // Allocation Series Data/Time stamp
  int32_t  se_actual_dt;           // Actual Series Data/Time stamp
  char     se_desc[ 30 ];          // Series Description
  char     pr_sysid[ 9 ];          // Primary Receiver Suite and Host
  char     pansysid[ 9 ];          // Archiver Suite and Host
  int16_t  se_typ;                 // Series Type
  int16_t  se_source;              // Series from which prescribed
  int16_t  se_plane;               // Most-like Plane (for L/S)
  int16_t  scan_type;              // Scout or Axial (for CT)
  int32_t  position;               // Patient Position
  int32_t  entry;                  // Patient Entry
  char     anref[ 3 ];             // Anatomical reference
  float    lmhor;                  // Horizontal Landmark
  char     prtcl[ 25 ];            // Scan Protocol Name
  int16_t  se_contrast;            // Non-zero if > 0 image used contrast(L/S)
  char     start_ras;              // RAS letter for first scan location (L/S)
  float    start_loc;              // First scan location (L/S)
  char     end_ras;                // RAS letter for last scan location (L/S)
  float    end_loc;                // Last scan location (L/S)
  int16_t  se_pseq;                // Last Pulse Sequence Used (L/S)
  int16_t  se_sortorder;           // Image Sort Order (L/S)
  int32_t  se_lndmrkcnt;           // Landmark Counter
  int16_t  se_nacq;                // Number of Acquisitions
  int16_t  xbasest;                // Starting number for baselines
  int16_t  xbaseend;               // Ending number for baselines
  int16_t  xenhst;                 // Starting number for enhanced scans
  int16_t  xenhend;                // Ending number for enhanced scans
  int32_t  se_lastmod;             // Date/Time of Last Change
  char     se_alloc_key[ 13 ];     // Process that allocated this record
  ATOMIC   se_delta_cnt;           // Indicates number of updates to header
  BLOCK    se_verscre[ 2 ];        // Genesis Version - Created
  BLOCK    se_verscur[ 2 ];        // Genesis Version - Now
  float    se_pds_a;               // PixelData size - as stored
  float    se_pds_c;               // PixelData size - Compressed
  float    se_pds_u;               // PixelData size - UnCompressed
  uint32_t se_checksum;            // Series Record checksum
  ATOMIC   se_complete;            // Series Complete Flag
  ATOMIC   se_numarch;             // Number of Images Archived
  ATOMIC   se_imagect;             // Last Image Number Used
  ATOMIC   se_numimages;           // Number of Images Existing
  VARTYPE  se_images;              // Image Keys for this Series
  ATOMIC   se_numunimg;            // Number of Unstored Images
  VARTYPE  se_unimages;            // Unstored Image Keys for this Series
  ATOMIC   se_toarchcnt;           // Number of Unarchived Images
  VARTYPE  se_toarchive;           // Unarchived Image Keys for this Series
  float    echo1_alpha;            // Echo 1 Alpha Value
  float    echo1_beta;             // Echo 1 Beta Value
  uint16_t echo1_window;           // Echo 1 Window Value
  int16_t  echo1_level;            // Echo 1 Level Value
  float    echo2_alpha;            // Echo 2 Alpha Value
  float    echo2_beta;             // Echo 2 Beta Value
  uint16_t echo2_window;           // Echo 2 Window Value
  int16_t  echo2_level;            // Echo 2 Level Value
  float    echo3_alpha;            // Echo 3 Alpha Value
  float    echo3_beta;             // Echo 3 Beta Value
  uint16_t echo3_window;           // Echo 3 Window Value
  int16_t  echo3_level;            // Echo 3 Level Value
  float    echo4_alpha;            // Echo 4 Alpha Value
  float    echo4_beta;             // Echo 4 Beta Value
  uint16_t echo4_window;           // Echo 4 Window Value
  int16_t  echo4_level;            // Echo 4 Level Value
  float    echo5_alpha;            // Echo 5 Alpha Value
  float    echo5_beta;             // Echo 5 Beta Value
  uint16_t echo5_window;           // Echo 5 Window Value
  int16_t  echo5_level;            // Echo 5 Level Value
  float    echo6_alpha;            // Echo 6 Alpha Value
  float    echo6_beta;             // Echo 6 Beta Value
  uint16_t echo6_window;           // Echo 6 Window Value
  int16_t  echo6_level;            // Echo 6 Level Value
  float    echo7_alpha;            // Echo 7 Alpha Value
  float    echo7_beta;             // Echo 7 Beta Value
  uint16_t echo7_window;           // Echo 7 Window Value
  int16_t  echo7_level;            // Echo 7 Level Value
  float    echo8_alpha;            // Echo 8 Alpha Value
  float    echo8_beta;             // Echo 8 Beta Value
  uint16_t echo8_window;           // Echo 8 Window Value
  int16_t  echo8_level;            // Echo 8 Level Value
  BLOCK    series_uid[ 32 ];       // Series Entity Unique ID
  BLOCK    landmark_uid[ 32 ];     // Landmark Unique ID
  BLOCK    equipmnt_uid[ 32 ];     // Equipment Unique ID

  // only in revision 9.x
  BLOCK    refsopcuids[ 32 ];      // Ref SOP Class UID
  BLOCK    refsopiuids[ 32 ];      // Ref SOP Instance UID
  BLOCK    schacitval[ 16 ];       // Sched Proc Action Item Seq - Value
  BLOCK    schacitdesc[ 16 ];      // Sched Proc Action Item Seq - Description
  BLOCK    schacitmea[ 64 ];       // Sched Proc Action Item Seq - Meaning
  BLOCK    schprocstdesc[ 64 ];    // Sched Proc Step Desc
  BLOCK    schprocstid[ 16 ];      // Sched Proc Step ID 1
  BLOCK    reqprocstid[ 16 ];      // Req Proc Step ID 1
  BLOCK    perprocstid[ 16 ];      // PPS ID
  BLOCK    perprocstdesc[ 64 ];    // PPS Description

  BLOCK    reqprocstid2[ 16 ];     // Req Proc Step ID 2
  BLOCK    reqprocstid3[ 16 ];     // Req Proc Step ID 3
  BLOCK    schprocstid2[ 16 ];     // Sched Proc Step ID 2
  BLOCK    schprocstid3[ 16 ];     // Sched Proc Step ID 3
  BLOCK    refImgUID[4][ 32 ];     // Dicom Reference Image
  int16_t  table_entry;            // Table position for nMR and iMR
  int16_t  SwingAngle;             // nMR - Swing Angle
  int16_t  LateralOffset;          // nMR - Offset
  int16_t  GradientCoil;           // Gradient Coil Selection
  int16_t  se_subtype;             // supplements se_typ, see DICOM (0008,0008)
  int16_t  BWRT;                   // for fMRI till ExptTimePts
  BLOCK    PdgmStr[ 64 ];
  BLOCK    PdgmDesc[ 256 ];
  BLOCK    PdgmUID[ 64 ];
  int32_t  ExpType;
  int32_t  TrRest;
  int32_t  TrActive;
  int32_t  DumAcq;
  BLOCK    ApplName[ 16 ];
  BLOCK    ApplVer[ 16 ];
  int32_t  ExptTimePts;

};


//
// MRIMAGEDATATYPE
//


struct MRIMAGEDATATYPE 
{

  BLOCK    im_suid[ 4 ];           // Suite id for this image
  int16_t  im_uniq;                // The Make-Unique Flag
  char     im_diskid;              // Disk ID for this Image
  uint16_t im_exno;                // Exam number for this image
  int16_t  im_seno;                // Series Number for this image
  int16_t  im_no;                  // Image Number
  int32_t  im_datetime;            // Allocation Image date/time stamp
  int32_t  im_actual_dt;           // Actual Image date/time stamp
  float    sctime;                 // Duration of scan
  float    slthick;                // Slice Thickness (mm)
  IMATRIXTYPE imatrix_X;           // Image matrix size - X
  IMATRIXTYPE imatrix_Y;           // Image matrix size - Y
  float    dfov;                   // Display field of view - X (mm)
  float    dfov_rect;              // Display field of view - Y (if different)
  DIMXYTYPE dim_X;                 // Image dimension - X
  DIMXYTYPE dim_Y;                 // Image dimension - Y
  PIXSIZETYPE pixsize_X;           // Image pixel size - X
  PIXSIZETYPE pixsize_Y;           // Image pixel size - Y
  BLOCK    pdid[ 14 ];             // Pixel Data ID
  char     contrastIV[ 17 ];       // IV Contrast Agent
  char     contrastOral[ 17 ];     // Oral Contrast Agent
  int16_t  contmode;               // Image Contrast Mode
  int16_t  serrx;                  // Series from which prescribed
  int16_t  imgrx;                  // Image from which prescribed
  int16_t  screenformat;           // Screen Format(8/16 bit)
  int16_t  plane;                  // Plane Type
  float    scanspacing;            // Spacing between scans (mm?)
  int16_t  im_compress;            // Image compression type for allocation
  int16_t  im_scouttype;           // Scout Type (AP or lateral)
  char     loc_ras;                // RAS letter of image location
  float    loc;                    // Image location
  RASPOINT ctr_R;                  // Center R coord of plane image
  RASPOINT ctr_A;                  // Center A coord of plane image
  RASPOINT ctr_S;                  // Center S coord of plane image
  RASPOINT norm_R;                 // Normal R coord
  RASPOINT norm_A;                 // Normal A coord
  RASPOINT norm_S;                 // Normal S coord
  RASPOINT tlhc_R;                 // R Coord of Top Left Hand Corner
  RASPOINT tlhc_A;                 // A Coord of Top Left Hand Corner
  RASPOINT tlhc_S;                 // S Coord of Top Left Hand Corner
  RASPOINT trhc_R;                 // R Coord of Top Right Hand Corner
  RASPOINT trhc_A;                 // A Coord of Top Right Hand Corner
  RASPOINT trhc_S;                 // S Coord of Top Right Hand Corner
  RASPOINT brhc_R;                 // R Coord of Bottom Right Hand Corner
  RASPOINT brhc_A;                 // A Coord of Bottom Right Hand Corner
  RASPOINT brhc_S;                 // S Coord of Bottom Right Hand Corner
  char     forimgrev[ 4 ];         // Foreign Image Revision
  int32_t  tr;                     // Pulse repetition time(usec)
  int32_t  ti;                     // Pulse inversion time(usec)
  int32_t  te;                     // Pulse echo time(usec)
  int32_t  te2;                    // Second echo echo (usec)
  int16_t  numecho;                // Number of echoes
  int16_t  echonum;                // Echo Number
  float    tbldlta;                // Table Delta
  float    nex;                    // Number of Excitations
  int16_t  contig;                 // Continuous Slices Flag
  int16_t  hrtrate;                // Cardiac Heart Rate (bpm)
  int32_t  tdel;                   // Delay time after trigger (msec)
  float    saravg;                 // Average SAR
  float    sarpeak;                // Peak SAR
  int16_t  monsar;                 // Monitor SAR flag
  int16_t  trgwindow;              // Trigger window (% of R-R interval)
  float    reptime;                // Cardiac repetition time
  int16_t  imgpcyc;                // Images per cardiac cycle
  int16_t  xmtgain;                // Actual Transmit Gain (.1 db)
  int16_t  rcvgain1;               // Actual Receive Gain Analog (.1 db)
  int16_t  rcvgain2;               // Actual Receive Gain Digital (.1 db)
  int16_t  mr_flip;                // Flip Angle for GRASS scans (deg.)
  int32_t  mindat;                 // Minimum Delay after Trigger (uSec)
  int16_t  cphase;                 // Total Cardiac Phase prescribed
  int16_t  swappf;                 // Swap Phase/Frequency Axis
  int16_t  pauseint;               // Pause Interval (slices)
  float    pausetime;              // Pause Time
  int32_t  obplane;                // Oblique Plane
  int32_t  slocfov;                // Slice Offsets on Freq axis
  int32_t  xmtfreq;                // Center Frequency (0.1 Hz)
  int32_t  autoxmtfreq;            // Auto Center Frequency (0.1 Hz)
  int16_t  autoxmtgain;            // Auto Transmit Gain (0.1 dB)
  int16_t  prescan_r1;             // PreScan R1 - Analog
  int16_t  prescan_r2;             // PreScan R2 - Digital
  int32_t  user_bitmap;            // Bitmap defining user CVs
  int16_t  cenfreq;                // Center Frequency Method
  int16_t  imode;                  // Imaging Mode
  int32_t  iopt;                   // Imaging Options
  int16_t  pseq;                   // Pulse Sequence
  int16_t  pseqmode;               // Pulse Sequence Mode
  char     psdname[ 33 ];          // Pulse Sequence Name
  int32_t  psd_datetime;           // PSD Creation Date and Time
  char     psd_iname[ 13 ];        // PSD name from inside PSD
  int16_t  ctyp;                   // Coil Type
  char     cname[ 17 ];            // Coil Name
  int16_t  surfctyp;               // Surface Coil Type
  int16_t  surfcext;               // Extremity Coil Flag
  int32_t  rawrunnum;              // RawData Run Number
  uint32_t cal_fldstr;             // Calibrated Field Strength (x10 uGauss)
  int16_t  supp_tech;              // SAT fat/water/none
  float    vbw;                    // Variable Bandwidth (Hz)
  int16_t  slquant;                // Number of slices in this scan group
  int16_t  gpre;                   // Graphically prescribed
  int32_t  intr_del;               // Interimage/interloc delay (uSec)
  float    user0;                  // User Variable 0
  float    user1;                  // User Variable 1
  float    user2;                  // User Variable 2
  float    user3;                  // User Variable 3
  float    user4;                  // User Variable 4
  float    user5;                  // User Variable 5
  float    user6;                  // User Variable 6
  float    user7;                  // User Variable 7
  float    user8;                  // User Variable 8
  float    user9;                  // User Variable 9
  float    user10;                 // User Variable 10
  float    user11;                 // User Variable 11
  float    user12;                 // User Variable 12
  float    user13;                 // User Variable 13
  float    user14;                 // User Variable 14
  float    user15;                 // User Variable 15
  float    user16;                 // User Variable 16
  float    user17;                 // User Variable 17
  float    user18;                 // User Variable 18
  float    user19;                 // User Variable 19
  float    user20;                 // User Variable 20
  float    user21;                 // User Variable 21
  float    user22;                 // User Variable 22
  float    user23;                 // Projection Angle
  float    user24;                 // Concat Sat Type Flag
  char     im_alloc_key[ 13 ];
  int32_t  im_lastmod;             // Date/Time of Last Change
  BLOCK    im_verscre[ 2 ];        // Genesis Version - Created
  BLOCK    im_verscur[ 2 ];        // Genesis Version - Now
  int32_t  im_pds_a;               // PixelData size - as stored
  int32_t  im_pds_c;               // PixelData size - Compressed
  int32_t  im_pds_u;               // PixelData size - UnCompressed
  uint32_t im_checksum;            // AcqRecon record checksum
  ATOMIC   im_archived;            // Image Archive Flag
  ATOMIC   im_complete;            // Image Complete Flag
  int16_t  satbits;                // Bitmap of SAT selections
  int16_t  scic;                   // Surface Coil Intensity Correction Flag
  int16_t  satxloc1;               // R-side SAT pulse loc rel to lndmrk
  int16_t  satxloc2;               // L-side SAT pulse loc rel to lndmrk
  int16_t  satyloc1;               // A-side SAT pulse loc rel to lndmrk
  int16_t  satyloc2;               // P-side SAT pulse loc rel to lndmrk
  int16_t  satzloc1;               // S-side SAT pulse loc rel to lndmrk
  int16_t  satzloc2;               // I-side SAT pulse loc rel to lndmrk
  int16_t  satxthick;              // Thickness of X-axis SAT pulse
  int16_t  satythick;              // Thickness of Y-axis SAT pulse
  int16_t  satzthick;              // Thickness of Z-axis SAT pulse
  int16_t  flax;                   // Phase contrast flow axis
  int16_t  venc;                   // Phase contrast velocity encoding
  int16_t  thk_disclmr;            // Slice Thickness
  int16_t  ps_flag;                // Auto/Manual Prescan flag
  int16_t  ps_status;              // Bitmap of changed values
  int16_t  image_type;             // Magnitude, Phase, Imaginary, or Real
  int16_t  vas_collapse;           // Collapse Image
  float    user23n;                // User Variable 23
  float    user24n;                // User Variable 24
  int16_t  proj_alg;               // Projection Algorithm
  char     proj_name[ 13 ];        // Projection Algorithm Name
  float    x_axis_rot;             // X Axis Rotation
  float    y_axis_rot;             // Y Axis Rotation
  float    z_axis_rot;             // Z Axis Rotation
  int32_t  thresh_min1;            // Lower Range of Pixels 1
  int32_t  thresh_max1;            // Upper Range of Pixels 1
  int32_t  thresh_min2;            // Lower Range of Pixels 2
  int32_t  thresh_max2;            // Upper Range of Pixels 2
  int16_t  echo_trn_len;           // Echo Train Length for Fast Spin Echo
  int16_t  frac_echo;              // Fractional Echo - Effective TE Flag
  int16_t  prep_pulse;             // Preporatory Pulse Option
  int16_t  cphasenum;              // Cardiac Phase Number
  int16_t  var_echo;               // Variable Echo Flag
  char     ref_img;                // Reference Image Field
  char     sum_img;                // Summary Image Field
  uint16_t img_window;             // Window Value
  int16_t  img_level;              // Level Value
  int32_t  slop_int_1;             // Number of 3D Slabs
  int32_t  slop_int_2;             // Slice Locs Per 3D Slab
  int32_t  slop_int_3;             // # of Slice Locs on Each Slab Which 
                                   // Overlap Neighbors
  int32_t  slop_int_4;             // Image Filtering 0.5/0.2T
  int32_t  slop_int_5;             // Integer Slop Field 5
  float    slop_float_1;           // Float Slop Field 1
  float    slop_float_2;           // Float Slop Field 2
  float    slop_float_3;           // Float Slop Field 3
  float    slop_float_4;           // Float Slop Field 4
  float    slop_float_5;           // Float Slop Field 5
  char     slop_str_1[ 16 ];       // String Slop Field 1
  char     slop_str_2[ 16 ];       // String Slop Field 2
  int16_t  scanactno;              // Scan Acquisition Number
  int16_t  vasflags;               // Magnitude Weighting Flag
  float    vencscale;              // Scale Weighted Venc
  int16_t  integrity;              // GE Image Integrity
  int32_t  fphase;                 // Number Of Phases
  int16_t  freq_dir;               // Frequency Direction
  int16_t  vas_mode;               // Vascular Mode
  BLOCK    image_uid[ 32 ];        // Image Unique ID
  BLOCK    sop_uid[ 32 ];          // Service Obj Class Unique ID
  int16_t  dont_use_1;             // This field is not used
  int16_t  dont_use_2;             // This field is not used
  int16_t  dont_use_3;             // This field is not used
  int16_t  pscopts;                // bitmap of prescan options
  int16_t  asoffsetx;              // gradient offset in X-direction
  int16_t  asoffsety;              // gradient offset in Y-direction
  int16_t  asoffsetz;              // gradient offset in Z-direction
  int16_t  unoriginal;             // identifies image as original or unoriginal
  int16_t  interleaves;            // number of EPI shots
  int16_t  effechospace;           // effective echo spacing for EPI
  int16_t  viewsperseg;            // views per segment
  int16_t  rbpm;                   // respiratory rate, breaths per min
  int16_t  rtpoint;                // respiratory trigger point as percent of 
                                   // max.
  int16_t  rcvrtype;               // type of receiver used
  float    dbdt;                   // peak rate of change of gradient field, 
                                   // tesla/sec
  float    dbdtper;                // limit in units of percent of theoretical 
                                   // curve
  float    estdbdtper;             // PSD estimated limit in units of percent
  float    estdbdtts;              // PSD estimated limit in Teslas/sec
  float    saravghead;             // Avg head SAR
  float    neg_scanspacing;        // Negative scan spacing for overlap slices 
  int32_t  offsetfreq;             // Offset Frequency - Mag.Transfer
  uint32_t user_usage_tag;         // Defines how following user CVs are to be 
                                   // filled in
                                   // Default value = 0x00000000
                                   // GE range = 0x00000001 - 0x7fffffff
                                   // Research = 0x80000000 - 0xffffffff
  uint32_t user_fill_mapMSW;       // Define what process fills in the user CVs,
                                   // ifcc or TIR
  uint32_t user_fill_mapLSW;       // Define what process fills in the user CVs,
                                   // ifcc or TIR

  float    user25;                 // User Variable 25
  float    user26;                 // User Variable 26
  float    user27;                 // User Variable 27
  float    user28;                 // User Variable 28
  float    user29;                 // User Variable 29
  float    user30;                 // User Variable 30
  float    user31;                 // User Variable 31
  float    user32;                 // User Variable 32
  float    user33;                 // User Variable 33
  float    user34;                 // User Variable 34
  float    user35;                 // User Variable 35
  float    user36;                 // User Variable 36
  float    user37;                 // User Variable 37
  float    user38;                 // User Variable 38
  float    user39;                 // User Variable 39
  float    user40;                 // User Variable 40
  float    user41;                 // User Variable 41
  float    user42;                 // User Variable 42
  float    user43;                 // User Variable 43
  float    user44;                 // User Variable 44
  float    user45;                 // User Variable 45
  float    user46;                 // User Variable 46
  float    user47;                 // User Variable 47
  float    user48;                 // User Variable 48

  int32_t  iopt2;                  //  imaging option field #2 

  // only in revision 7.x
  int32_t  slop_int_6;             // Integer Slop Field 6
  int32_t  slop_int_8;             // Integer Slop Field 8
  int32_t  slop_int_9;             // Integer Slop Field 9

  // only in revision 9.x
  int32_t  b_value;                // B-value for DW-EPI
  int32_t  ihtagging;              // tag type
  int32_t  ihtagspc;               // tag space
  int32_t  ihfcineim;              // Fast CINE interpolation method
  int32_t  ihfcinent;              // Fast CINE normalization type
  int32_t  num_seg;
  int32_t  oprtarr;                // Respiratory Trigger window
  int32_t  averages;               // Number of averages for spectro
  int32_t  station_index;          // Station Index
  int32_t  station_total;          // Station Total
  int32_t  slop_int_17;            // the LAST Spare integer field!
  float    RegressorVal;
  int32_t  delAcq;                 // Delay after Acquisition (MP/fMRI screen)

};


//
// RDBM Header
//

struct RDBM_HEADER
{

  RDB_HEADER_REC rdb_hdr_rec;
  RDB_PER_PASS_TAB rdb_hdr_per_pass;
  RDB_PER_PASS_TAB rdb_hdr_unlock_raw;
  RDB_DATA_ACQ_TAB rdb_hdr_data_acq_tab;
  RDB_NEX_TYPE rdb_hdr_nex_tab;
  RDB_NEX_TYPE rdb_hdr_nex_abort_tab;

};


//
// IMAGE Header
//

struct IMAGE_HEADER
{

  TOOLSDATA rdb_hdr_tool;
  EXAMDATATYPE rdb_hdr_exam;
  SERIESDATATYPE rdb_hdr_series;
  MRIMAGEDATATYPE rdb_hdr_image;

};


//
// POOL Header
//

struct POOL_HEADER
{

  RDB_HEADER_REC rdb_hdr_rec;
  RDB_PER_PASS_TAB rdb_hdr_per_pass;
  RDB_PER_PASS_TAB rdb_hdr_unlock_raw;
  RDB_DATA_ACQ_TAB rdb_hdr_data_acq_tab;

  RDB_NEX_TYPE rdb_hdr_nex_tab;
  RDB_NEX_TYPE rdb_hdr_nex_abort_tab;

  TOOLSDATA rdb_hdr_tool;
  EXAMDATATYPE rdb_hdr_exam;
  SERIESDATATYPE rdb_hdr_series;
  MRIMAGEDATATYPE rdb_hdr_image;

};


}


//
//  POOL_HEADER size(s) in byte(s)
//                      +--------------+--------------+--------------+
//                      | revision 7.x | revision 8.x | revision 9.x |
//  +-------------------+--------------+--------------+--------------+
//  |                   |              |              |              |
//  | RDB_HEADER_REC    |      2048    |      2048    |      2048    |
//  | RDB_PER_PASS_TAB  |      4096    |      4096    |      4096    |
//  | RDB_PER_PASS_TAB  |      4096    |      4096    |      4096    |
//  | RDB_DATA_ACQ_TAB  |     20480    |     40960    |     40960    |
//  |                   |              |              |              |
//  | RDB_NEX_TYPE      |      2052    |      2052    |      2052    |
//  | RDB_NEX_TYPE      |      2052    |      2052    |      2052    |
//  |                   |              |              |              |
//  | TOOLSDATA         |      2048    |      2048    |      2048    |
//  | EXAMDATATYPE      |      1040    |      1040    |      1040    |
//  | SERIESDATATYPE    |      1028    |      1028    |      1536    |
//  | MRIMAGEDATATYPE   |      1044    |      1044    |      1536    |
//  |                   |              |              |              |
//  +-------------------+--------------+--------------+--------------+
//                      |     39984    |     60464    |     61464    |
//                      +--------------+--------------+--------------+
//


//
// useful size define macros
//

// revision 7.x

#define GEHC_SIGNA_PFILE_RDB_HEADER_REC_SIZE_REV_7_x       2048U
#define GEHC_SIGNA_PFILE_RDB_PER_PASS_TAB_SIZE_REV_7_x     4096U
#define GEHC_SIGNA_PFILE_RDB_DATA_ACQ_TAB_SIZE_REV_7_x    20480U
#define GEHC_SIGNA_PFILE_RDB_NEX_TYPE_SIZE_REV_7_x         2052U
#define GEHC_SIGNA_PFILE_TOOLSDATA_SIZE_REV_7_x            2048U
#define GEHC_SIGNA_PFILE_EXAMDATATYPE_SIZE_REV_7_x         1040U
#define GEHC_SIGNA_PFILE_SERIESDATATYPE_SIZE_REV_7_x       1028U
#define GEHC_SIGNA_PFILE_MRIMAGEDATATYPE_SIZE_REV_7_x      1044U

#define GEHC_SIGNA_PFILE_HEADER_SIZE_REV_7_x              39984U


// revision 8.x

#define GEHC_SIGNA_PFILE_RDB_HEADER_REC_SIZE_REV_8_x       2048U
#define GEHC_SIGNA_PFILE_RDB_PER_PASS_TAB_SIZE_REV_8_x     4096U
#define GEHC_SIGNA_PFILE_RDB_DATA_ACQ_TAB_SIZE_REV_8_x    40960U
#define GEHC_SIGNA_PFILE_RDB_NEX_TYPE_SIZE_REV_8_x         2052U
#define GEHC_SIGNA_PFILE_TOOLSDATA_SIZE_REV_8_x            2048U
#define GEHC_SIGNA_PFILE_EXAMDATATYPE_SIZE_REV_8_x         1040U
#define GEHC_SIGNA_PFILE_SERIESDATATYPE_SIZE_REV_8_x       1028U
#define GEHC_SIGNA_PFILE_MRIMAGEDATATYPE_SIZE_REV_8_x      1044U

#define GEHC_SIGNA_PFILE_HEADER_SIZE_REV_8_x              60464U


// revision 9.x

#define GEHC_SIGNA_PFILE_RDB_HEADER_REC_SIZE_REV_9_x       2048U
#define GEHC_SIGNA_PFILE_RDB_PER_PASS_TAB_SIZE_REV_9_x     4096U
#define GEHC_SIGNA_PFILE_RDB_DATA_ACQ_TAB_SIZE_REV_9_x    40960U
#define GEHC_SIGNA_PFILE_RDB_NEX_TYPE_SIZE_REV_9_x         2052U
#define GEHC_SIGNA_PFILE_TOOLSDATA_SIZE_REV_9_x            2048U
#define GEHC_SIGNA_PFILE_EXAMDATATYPE_SIZE_REV_9_x         1040U
#define GEHC_SIGNA_PFILE_SERIESDATATYPE_SIZE_REV_9_x       1536U
#define GEHC_SIGNA_PFILE_MRIMAGEDATATYPE_SIZE_REV_9_x      1536U

#define GEHC_SIGNA_PFILE_HEADER_SIZE_REV_9_x              61464U


#endif
