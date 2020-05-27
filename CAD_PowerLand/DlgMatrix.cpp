// DlgMatrix.cpp : implementation file
//

#include "stdafx.h"
#include "cad_powerland.h"
#include "DlgMatrix.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgMatrix dialog


CDlgMatrix::CDlgMatrix(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMatrix::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgMatrix)
	m_EditHorNum = 3;
	m_EditVerNum = 3;
	m_dEditHorOfst = 0.0;
	m_dEditVerOfst = 0.0;
	m_dEditCreateLong = 0.0;
	m_dEditCreateWide = 0.0;
	m_checkRotateMove = FALSE;
	m_dEditYouHuaXMove = 0.0;
	m_dEditYouHuaYMove = 0.0;
	//}}AFX_DATA_INIT
}


void CDlgMatrix::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgMatrix)
	DDX_Text(pDX, IDC_EDIT_HORIZONTAL, m_EditHorNum);
	DDV_MinMaxInt(pDX, m_EditHorNum, 1, 1000);
	DDX_Text(pDX, IDC_EDIT_VERITCAL, m_EditVerNum);
	DDV_MinMaxInt(pDX, m_EditVerNum, 1, 1000);
	DDX_Text(pDX, IDC_EDIT_HORIZONTAL_OFFSET, m_dEditHorOfst);
	DDX_Text(pDX, IDC_EDIT_VERITCAL_OFFSET, m_dEditVerOfst);
	DDX_Text(pDX, IDC_EDIT_CREATE_LONG, m_dEditCreateLong);
	DDX_Text(pDX, IDC_EDIT_CREATE_WIDE, m_dEditCreateWide);
	DDX_Check(pDX, IDC_CHECK_ROTATE_MOVE, m_checkRotateMove);
	DDX_Text(pDX, IDC_EDIT_YOUHUA_XMOVE, m_dEditYouHuaXMove);
	DDX_Text(pDX, IDC_EDIT_YOUHUA_YMOVE, m_dEditYouHuaYMove);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgMatrix, CDialog)
	//{{AFX_MSG_MAP(CDlgMatrix)
	ON_BN_CLICKED(IDC_BTN_MATRIX_OK, OnBtnMatrixOk)
	ON_BN_CLICKED(IDC_BTN_MATRIX_NO, OnBtnMatrixNo)
	ON_EN_KILLFOCUS(IDC_EDIT_HORIZONTAL, OnKillfocusEditHorizontal)
	ON_EN_KILLFOCUS(IDC_EDIT_VERITCAL, OnKillfocusEditVeritcal)
	ON_EN_KILLFOCUS(IDC_EDIT_HORIZONTAL_OFFSET, OnKillfocusEditHorizontalOffset)
	ON_EN_KILLFOCUS(IDC_EDIT_VERITCAL_OFFSET, OnKillfocusEditVeritcalOffset)
	ON_BN_CLICKED(IDC_BTN_GET_LONG_WIDE, OnBtnGetLongWide)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMatrix message handlers
/*
 *	ȷ������
 */
void CDlgMatrix::OnBtnMatrixOk() 
{
	// TODO: Add your control notification handler code here
	if(!UpdateData())//�ѱ༭��������ݴ����������
		return;
	//ѡ�񼯵Ĵ�С
	int l_iSizeOfCollect = g_pDoc->m_CollectOfPicked.GetSize();
	if(l_iSizeOfCollect == 0)//����ǿյĻ�
	{
		//���Ļ���Ӣ��
		if (g_pDoc->m_stcWorkFor.IsChinese)
			AfxMessageBox("û��ѡ��ͼ��");
		else
			AfxMessageBox("No choose the graphics");
		return;
	}
	//Ҫ�Ǻ�������1�Ļ�û�б�
	if(m_EditHorNum == 1 && m_EditVerNum == 1)
	{
		CDlgMatrix::OnOK();
		//���Ļ���Ӣ��
		if (g_pDoc->m_stcWorkFor.IsChinese)
			AfxMessageBox("�ޱ仯��");
		else
			AfxMessageBox("No change!");
		return;
	}
	////////////////����ɶ��
	CDC*	pDC = g_pView->GetDC(); // ���������豸����ָ��

	//��ӳ�������
	m_pUndoList = m_UndoRedo.NewUndoList();
	m_pUndoList->OperatieWhat = ID_LINE;	
	m_pheadGraphPos = m_pCurGraphPos = m_UndoRedo.NewGraphPos();
	m_pUndoList->pGraphPos = m_pheadGraphPos;

	//������Ż�����Ļ�
	if(m_checkRotateMove == 1)
	{
		//�õ���Χ�����ĵ�
		Position l_posCenterBox(0,0),l_posBase(0,0),l_posMove(m_dEditYouHuaXMove,m_dEditYouHuaYMove);
		BOX2D pBox;
		g_pDoc->GetCollectBox(&pBox);
		l_posCenterBox.x = (pBox.min[0]+pBox.max[0])/2;
		l_posCenterBox.y = (pBox.min[1]+pBox.max[1])/2;
		//��ѡ�񼯸�����ת�ƶ�
		for(int i=0;i<l_iSizeOfCollect;i++)
		{
			CCreateBase*	pEntity = (CCreateBase*)g_pDoc->m_CollectOfPicked[i];
			CCreateBase* pCopy = pEntity->Copy();	//����һ���µ�Ҫ���Ƶ�ͼ�� 
			pCopy->Rotate(l_posCenterBox,MATH_PI);
			pCopy->Move(l_posBase,l_posMove);
			pCopy->DrawMove(pDC,dmNormal); // ��Ŀ��λ���ϻ���ͼԪ
			g_pDoc->m_CCreateBaseList.AddTail(pCopy);//���ƺ��ͼԪ�ŵ�������
			//�������������Ԫ��
			if (m_pheadGraphPos->pCreateBasepos == NULL)	
				m_pCurGraphPos->pCreateBasepos = (long)pCopy;
			else
			{
				m_pGraphPos = m_UndoRedo.NewGraphPos();						
				m_pGraphPos->pCreateBasepos = (long)pCopy;
				m_pCurGraphPos->pNext = m_pGraphPos;
				m_pGraphPos->pPrev = m_pCurGraphPos;
				m_pCurGraphPos = m_pGraphPos;
			}
			g_pDoc->m_CollectOfPicked.Add(pCopy);
		}
	}
	//������ͼԪ
	DrawMatrix(pDC);
	g_pDoc->m_CollectOfPicked.RemoveAll(); //  ���ѡ��
	g_pView->ReleaseDC(pDC); // �ͷ�������豸����ָ��
	//������ͼԪ//��Ҫ����ͼԪɾ��
	delete g_pView->m_pDrawBase;
	g_pView->m_pDrawBase = NULL;	//��Ϊ�գ����´ε�����ʱ���������Ƿ��ѡ�С�
	g_pView->Invalidate(TRUE);	
	CDlgMatrix::OnOK();
}

/*
 *  �������ͼԪ
 */
void CDlgMatrix::DrawMatrix(CDC* pDC)
{
	Position l_posBase(0,0),l_posDesHor(0,0),l_posDesVer(0,0);
	int i, n = g_pDoc->m_CollectOfPicked.GetSize(), h, v;
	//ɨ��ѡ�е�����׼��������
	for(h = 0;h<m_EditHorNum-1;h++)
	{
		for(v = 0;v<m_EditVerNum-1;v++)//�ȸ�������
		{
			l_posDesVer.y = l_posDesVer.y + m_dWide + m_dEditVerOfst;//ÿ�ζ��������ŵ�ƫ�õ�ֵ
			for(i = 0; i < n; i++)//
			{//�õ�ѡ�е�ͼԪ
				CCreateBase*	pEntity = (CCreateBase*)g_pDoc->m_CollectOfPicked[i];
				CCreateBase* pCopy = pEntity->Copy();	//����һ���µ�Ҫ���Ƶ�ͼ�� 
				pCopy->Move(l_posBase, l_posDesVer);
				pCopy->DrawMove(pDC,dmNormal); // ��Ŀ��λ���ϻ���ͼԪ
				g_pDoc->m_CCreateBaseList.AddTail(pCopy);//���ƺ��ͼԪ�ŵ�������
				//�������������Ԫ��
				if (m_pheadGraphPos->pCreateBasepos == NULL)	
					m_pCurGraphPos->pCreateBasepos = (long)pCopy;
				else
				{
					m_pGraphPos = m_UndoRedo.NewGraphPos();						
					m_pGraphPos->pCreateBasepos = (long)pCopy;
					m_pCurGraphPos->pNext = m_pGraphPos;
					m_pGraphPos->pPrev = m_pCurGraphPos;
					m_pCurGraphPos = m_pGraphPos;
				}
			}
		}	
		//�ӵڶ������е�ͷ
		l_posDesHor.x =l_posDesHor.x + m_dLong + m_dEditHorOfst;
		for(i = 0; i < n; i++)//
		{//�õ�ѡ�е�ͼԪ
			CCreateBase*	pEntity = (CCreateBase*)g_pDoc->m_CollectOfPicked[i];
			CCreateBase* pCopy = pEntity->Copy();	//����һ���µ�Ҫ���Ƶ�ͼ�� 
			pCopy->Move(l_posBase, l_posDesHor);
			pCopy->DrawMove(pDC,dmNormal); // ��Ŀ��λ���ϻ���ͼԪ
			g_pDoc->m_CCreateBaseList.AddTail(pCopy);//���ƺ��ͼԪ�ŵ�������
			//�������������Ԫ��
			if (m_pheadGraphPos->pCreateBasepos == NULL)	
				m_pCurGraphPos->pCreateBasepos = (long)pCopy;
			else
			{
				m_pGraphPos = m_UndoRedo.NewGraphPos();						
				m_pGraphPos->pCreateBasepos = (long)pCopy;
				m_pCurGraphPos->pNext = m_pGraphPos;
				m_pGraphPos->pPrev = m_pCurGraphPos;
				m_pCurGraphPos = m_pGraphPos;
			}
			
		}
		//�����趨���е�ƫ�õ�ֵ
		l_posDesVer.x =l_posDesVer.x + m_dLong +  m_dEditHorOfst;
		l_posDesVer.y = 0;
		//����һ��
		if(h == m_EditHorNum-2)
		{
			for(v = 0;v<m_EditVerNum-1;v++)
			{
				l_posDesVer.y =l_posDesVer.y + m_dWide + m_dEditVerOfst;//ÿ�ζ��������ŵ�ƫ�õ�ֵ
				for(i = 0; i < n; i++)//
				{//�õ�ѡ�е�ͼԪ
					CCreateBase*	pEntity = (CCreateBase*)g_pDoc->m_CollectOfPicked[i];
					CCreateBase* pCopy = pEntity->Copy();	//����һ���µ�Ҫ���Ƶ�ͼ�� 
					pCopy->Move(l_posBase, l_posDesVer);
					pCopy->DrawMove(pDC,dmNormal); // ��Ŀ��λ���ϻ���ͼԪ
					g_pDoc->m_CCreateBaseList.AddTail(pCopy);//���ƺ��ͼԪ�ŵ�������
					//�������������Ԫ��
					if (m_pheadGraphPos->pCreateBasepos == NULL)	
						m_pCurGraphPos->pCreateBasepos = (long)pCopy;
					else
					{
						m_pGraphPos = m_UndoRedo.NewGraphPos();						
						m_pGraphPos->pCreateBasepos = (long)pCopy;
						m_pCurGraphPos->pNext = m_pGraphPos;
						m_pGraphPos->pPrev = m_pCurGraphPos;
						m_pCurGraphPos = m_pGraphPos;
					}
					
				}
			}
			break;
		}
	}
	//����Щ�ṹ�ŵ�������
	g_pDoc->m_Undo->AddUndoList(m_pUndoList);		
}


/*
 *	ȡ���رնԻ���
 */
void CDlgMatrix::OnBtnMatrixNo() 
{
	// TODO: Add your control notification handler code here
	CDlgMatrix::OnCancel();
}

BOOL CDlgMatrix::OnInitDialog() 
{
	CDialog::OnInitDialog();
	//���Ļ���Ӣ��
	if (g_pDoc->m_stcWorkFor.IsChinese)
		SetWindowText("�������в���");
	else
		SetWindowText("Set the array parameter");		
	OnBtnGetLongWide() ;//�ڶԻ��򵯳�ʱ�趨����ľ���

	// TODO: Add extra initialization here
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



void CDlgMatrix::OnKillfocusEditHorizontal() 
{
	// TODO: Add your control notification handler code here
	UpdateData();//�鿴д��ĸ�ʽ�Բ���
}

void CDlgMatrix::OnKillfocusEditVeritcal() 
{
	// TODO: Add your control notification handler code here
	UpdateData();//�鿴д��ĸ�ʽ�Բ���
}

void CDlgMatrix::OnKillfocusEditHorizontalOffset() 
{
	// TODO: Add your control notification handler code here
	UpdateData();//�鿴д��ĸ�ʽ�Բ���
}

void CDlgMatrix::OnKillfocusEditVeritcalOffset() 
{
	// TODO: Add your control notification handler code here
	UpdateData();//�鿴д��ĸ�ʽ�Բ���
}

/*
 *	��ʼ����ִ�У��趨ͼ�εĳ���
 */
void CDlgMatrix::OnBtnGetLongWide() 
{
	// TODO: Add your control notification handler code here
	if(g_pDoc->m_CollectOfPicked.GetSize() == 0)  return;//����ǿռ�����Ҫ����
	
	BOX2D pBox;
	Position l_posMin,l_posMax;	//ѡ�񼯵İ�Χ�е�������
	/////////////////���ѡ�񼯵İ�Χ��//////////////////////////////
	g_pDoc->GetCollectBox(&pBox);
	l_posMin.x = pBox.min[0];
	l_posMin.y = pBox.min[1];
	l_posMax.x = pBox.max[0];
	l_posMax.y = pBox.max[1];
	//�Ѱ�Χ���趨���༭����
	CString str;
	m_dLong = l_posMax.x - l_posMin.x;
	str.Format("%.4f",m_dLong);
	SetDlgItemText(IDC_EDIT_CREATE_LONG,str);
	m_dWide = l_posMax.y - l_posMin.y;
	str.Format("%.4f",m_dWide);
	SetDlgItemText(IDC_EDIT_CREATE_WIDE,str);
}

