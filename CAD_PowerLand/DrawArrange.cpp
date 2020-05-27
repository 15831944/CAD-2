// Arrange.cpp: implementation of the CArrange class.
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

CDrawArrange::CDrawArrange()
{
	m_PosOfArrange = g_pDoc->m_CCreateBaseList.GetHeadPosition();
	g_pDoc->m_CCreateBaseList.GetNext(m_PosOfArrange);
	while (m_PosOfArrange != NULL)//��������i�ɱ�ʾ������������
	{
		CCreateBase* l_pDelList = (CCreateBase*)g_pDoc->m_CCreateBaseList.GetNext(m_PosOfArrange);
		if (!l_pDelList->IsKindOf(RUNTIME_CLASS(CCreateMCode)))//��ǰ���ж��ٸ�M����
			break;
	}
	if (m_PosOfArrange == NULL)
	{
		//ȡ������Ĺ���
		delete g_pView->m_pDrawBase;
		g_pView->m_pDrawBase = NULL;
	}
	else
		g_pDoc->m_CCreateBaseList.GetPrev(m_PosOfArrange);	
}

CDrawArrange::~CDrawArrange()
{

}

int CDrawArrange::OnLButtonDown(UINT nFlags,const Position &pos,CString cmd)
{
	SetCursor(AfxGetApp()->LoadCursor(IDC_BREAK_TWO));
	CPoint l_ptScreen;
	BOOL l_bFlagClick = FALSE; //��־�Ƿ����
	POSITION l_posPre;	//��ǰһͼԪ��
	g_pView->WorldToScreem(pos,l_ptScreen);	//ת����Ļ����
	POSITION	position = g_pDoc->m_CCreateBaseList.GetHeadPosition();	//���ͷָ�룬������ѭ�����á�
	//�鿴�ĸ�ͼ�α���ѡ��
	while(position != NULL)//����ɨ������
	{
		l_posPre = position;
		CCreateBase*	l_pCreateBase = (CCreateBase *)g_pDoc->m_CCreateBaseList.GetNext(position);
		// ����������ͼԪ����ѡѡ��
		if( l_pCreateBase->IsPick(l_ptScreen) != -1)
		{
			//���㵽���һ��ʱҪ�˳���
			if(m_PosOfArrange == g_pDoc->m_CCreateBaseList.GetTailPosition())
			{
				//���Ļ���Ӣ��
				if (g_pDoc->m_stcWorkFor.IsChinese)
					RemindContent("\r\n������ɣ�");
				else
					RemindContent("\r\nArrange to complete��");
				//ȡ������Ĺ���
				delete g_pView->m_pDrawBase;
				g_pView->m_pDrawBase = NULL;
				return 0;
			}
			if (l_posPre == m_PosOfArrange)//������е��ǲ�����Ҫ�����һ��
				g_pDoc->m_CCreateBaseList.GetNext(m_PosOfArrange);
			//������
			position = l_posPre;
			g_pDoc->m_CCreateBaseList.GetPrev(position);
			g_pDoc->m_CCreateBaseList.RemoveAt(l_posPre);
			AddUndoList(position,(LONG)l_pCreateBase);
			g_pDoc->m_CCreateBaseList.InsertBefore(m_PosOfArrange,l_pCreateBase);
			g_pView->Invalidate();
			l_bFlagClick = TRUE;//��־����
			break;
		}
	}
	if (l_bFlagClick == TRUE)
	{
		//���Ļ���Ӣ��
		if (g_pDoc->m_stcWorkFor.IsChinese)
			RemindContent("\r\n���У�");
		else
			RemindContent("\r\nClicked��");	
	}		
	else
	{
		//���Ļ���Ӣ��
		if (g_pDoc->m_stcWorkFor.IsChinese)
			RemindContent("\r\nû�е��У�");
		else
			RemindContent("\r\nNo clicked��");	
	}
	return 0;
}

/*
 *	��ӳ�������
 */
void CDrawArrange::AddUndoList(POSITION pos,LONG pCreateBase)
{
	//������������Ӳ���
	CUndoRedo l_UndoRedo;//������Ķ��� 
	PUNDOLIST pUndoList = l_UndoRedo.NewUndoList();
	pUndoList->OperatieWhat = ID_RE_ARRANG;

	pUndoList->pGraphPos = l_UndoRedo.NewGraphPos();
	pUndoList->pGraphPos->pCreateBasepos = pCreateBase;
	long pGraph = (long)g_pDoc->m_CCreateBaseList.GetAt(pos);
	pUndoList->pGraphPos->pCreateBaseposSpare1 = pGraph;
	pGraph = (long)g_pDoc->m_CCreateBaseList.GetAt(m_PosOfArrange);
	pUndoList->pGraphPos->pCreateBaseposSpare2 = pGraph;
	g_pDoc->m_Undo->AddUndoList(pUndoList);
}

int CDrawArrange::OnMouseMove(UINT nFlags,const Position &pos)
{
	SetCursor(AfxGetApp()->LoadCursor(IDC_BREAK_TWO));
	return 0;	
}

int CDrawArrange::OnRButtonDown(UINT nFlags,const Position &pos)
{
	//���Ļ���Ӣ��
	if (g_pDoc->m_stcWorkFor.IsChinese)
		RemindContent("\r\n������ɣ�");
	else
		RemindContent("\r\nArrange to complete��");
	//ȡ������Ĺ���
	delete g_pView->m_pDrawBase;
	g_pView->m_pDrawBase = NULL;
	return 0;
}

