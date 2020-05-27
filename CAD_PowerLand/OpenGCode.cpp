// OpenGCode.cpp: implementation of the COpenGCode class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cad_powerland.h"
#include "OpenGCode.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COpenGCode::COpenGCode()
	:m_Cur(0,0)
{
	m_iStyle = -1;
	m_dCurTudu = 0.0;
	m_point.RemoveAll();
	m_aryTudu.RemoveAll();
	m_CollectOfG00.RemoveAll();
}

COpenGCode::~COpenGCode()
{
	m_point.RemoveAll();
	m_aryTudu.RemoveAll();
}

/*
 *	被DOC调用的打开文件解释G代码
 */
BOOL COpenGCode::ReadGCode(CString PathName)
{	
	CString  l_strLineContent;	//每行文本的内容
	CFileException ex;	
	BOOL isEof=FALSE;		
	if (!m_cReadGCode.Open(PathName,CFile::modeRead | CFile::shareDenyWrite, &ex)) //打开文件		
	{	
		//如果没打开就报错
		TCHAR szError[1024];		
		ex.GetErrorMessage(szError, 1024);		
		AfxMessageBox(szError);		
		return  FALSE;		
	}
	else		
	{	
		//读取文本
 		while(!isEof)			
		{	//从第一行开始读，
			isEof=!m_cReadGCode.ReadString(l_strLineContent);//读取一行文本。如果最后一行了就是isEof==1			
			if(isEof)//如果没有了就是1并退出
				break;	
			if(l_strLineContent.Find("G00") != -1 )
			{
				TurnG00(l_strLineContent);
			}
			else if(l_strLineContent.Find("G01") != -1)
			{
				TurnG01(l_strLineContent);
			}
			else if(l_strLineContent.Find("G02") != -1)
			{
				TurnG02(l_strLineContent);
			}
			else if(l_strLineContent.Find("G03") != -1)
			{
				TurnG03(l_strLineContent);
			}
			else if(l_strLineContent.Find("M") != -1)
			{
				MWhat(l_strLineContent);
			}
			else if(l_strLineContent.Find("G41") != -1)
			{
				m_iG41orG42 = 41;
			}
			else if(l_strLineContent.Find("G42") != -1)
			{
				m_iG41orG42 = 42;
			}
		}
		if(m_iStyle == 0&&m_aryOnceM.GetSize()>1)	//如果是点的话
		{
			/*玻璃里没有点
			CCreatePoint* pPoint = new CCreatePoint(m_Cur);
			pPoint->m_ColorOfLine = DistriPointMCode();
			pPoint->m_strSBM.Copy(m_aryOnceM);
			pPoint->m_strCBM.Copy(m_aryTwicM);	
			g_pDoc->m_CCreateBaseList.AddTail(pPoint);
			
			m_CollectOfG00.RemoveAll();
			m_point.RemoveAll();
			m_aryTudu.RemoveAll();
			m_aryOnceM.RemoveAll();	
			m_aryTwicM.RemoveAll();	*/	
		}
		//否则是图形，说明该画图了
		Draw(RtnColor(),m_Cur);		
		g_pView->Invalidate(TRUE);
	}	

	return TRUE;
}

/*
 *	解释G00
 */
void COpenGCode::TurnG00(CString LineG00)
{
	Position posPre = m_Cur;
	int nLength = LineG00.GetLength();
	int nNum = LineG00.FindOneOf("X");
	if (nLength>nNum)
	{
		m_Cur.x = atof(LineG00.Mid(nNum+1));
	}
	nNum = LineG00.FindOneOf("Y");
	if (nLength>nNum)
	{
		m_Cur.y = atof(LineG00.Mid(nNum+1));
	}
	m_iStyle = 0;	//当前线型

	if (m_point.GetSize() == 0)//在磨玻璃工艺中，开头要有几个M代码要保存的！
	{	
		if (m_iStyle == -1)
		{
			for(int i=0;i<m_aryTwicM.GetSize();i++)
			{
				CCreateMCode* pTempMWhat = new CCreateMCode(m_aryTwicM.GetAt(i));
				g_pDoc->m_CCreateBaseList.AddTail(pTempMWhat);
			}
			m_aryTwicM.RemoveAll();
			return;
		}
		else if(m_iStyle == 0&&m_aryOnceM.GetSize()>1)	//如果是点的话
		{
/*玻璃里没有点
			CCreatePoint* pPoint = new CCreatePoint(posPre);
			pPoint->m_ColorOfLine = DistriPointMCode();
			pPoint->m_strSBM.Copy(m_aryOnceM);
			pPoint->m_strCBM.Copy(m_aryTwicM);	
			g_pDoc->m_CCreateBaseList.AddTail(pPoint);
			
			m_CollectOfG00.RemoveAll();
			m_point.RemoveAll();
			m_aryTudu.RemoveAll();
			m_aryOnceM.RemoveAll();	
			m_aryTwicM.RemoveAll();	*/
			return;		
		}
	}
	//否则是图形，说明该画图了
	Draw(RtnColor(),posPre);
}

/*
 *	解释G01
 */
void COpenGCode::TurnG01(CString LineG01)
{
	m_point.Add(m_Cur);
	m_dCurTudu = 0.0;
	m_aryTudu.Add(m_dCurTudu);
	int nLength = LineG01.GetLength();
	int nNum = LineG01.FindOneOf("X");
	if (nLength>nNum)
	{
		m_Cur.x = atof(LineG01.Mid(nNum+1));	
	}
	nNum = LineG01.FindOneOf("Y");
	if (nLength>nNum)
	{
		m_Cur.y = atof(LineG01.Mid(nNum+1));
	}
	m_iStyle = 1;	//当前线型
}

/*
 *	解释G02
 */
void COpenGCode::TurnG02(CString LineG02)
{
	//圆弧的末点	
	Position l_posEnd,l_posCenter,l_posBegin;
	//中心X减起始X      中心X减起始Y 
	double l_dCtrSubBgX,l_dCtrSubBgY,l_dTudu=0.0;
	int nLength = LineG02.GetLength();
	int nNum = LineG02.FindOneOf("X");	//终点X
	if (nLength>nNum)
	{
		l_posEnd.x = atof(LineG02.Mid(nNum+1));
	}
	nNum = LineG02.FindOneOf("Y");//终点Y
	if (nLength>nNum)
	{
		l_posEnd.y = atof(LineG02.Mid(nNum+1));
	}
	nNum = LineG02.FindOneOf("I");	//中心X减起始X
	if (nLength>nNum)
	{
		l_dCtrSubBgX = atof(LineG02.Mid(nNum+1));
	}
	nNum = LineG02.FindOneOf("J");	//中心Y减起始Y
	if (nLength>nNum)
	{
		l_dCtrSubBgY = atof(LineG02.Mid(nNum+1));
	}
	//圆弧的中心点
	l_posCenter.x = m_Cur.x+l_dCtrSubBgX;
	l_posCenter.y = m_Cur.y+l_dCtrSubBgY;
	//求凸度
	l_dTudu = g_pDoc->RtnTudu(m_Cur,l_posEnd,l_posCenter,2);
	//如果是圆的话
	if(fabs(m_Cur.x-l_posEnd.x)<DOUBLE_EQUAL&&fabs(m_Cur.y-l_posEnd.y)<DOUBLE_EQUAL)
	{
		m_point.Add(l_posEnd);
		l_dTudu = 0.0;
		m_aryTudu.Add(l_dTudu);
		if(m_point.GetSize()>=2)
		{
			CCreatePolyline* l_pTempPolyline = new CCreatePolyline(m_point,m_aryTudu);	
			m_CollectOfG00.Add(l_pTempPolyline);//把图元加进到中间的链表，等下一个M代码出现时要负上颜色
		}
		m_point.RemoveAll();	//并且删除点
		m_aryTudu.RemoveAll();
		//上面是加一个多段线，下面加圆
		CCreateRound* l_pRound = new CCreateRound(l_posCenter,l_posEnd,AD_CLOCKWISE);	//构造
		m_CollectOfG00.Add(l_pRound);//把图元加进到中间的链表，等下一个M代码出现时要负上颜色	
	}
	else
	{
		m_point.Add(m_Cur);
		m_aryTudu.Add(l_dTudu);
	}
	m_Cur = l_posEnd;
	m_dCurTudu = l_dTudu;
	m_iStyle = 2;	//当前线型
}

/*
 *	解释G03
 */
void COpenGCode::TurnG03(CString LineG03)
{
	//圆弧的末点	
	Position l_posEnd,l_posCenter,l_posBegin;
	//中心X减起始X      中心X减起始Y 
	double l_dCtrSubBgX,l_dCtrSubBgY,l_dTudu=0.0;
	int nLength = LineG03.GetLength();
	int nNum = LineG03.FindOneOf("X");	//终点X
	if (nLength>nNum)
	{
		l_posEnd.x = atof(LineG03.Mid(nNum+1));
	}
	nNum = LineG03.FindOneOf("Y");//终点Y
	if (nLength>nNum)
	{
		l_posEnd.y = atof(LineG03.Mid(nNum+1));
	}
	nNum = LineG03.FindOneOf("I");	//中心X减起始X
	if (nLength>nNum)
	{
		l_dCtrSubBgX = atof(LineG03.Mid(nNum+1));
	}
	nNum = LineG03.FindOneOf("J");	//中心Y减起始Y
	if (nLength>nNum)
	{
		l_dCtrSubBgY = atof(LineG03.Mid(nNum+1));
	}
	//圆弧的中心点
	l_posCenter.x = m_Cur.x+l_dCtrSubBgX;
	l_posCenter.y = m_Cur.y+l_dCtrSubBgY;
	//求凸度
	l_dTudu = g_pDoc->RtnTudu(m_Cur,l_posEnd,l_posCenter,3);
	//如果是圆的话
	if(fabs(m_Cur.x-l_posEnd.x)<DOUBLE_EQUAL&&fabs(m_Cur.y-l_posEnd.y)<DOUBLE_EQUAL)
	{
		m_point.Add(l_posEnd);
		l_dTudu = 0.0;
		m_aryTudu.Add(l_dTudu);
		if(m_point.GetSize()>=2)
		{
			CCreatePolyline* l_pTempPolyline = new CCreatePolyline(m_point,m_aryTudu);	
			m_CollectOfG00.Add(l_pTempPolyline);//把图元加进到中间的链表，等下一个M代码出现时要负上颜色
		}
		m_point.RemoveAll();	//并且删除点
		m_aryTudu.RemoveAll();
		//上面是加一个多段线，下面加圆
		CCreateRound* l_pRound = new CCreateRound(l_posCenter,l_posEnd,AD_COUNTERCLOCKWISE);	//构造
		m_CollectOfG00.Add(l_pRound);//把图元加进到中间的链表，等下一个M代码出现时要负上颜色	
	}
	else
	{
		m_point.Add(m_Cur);
		m_aryTudu.Add(l_dTudu);	
	}
	m_dCurTudu = l_dTudu;
	m_Cur = l_posEnd;
	m_iStyle = 3;	//当前线型
}

/*
 *	解析M代码然后解析图元
 */
void COpenGCode::MWhat(CString LineM)
{
	if (m_iStyle == 0)		//说明是G00下面的M代码
		m_aryOnceM.Add(LineM);
	else
		m_aryTwicM.Add(LineM);
	return;
}

/*
 *	画线函数
 */
void COpenGCode::Draw(COLORREF color,Position posPer)
{	//传进来的是线型和线色参数
	//把最后一点加上
	m_point.Add(posPer);
	double l_dZore = 0.0;
	m_aryTudu.Add(l_dZore);
	if(m_point.GetSize()>1)
	{
		//如果多段线里就一个圆弧就要变成圆弧的对象 
		if(m_point.GetSize() == 2&&m_aryTudu.GetAt(0)!=0.0)
		{
			//		圆弧的起点		中心			末点
			Position l_posBeginPos,l_posCenterPos,l_posEndPos;
			//凸度
			double l_dTudu = m_aryTudu.GetAt(0);
			l_posBeginPos = m_point.GetAt(0);
			l_posEndPos = m_point.GetAt(1);
			g_pDoc->polylineToArc(l_posBeginPos,l_posEndPos,l_dTudu,l_posCenterPos);
			//新圆弧
			CCreateRoundArc *l_pTempArc = new CCreateRoundArc(l_posBeginPos,l_posCenterPos,l_posEndPos,RtnArcDir(l_dTudu));
			m_CollectOfG00.Add(l_pTempArc);//把图元加进到中间的链表，等下一个M代码出现时要负上颜色
			m_point.RemoveAll();	//并且删除点
			m_aryTudu.RemoveAll();
		}
		else//否则就是多段线
		{
			CCreatePolyline* l_pTempPolyline = new CCreatePolyline(m_point,m_aryTudu);	
			m_CollectOfG00.Add(l_pTempPolyline);//把图元加进到中间的链表，等下一个M代码出现时要负上颜色
			m_point.RemoveAll();	//并且删除点
			m_aryTudu.RemoveAll();
		}
	}
	//把一对M 之间的图元负上颜色并加到链表里
	for( int i = 0 ; i < m_CollectOfG00.GetSize() ; i++ )
	{
		CCreateBase*	pSelEntity = (CCreateBase*) m_CollectOfG00[i] ;
		pSelEntity->m_ColorOfLine = color;
		pSelEntity->m_strSBM.Copy(m_aryOnceM);
		pSelEntity->m_strCBM.Copy(m_aryTwicM);
		pSelEntity->m_iKnifeStyle = m_iG41orG42;
		g_pDoc->m_CCreateBaseList.AddTail(pSelEntity);
	}
	m_CollectOfG00.RemoveAll();
	m_point.RemoveAll();
	m_aryTudu.RemoveAll();
	m_aryOnceM.RemoveAll();	
	m_aryTwicM.RemoveAll();
}

/*
 *	//两个M字符集之间会产生的颜色
 */
COLORREF COpenGCode::RtnColor()
{
	COLORREF l_cColorOfLine = g_pDoc->m_stcColorToM.m_color5;;
	//一个图形两边的M代码
	CString l_strOneM,l_strTwoM;
	//两个M代码集的大小 
	int l_iSizeOne = m_aryOnceM.GetSize();
	int l_iSizeTwo = m_aryTwicM.GetSize();
	for(int i=0;i<l_iSizeOne;i++)
	{
		l_strOneM = m_aryOnceM.GetAt(i);
		for(int j=0;j<l_iSizeTwo;j++)
		{
			l_strTwoM = m_aryTwicM.GetAt(j);
			if (l_strOneM == g_pDoc->m_stcColorToM.m_SBM1&&l_strTwoM == g_pDoc->m_stcColorToM.m_CBM1)
			{
				l_cColorOfLine = g_pDoc->m_stcColorToM.m_color1;	//刀一
				return l_cColorOfLine;
			}
			else if (l_strOneM == g_pDoc->m_stcColorToM.m_SBM2&&l_strTwoM == g_pDoc->m_stcColorToM.m_CBM2)
			{
				l_cColorOfLine = g_pDoc->m_stcColorToM.m_color2;	//刀二色
				return l_cColorOfLine;
			}
			else if (l_strOneM == g_pDoc->m_stcColorToM.m_SBM3&&l_strTwoM == g_pDoc->m_stcColorToM.m_CBM3)
			{
				l_cColorOfLine = g_pDoc->m_stcColorToM.m_color3;	//刀三色
				return l_cColorOfLine;
			}
			else if (l_strOneM == g_pDoc->m_stcColorToM.m_SBM4&&l_strTwoM == g_pDoc->m_stcColorToM.m_CBM4)
			{
				l_cColorOfLine = g_pDoc->m_stcColorToM.m_color4;	//刀四色
				return l_cColorOfLine;
			}
			else if (l_strOneM == g_pDoc->m_stcColorToM.m_SBM5&&l_strTwoM == g_pDoc->m_stcColorToM.m_CBM5)
			{
				l_cColorOfLine = g_pDoc->m_stcColorToM.m_color5;	//刀五色
				return l_cColorOfLine;
			}
			else 
				l_cColorOfLine = COLOR_GRAY;	//刀五色			
		}
	}
	return l_cColorOfLine;
}

/*
*	把M代码集解析成字符串
*/
CString COpenGCode::AnalysisAryM(CArray<class CString,class CString &>& aryM)
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

/*
 *	给点分配M代码
 */
COLORREF COpenGCode::DistriPointMCode()
{
	COLORREF l_cColorOfLine = g_pDoc->m_stcColorToM.m_color5;;
	//一个图形两边的M代码
	CString l_strOneM,l_strTwoM;
	//两个M代码集的大小 
	CArray<class CString,class CString &> l_aryCopyOnceM;//复制M代码
	l_aryCopyOnceM.Copy(m_aryOnceM);
	m_aryOnceM.RemoveAll();
	int l_iSizeOne = l_aryCopyOnceM.GetSize();
	for(int i=0;i<l_iSizeOne-1;i++)
	{
		l_strOneM = l_aryCopyOnceM.GetAt(i);
		l_strTwoM = l_aryCopyOnceM.GetAt(i+1);
		if (l_strOneM == g_pDoc->m_stcColorToM.m_SBM1&&l_strTwoM == g_pDoc->m_stcColorToM.m_CBM1)
		{
			l_cColorOfLine = g_pDoc->m_stcColorToM.m_color1;	//刀一
			m_aryOnceM.Add(l_strOneM);
			m_aryTwicM.Add(l_strTwoM);
			return l_cColorOfLine;
		}
		else if (l_strOneM == g_pDoc->m_stcColorToM.m_SBM2&&l_strTwoM == g_pDoc->m_stcColorToM.m_CBM2)
		{
			l_cColorOfLine = g_pDoc->m_stcColorToM.m_color2;	//刀二色
			m_aryOnceM.Add(l_strOneM);
			m_aryTwicM.Add(l_strTwoM);
			return l_cColorOfLine;
		}
		else if (l_strOneM == g_pDoc->m_stcColorToM.m_SBM3&&l_strTwoM == g_pDoc->m_stcColorToM.m_CBM3)
		{
			l_cColorOfLine = g_pDoc->m_stcColorToM.m_color3;	//刀三色
			m_aryOnceM.Add(l_strOneM);
			m_aryTwicM.Add(l_strTwoM);
			return l_cColorOfLine;
		}
		else if (l_strOneM == g_pDoc->m_stcColorToM.m_SBM4&&l_strTwoM == g_pDoc->m_stcColorToM.m_CBM4)
		{
			l_cColorOfLine = g_pDoc->m_stcColorToM.m_color4;	//刀四色
			m_aryOnceM.Add(l_strOneM);
			m_aryTwicM.Add(l_strTwoM);
			return l_cColorOfLine;
		}
		else if (l_strOneM == g_pDoc->m_stcColorToM.m_SBM5&&l_strTwoM == g_pDoc->m_stcColorToM.m_CBM5)
		{
			l_cColorOfLine = g_pDoc->m_stcColorToM.m_color5;	//刀五色
			m_aryOnceM.Add(l_strOneM);
			m_aryTwicM.Add(l_strTwoM);
			return l_cColorOfLine;
		}
		else 
			l_cColorOfLine = COLOR_GRAY;	//刀五色			
	}
	return l_cColorOfLine;	
}


