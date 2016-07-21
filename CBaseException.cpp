#include"CBaseException.h"
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////CBaseException�N���X

CBaseException::CBaseException()
{
	iErrCode = 0;
	strMethodName = "";
	strClassName = "";
	strErrDetail = "";
}

/*
 * �R���X�g���N�^(�G���[����ݒ肷��)
 * @param iCode       �G���[�R�[�h
 * @param sMethodName �֐���
 * @param sClassName  �N���X��
 * @param sDetail     �G���[�ڍ�
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
 * �R���X�g���N�^(�G���[����ݒ肷��)
 * @param iCode       �G���[�R�[�h
 * @param iLine       �s��
 * @param sMethodName �֐���
 * @param sClassName  �N���X��
 * @param sDetail     �G���[�ڍ�
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
 * �R���X�g���N�^(�G���[����ݒ肷��)
 * @param iCode       �G���[�R�[�h
 * @param sMethodName �֐���
 * @param sClassName  �N���X��
 * @param sDetail     �G���[�ڍ�
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
 * �R���X�g���N�^(�G���[����ݒ肷��)
 * @param iCode       �G���[�R�[�h
 * @param iLine       �s��
 * @param sMethodName �֐���
 * @param sClassName  �N���X��
 * @param sDetail     �G���[�ڍ�
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
 * �G���[�ԍ����o�͂���
 * @author kobayashi
 * @since 2009/6/14
 * @version 1.0
 */
int CBaseException::iGetErrCode()
{
	return iErrCode;
}

/*
 * �G���[���N�������s�����o�͂���
 * @author kobayashi
 * @since 2009/6/14
 * @version 1.0
 */
int CBaseException::iGetErrorLine()
{
	return iErrLine;
}

/*
 * �G���[���N�������֐������o�͂���
 * @author kobayashi
 * @since 2009/6/14
 * @version 1.0
 */
std::string CBaseException::strGetMethodName()
{
	return strMethodName;
}

/*
 * �G���[���N�������N���X���o�͂���
 * @author kobayashi
 * @since 2009/6/14
 * @version 1.0
 */
std::string CBaseException::strGetClassName()
{
	return strClassName;
}

/*
 * �G���[�̏ڍ׏����o�͂���
 * @author kobayashi
 * @since 2009/6/14
 * @version 1.0
 */
std::string CBaseException::strGetErrDetail()
{
	return strErrDetail;
}
