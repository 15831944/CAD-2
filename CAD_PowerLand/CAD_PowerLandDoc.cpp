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
	m_bFirstOpen = TRUE;//�Ƿ��ǵ�һ�δ�
	InitDate();//��ʼ������
	g_strCBM = g_pDoc->m_stcColorToM.m_CBM1;
	g_strSBM = g_pDoc->m_stcColorToM.m_SBM1;
	g_CurColor = g_pDoc->m_stcColorToM.m_color1;	
}

CCAD_PowerLandDoc::~CCAD_PowerLandDoc()
{
	//�������ͼԪ��ռ�õ��ڴ�
	POSITION pos = m_CCreateBaseList.GetHeadPosition();
	while (pos != NULL)
	{
		CCreateBase* l_pDelList = (CCreateBase*)m_CCreateBaseList.GetNext(pos);
		delete l_pDelList;
	}
	m_CCreateBaseList.RemoveAll();
	//���ѡ��ͼԪ
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
	//������������µ��ڴ�
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
	m_bFirstOpen = TRUE;//��һ�δ��ļ���������Ҫ���Ϊ
	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
//	SetTitle("PNC2000CAD--ģ��༭���");           //�����ĵ����� 
	SetTitle("PNC2000CAD  V1.1");           //�����ĵ����� 
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
	//�������ĳߴ�
	DrawMeasure(pDC);
	// ����ͼ��1�е�ͼԪ
	POSITION pos = m_CCreateBaselayer1.GetHeadPosition();
	while (pos != NULL)
	{
		CCreateBase* l_pCreateBase = (CCreateBase*)m_CCreateBaselayer1.GetNext(pos);
		l_pCreateBase->DrawMove(pDC,dmLayer);
	}
	// ���������е�ͼԪ
	pos = m_CCreateBaseList.GetHeadPosition();
	while (pos != NULL)
	{
		CCreateBase* l_pCreateBase = (CCreateBase*)m_CCreateBaseList.GetNext(pos);
		l_pCreateBase->DrawMove(pDC,dmNormal,g_pView->m_stcSwitch.bShowArrow,g_pView->m_stcSwitch.bShowNum);
	}
	// ����ѡ���е�ͼԪ
	for(int i = 0;  i<m_CollectOfPicked.GetSize();i++)
	{
		CCreateBase* l_pCreateBaseList = (CCreateBase*)m_CollectOfPicked[i];
		l_pCreateBaseList->DrawMove(pDC,dmSelect);
	}
	//��ѡ����ֻ��һ��Ԫ��
	if(g_pDoc->m_CollectOfPicked.GetSize() == 1)
	{
		CCreateBase* pPropertCreate = (CCreateBase*)g_pDoc->m_CollectOfPicked.GetAt(0);
		pPropertCreate->DrawMove(pDC,dmSelect,0,0);
		if(pPropertCreate->IsKindOf(RUNTIME_CLASS(CCreatePolyline))||
			pPropertCreate->IsKindOf(RUNTIME_CLASS(CCreateRectangle))||
			pPropertCreate->IsKindOf(RUNTIME_CLASS(CCreateSpline))||
			pPropertCreate->IsKindOf(RUNTIME_CLASS(CCreateEllipse)))//����Ƕ���߻򷽻���Բ����Ҫ����
		{
			//�����ڵ�X
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
 *	�������ߴ�
 */
void CCAD_PowerLandDoc::DrawMeasure(CDC* pDC)
{
	if (m_pMeasureCreateBase == NULL)
		return;
	CCreateLine *pLine = (CCreateLine*)m_pMeasureCreateBase;
	//��ֱ���룬ˮƽ���룬ֱ�߳���
	double disVer,disHor,disLong;
	disVer = fabs(pLine->m_Begin.y - pLine->m_End.y);
	disHor = fabs(pLine->m_Begin.x - pLine->m_End.x);
	disLong = pLine->m_Begin.Distance(pLine->m_End);
	CString strVer,strHor,strLong;
	strVer.Format("��ֱ%.3f",disVer);
	strHor.Format("ˮƽ%.3f",disHor);
	strLong.Format("ֱ�߳�%.3f",disLong);	
	pDC->SetTextColor(RGB(64,128,128));
	pDC->SetBkMode(TRANSPARENT);  //�����Ϳ���͸����������				
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
	//�ȸ�ͼ��������һ���޹ص�M���룬��֤���������ĵ�ַ�����ǿյġ�
	CCreateKong* pTempMWhat = new CCreateKong();
	m_CCreateBaseList.AddTail(pTempMWhat);

	//��ճ�����ռ�õ��ڴ�
	if (m_Undo)
	{
		delete m_Undo;
		m_Undo = NULL;
	}
	m_Undo = new CUndoRedo();
	//������������µ��ڴ�
	if (m_pMeasureCreateBase != NULL) 
	{
		delete 	m_pMeasureCreateBase;	
		m_pMeasureCreateBase = NULL;	
	}	
	CDocument::DeleteContents();
}

void CCAD_PowerLandDoc::OnLButtonDown(UINT nFlags, CPoint point)
{
	CDC*	pDC = g_pView->GetDC() ;// �õ��ӵ��豸����ָ��
 	BOOL	l_bPicked = FALSE;
	POSITION	position = m_CCreateBaseList.GetHeadPosition();	//���ͷָ�룬������ѭ�����á�
	CCreateBase*	l_pCreateBase;
	while(position != NULL)//����ɨ������
	{
		l_pCreateBase = (CCreateBase *)m_CCreateBaseList.GetNext(position);
		// ����������ͼԪ��ѡ��
		if( l_pCreateBase->IsPick(point) != -1)
		{ 
			l_bPicked = TRUE;
			
			int l_iNumCollect = IsSelected(l_pCreateBase);
			if( l_iNumCollect == -1 )// �ж�ͼԪ�����Ƿ��Ѿ���ѡ����,û��ѡ����
			{
				l_pCreateBase->DrawMove(pDC,dmSelect);
				m_CollectOfPicked.Add(l_pCreateBase);  // ��ͼԪ����ѡ����
				//��ѡ����ֻ��һ��Ԫ��
				if(g_pDoc->m_CollectOfPicked.GetSize() == 1)
				{
					l_pCreateBase->PropertyView();
					((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.SetDlgItemText(IDC_STATIC10,"�ӹ�˳��:");					
				}	
				else 
				{
					CCreateBase l_show;
					((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.SetDlgItemText(IDC_EDIT_SEQUENCE,"");				
					((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.m_ComboColor.SetCurSel(5);					
					l_show.ShowControl(0,FALSE);
				}
				
				break;// ��ĳһ��ͼԪ��ʰȡ���󣬼��˳�ѭ��
			}
			else//��ѡ����
			{
				if (nFlags == 5)//��SHIFT����ѡ��ѡ�����Ƴ�
				{
					m_CollectOfPicked.RemoveAt(l_iNumCollect);
					l_pCreateBase->DrawMove(pDC,dmNormal);
				}

			}
			//��ѡ����ֻ��һ��Ԫ��
			if(g_pDoc->m_CollectOfPicked.GetSize() == 1)
			{
				CCreateBase* pPropertCreate = (CCreateBase*)g_pDoc->m_CollectOfPicked.GetAt(0);
				pPropertCreate->PropertyView();
				((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.SetDlgItemText(IDC_STATIC10,"�ӹ�˳��:");					
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
	if(l_bPicked == FALSE)//û��ѡ�к��Ҫ�����ѡ
	{
		Position pos;
		g_pView->ScreemToWorld(point,pos);
		g_pView->m_pDrawBase = new CDrawSelect();
		g_pView->m_iFlagWhichBtnDown = DRAW_SELECT;		
		SetCursor(AfxGetApp()->LoadCursor(IDC_NONE));
		g_pView->m_pDrawBase->OnLButtonDown(1,pos,"");
	}
	g_pView->ReleaseDC(pDC);// �ͷ��ӵ��豸����ָ��
}

void CCAD_PowerLandDoc::OnLButtonDblClk(CPoint point)
{
/*
	CDC*	pDC = g_pView->GetDC() ;// �õ��ӵ��豸����ָ��
	POSITION	position = m_CCreateBaseList.GetHeadPosition();
	while(position != NULL)
	{
		CCreateBase*	l_pCreateBase = (CCreateBase *)m_CCreateBaseList.GetNext(position);
		// �õ���ǰ״̬��ʰȡ�뾶
		if( l_pCreateBase->IsPick(point) != -1)
		{ 
			RemoveAllCollected();
			// ��ĳһ��ͼԪ��ʰȡ���󣬼��˳�ѭ��
			l_pCreateBase->PropertyView();
			if(m_pTwoCreateBase)	//���˫��ѡ�еĻ�
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
				l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRoundArc)))//������ߣ�Բ�����ͰѲX��Ūû
			{
				//�����ڵ�X
				CPoint SCreemPos;
				g_pView->WorldToScreem(((CMainFrame*)AfxGetMainWnd())->m_PerXPoint,SCreemPos);
				CRect rect(SCreemPos.x-15,SCreemPos.y-15,SCreemPos.x+15,SCreemPos.y+15);
				g_pView->InvalidateRect(rect);
			}
			//////////////////////////////////////////////////////////////////////////
			if(m_pTwoCreateBase)
				m_pTwoCreateBase->DrawMove(pDC,dmPrompt);//�ѵ�ǰ�Ļ���˫��ѡ�е�	
			break;
		}
	}		
	g_pView->ReleaseDC(pDC);// �ͷ��ӵ��豸����ָ��*/

}

void CCAD_PowerLandDoc::OnMouseMove(UINT nFlags, CPoint point)
{	

}

/*
 *	���ѡ��
 */
void CCAD_PowerLandDoc::RemoveAllCollected()
{
	// ���Ƚ�ѡ���е�Ԫ�ػ���Ϊ����״̬��Ȼ�����ѡ��
	CDC*	pDC = g_pView->GetDC();
	//ѡ�񼯵Ĵ�С
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
{	// �ж�ͼԪ�����Ƿ��Ѿ���ѡ����
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
*	�����ļ���Ť
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
	if(((CMainFrame*)AfxGetMainWnd())->m_bSaveFile == FALSE)//�Ƿ��м��ܹ������û�оͲ�����
			return FALSE;
	COutputGCode l_cGCode;
	COutputDXFCode l_cDXFCode;
	//����ʱ�Ƿ񵯳��Ի���
	if(m_strPathFile != "")
	{
		if(m_iStyleFile == 1)
			l_cGCode.OutputGCode(m_strPathFile);
		else if(m_iStyleFile ==2)
			l_cDXFCode.OutputDXFCode(m_strPathFile);
		return TRUE;
	}

	// TODO: Add your command handler code here
	char szFilter[] = "G�����ļ�(*.G)|*.g|DXF�ļ�(*.DXF)|*.dxf||";		//�����ʽ
	CFileDialog Dlg(FALSE, NULL, NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);//��һ���ļ��Ի���
	Dlg.m_ofn.lpstrTitle = "����";
	if (Dlg.DoModal() == IDOK)	//������ȷ��
	{
		CString l_strPathOfFile;//�ļ�·��
		if(Dlg.m_ofn.nFilterIndex == 1)	//��������Ϊ��һ��.G��ʽ��
		{
			l_strPathOfFile = Dlg.GetPathName();//�Ȼ�ȡҪ������ļ�·��
			if (Dlg.GetFileExt() != "G"&&Dlg.GetFileExt() != "g"&&Dlg.GetFileExt() != "DXF"&&Dlg.GetFileExt() != "dxf")		//�ļ�û����չ��ʱ������.g�ĸ�ʽ
				l_strPathOfFile += ".g";
			if (!l_cGCode.OutputGCode(l_strPathOfFile)) 
			{
				//���Ļ���Ӣ��
				if (g_pDoc->m_stcWorkFor.IsChinese)
					AfxMessageBox("������");			
				else
					AfxMessageBox("something the matter");	
			}
			else
			{
				CString l_strFileName = "PNC2000CAD  V1.1                    "+m_strNameFile+".g"+"              ";			
				SetTitle(l_strFileName);				
				m_strPathFile = l_strPathOfFile;//��·���ŵ���Ա��ʱ����ʱ��
				m_iStyleFile = 1;	
			}
		}
		else if(Dlg.m_ofn.nFilterIndex == 2)	//��������Ϊ�ڶ���.DXF��ʽ��
		{
			l_strPathOfFile = Dlg.GetPathName();//�Ȼ�ȡҪ������ļ�·��
			if (Dlg.GetFileExt() != "G"&&Dlg.GetFileExt() != "g"&&Dlg.GetFileExt() != "DXF"&&Dlg.GetFileExt() != "dxf")		//�ļ�û����չ��ʱ������.g�ĸ�ʽ
				l_strPathOfFile += ".dxf";
			if (!l_cDXFCode.OutputDXFCode(l_strPathOfFile)) 
			{
				//���Ļ���Ӣ��
				if (g_pDoc->m_stcWorkFor.IsChinese)
					AfxMessageBox("������");			
				else
					AfxMessageBox("something the matter");	
			}
			else
			{
				CString l_strFileName = "PNC2000CAD  V1.1                    "+m_strNameFile+".dxf"+"              ";			
				SetTitle(l_strFileName);
				m_strPathFile = l_strPathOfFile;//��·���ŵ���Ա��ʱ����ʱ��
				m_iStyleFile = 2;	
			}			
		}
		m_bFirstOpen = FALSE;//�Ƿ��ǵ�һ�δ�	
	}
	else
		return FALSE;
	return TRUE;	
}

/*
*	���ļ���Ť
*/
void CCAD_PowerLandDoc::OnFileOpen() 
{
	// TODO: Add your command handler code here
	char szFilter[] = "All Files (*.*)|*.*|G�����ļ�(*.g)|*.g|DXF�ļ�(*.DXF)|*.dxf|PLT�ļ� (*.PLT)|*.PLT||";
	CFileDialog Dlg(TRUE, NULL, NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, szFilter);
	if (Dlg.DoModal() == IDOK)	//������ȷ��
	{
		//////////////////////////////////////////////////////////////////////////
		//������ɾ��֮ǰ�����ݣ�Ȼ���������
		POSITION pos = m_CCreateBaseList.GetHeadPosition();
		while (pos != NULL)
		{
			CCreateBase* l_pDelList = (CCreateBase*)m_CCreateBaseList.GetNext(pos);
			delete l_pDelList;
			l_pDelList = NULL;
		}
		g_pDoc->m_CCreateBaselayer1.RemoveAll();
		m_CCreateBaseList.RemoveAll();	
		//ɾ��ѡ�����ָ�� 
		m_CollectOfPicked.RemoveAll() ;
		//�峷������
		if (m_Undo)
		{
			delete m_Undo;
			m_Undo = NULL;
		}
		m_Undo = new CUndoRedo();
		//�ȸ�ͼ��������һ���޹ص�M���룬��֤���������ĵ�ַ�����ǿյġ�
		CCreateKong* pTempMWhat = new CCreateKong();
		m_CCreateBaseList.AddTail(pTempMWhat);
		//////////////////////////////////////////////////////////////////////////
		CString l_strPathOfFile = Dlg.GetPathName();//�Ȼ�ȡҪ�򿪵��ļ�·��
		CString l_strFileExt = Dlg.GetFileExt();
		m_strNameFile = Dlg.GetFileTitle();
		if (l_strFileExt == "g"||l_strFileExt == "G")//�����G����
		{
			COpenGCode l_cGCode;
			//��ʼ��G�ļ���������
			if (!l_cGCode.ReadGCode(l_strPathOfFile)) 
			{
				//���Ļ���Ӣ��
				if (g_pDoc->m_stcWorkFor.IsChinese)
					AfxMessageBox("��G����������");
				else
					AfxMessageBox("Open the G code has a problem");
			}
			else
			{
//				CString l_strFileName = "PNC2000CAD--ģ��༭���                    "+Dlg.GetFileName()+"              ";			
				CString l_strFileName = "PNC2000CAD  V1.1                    "+Dlg.GetFileName()+"              ";			
				SetTitle(l_strFileName);
				m_strPathFile = l_strPathOfFile;//��·���ŵ���Ա��ʱ����ʱ��
				m_iStyleFile = 1;
			}	
		}
		else if (l_strFileExt == "dxf"||l_strFileExt == "DXF")	//�����DXF�ļ�
		{
			COpenDXFCode l_cDXFCode;
			if (!l_cDXFCode.ReadDXFCode(l_strPathOfFile))
			{
				//���Ļ���Ӣ��
				if (g_pDoc->m_stcWorkFor.IsChinese)
					AfxMessageBox("��DXF����������");
				else
					AfxMessageBox("Open the DXF code has a problem");
			}
			else
			{
//				CString l_strFileName = "PNC2000CAD--ģ��༭���                    "+Dlg.GetFileName()+"              ";			
				CString l_strFileName = "PNC2000CAD  V1.1                    "+Dlg.GetFileName()+"              ";			
				SetTitle(l_strFileName);
				m_strPathFile = l_strPathOfFile;//��·���ŵ���Ա��ʱ����ʱ��
				m_iStyleFile = 2;
			}
		}
		else if (l_strFileExt == "plt"||l_strFileExt == "PLT")	//�����plt�ļ�,�ʹ�
		{
			COpenPLTCode l_cPLTCode;	//�򿪺�ת���G����
			if (!l_cPLTCode.AnalysisPlt(l_strPathOfFile))
			{
				//���Ļ���Ӣ��
				if (g_pDoc->m_stcWorkFor.IsChinese)
					AfxMessageBox("��PLT����������");
				else
					AfxMessageBox("Open the PLT code has a problem");
			}
			else
			{
//				CString l_strFileName = "PNC2000CAD--ģ��༭���                    "+Dlg.GetFileName()+"              ";			
				CString l_strFileName = "PNC2000CAD  V1.1                    "+Dlg.GetFileName()+"              ";			
				SetTitle(l_strFileName);//�趨�ļ�·��
				m_strPathFile = l_strPathOfFile;//��·���ŵ���Ա��ʱ����ʱ��
				m_iStyleFile = 2;
			}
			//��ת����G����
			COpenGCode l_cOpenTurned;
			//��ʼ��G�ļ�����������		   //G�����ļ���·��
			if (!l_cOpenTurned.ReadGCode(l_cPLTCode.m_strOutFile)) 
			{
				//���Ļ���Ӣ��
				if (g_pDoc->m_stcWorkFor.IsChinese)
					AfxMessageBox("��G����������");
				else
					AfxMessageBox("Open the G code has a problem");
			}
		}
		else
		{
			//���Ļ���Ӣ��
			if (g_pDoc->m_stcWorkFor.IsChinese)
				AfxMessageBox("���.DXF��.G��.PLT���ļ�");			
			else
				AfxMessageBox("Please open the DXF or. G or. PLT file");			
		}
		m_bFirstOpen = TRUE;//�Ƿ��ǵ�һ�δ�
	}
}

/*
 *	����ɫ��Ӧ�ŵ�M����Ķ���Ի���İ�Ť
 */
void CCAD_PowerLandDoc::OnProperty() 
{
	// TODO: Add your command handler code here
	CDlgColorToM dlg;
	dlg.DoModal();
}

/*
 *	�򿪲���1�ĶԻ���
 */
void CCAD_PowerLandDoc::OnOpenDlgParamGlassGrind()
{
	// TODO: Add your command handler code here
	CDlgParamGlassGrind dlg;
	dlg.DoModal();
}

/*
 *	�򿪾���Ի���
 */
void CCAD_PowerLandDoc::OnOpenDlgMatrix() 
{
	// TODO: Add your command handler code here
	CDlgMatrix dlg;
	dlg.DoModal();
}

/*
 *	�򿪽��˵��Ի���
 */
void CCAD_PowerLandDoc::OnOpenDlgParamInoutKnife() 
{
	// TODO: Add your command handler code here
	CDlgParamInoutKnife dlg;
	dlg.DoModal();
}

/*
 *	�򿪹��жԻ���
 */
void CCAD_PowerLandDoc::OnOpenDlgParamOverCut() 
{
	// TODO: Add your command handler code here
	CDlgParamClothCut dlg;
	dlg.DoModal();
}

/*
 *	�򿪹��ղ����Ի���
 */
void CCAD_PowerLandDoc::OnOpenDlgWorkFor()
{
	CDlgWorkFor dlg;
	dlg.DoModal();
}

//ĥ��
void CCAD_PowerLandDoc::OnGraphForGrind()
{
	CGlassGrind GlassGrind;
	GlassGrind.GetFinishList();
}


//ĥ��
void CCAD_PowerLandDoc::OnGraphKinfe()
{
	CGraphIOKinfe knife;
	knife.PutGraphIOKife();
}

//ϳ��
void CCAD_PowerLandDoc::OnMillSide()
{
//	TransToGraphList turn;
	CDlgMillSide dlg;
	dlg.DoModal();
//	if (IDOK == dlg.DoModal()) 
//		turn.OutMillSide();//�����������ͼ��
}

//����
void CCAD_PowerLandDoc::OnGraphScale()
{
	CGraphScale Scale;
	Scale.ZoomInOut(m_stcClothCut.m_dScaleX,m_stcClothCut.m_dScaleY);
}

/*
 *	ɾ���ص�
 */
void CCAD_PowerLandDoc::OnDeleteMoreLine()
{
//	CDeleteMoreLine obj;
//	obj.DeleteMoreLine();
}

/*
 *	ƽ����
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
		//���Ļ���Ӣ��
		if (g_pDoc->m_stcWorkFor.IsChinese)
			RemindContent("\r\nû��ѡ�У�����ѡ��",2);
		else
			RemindContent("\r\nNot selected, choose again��");			
	}
}
/*
 *	��ͼ�ε����ĵ���ת
 */
void CCAD_PowerLandDoc::OnRotateCenter(double angle)
{
	CDC* pDC = g_pView->GetDC();
	Position l_posCenter;	//ѡ�񼯵İ�Χ�е�������
	int l_iSize = m_CollectOfPicked.GetSize();
	BOX2D pBox;
	//������Ķ��� 
	CUndoRedo l_UndoRedo;
	PGRAPHPOS l_pGraphPos,l_pCurGraphPos;
	PUNDOLIST pUndoList	;
	if (l_iSize)
	{
		/////////////////���ѡ�񼯵İ�Χ��//////////////////////////////
		GetCollectBox(&pBox);
		l_posCenter.x = (pBox.min[0]+pBox.max[0])/2;
		l_posCenter.y = (pBox.min[1]+pBox.max[1])/2;
		pUndoList = l_UndoRedo.NewUndoList();
		pUndoList->OperatieWhat = ID_ROTATE_180;
		pUndoList->pGraphPos = l_pCurGraphPos = l_pGraphPos = l_UndoRedo.NewGraphPos();
	}
	else
	{		
		//���Ļ���Ӣ��
		if (g_pDoc->m_stcWorkFor.IsChinese)
			RemindContent("\r\nû��ѡ�У�����ѡ��",2);
		else
			RemindContent("\r\nNot selected, choose again��");	
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
		//���������ԭͼ
		if (!m_stcWorkFor.RotateSymmetrySaveOld)
		{
			m_CCreateBaseList.RemoveAt(POS);
			l_pGraphPos->pCreateBaseposSpare2 = 1;
		}
		//�������������
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
	//�������������
	m_Undo->AddUndoList(pUndoList);	
	//ˢ��
	m_CollectOfPicked.RemoveAll();
	CRect rect;
	g_pView->GetClientRect(rect);
	g_pView->InvalidateRect(rect);
	g_pView->ReleaseDC(pDC);
}

/*
 *	��ͼ�ε����£����ң�XY,�Գ�
 */
void CCAD_PowerLandDoc::OnSymmetry(int Case)
{
	CDC* pDC = g_pView->GetDC();
	Position l_pos1,l_pos2;	//ѡ�񼯵İ�Χ�е�������
	int l_iSize = m_CollectOfPicked.GetSize();
	BOX2D pBox;
	//������Ķ��� 
	CUndoRedo l_UndoRedo;
	PGRAPHPOS l_pGraphPos,l_pCurGraphPos;
	PUNDOLIST pUndoList	;
	if (l_iSize)
	{
		/////////////////���ѡ�񼯵İ�Χ��//////////////////////////////
		GetCollectBox(&pBox);
		pUndoList = l_UndoRedo.NewUndoList();
		pUndoList->OperatieWhat = ID_ROTATE_180;
		pUndoList->pGraphPos = l_pCurGraphPos = l_pGraphPos = l_UndoRedo.NewGraphPos();
	}
	else
	{
		//���Ļ���Ӣ��
		if (g_pDoc->m_stcWorkFor.IsChinese)
			RemindContent("\r\nû��ѡ�У�����ѡ��",2);
		else
			RemindContent("\r\nNot selected, choose again��");			return;
	}
	//��ʲô��ʽ����
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
		//���������ԭͼ
		if (!m_stcWorkFor.RotateSymmetrySaveOld)
		{
			m_CCreateBaseList.RemoveAt(POS);
			l_pGraphPos->pCreateBaseposSpare2 = 1;
		}
		//�������������
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
	//�������������
	m_Undo->AddUndoList(pUndoList);	
	//ˢ��
	m_CollectOfPicked.RemoveAll();
	CRect rect;
	g_pView->GetClientRect(rect);
	g_pView->InvalidateRect(rect);
	g_pView->ReleaseDC(pDC);
}

/*
 *	�Ѷ��Ķ���߱���ٵ��
 */
void CCAD_PowerLandDoc::OnToLessPoint()
{
	CToLessPoint ToLess;
	ToLess.ToLessPoint();
}

//����
void CCAD_PowerLandDoc::OverCut()
{
	COverCut overcut;
	overcut.NeedOverCut();
}

/*
 *	�ӱ߿����
 */
void CCAD_PowerLandDoc::OnAddRect()
{
	CDlgAddRect DlgAddRect;
	DlgAddRect.DoModal();
}

//���߱��ϳ������
void CCAD_PowerLandDoc::OnTurnMill()
{
	CTurnMill TurnMill;
	TurnMill.TurnGraphMill();
}

//�ϲ�
void CCAD_PowerLandDoc::OnGraphTogether()
{
	CGraphTogether GraphTogether;
	GraphTogether.PutGraphTogether();


	//���浱ǰ��ͼ����Ϣ
	COutputGCode SaveGCode;
	SaveGCode.OutputGCode("C:\\Program Files\\CAD_PowerLand\\Together.g");
	//�������ͼԪ
	m_CCreateBaseList.RemoveAll();
	//���ѡ��ͼԪ
	m_CollectOfPicked.RemoveAll();
	CCreateKong* pTempMWhat = new CCreateKong();
	m_CCreateBaseList.AddTail(pTempMWhat);
	//�ٴ�
	COpenGCode OpenGCode;
	OpenGCode.ReadGCode("C:\\Program Files\\CAD_PowerLand\\Together.g");
}

/*
 *	�����ļ�
 */
void CCAD_PowerLandDoc::OnFileSend()
{
	if(((CMainFrame*)AfxGetMainWnd())->m_bSaveFile == FALSE)//�Ƿ���ܣ����Ա���
	{
		//���Ļ���Ӣ��
		if (g_pDoc->m_stcWorkFor.IsChinese)
			RemindContent("\r\nû��ʹ��Ȩ��",2);
		else
			RemindContent("\r\nNo access to ",2);		
		return ;
	}
	if(m_strPathFile == ""&&g_pDoc->m_stcWorkFor.bSaveDefault == 1)//�Ƿ�Ϊ���ļ�
	{
		//���Ļ���Ӣ��
		if (g_pDoc->m_stcWorkFor.IsChinese)
			RemindContent("\r\n��������Ϊ��",2);
		else
			RemindContent("\r\nsend content is empty ",2);		
		return ;
	}
	CString l_strFileName;//�ļ�������
	if(g_pDoc->m_stcWorkFor.bSaveDefault == 1)//˵���Ǳ���Ĭ�ϵ�����
	{
		if(g_pDoc->m_stcWorkFor.bSendStyleG == 1)//����g�ļ�
		{
			l_strFileName = "\\" + m_strNameFile + ".g";		
			l_strFileName = m_stcWorkFor.strFilePathG + l_strFileName;
			COutputGCode l_cGCode;
			if (!l_cGCode.OutputGCode(l_strFileName)) 
			{
				//���Ļ���Ӣ��
				if (g_pDoc->m_stcWorkFor.IsChinese)
					AfxMessageBox("����·��������");
				else
					AfxMessageBox("Save the path has a problem");
			}
			else
			{
				//����һ�����̴�����
				HWND receiveWindow = ::FindWindow(NULL,"PNC2000CT");  
				if (receiveWindow == NULL)  
				{
					//���Ļ���Ӣ��
					if (g_pDoc->m_stcWorkFor.IsChinese)
						RemindContent("\r\nû�ҵ��и����",2);
					else
						RemindContent("\r\nDidn't find the cutting software",2);
					return;
				} 	
				COPYDATASTRUCT copyData = { 0 };
				copyData.lpData = l_strFileName.GetBuffer(0);
				copyData.cbData = l_strFileName.GetLength();	
				::SendMessage(receiveWindow, WM_COPYDATA, (WPARAM)this, (LPARAM)&copyData);	
				l_strFileName.ReleaseBuffer();
				//���Ļ���Ӣ��
				if (g_pDoc->m_stcWorkFor.IsChinese)
					RemindContent("\r\n���ͳɹ�",2);
				else
					RemindContent("\r\nSend successful ",2);
				(CMainFrame*)AfxGetMainWnd()->ShowWindow(SW_MINIMIZE);
			}
			
		}
		else//����dxf�ļ�
		{
			l_strFileName = "\\" + m_strNameFile + ".dxf";		
			l_strFileName = m_stcWorkFor.strFilePathDxf + l_strFileName;
			COutputDXFCode l_cDXFCode;
			if (!l_cDXFCode.OutputDXFCode(l_strFileName)) 
			{
				//���Ļ���Ӣ��
				if (g_pDoc->m_stcWorkFor.IsChinese)
					AfxMessageBox("����·��������");
				else
					AfxMessageBox("Save the path has a problem");
			}
			else
			{
				//����һ�����̴�����
				HWND receiveWindow = ::FindWindow(NULL,"PNC2000CT");  
				if (receiveWindow == NULL)  
				{
					//���Ļ���Ӣ��
					if (g_pDoc->m_stcWorkFor.IsChinese)
						RemindContent("\r\nû�ҵ��и����",2);
					else
						RemindContent("\r\nDidn't find the cutting software",2);
					return;
				} 	
				COPYDATASTRUCT copyData = { 0 };
				copyData.lpData = l_strFileName.GetBuffer(0);
				copyData.cbData = l_strFileName.GetLength();	
				::SendMessage(receiveWindow, WM_COPYDATA, (WPARAM)this, (LPARAM)&copyData);	
				l_strFileName.ReleaseBuffer();
				//���Ļ���Ӣ��
				if (g_pDoc->m_stcWorkFor.IsChinese)
					RemindContent("\r\n���ͳɹ�",2);
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
		//���Ļ���Ӣ��
		if (g_pDoc->m_stcWorkFor.IsChinese)
			RemindContent("\r\nȡ������!",2);
		else
			RemindContent("\r\nUndo Send!",2);
		return;
	}
	if(g_pDoc->m_stcWorkFor.bSendStyleG == 1)//����g�ļ�
	{
		l_strFileName = "\\" + m_stcWorkFor.FileName + ".g";
		COutputGCode l_cGCode;
		l_strFileName = m_stcWorkFor.strFilePathG + l_strFileName;
		if (!l_cGCode.OutputGCode(l_strFileName)) 
		{
			//���Ļ���Ӣ��
			if (g_pDoc->m_stcWorkFor.IsChinese)
				AfxMessageBox("����·��������");
			else
				AfxMessageBox("Save the path has a problem");
		}
		else
		{
			//����һ�����̴�����
			HWND receiveWindow = ::FindWindow(NULL,"PNC2000CT");  
			if (receiveWindow == NULL)  
			{
				//���Ļ���Ӣ��
				if (g_pDoc->m_stcWorkFor.IsChinese)
					RemindContent("\r\nû�ҵ��и����",2);
				else
					RemindContent("\r\nDidn't find the cutting software",2);
				return;
			} 	
			COPYDATASTRUCT copyData = { 0 };
			copyData.lpData = l_strFileName.GetBuffer(0);
			copyData.cbData = l_strFileName.GetLength();	
			::SendMessage(receiveWindow, WM_COPYDATA, (WPARAM)this, (LPARAM)&copyData);	
			l_strFileName.ReleaseBuffer();
			//���Ļ���Ӣ��
			if (g_pDoc->m_stcWorkFor.IsChinese)
				RemindContent("\r\n���ͳɹ�",2);
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
			//���Ļ���Ӣ��
			if (g_pDoc->m_stcWorkFor.IsChinese)
				AfxMessageBox("����·��������");
			else
				AfxMessageBox("Save the path has a problem");
		}
		else
		{
			//����һ�����̴�����
			HWND receiveWindow = ::FindWindow(NULL,"PNC2000CT");  
			if (receiveWindow == NULL)  
			{
				//���Ļ���Ӣ��
				if (g_pDoc->m_stcWorkFor.IsChinese)
					RemindContent("\r\nû�ҵ��и����",2);
				else
					RemindContent("\r\nDidn't find the cutting software",2);
				return;
			} 	
			COPYDATASTRUCT copyData = { 0 };
			copyData.lpData = l_strFileName.GetBuffer(0);
			copyData.cbData = l_strFileName.GetLength();	
			::SendMessage(receiveWindow, WM_COPYDATA, (WPARAM)this, (LPARAM)&copyData);	
			l_strFileName.ReleaseBuffer();
			//���Ļ���Ӣ��
			if (g_pDoc->m_stcWorkFor.IsChinese)
				RemindContent("\r\n���ͳɹ�",2);
			else
				RemindContent("\r\nSend successful ",2);
			(CMainFrame*)AfxGetMainWnd()->ShowWindow(SW_MINIMIZE);
		}				
	}
}

/*
 *	������ɫ�ŷ�����ɫ��
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
 *	������ɫ�ŷ���M���롣
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
 *	��ð�Χ��
 */
BOOL CCAD_PowerLandDoc::GetCollectBox(BOX2D *pBox)
{
	int i, n = m_CollectOfPicked.GetSize();
	if(n == 0)  return FALSE;//����ǿռ�����Ҫ����
	
	BOX2D pBox1,pBox2;
	Position l_posMin,l_posMax;	//ѡ�񼯵İ�Χ�е�������
	/////////////////���ѡ�񼯵İ�Χ��//////////////////////////////
	CCreateBase*	pEntity1 = (CCreateBase*)m_CollectOfPicked[0];
	pEntity1->GetBox(&pBox1);	//1����õ�һ��ͼԪ��Χ��
	l_posMin.x = pBox1.min[0];
	l_posMin.y = pBox1.min[1];
	l_posMax.x = pBox1.max[0];
	l_posMax.y = pBox1.max[1];
	//2�������а�Χ�бȽϵ÷����հ�Χ��
	for(i=0;i<n-1;i++)
	{
		CCreateBase*	pEntity = (CCreateBase*)m_CollectOfPicked[i+1];
		pEntity->GetBox(&pBox2);
		l_posMin.x = min(l_posMin.x,pBox2.min[0]);
		l_posMin.y = min(l_posMin.y,pBox2.min[1]);
		l_posMax.x = max(l_posMax.x,pBox2.max[0]);
		l_posMax.y = max(l_posMax.y,pBox2.max[1]);		
	}
	//�����Χ��
	pBox->min[0] = l_posMin.x;
	pBox->min[1] = l_posMin.y;
	pBox->max[0] = l_posMax.x;
	pBox->max[1] = l_posMax.y;
	return TRUE;
}

/*
*	��͹�ȵĺ��� 1
*/																//Ϊ2ʱ˳ʱ�룬����Ϊ��ʱ��
double CCAD_PowerLandDoc::RtnTudu(Position begin,Position end,Position center,int clock)
{	
	//����ʼ��  	//��ĩ��  //���	
	double l_dStartAngle,l_dEndAngle,l_dAlfa;
	l_dStartAngle = GetAngleToXAxis(center,begin);
	l_dEndAngle = GetAngleToXAxis(center,end);
	if(clock == 2)		//˳ʱ��
	{
		if(l_dEndAngle<l_dStartAngle)//û��0�Ƚ�ʱ
			l_dAlfa = l_dEndAngle - l_dStartAngle;
		else//����Ƚ�ʱ
			l_dAlfa = l_dEndAngle - MATH_2PI -l_dStartAngle;
	}
	else//��ʱ��
	{
		if(l_dEndAngle>l_dStartAngle)//û��0�Ƚ�ʱ
			l_dAlfa = l_dEndAngle - l_dStartAngle;
		else//����Ƚ�ʱ
			l_dAlfa = MATH_2PI -l_dStartAngle + l_dEndAngle;
	}
	double dBulge = 0.0;	
	dBulge = tan(l_dAlfa / 4.0);	
	return dBulge;
}

//����Բ����Ϣ��Բ�ģ���������Ǵ�Dxf2GCode��̬���ﳭ�����ģ�                  ͹��
void CCAD_PowerLandDoc::polylineToArc(Position& BeginPoint, Position& EndPoint, double u, Position& CenterPoint)
{
		double centerAngle;//����
	double ttu = fabs(u);
	centerAngle=atan(ttu);
	centerAngle *=4;
	
	double x1,x2,y1,y2;//Բ����ʼ�����ֹ��
	x1=BeginPoint.x;
	x2=EndPoint.x;
	y1=BeginPoint.y;
	y2=EndPoint.y;
	
	double L; //�ҳ�
	L=sqrt(pow((x1-x2),2)+pow((y1-y2),2)); 
	
	double R;//Բ���뾶
	R=(0.5*sqrt(pow((x1-x2),2)+pow((y1-y2),2)))/(sin(0.5*centerAngle));
	
	//��֪Բ������Ͱ뾶����Բ������
	double h;//Բ�ĵ��ҵľ���
	h=sqrt(R*R-(pow((x1-x2),2)+pow((y1-y2),2))/4);
	
//	double k;//��ʼ�����ֹ�����ߵ��д���б��
	double xc,yc;//Բ������
	double xa,ya; //��ʼ�����ֹ�����ߵ��е��������
	xa=0.5*(x1+x2);
	ya=0.5*(y1+y2);
	
	//�ҵķ���ǣ�0-2PI֮��
	
	double angle;//��㵽�յ����������x������֮�����б��
	angle=acos((x2-x1)/sqrt(pow(x2-x1,2)+pow(y2-y1,2)));
	
	double amass; //��������X������λ�����Ĳ��
	amass = y1-y2;//�ɣ���(x2-x1)*0-1*(y2-y1)���õ�
	const double eps=0.000000000001; 	
	if (amass>eps)
	{  
		angle = -angle;
		angle=MATH_2PI+angle;
	}

	double DirectionAngel;//���е㵽Բ�ĵ�ֱ�������ķ����
	if ((u>0 && centerAngle<MATH_PI)||(u<0 && centerAngle>MATH_PI))
		DirectionAngel=angle+MATH_PI/2;
	if((u<0 && centerAngle<MATH_PI)||(u>0 && centerAngle>MATH_PI))
		DirectionAngel=angle-MATH_PI/2;
//ȷ�ϽǶ��ڣ�0-2PI֮�䣩2014-10-9
	if (DirectionAngel<0)
        DirectionAngel= DirectionAngel+2*MATH_PI;
	if (DirectionAngel>MATH_2PI)
		DirectionAngel= DirectionAngel-MATH_2PI;
	
	double d;//Բ�ĵ��ҵľ���
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
		double nslope,k;//nslope Ϊ�ҵ�б�ʣ�KΪ���д��ߵ�б��
		double nAngle;//�д��ߵ���б�ǣ�
		double X,Y; //Բ������������ĵ������ƫ����
		
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
 *	������ֵ���
 */
void CCAD_PowerLandDoc::PosBeEqualed(CADPOINT &pos1,Position pos2)
{
	pos1.x = pos2.x;
	pos1.y = pos2.y;
}

/*
 *	������ֵ���
 */
void CCAD_PowerLandDoc::PosBeEqualed(Position &pos1,CADPOINT pos2)
{
	pos1.x = pos2.x;
	pos1.y = pos2.y;
}

/*
 *	����ѡ�е�������
 */
int CCAD_PowerLandDoc::NumPicked(Position pos,long& createbase)
{
	POSITION	POS = g_pDoc->m_CCreateBaseList.GetHeadPosition();	//���ͷָ�룬������ѭ�����á�
	int l_iNumPick = -1;
	CPoint l_pot;
	g_pView->WorldToScreem(pos,l_pot);
	CCreateBase *pCreate;
	while(POS != NULL)//����ɨ������
	{
		pCreate = (CCreateBase *)g_pDoc->m_CCreateBaseList.GetNext(POS);

		l_iNumPick = pCreate->IsPick(l_pot);
		// ����������ͼԪ��ѡ��
		if( l_iNumPick != -1)
		{
			createbase = (long)pCreate;
			return l_iNumPick;
		}
	}
	return -1;
}

/*
 *	��ͼ����ʾ��ȫ��
 */
void CCAD_PowerLandDoc::OnFullScreemShow()
{
	int n = m_CCreateBaseList.GetCount();
	if(n == 1)  return;//����ǿռ�����Ҫ����
	BOX2D pBox1={0},pBox2={0};
	Position l_posMin,l_posMax;	//ѡ�񼯵İ�Χ�е�������
	/////////////////���ѡ�񼯵İ�Χ��//////////////////////////////
	POSITION POS = m_CCreateBaseList.GetHeadPosition();
	CCreateBase*	pEntity1;
	while(POS)
	{
		pEntity1 = (CCreateBase*)m_CCreateBaseList.GetNext(POS);
		if(pEntity1->GetBox(&pBox1))	//1����õ�һ��ͼԪ��Χ��
			break;
	}
	l_posMin.x = pBox1.min[0];
	l_posMin.y = pBox1.min[1];
	l_posMax.x = pBox1.max[0];
	l_posMax.y = pBox1.max[1];
	//2�������а�Χ�бȽϵ÷����հ�Χ��
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
	//��Ӧ���ƶ�������
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
 *	��ͼ���ƶ���ԭ��
 */
void CCAD_PowerLandDoc::OnMoveOrigin()
{
	int l_iSize = m_CollectOfPicked.GetSize();
	if(l_iSize < 1)  return;//����ǿռ�����Ҫ����
	BOX2D pBox1,pBox2;
	Position l_posMin,l_posMax;	//ѡ�񼯵İ�Χ�е�������
	/////////////////���ѡ�񼯵İ�Χ��//////////////////////////////
	CCreateBase*	pEntity1;
	for( int i = 0 ; i < l_iSize; i++ )
	{
		pEntity1 = (CCreateBase*) m_CollectOfPicked[i] ;
		if(pEntity1->GetBox(&pBox1))	//1����õ�һ��ͼԪ��Χ��
			break;
	}
	l_posMin.x = pBox1.min[0];
	l_posMin.y = pBox1.min[1];
	l_posMax.x = pBox1.max[0];
	l_posMax.y = pBox1.max[1];
	//2�������а�Χ�бȽϵ÷����հ�Χ��
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
	//��Ӧ���ƶ�������
	Position l_posBase,l_posDes,l_posDisOrigin(g_pDoc->m_stcClothCut.dMoveOriginY,g_pDoc->m_stcClothCut.dMoveOriginX);
	l_posDes = (l_posMax-l_posMin)/2+l_posDisOrigin;
	l_posBase = (l_posMax+l_posMin)/2;
	//��������
	CUndoRedo l_UndoRedo;//������Ķ��� 
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
	g_pDoc->m_Undo->AddUndoList(pUndoList);//��ӵ�����
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
	char szFilter[] = "G�����ļ�(*.G)|*.g|DXF�ļ�(*.DXF)|*.dxf||";		//�����ʽ
	CFileDialog Dlg(FALSE, NULL, NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);//��һ���ļ��Ի���
	Dlg.m_ofn.lpstrTitle = "���Ϊ";
	if (Dlg.DoModal() == IDOK)	//������ȷ��
	{
		m_strNameFile = Dlg.GetFileTitle();		
		CString l_strPathOfFile;//�ļ�·��
		if(Dlg.m_ofn.nFilterIndex == 1)	//��������Ϊ��һ��.G��ʽ��
		{
			l_strPathOfFile = Dlg.GetPathName();//�Ȼ�ȡҪ������ļ�·��
			if (Dlg.GetFileExt() != "G"&&Dlg.GetFileExt() != "g"&&Dlg.GetFileExt() != "DXF"&&Dlg.GetFileExt() != "dxf")		//�ļ�û����չ��ʱ������.g�ĸ�ʽ
				l_strPathOfFile += ".g";
			if (!l_cGCode.OutputGCode(l_strPathOfFile)) 
			{
				//���Ļ���Ӣ��
				if (g_pDoc->m_stcWorkFor.IsChinese)
					AfxMessageBox("������");			
				else
					AfxMessageBox("something the matter");	
			}
			else
			{
				CString l_strFileName = "PNC2000CAD  V1.1                    "+m_strNameFile+".g"+"              ";			
				SetTitle(l_strFileName);				
				m_strPathFile = l_strPathOfFile;//��·���ŵ���Ա��ʱ����ʱ��
				m_iStyleFile = 1;	
			}
		}
		else if(Dlg.m_ofn.nFilterIndex == 2)	//��������Ϊ�ڶ���.DXF��ʽ��
		{
			l_strPathOfFile = Dlg.GetPathName();//�Ȼ�ȡҪ������ļ�·��
			if (Dlg.GetFileExt() != "G"&&Dlg.GetFileExt() != "g"&&Dlg.GetFileExt() != "DXF"&&Dlg.GetFileExt() != "dxf")		//�ļ�û����չ��ʱ������.g�ĸ�ʽ
				l_strPathOfFile += ".dxf";
			if (!l_cDXFCode.OutputDXFCode(l_strPathOfFile)) 
			{
				//���Ļ���Ӣ��
				if (g_pDoc->m_stcWorkFor.IsChinese)
					AfxMessageBox("������");			
				else
					AfxMessageBox("something the matter");	
			}
			else
			{
				CString l_strFileName = "PNC2000CAD  V1.1                    "+m_strNameFile+".dxf"+"              ";			
				SetTitle(l_strFileName);
				m_strPathFile = l_strPathOfFile;//��·���ŵ���Ա��ʱ����ʱ��
				m_iStyleFile = 2;	
			}			
		}
		m_bFirstOpen = FALSE;//�Ƿ��ǵ�һ�δ�
	}
	else
		return FALSE;
	return TRUE;
}

/*
 *	���ĵ��д�����
 */
void CCAD_PowerLandDoc::InitDate()
{
	//���趨G������Mֵ�ĶԻ��������
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
	//���в����еĹ��еĶԻ��������
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
	
	//��ĥ�����ķ����ĶԻ��������
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
	
	////�����˵��ĶԻ��������
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
	//���ղ�����
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
 * ��M�ַ����е�M�ַ��滻
 */
BOOL CCAD_PowerLandDoc::ReplaceMCode(int color,CArray<class CString,class CString &>& aryMSB,CString strMSB, CArray<class CString,class CString &>& aryMCB,CString strMCB)
{
	//		��������Ӧ��M����
	CString l_strStockMSB,l_strStockMCB;
	//����ɫ��ӦM����
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
	//��־�������ܶ�Ӧ����
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
 *	�˳�������
 */
BOOL CCAD_PowerLandDoc::OnCloseSaveFile()
{
	if(OnFileSave())
		return TRUE;
	return FALSE;
}

/*
 *	����㷵���Ƿ�ѡ��ͼ�Σ�	�������			���ص�ͼ��			ͼ���ϵĵ�
 */
BOOL CCAD_PowerLandDoc::ClickCreateBase(Position posClick,long& pCreateBase,int& iPick)
{
	//��Ļ���ĵ�
	CPoint point;
	g_pView->WorldToScreem(posClick,point);
	POSITION	POS = g_pDoc->m_CCreateBaseList.GetHeadPosition();	//���ͷָ�룬������ѭ�����á�
	CCreateBase* l_pCreateBase = NULL;
	//�鿴�Ƿ�ѡ������
	while(POS != NULL)//����ɨ������
	{
		l_pCreateBase = (CCreateBase *)g_pDoc->m_CCreateBaseList.GetNext(POS);
		// ����������ͼԪ��ѡ��
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
 *	����ͼ������1ֱ�ߣ�2����ߣ�3���Σ�4Բ����5Բ��6��Բ
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
 *	���߱������
 */
void CCAD_PowerLandDoc::OnSolidToDash()
{
	CCreateBase* pCreateBase = NULL;
	//����������
	CUndoRedo l_cUndo;
	//ʵ�ߺͿհ׵ĳ���
	double l_dLongSolid = g_pDoc->m_stcClothCut.dEditDashSolid;
	double l_dLongEmpty = g_pDoc->m_stcClothCut.dEditDashEmpty;
	int l_iSize = m_CollectOfPicked.GetSize();
	//������
	for (int i=0;i<l_iSize;i++)
	{
		PUNDOLIST l_pUndolist = l_cUndo.NewUndoList();
		l_pUndolist->OperatieWhat = ID_SOLID_TO_DASH;
		l_pUndolist->pGraphPos = l_cUndo.NewGraphPos();
		//������
		pCreateBase = (CCreateBase*)m_CollectOfPicked.GetAt(i);
		PGRAPHPOS headGraphDash = pCreateBase->SolidTurnDash(l_dLongSolid,l_dLongEmpty,l_pUndolist);
		m_CCreateBaseList.RemoveAt(m_CCreateBaseList.Find(pCreateBase));
		//��ͼ�α��浽����������
		l_pUndolist->pGraphPos->pCreateBasepos = (long)pCreateBase;
		l_pUndolist->pGraphPosSpare = headGraphDash;
		g_pDoc->m_Undo->AddUndoList(l_pUndolist);
	}
	m_CollectOfPicked.RemoveAll();
	g_pView->Invalidate();
}

/*
 *	��������ͼ������PGRAPHPOS
 */
void CCAD_PowerLandDoc::AddTail(PGRAPHPOS headGraphPos,PGRAPHPOS tailGraphPos)
{
	while (headGraphPos->pNext != NULL)
		headGraphPos = headGraphPos->pNext;//ѭ����β
	headGraphPos->pNext = tailGraphPos;
	tailGraphPos->pPrev = headGraphPos;	
}



















