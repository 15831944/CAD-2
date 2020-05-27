// HarddogInfo.h: interface for the CHarddogInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HARDDOGINFO_H__8A8F2E65_E9F7_4D3D_9592_DCD7FABCE250__INCLUDED_)
#define AFX_HARDDOGINFO_H__8A8F2E65_E9F7_4D3D_9592_DCD7FABCE250__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "hasp_api_cpp.h"


class AFX_EXT_CLASS CHarddogInfo  
{
public:


	/*

	* 函数介绍：构造函数
  
	* 输入参数：无
		
	* 输出参数：无
		  
	* 返回值  ：无
			
	*/
	CHarddogInfo();


	/*

	* 函数介绍：析构函数
  
	* 输入参数：无
		
	* 输出参数：无
		  
	* 返回值  ：无
			
	*/
	virtual ~CHarddogInfo();


	/*

	* 函数介绍：检查加密狗是否存在
  
	* 输入参数：无
		
	* 输出参数：无
		  
	* 返回值  ：加密狗的状态：false表示加密狗不存在，true表示加密狗存在
			
	*/
	bool HarddogCheck();//检查加密狗是否存在


	/*

	* 函数介绍：获得加密狗的ID，并将其输出
  
	* 输入参数：无
		
	* 输出参数：strHarddogID:存放加密狗的ID
		  
	* 返回值  ："0":表示请求已成功完成。
				"1":表示未能登录到加密狗的指定功能；
				"2":表示未能得到加密狗的相关信息，读取加密狗的序列号失败；			
	*/
	int HarddogID(CString &strHarddogID);//获得加密狗的ID，并将其输出


	/*

	* 函数介绍：从加密狗指定内存中读取数据
  
	* 输入参数：offset：偏移量； iSize：字节数
		
	* 输出参数：strValue：读到的字符串；
		  
	* 返回值  ："0":表示请求已成功完成；
				"1":表示未能登录到加密狗的指定功能；
				"2":表示读值失败；
				"3"：表示读取的内存区为空或者内存区的第一个字节为空
				"4":表示输入的offset或者iSize太大；			
	*/
	int HarddogRead( CString &strValue, hasp_size_t offset, int iSize);//从加密狗指定内存中读取数据


	/*

	* 函数介绍：向加密狗指定内存中写入数据
  
	* 输入参数：strValue：需要写入的字符串； offset：偏移量； iSize：字节数
		
	* 输出参数：无
  
	* 返回值  ："0":表示请求已成功完成。
				"1":表示未能登录到加密狗的指定功能；
				"2":表示写值失败；
				"3":表示输入的offset或者iSize太大；
				"4":表示输入字符串strValue的长度小于字节数iSize；
				"5":表示输入字符串strValue的长度大于字节数iSize；				
	*/
	int HarddogWrite(CString strValue, hasp_size_t offset, int size);//向加密狗指定内存中写入数据
	
																	
	/*

	* 函数介绍：将数据加密
  
	* 输入参数： strValue:需要加密的字符串
		
	* 输出参数：strOut:加密后的字符串
		  
	* 返回值  ："0"：加密成功
				"1":表示未能登录到加密狗的指定功能；
				"2"：需要加密的字符串小于16字节
				"3"：加密失败								
	*/
	int HarddogEncrypt(CString strValue, CString &strOut);//将数据加密

	
	/*

	* 函数介绍：将数据解密
  
	* 输入参数： strValue:需要解密的字符串
		
	* 输出参数： strOut:解密后的字符串
		  
	* 返回值  ："0"：解密成功
				"1":表示未能登录到加密狗的指定功能；
				"2"：需要解密的字符串小于16字节
				"3"：解密失败								
	*/
	int HarddogDecrypt(CString strValue, CString &strOut);//将数据解密
	



	/*

	* 函数介绍：读取加密狗的时钟

	* 输入参数：无
 
	* 输出参数：uYear: 加密狗的年份；uMonth: 加密狗的月份； uDay: 加密狗的日期
   
	* 返回值  ："0":表示请求已成功完成。
				"1":表示未能登录到加密狗的指定功能；
				"2":表示读取加密狗时钟失败；		 	 
	*/
	int GetHarddogDate(UINT &uYear, UINT &uMonth, UINT& uDay);//读取加密狗的时钟


	/*

	* 函数介绍：读取加密狗的时钟

	* 输入参数：无
 
	* 输出参数：uHour: 加密狗的时；uMinute: 加密狗的分； uSecond: 加密狗的秒
   
	* 返回值  ："0":表示请求已成功完成。
				"1":表示未能登录到加密狗的指定功能；
				"2":表示读取加密狗时钟失败；		 	 
	*/
	int GetHarddogTime(UINT &uHour, UINT &uMinute, UINT &uSecond);


	/*

	* 函数介绍：在加密狗内存的第0~15字节内保存设定的年，月，日。

	* 输入参数：strYear: 需要保存的年份；strMonth:需要保存的月份； strDay:需要保存的日期
 
	* 输出参数：无
   
	* 返回值  ："0"：表示请求已成功完成。
				"1":表示未能登录到加密狗的指定功能；
				"2":表示保存设定时间的操作失败；
				"3":表示输入的数据不符合常规。
				"4":表示输入字符串的长度小于规定的字节数；
				"5":表示输入字符串的长度大于规定的字节数；
	*/
	int SaveDate(CString strYear, CString strMonth, CString strDay);

	
	
	/*

	* 函数介绍：从加密狗内存的第0~15字节内读取设定的年，月，日。

	* 输入参数：strYear: 读到的年份；strMonth: 读到的月份； strDay: 读到的日期。
	
	* 输出参数：无
   
	 * 返回值  ："0"：表示请求已成功完成。
				 "1":表示未能登录到加密狗的指定功能；
				 "2":表示读值操作失败；			
				 "3":表示读取的内存区为空或者内存区的第一个字节为空；
				 "4":表示设定的年，月，日不符合常规。			 				 
	*/
	int GetDate(CString &strYear, CString &strMonth, CString &strDay);

	/*

	* 函数介绍：在加密狗内存的第16~31，32~47，48~63字节内保存三个密码，每个密码必须小于16字节。

	* 输入参数：iNumber :密码的编号，只能为1或者2或者3；strPassword:需要保存的密码，小于16字节。
 
	* 输出参数：无
   
	* 返回值  ："0":表示请求已成功完成。
				"1":表示未能登录到加密狗的指定功能；
				"2":表示保存密码操作失败；
				"3":表示输入字符串的长度小于规定的字节数；
				"4":表示输入字符串的长度大于规定的字节数；			
	*/
	int SavePassword(int iNumber, CString strPassword);


	/*

	* 函数介绍：从加密狗内存的第16~31，32~47，48~63字节内分别读取密码。

	* 输入参数：iNumber :密码的编号，只能为1或者2或者3；strPassword:读到的密码 。
	
	* 输出参数：无
   
	 * 返回值  ："0"：表示请求已成功完成。
				 "1":表示未能登录到加密狗的指定功能；
				 "2":表示读值操作失败；			
				 "3":表示读取的内存区为空或者内存区的第一个字节为空；	 
	*/
	int GetPassword(int iNumber, CString &strPassword);
	
	/*

	* 函数介绍：得到加密狗的时钟与某个设定的年，月，日的差

	* 输入参数：无 
 
	* 输出参数：iTimeDifference ：加密狗的时钟与某个设定的年，月，日的时间差值，以天为单位
   
	* 返回值  ： "0":表示请求已成功完成。
				 "1":表示未能登录到加密狗的指定功能；
				 "2":表示读值操作失败；			 
				 "3":表示存放时间的内存区为空或者内存区的第一个字节为空；
				 "4":表示读取加密狗当前时钟失败；
	*/
	int GetTimeDifference (int &iTimeDifference);


	/*

	* 函数介绍：在加密狗内存的第64~79字节内保存软件版本号

	* 输入参数：strSoftwareVersion：软件版本号
 
	* 输出参数：无
   
	 * 返回值  ："0"：表示请求已成功完成。
				 "1": 表示未能登录到加密狗的指定功能；
				 "2": 表示保存软件版本号的操作失败；
				 "3":表示输入字符串的长度小于规定的字节数；
				 "4":表示输入字符串的长度大于规定的字节数；			
	*/
	int SetSoftwareVersion(CString strSoftwareVersion);


	/*

	* 函数介绍：从加密狗内存的第64~79字节内读取软件版本号

	* 输入参数：无
 
	* 输出参数：strSoftwareVersion：软件版本号
   
	 * 返回值  ："0"：表示请求已成功完成。
				 "1":表示未能登录到加密狗的指定功能；
				 "2":表示读值操作失败；			
				 "3":表示读取的内存区为空或者内存区的第一个字节为空；	 
	*/
	int GetSoftwareVersion(CString &strSoftwareVersion);

	/*

	* 函数介绍：在加密狗内存的第80字节起，以16字节为一个内存单元，保存数据

	* 输入参数：uIndex: 自加密狗内存的第80字节起，以16字节为单位，从0开始的索引值；strData:需要保存的数据
 
	* 输出参数：无
   
	 * 返回值  ："0"：表示请求已成功完成。
				 "1":表示未能登录到加密狗的指定功能；
				 "2":表示保存数据的操作失败；
				 "3":表示uIndex太大，导致内存越界；
				 "4":表示输入字符串的长度小于规定的字节数；
				 "5":表示输入字符串的长度大于规定的字节数；
	*/
	int SaveData(UINT uIndex, CString strData);


	/*

	* 函数介绍：从加密狗内存的第80字节起，以16字节为一个内存单元，读取数据

	* 输入参数：uIndex: 自加密狗内存的第80字节起，以16字节为单位，从0开始的索引值；strData:读到的数据
 
	* 输出参数：无
   
	* 返回值  ："0"：表示请求已成功完成。
				 "1":表示未能登录到加密狗的指定功能；
				 "2":表示读值操作失败；			
				 "3":表示读取的内存区为空或者内存区的第一个字节为空；
				 "4":表示uIndex太大，导致内存越界；
	*/
	int GetData(UINT uIndex, CString &strData);

};

#endif // !defined(AFX_HARDDOGINFO_H__8A8F2E65_E9F7_4D3D_9592_DCD7FABCE250__INCLUDED_)
