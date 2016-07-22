#include<cstdio>
#include<cmath>
#include<cfloat>
#include<memory>
#include<ctime>
#include<algorithm>

#include"mseq.h"
#include"ABC.h"
#include"constdata.h"
#include"cdfrand.h"

/**
 * <PRE>
 * �@�R���X�g���N�^
 * </PRE>
 * @author kobayashi
 * @since 2015/6/6
 * @version 1.0
 */
CAbc::CAbc()
{
	iGenerationNumber = 0;				// �v�Z��
	iAbcDataNum = 0;				// ���q�Q�̐�
	iAbcVectorDimNum = 0;				// �e���q�Q�̓����x�N�g��
	iAbcSearchNum = 0;				// �T���_�̑���
	pplfAbcData = NULL;				// ABC�f�[�^�z��
	pplfNAbcData = NULL;				// ABC�X�V�p�f�[�^�z��
	pplfVelocityData = NULL;			// ABC�f�[�^���x�z��
	pplfNVelocityData = NULL;			// ABC�X�V�p�f�[�^���x�z��
	piNonUpdateCount = NULL;			// �X�V����Ȃ�������
	plfVelocity = NULL;				// ���[���b�g�I���ɂ��I�����鑬�x�x�N�g��
	plfGlobalMaxAbcData = NULL;
	plfGlobalMinAbcData = NULL;			// ���I�œK����\�����q�̃f�[�^
	plfCrossOverData = NULL;
	plfLocalMaxAbcData = NULL;
	lfSolveRange = 0.0;
	lfGlobalMinAbcData = DBL_MAX;
	lfGlobalMaxAbcData = -DBL_MAX;
}

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
CAbc::CAbc( int iGenCount, int iGenNum, int iGenVectorDim, int iSearchNum, int iLimitCountData )
{
	vInitialize( iGenCount, iGenNum, iGenVectorDim, iSearchNum, iLimitCountData );
}

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
CAbc::CAbc( int iGenCount, int iGenNum, int iGenVectorDim, int iSearchNum, int iLimitCountData, int iIntervalMinNumData, int iAlphaData, double lfDrData, double lfBoundData, double lfAccuracyData )
{
	vInitialize( iGenCount, iGenNum, iGenVectorDim, iSearchNum, iLimitCountData, iIntervalMinNumData, iAlphaData, lfDrData, lfBoundData, lfAccuracyData );
}

/**
 * <PRE>
 * �@�f�X�g���N�^
 * </PRE>
 * @author kobayashi
 * @since 2015/6/6
 * @version 1.0
 */
CAbc::~CAbc()
{
}

/**
 * <PRE>
 * �@�l���I�R���j�[�̏����������s���܂��B
 * </PRE>
 * @param iGenCount       �v�Z��
 * @param iGenNum         �R���j�[��
 * @param iGenVectorDim   �T���_�̎�����
 * @param iSearchNum  �@  �T���_�̐�
 * @param iLimitCountData �X�V���Ȃ�������
 * @param iIntervalMinNum �Œᔽ����
 * @param iAlphaData      �T���_��ʐ�
 * @param lfDrData        �����󋵔���
 * @param lfBoundData     �K���x���e���E�l
 * @param lfAccuracyData  �K���x�]�����x
 * @throw AbcException
 * @author kobayashi
 * @since 2015/7/28
 * @version 0.1
 */
void CAbc::vInitialize( int iGenCount, int iGenNum, int iGenVectorDim, int iSearchNum, int iLimitCountData, int iIntervalMinNumData, int iAlphaData, double lfDrData, double lfBoundData, double lfAccuracyData )
{
	CAbcException cae;

	int i,j;

	iGenerationNumber	= iGenCount;
	iAbcIntervalMinNum	= iIntervalMinNumData;
	iAbcDataNum		= iGenNum;
	iAbcVectorDimNum	= iGenVectorDim;
	iAbcSearchNum		= iSearchNum;
	iAbcLimitCount		= iLimitCountData;
	iAbcUpperSearchNum	= iAlphaData;
	lfConvergenceParam	= lfDrData;
	lfFitBound		= lfBoundData;
	lfFitAccuracy		= lfAccuracyData;

	initrand(time(NULL));
	initrnd();

	try
	{
		pplfAbcData = new double*[iAbcDataNum];
		for( i = 0;i < iAbcDataNum; i++ )
		{
			pplfAbcData[i] = new double[iAbcVectorDimNum];
		}
		pplfNAbcData = new double*[iAbcDataNum];
		for( i = 0;i < iAbcDataNum; i++ )
		{
			pplfNAbcData[i] = new double[iAbcVectorDimNum];
		}
		pplfLocalMaxAbcData = new double*[iAbcDataNum];
		for( i = 0;i < iAbcDataNum; i++ )
		{
			pplfLocalMaxAbcData[i] = new double[iAbcVectorDimNum];
		}
		pplfVelocityData = new double*[iAbcDataNum];
		for( i = 0;i < iAbcDataNum; i++ )
		{
			pplfVelocityData[i] = new double[iAbcVectorDimNum];
		}
		pplfNVelocityData = new double*[iAbcDataNum];
		for( i = 0;i < iAbcDataNum; i++ )
		{
			pplfNVelocityData[i] = new double[iAbcVectorDimNum];
		}
		plfFit = new double[iAbcSearchNum];
		plfFitProb = new double[iAbcSearchNum];
		piNonUpdateCount = new int[iAbcSearchNum];
		plfVelocity = new double[iAbcVectorDimNum];
		plfGlobalMaxAbcData = new double[iAbcVectorDimNum];
		plfGlobalMinAbcData = new double[iAbcVectorDimNum];
		plfCrossOverData = new double[iAbcVectorDimNum];

		for( i= 0;i < iAbcDataNum; i++ )
		{
			for(j = 0;j < iAbcVectorDimNum; j++ )
			{
				pplfAbcData[i][j] = 0.0;
				pplfNAbcData[i][j] = 0.0;
				pplfLocalMaxAbcData[i][j] = 0.0;
				pplfVelocityData[i][j] = 0.0;
				pplfNVelocityData[i][j] = 0.0;
			}
		}
		for( i = 0;i < iAbcSearchNum ; i++ )
		{
			plfFit[i] = 0.0;
			plfFitProb[i] = 0.0;
			piNonUpdateCount[i] = 0;
		}
		for( i = 0;i < iAbcVectorDimNum; i++ )
		{
			plfVelocity[i] = 0.0;
			plfGlobalMaxAbcData[i] = 0.0;
			plfGlobalMinAbcData[i] = 0.0;
			plfCrossOverData[i] = 0.0;
		}
	}
	catch( std::bad_alloc ba )
	{
		cae.SetErrorInfo( ABC_MEMORY_ALLOCATE_ERROR, "vInitialize", "CAbc", "�������m�ۂɎ��s���܂����B", __LINE__ );
		throw( cae );
	}
	catch(...)
	{
		cae.SetErrorInfo( ABC_FATAL_ERROR, "vInitialize", "CAbc", "�v���I�G���[���������܂����B", __LINE__ );
		throw( cae );
	}
}

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
void CAbc::vInitialize( int iGenCount, int iGenNum, int iGenVectorDim, int iSearchNum, int iLimitCountData )
{
	CAbcException cae;

	int i,j;

	iGenerationNumber	= iGenCount;
	iAbcDataNum			= iGenNum;
	iAbcVectorDimNum	= iGenVectorDim;
	iAbcSearchNum		= iSearchNum;
	iAbcLimitCount		= iLimitCountData;
	initrand(time(NULL));
	initrnd();

	try
	{
		pplfAbcData = new double*[iAbcDataNum];
		for( i = 0;i < iAbcDataNum; i++ )
		{
			pplfAbcData[i] = new double[iAbcVectorDimNum];
		}
		pplfNAbcData = new double*[iAbcDataNum];
		for( i = 0;i < iAbcDataNum; i++ )
		{
			pplfNAbcData[i] = new double[iAbcVectorDimNum];
		}
		pplfLocalMaxAbcData = new double*[iAbcDataNum];
		for( i = 0;i < iAbcDataNum; i++ )
		{
			pplfLocalMaxAbcData[i] = new double[iAbcVectorDimNum];
		}
		pplfVelocityData = new double*[iAbcDataNum];
		for( i = 0;i < iAbcDataNum; i++ )
		{
			pplfVelocityData[i] = new double[iAbcVectorDimNum];
		}
		pplfNVelocityData = new double*[iAbcDataNum];
		for( i = 0;i < iAbcDataNum; i++ )
		{
			pplfNVelocityData[i] = new double[iAbcVectorDimNum];
		}
		plfFit = new double[iAbcSearchNum];
		plfFitProb = new double[iAbcSearchNum];
		piNonUpdateCount = new int[iAbcSearchNum];
		plfVelocity = new double[iAbcVectorDimNum];
		plfGlobalMaxAbcData = new double[iAbcVectorDimNum];
		plfGlobalMinAbcData = new double[iAbcVectorDimNum];
		plfCrossOverData = new double[iAbcVectorDimNum];

		for( i= 0;i < iAbcDataNum; i++ )
		{
			for(j = 0;j < iAbcVectorDimNum; j++ )
			{
				pplfAbcData[i][j] = 0.0;
				pplfNAbcData[i][j] = 0.0;
				pplfLocalMaxAbcData[i][j] = 0.0;
				pplfVelocityData[i][j] = 0.0;
				pplfNVelocityData[i][j] = 0.0;
			}
		}
		for( i = 0;i < iAbcSearchNum ; i++ )
		{
			plfFit[i] = 0.0;
			plfFitProb[i] = 0.0;
			piNonUpdateCount[i] = 0;
		}
		for( i = 0;i < iAbcVectorDimNum; i++ )
		{
			plfVelocity[i] = 0.0;
			plfGlobalMaxAbcData[i] = 0.0;
			plfGlobalMinAbcData[i] = 0.0;
			plfCrossOverData[i] = 0.0;
		}
	}
	catch( std::bad_alloc ba )
	{
		cae.SetErrorInfo( ABC_MEMORY_ALLOCATE_ERROR, "vInitialize", "CAbc", "�������m�ۂɎ��s���܂����B", __LINE__ );
		throw( cae );
	}
	catch(...)
	{
		cae.SetErrorInfo( ABC_FATAL_ERROR, "vInitialize", "CAbc", "�v���I�G���[���������܂����B", __LINE__ );
		throw( cae );
	}
}

/**
 * <PRE>
 * �@�l�H�I�R���j�[�̏����ʒu�𒆐S0�̔��arange���ň�l�����ɂ��ݒ肵�܂��B
 * </PRE>
 * @param lfRange ���q�̏����ʒu�̏o���͈�
 * @author kobayashi
 * @since 2015/7/28
 * @version 0.1
 */
void CAbc::vSetRandom( double lfRange )
{
	int i,j;
	double lfMin = DBL_MAX;
	int iMinLoc = 0;
	double lfObjFunc = 0.0;
	for( i= 0;i < iAbcDataNum; i++ )
	{
		for(j = 0;j < iAbcVectorDimNum; j++ )
		{
			pplfAbcData[i][j] = lfRange*(2.0*rnd()-1.0);
			pplfVelocityData[i][j] = lfRange*(2.0*rnd()-1.0);
			pplfLocalMaxAbcData[i][j] = pplfAbcData[i][j];
		}
	}
	lfSolveRange = lfRange;

	// �����̏�ԂōœK�l���擾���܂��B
	// �X�V���܂��B
	for( i = 0;i < iAbcSearchNum; i++ )
	{
		lfObjFunc = pflfObjectiveFunction( pplfAbcData[i], iAbcVectorDimNum );
		if( lfGlobalMinAbcData >= lfObjFunc )
		{
			iMinLoc = j;
			lfGlobalMinAbcData = lfObjFunc;
			for( j = 0; j < iAbcVectorDimNum; j++ )
			{
				plfGlobalMinAbcData[j] = pplfAbcData[i][j];
			}
		}
		if( lfGlobalMaxAbcData <= lfObjFunc )
		{
			lfGlobalMaxAbcData = lfObjFunc;
			for( j = 0; j < iAbcVectorDimNum; j++ )
			{
				plfGlobalMaxAbcData[j] = pplfAbcData[i][j];
			}
		}
	}
}

/**
 * <PRE>
 * �@�l�H�I�R���j�[�̏����ʒu�𒆐S0�̔��arange���ň�l�����ɂ��ݒ肵�܂��B
 * </PRE>
 * @param lfRange ���q�̏����ʒu�̏o���͈�
 * @author kobayashi
 * @since 2015/7/28
 * @version 0.1
 */
void CAbc::vSetModifiedRandom( double lfRange )
{
	int i,j;
	double lfMin = DBL_MAX;
	int iMinLoc = 0;
	double lfObjFunc = 0.0;
	for( i= 0;i < iAbcDataNum; i++ )
	{
		for(j = 0;j < iAbcVectorDimNum; j++ )
		{
			pplfAbcData[i][j] = lfRange*(2.0*rnd()-1.0);
			pplfVelocityData[i][j] = lfRange*(2.0*rnd()-1.0);
			pplfLocalMaxAbcData[i][j] = pplfAbcData[i][j];
		}
	}
	lfSolveRange = lfRange;

	// �����̏�ԂōœK�l���擾���܂��B
	// �X�V���܂��B
	for( i = 0;i < iAbcSearchNum; i++ )
	{
		lfObjFunc = pflfObjectiveFunction( pplfAbcData[i], iAbcVectorDimNum );
		if( lfGlobalMinAbcData >= lfObjFunc )
		{
			iMinLoc = j;
			lfGlobalMinAbcData = lfObjFunc;
			for( j = 0; j < iAbcVectorDimNum; j++ )
			{
				plfGlobalMinAbcData[j] = pplfAbcData[i][j];
			}
			lfFitCurrentBest = lfMin;
		}
		if( lfGlobalMaxAbcData <= lfObjFunc )
		{
			lfGlobalMaxAbcData = lfObjFunc;
			for( j = 0; j < iAbcVectorDimNum; j++ )
			{
				plfGlobalMaxAbcData[j] = pplfAbcData[i][j];
			}
			lfFitCurrentBest = lfMin;
		}
	}

	lfObjFunc = 0.0;
	// ������Ԃɂ�����O�T���_�̕��ϕ]���l���Z�o���܂��B
	for( i = 0;i < iAbcSearchNum; i++ )
	{
		lfObjFunc += pflfObjectiveFunction( pplfAbcData[i], iAbcVectorDimNum );
	}
	lfFitInit = lfObjFunc/(double)iAbcSearchNum;
}

/**
 * <PRE>
 * �@�I�����������s���܂��B
 * </PRE>
 * @throw CAbcException 
 * @author kobayashi
 * @since 2015/7/28
 * @version 0.1
 */
void CAbc::vTerminate()
{
	int i;
	CAbcException cae;
	try
	{
		if( pplfAbcData != NULL )
		{
			for( i = 0;i < iAbcDataNum; i++ )
			{
				delete[] pplfAbcData[i];
				pplfAbcData[i] = NULL;
			}
			delete[] pplfAbcData;
			pplfAbcData = NULL;
		}
		if( pplfNAbcData != NULL )
		{
			for( i = 0;i < iAbcDataNum; i++ )
			{
				delete[] pplfNAbcData[i];
				pplfNAbcData[i] = NULL;
			}
			delete[] pplfNAbcData;
			pplfNAbcData = NULL;
		}
		if( pplfVelocityData != NULL )
		{
			for( i = 0;i < iAbcDataNum; i++ )
			{
				delete[] pplfVelocityData[i];
				pplfVelocityData[i] = NULL;
			}
			delete[] pplfVelocityData;
			pplfVelocityData = NULL;
		}
		if( pplfNVelocityData != NULL )
		{
			for( i = 0;i < iAbcDataNum; i++ )
			{
				delete[] pplfNVelocityData[i];
				pplfNVelocityData[i] = NULL;
			}
			delete[] pplfNVelocityData;
			pplfNVelocityData = NULL;
		}
		if( plfFit != NULL )
		{
			delete[] plfFit;
			plfFit = NULL;
		}
		if( plfFitProb != NULL )
		{
			delete[] plfFitProb;
			plfFitProb = NULL;
		}
		if( piNonUpdateCount != NULL )
		{
			delete[] piNonUpdateCount;
			piNonUpdateCount = NULL;
		}
		if( plfVelocity != NULL )
		{
			delete[] plfVelocity;
			plfVelocity = NULL;
		}
		if( plfCrossOverData != NULL )
		{
			delete[] plfCrossOverData;
			plfCrossOverData = NULL;
		}
		if( plfGlobalMinAbcData != NULL )
		{
			delete[] plfGlobalMinAbcData;
			plfGlobalMinAbcData = NULL;
		}
		if( plfGlobalMaxAbcData != NULL )
		{
			delete[] plfGlobalMaxAbcData;
			plfGlobalMaxAbcData = NULL;
		}
		
	}
	catch(...)
	{
		cae.SetErrorInfo( ABC_FATAL_ERROR, "vTerminate", "CAbc", "����������Ɏ��s���܂����B", __LINE__ );
		throw( cae );
	}
}

/**
 * <PRE>
 * �@�ړI�֐����C���X�g�[�����܂��B
 * �@���ۂɂ̓R�[���o�b�N�֐����C���X�g�[�����܂��B
 * </PRE>
 * @param pflfFunction �ړI�֐��̊֐��|�C���^
 * @author kobayashi
 * @since 2015/7/28
 * @version 0.1
 */
void CAbc::vSetConstraintFunction( double (*pflfFunction)( double *plfData, int iVectorLen ) )
{
	pflfObjectiveFunction = pflfFunction;
}

/**
 * <PRE>
 * �@�ړI�֐����A���C���X�g�[�����܂��B
 * �@���ۂɂ̓R�[���o�b�N�֐����A���C���X�g�[�����܂��B
 * </PRE>
 * @author kobayashi
 * @since 2015/7/28
 * @version 0.1
 */
void CAbc::vReleaseCallConstraintFunction()
{
	pflfObjectiveFunction = NULL;
}

/**
 * <PRE>
 * �@�l�H�I�R���j�[�œK���@�����s���܂��B
 * </PRE>
 * @author kobayashi
 * @since 2015/7/28
 * @version 0.1
 */
void CAbc::vAbc()
{
	int i,j,k,m,h,c;
	int iRankCount = 0;
	int iLocalMaxAbcLoc = 0;
	int iGlobalMaxAbcLoc = 0;
	int iMinLoc = 0;
	double lfTempAbcData = -DBL_MAX;
	double lfTempWeight = 0.0;
	double lfMin = 0.0;
	double lfObjFunc = 0.0;
	double lfRand = 0.0;
	double lfFunc1 = 0.0;
	double lfFunc2 = 0.0;
	double lfRes = 0.0;
	double lfPrevProb = 0.0;
	double lfProb = 0.0;
	double lfFitProb = 0.0;
	int icNonUpdateCount = 0;

	// employee bee �̓���
	// �X�V�_�����Z�o���܂��B
	m = mrand() % ( iAbcSearchNum - 1 );
	h = mrand() % ( iAbcVectorDimNum - 1);
	
	for( i = 0;i < iAbcSearchNum; i++ )
	{
		lfRand = 2*rnd()-1;
		for( j = 0; j < iAbcVectorDimNum; j++ )
			pplfVelocityData[i][j] = pplfAbcData[i][j];
		pplfVelocityData[i][h] = pplfAbcData[i][h] + lfRand*( pplfAbcData[i][h] - pplfAbcData[m][h] );
	}

	// �e�T���_�ƍX�V���Ȃ������񐔂��i�[����ϐ����X�V���܂��B
	for( i = 0;i < iAbcSearchNum; i++ )
	{
		lfFunc1 = pflfObjectiveFunction( pplfVelocityData[i], iAbcVectorDimNum );
		lfFunc2 = pflfObjectiveFunction( pplfAbcData[i], iAbcVectorDimNum );

		if( lfFunc1 < lfFunc2 )
		{
			for( j = 0;j < iAbcVectorDimNum; j++ )
				pplfAbcData[i][j] = pplfVelocityData[i][j];
			piNonUpdateCount[i] = 0;
		}
		else piNonUpdateCount[i] = piNonUpdateCount[i] + 1;
	}

	// onlookers bee�̓���
	for( i = 0;i < iAbcDataNum-iAbcSearchNum;  i++ )
	{
		lfRes = 0.0;
		for(j = 0;j < iAbcSearchNum; j++ )
		{
			// �K���x�̎Z�o
			lfObjFunc = pflfObjectiveFunction( pplfAbcData[j], iAbcVectorDimNum );
			if( lfObjFunc >= 0.0 ) lfFitProb = 1.0/( 1.0+lfObjFunc );
			else		       lfFitProb = 1.0+fabs( lfObjFunc );
			lfRes += lfFitProb;
			plfFit[j] = lfFitProb;
		}
		// �K���x�̐��K��
		for( j = 0;j < iAbcSearchNum; j++ ) plfFitProb[j] = plfFit[j]/lfRes;
		
		// ���[���b�g�헪�����s
		lfPrevProb = 0.0;
		lfProb = 0.0;
		lfRand = rnd();
		c = 0;
		for( j = 0;j < iAbcSearchNum; j++ )
		{
			lfProb += plfFitProb[j];
			if( lfPrevProb <= lfRand && lfRand <= lfProb ) c = j;
			lfPrevProb = lfProb;
		}

		// ���[���b�g�I�������T���_�ɑ΂��čX�V�������߂܂��B

		// �X�V�_�����Z�o����B
		// �X�V�_���𗐐��ɂ�茈�肷��B
		m = mrand() % ( iAbcSearchNum-1 );
		h = mrand() % ( iAbcVectorDimNum - 1);

		lfRand = 2*rnd()-1;
		for( j = 0; j < iAbcVectorDimNum; j++ ) pplfVelocityData[c][j] = pplfAbcData[c][j];
		pplfVelocityData[c][h] = pplfAbcData[c][h] + lfRand*( pplfAbcData[c][h] - pplfAbcData[m][h] );
		// �X�V�_�������̂悤�ɍX�V���܂��B
		lfFunc1 = pflfObjectiveFunction( pplfVelocityData[c], iAbcVectorDimNum );
		lfFunc2 = pflfObjectiveFunction( pplfAbcData[c], iAbcVectorDimNum );

		if( lfFunc1 < lfFunc2 )
		{
			for( j = 0;j < iAbcVectorDimNum; j++ ) pplfAbcData[c][j] = pplfVelocityData[c][j];
			piNonUpdateCount[c] = 0;
		}
		else piNonUpdateCount[c] = piNonUpdateCount[c] + 1;
	}

	// scout bee �̎��s
	// �V���ȒT���_�����߂ĒT�������s���܂��B
	for( i = 0;i < iAbcSearchNum; i++ )
	{
		if( piNonUpdateCount[i] > iAbcLimitCount )
		{
			for( k = 0;k < iAbcVectorDimNum; k++ )
			{
				lfRand = lfSolveRange*(2*rnd()-1.0);
				pplfAbcData[i][k] = lfRand;
			}
		}
		else
		{
			for( k = 0;k < iAbcVectorDimNum; k++ )
			{
				pplfAbcData[i][k] = pplfAbcData[i][k];
			}
		}
	}
	
//	lfMin = pflfObjectiveFunction( plfGlobalMaxAbcData, iAbcVectorDimNum );
//	lfMin = DBL_MAX;
	// �X�V���܂��B
	for( i = 0;i < iAbcSearchNum; i++ )
	{
		lfObjFunc = pflfObjectiveFunction( pplfAbcData[i], iAbcVectorDimNum );
		if( lfGlobalMinAbcData >= lfObjFunc )
		{
			iMinLoc = i;
			lfGlobalMinAbcData = lfObjFunc;
			for( j = 0; j < iAbcVectorDimNum; j++ )
			{
				plfGlobalMinAbcData[j] = pplfAbcData[i][j];
			}
		}
	}
}

/**
 * <PRE>
 * �@�l�H�I�R���j�[�œK���@�����s���܂��B
 * </PRE>
 * @author kobayashi
 * @since 2015/7/28
 * @version 0.1
 */
void CAbc::vModified2Abc()
{
	int i,j,k,m,h,c;
	int iRankCount = 0;
	int iLocalMaxAbcLoc = 0;
	int iGlobalMaxAbcLoc = 0;
	int iMinLoc = 0;
	double lfTempAbcData = -DBL_MAX;
	double lfTempWeight = 0.0;
	double lfMin = 0.0;
	double lfObjFunc = 0.0;
	double lfRand = 0.0;
	double lfRand2 = 0.0;
	double lfFunc1 = 0.0;
	double lfFunc2 = 0.0;
	double lfRes = 0.0;
	double lfPrevProb = 0.0;
	double lfProb = 0.0;
	double lfFitProb = 0.0;
	int icNonUpdateCount = 0;

	double lfSigma = 0.0;

	// employee bee �̓���
	// �X�V�_�����Z�o���܂��B
	m = mrand() % ( iAbcSearchNum - 1 );
	h = mrand() % ( iAbcVectorDimNum - 1);
	
	lfSigma = 1.0/(double)iAbcSearchNum;
//	lfSigma = sqrt((double)iAbcSearchNum);
	for( i = 0;i < iAbcSearchNum; i++ )
	{
//		lfRand = lfNormalRand( lfSigma, 0.0 );
		lfRand = 2.0*rnd()-1.0;
		lfRand2 = 2.0*rnd()-1.0;
		for( j = 0; j < iAbcVectorDimNum; j++ )
			pplfVelocityData[i][j] = pplfAbcData[i][j];
//		pplfVelocityData[i][h] = pplfAbcData[i][h] + lfRand*( pplfAbcData[i][h] - pplfAbcData[m][h] );
		pplfVelocityData[i][h] = pplfAbcData[i][h] + lfRand*( pplfAbcData[i][h] - pplfAbcData[m][h] ) + lfRand2*( pplfAbcData[i][h] - pplfLocalMaxAbcData[m][h] );
	}

	// �e�T���_�ƍX�V���Ȃ������񐔂��i�[����ϐ����X�V���܂��B
	for( i = 0;i < iAbcSearchNum; i++ )
	{
		lfFunc1 = pflfObjectiveFunction( pplfVelocityData[i], iAbcVectorDimNum );
		lfFunc2 = pflfObjectiveFunction( pplfAbcData[i], iAbcVectorDimNum );

		if( lfFunc1 < lfFunc2 )
		{
			for( j = 0;j < iAbcVectorDimNum; j++ )
				pplfAbcData[i][j] = pplfVelocityData[i][j];
			piNonUpdateCount[i] = 0;
		}
		else 	piNonUpdateCount[i] = piNonUpdateCount[i] + 1;
	}

	// onlookers bee�̓���

	for( i = 0;i < iAbcDataNum-iAbcSearchNum;  i++ )
	{
		lfRes = 0.0;
		for(j = 0;j < iAbcSearchNum; j++ )
		{
			// �K���x�̎Z�o
			lfObjFunc = pflfObjectiveFunction( pplfAbcData[j], iAbcVectorDimNum );
			if( lfObjFunc >= 0.0 )	lfFitProb = 1.0/( 1.0+lfObjFunc );
			else					lfFitProb = 1.0+fabs( lfObjFunc );
			lfRes += lfFitProb;
			plfFit[j] = lfFitProb;
		}
		// �K���x�̐��K��
		for( j = 0;j < iAbcSearchNum; j++ ) plfFitProb[j] = plfFit[j]/lfRes;
		// ���[���b�g�헪�����s
		lfPrevProb = 0.0;
		lfProb = 0.0;
		lfRand = rnd();
		c = 0;
		for( j = 0;j < iAbcSearchNum; j++ )
		{
			lfProb += plfFitProb[j];
			if( lfPrevProb <= lfRand && lfRand <= lfProb )	c = j;
			lfPrevProb = lfProb;
		}

		// ���[���b�g�I�������T���_�ɑ΂��čX�V�������߂܂��B

		// �X�V�_�����Z�o����B
		// �X�V�_���𗐐��ɂ�茈�肷��B
		m = mrand() % ( iAbcSearchNum-1 );
		h = mrand() % ( iAbcVectorDimNum - 1);

		lfSigma = 1.0/(double)iAbcSearchNum;
//		lfRand = lfNormalRand( lfSigma, 0.0 );
		lfRand = 2.0*rnd()-1.0;
		lfRand2 = 2.0*rnd()-1.0;
		for( j = 0; j < iAbcVectorDimNum; j++ )
			pplfVelocityData[c][j] = pplfAbcData[c][j];
		pplfVelocityData[c][h] = pplfAbcData[c][h] + lfRand*( pplfAbcData[c][h] - pplfAbcData[m][h] ) + lfRand2*( pplfAbcData[c][h] - plfGlobalMinAbcData[h] );
		// �X�V�_�������̂悤�ɍX�V���܂��B
		lfFunc1 = pflfObjectiveFunction( pplfVelocityData[c], iAbcVectorDimNum );
		lfFunc2 = pflfObjectiveFunction( pplfAbcData[c], iAbcVectorDimNum );

		if( lfFunc1 < lfFunc2 )
		{
			for( j = 0;j < iAbcVectorDimNum; j++ )
				pplfAbcData[c][j] = pplfVelocityData[c][j];
			piNonUpdateCount[c] = 0;
		}
		else 	piNonUpdateCount[c] = piNonUpdateCount[c] + 1;
	}

	// scout bee �̎��s
	// �V���ȒT���_�����߂ĒT�������s���܂��B
	for( i = 0;i < iAbcSearchNum; i++ )
	{
		if( piNonUpdateCount[i] > iAbcLimitCount )
		{
			for( k = 0;k < iAbcVectorDimNum; k++ )
			{
				lfRand = rnd();
				pplfAbcData[i][k] = plfGlobalMinAbcData[k] + lfRand*( plfGlobalMaxAbcData[k]-plfGlobalMinAbcData[k] );
			}
		}
		else
		{
			for( k = 0;k < iAbcVectorDimNum; k++ )
			{
				pplfAbcData[i][k] = pplfAbcData[i][k];
			}
		}
	}

	// ���[�J���̍ő�l���X�V���܂��B
	for( i = 0;i < iAbcSearchNum; i++ )
	{
		lfFunc1 = pflfObjectiveFunction( pplfLocalMaxAbcData[i], iAbcVectorDimNum );
		lfFunc2 = pflfObjectiveFunction( pplfAbcData[i], iAbcVectorDimNum );

		if( lfFunc1 > lfFunc2 )
		{
			for( j = 0;j < iAbcVectorDimNum; j++ )
				pplfLocalMaxAbcData[i][j] = pplfAbcData[i][j];
		}
	}
	
//	lfGlobalMinAbcData = pflfObjectiveFunction( plfGlobalMinAbcData, iAbcVectorDimNum );
	// �X�V���܂��B
	for( i = 0;i < iAbcSearchNum; i++ )
	{
		lfObjFunc = pflfObjectiveFunction( pplfAbcData[i], iAbcVectorDimNum );
		if( lfGlobalMinAbcData >= lfObjFunc )
		{
			iMinLoc = i;
			lfGlobalMinAbcData = lfObjFunc;
			for( j = 0; j < iAbcVectorDimNum; j++ )
				plfGlobalMinAbcData[j] = pplfAbcData[i][j];
		}
		if( lfGlobalMaxAbcData <= lfObjFunc )
		{
			iMinLoc = i;
			lfGlobalMaxAbcData = lfObjFunc;
			for( j = 0; j < iAbcVectorDimNum; j++ )
				plfGlobalMaxAbcData[j] = pplfAbcData[i][j];
		}
	}
}

/**
 * <PRE>
 * �@�l�H�I�R���j�[�œK���@�����s���܂��B
 * </PRE>
 * @author kobayashi
 * @since 2015/7/28
 * @version 0.1
 */
void CAbc::vModifiedAbc( int iUpdateCount )
{
	int i,j,c,m,h;
	double lfObjFunc = 0.0;
	double lfFitProb = 0.0;
	double lfFitJudge = 0.0;
	double lfFunc1 = 0.0;
	double lfFunc2 = 0.0;
	double lfRes = 0.0;
	double lfPrevProb = 0.0;
	double lfProb = 0.0;
	double lfRand = 0.0;
	double lfMin = 0.0;
	int iMinLoc = 0.0;

	// employed bees�ɂ��T�������{���܂��B
	for( i = 0;i < iAbcSearchNum; i++ )
	{
		// �K���x�̎Z�o
		lfObjFunc = pflfObjectiveFunction( pplfAbcData[i], iAbcVectorDimNum );
		if( lfObjFunc-lfFitBound >= lfFitAccuracy )	lfFitProb = 1.0/( lfObjFunc-lfFitBound );
		else										lfFitProb = 1.0/lfFitAccuracy;
		plfFit[i] = lfFitProb;
	}
	// �K���x�̃\�[�g�����s���܂��B
	qsort( 0, iAbcSearchNum, plfFit );

	// 
	if( iUpdateCount >= iAbcIntervalMinNum )
	{
		if( lfFitJudge < lfConvergenceParam )
		{
			// Fjudge�̒l���X�V���܂��B
			lfFitJudge = ( lfFitInit - lfFitCurrentBest ) / ( lfFitInit - lfFitBound );
			if( lfFitJudge >= lfConvergenceParam )
			{
				// �e�T���_�̑��Ε]���m�����Z�o���܂��B
				lfRes = 0.0;
				for( i = 0;i < iAbcSearchNum; i++ )	lfRes += plfFit[i]; 
				for( i = 0;i < iAbcSearchNum; i++ )	plfFitProb[i] = plfFit[i]/lfRes;
			}
			else
			{
				// �X�V�_�����Z�o���܂��B
				if( iUpdateCount >= iAbcIntervalMinNum && lfFitJudge >= lfConvergenceParam )
				{
					// ���[���b�g�헪�ɂ��Am�̒l�����肵�܂��B
					lfProb = lfPrevProb = 0.0;
					lfRand = rnd();
					for( j = 0;j < iAbcSearchNum; j++ )
					{
						lfProb += plfFitProb[j];
						if( lfPrevProb <= lfRand && lfRand <= lfProb ) m = j;
						lfPrevProb = lfProb;
					}
				}
				else
				{
					// �K���x��ʃ����烉���_���Ɍ��肵�܂��B
					m = mrand() % ( iAbcSearchNum - 1 - iAbcUpperSearchNum ) + iAbcUpperSearchNum;
				}
				// �����_���Ɍ��肵�܂��B
				h = mrand() % ( iAbcVectorDimNum - 1);
	
				for( i = 0;i < iAbcSearchNum; i++ )
				{
					lfRand = 2*rnd()-1;
					for( j = 0; j < iAbcVectorDimNum; j++ )
						pplfVelocityData[i][j] = pplfAbcData[i][j];
					pplfVelocityData[i][h] = pplfAbcData[i][h] + lfRand*( pplfAbcData[i][h] - pplfAbcData[m][h] );
				}
			}
		}
		else
		{
			// �e�T���_�̑��Ε]���m�����Z�o���܂��B
			lfRes = 0.0;
			for( i = 0;i < iAbcSearchNum; i++ )	lfRes += plfFit[i]; 
			for( i = 0;i < iAbcSearchNum; i++ )	plfFitProb[i] = plfFit[i]/lfRes;

			// �X�V�_�����Z�o���܂��B
			if( iUpdateCount >= iAbcIntervalMinNum && lfFitJudge >= lfConvergenceParam )
			{
				// ���[���b�g�헪�ɂ��Am�̒l�����肵�܂��B
				lfProb = lfPrevProb = 0.0;
				lfRand = rnd();
				for( j = 0;j < iAbcSearchNum; j++ )
				{
					lfProb += plfFitProb[j];
					if( lfPrevProb <= lfRand && lfRand <= lfProb )	m = j;
					lfPrevProb = lfProb;
				}
			}
			else
			{
				// ���̑��̏ꍇ�̓����_���Ɍ��肵�܂��B
				m = mrand() % ( iAbcSearchNum - 1 - iAbcUpperSearchNum ) + iAbcUpperSearchNum;
			}
			// �����_���Ɍ��肵�܂��B
			h = mrand() % ( iAbcVectorDimNum - 1);
	
			for( i = 0;i < iAbcSearchNum; i++ )
			{
				lfRand = 2*rnd()-1;
				for( j = 0; j < iAbcVectorDimNum; j++ )
					pplfVelocityData[i][j] = pplfAbcData[i][j];
				pplfVelocityData[i][h] = pplfAbcData[i][h] + lfRand*( pplfAbcData[i][h] - pplfAbcData[m][h] );
			}
		}
	}
	else
	{
		// �X�V�_�����Z�o���܂��B
		if( iUpdateCount >= iAbcIntervalMinNum && lfFitJudge >= lfConvergenceParam )
		{
			// ���[���b�g�헪�ɂ��Am�̒l�����肵�܂��B
			lfProb = lfPrevProb = 0.0;
			lfRand = rnd();
			for( j = 0;j < iAbcSearchNum; j++ )
			{
				lfProb += plfFitProb[j];
				if( lfPrevProb <= lfRand && lfRand <= lfProb )	m = j;
				lfPrevProb = lfProb;
			}
		}
		else
		{
			// ���̑��̏ꍇ�̓����_���Ɍ��肵�܂��B
			m = mrand() % ( iAbcSearchNum - 1 - iAbcUpperSearchNum ) + iAbcUpperSearchNum;
		}
		// �����_���Ɍ��肵�܂��B
		h = mrand() % ( iAbcVectorDimNum - 1);

		for( i = 0;i < iAbcSearchNum; i++ )
		{
			lfRand = 2.0*rnd()-1.0;
			for( j = 0; j < iAbcVectorDimNum; j++ )
				pplfVelocityData[i][j] = pplfAbcData[i][j];
			pplfVelocityData[i][h] = pplfAbcData[i][h] + lfRand*( pplfAbcData[i][h] - pplfAbcData[m][h] );
		}
	}

	// �e�T���_���X�V���܂��B
	for( i = 0;i < iAbcSearchNum; i++ )
	{
		lfFunc1 = pflfObjectiveFunction( pplfVelocityData[i], iAbcVectorDimNum );
		lfFunc2 = pflfObjectiveFunction( pplfAbcData[i], iAbcVectorDimNum );
		if( lfFunc1 < lfFunc2 )
		{
			for( j = 0;j < iAbcVectorDimNum; j++ )
				pplfAbcData[i][j] = pplfVelocityData[i][j];
		}
	}

	// onlooker bees�ɂ��T��

	// �X�V�_�����Z�o���܂��B
	if( iUpdateCount >= iAbcIntervalMinNum && lfFitJudge >= lfConvergenceParam )
	{
		// ���[���b�g�헪�ɂ��Am�̒l�����肵�܂��B
		lfProb = lfPrevProb = 0.0;
		lfRand = rnd();
		c = 0;
		for( j = 0;j < iAbcSearchNum; j++ )
		{
			lfProb += plfFitProb[j];
			if( lfPrevProb <= lfRand && lfRand <= lfProb )	c = j;
			lfPrevProb = lfProb;
		}
	}
	else
	{
		// ���̑��̏ꍇ�̓����_���Ɍ��肵�܂��B
		c = mrand() % ( iAbcSearchNum - 1 - iAbcUpperSearchNum ) + iAbcUpperSearchNum;
	}
	// �����_���Ɍ��肵�܂��B
	h = mrand() % ( iAbcVectorDimNum - 1);

	// �X�V�_���𐶐����܂��B
	for( i = 0;i < iAbcSearchNum; i++ )
	{
		lfRand = 2*rnd()-1;
		for( j = 0; j < iAbcVectorDimNum; j++ )
			pplfVelocityData[i][j] = pplfAbcData[i][j];
		pplfVelocityData[i][h] = pplfAbcData[i][h] + lfRand*( pplfAbcData[i][h] - pplfAbcData[c][h] );
	}
	// �e�T���_���X�V���܂��B
	lfFunc1 = pflfObjectiveFunction( pplfVelocityData[c], iAbcVectorDimNum );
	lfFunc2 = pflfObjectiveFunction( pplfAbcData[c], iAbcVectorDimNum );
	if( lfFunc1 < lfFunc2 )
	{
		for( j = 0;j < iAbcVectorDimNum; j++ )
			pplfAbcData[c][j] = pplfVelocityData[c][j];
	}

	// �ŗǓ_�̍X�V�����܂��B
	// �����̏�ԂōœK�l���擾���܂��B
	lfMin = DBL_MAX;
	// �X�V���܂��B
	for( i = 0;i < iAbcSearchNum; i++ )
	{
		lfObjFunc = pflfObjectiveFunction( pplfAbcData[i], iAbcVectorDimNum );
		if( lfGlobalMinAbcData >= lfObjFunc )
		{
			iMinLoc = j;
			lfGlobalMinAbcData = lfObjFunc;
			for( j = 0; j < iAbcVectorDimNum; j++ )
			{
				plfGlobalMinAbcData[j] = pplfAbcData[i][j];
			}
			lfFitCurrentBest = lfMin;
		}
	}
}

/**
 * <PRE>
 * �@�l�H�I�R���j�[�œK���@�����s���܂��B
 * </PRE>
 * @author kobayashi
 * @since 2015/7/28
 * @version 0.1
 */
void CAbc::vModified3Abc( int iUpdateCount )
{
	int i,j,k,m,h,c;
	int iRankCount = 0;
	int iLocalMaxAbcLoc = 0;
	int iGlobalMaxAbcLoc = 0;
	int iMinLoc = 0;
	double lfTempAbcData = -DBL_MAX;
	double lfTempWeight = 0.0;
	double lfMin = 0.0;
	double lfObjFunc = 0.0;
	double lfRand = 0.0;
	double lfRand2 = 0.0;
	double lfFunc1 = 0.0;
	double lfFunc2 = 0.0;
	double lfRes = 0.0;
	double lfPrevProb = 0.0;
	double lfProb = 0.0;
	double lfFitProb = 0.0;
	int icNonUpdateCount = 0;

	double lfFai = 0.0;
	double lfK = 0.0;
	double lfCoe1 = 2.05;
	double lfCoe2 = 2.05;
	double lfWeight = 0.0;
	double lfMaxWeight = 1.1;
	double lfMinWeight = 0.4;

	double lfSigma = 0.0;

	lfFai = lfCoe1 + lfCoe2;
	if( lfFai > 4.0 )
	{
		lfK = 2.0/(fabs(2.0-lfFai - sqrt( lfFai*lfFai-4.0*lfFai ) )  );
	}
	else
	{
		lfK = 1.0;
	}
	printf("%d\n",iGenerationNumber);
	iGenerationNumber = 100000;
	lfWeight = lfMaxWeight - (lfMaxWeight-lfMinWeight)/(double)iGenerationNumber*(double)iUpdateCount;
	printf("%lf\n",lfK);
	// employee bee �̓���
	// �X�V�_�����Z�o���܂��B
	m = mrand() % ( iAbcSearchNum - 1 );
	h = mrand() % ( iAbcVectorDimNum - 1);
	
	lfSigma = 1.0/(double)iAbcSearchNum;
//	lfSigma = sqrt((double)iAbcSearchNum);
	for( i = 0;i < iAbcSearchNum; i++ )
	{
//		lfRand = lfNormalRand( lfSigma, 0.0 );
		lfRand = 2.0*rnd()-1.0;
		lfRand2 = 2.0*rnd()-1.0;
		for( j = 0; j < iAbcVectorDimNum; j++ )
			pplfVelocityData[i][j] = pplfAbcData[i][j];
//		pplfVelocityData[i][h] = pplfAbcData[i][h] + lfRand*( pplfAbcData[i][h] - pplfAbcData[m][h] );
		pplfVelocityData[i][h] = lfK*( pplfAbcData[i][h]*lfWeight + lfCoe1*lfRand*( pplfAbcData[i][h] - pplfAbcData[m][h] ) + lfCoe2*lfRand2*( pplfAbcData[i][h] - pplfLocalMaxAbcData[m][h] ));
	}

	// �e�T���_�ƍX�V���Ȃ������񐔂��i�[����ϐ����X�V���܂��B
	for( i = 0;i < iAbcSearchNum; i++ )
	{
		lfFunc1 = pflfObjectiveFunction( pplfVelocityData[i], iAbcVectorDimNum );
		lfFunc2 = pflfObjectiveFunction( pplfAbcData[i], iAbcVectorDimNum );

		if( lfFunc1 < lfFunc2 )
		{
			for( j = 0;j < iAbcVectorDimNum; j++ )
				pplfAbcData[i][j] = pplfVelocityData[i][j];
			piNonUpdateCount[i] = 0;
		}
		else 	piNonUpdateCount[i] = piNonUpdateCount[i] + 1;
	}

	// onlookers bee�̓���

	for( i = 0;i < iAbcDataNum-iAbcSearchNum;  i++ )
	{
		lfRes = 0.0;
		for(j = 0;j < iAbcSearchNum; j++ )
		{
			// �K���x�̎Z�o
			lfObjFunc = pflfObjectiveFunction( pplfAbcData[j], iAbcVectorDimNum );
			if( lfObjFunc >= 0.0 )	lfFitProb = 1.0/( 1.0+lfObjFunc );
			else					lfFitProb = 1.0+fabs( lfObjFunc );
			lfRes += lfFitProb;
			plfFit[j] = lfFitProb;
		}
		// �K���x�̐��K��
		for( j = 0;j < iAbcSearchNum; j++ ) plfFitProb[j] = plfFit[j]/lfRes;
		// ���[���b�g�헪�����s
		lfPrevProb = 0.0;
		lfProb = 0.0;
		lfRand = rnd();
		c = 0;
		for( j = 0;j < iAbcSearchNum; j++ )
		{
			lfProb += plfFitProb[j];
			if( lfPrevProb <= lfRand && lfRand <= lfProb )	c = j;
			lfPrevProb = lfProb;
		}

		// ���[���b�g�I�������T���_�ɑ΂��čX�V�������߂܂��B

		// �X�V�_�����Z�o����B
		// �X�V�_���𗐐��ɂ�茈�肷��B
		m = mrand() % ( iAbcSearchNum-1 );
		h = mrand() % ( iAbcVectorDimNum - 1);

		lfSigma = 1.0/(double)iAbcSearchNum;
//		lfRand = lfNormalRand( lfSigma, 0.0 );
		lfRand = 2.0*rnd()-1.0;
		lfRand2 = 2.0*rnd()-1.0;
		for( j = 0; j < iAbcVectorDimNum; j++ )
			pplfVelocityData[c][j] = pplfAbcData[c][j];
		pplfVelocityData[c][h] = lfWeight*pplfAbcData[c][h] + lfCoe1*lfRand*( pplfAbcData[c][h] - pplfAbcData[m][h] ) + lfCoe2*lfRand2*( pplfAbcData[c][h] - plfGlobalMinAbcData[h] );
		// �X�V�_�������̂悤�ɍX�V���܂��B
		lfFunc1 = pflfObjectiveFunction( pplfVelocityData[c], iAbcVectorDimNum );
		lfFunc2 = pflfObjectiveFunction( pplfAbcData[c], iAbcVectorDimNum );

		if( lfFunc1 < lfFunc2 )
		{
			for( j = 0;j < iAbcVectorDimNum; j++ )
				pplfAbcData[c][j] = pplfVelocityData[c][j];
			piNonUpdateCount[c] = 0;
		}
		else 	piNonUpdateCount[c] = piNonUpdateCount[c] + 1;
	}

	// scout bee �̎��s
	// �V���ȒT���_�����߂ĒT�������s���܂��B
	for( i = 0;i < iAbcSearchNum; i++ )
	{
		if( piNonUpdateCount[i] > iAbcLimitCount )
		{
			for( k = 0;k < iAbcVectorDimNum; k++ )
			{
				lfRand = rnd();
				pplfAbcData[i][k] = plfGlobalMinAbcData[k] + lfCoe1*lfRand*( plfGlobalMaxAbcData[k]-plfGlobalMinAbcData[k] );
			}
		}
		else
		{
			for( k = 0;k < iAbcVectorDimNum; k++ )
			{
				pplfAbcData[i][k] = pplfAbcData[i][k];
			}
		}
	}

	// ���[�J���̍ő�l���X�V���܂��B
	for( i = 0;i < iAbcSearchNum; i++ )
	{
		lfFunc1 = pflfObjectiveFunction( pplfLocalMaxAbcData[i], iAbcVectorDimNum );
		lfFunc2 = pflfObjectiveFunction( pplfAbcData[i], iAbcVectorDimNum );

		if( lfFunc1 > lfFunc2 )
		{
			for( j = 0;j < iAbcVectorDimNum; j++ )
				pplfLocalMaxAbcData[i][j] = pplfAbcData[i][j];
		}
	}
	
//	lfGlobalMinAbcData = pflfObjectiveFunction( plfGlobalMinAbcData, iAbcVectorDimNum );
	// �X�V���܂��B
	for( i = 0;i < iAbcSearchNum; i++ )
	{
		lfObjFunc = pflfObjectiveFunction( pplfAbcData[i], iAbcVectorDimNum );
		if( lfGlobalMinAbcData >= lfObjFunc )
		{
			iMinLoc = i;
			lfGlobalMinAbcData = lfObjFunc;
			for( j = 0; j < iAbcVectorDimNum; j++ )
				plfGlobalMinAbcData[j] = pplfAbcData[i][j];
		}
		if( lfGlobalMaxAbcData <= lfObjFunc )
		{
			iMinLoc = i;
			lfGlobalMaxAbcData = lfObjFunc;
			for( j = 0; j < iAbcVectorDimNum; j++ )
				plfGlobalMaxAbcData[j] = pplfAbcData[i][j];
		}
	}
}

/**
 * <PRE>
 * �@�l�H�I�R���j�[�œK���@�i�����𓱓�������@�j�����s���܂��B
 *   A Novel Hybrid Crossover based Artificial Bee Colony Algorithm for Optimization Problem International Journal of Computer Applications 2013���
 * </PRE>
 * @author kobayashi
 * @since 2016/4/11
 * @version 0.1
 */
void CAbc::vCbAbc()
{
	int i,j,k,m,h,c;
	int iRankCount = 0;
	int iLocalMaxAbcLoc = 0;
	int iGlobalMaxAbcLoc = 0;
	int iMinLoc = 0;
	double lfTempAbcData = -DBL_MAX;
	double lfTempWeight = 0.0;
	double lfMin = 0.0;
	double lfObjFunc = 0.0;
	double lfRand = 0.0;
	double lfFunc1 = 0.0;
	double lfFunc2 = 0.0;
	double lfRes = 0.0;
	double lfPrevProb = 0.0;
	double lfProb = 0.0;
	double lfFitProb = 0.0;
	int icNonUpdateCount = 0;

	int iParentLoc1;
	int iParentLoc2;

	// employee bee �̓���
	// �X�V�_�����Z�o���܂��B
	m = mrand() % ( iAbcSearchNum - 1 );
	h = mrand() % ( iAbcVectorDimNum - 1);
	
	for( i = 0;i < iAbcSearchNum; i++ )
	{
		lfRand = 2*rnd()-1;
		for( j = 0; j < iAbcVectorDimNum; j++ )
		{
			if( j != h )
			{
				pplfVelocityData[i][j] = pplfAbcData[i][j];
			}
			else
			{
				pplfVelocityData[i][h] = pplfAbcData[i][h] + lfRand*( pplfAbcData[i][h] - pplfAbcData[m][h] );
			}
		}
	}

	// �e�T���_�ƍX�V���Ȃ������񐔂��i�[����ϐ����X�V���܂��B
	for( i = 0;i < iAbcSearchNum; i++ )
	{
		lfFunc1 = pflfObjectiveFunction( pplfVelocityData[i], iAbcVectorDimNum );
		lfFunc2 = pflfObjectiveFunction( pplfAbcData[i], iAbcVectorDimNum );

		if( lfFunc1 < lfFunc2 )
		{
			for( j = 0;j < iAbcVectorDimNum; j++ )
			{
				pplfAbcData[i][j] = pplfVelocityData[i][j];
			}
			piNonUpdateCount[i] = 0;
		}
		else
		{
			piNonUpdateCount[i] = piNonUpdateCount[i] + 1;
		}
	}

	// crossover�����s���܂��B(�����Ƃ������̎����l�f�`�̌���)
	iParentLoc1 = mrand() % (iAbcSearchNum-1);
	iParentLoc2 = mrand() % (iAbcSearchNum-1);

	for( i = 0;i < iAbcVectorDimNum; i++ )
	{
		plfCrossOverData[i] = 0.5*( pplfAbcData[iParentLoc1][i] + pplfAbcData[iParentLoc2][i] );
//		plfCrossOverData[i] = 1.5*pplfAbcData[iParentLoc1][i] + 0.5*pplfAbcData[iParentLoc2][i];
//		plfCrossOverData[i] = -0.5*pplfAbcData[iParentLoc1][i] + 1.5*pplfAbcData[iParentLoc2][i];
	}
	
	// �����_���ɑI�������e�̒��ŕ]���֐��̒l���������̂Ɛ��������q�����������܂��B
	lfFunc1 = pflfObjectiveFunction( pplfAbcData[iParentLoc1], iAbcVectorDimNum );
	lfFunc2 = pflfObjectiveFunction( pplfAbcData[iParentLoc2], iAbcVectorDimNum );
	if( lfFunc1 < lfFunc2 )
	{
		for( i = 0;i < iAbcVectorDimNum; i++ )
		{
			pplfAbcData[iParentLoc1][i] = plfCrossOverData[i];
		}
	}
	else
	{
		for( i = 0;i < iAbcVectorDimNum; i++ )
		{
			pplfAbcData[iParentLoc2][i] = plfCrossOverData[i];
		}
	}


	// onlookers bee�̓���

	for( i = 0;i < iAbcDataNum-iAbcSearchNum;  i++ )
	{
		lfRes = 0.0;
		for(j = 0;j < iAbcSearchNum; j++ )
		{
			// �K���x�̎Z�o
			lfObjFunc = pflfObjectiveFunction( pplfAbcData[j], iAbcVectorDimNum );
			if( lfObjFunc >= 0.0 )
			{
				lfFitProb = 1.0/( 1.0+lfObjFunc );
			}
			else
			{
				lfFitProb = 1.0+fabs( lfObjFunc );
			}
			lfRes += lfFitProb;
			plfFit[j] = lfFitProb;
		}
		// �K���x�̐��K��
		for( j = 0;j < iAbcSearchNum; j++ )
		{
			plfFitProb[j] = plfFit[j]/lfRes;
		}
		// ���[���b�g�헪�����s
		lfPrevProb = 0.0;
		lfProb = 0.0;
		lfRand = rnd();
		c = 0;
		for( j = 0;j < iAbcSearchNum; j++ )
		{
			lfProb += plfFitProb[j];
			if( lfPrevProb <= lfRand && lfRand <= lfProb )
			{
				c = j;
			}
			lfPrevProb = lfProb;
		}

		// ���[���b�g�I�������T���_�ɑ΂��čX�V�������߂܂��B

		// �X�V�_�����Z�o����B
		// �X�V�_���𗐐��ɂ�茈�肷��B
		m = mrand() % ( iAbcSearchNum-1 );
		h = mrand() % ( iAbcVectorDimNum - 1);

		lfRand = 2*rnd()-1;
		for( j = 0; j < iAbcVectorDimNum; j++ )
		{
			if( j != h )
			{
				pplfVelocityData[c][j] = pplfAbcData[c][j];
			}
			else
			{
				pplfVelocityData[c][h] = pplfAbcData[c][h] + lfRand*( pplfAbcData[c][h] - pplfAbcData[m][h] );
			}
		}
		// �X�V�_�������̂悤�ɍX�V���܂��B
		lfFunc1 = pflfObjectiveFunction( pplfVelocityData[c], iAbcVectorDimNum );
		lfFunc2 = pflfObjectiveFunction( pplfAbcData[c], iAbcVectorDimNum );

		if( lfFunc1 < lfFunc2 )
		{
			for( j = 0;j < iAbcVectorDimNum; j++ )
			{
				pplfAbcData[c][j] = pplfVelocityData[c][j];
			}
			piNonUpdateCount[c] = 0;
		}
		else
		{
			piNonUpdateCount[c] = piNonUpdateCount[c] + 1;
		}
	}

	// scout bee �̎��s
	// �V���ȒT���_�����߂ĒT�������s���܂��B
	for( i = 0;i < iAbcSearchNum; i++ )
	{
		if( piNonUpdateCount[i] > iAbcLimitCount )
		{
			for( k = 0;k < iAbcVectorDimNum; k++ )
			{
				lfRand = rnd();
				pplfAbcData[i][k] = pplfAbcData[i][k] + lfRand*(plfGlobalMaxAbcData[k]-pplfAbcData[i][k]);
			}
		}
		else
		{
			for( k = 0;k < iAbcVectorDimNum; k++ )
			{
				pplfAbcData[i][k] = pplfAbcData[i][k];
			}
		}
	}
	
	lfMin = pflfObjectiveFunction( plfGlobalMaxAbcData, iAbcVectorDimNum );
	// �X�V���܂��B
	for( i = 0;i < iAbcSearchNum; i++ )
	{
		lfObjFunc = pflfObjectiveFunction( pplfAbcData[i], iAbcVectorDimNum );
		if( lfMin >= lfObjFunc )
		{
			iMinLoc = i;
			lfMin = lfObjFunc;
			for( j = 0; j < iAbcVectorDimNum; j++ )
			{
				plfGlobalMaxAbcData[j] = pplfAbcData[i][j];
			}
		}

	}
}

/**
 * <PRE>
 * �@ABC�@��K�p�������ʂ��o�͂��܂��B(�e�I�̈ʒu)
 * </PRE>
 * @author kobayashi
 * @since 2015/6/6
 * @version 1.0
 */
void CAbc::vOutputAbcData()
{
	int i,j;
	// �����_�ł̖I�̈ʒu���o�͂��܂��B
	for( i = 0; i < iAbcDataNum; i++ )
	{
		for( j = 0;j < iAbcVectorDimNum; j++ )
		{
			printf("%lf,", pplfAbcData[i][j]);
		}
		printf("\n");
	}
}

/**
 * <PRE>
 * �@ABC�@��K�p�������ʂ��o�͂��܂��B(�eABC�̑��x)
 * </PRE>
 * @author kobayashi
 * @since 2015/6/6
 * @version 1.0
 */
void CAbc::vOutputVelocityData()
{
	int i,j;
	// �����_�ł̖I�̑��x���o�͂��܂��B
	for( i = 0; i < iAbcDataNum; i++ )
	{
		for( j = 0;j < iAbcVectorDimNum; j++ )
		{
			printf("%lf,", pplfVelocityData[i][j]);
		}
		printf("\n");
	}
}

/**
 * <PRE>
 * �@ABC�@��K�p�������ʂ��o�͂��܂��B(�e�I�̖ړI�֐��l)
 * </PRE>
 * @author kobayashi
 * @since 2015/6/6
 * @version 1.0
 */
void CAbc::vOutputConstraintFunction()
{
	int i;
	// �����_�ł̊e�I�̖ړI�֐��̒l���o�͂��܂��B
	for( i = 0; i < iAbcDataNum; i++ )
	{
		printf("%lf,", pflfObjectiveFunction(pplfAbcData[i], iAbcVectorDimNum) );
	}
	printf("\n");
}

/**
 * <PRE>
 * �@�����_�ł̍ő�l���o�����q�̈ʒu���o�͂��܂��B
 * </PRE>
 * @author kobayashi
 * @since 2015/6/19
 * @version 1.0
 */
void CAbc::vOutputGlobalMaxAbcData()
{
	int i;
	// �����_�ł̊e���q�̒l���o�͂��܂��B
	for( i = 0; i < iAbcVectorDimNum; i++ )
	{
		printf("%lf,", plfGlobalMaxAbcData[i] );
	}
	printf("\n");
}

/**
 * <PRE>
 * �@�����_�ł̍ő�l�̗��q�̖ړI�֐��l���o�͂��܂��B
 * </PRE>
 * @author kobayashi
 * @since 2015/6/19
 * @version 1.0
 */
void CAbc::vOutputGlobalMaxAbcDataConstFuncValue()
{
	// �����_�ł̊e���q�̖ړI�֐��̒l���o�͂��܂��B
	printf("%lf\n", lfGlobalMaxAbcData );
}

/**
 * <PRE>
 * �@�����_�ł̍ŏ��l���o�����q�̈ʒu���o�͂��܂��B
 * </PRE>
 * @author kobayashi
 * @since 2015/6/19
 * @version 1.0
 */
void CAbc::vOutputGlobalMinAbcData()
{
	int i;
	// �����_�ł̊e���q�̒l���o�͂��܂��B
	for( i = 0; i < iAbcVectorDimNum; i++ )
	{
		printf("%lf,", plfGlobalMinAbcData[i] );
	}
	printf("\n");
}

/**
 * <PRE>
 * �@�����_�ł̍ŏ��l�̗��q�̖ړI�֐��l���o�͂��܂��B
 * </PRE>
 * @author kobayashi
 * @since 2015/6/19
 * @version 1.0
 */
void CAbc::vOutputGlobalMinAbcDataConstFuncValue()
{
	// �����_�ł̊e���q�̖ړI�֐��̒l���o�͂��܂��B
	printf("%lf\n", lfGlobalMinAbcData );
}



/**
 * <PRE>
 * �@�����_�ł̂����Ƃ��悢���q�ʒu�Ƃ��̑��̗��q�Ƃ̋������o�͂��܂��B
 * </PRE>
 * @param iOutFlag 0 �e���q�Ƒ��̗��q�̋����̂ݏo�́B
 * �@�@�@�@�@�@�@�@1 ���ϋ������o�́B
 * @author kobayashi
 * @since 2015/6/19
 * @version 0.1
 */
void CAbc::vOutputAbcDataLocDist( int iOutFlag )
{
	int i,j;
	double lfRes = 0.0;
	double lfAvgDist = 0.0;
	double lfDist = 0.0;
	double lfDist2 = 0.0;
	// �����_�ł̊e���q�Ƒ��̗��q�Ƃ̋������o�͂��܂��B
	for( i= 0;i < iAbcDataNum; i++ )
	{
		lfRes = 0.0;
		for( j = 0;j < iAbcVectorDimNum; j++ )
		{
			lfDist = plfGlobalMaxAbcData[j]-pplfAbcData[i][j];
			lfDist2 = lfDist*lfDist;
			lfRes += lfDist2;
		}
		lfAvgDist += lfRes;
		printf("%lf,", lfRes );
	}
	lfAvgDist /= (double)iAbcDataNum;
	if( iOutFlag == 1 )
	{
		// �����_���q�Ԃ̕��ϋ������o�͂��܂��B
		printf("%lf\n",lfAvgDist);
	}
}


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
void CAbc::vOutputLocalMaxAbcData( int iOutFlag )
{
	int i,j;
	double lfRes = 0.0;
	double lfAvgDist = 0.0;
	double lfDist = 0.0;
	double lfDist2 = 0.0;
	// �����_�ł̊e���q���Ƃ̍ŗǈʒu���o�͂��܂��B
	for( i= 0;i < iAbcDataNum; i++ )
	{
		for( j = 0; j < iAbcVectorDimNum; j++ )
		{
			printf("%lf,", pplfLocalMaxAbcData[i][j] );
		}
		if( iOutFlag == 1 )
		{
			// �����_�ł̊e���q�̖ړI�֐��̒l���o�͂��܂��B
			printf("%lf,", plfLocalMaxObjectiveAbcData[i] );
		}
	}
	printf("\n");
}

void CAbc::vSetRange( double lfRange )
{
	lfSolveRange = lfRange;
}

// �K���x�\�[�g�p

void CAbc::swap(double *swap1,double *swap2)
{
	double temp;
	temp = *swap1;
	*swap1 = *swap2;
	*swap2 = temp;
}

void CAbc::qsort(int start,int end, double *sort )
{
	int i,j;
	int pibo = 0;
	double pibovalue = 0;
	i = start;
	j = end;
	pibo = (i + j) / 2;
	pibovalue = sort[pibo];

	while(1)
	{
		while( sort[i] > pibovalue ) i++;
		while( pibovalue > sort[j] ) j--;
		if( i >= j ) break;
		swap(&sort[i],&sort[j]);
		i++;
		j--;
	}
	if(start < i - 1) qsort(start,i - 1,sort);
	if(j + 1 < end)	qsort(j + 1,end,sort);
}

double CAbc::lfNormalRand( double lfSigma, double lfMean )
{
	int i;
	double lfRand = 0.0;
	for( i = 0;i < 100; i++ )
	{
		lfRand = grand( lfSigma, lfMean );
//		if( -1.0 <= lfRand && lfRand <= 1.0 ) break;
	}
	return lfRand;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////CCmdCheckException�N���X

CAbcException::CAbcException()
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
 * @since 2015/6/6
 * @version 1.0
 */
CAbcException::CAbcException(int iCode, std::string sMethodName, std::string sClassName, std::string sDetail)
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
 * @since 2015/6/6
 * @version 1.0
 */
CAbcException::CAbcException(int iCode, std::string sMethodName, std::string sClassName, std::string sDetail, int iLine)
{
	iErrCode = iCode;
	iErrLine = iLine;
	strMethodName = sMethodName;
	strClassName = sClassName;
	strErrDetail = sDetail;
}


CAbcException::~CAbcException()
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
 * @since 2015/6/6
 * @version 1.0
 */
void CAbcException::SetErrorInfo(int iCode, std::string sMethodName, std::string sClassName, std::string sDetail)
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
 * @since 2015/6/6
 * @version 1.0
 */
void CAbcException::SetErrorInfo(int iCode, std::string sMethodName, std::string sClassName, std::string sDetail, int iLine)
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
 * @since 2015/6/6
 * @version 1.0
 */
int CAbcException::iGetErrCode()
{
	return iErrCode;
}

/**
 * �G���[���N�������s�����o�͂���
 * @author kobayashi
 * @since 2015/6/6
 * @version 1.0
 */
int CAbcException::iGetErrLine()
{
	return iErrLine;
}

/**
 * �G���[���N�������֐������o�͂���
 * @author kobayashi
 * @since 2015/6/6
 * @version 1.0
 */
std::string CAbcException::strGetMethodName()
{
	return strMethodName;
}

/**
 * �G���[���N�������N���X���o�͂���
 * @author kobayashi
 * @since 2015/6/6
 * @version 1.0
 */
std::string CAbcException::strGetClassName()
{
	return strClassName;
}

/**
 * �G���[�̏ڍ׏����o�͂���
 * @author kobayashi
 * @since 2015/6/6
 * @version 1.0
 */
std::string CAbcException::strGetErrDetail()
{
	return strErrDetail;
}
