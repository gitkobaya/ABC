#ifndef _ABC_H_
#define _ABC_H_

#include"CRealCodedGa.h"
#include"CUndx.h"
#include"CRex.h"
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
 	* 　人口蜂コロニーの初期化を実行します。
 	* </PRE>
 	* @param iGenCount      	 計算回数
 	* @param iGenNum        	 コロニー数
 	* @param iGenVectorDim  	 探索点の次元数
 	* @param iSearchNum  　 	 探索点の数
 	* @param iLimitCountData	 更新しなかった回数
 	* @param iCrossOverNumData	 交叉回数
 	* @param lfAlpha		 UNDXにおけるAlpha
 	* @param lfBeta			 UNDXにおけるBeta
 	* @throw AbcException
 	* @author kobayashi
 	* @since 2015/8/12
 	* @version 0.1
 	*/
	void vInitialize( int iGenCount, int iGenNum, int iGenVectorDim, int iSearchNum, int iLimitCountData, int iCrossOverNumData, double lfAlpha, double lfBeta );

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
	 * @since 2015/8/19
	 * @version 0.1
	 */
	void vInitialize( int iGenCount, int iGenNum, int iGenVectorDim, int iSearchNum, int iLimitCountData, int iIntervalMinNumData, int iAlphaData, double lfDrData, double lfBoundData, double lfAccuracyData, int iCrossOverNumData, double lfAlphaData, double lfBetaData  );

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
	void vInitialize( int iGenCount, int iGenNum, int iGenVectorDim, int iSearchNum, int iLimitCountData, int iIntervalMinNumData, int iAlphaData, double lfDrData, double lfBoundData, double lfAccuracyData, int iParentNum, int iChildrenNum, int iUpperEvalChildrenNum, double lfLearningRateData  );

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
	 *	 GBestを利用したABC法
	 *   Gbest-guided artificial bee colony algorithm for numerical function optimization.
	 *   Applied Mathematics and Computation, 217(7):3166-3173,2010.
	 * </PRE>
	 * @author kobayashi
	 * @since 2016/7/10
	 * @version 0.1
	 */
	void vGAbc();

	/**
	 * <PRE>
	 * 　人工蜂コロニー最適化法を実行します。
	 *   粒子群最適化法のIWCFAを適用
	 * </PRE>
	 * @author kobayashi
	 * @since 2015/8/3
	 * @version 0.1
	 */
	void vIWCFAAbc( int iUpdateCount );

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
	 * 　2014 Randomized Memetic Artificial Bee Colony Algorthimより
	 *   International Journal of Emerging Trends of Technology in Computer Science, vol.3(1), 2014
	 * </PRE>
	 * @author kobayashi
	 * @since 2015/7/28
	 * @version 0.1
	 */
	void vRMAbc( int iUpdateCount );

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
	 * <PRE>
	 * 　人工蜂コロニー最適化法（交叉を導入した手法）を実行します。
	 *   完全自作アレンジ
	 * </PRE>
	 * @author kobayashi
	 * @since 2016/8/12
	 * @version 0.1
	 */
	void vUndxAbc();
	
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
	void vUndxEnhancedAbc( int iUpdateCount );

	/**
	 * <PRE>
	 * 　人工蜂コロニー最適化法（交叉を導入した手法）を実行します。
	 *   完全自作アレンジ（Emsanble Real Coded CrossOverを用います。）
	 * </PRE>
	 * @author kobayashi
	 * @since 2016/8/26
	 * @version 0.1
	 */
	void vRexAbc();

	/**
	 * <PRE>
	 * 　人工蜂コロニー最適化法（交叉を導入した手法）を実行します。
	 *   完全自作アレンジ（Adaptation Emsanble Real Coded CrossOverを用います。）
	 * </PRE>
	 * @author kobayashi
	 * @since 2016/8/27
	 * @version 0.1
	 */
	void vARexAbc();

	/**
	* <PRE>
	* 　人工蜂コロニー最適化法を実行します。
	*   HJABC法を適用します。
	*   ver 0.1 初版
	*   ver 0.2 アルゴリズムが実現できていなかったので実現中。
	* </PRE>
	* @param iUpdateCount
	* @author kobayashi
	* @since 2016/10/3
	* @version 0.1
	*/
	void vHJAbc(int iUpdateCount);

	/**
	* <PRE>
	* 　人工蜂コロニー最適化法を実行します。
	*   ACABC法を適用します。
	*   ver 0.1 初版
	* </PRE>
	* @author kobayashi
	* @since 2016/10/3
	* @version 0.1
	*/
	void vACAbc();

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
	void vCBAbc(int iUpdateCount );

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
	double lfGetGlobalMinAbcDataConstFuncValue();
private:
	/**
	 * <PRE>
	 * 　Employ Beeを実行します。
	 * </PRE>
	 * @author kobayashi
	 * @since 2016/8/18
	 * @version 0.1
	 */
	void vEmployBeeOrigin();
	
	/**
	 * <PRE>
	 * 　Employ Beeを実行します。(高精度化バージョン)
	 *   2011の電子情報通信学会の論文より
	 *   ver 0.1 
	 * </PRE>
	 * @author kobayashi
	 * @since 2016/8/18
	 * @version 0.1
	 */
	double lfEmployBeeEnhanced( int iUpdateCount );

	/**
	 * <PRE>
	 * 　Employ Beeを実行します。(高精度化バージョン)
	 *   2011の電子情報通信学会の論文より
	 *   ver 0.1 
	 * </PRE>
	 * @author kobayashi
	 * @since 2016/8/18
	 * @version 0.1
	 */
	double lfEmployBeeBestEnhanced( int iUpdateCount );

	/**
	 * <PRE>
	 * 　Employ Beeを実行します。
	 *   ver 0.1 
	 *   ver 0.2 NBest版に修正
	 * </PRE>
	 * @author kobayashi
	 * @since 2016/8/10
	 * @version 0.2
	 */
	void vEmployBeeBest();
	
	/**
	 * <PRE>
	 * 　Employ Beeを実行します。(GBest版)
	 *   ver 0.1 
	 *   ver 0.2 GBest版に修正
	 * </PRE>
	 * @author kobayashi
	 * @since 2016/8/10
	 * @version 0.2
	 */
	void vEmployBeeGBest();

	/**
	 * <PRE>
	 * 　Employ Beeを実行します。
	 *   ver 0.1 
	 *   ver 0.2 2016/08/18 IWCFA(粒子群最適化法の一手法を適用)
	 * </PRE>
	 * @param lfK
	 * @param lfCoe1
	 * @param lfCoe2
	 * @param iUpdateCount
	 * @author kobayashi
	 * @since 2016/8/10
	 * @version 0.2
	 */
	void vEmployBeeIWCFA( double lfK, double lfCoe1, double lfCoe2, int iUpdateCount );

	/**
	* <PRE>
	*   Ivona Brajevic, Crossover-based artificial bee colony algorithm for constrained optimization problems, Neural Computing & Application (2015) 26:1587-1601.
	*   ver 0.1
	* </PRE>
	* @param lfMr 更新用パラメーター
	* @author kobayashi
	* @since 2016/10/19
	* @version 0.1
	*/
	void vEmployBeeCB(double lfMr);

	/**
	 * <PRE>
	 * 　Onlooker Beeを実行します。
	 * </PRE>
	 * @author kobayashi
	 * @since 2016/8/10
	 * @version 0.1
	 */
	void vOnlookerBeeOrigin();
	
	/**
	 * <PRE>
	 * 　Onlooker Beeを実行します。(高精度化バージョン)
	 *   2011の電子情報通信学会の論文より
	 *   ver 0.1 
	 * </PRE>
	 * @author kobayashi
	 * @since 2016/8/18
	 * @version 0.1
	 */
	void vOnlookerBeeEnhanced( int iUpdateCount, double lfFitJudge );

	/**
	 * <PRE>
	 * 　Onlooker Beeを実行します。(高精度化バージョン)
	 *   2011の電子情報通信学会の論文より
	 *   ver 0.1 
	 * </PRE>
	 * @author kobayashi
	 * @since 2016/8/18
	 * @version 0.1
	 */
	void vOnlookerBeeBestEnhanced( int iUpdateCount, double lfFitJudge );

	/**
	 * <PRE>
	 * Onlooker Beeを実行します。(NBest版)
	 * ver 0.1
	 * ver 0.2 NBest版を追加
	 * </PRE>
	 * @author kobayashi
	 * @since 2016/8/10
	 * @version 0.2
	 */
	void vOnlookerBeeBest();
	
	/**
	 * <PRE>
	 * Onlooker Beeを実行します。(GBest版)
	 * ver 0.1
	 * ver 0.2 NBest版を追加
	 * </PRE>
	 * @author kobayashi
	 * @since 2016/8/10
	 * @version 0.2
	 */
	void vOnlookerBeeGBest();

	/**
	 * <PRE>
	 * Onlooker Beeを実行します。(IWCFA版)
	 * ver 0.1
	 * ver 0.2 IWCFA版を追加
	 * </PRE>
	 * @param lfK
	 * @param lfCoe1
	 * @param lfCoe2
	 * @param iUpdateCount
	 * @author kobayashi
	 * @since 2016/8/10
	 * @version 0.2
	 */
	void vOnlookerBeeIWCFA( double lfK, double lfCoe1, double lfCoe2, int iUpdateCount );

	/**
	 * <PRE>
	 * Onlooker Beeを実行します。
	 * Randomized Memtic Bee Colony Method用
	 * </PRE>
	 * @author kobayashi
	 * @since 2016/9/22
	 * @version 0.1
	 */
	void vOnlookerBeeRM();

	/**
	 * <PRE>
	 * Onlooker Beeを実行します。(Hooke-Jeeves法用)
	 * ver 0.1 初版
	 * </PRE>
	 * @author kobayashi
	 * @since 2016/10/3
	 * @version 0.1
	 */
	void vOnlookerBeeHJ();

	/**
	 * <PRE>
	 * Scout Beeを実行します。(大本の手法)
	 * ver 0.1
	 * </PRE>
	 * @author kobayashi
	 * @since 2016/8/10
	 * @version 0.2
	 */
	void vScoutBeeOrigin();

	/**
	* <PRE>
	*   Ivona Brajevic, Crossover-based artificial bee colony algorithm for constrained optimization problems, Neural Computing & Application (2015) 26:1587-1601.
	*   ver 0.1
	* </PRE>
	* @author kobayashi
	* @since 2016/10/19
	* @version 0.1
	*/
	void vOnlookerBeeCB(double lfMr);
		
	/**
	 * <PRE>
	 * Scout Beeを実行します。
	 * ver 0.1
	 * ver 0.2 手法の変更。（粒子群最適化法のような更新手法。論文より）
	 * </PRE>
	 * @author kobayashi
	 * @since 2016/8/10
	 * @version 0.2
	 */
	void vScoutBeeNormal();

	/**
	 * <PRE>
	 * Scout Beeを実行します。（完全アレンジ版UNDXを実行する。）
	 * ver 0.1
	 * </PRE>
	 * @author kobayashi
	 * @since 2016/8/18
	 * @version 0.1
	 */
	void vScoutBeeUndx();
	
	/**
	 * <PRE>
	 * Scout Beeを実行します。（完全アレンジ版REXを実行する。）
	 * ver 0.1
	 * </PRE>
	 * @author kobayashi
	 * @since 2016/8/26
	 * @version 0.1
	 */
	void vScoutBeeRex();

	/**
	 * <PRE>
	 * Scout Beeを実行します。（完全アレンジ版AREXを実行する。）
	 * ver 0.1
	 * </PRE>
	 * @author kobayashi
	 * @since 2016/8/27
	 * @version 0.1
	 */
	void vScoutBeeARex();

	/**
	* <PRE>
	* Onlooker Beeを実行します。(AC-ABC用算術交叉を利用したABC法。)
	* ver 0.1 初版
	* </PRE>
	* @author kobayashi
	* @since 2016/10/13
	* @version 0.1
	*/
	void vOnlookerBeeAC();
		
	/**
	* <PRE>
	*   Ivona Brajevic, Crossover-based artificial bee colony algorithm for constrained optimization problems, Neural Computing & Application (2015) 26:1587-1601.
	*   ver 0.1
	* </PRE>
	* @author kobayashi
	* @since 2016/10/19
	* @version 0.1
	*/
	void vScoutBeeCB(int iCount);
		
	/**
	 * <PRE>
	 * 　局所最大値、最小値を取得します。
	 * </PRE>
	 * @author kobayashi
	 * @since 2016/8/10
	 * @version 0.1
	 */
	void vGetLocalMaxMin();

	/**
	 * <PRE>
	 * 　大域的最大値、最小値を取得します。
	 * </PRE>
	 * @author kobayashi
	 * @since 2016/8/10
	 * @version 0.1
	 */
	void vGetGlobalMaxMin();

	void swap(double *swap1,double *swap2);
	void qsort(int start,int end, double *sort );
	double lfNormalRand( double lfSigma, double lfMean );

	bool bHJEmStep(double *plfX1, double *plfX0, std::vector<double> stlStepSize);
	void vModifiedHookeJeevesMethod(std::vector<double> stlStepSize, double *plfX1, double *plfX2, double *plfX0);

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
	double **pplfLocalMinAbcData;		// 局所最適解を表す粒子ごとの最小値を格納するデータ
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
	CUndx *pcUndx;						// UNDX用のインスタンス
	int iCrossOverNum;					// 交叉回数
	double lfAlpha;						// UNDX用第一親決定用α
	double lfBeta;						// UNDX用第二親決定用β
	CRex *pcRex;						// REX用のインスタンス
	int iParentNumber;					// Rex用選択する親の数
	int iChildrenNumber;				// Rex用生成する子供の数
	int iUpperEvalChildrenNumber;		// ARex用生成した子供の上位を選択する数
	double lfLearningRate;				// ARex用学習率
	std::vector<Rank_t> stlFitProb;
	int iReCounter;						// HJABC用カウンター
	int iHJInterval;					// HJABC用HookeJeeves法適用間隔
	int iHJCounter;						// HJABC用HookeJeeves法繰り返し回数
	double *plfX0;						// HJABC用更新用一時格納配列
	double *plfX1;						// HJABC用更新用一時格納配列
	double *plfX2;						// HJABC用更新用一時格納配列
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
