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
	if(!SaveParam()) //�����ĸ�ʽ ����
		return;	
	//ִ�в���
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
			SetDlgItemText(IDC_STATIC1,"ˮƽ");
			SetDlgItemText(IDC_STATIC2,"CM");
			m_dEditParallelVert = g_pDoc->m_stcClothCut.m_dParallelVert;
			SetDlgItemText(IDC_STATIC3,"��ֱ");
			SetDlgItemText(IDC_STATIC4,"CM");
*/
		}
		break;
	case 2:
		{
			m_dEditParallelHori = 0;//g_pDoc->m_stcClothCut.m_dRotateAngle;
			if (g_pDoc->m_stcWorkFor.IsChinese)
				SetDlgItemText(IDC_STATIC1,"ָ����ת�Ƕ�");
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
 *	��ӽ������������
 */
void CDlgParallel::AddUndoList(long pCreatebase1,long pCreatebase2)
{
	switch(m_iCase)
	{
		case 1:
			{	
				CUndoRedo l_UndoRedo;//������Ķ��� 
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
 *	�������
 */
BOOL CDlgParallel::SaveParam()
{
	if(!UpdateData(TRUE)) //�����ĸ�ʽ ����
		return FALSE;	
	switch(m_iCase)
	{
	case 1:
		{
			g_pDoc->m_stcClothCut.m_dParallelHori = m_dEditParallelHori;
			CString l_strFlieName(INI_FILE_COLORM);
			l_strFlieName = l_strFlieName + "ParamClothCut.ini";			
			CString l_strDataName;			//�����õ��м�����һ�������ݣ�һ���Ǽ���
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
			CString l_strDataName;			//�����õ��м�����һ�������ݣ�һ���Ǽ���
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
 *	ִ��ƽ���߹���
 */
void CDlgParallel::DoParallel()
{
	//���				���Ƶ���λ��
	Position l_posZore(0,0),l_posMove(g_pDoc->m_stcClothCut.m_dParallelHori,g_pDoc->m_stcClothCut.m_dParallelVert);
	CCreateBase	*pEntity,*pCopy;
	CDC* pDC = g_pView->GetDC();
	int l_iSize = g_pDoc->m_CollectOfPicked.GetSize();
	CUndoRedo l_UndoRedo;//������Ķ��� 
	PUNDOLIST pUndoList = l_UndoRedo.NewUndoList();
	pUndoList->OperatieWhat = ID_LINE;
	
	PGRAPHPOS CurGraphPos,headGraphPos,GraphPos;
	headGraphPos = CurGraphPos = l_UndoRedo.NewGraphPos();
	pUndoList->pGraphPos = headGraphPos;

	for(int i = 0;i<l_iSize;i++)
	{
		pEntity = (CCreateBase*)g_pDoc->m_CollectOfPicked[i];
		pCopy = pEntity->Copy();	//����һ���µ�Ҫ���Ƶ�ͼ�� 
		pCopy->Move(l_posZore, l_posMove);
		pCopy->DrawMove(pDC,dmNormal); // ��Ŀ��λ���ϻ���ͼԪ
		g_pDoc->m_CCreateBaseList.AddTail(pCopy);//���ƺ��ͼԪ�ŵ�������
		pEntity->DrawMove(pDC,dmNormal);	//��ԭ����ͼ���ػ�һ�£���ѡ�б�Ϊ��ѡ�е�����
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
	//�Ѳ���������ӵ�����������
	g_pDoc->m_Undo->AddUndoList(pUndoList);
	g_pDoc->m_CollectOfPicked.RemoveAll();
	g_pView->ReleaseDC(pDC);
}

/*
 *	ִ��ָ���Ƕ���ת����
 */
void CDlgParallel::DoRotateAngle()
{
	double l_dAngle = m_dEditParallelHori/180 * MATH_PI;
	g_pDoc->OnRotateCenter(l_dAngle);
}
