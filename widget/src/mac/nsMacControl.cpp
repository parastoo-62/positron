/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 *
 * The contents of this file are subject to the Netscape Public License
 * Version 1.0 (the "NPL"); you may not use this file except in
 * compliance with the NPL.  You may obtain a copy of the NPL at
 * http://www.mozilla.org/NPL/
 *
 * Software distributed under the NPL is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the NPL
 * for the specific language governing rights and limitations under the
 * NPL.
 *
 * The Initial Developer of this code under the NPL is Netscape
 * Communications Corporation.  Portions created by Netscape are
 * Copyright (C) 1998 Netscape Communications Corporation.  All Rights
 * Reserved.
 */

#include "nsIFontMetrics.h"
#include "nsIDeviceContext.h"
#include "nsFont.h"
#include "nsToolkit.h"

#include "nsMacControl.h"
#include "nsColor.h"
#include "nsFontMetricsMac.h"

#if TARGET_CARBON
#include <ControlDefinitions.h>
#endif

#include <Appearance.h>
#include <TextUtils.h>
#include <UnicodeConverter.h>
#include <Fonts.h>

//-------------------------------------------------------------------------
//
//
//-------------------------------------------------------------------------
nsMacControl::nsMacControl()
{
	mValue			= 0;
	mMin			= 0;
	mMax			= 1;
	mWidgetArmed	= PR_FALSE;
	mMouseInButton	= PR_FALSE;

	mControl		= nsnull;
	mControlType	= pushButProc;

	mLastLabel = "";
	mLastBounds.SetRect(0,0,0,0);
	mLastValue = 0;
	mLastHilite = 0;

	AcceptFocusOnClick(PR_FALSE);
}

/**-------------------------------------------------------------------------
 * See documentation in nsMacControl.h
 * @update -- 12/10/98 dwc
 */
NS_IMETHODIMP nsMacControl::Create(nsIWidget *aParent,
                      const nsRect &aRect,
                      EVENT_CALLBACK aHandleEventFunction,
                      nsIDeviceContext *aContext,
                      nsIAppShell *aAppShell,
                      nsIToolkit *aToolkit,
                      nsWidgetInitData *aInitData) 
{
	Inherited::Create(aParent, aRect, aHandleEventFunction,
						aContext, aAppShell, aToolkit, aInitData);
  
	// create native control. mBounds has been set up at this point
	nsresult		theResult = CreateOrReplaceMacControl(mControlType);

	mLastBounds = mBounds;

	return theResult;
}

//-------------------------------------------------------------------------
//
//
//-------------------------------------------------------------------------
nsMacControl::~nsMacControl()
{
	if (mControl)
	{
		Show(PR_FALSE);
		::DisposeControl(mControl);
		mControl = nsnull;
	}
}

#pragma mark -
//-------------------------------------------------------------------------
//
//
//-------------------------------------------------------------------------
PRBool nsMacControl::OnPaint(nsPaintEvent &aEvent)
{
	if (mControl && mVisible)
	{
		// turn off drawing for setup to avoid ugliness
		Boolean		isVisible = IsControlVisible(mControl);
		::SetControlVisibility(mControl, false, false);

		// update title
		if (mLabel != mLastLabel)
		{
			NSStringSetControlTitle(mControl,mLabel);
		}

		// update bounds
		if (mBounds != mLastBounds)
		{
			nsRect ctlRect;
			GetRectForMacControl(ctlRect);
			Rect macRect;
			nsRectToMacRect(ctlRect, macRect);

			if ((mBounds.x != mLastBounds.x) || (mBounds.y != mLastBounds.y))
				::MoveControl(mControl, macRect.left, macRect.top);
			if ((mBounds.width != mLastBounds.width) || (mBounds.height != mLastBounds.height))
				::SizeControl(mControl, ctlRect.width, ctlRect.height);

			mLastBounds = mBounds;

			// Erase the widget rect (which can be larger than the control rect).
			// Note: this should paint the backgrount with the right color but
			// it doesn't work right now, see bug #5685 for more info.
			nsRect bounds = mBounds;
			bounds.x = bounds. y = 0;
			nsRectToMacRect(bounds, macRect);
			::EraseRect(&macRect);
		}

		// update value
		if (mValue != mLastValue)
		{
			mLastValue = mValue;
			if (nsToolkit::HasAppearanceManager())
				::SetControl32BitValue(mControl, mValue);
			else
				::SetControlValue(mControl, mValue);
		}

		// update hilite
		PRInt16 hilite;
		if (mEnabled)
			hilite = (mWidgetArmed && mMouseInButton ? 1 : 0);
		else
			hilite = kControlInactivePart;
		if (hilite != mLastHilite)
		{
			mLastHilite = hilite;
			::HiliteControl(mControl, hilite);
		}

		::SetControlVisibility(mControl, isVisible, false);

		// Draw the control
		::DrawOneControl(mControl);

		Rect macRect;
		nsRect bounds = mBounds;
		bounds.x = bounds. y = 0;
		nsRectToMacRect(bounds, macRect);
#if TARGET_CARBON
		::ValidWindowRect(mWindowPtr, &macRect);
#else
		::ValidRect(&macRect);
#endif
	}
	return PR_FALSE;
}

//-------------------------------------------------------------------------
//
//
//-------------------------------------------------------------------------
PRBool  nsMacControl::DispatchMouseEvent(nsMouseEvent &aEvent)
{
	PRBool eatEvent = PR_FALSE;
	switch (aEvent.message)
	{
		case NS_MOUSE_LEFT_DOUBLECLICK:
		case NS_MOUSE_LEFT_BUTTON_DOWN:
			if (mEnabled)
			{
				mMouseInButton = PR_TRUE;
				mWidgetArmed = PR_TRUE;
				Invalidate(PR_TRUE);
			}
			break;

		case NS_MOUSE_LEFT_BUTTON_UP:
			// if the widget was not armed, eat the event
			if (!mWidgetArmed)
				eatEvent = PR_TRUE;
			// if the mouseUp happened on another widget, eat the event too
			// (the widget which got the mouseDown is always notified of the mouseUp)
			if (! mMouseInButton)
				eatEvent = PR_TRUE;
			mWidgetArmed = PR_FALSE;
			if (mMouseInButton)
				Invalidate(PR_TRUE);
			break;

		case NS_MOUSE_EXIT:
			mMouseInButton = PR_FALSE;
			if (mWidgetArmed)
				Invalidate(PR_TRUE);
			break;

		case NS_MOUSE_ENTER:
			mMouseInButton = PR_TRUE;
			if (mWidgetArmed)
				Invalidate(PR_TRUE);
			break;
	}
	if (eatEvent)
		return PR_TRUE;
	return (Inherited::DispatchMouseEvent(aEvent));
}

//-------------------------------------------------------------------------
//
//
//-------------------------------------------------------------------------
void  nsMacControl::ControlChanged(PRInt32 aNewValue)
{
	if (aNewValue != mValue)
	{
		mValue = aNewValue;
		mLastValue = mValue;	// safely assume that the control has been repainted already

		nsGUIEvent guiEvent;
		nsPoint point(0,0);
		guiEvent.eventStructType = NS_GUI_EVENT;
		guiEvent.message	= NS_CONTROL_CHANGE;
 		guiEvent.point		= point;
 		guiEvent.time	 	= PR_IntervalNow();
 		guiEvent.widget	 	= this;
 		guiEvent.nativeMsg	= nsnull;
		Inherited::DispatchWindowEvent(guiEvent);
	}
}


#pragma mark -
//-------------------------------------------------------------------------
//
//
//-------------------------------------------------------------------------
NS_IMETHODIMP nsMacControl::Enable(PRBool bState)
{
  PRBool priorState = mEnabled;
  Inherited::Enable(bState);
  if ( priorState != bState )
    Invalidate(PR_FALSE);
  return NS_OK;
}
//-------------------------------------------------------------------------
//
//
//-------------------------------------------------------------------------
NS_IMETHODIMP nsMacControl::Show(PRBool bState)
{
  Inherited::Show(bState);
  if (mControl)
  {
  	::SetControlVisibility(mControl, bState, false);		// don't redraw
  }
  return NS_OK;
}

//-------------------------------------------------------------------------
//
// Set this control font
//
//-------------------------------------------------------------------------
NS_IMETHODIMP nsMacControl::SetFont(const nsFont &aFont)
{
	Inherited::SetFont(aFont);	

	SetupMacControlFont();
	
 	return NS_OK;
}

#pragma mark -

//-------------------------------------------------------------------------
//
// Get the rect which the Mac control uses. This may be different for
// different controls, so this method allows overriding
//
//-------------------------------------------------------------------------
void nsMacControl::GetRectForMacControl(nsRect &outRect)
{
		outRect = mBounds;
		outRect.x = outRect.y = 0;
}

//-------------------------------------------------------------------------
//
//
//-------------------------------------------------------------------------

NS_METHOD nsMacControl::CreateOrReplaceMacControl(short inControlType)
{
	nsRect		controlRect;
	GetRectForMacControl(controlRect);
	Rect macRect;
	nsRectToMacRect(controlRect, macRect);

	if(nsnull != mWindowPtr)
	{
#ifdef DEBUG
		// we should have a root control at this point. If not, something's wrong.
		// it's made in nsMacWindow
		ControlHandle		rootControl = nil;
		OSErr		err = ::GetRootControl(mWindowPtr, &rootControl);
		NS_ASSERTION((err == noErr && rootControl != nil), "No root control exists for the window");
#endif

		if (mControl)
			::DisposeControl(mControl);

		StartDraw();
		mControl = ::NewControl(mWindowPtr, &macRect, "\p", mVisible, mValue, mMin, mMax, inControlType, nil);
  		EndDraw();
		
		// need to reset the font now
		// XXX to do: transfer the text in the old control over too
		if (mControl && mFontMetrics)
		{
			SetupMacControlFont();
		}
	}

	return (mControl) ? NS_OK : NS_ERROR_NULL_POINTER;
}


//-------------------------------------------------------------------------
//
//
//-------------------------------------------------------------------------
void nsMacControl::SetupMacControlFont()
{
	NS_PRECONDITION(mFontMetrics != nsnull, "No font metrics in SetupMacControlFont");
	NS_PRECONDITION(mContext != nsnull, "No context metrics in SetupMacControlFont");
	
	TextStyle		theStyle;
	nsFontMetricsMac::GetNativeTextStyle(*mFontMetrics, *mContext, theStyle);
	
	// if needed, impose a min size of 9pt on the control font
	if (theStyle.tsSize < 9)
		theStyle.tsSize = 9;
	
	ControlFontStyleRec fontStyleRec;
	fontStyleRec.flags = (kControlUseFontMask | kControlUseFaceMask | kControlUseSizeMask);
	fontStyleRec.font = theStyle.tsFont;
	fontStyleRec.size = theStyle.tsSize;
	fontStyleRec.style = theStyle.tsFace;
	::SetControlFontStyle(mControl, &fontStyleRec);
}

#pragma mark -

//-------------------------------------------------------------------------
//
//
//-------------------------------------------------------------------------

void nsMacControl::StringToStr255(const nsString& aText, Str255& aStr255)
{
	char buffer[256];
	
	aText.ToCString(buffer,255);
		
	PRInt32 len = strlen(buffer);
	memcpy(&aStr255[1],buffer,len);
	aStr255[0] = len;
}

//-------------------------------------------------------------------------
//
//
//-------------------------------------------------------------------------

void nsMacControl::Str255ToString(const Str255& aStr255, nsString& aText)
{
	char 		buffer[256];
	PRInt32 len = aStr255[0];
  
	memcpy(buffer,&aStr255[1],len);
	buffer[len] = 0;

	aText = buffer;		
}

//-------------------------------------------------------------------------
//
//
//-------------------------------------------------------------------------

void nsMacControl::NSStringSetControlTitle(ControlHandle theControl, nsString title)
{	
	TextStyle				theStyle;
	ScriptCode				fontScript;
	OSErr					err;
	UnicodeToTextRunInfo	unicodeTextRunInfo;
	const PRUnichar*		unicodeText;
	char*					scriptRunText;
	size_t					unicodeTextLengthInBytes, unicodeTextReadInBytes,
							scriptRunTextSizeInBytes, scriptRunTextLengthInBytes,
							scriptCodeRunListLength;
	ScriptCodeRun			convertedTextScript;
	
	NS_PRECONDITION(mFontMetrics != nsnull, "nsMacControl::NSStringSetControlTitle: no Font Metrics");
	
	//
	// determine the script of the font that the control is supposed to be drawn in
	//
	nsFontMetricsMac::GetNativeTextStyle(*mFontMetrics, *mContext, theStyle);
	fontScript = ::FontToScript(theStyle.tsFont);
	
	//
	// create a Unicode Conveter object (from Unicode -> font)
	//
 	err = ::CreateUnicodeToTextRunInfoByScriptCode(1,&fontScript,&unicodeTextRunInfo);
  	NS_ASSERTION(err==noErr,"nsMacControl::NSStringSetControlTitle: CreateUnicodeToTextRunInfoByScriptCode failed.");
  	if (err!=noErr) { return; }

	//
	// get the Unicode text and prepare buffers
	//
	unicodeText = title.GetUnicode();
	unicodeTextLengthInBytes = title.Length() * sizeof(PRUnichar);
	scriptRunTextSizeInBytes = unicodeTextLengthInBytes * 2;
	scriptRunText = new char[scriptRunTextSizeInBytes];


  	//
  	// convert from Unicode to script run
  	// 
	err = ::ConvertFromUnicodeToScriptCodeRun(unicodeTextRunInfo,
				unicodeTextLengthInBytes,unicodeText,
				0, /* no flags */
				0,NULL,NULL,NULL, /* no offset arrays */
				scriptRunTextSizeInBytes,&unicodeTextReadInBytes,&scriptRunTextLengthInBytes,
				scriptRunText,
				1 /* count of scrip runs*/,&scriptCodeRunListLength,&convertedTextScript);
  	if (err!=noErr)
  	{ 
  		//
  		// the font script is not capable of rendering this string, we need to find an installed
  		//	script that can
  		//
 		err = ::CreateUnicodeToTextRunInfoByScriptCode(0,NULL,&unicodeTextRunInfo);
  		NS_ASSERTION(err==noErr,"nsMacControl::NSStringSetControlTitle: CreateUnicodeToTextRunInfoByScriptCode failed.");
  		if (err!=noErr) { return; }

	  	//
	  	// convert from Unicode to script run
	  	// 
		err = ::ConvertFromUnicodeToScriptCodeRun(unicodeTextRunInfo,
					unicodeTextLengthInBytes,unicodeText,
					0, /* no flags */
					0,NULL,NULL,NULL, /* no offset arrays */
					scriptRunTextSizeInBytes,&unicodeTextReadInBytes,&scriptRunTextLengthInBytes,
					scriptRunText,
					1 /* count of scrip runs*/,&scriptCodeRunListLength,&convertedTextScript);
  					NS_ASSERTION(err==noErr,"nsMacControl::NSStringSetControlTitle: CreateUnicodeToTextRunInfoByScriptCode failed.");
  					if (err!=noErr) { delete [] scriptRunText; return;}
  	}	

	scriptRunText[scriptRunTextLengthInBytes] = 0;	// null terminate

	if (convertedTextScript.script!=fontScript)
		SetupMacControlFontForScript(convertedTextScript.script);

	//
	// set the control title
	//
	::SetControlTitle(theControl,c2pstr(scriptRunText));
	delete [] scriptRunText;	
					
}
//-------------------------------------------------------------------------
//
//
//-------------------------------------------------------------------------
void nsMacControl::SetupMacControlFontForScript(short theScript)
{
	short					themeFontID;
	Str255					themeFontName;
	SInt16					themeFontSize;
	Style					themeFontStyle;
	TextStyle				theStyle;
	OSErr					err;

	NS_PRECONDITION(mFontMetrics != nsnull, "No font metrics in SetupMacControlFont");
	NS_PRECONDITION(mContext != nsnull, "No context metrics in SetupMacControlFont");
	
	nsFontMetricsMac::GetNativeTextStyle(*mFontMetrics, *mContext, theStyle);
	
	//
	// take the script and select and override font
	//
	err = ::GetThemeFont(kThemeSystemFont,theScript,themeFontName,&themeFontSize,&themeFontStyle);
	NS_ASSERTION(err==noErr,"nsMenu::NSStringNewMenu: GetThemeFont failed.");
	::GetFNum(themeFontName,&themeFontID);
	
	// if needed, impose a min size of 9pt on the control font
	if (theStyle.tsSize < 9)
		theStyle.tsSize = 9;
	
	ControlFontStyleRec fontStyleRec;
	fontStyleRec.flags = (kControlUseFontMask | kControlUseFaceMask | kControlUseSizeMask);
	fontStyleRec.font = themeFontID;
	fontStyleRec.size = theStyle.tsSize;
	fontStyleRec.style = theStyle.tsFace;
	::SetControlFontStyle(mControl, &fontStyleRec);
}
	