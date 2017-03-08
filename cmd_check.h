#ifndef _CCMD_CHECK_H_
#define _CCMD_CHECK_H_

#include<string>

#define CCMD_SUCCESS							 0
#define CCMD_ERROR_INVALID_FORMAT				-1
#define CCMD_ERROR_INVALID_DATA					-2
#define CCMD_ERROR_MEMORY_ALLOCATE				-3
#define CCMD_ERROR_MEMORY_RELEASE				-4

class CCmdCheck
{
public:
	CCmdCheck();
	virtual ~CCmdCheck();
	
	/**
	 *<PRE>
	 * ���͂��ꂽ�R�}���h�����
	 *</PRE>
	 * @author kobayashi
	 * @param argc
	 * @param argv
	 * @version 1.0
	 */
	long lCommandCheck( int argc, char* argv[] );

	/**
	 *<PRE>
	 * ���͂��ꂽ�R�}���h���t�H�[�}�b�g�ɓK���Ă��Ȃ��ꍇ�A�G���[��ԋp
	 *</PRE>
	 * @author kobayashi
	 * @param argc
	 * @param argv
	 * @version 1.0
	 */
	long lCommandErrorCheck( char* argv );
	
	/**
	 *<PRE>
	 * �g�p���@��\������B
	 *</PRE>
	 * @author kobayashi
	 * @since 2015/6/10
	 * @version 0.2
	 */
	void vHelp();

public:

	int iGetIntervalMinNum();
	int iGetAbcLimitCount();
	int iGetAbcSearchNum();
	double iGetBestWeight();
	int iGetAbcUpperSearchNum();
	double lfGetConvergenceParam();
	double lfGetFitBound();
	double lfGetFitAccuracy();
	char* pcGetFuncName();
	char* pcGetConditionName();
	int iGetGenerationNumber();
	int iGetAbcDataNum();
	int iGetAbcVectorDimNum();
	int iGetAbcMethod();
	int iGetOutputFlag();
	double lfGetRange();
	double lfGetRangeMin();
	double lfGetRangeMax();
	int iGetCrossOverNum();
	double lfGetAlpha();
	double lfGetBeta();
	double lfGetLearningRate();
	int iGetParentNum();
	int iGetChildrenNum();
	int iGetUpperEvalChildrenNum();
	int iGetFinishFlag();

private:
	int iGenerationNumber;		// �v�Z��
	int iAbcDataNum;		// ���q�Q�̐�
	int iAbcVectorDimNum;		// �e���q�Q�̓����x�N�g��
	int iAbcMethod;			// ���q�Q�œK���@�̎�@�ݒ�
	char* pcFuncName;		// �g�p����֐��p�t���O
	char* pcConditionName;	// �g�p���鐧������̃t���O
	int iOutputFlag;		// ���ʏo�͗p�t���O
	int iIntervalMinNum;		// �Œᔽ����
	int iAbcSearchNum;		// employ bee�̑���
	int iAbcLimitCount;		// scout bee����ϐ�
	int iAbcUpperSearchNum;		// �T���_��ʐ�
	double lfConvergenceParam;	// ���ւ̎����󋵃p�����[�^
	double lfFitBound;		// �K���x���e���E�l
	double lfFitAccuracy;		// �K���x�̎������x
	double lfRange;			// ���̒T���͈�
	double lfRangeMin;			// ���̒T���͈͂̍ŏ��l
	double lfRangeMax;			// ���̒T���͈͂̍ő�l
	int iCrossOverNum;		// ������
	double lfAlpha;			// UNDX�p�����[�^�̃�
	double lfBeta;			// UNDX�p�����[�^�̃�
	double lfLearningRate;
	int iParentNum;
	int iChildrenNum;
	int iUpperEvalChildrenNum;
	int iFinishFlag;		// �I������
};

class CCmdCheckException
{
public:
	CCmdCheckException();
	CCmdCheckException( int iCode, std::string sMethodName, std::string sClassName, std::string sDetail );
	CCmdCheckException( int iCode, std::string sMethodName, std::string sClassName, std::string sDetail, int iErrorLine );
	~CCmdCheckException();
	void SetErrorInfo( int iCode, std::string sMethodName, std::string sClassName, std::string sDetail );
	void SetErrorInfo( int iCode, std::string sMethodName, std::string sClassName, std::string sDetail, int iErrorLine );
	int iGetErrCode();
	int iGetErrLine();
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

#endif /*_CCMDCHECK_H_*/
