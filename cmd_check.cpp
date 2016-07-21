#include <iostream>
#include <cstdio>
#include <cstdarg>
#include<cstdlib>
#include<cstring>
#include "cmd_check.h"

CCmdCheck::CCmdCheck()
{
	iGenerationNumber = 0;		// 計算回数
	iAbcDataNum = 0;			// 粒子群の数
	iAbcVectorDimNum = 0;		// 各粒子群の特徴ベクトル
	iAbcMethod = 0;				// 粒子群最適化法の手法設定
	pcFuncName = NULL;			// 使用する関数用フラグ
	iOutputFlag = 0;			// 結果出力用フラグ
	iIntervalMinNum = 0;		// 最低反復回数
	iAbcSearchNum = 0;			// employ beeの総数
	iAbcLimitCount = 0;			// scout bee制御変数
	iAbcUpperSearchNum = 0;		// 探索点上位数
	lfConvergenceParam = 0.0;	// 解への収束状況パラメータ
	lfFitBound = 0.0;			// 適合度許容限界値
	lfFitAccuracy = 0.0;		// 適合度の収束精度
}

CCmdCheck::~CCmdCheck()
{
}

/**
 *<PRE>
 *  入力されたコマンドをチェック
 *  ver 0.1 初版
 *  ver 0.2 オプション等の追加、およびそのほかの修正。
 *  ver 0.3 人工知能用グラフ生成ツール作成のため新たに修正。
 *</PRE>
 * @param argc	コマンドの入力項目数
 * @param argv	入力したコマンドの詳細
 * @return	CCMD_SUCCESS
 * 			CCMD_ERROR_INVALID_FORMAT
 * 			CCMD_ERROR_INVALID_DATA
 * 			CCMD_ERROR_MEMORY_ALLOCATE
 * 			CCMD_ERROR_MEMORY_RELEASE
 * @author kobayashi
 * @since 0.1 2014/05/02
 * @version 0.1
 */
long CCmdCheck::lCommandCheck( int argc, char* argv[] )
{
	int i;
	long lRet = 0;

	/* コマンドのチェック */
	if( argc <= 1 )
	{
		return CCMD_ERROR_INVALID_FORMAT;
	}
	if( argv == NULL )
	{
		return CCMD_ERROR_INVALID_FORMAT;
	}
	for( i=1; i<argc ;i++ )
	{
		/* 計算回数 */
		if( strcmp( argv[i], "-gn" ) == 0 )
		{
			lRet = lCommandErrorCheck( argv[i] );
			if( lRet != 0 ) return lRet;
			iGenerationNumber = atoi( argv[i+1] );
			i++;
		}
		/* colonyの数 */
		else if( strcmp( argv[i], "-an" ) == 0 )
		{
			lRet = lCommandErrorCheck( argv[i] );
			if( lRet != 0 ) return lRet;
			iAbcDataNum = atoi( argv[i+1] );
			i++;
		}
		/* 蜂のベクトル数 */
		else if( strcmp( argv[i], "-vn" ) == 0 )
		{
			lRet = lCommandErrorCheck( argv[i] );
			if( lRet != 0 ) return lRet;
			iAbcVectorDimNum = atoi( argv[i+1] );
			i++;
		}
		/* employ beeの総数 */
		else if( strcmp( argv[i], "-sn" ) == 0 )
		{
			lRet = lCommandErrorCheck( argv[i] );
			if( lRet != 0 ) return lRet;
			iAbcSearchNum = atoi( argv[i+1] );
			i++;
		}
		/* scout 制御変数 */
		else if( strcmp( argv[i], "-cl" ) == 0 )
		{
			lRet = lCommandErrorCheck( argv[i] );
			if( lRet != 0 ) return lRet;
			iAbcLimitCount = atof( argv[i+1] );
			i++;
		}
		/* 適合度上位の探索点数 */
		else if( strcmp( argv[i], "-usn" ) == 0 )
		{
			lRet = lCommandErrorCheck( argv[i] );
			if( lRet != 0 ) return lRet;
			iAbcUpperSearchNum = atoi( argv[i+1] );
			i++;
		}
		/* 適合度収束精度 */
		else if( strcmp( argv[i], "-fa" ) == 0 )
		{
			lRet = lCommandErrorCheck( argv[i] );
			if( lRet != 0 ) return lRet;
			lfFitAccuracy = atof( argv[i+1] );
			i++;
		}
		/* 適応度許容限界値 */
		else if( strcmp( argv[i], "-fb" ) == 0 )
		{
			lRet = lCommandErrorCheck( argv[i] );
			if( lRet != 0 ) return lRet;
			lfFitBound = atof( argv[i+1] );
			i++;
		}
		/* 最低反復回数 */
		else if( strcmp( argv[i], "-imn" ) == 0 )
		{
			lRet = lCommandErrorCheck( argv[i] );
			if( lRet != 0 ) return lRet;
			iIntervalMinNum = atoi( argv[i+1] );
			i++;
		}
		/* 解への収束状況パラメータ */
		else if( strcmp( argv[i], "-cp" ) == 0 )
		{
			lRet = lCommandErrorCheck( argv[i] );
			if( lRet != 0 ) return lRet;
			lfConvergenceParam = atof( argv[i+1] );
			i++;
		}
		/* ABC法の手法設定 */
		else if( strcmp( argv[i], "-abcm" ) == 0 )
		{
			lRet = lCommandErrorCheck( argv[i] );
			if( lRet != 0 ) return lRet;
			iAbcMethod = atoi( argv[i+1] );
			i++;
		}
		/* 評価を実施する目的関数 */
		else if( strcmp( argv[i], "-f" ) == 0 )
		{
			lRet = lCommandErrorCheck( argv[i] );
			if( lRet != 0 ) return lRet;
			pcFuncName = new char[strlen(argv[i+1])+1];
			memset( pcFuncName, '\0', sizeof(pcFuncName) );
			strcpy( pcFuncName, argv[i+1] );
			i++;
		}
		/* 解探索範囲 */
		else if( strcmp( argv[i], "-r" ) == 0 )
		{
			lRet = lCommandErrorCheck( argv[i] );
			if( lRet != 0 ) return lRet;
			lfRange = atof( argv[i+1] );
			i++;
		}
		/* 結果出力 */
		else if( strcmp( argv[i], "-out" ) == 0 )
		{
			lRet = lCommandErrorCheck( argv[i] );
			if( lRet != 0 ) return lRet;
			iOutputFlag = atoi( argv[i+1] );
			i++;
		}
		else
		{
			lRet = CCMD_ERROR_INVALID_DATA;
			break;
		}
	}
	return lRet;
}

/**
 *<PRE>
 * 入力オプションかどうかをチェックする
 * ver 0.1 新規作成
 * ver 0.2 人工知能用グラフ生成ツール作成用に修正。
 *</PRE>
 * @param argc
 * @param argv
 * @return 0
 *		  -1
 *        -2
 * @author kobayashi
 * @since 2013/1/1
 * @version 0.2
 */
long CCmdCheck::lCommandErrorCheck( char *argv )
{
	long lRet = 0L;
	if( ( strcmp( argv, "-gn" ) == 0 ) ||
		( strcmp( argv, "-an" ) == 0 ) ||
		( strcmp( argv, "-vn" ) == 0 ) ||
		( strcmp( argv, "-sn" ) == 0 )  ||
		( strcmp( argv, "-cl" ) == 0 )  ||
		( strcmp( argv, "-usn" ) == 0 )  ||
		( strcmp( argv, "-fa" ) == 0 )  ||
		( strcmp( argv, "-fb" ) == 0 )  ||
		( strcmp( argv, "-imn" ) == 0 ) ||
		( strcmp( argv, "-cp" ) == 0 ) ||
		( strcmp( argv, "-abcm" ) == 0 ) ||
		( strcmp( argv, "-f" ) == 0 ) ||
		( strcmp( argv, "-r" ) == 0 ) ||
		( strcmp( argv, "-out" ) == 0 ))
	{
		lRet = 0;
	}
	else
	{
		lRet = -2;
	}
	return lRet;
}

/**
 *<PRE>
 * 使用方法を表示する。
 *</PRE>
 * @author kobayashi
 * @since 2015/8/3
 * @version 0.2
 */
void CCmdCheck::vHelp()
{
	printf("ABC法計算\n");
	printf("使用方法\n");
	printf("abc [-gn][-an][-vn][-sn][-cl][-usn][-fa][-fb][-imn][-cp][-abcm][-f][-r][-out]\n");
	printf("-gn 更新回数\n");
	printf("-an colony数)\n");
	printf("-vn 蜂のベクトル数\n");
	printf("-sn employ beeの数\n");
	printf("-cl scout bee制御変数\n");
	printf("-usn 適合度上位の探索点数\n");
	printf("-fa 適合度収束精度\n");
	printf("-fb 適合度許容限界値\n");
	printf("-imn 最低反復回数\n");
	printf("-cp 解への収束状況パラメータ\n");
	printf("-abcm ABC法の手法設定\n");
	printf("-f 目的関数の設定\n");
	printf("-r 目的関数の解探索範囲\n");
	printf("-out 結果出力\n");
}

int CCmdCheck::iGetIntervalMinNum()
{
	return iIntervalMinNum;
}

int CCmdCheck::iGetAbcLimitCount()
{
	return iAbcLimitCount;
}

int CCmdCheck::iGetAbcSearchNum()
{
	return iAbcSearchNum;
}

int CCmdCheck::iGetAbcUpperSearchNum()
{
	return iAbcUpperSearchNum;
}

double CCmdCheck::lfGetConvergenceParam()
{
	return lfConvergenceParam;
}

double CCmdCheck::lfGetFitBound()
{
	return lfFitBound;
}

double CCmdCheck::lfGetFitAccuracy()
{
	return lfFitAccuracy;
}

char* CCmdCheck::pcGetFuncName()
{
	return pcFuncName;
}

int CCmdCheck::iGetGenerationNumber()
{
	return iGenerationNumber;
}

int CCmdCheck::iGetAbcDataNum()
{
	return iAbcDataNum;
}

int CCmdCheck::iGetAbcVectorDimNum()
{
	return iAbcVectorDimNum;
}

int CCmdCheck::iGetAbcMethod()
{
	return iAbcMethod;
}

int CCmdCheck::iGetOutputFlag()
{
	return iOutputFlag;
}

double CCmdCheck::lfGetRange()
{
	return lfRange;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////CCmdCheckExceptionクラス

/**
 * <PRE>
 * コンストラクタ(エラー情報を設定する)
 * </PRE>
 * @author kobayashi
 * @since 2015/6/10
 * @version 1.0
 */
CCmdCheckException::CCmdCheckException()
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
 * @since 2009/6/14
 * @version 1.0
 */
CCmdCheckException::CCmdCheckException(int iCode, std::string sMethodName, std::string sClassName, std::string sDetail)
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
 * @since 2009/6/14
 * @version 1.0
 */
CCmdCheckException::CCmdCheckException(int iCode, std::string sMethodName, std::string sClassName, std::string sDetail, int iLine)
{
	iErrCode = iCode;
	iErrLine = iLine;
	strMethodName = sMethodName;
	strClassName = sClassName;
	strErrDetail = sDetail;
}


/**
 * <PRE>
 * デストラクタ
 * </PRE>
 * @author kobayashi
 * @since 2015/6/10
 * @version 1.0
 */
CCmdCheckException::~CCmdCheckException()
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
 * @since 2009/6/14
 * @version 1.0
 */
void CCmdCheckException::SetErrorInfo(int iCode, std::string sMethodName, std::string sClassName, std::string sDetail)
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
 * @since 2009/6/14
 * @version 1.0
 */
void CCmdCheckException::SetErrorInfo(int iCode, std::string sMethodName, std::string sClassName, std::string sDetail, int iLine)
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
 * @since 2009/6/14
 * @version 1.0
 */
int CCmdCheckException::iGetErrCode()
{
	return iErrCode;
}

/**
 * エラーを起こした行数を出力する
 * @author kobayashi
 * @since 2009/6/14
 * @version 1.0
 */
int CCmdCheckException::iGetErrLine()
{
	return iErrLine;
}

/**
 * エラーを起こした関数名を出力する
 * @author kobayashi
 * @since 2009/6/14
 * @version 1.0
 */
std::string CCmdCheckException::strGetMethodName()
{
	return strMethodName;
}

/**
 * エラーを起こしたクラスを出力する
 * @author kobayashi
 * @since 2009/6/14
 * @version 1.0
 */
std::string CCmdCheckException::strGetClassName()
{
	return strClassName;
}

/**
 * エラーの詳細情報を出力する
 * @author kobayashi
 * @since 2009/6/14
 * @version 1.0
 */
std::string CCmdCheckException::strGetErrDetail()
{
	return strErrDetail;
}
