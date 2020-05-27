// TransToGraphList.h: interface for the TransToGraphList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TRANSTOGRAPHLIST_H__7D1F946D_6D6B_4B6C_BEA9_C657ACED3C7F__INCLUDED_)
#define AFX_TRANSTOGRAPHLIST_H__7D1F946D_6D6B_4B6C_BEA9_C657ACED3C7F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class TransToGraphList  //��DATASTRUCT�ṹת��CAD��ʽ
{
public:
	TransToGraphList();
	virtual ~TransToGraphList();

	//ĥ��������ĺ���
	PGRAPHLIST OutGraphList(PUNDOLIST list);
	//ĥ������ĺ���
	void OutMillSide();


	//------------------------------------------
private:

//////////////////////////////////////////////////////////////////////////
	//���PGRAPHLIST
	void ClearGraphList();
	//��ȡMCode��ֵ
	int MCodeValue(CString str);
	//�µ�̧����M����
	CString CintToCBMstr(int RtnColor);
	CString CintToSBMstr(int RtnColor);	
	// д�ļ�β
	BOOL	TransTail();	
	// дͼԪ��Ϣ
	BOOL	TransLine(CCreateLine*	pLine);
	BOOL	TransRectangle(CCreateRectangle*	pRectangle);
	BOOL	TransRound(CCreateRound*	pRound);
	BOOL	TransArc(CCreateRoundArc*	pArc);
	BOOL	TransPolyline(CCreatePolyline* Polyline);
	BOOL	TransEllipse(CCreateEllipse* Ellipse);
	
	void AddAction(); 
	void AddGraphList(); 
	void AddArcInfo();
	void AddCADData();
	void AddGraph();
	BOOL AddG01(DOUBLEPOINT ptEnd);			//���G00
	BOOL AddG0X(CCreateRoundArc *pArc);		//���G02/03
	BOOL AddG0X(CCreateRound *pRound, DOUBLEPOINT ptBegAndEnd);		//���G02/03
	BOOL AddG00(DOUBLEPOINT ptEnd, CString l_strSBM_What);			//���G00
	double PointToPointAngle(DOUBLEPOINT ptBeg, DOUBLEPOINT ptEnd);	//����Բ�������/�е�/�յ�Ƕ�
	double CalculateTraverseAngle(char chDirection, double dBeginAngle, double dEndAngle);	//����ɨ���Ƕ�
	//��ӡ����
	void WriteGraphList(PGRAPHLIST pList);
	//��M���뼯�������ַ���
	CString AnalysisAryM(CArray<class CString,class CString &>& aryM);
	
	PGRAPHLIST		m_pHeadList;
	PGRAPHLIST		m_pCurrList;
	PGRAPH			m_pCurrGraph;
	PCADDATA		m_pCurrData;
	DOUBLEPOINT		m_ptSet;
	
	CFile m_cGCode;
	double m_dCurX;	//��ǰ��X
	double m_dCurY;	//��ǰ��Y
	int m_iColorLast;//������ɫ
	CString m_strCBM_What;	//����ߵ���ɫ
	BOOL m_bFlagAddList;//�Ƿ���ṹ���ͼ��
/////////////////////////////////////////////////////////////////////////

	//ת������
	void Transfer();
	//ת����ϳ�����ͼ��
	void TransMillside();

	//�ӵ�������
	PGRAPHLIST AddKnifeStyle(long lData);
	//�ӵ�������
	PGRAPHLIST AddKnifeStyleForMillSide(long lData);
	

	//�ӵ����ߴ�
	void AddKnifeLong(POSITION POS);
	//�ӵ����ߴ��ϳ��ͼ��
	void AddKnifeLongForMillSide(POSITION POS);

	//��DATASTRUCT�Ľṹת��CreateBase�ṹ			    			    ����ĥ��  
	BOOL GraphListTurnCreateStc(PGRAPHLIST pHeadList,POSITION POS,int StyleKnife);
	//GRAPHLISTת��Createbase			    			          
	void ListToCreateForMillSide(PGRAPHLIST pHeadList,POSITION POS);

	//���ش֣������׹����ɫ
	COLORREF RtnColorOfKnife(int StyleKnife,int l_iMcode);//0Ϊԭͼɫ��1Ϊ��ĥ��2Ϊ��ĥ��3Ϊ�׹�

	//�������������
	void AddUndoList(LONG CreateBase,POSITION pos);
	//ɾ��ͼԪ����������ĵ����ɾ������ťҪʵ�ֵĹ���
	void DeleteOriginal(PUNDOLIST pUndoList);


	CObArray    m_CollectOfG00;	// ������G00֮����ͼԪ
	//������Ķ���
	CUndoRedo m_UndoRedo;
	//������������ӵ�һ�����	
	PUNDOLIST m_pUndoList;
	//��ͼ�ε�ָ��
	PGRAPHPOS m_pCurGraphPos,m_pGraphPos;

};

#endif // !defined(AFX_TRANSTOGRAPHLIST_H__7D1F946D_6D6B_4B6C_BEA9_C657ACED3C7F__INCLUDED_)
