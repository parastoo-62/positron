/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*-
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
   EditorFrame.h -- class definition for the editor frame class
   Created: Richard Hess <rhess@netscape.com>, 11-Nov-96
 */



#ifndef _xfe_editorframe_h
#define _xfe_editorframe_h

#include "Frame.h"
#include "EditorToolbar.h"
#include "Dashboard.h"
#include "PopupMenu.h"
#include "xp_core.h"
#include <Xm/Xm.h>

class XFE_EditorFrame : public XFE_Frame
{
public:
  XFE_EditorFrame(Widget toplevel, XFE_Frame *parent_frame, Chrome *);

  virtual int getURL(URL_Struct *url);
  virtual MWContext* getContext();

  virtual XP_Bool isCommandEnabled(CommandType cmd, void *calldata = NULL,
						 XFE_CommandInfo* = NULL);
  virtual void    doCommand(CommandType cmd, void *calldata = NULL,
						 XFE_CommandInfo* = NULL);
  virtual XP_Bool handlesCommand(CommandType cmd, void *calldata = NULL,
						 XFE_CommandInfo* = NULL);
  virtual char*   commandToString(CommandType cmd, void *calldata = NULL,
						 XFE_CommandInfo* = NULL);
  virtual XFE_Command* getCommand(CommandType);
  virtual Boolean XFE_EditorFrame::isCommandSelected(CommandType cmd, 
								  void* calldata, XFE_CommandInfo* info);

  // void    delete_response();
  XP_Bool isOkToClose();
  virtual void	doClose();

public:
  // Edit submenus
  static MenuSpec delete_table_menu_spec[];
  // insert menu
  static MenuSpec insert_menu_spec[];
  // Format menu & submenus
  static MenuSpec format_menu_spec[];
  static MenuSpec character_style_menu_spec[];
  static MenuSpec character_size_menu_spec[];
  static MenuSpec heading_style_menu_spec[];
  static MenuSpec paragraph_style_menu_spec[];
  static MenuSpec list_style_menu_spec[];
  static MenuSpec alignment_style_menu_spec[];

  // Table menu & submenus
  static MenuSpec table_menu_spec[];
  static MenuSpec table_select_submenu_spec[];
  static MenuSpec insert_table_menu_spec[];

  static MenuSpec tools_menu_spec[];

  static MenuSpec new_submenu_spec[];
  static MenuSpec save_submenu_spec[];
#if defined(MOZ_MAIL_NEWS) || defined(MOZ_MAIL_COMPOSE)
  static MenuSpec publish_submenu_spec[];
#endif

  // alignment menu spec needs to be public so BrowserFrame can
  // use it for embedded editors:
  static ToolbarSpec alignment_menu_spec[];

private:
  XFE_EditorToolbar* m_format_toolbar;

  static MenuSpec menu_bar_spec[];
  static MenuSpec file_menu_spec[];
  static MenuSpec edit_menu_spec[];
  static MenuSpec view_menu_spec[];

  static MenuSpec go_menu_spec[];
  static MenuSpec bookmark_menu_spec[];

  static MenuSpec encoding_menu_spec[];

  // static MenuSpec navigate_menu_spec[];
  // static MenuSpec help_menu_spec[];

//  static ToolbarSpec toolbar_spec[];

  XFE_CALLBACK_DECL(navigateToURL) // URL_Struct is sent in callData
  XFE_CALLBACK_DECL(newPageLoading) // URL_Struct is sent in callData


protected:
  virtual void			configureLogo	();
  virtual XFE_Logo *	getLogo			();

  // Toolbox methods
  virtual void		toolboxItemSnap			(XFE_ToolboxItem * item);
  virtual void		toolboxItemClose		(XFE_ToolboxItem * item);
  virtual void		toolboxItemOpen		(XFE_ToolboxItem * item);
  virtual void		toolboxItemChangeShowing(XFE_ToolboxItem * item);

  virtual void		configureToolbox	();

};

MenuSpec* fe_EditorInstanciateMenu(XFE_Frame* frame, MenuSpec* spec);
XFE_PopupMenu* fe_EditorNewPopupMenu(XFE_Frame*, Widget, MWContext*);

#endif /* _xfe_editorframe_h */
