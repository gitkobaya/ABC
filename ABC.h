#ifndef _ABC_H_
#define _ABC_H_

#include<string>

const int ABC_SUCCESS = 0;
const int ABC_FATAL_ERROR = -101;
const int ABC_MEMORY_ALLOCATE_ERROR = -102;
const int ABC_INVALID_ARGUMENT_ERROR = -103;
const int ABC_INVALID_DATA_FORMAT_ERROR = -104;

class CAbc 
{
public:
	/**
	 * <PRE>
	 * �@�R���X�g���N�^
	 * </PRE>
	 * @author kobayashi
	 * @since 2015/8/4
	 * @version 0.1
	 */
	CAbc();

	/**
	 * <PRE>
	 * �@�l���I�R���j�[�̏����������s���܂��B(�R���X�g���N�^)
	 * </PRE>
	 * @param iGenCount       �v�Z��
	 * @param iGenNum         �R���j�[��
	 * @param iGenVectorDim   �T���_�̎�����
	 * @param iSearchNum  �@  �T���_�̐�
	 * @param iLimitCountData �X�V���Ȃ�������
	 * @throw AbcException
	 * @author kobayashi
	 * @since 2015/8/4
	 * @version 0.1
	 */
	CAbc( int iGenCount, int iGenNum, int iGenVectorDim, int iSearchNum, int iLimitCountData );

	/**
	 * <PRE>
	 * �@�l���I�R���j�[�̏����������s���܂��B(�R���X�g���N�^)
	 * </PRE>
	 * @param iGenCount       �v�Z��
	 * @param iGenNum         �R���j�[��
	 * @param iGenVectorDim   �T���_�̎�����
	 * @param iSearchNum  �@  �T���_�̐�
	 * @param iLimitCountData �X�V���Ȃ�������
	 * @param iIntervalMinNum �Œᔽ����
	 * @param lfDrData        �����󋵔���
	 * @param lfBoundData     �K���x���e���E�l
	 * @param lfAccuracyData  �K���x�]�����x
	 * @throw AbcException
	 * @author kobayashi
	 * @since 2015/7/28
	 * @version 0.1
	 */
	CAbc( int iGenCount, int iGenNum, int iGenVectorDim, int iSearchNum, int iLimitCountData, int iIntervalMinNumData, int iAlphaData, double lfDrData, double lfBoundData, double lfAccuracyData );

	/**
	 * <PRE>
	 * �@�f�X�g���N�^
	 * </PRE>
	 * @author kobayashi
	 * @since 2015/6/6
	 * @version 1.0
	 */
	virtual ~CAbc();

	/**
	 * <PRE>
	 * �@�l���I�R���j�[�̏����������s���܂��B
	 * </PRE>
	 * @param iGenCount           �v�Z��
	 * @param iGenNum             �R���j�[��
	 * @param iGenVectorDim       �T���_�̎�����
	 * @param iSearchNum  �@      �T���_�̐�
	 * @param iLimitCountData     �X�V���Ȃ�������
	 * @param iIntervalMinNumData �Œᔽ����
	 * @param iAlphaData          �T���_��ʐ�
	 * @param lfDrData            �����󋵔���
	 * @param lfBoundData         �K���x���e���E�l
	 * @param lfAccuracyData      �K���x�]�����x
	 * @throw AbcException
	 * @author kobayashi
	 * @since 2015/7/28
	 * @version 0.1
	 */
	void vInitialize( int iGenCount, int iGenNum, int iGenVectorDim, int iSearchNum, int iLimitCountData, int iIntervalMinNumData, int iAlphaData, double lfDrData, double lfBoundData, double lfAccuracyData );

	/**
	 * <PRE>
	 * �@�l���I�R���j�[�̏����������s���܂��B
	 * </PRE>
	 * @param iGenCount       �v�Z��
	 * @param iGenNum         �R���j�[��
	 * @param iGenVectorDim   �T���_�̎�����
	 * @param iSearchNum  �@  �T���_�̐�
	 * @param iLimitCountData �X�V���Ȃ�������
	 * @throw AbcException
	 * @author kobayashi
	 * @since 2015/7/28
	 * @version 0.1
	 */
	void vInitialize( int iGenCount, int iGenNum, int iGenVectorDim, int iSearchNum, int iLimitCountData );

	/**
	 * <PRE>
	 * �@�����������ړI�֐��̓o�^���s���܂��B
	 * </PRE>
	 * @param pflfObjectiveFunction �R�[���o�b�N�֐�
	 * @author kobayashi
	 * @since 2015/6/8
	 * @version 1.0
	 */
	void vSetConstraintFunction( double (*pflfObjectiveFunction)( double *plfData, int iVectorLen ) );

	/**
	 * <PRE>
	 * �@�o�^�����R�[���o�b�N�֐��̉�@���������s���܂��B
	 * </PRE>
	 * @author kobayashi
	 * @since 2015/6/8
	 * @version 1.0
	 */
	void vReleaseCallConstraintFunction();

	/**
	 * <PRE>
	 * �@���q����l�����ŏ��������܂��B
	 * </PRE>
	 * @param lfRange ���q�̏����ʒu�̏o���͈�
	 * @author kobayashi
	 * @since 2015/6/8
	 * @version 1.0
	 */
	void vSetRandom( double lfRange );

	/**
	 * <PRE>
	 * �@���q����l�����ŏ��������܂��B(ModifiedAbc�@�p)
	 * </PRE>
	 * @param lfRange ���q�̏����ʒu�̏o���͈�
	 * @author kobayashi
	 * @since 2015/6/8
	 * @version 1.0
	 */
	void vSetModifiedRandom( double lfRange );

	/**
	 * <PRE>
	 * �@�I�����������s���܂��B
	 * </PRE>
	 * @author kobayashi
	 * @since 2015/8/3
	 * @version 1.0
	 */
	void vTerminate();

	/**
	 * <PRE>
	 * �@ABC�@�����s���܂��B
	 * </PRE>
	 * @author kobayashi
	 * @since 2015/6/17
	 * @version 0.1
	 */
	void vAbc();

	/**
	 * <PRE>
	 * �@ABC�@���P��@�����s���܂��B
	 * </PRE>
	 * @author kobayashi
	 * @since 2015/8/3
	 * @version 0.1
	 */
	void vModifiedAbc( int iUpdateCount );

	/**
	 * <PRE>
	 * �@�l�H�I�R���j�[�œK���@�����s���܂��B
	 * </PRE>
	 * @author kobayashi
	 * @since 2016/7/10
	 * @version 0.1
	 */
	void vModified2Abc();

	/**
	 * <PRE>
	 * �@ABC�@���P��@�����s���܂��B
	 * </PRE>
	 * @author kobayashi
	 * @since 2015/8/3
	 * @version 0.1
	 */
	void vModified3Abc( int iUpdateCount );

	/**
	 * <PRE>
	 * �@2013 Memetic search in artificial bee colony algorthim���
	 * </PRE>
	 * @author kobayashi
	 * @since 2015/7/28
	 * @version 0.1
	 */
	void vMeAbc( int iUpdateCount );

	/**
	 * <PRE>
	 * �@�l�H�I�R���j�[�œK���@�i�����𓱓�������@�j�����s���܂��B
	 *   A Novel Hybrid Crossover based Artificial Bee Colony Algorithm for Optimization Problem International Journal of Computer Applications 2013���
	 * </PRE>
	 * @author kobayashi
	 * @since 2016/4/11
	 * @version 0.1
	 */
	void vCbAbc();

	/**
	  *<PRE>
	  * �R�[���o�b�N�֐��̒�`�ł��B
	  *</PRE>
	  */
	double (*pflfObjectiveFunction)( double *pplfData, int iVectorLen );

	/**
	 * <PRE>
	 * �@���q�Q�œK���@��K�p�������ʂ��o�͂��܂��B(�e���q�̈ʒu)
	 * </PRE>
	 * @author kobayashi
	 * @since 2015/6/6
	 * @version 1.0
	 */
	void vOutputAbcData();

	/**
	 * <PRE>
	 * �@���q�Q�œK���@��K�p�������ʂ��o�͂��܂��B(�e���q�̑��x)
	 * </PRE>
	 * @author kobayashi
	 * @since 2015/6/6
	 * @version 1.0
	 */
	void vOutputVelocityData();

	/**
	 * <PRE>
	 * �@���q�Q�œK���@��K�p�������ʂ��o�͂��܂��B(�e���q�̖ړI�֐��l)
	 * </PRE>
	 * @author kobayashi
	 * @since 2015/6/6
	 * @version 1.0
	 */
	void vOutputConstraintFunction();

	/**
	 * <PRE>
	 * �@�����_�ł̍ő�l���q�̈ʒu���o�͂��܂��B
	 * </PRE>
	 * @author kobayashi
	 * @since 2015/6/19
	 * @version 1.0
	 */
	void vOutputGlobalMaxAbcData();

	/**
	 * <PRE>
	 * �@�����_�ł̍ő�l���q�̖ړI�֐��l���o�͂��܂��B
	 * </PRE>
	 * @author kobayashi
	 * @since 2015/6/19
	 * @version 1.0
	 */
	void vOutputGlobalMaxAbcDataConstFuncValue();

	/**
	 * <PRE>
	 * �@�����_�ł̍ŏ��l���q�̈ʒu���o�͂��܂��B
	 * </PRE>
	 * @author kobayashi
	 * @since 2015/6/19
	 * @version 1.0
	 */
	void vOutputGlobalMinAbcData();

	/**
	 * <PRE>
	 * �@�����_�ł̍ŏ��l���q�̖ړI�֐��l���o�͂��܂��B
	 * </PRE>
	 * @author kobayashi
	 * @since 2015/6/19
	 * @version 1.0
	 */
	void vOutputGlobalMinAbcDataConstFuncValue();

	/**
	 * <PRE>
	 * �@�����_�ł̂����Ƃ��悢���q�ʒu�Ƃ��̑��̗��q�Ƃ̋������o�͂��܂��B
	 * </PRE>
	 * @param iOutFlag �o�͕��@���w�肷��t���O
	 * @author kobayashi
	 * @since 2015/6/19
	 * @version 1.0
	 */
	void vOutputAbcDataLocDist( int iOutFlag );
	
	/**
	 * <PRE>
	 * �@�����_�ł̊e���q���Ƃ̍ŗǈʒu���o�͂��܂��B
	 * </PRE>
	 * @param iFlag 0 �ړI�֐��l���o�͂��Ȃ��B
	 * �@�@�@�@�@�@ 1 �ړI�֐��l���o�͂���B
	 * @author kobayashi
	 * @since 2015/7/6
	 * @version 0.2
	 */
	void vOutputLocalMaxAbcData( int iOutFlag );

	void vSetRange( double lfRange );

	void swap(double *swap1,double *swap2);
	void qsort(int start,int end, double *sort );
	double lfNormalRand( double lfSigma, double lfMean );

private:
	int iGenerationNumber;				// �v�Z��
	int iAbcDataNum;					// �R���j�[�̐�
	int iAbcVectorDimNum;				// �I�̓����x�N�g��
	int iAbcSearchNum;					// �T���_��
	int iAbcLimitCount;					// �X�V���Ȃ�������
	int iAbcIntervalMinNum;				// �Œᔽ����
	int iAbcUpperSearchNum;				// �T���_��ʐ�
	double lfConvergenceParam;			// �����󋵃p�����[�^
	double lfFitBound;					// �K���x���e���E�l
	double lfFitAccuracy;				// �K���x�]�����x
	int *piLocalMaxAbcLoc;				// 
	double **pplfAbcData;				// ABC�f�[�^�z��
	double **pplfNAbcData;				// ABC�X�V�p�f�[�^�z��
	double lfGlobalMaxAbcData;			// ���I�œK�l
	double lfGlobalMinAbcData;			// ���I�ŏ��l
	double *plfGlobalMaxAbcData;		// ���I�œK����\�����q�̃f�[�^
	double *plfGlobalMinAbcData;		// ���I�œK����\�����q�̃f�[�^
	double *plfLocalMaxAbcData;			// �Ǐ��œK����\�����q�̃f�[�^
	double lfLocalMaxAbcData;			// �Ǐ��œK�l
	double **pplfLocalMaxAbcData;		// �Ǐ��œK����\�����q���Ƃ̍ő�l���i�[����f�[�^
	double *plfLocalMaxObjectiveAbcData;// �Ǐ��œK����\�����q�̃f�[�^
	double **pplfVelocityData;			// ABC�f�[�^���x�z��
	double **pplfNVelocityData;			// ABC�X�V�p�f�[�^���x�z��
	double *plfFitProb;					// �K���x���Ίm��
	double *plfFit;						// �K���x
	int *piNonUpdateCount;				// �X�V���Ȃ���
	int *piTotalNonUpdateCount;			// �X�V���Ȃ�����
	double *plfVelocity;				// ���[���b�g�I���ɂ��I�����鑬�x�x�N�g��
	double *plfCrossOverData;			// �����ꎞ�i�[�p�z��
	double lfSolveRange;				// �o�͔͈�
	double lfFitInit;					// ���ϕ]���l
	double lfFitCurrentBest;			// ���݂̍ŗǒl�̓K���x
	double *plfXnew1;					// Memetic Algorithm�p�X�V�z��
	double *plfXnew2;					// Memetic Algorithm�p�X�V�z��
};

class CAbcException
{
public:
	CAbcException();
	CAbcException( int iCode, std::string sMethodName, std::string sClassName, std::string sDetail );
	CAbcException( int iCode, std::string sMethodName, std::string sClassName, std::string sDetail, int iErrorLine );
	~CAbcException();
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

#endif /* _ABC_H_ */
