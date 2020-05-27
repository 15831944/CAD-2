// OpenDXFCode.h: interface for the COpenDXFCode class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OPENDXFCODE_H__61D97F31_FB82_4990_988F_9EF783039AE1__INCLUDED_)
#define AFX_OPENDXFCODE_H__61D97F31_FB82_4990_988F_9EF783039AE1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class COpenDXFCode  
{
public:
	BOOL ReadDXFCode(CString PathOfFile);
	COpenDXFCode();
	virtual ~COpenDXFCode();

	CStdioFile m_cReadDXFCode;	//文件的对象，这个类里的ReadString函数可以一行一行读文件
	CArray<Position,Position&> m_point;		//存多线段的点
private:
	void TurnLine();	//在DXF文件中发现LINE行转成直线
	void TurnArc();		//在DXF文件中发现ARC行转成圆弧
	void TurnCircle();	//在DXF文件中发现CIRCLE行转成圆
	void TurnPolyline();//在DXF文件中发现POLYLINE行转成多线段
	void TurnPoint();	//在DXF文件中发现POINT行转成多线段
};

#endif // !defined(AFX_OPENDXFCODE_H__61D97F31_FB82_4990_988F_9EF783039AE1__INCLUDED_)
