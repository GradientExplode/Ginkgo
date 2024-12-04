#ifndef _gkg_graphics_X11_X11_Xlib_h_
#define _gkg_graphics_X11_X11_Xlib_h_


// C++ interface to standard Xlib.h
extern "C"
{

#ifdef AIXV3

struct _XDisplay;
struct _XFreeFuncs;
struct _XSQEvent;
struct _XExten;
struct _XKeytrans;
struct _XDisplayAtoms;
struct _XContextDB;
struct _XIMFilter;
struct _XrmHashBucketRec;

#endif

#ifdef __DECCXX

struct _XDisplay;
struct _XPrivate;
struct _XrmHashBucketRec;

#endif

#include <gkg-graphics-X11-X11/Xdefs.h>
#include <X11/Xlib.h>
#include <gkg-graphics-X11-X11/Xundefs.h>


}


#endif
