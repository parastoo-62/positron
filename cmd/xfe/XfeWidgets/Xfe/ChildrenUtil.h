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
/* Name:		<Xfe/ChildrenUtil.h>									*/
/* Description:	Children misc utilities header.							*/
/* Author:		Ramiro Estrugo <ramiro@netscape.com>					*/
/*																		*/
/*----------------------------------------------------------------------*/

#ifndef _XfeChildrenUtil_h_						/* start ChildrenUtil.h	*/
#define _XfeChildrenUtil_h_

#include <X11/Intrinsic.h>						/* Xt public defs		*/

XFE_BEGIN_CPLUSPLUS_PROTECTION

/*----------------------------------------------------------------------*/
/*																		*/
/* Children access														*/
/*																		*/
/*----------------------------------------------------------------------*/
extern Cardinal		XfeNumChildren			(Widget w);
extern WidgetList	XfeChildren				(Widget w);
extern Widget		XfeChildrenIndex		(Widget w,Cardinal i);
extern Cardinal		XfeChildrenCountAlive	(Widget w);
/*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------*/
/*																		*/
/* Popup children access												*/
/*																		*/
/*----------------------------------------------------------------------*/
extern Cardinal		XfeNumPopups			(Widget w);
extern WidgetList	XfePopupList			(Widget w);
extern Widget		XfePopupListIndex		(Widget w,Cardinal i);
extern Cardinal		XfePopupListCountAlive	(Widget w);
/*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------*/
/*																		*/
/* Children access														*/
/*																		*/
/*----------------------------------------------------------------------*/
extern void
XfeChildrenGet				(Widget			w,
							 WidgetList *	children,
							 Cardinal *		num_children);
/*----------------------------------------------------------------------*/
extern Cardinal
XfeChildrenGetNumManaged	(Widget			w);
/*----------------------------------------------------------------------*/
extern Widget
XfeChildrenGetLast			(Widget			w);
/*----------------------------------------------------------------------*/
extern int
XfeChildGetIndex			(Widget			w);
/*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------*/
/*																		*/
/* Destroy																*/
/*																		*/
/*----------------------------------------------------------------------*/
extern void
XfeChildrenDestroy			(Widget			w);
/*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------*/
/*																		*/
/* Callbacks / Event handlers											*/
/*																		*/
/*----------------------------------------------------------------------*/
extern void
XfeChildrenAddCallback		(Widget			w,
							 String			callback_name,
							 XtCallbackProc	callback,
							 XtPointer		data);
/*----------------------------------------------------------------------*/
extern void
XfeChildrenAddEventHandler	(Widget			w,
							 EventMask		event_mask,
							 Boolean		nonmaskable,
							 XtEventHandler	proc,
							 XtPointer		data);
/*----------------------------------------------------------------------*/
extern void
XfeChildrenRemoveCallback	(Widget			w,
							 String			callback_name,
							 XtCallbackProc	callback,
							 XtPointer		data);
/*----------------------------------------------------------------------*/
extern void
XfeChildrenRemoveEventHandler(Widget			w,
							  EventMask			event_mask,
							  Boolean			nonmaskable,
							  XtEventHandler	proc,
							  XtPointer			data);
/*----------------------------------------------------------------------*/



XFE_END_CPLUSPLUS_PROTECTION

#endif											/* end ChildrenUtil.h	*/
