/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*-
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
/*
  g-view.h -- views.h
  Created: Chris Toshok <toshok@hungry.com>, 9-Apr-98.
*/

#ifndef _moz_view_h
#define _moz_view_h

#include "structs.h"
#include "ntypes.h"

#include "g-frame.h"
#include "g-component.h"

struct _MozView {
  MozComponent _component;
  
  MWContext *context;

  MozFrame *parent_frame;
  MozView *parent_view;

  GtkWidget *subview_parent;
  GList *subviews;
};

extern void			moz_view_init(MozView *view, MozFrame *parent_frame, MWContext *context);
extern void			moz_view_deinit(MozView *view);

extern void			moz_view_add_view(MozView *parent_view, MozView *child);

extern void			moz_view_set_context(MozView *view, MWContext *context);
extern MWContext*	moz_view_get_context(MozView *view);

#endif /* _moz_view_h */
