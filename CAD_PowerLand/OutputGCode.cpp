// OutputGCode.cpp: implementation of the COutputGCode class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cad_powerland.h"
#include "OutputGCode.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COutputGCode::COutputGCode()
{
	m_dCurX = m_dCurY = 0;	//当前的点	
	m_strCBM_What = "";	//最后的M代码
	m_iColorLast = -1;	//最后的颜色
}

COutputGCode::~COutputGCode()
{

}

/*
 *	写文件-----总
 */
BOOL COutputGCode::OutputGCode(CString filename)
{
// STEP 1 
	// 创建一个文件，保存g格式信息
	if(!m_cGCode.Open(filename,CFile::modeCreate|CFile::modeWrite))
		return FALSE;
// STEP 2 : 将信息写入g文件中，必须按次序写文件
	if( !WriteHeader() ||!WriteContent() ||!WriteTail() )		// 写文件
	{
		return FALSE;
	}
// STEP 3 : 关闭文件
	m_cGCode.Close();
	return TRUE;
}

/*
 *	写文件头
 */
BOOL COutputGCode::WriteHeader()
{
	m_cGCode.Write("G90\r\n",sizeof("G90\n"));
	m_bFirstG00 = TRUE;
	CString str;
	//加文件头的几个M代码
	POSITION	position = g_pDoc->m_CCreateBaseList.GetHeadPosition();
	g_pDoc->m_CCreateBaseList.GetNext(position);
	while(position != NULL)
	{
		CCreateBase*	l_pCreateBase = (CCreateBase *)g_pDoc->m_CCreateBaseList.GetNext(position);
		//根据类型选择画什么样的G代码
		if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateMCode)))
		{
			CCreateMCode *l_pMCode = (CCreateMCode*)l_pCreateBase;
			str = l_pMCode->m_strM_What + "\r\n";
			m_cGCode.Write(str,strlen(str));
		}
		else
			break;
	}
	return TRUE;
}

/*
 *	写文件中
 */
BOOL COutputGCode::WriteContent()
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
BOOL COutputGCode::WriteTail()
{
	CString l_str="\r\nG00X0Y0Z0\r\n";
	l_str = m_strCBM_What + l_str+ "M23\r\nM24\r\nM30";//;
	m_cGCode.Write(l_str,strlen(l_str));	
	return TRUE;

}

/*
 *	数据转成G代码----画直线的
 */
BOOL COutputGCode::WriteLine(CCreateLine* pLine)
{
	double x1,y1,x2,y2;	//起终点的坐标
	CString l_content,l_strSBM_What;	//写入文件字符串的内容
	//起点的坐标
	x1 = pLine->m_Begin.x;
	y1 = pLine->m_Begin.y;
	//把对应M代码集转成字符串M
	l_strSBM_What = AnalysisAryM(pLine->m_strSBM);
	//				如果上一点和这一点不相等							颜色不一样   就要加一个G00
	if(fabs(m_dCurX-x1)>DISTANCE_ZERO||fabs(m_dCurY-y1)>DISTANCE_ZERO||(m_iColorLast != pLine->m_ColorOfLine))
	{
		if(m_bFirstG00 == TRUE)//如果是第一个不加下一行（\r\n）
			l_content.Format("G00 X%f Y%f\r\n",x1,y1);//就要快速定位到这点
		else
			l_content.Format("\r\nG00 X%f Y%f\r\n",x1,y1);//就要快速定位到这点		
		m_bFirstG00 = FALSE;
		if(g_pDoc->m_stcWorkFor.IsGlassGrind == 1)//如果是磨玻璃的工艺就要加G41和G42
			l_content.Format("G%d\r\nG00 X%f Y%f\r\n",pLine->m_iKnifeStyle,x1,y1);//就要快速定位到这点
		l_content = m_strCBM_What + l_content + l_strSBM_What+"\r\n";
		m_cGCode.Write(l_content,strlen(l_content));		
	}
	//终点的坐标
	m_dCurX = x2 = pLine->m_End.x;
	m_dCurY = y2 = pLine->m_End.y;
	l_content.Format("G01 X%f Y%f\r\n",x2,y2);
	m_cGCode.Write(l_content,strlen(l_content));
	//把对应颜色转成字符串M，当成最后的
	m_strCBM_What = AnalysisAryM(pLine->m_strCBM);	
	//把最后的颜色保存起来
	m_iColorLast = pLine->m_ColorOfLine;
	return TRUE;
}

/*
 *	数据转成G代码----画圆的
 */
BOOL COutputGCode::WriteRound(CCreateRound* pRound)
{
	double x1,y1;	//起终点的坐标
	CString l_content,l_strSBM_What;	//写入文件字符串的内容
	//起点的坐标
	x1 = pRound->m_EndPos.x;
	y1 = pRound->m_EndPos.y;
	//把对应颜色转成字符串M
	l_strSBM_What = AnalysisAryM(pRound->m_strSBM);
	
	//				如果上一点和这一点不相等							颜色不一样   就要加一个G00
	if(fabs(m_dCurX-x1)>DISTANCE_ZERO||fabs(m_dCurY-y1)>DISTANCE_ZERO||(m_iColorLast != pRound->m_ColorOfLine))
	{
		if(m_bFirstG00 == TRUE)//如果是第一个不加下一行（\r\n）
			l_content.Format("G00 X%f Y%f\r\n",x1,y1);//就要快速定位到这点
		else
			l_content.Format("\r\nG00 X%f Y%f\r\n",x1,y1);//就要快速定位到这点		
		m_bFirstG00 = FALSE;
		if(g_pDoc->m_stcWorkFor.IsGlassGrind == 1)//如果是磨玻璃的工艺就要加G41和G42
			l_content.Format("\r\nG%d\r\nG00 X%f Y%f\r\n",pRound->m_iKnifeStyle,x1,y1);//就要快速定位到这点
		l_content = m_strCBM_What + l_content + l_strSBM_What+"\r\n";
		m_cGCode.Write(l_content,strlen(l_content));
	}
	//把当前的点负上值
	m_dCurX = x1;
	m_dCurY = y1;
	//画圆
	int a = pRound->m_iDirect;
	if (pRound->m_iDirect == AD_CLOCKWISE)//如果是顺时针
		l_content.Format("G02 X%f Y%f I%f J%f\r\n",x1,y1,pRound->m_Center.x-pRound->m_EndPos.x,pRound->m_Center.y-pRound->m_EndPos.y);
	else//否则逆时针
		l_content.Format("G03 X%f Y%f I%f J%f\r\n",x1,y1,pRound->m_Center.x-pRound->m_EndPos.x,pRound->m_Center.y-pRound->m_EndPos.y);
	m_cGCode.Write(l_content,strlen(l_content));
	//把对应颜色转成字符串M
	m_strCBM_What = AnalysisAryM(pRound->m_strCBM);	
	return TRUE;
}

/*
 *	数据转成G代码----画多线段的
 */
BOOL COutputGCode::WritePolyline(CCreatePolyline* Polyline)
{
	//俩个点和圆心
	Position pos,posNext,l_posCenter;	
	//起终点的坐标,凸度
	double x1,y1,x2,y2,l_dTuDu;	
	CString l_content,l_strSBM_What;	//写入文件字符串的内容

	//把对应颜色转成字符串M
	l_strSBM_What = AnalysisAryM(Polyline->m_strSBM);
	//点集的多少
	int l_iSizeOfPoint = Polyline->m_point.GetSize();
	//生成代码
	for(int i=0;i<l_iSizeOfPoint-1;i++)
	{
		pos = Polyline->m_point.GetAt(i);
		posNext = Polyline->m_point.GetAt(i+1);
		l_dTuDu = Polyline->m_dTuDu.GetAt(i);
		//变成数据
		x1 = pos.x;
		y1 = pos.y;
		x2 = posNext.x;
		y2 = posNext.y;
		////多段线的第一点是否与上一点相同,颜色是否相同 
		if((i==0)&&(fabs(m_dCurX-x1)>DISTANCE_ZERO||fabs(m_dCurY-y1)>DISTANCE_ZERO||m_iColorLast != Polyline->m_ColorOfLine))
		{
			if(m_bFirstG00 == TRUE)//如果是第一个不加下一行（\r\n）
				l_content.Format("G00 X%f Y%f\r\n",x1,y1);//就要快速定位到这点
			else
				l_content.Format("\r\nG00 X%f Y%f\r\n",x1,y1);//就要快速定位到这点		
			m_bFirstG00 = FALSE;
			if(g_pDoc->m_stcWorkFor.IsGlassGrind == 1)//如果是磨玻璃的工艺就要加G41和G42
				l_content.Format("\r\nG%d\r\nG00 X%f Y%f\r\n",Polyline->m_iKnifeStyle,x1,y1);//就要快速定位到这点
			l_content = m_strCBM_What + l_content + l_strSBM_What+"\r\n";		
			m_cGCode.Write(l_content,strlen(l_content));
		}
		if(l_dTuDu == 0.0)//	直线
			l_content.Format("G01 X%f Y%f\r\n",x2,y2);
		else//圆弧
		{
			g_pDoc->polylineToArc(pos,posNext,l_dTuDu,l_posCenter);	
			if (l_dTuDu<0.0)//如果是顺时针
				l_content.Format("G02 X%f Y%f I%f J%f\r\n",x2,y2,l_posCenter.x - pos.x,l_posCenter.y - pos.y);
			else//否则逆时针
				l_content.Format("G03 X%f Y%f I%f J%f\r\n",x2,y2,l_posCenter.x - pos.x,l_posCenter.y - pos.y);			
		}
		//写入G文件中
		m_cGCode.Write(l_content,strlen(l_content));
		
		m_dCurX = x2;
		m_dCurY = y2;
	}
	//把对应颜色转成字符串M
	m_strCBM_What = AnalysisAryM(Polyline->m_strCBM);	
	//把最后的颜色保存起来
	m_iColorLast = Polyline->m_ColorOfLine;
	return TRUE;
}

/*
 *	数据转成G代码----画椭圆的
 */
BOOL COutputGCode::WriteEllipse(CCreateEllipse* Ellipse)
{
	Position pos;	//起点
	double x1,y1;	//起终点的坐标
	CString l_content,l_strSBM_What;	//写入文件字符串的内容
	
	//把对应颜色转成字符串M
	l_strSBM_What = AnalysisAryM(Ellipse->m_strSBM);
	//生成代码
	for(int i=0  ;   i < Ellipse->m_point.GetSize()  ;i++)
	{
		pos = Ellipse->m_point.GetAt(i);
		x1 = pos.x;
		y1 = pos.y;		
		if (i!=0)//零时为第一点
		{
			l_content.Format("G01 X%f Y%f\r\n",x1,y1);
			m_cGCode.Write(l_content,strlen(l_content));
		}
		else
		{
			m_dCurX = x1;
			m_dCurY = y1;
			if(m_bFirstG00 == TRUE)//如果是第一个不加下一行（\r\n）
				l_content.Format("G00 X%f Y%f\r\n",x1,y1);//就要快速定位到这点
			else
				l_content.Format("\r\nG00 X%f Y%f\r\n",x1,y1);//就要快速定位到这点		
			m_bFirstG00 = FALSE;
			if(g_pDoc->m_stcWorkFor.IsGlassGrind == 1)//如果是磨玻璃的工艺就要加G41和G42
				l_content.Format("\r\nG%d\r\nG00 X%f Y%f\r\n",Ellipse->m_iKnifeStyle,x1,y1);//第一点快速定位到这点
			l_content = m_strCBM_What + l_content + l_strSBM_What+"\r\n";
			m_cGCode.Write(l_content,strlen(l_content));
		}
	}
	//把对应颜色转成字符串M
	m_strCBM_What = AnalysisAryM(Ellipse->m_strCBM);	
	return TRUE;
}

/*
 *	数据转成G代码----画圆弧的
 */
BOOL COutputGCode::WriteArc(CCreateRoundArc* pArc)
{	
	double x1,y1;	//起终点的坐标
	CString l_content,l_strSBM_What;	//写入文件字符串的内容
	//起点的坐标
	x1 = pArc->m_Begin.x;
	y1 = pArc->m_Begin.y;
	//把对应颜色转成字符串M
	l_strSBM_What = AnalysisAryM(pArc->m_strSBM);
	
	//				如果上一点和这一点不相等							颜色不一样   就要加一个G00
	if(fabs(m_dCurX-x1)>DISTANCE_ZERO||fabs(m_dCurY-y1)>DISTANCE_ZERO||(m_iColorLast != pArc->m_ColorOfLine))
	{
		if(m_bFirstG00 == TRUE)//如果是第一个不加下一行（\r\n）
			l_content.Format("G00 X%f Y%f\r\n",x1,y1);//就要快速定位到这点
		else
			l_content.Format("\r\nG00 X%f Y%f\r\n",x1,y1);//就要快速定位到这点		
		m_bFirstG00 = FALSE;
		if(g_pDoc->m_stcWorkFor.IsGlassGrind == 1)//如果是磨玻璃的工艺就要加G41和G42
			l_content.Format("\r\nG%d\r\nG00 X%f Y%f\r\n",pArc->m_iKnifeStyle,x1,y1);//就要快速定位到这点
		l_content = m_strCBM_What + l_content + l_strSBM_What+"\r\n";
		m_cGCode.Write(l_content,strlen(l_content));			
	}	
	
	//画圆弧
	m_dCurX = x1 = pArc->m_End.x;
	m_dCurY = y1 = pArc->m_End.y;

	if (pArc->m_iDirect == AD_CLOCKWISE)//如果是顺时针
		l_content.Format("G02 X%f Y%f I%f J%f\r\n",x1,y1,   \
		pArc->m_Center.x - pArc->m_Begin.x   ,pArc->m_Center.y - pArc->m_Begin.y);
	else//否则逆时针
		l_content.Format("G03 X%f Y%f I%f J%f\r\n",x1,y1,   \
		pArc->m_Center.x - pArc->m_Begin.x   ,pArc->m_Center.y - pArc->m_Begin.y);
	m_cGCode.Write(l_content,strlen(l_content));
	//把对应颜色转成字符串M
	m_strCBM_What = AnalysisAryM(pArc->m_strCBM);	
	//把最后的颜色保存起来
	m_iColorLast = pArc->m_ColorOfLine;
	return TRUE;
}

/*
 *	数据转成G代码----画矩形的
 */
BOOL COutputGCode::WriteRectangle(CCreateRectangle* pRectangle)
{
	//俩个点和圆心
	Position pos,posNext,l_posCenter;	
	//起终点的坐标,凸度
	double x1,y1,x2,y2,l_dTuDu;	
	CString l_content,l_strSBM_What;	//写入文件字符串的内容
	
	//把对应颜色转成字符串M
	l_strSBM_What = AnalysisAryM(pRectangle->m_strSBM);
	//点集的多少
	int l_iSizeOfPoint = pRectangle->m_point.GetSize();
	//生成代码
	for(int i=0;i<l_iSizeOfPoint-1;i++)
	{
		pos = pRectangle->m_point.GetAt(i);
		posNext = pRectangle->m_point.GetAt(i+1);
		l_dTuDu = pRectangle->m_dTuDu.GetAt(i);
		//变成数据
		x1 = pos.x;
		y1 = pos.y;
		x2 = posNext.x;
		y2 = posNext.y;
		////多段线的第一点是否与上一点相同,颜色是否相同 
		if((i==0)&&(fabs(m_dCurX-x1)>DISTANCE_ZERO||fabs(m_dCurY-y1)>DISTANCE_ZERO||m_iColorLast != pRectangle->m_ColorOfLine))
		{
			if(m_bFirstG00 == TRUE)//如果是第一个不加下一行（\r\n）
				l_content.Format("G00 X%f Y%f\r\n",x1,y1);//就要快速定位到这点
			else
				l_content.Format("\r\nG00 X%f Y%f\r\n",x1,y1);//就要快速定位到这点		
			m_bFirstG00 = FALSE;
			if(g_pDoc->m_stcWorkFor.IsGlassGrind == 1)//如果是磨玻璃的工艺就要加G41和G42
				l_content.Format("\r\nG%d\r\nG00 X%f Y%f\r\n",pRectangle->m_iKnifeStyle,x1,y1);//就要快速定位到这点
			l_content = m_strCBM_What + l_content + l_strSBM_What+"\r\n";		
			m_cGCode.Write(l_content,strlen(l_content));
		}
		if(l_dTuDu == 0.0)//	直线
			l_content.Format("G01 X%f Y%f\r\n",x2,y2);
		else//圆弧
		{
			g_pDoc->polylineToArc(pos,posNext,l_dTuDu,l_posCenter);	
			if (l_dTuDu<0.0)//如果是顺时针
				l_content.Format("G02 X%f Y%f I%f J%f\r\n",x2,y2,l_posCenter.x - pos.x,l_posCenter.y - pos.y);
			else//否则逆时针
				l_content.Format("G03 X%f Y%f I%f J%f\r\n",x2,y2,l_posCenter.x - pos.x,l_posCenter.y - pos.y);			
		}
		//写入G文件中
		m_cGCode.Write(l_content,strlen(l_content));
		
		m_dCurX = x2;
		m_dCurY = y2;
	}
	//把对应颜色转成字符串M
	m_strCBM_What = AnalysisAryM(pRectangle->m_strCBM);	
	//把最后的颜色保存起来
	m_iColorLast = pRectangle->m_ColorOfLine;
	return TRUE;
}

/*
 *	根据颜色转成字符抬刀M多少的函数
 */
CString COutputGCode::CintToCBMstr(int RtnColor)
{
	CString l_strM;		//根据颜色转成字符
	//字符由外部对应设定的
	switch(RtnColor)	
	{
	case 1:
		l_strM = g_pDoc->m_stcColorToM.m_CBM1;
		break;
	case 2:
		l_strM = g_pDoc->m_stcColorToM.m_CBM2;
		break;
	case 3:
		l_strM = g_pDoc->m_stcColorToM.m_CBM3;
		break;
	case 4:
		l_strM = g_pDoc->m_stcColorToM.m_CBM4;
		break;
	case 5:
		l_strM = g_pDoc->m_stcColorToM.m_CBM5;
		break;
	default:
		break;
	}
	return l_strM;
}

/*
 *	根据颜色转成字符下刀M多少的函数
 */
CString COutputGCode::CintToSBMstr(int RtnColor)
{
	CString l_strM;		//根据颜色转成字符
	//字符由外部对应设定的
	switch(RtnColor)	
	{
	case 1:
		l_strM = g_pDoc->m_stcColorToM.m_SBM1;
		break;
	case 2:
		l_strM = g_pDoc->m_stcColorToM.m_SBM2;
		break;
	case 3:
		l_strM = g_pDoc->m_stcColorToM.m_SBM3;
		break;
	case 4:
		l_strM = g_pDoc->m_stcColorToM.m_SBM4;
		break;
	case 5:
		l_strM = g_pDoc->m_stcColorToM.m_SBM5;
		break;
	default:
		break;
	}
	return l_strM;
}

/*
 *	数据转成G代码----画点的
 */
BOOL COutputGCode::WritePoint(CCreatePoint* Point)
{
	double x1,y1;	//起终点的坐标
	CString l_content,l_strSBM_What;	//写入文件字符串的内容
	//起点的坐标
	x1 = Point->m_Begin.x;
	y1 = Point->m_Begin.y;
	//把对应颜色转成字符串M
	l_strSBM_What = AnalysisAryM(Point->m_strSBM);	
	//				如果上一点和这一点不相等							颜色不一样   就要加一个G00
	if(m_bFirstG00 == TRUE)//如果是第一个不加下一行（\r\n）
		l_content.Format("G00 X%f Y%f\r\n",x1,y1);//就要快速定位到这点
	else
		l_content.Format("\r\nG00 X%f Y%f\r\n",x1,y1);//就要快速定位到这点		
	m_bFirstG00 = FALSE;
	if(g_pDoc->m_stcWorkFor.IsGlassGrind == 1)//如果是磨玻璃的工艺就要加G41和G42
		l_content.Format("\r\nG%d\r\nG00 X%f Y%f\r\n",Point->m_iKnifeStyle,x1,y1);//就要快速定位到这点
	l_content = m_strCBM_What + l_content + l_strSBM_What+"\r\n";
	m_cGCode.Write(l_content,strlen(l_content));		
	//终点的坐标
	m_dCurX = Point->m_Begin.x;
	m_dCurY = Point->m_Begin.x;
	//把对应颜色转成字符串M，当成最后的
	m_strCBM_What = AnalysisAryM(Point->m_strCBM);	
	//把最后的颜色保存起来
	m_iColorLast = Point->m_ColorOfLine;
	
	return TRUE;
}

/*
 *	把M代码集解析成字符串
 */
CString COutputGCode::AnalysisAryM(CArray<class CString,class CString &>& aryM)
{
	int l_iSizeAryM = aryM.GetSize();
	CString l_strMCode = "";
	for(int i=0;i<l_iSizeAryM;i++)
	{
		if(i != 0)
			l_strMCode = l_strMCode + "\r\n";
		l_strMCode = l_strMCode + aryM.GetAt(i);
	}
	return l_strMCode;
}





