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
/* Name:		<Xfe/ProgressBarP.h>									*/
/* Description:	XfeProgressBar widget private header file.				*/
/* Author:		Ramiro Estrugo <ramiro@netscape.com>					*/
/*																		*/
/*----------------------------------------------------------------------*/

#ifndef _XfeProgressBarP_h_					/* start ProgressBarP.h		*/
#define _XfeProgressBarP_h_

#include <Xfe/ProgressBar.h>
#include <Xfe/LabelP.h>

XFE_BEGIN_CPLUSPLUS_PROTECTION

/*----------------------------------------------------------------------*/
/*																		*/
/* XfeProgressBarClassRec												*/
/*																		*/
/*----------------------------------------------------------------------*/
typedef struct
{
    XtPointer		extension;					/* Extension			*/
} XfeProgressBarClassPart;

/*----------------------------------------------------------------------*/
/*																		*/
/* XfeProgressBarPart													*/
/*																		*/
/*----------------------------------------------------------------------*/
typedef struct _XfeProgressBarClassRec
{
    CoreClassPart			core_class;				/* Core class		*/
    XmPrimitiveClassPart	primitive_class;		/* XmPrimitive		*/
    XfePrimitiveClassPart	xfe_primitive_class;	/* XfePrimitive		*/
    XfeLabelClassPart		xfe_label_class;		/* XfeLabel			*/
    XfeProgressBarClassPart	xfe_progress_bar_class;	/* XfeProgressBar	*/
} XfeProgressBarClassRec;

externalref XfeProgressBarClassRec xfeProgressBarClassRec;

/*----------------------------------------------------------------------*/
/*																		*/
/* XfeProgressBarRec													*/
/*																		*/
/*----------------------------------------------------------------------*/
typedef struct _XfeProgressBarPart
{
    /* Callback resources */
    Pixel				bar_color;				/* Bar color			*/
    int					start_percent;			/* Start percent		*/
    int					end_percent;			/* End percent			*/

	int					cylon_interval;			/* Cylon interval		*/
	int					cylon_offset;			/* Cylon offset			*/
	Boolean				cylon_running;			/* Cylon running		*/
	int					cylon_width;			/* Cylon width			*/
    
    /* Private Data Members */
    XRectangle			bar_rect;				/* Bar rectangle		*/
    GC					bar_GC;					/* Bar gc				*/
    GC					bar_insens_GC;			/* Bar insensitive gc	*/

    /* Private Data Members */
	XtIntervalId		timer_id;				/* Timer id				*/
	Boolean				cylon_direction;		/* Cylon direction		*/
	int					cylon_index;			/* Cylon index			*/

} XfeProgressBarPart;

/*----------------------------------------------------------------------*/
/*																		*/
/* XfeProgressBarPart													*/
/*																		*/
/*----------------------------------------------------------------------*/
typedef struct _XfeProgressBarRec
{
    CorePart			core;					/* Core Part			*/
    XmPrimitivePart		primitive;				/* XmPrimitive Part		*/
    XfePrimitivePart	xfe_primitive;			/* XfePrimitive Part	*/
    XfeLabelPart		xfe_label;				/* XfeLabel Part		*/
    XfeProgressBarPart	xfe_progress_bar;		/* XfeProgressBar Part	*/
} XfeProgressBarRec;

/*----------------------------------------------------------------------*/
/*																		*/
/* XfeProgressBarPart Access Macro										*/
/*																		*/
/*----------------------------------------------------------------------*/
#define _XfeProgressBarPart(w) &(((XfeProgressBarWidget) w) -> xfe_progress_bar)

XFE_END_CPLUSPLUS_PROTECTION

#endif											/* end ProgressBarP.h	*/
