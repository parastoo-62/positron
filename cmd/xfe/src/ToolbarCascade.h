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

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// Name:        ToolbarCascade.h                                        //
//                                                                      //
// Description:	XFE_ToolbarCascade class header.                        //
//              A cascading toolbar push button.                        //
//                                                                      //
// Author:		Ramiro Estrugo <ramiro@netscape.com>                    //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#ifndef _xfe_toolbar_cascade_h_
#define _xfe_toolbar_cascade_h_

#include "ToolbarButton.h"

class XFE_ToolbarCascade : public XFE_ToolbarButton
{
public:
	
    XFE_ToolbarCascade(XFE_Frame *		frame,
					   Widget			parent,
					   HT_Resource		htResource,
					   const String		name);

    virtual ~XFE_ToolbarCascade();

	//////////////////////////////////////////////////////////////////////
	//                                                                  //
	// Accessors                                                        //
	//                                                                  //
	//////////////////////////////////////////////////////////////////////
	Widget			getSubmenu();

	//////////////////////////////////////////////////////////////////////
	//                                                                  //
	// Initialize                                                       //
	//                                                                  //
	//////////////////////////////////////////////////////////////////////
 	virtual void	initialize			();

protected:

	//////////////////////////////////////////////////////////////////////
	//                                                                  //
	// Widget creation interface                                        //
	//                                                                  //
	//////////////////////////////////////////////////////////////////////
	virtual Widget	createBaseWidget	(Widget			parent,
										 const String	name);

	//////////////////////////////////////////////////////////////////////
	//                                                                  //
	// Configure                                                        //
	//                                                                  //
	//////////////////////////////////////////////////////////////////////
	virtual void	configure			();

	//////////////////////////////////////////////////////////////////////
	//                                                                  //
	// addCallbacks                                                     //
	//                                                                  //
	//////////////////////////////////////////////////////////////////////
 	virtual void	addCallbacks		();

	//////////////////////////////////////////////////////////////////////
	//                                                                  //
	// Button callback interface                                        //
	//                                                                  //
	//////////////////////////////////////////////////////////////////////
	virtual void	cascading			();

private:

	//////////////////////////////////////////////////////////////////////
	//                                                                  //
	// Private data                                                     //
	//                                                                  //
	//////////////////////////////////////////////////////////////////////
	Widget			m_submenu;

	//////////////////////////////////////////////////////////////////////
	//                                                                  //
	// Private callbacks                                                //
	//                                                                  //
	//////////////////////////////////////////////////////////////////////
	static void		cascadingCB			(Widget			w,
 										 XtPointer		clientData,
 										 XtPointer		callData);

};

#endif // _xfe_toolbar_cascade_h_
