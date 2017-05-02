import abc.Abc;
import sfmt.Sfmt;
import objectivefunction.ObjectiveFunction;
import objectivefunction.ObjectiveFunctionInterface;
import constraintcondition.ConstraintCondition;
import constraintcondition.ConstraintConditionInterface;

public class ABCMain
{
	private static double lfPrevFuncValue = Double.MAX_VALUE;
	private static int iConvergenceCount = 0;
	private static ObjectiveFunctionInterface pflfFunction;
	private static ConstraintConditionInterface pflfCondition;

	public static void main( String args[] )
	{
		int i;
		long lRet = 0L;
		Abc abc;
		CCmdCheck cmd;

		try
		{
			cmd = new CCmdCheck();
			abc = new Abc();

			lRet = cmd.lCommandCheck( args );
			if( lRet == 0 )
			{
				// 初期化を実行します。
				vInitialize( cmd, abc );

				// 目的関数を設定します。
				vSetObjectiveFunction( cmd, abc );

				// 制約条件を設定します。
				vSetConstraintCondition( cmd, abc );

				// 初期値を設定します。
				vSetRandom( cmd, abc );

				// 結果を出力します。
//				vOutputData( cmd, abc );

				for( i = 0;i < cmd.iGetGenerationNumber(); i++ )
				{
					// 粒子群最適化を実行します。
					vStartAbc( cmd, abc, i );

					// 結果を出力します。
					vOutputData( cmd, abc, i );

					// 終了条件を監視します。
					lRet = iFinisher( cmd, abc, i );
					if( lRet == 1) break;
				}
				if( cmd.iGetFinishFlag() == 2 )
				{
					System.out.println("繰り返し回数" + i);
				}
				// 終了処理を実行します。
				vTerminate( abc );
			}
			else
			{
				// コマンドの使用方法を表示します。
				cmd.vHelp();
			}
		}
		catch( OutOfMemoryError oome )
		{
			oome.getStackTrace();
		}
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
	public static void vInitialize( CCmdCheck pcCmd, Abc pcAbc )
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
		int iCrossOverNum;
		double lfAlpha;
		double lfBeta;
		int iParentNumber;
		int iChildrenNumber;
		int iUpperEvalChildrenNumber;
		double lfLearningRate;

		iGenerationNumber		= pcCmd.iGetGenerationNumber();
		iIntervalMinNum			= pcCmd.iGetIntervalMinNum();
		iAbcDataNum				= pcCmd.iGetAbcDataNum();
		iAbcVectorDimNum		= pcCmd.iGetAbcVectorDimNum();
		iAbcSearchNum			= pcCmd.iGetAbcSearchNum();
		iAbcLimitCount			= pcCmd.iGetAbcLimitCount();
		iAbcUpperSearchNum		= pcCmd.iGetAbcUpperSearchNum();
		lfConvergenceParam		= pcCmd.lfGetConvergenceParam();
		lfFitBound				= pcCmd.lfGetFitBound();
		lfFitAccuracy			= pcCmd.lfGetFitAccuracy();
		lfRange					= pcCmd.lfGetRange();
		iCrossOverNum			= pcCmd.iGetCrossOverNum();
		lfAlpha					= pcCmd.lfGetAlpha();
		lfBeta					= pcCmd.lfGetBeta();
		iChildrenNumber			= pcCmd.iGetChildrenNum();
		iParentNumber			= pcCmd.iGetParentNum();
		iUpperEvalChildrenNumber= pcCmd.iGetUpperEvalChildrenNum();
		lfLearningRate			= pcCmd.lfGetLearningRate();

		// オリジナルArtificial Bee Colony Method(2005)
	if( pcCmd.iGetAbcMethod() == 1 )
	{
		pcAbc.vInitialize( iGenerationNumber, iAbcDataNum, iAbcVectorDimNum, iAbcSearchNum, iAbcLimitCount );
		pcAbc.vSetRange( lfRange );
	}
	// 変形Artificial Bee Colony Method (2011)
	else if( pcCmd.iGetAbcMethod() == 2 )
	{
		pcAbc.vInitialize( iGenerationNumber, iAbcDataNum, iAbcVectorDimNum, iAbcSearchNum, iAbcLimitCount, iIntervalMinNum, iAbcUpperSearchNum, lfConvergenceParam, lfFitBound, lfFitAccuracy );
		pcAbc.vSetRange( lfRange );
	}
	// 交叉を用いたArtificial Bee Colony Method (2013)
	else if( pcCmd.iGetAbcMethod() == 3 )
	{
		pcAbc.vInitialize( iGenerationNumber, iAbcDataNum, iAbcVectorDimNum, iAbcSearchNum, iAbcLimitCount );
		pcAbc.vSetRange( lfRange );
	}
	//　GbestABC法
	else if( pcCmd.iGetAbcMethod() == 4 )
	{
		pcAbc.vInitialize( iGenerationNumber, iAbcDataNum, iAbcVectorDimNum, iAbcSearchNum, iAbcLimitCount );
		pcAbc.vSetRange( lfRange );
	}
	//　Memetic ABC法
	else if( pcCmd.iGetAbcMethod() == 5 )
	{
		pcAbc.vInitialize( iGenerationNumber, iAbcDataNum, iAbcVectorDimNum, iAbcSearchNum, iAbcLimitCount );
		pcAbc.vSetRange( lfRange );
	}
	//　Memetic ABC Algorithm(2013)
	else if( pcCmd.iGetAbcMethod() == 6 )
	{
	//		pcAbc.vInitialize( iGenerationNumber, iAbcDataNum, iAbcVectorDimNum, iAbcSearchNum, iAbcLimitCount );
		pcAbc.vInitialize( iGenerationNumber, iAbcDataNum, iAbcVectorDimNum, iAbcSearchNum, iAbcLimitCount, iCrossOverNum, lfAlpha, lfBeta );
		pcAbc.vSetRange( lfRange );
	}
	// UNDXを混ぜたハイブリッドABC法(提案手法)
	else if( pcCmd.iGetAbcMethod() == 7 )
	{
		pcAbc.vInitialize( iGenerationNumber, iAbcDataNum, iAbcVectorDimNum, iAbcSearchNum, iAbcLimitCount, iCrossOverNum, lfAlpha, lfBeta );
		pcAbc.vSetRange( lfRange );
	}
	// REXを混ぜたハイブリッドABC法(提案手法3)
	else if( pcCmd.iGetAbcMethod() == 8 )
	{
		pcAbc.vInitialize( iGenerationNumber, iAbcDataNum, iAbcVectorDimNum, iAbcSearchNum, iAbcLimitCount, iIntervalMinNum, iAbcUpperSearchNum, lfConvergenceParam, lfFitBound, lfFitAccuracy, iParentNumber, iChildrenNumber, iUpperEvalChildrenNumber, lfLearningRate );
		pcAbc.vSetRange( lfRange );
	}
	// AREXを混ぜたハイブリッドABC法(提案手法4)
	else if( pcCmd.iGetAbcMethod() == 9 )
	{
		pcAbc.vInitialize( iGenerationNumber, iAbcDataNum, iAbcVectorDimNum, iAbcSearchNum, iAbcLimitCount, iIntervalMinNum, iAbcUpperSearchNum, lfConvergenceParam, lfFitBound, lfFitAccuracy, iParentNumber, iChildrenNumber, iUpperEvalChildrenNumber, lfLearningRate );
		pcAbc.vSetRange( lfRange );
	}
	// HJABC法
	else if (pcCmd.iGetAbcMethod() == 10)
	{
		pcAbc.vInitialize(iGenerationNumber, iAbcDataNum, iAbcVectorDimNum, iAbcSearchNum, iAbcLimitCount, iIntervalMinNum, iAbcUpperSearchNum, lfConvergenceParam, lfFitBound, lfFitAccuracy, iParentNumber, iChildrenNumber, iUpperEvalChildrenNumber, lfLearningRate);
		pcAbc.vSetRange(lfRange);
	}
	// ACABC法
	else if (pcCmd.iGetAbcMethod() == 11 )
	{
		pcAbc.vInitialize(iGenerationNumber, iAbcDataNum, iAbcVectorDimNum, iAbcSearchNum, iAbcLimitCount, iIntervalMinNum, iAbcUpperSearchNum, lfConvergenceParam, lfFitBound, lfFitAccuracy, iParentNumber, iChildrenNumber, iUpperEvalChildrenNumber, lfLearningRate);
		pcAbc.vSetRange(lfRange);
	}
	// BestSoFarABC法
	else if (pcCmd.iGetAbcMethod() == 12)
	{
		pcAbc.vInitialize(iGenerationNumber, iAbcDataNum, iAbcVectorDimNum, iAbcSearchNum, iAbcLimitCount, iCrossOverNum, lfAlpha, lfBeta);
		pcAbc.vSetRange(lfRange);
	}
	// PABC法
	else if (pcCmd.iGetAbcMethod() == 13)
	{
		pcAbc.vInitialize(iGenerationNumber, iAbcDataNum, iAbcVectorDimNum, iAbcSearchNum, iAbcLimitCount, iIntervalMinNum, iAbcUpperSearchNum, lfConvergenceParam, lfFitBound, lfFitAccuracy, iParentNumber, iChildrenNumber, iUpperEvalChildrenNumber, lfLearningRate);
		pcAbc.vSetRange(lfRange);
	}
	// UXABC法
	else if (pcCmd.iGetAbcMethod() == 14)
	{
		pcAbc.vInitialize(iGenerationNumber, iAbcDataNum, iAbcVectorDimNum, iAbcSearchNum, iAbcLimitCount, iCrossOverNum, lfAlpha, lfBeta);
		pcAbc.vSetRange(lfRange);
	}
	// REXを混ぜたハイブリッドABC法(提案手法3)
	else if (pcCmd.iGetAbcMethod() == 15)
	{
		pcAbc.vInitialize(iGenerationNumber, iAbcDataNum, iAbcVectorDimNum, iAbcSearchNum, iAbcLimitCount, iIntervalMinNum, iAbcUpperSearchNum, lfConvergenceParam, lfFitBound, lfFitAccuracy, iParentNumber, iChildrenNumber, iUpperEvalChildrenNumber, lfLearningRate);
		pcAbc.vSetRange(lfRange);
	}
	// AREXを混ぜたハイブリッドABC法(提案手法4)
		else if (pcCmd.iGetAbcMethod() == 16)
		{
			pcAbc.vInitialize(iGenerationNumber, iAbcDataNum, iAbcVectorDimNum, iAbcSearchNum, iAbcLimitCount, iIntervalMinNum, iAbcUpperSearchNum, lfConvergenceParam, lfFitBound, lfFitAccuracy, iParentNumber, iChildrenNumber, iUpperEvalChildrenNumber, lfLearningRate);
			pcAbc.vSetRange(lfRange);
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
	public static void  vTerminate( Abc pcAbc )
	{
		// 目的関数のアンインストールを実行します。
	pcAbc.vReleaseCallbackConstraintFunction();

	// 粒子群最適化の処理を終了します。
		pcAbc.vTerminate();
	}

	/**
	 *<PRE>
	 *  目的関数を設定します。
	 *  ver 0.1 初版
	 *  ver 0.2 2016/08/10 ベンチマーク関数追加
	 *  ver 0.3 2016/09/14 ベンチマーク関数追加
	 *</PRE>
	 * @param pcCmd	コマンドチェッククラス
	 * @param pcAbc	ABCアルゴリズムを実行するクラスインスタンス
	 * @throw CAbcException
	 * @author kobayashr
	 * @since 0.1 2015/07/27
	 * @version 0.3
	 */
	public static void  vSetObjectiveFunction( CCmdCheck pcCmd, Abc pcAbc )
	{
		if( pcCmd.strGetFuncName().equals( "sphere" ) == true )
		{
			pflfFunction = new ObjectiveFunction();
			pcAbc.vSetConstraintFunction(pflfFunction);
			pcAbc.vSetConstarintFunctionMode( 1 );
		}
		else if(pcCmd.strGetFuncName().equals( "ellipsoid" ) == true )
		{
			pflfFunction = new ObjectiveFunction();
			pcAbc.vSetConstraintFunction(pflfFunction);
			pcAbc.vSetConstarintFunctionMode( 2 );
		}
		else if(pcCmd.strGetFuncName().equals( "hyper-ellipsoid" ) == true )
		{
			pflfFunction = new ObjectiveFunction();
			pcAbc.vSetConstraintFunction(pflfFunction);
			pcAbc.vSetConstarintFunctionMode( 3 );
		}
		else if(pcCmd.strGetFuncName().equals( "axis-parallel-hyper-ellipsoid" ) == true )
		{
			pflfFunction = new ObjectiveFunction();
			pcAbc.vSetConstraintFunction(pflfFunction);
			pcAbc.vSetConstarintFunctionMode( 4 );
		}
		else if(pcCmd.strGetFuncName().equals( "rotated-hyper-ellipsoid" ) == true )
		{
			pflfFunction = new ObjectiveFunction();
			pcAbc.vSetConstraintFunction(pflfFunction);
			pcAbc.vSetConstarintFunctionMode( 5 );
		}
		else if(pcCmd.strGetFuncName().equals( "moved-axis-parallel-hyper-ellipsoid" ) == true )
		{
			pflfFunction = new ObjectiveFunction();
			pcAbc.vSetConstraintFunction(pflfFunction);
			pcAbc.vSetConstarintFunctionMode( 6 );
		}
		else if(pcCmd.strGetFuncName().equals( "sum-of-different-power" ) == true )
		{
			pflfFunction = new ObjectiveFunction();
			pcAbc.vSetConstraintFunction(pflfFunction);
			pcAbc.vSetConstarintFunctionMode( 7 );
		}
		else if(pcCmd.strGetFuncName().equals( "rosenbrock" ) == true )
		{
			pflfFunction = new ObjectiveFunction();
			pcAbc.vSetConstraintFunction(pflfFunction);
			pcAbc.vSetConstarintFunctionMode( 8 );
		}
		else if(pcCmd.strGetFuncName().equals( "rosenbrockstar" ) == true )
		{
			pflfFunction = new ObjectiveFunction();
			pcAbc.vSetConstraintFunction(pflfFunction);
			pcAbc.vSetConstarintFunctionMode( 9 );
		}
		else if(pcCmd.strGetFuncName().equals( "3rd-de-jongs" ) == true )
		{
			pflfFunction = new ObjectiveFunction();
			pcAbc.vSetConstraintFunction(pflfFunction);
			pcAbc.vSetConstarintFunctionMode( 10 );
		}
		else if(pcCmd.strGetFuncName().equals( "modified-3rd-de-jongs" ) == true )
		{
			pflfFunction = new ObjectiveFunction();
			pcAbc.vSetConstraintFunction(pflfFunction);
			pcAbc.vSetConstarintFunctionMode( 11 );
		}
		else if( (pcCmd.strGetFuncName().equals( "4th-de-jongs" ) == true ) ||
			     (pcCmd.strGetFuncName().equals( "QuarticFunction") == true) )
		{
			pflfFunction = new ObjectiveFunction();
			pcAbc.vSetConstraintFunction(pflfFunction);
			pcAbc.vSetConstarintFunctionMode( 12 );
		}
		else if( (pcCmd.strGetFuncName().equals( "modified-4th-de-jongs" ) == true)  ||
			     (pcCmd.strGetFuncName().equals( "QuarticGussianFunction") == true) )
		{
			pflfFunction = new ObjectiveFunction();
			pcAbc.vSetConstraintFunction(pflfFunction);
			pcAbc.vSetConstarintFunctionMode( 13 );
		}
		else if(pcCmd.strGetFuncName().equals( "5th-de-jongs" ) == true )
		{
			pflfFunction = new ObjectiveFunction();
			pcAbc.vSetConstraintFunction(pflfFunction);
			pcAbc.vSetConstarintFunctionMode( 14 );
		}
		else if(pcCmd.strGetFuncName().equals( "Ackley" ) == true )
		{
			pflfFunction = new ObjectiveFunction();
			pcAbc.vSetConstraintFunction(pflfFunction);
			pcAbc.vSetConstarintFunctionMode( 15 );
		}
		else if(pcCmd.strGetFuncName().equals( "Easoms" ) == true )
		{
			pflfFunction = new ObjectiveFunction();
			pcAbc.vSetConstraintFunction(pflfFunction);
			pcAbc.vSetConstarintFunctionMode( 16 );
		}
		else if(pcCmd.strGetFuncName().equals( "ExtendEasoms" ) == true )
		{
			pflfFunction = new ObjectiveFunction();
			pcAbc.vSetConstraintFunction(pflfFunction);
			pcAbc.vSetConstarintFunctionMode( 17 );
		}
		else if(pcCmd.strGetFuncName().equals( "EqualityConstrained" ) == true )
		{
			pflfFunction = new ObjectiveFunction();
			pcAbc.vSetConstraintFunction(pflfFunction);
			pcAbc.vSetConstarintFunctionMode( 18 );
		}
		else if(pcCmd.strGetFuncName().equals( "griewank" ) == true )
		{
			pflfFunction = new ObjectiveFunction();
			pcAbc.vSetConstraintFunction(pflfFunction);
			pcAbc.vSetConstarintFunctionMode( 19 );
		}
		else if(pcCmd.strGetFuncName().equals( "michaelwicz" ) == true )
		{
			pflfFunction = new ObjectiveFunction();
			pcAbc.vSetConstraintFunction(pflfFunction);
			pcAbc.vSetConstarintFunctionMode( 20 );
		}
		else if(pcCmd.strGetFuncName().equals( "katsuura" ) == true )
		{
			pflfFunction = new ObjectiveFunction();
			pcAbc.vSetConstraintFunction(pflfFunction);
			pcAbc.vSetConstarintFunctionMode( 21 );
		}
		else if(pcCmd.strGetFuncName().equals( "rastrigin" ) == true )
		{
			pflfFunction = new ObjectiveFunction();
			pcAbc.vSetConstraintFunction(pflfFunction);
			pcAbc.vSetConstarintFunctionMode( 22 );
		}
		else if(pcCmd.strGetFuncName().equals( "rastriginshift" ) == true )
		{
			pflfFunction = new ObjectiveFunction();
			pcAbc.vSetConstraintFunction(pflfFunction);
			pcAbc.vSetConstarintFunctionMode( 23 );
		}
		else if(pcCmd.strGetFuncName().equals( "Schwefel" ) == true )
		{
			pflfFunction = new ObjectiveFunction();
			pcAbc.vSetConstraintFunction(pflfFunction);
			pcAbc.vSetConstarintFunctionMode( 24 );
		}
		else if(pcCmd.strGetFuncName().equals( "Sixhumpcamelback" ) == true )
		{
			pflfFunction = new ObjectiveFunction();
			pcAbc.vSetConstraintFunction(pflfFunction);
			pcAbc.vSetConstarintFunctionMode( 25 );
		}
		else if(pcCmd.strGetFuncName().equals( "langermann" ) == true )
		{
			pflfFunction = new ObjectiveFunction();
			pcAbc.vSetConstraintFunction(pflfFunction);
			pcAbc.vSetConstarintFunctionMode( 26 );
		}
		else if(pcCmd.strGetFuncName().equals( "Braninsrcos" ) == true )
		{
			pflfFunction = new ObjectiveFunction();
			pcAbc.vSetConstraintFunction(pflfFunction);
			pcAbc.vSetConstarintFunctionMode( 27 );
		}
		else if(pcCmd.strGetFuncName().equals( "Shubert" ) == true )
		{
			pflfFunction = new ObjectiveFunction();
			pcAbc.vSetConstraintFunction(pflfFunction);
			pcAbc.vSetConstarintFunctionMode( 28 );
		}
		else if(pcCmd.strGetFuncName().equals( "dropwave" ) == true )
		{
			pflfFunction = new ObjectiveFunction();
			pcAbc.vSetConstraintFunction(pflfFunction);
			pcAbc.vSetConstarintFunctionMode( 29 );
		}
		else if(pcCmd.strGetFuncName().equals( "goldsteinprice" ) == true )
		{
			pflfFunction = new ObjectiveFunction();
			pcAbc.vSetConstraintFunction(pflfFunction);
			pcAbc.vSetConstarintFunctionMode( 30 );
		}
		else if(pcCmd.strGetFuncName().equals( "Shekelsfoxholes" ) == true )
		{
			pflfFunction = new ObjectiveFunction();
			pcAbc.vSetConstraintFunction(pflfFunction);
			pcAbc.vSetConstarintFunctionMode( 31 );
		}
		else if(pcCmd.strGetFuncName().equals( "Pavianifoxholes" ) == true )
		{
			pflfFunction = new ObjectiveFunction();
			pcAbc.vSetConstraintFunction(pflfFunction);
			pcAbc.vSetConstarintFunctionMode( 32 );
		}
		else if(pcCmd.strGetFuncName().equals( "Sineenvelopesinewave" ) == true )
		{
			pflfFunction = new ObjectiveFunction();
			pcAbc.vSetConstraintFunction(pflfFunction);
			pcAbc.vSetConstarintFunctionMode( 33 );
		}
		else if(pcCmd.strGetFuncName().equals( "Eggholder" ) == true )
		{
			pflfFunction = new ObjectiveFunction();
			pcAbc.vSetConstraintFunction(pflfFunction);
			pcAbc.vSetConstarintFunctionMode( 34 );
		}
		else if(pcCmd.strGetFuncName().equals( "rana" ) == true )
		{
			pflfFunction = new ObjectiveFunction();
			pcAbc.vSetConstraintFunction(pflfFunction);
			pcAbc.vSetConstarintFunctionMode( 35 );
		}
		else if(pcCmd.strGetFuncName().equals( "pathologicaltest" ) == true )
		{
			pflfFunction = new ObjectiveFunction();
			pcAbc.vSetConstraintFunction(pflfFunction);
			pcAbc.vSetConstarintFunctionMode( 36 );
		}
		else if(pcCmd.strGetFuncName().equals( "Mastercosniewave" ) == true )
		{
			pflfFunction = new ObjectiveFunction();
			pcAbc.vSetConstraintFunction(pflfFunction);
			pcAbc.vSetConstarintFunctionMode( 37 );
		}
		else if(pcCmd.strGetFuncName().equals( "keane" ) == true )
		{
			pflfFunction = new ObjectiveFunction();
			pcAbc.vSetConstraintFunction(pflfFunction);
			pcAbc.vSetConstarintFunctionMode( 38 );
		}
		else if(pcCmd.strGetFuncName().equals( "trid" ) == true )
		{
			pflfFunction = new ObjectiveFunction();
			pcAbc.vSetConstraintFunction(pflfFunction);
			pcAbc.vSetConstarintFunctionMode( 39 );
		}
		else if(pcCmd.strGetFuncName().equals( "ktablet" ) == true )
		{
			pflfFunction = new ObjectiveFunction();
			pcAbc.vSetConstraintFunction(pflfFunction);
			pcAbc.vSetConstarintFunctionMode( 40 );
		}
		else if(pcCmd.strGetFuncName().equals( "Schaffer" ) == true )
		{
			pflfFunction = new ObjectiveFunction();
			pcAbc.vSetConstraintFunction(pflfFunction);
			pcAbc.vSetConstarintFunctionMode( 41 );
		}
		else if(pcCmd.strGetFuncName().equals("Bohachevsky") == true )
		{
			pflfFunction = new ObjectiveFunction();
			pcAbc.vSetConstraintFunction(pflfFunction);
			pcAbc.vSetConstarintFunctionMode( 42 );
		}
		else if (pcCmd.strGetFuncName().equals("Zakharov") == true)
		{
			pflfFunction = new ObjectiveFunction();
			pcAbc.vSetConstraintFunction(pflfFunction);
			pcAbc.vSetConstarintFunctionMode( 43 );
		}
		else if (pcCmd.strGetFuncName().equals("SalomonProblem") == true)
		{
			pflfFunction = new ObjectiveFunction();
			pcAbc.vSetConstraintFunction(pflfFunction);
			pcAbc.vSetConstarintFunctionMode( 44 );
		}
		else if (pcCmd.strGetFuncName().equals("Alpine") == true )
		{
			pflfFunction = new ObjectiveFunction();
			pcAbc.vSetConstraintFunction(pflfFunction);
			pcAbc.vSetConstarintFunctionMode( 45 );
		}
		else if (pcCmd.strGetFuncName().equals("Weierstrass") == true )
		{
			pflfFunction = new ObjectiveFunction();
			pcAbc.vSetConstraintFunction(pflfFunction);
			pcAbc.vSetConstarintFunctionMode( 46 );
		}
		else if (pcCmd.strGetFuncName().equals("Levy") == true )
		{
			pflfFunction = new ObjectiveFunction();
			pcAbc.vSetConstraintFunction(pflfFunction);
			pcAbc.vSetConstarintFunctionMode( 47 );
		}
		else if (pcCmd.strGetFuncName().equals("Bukin") == true )
		{
			pflfFunction = new ObjectiveFunction();
			pcAbc.vSetConstraintFunction(pflfFunction);
			pcAbc.vSetConstarintFunctionMode( 48 );
		}
		else if (pcCmd.strGetFuncName().equals("Mccormick") == true )
		{
			pflfFunction = new ObjectiveFunction();
			pcAbc.vSetConstraintFunction(pflfFunction);
			pcAbc.vSetConstarintFunctionMode( 49 );
		}
		else if (pcCmd.strGetFuncName().equals("nedocs") == true )
		{
			pflfFunction = new ObjectiveFunction();
			pcAbc.vSetConstraintFunction(pflfFunction);
			pcAbc.vSetConstarintFunctionMode( 101 );
		}
		else if (pcCmd.strGetFuncName().equals("edworkscore") == true )
		{
			pflfFunction = new ObjectiveFunction();
			pcAbc.vSetConstraintFunction(pflfFunction);
			pcAbc.vSetConstarintFunctionMode( 102 );
		}
		else if (pcCmd.strGetFuncName().equals("edwin") == true )
		{
			pflfFunction = new ObjectiveFunction();
			pcAbc.vSetConstraintFunction(pflfFunction);
			pcAbc.vSetConstarintFunctionMode( 103 );
		}
	}

	/**
	 *<PRE>
	 *  制約条件を設定します。
	 *  ver 0.1 初版
	 *</PRE>
	 * @param pcCmd	コマンドチェッククラス
	 * @param pcAbc	ABCアルゴリズムを実行するクラスインスタンス
	 * @throw CAbcException
	 * @author kobayashr
	 * @since 0.1 2017/02/17
	 * @version 0.3
	 */
	public static void vSetConstraintCondition( CCmdCheck pcCmd, Abc pcAbc )
	{
		if( pcCmd.strGetConditionName().equals("none") == true )
		{
			pflfCondition = new ConstraintCondition();
			pcAbc.vSetConstraintCondition(pflfCondition);
			pcAbc.vSetConstarintConditionMode( 0 );
		}
		else if( pcCmd.strGetConditionName().equals("nedocs") == true )
		{
			pflfCondition = new ConstraintCondition();
			pcAbc.vSetConstraintCondition(pflfCondition);
			pcAbc.vSetConstarintConditionMode( 2 );
		}
		else if( pcCmd.strGetConditionName().equals("inverse_simulation") == true )
		{
			pflfCondition = new ConstraintCondition();
			pcAbc.vSetConstraintCondition(pflfCondition);
			pcAbc.vSetConstarintConditionMode( 3 );
		}
		// 0以外はEDの逆シミュレーション用。
		// 1は単純な制約条件
		// 2は部屋数及び、医師、看護師、医療技師の数がうまく制約条件にはまるように調整したもの。
		// 3は現時点でのすべての制約条件を付与したバージョン
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
	public static void  vStartAbc( CCmdCheck pcCmd, Abc pcAbc, int iLoc )
	{
		if( pcCmd.iGetAbcMethod() == 1 )
		{
			pcAbc.vAbc();
		}
		else if( pcCmd.iGetAbcMethod() == 2 )
		{
			pcAbc.vModifiedAbc( iLoc );
		}
		else if( pcCmd.iGetAbcMethod() == 3 )
		{
			pcAbc.vCbAbc();
		}
		else if( pcCmd.iGetAbcMethod() == 4 )
		{
			pcAbc.vGAbc();
		}
		else if( pcCmd.iGetAbcMethod() == 5 )
		{
			pcAbc.vMeAbc( iLoc );
		}
		else if( pcCmd.iGetAbcMethod() == 6 )
		{
			pcAbc.vRMAbc( iLoc );
		}
		else if( pcCmd.iGetAbcMethod() == 7 )
		{
			pcAbc.vUndxAbc();
		}
		else if( pcCmd.iGetAbcMethod() == 8 )
		{
			pcAbc.vRexAbc();
		}
		else if( pcCmd.iGetAbcMethod() == 9 )
		{
			pcAbc.vARexAbc();
		}
		else if( pcCmd.iGetAbcMethod() == 10 )
		{
			pcAbc.vHJAbc( iLoc );
		}
		else if (pcCmd.iGetAbcMethod() == 11 )
		{
			pcAbc.vACAbc();
		}
		else if (pcCmd.iGetAbcMethod() == 12)
		{
			pcAbc.vBFAbc( iLoc );
		}
		else if (pcCmd.iGetAbcMethod() == 13)
		{
			pcAbc.vPAbc( iLoc );
		}
		else if (pcCmd.iGetAbcMethod() == 14)
		{
			pcAbc.vUXAbc();
		}
		else if (pcCmd.iGetAbcMethod() == 15)
		{
			pcAbc.vBFRexAbc();
		}
		else if (pcCmd.iGetAbcMethod() == 16)
		{
			pcAbc.vBFARexAbc();
		}
		else
		{
		}
	}

	/**
	 *<PRE>
	 *  Artificial Bee Colony法を実行した結果を出力します。
	 *  0を指定すると何も出力しません。
	 *  ver 0.1 初版
	 *</PRE>
	 * @param pcCmd	コマンドチェッククラス
	 * @param pcAbc	ABCアルゴリズムを実行するクラスインスタンス
	 * @throw CAbcException
	 * @author kobayashi
	 * @since 0.1 2015/07/28
	 * @version 0.1
	 */
	public static void  vOutputData( CCmdCheck pcCmd, Abc pcAbc, int iLoc )
	{
		if( pcCmd.iGetOutputFlag() == 1 )
		{
			pcAbc.vOutputAbcData();
		}
		else if( pcCmd.iGetOutputFlag() == 2 )
		{
			pcAbc.vOutputVelocityData();
		}
		else if( pcCmd.iGetOutputFlag() == 3 )
		{
			pcAbc.vOutputConstraintFunction();
		}
		else if( pcCmd.iGetOutputFlag() == 4 )
		{
			pcAbc.vOutputGlobalMaxAbcData( 0 );
		}
		else if( pcCmd.iGetOutputFlag() == 5 )
		{
			pcAbc.vOutputGlobalMinAbcData( 0 );
		}
		else if( pcCmd.iGetOutputFlag() == 6 )
		{
			pcAbc.vOutputGlobalMinAbcDataConstFuncValue();
		}
		else if (pcCmd.iGetOutputFlag() == 7)
		{
			pcAbc.vOutputGlobalMinAbcData( 1 );
		}
		else if (pcCmd.iGetOutputFlag() == 8)
		{
			pcAbc.vOutputGlobalMaxAbcDataConstFuncValue();
		}
		else if (pcCmd.iGetOutputFlag() == 9)
		{
			pcAbc.vOutputGlobalMaxAbcData(1);
		}
		else if( pcCmd.iGetOutputFlag() == 10 )
		{
			pcAbc.vOutputLocalMaxAbcData( 0 );
		}
		else if( pcCmd.iGetOutputFlag() == 11 )
		{
			pcAbc.vOutputLocalMaxAbcData( 1 );
		}
		else if (pcCmd.iGetOutputFlag() == 11)
		{
			pcAbc.vOutputLocalMinAbcData(0);
		}
		else if (pcCmd.iGetOutputFlag() == 12)
		{
			pcAbc.vOutputLocalMinAbcData(1);
		}
		else if (pcCmd.iGetOutputFlag() == 13 )
		{
//			pcAbc.vOutputLocalMinAbcDataConstFuncValue(iLoc);
		}
		else if (pcCmd.iGetOutputFlag() == 14 )
		{
//			pcAbc.vOutputGlobalMaxMinDistance();
		}
		else if (pcCmd.iGetOutputFlag() == 15 )
		{
			pcAbc.vOutputAbcDataLocDist(0);
		}
		else if (pcCmd.iGetOutputFlag() == 16 )
		{
			pcAbc.vOutputAbcDataLocDist(1);
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
	public static void  vSetRandom( CCmdCheck pcCmd, Abc pcAbc )
	{
		if( pcCmd.iGetAbcMethod() == 2 )
		{
			if (pcCmd.lfGetRange() != 0.0)
			{
				pcAbc.vSetModifiedRandom( pcCmd.lfGetRange() );
			}
			else
			{
				pcAbc.vSetModifiedRandom( pcCmd.lfGetRangeMin(), pcCmd.lfGetRangeMax() );
			}
		}
		else if( pcCmd.iGetAbcMethod() == 1 )
		{
			if (pcCmd.lfGetRange() != 0.0)
			{
				pcAbc.vSetRandom( pcCmd.lfGetRange() );
			}
			else
			{
				pcAbc.vSetRandom( pcCmd.lfGetRangeMin(), pcCmd.lfGetRangeMax() );
			}
		}
		else if(pcCmd.iGetAbcMethod() == 3 || pcCmd.iGetAbcMethod() == 12 || pcCmd.iGetAbcMethod() == 4 || pcCmd.iGetAbcMethod() == 5 || pcCmd.iGetAbcMethod() == 6 || pcCmd.iGetAbcMethod() == 10 || pcCmd.iGetAbcMethod() == 11 || pcCmd.iGetAbcMethod() == 13 || pcCmd.iGetAbcMethod() == 14 )
		{
			if (pcCmd.lfGetRange() != 0.0)
			{
				pcAbc.vSetRandomPso(pcCmd.lfGetRange());
			}
			else
			{
				pcAbc.vSetRandomPso(pcCmd.lfGetRangeMin(), pcCmd.lfGetRangeMax());
			}
		}
		else if (pcCmd.iGetAbcMethod() == 7 || pcCmd.iGetAbcMethod() == 14 )
		{
			if (pcCmd.lfGetRange() != 0.0)
			{
				pcAbc.vSetRandomUndx(pcCmd.lfGetRange());
			}
			else
			{
				pcAbc.vSetRandomUndx(pcCmd.lfGetRangeMin(), pcCmd.lfGetRangeMax());
			}
		}
		else if (pcCmd.iGetAbcMethod() == 8 || pcCmd.iGetAbcMethod() == 15)
		{
			if (pcCmd.lfGetRange() != 0.0)
			{
				pcAbc.vSetRandomRex(pcCmd.lfGetRange());
			}
			else
			{
				pcAbc.vSetRandomRex(pcCmd.lfGetRangeMin(), pcCmd.lfGetRangeMax());
			}
		}
		else if (pcCmd.iGetAbcMethod() == 9 || pcCmd.iGetAbcMethod() == 16 )
		{
			if (pcCmd.lfGetRange() != 0.0)
			{
//				pcAbc.vSetRandomARex(pcCmd.lfGetRange());
				pcAbc.vSetRandom(pcCmd.lfGetRange());
			}
			else
			{
				pcAbc.vSetRandom(pcCmd.lfGetRangeMin(), pcCmd.lfGetRangeMax());
			}
		}
		// 逆シミュレーションの評価の場合は別の関数から初期化します。
		if (pcCmd.strGetConditionName().equals( "inverse_simulation" ) == true )
		{
			pcAbc.vSetData();
		}
	}

	/**
	*<PRE>
	*  登場する粒子の値で特定の部分を再度ランダムに設定します。実験用。NEDOCSの逆シミュレーション用の特殊関数。
	*  ver 0.1 初版
	*</PRE>
	* @param pcCmd	コマンドチェッククラス
	* @param pcAbc	ABCアルゴリズムを実行するクラスインスタンス
	* @author kobayashi
	* @since 2017/04/25
	* @version 0.1
	*/
	void vSetUpdateData( CCmdCheck pcCmd, Abc pcAbc)
	{
		if ( pcCmd.strGetConditionName().equals( "inverse_simulation") == true )
		{
			if (pcCmd.lfGetRange() != 0.0)
			{
				pcAbc.vSetUpdateData(pcCmd.lfGetRange(), pcCmd.iGetAbcVectorDimNum()-12, pcCmd.iGetAbcVectorDimNum() );
			}
			else
			{
				pcAbc.vSetUpdateData(pcCmd.lfGetRangeMin(), pcCmd.lfGetRangeMax(), pcCmd.iGetAbcVectorDimNum()-12, pcCmd.iGetAbcVectorDimNum() );
			}
		}
	}

	/**
	 *<PRE>
	 *  終了条件を設定します。
	 *  ver 0.1 初版
	 *</PRE>
	 * @param pcCmd	コマンドチェッククラス
	 * @param pcAbc	ABCアルゴリズムを実行するクラスインスタンス
	 * @param iCount 繰り返し回数
	 * @throw CAbcException
	 * @author kobayashi
	 * @since 0.1 2016/09/14
	 * @version 0.1
	 */
	public static int iFinisher( CCmdCheck pcCmd, Abc pcAbc, int iCount )
	{
		int iRet = 0;
		// 回数による終了を指定した場合
		if( pcCmd.iGetFinishFlag() == 1 )
		{
			if( iCount == pcCmd.iGetGenerationNumber() )
			{
				iRet = 1;
			}
		}
		// 最適解に収束した場合に終了する場合
		else if( pcCmd.iGetFinishFlag() == 2 )
		{
			if (pcCmd.strGetFuncName().equals("michaelwicz") == true)
			{
				if ( Math.abs(pcAbc.lfGetGlobalMinAbcDataConstFuncValue()-lfPrevFuncValue) <= 0.0000001)
				{
					iConvergenceCount++;
					if (iConvergenceCount >= 10000)
					{
						iRet = 1;
					}
				}
				else
				{
					iConvergenceCount = 0;
				}
				lfPrevFuncValue = pcAbc.lfGetGlobalMinAbcDataConstFuncValue();
			}
			else if ( pcCmd.strGetFuncName().equals("Schwefel") == true)
			{
				if (Math.abs(pcAbc.lfGetGlobalMinAbcDataConstFuncValue()) <= 0.0000001)
				{
					iConvergenceCount++;
					if (iConvergenceCount >= 10000)
						iRet = 1;
				}
				else
				{
					iConvergenceCount = 0;
				}
				lfPrevFuncValue = pcAbc.lfGetGlobalMinAbcDataConstFuncValue();
			}
			else
			{
				if (Math.abs(pcAbc.lfGetGlobalMinAbcDataConstFuncValue()) <= 0.0000001)
//				if (Math.abs(pcAbc.lfGetGlobalMinAbcDataConstFuncValue()) <= 1e-4)
				{
					iRet = 1;
				}
			}
		}
		else
		{
		}
		return iRet;
	}
}