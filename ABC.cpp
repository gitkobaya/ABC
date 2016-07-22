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
	plfVelocity = NULL;				// ルーレット選択により選択する速度ベクトル
	plfGlobalMaxAbcData = NULL;
	plfGlobalMinAbcData = NULL;			// 大域的最適解を表す粒子のデータ
	plfCrossOverData = NULL;
	plfLocalMaxAbcData = NULL;
	lfSolveRange = 0.0;
	lfGlobalMinAbcData = DBL_MAX;
	lfGlobalMaxAbcData = -DBL_MAX;
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
}

/**
 * <PRE>
 * 　人工蜂コロニー最適化法を実行します。
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

	// employee bee の動作
	// 更新点候補を算出します。
	m = mrand() % ( iAbcSearchNum - 1 );
	h = mrand() % ( iAbcVectorDimNum - 1);
	
	for( i = 0;i < iAbcSearchNum; i++ )
	{
		lfRand = 2*rnd()-1;
		for( j = 0; j < iAbcVectorDimNum; j++ )
			pplfVelocityData[i][j] = pplfAbcData[i][j];
		pplfVelocityData[i][h] = pplfAbcData[i][h] + lfRand*( pplfAbcData[i][h] - pplfAbcData[m][h] );
	}

	// 各探索点と更新しなかった回数を格納する変数を更新します。
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

	// onlookers beeの動作
	for( i = 0;i < iAbcDataNum-iAbcSearchNum;  i++ )
	{
		lfRes = 0.0;
		for(j = 0;j < iAbcSearchNum; j++ )
		{
			// 適応度の算出
			lfObjFunc = pflfObjectiveFunction( pplfAbcData[j], iAbcVectorDimNum );
			if( lfObjFunc >= 0.0 ) lfFitProb = 1.0/( 1.0+lfObjFunc );
			else		       lfFitProb = 1.0+fabs( lfObjFunc );
			lfRes += lfFitProb;
			plfFit[j] = lfFitProb;
		}
		// 適応度の正規化
		for( j = 0;j < iAbcSearchNum; j++ ) plfFitProb[j] = plfFit[j]/lfRes;
		
		// ルーレット戦略を実行
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

		// ルーレット選択した探索点に対して更新候補を求めます。

		// 更新点候補を算出する。
		// 更新点候補を乱数により決定する。
		m = mrand() % ( iAbcSearchNum-1 );
		h = mrand() % ( iAbcVectorDimNum - 1);

		lfRand = 2*rnd()-1;
		for( j = 0; j < iAbcVectorDimNum; j++ ) pplfVelocityData[c][j] = pplfAbcData[c][j];
		pplfVelocityData[c][h] = pplfAbcData[c][h] + lfRand*( pplfAbcData[c][h] - pplfAbcData[m][h] );
		// 更新点候補を次のように更新します。
		lfFunc1 = pflfObjectiveFunction( pplfVelocityData[c], iAbcVectorDimNum );
		lfFunc2 = pflfObjectiveFunction( pplfAbcData[c], iAbcVectorDimNum );

		if( lfFunc1 < lfFunc2 )
		{
			for( j = 0;j < iAbcVectorDimNum; j++ ) pplfAbcData[c][j] = pplfVelocityData[c][j];
			piNonUpdateCount[c] = 0;
		}
		else piNonUpdateCount[c] = piNonUpdateCount[c] + 1;
	}

	// scout bee の実行
	// 新たな探索点を求めて探索を実行します。
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
	// 更新します。
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
 * 　人工蜂コロニー最適化法を実行します。
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

	// employee bee の動作
	// 更新点候補を算出します。
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

	// 各探索点と更新しなかった回数を格納する変数を更新します。
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

	// onlookers beeの動作

	for( i = 0;i < iAbcDataNum-iAbcSearchNum;  i++ )
	{
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

		// ルーレット選択した探索点に対して更新候補を求めます。

		// 更新点候補を算出する。
		// 更新点候補を乱数により決定する。
		m = mrand() % ( iAbcSearchNum-1 );
		h = mrand() % ( iAbcVectorDimNum - 1);

		lfSigma = 1.0/(double)iAbcSearchNum;
//		lfRand = lfNormalRand( lfSigma, 0.0 );
		lfRand = 2.0*rnd()-1.0;
		lfRand2 = 2.0*rnd()-1.0;
		for( j = 0; j < iAbcVectorDimNum; j++ )
			pplfVelocityData[c][j] = pplfAbcData[c][j];
		pplfVelocityData[c][h] = pplfAbcData[c][h] + lfRand*( pplfAbcData[c][h] - pplfAbcData[m][h] ) + lfRand2*( pplfAbcData[c][h] - plfGlobalMinAbcData[h] );
		// 更新点候補を次のように更新します。
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

	// scout bee の実行
	// 新たな探索点を求めて探索を実行します。
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

	// ローカルの最大値を更新します。
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
	// 更新します。
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
 * 　人工蜂コロニー最適化法を実行します。
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

	// employed beesによる探索を実施します。
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
					m = mrand() % ( iAbcSearchNum - 1 - iAbcUpperSearchNum ) + iAbcUpperSearchNum;
				}
				// ランダムに決定します。
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
				m = mrand() % ( iAbcSearchNum - 1 - iAbcUpperSearchNum ) + iAbcUpperSearchNum;
			}
			// ランダムに決定します。
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
			m = mrand() % ( iAbcSearchNum - 1 - iAbcUpperSearchNum ) + iAbcUpperSearchNum;
		}
		// ランダムに決定します。
		h = mrand() % ( iAbcVectorDimNum - 1);

		for( i = 0;i < iAbcSearchNum; i++ )
		{
			lfRand = 2.0*rnd()-1.0;
			for( j = 0; j < iAbcVectorDimNum; j++ )
				pplfVelocityData[i][j] = pplfAbcData[i][j];
			pplfVelocityData[i][h] = pplfAbcData[i][h] + lfRand*( pplfAbcData[i][h] - pplfAbcData[m][h] );
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

	// onlooker beesによる探索

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
		c = mrand() % ( iAbcSearchNum - 1 - iAbcUpperSearchNum ) + iAbcUpperSearchNum;
	}
	// ランダムに決定します。
	h = mrand() % ( iAbcVectorDimNum - 1);

	// 更新点候補を生成します。
	for( i = 0;i < iAbcSearchNum; i++ )
	{
		lfRand = 2*rnd()-1;
		for( j = 0; j < iAbcVectorDimNum; j++ )
			pplfVelocityData[i][j] = pplfAbcData[i][j];
		pplfVelocityData[i][h] = pplfAbcData[i][h] + lfRand*( pplfAbcData[i][h] - pplfAbcData[c][h] );
	}
	// 各探索点を更新します。
	lfFunc1 = pflfObjectiveFunction( pplfVelocityData[c], iAbcVectorDimNum );
	lfFunc2 = pflfObjectiveFunction( pplfAbcData[c], iAbcVectorDimNum );
	if( lfFunc1 < lfFunc2 )
	{
		for( j = 0;j < iAbcVectorDimNum; j++ )
			pplfAbcData[c][j] = pplfVelocityData[c][j];
	}

	// 最良点の更新をします。
	// 初期の状態で最適値を取得します。
	lfMin = DBL_MAX;
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
			lfFitCurrentBest = lfMin;
		}
	}
}

/**
 * <PRE>
 * 　人工蜂コロニー最適化法を実行します。
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
	// employee bee の動作
	// 更新点候補を算出します。
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

	// 各探索点と更新しなかった回数を格納する変数を更新します。
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

	// onlookers beeの動作

	for( i = 0;i < iAbcDataNum-iAbcSearchNum;  i++ )
	{
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

		// ルーレット選択した探索点に対して更新候補を求めます。

		// 更新点候補を算出する。
		// 更新点候補を乱数により決定する。
		m = mrand() % ( iAbcSearchNum-1 );
		h = mrand() % ( iAbcVectorDimNum - 1);

		lfSigma = 1.0/(double)iAbcSearchNum;
//		lfRand = lfNormalRand( lfSigma, 0.0 );
		lfRand = 2.0*rnd()-1.0;
		lfRand2 = 2.0*rnd()-1.0;
		for( j = 0; j < iAbcVectorDimNum; j++ )
			pplfVelocityData[c][j] = pplfAbcData[c][j];
		pplfVelocityData[c][h] = lfWeight*pplfAbcData[c][h] + lfCoe1*lfRand*( pplfAbcData[c][h] - pplfAbcData[m][h] ) + lfCoe2*lfRand2*( pplfAbcData[c][h] - plfGlobalMinAbcData[h] );
		// 更新点候補を次のように更新します。
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

	// scout bee の実行
	// 新たな探索点を求めて探索を実行します。
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

	// ローカルの最大値を更新します。
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
	// 更新します。
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
 * 　人工蜂コロニー最適化法（交叉を導入した手法）を実行します。
 *   A Novel Hybrid Crossover based Artificial Bee Colony Algorithm for Optimization Problem International Journal of Computer Applications 2013より
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

	// employee bee の動作
	// 更新点候補を算出します。
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

	// 各探索点と更新しなかった回数を格納する変数を更新します。
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

	// crossoverを実行します。(もっとも旧式の実数値ＧＡの交叉)
	iParentLoc1 = mrand() % (iAbcSearchNum-1);
	iParentLoc2 = mrand() % (iAbcSearchNum-1);

	for( i = 0;i < iAbcVectorDimNum; i++ )
	{
		plfCrossOverData[i] = 0.5*( pplfAbcData[iParentLoc1][i] + pplfAbcData[iParentLoc2][i] );
//		plfCrossOverData[i] = 1.5*pplfAbcData[iParentLoc1][i] + 0.5*pplfAbcData[iParentLoc2][i];
//		plfCrossOverData[i] = -0.5*pplfAbcData[iParentLoc1][i] + 1.5*pplfAbcData[iParentLoc2][i];
	}
	
	// ランダムに選択した親の中で評価関数の値が悪いものと生成した子供を交換します。
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


	// onlookers beeの動作

	for( i = 0;i < iAbcDataNum-iAbcSearchNum;  i++ )
	{
		lfRes = 0.0;
		for(j = 0;j < iAbcSearchNum; j++ )
		{
			// 適応度の算出
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
		// 適応度の正規化
		for( j = 0;j < iAbcSearchNum; j++ )
		{
			plfFitProb[j] = plfFit[j]/lfRes;
		}
		// ルーレット戦略を実行
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

		// ルーレット選択した探索点に対して更新候補を求めます。

		// 更新点候補を算出する。
		// 更新点候補を乱数により決定する。
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
		// 更新点候補を次のように更新します。
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

	// scout bee の実行
	// 新たな探索点を求めて探索を実行します。
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
	// 更新します。
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
//		if( -1.0 <= lfRand && lfRand <= 1.0 ) break;
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
