// UndoRedo.h: interface for the CUndoRedo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UNDOREDO_H__EF1F5C60_2758_4F28_9B44_C0C3815DE69A__INCLUDED_)
#define AFX_UNDOREDO_H__EF1F5C60_2758_4F28_9B44_C0C3815DE69A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Undo.h"

class CUndoRedo  
{
public:
	CUndoRedo();
	virtual ~CUndoRedo();

	//添加操作步骤
	void AddUndoList(PUNDOLIST UndoList);
	//执行撤消功能
	void OnUndo();
	//执行重做功能
	void OnRedo();

	//新建一个UNDOLIST
	PUNDOLIST NewUndoList();
	//新建一个POPERATE_PARAM
	POPERATE_PARAM NewOperateParam();
	//新建一个GRAPHPOS
	PGRAPHPOS NewGraphPos();
	//释放LIST				如果是TRUE的话就是撤消后操作别的功能的释放法
	void ClearUndoList(PUNDOLIST pUndoList,BOOL IsDelCreate = FALSE);


//					当前的			第一个
	PUNDOLIST		m_pCurrList,m_pHeadList;


private:
	//执行撤消移动操作;TRUE为撤消，FALSE为重做
	void OnMove(BOOL IsUndo);
	//执行撤消镜像操作;TRUE为撤消，FALSE为重做
	void OnMirror(BOOL IsUndo);
	//执行撤消旋转操作;TRUE为撤消，FALSE为重做
	void OnRotate(BOOL IsUndo);
	//执行撤消画图操作;TRUE为撤消，FALSE为重做
	void OnCreateGraph(BOOL IsUndo);
	//执行撤消删除操作;TRUE为撤消，FALSE为重做
	void OnDeleteGraph(BOOL IsUndo);
	//执行撤消倒角操作;TRUE为撤消，FALSE为重做
	void OnChamferGraph(BOOL IsUndo);
	//执行撤消重新排序操作;TRUE为撤消，FALSE为重做
	void OnArrangeGraph(BOOL IsUndo);	
	//执行撤消打断操作;TRUE为撤消，FALSE为重做
	void OnBreakGraph(BOOL IsUndo);
	//执行撤消过切操作;TRUE为撤消，FALSE为重做
	void OnOverCutGraph(BOOL IsUndo);
	//执行撤消XY缩放操作;TRUE为撤消，FALSE为重做
	void OnScaleGraph(BOOL IsUndo);
	//执行撤消变铣刀操作;TRUE为撤消，FALSE为重做
	void OnIOKnife(BOOL IsUndo);
	//执行撤消合并操作;TRUE为撤消，FALSE为重做
	void OnTogetherGraph(BOOL IsUndo);
	//执行撤消磨玻璃操作;TRUE为撤消，FALSE为重做
	void OnGrindGraph(BOOL IsUndo);	
	//执行撤消变铣刀操作;TRUE为撤消，FALSE为重做
	void OnMillGrooves(BOOL IsUndo);
	//执行撤消翻转操作;TRUE为撤消，FALSE为重做
	void OnRotateSymmetry(BOOL IsUndo);
	//执行撤消改变切割方向操作;TRUE为撤消，FALSE为重做
	void OnChangeDir(BOOL IsUndo);
	//执行撤消变颜色操作;TRUE为撤消，FALSE为重做
	void OnChangeColor(BOOL IsUndo);
	//执行撤消属性对话框的编辑框操作;TRUE为撤消，FALSE为重做
	void OnPropertyChange(BOOL IsUndo);
	//执行撤消改变刀序操作;TRUE为撤消，FALSE为重做
	void OnInsertAfterGraph(BOOL IsUndo);
	//执行撤消改变刀序操作;TRUE为撤消，FALSE为重做
	void OnInsertBeforGraph(BOOL IsUndo);
	//执行撤消缩放操作;TRUE为撤消，FALSE为重做
	void OnZoomScale(BOOL IsUndo);
	//执行撤消变虚线操作;TRUE为撤消，FALSE为重做
	void OnTurnDash(BOOL IsUndo);
	//执行点移动操作;TRUE为撤消，FALSE为重做
	void OnMovePoint(BOOL IsUndo);
	

	//		 	最后一个
	PUNDOLIST m_pTailList;


};

#endif // !defined(AFX_UNDOREDO_H__EF1F5C60_2758_4F28_9B44_C0C3815DE69A__INCLUDED_)
