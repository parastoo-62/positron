/* -*- Mode: C; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*-
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
/*---------------------------------------*/
/*                                                                      */
/* Name:        AnimationType.h                                         */
/* Description: EAnimationType component header file.                   */
/* Created:     Daniel Malmer <malmer@netscape.com>                     */
/*                                                                      */
/* Since Logo.h and e_kit.c both need this enum type.                   */
/*----------------------------------------------------------------------*/


#ifndef _xfe_animation_type_h
#define _xfe_animation_type_h

/*
 * Animation Type
 * Make sure these match the order in xfe/icons/Makefile.
*/
typedef enum
{
    XFE_ANIMATION_MAIN = 0,
#ifdef NETSCAPE_PRIV
    XFE_ANIMATION_COMPASS,
    XFE_ANIMATION_MOZILLA,
#endif /* NETSCAPE_PRIV */
    XFE_ANIMATION_CUSTOM,
    XFE_ANIMATION_MAX
} EAnimationType;

#endif
