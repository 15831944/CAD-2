// GraphScale.h: interface for the CGraphScale class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRAPHSCALE_H__9F2BC737_A5AC_4580_9AC3_54BC31F6A0D9__INCLUDED_)
#define AFX_GRAPHSCALE_H__9F2BC737_A5AC_4580_9AC3_54BC31F6A0D9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CGraphScale  //Ëõ·Å¹¦ÄÜ
{
public:
	CGraphScale();
	virtual ~CGraphScale();
	short ZoomInOut(double dValueX, double dValueY);

private:
	void GetPointBox(DOUBLEPOINT point,BOX2D &BoxRect);
	BOX2D GetListRect(PGRAPHLIST pList);
};

#endif // !defined(AFX_GRAPHSCALE_H__9F2BC737_A5AC_4580_9AC3_54BC31F6A0D9__INCLUDED_)
