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
   BookmarkMenu.cpp -- class for doing the dynamic bookmark menus
   Created: Chris Toshok <toshok@netscape.com>, 19-Dec-1996.
 */



#include "BookmarkMenu.h"

#include <Xfe/BmButton.h>
#include <Xfe/BmCascade.h>

#include <Xfe/Cascade.h>

#include <Xm/CascadeB.h>
#include <Xm/CascadeBG.h>
#include <Xm/PushB.h>
#include <Xm/PushBG.h>

#define IS_CASCADE(w)	(XmIsCascadeButton(w) || XmIsCascadeButtonGadget(w))
#define IS_PUSH(w)		(XmIsPushButton(w) || XmIsPushButtonGadget(w))

//////////////////////////////////////////////////////////////////////////
XFE_BookmarkMenu::XFE_BookmarkMenu(Widget		cascade,
								   XFE_Frame *	frame,
								   XP_Bool		onlyHeaders,
								   XP_Bool		fancyItems) :
	XFE_RDFMenuToolbarBase(frame,onlyHeaders,fancyItems),
	_cascade(cascade),
	_subMenu(NULL),
	_firstSlot(0)
{
	// Obtain the submenu and the first available slot
	XtVaGetValues(_cascade,XmNsubMenuId,&_subMenu,NULL);
	XtVaGetValues(_subMenu,XmNnumChildren,&_firstSlot,NULL);

	// When the cascade is blown away, so are we
	XtAddCallback(_cascade,
				  XmNdestroyCallback,
				  &XFE_BookmarkMenu::destroy_cb,
				  (XtPointer) this);

	// make sure we initially install an update callback
 	XtAddCallback(_cascade,
				  XmNcascadingCallback,
				  &XFE_BookmarkMenu::update_cb,
				  (XtPointer) this);

	// Keep track of the submenu mapping
	trackSubmenuMapping(_subMenu);

    newBookmarksPane();
}
//////////////////////////////////////////////////////////////////////////
void
XFE_BookmarkMenu::generate(Widget		cascade, 
						   XtPointer	clientData,
						   XFE_Frame *	frame)
{
	XFE_BookmarkMenu * object;

	object = new XFE_BookmarkMenu(cascade,
								  frame,
								  (int) clientData,
								  True);
}
//////////////////////////////////////////////////////////////////////////
/* static */ void
XFE_BookmarkMenu::generateQuickfile(Widget		cascade, 
									XtPointer	clientData,
									XFE_Frame *	frame)
{
	XFE_BookmarkMenu * object;

	object = new XFE_BookmarkMenu(cascade,
								  frame,
								  (int) clientData,
								  True);

	// Store the BookmarkMenu instance in the quickfile button 
	// XmNinstancePointer.  This overrides the XmNinstancePointer 
	// installed by the XFE_Button class
	XtVaSetValues(cascade,XmNinstancePointer,(XtPointer) object,NULL);
}
//////////////////////////////////////////////////////////////////////////
/* static */ void
XFE_BookmarkMenu::destroy_cb(Widget		/* w */,
							 XtPointer	client_data,
							 XtPointer	/* call_data*/)
{
	XFE_BookmarkMenu * object = (XFE_BookmarkMenu *) client_data;

	delete object;
}
//////////////////////////////////////////////////////////////////////////
/* static */ void
XFE_BookmarkMenu::update_cb(Widget		cascade,
							XtPointer	client_data,
							XtPointer	/* call_data */)
{
	XFE_BookmarkMenu *	object = (XFE_BookmarkMenu *) client_data;
	Widget				subMenu;

	XtVaGetValues(cascade,XmNsubMenuId,&subMenu,NULL);

	// Really update
	object->updateRoot();

	// Make sure the submenu is realized
	XtRealizeWidget(subMenu);

	// Remove this callback now that we have been updated
	XtRemoveCallback(cascade,
					 XmNcascadingCallback,
					 &XFE_BookmarkMenu::update_cb,
					 (XtPointer) object);
}
//////////////////////////////////////////////////////////////////////////
/* virtual */ void
XFE_BookmarkMenu::prepareToUpdateRoot()
{
	// This may seem stupid, but it keeps us from having more than
	// one reference to this particular callback without having
	// to worry about other cascadingCallbacks.
	
	// remove it if it's already there
	XtRemoveCallback(_cascade,
					 XmNcascadingCallback,
					 &XFE_BookmarkMenu::update_cb,
					 (XtPointer) this);
	
	// and then add it back.
	XtAddCallback(_cascade,
				  XmNcascadingCallback,
				  &XFE_BookmarkMenu::update_cb,
				  (XtPointer) this);
}
//////////////////////////////////////////////////////////////////////////
/* virtual */ void
XFE_BookmarkMenu::updateRoot()
{
 	WidgetList		children;
 	Cardinal		numChildren;
 	HT_Resource		root = getMenuFolder();

#ifdef OLD_BOOKMARKS
	// Ignore the root header (ie, "Joe's Bookmarks")
	if (root && HT_IsContainer(root))
	{
		root = BM_GetChildren(root);
	}
#endif

 	XfeChildrenGet(_subMenu,&children,&numChildren);	
	
	//  XtUnrealizeWidget(m_subMenu);

 	// Get rid of the previous items we created
 	if (children && numChildren)
	{
 		children += _firstSlot;

 		numChildren -= _firstSlot;

 		if (children && numChildren)
		{
 			XtUnmanageChildren(children,numChildren);
      
 			XfeDestroyMenuWidgetTree(children,numChildren,False);
		}
	}

 	// Create the entries if any
 	if (root)
 	{
		// Make sure the "Joe's Bookmarks" cascade gets created for the
		// first item in the "File Bookmarks" hierarchy.
		if (getOnlyHeaders())
		{
			XtManageChild(createCascadeButton(_subMenu,getMenuFolder(),True));
			XtManageChild(createSeparator(_subMenu));
		}

 		createItemTree(_subMenu,root);
 	}
}
//////////////////////////////////////////////////////////////////////////
/* virtual */ void
XFE_BookmarkMenu::enableDropping()
{
	// Gurantee that the popup and items and created and realized or
	// else the setFixedSensitive() call will have no items to modify
	update_cb(_cascade,(XtPointer) this,(XtPointer) NULL);

	// Chain
	XFE_RDFMenuToolbarBase::enableDropping();

	// Make all the fixed items insensitive
	setFixedItemSensitive(False);
}
//////////////////////////////////////////////////////////////////////////
/* virtual */ void
XFE_BookmarkMenu::disableDropping()
{
	// Chain
	XFE_RDFMenuToolbarBase::disableDropping();

	// Make all the fixed items sensitive
	setFixedItemSensitive(True);
}
//////////////////////////////////////////////////////////////////////////
/* virtual */ void
XFE_BookmarkMenu::enableFiling()
{
	XfeBmAccentSetFileMode(XmACCENT_FILE_SELF);
	XfeBmAccentEnable();
}
//////////////////////////////////////////////////////////////////////////
/* virtual */ void
XFE_BookmarkMenu::disableFiling()
{
	XfeBmAccentSetFileMode(XmACCENT_FILE_ANYWHERE);
	XfeBmAccentDisable();
}
//////////////////////////////////////////////////////////////////////////
void
XFE_BookmarkMenu::setFixedItemSensitive(XP_Bool state)
{
 	WidgetList		children;
 	Cardinal		numChildren;

 	XfeChildrenGet(_subMenu,&children,&numChildren);	
	
 	// Make sure some fixed items exist
 	if (children && numChildren && _firstSlot && (_firstSlot < numChildren))
	{
		Cardinal i;

		// Set the sensitivity state for all the fixed push button items
		for (i = 0; i < _firstSlot; i++)
		{
			if (IS_PUSH(children[i]) || IS_CASCADE(children[i]))
			{
				XtSetSensitive(children[i],state);
			}
		}
	}
}
//////////////////////////////////////////////////////////////////////////
