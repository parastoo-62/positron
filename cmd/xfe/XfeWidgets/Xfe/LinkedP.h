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
/* Name:		<Xfe/LinkedP.h>											*/
/* Description:	XfeLinked object tweaked to be used in widgets.			*/
/* Author:		Ramiro Estrugo <ramiro@netscape.com>					*/
/*																		*/
/*----------------------------------------------------------------------*/

#ifndef _XfeLinkedP_h_							/* LinkedP.h			*/
#define _XfeLinkedP_h_

#include <Xfe/Linked.h>

XFE_BEGIN_CPLUSPLUS_PROTECTION

/*----------------------------------------------------------------------*/
/*																		*/
/* XfeLinkNodeRec														*/
/*																		*/
/*----------------------------------------------------------------------*/
typedef struct _XfeLinkNodeRec
{
   Cardinal						index;		/* Index of current node	*/
   XtPointer					item;		/* User's Data				*/
   struct _XfeLinkNodeRec *		next;		/* Next Item in List		*/
   struct _XfeLinkNodeRec *		prev;		/* Previous Item in List	*/
} XfeLinkNodeRec;

/*----------------------------------------------------------------------*/
/*																		*/
/* XfeLinkedRec															*/
/*																		*/
/*----------------------------------------------------------------------*/
typedef struct _XfeLinkedRec
{
   XfeLinkNodeRec *				data;		/* Linked list of items		*/
   XfeLinkNodeRec *				head;		/* Index of head bucket		*/
   XfeLinkNodeRec *				tail;		/* Index of tail bucket		*/
   Cardinal						count;		/* Item Count				*/
} XfeLinkedRec; 

XFE_END_CPLUSPLUS_PROTECTION

#endif /* LinkedP.h */

