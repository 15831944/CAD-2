// DrawZoomScale1.cpp: implementation of the CDrawZoomScale class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cad_powerland.h"
#include "Draw.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDrawZoomScale::CDrawZoomScale()
{
	m_bBase =FALSE;
}

CDrawZoomScale::~CDrawZoomScale()
{

}

int CDrawZoomScale::OnLButtonDown(UINT nFlags,const Position &pos,CString cmd)
{
	Position l_cPos(pos);
	//�趨��׽���ĵ�
	SetLButtonDownPot(l_cPos,m_iNumOfClick);
	if (nFlags == 2)	//���ǿո��س���Ҫ�������������Ϊ��
	{
		int l_num = cmd.FindOneOf(",");
		if(l_num == -1)//����������Ǳ�������
		{		
			if(m_bBase == FALSE)
				return InvalidContent();//��������ʾ���������Ч�����			
			double l_dLong = atof(cmd);
			if (l_dLong>DISTANCE_ZERO)
				return OnZoomScale(l_dLong);
			else
				return InvalidContent();//��������ʾ���������Ч�����
		}
		else
		{
			if(m_bBase == FALSE)//���û�л���
			{
				l_cPos.x = atof(cmd.Left(l_num));	//����ǰ������
				l_cPos.y = atof(cmd.Mid(l_num+1));	//���ź������
			}
			else
				return InvalidContent();//��������ʾ���������Ч�����			
		}
	}
	m_posBase = l_cPos;
	m_bBase = TRUE;
	//���Ļ���Ӣ��
	if (g_pDoc->m_stcWorkFor.IsChinese)
		RemindContent("\r\nָ����������:");
	else
		RemindContent("\r\nSpecifies the scaling factor:");					
	((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.SetDlgItemText(IDC_EDIT_COMMAND,"");
	//�ѽ�����������������
	((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.GetDlgItem(IDC_EDIT_COMMAND)->SetFocus();
	
	return 0;
}

int CDrawZoomScale::OnMouseMove(UINT nFlags,const Position &pos)
{
	SetCursor(AfxGetApp()->LoadCursor(IDC_DRAW));//�ı������״	
	CDC* pDC = g_pView->GetDC(); 
	FindPointAll(pDC,pos,pos,g_pView->m_iSwitch);		
	g_pView->ReleaseDC(pDC); 
	return 0;
}



int CDrawZoomScale::OnRButtonDown(UINT nFlags,const Position &pos)
{

	return 0;
}

/*
 *	��������
 */
int CDrawZoomScale::OnZoomScale(double dScale)
{
	CDC* pDC = g_pView->GetDC(); 
	//ѡ�񼯵Ĵ�С
	int l_iSize = g_pDoc->m_CollectOfPicked.GetSize();
	CCreateBase*	pSelEntity;
	//������������Ӳ���
	CUndoRedo l_UndoRedo;//������Ķ��� 
	PUNDOLIST pUndoList = l_UndoRedo.NewUndoList();
	pUndoList->OperatieWhat = ID_SCALE;
	
	pUndoList->pOperatieParam = l_UndoRedo.NewOperateParam();
	g_pDoc->PosBeEqualed(pUndoList->pOperatieParam->potBase,m_posBase);
	pUndoList->pOperatieParam->angle = dScale;
	
	PGRAPHPOS CurGraphPos,headGraphPos,GraphPos;
	headGraphPos = CurGraphPos = l_UndoRedo.NewGraphPos();
	pUndoList->pGraphPos = headGraphPos;
	for( int i = 0 ; i < l_iSize ; i++ )
	{
		pSelEntity = (CCreateBase*)g_pDoc->m_CollectOfPicked[i] ;
		pSelEntity->GraphZoomScale(m_posBase,dScale);
		if (CurGraphPos->pCreateBasepos == NULL)
			CurGraphPos->pCreateBasepos = (long)pSelEntity;
		else
		{
			GraphPos = l_UndoRedo.NewGraphPos();						
			GraphPos->pCreateBasepos = (long)pSelEntity;
			CurGraphPos->pNext = GraphPos;
			GraphPos->pPrev = CurGraphPos;
			CurGraphPos = GraphPos;
		} 
		
	}
	//��ӳ�������
	g_pDoc->m_Undo->AddUndoList(pUndoList);
	g_pDoc->RemoveAllCollected();
	//������ͼԪ//��Ҫ����ͼԪɾ��
	delete g_pView->m_pDrawBase;
	g_pView->m_pDrawBase = NULL;	
	g_pView->ReleaseDC(pDC); 
	CRect rect;
	g_pView->GetClientRect(rect);
	g_pView->InvalidateRect(rect);
	return 1;
}

