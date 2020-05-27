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

// PLT文件格式解析   函数输入为文件目录，例如：strFile = "D:\\Program Files\\Garment Pattern\\GCode\\" + "dxf.g"；
//返回值为plt转化为G代码的路径
CString COpenPLTCode::AnalysisPlt(CString strFile)
{
	CString g_strOriginalPathC= "C:\\Program Files\\CAD_PowerLand\\";
	CStdioFile  m_saveFile, m_stdioFile;
	CString		m_strFile, m_strTemp, l_strValue;
	m_strOutFile = g_strOriginalPathC + "PLT.g";
	char		szBuffer[100] = {0};
	int			i, t;
	int			m_iDownOrUp = 0; // 判断前面的为M07 还是 M08，不重复
	double		l_dValue_X, l_dValue_Y;
	int			l_iSemCount = 0; // 分号数量
	int			m_iPltScale = 40;// PLT文件中设备单位与标准长度单位的换算关系如下: 40个绘图仪单位 = 1 mm 
	if(m_saveFile.Open(m_strOutFile,CFile::modeCreate|CFile::modeWrite|CFile::typeText)==0)
	{
		return 1;
	}
	char seps[] = ";\t\n";	// 字符串分隔符" " "," "Tab" "换行"
	char sepssem[] = ";";		// 分隔符“;”
	char sepscommma[] = " ,";	// 分隔符“,”
	char sepsU[] = "U";			// 分隔符"U"
	char sepsD[] = "D";
	char sepsA[] = "A";
	char sepsP[] = "P";
	char* token = NULL;
	char* TempToken = NULL;
	char* tokenPU = NULL;
	char* tokenPD = NULL;
	char* tokenPA = NULL;
	char* tokenSP = NULL;
	int l_iIsOpen = m_stdioFile.Open(strFile,CFile::modeRead); // 打开文件
	if (l_iIsOpen == 0)  
	{
		//中文还是英文
		if (g_pDoc->m_stcWorkFor.IsChinese)
			AfxMessageBox("打开文件失败！");
		else
			AfxMessageBox("Failed to open the file!");
		return m_strOutFile ;	// 若没有读到文档，跳出循环
	}
	while (1)
	{
		memset(szBuffer,0,sizeof(szBuffer));
		DWORD l_iIsRead =m_stdioFile.ReadString(m_strFile); // 读取文件(一次读取一行)
		if (l_iIsRead == 0) 
		{
			break;	// 若没有读到文档，跳出循环
		}
		m_strTemp = m_strFile;
		int l_iLength = strlen(m_strTemp); // 得到字符串的长度
		token = strtok( m_strFile.GetBuffer(strlen(m_strFile)),seps);
		l_iSemCount = 0; // 读取新的一行数据时设为0,从头开始
		while (token != NULL)
		{
			if (strstr(token,"PU"))
			{		
				if (l_iSemCount == 3)
				{
					if (m_iDownOrUp == 0) //连续的多个M08只下载一个M08
					{
						m_saveFile.WriteString("M08\n");   //抬刀
						m_iDownOrUp = 1;
					}
				}
				else
				{
					if (m_iDownOrUp == 0)
					{
						m_saveFile.WriteString("M08\n");   //抬刀
						m_iDownOrUp = 1;
					}
				
					tokenPU = token;
					TempToken = strtok(tokenPU,sepsU); // TempToken == "P"
					TempToken = strtok(NULL,sepscommma); // sepsU之后到sepscommma之间的字符
					if (TempToken == NULL)	// 若PU后无数值，则跳出循环
					{
						break;
					}
					l_dValue_X = atof(TempToken);
					TempToken = strtok(NULL,sepscommma); // sepsU之后到sepscommma之间的字符
					l_dValue_Y = atof(TempToken);
					l_strValue.Format("G00 X%2f Y%2f\n",(double)(l_dValue_X/m_iPltScale),(double)(l_dValue_Y/m_iPltScale));
					m_saveFile.WriteString(l_strValue);	
				}	
			}
			if(strstr(token,"PD")) // 下笔
			{				
				if (l_iSemCount == 3)	// 判断PD直接跟点的信息还是只有抬下刀动作
				{
					if (m_iDownOrUp == 1)//连续的多个M07只下载一个M07
					{
						m_saveFile.WriteString("M07\n");   //抬刀
						m_iDownOrUp = 0;
					}
				}
				else
				{
					if (m_iDownOrUp == 1)
					{
						m_saveFile.WriteString("M07\n");   //抬刀
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
					if (t+(strlen(TempToken)) < l_iLength) // 寻找的下一个字符的位置值小于本字符串的长度
					{
						if (m_strTemp.GetAt(t+(strlen(TempToken))) == ',') // 若提取的下一个字符为逗号 
						{
							while (1)
							{
								TempToken = strtok(NULL,sepscommma);
								if (TempToken == NULL)
								{
									break;
								}
								t =	m_strTemp.Find(TempToken,t);
								if (m_strTemp.GetAt(t+(strlen(TempToken))) != ',') // 若下一个不为逗号，则退出
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
			if (strstr(token,"PA")) // 运动到指定的绝对位置处
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
				if (m_strTemp.GetAt(t+(strlen(TempToken))) == ',') // 若提取的下一个字符为逗号 
				{
					while (1)
					{
						TempToken = strtok(NULL,sepscommma);
						if (TempToken == NULL)
						{
							break;
						}
						t =	m_strTemp.Find(TempToken,t);
						if (m_strTemp.GetAt(t+(strlen(TempToken))) != ',') // 若下一个不为逗号，则退出
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
			i = m_strTemp.Find(';',l_iSemCount); // 寻找下一个分号的位置值
			if ( i == 0)
			{
				if (l_iLength != 0) // 若是字符串长度不为0 说明第一个字符为分号，后面还有字符，把i = 1;
				{
					i = 1;
				}
				else
				{
					break;
				}	
			}
			m_strFile = m_strTemp.Mid(i); // 把分号后的字符串放在m_strFile字符串中,把以前的舍去，从下一个分号重新开始
			m_strTemp = m_strFile;
			token = strtok( m_strFile.GetBuffer(strlen(m_strFile)),";");
			if (token != NULL)
			{
				l_iSemCount = m_strTemp.Find(';',1); // 得到新的字符串的下一个分号的位置值
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