/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 4 -*-
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
   ComposeFrame.cpp -- Compose window stuff
   Created: Dora Hsu <dora@netscape.com>, 23-Sept-96.
 */


#include "rosetta.h"
#include "ComposeFrame.h"
#include "EditorFrame.h"
#include "LdapSearchFrame.h"
#include "ComposeFolderView.h"
#include "AddressFolderView.h"
#include "ComposeView.h"
#include "TextEditorView.h"
#include "Command.h"
#include "xpassert.h"
#include "ViewGlue.h"
#include "Dashboard.h"
#include "Xfe/Xfe.h"
#include "edt.h"
#include "msgcflds.h"
#include "csid.h"
#include "msgcom.h"
#include <xpgetstr.h>     /* for XP_GetString() */

extern "C" int FE_GetMessageBody (MSG_Pane* comppane,
                   char **body,
                   uint32 *body_size,
                   MSG_FontCode **font_changes);

extern int XFE_MNC_CLOSE_WARNING;

#ifdef DEBUG_dora
#define XDEBUG(x) x
#else
#define XDEBUG(x)
#endif

extern "C" {
  void xfe2_EditorInit(MWContext* context);
  void fe_sec_logo_cb (Widget widget, XtPointer closure, XtPointer call_data);
  int fe_YesNoCancelDialog(MWContext* context, char* name, char* message);
  void fe_set_scrolled_default_size(MWContext *context);
}


// Compose Encoding Menu Spec - no longer shared between Browsers, and Mail/News
MenuSpec XFE_ComposeFrame::encoding_menu_spec[] = {
	{ xfeCmdChangeDocumentEncoding,	TOGGLEBUTTON, NULL, "EncodingRadioGroup", False, (void*)CS_LATIN1 },
	MENU_SEPARATOR,
	{ xfeCmdChangeDocumentEncoding,	TOGGLEBUTTON, NULL, "EncodingRadioGroup", False, (void*)CS_LATIN2 },
	{ xfeCmdChangeDocumentEncoding,	TOGGLEBUTTON, NULL, "EncodingRadioGroup", False, (void*)CS_CP_1250 },
	MENU_SEPARATOR,
	{ xfeCmdChangeDocumentEncoding,	TOGGLEBUTTON, NULL, "EncodingRadioGroup", False, (void*)CS_JIS },
	MENU_SEPARATOR,
	{ xfeCmdChangeDocumentEncoding,	TOGGLEBUTTON, NULL, "EncodingRadioGroup", False, (void*)CS_BIG5 },
	MENU_SEPARATOR,
	{ xfeCmdChangeDocumentEncoding,	TOGGLEBUTTON, NULL, "EncodingRadioGroup", False, (void*)CS_GB_8BIT },
	MENU_SEPARATOR,
	{ xfeCmdChangeDocumentEncoding,	TOGGLEBUTTON, NULL, "EncodingRadioGroup", False, (void*)CS_KSC_8BIT },
	{ xfeCmdChangeDocumentEncoding,	TOGGLEBUTTON, NULL, "EncodingRadioGroup", False, (void*)CS_2022_KR },
	MENU_SEPARATOR,
	{ xfeCmdChangeDocumentEncoding,	TOGGLEBUTTON, NULL, "EncodingRadioGroup", False, (void*)CS_8859_5 },
	{ xfeCmdChangeDocumentEncoding,	TOGGLEBUTTON, NULL, "EncodingRadioGroup", False, (void*)CS_KOI8_R },
	{ xfeCmdChangeDocumentEncoding,	TOGGLEBUTTON, NULL, "EncodingRadioGroup", False, (void*)CS_CP_1251 },
	MENU_SEPARATOR,
	{ xfeCmdChangeDocumentEncoding,	TOGGLEBUTTON, NULL, "EncodingRadioGroup", False, (void*)CS_KOI8_U },
	MENU_SEPARATOR,
	{ xfeCmdChangeDocumentEncoding,	TOGGLEBUTTON, NULL, "EncodingRadioGroup", False, (void*)CS_ARMSCII8 },
	MENU_SEPARATOR,
	{ xfeCmdChangeDocumentEncoding,	TOGGLEBUTTON, NULL, "EncodingRadioGroup", False, (void*)CS_8859_7 },
	{ xfeCmdChangeDocumentEncoding,	TOGGLEBUTTON, NULL, "EncodingRadioGroup", False, (void*)CS_CP_1253 },
	MENU_SEPARATOR,
	{ xfeCmdChangeDocumentEncoding,	TOGGLEBUTTON, NULL, "EncodingRadioGroup", False, (void*)CS_8859_9 },
	MENU_SEPARATOR,
	{ xfeCmdChangeDocumentEncoding,	TOGGLEBUTTON, NULL, "EncodingRadioGroup", False, (void*)CS_TIS620 },
	MENU_SEPARATOR,
	{ xfeCmdChangeDocumentEncoding,	TOGGLEBUTTON, NULL, "EncodingRadioGroup", False, (void*)CS_UTF8 },
	{ xfeCmdChangeDocumentEncoding,	TOGGLEBUTTON, NULL, "EncodingRadioGroup", False, (void*)CS_UTF7 },
	MENU_SEPARATOR,
	{ xfeCmdChangeDocumentEncoding,	TOGGLEBUTTON, NULL, "EncodingRadioGroup", False, (void*)CS_USRDEF2 },
	{ NULL }
};
MenuSpec XFE_ComposeFrame::message_attach_menu_spec[] = {
  { xfeCmdAttachFile,			PUSHBUTTON },
  { xfeCmdAttachWebPage,		PUSHBUTTON },
  //  { xfeCmdDeleteAttachment,		PUSHBUTTON },
  MENU_SEPARATOR,
  { xfeCmdAttachAddressBookCard,	TOGGLEBUTTON },
  { NULL }
};

MenuSpec XFE_ComposeFrame::file_menu_spec[] = {
  { "newSubmenu",		CASCADEBUTTON,
	(MenuSpec*)&XFE_Frame::new_menu_spec },
  { xfeCmdSaveAsMenu,   CASCADEBUTTON,
    (MenuSpec*)&XFE_ComposeFrame::saveas_spec},
  { xfeCmdAttach,		CASCADEBUTTON,
    (MenuSpec*)&XFE_ComposeFrame::message_attach_menu_spec},
  MENU_SEPARATOR,
  { xfeCmdSendMessageNow,		PUSHBUTTON },
  { xfeCmdSendMessageLater,		PUSHBUTTON },
  { xfeCmdQuoteOriginalText,	PUSHBUTTON},
  { xfeCmdAddresseePicker,	PUSHBUTTON},
  //  MENU_SEPARATOR,
  //{ xfeCmdPrintSetup,		PUSHBUTTON },
  //{ xfeCmdPrintPreview,		PUSHBUTTON },
  //{ xfeCmdPrint,		PUSHBUTTON },
  MENU_SEPARATOR,
  { xfeCmdClose,		PUSHBUTTON },
  { xfeCmdExit,		PUSHBUTTON },
  { NULL }
};

MenuSpec XFE_ComposeFrame::saveas_spec[] = {
  { xfeCmdSaveAs,		PUSHBUTTON },
  MENU_SEPARATOR,
  { xfeCmdSaveDraft,		PUSHBUTTON },
  { xfeCmdSaveTemplate,     PUSHBUTTON },
  { NULL }
};

MenuSpec XFE_ComposeFrame::edit_menu_spec[] = {
  { xfeCmdUndo,			PUSHBUTTON },
  //  { xfeCmdRedo,			PUSHBUTTON },
  MENU_SEPARATOR,
  { xfeCmdCut,			PUSHBUTTON },
  { xfeCmdCopy,			PUSHBUTTON },
  { xfeCmdPaste,		PUSHBUTTON },
  { xfeCmdPasteAsQuoted,	PUSHBUTTON },
  //  { xfeCmdDeleteItem,		PUSHBUTTON },
  { xfeCmdSelectAll,		PUSHBUTTON },
  MENU_SEPARATOR,
  { xfeCmdFindInObject,		PUSHBUTTON },
  { xfeCmdFindAgain,		PUSHBUTTON },
  { xfeCmdSearchAddress,	PUSHBUTTON },
  MENU_SEPARATOR,
  { xfeCmdEditPreferences,	PUSHBUTTON },
  { NULL }
};

static char view_radio_group_name[] = "viewRadiogroup";

#define RADIOBUTTON TOGGLEBUTTON, NULL

MenuSpec XFE_ComposeFrame::view_menu_spec[] = {
  { xfeCmdShowChrome,		CASCADEBUTTON,
    (MenuSpec*)&XFE_ComposeFrame::show_chrome_spec},
  MENU_SEPARATOR,
  { xfeCmdWrapLongLines,    TOGGLEBUTTON},
  { NULL }
};

MenuSpec XFE_ComposeFrame::show_chrome_spec[] = {

    { xfeCmdToggleNavigationToolbar, PUSHBUTTON },
    { xfeCmdToggleAddressArea, PUSHBUTTON },
    MENU_SEPARATOR,
    { xfeCmdFloatingTaskBarClose, PUSHBUTTON },
    MENU_SEPARATOR,
    { xfeCmdViewAddresses,	RADIOBUTTON, view_radio_group_name },
    { xfeCmdViewAttachments,	RADIOBUTTON, view_radio_group_name },
    { xfeCmdViewOptions,	    RADIOBUTTON, view_radio_group_name },
    MENU_SEPARATOR,
    { xfeCmdToggleParagraphMarks, PUSHBUTTON }, /* with toggling label */
    { NULL }

};

static MenuSpec text_tools_menu_spec[] = {
	{ xfeCmdSpellCheck,	PUSHBUTTON },
	{ NULL }
};

MenuSpec XFE_ComposeFrame::menu_bar_spec[] = {
  { xfeMenuFile, 	CASCADEBUTTON, (MenuSpec*)&XFE_ComposeFrame::file_menu_spec },
  { xfeMenuEdit, 	CASCADEBUTTON, (MenuSpec*)&XFE_ComposeFrame::edit_menu_spec },
  { xfeMenuView, 	CASCADEBUTTON, (MenuSpec*)&XFE_ComposeFrame::view_menu_spec },
  { xfeMenuTools, 	CASCADEBUTTON, (MenuSpec*)&text_tools_menu_spec },
  { "bookmarksSubmenu",	CASCADEBUTTON, XFE_Frame::bookmark_submenu_spec },
  { xfeMenuWindow, 	CASCADEBUTTON, (MenuSpec*)&XFE_Frame::window_menu_spec },
  { xfeMenuHelp,        CASCADEBUTTON, (MenuSpec*)&XFE_Frame::help_menu_spec},
  { NULL }
};

ToolbarSpec XFE_ComposeFrame::toolbar_spec[] = {
  { xfeCmdSendMessageNow,	PUSHBUTTON, &MNC_Send_group },
  { xfeCmdQuote,		PUSHBUTTON, &MNC_Quote_group },
  { xfeCmdAddresseePicker,		PUSHBUTTON, &MNC_Address_group },
  {
	  xfeCmdAttach,
	  CASCADEBUTTON, 
	  &MNTB_Next_group, NULL, NULL, NULL,					// Icons
	  message_attach_menu_spec,								// Submenu spec
	  NULL, NULL,											// Generate proc/arg
	  XFE_TOOLBAR_DELAY_SHORT								// Popup delay
  },
  { xfeCmdSpellCheck,		PUSHBUTTON, &MNC_SpellCheck_group },
  { xfeCmdSaveDraft,		PUSHBUTTON, &MNC_Save_group },
  TOOLBAR_SEPARATOR,
#ifdef MOZ_MAIL_NEWS
  HG82981
#endif
  { xfeCmdStopLoading,		PUSHBUTTON, &TB_Stop_group },
  { NULL }
};

MenuSpec XFE_ComposeFrame::html_edit_menu_spec[] = {
  { xfeCmdUndo,			PUSHBUTTON },
  //  { xfeCmdRedo,			PUSHBUTTON },
  MENU_SEPARATOR,
  { xfeCmdCut,			PUSHBUTTON },
  { xfeCmdCopy,			PUSHBUTTON },
  { xfeCmdPaste,		PUSHBUTTON },
  { xfeCmdPasteAsQuoted,	PUSHBUTTON },
  //  { xfeCmdDeleteItem,		PUSHBUTTON },
  { xfeCmdSelectAll,	PUSHBUTTON },
  MENU_SEPARATOR,
  { "deleteTableMenu",	CASCADEBUTTON,
	(MenuSpec*)&XFE_EditorFrame::delete_table_menu_spec },
  { xfeCmdRemoveLink,	PUSHBUTTON },
  MENU_SEPARATOR,
  { xfeCmdSelectTable,	PUSHBUTTON },
  MENU_SEPARATOR,
  { xfeCmdFindInObject,	PUSHBUTTON },
  { xfeCmdFindAgain,	PUSHBUTTON },
#if 0
  /* temporary take out
   */
  { xfeCmdSearchAddress,	PUSHBUTTON },
#endif
  MENU_SEPARATOR,
  { xfeCmdEditPreferences,PUSHBUTTON },
  { NULL }
};

MenuSpec XFE_ComposeFrame::html_view_menu_spec[] = {
  { xfeCmdShowChrome, CASCADEBUTTON,
    (MenuSpec*)&XFE_ComposeFrame::html_show_chrome_spec },
  MENU_SEPARATOR,
  { xfeCmdViewPageSource,	PUSHBUTTON },
  { xfeCmdViewPageInfo,		PUSHBUTTON },
  MENU_SEPARATOR,
  { "encodingSubmenu",		CASCADEBUTTON,
	(MenuSpec*)&XFE_ComposeFrame::encoding_menu_spec },
  { NULL }
};

MenuSpec XFE_ComposeFrame::html_show_chrome_spec[] = {
    { xfeCmdToggleNavigationToolbar, PUSHBUTTON },
    { xfeCmdToggleAddressArea, PUSHBUTTON },
    { xfeCmdToggleFormatToolbar, PUSHBUTTON },
    MENU_SEPARATOR,
    { xfeCmdFloatingTaskBarClose, PUSHBUTTON },
    MENU_SEPARATOR,
    { xfeCmdViewAddresses,	RADIOBUTTON, view_radio_group_name },
    { xfeCmdViewAttachments,	RADIOBUTTON, view_radio_group_name },
    { xfeCmdViewOptions,	    RADIOBUTTON, view_radio_group_name },
    MENU_SEPARATOR,
    { xfeCmdToggleParagraphMarks, PUSHBUTTON }, /* with toggling label */
  { NULL }
};

MenuSpec XFE_ComposeFrame::html_menu_bar_spec[] = {
  { xfeMenuFile, CASCADEBUTTON,
	(MenuSpec*)&XFE_ComposeFrame::file_menu_spec },
  { xfeMenuEdit, 	CASCADEBUTTON, (MenuSpec*)&html_edit_menu_spec },
  { xfeMenuView, 	CASCADEBUTTON, (MenuSpec*)&html_view_menu_spec },
  { xfeMenuInsert,  CASCADEBUTTON,
	(MenuSpec*)&XFE_EditorFrame::insert_menu_spec },
  { xfeMenuFormat,  CASCADEBUTTON,
	(MenuSpec*)&XFE_EditorFrame::format_menu_spec },
  { xfeMenuTools, 	CASCADEBUTTON,
	(MenuSpec*)&XFE_EditorFrame::tools_menu_spec },
  { xfeMenuWindow, 	CASCADEBUTTON, (MenuSpec*)&XFE_Frame::window_menu_spec },
  { "bookmarksSubmenu",	CASCADEBUTTON, XFE_Frame::bookmark_submenu_spec },
  { xfeMenuHelp, 	CASCADEBUTTON, (MenuSpec*)&XFE_Frame::help_menu_spec },
  { NULL }
};

static unsigned html_menu_bar_initialized = 0;

XFE_ComposeFrame::XFE_ComposeFrame(Widget toplevel, 
        Chrome *chromespec,
	MWContext *old_context, MSG_CompositionFields *fields,
	const char *draftInitialText, XP_Bool preferToUseHtml)  
	:XFE_Frame("Composition", 
			   toplevel,
			   old_context ? ViewGlue_getFrame(old_context) : 0,
			   FRAME_MAILNEWS_COMPOSE, chromespec, True, True, True, True, True),
	m_destroyWhenConnectionsComplete(False)
{
XDEBUG(	printf ("in XFE_ComposeFrame::XFE_ComposeFrame()\n");)
  geometryPrefName = "mail.compose";

  // create the compose view
  XFE_ComposeView *view = new XFE_ComposeView(this, 
                                              getChromeParent(), 
                                              NULL, 
                                              m_context, 
											  old_context, 
											  fields, 
											  NULL,
											  draftInitialText, 
											  preferToUseHtml);

  setView(view);

  view->show();

  if (preferToUseHtml) {
	  if (!html_menu_bar_initialized) {
		  fe_EditorInstanciateMenu(this, html_menu_bar_spec);
		  html_menu_bar_initialized++;
	  }

	  fe_set_scrolled_default_size(m_context);

	  setMenubar(html_menu_bar_spec);
  } else {
	  setMenubar(menu_bar_spec);
  }

  setToolbar(toolbar_spec);
  

  // Configure the dashboard
  XP_ASSERT( m_dashboard != NULL );

  HG87288
  m_dashboard->setShowStatusBar(True);
  m_dashboard->setShowProgressBar(True);

  if (old_context && CONTEXT_DATA(old_context)->stealth_cmd) {
#ifdef DEBUG_rhess
	  fprintf(stderr, "WARNING... [ stealth_cmd ]\n");
#endif
	  CONTEXT_DATA(old_context)->stealth_cmd = False;
	  CONTEXT_DATA(m_context)->stealth_cmd = False;
  }

  // Configure the toolbox for the first time
  configureToolbox();


XDEBUG(	printf ("leaving XFE_ComposeFrame::XFE_ComposeFrame()\n");)
}


XFE_ComposeFrame::~XFE_ComposeFrame()
{
XDEBUG(	printf ("in XFE_ComposeFrame::~XFE_ComposeFrame()\n");)
	if (!m_destroyWhenConnectionsComplete)
		((XFE_ComposeView*)m_view)->destroyPane();

	XDEBUG(	printf ("leaving XFE_ComposeFrame::~XFE_ComposeFrame()\n");)
}

void
XFE_ComposeFrame::allConnectionsComplete(MWContext  */* context */)
{
  MSG_Pane *pane = getPane();

  if (!pane) return; // when pane is NULL, don't do anything anymore

  // BAD here... Backend will delete the pane inside 
  // MSG_MailCompositionAllConnectionsComplete

  MSG_MailCompositionAllConnectionsComplete(pane); 

  if (m_destroyWhenConnectionsComplete)
    {
		XDEBUG( printf ("All connections are complete.  Bye bye\n");)

		// So, when backend delete the pane, we should set
		// our pane to NULL too so that we will not be
		// checking on an invalid pane
		XDEBUG( printf("XFE_ComposeFrame::allConnectionsComplete()\n");)

		if (m_view)
			((XFE_ComposeView*)m_view)->setPane(NULL);
		delete_response();
    }
  else
    {
		XDEBUG( printf ("All connections are complete.  Notifying interested parties\n");)
		notifyInterested(XFE_Frame::allConnectionsCompleteCallback);
    }
}

void
XFE_ComposeFrame::destroyWhenAllConnectionsComplete()
{
	XDEBUG (printf ("****I'm going to destroy myself when all connections are complete\n");)
	m_destroyWhenConnectionsComplete = True;
	XDEBUG (printf("XFE_ComposeFrame::destroyWhenAllConnectionsComplete()\n");)
	if (m_view)
		((XFE_ComposeView*)m_view)->setPane(NULL);
	if (m_view && ((XFE_ComposeView *)m_view)->isDelayedSent() )
		{
			XDEBUG( printf ("All connections are complete.  Bye bye\n"); )

			delete_response();
		}
}

MSG_Pane*
XFE_ComposeFrame::getPane()
{
  return (m_view ? ((XFE_ComposeView *)m_view)->getPane(): 0);
}


XP_Bool
XFE_ComposeFrame::isCommandEnabled(CommandType cmd, void *cd, XFE_CommandInfo* info)
{
  if ( HG82111
       || (cmd == xfeCmdSearchAddress) 
       || cmd == xfeCmdAddresseePicker )
	return True;
  else {
	  // NOTE... [ we need to intercept these commands in order to be
	  //         [ able to do the "right thing" in the ComposeView
	  //
	  if ( cmd == xfeCmdCut   ||
		   cmd == xfeCmdCopy  ||
		   cmd == xfeCmdPaste ||
		   cmd == xfeCmdDeleteItem ||
		   cmd == xfeCmdUndo
		   ) {
		  return m_view->isCommandEnabled(cmd, cd, info);
	  }
	  else 
		  return XFE_Frame::isCommandEnabled(cmd, cd, info);
  }
}

void
XFE_ComposeFrame::doCommand(CommandType cmd, void *cd, XFE_CommandInfo* info)
{
#ifdef MOZ_MAIL_NEWS
    if (cmd == xfeCmdSearchAddress)
    {
        fe_showLdapSearch(XfeAncestorFindApplicationShell(getBaseWidget()), this,
                          (Chrome*)NULL);
    }
	// NOTE... [ we need to intercept these commands in order to be
	//         [ able to do the "right thing" in the ComposeView
	//
	else
#endif /* MOZ_MAIL_NEWS */
        {
		if ( cmd == xfeCmdPaste ||
			 cmd == xfeCmdCut   ||
			 cmd == xfeCmdCopy  ||
			 cmd == xfeCmdDeleteItem ||
			 cmd == xfeCmdUndo
			 ) {
			m_view->doCommand(cmd, cd, info);
		}
		else
			XFE_Frame::doCommand(cmd, cd, info);
	}
}

XP_Bool
XFE_ComposeFrame::handlesCommand(CommandType cmd, void *, XFE_CommandInfo*)
{
  if ( HG92828
       cmd == xfeCmdSearchAddress   ||
       cmd == xfeCmdAddresseePicker ||
	   cmd == xfeCmdCut             ||
	   cmd == xfeCmdCopy            ||
	   cmd == xfeCmdPaste           ||
	   cmd == xfeCmdDeleteItem      ||
	   cmd == xfeCmdUndo
	   )
  {
	return True;
  }
  else
  	return XFE_Frame::handlesCommand(cmd);
}

XFE_Command*
XFE_ComposeFrame::getCommand(CommandType cmd)
{
	if (m_view)
		return m_view->getCommand(cmd);
	else
		return XFE_Frame::getCommand(cmd);
}

int
XFE_ComposeFrame::initEditor()
{
  XFE_ComposeView *c_view = (XFE_ComposeView*)m_view;

  return c_view->initEditor();
}

extern "C" MSG_Pane *
fe_showCompose(Widget toplevel, Chrome *chromespec, MWContext *old_context, 
	       MSG_CompositionFields* fields,
	       const char * draftInitialText,
	       MSG_EditorType editorType,
               XP_Bool defaultAddressIsNewsgroup)
{
	// not a static global, since we can have multiple editors.
	XFE_ComposeFrame *theFrame = NULL;
  
	XDEBUG(	printf ("in fe_showCompose()\n");)

	XP_Bool useHtml;

        if (editorType == MSG_HTML_EDITOR) {
            useHtml = TRUE;
        }
        else if (editorType == MSG_PLAINTEXT_EDITOR) {
            useHtml = FALSE;
        }
#if FORCE_EDITOR_FROM_EDITOR
        /* This would probably be a good idea for Publish/Send Page
         * if we had Forward Inline, but unfortunately we don't.
         */
        else if (old_context && EDT_IS_EDITOR(old_context)) {
            useHtml = TRUE;
        }
#endif
        else {
            if (old_context && CONTEXT_DATA(old_context)->stealth_cmd) {
                useHtml = !fe_globalPrefs.send_html_msg;
            }
            else {
                useHtml = fe_globalPrefs.send_html_msg;
            }
        }
	
	if (old_context)
		CONTEXT_DATA(old_context)->stealth_cmd = False;

	if (!fields)
		fields = new MSG_CompositionFields();

	theFrame = new XFE_ComposeFrame(toplevel, chromespec, old_context, fields, 
									draftInitialText, useHtml);
        if (defaultAddressIsNewsgroup) {
            // dig deep to determine if this window has any recipients.
            // If not, and the caller asked to default address type to
            // "Newsgroup:", then dig down again and change address header.
            // for the first item in the empty address list.
            // (only known user of this flag is XFE_TaskBarDrop::openComposeWindow())
            XFE_ComposeView *cv=(XFE_ComposeView*)(theFrame->getView());
            XFE_ComposeFolderView *cfv=(XFE_ComposeFolderView*)cv->getComposeFolderView();
            XFE_AddressFolderView *afv=cfv->getAddrFolderView();

            if (afv->getTotalData()==0)
                  afv->setHeader(0, MSG_NEWSGROUPS_HEADER_MASK);
        }

    theFrame->show();
    
	if (useHtml) {
		MWContext *context = theFrame->getContext();
		fe_UserActivity(context);
		xfe2_EditorInit(context); // im registration, focus event handlers, ...
		theFrame->initEditor();
	}

    XDEBUG(	printf ("leaving fe_showCompose()\n");)

	return ((XFE_ComposeFrame*)theFrame)->getPane();
}

int
XFE_ComposeFrame::getSecurityStatus()
{
    XFE_MailSecurityStatusType status = XFE_UNSECURE_UNSIGNED;
   HG18181
   return status;
}

XP_Bool
XFE_ComposeFrame::isOkToClose()
{
  char *pBody;
  uint32 bodySize;
  MSG_FontCode *font_changes;

  MWContext* context = getContext();
  XP_Bool okToClose;

  if ( m_destroyWhenConnectionsComplete) 
  {
	return True;
  }

  XFE_ComposeView *c_view = (XFE_ComposeView*)m_view;
  int  n = 0;

  if (c_view->isHTML()) {
	  // NOTE:  we need to "shutdown" edt plugins before
	  //        doing anything else...
	  //
	  n = 0;
	  while (EDT_IsPluginActive(context)) {
		  EDT_StopPlugin(context);
#ifdef DEBUG_rhess
		  fprintf(stderr, "stop::[ %d ]\n", n);
#endif
		  fe_EventLoop ();
		  n++;
	  }

	  n = 0;
	  while (EDT_IsBlocked(context)) {
#ifdef DEBUG_rhess
		  fprintf(stderr, "pause::[ %d ]\n", n);
#endif
		  fe_EventLoop ();
		  n++;
	  }
  }

  // If there is no message body and there are no attachments,
  // don't offer to save as a draft:
  FE_GetMessageBody(getPane(), &pBody, &bodySize, &font_changes);
  XP_FREE(pBody);
  if (bodySize == 0)
  {
      // getAttachmentData() doesn't always tell us about attachments!
      if ((c_view == 0) || (c_view->getAttachmentData() == 0))
	  return True;
  }

  if ( !((XFE_ComposeView*)m_view)->isModified() ) return True;

  int state = fe_YesNoCancelDialog(context, "composeCloseWarning", 
		XP_GetString(XFE_MNC_CLOSE_WARNING));

  if (state == XmDIALOG_OK_BUTTON) 
  {
        n = 0;

        c_view->doCommand( xfeCmdSaveDraft, NULL,NULL);

		if (c_view->isHTML()) {
			XtUnmapWidget(getChromeParent());

			XSync(XtDisplay(getChromeParent()), False);

			// NOTE:  we need to let edt plugins "cleanup" after
			//        the SaveDraft before waiting on the editor 
			//        to finish it's work...
			//
			n = 0;
			while (EDT_IsPluginActive(context)) {
#ifdef DEBUG_rhess
				fprintf(stderr, "plug::[ %d ]\n", n);
#endif
				fe_EventLoop ();
				n++;
			}

			n = 0;
			while (EDT_IsBlocked(context)) {
#ifdef DEBUG_rhess
				fprintf(stderr, "wait::[ %d ]\n", n);
#endif
				fe_EventLoop ();
				n++;
			}
		}
		okToClose = True;
  }
  else if (state == XmDIALOG_APPLY_BUTTON )
  {
	okToClose = True;
  } 
  else 
  { 
	okToClose = False;
  }
  return okToClose;
}

//////////////////////////////////////////////////////////////////////////
//
// Toolbox methods
//
//////////////////////////////////////////////////////////////////////////
void
XFE_ComposeFrame::toolboxItemClose(XFE_ToolboxItem * item)
{
	XP_ASSERT( item != NULL );

	// Message_Toolbar
	if (item == m_toolbar)
	{
		fe_globalPrefs.compose_message_message_toolbar_open = False;
	}
}
//////////////////////////////////////////////////////////////////////////
void
XFE_ComposeFrame::toolboxItemOpen(XFE_ToolboxItem * item)
{
	XP_ASSERT( item != NULL );

	// Message_Toolbar
	if (item == m_toolbar)
	{
		fe_globalPrefs.compose_message_message_toolbar_open = True;
	}
}
//////////////////////////////////////////////////////////////////////////
void
XFE_ComposeFrame::toolboxItemChangeShowing(XFE_ToolboxItem * item)
{
	XP_ASSERT( item != NULL );

	// Message_Toolbar
	if (item == m_toolbar)
	{
		fe_globalPrefs.compose_message_message_toolbar_showing = item->isShown();
	}
}
//////////////////////////////////////////////////////////////////////////
void
XFE_ComposeFrame::configureToolbox()
{
	// If a the frame was constructed with a chromespec, then we ignore
	// all the preference magic.
	if (m_chromespec_provided)
	{
		return;
	}

	// Make sure the toolbox is alive
	if (!m_toolbox || (m_toolbox && !m_toolbox->isAlive()))
	{
		return;
	}

	// Message_Toolbar
	if (m_toolbar)
	{
		m_toolbar->setShowing(fe_globalPrefs.compose_message_message_toolbar_showing);
		m_toolbar->setOpen(fe_globalPrefs.compose_message_message_toolbar_open);
	}
}
//////////////////////////////////////////////////////////////////////////
