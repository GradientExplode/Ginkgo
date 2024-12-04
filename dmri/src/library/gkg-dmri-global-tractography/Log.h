#ifndef _gkg_dmri_global_tractography_Log_h_
#define _gkg_dmri_global_tractography_Log_h_


#include <fstream>


#define DEBUG 0


#define INIT_DEBUG if ( DEBUG ) \
                    { \
                      std::ofstream os( "/tmp/global_tractography.txt" ); \
                    }


#define BEGIN_DEBUG if ( DEBUG ) \
                    { \
                      std::ofstream os( "/tmp/global_tractography.txt", \
                                        std::fstream::app );



#define END_DEBUG     os.close(); \
                    }


#endif

