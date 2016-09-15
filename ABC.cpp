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

	plfXnew1 = NULL;
	plfXnew2 = NULL;

	pcUndx = NULL;
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
		plfCrossOverData = new double[iAbcVectorDimNum];
		plfXnew1 = new double[iAbcVectorDimNum];
		plfXnew2 = new double[iAbcVectorDimNum];

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
			plfCrossOverData[i] = 0.0;
			plfXnew1[i] = 0.0;
			plfXnew2[i] = 0.0;
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
		plfCrossOverData = new double[iAbcVectorDimNum];
		plfXnew1 = new double[iAbcVectorDimNum];
		plfXnew2 = new double[iAbcVectorDimNum];

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
			plfCrossOverData[i] = 0.0;
			plfXnew1[i] = 0.0;
			plfXnew2[i] = 0.0;
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
		plfCrossOverData = new double[iAbcVectorDimNum];
		plfXnew1 = new double[iAbcVectorDimNum];
		plfXnew2 = new double[iAbcVectorDimNum];

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
			plfCrossOverData[i] = 0.0;
			plfXnew1[i] = 0.0;
			plfXnew2[i] = 0.0;
		}
		pcUndx = new CUndx();
		iCrossOverNum = iCrossOverNumData;
		lfAlpha = lfAlphaData;
		lfBeta = lfBetaData;
		// UNDX�̏����������s���܂��B
		pcUndx->vInitialize( iGenerationNumber, iAbcDataNum, iAbcVectorDimNum, iCrossOverNum );
		pcUndx->vSetAlpha( lfAlpha );
		pcUndx->vSetBeta( lfBeta );
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
		plfCrossOverData = new double[iAbcVectorDimNum];
		plfXnew1 = new double[iAbcVectorDimNum];
		plfXnew2 = new double[iAbcVectorDimNum];

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
			plfCrossOverData[i] = 0.0;
			plfXnew1[i] = 0.0;
			plfXnew2[i] = 0.0;
		}
		pcUndx = new CUndx();
		iCrossOverNum = iCrossOverNumData;
		lfAlpha = lfAlphaData;
		lfBeta = lfBetaData;
		// UNDX�̏����������s���܂��B
		pcUndx->vInitialize( iGenerationNumber, iAbcDataNum, iAbcVectorDimNum, iCrossOverNum );
		pcUndx->vSetAlpha( lfAlpha );
		pcUndx->vSetBeta( lfBeta );
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
		plfCrossOverData = new double[iAbcVectorDimNum];
		plfXnew1 = new double[iAbcVectorDimNum];
		plfXnew2 = new double[iAbcVectorDimNum];

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
			plfCrossOverData[i] = 0.0;
			plfXnew1[i] = 0.0;
			plfXnew2[i] = 0.0;
		}
		pcRex = new CRex();
		// UNDX�̏����������s���܂��B
		pcRex->vInitialize( iGenerationNumber, iAbcDataNum, iAbcVectorDimNum, iParentNumber, iChildrenNumber, lfLearningRate, iUpperEvalChildrenNumber );
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
		// UNDX������L���ɂ��Ă���ꍇ�ɂ̂ݎ��s���܂��B
		if( pcUndx != NULL )
		{
			pcUndx->vTerminate();
			delete pcUndx;
			pcUndx = NULL;
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
 * </PRE>
 * @author kobayashi
 * @since 2015/7/28
 * @version 0.1
 */
void CAbc::vModified2Abc()
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
 * </PRE>
 * @author kobayashi
 * @since 2015/7/28
 * @version 0.1
 */
void CAbc::vModified3Abc( int iUpdateCount )
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
 * </PRE>
 * @author kobayashi
 * @since 2015/7/28
 * @version 0.1
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
	double lfPr = 0.3;
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
	while( fabs(lfA-lfB) < 0.00000001 )
	{
		lfF1 = rnd()*( lfB-(lfB-lfA)*lfFai );
		lfF2 = (1.0-rnd())*( lfA+(lfB-lfA)*lfFai );

		j = mrand() % ( iAbcSearchNum - 1 );
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
	vEmployBeeOrigin();

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
			pplfAbcData[iParentLoc1][i] = plfCrossOverData[i];
	}
	else
	{
		for( i = 0;i < iAbcVectorDimNum; i++ )
			pplfAbcData[iParentLoc2][i] = plfCrossOverData[i];
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
 * �@Employ Bee�����s���܂��B(��{�̃o�[�W�����Ɠ�����@)
 *   ver 0.1 
 * </PRE>
 * @author kobayashi
 * @since 2016/8/18
 * @version 0.1
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
		else	piNonUpdateCount[i] = piNonUpdateCount[i] + 1;
	}
}

/**
 * <PRE>
 * �@Employ Bee�����s���܂��B(�����x���o�[�W����)
 *   2011�̓d�q���ʐM�w��̘_�����
 *   ver 0.1 
 * </PRE>
 * @author kobayashi
 * @since 2016/8/18
 * @version 0.1
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
	return lfFitJudge;
}

/**
 * <PRE>
 * �@Employ Bee�����s���܂��B(�����x���o�[�W����)
 *   2011�̓d�q���ʐM�w��̘_�����
 *   ver 0.1 
 * </PRE>
 * @author kobayashi
 * @since 2016/8/18
 * @version 0.1
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
					m = mrand() % ( iAbcSearchNum - 1 - iAbcUpperSearchNum ) + iAbcUpperSearchNum;
				}
				// �����_���Ɍ��肵�܂��B
				h = mrand() % ( iAbcVectorDimNum - 1);
	
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
				m = mrand() % ( iAbcSearchNum - 1 - iAbcUpperSearchNum ) + iAbcUpperSearchNum;
			}
			// �����_���Ɍ��肵�܂��B
			h = mrand() % ( iAbcVectorDimNum - 1);
	
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
			m = mrand() % ( iAbcSearchNum - 1 - iAbcUpperSearchNum ) + iAbcUpperSearchNum;
		}
		// �����_���Ɍ��肵�܂��B
		h = mrand() % ( iAbcVectorDimNum - 1);

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
 * </PRE>
 * @author kobayashi
 * @since 2016/8/10
 * @version 0.2
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
	m = mrand() % ( iAbcSearchNum - 1 );
	h = mrand() % ( iAbcVectorDimNum - 1);
	
	for( i = 0;i < iAbcSearchNum; i++ )
	{
		lfRand = 2*rnd()-1;
		for( j = 0; j < iAbcVectorDimNum; j++ )
			pplfVelocityData[i][j] = pplfAbcData[i][j];
		pplfVelocityData[i][h] = pplfAbcData[i][h] + lfRand*( pplfAbcData[i][h] - pplfLocalMinAbcData[m][h] );
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
		else	piNonUpdateCount[i] = piNonUpdateCount[i] + 1;
	}
}

/**
 * <PRE>
 * �@Employ Bee�����s���܂��B(GBest��)
 *   ver 0.1 
 *   ver 0.2 NBest�łɏC��
 * </PRE>
 * @author kobayashi
 * @since 2016/8/10
 * @version 0.2
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
	m = mrand() % ( iAbcSearchNum - 1 );
	h = mrand() % ( iAbcVectorDimNum - 1);
	
	for( i = 0;i < iAbcSearchNum; i++ )
	{
		lfRand = rnd();
		lfRand2 = rnd();
		for( j = 0; j < iAbcVectorDimNum; j++ )
			pplfVelocityData[i][j] = pplfAbcData[i][j];
		pplfVelocityData[i][h] = pplfAbcData[i][h] + lfRand*( pplfAbcData[i][h] - pplfAbcData[m][h] ) + lfRand2*( pplfAbcData[i][h] - pplfLocalMinAbcData[m][h] );
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
}

/**
 * <PRE>
 * �@Employ Bee�����s���܂��B
 *   ver 0.1 
 *   ver 0.2 2016/08/18 IWCFA(���q�Q�œK���@�̈��@��K�p)
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
	m = mrand() % ( iAbcSearchNum - 1 );
	h = mrand() % ( iAbcVectorDimNum - 1);
	
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
		else
		{
			piNonUpdateCount[i] = piNonUpdateCount[i] + 1;
			piTotalNonUpdateCount[i] = piTotalNonUpdateCount[i] + 1;
		}
	}
}

/**
 * <PRE>
 * Onlooker Bee�����s���܂��B(��{�̃o�[�W����)
 * </PRE>
 * @author kobayashi
 * @since 2016/8/10
 * @version 0.1
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

	for( i = 0;i < iAbcDataNum-iAbcSearchNum;  i++ )
	{
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
		m = mrand() % ( iAbcSearchNum-1 );
		h = mrand() % ( iAbcVectorDimNum - 1);

		lfRand = 2*rnd()-1;
		for( j = 0; j < iAbcVectorDimNum; j++ )
		{
			lfRand = 2*rnd()-1;
			for( j = 0; j < iAbcVectorDimNum; j++ )
				pplfVelocityData[c][j] = pplfAbcData[c][j];
			pplfVelocityData[c][h] = pplfAbcData[c][h] + lfRand*( pplfAbcData[c][h] - pplfAbcData[m][h] );
		}
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
}

/**
 * <PRE>
 * �@Onlooker Bee�����s���܂��B(�����x���o�[�W����)
 *   2011�̓d�q���ʐM�w��̘_�����
 *   ver 0.1 
 * </PRE>
 * @author kobayashi
 * @since 2016/8/18
 * @version 0.1
 */
void CAbc::vOnlookerBeeEnhanced( int iUpdateCount, double lfFitJudge )
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
}

/**
 * <PRE>
 * �@Onlooker Bee�����s���܂��B(�����x���o�[�W����)
 *   2011�̓d�q���ʐM�w��̘_�����
 *   ver 0.1 
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
		pplfVelocityData[i][h] = pplfAbcData[i][h] + lfRand*( pplfAbcData[i][h] - plfGlobalMinAbcData[h] );
	}
	// �e�T���_���X�V���܂��B
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
 * Onlooker Bee�����s���܂��B(NBest��)
 * ver 0.1
 * ver 0.2 NBest�ł�ǉ�
 * </PRE>
 * @author kobayashi
 * @since 2016/8/10
 * @version 0.2
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

	for( i = 0;i < iAbcDataNum-iAbcSearchNum;  i++ )
	{
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
		m = mrand() % ( iAbcSearchNum-1 );
		h = mrand() % ( iAbcVectorDimNum - 1);

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
}

/**
 * <PRE>
 * Onlooker Bee�����s���܂��B(GBest��)
 * ver 0.1
 * ver 0.2 NBest�ł�ǉ�
 * </PRE>
 * @author kobayashi
 * @since 2016/8/10
 * @version 0.2
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
		m = mrand() % ( iAbcSearchNum-1 );
		h = mrand() % ( iAbcVectorDimNum - 1);

		lfRand = rnd();
		lfRand2 = rnd();
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
		else 	
		{
			piNonUpdateCount[c] = piNonUpdateCount[c] + 1;
//			piTotalNonUpdateCount[c] = piTotalNonUpdateCount[c] + 1;
		}
	}
}

/**
 * <PRE>
 * Onlooker Bee�����s���܂��B(IWCFA��)
 * ver 0.1
 * ver 0.2 IWCFA�ł�ǉ�
 * </PRE>
 * @param lfK
 * @param lfCoe1
 * @param lfCoe2
 * @param iUpdateCount
 * @author kobayashi
 * @since 2016/8/10
 * @version 0.2
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
		m = mrand() % ( iAbcSearchNum-1 );
		h = mrand() % ( iAbcVectorDimNum - 1);

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
		else
		{
			for( k = 0;k < iAbcVectorDimNum; k++ )
			{
				pplfAbcData[i][k] = pplfAbcData[i][k];
			}
		}
	}
}

/**
 * <PRE>
 * Scout Bee�����s���܂��B
 * ver 0.1
 * ver 0.2 ��@�̕ύX�B�i���q�Q�œK���@�̂悤�ȍX�V��@�B�_�����j
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
		else
		{
			for( k = 0;k < iAbcVectorDimNum; k++ )
			{
				pplfAbcData[i][k] = pplfAbcData[i][k];
			}
		}
	}
}

/**
 * <PRE>
 * Scout Bee�����s���܂��B�i���S�A�����W��UNDX�����s����B�j
 * ver 0.1
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
		}
	}
}

/**
 * <PRE>
 * Scout Bee�����s���܂��B�i���S�A�����W��REX�����s����B�j
 * ver 0.1
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
			pcRex->vGetGenData(pplfAbcData );
		}
	}
}

/**
 * <PRE>
 * Scout Bee�����s���܂��B�i���S�A�����W��AREX�����s����B�j
 * ver 0.1
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
//			pcRex->vGetBestGenData(pplfAbcData[i] );
			pcRex->vGetGenData(pplfAbcData );
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
