#ifndef _CBASEEXCEPTION_H_
#define _CBASEEXCEPTION_H_

#include<string>
#include<ostream>

class CBaseException
{
public:
	CBaseException();
	CBaseException( int iCode, std::string sMethodName, std::string sClassName, std::string sDetail );
	CBaseException( int iCode, std::string sMethodName, std::string sClassName, std::string sDetail, int iLine );
	~CBaseException();
	void SetErrorInfo( int iCode, std::string sMethodName, std::string sClassName, std::string sDetail );
	void SetErrorInfo( int iCode, std::string sMethodName, std::string sClassName, std::string sDetail, int iLine );
	int iGetErrCode();
	int iGetErrorLine();
	std::string strGetMethodName();
	std::string strGetClassName();
	std::string strGetErrDetail();
private:
	int iErrCode;
	int iErrLine;
	std::string strMethodName;
	std::string strClassName;
	std::string strModuleName;
	std::string strErrDetail;
};

#endif _CBASEEXCEPTION_H_
