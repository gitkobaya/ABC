#ifndef _ABC_H_
#define _ABC_H_

#include<string>

const int ABC_SUCCESS = 0;
const int ABC_FATAL_ERROR = -101;
const int ABC_MEMORY_ALLOCATE_ERROR = -102;
const int ABC_INVALID_ARGUMENT_ERROR = -103;
const int ABC_INVALID_DATA_FORMAT_ERROR = -104;

class CAbc 
{
public:
	/**
	 * <PRE>
	 * 　コンストラクタ
	 * </PRE>
	 * @author kobayashi
	 * @since 2015/8/4
	 * @version 0.1
	 */
	CAbc();

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
	CAbc( int iGenCount, int iGenNum, int iGenVectorDim, int iSearchNum, int iLimitCountData );

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
	CAbc( int iGenCount, int iGenNum, int iGenVectorDim, int iSearchNum, int iLimitCountData, int iIntervalMinNumData, int iAlphaData, double lfDrData, double lfBoundData, double lfAccuracyData );

	/**
	 * <PRE>
	 * 　デストラクタ
	 * </PRE>
	 * @author kobayashi
	 * @since 2015/6/6
	 * @version 1.0
	 */
	virtual ~CAbc();

	/**
	 * <PRE>
	 * 　人口蜂コロニーの初期化を実行します。
	 * </PRE>
	 * @param iGenCount           計算回数
	 * @param iGenNum             コロニー数
	 * @param iGenVectorDim       探索点の次元数
	 * @param iSearchNum  　      探索点の数
	 * @param iLimitCountData     更新しなかった回数
	 * @param iIntervalMinNumData 最低反復回数
	 * @param iAlphaData          探索点上位数
	 * @param lfDrData            収束状況判定
	 * @param lfBoundData         適合度許容限界値
	 * @param lfAccuracyData      適合度評価精度
	 * @throw AbcException
	 * @author kobayashi
	 * @since 2015/7/28
	 * @version 0.1
	 */
	void vInitialize( int iGenCount, int iGenNum, int iGenVectorDim, int iSearchNum, int iLimitCountData, int iIntervalMinNumData, int iAlphaData, double lfDrData, double lfBoundData, double lfAccuracyData );

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
	void vInitialize( int iGenCount, int iGenNum, int iGenVectorDim, int iSearchNum, int iLimitCountData );

	/**
	 * <PRE>
	 * 　処理したい目的関数の登録を行います。
	 * </PRE>
	 * @param pflfObjectiveFunction コールバック関数
	 * @author kobayashi
	 * @since 2015/6/8
	 * @version 1.0
	 */
	void vSetConstraintFunction( double (*pflfObjectiveFunction)( double *plfData, int iVectorLen ) );

	/**
	 * <PRE>
	 * 　登録したコールバック関数の解法処理を実行します。
	 * </PRE>
	 * @author kobayashi
	 * @since 2015/6/8
	 * @version 1.0
	 */
	void vReleaseCallConstraintFunction();

	/**
	 * <PRE>
	 * 　粒子を一様乱数で初期化します。
	 * </PRE>
	 * @param lfRange 粒子の初期位置の出現範囲
	 * @author kobayashi
	 * @since 2015/6/8
	 * @version 1.0
	 */
	void vSetRandom( double lfRange );

	/**
	 * <PRE>
	 * 　粒子を一様乱数で初期化します。(ModifiedAbc法用)
	 * </PRE>
	 * @param lfRange 粒子の初期位置の出現範囲
	 * @author kobayashi
	 * @since 2015/6/8
	 * @version 1.0
	 */
	void vSetModifiedRandom( double lfRange );

	/**
	 * <PRE>
	 * 　終了処理を実行します。
	 * </PRE>
	 * @author kobayashi
	 * @since 2015/8/3
	 * @version 1.0
	 */
	void vTerminate();

	/**
	 * <PRE>
	 * 　ABC法を実行します。
	 * </PRE>
	 * @author kobayashi
	 * @since 2015/6/17
	 * @version 0.1
	 */
	void vAbc();

	/**
	 * <PRE>
	 * 　ABC法改善手法を実行します。
	 * </PRE>
	 * @author kobayashi
	 * @since 2015/8/3
	 * @version 0.1
	 */
	void vModifiedAbc( int iUpdateCount );

	/**
	 * <PRE>
	 * 　人工蜂コロニー最適化法を実行します。
	 * </PRE>
	 * @author kobayashi
	 * @since 2016/7/10
	 * @version 0.1
	 */
	void vModified2Abc();

	/**
	 * <PRE>
	 * 　ABC法改善手法を実行します。
	 * </PRE>
	 * @author kobayashi
	 * @since 2015/8/3
	 * @version 0.1
	 */
	void vModified3Abc( int iUpdateCount );

	/**
	 * <PRE>
	 * 　2013 Memetic search in artificial bee colony algorthimより
	 * </PRE>
	 * @author kobayashi
	 * @since 2015/7/28
	 * @version 0.1
	 */
	void vMeAbc( int iUpdateCount );

	/**
	 * <PRE>
	 * 　人工蜂コロニー最適化法（交叉を導入した手法）を実行します。
	 *   A Novel Hybrid Crossover based Artificial Bee Colony Algorithm for Optimization Problem International Journal of Computer Applications 2013より
	 * </PRE>
	 * @author kobayashi
	 * @since 2016/4/11
	 * @version 0.1
	 */
	void vCbAbc();

	/**
	  *<PRE>
	  * コールバック関数の定義です。
	  *</PRE>
	  */
	double (*pflfObjectiveFunction)( double *pplfData, int iVectorLen );

	/**
	 * <PRE>
	 * 　粒子群最適化法を適用した結果を出力します。(各粒子の位置)
	 * </PRE>
	 * @author kobayashi
	 * @since 2015/6/6
	 * @version 1.0
	 */
	void vOutputAbcData();

	/**
	 * <PRE>
	 * 　粒子群最適化法を適用した結果を出力します。(各粒子の速度)
	 * </PRE>
	 * @author kobayashi
	 * @since 2015/6/6
	 * @version 1.0
	 */
	void vOutputVelocityData();

	/**
	 * <PRE>
	 * 　粒子群最適化法を適用した結果を出力します。(各粒子の目的関数値)
	 * </PRE>
	 * @author kobayashi
	 * @since 2015/6/6
	 * @version 1.0
	 */
	void vOutputConstraintFunction();

	/**
	 * <PRE>
	 * 　現時点での最大値粒子の位置を出力します。
	 * </PRE>
	 * @author kobayashi
	 * @since 2015/6/19
	 * @version 1.0
	 */
	void vOutputGlobalMaxAbcData();

	/**
	 * <PRE>
	 * 　現時点での最大値粒子の目的関数値を出力します。
	 * </PRE>
	 * @author kobayashi
	 * @since 2015/6/19
	 * @version 1.0
	 */
	void vOutputGlobalMaxAbcDataConstFuncValue();

	/**
	 * <PRE>
	 * 　現時点での最小値粒子の位置を出力します。
	 * </PRE>
	 * @author kobayashi
	 * @since 2015/6/19
	 * @version 1.0
	 */
	void vOutputGlobalMinAbcData();

	/**
	 * <PRE>
	 * 　現時点での最小値粒子の目的関数値を出力します。
	 * </PRE>
	 * @author kobayashi
	 * @since 2015/6/19
	 * @version 1.0
	 */
	void vOutputGlobalMinAbcDataConstFuncValue();

	/**
	 * <PRE>
	 * 　現時点でのもっともよい粒子位置とその他の粒子との距離を出力します。
	 * </PRE>
	 * @param iOutFlag 出力方法を指定するフラグ
	 * @author kobayashi
	 * @since 2015/6/19
	 * @version 1.0
	 */
	void vOutputAbcDataLocDist( int iOutFlag );
	
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
	void vOutputLocalMaxAbcData( int iOutFlag );

	void vSetRange( double lfRange );

	void swap(double *swap1,double *swap2);
	void qsort(int start,int end, double *sort );
	double lfNormalRand( double lfSigma, double lfMean );

private:
	int iGenerationNumber;				// 計算回数
	int iAbcDataNum;					// コロニーの数
	int iAbcVectorDimNum;				// 蜂の特徴ベクトル
	int iAbcSearchNum;					// 探索点数
	int iAbcLimitCount;					// 更新しなかった回数
	int iAbcIntervalMinNum;				// 最低反復回数
	int iAbcUpperSearchNum;				// 探索点上位数
	double lfConvergenceParam;			// 収束状況パラメータ
	double lfFitBound;					// 適合度許容限界値
	double lfFitAccuracy;				// 適合度評価精度
	int *piLocalMaxAbcLoc;				// 
	double **pplfAbcData;				// ABCデータ配列
	double **pplfNAbcData;				// ABC更新用データ配列
	double lfGlobalMaxAbcData;			// 大域的最適値
	double lfGlobalMinAbcData;			// 大域的最小値
	double *plfGlobalMaxAbcData;		// 大域的最適解を表す粒子のデータ
	double *plfGlobalMinAbcData;		// 大域的最適解を表す粒子のデータ
	double *plfLocalMaxAbcData;			// 局所最適解を表す粒子のデータ
	double lfLocalMaxAbcData;			// 局所最適値
	double **pplfLocalMaxAbcData;		// 局所最適解を表す粒子ごとの最大値を格納するデータ
	double *plfLocalMaxObjectiveAbcData;// 局所最適解を表す粒子のデータ
	double **pplfVelocityData;			// ABCデータ速度配列
	double **pplfNVelocityData;			// ABC更新用データ速度配列
	double *plfFitProb;					// 適合度相対確率
	double *plfFit;						// 適合度
	int *piNonUpdateCount;				// 更新しない回数
	int *piTotalNonUpdateCount;			// 更新しない総回数
	double *plfVelocity;				// ルーレット選択により選択する速度ベクトル
	double *plfCrossOverData;			// 交叉一時格納用配列
	double lfSolveRange;				// 出力範囲
	double lfFitInit;					// 平均評価値
	double lfFitCurrentBest;			// 現在の最良値の適応度
	double *plfXnew1;					// Memetic Algorithm用更新配列
	double *plfXnew2;					// Memetic Algorithm用更新配列
};

class CAbcException
{
public:
	CAbcException();
	CAbcException( int iCode, std::string sMethodName, std::string sClassName, std::string sDetail );
	CAbcException( int iCode, std::string sMethodName, std::string sClassName, std::string sDetail, int iErrorLine );
	~CAbcException();
	void SetErrorInfo( int iCode, std::string sMethodName, std::string sClassName, std::string sDetail );
	void SetErrorInfo( int iCode, std::string sMethodName, std::string sClassName, std::string sDetail, int iErrorLine );
	int iGetErrCode();
	int iGetErrLine();
	std::string strGetMethodName();
	std::string strGetClassName();
	std::string strGetErrDetail();
private:
	int iErrCode;
	int iErrLine;
	std::string strMethodName;
	std::string strClassName;
	std::string strModuleName;
	std::string strErrDetail;
};

#endif /* _ABC_H_ */
