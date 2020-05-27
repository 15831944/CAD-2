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

	//��Ӳ�������
	void AddUndoList(PUNDOLIST UndoList);
	//ִ�г�������
	void OnUndo();
	//ִ����������
	void OnRedo();

	//�½�һ��UNDOLIST
	PUNDOLIST NewUndoList();
	//�½�һ��POPERATE_PARAM
	POPERATE_PARAM NewOperateParam();
	//�½�һ��GRAPHPOS
	PGRAPHPOS NewGraphPos();
	//�ͷ�LIST				�����TRUE�Ļ����ǳ����������Ĺ��ܵ��ͷŷ�
	void ClearUndoList(PUNDOLIST pUndoList,BOOL IsDelCreate = FALSE);


//					��ǰ��			��һ��
	PUNDOLIST		m_pCurrList,m_pHeadList;


private:
	//ִ�г����ƶ�����;TRUEΪ������FALSEΪ����
	void OnMove(BOOL IsUndo);
	//ִ�г����������;TRUEΪ������FALSEΪ����
	void OnMirror(BOOL IsUndo);
	//ִ�г�����ת����;TRUEΪ������FALSEΪ����
	void OnRotate(BOOL IsUndo);
	//ִ�г�����ͼ����;TRUEΪ������FALSEΪ����
	void OnCreateGraph(BOOL IsUndo);
	//ִ�г���ɾ������;TRUEΪ������FALSEΪ����
	void OnDeleteGraph(BOOL IsUndo);
	//ִ�г������ǲ���;TRUEΪ������FALSEΪ����
	void OnChamferGraph(BOOL IsUndo);
	//ִ�г��������������;TRUEΪ������FALSEΪ����
	void OnArrangeGraph(BOOL IsUndo);	
	//ִ�г�����ϲ���;TRUEΪ������FALSEΪ����
	void OnBreakGraph(BOOL IsUndo);
	//ִ�г������в���;TRUEΪ������FALSEΪ����
	void OnOverCutGraph(BOOL IsUndo);
	//ִ�г���XY���Ų���;TRUEΪ������FALSEΪ����
	void OnScaleGraph(BOOL IsUndo);
	//ִ�г�����ϳ������;TRUEΪ������FALSEΪ����
	void OnIOKnife(BOOL IsUndo);
	//ִ�г����ϲ�����;TRUEΪ������FALSEΪ����
	void OnTogetherGraph(BOOL IsUndo);
	//ִ�г���ĥ��������;TRUEΪ������FALSEΪ����
	void OnGrindGraph(BOOL IsUndo);	
	//ִ�г�����ϳ������;TRUEΪ������FALSEΪ����
	void OnMillGrooves(BOOL IsUndo);
	//ִ�г�����ת����;TRUEΪ������FALSEΪ����
	void OnRotateSymmetry(BOOL IsUndo);
	//ִ�г����ı��и�����;TRUEΪ������FALSEΪ����
	void OnChangeDir(BOOL IsUndo);
	//ִ�г�������ɫ����;TRUEΪ������FALSEΪ����
	void OnChangeColor(BOOL IsUndo);
	//ִ�г������ԶԻ���ı༭�����;TRUEΪ������FALSEΪ����
	void OnPropertyChange(BOOL IsUndo);
	//ִ�г����ı䵶�����;TRUEΪ������FALSEΪ����
	void OnInsertAfterGraph(BOOL IsUndo);
	//ִ�г����ı䵶�����;TRUEΪ������FALSEΪ����
	void OnInsertBeforGraph(BOOL IsUndo);
	//ִ�г������Ų���;TRUEΪ������FALSEΪ����
	void OnZoomScale(BOOL IsUndo);
	//ִ�г��������߲���;TRUEΪ������FALSEΪ����
	void OnTurnDash(BOOL IsUndo);
	//ִ�е��ƶ�����;TRUEΪ������FALSEΪ����
	void OnMovePoint(BOOL IsUndo);
	

	//		 	���һ��
	PUNDOLIST m_pTailList;


};

#endif // !defined(AFX_UNDOREDO_H__EF1F5C60_2758_4F28_9B44_C0C3815DE69A__INCLUDED_)
