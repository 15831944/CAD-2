// OpenPLTCode.h: interface for the COpenPLTCode class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OPENPLTCODE_H__27106F03_B66E_435D_A192_24E4B22D5D67__INCLUDED_)
#define AFX_OPENPLTCODE_H__27106F03_B66E_435D_A192_24E4B22D5D67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class COpenPLTCode  
{
public:
	COpenPLTCode();
	virtual ~COpenPLTCode();
	CString AnalysisPlt(CString strFile);

	CString m_strOutFile;
};

#endif // !defined(AFX_OPENPLTCODE_H__27106F03_B66E_435D_A192_24E4B22D5D67__INCLUDED_)
