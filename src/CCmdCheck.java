
public class CCmdCheck
{

	private int iGenerationNumber;		// 計算回数
	private int iAbcDataNum;		// 粒子群の数
	private int iAbcVectorDimNum;		// 各粒子群の特徴ベクトル
	private int iAbcMethod;			// 粒子群最適化法の手法設定
	private String strFuncName;		// 使用する関数用フラグ
	private String strConditionName;// 制約条件名
	private int iOutputFlag;		// 結果出力用フラグ
	private int iIntervalMinNum;		// 最低反復回数
	private int iAbcSearchNum;		// employ beeの総数
	private int iAbcLimitCount;		// scout bee制御変数
	private int iAbcUpperSearchNum;		// 探索点上位数
	private double lfConvergenceParam;	// 解への収束状況パラメータ
	private double lfFitBound;		// 適合度許容限界値
	private double lfFitAccuracy;		// 適合度の収束精度
	private double lfRange;			// 解の探索範囲
	private double lfRangeMin;			// 解の探索範囲の最小値
	private double lfRangeMax;			// 解の探索範囲の最大値
	private int iCrossOverNum;		// 交叉回数
	private double lfAlpha;			// UNDXパラメータのα
	private double lfBeta;			// UNDXパラメータのβ
	private double lfLearningRate;
	private int iParentNum;
	private int iChildrenNum;
	private int iUpperEvalChildrenNum;
	private int iFinishFlag;		// 終了条件

	public CCmdCheck()
	{
		iGenerationNumber = 0;		// 計算回数
		iAbcDataNum = 0;			// 粒子群の数
		iAbcVectorDimNum = 0;		// 各粒子群の特徴ベクトル
		iAbcMethod = 0;				// 粒子群最適化法の手法設定
		strFuncName = null;			// 目的関数
		strConditionName = "none";	// 制約条件
		iOutputFlag = 0;			// 結果出力用フラグ
		iIntervalMinNum = 0;		// 最低反復回数
		iAbcSearchNum = 0;			// employ beeの総数
		iAbcLimitCount = 0;			// scout bee制御変数
		iAbcUpperSearchNum = 0;		// 探索点上位数
		lfConvergenceParam = 0.0;	// 解への収束状況パラメータ
		lfFitBound = 0.0;			// 適合度許容限界値
		lfFitAccuracy = 0.0;		// 適合度の収束精度
		lfAlpha = 0.0;				// Undxのα
		lfBeta = 0.0;				// Undxのβ
		iCrossOverNum = 0;			// 交叉回数
		lfRangeMin = lfRangeMax = lfRange = 0.0;
	}

	/**
	 *<PRE>
	 *  入力されたコマンドをチェック
	 *  ver 0.1 初版
	 *  ver 0.2 オプション等の追加、およびそのほかの修正。
	 *  ver 0.3 人工知能用グラフ生成ツール作成のため新たに修正。
	 *  ver 0.4
	 *</PRE>
	 * @param args	コマンドの入力項目数
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
	public long lCommandCheck( String[] args )
	{
		int i;
		long lRet = 0;

		/* コマンドのチェック */
		if( args.length <= 1 )
		{
			return -101;
		}
		if( args == null )
		{
			return -101;
		}
		for( i=0; i< args.length ;i++ )
		{
			/* 計算回数 */
			if( args[i].equals( "-gn" ) == true )
			{
				lRet = lCommandErrorCheck( args[i] );
				if( lRet != 0 ) return lRet;
				iGenerationNumber = Integer.parseInt( args[i+1] );
				i++;
			}
			/* colonyの数 */
			else if( args[i].equals( "-an" ) == true )
			{
				lRet = lCommandErrorCheck( args[i] );
				if( lRet != 0 ) return lRet;
				iAbcDataNum = Integer.parseInt( args[i+1] );
				i++;
			}
			/* 蜂のベクトル数 */
			else if( args[i].equals( "-vn" ) == true )
			{
				lRet = lCommandErrorCheck( args[i] );
				if( lRet != 0 ) return lRet;
				iAbcVectorDimNum = Integer.parseInt( args[i+1] );
				i++;
			}
			/* employ beeの総数 */
			else if( args[i].equals( "-sn" ) == true )
			{
				lRet = lCommandErrorCheck( args[i] );
				if( lRet != 0 ) return lRet;
				iAbcSearchNum = Integer.parseInt( args[i+1] );
				i++;
			}
			/* scout 制御変数 */
			else if( args[i].equals( "-cl" ) == true )
			{
				lRet = lCommandErrorCheck( args[i] );
				if( lRet != 0 ) return lRet;
				iAbcLimitCount = Integer.parseInt( args[i+1] );
				i++;
			}
			/* 適合度上位の探索点数 */
			else if( args[i].equals( "-usn" ) == true )
			{
				lRet = lCommandErrorCheck( args[i] );
				if( lRet != 0 ) return lRet;
				iAbcUpperSearchNum = Integer.parseInt( args[i+1] );
				i++;
			}
			/* 適合度収束精度 */
			else if( args[i].equals( "-fa" ) == true )
			{
				lRet = lCommandErrorCheck( args[i] );
				if( lRet != 0 ) return lRet;
				lfFitAccuracy = Double.parseDouble( args[i+1] );
				i++;
			}
			/* 適応度許容限界値 */
			else if( args[i].equals( "-fb" ) == true )
			{
				lRet = lCommandErrorCheck( args[i] );
				if( lRet != 0 ) return lRet;
				lfFitBound = Double.parseDouble( args[i+1] );
				i++;
			}
			/* 最低反復回数 */
			else if( args[i].equals( "-imn" ) == true )
			{
				lRet = lCommandErrorCheck( args[i] );
				if( lRet != 0 ) return lRet;
				iIntervalMinNum = Integer.parseInt( args[i+1] );
				i++;
			}
			/* 解への収束状況パラメータ */
			else if( args[i].equals( "-cp" ) == true )
			{
				lRet = lCommandErrorCheck( args[i] );
				if( lRet != 0 ) return lRet;
				lfConvergenceParam = Double.parseDouble( args[i+1] );
				i++;
			}
			/* ABC法の手法設定 */
			else if( args[i].equals( "-abcm" ) == true )
			{
				lRet = lCommandErrorCheck( args[i] );
				if( lRet != 0 ) return lRet;
				iAbcMethod = Integer.parseInt( args[i+1] );
				i++;
			}
			/* 評価を実施する目的関数 */
			else if( args[i].equals( "-f" ) == true )
			{
				lRet = lCommandErrorCheck( args[i] );
				if( lRet != 0 ) return lRet;
				strFuncName = args[i+1];
				i++;
			}
			/* 評価を実施する目的関数 */
			else if (args[i].equals("-cc") == true )
			{
				lRet = lCommandErrorCheck(args[i]);
				if (lRet != 0) return lRet;
				strConditionName = args[i + 1];
				i++;
			}
			/* 解探索範囲 */
			else if( args[i].equals( "-r" ) == true )
			{
				lRet = lCommandErrorCheck( args[i] );
				if( lRet != 0 ) return lRet;
				lfRange = Double.parseDouble( args[i+1] );
				i++;
			}
			/* 解探索範囲 */
			else if ( args[i].equals( "-rmin" ) == true )
			{
				lRet = lCommandErrorCheck(args[i]);
				if (lRet != 0) return lRet;
				lfRangeMin = Double.parseDouble(args[i + 1]);
				i++;
			}
			/* 解探索範囲 */
			else if ( args[i].equals( "-rmax" ) == true )
			{
				lRet = lCommandErrorCheck(args[i]);
				if (lRet != 0) return lRet;
				lfRangeMax = Double.parseDouble(args[i + 1]);
				i++;
			}
			/* 結果出力 */
			else if( args[i].equals( "-out" ) == true )
			{
				lRet = lCommandErrorCheck( args[i] );
				if( lRet != 0 ) return lRet;
				iOutputFlag = Integer.parseInt( args[i+1] );
				i++;
			}
			/* 交叉回数 */
			else if( args[i].equals( "-cr" ) == true )
			{
				lRet = lCommandErrorCheck( args[i] );
				if( lRet != 0 ) return lRet;
				iCrossOverNum = Integer.parseInt( args[i+1] );
				i++;
			}
			/* Undxのパラメータα */
			else if( args[i].equals( "-alpha" ) == true )
			{
				lRet = lCommandErrorCheck( args[i] );
				if( lRet != 0 ) return lRet;
				lfAlpha = Double.parseDouble( args[i+1] );
				i++;
			}
			/* Undxのパラメータβ */
			else if( args[i].equals( "-beta" ) == true )
			{
				lRet = lCommandErrorCheck( args[i] );
				if( lRet != 0 ) return lRet;
				lfBeta = Double.parseDouble( args[i+1] );
				i++;
			}
			/* Rexの選択する親の数 */
			else if( args[i].equals( "-pn" ) == true )
			{
				lRet = lCommandErrorCheck( args[i] );
				if( lRet != 0 ) return lRet;
				iParentNum = Integer.parseInt( args[i+1] );
				i++;
			}
			/* Rexの生成する子供の数 */
			else if( args[i].equals( "-cn" ) == true )
			{
				lRet = lCommandErrorCheck( args[i] );
				if( lRet != 0 ) return lRet;
				iChildrenNum = Integer.parseInt( args[i+1] );
				i++;
			}
			/* ARexの生成した子供から上位選択する数 */
			else if( args[i].equals( "-cl" ) == true )
			{
				lRet = lCommandErrorCheck( args[i] );
				if( lRet != 0 ) return lRet;
				iUpperEvalChildrenNum = Integer.parseInt( args[i+1] );
				i++;
			}
			/* ARexのパラメータ学習率 */
			else if( args[i].equals( "-lr" ) == true )
			{
				lRet = lCommandErrorCheck( args[i] );
				if( lRet != 0 ) return lRet;
				lfLearningRate = Double.parseDouble( args[i+1] );
				i++;
			}
			/* 終了条件の指定 */
			else if( args[i].equals( "-fin" ) == true )
			{
				lRet = lCommandErrorCheck( args[i] );
				if( lRet != 0 ) return lRet;
				iFinishFlag = Integer.parseInt( args[i+1] );
				i++;
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
	 *	  -1
	 *        -2
	 * @author kobayashi
	 * @since 2013/1/1
	 * @version 0.2
	 */
	private long lCommandErrorCheck( String arg )
	{
		long lRet = 0L;
		if( arg.equals( "-gn" ) == true 		||
			arg.equals( "-an" ) == true			||
			arg.equals( "-vn" ) == true 		||
			arg.equals( "-sn" ) == true  	 	||
			arg.equals( "-cl" ) == true  	 	||
			arg.equals( "-usn" ) == true 		||
			arg.equals( "-fa" ) == true 		||
			arg.equals( "-fb" ) == true 		||
			arg.equals( "-imn" ) == true 		||
			arg.equals( "-cp" ) == true 		||
			arg.equals( "-abcm" ) == true 		||
			arg.equals( "-f" ) == true 			||
			arg.equals( "-cc" ) == true 		||
			arg.equals( "-r" ) == true 			||
			arg.equals( "-rmin") == true		||
			arg.equals( "-rmax") == true		||
			arg.equals( "-out" ) == true  		||
			arg.equals( "-cr" ) == true 		||
			arg.equals( "-alpha" ) == true 		||
			arg.equals( "-beta" ) == true 		||
			arg.equals( "-pn" ) == true 		||
			arg.equals( "-cn" ) == true 		||
			arg.equals( "-cl" ) == true 		||
			arg.equals( "-fin" ) == true 		||
			arg.equals( "-lr" ) == true )
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
	 * ver 0.1
	 * ver 0.2 Rexを使用するためのパラメータの追加
	 *</PRE>
	 * @author kobayashi
	 * @since 2015/8/3
	 * @version 0.2
	 */
	public void vHelp()
	{
		System.out.print("ABC法計算\n");
		System.out.print("使用方法\n");
		System.out.print("abc [-gn][-an][-vn][-sn][-cl][-usn][-fa][-fb][-imn][-cp][-abcm][-f][-cc][-r][-rmin][-rmax][-cr][alpha][-beta][-pn][-cn][-cl][-lr][-fin][-out]\n");
		System.out.print("-gn 更新回数\n");
		System.out.print("-an colony数)\n");
		System.out.print("-vn 蜂のベクトル数\n");
		System.out.print("-sn employ beeの数\n");
		System.out.print("-cl scout bee制御変数\n");
		System.out.print("-usn 適合度上位の探索点数\n");
		System.out.print("-fa 適合度収束精度\n");
		System.out.print("-fb 適合度許容限界値\n");
		System.out.print("-imn 最低反復回数\n");
		System.out.print("-cp 解への収束状況パラメータ\n");
		System.out.print("-abcm ABC法の手法設定\n");
		System.out.print("-f 目的関数の設定\n");
		System.out.print("-cc 制約条件の設定\n");
		System.out.print("-r 目的関数の解探索範囲\n");
		System.out.print("-rmin 目的関数の解探索範囲の最小値\n");
		System.out.print("-rmax 目的関数の解探索範囲の最大値\n");
		System.out.print("-cr 交叉回数\n");
		System.out.print("-alpha Undxのα\n");
		System.out.print("-beta Undxのβ\n");
		System.out.print("-pn Rexの選択する親の数\n");
		System.out.print("-cn Rexの生成する子供の数\n");
		System.out.print("-cl ARexの生成した子供のうち上位選択する数\n");
		System.out.print("-lr ARexの学習率\n");
		System.out.print("-fin 終了条件の設定1:回数, 2:収束\n");
		System.out.print("-out 結果出力\n");
	}

	public 	int iGetIntervalMinNum()
	{
		return iIntervalMinNum;
	}

	public 	int iGetAbcLimitCount()
	{
		return iAbcLimitCount;
	}

	public 	int iGetAbcSearchNum()
	{
		return iAbcSearchNum;
	}

	public 	int iGetAbcUpperSearchNum()
	{
		return iAbcUpperSearchNum;
	}

	public 	double lfGetConvergenceParam()
	{
		return lfConvergenceParam;
	}

	public 	double lfGetFitBound()
	{
		return lfFitBound;
	}

	public 	double lfGetFitAccuracy()
	{
		return lfFitAccuracy;
	}

	public String strGetFuncName()
	{
		return strFuncName;
	}

	public String strGetConditionName()
	{
		return strConditionName;
	}

	public 	int iGetGenerationNumber()
	{
		return iGenerationNumber;
	}

	public 	int iGetAbcDataNum()
	{
		return iAbcDataNum;
	}

	public 	int iGetAbcVectorDimNum()
	{
		return iAbcVectorDimNum;
	}

	public 	int iGetAbcMethod()
	{
		return iAbcMethod;
	}

	public 	int iGetOutputFlag()
	{
		return iOutputFlag;
	}

	public 	double lfGetRange()
	{
		return lfRange;
	}

	double lfGetRangeMin()
	{
		return lfRangeMin;
	}

	public 	double lfGetRangeMax()
	{
		return lfRangeMax;
	}

	public 	int iGetCrossOverNum()
	{
		return iCrossOverNum;
	}

	public 	double lfGetAlpha()
	{
		return lfAlpha;
	}

	public 	double lfGetBeta()
	{
		return lfBeta;
	}

	public 	double lfGetLearningRate()
	{
		return lfLearningRate;
	}

	public 	int iGetParentNum()
	{
		return iParentNum;
	}

	public 	int iGetChildrenNum()
	{
		return iChildrenNum;
	}

	public int iGetUpperEvalChildrenNum()
	{
		return iUpperEvalChildrenNum;
	}

	public int iGetFinishFlag()
	{
		return iFinishFlag;
	}
}