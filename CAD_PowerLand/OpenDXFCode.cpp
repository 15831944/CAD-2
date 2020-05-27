// OpenDXFCode.cpp: implementation of the COpenDXFCode class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cad_powerland.h"
#include "OpenDXFCode.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COpenDXFCode::COpenDXFCode()
{

}

COpenDXFCode::~COpenDXFCode()
{
	m_point.RemoveAll();
}

BOOL COpenDXFCode::ReadDXFCode(CString PathOfFile)
{
	CString  l_strLineContent;	//每行文本的内容
	CFileException ex;	
	BOOL isEof=FALSE;
	BOOL aaa = FALSE;//直到遇到“ENTITIES”时才解析图形
	if (!m_cReadDXFCode.Open(PathOfFile,CFile::modeRead | CFile::shareDenyWrite, &ex)) //打开文件		
	{	
		//如果没打开就报错
		TCHAR szError[1024];		
		ex.GetErrorMessage(szError, 1024);		
		AfxMessageBox(szError);		
		return  FALSE;		
	}
	else		
	{	//读取文本
		for(int i = 1;;i++)
		{	
			//从第一行开始读，
			isEof=!m_cReadDXFCode.ReadString(l_strLineContent);//读取一行文本。如果最后一行了就是isEof==1			
			if(isEof)//如果没有了就是1并退出
				break;	
			if(l_strLineContent == "ENTITIES")
				aaa = TRUE;
			if(l_strLineContent == "LINE"&& aaa == TRUE )
			{
				TurnLine();			//把DXF里的直线信息转换成本程序的直线信息
			}
			else if(l_strLineContent == "ARC"&& aaa == TRUE)
			{
				TurnArc();		//在DXF文件中发现ARC行转成圆弧
			}
			else if(l_strLineContent == "CIRCLE"&& aaa == TRUE)
			{
				TurnCircle();	//在DXF文件中发现CIRCLE行转成圆
			}
			else if(l_strLineContent == "POLYLINE"&& aaa == TRUE)	
			{
				TurnPolyline();		//在DXF文件中发现POLYLINE行转成多线段
			}
			else if(l_strLineContent == "POINT"&& aaa == TRUE)	
			{
				TurnPoint();		//在DXF文件中发现POLYLINE行转成多线段
			}
			else if(l_strLineContent == "EOF"&& aaa == TRUE)
			{
				break;
			}
		}
		g_pView->Invalidate();
	}		
	return TRUE;
}

/*
 *把DXF里的直线信息转换成本程序的直线信息	
 */
void COpenDXFCode::TurnLine()
{
	CString l_strLineContent;
	Position l_cBeginPos,l_cEndPos;
	COLORREF l_cColor =g_pDoc->m_stcColorToM.m_color5;	//线色

	while(l_strLineContent != "  0")//这段线结束
	{
		m_cReadDXFCode.ReadString(l_strLineContent);
		if(l_strLineContent == " 62")	//如果有62说明是有颜色的，62组码是颜色号（固定）
		{
			m_cReadDXFCode.ReadString(l_strLineContent);
			l_cColor = g_pDoc->RtnColorRGB(atoi(l_strLineContent));	//是第几个颜色	
		}
		else if(l_strLineContent == " 10")	//走到“ 10”的下面，正好是起始点X的坐标
		{
			m_cReadDXFCode.ReadString(l_strLineContent);
			l_cBeginPos.x = atof(l_strLineContent);	
		}
		else if(l_strLineContent == " 20")	//走到“ 20”的下面，正好是起始点Y的坐标
		{
			m_cReadDXFCode.ReadString(l_strLineContent);
			l_cBeginPos.y = atof(l_strLineContent);	
		}
		else if(l_strLineContent == " 11")	//走到“ 10”的下面，正好是起始点X的坐标
		{
			m_cReadDXFCode.ReadString(l_strLineContent);
			l_cEndPos.x = atof(l_strLineContent);	
		}
		else if(l_strLineContent == " 21")	//走到“ 20”的下面，正好是起始点Y的坐标
		{
			m_cReadDXFCode.ReadString(l_strLineContent);
			l_cEndPos.y = atof(l_strLineContent);	
		}
	}
	//保存到链表里
	CCreateLine* l_pTempLine = new CCreateLine(l_cBeginPos,l_cEndPos);	
	l_pTempLine->m_ColorOfLine = l_cColor;		//把获得到的颜色加到图元里然后下面加进链表
	CString l_strSBM,l_strCBM;//两个M代码
	g_pDoc->RtnColorMCode(l_strSBM,l_strCBM,-1,l_cColor);//输入线色返回M代码
	l_pTempLine->m_strSBM.RemoveAll();
	l_pTempLine->m_strCBM.RemoveAll();	
	l_pTempLine->m_strSBM.Add(l_strSBM);
	l_pTempLine->m_strCBM.Add(l_strCBM);
				
	g_pDoc->m_CCreateBaseList.AddTail(l_pTempLine);	
}

//在DXF文件中发现ARC行转成圆弧
void COpenDXFCode::TurnArc()	
{
	CString l_strLineContent;
	Position l_cBeginPos,l_cEndPos,l_cCenter;//起，终，中心点
	double l_cRadius,l_cAngleBegin,l_cAngleEnd;	//半径，起点角度，末点角度
	COLORREF l_cColor =g_pDoc->m_stcColorToM.m_color5;	//线色

	while(l_strLineContent != "  0")//这段线结束
	{
		m_cReadDXFCode.ReadString(l_strLineContent);
		if(l_strLineContent == " 62")	//如果有62说明是有颜色的，62组码是颜色号（固定）
		{
			m_cReadDXFCode.ReadString(l_strLineContent);
			l_cColor = g_pDoc->RtnColorRGB(atoi(l_strLineContent));	//是第几个颜色	
		}
		else if(l_strLineContent == " 10")	//走到“ 10”的下面，正好是圆心点X的坐标
		{
			m_cReadDXFCode.ReadString(l_strLineContent);
			l_cCenter.x = atof(l_strLineContent);	
		}
		else if(l_strLineContent == " 20")	//走到“ 20”的下面，正好是圆心点Y的坐标
		{
			m_cReadDXFCode.ReadString(l_strLineContent);
			l_cCenter.y = atof(l_strLineContent);	
		}
		else if(l_strLineContent == " 40")	//走到“40”的下面，正好是半径
		{
			m_cReadDXFCode.ReadString(l_strLineContent);
			l_cRadius = atof(l_strLineContent);	
		}
		else if(l_strLineContent == " 50")	//走到“ 50”的下面，正好是起点角度
		{
			m_cReadDXFCode.ReadString(l_strLineContent);
			l_cAngleBegin = atof(l_strLineContent);	
		}
		else if(l_strLineContent == " 51")	//走到“ 51”的下面，正好是终点角度
		{
			m_cReadDXFCode.ReadString(l_strLineContent);
			l_cAngleEnd = atof(l_strLineContent);	
		}
	}
	//求，起点，终点。
	l_cBeginPos.x = l_cCenter.x + l_cRadius*cos(MATH_PI*l_cAngleBegin/180);
	l_cBeginPos.y = l_cCenter.y + l_cRadius*sin(MATH_PI*l_cAngleBegin/180);

	l_cEndPos.x = l_cCenter.x + l_cRadius*cos(MATH_PI*l_cAngleEnd/180);
	l_cEndPos.y = l_cCenter.y + l_cRadius*sin(MATH_PI*l_cAngleEnd/180);

	//保存到链表里
	CCreateRoundArc* l_pTempArc = new CCreateRoundArc(l_cBeginPos,l_cCenter,l_cEndPos,AD_COUNTERCLOCKWISE);
	CString l_strSBM,l_strCBM;//两个M代码
	g_pDoc->RtnColorMCode(l_strSBM,l_strCBM,-1,l_cColor);//输入线色返回M代码
	l_pTempArc->m_strSBM.RemoveAll();
	l_pTempArc->m_strCBM.RemoveAll();	
	l_pTempArc->m_strSBM.Add(l_strSBM);
	l_pTempArc->m_strCBM.Add(l_strCBM);
	l_pTempArc->m_ColorOfLine = l_cColor;		//把获得到的颜色加到图元里然后下面加进链表
	g_pDoc->m_CCreateBaseList.AddTail(l_pTempArc);		
}

//在DXF文件中发现CIRCLE行转成圆
void COpenDXFCode::TurnCircle()
{
	CString l_strLineContent;
	Position l_cCenter,l_cEndPos;//起，终，中心点
	double l_cRadius;	//半径
	COLORREF l_cColor =g_pDoc->m_stcColorToM.m_color5;	//线色

	while(l_strLineContent != "  0")//这段线结束
	{
		m_cReadDXFCode.ReadString(l_strLineContent);
		if(l_strLineContent == " 62")	//如果有62说明是有颜色的，62组码是颜色号（固定）
		{
			m_cReadDXFCode.ReadString(l_strLineContent);
			l_cColor = g_pDoc->RtnColorRGB(atoi(l_strLineContent));	//是第几个颜色	
		}
		else if(l_strLineContent == " 10")	//走到“ 10”的下面，正好是起始点X的坐标
		{
			m_cReadDXFCode.ReadString(l_strLineContent);
			l_cCenter.x = atof(l_strLineContent);	
		}
		else if(l_strLineContent == " 20")	//走到“ 20”的下面，正好是起始点Y的坐标
		{
			m_cReadDXFCode.ReadString(l_strLineContent);
			l_cCenter.y = atof(l_strLineContent);	
		}
		else if(l_strLineContent == " 40")	//走到“40”的下面，正好是半径
		{
			m_cReadDXFCode.ReadString(l_strLineContent);
			l_cRadius = atof(l_strLineContent);	
		}
	}
	//求终点。
	l_cEndPos.x = l_cCenter.x + l_cRadius;
	l_cEndPos.y = l_cCenter.y;	
	//保存到链表里
	CCreateRound* l_pTempRound = new CCreateRound(l_cCenter,l_cEndPos,AD_COUNTERCLOCKWISE);
	CString l_strSBM,l_strCBM;//两个M代码
	g_pDoc->RtnColorMCode(l_strSBM,l_strCBM,-1,l_cColor);//输入线色返回M代码
	l_pTempRound->m_strSBM.RemoveAll();
	l_pTempRound->m_strCBM.RemoveAll();	
	l_pTempRound->m_strSBM.Add(l_strSBM);
	l_pTempRound->m_strCBM.Add(l_strCBM);
	l_pTempRound->m_ColorOfLine = l_cColor;		//把获得到的颜色加到图元里然后下面加进链表
	g_pDoc->m_CCreateBaseList.AddTail(l_pTempRound);			
}

//在DXF文件中发现POLYLINE行转成多线段
void COpenDXFCode::TurnPolyline()
{
	CString l_strLineContent;
	//   运算中的点		中心点		起始点  	结束点
	Position l_cPos,l_PosCenter,l_PosBegin,l_PosEnd;
	COLORREF l_cColor = g_pDoc->m_stcColorToM.m_color5;	//线色
		//是否是圆弧	圆弧的方向         
	int l_iNumOfPoint = 1,l_iClockWise = 0;
	double l_dArcParm;	//凸度
	//    是不是弧   是否是闭合的
	BOOL l_bIsArc,l_bIsRect=FALSE;
	//每个弧的凸度
	CArray<double,double&> TuDu;//凸度
	//期间的多线段
	CArray<Position,Position&> l_point;
	
	while(l_strLineContent != "  0")//看是不是闭合的
	{
		m_cReadDXFCode.ReadString(l_strLineContent);
		if (l_strLineContent == " 70")
		{			
			m_cReadDXFCode.ReadString(l_strLineContent);
			l_bIsRect = atoi(l_strLineContent);	//为1是闭合的				
		}
	}
	//3,在整个多段线里扫描//////////////////////////////////////////////
	while (l_strLineContent != "SEQEND")//扫描这段PolyLine直到遇到“SEQEND”结束
	{	
		m_cReadDXFCode.ReadString(l_strLineContent);
		//2,/////////////////////如果遇到“VERTEX”说明的一个顶点///////////////////////
		if (l_strLineContent == "VERTEX") //遇到“VERTEX”说明的一个顶点
		{	
			l_bIsArc = FALSE;	//开始是没有弧的标志，如果下面有42说明有弧的标志了
			//1,//////////////////到顶点里扫描///////////////////////////////
			while(l_strLineContent != "  0")//这段线结束
			{
				m_cReadDXFCode.ReadString(l_strLineContent);
				if(l_strLineContent == " 62")	//如果有62说明是有颜色的，62组码是颜色号（固定）
				{
					m_cReadDXFCode.ReadString(l_strLineContent);
					l_cColor = g_pDoc->RtnColorRGB(atoi(l_strLineContent));	//是第几个颜色	
				}
				else if(l_strLineContent == " 10")	//走到“ 10”的下面，正好是起始点X的坐标
				{
					m_cReadDXFCode.ReadString(l_strLineContent);
					l_cPos.x = atof(l_strLineContent);	
				}
				else if(l_strLineContent == " 20")	//走到“ 20”的下面，正好是起始点Y的坐标
				{
					m_cReadDXFCode.ReadString(l_strLineContent);
					l_cPos.y = atof(l_strLineContent);	
				}	

				else if(l_strLineContent == " 42")	//走到“ 42”的下面，多段线里的圆弧
				{	
					l_bIsArc = TRUE;	//标志是弧
					m_cReadDXFCode.ReadString(l_strLineContent);
					l_dArcParm = atof(l_strLineContent);//凸度
				}
				l_iNumOfPoint++;
			}	
			//1./////////////////////到顶点里扫描///////////////////////////////////
			m_point.Add(l_cPos);	//扫描完的点要放到点集里
			if (l_bIsArc == FALSE)	//如果不是圆弧
			{
				double l_dZore = 0;
				TuDu.Add(l_dZore);
			}
			else	//否则是圆弧
				TuDu.Add(l_dArcParm);
		//2.////////////////如果遇到“VERTEX”说明的一个顶点///////////////////////////////
		}
	}
	//3.在整个多段线里扫描/////////////////////////////////////////////////////////////
	if(l_bIsRect == TRUE)	//扫描完后如果是闭合的图形,就再把第一点加上
	{
		double l_dZore1 = 0.0;
		m_point.Add(m_point.GetAt(0));	//就把第一点再加进去
		TuDu.Add(l_dZore1);//凸度为0
	}
	//4，上面已经扫描完了，现在要查看有多少个点，看看他们是弧还是线以下就是生成图形	
	CCreatePolyline* l_pTempPolyline = new CCreatePolyline(m_point,TuDu);
	CString l_strSBM,l_strCBM;//两个M代码
	g_pDoc->RtnColorMCode(l_strSBM,l_strCBM,-1,l_cColor);//输入线色返回M代码
	l_pTempPolyline->m_strSBM.RemoveAll();
	l_pTempPolyline->m_strCBM.RemoveAll();	
	l_pTempPolyline->m_strSBM.Add(l_strSBM);
	l_pTempPolyline->m_strCBM.Add(l_strCBM);
	l_pTempPolyline->m_ColorOfLine = l_cColor;		//把获得到的颜色加到图元里然后下面加进链表
	g_pDoc->m_CCreateBaseList.AddTail(l_pTempPolyline);	//加到链表里
	//清除两个数组
	m_point.RemoveAll();
	TuDu.RemoveAll();
}
/*
 *	在DXF文件中发现POINT行转成多线段
 */
void COpenDXFCode::TurnPoint()
{
	CString l_strLineContent;
	Position l_cBeginPos;
	COLORREF l_cColor =g_pDoc->m_stcColorToM.m_color5;	//线色
	
	while(l_strLineContent != "  0")//这段线结束
	{
		m_cReadDXFCode.ReadString(l_strLineContent);
		if(l_strLineContent == " 62")	//如果有62说明是有颜色的，62组码是颜色号（固定）
		{
			m_cReadDXFCode.ReadString(l_strLineContent);
			l_cColor = g_pDoc->RtnColorRGB(atoi(l_strLineContent));	//是第几个颜色	
		}
		else if(l_strLineContent == " 10")	//走到“ 10”的下面，正好是起始点X的坐标
		{
			m_cReadDXFCode.ReadString(l_strLineContent);
			l_cBeginPos.x = atof(l_strLineContent);	
		}
		else if(l_strLineContent == " 20")	//走到“ 20”的下面，正好是起始点Y的坐标
		{
			m_cReadDXFCode.ReadString(l_strLineContent);
			l_cBeginPos.y = atof(l_strLineContent);	
		}
	}
	//保存到链表里
	CCreatePoint* l_pTempLine = new CCreatePoint(l_cBeginPos);	
	CString l_strSBM,l_strCBM;//两个M代码
	g_pDoc->RtnColorMCode(l_strSBM,l_strCBM,-1,l_cColor);//输入线色返回M代码
	l_pTempLine->m_strSBM.RemoveAll();
	l_pTempLine->m_strCBM.RemoveAll();	
	l_pTempLine->m_strSBM.Add(l_strSBM);
	l_pTempLine->m_strCBM.Add(l_strCBM);
	l_pTempLine->m_ColorOfLine = l_cColor;		//把获得到的颜色加到图元里然后下面加进链表
	g_pDoc->m_CCreateBaseList.AddTail(l_pTempLine);	
}
