// ModifyMirror.cpp: implementation of the CModifyMirror class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cad_powerland.h"
#include "Modify.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CModifyMirror::CModifyMirror()
{
	m_iNumOfClick = 0;
	m_bFlagIsDeletePer = FALSE;			
}

CModifyMirror::~CModifyMirror()
{

}

int CModifyMirror::OnLButtonDown(UINT nFlags,const Position &pos,CString cmd)
{
	Position l_cPos(pos);
	//�趨��׽���ĵ�
	SetLButtonDownPot(l_cPos,m_iNumOfClick);
	if (nFlags == 2)	//���ǿո��س���Ҫ�������������Ϊ��
	{
		//����������
		int l_num = cmd.FindOneOf(",");
		if(l_num == -1&&(cmd =="Y"||cmd=="y")&&m_iNumOfClick == 0)//����õ�����Y���Ǳ���ԭ����ͼ��
		{
			m_bFlagIsDeletePer = TRUE;
			//���Ļ���Ӣ��
			if (g_pDoc->m_stcWorkFor.IsChinese)
				RemindContent("\r\n�Ѿ�ѡ��Ϊɾ��ԭ��ͼ��",2);
			else
				RemindContent("\r\nHas chosen to delete the original graphics",2);
			((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.SetDlgItemText(IDC_EDIT_COMMAND,"");
			return 1;
		}
		else if(l_num == -1&&(cmd =="N"||cmd=="n")&&m_iNumOfClick == 0)
		{	
			m_bFlagIsDeletePer = FALSE;
			//���Ļ���Ӣ��
			if (g_pDoc->m_stcWorkFor.IsChinese)
				RemindContent("\r\n�Ѿ�ѡ��Ϊ��ɾ��ԭ��ͼ��",2);
			else
				RemindContent("\r\nHas chosen to not to delete the original graphics",2);
			((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.SetDlgItemText(IDC_EDIT_COMMAND,"");
			return 1;
		}
		if(l_num == -1&&m_iNumOfClick != 0)//��������ݲ���
			return InvalidContent();//��������ʾ���������Ч�����
		l_cPos.x = atof(cmd.Left(l_num));	//����ǰ������
		l_cPos.y = atof(cmd.Mid(l_num+1));	//���ź������
	}
	m_iNumOfClick ++ ;
	g_refresh = 0;
	switch(m_iNumOfClick)
	{
	case 1:
		// ��һ�ε���������ʱ�õ�����λ�ã��������趨Ŀ��λ��
		RemindContent("\r\nָ���ڶ�����,��ESC��ȡ��:");
		((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.SetDlgItemText(IDC_EDIT_COMMAND,"");
		m_basePos = m_desPos = l_cPos;
		break;
	case 2:
		{
			CDC*	pDC = g_pView->GetDC(); // ���������豸����ָ��
			//ɾ���ƶ�·������Ƥ��
			CCreateLine*	pTempLine2 = new CCreateLine(m_basePos, m_desPos);	
			pTempLine2->DrawMove(pDC,dmDrag);
			delete pTempLine2;

			m_desPos = l_cPos;
			int i, n;
			//��ѡ�񼯶�����һ��
			for(n = g_pDoc->m_CollectOfPicked.GetSize(), i = 0; i < n; i++)
			{
				CCreateBase*	pEntity = (CCreateBase*)g_pDoc->m_CollectOfPicked[i];
				if(m_bFlagIsDeletePer == FALSE)	//����ԭ����ͼ��
				{
					CCreateBase* pCopy = pEntity->Copy();
					pCopy->Mirror(m_basePos,m_desPos);
					pCopy->DrawMove(pDC,dmNormal,g_pView->m_stcSwitch.bShowArrow,g_pView->m_stcSwitch.bShowNum);
					g_pDoc->m_CCreateBaseList.AddTail(pCopy);
					m_CopyList.AddTail(pCopy);
				}
				else//������ԭ����ͼ��
				{
					pEntity->DrawMove(pDC,dmInvalid,g_pView->m_stcSwitch.bShowArrow,g_pView->m_stcSwitch.bShowNum);
					pEntity->Mirror(m_basePos, m_desPos);
				}
				pEntity->DrawMove(pDC,dmNormal,g_pView->m_stcSwitch.bShowArrow,g_pView->m_stcSwitch.bShowNum); // ��Ŀ��λ���ϻ���ͼԪ
			}
			//�Ѳ���������ӵ�����������
			AddUndoList();
			g_pDoc->m_CollectOfPicked.RemoveAll(); //  ���ѡ��
			g_pView->ReleaseDC(pDC); // �ͷ�������豸����ָ��
			m_iNumOfClick = 0; 
			//������ͼԪ//��Ҫ����ͼԪɾ��
			delete g_pView->m_pDrawBase;
			g_pView->m_pDrawBase = NULL;	//��Ϊ�գ����´ε�����ʱ���������Ƿ��ѡ�С�
			g_pView->Invalidate();
			//���Ļ���Ӣ��
			if (g_pDoc->m_stcWorkFor.IsChinese)
				RemindContent("\r\n���:");
			else
				RemindContent("\r\nComplete:");		
			((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.SetDlgItemText(IDC_EDIT_COMMAND,"");
			break;
		}
	default:
		break;
	}
	return 0;
}

/*
 *	��ӳ�������
 */
void CModifyMirror::AddUndoList()
{
	//������������Ӳ���
	CUndoRedo l_UndoRedo;//������Ķ��� 
	PUNDOLIST pUndoList = l_UndoRedo.NewUndoList();
	pUndoList->OperatieWhat = ID_MIRROR;

	pUndoList->pOperatieParam = l_UndoRedo.NewOperateParam();
	g_pDoc->PosBeEqualed(pUndoList->pOperatieParam->potBase,m_basePos);
	g_pDoc->PosBeEqualed(pUndoList->pOperatieParam->potDes,m_desPos);
	pUndoList->pOperatieParam->direction = m_bFlagIsDeletePer;
	
	PGRAPHPOS CurGraphPos,headGraphPos,GraphPos;
	headGraphPos = CurGraphPos = l_UndoRedo.NewGraphPos();
	pUndoList->pGraphPos = headGraphPos;
	int n,i;
	CCreateBase*	pEntity;
	//��ѡ����ԭͼʱ
	if(m_bFlagIsDeletePer == FALSE)
	{
		POSITION POS = m_CopyList.GetHeadPosition();
		pEntity = (CCreateBase*)m_CopyList.GetNext(POS);		
		CurGraphPos->pCreateBasepos = (long)pEntity;
		while (POS)
		{
			pEntity = (CCreateBase*)m_CopyList.GetNext(POS);		
			GraphPos = l_UndoRedo.NewGraphPos();						
			GraphPos->pCreateBasepos = (long)pEntity;
			CurGraphPos->pNext = GraphPos;
			GraphPos->pPrev = CurGraphPos;
			CurGraphPos = GraphPos;									
		}
		g_pDoc->m_Undo->AddUndoList(pUndoList);
		return;
	}
	for(n = g_pDoc->m_CollectOfPicked.GetSize(), i = 0; i < n; i++)
	{		
		pEntity = (CCreateBase*)g_pDoc->m_CollectOfPicked[i];		
		if (i == 0)
			CurGraphPos->pCreateBasepos = (long)pEntity;
		else
		{
			GraphPos = l_UndoRedo.NewGraphPos();						
			GraphPos->pCreateBasepos = (long)pEntity;
			CurGraphPos->pNext = GraphPos;
			GraphPos->pPrev = CurGraphPos;
			CurGraphPos = GraphPos;
		}							
	}
	g_pDoc->m_Undo->AddUndoList(pUndoList);
}

int CModifyMirror::OnMouseMove(UINT nFlags,const Position &pos)
{
	SetCursor(AfxGetApp()->LoadCursor(IDC_DRAW));//�ı������״	
	CDC*	pDC = g_pView->GetDC(); 
	if (g_refresh == 1000)
		g_refresh = 100;
	g_refresh++;
	switch(m_iNumOfClick)
	{
	case 0:
		//����׽
		FindPointAll(pDC,pos,pos,g_pView->m_iSwitch);
		break;
	case 1:
		{
			Position	prePos, curPos;
			prePos = m_desPos; // �����һ��Ŀ��λ��
			curPos = pos; // �õ���ǰλ��
			//����׽
			FindPointAll(pDC,m_basePos,curPos,g_pView->m_iSwitch);			
			// ����ڲ��������д���û�б�ˢ�£���Ҫ�����һ�λ��Ƶ���Ƥ��
			CCreateLine*	pTempLine1 = new CCreateLine(m_basePos, prePos);
			if(g_refresh != 1)	//����Ϊ�˵�һ��û�в���ʱ������
				// ���Ӵ�û�б�ˢ��ʱ���ػ�ԭ������Ƥ��ʹ�䱻����
				pTempLine1->DrawMove(pDC,dmDrag);
			delete pTempLine1;
			//�趨��׽���ĵ�
			if((g_pView->m_iSwitch&ORTHOGONAL) == ORTHOGONAL)
				SetLButtonDownPot(curPos,m_iNumOfClick);
			// ������ʱ���󣬸��ݵ�ǰλ�û���һ����Ƥ��
			CCreateLine*	pTempLine2 = new CCreateLine(m_basePos, curPos);	
			pTempLine2->DrawMove(pDC,dmDrag);
			delete pTempLine2;
			// ���ݵ�ǰλ�ø���ѡ��ͼԪ��ʵʱλ��
			
			
			int i, n;
			for(n = g_pDoc->m_CollectOfPicked.GetSize(), i = 0; i < n; i++)
			{
				CCreateBase*	pEntity = (CCreateBase*)g_pDoc->m_CollectOfPicked[i];
				
				//		pEntity->DrawMove(pDC,dmSelect);
				// ����ڲ��������д���û�б�ˢ�£���Ҫ�����һ��λ���ϻ��Ƶ�ͼԪ
				if(g_refresh != 1)	//����Ϊ�˵�һ��û�в���ʱ������
					// ���Ӵ�û�б�ˢ��ʱ���ػ�ԭ������Ƥ��ʹ�䱻����
				{
					// �����һ��λ���ϻ��Ƶ�ͼԪ
					CCreateBase*	pCopy1 = pEntity->Copy(); // �õ�ͼԪ�Ŀ���
					pCopy1->Mirror(m_basePos, prePos); // �������ƶ�����һ��λ��
					pCopy1->DrawMove(pDC,dmDrag); // ������һ��λ�öԿ��������ػ�
					delete pCopy1; // ɾ����ʱ����
				}
				// �ڵ�ǰλ���ϻ���ͼԪ
				CCreateBase*	pCopy2 = pEntity->Copy();// �õ�ͼԪ�Ŀ���
				pCopy2->Mirror(m_basePos, curPos); // �������ƶ�����һ��λ��
				pCopy2->DrawMove(pDC,dmDrag);	// �Ե�ǰλ�û��ƿ���
				delete pCopy2; // ɾ����ʱ���� 
			}
			m_desPos = curPos; // ��Ŀ������Ϊ��ǰλ��
		}
	default:
		break;
	}	
	g_pView->ReleaseDC(pDC);
	return 0;
}

int CModifyMirror::OnRButtonDown(UINT nFlags,const Position &pos)
{
	
	return 0;
}