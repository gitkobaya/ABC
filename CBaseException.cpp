#include"CBaseException.h"
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////CBaseExceptionクラス

CBaseException::CBaseException()
{
	iErrCode = 0;
	strMethodName = "";
	strClassName = "";
	strErrDetail = "";
}

/*
 * コンストラクタ(エラー情報を設定する)
 * @param iCode       エラーコード
 * @param sMethodName 関数名
 * @param sClassName  クラス名
 * @param sDetail     エラー詳細
 * @author kobayashi
 * @since 2009/6/14
 * @version 1.0
 */
CBaseException::CBaseException(int iCode, std::string sMethodName, std::string sClassName, std::string sDetail)
{
	iErrCode		= iCode;
	strMethodName	= sMethodName;
	strClassName	= sClassName;
	strErrDetail	= sDetail;
}

/*
 * コンストラクタ(エラー情報を設定する)
 * @param iCode       エラーコード
 * @param iLine       行数
 * @param sMethodName 関数名
 * @param sClassName  クラス名
 * @param sDetail     エラー詳細
 * @author kobayashi
 * @since 2009/6/14
 * @version 1.0
 */
CBaseException::CBaseException(int iCode, std::string sMethodName, std::string sClassName, std::string sDetail, int iLine )
{
	iErrCode		= iCode;
	iErrLine		= iLine;
	strMethodName	= sMethodName;
	strClassName	= sClassName;
	strErrDetail	= sDetail;
}


CBaseException::~CBaseException()
{
	iErrCode		= 0;
	strMethodName	= "";
	strClassName	= "";
	strErrDetail	= "";
}

/*
 * コンストラクタ(エラー情報を設定する)
 * @param iCode       エラーコード
 * @param sMethodName 関数名
 * @param sClassName  クラス名
 * @param sDetail     エラー詳細
 * @author kobayashi
 * @since 2009/6/14
 * @version 1.0
 */
void CBaseException::SetErrorInfo(int iCode, std::string sMethodName, std::string sClassName, std::string sDetail)
{
	iErrCode = iCode;
	strMethodName = sMethodName;
	strClassName = sClassName;
	strErrDetail = sDetail;
}

/*
 * コンストラクタ(エラー情報を設定する)
 * @param iCode       エラーコード
 * @param iLine       行数
 * @param sMethodName 関数名
 * @param sClassName  クラス名
 * @param sDetail     エラー詳細
 * @author kobayashi
 * @since 2009/6/14
 * @version 1.0
 */
void CBaseException::SetErrorInfo(int iCode, std::string sMethodName, std::string sClassName, std::string sDetail, int iLine )
{
	iErrCode = iCode;
	iErrLine = iLine;
	strMethodName = sMethodName;
	strClassName = sClassName;
	strErrDetail = sDetail;
}

/*
 * エラー番号を出力する
 * @author kobayashi
 * @since 2009/6/14
 * @version 1.0
 */
int CBaseException::iGetErrCode()
{
	return iErrCode;
}

/*
 * エラーを起こした行数を出力する
 * @author kobayashi
 * @since 2009/6/14
 * @version 1.0
 */
int CBaseException::iGetErrorLine()
{
	return iErrLine;
}

/*
 * エラーを起こした関数名を出力する
 * @author kobayashi
 * @since 2009/6/14
 * @version 1.0
 */
std::string CBaseException::strGetMethodName()
{
	return strMethodName;
}

/*
 * エラーを起こしたクラスを出力する
 * @author kobayashi
 * @since 2009/6/14
 * @version 1.0
 */
std::string CBaseException::strGetClassName()
{
	return strClassName;
}

/*
 * エラーの詳細情報を出力する
 * @author kobayashi
 * @since 2009/6/14
 * @version 1.0
 */
std::string CBaseException::strGetErrDetail()
{
	return strErrDetail;
}
