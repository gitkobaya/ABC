#include<cstdio>
#include<cstdlib>
#include<ctime>
#include<memory>
#include<cfloat>
#include"mseq.h"
#include"CRealCodedGa.h"

CRealCodedGa::CRealCodedGa()
{
	iGenerationNumber = 0;
	iGenNumber = 0;
	iGenVector = 0;
	pplfGens =  NULL;
}

CRealCodedGa::CRealCodedGa( int iGenerationNum, int iGenNum, int iGenVectorData )
{
	vInitialize( iGenerationNum, iGenNum, iGenVectorData );
}

CRealCodedGa::~CRealCodedGa()
{
//	vTerminate();
}

void CRealCodedGa::vInitialize( int iGenerationNum, int iGenNum, int iGenVectorData )
{
	int i,j;
	CRealCodedGaException crcgae;

	iGenerationNumber = iGenerationNum;
	iGenNumber = iGenNum;
	iGenVector = iGenVectorData;
	srand( time(NULL) );

	try
	{
		pplfGens = new double*[iGenNumber];
		if( pplfGens == NULL )
		{
			crcgae.SetErrorInfo( RCGA_MEMORY_ALLOCATE_ERROR, "vInitialize", "CRealCodedGa", "メモリ確保に失敗しました。", __LINE__ );
			throw( crcgae );
		}
		for( i = 0;i < iGenNumber; i++ )
		{
			pplfGens[i] = new double[iGenVector];
		}
		
		for( i = 0;i < iGenNumber; i++ )
		{
			for( j = 0;j < iGenVector; j++ )
			{
				pplfGens[i][j] = 0.0;
			}
		}
		initrand( time(NULL) );
		initrnd();
	}
	catch( std::bad_alloc ba )
	{
		crcgae.SetErrorInfo( RCGA_MEMORY_ALLOCATE_ERROR, "vInitialize", "CRealCodedGa", "メモリ確保に失敗しました。", __LINE__ );
		throw( crcgae );
	}
	catch( ... )
	{
		crcgae.SetErrorInfo( RCGA_ARRAY_INDEX_ERROR, "vInitialize", "CRealCodedGa", "配列範囲外を参照しました。", __LINE__ );
		throw( crcgae );
	}
}

/**
 * <PRE>
 * 　乱数によりGAを設定します。
 * </PRE>
 * @author kobayashi
 * @since 2015/10/30
 * @version 1.0
 */
void CRealCodedGa::vSetRandom()
{
	int i,j;
	CRealCodedGaException crcgae;

	try
	{
		for( i = 0;i < iGenNumber; i++ )
		{
			for( j = 0;j < iGenVector; j++ )
			{
				pplfGens[i][j] = rnd();
			}
		}
	}
	catch( ... )
	{
		crcgae.SetErrorInfo( RCGA_ARRAY_INDEX_ERROR, "vSetRandom", "CRealCodedGa", "配列の範囲外を参照しました。", __LINE__ );
		throw( crcgae );
	}
}

/**
 * <PRE>
 * 　目的関数をインストールします。
 * </PRE>
 * @param lfDomainMin 乱数発生最小値
 * @param lfDomainMax 乱数発生最大値
 * @author kobayashi
 * @since 2015/10/30
 * @version 1.0
 */
void CRealCodedGa::vSetRandom( double lfDomainMin, double lfDomainMax )
{
	int i,j;
	CRealCodedGaException crcgae;

	try
	{
		for( i = 0;i < iGenNumber; i++ )
		{
			for( j = 0;j < iGenVector; j++ )
			{
				pplfGens[i][j] = rnd()*(lfDomainMax-lfDomainMin) + lfDomainMin;
			}
		}
	}
	catch( ... )
	{
		crcgae.SetErrorInfo( RCGA_ARRAY_INDEX_ERROR, "vSetRandom", "CRealCodedGa", "配列の範囲外を参照しました。", __LINE__ );
		throw( crcgae );
	}
}

/**
 * <PRE>
 * 　目的関数をインストールします。
 * </PRE>
 * @author kobayashi
 * @since 2015/10/30
 * @version 1.0
 */
void CRealCodedGa::vTerminate()
{
	int i;
	CRealCodedGaException crcgae;

	try
	{
		if( pplfGens != NULL )
		{
			for( i = 0;i < iGenNumber; i++ )
			{
				if( pplfGens[i] != NULL )
				{
					delete[] pplfGens[i];
					pplfGens[i] = NULL;
				}
			}
			delete[] pplfGens;
			pplfGens = NULL;
		}
	}
	catch( ... )
	{
		crcgae.SetErrorInfo( RCGA_MEMORY_RELEASE_ERROR, "vSetRandom", "CRealCodedGa", "確保したメモリの解放に失敗しました。", __LINE__ );
		throw( crcgae );
	}
}

/**
 * <PRE>
 * 　目的関数をインストールします。
 * 　実際にはコールバック関数をインストールします。
 * </PRE>
 * @param pflfFunction 目的関数の関数ポインタ
 * @author kobayashi
 * @since 2015/10/30
 * @version 1.0
 */
void CRealCodedGa::vSetConstraintFunction( double (*pflfFunction)( double *plfData, int iVectorLen ) )
{
	pflfConstraintFunction = pflfFunction;
}

/**
 * <PRE>
 * 　目的関数をアンインストールします。
 * 　実際にはコールバック関数をアンインストールします。
 * </PRE>
 * @author kobayashi
 * @since 2015/10/30
 * @version 1.0
 */
void CRealCodedGa::vReleaseCallbackConstraintFunction()
{
	pflfConstraintFunction = NULL;
}

/**
 * <PRE>
 * 　実数値ＧＡの結果を出力します。(各遺伝子のベクトル)
 * </PRE>
 * @author kobayashi
 * @since 2015/12/16
 * @version 1.0
 */
void CRealCodedGa::vOutputGenData()
{
	int i,j;
	// 現時点での粒子の位置を出力します。
	for( i = 0; i < iGenNumber; i++ )
	{
		for( j = 0;j < iGenVector; j++ )
		{
			printf("%lf,", pplfGens[i][j]);
		}
		printf("\n");
	}
}

/**
 * <PRE>
 * 　現在の実数値GAに目的関数を適用した結果を出力します。(各遺伝子の目的関数値)
 * </PRE>
 * @author kobayashi
 * @since 2015/12/16
 * @version 1.0
 */
void CRealCodedGa::vOutputConstraintFunction()
{
	int i;
	// 現時点での各粒子の目的関数の値を出力します。
	for( i = 0; i < iGenNumber; i++ )
	{
		printf("%lf,", pflfConstraintFunction(pplfGens[i], iGenVector) );
	}
	printf("\n");
}

/**
 * <PRE>
 * 　現時点でのもっともよい遺伝子の位置を出力します。
 * </PRE>
 * @author kobayashi
 * @since 2015/6/19
 * @version 1.0
 */
void CRealCodedGa::vOutputGlobalMaxGenData()
{
	int i;
	int iLoc = 0;
	double lfTemp = 0.0;
	double lfRes = 0.0;
	lfTemp = DBL_MAX;
	for( i = 0;i < iGenNumber; i++ )
	{
		// 現時点での各遺伝子の目的関数の値がもっとも最適値になっているものを出力します。
		lfRes = pflfConstraintFunction(pplfGens[i], iGenVector);
		if( lfRes <= lfTemp )
		{
			lfTemp = lfRes;
			iLoc = i;
		}
	}
	// 最適値になっている遺伝子を出力します。
	for( i = 0;i < iGenVector; i++ )
	{
		printf("%lf,", pplfGens[iLoc][i] );
	}
	printf("\n");
}

/**
 * <PRE>
 * 　現時点でのもっともよい遺伝子の目的関数値を出力します。
 * </PRE>
 * @author kobayashi
 * @since 2015/12/16
 * @version 1.0
 */
void CRealCodedGa::vOutputGlobalMaxConstFuncValue()
{
	int i;
	double lfTemp = 0.0;
	double lfRes = 0.0;
	lfTemp = DBL_MAX;
	for( i = 0;i < iGenNumber; i++ )
	{
		// 現時点での各遺伝子の目的関数の値を出力します。
		lfRes = pflfConstraintFunction(pplfGens[i], iGenVector);
		if( lfRes <= lfTemp )
		{
			lfTemp = lfRes;
		}
	}
	// 最適値になっている目的関数の値を出力します。
	printf("%lf\n", lfTemp );
}

/**
 * <PRE>
 * 　現時点でのもっともよい遺伝子及びその目的関数値を出力します。
 * </PRE>
 * @author kobayashi
 * @since 2015/12/16
 * @version 1.0
 */
void CRealCodedGa::vOutputGlobalMaxGenDataConstFuncValue()
{
	int i;
	int iLoc = 0;
	double lfTemp = 0.0;
	double lfRes = 0.0;
	lfTemp = DBL_MAX;
	for( i = 0;i < iGenNumber; i++ )
	{
		// 現時点での各遺伝子の目的関数の値を出力します。
		lfRes = pflfConstraintFunction(pplfGens[i], iGenVector);
		if( lfRes <= lfTemp )
		{
			lfTemp = lfRes;
			iLoc = i;
		}
	}
	// 最適値になっている遺伝子を出力します。
	for( i = 0;i < iGenVector; i++ )
	{
		printf("%lf,", pplfGens[iLoc][i] );
	}
	// 最適値になっている目的関数の値を出力します。
	printf("%lf\n", lfTemp );
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////CCmdCheckExceptionクラス

CRealCodedGaException::CRealCodedGaException()
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
CRealCodedGaException::CRealCodedGaException(int iCode, std::string sMethodName, std::string sClassName, std::string sDetail)
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
CRealCodedGaException::CRealCodedGaException(int iCode, std::string sMethodName, std::string sClassName, std::string sDetail, int iLine)
{
	iErrCode = iCode;
	iErrLine = iLine;
	strMethodName = sMethodName;
	strClassName = sClassName;
	strErrDetail = sDetail;
}


CRealCodedGaException::~CRealCodedGaException()
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
void CRealCodedGaException::SetErrorInfo(int iCode, std::string sMethodName, std::string sClassName, std::string sDetail)
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
void CRealCodedGaException::SetErrorInfo(int iCode, std::string sMethodName, std::string sClassName, std::string sDetail, int iLine)
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
int CRealCodedGaException::iGetErrCode()
{
	return iErrCode;
}

/**
 * エラーを起こした行数を出力する
 * @author kobayashi
 * @since 2015/6/6
 * @version 1.0
 */
int CRealCodedGaException::iGetErrLine()
{
	return iErrLine;
}

/**
 * エラーを起こした関数名を出力する
 * @author kobayashi
 * @since 2015/6/6
 * @version 1.0
 */
std::string CRealCodedGaException::strGetMethodName()
{
	return strMethodName;
}

/**
 * エラーを起こしたクラスを出力する
 * @author kobayashi
 * @since 2015/6/6
 * @version 1.0
 */
std::string CRealCodedGaException::strGetClassName()
{
	return strClassName;
}

/**
 * エラーの詳細情報を出力する
 * @author kobayashi
 * @since 2015/6/6
 * @version 1.0
 */
std::string CRealCodedGaException::strGetErrDetail()
{
	return strErrDetail;
}
