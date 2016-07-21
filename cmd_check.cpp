#include <iostream>
#include <cstdio>
#include <cstdarg>
#include<cstdlib>
#include<cstring>
#include "cmd_check.h"

CCmdCheck::CCmdCheck()
{
	iGenerationNumber = 0;		// �v�Z��
	iAbcDataNum = 0;			// ���q�Q�̐�
	iAbcVectorDimNum = 0;		// �e���q�Q�̓����x�N�g��
	iAbcMethod = 0;				// ���q�Q�œK���@�̎�@�ݒ�
	pcFuncName = NULL;			// �g�p����֐��p�t���O
	iOutputFlag = 0;			// ���ʏo�͗p�t���O
	iIntervalMinNum = 0;		// �Œᔽ����
	iAbcSearchNum = 0;			// employ bee�̑���
	iAbcLimitCount = 0;			// scout bee����ϐ�
	iAbcUpperSearchNum = 0;		// �T���_��ʐ�
	lfConvergenceParam = 0.0;	// ���ւ̎����󋵃p�����[�^
	lfFitBound = 0.0;			// �K���x���e���E�l
	lfFitAccuracy = 0.0;		// �K���x�̎������x
}

CCmdCheck::~CCmdCheck()
{
}

/**
 *<PRE>
 *  ���͂��ꂽ�R�}���h���`�F�b�N
 *  ver 0.1 ����
 *  ver 0.2 �I�v�V�������̒ǉ��A����т��̂ق��̏C���B
 *  ver 0.3 �l�H�m�\�p�O���t�����c�[���쐬�̂��ߐV���ɏC���B
 *</PRE>
 * @param argc	�R�}���h�̓��͍��ڐ�
 * @param argv	���͂����R�}���h�̏ڍ�
 * @return	CCMD_SUCCESS
 * 			CCMD_ERROR_INVALID_FORMAT
 * 			CCMD_ERROR_INVALID_DATA
 * 			CCMD_ERROR_MEMORY_ALLOCATE
 * 			CCMD_ERROR_MEMORY_RELEASE
 * @author kobayashi
 * @since 0.1 2014/05/02
 * @version 0.1
 */
long CCmdCheck::lCommandCheck( int argc, char* argv[] )
{
	int i;
	long lRet = 0;

	/* �R�}���h�̃`�F�b�N */
	if( argc <= 1 )
	{
		return CCMD_ERROR_INVALID_FORMAT;
	}
	if( argv == NULL )
	{
		return CCMD_ERROR_INVALID_FORMAT;
	}
	for( i=1; i<argc ;i++ )
	{
		/* �v�Z�� */
		if( strcmp( argv[i], "-gn" ) == 0 )
		{
			lRet = lCommandErrorCheck( argv[i] );
			if( lRet != 0 ) return lRet;
			iGenerationNumber = atoi( argv[i+1] );
			i++;
		}
		/* colony�̐� */
		else if( strcmp( argv[i], "-an" ) == 0 )
		{
			lRet = lCommandErrorCheck( argv[i] );
			if( lRet != 0 ) return lRet;
			iAbcDataNum = atoi( argv[i+1] );
			i++;
		}
		/* �I�̃x�N�g���� */
		else if( strcmp( argv[i], "-vn" ) == 0 )
		{
			lRet = lCommandErrorCheck( argv[i] );
			if( lRet != 0 ) return lRet;
			iAbcVectorDimNum = atoi( argv[i+1] );
			i++;
		}
		/* employ bee�̑��� */
		else if( strcmp( argv[i], "-sn" ) == 0 )
		{
			lRet = lCommandErrorCheck( argv[i] );
			if( lRet != 0 ) return lRet;
			iAbcSearchNum = atoi( argv[i+1] );
			i++;
		}
		/* scout ����ϐ� */
		else if( strcmp( argv[i], "-cl" ) == 0 )
		{
			lRet = lCommandErrorCheck( argv[i] );
			if( lRet != 0 ) return lRet;
			iAbcLimitCount = atof( argv[i+1] );
			i++;
		}
		/* �K���x��ʂ̒T���_�� */
		else if( strcmp( argv[i], "-usn" ) == 0 )
		{
			lRet = lCommandErrorCheck( argv[i] );
			if( lRet != 0 ) return lRet;
			iAbcUpperSearchNum = atoi( argv[i+1] );
			i++;
		}
		/* �K���x�������x */
		else if( strcmp( argv[i], "-fa" ) == 0 )
		{
			lRet = lCommandErrorCheck( argv[i] );
			if( lRet != 0 ) return lRet;
			lfFitAccuracy = atof( argv[i+1] );
			i++;
		}
		/* �K���x���e���E�l */
		else if( strcmp( argv[i], "-fb" ) == 0 )
		{
			lRet = lCommandErrorCheck( argv[i] );
			if( lRet != 0 ) return lRet;
			lfFitBound = atof( argv[i+1] );
			i++;
		}
		/* �Œᔽ���� */
		else if( strcmp( argv[i], "-imn" ) == 0 )
		{
			lRet = lCommandErrorCheck( argv[i] );
			if( lRet != 0 ) return lRet;
			iIntervalMinNum = atoi( argv[i+1] );
			i++;
		}
		/* ���ւ̎����󋵃p�����[�^ */
		else if( strcmp( argv[i], "-cp" ) == 0 )
		{
			lRet = lCommandErrorCheck( argv[i] );
			if( lRet != 0 ) return lRet;
			lfConvergenceParam = atof( argv[i+1] );
			i++;
		}
		/* ABC�@�̎�@�ݒ� */
		else if( strcmp( argv[i], "-abcm" ) == 0 )
		{
			lRet = lCommandErrorCheck( argv[i] );
			if( lRet != 0 ) return lRet;
			iAbcMethod = atoi( argv[i+1] );
			i++;
		}
		/* �]�������{����ړI�֐� */
		else if( strcmp( argv[i], "-f" ) == 0 )
		{
			lRet = lCommandErrorCheck( argv[i] );
			if( lRet != 0 ) return lRet;
			pcFuncName = new char[strlen(argv[i+1])+1];
			memset( pcFuncName, '\0', sizeof(pcFuncName) );
			strcpy( pcFuncName, argv[i+1] );
			i++;
		}
		/* ��T���͈� */
		else if( strcmp( argv[i], "-r" ) == 0 )
		{
			lRet = lCommandErrorCheck( argv[i] );
			if( lRet != 0 ) return lRet;
			lfRange = atof( argv[i+1] );
			i++;
		}
		/* ���ʏo�� */
		else if( strcmp( argv[i], "-out" ) == 0 )
		{
			lRet = lCommandErrorCheck( argv[i] );
			if( lRet != 0 ) return lRet;
			iOutputFlag = atoi( argv[i+1] );
			i++;
		}
		else
		{
			lRet = CCMD_ERROR_INVALID_DATA;
			break;
		}
	}
	return lRet;
}

/**
 *<PRE>
 * ���̓I�v�V�������ǂ������`�F�b�N����
 * ver 0.1 �V�K�쐬
 * ver 0.2 �l�H�m�\�p�O���t�����c�[���쐬�p�ɏC���B
 *</PRE>
 * @param argc
 * @param argv
 * @return 0
 *		  -1
 *        -2
 * @author kobayashi
 * @since 2013/1/1
 * @version 0.2
 */
long CCmdCheck::lCommandErrorCheck( char *argv )
{
	long lRet = 0L;
	if( ( strcmp( argv, "-gn" ) == 0 ) ||
		( strcmp( argv, "-an" ) == 0 ) ||
		( strcmp( argv, "-vn" ) == 0 ) ||
		( strcmp( argv, "-sn" ) == 0 )  ||
		( strcmp( argv, "-cl" ) == 0 )  ||
		( strcmp( argv, "-usn" ) == 0 )  ||
		( strcmp( argv, "-fa" ) == 0 )  ||
		( strcmp( argv, "-fb" ) == 0 )  ||
		( strcmp( argv, "-imn" ) == 0 ) ||
		( strcmp( argv, "-cp" ) == 0 ) ||
		( strcmp( argv, "-abcm" ) == 0 ) ||
		( strcmp( argv, "-f" ) == 0 ) ||
		( strcmp( argv, "-r" ) == 0 ) ||
		( strcmp( argv, "-out" ) == 0 ))
	{
		lRet = 0;
	}
	else
	{
		lRet = -2;
	}
	return lRet;
}

/**
 *<PRE>
 * �g�p���@��\������B
 *</PRE>
 * @author kobayashi
 * @since 2015/8/3
 * @version 0.2
 */
void CCmdCheck::vHelp()
{
	printf("ABC�@�v�Z\n");
	printf("�g�p���@\n");
	printf("abc [-gn][-an][-vn][-sn][-cl][-usn][-fa][-fb][-imn][-cp][-abcm][-f][-r][-out]\n");
	printf("-gn �X�V��\n");
	printf("-an colony��)\n");
	printf("-vn �I�̃x�N�g����\n");
	printf("-sn employ bee�̐�\n");
	printf("-cl scout bee����ϐ�\n");
	printf("-usn �K���x��ʂ̒T���_��\n");
	printf("-fa �K���x�������x\n");
	printf("-fb �K���x���e���E�l\n");
	printf("-imn �Œᔽ����\n");
	printf("-cp ���ւ̎����󋵃p�����[�^\n");
	printf("-abcm ABC�@�̎�@�ݒ�\n");
	printf("-f �ړI�֐��̐ݒ�\n");
	printf("-r �ړI�֐��̉�T���͈�\n");
	printf("-out ���ʏo��\n");
}

int CCmdCheck::iGetIntervalMinNum()
{
	return iIntervalMinNum;
}

int CCmdCheck::iGetAbcLimitCount()
{
	return iAbcLimitCount;
}

int CCmdCheck::iGetAbcSearchNum()
{
	return iAbcSearchNum;
}

int CCmdCheck::iGetAbcUpperSearchNum()
{
	return iAbcUpperSearchNum;
}

double CCmdCheck::lfGetConvergenceParam()
{
	return lfConvergenceParam;
}

double CCmdCheck::lfGetFitBound()
{
	return lfFitBound;
}

double CCmdCheck::lfGetFitAccuracy()
{
	return lfFitAccuracy;
}

char* CCmdCheck::pcGetFuncName()
{
	return pcFuncName;
}

int CCmdCheck::iGetGenerationNumber()
{
	return iGenerationNumber;
}

int CCmdCheck::iGetAbcDataNum()
{
	return iAbcDataNum;
}

int CCmdCheck::iGetAbcVectorDimNum()
{
	return iAbcVectorDimNum;
}

int CCmdCheck::iGetAbcMethod()
{
	return iAbcMethod;
}

int CCmdCheck::iGetOutputFlag()
{
	return iOutputFlag;
}

double CCmdCheck::lfGetRange()
{
	return lfRange;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////CCmdCheckException�N���X

/**
 * <PRE>
 * �R���X�g���N�^(�G���[����ݒ肷��)
 * </PRE>
 * @author kobayashi
 * @since 2015/6/10
 * @version 1.0
 */
CCmdCheckException::CCmdCheckException()
{
	iErrCode = 0;
	strMethodName = "";
	strClassName = "";
	strErrDetail = "";
}

/**
 * �R���X�g���N�^(�G���[����ݒ肷��)
 * @param iCode       �G���[�R�[�h
 * @param sMethodName �֐���
 * @param sClassName  �N���X��
 * @param sDetail     �G���[�ڍ�
 * @author kobayashi
 * @since 2009/6/14
 * @version 1.0
 */
CCmdCheckException::CCmdCheckException(int iCode, std::string sMethodName, std::string sClassName, std::string sDetail)
{
	iErrCode = iCode;
	strMethodName = sMethodName;
	strClassName = sClassName;
	strErrDetail = sDetail;
}

/**
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
CCmdCheckException::CCmdCheckException(int iCode, std::string sMethodName, std::string sClassName, std::string sDetail, int iLine)
{
	iErrCode = iCode;
	iErrLine = iLine;
	strMethodName = sMethodName;
	strClassName = sClassName;
	strErrDetail = sDetail;
}


/**
 * <PRE>
 * �f�X�g���N�^
 * </PRE>
 * @author kobayashi
 * @since 2015/6/10
 * @version 1.0
 */
CCmdCheckException::~CCmdCheckException()
{
	iErrCode = 0;
	strMethodName = "";
	strClassName = "";
	strErrDetail = "";
}

/**
 * �R���X�g���N�^(�G���[����ݒ肷��)
 * @param iCode       �G���[�R�[�h
 * @param sMethodName �֐���
 * @param sClassName  �N���X��
 * @param sDetail     �G���[�ڍ�
 * @author kobayashi
 * @since 2009/6/14
 * @version 1.0
 */
void CCmdCheckException::SetErrorInfo(int iCode, std::string sMethodName, std::string sClassName, std::string sDetail)
{
	iErrCode = iCode;
	strMethodName = sMethodName;
	strClassName = sClassName;
	strErrDetail = sDetail;
}

/**
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
void CCmdCheckException::SetErrorInfo(int iCode, std::string sMethodName, std::string sClassName, std::string sDetail, int iLine)
{
	iErrCode = iCode;
	iErrLine = iLine;
	strMethodName = sMethodName;
	strClassName = sClassName;
	strErrDetail = sDetail;
}

/**
 * �G���[�ԍ����o�͂���
 * @author kobayashi
 * @since 2009/6/14
 * @version 1.0
 */
int CCmdCheckException::iGetErrCode()
{
	return iErrCode;
}

/**
 * �G���[���N�������s�����o�͂���
 * @author kobayashi
 * @since 2009/6/14
 * @version 1.0
 */
int CCmdCheckException::iGetErrLine()
{
	return iErrLine;
}

/**
 * �G���[���N�������֐������o�͂���
 * @author kobayashi
 * @since 2009/6/14
 * @version 1.0
 */
std::string CCmdCheckException::strGetMethodName()
{
	return strMethodName;
}

/**
 * �G���[���N�������N���X���o�͂���
 * @author kobayashi
 * @since 2009/6/14
 * @version 1.0
 */
std::string CCmdCheckException::strGetClassName()
{
	return strClassName;
}

/**
 * �G���[�̏ڍ׏����o�͂���
 * @author kobayashi
 * @since 2009/6/14
 * @version 1.0
 */
std::string CCmdCheckException::strGetErrDetail()
{
	return strErrDetail;
}
