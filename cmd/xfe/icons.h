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
/* */
/* icons.h --- icon creation stuff
   Created: Terry Weissman <terry@netscape.com>.
 */


#ifndef __xfe_icons_h_
#define __xfe_icons_h_

typedef struct fe_icon {
  Pixmap pixmap;
  Pixmap mask;
  Dimension width, height;
} fe_icon;

#ifdef __cplusplus
extern "C" {
#endif

void
fe_NewMakeIcon(Widget toplevel_widget, 
	       Pixel foreground_color,
	       Pixel transparent_color, fe_icon *result,
	       char *name, int width, int height, 
	       unsigned char *mono_data,
	       unsigned char *color_data,
	       unsigned char *mask_data,
	       Boolean hack_mask_and_cmap_p);

extern void
fe_MakeIcon(MWContext *context, Pixel transparent_color, fe_icon* result,
	    char *name,
	    int width, int height,
	    unsigned char *mono_data,
	    unsigned char *color_data,
	    unsigned char *mask_data,
	    Boolean hack_mask_and_cmap_p);

extern void fe_InitBookmarkIcons(MWContext *c, Widget outline);
extern void fe_InitMailNewsIcons(MWContext *c, Widget outline);
extern void fe_InitAddressBookIcons(MWContext *c, Widget outline);

#ifdef __cplusplus
}
#endif

#endif /* __xfe_icons_h_ */

