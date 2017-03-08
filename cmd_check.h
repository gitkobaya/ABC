#ifndef _CCMD_CHECK_H_
#define _CCMD_CHECK_H_

#include<string>

#define CCMD_SUCCESS							 0
#define CCMD_ERROR_INVALID_FORMAT				-1
#define CCMD_ERROR_INVALID_DATA					-2
#define CCMD_ERROR_MEMORY_ALLOCATE				-3
#define CCMD_ERROR_MEMORY_RELEASE				-4

class CCmdCheck
{
public:
	CCmdCheck();
	virtual ~CCmdCheck();
	
	/**
	 *<PRE>
	 * 入力されたコマンドを解析
	 *</PRE>
	 * @author kobayashi
	 * @param argc
	 * @param argv
	 * @version 1.0
	 */
	long lCommandCheck( int argc, char* argv[] );

	/**
	 *<PRE>
	 * 入力されたコマンドがフォーマットに適していない場合、エラーを返却
	 *</PRE>
	 * @author kobayashi
	 * @param argc
	 * @param argv
	 * @version 1.0
	 */
	long lCommandErrorCheck( char* argv );
	
	/**
	 *<PRE>
	 * 使用方法を表示する。
	 *</PRE>
	 * @author kobayashi
	 * @since 2015/6/10
	 * @version 0.2
	 */
	void vHelp();

public:

	int iGetIntervalMinNum();
	int iGetAbcLimitCount();
	int iGetAbcSearchNum();
	double iGetBestWeight();
	int iGetAbcUpperSearchNum();
	double lfGetConvergenceParam();
	double lfGetFitBound();
	double lfGetFitAccuracy();
	char* pcGetFuncName();
	char* pcGetConditionName();
	int iGetGenerationNumber();
	int iGetAbcDataNum();
	int iGetAbcVectorDimNum();
	int iGetAbcMethod();
	int iGetOutputFlag();
	double lfGetRange();
	double lfGetRangeMin();
	double lfGetRangeMax();
	int iGetCrossOverNum();
	double lfGetAlpha();
	double lfGetBeta();
	double lfGetLearningRate();
	int iGetParentNum();
	int iGetChildrenNum();
	int iGetUpperEvalChildrenNum();
	int iGetFinishFlag();

private:
	int iGenerationNumber;		// 計算回数
	int iAbcDataNum;		// 粒子群の数
	int iAbcVectorDimNum;		// 各粒子群の特徴ベクトル
	int iAbcMethod;			// 粒子群最適化法の手法設定
	char* pcFuncName;		// 使用する関数用フラグ
	char* pcConditionName;	// 使用する制約条件のフラグ
	int iOutputFlag;		// 結果出力用フラグ
	int iIntervalMinNum;		// 最低反復回数
	int iAbcSearchNum;		// employ beeの総数
	int iAbcLimitCount;		// scout bee制御変数
	int iAbcUpperSearchNum;		// 探索点上位数
	double lfConvergenceParam;	// 解への収束状況パラメータ
	double lfFitBound;		// 適合度許容限界値
	double lfFitAccuracy;		// 適合度の収束精度
	double lfRange;			// 解の探索範囲
	double lfRangeMin;			// 解の探索範囲の最小値
	double lfRangeMax;			// 解の探索範囲の最大値
	int iCrossOverNum;		// 交叉回数
	double lfAlpha;			// UNDXパラメータのα
	double lfBeta;			// UNDXパラメータのβ
	double lfLearningRate;
	int iParentNum;
	int iChildrenNum;
	int iUpperEvalChildrenNum;
	int iFinishFlag;		// 終了条件
};

class CCmdCheckException
{
public:
	CCmdCheckException();
	CCmdCheckException( int iCode, std::string sMethodName, std::string sClassName, std::string sDetail );
	CCmdCheckException( int iCode, std::string sMethodName, std::string sClassName, std::string sDetail, int iErrorLine );
	~CCmdCheckException();
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

#endif /*_CCMDCHECK_H_*/
