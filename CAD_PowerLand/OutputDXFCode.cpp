// OutputDXFCode.cpp: implementation of the COutputDXFCode class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cad_powerland.h"
#include "OutputDXFCode.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COutputDXFCode::COutputDXFCode()
{

}

COutputDXFCode::~COutputDXFCode()
{

}

BOOL COutputDXFCode::OutputDXFCode(CString filename)
{

	// STEP 1 
	// 创建一个文件，保存g格式信息
	if(!m_cDXFCode.Open(filename,CFile::modeCreate|CFile::modeWrite))
		return FALSE;
	// STEP 2 : 将信息写入g文件中，必须按次序写文件
	if( !WriteHeader() ||!WriteContent() ||!WriteTail() )		// 写文件
	{
		return FALSE;
	}
	// STEP 3 : 关闭文件
	m_cDXFCode.Close();
	return TRUE;
}

/*
 *	写文件头
 */
BOOL COutputDXFCode::WriteHeader()
{
	CString l_strHeader = "  0\nSECTION\n  2\nHEADER\n  9\n$ACADVER\n  1\nAC1009\n  9\n";
	m_cDXFCode.Write(l_strHeader,strlen(l_strHeader));
	l_strHeader = "$ANGBASE\n 50\n0.0\n  9\n$ANGDIR\n 70\n     0\n  9\n$LUNITS\n 70\n     0\n  0\n";
	m_cDXFCode.Write(l_strHeader,strlen(l_strHeader));
	l_strHeader = "ENDSEC\n  0\nSECTION\n  2\nENTITIES\n";
	m_cDXFCode.Write(l_strHeader,strlen(l_strHeader));
	return TRUE;
}

/*
 *	写文件中
 */
BOOL COutputDXFCode::WriteContent()
{
	POSITION	position = g_pDoc->m_CCreateBaseList.GetHeadPosition();
	while(position != NULL)
	{
		CCreateBase*	l_pCreateBase = (CCreateBase *)g_pDoc->m_CCreateBaseList.GetNext(position);
		//根据类型选择画什么样的G代码
		if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateLine)))
		{
			CCreateLine *line = (CCreateLine*)l_pCreateBase;
			WriteLine(line);	//直线
		}
		else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreatePolyline))
			||l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateSpline)))
		{
			CCreatePolyline *Polyline = (CCreatePolyline*)l_pCreateBase;
			WritePolyline(Polyline);	//多线段
		}
		else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRectangle)))
		{
			CCreateRectangle *Rectangle = (CCreateRectangle*)l_pCreateBase;
			WriteRectangle(Rectangle);//矩形
		}
		else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRound)))
		{
			CCreateRound *Round = (CCreateRound*)l_pCreateBase;
			int A = Round->m_iDirect;
			WriteRound(Round);//圆
		}

		else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRoundArc)))
		{
			CCreateRoundArc *Arc = (CCreateRoundArc*)l_pCreateBase;
			WriteArc(Arc);	//圆弧
		}
		else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateEllipse)))
		{
			CCreateEllipse *Ellipse = (CCreateEllipse*)l_pCreateBase;
			WriteEllipse(Ellipse);//椭圆
		}
		else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreatePoint)))
		{
			CCreatePoint *Point = (CCreatePoint*)l_pCreateBase;
			WritePoint(Point);//点
		}
	}
	return TRUE;
}

/*
 *	写文件尾
 */
BOOL COutputDXFCode::WriteTail()
{
	CString l_strHeader = "  0\nENDSEC\n  0\nEOF";
	m_cDXFCode.Write(l_strHeader,strlen(l_strHeader));
	return TRUE;
}

/*
 *	数据转成G代码----画直线的
 */
BOOL COutputDXFCode::WriteLine(CCreateLine* pLine)
{
	double x1,y1,x2,y2;	//起终点的坐标
	CString l_content;	//写入文件字符串的内容
	//起点的坐标
	x1 = pLine->m_Begin.x;
	y1 = pLine->m_Begin.y;
	//终点的坐标
	x2 = pLine->m_End.x;
	y2 = pLine->m_End.y;
	//把对应颜色和坐标格式到字符里，下面含意看这个类的头文件里的注释
	//				图元类型	图层	  颜色		起点X	起点Y								
	l_content.Format("  0\nLINE\n  8\n0\n 62\n     %d\n 10\n%.16f\n 20\n%.16f\n 30\n0.0\n\
 11\n%.16f\n 21\n%.16f\n 31\n0.0\n",pLine->RtnColorInt()-1,x1,y1,x2,y2);
	//写到文件中      终点X		终点Y					
	m_cDXFCode.Write(l_content,strlen(l_content));		
	return TRUE;
}

/*
 *	数据转成G代码----画圆的
 */
BOOL COutputDXFCode::WriteRound(CCreateRound* pRound)
{
	double x1,y1,radius;	//起终点的坐标
	CString l_content;	//写入文件字符串的内容
	//起点的坐标和半径
	x1 = pRound->m_Center.x;
	y1 = pRound->m_Center.y;
	radius = pRound->m_dRadius;
	//把对应颜色和坐标格式到字符里，下面含意看这个类的头文件里的注释
	//			    图元类型        图层	  颜色		圆心X	圆心Y                半径
	l_content.Format("  0\nCIRCLE\n  8\n0\n 62\n     %d\n 10\n%.16f\n 20\n%.16f\n 30\n0.0\n 40\n%.16f\n"\
					,pRound->RtnColorInt()-1,x1,y1,radius);//就要快速定位到这点
	m_cDXFCode.Write(l_content,strlen(l_content));
	return TRUE;
}

/*
 *	数据转成G代码----画多线段的
 */
BOOL COutputDXFCode::WritePolyline(CCreatePolyline* Polyline)
{
	int l_iPointSize = Polyline->m_point.GetSize();
	if(l_iPointSize<2)	//在多段线里最后是弧时，最后一点不要。
		return FALSE;
	CString l_content;	//写入文件字符串的内容
	Position pos;	//多段线中的每个点
	//      起终点的坐标
	double l_dPosx,l_dPosy,l_dTuDu;	
	//返回图元的颜色
	int l_iColor = Polyline->RtnColorInt()-1;
	//多段线块的开头	图元类型		图层					颜色
	l_content.Format("  0\nPOLYLINE\n  8\n0\n  6\nCONTINUOUS\n 62\n     %d\n 66\n     1\n\
 10\n0.0\n 20\n0.0\n 30\n0.0\n",l_iColor);
	//写入文件中
	m_cDXFCode.Write(l_content,strlen(l_content));
	//把点取出来然后写进DXF文件中
	for(int i=0  ;   i < l_iPointSize  ;i++)
	{
		pos = Polyline->m_point.GetAt(i);
		l_dTuDu = Polyline->m_dTuDu.GetAt(i);
		l_dPosx = pos.x;
		l_dPosy = pos.y;	
		if (l_dTuDu == 0.0)
		//多段线块的中间内容	图元类型  图层						颜色		 点X		点Y
		l_content.Format("  0\nVERTEX\n  8\n0\n  6\nCONTINUOUS\n 62\n     %d\n 10\n%.16f\n 20\n%.16f\n 30\n0.0\n",
			l_iColor,l_dPosx,l_dPosy);//第一点快速定位到这点
		else
		//多段线块的中间内容	图元类型  图层						颜色		 点X		点Y
		l_content.Format("  0\nVERTEX\n  8\n0\n  6\nCONTINUOUS\n 62\n     %d\n 10\n%.16f\n 20\n%.16f\n 30\n0.0\n 42\n%.16f\n",
			l_iColor,l_dPosx,l_dPosy,l_dTuDu);//第一点快速定位到这点
		//写入文件
		m_cDXFCode.Write(l_content,strlen(l_content));
	}
	//多段线块的结束内容
	l_content.Format("  0\nSEQEND\n  8\n0\n  6\nCONTINUOUS\n 62\n     %d\n",l_iColor);
	m_cDXFCode.Write(l_content,strlen(l_content));		
	return TRUE;
}

/*
 *	数据转成G代码----画椭圆的
 */
BOOL COutputDXFCode::WriteEllipse(CCreateEllipse* Ellipse)
{
	CString l_content;	//写入文件字符串的内容
	Position pos;	//多段线中的每个点
	double l_dPosx,l_dPosy;	//起终点的坐标
	//返回图元的颜色
	int l_iColor = Ellipse->RtnColorInt()-1;
	//多段线块的开头	图元类型		图层					颜色
	l_content.Format("  0\nPOLYLINE\n  8\n0\n  6\nCONTINUOUS\n 62\n     %d\n 66\n     1\n\
 10\n0.0\n 20\n0.0\n 30\n0.0\n",l_iColor);
	//写入文件中
	m_cDXFCode.Write(l_content,strlen(l_content));
	//把点取出来然后写进DXF文件中
	for(int i=0  ;   i < Ellipse->m_point.GetSize()  ;i++)
	{
		pos = Ellipse->m_point.GetAt(i);
		l_dPosx = pos.x;
		l_dPosy = pos.y;		
		//多段线块的中间内容	图元类型  图层						颜色     点X		点Y
		l_content.Format("  0\nVERTEX\n  8\n0\n  6\nCONTINUOUS\n 62\n     %d\n 10\n%.16f\n 20\n%.16f\n 30\n0.0\n",
			l_iColor,l_dPosx,l_dPosy);//第一点快速定位到这点
		//写入文件
		m_cDXFCode.Write(l_content,strlen(l_content));
	}
	//多段线块的结束内容
	l_content.Format("  0\nSEQEND\n  8\n0\n  6\nCONTINUOUS\n 62\n     %d\n",l_iColor);
	m_cDXFCode.Write(l_content,strlen(l_content));	
	
	return TRUE;
}

/*
 *	数据转成G代码----画圆弧的
 */
BOOL COutputDXFCode::WriteArc(CCreateRoundArc* pArc)
{
	
//	"0\nARC\n  8\n0\n  10\n%f\n  20\n%f\n  30\n0.0\n  40\n%f  50\n%f\n  51\n%f\n"
	//	    起点X			起点Y		半径		起始角度   终止角度
	double l_dCenterPosX,l_dCenterPosY,l_dRadius,l_dAngleBG,  l_dAngleEN;	
	//圆心的坐标
	l_dCenterPosX = pArc->m_Center.x;
	l_dCenterPosY = pArc->m_Center.y;
	//半径
	l_dRadius = pArc->m_Begin.Distance(pArc->m_Center);
	//起始角度和终止角度
	l_dAngleBG = GetAngle(pArc->m_Center,pArc->m_Begin);
	l_dAngleEN = GetAngle(pArc->m_Center,pArc->m_End);
	//写入文件字符串的内容
	CString l_strCcontent;	
	//判断一下正反转
	if (pArc->m_iDirect == AD_COUNTERCLOCKWISE)//
		//			          图元类型	 图层	 颜色      圆心X	 圆心Y							   半径	   起始角度   终止角度
		l_strCcontent.Format("  0\nARC\n  8\n0\n 62\n     %d\n 10\n%.16f\n 20\n%.16f\n 30\n0.0\n 40\n%.16f\n 50\n%.16f\n 51\n%.16f\n", \
			pArc->RtnColorInt()-1,l_dCenterPosX,l_dCenterPosY,l_dRadius,l_dAngleBG,l_dAngleEN);
	else
		//			          图元类型	 图层	 颜色					 圆心X	 圆心Y		           半径	   起始角度   终止角度
		l_strCcontent.Format("  0\nARC\n  8\n0\n 62\n     %d\n 10\n%.16f\n 20\n%.16f\n 30\n0.0\n 40\n%.16f\n 50\n%.16f\n 51\n%.16f\n", \
			pArc->RtnColorInt()-1,l_dCenterPosX,l_dCenterPosY,l_dRadius,l_dAngleEN,l_dAngleBG);
	//写入文件
	m_cDXFCode.Write(l_strCcontent,strlen(l_strCcontent));
	return TRUE;
}

/*
 *	数据转成G代码----画矩形的
 */
BOOL COutputDXFCode::WriteRectangle(CCreateRectangle* pRectangle)
{
	int l_iPointSize = pRectangle->m_point.GetSize();
	if(l_iPointSize<2)	//在多段线里最后是弧时，最后一点不要。
		return FALSE;
	CString l_content;	//写入文件字符串的内容
	Position pos;	//多段线中的每个点
	//      起终点的坐标
	double l_dPosx,l_dPosy,l_dTuDu;	
	//返回图元的颜色
	int l_iColor = pRectangle->RtnColorInt()-1;
	//多段线块的开头	图元类型		图层					颜色
	l_content.Format("  0\nPOLYLINE\n  8\n0\n  6\nCONTINUOUS\n 62\n     %d\n 66\n     1\n\
		10\n0.0\n 20\n0.0\n 30\n0.0\n",l_iColor);
	//写入文件中
	m_cDXFCode.Write(l_content,strlen(l_content));
	//把点取出来然后写进DXF文件中
	for(int i=0  ;   i < l_iPointSize  ;i++)
	{
		pos = pRectangle->m_point.GetAt(i);
		l_dTuDu = pRectangle->m_dTuDu.GetAt(i);
		l_dPosx = pos.x;
		l_dPosy = pos.y;	
		if (l_dTuDu == 0.0)
			//多段线块的中间内容	图元类型  图层						颜色		 点X		点Y
			l_content.Format("  0\nVERTEX\n  8\n0\n  6\nCONTINUOUS\n 62\n     %d\n 10\n%.16f\n 20\n%.16f\n 30\n0.0\n",
			l_iColor,l_dPosx,l_dPosy);//第一点快速定位到这点
		else
			//多段线块的中间内容	图元类型  图层						颜色		 点X		点Y
			l_content.Format("  0\nVERTEX\n  8\n0\n  6\nCONTINUOUS\n 62\n     %d\n 10\n%.16f\n 20\n%.16f\n 30\n0.0\n 42\n%.16f\n",
			l_iColor,l_dPosx,l_dPosy,l_dTuDu);//第一点快速定位到这点
		//写入文件
		m_cDXFCode.Write(l_content,strlen(l_content));
	}
	//多段线块的结束内容
	l_content.Format("  0\nSEQEND\n  8\n0\n  6\nCONTINUOUS\n 62\n     %d\n",l_iColor);
	m_cDXFCode.Write(l_content,strlen(l_content));			
	return TRUE;
}

/*
 *	数据转成G代码----画点的
 */
BOOL COutputDXFCode::WritePoint(CCreatePoint* Point)
{
	double x1,y1;	//起终点的坐标
	CString l_content;	//写入文件字符串的内容
	//点的坐标
	x1 = Point->m_Begin.x;
	y1 = Point->m_Begin.y;

	//把对应颜色和坐标格式到字符里，下面含意看这个类的头文件里的注释
	//				图元类型	图层	  颜色		起点X	起点Y								
	l_content.Format("  0\nPOINT\n  8\n0\n 62\n     %d\n 10\n%.16f\n 20\n%.16f\n 30\n0.0\n",Point->RtnColorInt()-1,x1,y1);
	//写到文件中      终点X		终点Y					
	m_cDXFCode.Write(l_content,strlen(l_content));	
	return TRUE;
}