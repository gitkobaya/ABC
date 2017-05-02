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
#include"CUndx.h"

CUndx::CUndx()
{
	iCrossOverNum = 0;
	lfAlpha = 0.0;
	lfBeta = 0.0;
	iParentOutputFlag = 0;
	iChildrenOutputFlag = 0;
	iChildrenNumber = 0;
	pplfChildren = NULL;
	plfChild1 = NULL;
	plfChild2 = NULL;
	piParentLoc = NULL;
}

CUndx::~CUndx()
{
}

void CUndx::vInitialize( int iGenerationNum, int iGenNum, int iGenVectorData, int iCrossOverNumData )
{
	int i;
	CUndxException cue;

	// 実数値GAの初期化を実行します。
	CRealCodedGa::vInitialize( iGenerationNum, iGenNum, iGenVectorData );

	// 交叉回数を設定します。
	iCrossOverNum = iCrossOverNumData;

	// 最大で作成される子供の個数を設定します。
	iChildrenNumber = iCrossOverNum * 2 + 2;
	try
	{
		// 子供のデータを作成します。
		pplfChildren = new double*[iChildrenNumber];
		if( pplfChildren == NULL )
		{
			cue.SetErrorInfo( UNDX_MEMORY_ALLOCATE_ERROR, "vInitialize", "CUndx", "メモリ確保に失敗しました。", __LINE__ );
			throw( cue );
		}
		for( i = 0;i < iChildrenNumber; i++ )
		{
			pplfChildren[i] = new double[iGenVector];
			if( pplfChildren[i] == NULL )
			{
				cue.SetErrorInfo( UNDX_MEMORY_ALLOCATE_ERROR, "vInitialize", "CUndx", "メモリ確保に失敗しました。", __LINE__ );
				throw( cue );
			}
		}
		
		for( i = 0;i < iChildrenNumber; i++ )
		{
			memset( pplfChildren[i], 0, iGenVector*sizeof(double) );
		}

		plfChild1 = new double[iGenVector];
		plfChild2 = new double[iGenVector];
		for( i = 0;i < iGenVector; i++ )
		{
			plfChild1[i] = 0.0;
			plfChild2[i] = 0.0;
		}
		piParentLoc = new int[iGenNumber];
		for (i = 0; i < iGenNum; i++)
		{
			piParentLoc[i] = i;
		}
		initrand( (unsigned long)time(NULL) );
		initrnd();
	}
	catch( std::bad_alloc ba )
	{
		cue.SetErrorInfo( UNDX_MEMORY_ALLOCATE_ERROR, "vInitialize", "CUndx", "メモリ確保に失敗しました。", __LINE__ );
		throw( cue );
	}
	catch( ... )
	{
		cue.SetErrorInfo( UNDX_ARRAY_INDEX_ERROR, "vInitialize", "CUndx", "配列範囲外を参照しました。", __LINE__ );
		throw( cue );
	}
}

void CUndx::vTerminate()
{
	int i;
	CUndxException cue;

	try
	{
		// 継承元クラスに属するメンバ変数の終了処理を実行します。
		CRealCodedGa::vTerminate();

		// 終了処理を実行します。
		if (pplfChildren != NULL)
		{
			for (i = 0; i < iChildrenNumber; i++)
			{
				if (pplfChildren[i] != NULL)
				{
					delete[] pplfChildren[i];
					pplfChildren[i] = NULL;
				}
			}
			delete[] pplfChildren;
			pplfChildren = NULL;
		}
		if (plfChild1 != NULL)
		{
			delete[] plfChild1;
			plfChild1 = NULL;
		}
		if (plfChild2 != NULL)
		{
			delete[] plfChild2;
			plfChild2 = NULL;
		}
		if (piParentLoc != NULL)
		{
			delete[] piParentLoc;
			piParentLoc = NULL;
		}
	}
	catch( ... )
	{
		cue.SetErrorInfo( UNDX_MEMORY_RELEASE_ERROR, "vTerminate", "CUndx", "確保したメモリの解放に失敗しました。", __LINE__ );
		throw( cue );
	}
}

void CUndx::vImplement()
{
	CUndxException cundxe;
	int i,j;
	int i1,i2,i3;
	int i1stGenLoc;
	int i2ndGenLoc;
	double *plfParent1 = NULL;
	double *plfParent2 = NULL;
	double *plfParent3 = NULL;

	double lfPrevProb = 0.0;
	double lfProb = 0.0;

	try
	{
		for( i = 0;i < iCrossOverNum; i++ )
		{
			// UNDXの2つの親を選択します。
			vSelectParent( &plfParent1, &plfParent2, &plfParent3, &i1, &i2, &i3 );
			
			// ここで、フラグが有効になっている場合に限り親を出力します。
			vOutputCurrentParent( plfParent1, plfParent2 );
		
			// UNDXを実行します。
			vUndx( plfParent1, plfParent2, plfParent3, lfAlpha, lfBeta, plfChild1, plfChild2 );

			// 生成した子供を追加します。
			for( j = 0;j < iGenVector; j++ )
			{
				pplfChildren[2*i][j] = plfChild1[j];
				pplfChildren[2*i+1][j] = plfChild2[j];
			}
		}
		// 最後に親もこの集団に追加します。
		for( j = 0;j < iGenVector; j++ )
		{
			pplfChildren[2*iCrossOverNum][j] = plfParent1[j];
			pplfChildren[2*iCrossOverNum+1][j] = plfParent2[j];
		}

		// ここで、フラグが有効になっている場合に限り子を出力します。
		vOutputCurrentChildren( pplfChildren );
		
	// 世代交代を実施します。2つのよい遺伝子を選択します

		// 最も評価値のよい遺伝子と、ルーレット選択により決定した遺伝子を選択します。
		vSelectGens( pplfChildren, &i1stGenLoc, &i2ndGenLoc );

		// 今回選択した親と子を交換します。
		for( j = 0;j < iGenVector; j++ )
		{
			pplfGens[i1][j] = pplfChildren[i1stGenLoc][j];
			pplfGens[i2][j] = pplfChildren[i2ndGenLoc][j];
		}
		// 現在の最良値の番号を取得します。
		i1stLoc = i1;
		i2ndLoc = i2;
		// 一時的に保持していた子の集合を削除します。
		for(i = 0;i < iChildrenNumber; i++ )
		{
			memset( pplfChildren[i], 0, iGenVector*sizeof(double) );
		}
	}
	catch( ... )
	{
		cundxe.SetErrorInfo( UNDX_ARRAY_INDEX_ERROR, "vImplement", "CUndx", "配列範囲外参照をしました。", __LINE__ );
		throw( cundxe );
	}
}

void CUndx::vSelectParent( double **pplfParent1, double **pplfParent2, double **pplfParent3, int *piLoc1, int *piLoc2, int *piLoc3 )
{
	int i,j;
	double lfSumAbs1, lfSumAbs2, lfSumAbs3;
	int i1, i2, i3;
	int iLoc;
	int iTemp;
	int iFlag = 0;
	// UNDXを行う親を2つ決定します。
	// 親をランダムにNp個選択します。
	for (i = iGenNumber - 1; i > 0; i--)
	{
		iLoc = (int)((i + 1)*rnd());
		iTemp = piParentLoc[i];
		piParentLoc[i] = piParentLoc[iLoc];
		piParentLoc[iLoc] = iTemp;
	}
	lfSumAbs1 = lfSumAbs2 = lfSumAbs3 = 0.0;
	i1 = piParentLoc[0];
	i2 = piParentLoc[1];
	i3 = piParentLoc[2];
	for (i = 1; i < iGenNumber-1; i++)
	{
		lfSumAbs1 = lfSumAbs2 = lfSumAbs3 = 0.0;
		if( iFlag == 1 )
		{
			i2 = piParentLoc[i - 1];
			i3 = piParentLoc[i];
		}
		else if( iFlag == 2 ) i2 = piParentLoc[i];
		else if (iFlag == 3 ) i3 = piParentLoc[i];
		else if (iFlag == 4 ) i3 = piParentLoc[i];
		else if( iFlag == 5 ) break;

		for (j = 0; j < iGenVector; j++)
		{
			lfSumAbs1 += fabs(pplfGens[i1][j] - pplfGens[i2][j]);
			lfSumAbs2 += fabs(pplfGens[i1][j] - pplfGens[i3][j]);
			lfSumAbs3 += fabs(pplfGens[i2][j] - pplfGens[i3][j]);
		}
		// i1 = i2 = i3の場合（親がすべて等しい場合）
		if (lfSumAbs1 <= 0.000000000001 && lfSumAbs2 <= 0.000000000001) iFlag = 1;
		// i1 = i2の場合（1つ目と2つ目の親が等しい場合）
		else if (lfSumAbs1 <= 0.000000000001) iFlag = 2;
		// i1 = i3の場合（1つ目と3つ目の親が等しい場合）
		else if (lfSumAbs2 <= 0.000000000001) iFlag = 3;
		// i2 = i3の場合（2つ目と3つ目の親が等しい場合）
		else if (lfSumAbs3 <= 0.000000000001) iFlag = 4;
		else iFlag = 5;
	}
	*pplfParent1 = pplfGens[i1];
	*pplfParent2 = pplfGens[i2];
	*pplfParent3 = pplfGens[i3];
	*piLoc1 = i1;
	*piLoc2 = i2;
	*piLoc3 = i3;
}

void CUndx::vUndx( double *plfParent1, double *plfParent2, double *plfParent3, double lfAlpha, double lfBeta, double *plfChild1, double *plfChild2 )
{
	int i;
	std::vector<double> stlUnityVector1,stlMedian;
	std::vector<double> stlTempT1,stlTempT2;
	double lfDistTemp = 0.0;
	double lfProduct = 0.0;
	double lfDist1 = 0.0;
	double lfDist2 = 0.0;
	double lfDist3 = 0.0;
	double lfSub1 = 0.0;
	double lfSub2 = 0.0;
	double lfSigma1 = 0.0;
	double lfSigma2 = 0.0;
	double lfS = 0.0;
	double lfTemp1 = 0.0;
	double lfTemp2 = 0.0;
	double lfTemp3 = 0.0;

	lfDist1 = lfDist2 = lfDistTemp = 0.0;
	for( i = 0; i < iGenVector; i++ )
	{
		// 2つの親の中点を算出します。
		stlMedian.push_back( ( plfParent1[i]+plfParent2[i] )/2.0 );
		stlUnityVector1.push_back( plfParent1[i]-plfParent2[i] );
		// 2つの親の距離を求めます。
		lfSub1 = (plfParent2[i]-plfParent1[i]);
		lfSub2 = (plfParent3[i]-plfParent1[i]);
		lfDist1 += lfSub1*lfSub1;
		lfDist2 += lfSub2*lfSub2;
		// 第3の親と2つの親との距離を求めます。
		lfDistTemp += lfSub1*lfSub2;
		// ここで、z1,z2を生成します。z1=N(0,σ_{1}^2), z2=N(0,σ_{2}^2)なので、これに従って生成します。
//		stlTempT1.push_back(grand(lfSigma1, 0.0));
		stlTempT2.push_back(grand(lfSigma2, 0.0));
	}
	lfDist1 = sqrt( lfDist1 );
	lfDist2 = sqrt( lfDist2 );
	lfDistTemp = lfDist1 > 0.0 && lfDist2 > 0.0 ? lfDistTemp/(lfDist1*lfDist2) : lfDistTemp/100000000.0;
	lfDist3 = lfDist2*sqrt(1.0-lfDistTemp*lfDistTemp);
	lfSigma1 = lfDist1*lfAlpha;
	lfSigma2 = lfDist3*lfBeta/sqrt((double)iGenVector);

#if 0
	for (i = 0; i < iGenVector; i++)
	{
		// ここで、z1,z2を生成します。z1=N(0,σ_{1}^2), z2=N(0,σ_{2}^2)なので、これに従って生成します。
		//		stlTempT1.push_back(grand(lfSigma1, 0.0));
		stlTempT2.push_back(grand(lfSigma2, 0.0));
	}
#endif
	lfProduct = 0.0;
	for( i = 0; i < iGenVector; i++ )
	{
		// 単位ベクトルを作成します。
		stlUnityVector1[i] /= lfDist1;
	//まずz2に直行するベクトルを算出します。
		// 内積を求めます。
		lfProduct += stlTempT2[i]*stlUnityVector1[i];
	}
	// z2に直行するベクトルを生成します。
	lfS = grand(lfSigma1,0.0);
	// 2子供を生成します。
	for( i = 0;i < iGenVector; i++ )
	{
		lfTemp1 = lfProduct*stlUnityVector1[i];
		lfTemp2 = lfS*stlUnityVector1[i];
		lfTemp3 = stlTempT2[i] - lfTemp1 + lfTemp2;
		plfChild1[i] = stlMedian[i] + lfTemp3;
		plfChild2[i] = stlMedian[i] - lfTemp3;
	}
}

void CUndx::vSelectGens( double **pplfChildren, int *pi1stGenLoc, int *pi2ndGenLoc )
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
	if (i1stGenLoc == INT_MAX)
	{
		iRank = mrand() % (iChildrenNumber - 1) + 1;
		i1stGenLoc = stlFitProb[iRank].iLoc;
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
 * 　分散の範囲内に確実に正規乱数を発生させるようにします。
 * </PRE>
 * @param sigma 分散
 * @param mean 平均
 * @author kobayashi
 * @since 2016/06/16
 * @version 0.1
 */
double CUndx::normalRand( double sigma, double mean )
{
	double lfRes = 0.0;
	double lfMin,lfMax;
	lfMin = -sigma + mean;
	lfMax = sigma + mean;
	for(;;)
	{
		lfRes = grand( sigma, mean );
		if( -lfMin <= lfRes && lfRes <= lfMax ) break;
	}
	return lfRes;
}

/**
 * <PRE>
 * 　現時点での選択した親を出力します。
 * </PRE>
 * @author kobayashi
 * @since 2015/12/16
 * @version 0.1
 */
void CUndx::vOutputCurrentParent( double *plfParent1, double *plfParent2 )
{
	int j;
	double lfRes = 0.0;
	double lfAvgDist = 0.0;
	double lfDist = 0.0;
	double lfDist2 = 0.0;
	if( iParentOutputFlag == 1 )
	{
		// 現時点で選択した第一親を出力します。
		for( j = 0;j < iGenVector; j++ )
		{
			printf("%lf,",plfParent1[j]);
		}
		printf("\n");
		// 現時点で選択した第二親を出力します。
		for( j = 0;j < iGenVector; j++ )
		{
			printf("%lf,",plfParent2[j]);
		}
		printf("\n");
	}
}

/**
 * <PRE>
 * 　現時点での生成した子を出力します。
 * </PRE>
 * @param pplfChildrenData 
 * @author kobayashi
 * @since 2015/12/16
 * @version 0.1
 */
void CUndx::vOutputCurrentChildren( double** pplfChildrenData )
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
 * @since 2016/08/10
 * @version 0.1
 */
void CUndx::vSetGenData( double** pplfGenData )
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
 * @since 2016/08/10
 * @version 0.1
 */
void CUndx::vGetGenData( double** pplfGenData )
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
 * 　現在の遺伝子データの最良値を取得します。
 *   ver 0.1 初版 
 * </PRE>
 * @param pplfGenData 
 * @author kobayashi
 * @since 2016/09/14
 * @version 0.1
 */
void CUndx::vGetBestGenData( double* plfGenData )
{
	int j;
	for( j = 0;j < iGenVector; j++ )
	{
		plfGenData[j] = pplfGens[iBestLoc][j];
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////CCmdCheckExceptionクラス

CUndxException::CUndxException()
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
CUndxException::CUndxException(int iCode, std::string sMethodName, std::string sClassName, std::string sDetail)
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
CUndxException::CUndxException(int iCode, std::string sMethodName, std::string sClassName, std::string sDetail, int iLine)
{
	iErrCode = iCode;
	iErrLine = iLine;
	strMethodName = sMethodName;
	strClassName = sClassName;
	strErrDetail = sDetail;
}


CUndxException::~CUndxException()
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
void CUndxException::SetErrorInfo(int iCode, std::string sMethodName, std::string sClassName, std::string sDetail)
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
void CUndxException::SetErrorInfo(int iCode, std::string sMethodName, std::string sClassName, std::string sDetail, int iLine)
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
int CUndxException::iGetErrCode()
{
	return iErrCode;
}

/**
 * エラーを起こした行数を出力する
 * @author kobayashi
 * @since 2015/6/6
 * @version 1.0
 */
int CUndxException::iGetErrLine()
{
	return iErrLine;
}

/**
 * エラーを起こした関数名を出力する
 * @author kobayashi
 * @since 2015/6/6
 * @version 1.0
 */
std::string CUndxException::strGetMethodName()
{
	return strMethodName;
}

/**
 * エラーを起こしたクラスを出力する
 * @author kobayashi
 * @since 2015/6/6
 * @version 1.0
 */
std::string CUndxException::strGetClassName()
{
	return strClassName;
}

/**
 * エラーの詳細情報を出力する
 * @author kobayashi
 * @since 2015/6/6
 * @version 1.0
 */
std::string CUndxException::strGetErrDetail()
{
	return strErrDetail;
}
