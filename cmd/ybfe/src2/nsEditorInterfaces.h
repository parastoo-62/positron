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

#ifndef nsEditorInterfaces_h__
#define nsEditorInterfaces_h__

#include "nsIDOMEvent.h"
#include "nsIDOMKeyListener.h"
#include "nsIDOMMouseListener.h"

//nsIDOMKeyListener interface
class nsEditorKeyListener : public nsIDOMKeyListener {
public:
  nsEditorKeyListener();
  virtual ~nsEditorKeyListener();

  NS_DECL_ISUPPORTS

  virtual nsresult ProcessEvent(nsIDOMEvent* aEvent);

public:
  virtual nsresult KeyDown(nsIDOMEvent* aKeyEvent);
  virtual nsresult KeyUp(nsIDOMEvent* aKeyEvent);
  virtual nsresult KeyPress(nsIDOMEvent* aKeyEvent);

private:
  virtual nsresult GetCharFromKeyCode(PRUint32 aKeyCode, PRBool aIsShift, char *aChar);
};

//nsIDOMMouseListener interface
class nsEditorMouseListener : public nsIDOMMouseListener {
public:
  nsEditorMouseListener();
  virtual ~nsEditorMouseListener();

  NS_DECL_ISUPPORTS

  virtual nsresult ProcessEvent(nsIDOMEvent* aEvent);

public:
  virtual nsresult MouseDown(nsIDOMEvent* aMouseEvent);
  virtual nsresult MouseUp(nsIDOMEvent* aMouseEvent);
  virtual nsresult MouseClick(nsIDOMEvent* aMouseEvent);
  virtual nsresult MouseDblClick(nsIDOMEvent* aMouseEvent);
  virtual nsresult MouseOver(nsIDOMEvent* aMouseEvent);
  virtual nsresult MouseOut(nsIDOMEvent* aMouseEvent);
};


extern nsresult NS_NewEditorKeyListener(nsIDOMEventListener ** aInstancePtrResult);

extern nsresult NS_NewEditorMouseListener(nsIDOMEventListener ** aInstancePtrResult);

#endif //nsEditorInterfaces_h__
