// GraphTogether.h: interface for the CGraphTogether class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRAPHTOGETHER_H__5D9F05C7_809E_442B_BA85_9C87F16CB6D6__INCLUDED_)
#define AFX_GRAPHTOGETHER_H__5D9F05C7_809E_442B_BA85_9C87F16CB6D6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CGraphTogether  //��������ͼ�κϲ���������
{
public:
	CGraphTogether();
	virtual ~CGraphTogether();
	//ʹ��λͼ�κϲ�
	void PutGraphTogether();



private:


/////////////////////////����////////////////////////////////////////////

	//�����е�ͼ��������һ��
	void CopyList();
	//��ջ�������
	void ClearBaseList();
	//�ı��и��
	void Changedir() ;
///////////////////����//////////////////////////////////////////////////
	
	//			���	�յ�
	Position m_PosBegin,m_PosEnd;
	//			��������
	CObList m_cpyBaseList;
	//Ҫ�ı䷽���ͼ��
	CCreateBase *m_ChangeDir;
};

#endif // !defined(AFX_GRAPHTOGETHER_H__5D9F05C7_809E_442B_BA85_9C87F16CB6D6__INCLUDED_)
