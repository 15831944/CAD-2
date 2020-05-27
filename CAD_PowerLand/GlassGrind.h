// GlassGrind.h: interface for the CGlassGrind class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GLASSGRIND_H__180C34E5_082B_4EC9_A4EB_85CDCB66486C__INCLUDED_)
#define AFX_GLASSGRIND_H__180C34E5_082B_4EC9_A4EB_85CDCB66486C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CGlassGrind  //磨玻璃
{
public:
	CGlassGrind();
	virtual ~CGlassGrind();
////////////////////公有函数///////////////////////////////////
	BOOL GetFinishList();	//得到最终的链表

////////////////////公有变量///////////////////////////////////

private:
	//清空PGRAPHLIST
	void ClearGraphList(PGRAPHLIST GraphList);
	//复制PGRAPHLIST
	PGRAPHLIST CopyGraphList(PGRAPHLIST pGraphList);
	//删除图元函数，上面的点击“删除”按钮要实现的功能
	void DeleteOriginal(PUNDOLIST pUndoList);


	
};

#endif // !defined(AFX_GLASSGRIND_H__180C34E5_082B_4EC9_A4EB_85CDCB66486C__INCLUDED_)
