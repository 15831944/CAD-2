// OpenPLTCode.cpp: implementation of the COpenPLTCode class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cad_powerland.h"
#include "OpenPLTCode.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COpenPLTCode::COpenPLTCode()
{

}

COpenPLTCode::~COpenPLTCode()
{

}

// PLT�ļ���ʽ����   ��������Ϊ�ļ�Ŀ¼�����磺strFile = "D:\\Program Files\\Garment Pattern\\GCode\\" + "dxf.g"��
//����ֵΪpltת��ΪG�����·��
CString COpenPLTCode::AnalysisPlt(CString strFile)
{
	CString g_strOriginalPathC= "C:\\Program Files\\CAD_PowerLand\\";
	CStdioFile  m_saveFile, m_stdioFile;
	CString		m_strFile, m_strTemp, l_strValue;
	m_strOutFile = g_strOriginalPathC + "PLT.g";
	char		szBuffer[100] = {0};
	int			i, t;
	int			m_iDownOrUp = 0; // �ж�ǰ���ΪM07 ���� M08�����ظ�
	double		l_dValue_X, l_dValue_Y;
	int			l_iSemCount = 0; // �ֺ�����
	int			m_iPltScale = 40;// PLT�ļ����豸��λ���׼���ȵ�λ�Ļ����ϵ����: 40����ͼ�ǵ�λ = 1 mm 
	if(m_saveFile.Open(m_strOutFile,CFile::modeCreate|CFile::modeWrite|CFile::typeText)==0)
	{
		return 1;
	}
	char seps[] = ";\t\n";	// �ַ����ָ���" " "," "Tab" "����"
	char sepssem[] = ";";		// �ָ�����;��
	char sepscommma[] = " ,";	// �ָ�����,��
	char sepsU[] = "U";			// �ָ���"U"
	char sepsD[] = "D";
	char sepsA[] = "A";
	char sepsP[] = "P";
	char* token = NULL;
	char* TempToken = NULL;
	char* tokenPU = NULL;
	char* tokenPD = NULL;
	char* tokenPA = NULL;
	char* tokenSP = NULL;
	int l_iIsOpen = m_stdioFile.Open(strFile,CFile::modeRead); // ���ļ�
	if (l_iIsOpen == 0)  
	{
		//���Ļ���Ӣ��
		if (g_pDoc->m_stcWorkFor.IsChinese)
			AfxMessageBox("���ļ�ʧ�ܣ�");
		else
			AfxMessageBox("Failed to open the file!");
		return m_strOutFile ;	// ��û�ж����ĵ�������ѭ��
	}
	while (1)
	{
		memset(szBuffer,0,sizeof(szBuffer));
		DWORD l_iIsRead =m_stdioFile.ReadString(m_strFile); // ��ȡ�ļ�(һ�ζ�ȡһ��)
		if (l_iIsRead == 0) 
		{
			break;	// ��û�ж����ĵ�������ѭ��
		}
		m_strTemp = m_strFile;
		int l_iLength = strlen(m_strTemp); // �õ��ַ����ĳ���
		token = strtok( m_strFile.GetBuffer(strlen(m_strFile)),seps);
		l_iSemCount = 0; // ��ȡ�µ�һ������ʱ��Ϊ0,��ͷ��ʼ
		while (token != NULL)
		{
			if (strstr(token,"PU"))
			{		
				if (l_iSemCount == 3)
				{
					if (m_iDownOrUp == 0) //�����Ķ��M08ֻ����һ��M08
					{
						m_saveFile.WriteString("M08\n");   //̧��
						m_iDownOrUp = 1;
					}
				}
				else
				{
					if (m_iDownOrUp == 0)
					{
						m_saveFile.WriteString("M08\n");   //̧��
						m_iDownOrUp = 1;
					}
				
					tokenPU = token;
					TempToken = strtok(tokenPU,sepsU); // TempToken == "P"
					TempToken = strtok(NULL,sepscommma); // sepsU֮��sepscommma֮����ַ�
					if (TempToken == NULL)	// ��PU������ֵ��������ѭ��
					{
						break;
					}
					l_dValue_X = atof(TempToken);
					TempToken = strtok(NULL,sepscommma); // sepsU֮��sepscommma֮����ַ�
					l_dValue_Y = atof(TempToken);
					l_strValue.Format("G00 X%2f Y%2f\n",(double)(l_dValue_X/m_iPltScale),(double)(l_dValue_Y/m_iPltScale));
					m_saveFile.WriteString(l_strValue);	
				}	
			}
			if(strstr(token,"PD")) // �±�
			{				
				if (l_iSemCount == 3)	// �ж�PDֱ�Ӹ������Ϣ����ֻ��̧�µ�����
				{
					if (m_iDownOrUp == 1)//�����Ķ��M07ֻ����һ��M07
					{
						m_saveFile.WriteString("M07\n");   //̧��
						m_iDownOrUp = 0;
					}
				}
				else
				{
					if (m_iDownOrUp == 1)
					{
						m_saveFile.WriteString("M07\n");   //̧��
						m_iDownOrUp = 0;
					}
					tokenPD = token;
					TempToken = strtok(tokenPD,sepsD);
					TempToken = strtok(NULL,sepscommma);
					if (TempToken == NULL)
					{
						break;
					}
					l_dValue_X = atof(TempToken);
					TempToken = strtok(NULL,sepscommma);
					l_dValue_Y = atof(TempToken);
					l_strValue.Format("G01 X%2f Y%2f\n",(double)(l_dValue_X/m_iPltScale),(double)(l_dValue_Y/m_iPltScale));
					m_saveFile.WriteString(l_strValue);
					t =	m_strTemp.Find(TempToken,0);
					if (t+(strlen(TempToken)) < l_iLength) // Ѱ�ҵ���һ���ַ���λ��ֵС�ڱ��ַ����ĳ���
					{
						if (m_strTemp.GetAt(t+(strlen(TempToken))) == ',') // ����ȡ����һ���ַ�Ϊ���� 
						{
							while (1)
							{
								TempToken = strtok(NULL,sepscommma);
								if (TempToken == NULL)
								{
									break;
								}
								t =	m_strTemp.Find(TempToken,t);
								if (m_strTemp.GetAt(t+(strlen(TempToken))) != ',') // ����һ����Ϊ���ţ����˳�
								{
									break;
								}
								l_dValue_X = atof(TempToken);
								TempToken = strtok(NULL,sepscommma);
								l_dValue_Y = atof(TempToken);
								l_strValue.Format("G01 X%2f Y%2f\n",(double)(l_dValue_X/m_iPltScale),(double)(l_dValue_Y/m_iPltScale));
								m_saveFile.WriteString(l_strValue);	
							}
						}
					}	
				}
			}
			if (strstr(token,"PA")) // �˶���ָ���ľ���λ�ô�
			{
				m_saveFile.WriteString("M07\n");
				tokenPA = token;
				TempToken = strtok(tokenPA,sepsA);
				TempToken = strtok(NULL,sepscommma);
				if (TempToken == NULL)
				{
					break;
				}
				l_dValue_X = atof(TempToken);
				TempToken = strtok(NULL,sepscommma);
				l_dValue_Y = atof(TempToken);
				l_strValue.Format("G01 X%2f Y%2f\n",(double)(l_dValue_X/m_iPltScale),(double)(l_dValue_Y/m_iPltScale));
				m_saveFile.WriteString(l_strValue);
				t =	m_strTemp.Find(TempToken,0);
				if (m_strTemp.GetAt(t+(strlen(TempToken))) == ',') // ����ȡ����һ���ַ�Ϊ���� 
				{
					while (1)
					{
						TempToken = strtok(NULL,sepscommma);
						if (TempToken == NULL)
						{
							break;
						}
						t =	m_strTemp.Find(TempToken,t);
						if (m_strTemp.GetAt(t+(strlen(TempToken))) != ',') // ����һ����Ϊ���ţ����˳�
						{
							break;
						}
						l_dValue_X = atof(TempToken);
						TempToken = strtok(NULL,sepscommma);
						l_dValue_Y = atof(TempToken);
						l_strValue.Format("G01 X%2f Y%2f\n",(double)(l_dValue_X/m_iPltScale),(double)(l_dValue_Y/m_iPltScale));
						m_saveFile.WriteString(l_strValue);	
					}
				}
			}
			i = m_strTemp.Find(';',l_iSemCount); // Ѱ����һ���ֺŵ�λ��ֵ
			if ( i == 0)
			{
				if (l_iLength != 0) // �����ַ������Ȳ�Ϊ0 ˵����һ���ַ�Ϊ�ֺţ����滹���ַ�����i = 1;
				{
					i = 1;
				}
				else
				{
					break;
				}	
			}
			m_strFile = m_strTemp.Mid(i); // �ѷֺź���ַ�������m_strFile�ַ�����,����ǰ����ȥ������һ���ֺ����¿�ʼ
			m_strTemp = m_strFile;
			token = strtok( m_strFile.GetBuffer(strlen(m_strFile)),";");
			if (token != NULL)
			{
				l_iSemCount = m_strTemp.Find(';',1); // �õ��µ��ַ�������һ���ֺŵ�λ��ֵ
				if (l_iSemCount < 0)
				{
					break;
				}
			}
		}
	}
	m_saveFile.WriteString("M08\n");
	m_saveFile.WriteString("M10\n");
	m_saveFile.WriteString("M12\n");
	m_saveFile.Close();
	m_strFile.ReleaseBuffer();
	return m_strOutFile ;
}