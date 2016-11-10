#pragma once
#ifndef _POWELL_H_
#define _POWELL_H_

class CPowell
{
public:
	CPowell();
	CPowell(int iVectorDimNum);
	virtual ~CPowell();

	void vInitialize(int iVectorDimNum);
	void vTerminate();

	/**
	*<PRE>
	* コールバック関数の定義です。
	*</PRE>
	*/
	double(*pflfObjectiveFunction)(double *pplfData, int iVectorLen);

	/**
	* <PRE>
	* 　処理したい目的関数の登録を行います。
	* </PRE>
	* @param pflfObjectiveFunction コールバック関数
	* @author kobayashi
	* @since 2015/6/8
	* @version 0.1
	*/
	void vSetConstraintFunction(double(*pflfObjectiveFunction)(double *plfData, int iVectorLen));

	/**
	* <PRE>
	* 　登録したコールバック関数の解法処理を実行します。
	* </PRE>
	* @author kobayashi
	* @since 2015/6/8
	* @version 0.1
	*/
	void vReleaseCallConstraintFunction();

	void vPowell(double *plfP, double **pplfXi, int iN, double lfFtol, int *piInter, double *plfFRet);

private:
	double lfF1Dim(double lfShift);
	void vMnbrak(double *plfAx, double *plfBx, double *plfCx, double *plfFa, double *plfFb, double *plfFc);
	double lfBrent(double lfAx, double lfBx, double lfCx, double lfTol, double *plfXMin);
	void vLineMin(double *plfP, double *plfXi, int iN, double *plfRet);
	double lfSgn(double lfA, double lfB);
	double lfMax(double lfA, double lfB);

private:
	double *plfXt;
	double *plfXCom;
	double *plfXiCom;
	double *plfPt;
	double *plfXit;
	double *plfPtt;

	int iVectorLen;
	int iNCom;
	const double lfGolden = 1.68034;	// ステップごとの区間の拡大率
	const double lfGLimit = 100.0;		// 放物線あてはめのステップの最大拡大率
	const double lfTiny = 1.0e-20;		// 許容誤差(囲い込み法で使用。)
	const double lfCGolden = 0.3819660;	// 黄金分割比
	const int iIntervalNumber = 100;	// 反復回数
	const double lfZepsilon = 1.0e-10;	// 許容誤差(Brentの方法で使用)。
	const double lfTol = 2.0e-4;		// 許容誤差(Powell法で使用
	const int iItmax = 200;
};

#endif /* _POWELL_H_*/
