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
 * 　コンストラクタ
 * </PRE>
 * @author kobayashi
 * @since 2015/6/6
 * @version 1.0
 */
CAbc::CAbc()
{
	iGenerationNumber = 0;				// 計算回数
	iAbcDataNum = 0;				// 粒子群の数
	iAbcVectorDimNum = 0;				// 各粒子群の特徴ベクトル
	iAbcSearchNum = 0;				// 探索点の総数
	pplfAbcData = NULL;				// ABCデータ配列
	pplfNAbcData = NULL;				// ABC更新用データ配列
	pplfVelocityData = NULL;			// ABCデータ速度配列
	pplfNVelocityData = NULL;			// ABC更新用データ速度配列
	piNonUpdateCount = NULL;			// 更新されなかった回数
	piTotalNonUpdateCount = NULL;			// 更新されなかった総回数
	plfVelocity = NULL;				// ルーレット選択により選択する速度ベクトル
	plfGlobalMaxAbcData = NULL;
	plfGlobalMinAbcData = NULL;			// 大域的最適解を表す粒子のデータ
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
 * 　人口蜂コロニーの初期化を実行します。(コンストラクタ)
 * </PRE>
 * @param iGenCount       計算回数
 * @param iGenNum         コロニー数
 * @param iGenVectorDim   探索点の次元数
 * @param iSearchNum  　  探索点の数
 * @param iLimitCountData 更新しなかった回数
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
 * 　人口蜂コロニーの初期化を実行します。(コンストラクタ)
 * </PRE>
 * @param iGenCount       計算回数
 * @param iGenNum         コロニー数
 * @param iGenVectorDim   探索点の次元数
 * @param iSearchNum  　  探索点の数
 * @param iLimitCountData 更新しなかった回数
 * @param iIntervalMinNum 最低反復回数
 * @param lfDrData        収束状況判定
 * @param lfBoundData     適合度許容限界値
 * @param lfAccuracyData  適合度評価精度
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
 * 　デストラクタ
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
 * 　人口蜂コロニーの初期化を実行します。
 * </PRE>
 * @param iGenCount       計算回数
 * @param iGenNum         コロニー数
 * @param iGenVectorDim   探索点の次元数
 * @param iSearchNum  　  探索点の数
 * @param iLimitCountData 更新しなかった回数
 * @param iIntervalMinNum 最低反復回数
 * @param iAlphaData      探索点上位数
 * @param lfDrData        収束状況判定
 * @param lfBoundData     適合度許容限界値
 * @param lfAccuracyData  適合度評価精度
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
		// ソート用適応度を格納するベクターです。
		stlFitProb.assign( iAbcSearchNum, Rank_t() );
	}
	catch( std::bad_alloc ba )
	{
		cae.SetErrorInfo( ABC_MEMORY_ALLOCATE_ERROR, "vInitialize", "CAbc", "メモリ確保に失敗しました。", __LINE__ );
		throw( cae );
	}
	catch(...)
	{
		cae.SetErrorInfo( ABC_FATAL_ERROR, "vInitialize", "CAbc", "致命的エラーが発生しました。", __LINE__ );
		throw( cae );
	}
}

/**
 * <PRE>
 * 　人口蜂コロニーの初期化を実行します。
 * </PRE>
 * @param iGenCount       計算回数
 * @param iGenNum         コロニー数
 * @param iGenVectorDim   探索点の次元数
 * @param iSearchNum  　  探索点の数
 * @param iLimitCountData 更新しなかった回数
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
		// ソート用適応度を格納するベクターです。
		stlFitProb.assign( iAbcSearchNum, Rank_t() );
	}
	catch( std::bad_alloc ba )
	{
		cae.SetErrorInfo( ABC_MEMORY_ALLOCATE_ERROR, "vInitialize", "CAbc", "メモリ確保に失敗しました。", __LINE__ );
		throw( cae );
	}
	catch(...)
	{
		cae.SetErrorInfo( ABC_FATAL_ERROR, "vInitialize", "CAbc", "致命的エラーが発生しました。", __LINE__ );
		throw( cae );
	}
}

/**
 * <PRE>
 * 　人口蜂コロニーの初期化を実行します。
 * </PRE>
 * @param iGenCount      	 計算回数
 * @param iGenNum        	 コロニー数
 * @param iGenVectorDim  	 探索点の次元数
 * @param iSearchNum  　 	 探索点の数
 * @param iLimitCountData	 更新しなかった回数
 * @param iCrossOverNumData	 交叉回数
 * @param lfAlpha		 UNDXにおけるAlpha
 * @param lfBeta		 UNDXにおけるBeta
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
		// UNDXの初期化を実行します。
		pcUndx->vInitialize( iGenerationNumber, iAbcDataNum, iAbcVectorDimNum, iCrossOverNum );
		pcUndx->vSetAlpha( lfAlpha );
		pcUndx->vSetBeta( lfBeta );
		// ソート用適応度を格納するベクターです。
		stlFitProb.assign( iAbcSearchNum, Rank_t() );
	}
	catch( std::bad_alloc ba )
	{
		cae.SetErrorInfo( ABC_MEMORY_ALLOCATE_ERROR, "vInitialize", "CAbc", "メモリ確保に失敗しました。", __LINE__ );
		throw( cae );
	}
	catch(...)
	{
		cae.SetErrorInfo( ABC_FATAL_ERROR, "vInitialize", "CAbc", "致命的エラーが発生しました。", __LINE__ );
		throw( cae );
	}
}

/**
 * <PRE>
 * 　人口蜂コロニーの初期化を実行します。
 * </PRE>
 * @param iGenCount       計算回数
 * @param iGenNum         コロニー数
 * @param iGenVectorDim   探索点の次元数
 * @param iSearchNum  　  探索点の数
 * @param iLimitCountData 更新しなかった回数
 * @param iIntervalMinNum 最低反復回数
 * @param iAlphaData      探索点上位数
 * @param lfDrData        収束状況判定
 * @param lfBoundData     適合度許容限界値
 * @param lfAccuracyData  適合度評価精度
 * @param iCrossOverNumData	 交叉回数
 * @param lfAlpha		 UNDXにおけるAlpha
 * @param lfBeta		 UNDXにおけるBeta
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
		// UNDXの初期化を実行します。
		pcUndx->vInitialize( iGenerationNumber, iAbcDataNum, iAbcVectorDimNum, iCrossOverNum );
		pcUndx->vSetAlpha( lfAlpha );
		pcUndx->vSetBeta( lfBeta );
		// ソート用適応度を格納するベクターです。
		stlFitProb.assign( iAbcSearchNum, Rank_t() );
	}
	catch( std::bad_alloc ba )
	{
		cae.SetErrorInfo( ABC_MEMORY_ALLOCATE_ERROR, "vInitialize", "CAbc", "メモリ確保に失敗しました。", __LINE__ );
		throw( cae );
	}
	catch(...)
	{
		cae.SetErrorInfo( ABC_FATAL_ERROR, "vInitialize", "CAbc", "致命的エラーが発生しました。", __LINE__ );
		throw( cae );
	}
}

/**
 * <PRE>
 * 　人口蜂コロニーの初期化を実行します。
 * </PRE>
 * @param iGenCount       計算回数
 * @param iGenNum         コロニー数
 * @param iGenVectorDim   探索点の次元数
 * @param iSearchNum  　  探索点の数
 * @param iLimitCountData 更新しなかった回数
 * @param iIntervalMinNum 最低反復回数
 * @param iAlphaData      探索点上位数
 * @param lfDrData        収束状況判定
 * @param lfBoundData     適合度許容限界値
 * @param lfAccuracyData  適合度評価精度
 * @param iParentNumberData			 交叉回数
 * @param iChildrenNumber			 REXにおけるAlpha
 * @param iUpperEvalChildrenNumber	 UNDXにおけるBeta
 * @param lfLearningRate	 UNDXにおけるBeta
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
	iParentNumber			= iParentNum;			// Rex用選択する親の数
	iChildrenNumber			= iChildrenNum;			// Rex用生成する子供の数
	iUpperEvalChildrenNumber	= iUpperEvalChildrenNum;	// ARex用生成した子供の上位を選択する数
	lfLearningRate			= lfLearningRateData;		// ARex用学習率

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
		// UNDXの初期化を実行します。
		pcRex->vInitialize( iGenerationNumber, iAbcDataNum, iAbcVectorDimNum, iParentNumber, iChildrenNumber, lfLearningRate, iUpperEvalChildrenNumber );
		// ソート用適応度を格納するベクターです。
		stlFitProb.assign( iAbcSearchNum, Rank_t() );
	}
	catch( std::bad_alloc ba )
	{
		cae.SetErrorInfo( ABC_MEMORY_ALLOCATE_ERROR, "vInitialize", "CAbc", "メモリ確保に失敗しました。", __LINE__ );
		throw( cae );
	}
	catch(...)
	{
		cae.SetErrorInfo( ABC_FATAL_ERROR, "vInitialize", "CAbc", "致命的エラーが発生しました。", __LINE__ );
		throw( cae );
	}
}

/**
 * <PRE>
 * 　人工蜂コロニーの初期位置を中心0の半径range内で一様乱数により設定します。
 * </PRE>
 * @param lfRange 粒子の初期位置の出現範囲
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

	// 初期の状態で最適値を取得します。
	// 更新します。
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
 * 　人工蜂コロニーの初期位置を中心0の半径range内で一様乱数により設定します。
 * </PRE>
 * @param lfRange 粒子の初期位置の出現範囲
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

	// 初期の状態で最適値を取得します。
	// 更新します。
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
	// 初期状態における前探索点の平均評価値を算出します。
	for( i = 0;i < iAbcSearchNum; i++ )
	{
		lfObjFunc += pflfObjectiveFunction( pplfAbcData[i], iAbcVectorDimNum );
	}
	lfFitInit = lfObjFunc/(double)iAbcSearchNum;
}

/**
 * <PRE>
 * 　終了処理を実行します。
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
		// UNDX交叉を有効にしている場合にのみ実行します。
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
		cae.SetErrorInfo( ABC_FATAL_ERROR, "vTerminate", "CAbc", "メモリ解放に失敗しました。", __LINE__ );
		throw( cae );
	}
}

/**
 * <PRE>
 * 　目的関数をインストールします。
 * 　実際にはコールバック関数をインストールします。
 * </PRE>
 * @param pflfFunction 目的関数の関数ポインタ
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
 * 　目的関数をアンインストールします。
 * 　実際にはコールバック関数をアンインストールします。
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
 * 　人工蜂コロニー最適化法を実行します。
 *   ver 0.1
 *   ver 0.2 機能わけを実施。
 * </PRE>
 * @author kobayashi
 * @since 2015/7/28
 * @version 0.2
 */
void CAbc::vAbc()
{
	// employee bee の動作
	vEmployBeeOrigin();

	// onlookers beeの動作
	vOnlookerBeeOrigin();

	// scout bee の実行
	vScoutBeeOrigin();

	// 更新します。
	vGetGlobalMaxMin();
}

/**
 * <PRE>
 * 　人工蜂コロニー最適化法を実行します。
 *	 GBestを利用したABC法
 *   Gbest-guided artificial bee colony algorithm for numerical function optimization.
 *   Applied Mathematics and Computation, 217(7):3166-3173,2010.
 * </PRE>
 * @author kobayashi
 * @since 2015/7/28
 * @version 0.1
 */
void CAbc::vGAbc()
{
	// employee bee の動作
	vEmployBeeGBest();

	// onlookers beeの動作
	vOnlookerBeeGBest();

	// scout bee の実行
	vScoutBeeNormal();
	
	// 局所最大値、最小値を取得します。
	vGetLocalMaxMin();

	// 大域的最大値、最小値を取得します。
	vGetGlobalMaxMin();
}

/**
 * <PRE>
 * 　人工蜂コロニー最適化法を実行します。
 * </PRE>
 * @author kobayashi
 * @since 2015/7/28
 * @version 0.1
 */
void CAbc::vModifiedAbc( int iUpdateCount )
{
	double lfFitJudge = 0.0;
	// employed beesによる探索を実施します。
	lfFitJudge = lfEmployBeeEnhanced( iUpdateCount );

	// onlooker beesによる探索
	vOnlookerBeeEnhanced( iUpdateCount, lfFitJudge );
	
	// 局所最大値、最小値を取得します。
	vGetLocalMaxMin();

	// 大域的最大値、最小値を取得します。
	vGetGlobalMaxMin();
}

/**
 * <PRE>
 * 　人工蜂コロニー最適化法を実行します。
 *   粒子群最適化法のIWCFAを適用
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
	// employee bee の動作
	vEmployBeeIWCFA( lfK, lfCoe1, lfCoe2, iUpdateCount );

	// onlookers beeの動作
	vOnlookerBeeIWCFA( lfK, lfCoe1, lfCoe2, iUpdateCount );

	// scout bee の実行
	vScoutBeeNormal();

	// 局所最大値、最小値を取得します。
	vGetLocalMaxMin();

	// 大域的最大値、最小値を取得します。
	vGetGlobalMaxMin();
}

/**
 * <PRE>
 * 　2013 Memetic search in artificial bee colony algorthimより
 *   ver 0.1 2016/07/28 初版
 *   ver 0.2 2016/10/25 更新候補点の算出に誤りを発見し修正。
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

	// employee bee の動作
	vEmployBeeOrigin();

	// onlookers beeの動作
	vOnlookerBeeGBest();

	// scout bee の実行
	vScoutBeeNormal();
//	vScoutBeeUndx();

	// Memetic artificial bee colony Algorithm(Prに関しては0～1の間の適当な値を設定。)
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
	
	// 局所最大値、最小値を取得します。
	vGetLocalMaxMin();

	// 大域的最大値、最小値を取得します。
	vGetGlobalMaxMin();
}

/**
 * <PRE>
 * 　Randomized Memetic Artificial Bee Colony Algorthimより
 *   International Journal of Emerging Trends of Technology in Computer Science, vol.3(1), 2014
 *   ver 0.1 2016/09/23 初版
 *   ver 0.2 2016/10/25 更新候補点の算出に誤りを発見し修正。
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

	// employee bee の動作
	vEmployBeeOrigin();

	// onlookers beeの動作
	vOnlookerBeeRM();

	// scout bee の実行
	vScoutBeeNormal();
//	vScoutBeeUndx();

	// Randomized Memetic artificial bee colony Algorithm(Prに関しては0～1の間の適当な値を設定。)前回論文では0.3が推奨値。
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
	
	// 局所最大値、最小値を取得します。
	vGetLocalMaxMin();

	// 大域的最大値、最小値を取得します。
	vGetGlobalMaxMin();
}

/**
 * <PRE>
 * 　人工蜂コロニー最適化法（交叉を導入した手法）を実行します。
 *   A Novel Hybrid Crossover based Artificial Bee Colony Algorithm for Optimization Problem International Journal of Computer Applications 2013より
 *   ver 0.1 2016/04/11 初版
 *   ver 0.2 2016/10/25 更新候補点の算出に誤りを発見し修正。
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

	// employee bee の動作
	vEmployBeeOrigin();

	// crossoverを実行します。(もっとも旧式の実数値ＧＡの交叉)
	iParentLoc1 = mrand() % iAbcSearchNum;
	iParentLoc2 = mrand() % iAbcSearchNum;

// 実数値ＧＡの平均交叉を実行します。中点、内分点、外分点の３点を算出します。
	for( i = 0;i < iAbcVectorDimNum; i++ )
	{
		stlCrossOverData1.push_back( 0.5*( pplfAbcData[iParentLoc1][i] + pplfAbcData[iParentLoc2][i] ) );
		stlCrossOverData2.push_back( 1.5*( pplfAbcData[iParentLoc1][i] + 0.5*pplfAbcData[iParentLoc2][i] ) );
		stlCrossOverData3.push_back( -0.5*( pplfAbcData[iParentLoc1][i] + 1.5*pplfAbcData[iParentLoc2][i] ) );
	}
	
	// ランダムに選択した親の中で評価関数の値が悪いものと生成した子供を交換します。
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


	// onlookers beeの動作
	vOnlookerBeeOrigin();

	// scout bee の実行
	vScoutBeeNormal();
	
	// 局所最大値、最小値を取得します。
	vGetLocalMaxMin();

	// 大域的最大値、最小値を取得します。
	vGetGlobalMaxMin();
}

/**
 * <PRE>
 * 　人工蜂コロニー最適化法（交叉を導入した手法）を実行します。
 *   完全自作アレンジ
 * </PRE>
 * @author kobayashi
 * @since 2016/8/10
 * @version 0.1
 */
void CAbc::vUndxAbc()
{
	// employee bee の動作
	vEmployBeeBest();

	// onlookers beeの動作
	vOnlookerBeeBest();

	// scout bee の実行
	vScoutBeeUndx();
	
	// 局所最大値、最小値を取得します。
	vGetLocalMaxMin();

	// 大域的最大値、最小値を取得します。
	vGetGlobalMaxMin();
}

/**
 * <PRE>
 * 　人工蜂コロニー最適化法（交叉を導入した手法）を実行します。
 *   完全自作アレンジ
 *   2011の高性能化を導入
 * </PRE>
 * @author kobayashi
 * @since 2016/8/19
 * @version 0.1
 */
void CAbc::vUndxEnhancedAbc( int iUpdateCount )
{
	double lfFitJudge = 0.0;

	// employee bee の動作
//	lfFitJudge = lfEmployBeeBestEnhanced( iUpdateCount );
	lfFitJudge = lfEmployBeeEnhanced( iUpdateCount );

	// onlookers beeの動作
//	vOnlookerBeeBestEnhanced( iUpdateCount, lfFitJudge );
	vOnlookerBeeEnhanced( iUpdateCount, lfFitJudge );

	// scout bee の実行
	vScoutBeeUndx();
	
	// 局所最大値、最小値を取得します。
	vGetLocalMaxMin();

	// 大域的最大値、最小値を取得します。
	vGetGlobalMaxMin();
}

/**
 * <PRE>
 * 　人工蜂コロニー最適化法（交叉を導入した手法）を実行します。
 *   完全自作アレンジ（Emsanble Real Coded CrossOverを用います。）
 * </PRE>
 * @author kobayashi
 * @since 2016/8/10
 * @version 0.1
 */
void CAbc::vRexAbc()
{
	// employee bee の動作
	vEmployBeeBest();

	// onlookers beeの動作
	vOnlookerBeeBest();

	// scout bee の実行
	vScoutBeeRex();
	
	// 局所最大値、最小値を取得します。
	vGetLocalMaxMin();

	// 大域的最大値、最小値を取得します。
	vGetGlobalMaxMin();
}

/**
 * <PRE>
 * 　人工蜂コロニー最適化法（交叉を導入した手法）を実行します。
 *   完全自作アレンジ（Adaptation Emsanble Real Coded CrossOverを用います。）
 * </PRE>
 * @author kobayashi
 * @since 2016/8/27
 * @version 0.1
 */
void CAbc::vARexAbc()
{
	// employee bee の動作
	vEmployBeeBest();

	// onlookers beeの動作
	vOnlookerBeeBest();

	// scout bee の実行
	vScoutBeeARex();
	
	// 局所最大値、最小値を取得します。
	vGetLocalMaxMin();

	// 大域的最大値、最小値を取得します。
	vGetGlobalMaxMin();
}

/**
 * <PRE>
 * 　人工蜂コロニー最適化法を実行します。
 *   HJABC法を適用します。
 *   Artificial Bee Colony Algorithm ith Local Search for Numerical Optimization, Jornal of Software, Vol.6, no.3, march 2011より
 *   ver 0.1 2016/10/03 初版
 *   ver 0.2 2016/10/25 アルゴリズムが実現できていなかったので実現中。
 *   ver 0.3 2016/10/27 アルゴリズム実装完了。
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

	// employee bee の動作
	vEmployBeeOrigin();

	// onlookers beeの動作
	vOnlookerBeeHJ();

	// scout bee の実行
	vScoutBeeNormal();
	
	// 局所最大値、最小値を取得します。
	vGetLocalMaxMin();

	// 大域的最大値、最小値を取得します。
	vGetGlobalMaxMin();

	// ステップサイズの計算を実行します。
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
		// Hooke-Jeeves法を適用します。
		vModifiedHookeJeevesMethod(plfStepSize, plfX1, plfX2, plfX0);

		lfObjFunc0 = pflfObjectiveFunction(plfX0, iAbcVectorDimNum);
		lfObjFunc1 = pflfObjectiveFunction(plfX1, iAbcVectorDimNum);
		lfObjFunc2 = pflfObjectiveFunction(plfX2, iAbcVectorDimNum);

		// 現在の最適値を更新します。
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
			// Hooke-Jeeves法を適用します。
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

	// Hooke-Jeeves法を適用します。
	for (k = 0; k < iHJCounter; k++)
	{
		// 各ベクトルのステップサイズの計算をします。
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
* 　人工蜂コロニー最適化法（交叉を導入した手法）を実行します。
*   算術交叉を用いた改良型Artificial Bee Colonyアルゴリズム, 第28回Fuzzy System Symposium, 9, 2012. より.
*   ver 0.1 初版
* </PRE>
* @author kobayashi
* @since 2016/10/17
* @version 0.1
*/
void CAbc::vACAbc()
{
	// employee bee の動作
	vEmployBeeOrigin();

	// onlookers beeの動作
	vOnlookerBeeAC();

	// scout bee の実行
	vScoutBeeNormal();

	// 局所最大値、最小値を取得します。
	vGetLocalMaxMin();

	// 大域的最大値、最小値を取得します。
	vGetGlobalMaxMin();
}

/**
* <PRE>
* 　交叉手法を導入した人工蜂コロニー最適化法を実行します。
*   Ivona Brajevic, Crossover-based artificial bee colony algorithm for constrained optimization problems, Neural Computing & Application (2015) 26:1587-1601.
*   ver 0.1 初版
* </PRE>
* @param iCount 現在の計算回数
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

	// employee bee の動作
	vEmployBeeCB( lfMr );

	// onlookers beeの動作
	vOnlookerBeeCB( lfMr );

	// scout bee の実行
	vScoutBeeCB( iUpdateCount );

	// 閾値判定を更新。
	lfMr = lfMr < lfMrMax ? lfMr + (lfMrMax - 0.1) / (lfP*lfMCN) : lfMrMax;

	// 局所最大値、最小値を取得します。
	vGetLocalMaxMin();

	// 大域的最大値、最小値を取得します。
	vGetGlobalMaxMin();
}

/**
 * <PRE>
 * 　Best-so-far Artificial Bee Colony Methodを実行します。
 *   The best-so-far selection in Artificial Bee Colony algorithm Applied Soft Computing 11 (2011) 2888-2901
 *   ver 0.1 2016.10.28 初版
 * </PRE>
 * @param iCount 現在の計算回数
 * @author kobayashi
 * @since 2016/10/28
 * @version 0.1
 */
void CAbc::vBFAbc( int iUpdateCount )
{
	// employee bee の動作
	vEmployBeeBF();

	// onlookers beeの動作
	vOnlookerBeeBF();

	// scout bee の実行
	vScoutBeeBF( iUpdateCount );

	// 局所最大値、最小値を取得します。
	vGetLocalMaxMin();

	// 大域的最大値、最小値を取得します。
	vGetGlobalMaxMin();
}

/**
* <PRE>
* 　人工蜂コロニー最適化法を実行します。
*   Powell法を導入したPABC法を実行します。
*   ver 0.1 2016/1104 初版
* </PRE>
* @author kobayashi
* @since 2016/11/04
* @version 0.1
*/
void CAbc::vPAbc()
{
	// employee bee の動作
	vEmployBeeBest();

	// onlookers beeの動作
	vOnlookerBeeBest();

	// scout bee の実行
	vScoutBeeUndx();

	// 局所最大値、最小値を取得します。
	vGetLocalMaxMin();

	// 大域的最大値、最小値を取得します。
	vGetGlobalMaxMin();
}


/**
 * <PRE>
 * 　Employ Beeを実行します。(大本のバージョンと同じ手法)
 *   ver 0.1 2016/08/18 初版 
 *   ver 0.2 2016/10/25 更新候補点の算出に誤りを発見し修正。
 *   ver 0.3 2016/10/28 余計なループの削除を実施。
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
// employee bee の動作

	// 更新点候補を算出します。
	m = mrand() % iAbcSearchNum;
	h = mrand() % iAbcVectorDimNum;
	
	for( i = 0;i < iAbcSearchNum; i++ )
	{
		lfRand = 2*rnd()-1;
		for( j = 0; j < iAbcVectorDimNum; j++ )
			pplfVelocityData[i][j] = pplfAbcData[i][j];
		pplfVelocityData[i][h] = pplfAbcData[i][h] + lfRand*( pplfAbcData[i][h] - pplfAbcData[m][h] );

		// 各探索点と更新しなかった回数を格納する変数を更新します。
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
 * 　Employ Beeを実行します。(高精度化バージョン)
 *   2011の電子情報通信学会の論文より
 *   ver 0.1 
 *   ver 0.2 2016/10/25 更新候補点の算出に誤りを発見し修正。
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

	// employee bee の動作
	for( i = 0;i < iAbcSearchNum; i++ )
	{
		// 適応度の算出
		lfObjFunc = pflfObjectiveFunction( pplfAbcData[i], iAbcVectorDimNum );
		if( lfObjFunc-lfFitBound >= lfFitAccuracy )	lfFitProb = 1.0/( lfObjFunc-lfFitBound );
		else										lfFitProb = 1.0/lfFitAccuracy;
		plfFit[i] = lfFitProb;

		// 評価値をNp個算出します。
		stlFitProb[i].iLoc = i;
		stlFitProb[i].lfFitProb = lfFitProb;
	}
//	// 適応度のソートを実行します。
//	qsort( 0, iAbcSearchNum, plfFit );
	// 目的関数値によるソートを実施します。
	std::sort( stlFitProb.begin(), stlFitProb.end(), CCompareToRank() );

	// 
	if( iUpdateCount >= iAbcIntervalMinNum )
	{
		if( lfFitJudge < lfConvergenceParam )
		{
			// Fjudgeの値を更新します。
			lfFitJudge = ( lfFitInit - lfFitCurrentBest ) / ( lfFitInit - lfFitBound );
			if( lfFitJudge >= lfConvergenceParam )
			{
				// 各探索点の相対評価確率を算出します。
				lfRes = 0.0;
				for( i = 0;i < iAbcSearchNum; i++ )	lfRes += stlFitProb[i].lfFitProb; 
				for( i = 0;i < iAbcSearchNum; i++ )	plfFitProb[i] = stlFitProb[i].lfFitProb/lfRes;
			}
			// 更新点候補を算出します。
			if( iUpdateCount >= iAbcIntervalMinNum && lfFitJudge >= lfConvergenceParam )
			{
				// ルーレット戦略により、mの値を決定します。
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
				// 適応度上位αからランダムに決定します。
				m = mrand() % iAbcUpperSearchNum;
			}
			// ランダムに決定します。
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
			// 各探索点の相対評価確率を算出します。
			lfRes = 0.0;
			for( i = 0;i < iAbcSearchNum; i++ )	lfRes += stlFitProb[i].lfFitProb; 
			for( i = 0;i < iAbcSearchNum; i++ )	plfFitProb[i] = stlFitProb[i].lfFitProb/lfRes;

			// 更新点候補を算出します。
			if( iUpdateCount >= iAbcIntervalMinNum && lfFitJudge >= lfConvergenceParam )
			{
				// ルーレット戦略により、mの値を決定します。
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
				// その他の場合はランダムに決定します。
				m = mrand() % iAbcUpperSearchNum;
			}
			// ランダムに決定します。
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
		// 更新点候補を算出します。
		if( iUpdateCount >= iAbcIntervalMinNum && lfFitJudge >= lfConvergenceParam )
		{
			// ルーレット戦略により、mの値を決定します。
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
			// その他の場合はランダムに決定します。
			m = mrand() % iAbcUpperSearchNum;
		}
		// ランダムに決定します。
		h = mrand() % iAbcVectorDimNum;

		for( i = 0;i < iAbcSearchNum; i++ )
		{
			lfRand = 2.0*rnd()-1.0;
			for( j = 0; j < iAbcVectorDimNum; j++ )
				pplfVelocityData[i][j] = pplfAbcData[i][j];
			pplfVelocityData[i][h] = pplfAbcData[i][h] + lfRand*( pplfAbcData[i][h] - pplfAbcData[stlFitProb[m].iLoc][h] );
		}
	}

	// 各探索点を更新します。
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
 * 　Employ Beeを実行します。(高精度化バージョン)
 *   2011の電子情報通信学会の論文より
 *   ver 0.1 
 *   ver 0.2 2016/10/25 更新候補点の算出に誤りを発見し修正。
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

	// employee bee の動作
	for( i = 0;i < iAbcSearchNum; i++ )
	{
		// 適応度の算出
		lfObjFunc = pflfObjectiveFunction( pplfAbcData[i], iAbcVectorDimNum );
		if( lfObjFunc-lfFitBound >= lfFitAccuracy )	lfFitProb = 1.0/( lfObjFunc-lfFitBound );
		else										lfFitProb = 1.0/lfFitAccuracy;
		plfFit[i] = lfFitProb;
	}
	// 適応度のソートを実行します。
	qsort( 0, iAbcSearchNum, plfFit );

	// 
	if( iUpdateCount >= iAbcIntervalMinNum )
	{
		if( lfFitJudge < lfConvergenceParam )
		{
			// Fjudgeの値を更新します。
			lfFitJudge = ( lfFitInit - lfFitCurrentBest ) / ( lfFitInit - lfFitBound );
			if( lfFitJudge >= lfConvergenceParam )
			{
				// 各探索点の相対評価確率を算出します。
				lfRes = 0.0;
				for( i = 0;i < iAbcSearchNum; i++ )	lfRes += plfFit[i]; 
				for( i = 0;i < iAbcSearchNum; i++ )	plfFitProb[i] = plfFit[i]/lfRes;
			}
			else
			{
				// 更新点候補を算出します。
				if( iUpdateCount >= iAbcIntervalMinNum && lfFitJudge >= lfConvergenceParam )
				{
					// ルーレット戦略により、mの値を決定します。
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
					// 適応度上位αからランダムに決定します。
					m = mrand() % iAbcUpperSearchNum;
				}
				// ランダムに決定します。
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
			// 各探索点の相対評価確率を算出します。
			lfRes = 0.0;
			for( i = 0;i < iAbcSearchNum; i++ )	lfRes += plfFit[i]; 
			for( i = 0;i < iAbcSearchNum; i++ )	plfFitProb[i] = plfFit[i]/lfRes;

			// 更新点候補を算出します。
			if( iUpdateCount >= iAbcIntervalMinNum && lfFitJudge >= lfConvergenceParam )
			{
				// ルーレット戦略により、mの値を決定します。
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
				// その他の場合はランダムに決定します。
				m = mrand() % iAbcUpperSearchNum;
			}
			// ランダムに決定します。
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
		// 更新点候補を算出します。
		if( iUpdateCount >= iAbcIntervalMinNum && lfFitJudge >= lfConvergenceParam )
		{
			// ルーレット戦略により、mの値を決定します。
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
			// その他の場合はランダムに決定します。
			m = mrand() % iAbcUpperSearchNum;
		}
		// ランダムに決定します。
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

	// 各探索点を更新します。
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
 * 　Employ Beeを実行します。
 *   ver 0.1 
 *   ver 0.2 NBest版に修正
 *   ver 0.3 2016/10/25 更新候補点の算出に誤りを発見し修正。
 *   ver 0.4 2016/10/28 余計なループの削除を実施。
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
// employee bee の動作

	// 更新点候補を算出します。
	m = mrand() % iAbcSearchNum;
	h = mrand() % iAbcVectorDimNum;
	
	for( i = 0;i < iAbcSearchNum; i++ )
	{
		lfRand = 2*rnd()-1;
		for( j = 0; j < iAbcVectorDimNum; j++ )
			pplfVelocityData[i][j] = pplfAbcData[i][j];
		pplfVelocityData[i][h] = pplfAbcData[i][h] + lfRand*( pplfAbcData[i][h] - pplfLocalMinAbcData[m][h] );

		// 各探索点と更新しなかった回数を格納する変数を更新します。
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
 * 　Employ Beeを実行します。(GBest版)
 *   ver 0.1 
 *   ver 0.2 NBest版に修正
 *   ver 0.3 2016/10/24 論文を基に修正
 *   ver 0.4 2016/10/25 更新候補点の算出に誤りを発見し修正。
 *   ver 0.5 2016/10/28 余計なループの削除を実施。
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
// employee bee の動作

	// 更新点候補を算出します。
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

		// 各探索点と更新しなかった回数を格納する変数を更新します。
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
 * 　Employ Beeを実行します。
 *   ver 0.1 
 *   ver 0.2 2016/08/18 IWCFA(粒子群最適化法の一手法を適用)
 *   ver 0.3 2016/10/28 余計なループの削除を実施。
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

	// 更新点候補を算出します。
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

		// 各探索点と更新しなかった回数を格納する変数を更新します。
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
*   ver 0.1 2016/10/19 初版
*   ver 0.2 2016/10/28 余計なループの削除を実施。
* </PRE>
* @param lfMr 更新用パラメーター
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

	// employee bee の動作
	// 更新点候補を算出します。
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
		// 各探索点と更新しなかった回数を格納する変数を更新します。
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
 * 　Employ Beeを実行します。(Best-so-Far版)
 *   The best-so-far selection in Artificial Bee Colony algorithm Applied Soft Computing 11 (2011) 2888-2901
 *   ver 0.1 2016.10.28 初版
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

	// employee bee の動作
	// 更新点候補を算出します。
	m = mrand() % iAbcSearchNum;
	h = mrand() % iAbcVectorDimNum;
	
	for( i = 0;i < iAbcSearchNum; i++ )
	{
		lfRand = 2.0*rnd()-1.0;
		for( j = 0; j < iAbcVectorDimNum; j++ )
			pplfVelocityData[i][j] = pplfAbcData[i][j];
		pplfVelocityData[i][h] = pplfAbcData[i][h] + lfRand*(pplfAbcData[i][h] - pplfAbcData[m][h]);

		// 各探索点と更新しなかった回数を格納する変数を更新します。
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
 *   Onlooker Beeを実行します。(大本のバージョン)
 *   ver 0.1 2016/08/10 初版
 *   ver 0.2 2016/10/25 更新候補点の算出に誤りを発見し修正。
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
		// 適応度の算出
		lfObjFunc = pflfObjectiveFunction( pplfAbcData[j], iAbcVectorDimNum );
		if( lfObjFunc >= 0.0 )	lfFitProb = 1.0/( 1.0+lfObjFunc );
		else			lfFitProb = 1.0+fabs( lfObjFunc );
		lfRes += lfFitProb;
		plfFit[j] = lfFitProb;
	}
	// 適応度の正規化
	for( j = 0;j < iAbcSearchNum; j++ )	plfFitProb[j] = plfFit[j]/lfRes;
	// ルーレット戦略を実行
	lfProb = lfPrevProb = 0.0;
	lfRand = rnd();
	c = 0;
	for( j = 0;j < iAbcSearchNum; j++ )
	{
		lfProb += plfFitProb[j];
		if( lfPrevProb <= lfRand && lfRand <= lfProb )	c = j;
		lfPrevProb = lfProb;
	}
	// ルーレット選択した探索点に対して更新候補を求めます。

	// 更新点候補を算出します。
	// 更新点候補を乱数により決定します。
	m = mrand() % iAbcSearchNum;
	h = mrand() % iAbcVectorDimNum;

	lfRand = 2*rnd()-1;
	for( j = 0; j < iAbcVectorDimNum; j++ )
		pplfVelocityData[c][j] = pplfAbcData[c][j];
	pplfVelocityData[c][h] = pplfAbcData[c][h] + lfRand*( pplfAbcData[c][h] - pplfAbcData[m][h] );
	
	// 更新点候補を次のように更新します。
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
 * 　Onlooker Beeを実行します。(高精度化バージョン)
 *   2011の電子情報通信学会の論文より
 *   ver 0.1 2016/08/18 初版
 *   ver 0.2 2016/10/25 更新候補点の算出に誤りを発見し修正。
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

	// 更新点候補を算出します。
	if( iUpdateCount >= iAbcIntervalMinNum && lfFitJudge >= lfConvergenceParam )
	{
		// ルーレット戦略により、mの値を決定します。
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
		// その他の場合は適応度上位αからランダムに決定します。
		c = mrand() % iAbcUpperSearchNum;
	}
	// ランダムに決定します。
	h = mrand() % iAbcVectorDimNum;
	
	// 更新点候補を生成します。
	lfRand = 2*rnd()-1;
	for( j = 0; j < iAbcVectorDimNum; j++ )
		pplfVelocityData[stlFitProb[c].iLoc][j] = pplfAbcData[stlFitProb[c].iLoc][j];
	pplfVelocityData[stlFitProb[c].iLoc][h] = pplfAbcData[stlFitProb[c].iLoc][h] + lfRand*( pplfAbcData[stlFitProb[c].iLoc][h] - pplfAbcData[stlFitProb[c].iLoc][h] );

	// 各探索点を更新します。
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
 * 　Onlooker Beeを実行します。(高精度化バージョン)
 *   2011の電子情報通信学会の論文より
 *   ver 0.1 2016/08/18 初版 
 *   ver 0.2 2016/10/25 更新候補点の算出に誤りを発見し修正。
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

	// 更新点候補を算出します。
	if( iUpdateCount >= iAbcIntervalMinNum && lfFitJudge >= lfConvergenceParam )
	{
		// ルーレット戦略により、mの値を決定します。
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
		// その他の場合はランダムに決定します。
		c = mrand() % iAbcUpperSearchNum;
	}
	// ランダムに決定します。
	h = mrand() % iAbcVectorDimNum;

	// 更新点候補を生成します。
	for( i = 0;i < iAbcSearchNum; i++ )
	{
		lfRand = 2*rnd()-1;
		for( j = 0; j < iAbcVectorDimNum; j++ )
			pplfVelocityData[i][j] = pplfAbcData[i][j];
		pplfVelocityData[i][h] = pplfAbcData[i][h] + lfRand*( pplfAbcData[i][h] - plfGlobalMinAbcData[h] );
	}
	// 各探索点を更新します。
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
 *   Onlooker Beeを実行します。(NBest版)
 *   ver 0.1 2016/08/10 初版
 *   ver 0.2 2016/09/13 NBest版を追加
 *   ver 0.3 2016/10/25 更新候補点の算出に誤りを発見し修正。
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
		// 適応度の算出
		lfObjFunc = pflfObjectiveFunction( pplfAbcData[j], iAbcVectorDimNum );
		if( lfObjFunc >= 0.0 )	lfFitProb = 1.0/( 1.0+lfObjFunc );
		else			lfFitProb = 1.0+fabs( lfObjFunc );
		lfRes += lfFitProb;
		plfFit[j] = lfFitProb;
	}
	// 適応度の正規化
	for( j = 0;j < iAbcSearchNum; j++ )	plfFitProb[j] = plfFit[j]/lfRes;
	// ルーレット戦略を実行
	lfProb = lfPrevProb = 0.0;
	lfRand = rnd();
	c = 0;
	for( j = 0;j < iAbcSearchNum; j++ )
	{
		lfProb += plfFitProb[j];
		if( lfPrevProb <= lfRand && lfRand <= lfProb )	c = j;
		lfPrevProb = lfProb;
	}

	// ルーレット選択した探索点に対して更新候補を求めます。

	// 更新点候補を算出します。
	// 更新点候補を乱数により決定します。
	m = mrand() % iAbcSearchNum;
	h = mrand() % iAbcVectorDimNum;

	lfRand = 2*rnd()-1;
	for( j = 0; j < iAbcVectorDimNum; j++ )
		pplfVelocityData[c][j] = pplfAbcData[c][j];
	pplfVelocityData[c][h] = pplfAbcData[c][h] + lfRand*( pplfAbcData[c][h] - plfGlobalMinAbcData[h] );

	// 更新点候補を次のように更新します。
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
 *   Onlooker Beeを実行します。(GBest版)
 *   ver 0.1 2016/08/10 初版
 *   ver 0.2 2016/09/13 NBest版を追加
 *   ver 0.3 2016/10/25 更新候補点の算出に誤りを発見し修正。
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
		// 適応度の算出
		lfObjFunc = pflfObjectiveFunction( pplfAbcData[j], iAbcVectorDimNum );
//		if( lfObjFunc >= 0.0 )	lfFitProb = 1.0/( 1.0+lfObjFunc );
//		else					lfFitProb = 1.0+fabs( lfObjFunc );
		lfFitProb = lfObjFunc;
		lfRes += lfFitProb;
		plfFit[j] = lfFitProb;
	}
	// 適応度の正規化
	for( j = 0;j < iAbcSearchNum; j++ ) plfFitProb[j] = plfFit[j]/lfRes;
	// ルーレット戦略を実行
	lfProb = lfPrevProb = 0.0;
	lfRand = rnd();
	c = 0;
	for( j = 0;j < iAbcSearchNum; j++ )
	{
		lfProb += plfFitProb[j];
		if( lfPrevProb <= lfRand && lfRand <= lfProb )	c = j;
		lfPrevProb = lfProb;
	}

	// ルーレット選択した探索点に対して更新候補を求めます。

	// 更新点候補を算出する。
	// 更新点候補を乱数により決定する。
	m = mrand() % iAbcSearchNum;
	h = mrand() % iAbcVectorDimNum;

	lfRand = 2.0*rnd()-1.0;
	lfRand2 = 1.5*rnd();
	for( j = 0; j < iAbcVectorDimNum; j++ )
//		pplfVelocityData[c][j] = pplfAbcData[c][j] + lfRand*(pplfAbcData[c][j] - pplfAbcData[m][j]) + lfRand2*(plfGlobalMinAbcData[j] - pplfAbcData[c][j]);
		pplfVelocityData[c][j] = pplfAbcData[c][j];
	pplfVelocityData[c][h] = pplfAbcData[c][h] + lfRand*( pplfAbcData[c][h] - pplfAbcData[m][h] ) + lfRand2*( plfGlobalMinAbcData[h] - pplfAbcData[c][h] );
	// 更新点候補を次のように更新します。
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
 *   Onlooker Beeを実行します。(IWCFA版)
 *   ver 0.1 2016/08/10 初版
 *   ver 0.2 2016/09/13 IWCFA版を追加
 *   ver 0.3 2016/10/25 更新候補点の算出に誤りを発見し修正。
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
		// 適応度の算出
		lfObjFunc = pflfObjectiveFunction( pplfAbcData[j], iAbcVectorDimNum );
		if( lfObjFunc >= 0.0 )	lfFitProb = 1.0/( 1.0+lfObjFunc );
		else					lfFitProb = 1.0+fabs( lfObjFunc );
		lfRes += lfFitProb;
		plfFit[j] = lfFitProb;
	}
	// 適応度の正規化
	for( j = 0;j < iAbcSearchNum; j++ ) plfFitProb[j] = plfFit[j]/lfRes;
	// ルーレット戦略を実行
	lfProb = lfPrevProb = 0.0;
	lfRand = rnd();
	c = 0;
	for( j = 0;j < iAbcSearchNum; j++ )
	{
		lfProb += plfFitProb[j];
		if( lfPrevProb <= lfRand && lfRand <= lfProb )	c = j;
		lfPrevProb = lfProb;
	}

	// ルーレット選択した探索点に対して更新候補を求めます。

	// 更新点候補を算出する。
	// 更新点候補を乱数により決定する。
	m = mrand() % iAbcSearchNum;
	h = mrand() % iAbcVectorDimNum;

	lfRand = 2.0*rnd()-1.0;
	lfRand2 = 2.0*rnd()-1.0;
	for( j = 0; j < iAbcVectorDimNum; j++ )
		pplfVelocityData[c][j] = pplfAbcData[c][j];
	lfWeight = lfMaxWeight - (lfMaxWeight-lfMinWeight)/(double)iGenerationNumber*(double)(iUpdateCount-piTotalNonUpdateCount[c]);
	pplfVelocityData[c][h] = lfK*(pplfAbcData[c][h] + lfCoe1*lfRand*( pplfAbcData[c][h] - pplfAbcData[m][h] ) + lfCoe2*lfRand2*( pplfAbcData[c][h] - plfGlobalMinAbcData[h] ) );
	// 更新点候補を次のように更新します。
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
 *   Onlooker Beeを実行します。
 *   Randomized Memtic Bee Colony Method用
 *   ver 0.1 2016/09/22 初版
 *   ver 0.2 2016/10/25 更新候補点の算出に誤りを発見し修正。
 *   ver 0.3 2016/10/28 一部誤りがあり修正。
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
		// 適応度の算出
		lfObjFunc = pflfObjectiveFunction( pplfAbcData[j], iAbcVectorDimNum );
		if( lfObjFunc >= 0.0 )	lfFitProb = 1.0/( 2.0*lfObjFunc+1.0 );
		else			lfFitProb = 1.0+fabs( 1.0/lfObjFunc );
		lfRes += lfFitProb;
		plfFit[j] = lfFitProb;
	}
	// 適応度の正規化
	for( j = 0;j < iAbcSearchNum; j++ )	plfFitProb[j] = plfFit[j]/lfRes;
	// ルーレット戦略を実行
	lfProb = lfPrevProb = 0.0;
	lfRand = rnd();
	c = 0;
	for( j = 0;j < iAbcSearchNum; j++ )
	{
		lfProb += plfFitProb[j];
		if( lfPrevProb <= lfRand && lfRand <= lfProb )	c = j;
		lfPrevProb = lfProb;
	}

	// ルーレット選択した探索点に対して更新候補を求めます。

	// 更新点候補を算出します。
	// 更新点候補を乱数により決定します。
	m = mrand() % iAbcSearchNum;
	h = mrand() % iAbcVectorDimNum;

	lfRand = 2.0*rnd()-1.0;
	lfRand2 = 1.5*rnd();
	for( j = 0; j < iAbcVectorDimNum; j++ )
		pplfVelocityData[c][j] = pplfAbcData[c][j];
	pplfVelocityData[c][h] = pplfAbcData[c][h] + lfRand*( pplfAbcData[c][h] - pplfAbcData[m][h] ) + lfRand2*( plfGlobalMinAbcData[h] - pplfAbcData[c][h] );
		
	// 更新点候補を次のように更新します。
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
 *   Onlooker Beeを実行します。(Hooke-Jeeves法用)
 *   Artificial Bee Colony Algorithm ith Local Search for Numerical Optimization, Jornal of Software, Vol.6, no.3, march 2011より
 *   ver 0.1 2016/10/03 初版
 *   ver 0.2 2016/10/25 更新候補点の算出に誤りを発見し修正。
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

	// 適応度でソートする。
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
		// 適応度の算出
		lfObjFunc = pflfObjectiveFunction( pplfAbcData[stlRank.at(j).iLoc], iAbcVectorDimNum );
		lfFitProb = 2.0-lfSP+2.0*(lfSP-1.0)*(j-1.0)/( iAbcSearchNum-1.0 );
		lfRes += lfFitProb;
		plfFit[stlRank.at(j).iLoc] = lfFitProb;
	}
	// 適応度の正規化
	for( j = 0;j < iAbcSearchNum; j++ )	plfFitProb[j] = plfFit[stlRank.at(j).iLoc]/lfRes;
	// ルーレット戦略を実行
	lfProb = lfPrevProb = 0.0;
	lfRand = rnd();
	c = 0;
	for( j = 0;j < iAbcSearchNum; j++ )
	{
		lfProb += plfFitProb[stlRank.at(j).iLoc];
		if( lfPrevProb <= lfRand && lfRand <= lfProb )	c = stlRank.at(j).iLoc;
		lfPrevProb = lfProb;
	}

	// ルーレット選択した探索点に対して更新候補を求めます。

	// 更新点候補を算出します。
	// 更新点候補を乱数により決定します。
	m = mrand() % iAbcSearchNum;
	h = mrand() % iAbcVectorDimNum;
	lfRand = 2*rnd()-1;
	for( j = 0; j < iAbcVectorDimNum; j++ )
		pplfVelocityData[c][j] = pplfAbcData[c][j];
	pplfVelocityData[c][h] = pplfAbcData[c][h] + lfRand*( pplfAbcData[c][h] - pplfAbcData[m][h] );
	// 更新点候補を次のように更新します。
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
*   Onlooker Beeを実行します。(AC-ABC用算術交叉を利用したABC法。)
*   算術交叉を用いた改良型Artificial Bee Colony アルゴリズム 第28回ファジーシンポジウム, 2012.9.12～14
*   ver 0.1 2016/10/13 初版
*   ver 0.2 2016/10/25 更新候補点の算出に誤りを発見し修正。
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

	// 論文記載の内容より。
	double lfLambda = 0.1;
	double lfCrossOverRate = 0.1;

	lfRes = 0.0;
	for (j = 0; j < iAbcSearchNum; j++)
	{
		// 適応度の算出
		lfObjFunc = pflfObjectiveFunction(pplfAbcData[j], iAbcVectorDimNum);
//		if (lfObjFunc >= 0.0)	lfFitProb = 1.0 / (1.0 + lfObjFunc);
//		else					lfFitProb = 1.0 + fabs(lfObjFunc);
		lfFitProb = lfObjFunc;
		lfRes += lfFitProb;
		plfFit[j] = lfFitProb;
	}
	// 適応度の正規化
	for (j = 0; j < iAbcSearchNum; j++)	plfFitProb[j] = plfFit[j] / lfRes;
	// ルーレット戦略を実行
	lfProb = lfPrevProb = 0.0;
	lfRand = rnd();
	c1 = c2 = 0;
	for (j = 0; j < iAbcSearchNum; j++)
	{
		lfProb += plfFitProb[j];
		if (lfPrevProb <= lfRand && lfRand <= lfProb)
		{
			c1 = j;
			// 簡単な乱択アルゴリズム
			if (j == 0) c2 = mrand() % iAbcSearchNum + 1;
			else	    c2 = mrand() % j;
		}
		lfPrevProb = lfProb;
	}
	// ルーレット選択した探索点に対して更新候補を求めます。

	// 更新点候補を算出します。
	// 更新点候補を乱数により決定します。
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

	// 更新点候補を次のように更新します。
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
 *   Onlooker Beeを実行します。
 *   Best-so-far Artificial Bee Colony Method用
 *   The best-so-far seelction in ARtificial Bee Colony algorithm, Applied Soft Computing 11 (2011) 2888-2901.
 *   ver 0.1 2016/10/28 初版
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
		// 適応度の算出
		lfObjFunc = pflfObjectiveFunction(pplfAbcData[j], iAbcVectorDimNum);
		if (lfObjFunc >= 0.0)	lfFitProb = 1.0 / (1.0 + lfObjFunc);
		else					lfFitProb = 1.0 + fabs(lfObjFunc);
		lfRes += lfFitProb;
		plfFit[j] = lfFitProb;
	}
	// 適応度の正規化
	for (j = 0; j < iAbcSearchNum; j++)	plfFitProb[j] = plfFit[j] / lfRes;
	lfLocalBestRand /= lfRes;

	for (i = 0; i < iAbcSearchNum; i++)
	{
		// ルーレット戦略を実行
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
		// ルーレット選択した探索点に対して更新候補を求めます。

		// 更新点候補を算出します。
		// 更新点候補を乱数により決定します。
		h = mrand() % iAbcVectorDimNum;

		lfRand = 2.0*rnd() - 1.0;
		for (j = 0; j < iAbcVectorDimNum; j++)
//			pplfVelocityData[c][j] = pplfAbcData[c][j];
			pplfVelocityData[i][j] = pplfAbcData[c][h] + lfRand*lfLocalBestRand*(pplfAbcData[c][h] - pplfLocalMinAbcData[c][h]);
//			pplfVelocityData[c][h] = pplfAbcData[c][h] + lfRand*lfGlobalMinAbcData*( pplfAbcData[c][h] - pplfAbcData[m][h] );
//			pplfVelocityData[c][h] = pplfAbcData[c][h] + lfRand*(pplfAbcData[c][h] - plfGlobalMinAbcData[h]);

				// 更新点候補を次のように更新します。
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
*   ver 0.1 2016/10/19 初版
*   ver 0.2 2016/10/25 更新候補点の算出に誤りを発見し修正。
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
		// 適応度の算出
		plfFit[i] = 0.9*(plfFit[i] / lfMaxFit) + 0.1;
		lfRes += plfFit[i];
	}
	// 適応度の正規化
	for (j = 0; j < iAbcSearchNum; j++)	plfFitProb[j] = plfFit[j] / lfRes;
	// ルーレット戦略を実行
	lfProb = lfPrevProb = 0.0;
	lfRand = rnd();
	c = 0;
	for (j = 0; j < iAbcSearchNum; j++)
	{
		lfProb += plfFitProb[j];
		if (lfPrevProb <= lfRand && lfRand <= lfProb)	c = j;
		lfPrevProb = lfProb;
	}
	// ルーレット選択した探索点に対して更新候補を求めます。

	// 更新点候補を算出します。
	// 更新点候補を乱数により決定します。
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
	// 各探索点と更新しなかった回数を格納する変数を更新します。
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
 * Scout Beeを実行します。(大本の手法)
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
	// 新たな探索点を求めて探索を実行します。
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
 * Scout Beeを実行します。
 * ver 0.1 2016.8.10 初版
 * ver 0.2 2016.8.11 手法の変更。（粒子群最適化法のような更新手法。論文より）
 * </PRE>
 * @author kobayashi
 * @since 2016/8/10
 * @version 0.2
 */
void CAbc::vScoutBeeNormal()
{
	int i,j,k;
	double lfRand = 0.0;
	// 新たな探索点を求めて探索を実行します。
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
 * Scout Beeを実行します。（完全アレンジ版UNDXを実行する。）
 * ver 0.1 2016.8.18 初版
 * </PRE>
 * @author kobayashi
 * @since 2016/8/18
 * @version 0.1
 */
void CAbc::vScoutBeeUndx()
{
	int i,j,k;
	double lfRand = 0.0;
	// 新たな探索点を求めて探索を実行します。
	for( i = 0;i < iAbcSearchNum; i++ )
	{
		if( piNonUpdateCount[i] > iAbcLimitCount )
		{
			// 交叉を実行します。ここでUNDXを実行します。（１回分のみ。）
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
 * Scout Beeを実行します。（完全アレンジ版REXを実行する。）
 * ver 0.1 2016.8.26 初版
 * </PRE>
 * @author kobayashi
 * @since 2016/8/26
 * @version 0.1
 */
void CAbc::vScoutBeeRex()
{
	int i,j,k;
	double lfRand = 0.0;
	// 新たな探索点を求めて探索を実行します。
	for( i = 0;i < iAbcSearchNum; i++ )
	{
		if( piNonUpdateCount[i] > iAbcLimitCount )
		{
			// 交叉を実行します。ここでREXを実行します。（１回分のみ。）
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
 * Scout Beeを実行します。（完全アレンジ版AREXを実行する。）
 * ver 0.1 2016.8.27 初版
 * </PRE>
 * @author kobayashi
 * @since 2016/8/27
 * @version 0.1
 */
void CAbc::vScoutBeeARex()
{
	int i,j,k;
	double lfRand = 0.0;
	// 新たな探索点を求めて探索を実行します。
	for( i = 0;i < iAbcSearchNum; i++ )
	{
		if( piNonUpdateCount[i] > iAbcLimitCount )
		{
			// 交叉を実行します。ここでAREXを実行します。（１回分のみ。）
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
*   ver 0.1 2016.10.19 初版 
* </PRE>
* @author kobayashi
* @since 2016/10/19
* @version 0.1
*/
void CAbc::vScoutBeeCB( int iCount )
{
	int i, j, k;
	double lfRand = 0.0;
	// 新たな探索点を求めて探索を実行します。
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
 * Scout Beeを実行します。(Best-so-far Artificial Bee Colony Method用)
 * The best-so-far selection in Artificial Bee colony algorithm, Applied Soft Computing 11 (2011) 2888-2901.
 * ver 0.1 初版 2016.10.28
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

	// 新たな探索点を求めて探索を実行します。
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
 * 現時点での目的関数の最大、最小を求めます。
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
	// ローカルの最大値を更新します。
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
 * 現時点での目的関数の全体を通しての最大、最小値を求めます。
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
 * 　現時点での最小値の粒子の目的関数値を出力します。
 * </PRE>
 * @author kobayashi
 * @since 2016/9/14
 * @version 1.0
 */
double CAbc::lfGetGlobalMinAbcDataConstFuncValue()
{
	// 現時点での各粒子の目的関数の値を出力します。
	return lfGlobalMinAbcData;
}

/**
 * <PRE>
 * 　ABC法を適用した結果を出力します。(各蜂の位置)
 * </PRE>
 * @author kobayashi
 * @since 2015/6/6
 * @version 1.0
 */
void CAbc::vOutputAbcData()
{
	int i,j;
	// 現時点での蜂の位置を出力します。
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
 * 　ABC法を適用した結果を出力します。(各ABCの速度)
 * </PRE>
 * @author kobayashi
 * @since 2015/6/6
 * @version 1.0
 */
void CAbc::vOutputVelocityData()
{
	int i,j;
	// 現時点での蜂の速度を出力します。
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
 * 　ABC法を適用した結果を出力します。(各蜂の目的関数値)
 * </PRE>
 * @author kobayashi
 * @since 2015/6/6
 * @version 1.0
 */
void CAbc::vOutputConstraintFunction()
{
	int i;
	// 現時点での各蜂の目的関数の値を出力します。
	for( i = 0; i < iAbcDataNum; i++ )
	{
		printf("%lf,", pflfObjectiveFunction(pplfAbcData[i], iAbcVectorDimNum) );
	}
	printf("\n");
}

/**
 * <PRE>
 * 　現時点での最大値を出す粒子の位置を出力します。
 * </PRE>
 * @author kobayashi
 * @since 2015/6/19
 * @version 1.0
 */
void CAbc::vOutputGlobalMaxAbcData()
{
	int i;
	// 現時点での各粒子の値を出力します。
	for( i = 0; i < iAbcVectorDimNum; i++ )
	{
		printf("%lf,", plfGlobalMaxAbcData[i] );
	}
	printf("\n");
}

/**
 * <PRE>
 * 　現時点での最大値の粒子の目的関数値を出力します。
 * </PRE>
 * @author kobayashi
 * @since 2015/6/19
 * @version 1.0
 */
void CAbc::vOutputGlobalMaxAbcDataConstFuncValue()
{
	// 現時点での各粒子の目的関数の値を出力します。
	printf("%lf\n", lfGlobalMaxAbcData );
}

/**
 * <PRE>
 * 　現時点での最小値を出す粒子の位置を出力します。
 * </PRE>
 * @author kobayashi
 * @since 2015/6/19
 * @version 1.0
 */
void CAbc::vOutputGlobalMinAbcData()
{
	int i;
	// 現時点での各粒子の値を出力します。
	for( i = 0; i < iAbcVectorDimNum; i++ )
	{
		printf("%lf,", plfGlobalMinAbcData[i] );
	}
	printf("\n");
}

/**
 * <PRE>
 * 　現時点での最小値の粒子の目的関数値を出力します。
 * </PRE>
 * @author kobayashi
 * @since 2015/6/19
 * @version 1.0
 */
void CAbc::vOutputGlobalMinAbcDataConstFuncValue()
{
	// 現時点での各粒子の目的関数の値を出力します。
	printf("%lf\n", lfGlobalMinAbcData );
}

/**
 * <PRE>
 * 　現時点でのもっともよい粒子位置とその他の粒子との距離を出力します。
 * </PRE>
 * @param iOutFlag 0 各粒子と他の粒子の距離のみ出力。
 * 　　　　　　　　1 平均距離を出力。
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
	// 現時点での各粒子と他の粒子との距離を出力します。
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
		// 現時点粒子間の平均距離を出力します。
		printf("%lf\n",lfAvgDist);
	}
}


/**
 * <PRE>
 * 　現時点での各粒子ごとの最良位置を出力します。
 * </PRE>
 * @param iFlag 0 目的関数値を出力しない。
 * 　　　　　　 1 目的関数値を出力する。
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
	// 現時点での各粒子ごとの最良位置を出力します。
	for( i= 0;i < iAbcDataNum; i++ )
	{
		for( j = 0; j < iAbcVectorDimNum; j++ )
		{
			printf("%lf,", pplfLocalMaxAbcData[i][j] );
		}
		if( iOutFlag == 1 )
		{
			// 現時点での各粒子の目的関数の値を出力します。
			printf("%lf,", plfLocalMaxObjectiveAbcData[i] );
		}
	}
	printf("\n");
}

/**
* <PRE>
* 　現時点での最小値の粒子の目的関数値を出力します。
* </PRE>
* @author kobayashi
* @since 2015/6/19
* @version 1.0
*/
void CAbc::vOutputGlobalMinAbcDataConstFuncValue( int iOutputCount )
{
	// 現時点での各粒子の目的関数の値を10回おきに出力します。
	if (iOutputCount % 10 == 0)
	{
		printf("%lf,", lfGlobalMinAbcData);
	}
}

void CAbc::vSetRange( double lfRange )
{
	lfSolveRange = lfRange;
}

// 適応度ソート用

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
////////////////CCmdCheckExceptionクラス

CAbcException::CAbcException()
{
	iErrCode = 0;
	strMethodName = "";
	strClassName = "";
	strErrDetail = "";
}

/**
 * コンストラクタ(エラー情報を設定する)
 * @param iCode       エラーコード
 * @param sMethodName 関数名
 * @param sClassName  クラス名
 * @param sDetail     エラー詳細
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
 * コンストラクタ(エラー情報を設定する)
 * @param iCode       エラーコード
 * @param iLine       行数
 * @param sMethodName 関数名
 * @param sClassName  クラス名
 * @param sDetail     エラー詳細
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
 * コンストラクタ(エラー情報を設定する)
 * @param iCode       エラーコード
 * @param sMethodName 関数名
 * @param sClassName  クラス名
 * @param sDetail     エラー詳細
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
 * コンストラクタ(エラー情報を設定する)
 * @param iCode       エラーコード
 * @param iLine       行数
 * @param sMethodName 関数名
 * @param sClassName  クラス名
 * @param sDetail     エラー詳細
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
 * エラー番号を出力する
 * @author kobayashi
 * @since 2015/6/6
 * @version 1.0
 */
int CAbcException::iGetErrCode()
{
	return iErrCode;
}

/**
 * エラーを起こした行数を出力する
 * @author kobayashi
 * @since 2015/6/6
 * @version 1.0
 */
int CAbcException::iGetErrLine()
{
	return iErrLine;
}

/**
 * エラーを起こした関数名を出力する
 * @author kobayashi
 * @since 2015/6/6
 * @version 1.0
 */
std::string CAbcException::strGetMethodName()
{
	return strMethodName;
}

/**
 * エラーを起こしたクラスを出力する
 * @author kobayashi
 * @since 2015/6/6
 * @version 1.0
 */
std::string CAbcException::strGetClassName()
{
	return strClassName;
}

/**
 * エラーの詳細情報を出力する
 * @author kobayashi
 * @since 2015/6/6
 * @version 1.0
 */
std::string CAbcException::strGetErrDetail()
{
	return strErrDetail;
}
