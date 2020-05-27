// GraphTogether.h: interface for the CGraphTogether class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRAPHTOGETHER_H__5D9F05C7_809E_442B_BA85_9C87F16CB6D6__INCLUDED_)
#define AFX_GRAPHTOGETHER_H__5D9F05C7_809E_442B_BA85_9C87F16CB6D6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CGraphTogether  //把连续的图形合并起来的类
{
public:
	CGraphTogether();
	virtual ~CGraphTogether();
	//使单位图形合并
	void PutGraphTogether();



private:


/////////////////////////函数////////////////////////////////////////////

	//把所有的图形链表复制一下
	void CopyList();
	//清空基础链表
	void ClearBaseList();
	//改变切割方向
	void Changedir() ;
///////////////////变量//////////////////////////////////////////////////
	
	//			起点	终点
	Position m_PosBegin,m_PosEnd;
	//			复制链表
	CObList m_cpyBaseList;
	//要改变方向的图形
	CCreateBase *m_ChangeDir;
};

#endif // !defined(AFX_GRAPHTOGETHER_H__5D9F05C7_809E_442B_BA85_9C87F16CB6D6__INCLUDED_)
