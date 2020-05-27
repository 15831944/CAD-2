// OpenGCode.h: interface for the COpenGCode class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OPENGCODE_H__DACAF320_703B_4CE2_B75E_5886F3F3B298__INCLUDED_)
#define AFX_OPENGCODE_H__DACAF320_703B_4CE2_B75E_5886F3F3B298__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class COpenGCode  
{
public:
	BOOL ReadGCode(CString PathName);	//打开G代码
	COpenGCode();
	virtual ~COpenGCode();
public:
	CStdioFile m_cReadGCode;	//文件的对象，这个类里的ReadString函数可以一行一行读文件

private:
	//把M代码集解析成字符串
	CString AnalysisAryM(CArray<class CString,class CString &>& aryM);
	void TurnG00(CString LineG00);	//解释G00
	void TurnG01(CString LineG01);	//解释G01
	void TurnG02(CString LineG02);	//解释G02
	void TurnG03(CString LineG03);	//解释G02
	void MWhat(CString lineM);//解析M代码
	void Draw(COLORREF color,Position posPer);	//画线函数 
	COLORREF RtnColor();//两个M字符集之间会产生的颜色
	COLORREF DistriPointMCode();//给点分配M代码		



private:
	Position m_Cur;	//当前点坐标
	//保存G00下面的M代码用的 
	CString m_strOnceM;	
	//线的类型是什么 ,看是G41还是G42
	int m_iStyle,     m_iG41orG42;	
	double m_dCurTudu;
	CObArray    m_CollectOfG00;	// 在两个G00之间存的图元
	CArray<double,double&> m_aryTudu;//凸度m_aryTudu;
	CArray<Position,Position&> m_point;		//存多线段的点
	CArray<class CString,class CString &> m_aryOnceM;//g00下面的M代码
	CArray<class CString,class CString &> m_aryTwicM;//g00上面的M代码
	
};

#endif // !defined(AFX_OPENGCODE_H__DACAF320_703B_4CE2_B75E_5886F3F3B298__INCLUDED_)
