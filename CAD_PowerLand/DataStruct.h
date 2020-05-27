/*  �ڵ�������ʹ��  */
/*  ��ʾ�̿���ʹ��  */
/*  �ڽ��ȱ�ؼ�ʹ��  */
/*  ��GridCAD�ؼ���ʹ��  */
/*  �սǹ����������ʼ��ֹ�ٶ� */

#ifndef DATASTRUCT_H
	#define DATASTRUCT_H

typedef struct tagDOUBLEPOINT
{
	double		x;	
	double		y;
	double		z;
} DOUBLEPOINT;


typedef struct tagCADDATA
{
	DOUBLEPOINT		PointGrid;
	DOUBLEPOINT		PointPara;	/* XYZ������ */
	DOUBLEPOINT		PointParaR;	/* ABC������ */
	tagCADDATA		*pNext;
	tagCADDATA		*pPrev;
} CADDATA, *PCADDATA;

typedef struct tagARCINFO
{
	DOUBLEPOINT		PosCenter;		/* Բ�� */
	DOUBLEPOINT		PosIJK;			/* Բ������������� */
	double			dBeginAngle;	/* ��ʼ�Ƕ� */
	double			dEndAngle;		/* ��ֹ�Ƕ� */
	double			dAngleSweep;	/* �н� */
}ARCINFO, *PARCINFO;


typedef struct tagACTION
{
	int				nVar;			/* S����ֵ */
	int				nType;			/* �������� 1��M���롣*/
	int				nCode;			/* GMST��� */
	double			dVar;			/* Ԥ�� */
}ACTION, *PACTION; 

typedef struct tagGRAPH
{
	int				nIndex;			/* �ṹ��� */
	int             isOdditional;   /* �������ڿ��ͼ���� 0��ʾ���ǲ����Ľ�� */
	char			chLine;			/* ���ͣ�1Ϊֱ�ߣ�2Ϊ˳ʱ��Բ����3Ϊ��ʱ��Բ�� */
	PCADDATA		pPosBegin;		/* �岹��� */
	PCADDATA		pPosEnd;		/* �岹�յ� */
	PCADDATA		pPosMid;		/* Բ���еĵ�����꣬������G���������������ʱû������ */
	PARCINFO		pArcInfo;		/* Բ���������Ϣ */
	double			dBeginSpeed;	/* ��ʼ����ٶ� */
	double			dEndSpeed;		/* ��ֹ����ٶ� */
	int             nStartCornerCtrl;/* ��ʼ�սǿ��ƵĲ岹���� */
	int             nEndCornerCtrl;  /* �����սǿ��ƵĲ岹���� */
	int             nGouging;        //���У�1Ϊ���в岹
	tagGRAPH		*Next;
	tagGRAPH		*Prev;
}GRAPH, *PGRAPH;

typedef struct tagGRAPHLIST
{
	int				nSort;			/* �ṹ��� */
	PACTION			pAction;		/* ���� */
	PCADDATA		pCadData;
	PGRAPH			pGraph;			/* ����¼��λ�� */
	int				nSplineStyle;	/* 0���ޣ�1������B�������ߡ�2�����α��������ߡ�3�������ۼ��ҳ����� */
	PCADDATA		pBSplineData;	/* �����������ߵĵ������*/
	int				nTool;			/* ָ������ */
	int				nFeed;			/* ָ�������ٶ� */
	int				nOrder;			/* ָ��˳�� */
	char			chKerfComp;		/* �������� 41�󵶲� �� 42�ҵ���*/
	double			dToolSize;		/* �����ߴ� */
	BOOL			bClose;			/* ͼ���Ƿ�պ� */
	int             nAxises;        /*�Ƿ������ᣬ����Ϊ1������Ϊ��*/  
	tagGRAPHLIST	*Next;			/*  */
	tagGRAPHLIST	*Prev;			/*  */
}GRAPHLIST, *PGRAPHLIST; 

typedef struct tagMCODE
{
	char	chMCode[100][500];		/* M������� */
}MCODE, *PMCODE; 

#endif
/*===========================================================================*/
