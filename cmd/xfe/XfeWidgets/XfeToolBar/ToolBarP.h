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
/* Name:		<Xfe/ToolBarP.h>										*/
/* Description:	XfeToolBar widget private header file.					*/
/* Author:		Ramiro Estrugo <ramiro@netscape.com>					*/
/*																		*/
/*----------------------------------------------------------------------*/

#ifndef _XfeToolBarP_h_							/* start ToolBarP.h		*/
#define _XfeToolBarP_h_

#include <Xfe/ToolBar.h>
#include <Xfe/OrientedP.h>

XFE_BEGIN_CPLUSPLUS_PROTECTION

/*----------------------------------------------------------------------*/
/*																		*/
/* XfeToolBar method inheritance macros									*/
/*																		*/
/*----------------------------------------------------------------------*/
#define XfeInheritLayoutIndicator			((XtWidgetProc)			_XtInherit)

/*----------------------------------------------------------------------*/
/*																		*/
/* XfeToolBarClassPart													*/
/*																		*/
/*----------------------------------------------------------------------*/
typedef struct
{
    XtWidgetProc			layout_indicator;	/* layout_indicator		*/
	XtPointer				extension;			/* Extension			*/ 
} XfeToolBarClassPart;

/*----------------------------------------------------------------------*/
/*																		*/
/* XfeToolBarClassRec													*/
/*																		*/
/*----------------------------------------------------------------------*/
typedef struct _XfeToolBarClassRec
{
    CoreClassPart				core_class;
    CompositeClassPart			composite_class;
    ConstraintClassPart			constraint_class;
    XmManagerClassPart			manager_class;
    XfeManagerClassPart			xfe_manager_class;
	XfeDynamicManagerClassPart	xfe_dynamic_manager_class;
    XfeOrientedClassPart		xfe_oriented_class;
    XfeToolBarClassPart			xfe_tool_bar_class;
} XfeToolBarClassRec;

externalref XfeToolBarClassRec xfeToolBarClassRec;

/*----------------------------------------------------------------------*/
/*																		*/
/* XfeToolBarPart														*/
/*																		*/
/*----------------------------------------------------------------------*/
typedef struct _XfeToolBarPart
{
    /* Callback resources */
    XtCallbackList	button_3_down_callback;		/* Button 3 down cb		*/
    XtCallbackList	button_3_up_callback;		/* Button 3 up cb		*/
    XtCallbackList	selection_changed_callback;	/* Selection changed cb	*/
    XtCallbackList	value_changed_callback;		/* Value changed cb		*/
    XtCallbackList	force_dimension_callback;	/* Force dimensions cb	*/

    /* Button resources */
    unsigned char		button_layout;			/* Button layout		*/

    /* Separator resources */
	int					separator_thickness;	/* Separator thickness	*/

	/* Raised resources */
    Boolean				raised;					/* Raised ?				*/

	/* Radio resources */
    unsigned char		toggle_behavior;		/* Toggle behavior		*/
    Boolean				radio_behavior;			/* Radio behavior ?		*/
    Widget				active_button;			/* Active button ?		*/

	/* Selection resources */
    unsigned char		selection_policy;		/* Sel Radio behavior ?*/
    Widget				selected_button;		/* Selected button ?	*/
    Modifiers			selection_modifiers;	/* Selection modifiers	*/

	/* Indicator resources */
	unsigned char		indicator_location;		/* Indicator Location	*/
    int					indicator_position;		/* Indicator Position	*/
	Cardinal			indicator_threshold;	/* Indicator Threshold	*/
	Boolean				dynamic_indicator;		/* Dynamic indicator ?	*/

	/* Wrapping resources */
	Boolean				allow_wrap;				/* Allow wrap			*/
	Cardinal			max_num_columns;		/* Max num columns		*/
	Cardinal			max_num_rows;			/* Max num rows			*/
	Cardinal			num_columns;			/* Num columns			*/
	Cardinal			num_rows;				/* Num rows				*/
    
    /* Private data -- Dont even look past this comment -- */
	Widget				indicator;				/* Indicator			*/
	Widget				indicator_target;		/* Indicator target		*/
	Widget				edit_text;				/* Edit text			*/

} XfeToolBarPart;

/*----------------------------------------------------------------------*/
/*																		*/
/* XfeToolBarRec														*/
/*																		*/
/*----------------------------------------------------------------------*/
typedef struct _XfeToolBarRec
{
    CorePart				core;
    CompositePart			composite;
    ConstraintPart			constraint;
    XmManagerPart			manager;
    XfeManagerPart			xfe_manager;
    XfeDynamicManagerPart	xfe_dynamic_manager;
    XfeOrientedPart			xfe_oriented;
    XfeToolBarPart			xfe_tool_bar;
} XfeToolBarRec;

/*----------------------------------------------------------------------*/
/*																		*/
/* XfeToolBarConstraintPart												*/
/*																		*/
/*----------------------------------------------------------------------*/
typedef struct _XfeToolBarConstraintPart
{
	Boolean			force_dimension_to_max;	/* Force dimension to max ?	*/
} XfeToolBarConstraintPart;

/*----------------------------------------------------------------------*/
/*																		*/
/* XfeToolBarConstraintRec												*/
/*																		*/
/*----------------------------------------------------------------------*/
typedef struct _XfeToolBarConstraintRec
{
    XmManagerConstraintPart				manager;
    XfeManagerConstraintPart			xfe_manager;
    XfeDynamicManagerConstraintPart		xfe_dynamic_manager;
    XfeOrientedConstraintPart			xfe_oriented;
    XfeToolBarConstraintPart			xfe_tool_bar;
} XfeToolBarConstraintRec;

/*----------------------------------------------------------------------*/
/*																		*/
/* XfeToolBarPart Access Macro											*/
/*																		*/
/*----------------------------------------------------------------------*/
#define _XfeToolBarPart(w) &(((XfeToolBarWidget) w) -> xfe_tool_bar)

/*----------------------------------------------------------------------*/
/*																		*/
/* XfeToolBarPart child constraint part access macro					*/
/*																		*/
/*----------------------------------------------------------------------*/
#define _XfeToolBarConstraintPart(w) \
(&(((XfeToolBarConstraintRec *) _XfeConstraints(w)) -> xfe_tool_bar))

/*----------------------------------------------------------------------*/
/*																		*/
/* XfeToolBar Method invocation functions								*/
/*																		*/
/*----------------------------------------------------------------------*/
extern void
_XfeToolBarLayoutIndicator		(Widget			w);
/*----------------------------------------------------------------------*/

XFE_END_CPLUSPLUS_PROTECTION

#endif											/* end ToolBarP.h		*/

