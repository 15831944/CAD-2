/*

	����ʱ�õĽṹ��һЩ����

*/
//������ֻ����һ��
#if !defined(_POWERLAND_CAD_UNDO_H)
#define _POWERLAND_CAD_UNDO_H

#if _MSC_VER > 1000 //���MFC�汾����4.21
#pragma once	//ֻ����һ��
#endif 








typedef struct _CADPOINT//��
{
	double		x;	
	double		y;
} CADPOINT;

typedef struct _GRAPH_POS//ѡ�����ͼ��
{
	LONG		pCreateBasepos;	//ͼ���������е�����ָ��
	LONG        pCreateBaseposSpare1;//ͼ���������е�����ָ�뱸��
	LONG        pCreateBaseposSpare2;//ͼ���������е�����ָ�뱸��
	CADPOINT potData1;	
	CADPOINT potData2;	
	int	  iData1;		
	int   iData2;
	double dData1;
	double dData2;		
	POSITION	pListPos;	//ͼ���������еĽ��ָ��	
	POSITION	pListPosSpare;//ͼ���������еĽ��ָ�뱸��
	CArray<Position,Position&> aryPoint;//�㼯
	CArray<double,double&> aryTudu;//͹�ȼ�
	_GRAPH_POS	*pNext;
	_GRAPH_POS	*pPrev;
	
} GRAPHPOS, *PGRAPHPOS;

typedef struct _OPERATE_PARAM//��������
{
	CADPOINT potBase;	//����
	CADPOINT potDes;	//Ŀ�صĵ�
	LONG pPosition;		//λ��ָ��
	int direction;
	double distance;
	double angle;	
	
} OPERATE_PARAM, *POPERATE_PARAM;

typedef struct _UNDOLIST	
{
	int OperatieNum;	//��������
	int OperatieWhat;	//������
	POPERATE_PARAM pOperatieParam;//��������
	PGRAPHPOS pGraphPos;	//ͼ�εļ�
	PGRAPHPOS pGraphPosSpare;//ͼ�εļ�����
	_UNDOLIST		*pNext;
	_UNDOLIST		*pPrev;

} UNDOLIST, *PUNDOLIST;






#endif 