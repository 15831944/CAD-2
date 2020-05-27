// CAD_PowerLandDoc.cpp : implementation of the CCAD_PowerLandDoc class
//

#include "stdafx.h"
#include "CAD_PowerLand.h"
#include "CAD_PowerLandDoc.h"
#include "Draw.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CCAD_PowerLandDoc

IMPLEMENT_DYNCREATE(CCAD_PowerLandDoc, CDocument)

BEGIN_MESSAGE_MAP(CCAD_PowerLandDoc, CDocument)
	//{{AFX_MSG_MAP(CCAD_PowerLandDoc)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCAD_PowerLandDoc construction/destruction

CCAD_PowerLandDoc::CCAD_PowerLandDoc()
{
	// TODO: add one-time construction code here
	g_pDoc = this;
	m_pMeasureCreateBase = NULL;
	m_Undo = NULL;
	m_strPathFile = "";
	m_strNameFile = "";
	m_bFirstOpen = TRUE;//是否是第一次打开
	InitDate();//初始化参数
	g_strCBM = g_pDoc->m_stcColorToM.m_CBM1;
	g_strSBM = g_pDoc->m_stcColorToM.m_SBM1;
	g_CurColor = g_pDoc->m_stcColorToM.m_color1;	
}

CCAD_PowerLandDoc::~CCAD_PowerLandDoc()
{
	//清空所有图元所占用的内存
	POSITION pos = m_CCreateBaseList.GetHeadPosition();
	while (pos != NULL)
	{
		CCreateBase* l_pDelList = (CCreateBase*)m_CCreateBaseList.GetNext(pos);
		delete l_pDelList;
	}
	m_CCreateBaseList.RemoveAll();
	//清空选择集图元
	m_CollectOfPicked.RemoveAll();
	CCreateBase* pSelEntity;
	for( int i = 0 ; i < m_CollectOfPicked.GetSize() ; i++ )
	{
		pSelEntity = (CCreateBase*)m_CollectOfPicked[i] ;
		delete pSelEntity ;
		pSelEntity = NULL ;
	}	
	if (m_Undo)
	{
		delete m_Undo;
		m_Undo = NULL;
	}
	g_pDoc->m_CCreateBaselayer1.RemoveAll();
	//清除测量线留下的内存
	if (m_pMeasureCreateBase != NULL) 
	{
		delete 	m_pMeasureCreateBase;	
		m_pMeasureCreateBase = NULL;	
	}
}

BOOL CCAD_PowerLandDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	m_bFirstOpen = TRUE;//第一次打开文件，保存是要别存为
	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
//	SetTitle("PNC2000CAD--模板编辑软件");           //设置文档标题 
	SetTitle("PNC2000CAD  V1.1");           //设置文档标题 
	m_strPathFile = "";
	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CCAD_PowerLandDoc serialization

void CCAD_PowerLandDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CCAD_PowerLandDoc diagnostics

#ifdef _DEBUG
void CCAD_PowerLandDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CCAD_PowerLandDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCAD_PowerLandDoc commands

void CCAD_PowerLandDoc::Draw(CDC *pDC)
{
	//画测量的尺寸
	DrawMeasure(pDC);
	// 绘制图层1中的图元
	POSITION pos = m_CCreateBaselayer1.GetHeadPosition();
	while (pos != NULL)
	{
		CCreateBase* l_pCreateBase = (CCreateBase*)m_CCreateBaselayer1.GetNext(pos);
		l_pCreateBase->DrawMove(pDC,dmLayer);
	}
	// 绘制链表中的图元
	pos = m_CCreateBaseList.GetHeadPosition();
	while (pos != NULL)
	{
		CCreateBase* l_pCreateBase = (CCreateBase*)m_CCreateBaseList.GetNext(pos);
		l_pCreateBase->DrawMove(pDC,dmNormal,g_pView->m_stcSwitch.bShowArrow,g_pView->m_stcSwitch.bShowNum);
	}
	// 绘制选择集中的图元
	for(int i = 0;  i<m_CollectOfPicked.GetSize();i++)
	{
		CCreateBase* l_pCreateBaseList = (CCreateBase*)m_CollectOfPicked[i];
		l_pCreateBaseList->DrawMove(pDC,dmSelect);
	}
	//框选集里只有一个元素
	if(g_pDoc->m_CollectOfPicked.GetSize() == 1)
	{
		CCreateBase* pPropertCreate = (CCreateBase*)g_pDoc->m_CollectOfPicked.GetAt(0);
		pPropertCreate->DrawMove(pDC,dmSelect,0,0);
		if(pPropertCreate->IsKindOf(RUNTIME_CLASS(CCreatePolyline))||
			pPropertCreate->IsKindOf(RUNTIME_CLASS(CCreateRectangle))||
			pPropertCreate->IsKindOf(RUNTIME_CLASS(CCreateSpline))||
			pPropertCreate->IsKindOf(RUNTIME_CLASS(CCreateEllipse)))//如果是多段线或方或椭圆，就要画差
		{
			//画现在的X
			CPoint SCreemPos;
			g_pView->WorldToScreem(((CMainFrame*)AfxGetMainWnd())->m_PerXPoint,SCreemPos);
			pDC->MoveTo(SCreemPos.x-10,SCreemPos.y-10);
			pDC->LineTo(SCreemPos.x+10,SCreemPos.y+10);
			pDC->MoveTo(SCreemPos.x-10,SCreemPos.y+10);
			pDC->LineTo(SCreemPos.x+10,SCreemPos.y-10);
		}
	}	
}

/*
 *	画测量尺寸
 */
void CCAD_PowerLandDoc::DrawMeasure(CDC* pDC)
{
	if (m_pMeasureCreateBase == NULL)
		return;
	CCreateLine *pLine = (CCreateLine*)m_pMeasureCreateBase;
	//垂直距离，水平距离，直线长度
	double disVer,disHor,disLong;
	disVer = fabs(pLine->m_Begin.y - pLine->m_End.y);
	disHor = fabs(pLine->m_Begin.x - pLine->m_End.x);
	disLong = pLine->m_Begin.Distance(pLine->m_End);
	CString strVer,strHor,strLong;
	strVer.Format("垂直%.3f",disVer);
	strHor.Format("水平%.3f",disHor);
	strLong.Format("直线长%.3f",disLong);	
	pDC->SetTextColor(RGB(64,128,128));
	pDC->SetBkMode(TRANSPARENT);  //这样就可以透明化背景了				
	pDC->TextOut(20,20,strVer);
	pDC->TextOut(20,40,strHor);
	pDC->TextOut(20,60,strLong);
	pLine->DrawMove(pDC,dmLayer);
	pDC->SetTextColor(0);
}

void CCAD_PowerLandDoc::DeleteContents() 
{
	// TODO: Add your specialized code here and/or call the base class
	POSITION pos = m_CCreateBaseList.GetHeadPosition();
	CCreateBase* l_pCreateBase = NULL;
	while (pos != NULL)
	{
		l_pCreateBase = (CCreateBase*)m_CCreateBaseList.GetNext(pos);
		delete l_pCreateBase;
		l_pCreateBase = NULL;
	}
	m_CCreateBaseList.RemoveAll();
	m_CollectOfPicked.RemoveAll();
	g_pDoc->m_CCreateBaselayer1.RemoveAll();	
	//先给图形链表里一个无关的M代码，保证了这个链表的地址不会是空的。
	CCreateKong* pTempMWhat = new CCreateKong();
	m_CCreateBaseList.AddTail(pTempMWhat);

	//清空撤消所占用的内存
	if (m_Undo)
	{
		delete m_Undo;
		m_Undo = NULL;
	}
	m_Undo = new CUndoRedo();
	//清除测量线留下的内存
	if (m_pMeasureCreateBase != NULL) 
	{
		delete 	m_pMeasureCreateBase;	
		m_pMeasureCreateBase = NULL;	
	}	
	CDocument::DeleteContents();
}

void CCAD_PowerLandDoc::OnLButtonDown(UINT nFlags, CPoint point)
{
	CDC*	pDC = g_pView->GetDC() ;// 得到视的设备环境指针
 	BOOL	l_bPicked = FALSE;
	POSITION	position = m_CCreateBaseList.GetHeadPosition();	//获得头指针，在下面循环上用。
	CCreateBase*	l_pCreateBase;
	while(position != NULL)//进入扫描链表
	{
		l_pCreateBase = (CCreateBase *)m_CCreateBaseList.GetNext(position);
		// 如果链表里的图元被选中
		if( l_pCreateBase->IsPick(point) != -1)
		{ 
			l_bPicked = TRUE;
			
			int l_iNumCollect = IsSelected(l_pCreateBase);
			if( l_iNumCollect == -1 )// 判断图元对象是否已经在选择集中,没在选择集中
			{
				l_pCreateBase->DrawMove(pDC,dmSelect);
				m_CollectOfPicked.Add(l_pCreateBase);  // 将图元放入选择集中
				//框选集里只有一个元素
				if(g_pDoc->m_CollectOfPicked.GetSize() == 1)
				{
					l_pCreateBase->PropertyView();
					((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.SetDlgItemText(IDC_STATIC10,"加工顺序:");					
				}	
				else 
				{
					CCreateBase l_show;
					((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.SetDlgItemText(IDC_EDIT_SEQUENCE,"");				
					((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.m_ComboColor.SetCurSel(5);					
					l_show.ShowControl(0,FALSE);
				}
				
				break;// 当某一个图元被拾取到后，即退出循环
			}
			else//在选择集中
			{
				if (nFlags == 5)//按SHIFT，反选从选择集中移除
				{
					m_CollectOfPicked.RemoveAt(l_iNumCollect);
					l_pCreateBase->DrawMove(pDC,dmNormal);
				}

			}
			//框选集里只有一个元素
			if(g_pDoc->m_CollectOfPicked.GetSize() == 1)
			{
				CCreateBase* pPropertCreate = (CCreateBase*)g_pDoc->m_CollectOfPicked.GetAt(0);
				pPropertCreate->PropertyView();
				((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.SetDlgItemText(IDC_STATIC10,"加工顺序:");					
			}	
			else 
			{
				CCreateBase l_show;
				((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.SetDlgItemText(IDC_EDIT_SEQUENCE,"");				
				((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.m_ComboColor.SetCurSel(5);					
				l_show.ShowControl(0,FALSE);
			}
		}
	}	
	if(l_bPicked == FALSE)//没有选中后就要进入框选
	{
		Position pos;
		g_pView->ScreemToWorld(point,pos);
		g_pView->m_pDrawBase = new CDrawSelect();
		g_pView->m_iFlagWhichBtnDown = DRAW_SELECT;		
		SetCursor(AfxGetApp()->LoadCursor(IDC_NONE));
		g_pView->m_pDrawBase->OnLButtonDown(1,pos,"");
	}
	g_pView->ReleaseDC(pDC);// 释放视的设备环境指针
}

void CCAD_PowerLandDoc::OnLButtonDblClk(CPoint point)
{
/*
	CDC*	pDC = g_pView->GetDC() ;// 得到视的设备环境指针
	POSITION	position = m_CCreateBaseList.GetHeadPosition();
	while(position != NULL)
	{
		CCreateBase*	l_pCreateBase = (CCreateBase *)m_CCreateBaseList.GetNext(position);
		// 得到当前状态的拾取半径
		if( l_pCreateBase->IsPick(point) != -1)
		{ 
			RemoveAllCollected();
			// 当某一个图元被拾取到后，即退出循环
			l_pCreateBase->PropertyView();
			if(m_pTwoCreateBase)	//如果双击选中的话
			{
				m_pTwoCreateBase->DrawMove(pDC,dmNormal);
				if (m_pTwoCreateBase->IsKindOf(RUNTIME_CLASS(CCreatePoint)))
				{	
					CCreatePoint* pPoint = (CCreatePoint*)m_pTwoCreateBase;
					CPoint pot;
					g_pView->WorldToScreem(pPoint->m_Begin,pot);
					CRect rect(pot.x-5,pot.y-5,pot.x+5,pot.y+5);
					g_pView->InvalidateRect(rect);
				}
			}

			
			m_pTwoCreateBase = l_pCreateBase;
			//////////////////////////////////////////////////////////////////////////
			if(l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateLine))||
				l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRound))||
				l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRoundArc)))//如果是线，圆，弧就把差（X）弄没
			{
				//画现在的X
				CPoint SCreemPos;
				g_pView->WorldToScreem(((CMainFrame*)AfxGetMainWnd())->m_PerXPoint,SCreemPos);
				CRect rect(SCreemPos.x-15,SCreemPos.y-15,SCreemPos.x+15,SCreemPos.y+15);
				g_pView->InvalidateRect(rect);
			}
			//////////////////////////////////////////////////////////////////////////
			if(m_pTwoCreateBase)
				m_pTwoCreateBase->DrawMove(pDC,dmPrompt);//把当前的画成双击选中的	
			break;
		}
	}		
	g_pView->ReleaseDC(pDC);// 释放视的设备环境指针*/

}

void CCAD_PowerLandDoc::OnMouseMove(UINT nFlags, CPoint point)
{	

}

/*
 *	清空选择集
 */
void CCAD_PowerLandDoc::RemoveAllCollected()
{
	// 首先将选择集中的元素绘制为正常状态，然后清空选择集
	CDC*	pDC = g_pView->GetDC();
	//选择集的大小
	int l_iSize = m_CollectOfPicked.GetSize();
	CCreateBase*	pSelEntity;
	for( int i = 0 ; i < l_iSize ; i++ )
	{
		pSelEntity = (CCreateBase*) m_CollectOfPicked[i] ;
        pSelEntity->DrawMove(pDC,dmNormal);
		if (pSelEntity->IsKindOf(RUNTIME_CLASS(CCreatePoint)))
		{	
			CCreatePoint* pPoint = (CCreatePoint*)pSelEntity;
			CPoint pot;
			g_pView->WorldToScreem(pPoint->m_Begin,pot);
			CRect rect(pot.x-5,pot.y-5,pot.x+5,pot.y+5);
			g_pView->InvalidateRect(rect);
		}
	}
	m_CollectOfPicked.RemoveAll();
	g_pView->ReleaseDC(pDC);
}

int CCAD_PowerLandDoc::IsSelected(CCreateBase* pCreateBase)
{	// 判断图元对象是否已经在选择集中
	if( pCreateBase )
	{
		for( int i = 0 ; i < m_CollectOfPicked.GetSize() ; i++ )
		{
			if( pCreateBase == (CCreateBase*)m_CollectOfPicked[i] )
				return i ;
		}
	}
	return -1;
}

/*
*	保存文件按扭
*/
BOOL CCAD_PowerLandDoc::OnFileSave() 
{
	if(m_bFirstOpen == TRUE)
	{
		if(OnFileSaveAs())
			return TRUE;
		else
			return FALSE;
	}
	if(((CMainFrame*)AfxGetMainWnd())->m_bSaveFile == FALSE)//是否有加密狗，如果没有就不保存
			return FALSE;
	COutputGCode l_cGCode;
	COutputDXFCode l_cDXFCode;
	//保存时是否弹出对话框
	if(m_strPathFile != "")
	{
		if(m_iStyleFile == 1)
			l_cGCode.OutputGCode(m_strPathFile);
		else if(m_iStyleFile ==2)
			l_cDXFCode.OutputDXFCode(m_strPathFile);
		return TRUE;
	}

	// TODO: Add your command handler code here
	char szFilter[] = "G代码文件(*.G)|*.g|DXF文件(*.DXF)|*.dxf||";		//保存格式
	CFileDialog Dlg(FALSE, NULL, NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);//建一个文件对话框
	Dlg.m_ofn.lpstrTitle = "保存";
	if (Dlg.DoModal() == IDOK)	//如果点击确定
	{
		CString l_strPathOfFile;//文件路径
		if(Dlg.m_ofn.nFilterIndex == 1)	//保存类型为第一个.G格式的
		{
			l_strPathOfFile = Dlg.GetPathName();//先获取要保存的文件路径
			if (Dlg.GetFileExt() != "G"&&Dlg.GetFileExt() != "g"&&Dlg.GetFileExt() != "DXF"&&Dlg.GetFileExt() != "dxf")		//文件没有扩展名时给加上.g的格式
				l_strPathOfFile += ".g";
			if (!l_cGCode.OutputGCode(l_strPathOfFile)) 
			{
				//中文还是英文
				if (g_pDoc->m_stcWorkFor.IsChinese)
					AfxMessageBox("有问题");			
				else
					AfxMessageBox("something the matter");	
			}
			else
			{
				CString l_strFileName = "PNC2000CAD  V1.1                    "+m_strNameFile+".g"+"              ";			
				SetTitle(l_strFileName);				
				m_strPathFile = l_strPathOfFile;//把路径放到成员到时保存时用
				m_iStyleFile = 1;	
			}
		}
		else if(Dlg.m_ofn.nFilterIndex == 2)	//保存类型为第二个.DXF格式的
		{
			l_strPathOfFile = Dlg.GetPathName();//先获取要保存的文件路径
			if (Dlg.GetFileExt() != "G"&&Dlg.GetFileExt() != "g"&&Dlg.GetFileExt() != "DXF"&&Dlg.GetFileExt() != "dxf")		//文件没有扩展名时给加上.g的格式
				l_strPathOfFile += ".dxf";
			if (!l_cDXFCode.OutputDXFCode(l_strPathOfFile)) 
			{
				//中文还是英文
				if (g_pDoc->m_stcWorkFor.IsChinese)
					AfxMessageBox("有问题");			
				else
					AfxMessageBox("something the matter");	
			}
			else
			{
				CString l_strFileName = "PNC2000CAD  V1.1                    "+m_strNameFile+".dxf"+"              ";			
				SetTitle(l_strFileName);
				m_strPathFile = l_strPathOfFile;//把路径放到成员到时保存时用
				m_iStyleFile = 2;	
			}			
		}
		m_bFirstOpen = FALSE;//是否是第一次打开	
	}
	else
		return FALSE;
	return TRUE;	
}

/*
*	打开文件按扭
*/
void CCAD_PowerLandDoc::OnFileOpen() 
{
	// TODO: Add your command handler code here
	char szFilter[] = "All Files (*.*)|*.*|G代码文件(*.g)|*.g|DXF文件(*.DXF)|*.dxf|PLT文件 (*.PLT)|*.PLT||";
	CFileDialog Dlg(TRUE, NULL, NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, szFilter);
	if (Dlg.DoModal() == IDOK)	//如果点击确定
	{
		//////////////////////////////////////////////////////////////////////////
		//以下是删除之前的内容，然后打开新内容
		POSITION pos = m_CCreateBaseList.GetHeadPosition();
		while (pos != NULL)
		{
			CCreateBase* l_pDelList = (CCreateBase*)m_CCreateBaseList.GetNext(pos);
			delete l_pDelList;
			l_pDelList = NULL;
		}
		g_pDoc->m_CCreateBaselayer1.RemoveAll();
		m_CCreateBaseList.RemoveAll();	
		//删除选择集里的指针 
		m_CollectOfPicked.RemoveAll() ;
		//清撤消链表
		if (m_Undo)
		{
			delete m_Undo;
			m_Undo = NULL;
		}
		m_Undo = new CUndoRedo();
		//先给图形链表里一个无关的M代码，保证了这个链表的地址不会是空的。
		CCreateKong* pTempMWhat = new CCreateKong();
		m_CCreateBaseList.AddTail(pTempMWhat);
		//////////////////////////////////////////////////////////////////////////
		CString l_strPathOfFile = Dlg.GetPathName();//先获取要打开的文件路径
		CString l_strFileExt = Dlg.GetFileExt();
		m_strNameFile = Dlg.GetFileTitle();
		if (l_strFileExt == "g"||l_strFileExt == "G")//如果是G代码
		{
			COpenGCode l_cGCode;
			//开始读G文件解释内容
			if (!l_cGCode.ReadGCode(l_strPathOfFile)) 
			{
				//中文还是英文
				if (g_pDoc->m_stcWorkFor.IsChinese)
					AfxMessageBox("打开G代码有问题");
				else
					AfxMessageBox("Open the G code has a problem");
			}
			else
			{
//				CString l_strFileName = "PNC2000CAD--模板编辑软件                    "+Dlg.GetFileName()+"              ";			
				CString l_strFileName = "PNC2000CAD  V1.1                    "+Dlg.GetFileName()+"              ";			
				SetTitle(l_strFileName);
				m_strPathFile = l_strPathOfFile;//把路径放到成员到时保存时用
				m_iStyleFile = 1;
			}	
		}
		else if (l_strFileExt == "dxf"||l_strFileExt == "DXF")	//如果是DXF文件
		{
			COpenDXFCode l_cDXFCode;
			if (!l_cDXFCode.ReadDXFCode(l_strPathOfFile))
			{
				//中文还是英文
				if (g_pDoc->m_stcWorkFor.IsChinese)
					AfxMessageBox("打开DXF代码有问题");
				else
					AfxMessageBox("Open the DXF code has a problem");
			}
			else
			{
//				CString l_strFileName = "PNC2000CAD--模板编辑软件                    "+Dlg.GetFileName()+"              ";			
				CString l_strFileName = "PNC2000CAD  V1.1                    "+Dlg.GetFileName()+"              ";			
				SetTitle(l_strFileName);
				m_strPathFile = l_strPathOfFile;//把路径放到成员到时保存时用
				m_iStyleFile = 2;
			}
		}
		else if (l_strFileExt == "plt"||l_strFileExt == "PLT")	//如果是plt文件,就打开
		{
			COpenPLTCode l_cPLTCode;	//打开后，转变成G代码
			if (!l_cPLTCode.AnalysisPlt(l_strPathOfFile))
			{
				//中文还是英文
				if (g_pDoc->m_stcWorkFor.IsChinese)
					AfxMessageBox("打开PLT代码有问题");
				else
					AfxMessageBox("Open the PLT code has a problem");
			}
			else
			{
//				CString l_strFileName = "PNC2000CAD--模板编辑软件                    "+Dlg.GetFileName()+"              ";			
				CString l_strFileName = "PNC2000CAD  V1.1                    "+Dlg.GetFileName()+"              ";			
				SetTitle(l_strFileName);//设定文件路径
				m_strPathFile = l_strPathOfFile;//把路径放到成员到时保存时用
				m_iStyleFile = 2;
			}
			//打开转变后的G代码
			COpenGCode l_cOpenTurned;
			//开始读G文件，并画出来		   //G代码文件的路径
			if (!l_cOpenTurned.ReadGCode(l_cPLTCode.m_strOutFile)) 
			{
				//中文还是英文
				if (g_pDoc->m_stcWorkFor.IsChinese)
					AfxMessageBox("打开G代码有问题");
				else
					AfxMessageBox("Open the G code has a problem");
			}
		}
		else
		{
			//中文还是英文
			if (g_pDoc->m_stcWorkFor.IsChinese)
				AfxMessageBox("请打开.DXF或.G或.PLT的文件");			
			else
				AfxMessageBox("Please open the DXF or. G or. PLT file");			
		}
		m_bFirstOpen = TRUE;//是否是第一次打开
	}
}

/*
 *	打开颜色对应着的M代码的定义对话框的按扭
 */
void CCAD_PowerLandDoc::OnProperty() 
{
	// TODO: Add your command handler code here
	CDlgColorToM dlg;
	dlg.DoModal();
}

/*
 *	打开参数1的对话框
 */
void CCAD_PowerLandDoc::OnOpenDlgParamGlassGrind()
{
	// TODO: Add your command handler code here
	CDlgParamGlassGrind dlg;
	dlg.DoModal();
}

/*
 *	打开矩阵对话框
 */
void CCAD_PowerLandDoc::OnOpenDlgMatrix() 
{
	// TODO: Add your command handler code here
	CDlgMatrix dlg;
	dlg.DoModal();
}

/*
 *	打开进退刀对话框
 */
void CCAD_PowerLandDoc::OnOpenDlgParamInoutKnife() 
{
	// TODO: Add your command handler code here
	CDlgParamInoutKnife dlg;
	dlg.DoModal();
}

/*
 *	打开过切对话框
 */
void CCAD_PowerLandDoc::OnOpenDlgParamOverCut() 
{
	// TODO: Add your command handler code here
	CDlgParamClothCut dlg;
	dlg.DoModal();
}

/*
 *	打开工艺参数对话框
 */
void CCAD_PowerLandDoc::OnOpenDlgWorkFor()
{
	CDlgWorkFor dlg;
	dlg.DoModal();
}

//磨边
void CCAD_PowerLandDoc::OnGraphForGrind()
{
	CGlassGrind GlassGrind;
	GlassGrind.GetFinishList();
}


//磨边
void CCAD_PowerLandDoc::OnGraphKinfe()
{
	CGraphIOKinfe knife;
	knife.PutGraphIOKife();
}

//铣边
void CCAD_PowerLandDoc::OnMillSide()
{
//	TransToGraphList turn;
	CDlgMillSide dlg;
	dlg.DoModal();
//	if (IDOK == dlg.DoModal()) 
//		turn.OutMillSide();//刀补库出来的图形
}

//缩放
void CCAD_PowerLandDoc::OnGraphScale()
{
	CGraphScale Scale;
	Scale.ZoomInOut(m_stcClothCut.m_dScaleX,m_stcClothCut.m_dScaleY);
}

/*
 *	删除重刀
 */
void CCAD_PowerLandDoc::OnDeleteMoreLine()
{
//	CDeleteMoreLine obj;
//	obj.DeleteMoreLine();
}

/*
 *	平行线
 */
void CCAD_PowerLandDoc::OnParallel()
{
	int l_iSize = m_CollectOfPicked.GetSize();
	if (l_iSize != 0)
	{
		CDlgParallel dlg;
		dlg.m_iCase = PARALLEL;
		dlg.DoModal();
	}
	else
	{	
		//中文还是英文
		if (g_pDoc->m_stcWorkFor.IsChinese)
			RemindContent("\r\n没有选中，重新选择",2);
		else
			RemindContent("\r\nNot selected, choose again：");			
	}
}
/*
 *	以图形的中心点旋转
 */
void CCAD_PowerLandDoc::OnRotateCenter(double angle)
{
	CDC* pDC = g_pView->GetDC();
	Position l_posCenter;	//选择集的包围盒的两个点
	int l_iSize = m_CollectOfPicked.GetSize();
	BOX2D pBox;
	//撤消类的对象 
	CUndoRedo l_UndoRedo;
	PGRAPHPOS l_pGraphPos,l_pCurGraphPos;
	PUNDOLIST pUndoList	;
	if (l_iSize)
	{
		/////////////////获得选择集的包围盒//////////////////////////////
		GetCollectBox(&pBox);
		l_posCenter.x = (pBox.min[0]+pBox.max[0])/2;
		l_posCenter.y = (pBox.min[1]+pBox.max[1])/2;
		pUndoList = l_UndoRedo.NewUndoList();
		pUndoList->OperatieWhat = ID_ROTATE_180;
		pUndoList->pGraphPos = l_pCurGraphPos = l_pGraphPos = l_UndoRedo.NewGraphPos();
	}
	else
	{		
		//中文还是英文
		if (g_pDoc->m_stcWorkFor.IsChinese)
			RemindContent("\r\n没有选中，重新选择",2);
		else
			RemindContent("\r\nNot selected, choose again：");	
		return;
	}
	for(int i = 0; i < l_iSize; i++)
	{
		CCreateBase*	pEntity = (CCreateBase*)m_CollectOfPicked[i];
		CCreateBase* pCopy = pEntity->Copy();
		pCopy->Rotate(l_posCenter, angle);
		POSITION POS = m_CCreateBaseList.Find(pEntity);
		m_CCreateBaseList.InsertAfter(POS,pCopy);
		if (i != 0)
			l_pGraphPos = l_UndoRedo.NewGraphPos();		
		//如果不保留原图
		if (!m_stcWorkFor.RotateSymmetrySaveOld)
		{
			m_CCreateBaseList.RemoveAt(POS);
			l_pGraphPos->pCreateBaseposSpare2 = 1;
		}
		//向撤消链表里添加
		l_pGraphPos->pCreateBasepos = (long)pCopy;
		l_pGraphPos->pCreateBaseposSpare1 = (long)pEntity;
		if (i != 0)
		{
			l_pCurGraphPos->pNext = l_pGraphPos;
			l_pGraphPos->pPrev = l_pCurGraphPos;
			l_pCurGraphPos = l_pGraphPos;						
		}
		pCopy->DrawMove(pDC,dmNormal,g_pView->m_stcSwitch.bShowArrow,g_pView->m_stcSwitch.bShowNum);
	}
	//向撤消链表里添加
	m_Undo->AddUndoList(pUndoList);	
	//刷新
	m_CollectOfPicked.RemoveAll();
	CRect rect;
	g_pView->GetClientRect(rect);
	g_pView->InvalidateRect(rect);
	g_pView->ReleaseDC(pDC);
}

/*
 *	以图形的上下，左右，XY,对称
 */
void CCAD_PowerLandDoc::OnSymmetry(int Case)
{
	CDC* pDC = g_pView->GetDC();
	Position l_pos1,l_pos2;	//选择集的包围盒的两个点
	int l_iSize = m_CollectOfPicked.GetSize();
	BOX2D pBox;
	//撤消类的对象 
	CUndoRedo l_UndoRedo;
	PGRAPHPOS l_pGraphPos,l_pCurGraphPos;
	PUNDOLIST pUndoList	;
	if (l_iSize)
	{
		/////////////////获得选择集的包围盒//////////////////////////////
		GetCollectBox(&pBox);
		pUndoList = l_UndoRedo.NewUndoList();
		pUndoList->OperatieWhat = ID_ROTATE_180;
		pUndoList->pGraphPos = l_pCurGraphPos = l_pGraphPos = l_UndoRedo.NewGraphPos();
	}
	else
	{
		//中文还是英文
		if (g_pDoc->m_stcWorkFor.IsChinese)
			RemindContent("\r\n没有选中，重新选择",2);
		else
			RemindContent("\r\nNot selected, choose again：");			return;
	}
	//以什么方式镜像
	switch(Case)
	{
		case ID_SYMMETRY_XAXIS:
			{
				l_pos1.x = 0;
				l_pos1.y = 0;
				l_pos2.x = 100;
				l_pos2.y = 0;
			}
			break;
		case ID_SYMMETRY_YAXIS:
			{
				l_pos1.x = 0;
				l_pos1.y = 0;
				l_pos2.x = 0;
				l_pos2.y = 100;
			}
			break;
		case ID_SYMMETRY_LANDR:
			{
				l_pos1.x = (pBox.min[0]+pBox.max[0])/2;
				l_pos1.y = pBox.min[1];
				l_pos2.x = (pBox.min[0]+pBox.max[0])/2;
				l_pos2.y = pBox.max[1];
			}
			break;
		case ID_SYMMETRY_TANDB:
			{
				l_pos1.x = pBox.min[0];
				l_pos1.y = (pBox.min[1]+pBox.max[1])/2;
				l_pos2.x = pBox.max[0];
				l_pos2.y = (pBox.min[1]+pBox.max[1])/2;
			}
		break;		
		default:
			break;
	}
	for(int i = 0; i < l_iSize; i++)
	{
		CCreateBase*	pEntity = (CCreateBase*)m_CollectOfPicked[i];
		CCreateBase* pCopy = pEntity->Copy();
		pCopy->Mirror(l_pos1,l_pos2);
		POSITION POS = m_CCreateBaseList.Find(pEntity);
		m_CCreateBaseList.InsertAfter(POS,pCopy);
		if (i != 0)
			l_pGraphPos = l_UndoRedo.NewGraphPos();		
		//如果不保留原图
		if (!m_stcWorkFor.RotateSymmetrySaveOld)
		{
			m_CCreateBaseList.RemoveAt(POS);
			l_pGraphPos->pCreateBaseposSpare2 = 1;
		}
		//向撤消链表里添加
		l_pGraphPos->pCreateBasepos = (long)pCopy;
		l_pGraphPos->pCreateBaseposSpare1 = (long)pEntity;
		if (i != 0)
		{
			l_pCurGraphPos->pNext = l_pGraphPos;
			l_pGraphPos->pPrev = l_pCurGraphPos;
			l_pCurGraphPos = l_pGraphPos;						
		}
		pCopy->DrawMove(pDC,dmNormal,g_pView->m_stcSwitch.bShowArrow,g_pView->m_stcSwitch.bShowNum);
	}
	//向撤消链表里添加
	m_Undo->AddUndoList(pUndoList);	
	//刷新
	m_CollectOfPicked.RemoveAll();
	CRect rect;
	g_pView->GetClientRect(rect);
	g_pView->InvalidateRect(rect);
	g_pView->ReleaseDC(pDC);
}

/*
 *	把多点的多段线变成少点的
 */
void CCAD_PowerLandDoc::OnToLessPoint()
{
	CToLessPoint ToLess;
	ToLess.ToLessPoint();
}

//过切
void CCAD_PowerLandDoc::OverCut()
{
	COverCut overcut;
	overcut.NeedOverCut();
}

/*
 *	加边框的类
 */
void CCAD_PowerLandDoc::OnAddRect()
{
	CDlgAddRect DlgAddRect;
	DlgAddRect.DoModal();
}

//把线变成铣刀的线
void CCAD_PowerLandDoc::OnTurnMill()
{
	CTurnMill TurnMill;
	TurnMill.TurnGraphMill();
}

//合并
void CCAD_PowerLandDoc::OnGraphTogether()
{
	CGraphTogether GraphTogether;
	GraphTogether.PutGraphTogether();


	//保存当前的图形信息
	COutputGCode SaveGCode;
	SaveGCode.OutputGCode("C:\\Program Files\\CAD_PowerLand\\Together.g");
	//清空所有图元
	m_CCreateBaseList.RemoveAll();
	//清空选择集图元
	m_CollectOfPicked.RemoveAll();
	CCreateKong* pTempMWhat = new CCreateKong();
	m_CCreateBaseList.AddTail(pTempMWhat);
	//再打开
	COpenGCode OpenGCode;
	OpenGCode.ReadGCode("C:\\Program Files\\CAD_PowerLand\\Together.g");
}

/*
 *	发送文件
 */
void CCAD_PowerLandDoc::OnFileSend()
{
	if(((CMainFrame*)AfxGetMainWnd())->m_bSaveFile == FALSE)//是否加密，可以保存
	{
		//中文还是英文
		if (g_pDoc->m_stcWorkFor.IsChinese)
			RemindContent("\r\n没有使用权限",2);
		else
			RemindContent("\r\nNo access to ",2);		
		return ;
	}
	if(m_strPathFile == ""&&g_pDoc->m_stcWorkFor.bSaveDefault == 1)//是否为空文件
	{
		//中文还是英文
		if (g_pDoc->m_stcWorkFor.IsChinese)
			RemindContent("\r\n发送内容为空",2);
		else
			RemindContent("\r\nsend content is empty ",2);		
		return ;
	}
	CString l_strFileName;//文件的名字
	if(g_pDoc->m_stcWorkFor.bSaveDefault == 1)//说明是保存默认的名称
	{
		if(g_pDoc->m_stcWorkFor.bSendStyleG == 1)//保存g文件
		{
			l_strFileName = "\\" + m_strNameFile + ".g";		
			l_strFileName = m_stcWorkFor.strFilePathG + l_strFileName;
			COutputGCode l_cGCode;
			if (!l_cGCode.OutputGCode(l_strFileName)) 
			{
				//中文还是英文
				if (g_pDoc->m_stcWorkFor.IsChinese)
					AfxMessageBox("保存路径有问题");
				else
					AfxMessageBox("Save the path has a problem");
			}
			else
			{
				//给另一个进程传数据
				HWND receiveWindow = ::FindWindow(NULL,"PNC2000CT");  
				if (receiveWindow == NULL)  
				{
					//中文还是英文
					if (g_pDoc->m_stcWorkFor.IsChinese)
						RemindContent("\r\n没找到切割软件",2);
					else
						RemindContent("\r\nDidn't find the cutting software",2);
					return;
				} 	
				COPYDATASTRUCT copyData = { 0 };
				copyData.lpData = l_strFileName.GetBuffer(0);
				copyData.cbData = l_strFileName.GetLength();	
				::SendMessage(receiveWindow, WM_COPYDATA, (WPARAM)this, (LPARAM)&copyData);	
				l_strFileName.ReleaseBuffer();
				//中文还是英文
				if (g_pDoc->m_stcWorkFor.IsChinese)
					RemindContent("\r\n发送成功",2);
				else
					RemindContent("\r\nSend successful ",2);
				(CMainFrame*)AfxGetMainWnd()->ShowWindow(SW_MINIMIZE);
			}
			
		}
		else//保存dxf文件
		{
			l_strFileName = "\\" + m_strNameFile + ".dxf";		
			l_strFileName = m_stcWorkFor.strFilePathDxf + l_strFileName;
			COutputDXFCode l_cDXFCode;
			if (!l_cDXFCode.OutputDXFCode(l_strFileName)) 
			{
				//中文还是英文
				if (g_pDoc->m_stcWorkFor.IsChinese)
					AfxMessageBox("保存路径有问题");
				else
					AfxMessageBox("Save the path has a problem");
			}
			else
			{
				//给另一个进程传数据
				HWND receiveWindow = ::FindWindow(NULL,"PNC2000CT");  
				if (receiveWindow == NULL)  
				{
					//中文还是英文
					if (g_pDoc->m_stcWorkFor.IsChinese)
						RemindContent("\r\n没找到切割软件",2);
					else
						RemindContent("\r\nDidn't find the cutting software",2);
					return;
				} 	
				COPYDATASTRUCT copyData = { 0 };
				copyData.lpData = l_strFileName.GetBuffer(0);
				copyData.cbData = l_strFileName.GetLength();	
				::SendMessage(receiveWindow, WM_COPYDATA, (WPARAM)this, (LPARAM)&copyData);	
				l_strFileName.ReleaseBuffer();
				//中文还是英文
				if (g_pDoc->m_stcWorkFor.IsChinese)
					RemindContent("\r\n发送成功",2);
				else
					RemindContent("\r\nSend successful ",2);
				(CMainFrame*)AfxGetMainWnd()->ShowWindow(SW_MINIMIZE);
			}
			
		}	
		return;
	}
	CDlgSendFileName dlg;
	dlg.DoModal();
	if(!m_stcWorkFor.IsSendFile)
	{
		//中文还是英文
		if (g_pDoc->m_stcWorkFor.IsChinese)
			RemindContent("\r\n取消发送!",2);
		else
			RemindContent("\r\nUndo Send!",2);
		return;
	}
	if(g_pDoc->m_stcWorkFor.bSendStyleG == 1)//保存g文件
	{
		l_strFileName = "\\" + m_stcWorkFor.FileName + ".g";
		COutputGCode l_cGCode;
		l_strFileName = m_stcWorkFor.strFilePathG + l_strFileName;
		if (!l_cGCode.OutputGCode(l_strFileName)) 
		{
			//中文还是英文
			if (g_pDoc->m_stcWorkFor.IsChinese)
				AfxMessageBox("保存路径有问题");
			else
				AfxMessageBox("Save the path has a problem");
		}
		else
		{
			//给另一个进程传数据
			HWND receiveWindow = ::FindWindow(NULL,"PNC2000CT");  
			if (receiveWindow == NULL)  
			{
				//中文还是英文
				if (g_pDoc->m_stcWorkFor.IsChinese)
					RemindContent("\r\n没找到切割软件",2);
				else
					RemindContent("\r\nDidn't find the cutting software",2);
				return;
			} 	
			COPYDATASTRUCT copyData = { 0 };
			copyData.lpData = l_strFileName.GetBuffer(0);
			copyData.cbData = l_strFileName.GetLength();	
			::SendMessage(receiveWindow, WM_COPYDATA, (WPARAM)this, (LPARAM)&copyData);	
			l_strFileName.ReleaseBuffer();
			//中文还是英文
			if (g_pDoc->m_stcWorkFor.IsChinese)
				RemindContent("\r\n发送成功",2);
			else
				RemindContent("\r\nSend successful ",2);
			(CMainFrame*)AfxGetMainWnd()->ShowWindow(SW_MINIMIZE);
		}		
	}
	else
	{
		l_strFileName = "\\" + m_stcWorkFor.FileName + ".dxf";
		l_strFileName = m_stcWorkFor.strFilePathDxf + l_strFileName;
		COutputDXFCode l_cDXFCode;
		if (!l_cDXFCode.OutputDXFCode(l_strFileName)) 
		{
			//中文还是英文
			if (g_pDoc->m_stcWorkFor.IsChinese)
				AfxMessageBox("保存路径有问题");
			else
				AfxMessageBox("Save the path has a problem");
		}
		else
		{
			//给另一个进程传数据
			HWND receiveWindow = ::FindWindow(NULL,"PNC2000CT");  
			if (receiveWindow == NULL)  
			{
				//中文还是英文
				if (g_pDoc->m_stcWorkFor.IsChinese)
					RemindContent("\r\n没找到切割软件",2);
				else
					RemindContent("\r\nDidn't find the cutting software",2);
				return;
			} 	
			COPYDATASTRUCT copyData = { 0 };
			copyData.lpData = l_strFileName.GetBuffer(0);
			copyData.cbData = l_strFileName.GetLength();	
			::SendMessage(receiveWindow, WM_COPYDATA, (WPARAM)this, (LPARAM)&copyData);	
			l_strFileName.ReleaseBuffer();
			//中文还是英文
			if (g_pDoc->m_stcWorkFor.IsChinese)
				RemindContent("\r\n发送成功",2);
			else
				RemindContent("\r\nSend successful ",2);
			(CMainFrame*)AfxGetMainWnd()->ShowWindow(SW_MINIMIZE);
		}				
	}
}

/*
 *	输入线色号返回颜色。
 */
COLORREF CCAD_PowerLandDoc::RtnColorRGB(int num)
{
	COLORREF color;
	switch(num)
	{
	case 0:
		color = m_stcColorToM.m_color1;
		break;
	case 1:
		color = m_stcColorToM.m_color2;
		break;
	case 2:
		color = m_stcColorToM.m_color3;
		break;
	case 3:
		color = m_stcColorToM.m_color4;
		break;
	case 4:
		color = m_stcColorToM.m_color5;
		break;
	default:
		color = m_stcColorToM.m_color5;
		break;
	}
	return color;
}


/*
 *	输入线色号返回M代码。
 */
BOOL CCAD_PowerLandDoc::RtnColorMCode(CString& strMsb,CString& strMcb,int num /* = -1 */,COLORREF rgbColor/* = -1 */)
{
	switch(num)
	{
	case -1:
		strMsb = "";
		strMcb = "";
		break;
	case 0:
		strMsb = g_pDoc->m_stcColorToM.m_SBM1;
		strMcb = g_pDoc->m_stcColorToM.m_CBM1;
		break;
	case 1:
		strMsb = g_pDoc->m_stcColorToM.m_SBM2;
		strMcb = g_pDoc->m_stcColorToM.m_CBM2;
		break;
	case 2:
		strMsb = g_pDoc->m_stcColorToM.m_SBM3;
		strMcb = g_pDoc->m_stcColorToM.m_CBM3;
		break;
	case 3:
		strMsb = g_pDoc->m_stcColorToM.m_SBM4;
		strMcb = g_pDoc->m_stcColorToM.m_CBM4;
		break;
	case 4:
		strMsb = g_pDoc->m_stcColorToM.m_SBM5;
		strMcb = g_pDoc->m_stcColorToM.m_CBM5;
		break;
	default:
		strMsb = g_pDoc->m_stcColorToM.m_SBM5;
		strMcb = g_pDoc->m_stcColorToM.m_CBM5;
		break;
	}
	if(num != -1)
		return TRUE;
	if (rgbColor == g_pDoc->m_stcColorToM.m_color1 )
	{
		strMsb = g_pDoc->m_stcColorToM.m_SBM1;
		strMcb = g_pDoc->m_stcColorToM.m_CBM1;		
	}	
	else if (rgbColor == g_pDoc->m_stcColorToM.m_color2)
	{
		strMsb = g_pDoc->m_stcColorToM.m_SBM2;
		strMcb = g_pDoc->m_stcColorToM.m_CBM2;				
	}
	else if (rgbColor == g_pDoc->m_stcColorToM.m_color3)
	{
		strMsb = g_pDoc->m_stcColorToM.m_SBM3;
		strMcb = g_pDoc->m_stcColorToM.m_CBM3;				
	}
	else if (rgbColor == g_pDoc->m_stcColorToM.m_color4)
	{
		strMsb = g_pDoc->m_stcColorToM.m_SBM4;
		strMcb = g_pDoc->m_stcColorToM.m_CBM4;				
	}
	else if (rgbColor == g_pDoc->m_stcColorToM.m_color5)
	{
		strMsb = g_pDoc->m_stcColorToM.m_SBM5;
		strMcb = g_pDoc->m_stcColorToM.m_CBM5;				
	}
	else
		return FALSE;
	return TRUE;
	
}


/*
 *	获得包围盒
 */
BOOL CCAD_PowerLandDoc::GetCollectBox(BOX2D *pBox)
{
	int i, n = m_CollectOfPicked.GetSize();
	if(n == 0)  return FALSE;//如果是空集，就要返回
	
	BOX2D pBox1,pBox2;
	Position l_posMin,l_posMax;	//选择集的包围盒的两个点
	/////////////////获得选择集的包围盒//////////////////////////////
	CCreateBase*	pEntity1 = (CCreateBase*)m_CollectOfPicked[0];
	pEntity1->GetBox(&pBox1);	//1，获得第一个图元包围盒
	l_posMin.x = pBox1.min[0];
	l_posMin.y = pBox1.min[1];
	l_posMax.x = pBox1.max[0];
	l_posMax.y = pBox1.max[1];
	//2，与所有包围盒比较得反最终包围盒
	for(i=0;i<n-1;i++)
	{
		CCreateBase*	pEntity = (CCreateBase*)m_CollectOfPicked[i+1];
		pEntity->GetBox(&pBox2);
		l_posMin.x = min(l_posMin.x,pBox2.min[0]);
		l_posMin.y = min(l_posMin.y,pBox2.min[1]);
		l_posMax.x = max(l_posMax.x,pBox2.max[0]);
		l_posMax.y = max(l_posMax.y,pBox2.max[1]);		
	}
	//输出包围盒
	pBox->min[0] = l_posMin.x;
	pBox->min[1] = l_posMin.y;
	pBox->max[0] = l_posMax.x;
	pBox->max[1] = l_posMax.y;
	return TRUE;
}

/*
*	求凸度的函数 1
*/																//为2时顺时针，其它为逆时针
double CCAD_PowerLandDoc::RtnTudu(Position begin,Position end,Position center,int clock)
{	
	//求起始角  	//求末角  //差角	
	double l_dStartAngle,l_dEndAngle,l_dAlfa;
	l_dStartAngle = GetAngleToXAxis(center,begin);
	l_dEndAngle = GetAngleToXAxis(center,end);
	if(clock == 2)		//顺时针
	{
		if(l_dEndAngle<l_dStartAngle)//没过0度角时
			l_dAlfa = l_dEndAngle - l_dStartAngle;
		else//过零度角时
			l_dAlfa = l_dEndAngle - MATH_2PI -l_dStartAngle;
	}
	else//逆时针
	{
		if(l_dEndAngle>l_dStartAngle)//没过0度角时
			l_dAlfa = l_dEndAngle - l_dStartAngle;
		else//过零度角时
			l_dAlfa = MATH_2PI -l_dStartAngle + l_dEndAngle;
	}
	double dBulge = 0.0;	
	dBulge = tan(l_dAlfa / 4.0);	
	return dBulge;
}

//根据圆弧信息求圆心（这个函数是从Dxf2GCode动态库里抄过来的）                  凸度
void CCAD_PowerLandDoc::polylineToArc(Position& BeginPoint, Position& EndPoint, double u, Position& CenterPoint)
{
		double centerAngle;//包角
	double ttu = fabs(u);
	centerAngle=atan(ttu);
	centerAngle *=4;
	
	double x1,x2,y1,y2;//圆弧起始点和终止点
	x1=BeginPoint.x;
	x2=EndPoint.x;
	y1=BeginPoint.y;
	y2=EndPoint.y;
	
	double L; //弦长
	L=sqrt(pow((x1-x2),2)+pow((y1-y2),2)); 
	
	double R;//圆弧半径
	R=(0.5*sqrt(pow((x1-x2),2)+pow((y1-y2),2)))/(sin(0.5*centerAngle));
	
	//已知圆上两点和半径，求圆心坐标
	double h;//圆心到弦的距离
	h=sqrt(R*R-(pow((x1-x2),2)+pow((y1-y2),2))/4);
	
//	double k;//起始点和终止点连线的中垂线斜率
	double xc,yc;//圆心坐标
	double xa,ya; //起始点和终止点连线的中点横纵坐标
	xa=0.5*(x1+x2);
	ya=0.5*(y1+y2);
	
	//弦的方向角（0-2PI之）
	
	double angle;//起点到终点的弦向量与x正方向之间的倾斜角
	angle=acos((x2-x1)/sqrt(pow(x2-x1,2)+pow(y2-y1,2)));
	
	double amass; //弦向量与X轴正向单位向量的叉积
	amass = y1-y2;//由（由(x2-x1)*0-1*(y2-y1)）得到
	const double eps=0.000000000001; 	
	if (amass>eps)
	{  
		angle = -angle;
		angle=MATH_2PI+angle;
	}

	double DirectionAngel;//弦中点到圆心的直线向量的方向角
	if ((u>0 && centerAngle<MATH_PI)||(u<0 && centerAngle>MATH_PI))
		DirectionAngel=angle+MATH_PI/2;
	if((u<0 && centerAngle<MATH_PI)||(u>0 && centerAngle>MATH_PI))
		DirectionAngel=angle-MATH_PI/2;
//确认角度在（0-2PI之间）2014-10-9
	if (DirectionAngel<0)
        DirectionAngel= DirectionAngel+2*MATH_PI;
	if (DirectionAngel>MATH_2PI)
		DirectionAngel= DirectionAngel-MATH_2PI;
	
	double d;//圆心到弦的距离
	d=sqrt(R*R-L*L/4);
	if (fabs(DirectionAngel-0)<eps)
	{
		xc=xa+d;
		yc=ya;
    }
	else if(fabs(DirectionAngel-MATH_PI/2)<eps)
	{
		xc=xa;
		yc=ya+d;
	}
	else if (fabs(DirectionAngel-MATH_PI)<eps)
	{
		xc=xa-d;
		yc=ya;
	}
	else if (fabs(DirectionAngel-(MATH_PI+MATH_PI/2))<eps)
	{
		xc=xa;
		yc=ya-d;
	}
	else
	{
		double nslope,k;//nslope 为弦的斜率，K为弦中垂线的斜率
		double nAngle;//中垂线的倾斜角；
		double X,Y; //圆心相对于弦中心点的坐标偏移量
		
		nslope = (y2 - y1) / (x2-x1);
		k = -1 / nslope;
		nAngle = atan(k) ;
		X = cos(nAngle) * d;
		Y = sin(nAngle) * d;
		
		if (DirectionAngel > MATH_PI / 2 && DirectionAngel < MATH_PI )
		{
			X = -X;
            Y = -Y;
		}
		if (DirectionAngel > MATH_PI && DirectionAngel < (MATH_PI + MATH_PI / 2) )
		{
			X = -X;
			Y = -Y;
		}		
		xc=xa + X;
		yc=ya+ Y;
	}
	CenterPoint.x=xc;
	CenterPoint.y=yc;
}

/*
 *	让两个值相等
 */
void CCAD_PowerLandDoc::PosBeEqualed(CADPOINT &pos1,Position pos2)
{
	pos1.x = pos2.x;
	pos1.y = pos2.y;
}

/*
 *	让两个值相等
 */
void CCAD_PowerLandDoc::PosBeEqualed(Position &pos1,CADPOINT pos2)
{
	pos1.x = pos2.x;
	pos1.y = pos2.y;
}

/*
 *	返回选中的哪条线
 */
int CCAD_PowerLandDoc::NumPicked(Position pos,long& createbase)
{
	POSITION	POS = g_pDoc->m_CCreateBaseList.GetHeadPosition();	//获得头指针，在下面循环上用。
	int l_iNumPick = -1;
	CPoint l_pot;
	g_pView->WorldToScreem(pos,l_pot);
	CCreateBase *pCreate;
	while(POS != NULL)//进入扫描链表
	{
		pCreate = (CCreateBase *)g_pDoc->m_CCreateBaseList.GetNext(POS);

		l_iNumPick = pCreate->IsPick(l_pot);
		// 如果链表里的图元被选中
		if( l_iNumPick != -1)
		{
			createbase = (long)pCreate;
			return l_iNumPick;
		}
	}
	return -1;
}

/*
 *	把图形显示到全屏
 */
void CCAD_PowerLandDoc::OnFullScreemShow()
{
	int n = m_CCreateBaseList.GetCount();
	if(n == 1)  return;//如果是空集，就要返回
	BOX2D pBox1={0},pBox2={0};
	Position l_posMin,l_posMax;	//选择集的包围盒的两个点
	/////////////////获得选择集的包围盒//////////////////////////////
	POSITION POS = m_CCreateBaseList.GetHeadPosition();
	CCreateBase*	pEntity1;
	while(POS)
	{
		pEntity1 = (CCreateBase*)m_CCreateBaseList.GetNext(POS);
		if(pEntity1->GetBox(&pBox1))	//1，获得第一个图元包围盒
			break;
	}
	l_posMin.x = pBox1.min[0];
	l_posMin.y = pBox1.min[1];
	l_posMax.x = pBox1.max[0];
	l_posMax.y = pBox1.max[1];
	//2，与所有包围盒比较得反最终包围盒
	while(POS)
	{
		pEntity1 = (CCreateBase*)m_CCreateBaseList.GetNext(POS);
		if(pEntity1->GetBox(&pBox2))
		{
			l_posMin.x = min(l_posMin.x,pBox2.min[0]);
			l_posMin.y = min(l_posMin.y,pBox2.min[1]);
			l_posMax.x = max(l_posMax.x,pBox2.max[0]);
			l_posMax.y = max(l_posMax.y,pBox2.max[1]);	
		}
	}
	//求应该移动的坐标
	CRect rect;
	g_pView->GetClientRect(rect);
	g_pView->m_dOriginX = (l_posMin.x + l_posMax.x)/2;
	g_pView->m_dOriginY = (l_posMin.y + l_posMax.y)/2;
	double l_dPropX =(l_posMax.x-l_posMin.x) /(rect.right-8);
	double l_dPropY = (l_posMax.y-l_posMin.y)/(rect.bottom-8);
	if (l_dPropX >= l_dPropY)
		g_pView->m_dProportion = l_dPropX;
	else
		g_pView->m_dProportion = l_dPropY;	
	if(fabs(g_pView->m_dProportion) <= DOUBLE_EQUAL)
		g_pView->m_dProportion = 1;
	g_pView->Invalidate();
	return;
}

/*
 *	把图形移动到原点
 */
void CCAD_PowerLandDoc::OnMoveOrigin()
{
	int l_iSize = m_CollectOfPicked.GetSize();
	if(l_iSize < 1)  return;//如果是空集，就要返回
	BOX2D pBox1,pBox2;
	Position l_posMin,l_posMax;	//选择集的包围盒的两个点
	/////////////////获得选择集的包围盒//////////////////////////////
	CCreateBase*	pEntity1;
	for( int i = 0 ; i < l_iSize; i++ )
	{
		pEntity1 = (CCreateBase*) m_CollectOfPicked[i] ;
		if(pEntity1->GetBox(&pBox1))	//1，获得第一个图元包围盒
			break;
	}
	l_posMin.x = pBox1.min[0];
	l_posMin.y = pBox1.min[1];
	l_posMax.x = pBox1.max[0];
	l_posMax.y = pBox1.max[1];
	//2，与所有包围盒比较得反最终包围盒
	for( ; i < l_iSize; i++ )
	{
		pEntity1 = (CCreateBase*) m_CollectOfPicked[i] ;
		if(pEntity1->GetBox(&pBox2))
		{
			l_posMin.x = min(l_posMin.x,pBox2.min[0]);
			l_posMin.y = min(l_posMin.y,pBox2.min[1]);
			l_posMax.x = max(l_posMax.x,pBox2.max[0]);
			l_posMax.y = max(l_posMax.y,pBox2.max[1]);	
		}
	}
	//求应该移动的坐标
	Position l_posBase,l_posDes,l_posDisOrigin(g_pDoc->m_stcClothCut.dMoveOriginY,g_pDoc->m_stcClothCut.dMoveOriginX);
	l_posDes = (l_posMax-l_posMin)/2+l_posDisOrigin;
	l_posBase = (l_posMax+l_posMin)/2;
	//撤消操作
	CUndoRedo l_UndoRedo;//撤消类的对象 
	PUNDOLIST pUndoList = l_UndoRedo.NewUndoList();
	pUndoList->OperatieWhat = ID_MOVE;				
	pUndoList->pOperatieParam = l_UndoRedo.NewOperateParam();
	g_pDoc->PosBeEqualed(pUndoList->pOperatieParam->potBase,l_posBase);
	g_pDoc->PosBeEqualed(pUndoList->pOperatieParam->potDes,l_posDes);
	PGRAPHPOS CurGraphPos,headGraphPos,GraphPos;
	headGraphPos = CurGraphPos = l_UndoRedo.NewGraphPos();
	pUndoList->pGraphPos = headGraphPos;
	for( i = 0 ; i < l_iSize; i++ )
	{
		pEntity1 = (CCreateBase*) m_CollectOfPicked[i] ;
		pEntity1->Move(l_posBase,l_posDes);
		if (i == 0)
			CurGraphPos->pCreateBasepos = (long)pEntity1;
		else
		{
			GraphPos = new GRAPHPOS;
			memset(GraphPos,0,sizeof(GRAPHPOS));						
			GraphPos->pCreateBasepos = (long)pEntity1;
			CurGraphPos->pNext = GraphPos;
			GraphPos->pPrev = CurGraphPos;
			CurGraphPos = GraphPos;
		}	
	}
	g_pDoc->m_Undo->AddUndoList(pUndoList);//添加到撤消
	g_pView->Invalidate();
}



BOOL CCAD_PowerLandDoc::OnFileSaveAs() 
{
	if(((CMainFrame*)AfxGetMainWnd())->m_bSaveFile == FALSE)
		return FALSE;
	// TODO: Add your command handler code here
	// TODO: Add your command handler code here
	COutputGCode l_cGCode;
	COutputDXFCode l_cDXFCode;
	char szFilter[] = "G代码文件(*.G)|*.g|DXF文件(*.DXF)|*.dxf||";		//保存格式
	CFileDialog Dlg(FALSE, NULL, NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);//建一个文件对话框
	Dlg.m_ofn.lpstrTitle = "另存为";
	if (Dlg.DoModal() == IDOK)	//如果点击确定
	{
		m_strNameFile = Dlg.GetFileTitle();		
		CString l_strPathOfFile;//文件路径
		if(Dlg.m_ofn.nFilterIndex == 1)	//保存类型为第一个.G格式的
		{
			l_strPathOfFile = Dlg.GetPathName();//先获取要保存的文件路径
			if (Dlg.GetFileExt() != "G"&&Dlg.GetFileExt() != "g"&&Dlg.GetFileExt() != "DXF"&&Dlg.GetFileExt() != "dxf")		//文件没有扩展名时给加上.g的格式
				l_strPathOfFile += ".g";
			if (!l_cGCode.OutputGCode(l_strPathOfFile)) 
			{
				//中文还是英文
				if (g_pDoc->m_stcWorkFor.IsChinese)
					AfxMessageBox("有问题");			
				else
					AfxMessageBox("something the matter");	
			}
			else
			{
				CString l_strFileName = "PNC2000CAD  V1.1                    "+m_strNameFile+".g"+"              ";			
				SetTitle(l_strFileName);				
				m_strPathFile = l_strPathOfFile;//把路径放到成员到时保存时用
				m_iStyleFile = 1;	
			}
		}
		else if(Dlg.m_ofn.nFilterIndex == 2)	//保存类型为第二个.DXF格式的
		{
			l_strPathOfFile = Dlg.GetPathName();//先获取要保存的文件路径
			if (Dlg.GetFileExt() != "G"&&Dlg.GetFileExt() != "g"&&Dlg.GetFileExt() != "DXF"&&Dlg.GetFileExt() != "dxf")		//文件没有扩展名时给加上.g的格式
				l_strPathOfFile += ".dxf";
			if (!l_cDXFCode.OutputDXFCode(l_strPathOfFile)) 
			{
				//中文还是英文
				if (g_pDoc->m_stcWorkFor.IsChinese)
					AfxMessageBox("有问题");			
				else
					AfxMessageBox("something the matter");	
			}
			else
			{
				CString l_strFileName = "PNC2000CAD  V1.1                    "+m_strNameFile+".dxf"+"              ";			
				SetTitle(l_strFileName);
				m_strPathFile = l_strPathOfFile;//把路径放到成员到时保存时用
				m_iStyleFile = 2;	
			}			
		}
		m_bFirstOpen = FALSE;//是否是第一次打开
	}
	else
		return FALSE;
	return TRUE;
}

/*
 *	向单文档中传数据
 */
void CCAD_PowerLandDoc::InitDate()
{
	//传设定G代码中M值的对话框的数据
	CString l_strFlieName(INI_FILE_COLORM);
	l_strFlieName = l_strFlieName + "ColorM.ini";
	char l_chDataName[80]={0};
	GetPrivateProfileString("WorkParam","m_CBM1","-1",l_chDataName,20,l_strFlieName);
	g_pDoc->m_stcColorToM.m_CBM1 = l_chDataName;
	GetPrivateProfileString("WorkParam","m_CBM2","-1",l_chDataName,20,l_strFlieName);
	g_pDoc->m_stcColorToM.m_CBM2 = l_chDataName;
	GetPrivateProfileString("WorkParam","m_CBM3","-1",l_chDataName,20,l_strFlieName);
	g_pDoc->m_stcColorToM.m_CBM3 = l_chDataName;
	GetPrivateProfileString("WorkParam","m_CBM4","-1",l_chDataName,20,l_strFlieName);
	g_pDoc->m_stcColorToM.m_CBM4 = l_chDataName;
	GetPrivateProfileString("WorkParam","m_CBM5","-1",l_chDataName,20,l_strFlieName);
	g_pDoc->m_stcColorToM.m_CBM5 = l_chDataName;
	
	GetPrivateProfileString("WorkParam","m_SBM1","-1",l_chDataName,20,l_strFlieName);
	g_pDoc->m_stcColorToM.m_SBM1 = l_chDataName;
	GetPrivateProfileString("WorkParam","m_SBM2","-1",l_chDataName,20,l_strFlieName);
	g_pDoc->m_stcColorToM.m_SBM2 = l_chDataName;
	GetPrivateProfileString("WorkParam","m_SBM3","-1",l_chDataName,20,l_strFlieName);
	g_pDoc->m_stcColorToM.m_SBM3 = l_chDataName;
	GetPrivateProfileString("WorkParam","m_SBM4","-1",l_chDataName,20,l_strFlieName);
	g_pDoc->m_stcColorToM.m_SBM4 = l_chDataName;
	GetPrivateProfileString("WorkParam","m_SBM5","-1",l_chDataName,20,l_strFlieName);
	g_pDoc->m_stcColorToM.m_SBM5 = l_chDataName;
	
	GetPrivateProfileString("WorkParam","Color1","-1",l_chDataName,20,l_strFlieName);
	g_pDoc->m_stcColorToM.m_color1 = atoi(l_chDataName);
	GetPrivateProfileString("WorkParam","Color2","-1",l_chDataName,20,l_strFlieName);
	g_pDoc->m_stcColorToM.m_color2 = atoi(l_chDataName);
	GetPrivateProfileString("WorkParam","Color3","-1",l_chDataName,20,l_strFlieName);
	g_pDoc->m_stcColorToM.m_color3 = atoi(l_chDataName);
	GetPrivateProfileString("WorkParam","Color4","-1",l_chDataName,20,l_strFlieName);
	g_pDoc->m_stcColorToM.m_color4 = atoi(l_chDataName);
	GetPrivateProfileString("WorkParam","Color5","-1",l_chDataName,20,l_strFlieName);
	g_pDoc->m_stcColorToM.m_color5 = atoi(l_chDataName);

	GetPrivateProfileString("WorkParam","ColorMean1","-1",l_chDataName,20,l_strFlieName);
	g_pDoc->m_stcColorToM.m_ColorMean1 = l_chDataName;
	GetPrivateProfileString("WorkParam","ColorMean2","-1",l_chDataName,20,l_strFlieName);
	g_pDoc->m_stcColorToM.m_ColorMean2 = l_chDataName;
	GetPrivateProfileString("WorkParam","ColorMean3","-1",l_chDataName,20,l_strFlieName);
	g_pDoc->m_stcColorToM.m_ColorMean3 = l_chDataName;
	GetPrivateProfileString("WorkParam","ColorMean4","-1",l_chDataName,20,l_strFlieName);
	g_pDoc->m_stcColorToM.m_ColorMean4 = l_chDataName;
	GetPrivateProfileString("WorkParam","ColorMean5","-1",l_chDataName,20,l_strFlieName);
	g_pDoc->m_stcColorToM.m_ColorMean5 = l_chDataName;
	//传切布机中的过切的对话框的数据
	l_strFlieName = INI_FILE_COLORM;
	l_strFlieName = l_strFlieName + "ParamClothCut.ini";
	GetPrivateProfileString("OverCut","Angle","-1",l_chDataName,20,l_strFlieName);
	g_pDoc->m_stcClothCut.m_dOverCutAngle = atof(l_chDataName);
	GetPrivateProfileString("OverCut","Long","-1",l_chDataName,20,l_strFlieName);
	g_pDoc->m_stcClothCut.m_dOverCutLong = atof(l_chDataName);
	GetPrivateProfileString("OverCut","ScaleX","-1",l_chDataName,20,l_strFlieName);
	g_pDoc->m_stcClothCut.m_dScaleX = atof(l_chDataName);
	GetPrivateProfileString("OverCut","ScaleY","-1",l_chDataName,20,l_strFlieName);
	g_pDoc->m_stcClothCut.m_dScaleY = atof(l_chDataName);
	GetPrivateProfileString("OverCut","Chamfer","-1",l_chDataName,20,l_strFlieName);
	g_pDoc->m_stcClothCut.m_dChamfer = atof(l_chDataName);

	GetPrivateProfileString("OverCut","NeedOverCut1","-1",l_chDataName,20,l_strFlieName);
	g_pDoc->m_stcClothCut.m_bOverCutKnife1 = atoi(l_chDataName);
	GetPrivateProfileString("OverCut","NeedOverCut2","-1",l_chDataName,20,l_strFlieName);
	g_pDoc->m_stcClothCut.m_bOverCutKnife2 = atoi(l_chDataName);
	GetPrivateProfileString("OverCut","NeedOverCut3","-1",l_chDataName,20,l_strFlieName);
	g_pDoc->m_stcClothCut.m_bOverCutKnife3 = atoi(l_chDataName);
	GetPrivateProfileString("OverCut","NeedOverCut4","-1",l_chDataName,20,l_strFlieName);
	g_pDoc->m_stcClothCut.m_bOverCutKnife4 = atoi(l_chDataName);
	GetPrivateProfileString("OverCut","NeedOverCut5","-1",l_chDataName,20,l_strFlieName);
	g_pDoc->m_stcClothCut.m_bOverCutKnife5 = atoi(l_chDataName);

	GetPrivateProfileString("OverCut","MillSideDis","-1",l_chDataName,20,l_strFlieName);
	g_pDoc->m_stcClothCut.m_dMillSideDis = atof(l_chDataName);
	GetPrivateProfileString("OverCut","IsMillSideLeft","-1",l_chDataName,20,l_strFlieName);
	g_pDoc->m_stcClothCut.m_bMillSideLorR = atoi(l_chDataName);

	GetPrivateProfileString("OverCut","Simplify","-1",l_chDataName,20,l_strFlieName);
	g_pDoc->m_stcClothCut.dSimplify = atof(l_chDataName);
	GetPrivateProfileString("OverCut","LessPointRadius","-1",l_chDataName,20,l_strFlieName);
	g_pDoc->m_stcClothCut.dLessPointRadius = atof(l_chDataName);
	
	GetPrivateProfileString("OverCut","AddRectBottom","1",l_chDataName,20,l_strFlieName);
	g_pDoc->m_stcClothCut.m_dAddRectBottom = atof(l_chDataName);
	GetPrivateProfileString("OverCut","AddRectLeft","1",l_chDataName,20,l_strFlieName);
	g_pDoc->m_stcClothCut.m_dAddRectLeft = atof(l_chDataName);
	GetPrivateProfileString("OverCut","AddRectRight","1",l_chDataName,20,l_strFlieName);
	g_pDoc->m_stcClothCut.m_dAddRectRight = atof(l_chDataName);
	GetPrivateProfileString("OverCut","AddRectTop","1",l_chDataName,20,l_strFlieName);
	g_pDoc->m_stcClothCut.m_dAddRectTop = atof(l_chDataName);
	GetPrivateProfileString("OverCut","AddRectRadius","1",l_chDataName,20,l_strFlieName);
	g_pDoc->m_stcClothCut.dAddRectRadius = atof(l_chDataName);

	GetPrivateProfileString("OverCut","CheckChamferLT","-1",l_chDataName,20,l_strFlieName);
	g_pDoc->m_stcClothCut.bChamferLT = atoi(l_chDataName);
	GetPrivateProfileString("OverCut","CheckChamferLB","-1",l_chDataName,20,l_strFlieName);
	g_pDoc->m_stcClothCut.bChamferLB = atoi(l_chDataName);
	GetPrivateProfileString("OverCut","CheckChamferRT","-1",l_chDataName,20,l_strFlieName);
	g_pDoc->m_stcClothCut.bChamferRT = atoi(l_chDataName);
	GetPrivateProfileString("OverCut","CheckChamferRB","-1",l_chDataName,20,l_strFlieName);
	g_pDoc->m_stcClothCut.bChamferRB = atoi(l_chDataName);

	GetPrivateProfileString("OverCut","m_dParallelHori","-1",l_chDataName,20,l_strFlieName);
	g_pDoc->m_stcClothCut.m_dParallelHori = atof(l_chDataName);
	GetPrivateProfileString("OverCut","m_dParallelVert","-1",l_chDataName,20,l_strFlieName);
	g_pDoc->m_stcClothCut.m_dParallelVert = atof(l_chDataName);
	
	GetPrivateProfileString("OverCut","ChamferEndStyle","-1",l_chDataName,20,l_strFlieName);
	g_pDoc->m_stcClothCut.iChamferEndStyle = atoi(l_chDataName);

	GetPrivateProfileString("OverCut","dMoveOriginX","-1",l_chDataName,20,l_strFlieName);
	g_pDoc->m_stcClothCut.dMoveOriginX = atof(l_chDataName);
	GetPrivateProfileString("OverCut","dMoveOriginY","-1",l_chDataName,20,l_strFlieName);
	g_pDoc->m_stcClothCut.dMoveOriginY = atof(l_chDataName);
	
	GetPrivateProfileString("OverCut","dEditDashEmpty","-1",l_chDataName,20,l_strFlieName);
	g_pDoc->m_stcClothCut.dEditDashEmpty = atof(l_chDataName);
	GetPrivateProfileString("OverCut","dEditDashSolid","-1",l_chDataName,20,l_strFlieName);
	g_pDoc->m_stcClothCut.dEditDashSolid = atof(l_chDataName);
	
	//传磨玻璃的方法的对话框的数据
	l_strFlieName = INI_FILE_COLORM;
	l_strFlieName = l_strFlieName + "GlassGrind.ini";
	GetPrivateProfileString("Grind","RoughCount","-1",l_chDataName,20,l_strFlieName);
	g_pDoc->m_stcGlassGrind.RouphCount = atoi(l_chDataName);
	GetPrivateProfileString("Grind","RoughSize","-1",l_chDataName,20,l_strFlieName);
	g_pDoc->m_stcGlassGrind.RouphSize = atof(l_chDataName);
	GetPrivateProfileString("Grind","PerfectCount","-1",l_chDataName,20,l_strFlieName);
	g_pDoc->m_stcGlassGrind.PerfectCount = atoi(l_chDataName);
	GetPrivateProfileString("Grind","PerfectSize","-1",l_chDataName,20,l_strFlieName);
	g_pDoc->m_stcGlassGrind.PerfectSize = atof(l_chDataName);
	GetPrivateProfileString("Grind","PolishCount","-1",l_chDataName,20,l_strFlieName);
	g_pDoc->m_stcGlassGrind.PolishCount = atoi(l_chDataName);
	GetPrivateProfileString("Grind","PolishSize","-1",l_chDataName,20,l_strFlieName);
	g_pDoc->m_stcGlassGrind.PolishSize = atof(l_chDataName);
	GetPrivateProfileString("Grind","KnifeRadius","-1",l_chDataName,20,l_strFlieName);
//	g_pDoc->m_stcGlassGrind.KnifeRadius = atof(l_chDataName);
	g_pDoc->m_stcGlassGrind.KnifeRadius = 0.0;
	GetPrivateProfileString("Grind","Daobu","-1",l_chDataName,20,l_strFlieName);
	g_pDoc->m_stcGlassGrind.DaoBuStyle = atoi(l_chDataName);

	GetPrivateProfileString("Grind","iDirClock1","-1",l_chDataName,20,l_strFlieName);
	g_pDoc->m_stcGlassGrind.iDirClock1 = atoi(l_chDataName);
	GetPrivateProfileString("Grind","iDirClock2","-1",l_chDataName,20,l_strFlieName);
	g_pDoc->m_stcGlassGrind.iDirClock2 = atoi(l_chDataName);
	GetPrivateProfileString("Grind","iDirClock3","-1",l_chDataName,20,l_strFlieName);
	g_pDoc->m_stcGlassGrind.iDirClock3 = atoi(l_chDataName);
	GetPrivateProfileString("Grind","iDirClock4","-1",l_chDataName,20,l_strFlieName);
	g_pDoc->m_stcGlassGrind.iDirClock4 = atoi(l_chDataName);
	GetPrivateProfileString("Grind","iDirClock5","-1",l_chDataName,20,l_strFlieName);
	g_pDoc->m_stcGlassGrind.iDirClock5 = atoi(l_chDataName);
	
	////传进退刀的对话框的数据
	GetPrivateProfileString("IOKnife","i_ArcAngle","-1",l_chDataName,20,l_strFlieName);
	g_pDoc->m_stcIOKnife.IArcAngle = atof(l_chDataName);
	GetPrivateProfileString("IOKnife","i_ArcRadius","-1",l_chDataName,20,l_strFlieName);
	g_pDoc->m_stcIOKnife.IArcRadius = atof(l_chDataName);
	GetPrivateProfileString("IOKnife","i_ArcRadiusPercent","-1",l_chDataName,20,l_strFlieName);
	g_pDoc->m_stcIOKnife.IArcRadiusPercent = atof(l_chDataName);
	GetPrivateProfileString("IOKnife","i_LineLong","-1",l_chDataName,20,l_strFlieName);
	g_pDoc->m_stcIOKnife.ILineLong = atof(l_chDataName);
	GetPrivateProfileString("IOKnife","i_LineLongPercent","-1",l_chDataName,20,l_strFlieName);
	g_pDoc->m_stcIOKnife.ILineLongPercent = atof(l_chDataName);
	GetPrivateProfileString("IOKnife","o_ArcAngle","-1",l_chDataName,20,l_strFlieName);
	g_pDoc->m_stcIOKnife.OArcAngle = atof(l_chDataName);
	GetPrivateProfileString("IOKnife","o_ArcRadius","-1",l_chDataName,20,l_strFlieName);
	g_pDoc->m_stcIOKnife.OArcRadius = atof(l_chDataName);
	GetPrivateProfileString("IOKnife","o_ArcRadiusPercent","-1",l_chDataName,20,l_strFlieName);
	g_pDoc->m_stcIOKnife.OArcRadiusPercent = atof(l_chDataName);
	GetPrivateProfileString("IOKnife","o_LineLongPercent","-1",l_chDataName,20,l_strFlieName);
	g_pDoc->m_stcIOKnife.OLineLongPercent = atof(l_chDataName);
	GetPrivateProfileString("IOKnife","o_LineLong","-1",l_chDataName,20,l_strFlieName);
	g_pDoc->m_stcIOKnife.OLineLong = atof(l_chDataName);
	GetPrivateProfileString("IOKnife","IsCenterInput","-1",l_chDataName,20,l_strFlieName);
	g_pDoc->m_stcIOKnife.IsCenter = atoi(l_chDataName);
	GetPrivateProfileString("IOKnife","RadioILineVertical","-1",l_chDataName,20,l_strFlieName);
	g_pDoc->m_stcIOKnife.IsIKnifeVertical = atoi(l_chDataName);
	GetPrivateProfileString("IOKnife","o_OverLong","-1",l_chDataName,20,l_strFlieName);
	g_pDoc->m_stcIOKnife.OOverLong = atof(l_chDataName);
	GetPrivateProfileString("IOKnife","RadioOLineVertical","-1",l_chDataName,20,l_strFlieName);
	g_pDoc->m_stcIOKnife.IsOknifeVertical = atoi(l_chDataName);
	//工艺参数里
	l_strFlieName = INI_FILE_COLORM;
	l_strFlieName = l_strFlieName + "WorkFor.ini";
	GetPrivateProfileString("WorkFor","IsGlassCut","-1",l_chDataName,20,l_strFlieName);
	g_pDoc->m_stcWorkFor.IsGlassCut = atoi(l_chDataName);
	GetPrivateProfileString("WorkFor","IsGlassGrind","-1",l_chDataName,20,l_strFlieName);
	g_pDoc->m_stcWorkFor.IsGlassGrind = atoi(l_chDataName);
	GetPrivateProfileString("WorkFor","IsClothCut","-1",l_chDataName,20,l_strFlieName);
	g_pDoc->m_stcWorkFor.IsClothCut = atoi(l_chDataName);
	GetPrivateProfileString("WorkFor","strFilePathG","-1",l_chDataName,80,l_strFlieName);
	g_pDoc->m_stcWorkFor.strFilePathG = l_chDataName;
	GetPrivateProfileString("WorkFor","strFilePathDxf","-1",l_chDataName,80,l_strFlieName);
	g_pDoc->m_stcWorkFor.strFilePathDxf = l_chDataName;
	GetPrivateProfileString("WorkFor","MachineSizeX","-1",l_chDataName,20,l_strFlieName);
	g_pDoc->m_stcWorkFor.MachineX = atof(l_chDataName);
	GetPrivateProfileString("WorkFor","MachineSizeY","-1",l_chDataName,80,l_strFlieName);
	g_pDoc->m_stcWorkFor.MachineY = atof(l_chDataName);
	GetPrivateProfileString("WorkFor","IsShowSize","-1",l_chDataName,80,l_strFlieName);
	g_pDoc->m_stcWorkFor.IsShowSize = atoi(l_chDataName);
	GetPrivateProfileString("WorkFor","IsShowCoordinate","-1",l_chDataName,80,l_strFlieName);
	g_pDoc->m_stcWorkFor.IsShowCoordinate = atoi(l_chDataName);
	GetPrivateProfileString("WorkFor","IsShowRulerLine","-1",l_chDataName,80,l_strFlieName);
	g_pDoc->m_stcWorkFor.IsShowRulerLine = atoi(l_chDataName);
	GetPrivateProfileString("WorkFor","RotateSymmetrySaveOld","-1",l_chDataName,80,l_strFlieName);
	g_pDoc->m_stcWorkFor.RotateSymmetrySaveOld = atoi(l_chDataName);

	GetPrivateProfileString("WorkFor","bSaveDefault","-1",l_chDataName,80,l_strFlieName);
	g_pDoc->m_stcWorkFor.bSaveDefault = atoi(l_chDataName);
	GetPrivateProfileString("WorkFor","bSendStyleDxf","-1",l_chDataName,80,l_strFlieName);
	g_pDoc->m_stcWorkFor.bSendStyleDxf = atoi(l_chDataName);
	GetPrivateProfileString("WorkFor","bSendStyleG","-1",l_chDataName,80,l_strFlieName);
	g_pDoc->m_stcWorkFor.bSendStyleG = atoi(l_chDataName);
	
	GetPrivateProfileString("WorkFor","IsChinese","-1",l_chDataName,20,l_strFlieName);
	g_pDoc->m_stcWorkFor.IsChinese = atoi(l_chDataName);
}

/*
 * 把M字符集中的M字符替换
 */
BOOL CCAD_PowerLandDoc::ReplaceMCode(int color,CArray<class CString,class CString &>& aryMSB,CString strMSB, CArray<class CString,class CString &>& aryMCB,CString strMCB)
{
	//		代码库里对应的M代码
	CString l_strStockMSB,l_strStockMCB;
	//看颜色对应M代码
	if(color == this->m_stcColorToM.m_color1)
	{
		l_strStockMSB = m_stcColorToM.m_SBM1;
		l_strStockMCB = m_stcColorToM.m_CBM1;
	}
	else if(color == this->m_stcColorToM.m_color2)
	{
		l_strStockMSB = m_stcColorToM.m_SBM2;
		l_strStockMCB = m_stcColorToM.m_CBM2;
	}
	else if(color == this->m_stcColorToM.m_color3)
	{
		l_strStockMSB = m_stcColorToM.m_SBM3;
		l_strStockMCB = m_stcColorToM.m_CBM3;
	}
	else if(color == this->m_stcColorToM.m_color4)
	{
		l_strStockMSB = m_stcColorToM.m_SBM4;
		l_strStockMCB = m_stcColorToM.m_CBM4;
	}
	else if(color == this->m_stcColorToM.m_color5)
	{
		l_strStockMSB = m_stcColorToM.m_SBM5;
		l_strStockMCB = m_stcColorToM.m_CBM5;
	}
	else
		return FALSE;
	int l_iSizeSB = aryMSB.GetSize();
	//标志两个都能对应上吗
	BOOL l_bColorHave1 = FALSE,l_bColorHave2 = FALSE;
	for(int i=0;i<l_iSizeSB;i++)
	{
		if(l_strStockMSB == aryMSB.GetAt(i))
		{	
			l_bColorHave1 = TRUE;
			aryMSB.SetAt(i,strMSB);
		}
	}
	int l_iSizeCB = aryMCB.GetSize();
	for(int j=0;j<l_iSizeCB;j++)
	{
		if(l_strStockMCB == aryMCB.GetAt(j))
		{
			l_bColorHave2 = TRUE;
			aryMCB.SetAt(j,strMCB);		
		}
	}
	if(l_bColorHave1 == TRUE&&l_bColorHave2 == TRUE)
		return TRUE;
	return FALSE;	
}

/*
 *	退出进保存
 */
BOOL CCAD_PowerLandDoc::OnCloseSaveFile()
{
	if(OnFileSave())
		return TRUE;
	return FALSE;
}

/*
 *	输入点返回是否选中图形，	鼠标点击点			返回的图形			图形上的点
 */
BOOL CCAD_PowerLandDoc::ClickCreateBase(Position posClick,long& pCreateBase,int& iPick)
{
	//屏幕鼠标的点
	CPoint point;
	g_pView->WorldToScreem(posClick,point);
	POSITION	POS = g_pDoc->m_CCreateBaseList.GetHeadPosition();	//获得头指针，在下面循环上用。
	CCreateBase* l_pCreateBase = NULL;
	//查看是否选中链表
	while(POS != NULL)//进入扫描链表
	{
		l_pCreateBase = (CCreateBase *)g_pDoc->m_CCreateBaseList.GetNext(POS);
		// 如果链表里的图元被选中
		iPick = l_pCreateBase->IsPick(point);
		if( iPick != -1)
		{
			pCreateBase = (long)l_pCreateBase;	
			return TRUE;
		}
	}	
	return FALSE;
}

/*
 *	返回图形类型1直线，2多段线，3矩形，4圆弧，5圆，6椭圆
 */
int CCAD_PowerLandDoc::RtnIsKindOf(CCreateBase* pCreateBase)
{
	if (pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateLine))) 
		return MY_LINE;
	else if (pCreateBase->IsKindOf(RUNTIME_CLASS(CCreatePolyline))) 
		return MY_POLYLINE;
	else if (pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRectangle))) 
		return MY_RECTANGLE;
	else if (pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRoundArc))) 
		return MY_ARC;
	else if (pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRound))) 
		return MY_ROUND;
	else if (pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateEllipse))) 
		return MY_ELLIPSE;
	else if (pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateSpline))) 
		return MY_SPLINE;
	else if (pCreateBase->IsKindOf(RUNTIME_CLASS(CCreatePoint))) 
		return MY_POINT;
	return -1;
}

/*
 *	把线变成虚线
 */
void CCAD_PowerLandDoc::OnSolidToDash()
{
	CCreateBase* pCreateBase = NULL;
	//建撤消对像
	CUndoRedo l_cUndo;
	//实线和空白的长度
	double l_dLongSolid = g_pDoc->m_stcClothCut.dEditDashSolid;
	double l_dLongEmpty = g_pDoc->m_stcClothCut.dEditDashEmpty;
	int l_iSize = m_CollectOfPicked.GetSize();
	//变虚线
	for (int i=0;i<l_iSize;i++)
	{
		PUNDOLIST l_pUndolist = l_cUndo.NewUndoList();
		l_pUndolist->OperatieWhat = ID_SOLID_TO_DASH;
		l_pUndolist->pGraphPos = l_cUndo.NewGraphPos();
		//变虚线
		pCreateBase = (CCreateBase*)m_CollectOfPicked.GetAt(i);
		PGRAPHPOS headGraphDash = pCreateBase->SolidTurnDash(l_dLongSolid,l_dLongEmpty,l_pUndolist);
		m_CCreateBaseList.RemoveAt(m_CCreateBaseList.Find(pCreateBase));
		//把图形保存到撤消链表里
		l_pUndolist->pGraphPos->pCreateBasepos = (long)pCreateBase;
		l_pUndolist->pGraphPosSpare = headGraphDash;
		g_pDoc->m_Undo->AddUndoList(l_pUndolist);
	}
	m_CollectOfPicked.RemoveAll();
	g_pView->Invalidate();
}

/*
 *	连接两个图形链表PGRAPHPOS
 */
void CCAD_PowerLandDoc::AddTail(PGRAPHPOS headGraphPos,PGRAPHPOS tailGraphPos)
{
	while (headGraphPos->pNext != NULL)
		headGraphPos = headGraphPos->pNext;//循环到尾
	headGraphPos->pNext = tailGraphPos;
	tailGraphPos->pPrev = headGraphPos;	
}



















