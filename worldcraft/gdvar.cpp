//========= Copyright � 1996-2001, Valve LLC, All rights reserved. ============
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================

#include "stdafx.h"
#include "GameData.h"
#include "KeyValues.h"


typedef struct
{
	GDIV_TYPE eType;		// The enumeration of this type.
	char *pszName;			// The name of this type.
	trtoken_t eStoreAs;		// How this type is stored (STRING, INTEGER, etc).
} TypeMap_t;


//-----------------------------------------------------------------------------
// Maps type names to type enums and parsing logic for values.
//-----------------------------------------------------------------------------
static TypeMap_t TypeMap[] =
{
	{ ivAngle,			"angle",				STRING },
	{ ivChoices,		"choices",				STRING },
	{ ivColor1,			"color1",				STRING },
	{ ivColor255,		"color255",				STRING },
	{ ivDecal,			"decal",				STRING },
	{ ivFlags,			"flags",				INTEGER },
	{ ivInteger,		"integer",				INTEGER },
	{ ivSound,			"sound",				STRING },
	{ ivSprite,			"sprite",				STRING },
	{ ivString,			"string",				STRING },
	{ ivStudioModel,	"studio",				STRING },
	{ ivTargetDest,		"target_destination",	STRING },
	{ ivTargetSrc,		"target_source",		STRING },
	{ ivVector,			"vector",				STRING },
	{ ivNPCClass,		"npcclass",				STRING },
	{ ivFilterClass,	"filterclass",			STRING },
	{ ivFloat,			"float",				STRING },
	{ ivMaterial,		"material",				STRING },
	{ ivScene,			"scene",				STRING },
	{ ivSide,			"side",					STRING },
	{ ivSideList,		"sidelist",				STRING },
	{ ivOrigin,			"origin",				STRING },
	{ ivAxis,			"axis",					STRING },
	{ ivVecLine,		"vecline",				STRING },
};


char *GDinputvariable::m_pszEmpty = "";


//-----------------------------------------------------------------------------
// Purpose: 
// Output : 
//-----------------------------------------------------------------------------
GDinputvariable::GDinputvariable(void)
{
	m_szDefault[0] = 0;
	m_nDefault = 0;
	m_szValue[0] = 0;
	m_nItems = 0;
	m_bReportable = FALSE;
	m_bReadOnly = false;
	m_pszDescription = NULL;
}


//-----------------------------------------------------------------------------
// Purpose: Destructor.
//-----------------------------------------------------------------------------
GDinputvariable::~GDinputvariable(void)
{
	delete m_pszDescription;
	m_Items.RemoveAll();
}


//-----------------------------------------------------------------------------
// Purpose: Implements the copy operator.
//-----------------------------------------------------------------------------
GDinputvariable &GDinputvariable::operator =(GDinputvariable &Other)
{
	m_eType = Other.GetType();
	strcpy(m_szName, Other.m_szName);
	strcpy(m_szLongName, Other.m_szLongName);
	strcpy(m_szDefault, Other.m_szDefault);

	//
	// Copy the description.
	//
	delete m_pszDescription;
	if (Other.m_pszDescription != NULL)
	{
		m_pszDescription = new char[strlen(Other.m_pszDescription) + 1];
		strcpy(m_pszDescription, Other.m_pszDescription);
	}
	else
	{
		m_pszDescription = NULL;
	}

	m_nDefault = Other.m_nDefault;
	m_bReportable = Other.m_bReportable;
	m_bReadOnly = Other.m_bReadOnly;

	m_Items.RemoveAll();
	m_Items.Copy(Other.m_Items);
	m_nItems = Other.m_nItems;

	return(*this);
}


//-----------------------------------------------------------------------------
// Purpose: Returns the storage format of a given variable type.
// Input  : pszToken - Sting containing the token.
// Output : GDIV_TYPE corresponding to the token in the string, ivBadType if the
//			string does not correspond to a valid type.
//-----------------------------------------------------------------------------
trtoken_t GDinputvariable::GetStoreAsFromType(GDIV_TYPE eType)
{
	for (int i = 0; i < sizeof(TypeMap) / sizeof(TypeMap[0]); i++)
	{
		if (TypeMap[i].eType == eType)
		{
			return(TypeMap[i].eStoreAs);
		}
	}

	ASSERT(FALSE);
	return(STRING);
}


//-----------------------------------------------------------------------------
// Purpose: Returns the enumerated type of a string token.
// Input  : pszToken - Sting containing the token.
// Output : GDIV_TYPE corresponding to the token in the string, ivBadType if the
//			string does not correspond to a valid type.
//-----------------------------------------------------------------------------
GDIV_TYPE GDinputvariable::GetTypeFromToken(LPCSTR pszToken)
{
	for (int i = 0; i < sizeof(TypeMap) / sizeof(TypeMap[0]); i++)
	{
		if (IsToken(pszToken, TypeMap[i].pszName))
		{
			return(TypeMap[i].eType);
		}
	}

	return(ivBadType);
}


//-----------------------------------------------------------------------------
// Purpose: Returns a string representing the type of this variable, eg. "integer".
//-----------------------------------------------------------------------------
const char *GDinputvariable::GetTypeText(void)
{
	for (int i = 0; i < sizeof(TypeMap) / sizeof(TypeMap[0]); i++)
	{
		if (TypeMap[i].eType == m_eType)
		{
			return(TypeMap[i].pszName);
		}
	}

	return("unknown");
}


//-----------------------------------------------------------------------------
// Purpose: 
// Input  : tr - 
// Output : Returns TRUE on success, FALSE on failure.
//-----------------------------------------------------------------------------
BOOL GDinputvariable::InitFromTokens(TokenReader& tr)
{
	char szToken[128];

	if (!GDGetToken(tr, m_szName, sizeof(m_szName), IDENT))
	{
		return FALSE;
	}

	if (!GDSkipToken(tr, OPERATOR, "("))
	{
		return FALSE;
	}

	// check for "reportable" marker
	trtoken_t ttype = tr.NextToken(szToken, sizeof(szToken));
	if (ttype == OPERATOR)
	{
		if (!strcmp(szToken, "*"))
		{
			m_bReportable = true;
		}
	}
	else
	{
		tr.Stuff(ttype, szToken);
	}

	// get type
	if (!GDGetToken(tr, szToken, sizeof(szToken), IDENT))
	{
		return FALSE;
	}

	if (!GDSkipToken(tr, OPERATOR, ")"))
	{
		return FALSE;
	}

	//
	// Check for known variable types.
	//
	m_eType = GetTypeFromToken(szToken);
	if (m_eType == ivBadType)
	{
		GDError(tr, "'%s' is not a valid variable type", szToken);
		return FALSE;
	}

	//
	// Look ahead at the next token.
	//
	ttype = tr.PeekTokenType(szToken);

	//
	// Check for the "readonly" specifier.
	//
	if ((ttype == IDENT) && IsToken(szToken, "readonly"))
	{
		tr.NextToken(szToken, sizeof(szToken));
		m_bReadOnly = true;

		//
		// Look ahead at the next token.
		//
		ttype = tr.PeekTokenType(szToken);
	}

	//
	// Check for the ':' indicating a long name.
	//
	if (ttype == OPERATOR && IsToken(szToken, ":"))
	{
		//
		// Eat the ':'.
		//
		tr.NextToken(szToken, sizeof(szToken));

		if (m_eType == ivFlags)
		{
			GDError(tr, "flag sets do not have long names");
			return FALSE;
		}

		//
		// Get the long name.
		//
		if (!GDGetToken(tr, m_szLongName, sizeof(m_szLongName), STRING))
		{
			return(FALSE);
		}

		//
		// Look ahead at the next token.
		//
		ttype = tr.PeekTokenType(szToken);

		//
		// Check for the ':' indicating a default value.
		//
		if (ttype == OPERATOR && IsToken(szToken, ":"))
		{
			//
			// Eat the ':'.
			//
			tr.NextToken(szToken, sizeof(szToken));

			//
			// Look ahead at the next token.
			//
			ttype = tr.PeekTokenType(szToken);
			if (ttype == OPERATOR && IsToken(szToken, ":"))
			{
				//
				// No default value provided, skip to the description.
				//
			}
			else
			{
				//
				// Determine how to parse the default value. If this is a choices field, the
				// default could either be a string or an integer, so we must look ahead and
				// use whichever is there.
				//
				trtoken_t eStoreAs = GetStoreAsFromType(m_eType);

				if (eStoreAs == STRING)
				{
					if (!GDGetToken(tr, m_szDefault, sizeof(m_szDefault), STRING))
					{
						return(FALSE);
					}
				}
				else if (eStoreAs == INTEGER)
				{
					if (!GDGetToken(tr, szToken, sizeof(szToken), INTEGER))
					{
						return(FALSE);
					}

					m_nDefault = atoi(szToken);
				}

				//
				// Look ahead at the next token.
				//
				ttype = tr.PeekTokenType(szToken);
			}
		}

		//
		// Check for the ':' indicating a description.
		//
		if (ttype == OPERATOR && IsToken(szToken, ":"))
		{
			//
			// Eat the ':'.
			//
			tr.NextToken(szToken, sizeof(szToken));

			//
			// Read the description.
			//
			if (!GDGetTokenDynamic(tr, &m_pszDescription, STRING))
			{
				return(FALSE);
			}

			//
			// Look ahead at the next token.
			//
			ttype = tr.PeekTokenType(szToken);
		}
	}
	else
	{
		//
		// Default long name is short name.
		//
		strcpy(m_szLongName, m_szName);
	}

	//
	// Check for the ']' indicating the end of the class definition.
	//
	if ((ttype == OPERATOR && IsToken(szToken, "]")) ||	ttype != OPERATOR)
	{
		if (m_eType == ivFlags || m_eType == ivChoices)
		{
			//
			// Can't define a flags or choices variable without providing any flags or choices.
			//
			GDError(tr, "no %s specified", m_eType == ivFlags ? "flags" : "choices");
			return(FALSE);
		}
		return(TRUE);
	}

	if (!GDSkipToken(tr, OPERATOR, "="))
	{
		return(FALSE);
	}

	if (m_eType != ivFlags && m_eType != ivChoices)
	{
		GDError(tr, "didn't expect '=' here");
		return(FALSE);
	}

	// should be '[' to start flags/choices
	if (!GDSkipToken(tr, OPERATOR, "["))
	{
		return(FALSE);
	}

	// get flags?
	if (m_eType == ivFlags)
	{
		GDIVITEM ivi;

		while (1)
		{
			ttype = tr.PeekTokenType();
			if (ttype != INTEGER)
			{
				break;
			}

			// store bitflag value
			GDGetToken(tr, szToken, sizeof(szToken), INTEGER);
			ivi.iValue = atoi(szToken);

			// colon..
			if (!GDSkipToken(tr, OPERATOR, ":"))
			{
				return FALSE;
			}

			// get description
			if (!GDGetToken(tr, szToken, sizeof(szToken), STRING))
			{
				return FALSE;
			}
			strcpy(ivi.szCaption, szToken);

			// colon..
			if (!GDSkipToken(tr, OPERATOR, ":"))
			{
				return FALSE;
			}

			// get default setting
			if (!GDGetToken(tr, szToken, sizeof(szToken), INTEGER))
			{
				return FALSE;
			}
			ivi.bDefault = atoi(szToken) ? TRUE : FALSE;

			// add item to array of items
			m_Items.Add(ivi);
			++m_nItems;
		}
	}
	else if (m_eType == ivChoices)
	{
		GDIVITEM ivi;

		while (1)
		{
			ttype = tr.PeekTokenType();
			if ((ttype != INTEGER) && (ttype != STRING))
			{
				break;
			}

			// store choice value
			GDGetToken(tr, szToken, sizeof(szToken), ttype);
			ivi.iValue = 0;
			strcpy(ivi.szValue, szToken);

			// colon
			if (!GDSkipToken(tr, OPERATOR, ":"))
			{
				return FALSE;
			}

			// get description
			if (!GDGetToken(tr, szToken, sizeof(szToken), STRING))
			{
				return FALSE;
			}

			strcpy(ivi.szCaption, szToken);

			// add item to array of items
			m_Items.Add(ivi);
			++m_nItems;
		}
	}

	if (!GDSkipToken(tr, OPERATOR, "]"))
	{
		return FALSE;
	}

	return TRUE;
}


//-----------------------------------------------------------------------------
// Purpose: Decodes a key value from a string.
// Input  : pkv - Pointer to the key value object containing string encoded value.
//-----------------------------------------------------------------------------
void GDinputvariable::FromKeyValue(MDkeyvalue *pkv)
{
	trtoken_t eStoreAs = GetStoreAsFromType(m_eType);

	if (eStoreAs == STRING)
	{
		strcpy(m_szValue, pkv->szValue);
	}
	else if (eStoreAs == INTEGER)
	{
		m_nValue = atoi(pkv->szValue);
	}
}


//-----------------------------------------------------------------------------
// Purpose: Determines whether the given flag is set (assuming this is an ivFlags).
// Input  : uCheck - Flag to check.
// Output : Returns TRUE if flag is set, FALSE if not.
//-----------------------------------------------------------------------------
BOOL GDinputvariable::IsFlagSet(UINT uCheck)
{
	ASSERT(m_eType == ivFlags);
	return (((UINT)m_nValue & uCheck) == uCheck) ? TRUE : FALSE;
}


//-----------------------------------------------------------------------------
// Purpose: Combines the flags or choices items from another variable into our
//			list of flags or choices. Ours take priority if collisions occur.
// Input  : Other - The variable whose items are being merged with ours.
//-----------------------------------------------------------------------------
void GDinputvariable::Merge(GDinputvariable &Other)
{
	//
	// Only valid if we are of the same type.
	//
	if (Other.GetType() != GetType())
	{
		return;
	}

	//
	// Add Other's items to this ONLY if there is no same-value entry
	// for a specific item.
	//
	int nOurItems = m_nItems;
	for (int i = 0; i < Other.m_nItems; i++)
	{
		GDIVITEM &TheirItem = Other.m_Items[i];
		int j;
		for (j = 0; j < nOurItems; j++)
		{
			GDIVITEM &OurItem = m_Items[j];
			if (TheirItem.iValue == OurItem.iValue)
			{
				break;
			}
		}

		if (j == nOurItems)
		{
			//
			// Not found in our list - add their item to our list.
			//
			m_Items.Add(TheirItem);
			++m_nItems;
		}
	}
}


//-----------------------------------------------------------------------------
// Purpose: Determines whether the given flag is set (assuming this is an ivFlags).
// Input  : uFlags - Flags to set.
//			bSet - TRUE to set the flags, FALSE to clear them.
//-----------------------------------------------------------------------------
void GDinputvariable::SetFlag(UINT uFlags, BOOL bSet)
{
	ASSERT(m_eType == ivFlags);
	if (bSet)
	{
		m_nValue |= uFlags;
	}
	else
	{
		m_nValue &= ~uFlags;
	}
}


//-----------------------------------------------------------------------------
// Purpose: Sets this keyvalue to its default value.
//-----------------------------------------------------------------------------
void GDinputvariable::ResetDefaults(void)
{
	if (m_eType == ivFlags)
	{
		m_nValue = 0;

		//
		// Run thru flags and set any default flags.
		//
		for (int i = 0; i < m_nItems; i++)
		{
			if (m_Items[i].bDefault)
			{
				m_nValue |= m_Items[i].iValue;
			}
		}
	}
	else
	{
		m_nValue = m_nDefault;
		strcpy(m_szValue, m_szDefault);
	}
}


//-----------------------------------------------------------------------------
// Purpose: Encodes a key value as a string.
// Input  : pkv - Pointer to the key value object to receive the encoded string.
//-----------------------------------------------------------------------------
void GDinputvariable::ToKeyValue(MDkeyvalue *pkv)
{
	strcpy(pkv->szKey, m_szName);

	trtoken_t eStoreAs = GetStoreAsFromType(m_eType);

	if (eStoreAs == STRING)
	{
		strcpy(pkv->szValue, m_szValue);
	}
	else if (eStoreAs == INTEGER)
	{
		itoa(m_nValue, pkv->szValue, 10);
	}
}


//-----------------------------------------------------------------------------
// Purpose: Returns the description string that corresponds to a value string
//			for a choices list.
// Input  : pszString - The choices value string.
// Output : Returns the description string.
//-----------------------------------------------------------------------------
LPCTSTR GDinputvariable::ItemStringForValue(const char *szValue)
{
	for (int i = 0; i < m_nItems; i++)
	{
		if (!_stricmp(m_Items[i].szValue, szValue))
		{
			return(m_Items[i].szCaption);
		}
	}

	return(NULL);
}


//-----------------------------------------------------------------------------
// Purpose: Returns the value string that corresponds to a description string
//			for a choices list.
// Input  : pszString - The choices description string.
// Output : Returns the value string.
//-----------------------------------------------------------------------------
const char *GDinputvariable::ItemValueForString(const char *szString)
{
	for (int i = 0; i < m_nItems; i++)
	{
		if (!_strcmpi(m_Items[i].szCaption, szString))
		{
			return(m_Items[i].szValue);
		}
	}

	return(NULL);
}


