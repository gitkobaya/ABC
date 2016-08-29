#ifndef _CREAL_CODED_GA_H_
#define _CREAL_CODED_GA_H_

#include<cstdio>
#include<cstdlib>
#include<ctime>
#include<memory>
#include<string>

const int RCGA_SUCCESS = 0;
const int RCGA_FATAL_ERROR = -101;
const int RCGA_MEMORY_ALLOCATE_ERROR = -102;
const int RCGA_MEMORY_RELEASE_ERROR = -103;
const int RCGA_ARRAY_INDEX_ERROR = -104;

typedef struct tagRank_t
{
	double lfFitProb;
	int iLoc;
}Rank_t;

class CRealCodedGa
{
public:
	CRealCodedGa();
	CRealCodedGa( int iGenerationNum, int iGenNum, int iGenVectorData );
	virtual ~CRealCodedGa();
	void vInitialize( int iGenerationNum, int iGenNum, int iGenVectorData );
	void vSetRandom();
	void vSetRandom( double lfDomainMin, double lfDomainMax );
	void vTerminate();
	/**
	 * 	<PRE>
     * 　目的関数をインストールします。
	 * 　実際にはコールバック関数をインストールします。
	 * </PRE>
	 * @param pflfFunction 目的関数の関数ポインタ
	 * @author kobayashi
	 * @since 2015/10/30
	 * @version 1.0
	 */
	void vSetConstraintFunction( double (*pflfFunction)( double *plfData, int iVectorLen ) );

	/**
	 * <PRE>
	 * 　目的関数をアンインストールします。
	 * 　実際にはコールバック関数をアンインストールします。
	 * </PRE>
	 * @author kobayashi
	 * @since 2015/10/30
	 * @version 1.0
	 */
	void vReleaseCallbackConstraintFunction();

	/**
	  *<PRE>
	  * コールバック関数の定義です。
	  *</PRE>
	  */
	double (*pflfConstraintFunction)( double *pplfData, int iVectorLen );

	/**
	 * <PRE>
	 * 　実数値ＧＡの結果を出力します。(各遺伝子のベクトル)
	 * </PRE>
	 * @author kobayashi
	 * @since 2015/12/16
	 * @version 1.0
	 */
	void vOutputGenData();

	/**
	 * <PRE>
	 * 　現在の実数値GAに目的関数を適用した結果を出力します。(各遺伝子の目的関数値)
	 * </PRE>
	 * @author kobayashi
	 * @since 2015/12/16
	 * @version 1.0
	 */
	void vOutputConstraintFunction();
	
	/**
	 * <PRE>
	 * 　現時点でのもっともよい遺伝子の位置を出力します。
	 * </PRE>
	 * @author kobayashi
	 * @since 2015/12/16
	 * @version 1.0
	 */
	void vOutputGlobalMaxGenData();

	/**
	 * <PRE>
	 * 　現時点でのもっともよい遺伝子の目的関数値を出力します。
	 * </PRE>
	 * @author kobayashi
	 * @since 2015/12/16
	 * @version 1.0
	 */
	void vOutputGlobalMaxConstFuncValue();

	 /**
	 * <PRE>
	 * 　現時点でのもっともよい遺伝子及びその目的関数値を出力します。
	 * </PRE>
	 * @author kobayashi
	 * @since 2015/12/16
	 * @version 1.0
	 */
	void vOutputGlobalMaxGenDataConstFuncValue();

protected:
	int iGenerationNumber;
	int iGenNumber;
	int iGenVector;
	double **pplfGens;
};

// ソート関数用比較演算子クラス
class CCompareToRank
{
public:
	bool operator()( const Rank_t& tLeft, const Rank_t& tRight )
	{
		return tLeft.lfFitProb < tRight.lfFitProb;
	}
};


class CRealCodedGaException
{
public:
	CRealCodedGaException();
	CRealCodedGaException( int iCode, std::string sMethodName, std::string sClassName, std::string sDetail );
	CRealCodedGaException( int iCode, std::string sMethodName, std::string sClassName, std::string sDetail, int iErrorLine );
	~CRealCodedGaException();
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

#endif /* _CREAL_CODED_GA_H_ */
