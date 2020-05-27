/*  在刀补库中使用  */
/*  在示教库中使用  */
/*  在进度表控件使用  */
/*  在GridCAD控件中使用  */
/*  拐角过度中添加起始终止速度 */

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
	DOUBLEPOINT		PointPara;	/* XYZ轴数据 */
	DOUBLEPOINT		PointParaR;	/* ABC轴数据 */
	tagCADDATA		*pNext;
	tagCADDATA		*pPrev;
} CADDATA, *PCADDATA;

typedef struct tagARCINFO
{
	DOUBLEPOINT		PosCenter;		/* 圆心 */
	DOUBLEPOINT		PosIJK;			/* 圆心相对起点的增量 */
	double			dBeginAngle;	/* 起始角度 */
	double			dEndAngle;		/* 终止角度 */
	double			dAngleSweep;	/* 夹角 */
}ARCINFO, *PARCINFO;


typedef struct tagACTION
{
	int				nVar;			/* S命令值 */
	int				nType;			/* 命令类型 1：M代码。*/
	int				nCode;			/* GMST序号 */
	double			dVar;			/* 预留 */
}ACTION, *PACTION; 

typedef struct tagGRAPH
{
	int				nIndex;			/* 结构序号 */
	int             isOdditional;   /* 这个结点在库存图形中 0表示这是补偿的结点 */
	char			chLine;			/* 类型：1为直线，2为顺时针圆弧，3为逆时针圆弧 */
	PCADDATA		pPosBegin;		/* 插补起点 */
	PCADDATA		pPosEnd;		/* 插补终点 */
	PCADDATA		pPosMid;		/* 圆弧中的点的坐标，但是由G代码库生产的数据时没有数据 */
	PARCINFO		pArcInfo;		/* 圆弧的相关信息 */
	double			dBeginSpeed;	/* 开始点的速度 */
	double			dEndSpeed;		/* 终止点的速度 */
	int             nStartCornerCtrl;/* 开始拐角控制的插补距离 */
	int             nEndCornerCtrl;  /* 结束拐角控制的插补距离 */
	int             nGouging;        //过切；1为过切插补
	tagGRAPH		*Next;
	tagGRAPH		*Prev;
}GRAPH, *PGRAPH;

typedef struct tagGRAPHLIST
{
	int				nSort;			/* 结构序号 */
	PACTION			pAction;		/* 动作 */
	PCADDATA		pCadData;
	PGRAPH			pGraph;			/* 鼠标记录的位置 */
	int				nSplineStyle;	/* 0：无；1：二次B样条曲线、2：二次贝塞尔曲线、3：二次累加弦长曲线 */
	PCADDATA		pBSplineData;	/* 保存样条曲线的点的数据*/
	int				nTool;			/* 指定刀具 */
	int				nFeed;			/* 指定进给速度 */
	int				nOrder;			/* 指定顺序 */
	char			chKerfComp;		/* 刀补类型 41左刀补 和 42右刀补*/
	double			dToolSize;		/* 刀补尺寸 */
	BOOL			bClose;			/* 图形是否闭合 */
	int             nAxises;        /*是否是三轴，三轴为1，两轴为零*/  
	tagGRAPHLIST	*Next;			/*  */
	tagGRAPHLIST	*Prev;			/*  */
}GRAPHLIST, *PGRAPHLIST; 

typedef struct tagMCODE
{
	char	chMCode[100][500];		/* M代码解释 */
}MCODE, *PMCODE; 

#endif
/*===========================================================================*/
