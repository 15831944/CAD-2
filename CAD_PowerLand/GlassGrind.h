// GlassGrind.h: interface for the CGlassGrind class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GLASSGRIND_H__180C34E5_082B_4EC9_A4EB_85CDCB66486C__INCLUDED_)
#define AFX_GLASSGRIND_H__180C34E5_082B_4EC9_A4EB_85CDCB66486C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CGlassGrind  //ĥ����
{
public:
	CGlassGrind();
	virtual ~CGlassGrind();
////////////////////���к���///////////////////////////////////
	BOOL GetFinishList();	//�õ����յ�����

////////////////////���б���///////////////////////////////////

private:
	//���PGRAPHLIST
	void ClearGraphList(PGRAPHLIST GraphList);
	//����PGRAPHLIST
	PGRAPHLIST CopyGraphList(PGRAPHLIST pGraphList);
	//ɾ��ͼԪ����������ĵ����ɾ������ťҪʵ�ֵĹ���
	void DeleteOriginal(PUNDOLIST pUndoList);


	
};

#endif // !defined(AFX_GLASSGRIND_H__180C34E5_082B_4EC9_A4EB_85CDCB66486C__INCLUDED_)
