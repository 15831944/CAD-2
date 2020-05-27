// ModifyMove.cpp: implementation of the CModifyMove class.
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

CModifyMove::CModifyMove()
{
	m_iNumOfClick = 0;
}

CModifyMove::~CModifyMove()
{

}

int CModifyMove::OnLButtonDown(UINT nFlags,const Position &pos,CString cmd)
{
	CUndoRedo l_UndoRedo;//������Ķ��� 
	Position l_cPos(pos);
	//�趨��׽���ĵ�
	SetLButtonDownPot(l_cPos,m_iNumOfClick);
	if (nFlags == 2)	//���ǿո��س���Ҫ�������������Ϊ��
	{
		int l_num = cmd.FindOneOf(",");
		if(l_num == -1)//��������ݲ���
		{
			if(cmd == "d"||cmd == "D")//����������λ��
			{
				l_cPos.x = 0;
				l_cPos.y = 0;
				((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.SetDlgItemText(IDC_EDIT_COMMAND,"");				
			}
			else//�������
			{
				double l_dDistance = atof(cmd);//����ľ���
				double l_dAngle = GetAngleToXAxis(m_basePos,l_cPos);
				l_cPos.x = m_basePos.x + cos(l_dAngle)*l_dDistance;
				l_cPos.y = m_basePos.y + sin(l_dAngle)*l_dDistance;
			}
		}
		else
		{
			l_cPos.x = atof(cmd.Left(l_num));	//����ǰ������
			l_cPos.y = atof(cmd.Mid(l_num+1));	//���ź������
		}
	}
	m_iNumOfClick ++ ;
	g_refresh = 0;
	switch(m_iNumOfClick)
	{
	case 1:	
		//���Ļ���Ӣ��
		if (g_pDoc->m_stcWorkFor.IsChinese)// ��һ�ε���������ʱ�õ�����λ�ã��������趨Ŀ��λ��
			RemindContent("\r\nָ���ڶ������λ�ƾ���,��ESC��ȡ��:");
		else
			RemindContent("\r\nSpecify the second point or distance, or press the ESC key to cancel");		
		((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.SetDlgItemText(IDC_EDIT_COMMAND,"");
		//�ѽ�����������������
		((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.GetDlgItem(IDC_EDIT_COMMAND)->SetFocus();
		m_basePos = m_desPos = l_cPos;
		break;
	case 2:
		{
			//���Ļ���Ӣ��
			if (g_pDoc->m_stcWorkFor.IsChinese)
				RemindContent("\r\n���:");
			else
				RemindContent("\r\nComplete:");		
			((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.SetDlgItemText(IDC_EDIT_COMMAND,"");
			CDC*	pDC = g_pView->GetDC(); // ���������豸����ָ��
			//ɾ���ƶ�·������Ƥ��
			CCreateLine*	pTempLine2 = new CCreateLine(m_basePos, m_desPos);	
			pTempLine2->DrawMove(pDC,dmDrag);
			delete pTempLine2;
			m_desPos = l_cPos;

				PUNDOLIST pUndoList = l_UndoRedo.NewUndoList();
				pUndoList->OperatieWhat = ID_MOVE;				
				pUndoList->pOperatieParam = l_UndoRedo.NewOperateParam();

				g_pDoc->PosBeEqualed(pUndoList->pOperatieParam->potBase,m_basePos);
				g_pDoc->PosBeEqualed(pUndoList->pOperatieParam->potDes,m_desPos);
		

			// ��ѡ���е�ͼԪ�ƶ���Ŀ��λ�ò����л���
			int i, n;
	
				PGRAPHPOS CurGraphPos,headGraphPos,GraphPos;
				headGraphPos = CurGraphPos = l_UndoRedo.NewGraphPos();
				pUndoList->pGraphPos = headGraphPos;
			for(n = g_pDoc->m_CollectOfPicked.GetSize(), i = 0; i < n; i++)
			{
				CCreateBase*	pEntity = (CCreateBase*)g_pDoc->m_CollectOfPicked[i];
				pEntity->DrawMove(pDC,dmInvalid,g_pView->m_stcSwitch.bShowArrow,g_pView->m_stcSwitch.bShowNum);	//�ƶ���󣬰�ԭ����ͼԪ������
				pEntity->Move(m_basePos, m_desPos);
				pEntity->DrawMove(pDC,dmNormal,g_pView->m_stcSwitch.bShowArrow,g_pView->m_stcSwitch.bShowNum); // ��Ŀ��λ���ϻ���ͼԪ
					
					if (i == 0)
						CurGraphPos->pCreateBasepos = (long)pEntity;
					else
					{
						GraphPos = new GRAPHPOS;
						memset(GraphPos,0,sizeof(GRAPHPOS));						
						GraphPos->pCreateBasepos = (long)pEntity;
						CurGraphPos->pNext = GraphPos;
						GraphPos->pPrev = CurGraphPos;
						CurGraphPos = GraphPos;
					}							
			}
			g_pDoc->m_Undo->AddUndoList(pUndoList);

			g_pDoc->m_CollectOfPicked.RemoveAll(); //  ���ѡ��
			g_pView->ReleaseDC(pDC); // �ͷ�������豸����ָ��
			m_iNumOfClick = 0; 
			//������ͼԪ//��Ҫ����ͼԪɾ��
			delete g_pView->m_pDrawBase;
			g_pView->m_pDrawBase = NULL;	//��Ϊ�գ����´ε�����ʱ���������Ƿ��ѡ�С�
			g_pView->Invalidate();
			break;
		}
	default:
		break;
	}
	return 0;
}

int CModifyMove::OnMouseMove(UINT nFlags,const Position &pos)
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
					pCopy1->Move(m_basePos, prePos); // �������ƶ�����һ��λ��
					pCopy1->DrawMove(pDC,dmDrag); // ������һ��λ�öԿ��������ػ�
					delete pCopy1; // ɾ����ʱ����
				}
				// �ڵ�ǰλ���ϻ���ͼԪ
				CCreateBase*	pCopy2 = pEntity->Copy();// �õ�ͼԪ�Ŀ���
				pCopy2->Move(m_basePos, curPos); // �������ƶ�����һ��λ��
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

int CModifyMove::OnRButtonDown(UINT nFlags,const Position &pos)
{
	
	return 0;
}