#include<cstdio>
#include<cstring>
#include<iostream>
#include"ABC.h"
#include"EvaluationFunction.h"
#include"cmd_check.h"

extern void vInitialize( CCmdCheck *pcCmd, CAbc *pcAbc );
extern void vSetObjectiveFunction( CCmdCheck *pcCmd, CAbc *pcAbc );
extern void vStartAbc( CCmdCheck *pcCmd, CAbc *pcAbc, int iLoc );
extern void vTerminate( CAbc *pcAbc );
extern void vOutputData( CCmdCheck *pcCmd, CAbc *pcAbc );
extern void vSetRandom( CCmdCheck *pcCmd, CAbc *pcAbc );

int main(int argc, char* argv[])
{
	int i;
	long lRet = 0L;
	CAbc abc;
	CCmdCheck cmd;

	try
	{
		lRet = cmd.lCommandCheck( argc, argv );
		if( lRet == 0 )
		{
			// 初期化を実行します。
			vInitialize( &cmd, &abc );

			printf("通りました～\n");

			// 目的関数を設定します。
			vSetObjectiveFunction( &cmd, &abc );

			// 初期値を設定します。
			vSetRandom( &cmd, &abc );
						
			// 結果を出力します。
//			vOutputData( &cmd, &abc );
			
			for( i = 0;i < cmd.iGetGenerationNumber(); i++ )
			{
				// 粒子群最適化を実行します。
				vStartAbc( &cmd, &abc, i );
				
				// 結果を出力します。
				vOutputData( &cmd, &abc );
			}
			// 終了処理を実行します。
			vTerminate( &abc );
		}
		else
		{
			// コマンドの使用方法を表示します。
			cmd.vHelp();
		}
	}
	catch( CCmdCheckException cce )
	{
		std::cout << cce.strGetClassName() << "::" << cce.strGetMethodName() << ":" << cce.strGetErrDetail() << std::endl;
		return cce.iGetErrCode();
	}
	catch( CAbcException cae )
	{
		std::cout << cae.strGetClassName() << "::" << cae.strGetMethodName() << ":" << cae.strGetErrDetail() << std::endl;
		return cae.iGetErrCode();
	}
	catch( ... )
	{
		return -1;
	}
	return 0;
}

/**
 *<PRE>
 *  初期化を実行します。
 *  ver 0.1 初版
 *</PRE>
 * @param pcCmd	コマンドチェッククラス
 * @param pcAbc	ABCアルゴリズムを実行するクラスインスタンス
 * @throw CAbcException
 * @author kobayashi
 * @since 0.1 2015/07/28
 * @version 0.1
 */
void vInitialize( CCmdCheck *pcCmd, CAbc *pcAbc )
{
	int iGenerationNumber;
	int iAbcDataNum;
	int iAbcVectorDimNum;
	int iAbcLimitCount;
	int iAbcSearchNum;
	int iIntervalMinNum;
	int iAbcUpperSearchNum;
	double lfConvergenceParam;
	double lfFitBound;
	double lfFitAccuracy;
	double lfRange;
	
	printf("start initialization\n ");
	iGenerationNumber	= pcCmd->iGetGenerationNumber();
	iIntervalMinNum		= pcCmd->iGetIntervalMinNum();
	iAbcDataNum			= pcCmd->iGetAbcDataNum();
	iAbcVectorDimNum	= pcCmd->iGetAbcVectorDimNum();
	iAbcSearchNum		= pcCmd->iGetAbcSearchNum();
	iAbcLimitCount		= pcCmd->iGetAbcLimitCount();
	iAbcUpperSearchNum	= pcCmd->iGetAbcUpperSearchNum();
	lfConvergenceParam	= pcCmd->lfGetConvergenceParam();
	lfFitBound			= pcCmd->lfGetFitBound();
	lfFitAccuracy		= pcCmd->lfGetFitAccuracy();
	lfRange				= pcCmd->lfGetRange();

	// オリジナルArtificial Bee Colony Method
	if( pcCmd->iGetAbcMethod() == 1 )
	{
		printf("start initialization\n ");
		pcAbc->vInitialize( iGenerationNumber, iAbcDataNum, iAbcVectorDimNum, iAbcSearchNum, iAbcLimitCount );
		pcAbc->vSetRange( lfRange );
		printf("finish initialization\n ");
	}
	// 変形Artificial Bee Colony Method (2011)
	else if( pcCmd->iGetAbcMethod() == 2 )
	{
		pcAbc->vInitialize( iGenerationNumber, iAbcDataNum, iAbcVectorDimNum, iAbcSearchNum, iAbcLimitCount, iIntervalMinNum, iAbcUpperSearchNum, lfConvergenceParam, lfFitBound, lfFitAccuracy );
		pcAbc->vSetRange( lfRange );
	}
	// 交叉を用いたArtificial Bee Colony Method (2013)
	else if( pcCmd->iGetAbcMethod() == 3 )
	{
		pcAbc->vInitialize( iGenerationNumber, iAbcDataNum, iAbcVectorDimNum, iAbcSearchNum, iAbcLimitCount );
		pcAbc->vSetRange( lfRange );
	}
	//　新バージョン
	else if( pcCmd->iGetAbcMethod() == 4 )
	{
		pcAbc->vInitialize( iGenerationNumber, iAbcDataNum, iAbcVectorDimNum, iAbcSearchNum, iAbcLimitCount );
		pcAbc->vSetRange( lfRange );
	}
	//　新バージョンその２
	else if( pcCmd->iGetAbcMethod() == 5 )
	{
		pcAbc->vInitialize( iGenerationNumber, iAbcDataNum, iAbcVectorDimNum, iAbcSearchNum, iAbcLimitCount );
		pcAbc->vSetRange( lfRange );
	}
	//　Memetic ABC Algorithm
	else if( pcCmd->iGetAbcMethod() == 6 )
	{
		pcAbc->vInitialize( iGenerationNumber, iAbcDataNum, iAbcVectorDimNum, iAbcSearchNum, iAbcLimitCount );
		pcAbc->vSetRange( lfRange );
	}
}

/**
 *<PRE>
 *  終了処理を実行します。
 *  ver 0.1 初版
 *</PRE>
 * @param pcAbc	ABCアルゴリズムを実行するクラスインスタンス
 * @throw CAbcException
 * @author kobayashi
 * @since 0.1 2015/07/28
 * @version 0.1
 */
void vTerminate( CAbc *pcAbc )
{
	// 目的関数のアンインストールを実行します。
	pcAbc->vReleaseCallConstraintFunction();
	
	// 粒子群最適化の処理を終了します。
	pcAbc->vTerminate();
}

/**
 *<PRE>
 *  目的関数を設定します。
 *  ver 0.1 初版
 *</PRE>
 * @param pcCmd	コマンドチェッククラス
 * @param pcAbc	ABCアルゴリズムを実行するクラスインスタンス
 * @throw CAbcException
 * @author kobayashr
 * @since 0.1 2015/07/28
 * @version 0.1
 */
void vSetObjectiveFunction( CCmdCheck *pcCmd, CAbc *pcAbc )
{
	if( strcmp(pcCmd->pcGetFuncName(), "sphere" ) == 0 )
	{
		pcAbc->vSetConstraintFunction( lfSphere );
	}
	else if( strcmp(pcCmd->pcGetFuncName(), "rosenbrock" ) == 0 )
	{
		pcAbc->vSetConstraintFunction( lfRosenbrock );
	}
	else if( strcmp(pcCmd->pcGetFuncName(), "rastrigin" ) == 0 )
	{
		pcAbc->vSetConstraintFunction( lfRastrigin );
	}
	else if( strcmp(pcCmd->pcGetFuncName(), "griewank" ) == 0 )
	{
		pcAbc->vSetConstraintFunction( lfGriewank );
	}
	else if( strcmp(pcCmd->pcGetFuncName(), "5thDeJongs" ) == 0 )
	{
		pcAbc->vSetConstraintFunction( lf5thDeJongsFunc );
	}
	else
	{
	}
}

/**
 *<PRE>
 *  Artificial Bee Colony法を実行します。
 *  ver 0.1 初版
 *</PRE>
 * @param pcCmd	コマンドチェッククラス
 * @param pcAbc	ABCアルゴリズムを実行するクラスインスタンス
 * @throw CAbcException
 * @author kobayashi
 * @since 0.1 2015/07/28
 * @version 0.1
 */
void vStartAbc( CCmdCheck *pcCmd, CAbc *pcAbc, int iLoc )
{
	if( pcCmd->iGetAbcMethod() == 1 )
	{
		pcAbc->vAbc();
	}
	else if( pcCmd->iGetAbcMethod() == 2 )
	{
		pcAbc->vModifiedAbc( iLoc );
	}
	else if( pcCmd->iGetAbcMethod() == 3 )
	{
		pcAbc->vCbAbc();
	}
	else if( pcCmd->iGetAbcMethod() == 4 )
	{
		pcAbc->vModified2Abc();
	}
	else if( pcCmd->iGetAbcMethod() == 5 )
	{
		pcAbc->vModified3Abc( iLoc );
	}
	else if( pcCmd->iGetAbcMethod() == 6 )
	{
		pcAbc->vMeAbc( iLoc );
	}
	else
	{
	}
}

/**
 *<PRE>
 *  Artificial Bee Colony法を実行した結果を出力します。
 *  ver 0.1 初版
 *</PRE>
 * @param pcCmd	コマンドチェッククラス
 * @param pcAbc	ABCアルゴリズムを実行するクラスインスタンス
 * @throw CAbcException
 * @author kobayashi
 * @since 0.1 2015/07/28
 * @version 0.1
 */
void vOutputData( CCmdCheck *pcCmd, CAbc *pcAbc )
{
	if( pcCmd->iGetOutputFlag() == 1 )
	{
		pcAbc->vOutputAbcData();
	}
	else if( pcCmd->iGetOutputFlag() == 2 )
	{
		pcAbc->vOutputVelocityData();
	}
	else if( pcCmd->iGetOutputFlag() == 3 )
	{
		pcAbc->vOutputConstraintFunction();
	}
	else if( pcCmd->iGetOutputFlag() == 4 )
	{
		pcAbc->vOutputGlobalMaxAbcData();
	}
	else if( pcCmd->iGetOutputFlag() == 5 )
	{
		pcAbc->vOutputGlobalMaxAbcDataConstFuncValue();
	}
	else if( pcCmd->iGetOutputFlag() == 5 )
	{
		pcAbc->vOutputGlobalMinAbcData();
	}
	else if( pcCmd->iGetOutputFlag() == 6 )
	{
		pcAbc->vOutputGlobalMinAbcDataConstFuncValue();
	}
	else if( pcCmd->iGetOutputFlag() == 7 )
	{
		pcAbc->vOutputAbcDataLocDist( 0 );
	}
	else if( pcCmd->iGetOutputFlag() == 8 )
	{
		pcAbc->vOutputAbcDataLocDist( 1 );
	}
	else if( pcCmd->iGetOutputFlag() == 9 )
	{
		pcAbc->vOutputLocalMaxAbcData( 0 );
	}
	else if( pcCmd->iGetOutputFlag() == 10 )
	{
		pcAbc->vOutputLocalMaxAbcData( 1 );
	}
}

/**
 *<PRE>
 *  登場する粒子の値をランダムに設定します。
 *  ver 0.1 初版
 *</PRE>
 * @param pcCmd	コマンドチェッククラス
 * @param pcAbc	ABCアルゴリズムを実行するクラスインスタンス
 * @throw CAbcException
 * @author kobayashi
 * @since 0.1 2015/07/28
 * @version 0.1
 */
void vSetRandom( CCmdCheck *pcCmd, CAbc *pcAbc )
{
	pcAbc->vSetRandom( pcCmd->lfGetRange() );
}
