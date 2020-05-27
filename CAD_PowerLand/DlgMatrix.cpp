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
 *	确定矩阵
 */
void CDlgMatrix::OnBtnMatrixOk() 
{
	// TODO: Add your control notification handler code here
	if(!UpdateData())//把编辑框里的数据传下来并检查
		return;
	//选择集的大小
	int l_iSizeOfCollect = g_pDoc->m_CollectOfPicked.GetSize();
	if(l_iSizeOfCollect == 0)//如果是空的话
	{
		//中文还是英文
		if (g_pDoc->m_stcWorkFor.IsChinese)
			AfxMessageBox("没有选择图形");
		else
			AfxMessageBox("No choose the graphics");
		return;
	}
	//要是横竖都是1的话没有变
	if(m_EditHorNum == 1 && m_EditVerNum == 1)
	{
		CDlgMatrix::OnOK();
		//中文还是英文
		if (g_pDoc->m_stcWorkFor.IsChinese)
			AfxMessageBox("无变化！");
		else
			AfxMessageBox("No change!");
		return;
	}
	////////////////矩阵成多个
	CDC*	pDC = g_pView->GetDC(); // 获得视类的设备环境指针

	//添加撤消操作
	m_pUndoList = m_UndoRedo.NewUndoList();
	m_pUndoList->OperatieWhat = ID_LINE;	
	m_pheadGraphPos = m_pCurGraphPos = m_UndoRedo.NewGraphPos();
	m_pUndoList->pGraphPos = m_pheadGraphPos;

	//如果是优化矩阵的话
	if(m_checkRotateMove == 1)
	{
		//得到包围盒中心点
		Position l_posCenterBox(0,0),l_posBase(0,0),l_posMove(m_dEditYouHuaXMove,m_dEditYouHuaYMove);
		BOX2D pBox;
		g_pDoc->GetCollectBox(&pBox);
		l_posCenterBox.x = (pBox.min[0]+pBox.max[0])/2;
		l_posCenterBox.y = (pBox.min[1]+pBox.max[1])/2;
		//把选择集复制旋转移动
		for(int i=0;i<l_iSizeOfCollect;i++)
		{
			CCreateBase*	pEntity = (CCreateBase*)g_pDoc->m_CollectOfPicked[i];
			CCreateBase* pCopy = pEntity->Copy();	//创建一个新的要复制的图形 
			pCopy->Rotate(l_posCenterBox,MATH_PI);
			pCopy->Move(l_posBase,l_posMove);
			pCopy->DrawMove(pDC,dmNormal); // 在目标位置上绘制图元
			g_pDoc->m_CCreateBaseList.AddTail(pCopy);//复制后的图元放到链表里
			//向撤消链表里添加元素
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
	//画矩阵图元
	DrawMatrix(pDC);
	g_pDoc->m_CollectOfPicked.RemoveAll(); //  清空选择集
	g_pView->ReleaseDC(pDC); // 释放视类的设备环境指针
	//结束画图元//把要画的图元删除
	delete g_pView->m_pDrawBase;
	g_pView->m_pDrawBase = NULL;	//设为空，当下次点击鼠标时，会跳到是否会选中。
	g_pView->Invalidate(TRUE);	
	CDlgMatrix::OnOK();
}

/*
 *  画矩阵的图元
 */
void CDlgMatrix::DrawMatrix(CDC* pDC)
{
	Position l_posBase(0,0),l_posDesHor(0,0),l_posDesVer(0,0);
	int i, n = g_pDoc->m_CollectOfPicked.GetSize(), h, v;
	//扫描选中的链表，准备矩阵复制
	for(h = 0;h<m_EditHorNum-1;h++)
	{
		for(v = 0;v<m_EditVerNum-1;v++)//先复制竖的
		{
			l_posDesVer.y = l_posDesVer.y + m_dWide + m_dEditVerOfst;//每次都加上竖着的偏置的值
			for(i = 0; i < n; i++)//
			{//得到选中的图元
				CCreateBase*	pEntity = (CCreateBase*)g_pDoc->m_CollectOfPicked[i];
				CCreateBase* pCopy = pEntity->Copy();	//创建一个新的要复制的图形 
				pCopy->Move(l_posBase, l_posDesVer);
				pCopy->DrawMove(pDC,dmNormal); // 在目标位置上绘制图元
				g_pDoc->m_CCreateBaseList.AddTail(pCopy);//复制后的图元放到链表里
				//向撤消链表里添加元素
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
		//加第二个竖列的头
		l_posDesHor.x =l_posDesHor.x + m_dLong + m_dEditHorOfst;
		for(i = 0; i < n; i++)//
		{//得到选中的图元
			CCreateBase*	pEntity = (CCreateBase*)g_pDoc->m_CollectOfPicked[i];
			CCreateBase* pCopy = pEntity->Copy();	//创建一个新的要复制的图形 
			pCopy->Move(l_posBase, l_posDesHor);
			pCopy->DrawMove(pDC,dmNormal); // 在目标位置上绘制图元
			g_pDoc->m_CCreateBaseList.AddTail(pCopy);//复制后的图元放到链表里
			//向撤消链表里添加元素
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
		//重新设定竖列的偏置的值
		l_posDesVer.x =l_posDesVer.x + m_dLong +  m_dEditHorOfst;
		l_posDesVer.y = 0;
		//最后和一列
		if(h == m_EditHorNum-2)
		{
			for(v = 0;v<m_EditVerNum-1;v++)
			{
				l_posDesVer.y =l_posDesVer.y + m_dWide + m_dEditVerOfst;//每次都加上竖着的偏置的值
				for(i = 0; i < n; i++)//
				{//得到选中的图元
					CCreateBase*	pEntity = (CCreateBase*)g_pDoc->m_CollectOfPicked[i];
					CCreateBase* pCopy = pEntity->Copy();	//创建一个新的要复制的图形 
					pCopy->Move(l_posBase, l_posDesVer);
					pCopy->DrawMove(pDC,dmNormal); // 在目标位置上绘制图元
					g_pDoc->m_CCreateBaseList.AddTail(pCopy);//复制后的图元放到链表里
					//向撤消链表里添加元素
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
	//把这些结构放到链表里
	g_pDoc->m_Undo->AddUndoList(m_pUndoList);		
}


/*
 *	取消关闭对话框
 */
void CDlgMatrix::OnBtnMatrixNo() 
{
	// TODO: Add your control notification handler code here
	CDlgMatrix::OnCancel();
}

BOOL CDlgMatrix::OnInitDialog() 
{
	CDialog::OnInitDialog();
	//中文还是英文
	if (g_pDoc->m_stcWorkFor.IsChinese)
		SetWindowText("设置阵列参数");
	else
		SetWindowText("Set the array parameter");		
	OnBtnGetLongWide() ;//在对话框弹出时设定矩阵的距离

	// TODO: Add extra initialization here
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



void CDlgMatrix::OnKillfocusEditHorizontal() 
{
	// TODO: Add your control notification handler code here
	UpdateData();//查看写入的格式对不对
}

void CDlgMatrix::OnKillfocusEditVeritcal() 
{
	// TODO: Add your control notification handler code here
	UpdateData();//查看写入的格式对不对
}

void CDlgMatrix::OnKillfocusEditHorizontalOffset() 
{
	// TODO: Add your control notification handler code here
	UpdateData();//查看写入的格式对不对
}

void CDlgMatrix::OnKillfocusEditVeritcalOffset() 
{
	// TODO: Add your control notification handler code here
	UpdateData();//查看写入的格式对不对
}

/*
 *	初始化里执行，设定图形的长宽
 */
void CDlgMatrix::OnBtnGetLongWide() 
{
	// TODO: Add your control notification handler code here
	if(g_pDoc->m_CollectOfPicked.GetSize() == 0)  return;//如果是空集，就要返回
	
	BOX2D pBox;
	Position l_posMin,l_posMax;	//选择集的包围盒的两个点
	/////////////////获得选择集的包围盒//////////////////////////////
	g_pDoc->GetCollectBox(&pBox);
	l_posMin.x = pBox.min[0];
	l_posMin.y = pBox.min[1];
	l_posMax.x = pBox.max[0];
	l_posMax.y = pBox.max[1];
	//把包围盒设定到编辑框里
	CString str;
	m_dLong = l_posMax.x - l_posMin.x;
	str.Format("%.4f",m_dLong);
	SetDlgItemText(IDC_EDIT_CREATE_LONG,str);
	m_dWide = l_posMax.y - l_posMin.y;
	str.Format("%.4f",m_dWide);
	SetDlgItemText(IDC_EDIT_CREATE_WIDE,str);
}

