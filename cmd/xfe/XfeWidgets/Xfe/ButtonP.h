/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 *
 * The contents of this file are subject to the Netscape Public
 * License Version 1.1 (the "License"); you may not use this file
 * except in compliance with the License. You may obtain a copy of
 * the License at http://www.mozilla.org/NPL/
 *
 * Software distributed under the License is distributed on an "AS
 * IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or
 * implied. See the License for the specific language governing
 * rights and limitations under the License.
 *
 * The Original Code is mozilla.org code.
 *
 * The Initial Developer of the Original Code is Netscape
 * Communications Corporation.  Portions created by Netscape are
 * Copyright (C) 1998 Netscape Communications Corporation. All
 * Rights Reserved.
 *
 * Contributor(s): 
 */

/*----------------------------------------------------------------------*/
/*																		*/
/* Name:		<Xfe/ButtonP.h>											*/
/* Description:	XfeButton widget private header file.					*/
/* Author:		Ramiro Estrugo <ramiro@netscape.com>					*/
/*																		*/
/*----------------------------------------------------------------------*/

#ifndef _XfeButtonP_h_							/* start ButtonP.h		*/
#define _XfeButtonP_h_

#include <Xfe/Button.h>
#include <Xfe/LabelP.h>

XFE_BEGIN_CPLUSPLUS_PROTECTION

/*----------------------------------------------------------------------*/
/*																		*/
/* XfeButton method inheritance macros									*/
/*																		*/
/*----------------------------------------------------------------------*/
#define XfeInheritLayoutPixmap				((XtWidgetProc)			_XtInherit)
#define XfeInheritDrawPixmap				((XfeExposeProc)		_XtInherit)
#define XfeInheritDrawUnderline				((XfeExposeProc)		_XtInherit)
#define XfeInheritArmTimeout				((XtTimerCallbackProc)	_XtInherit)
   
/*----------------------------------------------------------------------*/
/*																		*/
/* XfeButtonClassPart													*/
/*																		*/
/*----------------------------------------------------------------------*/
typedef struct
{
    XtWidgetProc			layout_pixmap;		/* layout_pixmap		*/
    XfeExposeProc			draw_pixmap;		/* draw_pixmap			*/
    XfeExposeProc			draw_accent_border;	/* draw_raise_border	*/
    XfeExposeProc			draw_underline;		/* draw_underline		*/
	XtTimerCallbackProc		arm_timeout;		/* arm_timeout			*/
    XtPointer				extension;			/* Extension			*/
} XfeButtonClassPart;

/*----------------------------------------------------------------------*/
/*																		*/
/* XfeButtonClassRec													*/
/*																		*/
/*----------------------------------------------------------------------*/
typedef struct _XfeButtonClassRec
{
    CoreClassPart			core_class;
    XmPrimitiveClassPart	primitive_class;
    XfePrimitiveClassPart	xfe_primitive_class;
    XfeLabelClassPart		xfe_label_class;
    XfeButtonClassPart		xfe_button_class;
} XfeButtonClassRec;

externalref XfeButtonClassRec xfeButtonClassRec;

/*----------------------------------------------------------------------*/
/*																		*/
/* XfeButtonPart														*/
/*																		*/
/*----------------------------------------------------------------------*/
typedef struct _XfeButtonPart
{
    /* Callback resources */
    XtCallbackList		activate_callback;		/* Activate callback	*/
    XtCallbackList		arm_callback;			/* Arm callback			*/
    XtCallbackList		disarm_callback;		/* Disarm callback		*/
    XtCallbackList		button_3_down_callback;	/* Button 3 down cb		*/
    XtCallbackList		button_3_up_callback;	/* Button 3 up cb		*/
    
    /* Pixmap resources */
    Pixmap				armed_pixmap;			/* Armed pixmap			*/
    Pixmap				raised_pixmap;			/* Raised pixmap		*/
    Pixmap				insensitive_pixmap;		/* Insensitive pixmap	*/
    Pixmap				pixmap;					/* Pixmap				*/

    Pixmap				armed_pixmap_mask;		/* Armed pixmap mask	*/
    Pixmap				raised_pixmap_mask;		/* Raised pixmap mask	*/
    Pixmap				insensitive_pixmap_mask;/* Insens pixmap mask	*/
    Pixmap				pixmap_mask;			/* Pixmap mask			*/

    /* Arm resources */
    Boolean				armed;					/* Armed ?				*/
    Pixel				arm_background;			/* Arm background		*/
    Pixel				arm_foreground;			/* Arm foreground		*/
    Dimension			arm_offset;				/* Arm offset			*/
    Boolean				fill_on_arm;			/* Fill on arm ?		*/

    /* Raise resources */
    Boolean				fill_on_enter;			/* Fill on enter ?		*/
    Pixel				raise_foreground;		/* Raise foreground		*/
    Pixel				raise_background;		/* Raise background		*/
	Dimension			accent_border_thickness;/* Accent border thickness*/
    Boolean				raise_on_enter;			/* Raise on enter ?		*/
    Boolean				raised;					/* Raised ?				*/
    Boolean				determinate;			/* Determinate ?		*/

    /* Misc resources */
    unsigned char		button_type;			/* Button type			*/
    unsigned char		button_layout;			/* Button layout		*/
    unsigned char		button_trigger;			/* Button trigger		*/
	Boolean				emulate_motif;			/* Emulate motif		*/
    Dimension			spacing;				/* Spacing				*/

    /* Underline resources */
    Dimension			underline_thickness;	/* Underline thickness	*/

	/* Cursor resources */
    Cursor				transparent_cursor;		/* Transparent cursor	*/

    /* Private Data Members */
    GC					raised_GC;				/* Raised gc			*/
    GC					label_raised_GC;		/* Label raised gc		*/

    GC					armed_GC;				/* Armed gc				*/
    GC					label_armed_GC;			/* Label armed gc		*/

    GC					pixmap_GC;				/* Pixmap gc			*/
    XRectangle			pixmap_rect;			/* Box rectangle		*/

    Boolean				clicking;				/* Clicking				*/

    Boolean				transparent_cursor_state;/* Trans cursor state	*/

} XfeButtonPart;

/*----------------------------------------------------------------------*/
/*																		*/
/* XfeButtonRec															*/
/*																		*/
/*----------------------------------------------------------------------*/
typedef struct _XfeButtonRec
{
    CorePart			core;					/* Core Part			*/
    XmPrimitivePart		primitive;				/* XmPrimitive Part		*/
    XfePrimitivePart	xfe_primitive;			/* XfePrimitive Part	*/
    XfeLabelPart		xfe_label;				/* XfeLabel Part		*/
    XfeButtonPart		xfe_button;				/* XfeButton Part		*/
} XfeButtonRec;

/*----------------------------------------------------------------------*/
/*																		*/
/* XfeButtonPart Access Macro											*/
/*																		*/
/*----------------------------------------------------------------------*/
#define _XfeButtonPart(w) &(((XfeButtonWidget) w) -> xfe_button)

/*----------------------------------------------------------------------*/
/*																		*/
/* XfeButton - superclass = XfeLabel									*/
/*																		*/
/*----------------------------------------------------------------------*/
#define _XFE_PREPARE_BUTTON_PIXMAP					XfePrepare2
#define _XFE_PREPARE_BUTTON_ARMED_PIXMAP			XfePrepare3
#define _XFE_PREPARE_BUTTON_RAISED_PIXMAP			XfePrepare4
#define _XFE_PREPARE_BUTTON_INSENSITIVE_PIXMAP		XfePrepare5

/*----------------------------------------------------------------------*/
/*																		*/
/* XfeButton Method invocation functions								*/
/*																		*/
/*----------------------------------------------------------------------*/
extern void
_XfeButtonLayoutPixmap			(Widget			w);
/*----------------------------------------------------------------------*/
extern void
_XfeButtonDrawPixmap			(Widget			w,
								 XEvent *		event,
								 Region			region,
								 XRectangle *	clip_rect);
/*----------------------------------------------------------------------*/
extern void
_XfeButtonDrawAccentBorder		(Widget			w,
								 XEvent *		event,
								 Region			region,
								 XRectangle *	clip_rect);
/*----------------------------------------------------------------------*/
extern void
_XfeButtonDrawUnderline			(Widget			w,
								 XEvent *		event,
								 Region			region,
								 XRectangle *	clip_rect);
/*----------------------------------------------------------------------*/
extern void
_XfeButtonArmTimeout			(Widget			w,
								 XtPointer		client_data,
								 XtIntervalId *	id);
/*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------*/
/*																		*/
/* XfeButton action procedures											*/
/*																		*/
/*----------------------------------------------------------------------*/
extern void
_XfeButtonEnter					(Widget,XEvent *,char **,Cardinal *);
/*----------------------------------------------------------------------*/
extern void
_XfeButtonLeave					(Widget,XEvent *,char **,Cardinal *);
/*----------------------------------------------------------------------*/
extern void
_XfeButtonMotion				(Widget,XEvent *,char **,Cardinal *);
/*----------------------------------------------------------------------*/
extern void
_XfeButtonArm					(Widget,XEvent *,char **,Cardinal *);
/*----------------------------------------------------------------------*/
extern void
_XfeButtonActivate				(Widget,XEvent *,char **,Cardinal *);
/*----------------------------------------------------------------------*/
extern void
_XfeButtonDisarm				(Widget,XEvent *,char **,Cardinal *);
/*----------------------------------------------------------------------*/
extern void
_XfeButtonArmAndActivate		(Widget,XEvent *,char **,Cardinal *);
/*----------------------------------------------------------------------*/
extern void
_XfeButtonBtn3Down				(Widget,XEvent *,char **,Cardinal *);
/*----------------------------------------------------------------------*/
extern void
_XfeButtonBtn3Up				(Widget,XEvent *,char **,Cardinal *);
/*----------------------------------------------------------------------*/

XFE_END_CPLUSPLUS_PROTECTION

#endif											/* end ButtonP.h		*/
