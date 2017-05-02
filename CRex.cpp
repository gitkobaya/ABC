
#include<cstdio>
#include<cstdlib>
#include<cmath>
#include<climits>
#include<cfloat>
#include<cstring>
#include<memory>
#include<algorithm>

#include"mseq.h"
#include"cdfrand.h"
#include"CRealCodedGa.h"
#include"CRex.h"

CRex::CRex()
{
	iParentOutputFlag = 0;
	iChildrenOutputFlag = 0;
	iParentNumber = 0;
	iChildrenNumber = 0;
	pplfChildren = NULL;
	piParentLoc = NULL;
	iDistanceFlag = 2;
	lfAlpha = 1.0;
}

CRex::~CRex()
{
///	vTerminate();
}

void CRex::vInitialize( int iGenerationNum, int iGenNum, int iGenVectorData, int iParentNumberData, int iChildrenNumberData )
{
	int i;
	CRexException cre;

	// 親の生成数を設定します。
	iParentNumber = iParentNumberData > iGenNum ? iGenNum : iParentNumberData;

	// 子供の生成数を設定します。
	iChildrenNumber = iChildrenNumberData;

	// 実数値GAの初期化を実行します。
	CRealCodedGa::vInitialize( iGenerationNum, iGenNum, iGenVectorData );

	try
	{
		// 親選択用配列です。
		piParentLoc = new int[iGenNum];
		if( piParentLoc == NULL )
		{
			cre.SetErrorInfo( REX_MEMORY_ALLOCATE_ERROR, "vInitialize", "CRex", "メモリ確保に失敗しました。", __LINE__ );
			throw( cre );
		}
		// 子供のデータを作成します。
		pplfChildren = new double*[iChildrenNumber];
		if( pplfChildren == NULL )
		{
			cre.SetErrorInfo( REX_MEMORY_ALLOCATE_ERROR, "vInitialize", "CRex", "メモリ確保に失敗しました。", __LINE__ );
			throw( cre );
		}
		for( i = 0;i < iChildrenNumber; i++ )
		{
			pplfChildren[i] = new double[iGenVector];
			if( pplfChildren[i] == NULL )
			{
				cre.SetErrorInfo( REX_MEMORY_ALLOCATE_ERROR, "vInitialize", "CRex", "メモリ確保に失敗しました。", __LINE__ );
				throw( cre );
			}
		}
		// REX計算用ベクトルデータ一時保存変数です。
		plfTempVector = new double[iGenVector];
		if( plfTempVector == NULL )
		{
			cre.SetErrorInfo( REX_MEMORY_ALLOCATE_ERROR, "vInitialize", "CRex", "メモリ確保に失敗しました。", __LINE__ );
			throw( cre );
		}
		// 重心の計算結果を保持します。
		plfCentroid = new double[iGenVector];
		if( plfCentroid == NULL )
		{
			cre.SetErrorInfo( REX_MEMORY_ALLOCATE_ERROR, "vInitialize", "CRex", "メモリ確保に失敗しました。", __LINE__ );
			throw( cre );
		}
		plfChildVector = new double[iGenVector];
		if( plfChildVector == NULL )
		{
			cre.SetErrorInfo( REX_MEMORY_ALLOCATE_ERROR, "vInitialize", "CRex", "メモリ確保に失敗しました。", __LINE__ );
			throw( cre );
		}
		plfNormalizeRand = new double[iParentNumber];
		if( plfNormalizeRand == NULL )
		{
			cre.SetErrorInfo( REX_MEMORY_ALLOCATE_ERROR, "vInitialize", "CRex", "メモリ確保に失敗しました。", __LINE__ );
			throw( cre );
		}
		plfChildrenCentroid = new double[iGenVector];
		if( plfChildrenCentroid == NULL )
		{
			cre.SetErrorInfo( REX_MEMORY_ALLOCATE_ERROR, "vInitialize", "CRex", "メモリ確保に失敗しました。", __LINE__ );
			throw( cre );
		}
		plfUpperEvalChildrenCentroid = new double[iGenVector];
		if( plfUpperEvalChildrenCentroid == NULL )
		{
			cre.SetErrorInfo( REX_MEMORY_ALLOCATE_ERROR, "vInitialize", "CRex", "メモリ確保に失敗しました。", __LINE__ );
			throw( cre );
		}
		plfCentroidSteep = new double[iGenVector];
		if( plfCentroidSteep == NULL )
		{
			cre.SetErrorInfo( REX_MEMORY_ALLOCATE_ERROR, "vInitialize", "CRex", "メモリ確保に失敗しました。", __LINE__ );
			throw( cre );
		}

		for( i = 0;i < iChildrenNumber; i++ )
		{
			memset( pplfChildren[i], 0, iGenVector*sizeof(double) );
		}
		for( i = 0;i < iGenVector; i++ )
		{
			plfCentroid[i] = 0.0;
			plfChildVector[i] = 0.0;
			plfTempVector[i] = 0.0;
			plfUpperEvalChildrenCentroid[i] = 0.0;
			plfChildrenCentroid[i] = 0.0;
			plfCentroidSteep[i] = 0.0;
		}
		for( i = 0;i < iParentNumber; i++ )
		{
			plfNormalizeRand[i] = 0.0;
			piParentLoc[i] = i;
		}
		for( i = 0;i < iGenNum; i++ )
		{
			piParentLoc[i] = i;
		}

		initrand( (unsigned long)time(NULL) );
		initrnd();
	}
	catch( std::bad_alloc ba )
	{
		cre.SetErrorInfo( REX_MEMORY_ALLOCATE_ERROR, "vInitialize", "CRex", "メモリ確保に失敗しました。", __LINE__ );
		throw( cre );
	}
	catch( ... )
	{
		cre.SetErrorInfo( REX_ARRAY_INDEX_ERROR, "vInitialize", "CRex", "配列範囲外を参照しました。", __LINE__ );
		throw( cre );
	}
}

void CRex::vInitialize( int iGenerationNum, int iGenNum, int iGenVectorData, int iParentNumberData, int iChildrenNumberData, double lfLearningRateData,int iUpperEvalChildrenNumData )
{
	int i;
	CRexException cre;

	// 親の生成数を設定します。
	iParentNumber = iParentNumberData > iGenNum ? iGenNum : iParentNumberData;

	// 子供の生成数を設定します。
	iChildrenNumber = iChildrenNumberData;

	// 実数値GAの初期化を実行します。
	CRealCodedGa::vInitialize( iGenerationNum, iGenNum, iGenVectorData );

	// 学習率の設定をします。
	lfLearningRate = lfLearningRateData;

	// 交叉後の評価値上位の子供の数の閾値を設定します。
	iUpperEvalChildrenNumber = iUpperEvalChildrenNumData > iChildrenNumber ? iChildrenNumber : iUpperEvalChildrenNumData;

	lfAlpha = 1.0;
	
	try
	{
		// 親選択用配列です。
		piParentLoc = new int[iGenNum];
		if( piParentLoc == NULL )
		{
			cre.SetErrorInfo( REX_MEMORY_ALLOCATE_ERROR, "vInitialize", "CRex", "メモリ確保に失敗しました。", __LINE__ );
			throw( cre );
		}
		// 子供のデータを作成します。
		pplfChildren = new double*[iChildrenNumber];
		if( pplfChildren == NULL )
		{
			cre.SetErrorInfo( REX_MEMORY_ALLOCATE_ERROR, "vInitialize", "CRex", "メモリ確保に失敗しました。", __LINE__ );
			throw( cre );
		}
		for( i = 0;i < iChildrenNumber; i++ )
		{
			pplfChildren[i] = new double[iGenVector];
			if( pplfChildren[i] == NULL )
			{
				cre.SetErrorInfo( REX_MEMORY_ALLOCATE_ERROR, "vInitialize", "CRex", "メモリ確保に失敗しました。", __LINE__ );
				throw( cre );
			}
		}
		pplfNormalizeRand = new double*[iChildrenNumber];
		if( pplfNormalizeRand == NULL )
		{
			cre.SetErrorInfo( REX_MEMORY_ALLOCATE_ERROR, "vInitialize", "CRex", "メモリ確保に失敗しました。", __LINE__ );
			throw( cre );
		}
		for( i = 0;i < iChildrenNumber; i++ )
		{
			pplfNormalizeRand[i] = new double[iParentNumber];
			if( pplfNormalizeRand[i] == NULL )
			{
				cre.SetErrorInfo( REX_MEMORY_ALLOCATE_ERROR, "vInitialize", "CRex", "メモリ確保に失敗しました。", __LINE__ );
				throw( cre );
			}
		}		
		plfTempVector = new double[iGenVector];
		if( plfTempVector == NULL )
		{
			cre.SetErrorInfo( REX_MEMORY_ALLOCATE_ERROR, "vInitialize", "CRex", "メモリ確保に失敗しました。", __LINE__ );
			throw( cre );
		}
		plfCentroid = new double[iGenVector];
		if( plfCentroid == NULL )
		{
			cre.SetErrorInfo( REX_MEMORY_ALLOCATE_ERROR, "vInitialize", "CRex", "メモリ確保に失敗しました。", __LINE__ );
			throw( cre );
		}
		plfChildVector = new double[iGenVector];
		if( plfChildVector == NULL )
		{
			cre.SetErrorInfo( REX_MEMORY_ALLOCATE_ERROR, "vInitialize", "CRex", "メモリ確保に失敗しました。", __LINE__ );
			throw( cre );
		}
		plfNormalizeRand = new double[iParentNumber];
		if( plfNormalizeRand == NULL )
		{
			cre.SetErrorInfo( REX_MEMORY_ALLOCATE_ERROR, "vInitialize", "CRex", "メモリ確保に失敗しました。", __LINE__ );
			throw( cre );
		}
		plfChildrenCentroid = new double[iGenVector];
		if( plfChildrenCentroid == NULL )
		{
			cre.SetErrorInfo( REX_MEMORY_ALLOCATE_ERROR, "vInitialize", "CRex", "メモリ確保に失敗しました。", __LINE__ );
			throw( cre );
		}
		plfUpperEvalChildrenCentroid = new double[iGenVector];
		if( plfUpperEvalChildrenCentroid == NULL )
		{
			cre.SetErrorInfo( REX_MEMORY_ALLOCATE_ERROR, "vInitialize", "CRex", "メモリ確保に失敗しました。", __LINE__ );
			throw( cre );
		}
		plfCentroidSteep = new double[iGenVector];
		if( plfCentroidSteep == NULL )
		{
			cre.SetErrorInfo( REX_MEMORY_ALLOCATE_ERROR, "vInitialize", "CRex", "メモリ確保に失敗しました。", __LINE__ );
			throw( cre );
		}


		for( i = 0;i < iChildrenNumber; i++ )
		{
			memset( pplfChildren[i], 0, iGenVector*sizeof(double) );
		}
		for( i = 0;i < iGenVector; i++ )
		{
			plfCentroid[i] = 0.0;
			plfChildVector[i] = 0.0;
			plfTempVector[i] = 0.0;
			plfUpperEvalChildrenCentroid[i] = 0.0;
			plfChildrenCentroid[i] = 0.0;
			plfCentroidSteep[i] = 0.0;
		}
		for( i = 0;i < iParentNumber; i++ )
		{
			plfNormalizeRand[i] = 0.0;
		}
		for( i = 0;i < iGenNum; i++ )
		{
			piParentLoc[i] = i;
		}

		for( i = 0;i < iChildrenNumber; i++ )
		{
			memset( pplfNormalizeRand[i], 0, iParentNumber*sizeof(double) );
		}

		initrand( (unsigned long)time(NULL) );
		initrnd();
	}
	catch( std::bad_alloc ba )
	{
		cre.SetErrorInfo( REX_MEMORY_ALLOCATE_ERROR, "vInitialize", "CRex", "メモリ確保に失敗しました。", __LINE__ );
		throw( cre );
	}
	catch( ... )
	{
		cre.SetErrorInfo( REX_ARRAY_INDEX_ERROR, "vInitialize", "CRex", "配列範囲外を参照しました。", __LINE__ );
		throw( cre );
	}
}

void CRex::vTerminate()
{
	int i;
	CRexException cre;

	try
	{
		// 継承元クラスに属するメンバ変数の終了処理を実行します。
		CRealCodedGa::vTerminate();

		if( piParentLoc != NULL )
		{
			delete[] piParentLoc;
			piParentLoc = NULL;
		}
		
		if( pplfChildren != NULL )
		{
			for( i = 0;i < iChildrenNumber; i++ )
			{
				if( pplfChildren[i] != NULL )
				{
					delete[] pplfChildren[i];
					pplfChildren[i] = NULL;
				}
			}
			delete[] pplfChildren;
			pplfChildren = NULL;
		}
		if( pplfNormalizeRand != NULL )
		{
			for( i = 0;i < iChildrenNumber; i++ )
			{
				if( pplfNormalizeRand[i] != NULL )
				{
					delete[] pplfNormalizeRand[i];
					pplfNormalizeRand[i] = NULL;
				}
			}
			delete[] pplfNormalizeRand;
			pplfNormalizeRand = NULL;
		}
		if( plfTempVector != NULL )
		{
			delete[] plfTempVector;
			plfTempVector = NULL;
		}
		if( plfCentroid != NULL )
		{
			delete[] plfCentroid;
			plfCentroid = NULL;
		}
		if( plfChildVector != NULL )
		{
			delete[] plfChildVector;
			plfChildVector = NULL;
		}
		if( plfNormalizeRand != NULL )
		{
			delete[] plfNormalizeRand;
			plfNormalizeRand = NULL;
		}
		if( plfChildrenCentroid != NULL )
		{
			delete[] plfChildrenCentroid;
			plfChildrenCentroid = NULL;
		}
		if( plfUpperEvalChildrenCentroid != NULL )
		{
			delete[] plfUpperEvalChildrenCentroid;
			plfUpperEvalChildrenCentroid = NULL;
		}
		if( plfCentroidSteep != NULL )
		{
			delete[] plfCentroidSteep;
			plfCentroidSteep = NULL;
		}
	}
	catch( ... )
	{
		cre.SetErrorInfo( REX_MEMORY_RELEASE_ERROR, "vTerminate", "CUndx", "確保したメモリの解放に失敗しました。", __LINE__ );
		throw( cre );
	}
}

void CRex::vSelectParent()
{
	int i, j, k;
	double lfSumAbs1, lfSumAbs2, lfSumAbs3;
	int i1, i2, i3;
	int iLoc;
	int iTemp;
	int iFlag = 0;
	// REXを行う親を2つ決定します。
	// 親をランダムにNp個選択します。
	for (i = iGenNumber - 1; i > 0; i--)
	{
		iLoc = (int)((i + 1)*rnd());
		iTemp = piParentLoc[i];
		piParentLoc[i] = piParentLoc[iLoc];
		piParentLoc[iLoc] = iTemp;
	}
	for (i = 0; i < iGenNumber; i++)
	{
		for (j = i; j > 0; j--)
		{
			// 一致するかどうかを算出。
			lfSumAbs1 = 0.0;
			for (k = 0; k < iGenVector; k++)
			{
				lfSumAbs1 += fabs(pplfGens[piParentLoc[i]][k] - pplfGens[piParentLoc[j]][k]);
			}
			// 一致する場合は次の配列番号の値と交換する。
			if (lfSumAbs1 <= 0.0000000000001)
			{
				if (i < iGenNumber - 1)
				{
					iTemp = piParentLoc[i];
					piParentLoc[i + 1] = piParentLoc[i];
					piParentLoc[i] = iTemp;
				}
			}
		}
	}
}

void CRex::vRex()
{
	CRexException cre;
	int i,j,k;
	int iLoc;
	int iTemp = 0;
	double lfSigma = 0.0;
	std::vector<Rank_t> stlFitProb;
	Rank_t tTempRankData;
	
/* JGGモデル */

	try
	{
		// 親をランダムにNp個選択します。
//		for( i = iGenNumber-1; i > 0 ; i-- )
//		{
//			iLoc = (int)((i+1)*rnd());
//			iTemp = piParentLoc[i];
//			piParentLoc[i] = piParentLoc[iLoc];
//			piParentLoc[iLoc] = iTemp;
//		}

		vSelectParent();

		// 重心を算出します。
		for( j = 0;j < iGenVector; j++ )
		{
			plfCentroid[j] = 0.0;
			for( i = 0;i < iParentNumber; i++ )
				plfCentroid[j] += pplfGens[piParentLoc[i]][j];
			plfCentroid[j] /= (double)iParentNumber;
		}
	// REX(RealCoded Emsanble )を実行します。交叉回数Nc回実行し、Nc個の子供を生成します。
		// 統計量遺伝における普遍分散を算出します。
//		lfSigma = 1.0/(double)sqrt( (double)iParentNumber );
		lfSigma = sqrt(3.0/(double)iParentNumber);

		for( i = 0;i < iChildrenNumber; i++ )
		{
			for( j = 0;j < iParentNumber; j++ )
			{
//				plfNormalizeRand[j] = grand(lfSigma, 0.0);
				plfNormalizeRand[j] = lfSigma*(2.0*rnd()-1.0);
			}
			for( k = 0;k < iGenVector; k++ )
			{
			// REXを実行して、子供を生成します。
				// 正規乱数により乱数を発生させます。
				plfTempVector[k] = 0.0;
				for( j = 0;j < iParentNumber; j++ )
					plfTempVector[k] += plfNormalizeRand[j] * ( pplfGens[piParentLoc[j]][k] - plfCentroid[k] );
				pplfChildren[i][k] = plfCentroid[k] + plfTempVector[k];
			}
			// 評価値をNp個算出します。
			tTempRankData.lfFitProb = pflfConstraintFunction( pplfChildren[i], iGenVector );
			tTempRankData.iLoc = i;
			stlFitProb.push_back( tTempRankData );
		}

		// 目的関数値によるソートを実施します。
		std::sort( stlFitProb.begin(), stlFitProb.end(), CCompareToRank() );

		// 親を入れ替えます。(JGGモデルの場合は親はすべて変更するものとします。)
		for( i = 0; i < iParentNumber; i++ )
			for( j = 0;j < iGenVector; j++ )
				pplfGens[piParentLoc[i]][j] = pplfChildren[stlFitProb[i].iLoc][j];
		// 現在の最良位置を取得します。
		iBestLoc = piParentLoc[0];
	}
	catch(...)
	{
		cre.SetErrorInfo( REX_ARRAY_INDEX_ERROR, "vImplement", "CRex", "配列範囲外参照をしました。", __LINE__ );
		throw( cre );
	}
//	stlSelectParentLoc.clear();
}

void CRex::vRexStar()
{
	CRexException cre;
	int i,j,k;
	int iLoc;
	int iMaxSize = 0;
	int iOverLapLoc = 0;
	double lfSigma = 0.0;
	std::vector<Rank_t> stlFitProb;
	Rank_t tTempRankData;
	std::vector<int> stlSelectParentLoc;
	
/* JGGモデル */

	try
	{
		// 親をランダムにNp個選択します。
		for(;;)
		{
			iLoc = mrand() % iGenNumber;
			// 選択した親と重なっていないか調査します。
			iOverLapLoc = -1;
			for( i = 0;i < (unsigned int)stlSelectParentLoc.size(); i++ )
			{
				if( stlSelectParentLoc.at(i) == iLoc )
				{
					iOverLapLoc = i;
					break;
				}
			}
			// 重なっていなければ、位置を追加します。
			if( iOverLapLoc == -1 )
			{
				stlSelectParentLoc.push_back( iLoc );
				iMaxSize++;
			}
			// 指定した親の数になったら終了します。
			if( iMaxSize == iParentNumber ) break;
		}
		// 重心を算出します。
		for( i = 0;i < iGenVector; i++ )
		{
			plfCentroid[i] = 0.0;
		}
		for( i = 0;i < iParentNumber; i++ )
		{
			for( j = 0;j < iGenVector; j++ )
			{
				plfCentroid[j] += ( pplfGens[stlSelectParentLoc.at(i)][j] );
			}
		}
		for( i = 0;i < iParentNumber; i++ )
		{
			plfCentroid[j] /= (double)iParentNumber;
		}
	// REX(RealCoded Emsanble )を実行します。交叉回数Nc回実行し、Nc個の子供を生成します。
		// 統計量遺伝における普遍分散を算出します。
		lfSigma = 1.0/(double)sqrt( (double)iParentNumber );
		
		for( i = 0;i < iChildrenNumber; i++ )
		{
			// 正規乱数により乱数を発生させます。
			for( j = 0;j < iParentNumber; j++ )
			{
				plfNormalizeRand[j] = grand(lfSigma, 0.0);
			}
			for( j = 0;j < iGenVector; j++ )
			{
				plfTempVector[j] = 0.0;
				plfChildVector[j] = 0.0;
			}
			for( j = 0;j < iParentNumber; j++ )
			{
			// REXを実行して、子供を生成します。
				for( k = 0;k < iGenVector; k++ )
				{
					plfTempVector[k] += plfNormalizeRand[j] * ( pplfGens[stlSelectParentLoc.at(j)][k] - plfCentroid[k] );
				}
			}
			for( k = 0;k < iGenVector; k++ )
			{
				plfChildVector[k] = plfCentroid[k] + plfTempVector[k];
			}
			for( j = 0;j < iGenVector; j++ )
			{
				pplfChildren[i][j] = plfChildVector[j];
			}
		}
		
		// 評価値をNp個算出します。
		for( i = 0;i < iChildrenNumber; i++ )
		{
			tTempRankData.lfFitProb = pflfConstraintFunction( pplfChildren[i], iGenVector );
			tTempRankData.iLoc = i;
			stlFitProb.push_back( tTempRankData );
		}
		// 目的関数値によるソートを実施します。
		std::sort( stlFitProb.begin(), stlFitProb.end(), CCompareToRank() );
		// 親を入れ替えます。(JGGモデルの場合は親はすべて変更するものとします。)
		for( i = 0; i < iParentNumber; i++ )
		{
			for( j = 0;j < iGenVector; j++ )
			{
				pplfGens[stlSelectParentLoc.at(i)][j] = pplfChildren[stlFitProb[i].iLoc][j];
			}
		}
	}
	catch(...)
	{
		cre.SetErrorInfo( REX_ARRAY_INDEX_ERROR, "vImplement", "CRex", "配列範囲外参照をしました。", __LINE__ );
		throw( cre );
	}
	stlSelectParentLoc.clear();
}

/**
 * <PRE>
 *    適応的実数値交叉AREXを実行します。
 * </PRE>
 * @author kobayashi
 * @since 2016/6/10
 * @version 1.0
 */
void CRex::vARex()
{
	CRexException cre;
	int i,j,k;
	int iLoc;
	int iMaxSize = 0;
	int iOverLapLoc = 0;
	int iTemp = 0;
	double lfSigma = 0.0;
	double l;
	double lAvgDist = 0.0;
	double lfTemp = 0.0;
	std::vector<Rank_t> stlFitProb;
	std::vector<Rank_t> stlParentFitProb;
	Rank_t tTempRankData;
	std::vector<int> stlSelectParentLoc;

	int i1stGenLoc = 0;
	int i2ndGenLoc = 0;
	
/* JGGモデル */

	try
	{
		// 親をランダムにNp個選択します。
		for( i = iGenNumber-1; i >= 0 ; i-- )
		{
			iLoc = (int)((i+1)*rnd());
			iTemp = piParentLoc[i];
			piParentLoc[i] = piParentLoc[iLoc];
			piParentLoc[iLoc] = iTemp;
			if( i <= iParentNumber )
			{
				// 親データを適応度でソートするため、データを代入します。
				tTempRankData.lfFitProb = pflfConstraintFunction( pplfGens[piParentLoc[i]], iGenVector );
				tTempRankData.iLoc = piParentLoc[i];
				stlParentFitProb.push_back( tTempRankData );
			}
		}
	
		std::sort( stlParentFitProb.begin(), stlParentFitProb.end(), CCompareToRank() );
	
		// 重心及び交叉中心降下を算出します。
		for( j = 0;j < iGenVector; j++ )
		{
			plfCentroid[j] = 0.0;
			plfCentroidSteep[j] = 0.0;
			for( i = 0;i < iParentNumber; i++ )
			{
				plfCentroid[j] += pplfGens[stlParentFitProb.at(i).iLoc][j];
				plfCentroidSteep[j] += 2.0*(double)(iParentNumber+1-(i+1))*pplfGens[stlParentFitProb.at(i).iLoc][j];
			}
			plfCentroid[j] /= (double)iParentNumber;
			plfCentroidSteep[j] /= (double)(iParentNumber*(iParentNumber+1));
		}
	// REX(RealCoded Emsanble )を実行します。交叉回数Nc回実行し、Nc個の子供を生成します。
		// 統計量遺伝における普遍分散を算出します。
		lfSigma = 1.0/(double)sqrt( (double)iParentNumber-1 );
//		lfSigma = sqrt(3.0/(double)(iParentNumber));

		for( i = 0;i < iChildrenNumber; i++ )
		{
			// 正規乱数により乱数を発生させます。
			for( j = 0;j < iParentNumber; j++ )
			{
				plfNormalizeRand[j] = grand(lfSigma, 0.0);
//				plfNormalizeRand[j] = lfSigma*(2.0*rnd()-1.0);
				if( iDistanceFlag == 2 ) pplfNormalizeRand[i][j] = plfNormalizeRand[j];
			}
			for( k = 0;k < iGenVector; k++ )
			{
			// REXを実行して、子供を生成します。
				plfTempVector[k] = 0.0;
				for( j = 0;j < iParentNumber; j++ )
					plfTempVector[k] += plfNormalizeRand[j] * ( pplfGens[stlParentFitProb.at(j).iLoc][k] - plfCentroid[k] );
				pplfChildren[i][k] = plfCentroidSteep[k] + lfAlpha*plfTempVector[k];
			}
			// 評価値をNp個算出します。
			tTempRankData.lfFitProb = pflfConstraintFunction( pplfChildren[i], iGenVector );
			tTempRankData.iLoc = i;
			stlFitProb.push_back( tTempRankData );
		}
		
		// 目的関数値によるソートを実施します。
		std::sort( stlFitProb.begin(), stlFitProb.end(), CCompareToRank() );

		// 拡張率適応度を計算します。
		if( iDistanceFlag == 1 )
		{
			// 交叉の中心を求めます。
			// 評価値上位μα個の子供の中心を求めます。
			for( j = 0;j < iGenVector; j++ )
			{
				plfChildrenCentroid[j] = 0.0;
				plfUpperEvalChildrenCentroid[j] = 0.0;
				for( i = 0;i < iChildrenNumber; i++ )
				{
					plfChildrenCentroid[j] += pplfChildren[stlFitProb.at(i).iLoc][j];
					plfUpperEvalChildrenCentroid[j] += pplfChildren[stlFitProb.at(i).iLoc][j];
				}
				plfChildrenCentroid[j] /= (double)(iChildrenNumber);
				plfUpperEvalChildrenCentroid[j] /= (double)iUpperEvalChildrenNumber;
			}
			vAerEuclide( stlParentFitProb );
		}
		else if( iDistanceFlag == 2 )
		{
			vAerMahalanobis( stlFitProb );
		}
#if 1
		// 親を入れ替えます。(JGGモデルの場合は親はすべて変更するものとします。)
		for( i = 0; i < iParentNumber; i++ )
			for( j = 0;j < iGenVector; j++ )
				pplfGens[stlParentFitProb.at(i).iLoc][j] = pplfChildren[stlFitProb.at(i).iLoc][j];
		// 現在の最良位置を取得します。
#else
		vSelectGens( pplfChildren, &i1stGenLoc, &i2ndGenLoc );
		for( j = 0;j < iGenVector; j++ )
		{
			pplfGens[stlParentFitProb.at(0).iLoc][j] = pplfChildren[i1stGenLoc][j];
			pplfGens[stlParentFitProb.at(1).iLoc][j] = pplfChildren[i2ndGenLoc][j];
		}
#endif
		iBestLoc = stlParentFitProb.at(0).iLoc;
	}
	catch(...)
	{
		cre.SetErrorInfo( REX_ARRAY_INDEX_ERROR, "vImplement", "CRex", "配列範囲外参照をしました。", __LINE__ );
		throw( cre );
	}
	stlParentFitProb.clear();
	stlFitProb.clear();
}

/**
 * <PRE>
 * 　拡張率適応度をユークリッド距離により計算します。
 * </PRE>
 * @author kobayashi
 * @since 2016/8/25
 * @version 0.1
 */
void CRex::vAerEuclide( const std::vector<Rank_t>& stlParentFitProb )
{
	int i,j;
	double lfTemp;
	double lfSigma;
	double lfNorm = 0.0;

	// Ldcpを算出します。
	lfSigma = 1.0/(double)sqrt( (double)iParentNumber-1 );
	lfLcdp = 0.0;
	for( i = 0;i < iGenVector; i++ )
	{
		lfTemp = plfUpperEvalChildrenCentroid[i] - plfChildrenCentroid[i];
		lfLcdp += lfTemp*lfTemp;
	}

	// Lavgを算出します。
	lfLavg = 0.0;
	for( i = 0;i < iParentNumber; i++ )
	{
		lfNorm = 0.0;
		for( j = 0;j < iGenVector; j++ )
		{
			lfTemp = pplfGens[stlParentFitProb.at(i).iLoc][j] - plfCentroid[j];
			lfNorm += lfTemp*lfTemp;
		}
		lfLavg += lfNorm;
	}
	lfLavg = lfAlpha*lfAlpha*lfSigma*lfSigma/(double)iUpperEvalChildrenNumber;

	// αの更新を行います。
	lfTemp = lfAlpha * sqrt( (1.0-lfLearningRate)+lfLearningRate*lfLcdp/lfLavg );
	lfAlpha = lfTemp < 1.0 ? 1.0 : lfTemp;
}

/**
 * <PRE>
 * 　拡張率適応度をユークリッド距離により計算します。
 * </PRE>
 * @author kobayashi
 * @since 2016/8/25
 * @version 0.1
 */
void CRex::vAerMahalanobis( const std::vector<Rank_t>& stlFitProb )
{
	int i,j;
	double lfTemp;
	double lfSigma;
	double lfRandAvg = 0.0;
	double lfRandAvgSumSquare = 0.0;
	double lfRandAvgSumSquareAvg = 0.0;

	// Ldcpを算出します。
	lfSigma = 1.0/(double)sqrt( (double)iParentNumber-1 );
//	lfSigma = sqrt( 3.0/(double)iParentNumber );
	lfLcdp = 0.0;
	for( i = 0;i < iParentNumber; i++ )
	{
		lfRandAvg = 0.0;
		for( j = 0;j < iUpperEvalChildrenNumber; j++ )
		{
			lfRandAvg += pplfNormalizeRand[stlFitProb.at(j).iLoc][i];
		}
		lfRandAvg /= (double)iUpperEvalChildrenNumber;
		lfRandAvgSumSquare += lfRandAvg*lfRandAvg;
		lfRandAvgSumSquareAvg += lfRandAvg;
	}
	lfRandAvgSumSquareAvg = lfRandAvgSumSquareAvg*lfRandAvgSumSquareAvg/(double)iParentNumber;
	lfLcdp = lfAlpha*lfAlpha*(iParentNumber-1)*(lfRandAvgSumSquare-lfRandAvgSumSquareAvg);

	// Lavgを算出します。
	lfLavg = lfAlpha*lfAlpha*lfSigma*lfSigma*(iParentNumber-1)*(iParentNumber-1)/(double)iUpperEvalChildrenNumber;

	// αの更新を行います。
	lfTemp = lfAlpha * sqrt( (1.0-lfLearningRate)+lfLearningRate*lfLcdp/lfLavg );
	lfAlpha = lfTemp < 1.0 ? 1.0 : lfTemp;
//	printf("α = %lf, Lavg= %lf\n", lfAlpha, lfLavg);
}

void CRex::vSelectGens( double **pplfChildren, int *pi1stGenLoc, int *pi2ndGenLoc )
{
	unsigned int i;
	double lfProb = 0.0;
	double lfPrevProb = 0.0;
	double lfRes = 0.0;
	double lf1stGen = DBL_MAX;
	double lfRand = 0.0;
	int i1stGenLoc = INT_MAX;
	int i2ndGenLoc = INT_MAX;
	int iRank = 0;
	std::vector<Rank_t> stlFitProb;
	Rank_t tTempRankData;
	// まず、適応度関数の値を計算します。
	lfRes = 0.0;
	for( i = 0;i < (unsigned int)iChildrenNumber; i++ )
	{
		tTempRankData.lfFitProb = pflfConstraintFunction( pplfChildren[i], iGenVector );
		tTempRankData.iLoc = i;
		stlFitProb.push_back( tTempRankData );
		lfRes += stlFitProb[i].lfFitProb;
		if( stlFitProb[i].lfFitProb < lf1stGen )
		{
			lf1stGen = stlFitProb[i].lfFitProb;
			i1stGenLoc = i;
		}
	}
	// 目的関数値によるソートを実施します。
	std::sort( stlFitProb.begin(), stlFitProb.end(), CCompareToRank() );
	// ランクに基づくルーレット選択を実行。
	iRank = mrand() % ( iChildrenNumber-1 ) + 1;
	i2ndGenLoc = stlFitProb[iRank].iLoc;
	// 最良個体の位置とそれ以外でルーレット選択により選ばれた位置を返却します。
	*pi1stGenLoc = i1stGenLoc;
	*pi2ndGenLoc = i2ndGenLoc;
}

/**
 * <PRE>
 * 　現時点での選択した親を出力します。
 * </PRE>
 * @author kobayashi
 * @since 2015/12/16
 * @version 0.1
 */
void CRex::vOutputCurrentParent( std::vector<int> stlSelectParentLocData )
{
	int i,j;
	double lfRes = 0.0;
	double lfAvgDist = 0.0;
	double lfDist = 0.0;
	double lfDist2 = 0.0;
	if( iParentOutputFlag == 1 )
	{
		for(i = 0;i < (unsigned int)stlSelectParentLocData.size(); i++ )
		{
			// 現時点で選択した第一親を出力します。
			for( j = 0;j < iGenVector; j++ )
			{
				printf( "%lf," ,pplfGens[stlSelectParentLocData.at(i)][j] );
			}
			printf("\n");
		}
	}
}

/**
 * <PRE>
 * 　現時点での生成した子を出力します。
 * </PRE>
 * @author kobayashi
 * @since 2015/12/16
 * @version 0.1
 */
void CRex::vOutputCurrentChildren( double** pplfChildrenData )
{
	int i,j;

	if( iChildrenOutputFlag == 1 )
	{
		for( i= 0; i < iChildrenNumber; i++ )
		{
			for( j = 0;j < iGenVector; j++ )
			{
				printf("%lf,",pplfChildrenData[i][j]);
			}
			printf("\n");
		}
	}
}

/**
 * <PRE>
 * 　遺伝子に値を設定します。
 *   ver 0.1 初版 
 * </PRE>
 * @param pplfGenData 
 * @author kobayashi
 * @since 2016/08/26
 * @version 0.1
 */
void CRex::vSetGenData( double** pplfGenData )
{
	int i,j;
	for( i= 0; i < iGenNumber; i++ )
	{
		for( j = 0;j < iGenVector; j++ )
		{
			pplfGens[i][j] = pplfGenData[i][j];
		}
	}
}

/**
 * <PRE>
 * 　現在の遺伝子データを取得します。
 *   ver 0.1 初版 
 * </PRE>
 * @param pplfGenData 
 * @author kobayashi
 * @since 2016/08/26
 * @version 0.1
 */
void CRex::vGetGenData( double** pplfGenData )
{
	int i,j;
	for( i= 0; i < iGenNumber; i++ )
	{
		for( j = 0;j < iGenVector; j++ )
		{
			pplfGenData[i][j] = pplfGens[i][j];
		}
	}
}

/**
 * <PRE>
 * 　現在の遺伝子データを取得します。
 *   ver 0.1 初版 
 * </PRE>
 * @param pplfGenData 
 * @author kobayashi
 * @since 2016/09/14
 * @version 0.1
 */
void CRex::vGetBestGenData( double* plfGenData )
{
	int j;
	for( j = 0;j < iGenVector; j++ )
	{
		plfGenData[j] = pplfGens[iBestLoc][j];
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////CRexExceptionクラス

CRexException::CRexException()
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
CRexException::CRexException(int iCode, std::string sMethodName, std::string sClassName, std::string sDetail)
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
CRexException::CRexException(int iCode, std::string sMethodName, std::string sClassName, std::string sDetail, int iLine)
{
	iErrCode = iCode;
	iErrLine = iLine;
	strMethodName = sMethodName;
	strClassName = sClassName;
	strErrDetail = sDetail;
}

CRexException::~CRexException()
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
void CRexException::SetErrorInfo(int iCode, std::string sMethodName, std::string sClassName, std::string sDetail)
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
void CRexException::SetErrorInfo(int iCode, std::string sMethodName, std::string sClassName, std::string sDetail, int iLine)
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
int CRexException::iGetErrCode()
{
	return iErrCode;
}

/**
 * エラーを起こした行数を出力する
 * @author kobayashi
 * @since 2015/6/6
 * @version 1.0
 */
int CRexException::iGetErrLine()
{
	return iErrLine;
}

/**
 * エラーを起こした関数名を出力する
 * @author kobayashi
 * @since 2015/6/6
 * @version 1.0
 */
std::string CRexException::strGetMethodName()
{
	return strMethodName;
}

/**
 * エラーを起こしたクラスを出力する
 * @author kobayashi
 * @since 2015/6/6
 * @version 1.0
 */
std::string CRexException::strGetClassName()
{
	return strClassName;
}

/**
 * エラーの詳細情報を出力する
 * @author kobayashi
 * @since 2015/6/6
 * @version 1.0
 */
std::string CRexException::strGetErrDetail()
{
	return strErrDetail;
}
