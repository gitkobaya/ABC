#include<memory>
#include<cmath>
#include"CPowell.h"

void CPowell::vInitialize( int iVectorDimNum )
{
	iVector = iVectorDimNum;
	try
	{
		plfXt = new double[iVector];
		plfXCom = new double[iVector];
		plfXiCom = new double[iVector];

		for (i = 0; i < iVector; i++)
		{
			plfXt[i] = 0.0;
			plfXCom[i] = 0.0;
			plfXiCom[i] = 0.0;
		}
	}
	catch( bad_alloc ba )
	{

	}
}


void CPowell::vTerminate()
{
	if (plfXt != NULL)
	{
		delete[] plfX;
		plfXt = NULL;
	}
	if (plfXCom != NULL)
	{
		delete[] plfXCom;
		plfXCom = NULL;
	}
	if (plfXiCom != NULL)
	{
		delete[] plfXiCom;
		plfXiCom = NULL;
	}
}

/**
* <PRE>
* 　目的関数をインストールします。
* 　実際にはコールバック関数をインストールします。
* </PRE>
* @param pflfFunction 目的関数の関数ポインタ
* @author kobayashi
* @since 2015/7/28
* @version 0.1
*/
void CPowell::vSetConstraintFunction(double(*pflfFunction)(double *plfData, int iVectorLen))
{
	pflfObjectiveFunction = pflfFunction;
}

/**
* <PRE>
* 　目的関数をアンインストールします。
* 　実際にはコールバック関数をアンインストールします。
* </PRE>
* @author kobayashi
* @since 2015/7/28
* @version 0.1
*/
void CPowell::vReleaseCallConstraintFunction()
{
	pflfObjectiveFunction = NULL;
}

double CPowell::lfF1Dim( double lfShift )
{
	int i;
	for (i = 1; i <= iVectorLen; i++)
	{
		plfXt[i] = plfCom[i] + lfShift*plfXiCom[i];
	}
	return pflfConstraintFunction(plfXt, iVectorLen);
}

/**
 * <PRE>
 *    囲い込み法（Numerical Recipe in Cを参照。）
 *    ver 0.1 2016/11/02 初版
 * </PRE>
 * @param
 *
 * @since 2016/11/02
 * @version 0.1
 */ 
void CPowell::vMnbrak( double *plfAx, double *plfBx, double *plfCx, double *plfFa, double *plfFb, double *plfFc )
{
	double lfUlim, lfU, lfR, lfQ, lfFu, lfDum;

	*plfAX = lfF1Dim( plfAX );
	*plfBX = lfF1Dim( plfBX );

	if (*plfXB > *plfXA)
	{
		lfDum = *plfAx;	*plfAx = *plfBx; *plfBx = lfDum;
		lfDum = *plfFb;	*plfFb = *plfFa; *plfFa = lfDum;
	}
	// Cの初期設定
	*plfCx = *plfBx + lfGloden*(*plfBx-*plfAx);
	*plfFc = lfF1Dim( plfCx );

	// 囲い込みに成功するまで繰り返す。
	while( *plfBx > *plfCx )
	{
		// a, b, cから放物線補外でuを求める。
		lfR = (*plfBx - *plfAx)*(*plfFb-*plfFc);
		lfQ = (*plfBx - *plfCx)*(*plfFb-*plfFa);
		lfU = *plfBx - ((*plfBx-*plfCx)*lfQ-(*plfBx-*plfAx)*lfR)/(2.0*lfSgn((lfMax(fabs(lfQ-lfR), lfTiny), lfQ-lfR ) );
		lfULim = *plfBx + lfGLimit*( *plfCx-*plfBx );

		// これ以上進まずに様々な可能性を調べる。
		if( (*plfBx-lfU)*(lfU-*plfCx) > 0.0 )
		{
			lfFu = lfF1Dim( lfU );
			if( lfFu < *plfFc )
			{
				*plfAx = *plfBx;
				*plfBx = lfU;
				*plfFa = *plfFb;
				*plfFb = *plfFu;
				return;
			}
			else if( lfFu > *plfFb )
			{
				*plfCx = lfU;
				*plfFc = lfFu;
				return;
			}
			lfU = *plfCx + lfGolden*(*plfCx-*plfBx);
			lfFu = lfF1Dim( lfU );
		}
		else if( (*plfCx-lfU)*(lfU-lfULim) > 0.0 )
		{
			lfFu = lfF1Dim( lfU );
			if( lfFu < *plfFc )
			{
				*plfBx = *plfCx; *plfCx = lfU; lfU = *plfCx + lfGolden*( *plfCx-*plfBx );
				*plfFb = *plfFc; *plfFc = lfFu; lfFu = lfF1Dim( lfU );
			}
		}
		else if( (lfU-lfULim)*(lfULim-*plfCx) >= 0.0 )
		{
			lfU = lfULim;
			lfFu = lfF1Dim( lfU );
		}
		else
		{
			lfU = *plfCx + lfGolden*(*plfCx-*plfBx);
			lfU = lfF1Dim( lfU );
		}
		*plfAx = *plfBx; *plfBx = *plfCx; *plfCx = lfU;
		*plfFa = *plfFb; *plfFb = *plfFc; *plfFc = lfFu;
	}
}

/**
 * <PRE>
 *    Brentの方法
 * </PRE>
 *
 *
 *
 */ 
double CPowell::lfBrent( double lfAx, double lfBx, double lfCx, double lfTol, double *plfXMin )
{
	int iInter;
	double lfA, lfB, lfC, lfETemp, lfFu, lfFv, lfFw, lfFx, lfP, lfQ, lfR, lfToll, lfTol2, lfU, lfV, lfW, lfX, lfXm;
	double lfE = 0.0;

	// a < bにする。
	lfA = ( lfAx < lfCx ? lfAx : lfCx);
	lfB = ( lfAx > lfCx ? lfAx : lfCx);

	// 初期化
	lfX = lfW = lfV = lfBx;

	lfFw = lfFv = lfFx = lfF1Dim(lfX);

	for(iInter = 1; iInter <= iIntervalNumber; iInter++)
	{
		lfXm = 0.5*(lfA+lfB);
		lfTol2 = 2.0*(lfTol1 = lfTol*fabs(lfX)+lfZepsilon);
		if( fabs(lfX-lfXm) <= ( lfTol2-0.5*(lfB-lfA)))
		{
			*plfXmin = lfX;
			return lfFx;
		}
		if( fabs( lfE ) > lfTol1 )
		{
			lfR = ( lfX-lfW)*(lfFx-lfFv);
			lfQ = ( lfX-lfV)*(lfFx-lfFw);
			lfP = ( lfX-lfV )*lf!-(lfX-lfW)*lfR;
			lfQ = 2.0*(lfQ-lfR);
			if( lfQ > 0.0 ) lfP = -lfP;
			lfQ = fabs( lfQ );
			lfETemp = lfE;
			lfE = lfD;
			if( fabs( lfP ) >= fabs(0.5*lfQ*lfETemp) || lfP <= lfQ*(lfA-lfX) || lfP >= lfQ*(lfB-lfX))
			{
				lfD = lfCGolden*(lfE =(lfX >= lfXm ? lfA-lfX : lfB-lfX));
			}
			else
			{
				lfD = lfP /lfQ;
				lfU = lfX+lfD;
				if( lfU-lfA < lfTol2 || lfB-lfU < lfTol2 )
					lfD = lfSgn( lfTol1, lfXm-lfX );
			}
		}
		else
		{
			lfD = lfCGolden*(lfE=(lfX >= lfXm ? lfA-lfX : lfB-lfX));
		}
		lfU = (fabs(lfD) >= lfTol1 ? lfX+lfD : lfX+lfSgn(lfTol1,lfD));
		lfFu = lfF1Dim(lfU);
		if( lfFu <= lfFx )
		{
			if( lfU >= lfX ) lfA = lfX;
			else		 lfB = lfX;
			lfV = lfW; lfW = lfX; lfX = lfU;
			lfFv = lfFw; lfFw = lfFx; lfFx = lfFu;
		}
		else
		{
			if( lfU < lfX ) lfA = lfU;
			else		lfB = lfU;
			if( lfFu <= lfFw || lfW == lfX )
			{
				lfV = lfW; lfW = lfU; 
				lfFv = lfFw; lfFw = lfFu;
			}
			else if( lfFu <= lfFv || lfV == lfX || lfV == lfW )
			{
				lfV = lfU; lfFv = lfFu;
			}
		}
	}
	*plfXMin = lfX;
	return lfFx;
}

/**
 * <PRE>
 *   n次元の点p[1,...n]とn次元の方向xi[1,...n]を与えると, このルーチンはpを方向xiに沿って動かして,
 *   関数func(p)が最小になるようにする. xiはpが実際に動いた変位量で上書きされる. plfRetには最小店pでの関数値が入る.
 *   実際の仕事はルーチンmnbrak, brentを呼び出して行う.
 * </PRE>
 * 
 *
 *
 *
 *
 */ 
void CPowell::vLineMin(double *plfP, double *plfXi, int iN, double *plfRet )
{
	int i;
	double lfXX, lfXMin, lfFx, lfFb, lfFa, lfBx, lfAx;

	iNcom = iN;
	for(i = 1; i <= iN; i++ )
	{
		plfCom[i] = lfP[i];
		plfXiCom[i] = plfXi[i];
	}
	lfAx = 0.0;
	lfXX = 1.0;
	lfMnbrak( &lfAx, &lfXX, &lfBx, lfTol, &lfXMin );
	*plfRet = lfBrent( lfAx, lfXX, lfBx, lfTol, &lfXMin );
	for( i = 1;i <= iN; i++ )
	{
		plfXi[i] *= lfXMin;
		plfP[i] += plfXi[i];
	}
}

void CPowell::vPowell( double *plfP, double **pplfXi, int iN, double lfFtol, int *piInter, double *plfFRet )
{
	int i, j, iBig;
	double lfDel ,lfFp, lfFptt, lfT, *plfPt, *plfPtt, *plfXit;

	*plfFRet = pflfObjectiveFunction( plfP );
	for (i = 1; i <= iN; i++)
	{
		plfPt[i] = plfP[i];
	}
	for (*pinter = 1;; ++(*pinter))
	{
		plfFp[i] = *plfFRet;
		iBig = 0;
		// 関数値の最大減少量を求める変数
		lfDel = 0.0;
		// 各反復で、方向集合の全要素についてループ
		for (i = 1; i <= iN; i++)
		{
			// 方向をコピー
			for (j = 1; j <= iN; j++)
			{
				plfXit[j] = pplfXi[j][i];
			}
			lfFPtt = *plfFRet;
			// それに沿って最小化
			vLinMin(plfP, plfXit, iN, plfFRet);
			// 最大の現象であれば記録する。
			if (fabs(lfFPtt - *plfFRet) > lfDel)
			{
				lfDel = fabs(lfFPtt - *plfFRet);
				iBig = i;
			}
		}
		// 終了判定
		if (2.0*fabs(lfFp - *plfFRet) <= lfTol*(fabs(lfFp) + fabs(*plfFRet))
		{
			return;
		}
		if (*piInter == iItmax) break;
		for (j = 1; j <= iN; j++)
		{
			plfPtt[j] = 2.0*plfP[j] - plfPt[j];
			plfXit[j] = plfP[j] - plfPt[j];
			plfPt[j] = plfP[j];
		}
		lfPtt = pflfObjectiveFunction( plfPtt, iN );
		if (lfPtt < lfFp)
		{
			lfT = 2.0*( lfFp-2.0*(*plfFRet)+lfPtt)*( lfFp-(*plfFRet)-lfDel )*(lfFp - (*plfFRet) - lfDel) -lfDel*(lfFp-lfFPtt)*(lfFp - lfFPtt);
			if (lfT < 0.0)
			{
				vLinMin( plfP, plfXit, iN, plfFRet );
				for (j = 1; j <= iN; j++)
				{
					plfXi[j][iBig] = pplfXi[j][iN];
					plfXi[j][iN] = plfXit[j];
				}
			}
		}
	}
}



















































	*plfCx = *plfBx + lfCGold*( *plfBx - *plfAx );
	*plfFc = lfF1Dim( *plfCx );
}

void CPowell::vPowell()
{

}
