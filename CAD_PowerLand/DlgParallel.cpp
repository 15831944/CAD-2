// DlgParallel.cpp : implementation file
//

#include "stdafx.h"
#include "cad_powerland.h"
#include "DlgParallel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgParallel dialog


CDlgParallel::CDlgParallel(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgParallel::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgParallel)
	m_dEditParallelHori = 0.0;
	m_dEditParallelVert = 0.0;
	//}}AFX_DATA_INIT
}


void CDlgParallel::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgParallel)
	DDX_Text(pDX, IDC_EDIT_PARALLEL_HORI, m_dEditParallelHori);
	DDX_Text(pDX, IDC_EDIT_PARALLEL_VERT, m_dEditParallelVert);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgParallel, CDialog)
	//{{AFX_MSG_MAP(CDlgParallel)
	ON_BN_CLICKED(ID_PARALLEL_OK, OnParallelOk)
	ON_BN_CLICKED(ID_PARALLEL_CANCEL, OnParallelCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgParallel message handlers

void CDlgParallel::OnParallelOk() 
{
	// TODO: Add your control notification handler code here
	if(!SaveParam()) //如果添的格式 不对
		return;	
	//执行操作
	switch(m_iCase)
	{
	case 1:
		DoParallel();
		break;
	case 2:
		DoRotateAngle();
		break;
	default:
		break;
	}

	CDialog::OnOK();	
}

void CDlgParallel::OnParallelCancel() 
{
	// TODO: Add your control notification handler code here


	CDialog::OnCancel();	
}

BOOL CDlgParallel::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	switch(m_iCase)
	{
	case 1:
		{
/*
			m_dEditParallelHori = g_pDoc->m_stcClothCut.m_dParallelHori;
			SetDlgItemText(IDC_STATIC1,"水平");
			SetDlgItemText(IDC_STATIC2,"CM");
			m_dEditParallelVert = g_pDoc->m_stcClothCut.m_dParallelVert;
			SetDlgItemText(IDC_STATIC3,"垂直");
			SetDlgItemText(IDC_STATIC4,"CM");
*/
		}
		break;
	case 2:
		{
			m_dEditParallelHori = 0;//g_pDoc->m_stcClothCut.m_dRotateAngle;
			if (g_pDoc->m_stcWorkFor.IsChinese)
				SetDlgItemText(IDC_STATIC1,"指定旋转角度");
			else
				SetDlgItemText(IDC_STATIC1,"Angle");
			SetDlgItemText(IDC_STATIC2,"");	
			GetDlgItem(IDC_STATIC3)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC4)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_EDIT_PARALLEL_VERT)->ShowWindow(SW_HIDE);	
		}
		break;
	default:
		break;
	}
	UpdateData(FALSE);


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/*
 *	添加进撤消链表操作
 */
void CDlgParallel::AddUndoList(long pCreatebase1,long pCreatebase2)
{
	switch(m_iCase)
	{
		case 1:
			{	
				CUndoRedo l_UndoRedo;//撤消类的对象 
				PUNDOLIST pUndoList = l_UndoRedo.NewUndoList();
				pUndoList->OperatieWhat = ID_PARALLEL;
				
				pUndoList->pGraphPos = l_UndoRedo.NewGraphPos();
				pUndoList->pGraphPos->pCreateBasepos = pCreatebase1;
				pUndoList->pGraphPos->pCreateBaseposSpare1 = pCreatebase2;
				g_pDoc->m_Undo->AddUndoList(pUndoList);		
			}
			break;
		case 2:
			{
			}
			break;
		default:
			break;
	}
		
}

/*
 *	保存参数
 */
BOOL CDlgParallel::SaveParam()
{
	if(!UpdateData(TRUE)) //如果添的格式 不对
		return FALSE;	
	switch(m_iCase)
	{
	case 1:
		{
			g_pDoc->m_stcClothCut.m_dParallelHori = m_dEditParallelHori;
			CString l_strFlieName(INI_FILE_COLORM);
			l_strFlieName = l_strFlieName + "ParamClothCut.ini";			
			CString l_strDataName;			//保存用的中间数据一个是数据，一个是键名
			GetDlgItemText(IDC_EDIT_PARALLEL_HORI,l_strDataName);
			WritePrivateProfileString("OverCut","m_dParallelHori",l_strDataName,l_strFlieName);

			g_pDoc->m_stcClothCut.m_dParallelVert = m_dEditParallelVert;			
			GetDlgItemText(IDC_EDIT_PARALLEL_VERT,l_strDataName);
			WritePrivateProfileString("OverCut","m_dParallelVert",l_strDataName,l_strFlieName);
		}
		break;
	case 2:
		{
			g_pDoc->m_stcClothCut.m_dRotateAngle = m_dEditParallelHori;
			CString l_strFlieName(INI_FILE_COLORM);
			l_strFlieName = l_strFlieName + "ParamClothCut.ini";			
			CString l_strDataName;			//保存用的中间数据一个是数据，一个是键名
			GetDlgItemText(IDC_EDIT_PARALLEL_HORI,l_strDataName);
			WritePrivateProfileString("OverCut","RotateAngle",l_strDataName,l_strFlieName);
		}
		break;
	default:
		break;
	}
	return TRUE;
}

/*
 *	执行平行线功能
 */
void CDlgParallel::DoParallel()
{
	//零点				复制到的位置
	Position l_posZore(0,0),l_posMove(g_pDoc->m_stcClothCut.m_dParallelHori,g_pDoc->m_stcClothCut.m_dParallelVert);
	CCreateBase	*pEntity,*pCopy;
	CDC* pDC = g_pView->GetDC();
	int l_iSize = g_pDoc->m_CollectOfPicked.GetSize();
	CUndoRedo l_UndoRedo;//撤消类的对象 
	PUNDOLIST pUndoList = l_UndoRedo.NewUndoList();
	pUndoList->OperatieWhat = ID_LINE;
	
	PGRAPHPOS CurGraphPos,headGraphPos,GraphPos;
	headGraphPos = CurGraphPos = l_UndoRedo.NewGraphPos();
	pUndoList->pGraphPos = headGraphPos;

	for(int i = 0;i<l_iSize;i++)
	{
		pEntity = (CCreateBase*)g_pDoc->m_CollectOfPicked[i];
		pCopy = pEntity->Copy();	//创建一个新的要复制的图形 
		pCopy->Move(l_posZore, l_posMove);
		pCopy->DrawMove(pDC,dmNormal); // 在目标位置上绘制图元
		g_pDoc->m_CCreateBaseList.AddTail(pCopy);//复制后的图元放到链表里
		pEntity->DrawMove(pDC,dmNormal);	//把原来的图形重画一下，从选中变为不选中的样子
		if (i == 0)
			CurGraphPos->pCreateBasepos = (long)pCopy;
		else
		{
			GraphPos = l_UndoRedo.NewGraphPos();						
			GraphPos->pCreateBasepos = (long)pCopy;
			CurGraphPos->pNext = GraphPos;
			GraphPos->pPrev = CurGraphPos;
			CurGraphPos = GraphPos;
		}
	}
	//把操作步骤添加到撤消链表里
	g_pDoc->m_Undo->AddUndoList(pUndoList);
	g_pDoc->m_CollectOfPicked.RemoveAll();
	g_pView->ReleaseDC(pDC);
}

/*
 *	执行指定角度旋转功能
 */
void CDlgParallel::DoRotateAngle()
{
	double l_dAngle = m_dEditParallelHori/180 * MATH_PI;
	g_pDoc->OnRotateCenter(l_dAngle);
}
