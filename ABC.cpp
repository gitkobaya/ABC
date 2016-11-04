#include<cstdio>
#include<cmath>
#include<cfloat>
#include<memory>
#include<ctime>
#include<algorithm>

#include"mseq.h"
#include"ABC.h"
//#include"constdata.h"
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
	piTotalNonUpdateCount = NULL;			// �X�V����Ȃ���������
	plfVelocity = NULL;				// ���[���b�g�I���ɂ��I�����鑬�x�x�N�g��
	plfGlobalMaxAbcData = NULL;
	plfGlobalMinAbcData = NULL;			// ���I�œK����\�����q�̃f�[�^
	plfCrossOverData = NULL;
	plfLocalMaxAbcData = NULL;
	lfSolveRange = 0.0;
	lfGlobalMinAbcData = DBL_MAX;
	lfGlobalMaxAbcData = -DBL_MAX;
	iReCounter = 0;

	plfXnew1 = NULL;
	plfXnew2 = NULL;

	plfX0 = NULL;
	plfX1 = NULL;
	plfX2 = NULL;
	plfStepSize = NULL;
	plfScoutBeeResult = NULL;

	pcUndx = NULL;
	pcRex = NULL;
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
	Rank_t tTempRankData;

	iGenerationNumber	= iGenCount;
	iAbcIntervalMinNum	= iIntervalMinNumData;
	iAbcDataNum			= iGenNum;
	iAbcVectorDimNum	= iGenVectorDim;
	iAbcSearchNum		= iSearchNum;
	iAbcLimitCount		= iLimitCountData;
	iAbcUpperSearchNum	= iAlphaData;
	lfConvergenceParam	= lfDrData;
	lfFitBound			= lfBoundData;
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
		pplfLocalMinAbcData = new double*[iAbcDataNum];
		for( i = 0;i < iAbcDataNum; i++ )
		{
			pplfLocalMaxAbcData[i] = new double[iAbcVectorDimNum];
			pplfLocalMinAbcData[i] = new double[iAbcVectorDimNum];
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
		piTotalNonUpdateCount = new int[iAbcSearchNum];
		plfVelocity = new double[iAbcVectorDimNum];
		plfGlobalMaxAbcData = new double[iAbcVectorDimNum];
		plfGlobalMinAbcData = new double[iAbcVectorDimNum];
		plfLocalMaxAbcData = new double[iAbcVectorDimNum];
		plfLocalMinAbcData = new double[iAbcVectorDimNum];
		plfCrossOverData = new double[iAbcVectorDimNum];
		plfXnew1 = new double[iAbcVectorDimNum];
		plfXnew2 = new double[iAbcVectorDimNum];
		plfX0 = new double[iAbcVectorDimNum];
		plfX1 = new double[iAbcVectorDimNum];
		plfX2 = new double[iAbcVectorDimNum];
		plfStepSize = new double[iAbcVectorDimNum];
		plfScoutBeeResult = new double[iAbcVectorDimNum];

		for( i= 0;i < iAbcDataNum; i++ )
		{
			for(j = 0;j < iAbcVectorDimNum; j++ )
			{
				pplfAbcData[i][j] = 0.0;
				pplfNAbcData[i][j] = 0.0;
				pplfLocalMaxAbcData[i][j] = 0.0;
				pplfLocalMinAbcData[i][j] = 0.0;
				pplfVelocityData[i][j] = 0.0;
				pplfNVelocityData[i][j] = 0.0;
			}
		}
		for( i = 0;i < iAbcSearchNum ; i++ )
		{
			plfFit[i] = 0.0;
			plfFitProb[i] = 0.0;
			piNonUpdateCount[i] = 0;
			piTotalNonUpdateCount[i] = 0;
		}
		for( i = 0;i < iAbcVectorDimNum; i++ )
		{
			plfVelocity[i] = 0.0;
			plfGlobalMaxAbcData[i] = 0.0;
			plfGlobalMinAbcData[i] = 0.0;
			plfLocalMaxAbcData[i] = 0.0;
			plfLocalMinAbcData[i] = 0.0;
			plfCrossOverData[i] = 0.0;
			plfXnew1[i] = 0.0;
			plfXnew2[i] = 0.0;
			plfX0[i] = 0.0;
			plfX1[i] = 0.0;
			plfX2[i] = 0.0;
			plfStepSize[i] = 0.0;
			plfScoutBeeResult[i] = 0.0;
		}
		// �\�[�g�p�K���x���i�[����x�N�^�[�ł��B
		stlFitProb.assign( iAbcSearchNum, Rank_t() );
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
		pplfLocalMinAbcData = new double*[iAbcDataNum];
		for( i = 0;i < iAbcDataNum; i++ )
		{
			pplfLocalMaxAbcData[i] = new double[iAbcVectorDimNum];
			pplfLocalMinAbcData[i] = new double[iAbcVectorDimNum];
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
		piTotalNonUpdateCount = new int[iAbcSearchNum];
		plfVelocity = new double[iAbcVectorDimNum];
		plfGlobalMaxAbcData = new double[iAbcVectorDimNum];
		plfGlobalMinAbcData = new double[iAbcVectorDimNum];
		plfLocalMaxAbcData = new double[iAbcVectorDimNum];
		plfLocalMinAbcData = new double[iAbcVectorDimNum];
		plfCrossOverData = new double[iAbcVectorDimNum];
		plfXnew1 = new double[iAbcVectorDimNum];
		plfXnew2 = new double[iAbcVectorDimNum];
		plfX0 = new double[iAbcVectorDimNum];
		plfX1 = new double[iAbcVectorDimNum];
		plfX2 = new double[iAbcVectorDimNum];
		plfStepSize = new double[iAbcVectorDimNum];
		plfScoutBeeResult = new double[iAbcVectorDimNum];

		for( i= 0;i < iAbcDataNum; i++ )
		{
			for(j = 0;j < iAbcVectorDimNum; j++ )
			{
				pplfAbcData[i][j] = 0.0;
				pplfNAbcData[i][j] = 0.0;
				pplfLocalMaxAbcData[i][j] = 0.0;
				pplfLocalMinAbcData[i][j] = 0.0;
				pplfVelocityData[i][j] = 0.0;
				pplfNVelocityData[i][j] = 0.0;
			}
		}
		for( i = 0;i < iAbcSearchNum ; i++ )
		{
			plfFit[i] = 0.0;
			plfFitProb[i] = 0.0;
			piNonUpdateCount[i] = 0;
			piTotalNonUpdateCount[i] = 0;
		}
		for( i = 0;i < iAbcVectorDimNum; i++ )
		{
			plfVelocity[i] = 0.0;
			plfGlobalMaxAbcData[i] = 0.0;
			plfGlobalMinAbcData[i] = 0.0;
			plfLocalMaxAbcData[i] = 0.0;
			plfLocalMinAbcData[i] = 0.0;
			plfCrossOverData[i] = 0.0;
			plfXnew1[i] = 0.0;
			plfXnew2[i] = 0.0;
			plfX0[i] = 0.0;
			plfX1[i] = 0.0;
			plfX2[i] = 0.0;
			plfStepSize[i] = 0.0;
			plfScoutBeeResult[i] = 0.0;
		}
		// �\�[�g�p�K���x���i�[����x�N�^�[�ł��B
		stlFitProb.assign( iAbcSearchNum, Rank_t() );
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
 * @param iGenCount      	 �v�Z��
 * @param iGenNum        	 �R���j�[��
 * @param iGenVectorDim  	 �T���_�̎�����
 * @param iSearchNum  �@ 	 �T���_�̐�
 * @param iLimitCountData	 �X�V���Ȃ�������
 * @param iCrossOverNumData	 ������
 * @param lfAlpha		 UNDX�ɂ�����Alpha
 * @param lfBeta		 UNDX�ɂ�����Beta
 * @throw AbcException
 * @author kobayashi
 * @since 2015/8/10
 * @version 0.1
 */
void CAbc::vInitialize( int iGenCount, int iGenNum, int iGenVectorDim, int iSearchNum, int iLimitCountData, int iCrossOverNumData, double lfAlphaData, double lfBetaData )
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
		pplfLocalMinAbcData = new double*[iAbcDataNum];
		for( i = 0;i < iAbcDataNum; i++ )
		{
			pplfLocalMaxAbcData[i] = new double[iAbcVectorDimNum];
			pplfLocalMinAbcData[i] = new double[iAbcVectorDimNum];
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
		piTotalNonUpdateCount = new int[iAbcSearchNum];
		plfVelocity = new double[iAbcVectorDimNum];
		plfGlobalMaxAbcData = new double[iAbcVectorDimNum];
		plfGlobalMinAbcData = new double[iAbcVectorDimNum];
		plfLocalMaxAbcData = new double[iAbcVectorDimNum];
		plfLocalMinAbcData = new double[iAbcVectorDimNum];
		plfCrossOverData = new double[iAbcVectorDimNum];
		plfXnew1 = new double[iAbcVectorDimNum];
		plfXnew2 = new double[iAbcVectorDimNum];
		plfX0 = new double[iAbcVectorDimNum];
		plfX1 = new double[iAbcVectorDimNum];
		plfX2 = new double[iAbcVectorDimNum];
		plfStepSize = new double[iAbcVectorDimNum];
		plfScoutBeeResult = new double[iAbcVectorDimNum];

		for( i= 0;i < iAbcDataNum; i++ )
		{
			for(j = 0;j < iAbcVectorDimNum; j++ )
			{
				pplfAbcData[i][j] = 0.0;
				pplfNAbcData[i][j] = 0.0;
				pplfLocalMaxAbcData[i][j] = 0.0;
				pplfLocalMinAbcData[i][j] = 0.0;
				pplfVelocityData[i][j] = 0.0;
				pplfNVelocityData[i][j] = 0.0;
			}
		}
		for( i = 0;i < iAbcSearchNum ; i++ )
		{
			plfFit[i] = 0.0;
			plfFitProb[i] = 0.0;
			piNonUpdateCount[i] = 0;
			piTotalNonUpdateCount[i] = 0;
		}
		for( i = 0;i < iAbcVectorDimNum; i++ )
		{
			plfVelocity[i] = 0.0;
			plfGlobalMaxAbcData[i] = 0.0;
			plfGlobalMinAbcData[i] = 0.0;
			plfLocalMaxAbcData[i] = 0.0;
			plfLocalMinAbcData[i] = 0.0;
			plfCrossOverData[i] = 0.0;
			plfXnew1[i] = 0.0;
			plfXnew2[i] = 0.0;
			plfX0[i] = 0.0;
			plfX1[i] = 0.0;
			plfX2[i] = 0.0;
			plfStepSize[i] = 0.0;
			plfScoutBeeResult[i] = 0.0;
		}
		pcUndx = new CUndx();
		iCrossOverNum = iCrossOverNumData;
		lfAlpha = lfAlphaData;
		lfBeta = lfBetaData;
		// UNDX�̏����������s���܂��B
		pcUndx->vInitialize( iGenerationNumber, iAbcDataNum, iAbcVectorDimNum, iCrossOverNum );
		pcUndx->vSetAlpha( lfAlpha );
		pcUndx->vSetBeta( lfBeta );
		// �\�[�g�p�K���x���i�[����x�N�^�[�ł��B
		stlFitProb.assign( iAbcSearchNum, Rank_t() );
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
 * @param iIntervalMinNum �Œᔽ����
 * @param iAlphaData      �T���_��ʐ�
 * @param lfDrData        �����󋵔���
 * @param lfBoundData     �K���x���e���E�l
 * @param lfAccuracyData  �K���x�]�����x
 * @param iCrossOverNumData	 ������
 * @param lfAlpha		 UNDX�ɂ�����Alpha
 * @param lfBeta		 UNDX�ɂ�����Beta
 * @throw AbcException
 * @author kobayashi
 * @since 2015/7/28
 * @version 0.1
 */
void CAbc::vInitialize( int iGenCount, int iGenNum, int iGenVectorDim, int iSearchNum, int iLimitCountData, int iIntervalMinNumData, int iAlphaData, double lfDrData, double lfBoundData, double lfAccuracyData, int iCrossOverNumData, double lfAlphaData, double lfBetaData  )
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
		pplfLocalMinAbcData = new double*[iAbcDataNum];
		for( i = 0;i < iAbcDataNum; i++ )
		{
			pplfLocalMaxAbcData[i] = new double[iAbcVectorDimNum];
			pplfLocalMinAbcData[i] = new double[iAbcVectorDimNum];
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
		piTotalNonUpdateCount = new int[iAbcSearchNum];
		plfVelocity = new double[iAbcVectorDimNum];
		plfGlobalMaxAbcData = new double[iAbcVectorDimNum];
		plfGlobalMinAbcData = new double[iAbcVectorDimNum];
		plfLocalMaxAbcData = new double[iAbcVectorDimNum];
		plfLocalMinAbcData = new double[iAbcVectorDimNum];
		plfCrossOverData = new double[iAbcVectorDimNum];
		plfXnew1 = new double[iAbcVectorDimNum];
		plfXnew2 = new double[iAbcVectorDimNum];
		plfX0 = new double[iAbcVectorDimNum];
		plfX1 = new double[iAbcVectorDimNum];
		plfX2 = new double[iAbcVectorDimNum];
		plfStepSize = new double[iAbcVectorDimNum];
		plfScoutBeeResult = new double[iAbcVectorDimNum];

		for( i= 0;i < iAbcDataNum; i++ )
		{
			for(j = 0;j < iAbcVectorDimNum; j++ )
			{
				pplfAbcData[i][j] = 0.0;
				pplfNAbcData[i][j] = 0.0;
				pplfLocalMaxAbcData[i][j] = 0.0;
				pplfLocalMinAbcData[i][j] = 0.0;
				pplfVelocityData[i][j] = 0.0;
				pplfNVelocityData[i][j] = 0.0;
			}
		}
		for( i = 0;i < iAbcSearchNum ; i++ )
		{
			plfFit[i] = 0.0;
			plfFitProb[i] = 0.0;
			piNonUpdateCount[i] = 0;
			piTotalNonUpdateCount[i] = 0;
		}
		for( i = 0;i < iAbcVectorDimNum; i++ )
		{
			plfVelocity[i] = 0.0;
			plfGlobalMaxAbcData[i] = 0.0;
			plfGlobalMinAbcData[i] = 0.0;
			plfLocalMaxAbcData[i] = 0.0;
			plfLocalMinAbcData[i] = 0.0;
			plfCrossOverData[i] = 0.0;
			plfXnew1[i] = 0.0;
			plfXnew2[i] = 0.0;
			plfX0[i] = 0.0;
			plfX1[i] = 0.0;
			plfX2[i] = 0.0;
			plfStepSize[i] = 0.0;
			plfScoutBeeResult[i] = 0.0;
		}
		pcUndx = new CUndx();
		iCrossOverNum = iCrossOverNumData;
		lfAlpha = lfAlphaData;
		lfBeta = lfBetaData;
		// UNDX�̏����������s���܂��B
		pcUndx->vInitialize( iGenerationNumber, iAbcDataNum, iAbcVectorDimNum, iCrossOverNum );
		pcUndx->vSetAlpha( lfAlpha );
		pcUndx->vSetBeta( lfBeta );
		// �\�[�g�p�K���x���i�[����x�N�^�[�ł��B
		stlFitProb.assign( iAbcSearchNum, Rank_t() );
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
 * @param iIntervalMinNum �Œᔽ����
 * @param iAlphaData      �T���_��ʐ�
 * @param lfDrData        �����󋵔���
 * @param lfBoundData     �K���x���e���E�l
 * @param lfAccuracyData  �K���x�]�����x
 * @param iParentNumberData			 ������
 * @param iChildrenNumber			 REX�ɂ�����Alpha
 * @param iUpperEvalChildrenNumber	 UNDX�ɂ�����Beta
 * @param lfLearningRate	 UNDX�ɂ�����Beta
 * @throw AbcException
 * @author kobayashi
 * @since 2016/8/26
 * @version 0.1
 */
void CAbc::vInitialize( int iGenCount, int iGenNum, int iGenVectorDim, int iSearchNum, int iLimitCountData, int iIntervalMinNumData, int iAlphaData, double lfDrData, double lfBoundData, double lfAccuracyData, int iParentNum, int iChildrenNum, int iUpperEvalChildrenNum, double lfLearningRateData  )
{
	CAbcException cae;

	int i,j;

	iGenerationNumber		= iGenCount;
	iAbcIntervalMinNum		= iIntervalMinNumData;
	iAbcDataNum				= iGenNum;
	iAbcVectorDimNum		= iGenVectorDim;
	iAbcSearchNum			= iSearchNum;
	iAbcLimitCount			= iLimitCountData;
	iAbcUpperSearchNum		= iAlphaData;
	lfConvergenceParam		= lfDrData;
	lfFitBound				= lfBoundData;
	lfFitAccuracy			= lfAccuracyData;
	iParentNumber			= iParentNum;			// Rex�p�I������e�̐�
	iChildrenNumber			= iChildrenNum;			// Rex�p��������q���̐�
	iUpperEvalChildrenNumber	= iUpperEvalChildrenNum;	// ARex�p���������q���̏�ʂ�I�����鐔
	lfLearningRate			= lfLearningRateData;		// ARex�p�w�K��

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
		pplfLocalMinAbcData = new double*[iAbcDataNum];
		for( i = 0;i < iAbcDataNum; i++ )
		{
			pplfLocalMaxAbcData[i] = new double[iAbcVectorDimNum];
			pplfLocalMinAbcData[i] = new double[iAbcVectorDimNum];
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
		piTotalNonUpdateCount = new int[iAbcSearchNum];
		plfVelocity = new double[iAbcVectorDimNum];
		plfGlobalMaxAbcData = new double[iAbcVectorDimNum];
		plfGlobalMinAbcData = new double[iAbcVectorDimNum];
		plfLocalMaxAbcData = new double[iAbcVectorDimNum];
		plfLocalMinAbcData = new double[iAbcVectorDimNum];
		plfCrossOverData = new double[iAbcVectorDimNum];
		plfXnew1 = new double[iAbcVectorDimNum];
		plfXnew2 = new double[iAbcVectorDimNum];
		plfX0 = new double[iAbcVectorDimNum];
		plfX1 = new double[iAbcVectorDimNum];
		plfX2 = new double[iAbcVectorDimNum];
		plfStepSize = new double[iAbcVectorDimNum];
		plfScoutBeeResult = new double[iAbcVectorDimNum];

		for( i= 0;i < iAbcDataNum; i++ )
		{
			for(j = 0;j < iAbcVectorDimNum; j++ )
			{
				pplfAbcData[i][j] = 0.0;
				pplfNAbcData[i][j] = 0.0;
				pplfLocalMaxAbcData[i][j] = 0.0;
				pplfLocalMinAbcData[i][j] = 0.0;
				pplfVelocityData[i][j] = 0.0;
				pplfNVelocityData[i][j] = 0.0;
			}
		}
		for( i = 0;i < iAbcSearchNum ; i++ )
		{
			plfFit[i] = 0.0;
			plfFitProb[i] = 0.0;
			piNonUpdateCount[i] = 0;
			piTotalNonUpdateCount[i] = 0;
		}
		for( i = 0;i < iAbcVectorDimNum; i++ )
		{
			plfVelocity[i] = 0.0;
			plfGlobalMaxAbcData[i] = 0.0;
			plfGlobalMinAbcData[i] = 0.0;
			plfLocalMaxAbcData[i] = 0.0;
			plfLocalMinAbcData[i] = 0.0;
			plfCrossOverData[i] = 0.0;
			plfXnew1[i] = 0.0;
			plfXnew2[i] = 0.0;
			plfX0[i] = 0.0;
			plfX1[i] = 0.0;
			plfX2[i] = 0.0;
			plfStepSize[i] = 0.0;
			plfScoutBeeResult[i] = 0.0;
		}
		pcRex = new CRex();
		// UNDX�̏����������s���܂��B
		pcRex->vInitialize( iGenerationNumber, iAbcDataNum, iAbcVectorDimNum, iParentNumber, iChildrenNumber, lfLearningRate, iUpperEvalChildrenNumber );
		// �\�[�g�p�K���x���i�[����x�N�^�[�ł��B
		stlFitProb.assign( iAbcSearchNum, Rank_t() );
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
			lfFitCurrentBest = lfGlobalMinAbcData;
		}
		if( lfGlobalMaxAbcData <= lfObjFunc )
		{
			lfGlobalMaxAbcData = lfObjFunc;
			for( j = 0; j < iAbcVectorDimNum; j++ )
			{
				plfGlobalMaxAbcData[j] = pplfAbcData[i][j];
			}
//			lfFitCurrentBest = lfMin;
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
		if( piTotalNonUpdateCount != NULL )
		{
			delete[] piTotalNonUpdateCount;
			piTotalNonUpdateCount = NULL;
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
		if (plfLocalMinAbcData != NULL)
		{
			delete[] plfLocalMinAbcData;
			plfLocalMinAbcData = NULL;
		}
		if (plfLocalMaxAbcData != NULL)
		{
			delete[] plfLocalMaxAbcData;
			plfLocalMaxAbcData = NULL;
		}
		// UNDX������L���ɂ��Ă���ꍇ�ɂ̂ݎ��s���܂��B
		if( pcUndx != NULL )
		{
			pcUndx->vTerminate();
			delete pcUndx;
			pcUndx = NULL;
		}
		stlFitProb.clear();

		if (plfX0 != NULL)
		{
			delete[] plfX0;
			plfX0 = NULL;
		}
		if (plfX1 != NULL)
		{
			delete[] plfX1;
			plfX1 = NULL;
		}
		if (plfX2 != NULL)
		{
			delete[] plfX2;
			plfX2 = NULL;
		}
		if (plfStepSize != NULL)
		{
			delete[] plfStepSize;
			plfStepSize = NULL;
		}
		if (plfScoutBeeResult != NULL)
		{
			delete[] plfScoutBeeResult;
			plfScoutBeeResult = NULL;
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
	if( pcUndx != NULL )
	{
		pcUndx->vSetConstraintFunction( pflfObjectiveFunction );
	}
	if( pcRex != NULL )
	{
		pcRex->vSetConstraintFunction( pflfObjectiveFunction );
	}
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
	if( pcUndx != NULL )
	{
		pcUndx->vReleaseCallbackConstraintFunction();
	}
	if( pcRex != NULL )
	{
		pcRex->vReleaseCallbackConstraintFunction();
	}
}

/**
 * <PRE>
 * �@�l�H�I�R���j�[�œK���@�����s���܂��B
 *   ver 0.1
 *   ver 0.2 �@�\�킯�����{�B
 * </PRE>
 * @author kobayashi
 * @since 2015/7/28
 * @version 0.2
 */
void CAbc::vAbc()
{
	// employee bee �̓���
	vEmployBeeOrigin();

	// onlookers bee�̓���
	vOnlookerBeeOrigin();

	// scout bee �̎��s
	vScoutBeeOrigin();

	// �X�V���܂��B
	vGetGlobalMaxMin();
}

/**
 * <PRE>
 * �@�l�H�I�R���j�[�œK���@�����s���܂��B
 *	 GBest�𗘗p����ABC�@
 *   Gbest-guided artificial bee colony algorithm for numerical function optimization.
 *   Applied Mathematics and Computation, 217(7):3166-3173,2010.
 * </PRE>
 * @author kobayashi
 * @since 2015/7/28
 * @version 0.1
 */
void CAbc::vGAbc()
{
	// employee bee �̓���
	vEmployBeeGBest();

	// onlookers bee�̓���
	vOnlookerBeeGBest();

	// scout bee �̎��s
	vScoutBeeNormal();
	
	// �Ǐ��ő�l�A�ŏ��l���擾���܂��B
	vGetLocalMaxMin();

	// ���I�ő�l�A�ŏ��l���擾���܂��B
	vGetGlobalMaxMin();
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
	double lfFitJudge = 0.0;
	// employed bees�ɂ��T�������{���܂��B
	lfFitJudge = lfEmployBeeEnhanced( iUpdateCount );

	// onlooker bees�ɂ��T��
	vOnlookerBeeEnhanced( iUpdateCount, lfFitJudge );
	
	// �Ǐ��ő�l�A�ŏ��l���擾���܂��B
	vGetLocalMaxMin();

	// ���I�ő�l�A�ŏ��l���擾���܂��B
	vGetGlobalMaxMin();
}

/**
 * <PRE>
 * �@�l�H�I�R���j�[�œK���@�����s���܂��B
 *   ���q�Q�œK���@��IWCFA��K�p
 * </PRE>
 * @author kobayashi
 * @since 2015/7/28
 * @version 0.1
 */
void CAbc::vIWCFAAbc( int iUpdateCount )
{
	double lfFai = 0.0;
	double lfK = 0.0;
	double lfCoe1 = 2.0;
	double lfCoe2 = 2.0;

	lfFai = lfCoe1 + lfCoe2;
	if( lfFai > 4.0 )	lfK = 2.0/(fabs(2.0-lfFai - sqrt( lfFai*lfFai-4.0*lfFai ) )  );
	else				lfK = 1.0;

//	lfWeight = lfMaxWeight - (lfMaxWeight-lfMinWeight)/(double)iGenerationNumber*(double)(iUpdateCount-piTotalNonUpdateCount[i];
	// employee bee �̓���
	vEmployBeeIWCFA( lfK, lfCoe1, lfCoe2, iUpdateCount );

	// onlookers bee�̓���
	vOnlookerBeeIWCFA( lfK, lfCoe1, lfCoe2, iUpdateCount );

	// scout bee �̎��s
	vScoutBeeNormal();

	// �Ǐ��ő�l�A�ŏ��l���擾���܂��B
	vGetLocalMaxMin();

	// ���I�ő�l�A�ŏ��l���擾���܂��B
	vGetGlobalMaxMin();
}

/**
 * <PRE>
 * �@2013 Memetic search in artificial bee colony algorthim���
 *   ver 0.1 2016/07/28 ����
 *   ver 0.2 2016/10/25 �X�V���_�̎Z�o�Ɍ��𔭌����C���B
 * </PRE>
 * @author kobayashi
 * @since 2016/7/28
 * @version 0.2
 */
void CAbc::vMeAbc( int iUpdateCount )
{
	int j, k;
	double lfFunc1 = 0.0;
	double lfFunc2 = 0.0;
	double lfFai = 0.618;
	double lfA    = -1.2;
	double lfB    = 1.2;
	double lfK = 0.0;
	double lfPr = 0.4;
	double lfF1 = 0.0;
	double lfF2 = 0.0;

	// employee bee �̓���
	vEmployBeeOrigin();

	// onlookers bee�̓���
	vOnlookerBeeGBest();

	// scout bee �̎��s
	vScoutBeeNormal();
//	vScoutBeeUndx();

	// Memetic artificial bee colony Algorithm(Pr�Ɋւ��Ă�0�`1�̊Ԃ̓K���Ȓl��ݒ�B)
	while( fabs(lfA-lfB) < 0.01 )
	{
		lfF1 = ( lfB-(lfB-lfA)*lfFai );
		lfF2 = ( lfA+(lfB-lfA)*lfFai );

		j = mrand() % iAbcSearchNum;
		for( k = 0;k < iAbcVectorDimNum; k++ )
		{
			if( rnd() > lfPr )
			{
				plfXnew1[k] = plfGlobalMinAbcData[k] + lfF1*(plfGlobalMinAbcData[k]-pplfAbcData[j][k]);
				plfXnew2[k] = plfGlobalMinAbcData[k] + lfF2*(plfGlobalMinAbcData[k]-pplfAbcData[j][k]);
			}
			else
			{
				plfXnew1[k] = plfGlobalMinAbcData[k];
				plfXnew2[k] = plfGlobalMinAbcData[k];
			}
		}
		lfFunc1 = pflfObjectiveFunction( plfXnew1, iAbcVectorDimNum );
		lfFunc2 = pflfObjectiveFunction( plfXnew2, iAbcVectorDimNum );
		if( lfFunc1 < lfFunc2 )
		{
			lfB = lfF2;
			if( lfFunc1 < lfGlobalMinAbcData )
			{
				for( k = 0;k < iAbcVectorDimNum; k++ )
				{
					plfGlobalMinAbcData[k] = plfXnew1[k];
				}
			}
		}
		else
		{
			lfA = lfF1;
			if( lfFunc2 < lfGlobalMinAbcData )
			{
				for( k = 0;k < iAbcVectorDimNum; k++ )
				{
					plfGlobalMinAbcData[k] = plfXnew2[k];
				}
			}
		}
	}
	
	// �Ǐ��ő�l�A�ŏ��l���擾���܂��B
	vGetLocalMaxMin();

	// ���I�ő�l�A�ŏ��l���擾���܂��B
	vGetGlobalMaxMin();
}

/**
 * <PRE>
 * �@Randomized Memetic Artificial Bee Colony Algorthim���
 *   International Journal of Emerging Trends of Technology in Computer Science, vol.3(1), 2014
 *   ver 0.1 2016/09/23 ����
 *   ver 0.2 2016/10/25 �X�V���_�̎Z�o�Ɍ��𔭌����C���B
 * </PRE>
 * @author kobayashi
 * @since 2016/9/23
 * @version 0.2
 */
void CAbc::vRMAbc( int iUpdateCount )
{
	int j, k;
	double lfFunc1 = 0.0;
	double lfFunc2 = 0.0;
	double lfFai = 0.618;
	double lfA    = -1.2;
	double lfB    = 1.2;
	double lfK = 0.0;
	double lfPr = 0.4;
	double lfF1 = 0.0;
	double lfF2 = 0.0;

	// employee bee �̓���
	vEmployBeeOrigin();

	// onlookers bee�̓���
	vOnlookerBeeRM();

	// scout bee �̎��s
	vScoutBeeNormal();
//	vScoutBeeUndx();

	// Randomized Memetic artificial bee colony Algorithm(Pr�Ɋւ��Ă�0�`1�̊Ԃ̓K���Ȓl��ݒ�B)�O��_���ł�0.3�������l�B
	while( fabs(lfA-lfB) < 0.01 )
	{
		lfF1 = rnd()*( lfB-(lfB-lfA)*lfFai );
		lfF2 = (rnd()-1.0)*( lfA+(lfB-lfA)*lfFai );

		j = mrand() % iAbcSearchNum;
		for( k = 0;k < iAbcVectorDimNum; k++ )
		{
			if( rnd() > lfPr )
			{
				plfXnew1[k] = plfGlobalMinAbcData[k] + lfF1*(plfGlobalMinAbcData[k]-pplfAbcData[j][k]);
				plfXnew2[k] = plfGlobalMinAbcData[k] + lfF2*(plfGlobalMinAbcData[k]-pplfAbcData[j][k]);
			}
			else
			{
				plfXnew1[k] = plfGlobalMinAbcData[k];
				plfXnew2[k] = plfGlobalMinAbcData[k];
			}
		}
		lfFunc1 = pflfObjectiveFunction( plfXnew1, iAbcVectorDimNum );
		lfFunc2 = pflfObjectiveFunction( plfXnew2, iAbcVectorDimNum );
		if( lfFunc1 < lfFunc2 )
		{
			lfB = lfF2;
			if( lfFunc1 < lfGlobalMinAbcData )
			{
				for( k = 0;k < iAbcVectorDimNum; k++ )
				{
					plfGlobalMinAbcData[k] = plfXnew1[k];
				}
			}
		}
		else
		{
			lfA = lfF1;
			if( lfFunc2 < lfGlobalMinAbcData )
			{
				for( k = 0;k < iAbcVectorDimNum; k++ )
				{
					plfGlobalMinAbcData[k] = plfXnew2[k];
				}
			}
		}
	}
	
	// �Ǐ��ő�l�A�ŏ��l���擾���܂��B
	vGetLocalMaxMin();

	// ���I�ő�l�A�ŏ��l���擾���܂��B
	vGetGlobalMaxMin();
}

/**
 * <PRE>
 * �@�l�H�I�R���j�[�œK���@�i�����𓱓�������@�j�����s���܂��B
 *   A Novel Hybrid Crossover based Artificial Bee Colony Algorithm for Optimization Problem International Journal of Computer Applications 2013���
 *   ver 0.1 2016/04/11 ����
 *   ver 0.2 2016/10/25 �X�V���_�̎Z�o�Ɍ��𔭌����C���B
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
	double lfFunc3 = 0.0;
	double lfRes = 0.0;
	double lfPrevProb = 0.0;
	double lfProb = 0.0;
	double lfFitProb = 0.0;
	int icNonUpdateCount = 0;
	std::vector<double> stlCrossOverData1,stlCrossOverData2,stlCrossOverData3;

	int iParentLoc1;
	int iParentLoc2;

	// employee bee �̓���
	vEmployBeeOrigin();

	// crossover�����s���܂��B(�����Ƃ������̎����l�f�`�̌���)
	iParentLoc1 = mrand() % iAbcSearchNum;
	iParentLoc2 = mrand() % iAbcSearchNum;

// �����l�f�`�̕��ό��������s���܂��B���_�A�����_�A�O���_�̂R�_���Z�o���܂��B
	for( i = 0;i < iAbcVectorDimNum; i++ )
	{
		stlCrossOverData1.push_back( 0.5*( pplfAbcData[iParentLoc1][i] + pplfAbcData[iParentLoc2][i] ) );
		stlCrossOverData2.push_back( 1.5*( pplfAbcData[iParentLoc1][i] + 0.5*pplfAbcData[iParentLoc2][i] ) );
		stlCrossOverData3.push_back( -0.5*( pplfAbcData[iParentLoc1][i] + 1.5*pplfAbcData[iParentLoc2][i] ) );
	}
	
	// �����_���ɑI�������e�̒��ŕ]���֐��̒l���������̂Ɛ��������q�����������܂��B
	for( i = 0;i < iAbcVectorDimNum; i++ ) plfCrossOverData[i] = stlCrossOverData1[i];
	lfFunc1 = pflfObjectiveFunction( plfCrossOverData, iAbcVectorDimNum );
	for( i = 0;i < iAbcVectorDimNum; i++ ) plfCrossOverData[i] = stlCrossOverData2[i];
	lfFunc2 = pflfObjectiveFunction( plfCrossOverData, iAbcVectorDimNum );
	for( i = 0;i < iAbcVectorDimNum; i++ ) plfCrossOverData[i] = stlCrossOverData3[i];
	lfFunc3 = pflfObjectiveFunction( plfCrossOverData, iAbcVectorDimNum );
	if( lfFunc1 < lfFunc2 )
	{
		if( lfFunc2 < lfFunc3 )
		{
			for( i = 0;i < iAbcVectorDimNum; i++ )
			{
				pplfAbcData[iParentLoc1][i] = stlCrossOverData1[i];
				pplfAbcData[iParentLoc2][i] = stlCrossOverData2[i];
			}
		}
		else if( lfFunc1 < lfFunc3 )
		{
			for( i = 0;i < iAbcVectorDimNum; i++ )
			{
				pplfAbcData[iParentLoc1][i] = stlCrossOverData1[i];
				pplfAbcData[iParentLoc2][i] = stlCrossOverData3[i];
			}
		}
		else
		{
			for( i = 0;i < iAbcVectorDimNum; i++ )
			{
				pplfAbcData[iParentLoc1][i] = stlCrossOverData3[i];
				pplfAbcData[iParentLoc2][i] = stlCrossOverData1[i];
			}
		}
	}
	else if( lfFunc2 < lfFunc1 )
	{
		if( lfFunc1 < lfFunc3 )
		{
			for( i = 0;i < iAbcVectorDimNum; i++ )
			{
				pplfAbcData[iParentLoc1][i] = stlCrossOverData2[i];
				pplfAbcData[iParentLoc2][i] = stlCrossOverData1[i];
			}
		}
		else if( lfFunc2 < lfFunc3 )
		{
			for( i = 0;i < iAbcVectorDimNum; i++ )
			{
				pplfAbcData[iParentLoc1][i] = stlCrossOverData2[i];
				pplfAbcData[iParentLoc2][i] = stlCrossOverData3[i];
			}
		}
		else
		{
			for( i = 0;i < iAbcVectorDimNum; i++ )
			{
				pplfAbcData[iParentLoc1][i] = stlCrossOverData3[i];
				pplfAbcData[iParentLoc2][i] = stlCrossOverData2[i];
			}
		}
	}


	// onlookers bee�̓���
	vOnlookerBeeOrigin();

	// scout bee �̎��s
	vScoutBeeNormal();
	
	// �Ǐ��ő�l�A�ŏ��l���擾���܂��B
	vGetLocalMaxMin();

	// ���I�ő�l�A�ŏ��l���擾���܂��B
	vGetGlobalMaxMin();
}

/**
 * <PRE>
 * �@�l�H�I�R���j�[�œK���@�i�����𓱓�������@�j�����s���܂��B
 *   ���S����A�����W
 * </PRE>
 * @author kobayashi
 * @since 2016/8/10
 * @version 0.1
 */
void CAbc::vUndxAbc()
{
	// employee bee �̓���
	vEmployBeeBest();

	// onlookers bee�̓���
	vOnlookerBeeBest();

	// scout bee �̎��s
	vScoutBeeUndx();
	
	// �Ǐ��ő�l�A�ŏ��l���擾���܂��B
	vGetLocalMaxMin();

	// ���I�ő�l�A�ŏ��l���擾���܂��B
	vGetGlobalMaxMin();
}

/**
 * <PRE>
 * �@�l�H�I�R���j�[�œK���@�i�����𓱓�������@�j�����s���܂��B
 *   ���S����A�����W
 *   2011�̍����\���𓱓�
 * </PRE>
 * @author kobayashi
 * @since 2016/8/19
 * @version 0.1
 */
void CAbc::vUndxEnhancedAbc( int iUpdateCount )
{
	double lfFitJudge = 0.0;

	// employee bee �̓���
//	lfFitJudge = lfEmployBeeBestEnhanced( iUpdateCount );
	lfFitJudge = lfEmployBeeEnhanced( iUpdateCount );

	// onlookers bee�̓���
//	vOnlookerBeeBestEnhanced( iUpdateCount, lfFitJudge );
	vOnlookerBeeEnhanced( iUpdateCount, lfFitJudge );

	// scout bee �̎��s
	vScoutBeeUndx();
	
	// �Ǐ��ő�l�A�ŏ��l���擾���܂��B
	vGetLocalMaxMin();

	// ���I�ő�l�A�ŏ��l���擾���܂��B
	vGetGlobalMaxMin();
}

/**
 * <PRE>
 * �@�l�H�I�R���j�[�œK���@�i�����𓱓�������@�j�����s���܂��B
 *   ���S����A�����W�iEmsanble Real Coded CrossOver��p���܂��B�j
 * </PRE>
 * @author kobayashi
 * @since 2016/8/10
 * @version 0.1
 */
void CAbc::vRexAbc()
{
	// employee bee �̓���
	vEmployBeeBest();

	// onlookers bee�̓���
	vOnlookerBeeBest();

	// scout bee �̎��s
	vScoutBeeRex();
	
	// �Ǐ��ő�l�A�ŏ��l���擾���܂��B
	vGetLocalMaxMin();

	// ���I�ő�l�A�ŏ��l���擾���܂��B
	vGetGlobalMaxMin();
}

/**
 * <PRE>
 * �@�l�H�I�R���j�[�œK���@�i�����𓱓�������@�j�����s���܂��B
 *   ���S����A�����W�iAdaptation Emsanble Real Coded CrossOver��p���܂��B�j
 * </PRE>
 * @author kobayashi
 * @since 2016/8/27
 * @version 0.1
 */
void CAbc::vARexAbc()
{
	// employee bee �̓���
	vEmployBeeBest();

	// onlookers bee�̓���
	vOnlookerBeeBest();

	// scout bee �̎��s
	vScoutBeeARex();
	
	// �Ǐ��ő�l�A�ŏ��l���擾���܂��B
	vGetLocalMaxMin();

	// ���I�ő�l�A�ŏ��l���擾���܂��B
	vGetGlobalMaxMin();
}

/**
 * <PRE>
 * �@�l�H�I�R���j�[�œK���@�����s���܂��B
 *   HJABC�@��K�p���܂��B
 *   Artificial Bee Colony Algorithm ith Local Search for Numerical Optimization, Jornal of Software, Vol.6, no.3, march 2011���
 *   ver 0.1 2016/10/03 ����
 *   ver 0.2 2016/10/25 �A���S���Y���������ł��Ă��Ȃ������̂Ŏ������B
 *   ver 0.3 2016/10/27 �A���S���Y�����������B
 * </PRE>
 * @param iUpdateCount
 * @author kobayashi
 * @since 2016/10/3
 * @version 0.1
 */
void CAbc::vHJAbc( int iUpdateCount )
{
	int i, j;
	double lfRes = 0.0;
	double lfStepSize = 1.0;
	double lfObjFunc0 = 0.0;
	double lfObjFunc1 = 0.0;
	double lfObjFunc2 = 0.0;
	int iInterval;
	double rho = 0.5;
	bool bRet;

	iHJCounter = 50 * iAbcSearchNum;
	iHJInterval = 3 * iAbcSearchNum;
	rho = 0.5;

	// employee bee �̓���
	vEmployBeeOrigin();

	// onlookers bee�̓���
	vOnlookerBeeHJ();

	// scout bee �̎��s
	vScoutBeeNormal();
	
	// �Ǐ��ő�l�A�ŏ��l���擾���܂��B
	vGetLocalMaxMin();

	// ���I�ő�l�A�ŏ��l���擾���܂��B
	vGetGlobalMaxMin();

	// �X�e�b�v�T�C�Y�̌v�Z�����s���܂��B
	memcpy(plfX0, plfGlobalMinAbcData, sizeof(double)*iAbcVectorDimNum);
	if (iUpdateCount % iHJInterval == 0)
	{
		for (j = 0; j < iAbcVectorDimNum; j++)
		{
			lfRes = 0.0;
			for (i = 0; i < iAbcSearchNum; i++)
				lfRes += (pplfAbcData[i][j] - plfGlobalMinAbcData[j]);
			plfStepSize[j] = (0.1*lfRes / (double)iAbcSearchNum);
		}
		// Hooke-Jeeves�@��K�p���܂��B
		vModifiedHookeJeevesMethod(plfStepSize, plfX1, plfX2, plfX0);

		lfObjFunc0 = pflfObjectiveFunction(plfX0, iAbcVectorDimNum);
		lfObjFunc1 = pflfObjectiveFunction(plfX1, iAbcVectorDimNum);
		lfObjFunc2 = pflfObjectiveFunction(plfX2, iAbcVectorDimNum);

		// ���݂̍œK�l���X�V���܂��B
		if (lfObjFunc2 <= lfObjFunc0)
		{
			memcpy(plfGlobalMinAbcData, plfX2, sizeof(double)*iAbcVectorDimNum);
		}
		if (lfObjFunc0 <= lfObjFunc1)
		{
			memcpy(plfX1, plfX0, sizeof(double)*iAbcVectorDimNum);
			iReCounter = 0;
		}
		else iReCounter++;
		if (iReCounter > iHJCounter)
		{
			// Hooke-Jeeves�@��K�p���܂��B
			vModifiedHookeJeevesMethod(plfStepSize, plfX1, plfX2, plfX0);
		}
	}
}

void CAbc::vModifiedHookeJeevesMethod( double* plfStepSize, double *plfX1, double *plfX2, double *plfX0 )
{
	int i, j, k;
	int iCounter = 1000;
	double lfRes = 0.0;
	double lfStepSize = 1.0;
	double lfObjFunc = 0.0;
	double lfObjFunc1 = 0.0;
	double lfObjFunc2 = 0.0;
	double lfFuncMin = DBL_MAX;
	double rho = 0.5;
	bool bRet;

	// Hooke-Jeeves�@��K�p���܂��B
	for (k = 0; k < iHJCounter; k++)
	{
		// �e�x�N�g���̃X�e�b�v�T�C�Y�̌v�Z�����܂��B
		bRet = bHJEmStep(plfX1, plfX0, plfStepSize);
		if (bRet == false)
		{
			lfObjFunc1 = pflfObjectiveFunction(plfX0, iAbcVectorDimNum);
			lfObjFunc2 = pflfObjectiveFunction(plfX1, iAbcVectorDimNum);
			if (lfObjFunc2 < lfObjFunc1)
			{
				for (;;)
				{
					// Pattern Move(PM step)
					for (i = 0; i < iAbcVectorDimNum; i++)
					{
						if (plfX1[i] < plfX0[i]) plfStepSize[i] = -fabs(plfStepSize[i]);
						else                     plfStepSize[i] = fabs(plfStepSize[i]);
					}
					for (i = 0; i < iAbcVectorDimNum; i++)
					{
						plfX2[i] = plfX1[i] + (plfX1[i] - plfX0[i]);
						plfX0[i] = plfX1[i];
					}
					// EM(Expolration Move) Phase
					bRet = bHJEmStep(plfX1, plfX2, plfStepSize);
					lfObjFunc1 = pflfObjectiveFunction(plfX0, iAbcVectorDimNum);
					lfObjFunc2 = pflfObjectiveFunction(plfX1, iAbcVectorDimNum);
					if (lfObjFunc2 >= lfObjFunc1) break;
				}
			}
		}
		else
		{
			for (;;)
			{
				// Pattern Move(PM step)
				for (i = 0; i < iAbcVectorDimNum; i++)
				{
					if (plfX1[i] < plfX0[i]) plfStepSize[i] = -fabs(plfStepSize[i]);
					else                     plfStepSize[i] = fabs(plfStepSize[i]);
				}
				for (i = 0; i < iAbcVectorDimNum; i++)
				{
					plfX2[i] = plfX1[i] + (plfX1[i] - plfX0[i]);
					plfX0[i] = plfX1[i];
				}
				// EM(Expolration Move) Phase
				bRet = bHJEmStep(plfX1, plfX2, plfStepSize);
				lfObjFunc1 = pflfObjectiveFunction(plfX0, iAbcVectorDimNum);
				lfObjFunc2 = pflfObjectiveFunction(plfX1, iAbcVectorDimNum);
				if (lfObjFunc2 >= lfObjFunc1) break;
			}
		}

		if (lfStepSize < 0.001) break;
		else
		{
			lfStepSize = rho*lfStepSize;
			for (j = 0; j < iAbcVectorDimNum; j++)
				plfStepSize[j] = plfStepSize[j] * rho;
		}
	}
}

bool CAbc::bHJEmStep( double *plfX1, double *plfX0, double *plfStepSize )
{
	double lfObjFunc = 0.0;
	double lfFuncMin = DBL_MAX;
	double lfXi;
	int i;

	memcpy(plfX1, plfX0, sizeof(double)*iAbcVectorDimNum);
	for (i = 0; i < iAbcVectorDimNum; i++)
	{
		lfXi = plfX0[i]+plfStepSize[i];
		plfX1[i] = lfXi;
		lfObjFunc = pflfObjectiveFunction(plfX1, iAbcVectorDimNum);
		if (lfObjFunc < lfFuncMin)	lfFuncMin = lfObjFunc;
		else
		{
			lfXi = plfX0[i] - plfStepSize[i];
			plfX1[i] = lfXi;
			lfObjFunc = pflfObjectiveFunction(plfX1, iAbcVectorDimNum);
			if (lfObjFunc < lfFuncMin)	lfFuncMin = lfObjFunc;
			else				        lfXi = plfX0[i];
		}
		plfX1[i] = lfXi;
	}

	if (lfObjFunc >= lfFuncMin) return false;
	return true;
}

/**
* <PRE>
* �@�l�H�I�R���j�[�œK���@�i�����𓱓�������@�j�����s���܂��B
*   �Z�p������p�������ǌ^Artificial Bee Colony�A���S���Y��, ��28��Fuzzy System Symposium, 9, 2012. ���.
*   ver 0.1 ����
* </PRE>
* @author kobayashi
* @since 2016/10/17
* @version 0.1
*/
void CAbc::vACAbc()
{
	// employee bee �̓���
	vEmployBeeOrigin();

	// onlookers bee�̓���
	vOnlookerBeeAC();

	// scout bee �̎��s
	vScoutBeeNormal();

	// �Ǐ��ő�l�A�ŏ��l���擾���܂��B
	vGetLocalMaxMin();

	// ���I�ő�l�A�ŏ��l���擾���܂��B
	vGetGlobalMaxMin();
}

/**
* <PRE>
* �@������@�𓱓������l�H�I�R���j�[�œK���@�����s���܂��B
*   Ivona Brajevic, Crossover-based artificial bee colony algorithm for constrained optimization problems, Neural Computing & Application (2015) 26:1587-1601.
*   ver 0.1 ����
* </PRE>
* @param iCount ���݂̌v�Z��
* @author kobayashi
* @since 2016/10/19
* @version 0.1
*/
void CAbc::vCBAbc( int iUpdateCount )
{
	double lfMr = 0.1;
	double lfMrMax = 0.9;
	double lfMCN = iGenerationNumber;
	double lfP = 0.3;

	// employee bee �̓���
	vEmployBeeCB( lfMr );

	// onlookers bee�̓���
	vOnlookerBeeCB( lfMr );

	// scout bee �̎��s
	vScoutBeeCB( iUpdateCount );

	// 臒l������X�V�B
	lfMr = lfMr < lfMrMax ? lfMr + (lfMrMax - 0.1) / (lfP*lfMCN) : lfMrMax;

	// �Ǐ��ő�l�A�ŏ��l���擾���܂��B
	vGetLocalMaxMin();

	// ���I�ő�l�A�ŏ��l���擾���܂��B
	vGetGlobalMaxMin();
}

/**
 * <PRE>
 * �@Best-so-far Artificial Bee Colony Method�����s���܂��B
 *   The best-so-far selection in Artificial Bee Colony algorithm Applied Soft Computing 11 (2011) 2888-2901
 *   ver 0.1 2016.10.28 ����
 * </PRE>
 * @param iCount ���݂̌v�Z��
 * @author kobayashi
 * @since 2016/10/28
 * @version 0.1
 */
void CAbc::vBFAbc( int iUpdateCount )
{
	// employee bee �̓���
	vEmployBeeBF();

	// onlookers bee�̓���
	vOnlookerBeeBF();

	// scout bee �̎��s
	vScoutBeeBF( iUpdateCount );

	// �Ǐ��ő�l�A�ŏ��l���擾���܂��B
	vGetLocalMaxMin();

	// ���I�ő�l�A�ŏ��l���擾���܂��B
	vGetGlobalMaxMin();
}

/**
* <PRE>
* �@�l�H�I�R���j�[�œK���@�����s���܂��B
*   Powell�@�𓱓�����PABC�@�����s���܂��B
*   ver 0.1 2016/1104 ����
* </PRE>
* @author kobayashi
* @since 2016/11/04
* @version 0.1
*/
void CAbc::vPAbc()
{
	// employee bee �̓���
	vEmployBeeBest();

	// onlookers bee�̓���
	vOnlookerBeeBest();

	// scout bee �̎��s
	vScoutBeeUndx();

	// �Ǐ��ő�l�A�ŏ��l���擾���܂��B
	vGetLocalMaxMin();

	// ���I�ő�l�A�ŏ��l���擾���܂��B
	vGetGlobalMaxMin();
}


/**
 * <PRE>
 * �@Employ Bee�����s���܂��B(��{�̃o�[�W�����Ɠ�����@)
 *   ver 0.1 2016/08/18 ���� 
 *   ver 0.2 2016/10/25 �X�V���_�̎Z�o�Ɍ��𔭌����C���B
 *   ver 0.3 2016/10/28 �]�v�ȃ��[�v�̍폜�����{�B
 * </PRE>
 * @author kobayashi
 * @since 2016/8/18
 * @version 0.3
 */
void CAbc::vEmployBeeOrigin()
{
	int m,h;
	int i,j;
	double lfRand = 0.0;
	double lfFunc1 = 0.0;
	double lfFunc2 = 0.0;
// employee bee �̓���

	// �X�V�_�����Z�o���܂��B
	m = mrand() % iAbcSearchNum;
	h = mrand() % iAbcVectorDimNum;
	
	for( i = 0;i < iAbcSearchNum; i++ )
	{
		lfRand = 2*rnd()-1;
		for( j = 0; j < iAbcVectorDimNum; j++ )
			pplfVelocityData[i][j] = pplfAbcData[i][j];
		pplfVelocityData[i][h] = pplfAbcData[i][h] + lfRand*( pplfAbcData[i][h] - pplfAbcData[m][h] );

		// �e�T���_�ƍX�V���Ȃ������񐔂��i�[����ϐ����X�V���܂��B
		lfFunc1 = pflfObjectiveFunction( pplfVelocityData[i], iAbcVectorDimNum );
		lfFunc2 = pflfObjectiveFunction( pplfAbcData[i], iAbcVectorDimNum );

		if( lfFunc1 < lfFunc2 )
		{
			pplfAbcData[i][h] = pplfVelocityData[i][h];
			piNonUpdateCount[i] = 0;
		}
		else	piNonUpdateCount[i] = piNonUpdateCount[i] + 1;
	}
}

/**
 * <PRE>
 * �@Employ Bee�����s���܂��B(�����x���o�[�W����)
 *   2011�̓d�q���ʐM�w��̘_�����
 *   ver 0.1 
 *   ver 0.2 2016/10/25 �X�V���_�̎Z�o�Ɍ��𔭌����C���B
 * </PRE>
 * @author kobayashi
 * @since 2016/8/18
 * @version 0.2
 */
double CAbc::lfEmployBeeEnhanced( int iUpdateCount )
{
	int m,h;
	int i,j;
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

	// employee bee �̓���
	for( i = 0;i < iAbcSearchNum; i++ )
	{
		// �K���x�̎Z�o
		lfObjFunc = pflfObjectiveFunction( pplfAbcData[i], iAbcVectorDimNum );
		if( lfObjFunc-lfFitBound >= lfFitAccuracy )	lfFitProb = 1.0/( lfObjFunc-lfFitBound );
		else										lfFitProb = 1.0/lfFitAccuracy;
		plfFit[i] = lfFitProb;

		// �]���l��Np�Z�o���܂��B
		stlFitProb[i].iLoc = i;
		stlFitProb[i].lfFitProb = lfFitProb;
	}
//	// �K���x�̃\�[�g�����s���܂��B
//	qsort( 0, iAbcSearchNum, plfFit );
	// �ړI�֐��l�ɂ��\�[�g�����{���܂��B
	std::sort( stlFitProb.begin(), stlFitProb.end(), CCompareToRank() );

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
				for( i = 0;i < iAbcSearchNum; i++ )	lfRes += stlFitProb[i].lfFitProb; 
				for( i = 0;i < iAbcSearchNum; i++ )	plfFitProb[i] = stlFitProb[i].lfFitProb/lfRes;
			}
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
				m = mrand() % iAbcUpperSearchNum;
			}
			// �����_���Ɍ��肵�܂��B
			h = mrand() % iAbcVectorDimNum;
	
			for( i = 0;i < iAbcSearchNum; i++ )
			{
				lfRand = 2*rnd()-1;
				for( j = 0; j < iAbcVectorDimNum; j++ )
					pplfVelocityData[i][j] = pplfAbcData[i][j];
				pplfVelocityData[i][h] = pplfAbcData[i][h] + lfRand*( pplfAbcData[i][h] - pplfAbcData[stlFitProb[m].iLoc][h] );
			}
		}
		else
		{
			// �e�T���_�̑��Ε]���m�����Z�o���܂��B
			lfRes = 0.0;
			for( i = 0;i < iAbcSearchNum; i++ )	lfRes += stlFitProb[i].lfFitProb; 
			for( i = 0;i < iAbcSearchNum; i++ )	plfFitProb[i] = stlFitProb[i].lfFitProb/lfRes;

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
				m = mrand() % iAbcUpperSearchNum;
			}
			// �����_���Ɍ��肵�܂��B
			h = mrand() % iAbcVectorDimNum;
	
			for( i = 0;i < iAbcSearchNum; i++ )
			{
				lfRand = 2*rnd()-1;
				for( j = 0; j < iAbcVectorDimNum; j++ )
					pplfVelocityData[i][j] = pplfAbcData[i][j];
				pplfVelocityData[i][h] = pplfAbcData[i][h] + lfRand*( pplfAbcData[i][h] - pplfAbcData[stlFitProb[m].iLoc][h] );
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
			m = mrand() % iAbcUpperSearchNum;
		}
		// �����_���Ɍ��肵�܂��B
		h = mrand() % iAbcVectorDimNum;

		for( i = 0;i < iAbcSearchNum; i++ )
		{
			lfRand = 2.0*rnd()-1.0;
			for( j = 0; j < iAbcVectorDimNum; j++ )
				pplfVelocityData[i][j] = pplfAbcData[i][j];
			pplfVelocityData[i][h] = pplfAbcData[i][h] + lfRand*( pplfAbcData[i][h] - pplfAbcData[stlFitProb[m].iLoc][h] );
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
	return lfFitJudge;
}

/**
 * <PRE>
 * �@Employ Bee�����s���܂��B(�����x���o�[�W����)
 *   2011�̓d�q���ʐM�w��̘_�����
 *   ver 0.1 
 *   ver 0.2 2016/10/25 �X�V���_�̎Z�o�Ɍ��𔭌����C���B
 * </PRE>
 * @author kobayashi
 * @since 2016/8/18
 * @version 0.2
 */
double CAbc::lfEmployBeeBestEnhanced( int iUpdateCount )
{
	int m,h;
	int i,j;
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

	// employee bee �̓���
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
					m = mrand() % iAbcUpperSearchNum;
				}
				// �����_���Ɍ��肵�܂��B
				h = mrand() % iAbcVectorDimNum;
	
				for( i = 0;i < iAbcSearchNum; i++ )
				{
					lfRand = 2*rnd()-1;
					for( j = 0; j < iAbcVectorDimNum; j++ )
						pplfVelocityData[i][j] = pplfAbcData[i][j];
//					pplfVelocityData[i][h] = pplfAbcData[i][h] + lfRand*( pplfAbcData[i][h] - pplfAbcData[m][h] );
					pplfVelocityData[i][h] = pplfAbcData[i][h] + lfRand*( pplfAbcData[i][h] - pplfLocalMinAbcData[m][h] );
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
				m = mrand() % iAbcUpperSearchNum;
			}
			// �����_���Ɍ��肵�܂��B
			h = mrand() % iAbcVectorDimNum;
	
			for( i = 0;i < iAbcSearchNum; i++ )
			{
				lfRand = 2*rnd()-1;
				for( j = 0; j < iAbcVectorDimNum; j++ )
					pplfVelocityData[i][j] = pplfAbcData[i][j];
//				pplfVelocityData[i][h] = pplfAbcData[i][h] + lfRand*( pplfAbcData[i][h] - pplfAbcData[m][h] );
				pplfVelocityData[i][h] = pplfAbcData[i][h] + lfRand*( pplfAbcData[i][h] - pplfLocalMinAbcData[m][h] );
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
			m = mrand() % iAbcUpperSearchNum;
		}
		// �����_���Ɍ��肵�܂��B
		h = mrand() % iAbcVectorDimNum;

		for( i = 0;i < iAbcSearchNum; i++ )
		{
			lfRand = 2.0*rnd()-1.0;
			for( j = 0; j < iAbcVectorDimNum; j++ )
				pplfVelocityData[i][j] = pplfAbcData[i][j];
//			pplfVelocityData[i][h] = pplfAbcData[i][h] + lfRand*( pplfAbcData[i][h] - pplfAbcData[m][h] );
			pplfVelocityData[i][h] = pplfAbcData[i][h] + lfRand*( pplfAbcData[i][h] - pplfLocalMinAbcData[m][h] );
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
			piNonUpdateCount[i] = 0;
		}
		else	piNonUpdateCount[i] = piNonUpdateCount[i] + 1;
	}
	return lfFitJudge;
}

/**
 * <PRE>
 * �@Employ Bee�����s���܂��B
 *   ver 0.1 
 *   ver 0.2 NBest�łɏC��
 *   ver 0.3 2016/10/25 �X�V���_�̎Z�o�Ɍ��𔭌����C���B
 *   ver 0.4 2016/10/28 �]�v�ȃ��[�v�̍폜�����{�B
 * </PRE>
 * @author kobayashi
 * @since 2016/8/10
 * @version 0.3
 */
void CAbc::vEmployBeeBest()
{
	int m,h;
	int i,j;
	double lfRand = 0.0;
	double lfFunc1 = 0.0;
	double lfFunc2 = 0.0;
// employee bee �̓���

	// �X�V�_�����Z�o���܂��B
	m = mrand() % iAbcSearchNum;
	h = mrand() % iAbcVectorDimNum;
	
	for( i = 0;i < iAbcSearchNum; i++ )
	{
		lfRand = 2*rnd()-1;
		for( j = 0; j < iAbcVectorDimNum; j++ )
			pplfVelocityData[i][j] = pplfAbcData[i][j];
		pplfVelocityData[i][h] = pplfAbcData[i][h] + lfRand*( pplfAbcData[i][h] - pplfLocalMinAbcData[m][h] );

		// �e�T���_�ƍX�V���Ȃ������񐔂��i�[����ϐ����X�V���܂��B
		lfFunc1 = pflfObjectiveFunction( pplfVelocityData[i], iAbcVectorDimNum );
		lfFunc2 = pflfObjectiveFunction( pplfAbcData[i], iAbcVectorDimNum );

		if( lfFunc1 < lfFunc2 )
		{
			pplfAbcData[i][h] = pplfVelocityData[i][h];
			piNonUpdateCount[i] = 0;
		}
		else	piNonUpdateCount[i] = piNonUpdateCount[i] + 1;
	}
}

/**
 * <PRE>
 * �@Employ Bee�����s���܂��B(GBest��)
 *   ver 0.1 
 *   ver 0.2 NBest�łɏC��
 *   ver 0.3 2016/10/24 �_������ɏC��
 *   ver 0.4 2016/10/25 �X�V���_�̎Z�o�Ɍ��𔭌����C���B
 *   ver 0.5 2016/10/28 �]�v�ȃ��[�v�̍폜�����{�B
 * </PRE>
 * @author kobayashi
 * @since 2016/8/10
 * @version 0.4
 */
void CAbc::vEmployBeeGBest()
{
	int m,h;
	int i,j;
	double lfRand = 0.0;
	double lfRand2 = 0.0;
	double lfFunc1 = 0.0;
	double lfFunc2 = 0.0;
// employee bee �̓���

	// �X�V�_�����Z�o���܂��B
	m = mrand() % iAbcSearchNum;
	h = mrand() % iAbcVectorDimNum;
	
	for( i = 0;i < iAbcSearchNum; i++ )
	{
		lfRand = 2.0*rnd()-1.0;
		lfRand2 = 1.5*rnd();
		for( j = 0; j < iAbcVectorDimNum; j++ )
			pplfVelocityData[i][j] = pplfAbcData[i][j];
//			pplfVelocityData[i][j] = pplfAbcData[i][j] + lfRand*(pplfAbcData[i][j] - pplfAbcData[m][j]) + lfRand2*(plfGlobalMinAbcData[j] - pplfAbcData[i][j]);
		pplfVelocityData[i][h] = pplfAbcData[i][h] + lfRand*( pplfAbcData[i][h] - pplfAbcData[m][h] ) + lfRand2*(plfGlobalMinAbcData[h] - pplfAbcData[i][h] );

		// �e�T���_�ƍX�V���Ȃ������񐔂��i�[����ϐ����X�V���܂��B
		lfFunc1 = pflfObjectiveFunction( pplfVelocityData[i], iAbcVectorDimNum );
		lfFunc2 = pflfObjectiveFunction( pplfAbcData[i], iAbcVectorDimNum );

		if( lfFunc1 < lfFunc2 )
		{
			pplfAbcData[i][h] = pplfVelocityData[i][h];
			piNonUpdateCount[i] = 0;
		}
		else 	piNonUpdateCount[i] = piNonUpdateCount[i] + 1;
	}
}

/**
 * <PRE>
 * �@Employ Bee�����s���܂��B
 *   ver 0.1 
 *   ver 0.2 2016/08/18 IWCFA(���q�Q�œK���@�̈��@��K�p)
 *   ver 0.3 2016/10/28 �]�v�ȃ��[�v�̍폜�����{�B
 * </PRE>
 * @param lfK
 * @param lfCoe1
 * @param lfCoe2
 * @param iUpdateCount
 * @author kobayashi
 * @since 2016/8/10
 * @version 0.2
 */
void CAbc::vEmployBeeIWCFA( double lfK, double lfCoe1, double lfCoe2, int iUpdateCount )
{
	int m,h;
	int i,j;
	double lfRand = 0.0;
	double lfRand2 = 0.0;
	double lfFunc1 = 0.0;
	double lfFunc2 = 0.0;
	double lfWeight = 0.0;
	double lfMaxWeight = 1.0;
	double lfMinWeight = 0.3;

	// �X�V�_�����Z�o���܂��B
	m = mrand() % iAbcSearchNum;
	h = mrand() % iAbcVectorDimNum;
	
	for( i = 0;i < iAbcSearchNum; i++ )
	{
		lfRand = 2.0*rnd()-1.0;
//		lfRand2 = 2.0*rnd()-1.0;
		lfRand2 = rnd();
		for( j = 0; j < iAbcVectorDimNum; j++ )
			pplfVelocityData[i][j] = pplfAbcData[i][j];
//		pplfVelocityData[i][h] = pplfAbcData[i][h] + lfRand*( pplfAbcData[i][h] - pplfAbcData[m][h] );
		lfWeight = lfMaxWeight - (lfMaxWeight-lfMinWeight)/(double)iGenerationNumber*(double)(iUpdateCount-piTotalNonUpdateCount[m]);
		pplfVelocityData[i][h] = pplfAbcData[i][h] + lfCoe1*lfRand*( pplfAbcData[i][h] - pplfAbcData[m][h] ) + lfCoe2*lfRand2*( pplfAbcData[i][h] - pplfLocalMinAbcData[m][h] );

		// �e�T���_�ƍX�V���Ȃ������񐔂��i�[����ϐ����X�V���܂��B
		lfFunc1 = pflfObjectiveFunction( pplfVelocityData[i], iAbcVectorDimNum );
		lfFunc2 = pflfObjectiveFunction( pplfAbcData[i], iAbcVectorDimNum );

		if( lfFunc1 < lfFunc2 )
		{
			pplfAbcData[i][h] = pplfVelocityData[i][h];
			piNonUpdateCount[i] = 0;
		}
		else
		{
			piNonUpdateCount[i] = piNonUpdateCount[i] + 1;
			piTotalNonUpdateCount[i] = piTotalNonUpdateCount[i] + 1;
		}
	}
}

/**
* <PRE>
*   Ivona Brajevic, Crossover-based artificial bee colony algorithm for constrained optimization problems, Neural Computing & Application (2015) 26:1587-1601.
*   ver 0.1 2016/10/19 ����
*   ver 0.2 2016/10/28 �]�v�ȃ��[�v�̍폜�����{�B
* </PRE>
* @param lfMr �X�V�p�p�����[�^�[
* @author kobayashi
* @since 2016/10/19
* @version 0.1
*/
void CAbc::vEmployBeeCB( double lfMr )
{
	int m, h;
	int i, j;
	double lfRand = 0.0;
	double lfFunc1 = 0.0;
	double lfFunc2 = 0.0;
	double lfMrMax = 0.9;
	double lfP = 0.3;
	double lfMCN = iGenerationNumber;
	double lfLowerVelocity = -DBL_MAX;
	double lfUpperVelocity = DBL_MAX;

	// employee bee �̓���
	// �X�V�_�����Z�o���܂��B
	m = mrand() % iAbcSearchNum;

	for (i = 0; i < iAbcSearchNum; i++)
	{
		lfRand = 2 * rnd() - 1;
		for (j = 0; j < iAbcVectorDimNum; j++)
		{
			pplfVelocityData[i][j] = rnd() < lfMr ? pplfAbcData[i][j] + lfRand*(pplfAbcData[i][j] - pplfAbcData[m][j]) : pplfAbcData[i][j];
			if (pplfVelocityData[i][j] < lfLowerVelocity) pplfVelocityData[i][j] = 2.0*lfLowerVelocity - pplfVelocityData[i][j];
			else if (pplfVelocityData[i][j] > lfUpperVelocity) pplfVelocityData[i][j] = 2.0*lfUpperVelocity - pplfVelocityData[i][j];
		}
		// �e�T���_�ƍX�V���Ȃ������񐔂��i�[����ϐ����X�V���܂��B
		lfFunc1 = pflfObjectiveFunction(pplfVelocityData[i], iAbcVectorDimNum);
		lfFunc2 = pflfObjectiveFunction(pplfAbcData[i], iAbcVectorDimNum);

		if (lfFunc1 < lfFunc2)
		{
			for( j = 0;j < iAbcVectorDimNum; j++ )
				pplfAbcData[i][j] = pplfVelocityData[i][j];
			piNonUpdateCount[i] = 0;
		}
		else	piNonUpdateCount[i] = piNonUpdateCount[i] + 1;
	}
}

/**
 * <PRE>
 * �@Employ Bee�����s���܂��B(Best-so-Far��)
 *   The best-so-far selection in Artificial Bee Colony algorithm Applied Soft Computing 11 (2011) 2888-2901
 *   ver 0.1 2016.10.28 ����
 * </PRE>
 * @author kobayashi
 * @since 2016/10/28
 * @version 0.1
 */
void CAbc::vEmployBeeBF()
{
	int m,h;
	int i,j;
	double lfRand = 0.0;
	double lfRand2 = 0.0;
	double lfFunc1 = 0.0;
	double lfFunc2 = 0.0;
	double lfObjFunc = 0.0;
	double lfLocalMinAbcData = DBL_MAX;

	// employee bee �̓���
	// �X�V�_�����Z�o���܂��B
	m = mrand() % iAbcSearchNum;
	h = mrand() % iAbcVectorDimNum;
	
	for( i = 0;i < iAbcSearchNum; i++ )
	{
		lfRand = 2.0*rnd()-1.0;
		for( j = 0; j < iAbcVectorDimNum; j++ )
			pplfVelocityData[i][j] = pplfAbcData[i][j];
		pplfVelocityData[i][h] = pplfAbcData[i][h] + lfRand*(pplfAbcData[i][h] - pplfAbcData[m][h]);

		// �e�T���_�ƍX�V���Ȃ������񐔂��i�[����ϐ����X�V���܂��B
		lfFunc1 = pflfObjectiveFunction( pplfVelocityData[i], iAbcVectorDimNum );
		lfFunc2 = pflfObjectiveFunction( pplfAbcData[i], iAbcVectorDimNum );
		if( lfFunc1 < lfFunc2 )
		{
			pplfAbcData[i][h] = pplfVelocityData[i][h];
			piNonUpdateCount[i] = 0;
		}
		else 	piNonUpdateCount[i] = piNonUpdateCount[i] + 1;
	}

	for (j = 0; j < iAbcVectorDimNum; j++)
		plfLocalMinAbcData[j] = pplfAbcData[0][j];
	lfLocalMinAbcData = pflfObjectiveFunction(plfLocalMinAbcData, iAbcVectorDimNum);
	for(i = 1;i < iAbcSearchNum; i++ )
	{
		lfObjFunc = pflfObjectiveFunction(pplfAbcData[i], iAbcVectorDimNum );
		if( lfObjFunc < lfLocalMinAbcData )
		{
			for( j = 0; j < iAbcVectorDimNum; j++ )
				plfLocalMinAbcData[j] = pplfAbcData[i][j];
			lfLocalMinAbcData = lfObjFunc;
		}
	}
}

/**
 * <PRE>
 *   Onlooker Bee�����s���܂��B(��{�̃o�[�W����)
 *   ver 0.1 2016/08/10 ����
 *   ver 0.2 2016/10/25 �X�V���_�̎Z�o�Ɍ��𔭌����C���B
 * </PRE>
 * @author kobayashi
 * @since 2016/8/10
 * @version 0.2
 */
void CAbc::vOnlookerBeeOrigin()
{
	int i,j;
	int c,m,h;
	double lfRes = 0.0;
	double lfRand = 0.0;
	double lfFitProb = 0.0;
	double lfProb = 0.0;
	double lfPrevProb = 0.0;
	double lfFunc1 = 0.0;
	double lfFunc2 = 0.0;
	double lfObjFunc = 0.0;

	lfRes = 0.0;
	for(j = 0;j < iAbcSearchNum; j++ )
	{
		// �K���x�̎Z�o
		lfObjFunc = pflfObjectiveFunction( pplfAbcData[j], iAbcVectorDimNum );
		if( lfObjFunc >= 0.0 )	lfFitProb = 1.0/( 1.0+lfObjFunc );
		else			lfFitProb = 1.0+fabs( lfObjFunc );
		lfRes += lfFitProb;
		plfFit[j] = lfFitProb;
	}
	// �K���x�̐��K��
	for( j = 0;j < iAbcSearchNum; j++ )	plfFitProb[j] = plfFit[j]/lfRes;
	// ���[���b�g�헪�����s
	lfProb = lfPrevProb = 0.0;
	lfRand = rnd();
	c = 0;
	for( j = 0;j < iAbcSearchNum; j++ )
	{
		lfProb += plfFitProb[j];
		if( lfPrevProb <= lfRand && lfRand <= lfProb )	c = j;
		lfPrevProb = lfProb;
	}
	// ���[���b�g�I�������T���_�ɑ΂��čX�V�������߂܂��B

	// �X�V�_�����Z�o���܂��B
	// �X�V�_���𗐐��ɂ�茈�肵�܂��B
	m = mrand() % iAbcSearchNum;
	h = mrand() % iAbcVectorDimNum;

	lfRand = 2*rnd()-1;
	for( j = 0; j < iAbcVectorDimNum; j++ )
		pplfVelocityData[c][j] = pplfAbcData[c][j];
	pplfVelocityData[c][h] = pplfAbcData[c][h] + lfRand*( pplfAbcData[c][h] - pplfAbcData[m][h] );
	
	// �X�V�_�������̂悤�ɍX�V���܂��B
	lfFunc1 = pflfObjectiveFunction( pplfVelocityData[c], iAbcVectorDimNum );
	lfFunc2 = pflfObjectiveFunction( pplfAbcData[c], iAbcVectorDimNum );
	if( lfFunc1 < lfFunc2 )
	{
		for( j = 0;j < iAbcVectorDimNum; j++ )
			pplfAbcData[c][j] = pplfVelocityData[c][j];
		piNonUpdateCount[c] = 0;
	}
	else	piNonUpdateCount[c] = piNonUpdateCount[c] + 1;
}

/**
 * <PRE>
 * �@Onlooker Bee�����s���܂��B(�����x���o�[�W����)
 *   2011�̓d�q���ʐM�w��̘_�����
 *   ver 0.1 2016/08/18 ����
 *   ver 0.2 2016/10/25 �X�V���_�̎Z�o�Ɍ��𔭌����C���B
 * </PRE>
 * @author kobayashi
 * @since 2016/8/18
 * @version 0.1
 */
void CAbc::vOnlookerBeeEnhanced( int iUpdateCount, double lfFitJudge )
{
	int i,j,l;
	int c,m,h;
	double lfRes = 0.0;
	double lfRand = 0.0;
	double lfFitProb = 0.0;
	double lfProb = 0.0;
	double lfPrevProb = 0.0;
	double lfFunc1 = 0.0;
	double lfFunc2 = 0.0;
	double lfObjFunc = 0.0;

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
			if( lfPrevProb <= lfRand &&	lfRand <= lfProb )	c = j;
			lfPrevProb = lfProb;
		}
	}
	else
	{
		// ���̑��̏ꍇ�͓K���x��ʃ����烉���_���Ɍ��肵�܂��B
		c = mrand() % iAbcUpperSearchNum;
	}
	// �����_���Ɍ��肵�܂��B
	h = mrand() % iAbcVectorDimNum;
	
	// �X�V�_���𐶐����܂��B
	lfRand = 2*rnd()-1;
	for( j = 0; j < iAbcVectorDimNum; j++ )
		pplfVelocityData[stlFitProb[c].iLoc][j] = pplfAbcData[stlFitProb[c].iLoc][j];
	pplfVelocityData[stlFitProb[c].iLoc][h] = pplfAbcData[stlFitProb[c].iLoc][h] + lfRand*( pplfAbcData[stlFitProb[c].iLoc][h] - pplfAbcData[stlFitProb[c].iLoc][h] );

	// �e�T���_���X�V���܂��B
	lfFunc1 = pflfObjectiveFunction( pplfVelocityData[stlFitProb[c].iLoc], iAbcVectorDimNum );
	lfFunc2 = pflfObjectiveFunction( pplfAbcData[stlFitProb[c].iLoc], iAbcVectorDimNum );
	if( lfFunc1 < lfFunc2 )
	{
		for( j = 0;j < iAbcVectorDimNum; j++ )
			pplfAbcData[stlFitProb[c].iLoc][j] = pplfVelocityData[stlFitProb[c].iLoc][j];
	}
}

/**
 * <PRE>
 * �@Onlooker Bee�����s���܂��B(�����x���o�[�W����)
 *   2011�̓d�q���ʐM�w��̘_�����
 *   ver 0.1 2016/08/18 ���� 
 *   ver 0.2 2016/10/25 �X�V���_�̎Z�o�Ɍ��𔭌����C���B
 * </PRE>
 * @author kobayashi
 * @since 2016/8/18
 * @version 0.1
 */
void CAbc::vOnlookerBeeBestEnhanced( int iUpdateCount, double lfFitJudge )
{
	int i,j;
	int c,m,h;
	double lfRes = 0.0;
	double lfRand = 0.0;
	double lfFitProb = 0.0;
	double lfProb = 0.0;
	double lfPrevProb = 0.0;
	double lfFunc1 = 0.0;
	double lfFunc2 = 0.0;
	double lfObjFunc = 0.0;

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
		c = mrand() % iAbcUpperSearchNum;
	}
	// �����_���Ɍ��肵�܂��B
	h = mrand() % iAbcVectorDimNum;

	// �X�V�_���𐶐����܂��B
	for( i = 0;i < iAbcSearchNum; i++ )
	{
		lfRand = 2*rnd()-1;
		for( j = 0; j < iAbcVectorDimNum; j++ )
			pplfVelocityData[i][j] = pplfAbcData[i][j];
		pplfVelocityData[i][h] = pplfAbcData[i][h] + lfRand*( pplfAbcData[i][h] - plfGlobalMinAbcData[h] );
	}
	// �e�T���_���X�V���܂��B
	lfFunc1 = pflfObjectiveFunction( pplfVelocityData[stlFitProb[c].iLoc], iAbcVectorDimNum );
	lfFunc2 = pflfObjectiveFunction( pplfAbcData[stlFitProb[c].iLoc], iAbcVectorDimNum );
	if( lfFunc1 < lfFunc2 )
	{
		for( j = 0;j < iAbcVectorDimNum; j++ )
			pplfAbcData[c][j] = pplfVelocityData[stlFitProb[c].iLoc][j];
		piNonUpdateCount[c] = 0;
	}
	else	piNonUpdateCount[stlFitProb[c].iLoc] = piNonUpdateCount[stlFitProb[c].iLoc] + 1;
}

/**
 * <PRE>
 *   Onlooker Bee�����s���܂��B(NBest��)
 *   ver 0.1 2016/08/10 ����
 *   ver 0.2 2016/09/13 NBest�ł�ǉ�
 *   ver 0.3 2016/10/25 �X�V���_�̎Z�o�Ɍ��𔭌����C���B
 * </PRE>
 * @author kobayashi
 * @since 2016/8/10
 * @version 0.3
 */
void CAbc::vOnlookerBeeBest()
{
	int i,j;
	int c,m,h;
	double lfRes = 0.0;
	double lfRand = 0.0;
	double lfFitProb = 0.0;
	double lfProb = 0.0;
	double lfPrevProb = 0.0;
	double lfFunc1 = 0.0;
	double lfFunc2 = 0.0;
	double lfObjFunc = 0.0;

	lfRes = 0.0;
	for(j = 0;j < iAbcSearchNum; j++ )
	{
		// �K���x�̎Z�o
		lfObjFunc = pflfObjectiveFunction( pplfAbcData[j], iAbcVectorDimNum );
		if( lfObjFunc >= 0.0 )	lfFitProb = 1.0/( 1.0+lfObjFunc );
		else			lfFitProb = 1.0+fabs( lfObjFunc );
		lfRes += lfFitProb;
		plfFit[j] = lfFitProb;
	}
	// �K���x�̐��K��
	for( j = 0;j < iAbcSearchNum; j++ )	plfFitProb[j] = plfFit[j]/lfRes;
	// ���[���b�g�헪�����s
	lfProb = lfPrevProb = 0.0;
	lfRand = rnd();
	c = 0;
	for( j = 0;j < iAbcSearchNum; j++ )
	{
		lfProb += plfFitProb[j];
		if( lfPrevProb <= lfRand && lfRand <= lfProb )	c = j;
		lfPrevProb = lfProb;
	}

	// ���[���b�g�I�������T���_�ɑ΂��čX�V�������߂܂��B

	// �X�V�_�����Z�o���܂��B
	// �X�V�_���𗐐��ɂ�茈�肵�܂��B
	m = mrand() % iAbcSearchNum;
	h = mrand() % iAbcVectorDimNum;

	lfRand = 2*rnd()-1;
	for( j = 0; j < iAbcVectorDimNum; j++ )
		pplfVelocityData[c][j] = pplfAbcData[c][j];
	pplfVelocityData[c][h] = pplfAbcData[c][h] + lfRand*( pplfAbcData[c][h] - plfGlobalMinAbcData[h] );

	// �X�V�_�������̂悤�ɍX�V���܂��B
	lfFunc1 = pflfObjectiveFunction( pplfVelocityData[c], iAbcVectorDimNum );
	lfFunc2 = pflfObjectiveFunction( pplfAbcData[c], iAbcVectorDimNum );

	if( lfFunc1 < lfFunc2 )
	{
		for( j = 0;j < iAbcVectorDimNum; j++ )
			pplfAbcData[c][j] = pplfVelocityData[c][j];
		piNonUpdateCount[c] = 0;
	}
	else	piNonUpdateCount[c] = piNonUpdateCount[c] + 1;
}

/**
 * <PRE>
 *   Onlooker Bee�����s���܂��B(GBest��)
 *   ver 0.1 2016/08/10 ����
 *   ver 0.2 2016/09/13 NBest�ł�ǉ�
 *   ver 0.3 2016/10/25 �X�V���_�̎Z�o�Ɍ��𔭌����C���B
 * </PRE>
 * @author kobayashi
 * @since 2016/8/10
 * @version 0.3
 */
void CAbc::vOnlookerBeeGBest()
{
	int i,j;
	int c,m,h;
	double lfRes = 0.0;
	double lfRand = 0.0;
	double lfRand2 = 0.0;
	double lfFitProb = 0.0;
	double lfProb = 0.0;
	double lfPrevProb = 0.0;
	double lfFunc1 = 0.0;
	double lfFunc2 = 0.0;
	double lfObjFunc = 0.0;

	lfRes = 0.0;
	for(j = 0;j < iAbcSearchNum; j++ )
	{
		// �K���x�̎Z�o
		lfObjFunc = pflfObjectiveFunction( pplfAbcData[j], iAbcVectorDimNum );
//		if( lfObjFunc >= 0.0 )	lfFitProb = 1.0/( 1.0+lfObjFunc );
//		else					lfFitProb = 1.0+fabs( lfObjFunc );
		lfFitProb = lfObjFunc;
		lfRes += lfFitProb;
		plfFit[j] = lfFitProb;
	}
	// �K���x�̐��K��
	for( j = 0;j < iAbcSearchNum; j++ ) plfFitProb[j] = plfFit[j]/lfRes;
	// ���[���b�g�헪�����s
	lfProb = lfPrevProb = 0.0;
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
	m = mrand() % iAbcSearchNum;
	h = mrand() % iAbcVectorDimNum;

	lfRand = 2.0*rnd()-1.0;
	lfRand2 = 1.5*rnd();
	for( j = 0; j < iAbcVectorDimNum; j++ )
//		pplfVelocityData[c][j] = pplfAbcData[c][j] + lfRand*(pplfAbcData[c][j] - pplfAbcData[m][j]) + lfRand2*(plfGlobalMinAbcData[j] - pplfAbcData[c][j]);
		pplfVelocityData[c][j] = pplfAbcData[c][j];
	pplfVelocityData[c][h] = pplfAbcData[c][h] + lfRand*( pplfAbcData[c][h] - pplfAbcData[m][h] ) + lfRand2*( plfGlobalMinAbcData[h] - pplfAbcData[c][h] );
	// �X�V�_�������̂悤�ɍX�V���܂��B
	lfFunc1 = pflfObjectiveFunction( pplfVelocityData[c], iAbcVectorDimNum );
	lfFunc2 = pflfObjectiveFunction( pplfAbcData[c], iAbcVectorDimNum );

	if( lfFunc1 < lfFunc2 )
	{
		for( j = 0;j < iAbcVectorDimNum; j++ )
			pplfAbcData[c][j] = pplfVelocityData[c][j];
		piNonUpdateCount[c] = 0;
	}
	else 	
	{
		piNonUpdateCount[c] = piNonUpdateCount[c] + 1;
//		piTotalNonUpdateCount[c] = piTotalNonUpdateCount[c] + 1;
	}
}

/**
 * <PRE>
 *   Onlooker Bee�����s���܂��B(IWCFA��)
 *   ver 0.1 2016/08/10 ����
 *   ver 0.2 2016/09/13 IWCFA�ł�ǉ�
 *   ver 0.3 2016/10/25 �X�V���_�̎Z�o�Ɍ��𔭌����C���B
 * </PRE>
 * @param lfK
 * @param lfCoe1
 * @param lfCoe2
 * @param iUpdateCount
 * @author kobayashi
 * @since 2016/8/10
 * @version 0.3
 */
void CAbc::vOnlookerBeeIWCFA( double lfK, double lfCoe1, double lfCoe2, int iUpdateCount )
{
	int i,j;
	int c,m,h;
	double lfRes = 0.0;
	double lfRand = 0.0;
	double lfRand2 = 0.0;
	double lfFitProb = 0.0;
	double lfProb = 0.0;
	double lfPrevProb = 0.0;
	double lfFunc1 = 0.0;
	double lfFunc2 = 0.0;
	double lfObjFunc = 0.0;
	double lfWeight = 0.0;
	double lfMaxWeight = 1.0;
	double lfMinWeight = 0.3;

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
	lfProb = lfPrevProb = 0.0;
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
	m = mrand() % iAbcSearchNum;
	h = mrand() % iAbcVectorDimNum;

	lfRand = 2.0*rnd()-1.0;
	lfRand2 = 2.0*rnd()-1.0;
	for( j = 0; j < iAbcVectorDimNum; j++ )
		pplfVelocityData[c][j] = pplfAbcData[c][j];
	lfWeight = lfMaxWeight - (lfMaxWeight-lfMinWeight)/(double)iGenerationNumber*(double)(iUpdateCount-piTotalNonUpdateCount[c]);
	pplfVelocityData[c][h] = lfK*(pplfAbcData[c][h] + lfCoe1*lfRand*( pplfAbcData[c][h] - pplfAbcData[m][h] ) + lfCoe2*lfRand2*( pplfAbcData[c][h] - plfGlobalMinAbcData[h] ) );
	// �X�V�_�������̂悤�ɍX�V���܂��B
	lfFunc1 = pflfObjectiveFunction( pplfVelocityData[c], iAbcVectorDimNum );
	lfFunc2 = pflfObjectiveFunction( pplfAbcData[c], iAbcVectorDimNum );

	if( lfFunc1 < lfFunc2 )
	{
		for( j = 0;j < iAbcVectorDimNum; j++ )
			pplfAbcData[c][j] = pplfVelocityData[c][j];
		piNonUpdateCount[c] = 0;
	}
	else 	
	{
		piNonUpdateCount[c] = piNonUpdateCount[c] + 1;
		piTotalNonUpdateCount[c] = piTotalNonUpdateCount[c] + 1;
	}
}

/**
 * <PRE>
 *   Onlooker Bee�����s���܂��B
 *   Randomized Memtic Bee Colony Method�p
 *   ver 0.1 2016/09/22 ����
 *   ver 0.2 2016/10/25 �X�V���_�̎Z�o�Ɍ��𔭌����C���B
 *   ver 0.3 2016/10/28 �ꕔ��肪����C���B
 * </PRE>
 * @author kobayashi
 * @since 2016/9/22
 * @version 0.3
 */
void CAbc::vOnlookerBeeRM()
{
	int i,j;
	int c,m,h;
	double lfRes = 0.0;
	double lfRand = 0.0;
	double lfRand2 = 0.0;
	double lfFitProb = 0.0;
	double lfProb = 0.0;
	double lfPrevProb = 0.0;
	double lfFunc1 = 0.0;
	double lfFunc2 = 0.0;
	double lfObjFunc = 0.0;

	lfRes = 0.0;
	for(j = 0;j < iAbcSearchNum; j++ )
	{
		// �K���x�̎Z�o
		lfObjFunc = pflfObjectiveFunction( pplfAbcData[j], iAbcVectorDimNum );
		if( lfObjFunc >= 0.0 )	lfFitProb = 1.0/( 2.0*lfObjFunc+1.0 );
		else			lfFitProb = 1.0+fabs( 1.0/lfObjFunc );
		lfRes += lfFitProb;
		plfFit[j] = lfFitProb;
	}
	// �K���x�̐��K��
	for( j = 0;j < iAbcSearchNum; j++ )	plfFitProb[j] = plfFit[j]/lfRes;
	// ���[���b�g�헪�����s
	lfProb = lfPrevProb = 0.0;
	lfRand = rnd();
	c = 0;
	for( j = 0;j < iAbcSearchNum; j++ )
	{
		lfProb += plfFitProb[j];
		if( lfPrevProb <= lfRand && lfRand <= lfProb )	c = j;
		lfPrevProb = lfProb;
	}

	// ���[���b�g�I�������T���_�ɑ΂��čX�V�������߂܂��B

	// �X�V�_�����Z�o���܂��B
	// �X�V�_���𗐐��ɂ�茈�肵�܂��B
	m = mrand() % iAbcSearchNum;
	h = mrand() % iAbcVectorDimNum;

	lfRand = 2.0*rnd()-1.0;
	lfRand2 = 1.5*rnd();
	for( j = 0; j < iAbcVectorDimNum; j++ )
		pplfVelocityData[c][j] = pplfAbcData[c][j];
	pplfVelocityData[c][h] = pplfAbcData[c][h] + lfRand*( pplfAbcData[c][h] - pplfAbcData[m][h] ) + lfRand2*( plfGlobalMinAbcData[h] - pplfAbcData[c][h] );
		
	// �X�V�_�������̂悤�ɍX�V���܂��B
	lfFunc1 = pflfObjectiveFunction( pplfVelocityData[c], iAbcVectorDimNum );
	lfFunc2 = pflfObjectiveFunction( pplfAbcData[c], iAbcVectorDimNum );

	if( lfFunc1 < lfFunc2 )
	{
		for( j = 0;j < iAbcVectorDimNum; j++ )
			pplfAbcData[c][j] = pplfVelocityData[c][j];
		piNonUpdateCount[c] = 0;
	}
	else	piNonUpdateCount[c] = piNonUpdateCount[c] + 1;
}

/**
 * <PRE>
 *   Onlooker Bee�����s���܂��B(Hooke-Jeeves�@�p)
 *   Artificial Bee Colony Algorithm ith Local Search for Numerical Optimization, Jornal of Software, Vol.6, no.3, march 2011���
 *   ver 0.1 2016/10/03 ����
 *   ver 0.2 2016/10/25 �X�V���_�̎Z�o�Ɍ��𔭌����C���B
 * </PRE>
 * @author kobayashi
 * @since 2016/10/3
 * @version 0.2
 */
void CAbc::vOnlookerBeeHJ()
{
	int i,j;
	int c,m,h;
	double lfRes = 0.0;
	double lfRand = 0.0;
	double lfFitProb = 0.0;
	double lfProb = 0.0;
	double lfPrevProb = 0.0;
	double lfFunc1 = 0.0;
	double lfFunc2 = 0.0;
	double lfObjFunc = 0.0;
	std::vector<Rank_t> stlRank;
	Rank_t tempRank;
	double lfSP = 1.5;

	// �K���x�Ń\�[�g����B
	for( i = 0;i < iAbcSearchNum; i++ )
	{
		lfObjFunc = pflfObjectiveFunction( pplfAbcData[i], iAbcVectorDimNum );
		tempRank.iLoc = i;
		tempRank.lfFitProb = lfObjFunc;
		stlRank.push_back( tempRank );
	}
	std::sort( stlRank.begin(), stlRank.end(), CCompareToRank() );

	lfRes = 0.0;
	for(j = 0;j < iAbcSearchNum; j++ )
	{
		// �K���x�̎Z�o
		lfObjFunc = pflfObjectiveFunction( pplfAbcData[stlRank.at(j).iLoc], iAbcVectorDimNum );
		lfFitProb = 2.0-lfSP+2.0*(lfSP-1.0)*(j-1.0)/( iAbcSearchNum-1.0 );
		lfRes += lfFitProb;
		plfFit[stlRank.at(j).iLoc] = lfFitProb;
	}
	// �K���x�̐��K��
	for( j = 0;j < iAbcSearchNum; j++ )	plfFitProb[j] = plfFit[stlRank.at(j).iLoc]/lfRes;
	// ���[���b�g�헪�����s
	lfProb = lfPrevProb = 0.0;
	lfRand = rnd();
	c = 0;
	for( j = 0;j < iAbcSearchNum; j++ )
	{
		lfProb += plfFitProb[stlRank.at(j).iLoc];
		if( lfPrevProb <= lfRand && lfRand <= lfProb )	c = stlRank.at(j).iLoc;
		lfPrevProb = lfProb;
	}

	// ���[���b�g�I�������T���_�ɑ΂��čX�V�������߂܂��B

	// �X�V�_�����Z�o���܂��B
	// �X�V�_���𗐐��ɂ�茈�肵�܂��B
	m = mrand() % iAbcSearchNum;
	h = mrand() % iAbcVectorDimNum;
	lfRand = 2*rnd()-1;
	for( j = 0; j < iAbcVectorDimNum; j++ )
		pplfVelocityData[c][j] = pplfAbcData[c][j];
	pplfVelocityData[c][h] = pplfAbcData[c][h] + lfRand*( pplfAbcData[c][h] - pplfAbcData[m][h] );
	// �X�V�_�������̂悤�ɍX�V���܂��B
	lfFunc1 = pflfObjectiveFunction( pplfVelocityData[c], iAbcVectorDimNum );
	lfFunc2 = pflfObjectiveFunction( pplfAbcData[c], iAbcVectorDimNum );

	if( lfFunc1 < lfFunc2 )
	{
		for( j = 0;j < iAbcVectorDimNum; j++ )
			pplfAbcData[c][j] = pplfVelocityData[c][j];
		piNonUpdateCount[c] = 0;
	}
	else	piNonUpdateCount[c] = piNonUpdateCount[c] + 1;
}

/**
* <PRE>
*   Onlooker Bee�����s���܂��B(AC-ABC�p�Z�p�����𗘗p����ABC�@�B)
*   �Z�p������p�������ǌ^Artificial Bee Colony �A���S���Y�� ��28��t�@�W�[�V���|�W�E��, 2012.9.12�`14
*   ver 0.1 2016/10/13 ����
*   ver 0.2 2016/10/25 �X�V���_�̎Z�o�Ɍ��𔭌����C���B
* </PRE>
* @author kobayashi
* @since 2016/10/13
* @version 0.2
*/
void CAbc::vOnlookerBeeAC()
{
	int i, j;
	int c1, c2, m, h;
	double lfRes = 0.0;
	double lfRand = 0.0;
	double lfFitProb = 0.0;
	double lfProb = 0.0;
	double lfPrevProb = 0.0;
	double lfFunc1 = 0.0;
	double lfFunc2 = 0.0;
	double lfObjFunc = 0.0;
	double lfRand1 = 0.0;
	double lfRand2 = 0.0;

	// �_���L�ڂ̓��e���B
	double lfLambda = 0.1;
	double lfCrossOverRate = 0.1;

	lfRes = 0.0;
	for (j = 0; j < iAbcSearchNum; j++)
	{
		// �K���x�̎Z�o
		lfObjFunc = pflfObjectiveFunction(pplfAbcData[j], iAbcVectorDimNum);
//		if (lfObjFunc >= 0.0)	lfFitProb = 1.0 / (1.0 + lfObjFunc);
//		else					lfFitProb = 1.0 + fabs(lfObjFunc);
		lfFitProb = lfObjFunc;
		lfRes += lfFitProb;
		plfFit[j] = lfFitProb;
	}
	// �K���x�̐��K��
	for (j = 0; j < iAbcSearchNum; j++)	plfFitProb[j] = plfFit[j] / lfRes;
	// ���[���b�g�헪�����s
	lfProb = lfPrevProb = 0.0;
	lfRand = rnd();
	c1 = c2 = 0;
	for (j = 0; j < iAbcSearchNum; j++)
	{
		lfProb += plfFitProb[j];
		if (lfPrevProb <= lfRand && lfRand <= lfProb)
		{
			c1 = j;
			// �ȒP�ȗ����A���S���Y��
			if (j == 0) c2 = mrand() % iAbcSearchNum + 1;
			else	    c2 = mrand() % j;
		}
		lfPrevProb = lfProb;
	}
	// ���[���b�g�I�������T���_�ɑ΂��čX�V�������߂܂��B

	// �X�V�_�����Z�o���܂��B
	// �X�V�_���𗐐��ɂ�茈�肵�܂��B
	m = mrand() % iAbcSearchNum;
	h = mrand() % iAbcVectorDimNum;

	lfRand = rnd();
	for (j = 0; j < iAbcVectorDimNum; j++)
	{
		if (lfRand < lfCrossOverRate )
		{
			pplfVelocityData[m][j] = lfLambda*pplfAbcData[c1][j] + (1.0 - lfLambda)*pplfAbcData[c2][j];
		}
		else
		{
			pplfVelocityData[m][j] = pplfAbcData[m][j];
		}
	}

	// �X�V�_�������̂悤�ɍX�V���܂��B
	lfFunc1 = pflfObjectiveFunction(pplfVelocityData[m], iAbcVectorDimNum);
	lfFunc2 = pflfObjectiveFunction(pplfAbcData[m], iAbcVectorDimNum);
	if (lfFunc1 < lfFunc2)
	{
		for (j = 0; j < iAbcVectorDimNum; j++)
			pplfAbcData[m][j] = pplfVelocityData[m][j];
		piNonUpdateCount[m] = 0;
	}
	else	piNonUpdateCount[m] = piNonUpdateCount[m] + 1;
}

/**
 * <PRE>
 *   Onlooker Bee�����s���܂��B
 *   Best-so-far Artificial Bee Colony Method�p
 *   The best-so-far seelction in ARtificial Bee Colony algorithm, Applied Soft Computing 11 (2011) 2888-2901.
 *   ver 0.1 2016/10/28 ����
 * </PRE>
 * @author kobayashi
 * @since 2016/10/28
 * @version 0.1
 */
void CAbc::vOnlookerBeeBF()
{
	int i, j;
	int b, c, m, h;
	double lfRes = 0.0;
	double lfRand = 0.0;
	double lfFitProb = 0.0;
	double lfProb = 0.0;
	double lfPrevProb = 0.0;
	double lfFunc1 = 0.0;
	double lfFunc2 = 0.0;
	double lfObjFunc = 0.0;
	double lfLocalBestRand = 0.0;


	lfLocalBestRand = pflfObjectiveFunction(pplfAbcData[j], iAbcVectorDimNum);
	lfRes = 0.0;
	for (j = 0; j < iAbcSearchNum; j++)
	{
		// �K���x�̎Z�o
		lfObjFunc = pflfObjectiveFunction(pplfAbcData[j], iAbcVectorDimNum);
		if (lfObjFunc >= 0.0)	lfFitProb = 1.0 / (1.0 + lfObjFunc);
		else					lfFitProb = 1.0 + fabs(lfObjFunc);
		lfRes += lfFitProb;
		plfFit[j] = lfFitProb;
	}
	// �K���x�̐��K��
	for (j = 0; j < iAbcSearchNum; j++)	plfFitProb[j] = plfFit[j] / lfRes;
	lfLocalBestRand /= lfRes;

	for (i = 0; i < iAbcSearchNum; i++)
	{
		// ���[���b�g�헪�����s
		lfProb = lfPrevProb = 0.0;
		lfRand = rnd();
		c = 0;
		b = 0;
		for (j = 0; j < iAbcSearchNum; j++)
		{
			lfProb += plfFitProb[j];
			if (lfPrevProb <= lfRand && lfRand <= lfProb)	c = j;
			lfPrevProb = lfProb;
		}
		// ���[���b�g�I�������T���_�ɑ΂��čX�V�������߂܂��B

		// �X�V�_�����Z�o���܂��B
		// �X�V�_���𗐐��ɂ�茈�肵�܂��B
		h = mrand() % iAbcVectorDimNum;

		lfRand = 2.0*rnd() - 1.0;
		for (j = 0; j < iAbcVectorDimNum; j++)
//			pplfVelocityData[c][j] = pplfAbcData[c][j];
			pplfVelocityData[i][j] = pplfAbcData[c][h] + lfRand*lfLocalBestRand*(pplfAbcData[c][h] - pplfLocalMinAbcData[c][h]);
//			pplfVelocityData[c][h] = pplfAbcData[c][h] + lfRand*lfGlobalMinAbcData*( pplfAbcData[c][h] - pplfAbcData[m][h] );
//			pplfVelocityData[c][h] = pplfAbcData[c][h] + lfRand*(pplfAbcData[c][h] - plfGlobalMinAbcData[h]);

				// �X�V�_�������̂悤�ɍX�V���܂��B
		lfFunc1 = pflfObjectiveFunction(pplfVelocityData[i], iAbcVectorDimNum);
		lfFunc2 = pflfObjectiveFunction(pplfAbcData[c], iAbcVectorDimNum);

		if (lfFunc1 < lfFunc2)
		{
			for (j = 0; j < iAbcVectorDimNum; j++)
				pplfAbcData[c][j] = pplfVelocityData[i][j];
			piNonUpdateCount[c] = 0;
		}
		else	piNonUpdateCount[c] = piNonUpdateCount[c] + 1;
	}
}
/**
* <PRE>
*   Ivona Brajevic, Crossover-based artificial bee colony algorithm for constrained optimization problems, Neural Computing & Application (2015) 26:1587-1601.
*   ver 0.1 2016/10/19 ����
*   ver 0.2 2016/10/25 �X�V���_�̎Z�o�Ɍ��𔭌����C���B
* </PRE>
* @author kobayashi
* @since 2016/10/19
* @version 0.2
*/
void CAbc::vOnlookerBeeCB(double lfMr)
{
	int i, j;
	int c, m, h;
	double lfRes = 0.0;
	double lfRand = 0.0;
	double lfFitProb = 0.0;
	double lfProb = 0.0;
	double lfPrevProb = 0.0;
	double lfFunc1 = 0.0;
	double lfFunc2 = 0.0;
	double lfObjFunc = 0.0;
	double lfMrMax = 0.9;
	double lfP = 0.3;
	double lfMCN = iGenerationNumber;
	double lfLowerVelocity = -DBL_MAX;
	double lfUpperVelocity = DBL_MAX;
	double lfMaxFit =-DBL_MAX;
	double lfDelta = 0.0;

	lfRes = 0.0;
	for (j = 0; j < iAbcSearchNum; j++)
	{
		lfObjFunc = pflfObjectiveFunction(pplfAbcData[j], iAbcVectorDimNum);
		plfFit[j] = lfObjFunc;
		lfMaxFit = plfFit[j] < lfMaxFit ? lfMaxFit : plfFit[j];
	}
	lfRes = 0.0;
	for (j = 0; j < iAbcSearchNum; j++)
	{
		// �K���x�̎Z�o
		plfFit[i] = 0.9*(plfFit[i] / lfMaxFit) + 0.1;
		lfRes += plfFit[i];
	}
	// �K���x�̐��K��
	for (j = 0; j < iAbcSearchNum; j++)	plfFitProb[j] = plfFit[j] / lfRes;
	// ���[���b�g�헪�����s
	lfProb = lfPrevProb = 0.0;
	lfRand = rnd();
	c = 0;
	for (j = 0; j < iAbcSearchNum; j++)
	{
		lfProb += plfFitProb[j];
		if (lfPrevProb <= lfRand && lfRand <= lfProb)	c = j;
		lfPrevProb = lfProb;
	}
	// ���[���b�g�I�������T���_�ɑ΂��čX�V�������߂܂��B

	// �X�V�_�����Z�o���܂��B
	// �X�V�_���𗐐��ɂ�茈�肵�܂��B
	m = mrand() % iAbcSearchNum;
	h = mrand() % iAbcSearchNum;

	lfMr = lfMr < lfMrMax ? lfMr + (lfMrMax - 0.1) / (lfP*lfMCN) : lfMrMax;
	for (i = 0; i < iAbcSearchNum; i++)
	{
		if (lfDelta < plfFitProb[i])
		{
			lfRand = 2 * rnd() - 1;
			for (j = 0; j < iAbcVectorDimNum; j++)
			{
				pplfVelocityData[i][j] = rnd() < lfMr ? pplfAbcData[i][j] + lfRand*(pplfAbcData[m][j] - pplfAbcData[h][j]) : pplfAbcData[i][j];
				if (pplfVelocityData[i][j] < lfLowerVelocity) pplfVelocityData[i][j] = 2.0*lfLowerVelocity - pplfVelocityData[i][j];
				else if (pplfVelocityData[i][j] > lfUpperVelocity) pplfVelocityData[i][j] = 2.0*lfUpperVelocity - pplfVelocityData[i][j];
			}
		}
	}
	// �e�T���_�ƍX�V���Ȃ������񐔂��i�[����ϐ����X�V���܂��B
	for (i = 0; i < iAbcSearchNum; i++)
	{
		lfFunc1 = pflfObjectiveFunction(pplfVelocityData[i], iAbcVectorDimNum);
		lfFunc2 = pflfObjectiveFunction(pplfAbcData[i], iAbcVectorDimNum);

		if (lfFunc1 < lfFunc2)
		{
			for (j = 0; j < iAbcVectorDimNum; j++)
				pplfAbcData[i][j] = pplfVelocityData[i][j];
			piNonUpdateCount[i] = 0;
		}
		else	piNonUpdateCount[i] = piNonUpdateCount[i] + 1;
	}
}

/**
 * <PRE>
 * Scout Bee�����s���܂��B(��{�̎�@)
 * ver 0.1
 * </PRE>
 * @author kobayashi
 * @since 2016/8/10
 * @version 0.2
 */
void CAbc::vScoutBeeOrigin()
{
	int i,j,k;
	double lfRand = 0.0;
	// �V���ȒT���_�����߂ĒT�������s���܂��B
	for( i = 0;i < iAbcSearchNum; i++ )
	{
		if( piNonUpdateCount[i] > iAbcLimitCount )
		{
			for( k = 0;k < iAbcVectorDimNum; k++ )
			{
				lfRand = rnd();
				pplfAbcData[i][k] = lfSolveRange*(2.0*lfRand-1.0);
			}
		}
	}
}

/**
 * <PRE>
 * Scout Bee�����s���܂��B
 * ver 0.1 2016.8.10 ����
 * ver 0.2 2016.8.11 ��@�̕ύX�B�i���q�Q�œK���@�̂悤�ȍX�V��@�B�_�����j
 * </PRE>
 * @author kobayashi
 * @since 2016/8/10
 * @version 0.2
 */
void CAbc::vScoutBeeNormal()
{
	int i,j,k;
	double lfRand = 0.0;
	// �V���ȒT���_�����߂ĒT�������s���܂��B
	for( i = 0;i < iAbcSearchNum; i++ )
	{
		if( piNonUpdateCount[i] > iAbcLimitCount )
		{
			for( k = 0;k < iAbcVectorDimNum; k++ )
			{
				lfRand = rnd();
				pplfAbcData[i][k] = plfGlobalMinAbcData[k] + lfRand*(plfGlobalMaxAbcData[k]-plfGlobalMinAbcData[k]);
			}
		}
	}
}

/**
 * <PRE>
 * Scout Bee�����s���܂��B�i���S�A�����W��UNDX�����s����B�j
 * ver 0.1 2016.8.18 ����
 * </PRE>
 * @author kobayashi
 * @since 2016/8/18
 * @version 0.1
 */
void CAbc::vScoutBeeUndx()
{
	int i,j,k;
	double lfRand = 0.0;
	// �V���ȒT���_�����߂ĒT�������s���܂��B
	for( i = 0;i < iAbcSearchNum; i++ )
	{
		if( piNonUpdateCount[i] > iAbcLimitCount )
		{
			// ���������s���܂��B������UNDX�����s���܂��B�i�P�񕪂̂݁B�j
			pcUndx->vSetGenData(pplfAbcData );
			pcUndx->vImplement();
//			pcUndx->vGetBestGenData(pplfAbcData[i] );
			pcUndx->vGetGenData(pplfAbcData );
			for (j = 0; j < iAbcSearchNum; j++)
				piNonUpdateCount[j] = 0;
			break;
		}
	}
}

/**
 * <PRE>
 * Scout Bee�����s���܂��B�i���S�A�����W��REX�����s����B�j
 * ver 0.1 2016.8.26 ����
 * </PRE>
 * @author kobayashi
 * @since 2016/8/26
 * @version 0.1
 */
void CAbc::vScoutBeeRex()
{
	int i,j,k;
	double lfRand = 0.0;
	// �V���ȒT���_�����߂ĒT�������s���܂��B
	for( i = 0;i < iAbcSearchNum; i++ )
	{
		if( piNonUpdateCount[i] > iAbcLimitCount )
		{
			// ���������s���܂��B������REX�����s���܂��B�i�P�񕪂̂݁B�j
			pcRex->vSetGenData(pplfAbcData );
			pcRex->vRex();
//			pcRex->vGetBestGenData(pplfAbcData[i] );
//			pcRex->vGetBest2ndGenData(pplfAbcData );
			pcRex->vGetGenData(pplfAbcData );
			for (j = 0; j < iAbcSearchNum; j++)
				piNonUpdateCount[j] = 0;
			break;
		}
	}
}

/**
 * <PRE>
 * Scout Bee�����s���܂��B�i���S�A�����W��AREX�����s����B�j
 * ver 0.1 2016.8.27 ����
 * </PRE>
 * @author kobayashi
 * @since 2016/8/27
 * @version 0.1
 */
void CAbc::vScoutBeeARex()
{
	int i,j,k;
	double lfRand = 0.0;
	// �V���ȒT���_�����߂ĒT�������s���܂��B
	for( i = 0;i < iAbcSearchNum; i++ )
	{
		if( piNonUpdateCount[i] > iAbcLimitCount )
		{
			// ���������s���܂��B������AREX�����s���܂��B�i�P�񕪂̂݁B�j
			pcRex->vSetGenData(pplfAbcData );
			pcRex->vARex();
//			pcRex->vGetBestGenData( pplfAbcData[i] );
//			pcRex->vGetBest2ndGenData(pplfAbcData);
			pcRex->vGetGenData(pplfAbcData );
			for (j = 0; j < iAbcSearchNum; j++)
				piNonUpdateCount[j] = 0;
			break;
		}
	}
}

/**
* <PRE>
*   Ivona Brajevic, Crossover-based artificial bee colony algorithm for constrained optimization problems, Neural Computing & Application (2015) 26:1587-1601.
*   ver 0.1 2016.10.19 ���� 
* </PRE>
* @author kobayashi
* @since 2016/10/19
* @version 0.1
*/
void CAbc::vScoutBeeCB( int iCount )
{
	int i, j, k;
	double lfRand = 0.0;
	// �V���ȒT���_�����߂ĒT�������s���܂��B
	if (iCount % iAbcLimitCount == 0)
	{
		for (i = 0; i < iAbcSearchNum; i++)
		{
			for (k = 0; k < iAbcVectorDimNum; k++)
			{
				pplfVelocityData[i][j] = rnd() < 0.5 ? plfGlobalMaxAbcData[j] : pplfAbcData[i][j];
			}
		}
	}
}

/**
 * <PRE>
 * Scout Bee�����s���܂��B(Best-so-far Artificial Bee Colony Method�p)
 * The best-so-far selection in Artificial Bee colony algorithm, Applied Soft Computing 11 (2011) 2888-2901.
 * ver 0.1 ���� 2016.10.28
 * </PRE>
 * @author kobayashi
 * @since 2016/10/28
 * @version 0.1
 */
void CAbc::vScoutBeeBF( int iUpdateCount )
{
	int i,j,k;
	double lfRand = 0.0;
	double lfMaxWeight = 1.0;
	double lfMinWeight = 0.2;
	double lfFunc1 = 0.0;
	double lfFunc2 = 0.0;

	// �V���ȒT���_�����߂ĒT�������s���܂��B
	for( i = 0;i < iAbcSearchNum; i++ )
	{
		if( piNonUpdateCount[i] > iAbcLimitCount )
		{
			for( k = 0;k < iAbcVectorDimNum; k++ )
			{
				lfRand = 2.0*rnd()-1.0;
				plfScoutBeeResult[k] = pplfAbcData[i][k] + lfRand*(lfMaxWeight-(double)iUpdateCount/(double)iGenerationNumber*(lfMaxWeight-lfMinWeight))*pplfAbcData[i][k];
			}
			lfFunc1 = pflfObjectiveFunction(plfScoutBeeResult, iAbcVectorDimNum);
			lfFunc2 = pflfObjectiveFunction(pplfAbcData[i], iAbcVectorDimNum);
	
			if (lfFunc1 < lfFunc2)
			{
				for (k = 0; k < iAbcVectorDimNum; k++)
					pplfAbcData[i][k] = plfScoutBeeResult[k];
				piNonUpdateCount[i] = 0;
			}
			else	piNonUpdateCount[i] = piNonUpdateCount[i] + 1;
		}
	}
}


/**
 * <PRE>
 * �����_�ł̖ړI�֐��̍ő�A�ŏ������߂܂��B
 * </PRE>
 * @author kobayashi
 * @since 2016/8/10
 * @version 0.1
 */
void CAbc::vGetLocalMaxMin()
{
	int i,j;
	double lfFunc1 = 0.0;
	double lfFunc2 = 0.0;
	double lfMin = 0.0;

	lfMin = pflfObjectiveFunction( plfGlobalMaxAbcData, iAbcVectorDimNum );
	// ���[�J���̍ő�l���X�V���܂��B
	for( i = 0;i < iAbcSearchNum; i++ )
	{
		lfFunc1 = pflfObjectiveFunction( pplfLocalMaxAbcData[i], iAbcVectorDimNum );
		lfFunc2 = pflfObjectiveFunction( pplfAbcData[i], iAbcVectorDimNum );

		if( lfFunc1 > lfFunc2 )
		{
			for( j = 0;j < iAbcVectorDimNum; j++ )
				pplfLocalMinAbcData[i][j] = pplfAbcData[i][j];
		}
		else
		{
			for( j = 0;j < iAbcVectorDimNum; j++ )
				pplfLocalMaxAbcData[i][j] = pplfAbcData[i][j];
		}
	}
}


/**
 * <PRE>
 * �����_�ł̖ړI�֐��̑S�̂�ʂ��Ă̍ő�A�ŏ��l�����߂܂��B
 * </PRE>
 * @author kobayashi
 * @since 2016/8/10
 * @version 0.1
 */
void CAbc::vGetGlobalMaxMin()
{
	int i,j;
	int iMinLoc = 0;
	double lfObjFunc = 0.0;

	for( i = 0;i < iAbcSearchNum; i++ )
	{
		lfObjFunc = pflfObjectiveFunction( pplfAbcData[i], iAbcVectorDimNum );
		if( lfGlobalMinAbcData >= lfObjFunc )
		{
			iMinLoc = i;
			lfGlobalMinAbcData = lfObjFunc;
			for( j = 0; j < iAbcVectorDimNum; j++ )
				plfGlobalMinAbcData[j] = pplfAbcData[i][j];
			lfFitCurrentBest = lfGlobalMinAbcData;
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
 * �@�����_�ł̍ŏ��l�̗��q�̖ړI�֐��l���o�͂��܂��B
 * </PRE>
 * @author kobayashi
 * @since 2016/9/14
 * @version 1.0
 */
double CAbc::lfGetGlobalMinAbcDataConstFuncValue()
{
	// �����_�ł̊e���q�̖ړI�֐��̒l���o�͂��܂��B
	return lfGlobalMinAbcData;
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

/**
* <PRE>
* �@�����_�ł̍ŏ��l�̗��q�̖ړI�֐��l���o�͂��܂��B
* </PRE>
* @author kobayashi
* @since 2015/6/19
* @version 1.0
*/
void CAbc::vOutputGlobalMinAbcDataConstFuncValue( int iOutputCount )
{
	// �����_�ł̊e���q�̖ړI�֐��̒l��10�񂨂��ɏo�͂��܂��B
	if (iOutputCount % 10 == 0)
	{
		printf("%lf,", lfGlobalMinAbcData);
	}
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
		if( -1.0 <= lfRand && lfRand <= 1.0 ) break;
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
