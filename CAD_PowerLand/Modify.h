// Modify.h: interface for the CModifyMove class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MODIFY_H__A1E3C4DE_FD59_4310_B646_BC5F14069EC3__INCLUDED_)
#define AFX_MODIFY_H__A1E3C4DE_FD59_4310_B646_BC5F14069EC3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CModifyMove  :public CDrawBase
{
public:
	CModifyMove();
	virtual ~CModifyMove();
	int OnLButtonDown(UINT nFlags,const Position &pos,CString cmd);
	int OnMouseMove(UINT nFlags,const Position &pos);
	int OnRButtonDown(UINT nFlags,const Position &pos);

private:
	Position m_basePos;
	Position m_desPos;
};


class CModifyCopy  :public CDrawBase
{
public:
	CModifyCopy();
	virtual ~CModifyCopy();
	int OnLButtonDown(UINT nFlags,const Position &pos,CString cmd);
	int OnMouseMove(UINT nFlags,const Position &pos);
	int OnRButtonDown(UINT nFlags,const Position &pos);
private:


	Position m_basePos;
	Position m_desPos;
};


class CModifyMirror  :public CDrawBase
{
public:
	CModifyMirror();
	virtual ~CModifyMirror();
	int OnLButtonDown(UINT nFlags,const Position &pos,CString cmd);
	int OnMouseMove(UINT nFlags,const Position &pos);
	int OnRButtonDown(UINT nFlags,const Position &pos);
private:
	
	//把这个操作添加到撤消链表
	void AddUndoList();
	// 当选择保留原图时的新增加的链表
	CObList		m_CopyList;  
	Position m_basePos;
	Position m_desPos;
	BOOL m_bFlagIsDeletePer;//是否删除之前的
};


class CModifyDelete  :public CDrawBase
{
public:
	CModifyDelete();
	virtual ~CModifyDelete();
	int OnLButtonDown(UINT nFlags,const Position &pos,CString cmd);
	int OnMouseMove(UINT nFlags,const Position &pos);
	int OnRButtonDown(UINT nFlags,const Position &pos);


};


class CModifyRotate  :public CDrawBase
{
public:
	CModifyRotate();
	virtual ~CModifyRotate();
	int OnLButtonDown(UINT nFlags,const Position &pos,CString cmd);
	int OnMouseMove(UINT nFlags,const Position &pos);
	int OnRButtonDown(UINT nFlags,const Position &pos);
private:

	//把这个操作添加到撤消链表
	void AddUndoList();
	
	// 当选择保留原图时的新增加的链表
	CObList		m_CopyList;  
	Position m_basePos;
	Position m_desPos;
	BOOL m_bFlagIsDeletePer;//是否删除之前的
};

#endif // !defined(AFX_MODIFY_H__A1E3C4DE_FD59_4310_B646_BC5F14069EC3__INCLUDED_)
