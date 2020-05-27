// ModifyRotate.cpp: implementation of the CModifyRotate class.
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

CModifyRotate::CModifyRotate()
{
	m_iNumOfClick = 0;
	m_bFlagIsDeletePer = FALSE;	
	m_CopyList.RemoveAll();
}

CModifyRotate::~CModifyRotate()
{

}


int CModifyRotate::OnLButtonDown(UINT nFlags,const Position &pos,CString cmd)
{
	Position l_cPos(pos);
	//�趨��׽���ĵ�
	SetLButtonDownPot(l_cPos,m_iNumOfClick);
	BOOL l_FlagUseAngleInput = FALSE;	//��־�Ƕ��������;
	double l_dAngle;//��ת�ĽǶ�
	if (nFlags == 2)	//���ǿո��س���Ҫ�������������Ϊ��
	{
		int l_num = cmd.FindOneOf(",");
		if(l_num == -1&&(cmd =="Y"||cmd=="y")&&m_iNumOfClick == 0)
		{
			m_bFlagIsDeletePer = TRUE;
			//���Ļ���Ӣ��
			if (g_pDoc->m_stcWorkFor.IsChinese)
				RemindContent("\r\n�Ѿ�ѡ��Ϊɾ��ԭ��ͼ��:");
			else
				RemindContent("\r\nHave chosen to delete the original graphics",2);
			((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.SetDlgItemText(IDC_EDIT_COMMAND,"");
			return 1;			
		}
		else if (l_num == -1&&(cmd =="N"||cmd=="n")&&m_iNumOfClick == 0)
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
		if (m_iNumOfClick == 1) //��һ�����뷽��ʱ��������
		{
			l_dAngle = atof(cmd)/180*MATH_PI;
			l_FlagUseAngleInput = TRUE;
		}
		else if(m_iNumOfClick == 0)	//����Ϊ��ʱ
		{
			if(l_num == -1)//��������ݲ���
				return InvalidContent();//��������ʾ���������Ч�����
			l_cPos.x = atof(cmd.Left(l_num));	//����ǰ������
			l_cPos.y = atof(cmd.Mid(l_num+1));	//���ź������
		}
	}
	m_iNumOfClick ++ ;
	g_refresh = 0;
	switch(m_iNumOfClick)
	{
	case 1:
		RemindContent("\r\nָ����ת�Ƕ�,��ESC��ȡ��:");
		((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.SetDlgItemText(IDC_EDIT_COMMAND,"");
		// ��һ�ε���������ʱ�õ�����λ�ã��������趨Ŀ��λ��
		m_basePos = m_desPos = l_cPos;
		break;
	case 2:
		{
			CDC*	pDC = g_pView->GetDC(); // ���������豸����ָ��
			int i, n;
/*
			//ɾ���ƶ�·������Ƥ��
			CCreateLine*	pTempLine2 = new CCreateLine(m_basePos, m_desPos);	
			pTempLine2->DrawMove(pDC,dmDrag);
			delete pTempLine2;
			for(n = g_pDoc->m_CollectOfPicked.GetSize(), i = 0; i < n; i++)
			{
				CCreateBase*	pEntity1 = (CCreateBase*)g_pDoc->m_CollectOfPicked[i];
				// �����һ��λ���ϻ��Ƶ�ͼԪ
				CCreateBase*	pCopy1 = pEntity1->Copy(); // �õ�ͼԪ�Ŀ���
				double angle = ::GetAngleToXAxis(m_basePos, m_desPos);
				pCopy1->Rotate(m_basePos, angle); // �������ƶ�����һ��λ��
				pCopy1->DrawMove(pDC,dmDrag,g_pView->m_stcSwitch.bShowArrow,g_pView->m_stcSwitch.bShowNum); // ������һ��λ�öԿ��������ػ�
				delete pCopy1; // ɾ����ʱ����
			}			
*/
	////////////////////////////////////////////////////////////////////////
			m_desPos = l_cPos;
			int l_iSize = g_pDoc->m_CollectOfPicked.GetSize();
			for(n = l_iSize, i = 0; i < n; i++)
			{
				CCreateBase*	pEntity = (CCreateBase*)g_pDoc->m_CollectOfPicked[i];

				if(m_bFlagIsDeletePer == FALSE)	//����ԭ����ͼ��
				{
					CCreateBase* pCopy = pEntity->Copy();
					if (l_FlagUseAngleInput == FALSE)//�������Ƕ��ˣ������������γɵĽǶ�
						l_dAngle = GetAngleToXAxis(m_basePos, m_desPos);
					pCopy->Rotate(m_basePos, l_dAngle);
					pCopy->DrawMove(pDC,dmNormal,g_pView->m_stcSwitch.bShowArrow,g_pView->m_stcSwitch.bShowNum);
					g_pDoc->m_CCreateBaseList.AddTail(pCopy);
					m_CopyList.AddTail(pCopy);
				}
				else//������ԭ����ͼ��
				{
					pEntity->DrawMove(pDC,dmInvalid,g_pView->m_stcSwitch.bShowArrow,g_pView->m_stcSwitch.bShowNum);//���ԭ����ͼ��
					if (l_FlagUseAngleInput == FALSE)//�������Ƕ��ˣ������������γɵĽǶ�
						l_dAngle = GetAngleToXAxis(m_basePos, m_desPos);
					pEntity->Rotate(m_basePos, l_dAngle);
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
			break;
		}
	default:
		break;
	}
	//�ѽ�����������������
	((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.GetDlgItem(IDC_EDIT_COMMAND)->SetFocus();
	return 0;
}

int CModifyRotate::OnMouseMove(UINT nFlags,const Position &pos)
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
					double angle = ::GetAngleToXAxis(m_basePos, prePos);
					pCopy1->Rotate(m_basePos, angle); // �������ƶ�����һ��λ��
					pCopy1->DrawMove(pDC,dmDrag); // ������һ��λ�öԿ��������ػ�
					delete pCopy1; // ɾ����ʱ����
				}
				// �ڵ�ǰλ���ϻ���ͼԪ// 
				CCreateBase*	pCopy2 = pEntity->Copy();// �õ�ͼԪ�Ŀ���
				double angle = ::GetAngleToXAxis(m_basePos, curPos);
				pCopy2->Rotate(m_basePos, angle); // �������ƶ�����һ��λ��
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

int CModifyRotate::OnRButtonDown(UINT nFlags,const Position &pos)
{
	
	return 0;
}

/*
 *	��ӳ�������
 */
void CModifyRotate::AddUndoList()
{
	CUndoRedo l_UndoRedo;//������Ķ��� 
	PUNDOLIST pUndoList = l_UndoRedo.NewUndoList();
	pUndoList->OperatieWhat = ID_ROTATE;

	pUndoList->pOperatieParam = l_UndoRedo.NewOperateParam();
	g_pDoc->PosBeEqualed(pUndoList->pOperatieParam->potBase,m_basePos);
	double l_dAngle = GetAngleToXAxis(m_basePos, m_desPos);
	pUndoList->pOperatieParam->angle = l_dAngle;
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