// CreateMCode.cpp: implementation of the CCreateMCode class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cad_powerland.h"
#include "createbase.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CCreateMCode, CCreateBase, 0)
CCreateMCode::CCreateMCode()
{
	
}

CCreateMCode::CCreateMCode(CString str)
{
	m_strM_What = str;
}

CCreateMCode::~CCreateMCode()
{

}

/*
 *	µÃµ½°üÎ§ºÐ
 */
BOOL CCreateMCode::GetBox(BOX2D* pBox,BOOL IsOutside /* = FALSE */)
{

	return FALSE;
}