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

#include "xp_core.h"
#include "structs.h"
#include "ntypes.h"

XP_BEGIN_PROTOS

extern void GNOMEFE_GetFormElementInfo (MWContext * context, LO_FormElementStruct * form_element);
extern void GNOMEFE_DisplayFormElement (MWContext * context, int iLocation, LO_FormElementStruct * form_element);
extern void GNOMEFE_GetFormElementValue (MWContext * context, LO_FormElementStruct * form_element,
                                         XP_Bool hide);
extern void GNOMEFE_ResetFormElement (MWContext * context, LO_FormElementStruct * form_element);
extern void GNOMEFE_SetFormElementToggle (MWContext * context, LO_FormElementStruct * form_element,
                                          XP_Bool toggle);

XP_END_PROTOS
