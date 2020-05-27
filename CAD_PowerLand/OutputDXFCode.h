// OutputDXFCode.h: interface for the COutputDXFCode class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OUTPUTDXFCODE_H__8B6DDDAB_FB1E_47F7_B3E0_BE2DE7EC1F88__INCLUDED_)
#define AFX_OUTPUTDXFCODE_H__8B6DDDAB_FB1E_47F7_B3E0_BE2DE7EC1F88__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


//////////////////////////////////////////////////////////////////////////
/*						解释DXF中图元块
0	//图元类型块开始 
LINE	
5	//句柄，可不要。
140
8	//图层
0
10		//10,20,30，主要点的XYZ坐标
158.3427961847824
20
79.877121304845559
30
0.0
11		//11，21，31，其它点的XYZ坐标
212.7003931202612
21
117.0009327169245
31
0.0
*/
//////////////////////////////////////////////////////////////////////////

class COutputDXFCode  
{
public://公有函数

	COutputDXFCode();
	virtual ~COutputDXFCode();
	
	// 输出dxf代码文件
	BOOL	OutputDXFCode(CString filename);
		

public://公有变量

	CFile m_cDXFCode;
	double m_dCurX;	//当前点X
	double m_dCurY;	//当前点Y

private:
	// 写文件头
	BOOL	WriteHeader();
	// 写文件尾
	BOOL	WriteTail();
	// 写实体信息，该函数根据类型调用写图元信息的函数
	BOOL	WriteContent();
	// 写图元信息
	BOOL	WriteLine(CCreateLine*	pLine);
	BOOL	WriteRectangle(CCreateRectangle*	pRectangle);
	BOOL	WriteRound(CCreateRound*	pRound);
	BOOL	WriteArc(CCreateRoundArc*	pArc);
	BOOL	WritePolyline(CCreatePolyline* Polyline);
	BOOL	WriteEllipse(CCreateEllipse* Ellipse);
	BOOL	WritePoint(CCreatePoint* Point);
	//////////////////////////////////////////////////////////////////////////
	

};

#endif // !defined(AFX_OUTPUTDXFCODE_H__8B6DDDAB_FB1E_47F7_B3E0_BE2DE7EC1F88__INCLUDED_)
