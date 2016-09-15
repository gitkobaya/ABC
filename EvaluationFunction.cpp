#include<cmath>
#include"mseq.h"
#include"constdata.h"
#include"EvaluationFunction.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////評価関数

/**
 * <PRE>
 * 　目的関数のSphere(1st De Jong's function)関数の計算を実行します。
 * 　-5.12 <= x_i <= 5.12 f_i(x_i)=0,  x_i=0, i = 1,2,･･･,n
 * 　f(x) = sum(x_{i}^{2})
 * </PRE>
 * @param plfX			引数
 * @param iVectorLen	引数の次元数
 * @author kobayashi
 * @since 2015/6/6
 * @version 1.0
 */
double lfSphere( double *plfX, int iVectorLen )
{
	int i;
	double lfRes = 0.0;
	for( i = 0;i < iVectorLen; i++ )
	{
		lfRes += plfX[i]*plfX[i];
	}
	return lfRes;
}

/**
 * <PRE>
 * 　目的関数のEllipsoid関数の計算を実行します。
 * 　-5.12 <= x_i <= 5.12 f_i(x_i)=0,  x_i=0, i = 1,2,･･･,n
 * 　f(x) = sum(1000^{i-1/n-1}x_{i})^{2}
 * </PRE>
 * @param plfX			引数
 * @param iVectorLen	引数の次元数
 * @author kobayashi
 * @since 2016/8/24
 * @version 0.1
 */
double lfEllipsoid( double *plfX, int iVectorLen )
{
	int i;
	double lfRes = 0.0;
	double lfX = 0.0;
	for( i = 0;i < iVectorLen; i++ )
	{
		lfX = pow(1000.0, (double)(i-1)/(double)(iVectorLen-1))*plfX[i];
		lfRes += lfX*lfX;
	}
	return lfRes;
}

/**
 * <PRE>
 * 　目的関数のHyper-Ellipsoid関数の計算を実行します。
 * 　-1 <= x_i <= 1 f_i(x_i)=0,  x_i=0, i = 1,2,･･･,n
 * 　f(x) = sum(i^{2}*x_{i}^{2})
 * </PRE>
 * @param plfX			引数
 * @param iVectorLen	引数の次元数
 * @author kobayashi
 * @since 2015/6/12
 * @version 1.0
 */
double lfHyperEllipsoid( double *plfX, int iVectorLen )
{
	int i;
	double lfRes = 0.0;
	for( i = 0;i < iVectorLen; i++ )
	{
		lfRes += (double)i*(double)i*plfX[i]*plfX[i];
	}
	return lfRes;
}

/**
 * <PRE>
 * 　目的関数のAxis Parallel hyper-ellipsoid関数の計算を実行します。
 * 　-5.12 <= x_i <= 5.12,  f_{i}(x_{i})=0,  x_{i}=0, i = 1,2,･･･,n
 * 　f(x) = sum(i*x_{i}^{2})
 * </PRE>
 * @param plfX			引数
 * @param iVectorLen	引数の次元数
 * @author kobayashi
 * @since 2015/6/12
 * @version 1.0
 */
double lfAxisParallelHyperEllipsoid( double *plfX, int iVectorLen )
{
	int i;
	double lfRes = 0.0;
	for( i = 0;i < iVectorLen; i++ )
	{
		lfRes += (double)i*plfX[i]*plfX[i];
	}
	return lfRes;
}

/**
 * <PRE>
 * 　目的関数のRotated hyper-ellipsoid関数の計算を実行します。
 *   大域的最適解 x_{i} = 0 のときf(x_{i}) = 0 (-65.536 <= Xi <= 65.536)
 * 　\sum_{i=1}^{n}(\sum_{j=1}^{i}(x_{j}))^{2}
 * </PRE>
 * @param plfX			引数
 * @param iVectorLen	引数の次元数
 * @author kobayashi
 * @since 2015/6/12
 * @version 1.0
 */
double lfRotatedHyperEllipsoid( double *plfX, int iVectorLen )
{
	int i,j;
	double lfRes = 0.0;
	for( i = 0;i < iVectorLen; i++ )
	{
		for( j = 0;j < i; j++ )
		{
			lfRes += plfX[i]*plfX[i];
		}
	}
	return lfRes;
}

/**
 * <PRE>
 * 　目的関数のMoved axis parallel hyper-ellipsoid関数の計算を実行します。
 *   大域的最適解 x_{i} = 5i のときf(x_{i})=0 (-5.12 <= x_{i} <= 5.12)
 * 　\sum{i=1}^{n}(5*i*x_{i}^{2})
 * </PRE>
 * @param plfX			引数
 * @param iVectorLen	引数の次元数
 * @author kobayashi
 * @since 2015/6/12
 * @version 1.0
 */
double lfMovedAxisParallelHyperEllipsoid( double *plfX, int iVectorLen )
{
	int i;
	double lfRes = 0.0;
	for( i = 0;i < iVectorLen; i++ )
	{
		lfRes += 5.0*(double)i*plfX[i]*plfX[i];
	}
	return lfRes;
}

/**
 * <PRE>
 * 　目的関数のSum of different power関数の計算を実行します。
 *   大域的最適解 x_{i}=0 のとき f(x_{i})=0
 *   \sum_{i=1}^{n}|x_{i}|^{i+1}
 * </PRE>
 * @param plfX			引数
 * @param iVectorLen	引数の次元数
 * @author kobayashi
 * @since 2015/6/12
 * @version 1.0
 */
double lfSumOfDifferentPower( double *plfX, int iVectorLen )
{
	int i;
	double lfRes = 0.0;
	for( i = 0;i < iVectorLen; i++ )
	{
		lfRes += fabs( pow(plfX[i], i+1 ) );
	}
	return lfRes;
}

/**
 * <PRE>
 * 　目的関数のRosenbrock(2nd De Jong's function)関数の計算を実行します。
 * </PRE>
 * @param plfX			引数
 * @param iVectorLen	引数の次元数
 * @author kobayashi
 * @since 2015/6/6
 * @version 1.0
 */
double lfRosenbrock( double *plfX, int iVectorLen )
{
	int i;
	double lfRes = 0.0;
	double lfTempX1 = 0.0;
	double lfTempX2 = 0.0;
	double lfXX = 0.0;
	for( i = 0;i < iVectorLen-1; i++ )
	{
		lfXX = plfX[i]*plfX[i];
		lfTempX1 = 1.0-plfX[i];
		lfTempX2 = plfX[i+1]-lfXX;
		lfRes += (100*lfTempX2*lfTempX2+lfTempX1*lfTempX1);
	}
	return lfRes;
}

/**
 * <PRE>
 * 　目的関数のRosenbrockStar型関数の計算を実行します。
 * </PRE>
 * @param plfX			引数
 * @param iVectorLen	引数の次元数
 * @author kobayashi
 * @since 2016/8/24
 * @version 0.1
 */
double lfRosenbrockStar( double *plfX, int iVectorLen )
{
	int i;
	double lfRes = 0.0;
	double lfTempX1 = 0.0;
	double lfTempX2 = 0.0;
	double lfXX = 0.0;
	for( i = 1;i < iVectorLen; i++ )
	{
		lfXX = plfX[i]*plfX[i];
		lfTempX1 = 1.0-plfX[i];
		lfTempX2 = plfX[0]-lfXX;
		lfRes += (100*lfTempX2*lfTempX2+lfTempX1*lfTempX1);
	}
	return lfRes;
}

/**
 * <PRE>
 * 　目的関数の3rd De Jong's関数の計算を実行します。
 *   大域的最適解 Xi = 1 のときf(Xi) = 0
 * </PRE>
 * @param plfX			引数
 * @param iVectorLen	引数の次元数
 * @author kobayashi
 * @since 2015/6/12
 * @version 1.0
 */
double lf3rdDeJongsFunc( double *plfX, int iVectorLen )
{
	int i;
	double lfRes = 0.0;
	using namespace std;
	for( i = 0;i < iVectorLen; i++ )
	{
		lfRes += fabs( plfX[i] );
	}
	return lfRes;
}

/**
 * <PRE>
 * 　目的関数のModified 3rd De Jong's関数の計算を実行します。
 *   大域的最適解 Xi = 1 のときf(Xi) = 0
 * </PRE>
 * @param plfX			引数
 * @param iVectorLen	引数の次元数
 * @author kobayashi
 * @since 2015/6/12
 * @version 1.0
 */
double lfModified3rdDeJongsFunc( double *plfX, int iVectorLen )
{
	int i;
	double lfRes = 0.0;
	double lfConst = 30.0;
	for( i = 0;i < iVectorLen; i++ )
	{
		lfRes += floor( plfX[i] );
	}
	lfRes = lfRes + lfConst;
	return lfRes;
}

/**
 * <PRE>
 * 　目的関数の4th DeJong's Function関数の計算を実行します。
 *   大域的最適解 Xi = 1 のときf(Xi) = 0
 * </PRE>
 * @param plfX			引数
 * @param iVectorLen	引数の次元数
 * @author kobayashi
 * @since 2015/6/12
 * @version 1.0
 */
double lf4thDeJongsFunc( double *plfX, int iVectorLen )
{
	int i;
	double lfRes = 0.0;
	for( i = 0;i < iVectorLen; i++ )
	{
		lfRes += (double)i*plfX[i]*plfX[i]*plfX[i]*plfX[i];
	}
	return lfRes;
}

/**
 * <PRE>
 * 　目的関数のModified 4th DeJong's Function関数の計算を実行します。
 *   大域的最適解 Xi = 1 のときf(Xi) = 0
 * </PRE>
 * @param plfX			引数
 * @param iVectorLen	引数の次元数
 * @author kobayashi
 * @since 2015/10/16
 * @version 1.0
 */
double lfModified4thDeJongsFunc( double *plfX, int iVectorLen )
{
	int i;
	double lfRes = 0.0;
	for( i = 0;i < iVectorLen; i++ )
	{
		lfRes += (double)i*plfX[i]*plfX[i]*plfX[i]*plfX[i] + rnd();
	}
	return lfRes;
}

/**
 * <PRE>
 * 　目的関数のDe Jong's f5関数の計算を実行します。
 * </PRE>
 * @param plfX			引数
 * @param iVectorLen	引数の次元数
 * @author kobayashi
 * @since 2015/6/6
 * @version 1.0
 */
double lf5thDeJongsFunc( double *plfX, int iVectorLen )
{
	double lfRes = 0.0;
	double alfA[2][25] = {{-32,-16,  0, 16, 32,-32,-16,  0, 16, 32,-32,-16, 0, 16, 32, -32, -16,  0, 16, 32, -32, -16,  0, 16, 32},
						  {-32,-32,-32,-32,-32,-16,-16,-16,-16,-16,  0,  0, 0,  0,  0,  16,  16, 16, 16, 16,  32,  32, 32, 32, 32}};
	int i;

	for( i = 0;i < iVectorLen; i++ )
	{
//		lfTemp1 = pow( plfX[i]-
	}
//	lfRes = 1.0/500.0 + 
	return lfRes;
}

/**
 * <PRE>
 * 　目的関数のAckley Function関数の計算を実行します。
 *   大域的最適解 Xi = 0 のときf(Xi) = 0
 * </PRE>
 * @param plfX			引数
 * @param iVectorLen	引数の次元数
 * @author kobayashi
 * @since 2015/10/16
 * @version 1.0
 */
double lfAckley( double *plfX, int iVectorLen )
{
	int i;
	double lfRes = 0.0;
	double lfCos = 0.0;
	double lfX2 = 0.0;
	double lfE = exp(1.0);

	for( i = 0;i < iVectorLen; i++ )
	{
		lfX2 += plfX[i]*plfX[i];
		lfCos += cos( 2.0*pi*plfX[i] );
	}
	lfX2 = lfX2 / (double)iVectorLen;
	lfCos = lfCos / (double)iVectorLen;

	lfRes = 20.0-20.0*exp(-0.2*sqrt(lfX2)) + lfE - exp( lfCos );
	return lfRes;
}

/**
 * <PRE>
 * 　目的関数のEasom's Function関数の計算を実行します。
 *   大域的最適解 Xi = pi のときf(Xi) = -1
 * </PRE>
 * @param plfX			引数
 * @param iVectorLen	引数の次元数
 * @author kobayashi
 * @since 2015/10/16
 * @version 1.0
 */
double lfEasoms( double *plfX, int iVectorLen )
{
	int i;
	double lfRes = 0.0;
	double lfCos = 0.0;
	double lfX2 = 0.0;
	double lfE = 0.0;

	for( i = 0;i < iVectorLen; i++ )
	{
		lfX2 += plfX[i]*plfX[i];
		lfCos += cos( 2.0*pi*plfX[i] );
	}
	lfX2 = lfX2 / (double)iVectorLen;
	lfCos = lfCos / (double)iVectorLen;

	lfRes = -20.0*exp(-0.2*sqrt(lfX2) - exp( lfCos )) + 20.0 + lfE;
	return lfRes;
}


/**
 * <PRE>
 * 　目的関数のEasom's Function(Xin-She Yang extended in 2008 this function to n dimensions)の計算を実行します。
 *   大域的最適解 x_{i}=pi のとき f(x_{i}) = -1, -2π<=x_{i}<=2π
 * </PRE>
 * @param plfX			引数
 * @param iVectorLen	引数の次元数
 * @author kobayashi
 * @since 2015/10/16
 * @version 1.0
 */
double lfExtendEasoms( double *plfX, int iVectorLen )
{
	int i;
	double lfRes = 0.0;
	double lfProductRes = 1.0;
	double lfAddRes = 0.0;

	for( i = 0;i < iVectorLen; i++ )
	{
		lfProductRes *= cos( plfX[i] )*cos( plfX[i] );
		lfAddRes += ( plfX[i]-pi )*( plfX[i]-pi );
	}
	lfRes = -1.0*lfProductRes*exp( -lfAddRes );
	return lfRes;
}
/**
 * <PRE>
 * 　目的関数のEquality-Constrained 関数の計算を実行します。
 *   大域的最適解 Xi = 1.0/\sqrt(n) のときf(Xi) = -1 (0 <= Xi <= 1.0)
 * </PRE>
 * @param plfX			引数
 * @param iVectorLen	引数の次元数
 * @author kobayashi
 * @since 2015/6/6
 * @version 1.0
 */
double EqualityConstrained( double *plfX, int iVectorLen )
{
	int i;
	double lfRes  = 1.0;

	for( i = 0; i < iVectorLen; i++ )
	{
		lfRes *= sqrt((double)iVectorLen)*plfX[i];
		lfRes *= -1.0;
	}
	return lfRes;
}


/**
 * <PRE>
 * 　目的関数のGriewank関数の計算を実行します。
 *   大域的最適解 Xi = 0 のときf(Xi) = 0 (-600 <= Xi <= 600)
 * </PRE>
 * @param plfX			引数
 * @param iVectorLen	引数の次元数
 * @author kobayashi
 * @since 2015/6/6
 * @version 1.0
 */
double lfGriewank( double *plfX, int iVectorLen )
{
	int i;
	double lfRes  = 0.0;
	double lfRes1 = 0.0;
	double lfRes2 = 1.0;

	for( i = 0; i < iVectorLen; i++ )
	{
		lfRes1 += plfX[i]*plfX[i];
		lfRes2 *= cos(plfX[i]/sqrt((double)i+1));
	}
	lfRes = 1.0 + 1.0/4000.0*lfRes1 - lfRes2;
	return lfRes;
}

/**
 * <PRE>
 * 　目的関数のMichaelwicz's関数の計算を実行します。
 *   大域的最適解 Xi = (X1, X2) = (2.20319, 1.57049) のときf(Xi) = -1.8013 (-65536 <= Xi <= 65536)
 * </PRE>
 * @param plfX			引数
 * @param iVectorLen	引数の次元数
 * @author kobayashi
 * @since 2015/6/6
 * @version 1.0
 */
double lfMichaelwicz( double *plfX, int iVectorLen )
{
	int i;
	int m;
	double lfRes = 0.0;

	m = 10;
	for( i = 0; i < iVectorLen; i++ )
	{
		lfRes += sin(plfX[i])*pow(sin((double)i*plfX[i]*plfX[i]/pi),2.0*m);
	}
	return lfRes;
}

/**
 * <PRE>
 * 　目的関数のKatsuura's関数の計算を実行します。
 *   大域的最適解 Xi = 0 のときf(Xi) = 1 (-1000 <= Xi <= 1000)
 * </PRE>
 * @param plfX			引数
 * @param iVectorLen	引数の次元数
 * @author kobayashi
 * @since 2015/6/6
 * @version 1.0
 */
double lfKatsuura( double *plfX, int iVectorLen )
{
	int i,j;
	int M;
	double lfRes = 0.0;
	double lfRes1 = 0.0;
	double lfRes2 = 0.0;
	double lfCurK = 0.0;
	double lfPrevK = 0.0;

	M = 32;
	for( i = 0; i < iVectorLen; i++ )
	{
		for( j = 0;j < M; j++ )
		{ 
			lfCurK = 2.0*lfPrevK;
			lfRes1 += floor(lfCurK*plfX[i])*1.0/lfCurK;
			lfPrevK = lfCurK;
		}
		lfRes2 += (1.0+(i+1.0)*lfRes1);
	}
	lfRes = lfRes2;
	return lfRes;
}

/**
 * <PRE>
 * 　目的関数のRastrigin関数の計算を実行します。
 * 　大域的最適解 Xi = 0 f(Xi) = 0 (-5.12 <= Xi <= 5.12)
 * </PRE>
 * @param plfX			引数
 * @param iVectorLen	引数の次元数
 * @author kobayashi
 * @since 2015/6/17
 * @version 1.0
 */
double lfRastrigin( double *plfX, int iVectorLen )
{
	int i;
	double lfRes = 0.0;
	for( i = 0;i < iVectorLen; i++ )
	{
		lfRes += ( plfX[i]*plfX[i]-10*cos(2.0*pi*plfX[i])+10.0 );
	}
	return lfRes;
}

/**
 * <PRE>
 * 　目的関数のRastriginShift関数の計算を実行します。
 * 　大域的最適解 Xi = 1 f(Xi) = 0 (-5.12 <= Xi <= 5.12)
 * </PRE>
 * @param plfX			引数
 * @param iVectorLen	引数の次元数
 * @author kobayashi
 * @since 2016/8/24
 * @version 0.1
 */
double lfRastriginShift( double *plfX, int iVectorLen )
{
	int i;
	double lfXX;
	double lfRes = 0.0;
	for( i = 0;i < iVectorLen; i++ )
	{
		lfXX = 1.0-plfX[i];
		lfRes += ( lfXX*lfXX-10*cos(2.0*pi*lfXX)+10.0 );
	}
	return lfRes;
}

/**
 * <PRE>
 * 　目的関数のSchwefel's 関数の計算を実行します。
 * 　大域的最適解 x_{i}=420.09687 f(x_{i})=-418.9829n (-512<=x_{i}<=512)
 *   \sum^{n}_{i=1}(x_{i}\sin\sqr(|x_{i}|))
 * </PRE>
 * @param plfX			引数
 * @param iVectorLen	引数の次元数
 * @author kobayashi
 * @since 2015/6/17
 * @version 1.0
 */
double lfSchwefel( double *plfX, int iVectorLen )
{
	int i;
	double lfRes = 0.0;
	for( i = 0;i < iVectorLen; i++ )
	{
		lfRes += ( plfX[i]*sin(sqrt(fabs(plfX[i]))) );
	}
	return 418.9829*iVectorLen-lfRes;
}

/**
 * <PRE>
 * 　目的関数のSix-hump camel back 関数の計算を実行します。
 * 　大域的最適解 (x_{1},x_{2})=420.09687 f(x_{1},x_{2})=-1.0316 (-3<=x_{1}<=3, -2<=x_{2}<=2)
 *   \sum^{n}_{i=1}(x_{i}\sin\sqr(|x_{i}|))
 * </PRE>
 * @param plfX			引数
 * @param iVectorLen	引数の次元数
 * @author kobayashi
 * @since 2015/6/17
 * @version 1.0
 */
double lfSixHumpCamelBack( double *plfX, int iVectorLen )
{
	double lfRes = 0.0;
	lfRes = ( 4.0-2.1*plfX[0]*plfX[0]+1.0/3.0*plfX[0]*plfX[0]*plfX[0]*plfX[0] )*plfX[0]*plfX[0] + plfX[0]*plfX[1] + 4.0*(plfX[1]*plfX[1]-1.0)*plfX[1]*plfX[1];
	return -lfRes;
}

/**
 * <PRE>
 * 　目的関数のShubert's 関数の計算を実行します。(2次元関数)
 * 　大域的最適解 (x_{1},x_{2})=-186.7309 f(x_{1},x_{2})=-1.0316 (-3<=x_{1}<=3, -2<=x_{2}<=2)
 *   \sum^{n}_{i=1}(i*\cos(i+(i+1x))*\sum^{n}_{i=1}(i*\cos(i+(i+1x))
 * </PRE>
 * @param plfX			引数
 * @param iVectorLen	引数の次元数
 * @author kobayashi
 * @since 2015/6/17
 * @version 1.0
 */
double lfShubert( double *plfX, int iVectorLen )
{
	int i;
	int n;
	double lfResX = 0.0;
	double lfResY = 0.0;
	n = 5;

	for( i = 0;i < n; i++ )
	{
		lfResX += (double)i*cos(i+(i+1)*plfX[0]);
		lfResY += (double)i*cos(i+(i+1)*plfX[1]);
	}
	return lfResX*lfResY;
}

/**
 * <PRE>
 * 　目的関数のGoldstein-Price's 関数の計算を実行します。(2次元関数)
 * 　大域的最適解 (x_{1},x_{2})=3 f(x_{1},x_{2})=(0,1) (-2<=x_{1}<=2, -2<=x_{2}<=2)
 * </PRE>
 * @param plfX			引数
 * @param iVectorLen	引数の次元数
 * @author kobayashi
 * @since 2015/6/17
 * @version 1.0
 */
double lfGoldsteinPrice( double *plfX, int iVectorLen )
{
	double lfRes = 0.0;
	double lfX2_1,lfX2_2,lfX2_3,lfX2_4,lfX2_5;

	lfX2_1 = (plfX[0]+plfX[1]+1);
	lfX2_1 *= lfX2_1;
	lfX2_2 = (2*plfX[0]-3*plfX[1]);
	lfX2_2 *= lfX2_2;
	lfX2_3 = plfX[0]*plfX[0];
	lfX2_4 = plfX[1]*plfX[1];
	lfX2_5 = plfX[0]*plfX[1];

	lfRes = ( 1.0+lfX2_1*( 19.0-14.0*plfX[0]+3*lfX2_3-14*plfX[1]+6*lfX2_5+3*lfX2_4 ) ) * ( 30.0+lfX2_2*( 18.0-32.0*plfX[0]+12.0*lfX2_3+48.0*plfX[1]-36.0*lfX2_5+27.0*lfX2_2) );

	return lfRes;
}

/**
 * <PRE>
 * 　目的関数のBranins's rcos 関数の計算を実行します。(2次元関数)
 * 　大域的最適解 (x_{1},x_{2})=0.397887 f(x_{1},x_{2})=(-π,12.275), (π,2.275) (9.42478,2.475)
 * </PRE>
 * @param plfX			引数
 * @param iVectorLen	引数の次元数
 * @author kobayashi
 * @since 2015/6/17
 * @version 1.0
 */
double lfBraninsRCos( double *plfX, int iVectorLen )
{
	double lfRes = 0.0;
	double lfX2_1;
	double lfA,lfB,lfC,lfD,lfE,lfF;

	lfA = 1.0;
	lfB = 5.1/(4.0*pi*pi);
	lfC = 5.0/pi;
	lfD = 6.0;
	lfE = 10.0;
	lfF = 1.0/(8.0*pi);
	lfX2_1 = plfX[1]-lfB*plfX[0]*plfX[0]+lfC*plfX[1]-lfD;
	lfX2_1 *= lfX2_1;

	lfRes = lfA*lfX2_1 + lfE*(1.0-lfF)*cos(plfX[1]) + lfE;

	return lfRes;
}

/**
 * <PRE>
 * 　目的関数のLangermann's 関数の計算を実行します。
 *	 \sum^{m}_{i=1}c_{i}*\exp( -\dfrac{1}{\pi}\sum^{n}_{j=1}(x_{j}-a_{ij})^{2} )cos(\pi\sum^{n}_{j=1}(x_{j}-a_{ij})^{2})
 * 　大域的最適解 -5.12 \leq X_{1}, X_{2} \leq 5.12
 *   M の値は推奨値が5とされている。
 * </PRE>
 * @param plfX			引数
 * @param iVectorLen	引数の次元数
 * @author kobayashi
 * @since 2015/6/17
 * @version 1.0
 */
double lfLangermann( double *plfX, int iVectorLen )
{
	int i,j;
	int M = 5;
	double lfRes1 = 0.0;
	double lfRes2 = 0.0;
	double lfRes = 0.0;
	double pplfA[2][5] = {{3,5,2,1,7},{5,2,1,4,9}};
	double plfC[5] = {1,2,5,2,3};

	for( i = 0;i < M; i++ )
	{
		for( j = 0;j < iVectorLen; j++ )
		{
			lfRes1 += (plfX[j]-pplfA[i][j])*(plfX[j]-pplfA[i][j]);
		}
		lfRes2 = lfRes1;
		lfRes1 *= -1.0/pi;
		lfRes2 *= pi;
		lfRes += plfC[i]*exp(lfRes1)*cos(lfRes2);
	}
	return lfRes;
}

/**
 * <PRE>
 * 　目的関数のDrop wave 関数の計算を実行します。(2次元関数)
 *	 ( 1.0+\cos(12*\sqrt{X_{1}*X_{1}+X_{2}*X_{2}) )/( 1/2*(X_{1}*X_{1}+X_{2}*X_{2} )+2 )
 * 　大域的最適解 -5.12 \leq X_{1}, X_{2} \leq 5.12
 * </PRE>
 * @param plfX			引数
 * @param iVectorLen	引数の次元数
 * @author kobayashi
 * @since 2015/6/17
 * @version 1.0
 */
double lfDropWave( double *plfX, int iVectorLen )
{
	double lfRes = 0.0;

	if( iVectorLen == 2 )
	{	
		lfRes = ( 1.0+cos(12.0*sqrt(plfX[0]*plfX[0]+plfX[1]*plfX[1]) ) )/( 0.5*( plfX[0]*plfX[0]+plfX[1]*plfX[1] )+2.0 );
	}
	else
	{
		lfRes = 0.0;
	}
	return lfRes;
}

/**
 * <PRE>
 * 　目的関数のShekel's Foxholes 関数の計算を実行します。
 *	 -\sum^{m}_{i=1}( \sum^{n}_{j=1}(x_{j}-a_{ij})^{2} + c_{i} ) - 1
 * 　大域的最適解 -5.12 \leq X_{1}, X_{2} \leq 5.12
 *   M の値は推奨値が30とされている。
 * </PRE>
 * @param plfX			引数
 * @param iVectorLen	引数の次元数
 * @author kobayashi
 * @since 2015/6/17
 * @version 1.0
 */
double lfShekelsFoxholes( double *plfX, int iVectorLen )
{
	int i,j;
	int M = 30;
	double lfRes1 = 0.0;
	double lfRes2 = 0.0;
	double lfRes = 0.0;
	double pplfA[2][5] = {{3,5,2,1,7},{5,2,1,4,9}};
	double plfC[5] = {1,2,5,2,3};

	for( i = 0;i < M; i++ )
	{
		for( j = 0;j < iVectorLen; j++ )
		{
			lfRes1 += (plfX[j]-pplfA[i][j])*(plfX[j]-pplfA[i][j]) + plfC[i];
		}
	}
	lfRes -= 1.0;
	return lfRes;
}

/**
 * <PRE>
 * 　目的関数のPaviani's Foxholes 関数の計算を実行します。
 *	 -\sum^{m}_{i=1}( \sum^{n}_{j=1}(x_{j}-a_{ij})^{2} + c_{i} ) - 1
 * 　大域的最適解 -5.12 \leq X_{1}, X_{2} \leq 5.12
 *   M の値は推奨値が30とされている。
 * </PRE>
 * @param plfX			引数
 * @param iVectorLen	引数の次元数
 * @author kobayashi
 * @since 2015/6/17
 * @version 1.0
 */
double lfPavianiFoxholes( double *plfX, int iGenVector )
{
	double lfRes1 = 0.0;
	double lfRes2 = 0.0;
	double lfRes11 = 0.0;
	double lfRes12 = 1.0;
	int i;

	for( i = 0;i < iGenVector; i++ )
	{
		lfRes1 = log( plfX[i]-2 );
		lfRes1 *= lfRes1;
		lfRes2 = log( 10.0-plfX[i] );
		lfRes2 *= lfRes2;
		lfRes11 += lfRes1 + lfRes2;
	}
	for( i = 0;i < iGenVector; i++ )
	{
		lfRes12 *= plfX[i];
	}
	lfRes12 = pow( lfRes12, 0.2 );
	return lfRes11 - lfRes12;
}

/**
 * <PRE>
 * 　目的関数のSine envelope sine wave 関数の計算を実行します。
 *	 -\sum^{m}_{i=1}( \sum^{n}_{j=1}(x_{j}-a_{ij})^{2} + c_{i} ) - 1
 * 　大域的最適解 -5.12 \leq X_{1}, X_{2} \leq 5.12
 *   M の値は推奨値が30とされている。
 * </PRE>
 * @param plfX			引数
 * @param iVectorLen	引数の次元数
 * @author kobayashi
 * @since 2015/6/17
 * @version 1.0
 */
double lfSineEnvelopeSineWave( double *plfX, int iGenVector )
{
	double lfRes = 0.0;
	double lfDist = 0.0;
	double lfDist2 = 0.0;
	double lfSin = 0.0;
	int i;

	for( i = 0;i < iGenVector; i++ )
	{
		lfDist = plfX[i]*plfX[i]+plfX[i+1]*plfX[i+1];
		lfSin = sin( sqrt(lfDist) - 0.5 );
		lfDist = (0.001*lfDist+1.0);
		lfRes += (lfSin*lfSin)/(lfDist*lfDist) + 0.5;
	}
	return lfRes;
}

/**
 * <PRE>
 * 　目的関数のEgg Hloder 関数の計算を実行します。
 *	 -\sum^{m}_{i=1}( \sum^{n}_{j=1}(x_{j}-a_{ij})^{2} + c_{i} ) - 1
 * 　大域的最適解 -512 \leq X_{1}, X_{2} \leq 512
 *   M の値は推奨値が30とされている。
 * </PRE>
 * @param plfX			引数
 * @param iVectorLen	引数の次元数
 * @author kobayashi
 * @since 2015/6/17
 * @version 1.0
 */
double lfEggHolder( double *plfX, int iGenVector )
{
	double lfRes = 0.0;
	double lfDist = 0.0;
	double lfDist2 = 0.0;
	double lfSin = 0.0;
	double lfXi = 0.0;
	int i;

	for( i = 0;i < iGenVector-1; i++ )
	{
		lfXi = plfX[i+1]+47.0;
		lfRes += -lfXi + sqrt( lfXi+0.5*plfX[i] )+-plfX[i]*sin( sqrt(fabs( plfX[i]-lfXi )) );
	}
	return lfRes;
}

/**
 * <PRE>
 * 　目的関数のRana's 関数の計算を実行します。
 *	 -(x_{2}-47)\sin{sqrt(x_{2}+dfrac{x_{1}}{2}+47))-x_{1}\sin{\sqrt{\abs{x_{1}-(x_{2}+47)}}}
 * 　大域的最適解 -d^2 \leq X_{1}, X_{2} \leq d^2
 *   f(x)=-50, x= -200
 * </PRE>
 * @param plfX			引数
 * @param iVectorLen	引数の次元数
 * @author kobayashi
 * @since 2015/6/17
 * @version 1.0
 */
double lfRana( double *plfX, int iGenVector )
{
	double lfRes = 0.0;
	double lfX11 = 0.0;
	double lfX12 = 0.0;
	int i;

	for( i = 1;i < iGenVector-1; i++ )
	{
		lfX11 = plfX[i]+1;
		lfX12 = -plfX[i]+1;
		lfRes = lfX11*cos(sqrt(fabs(plfX[i+1]+lfX12)))*sin(sqrt(fabs(plfX[i+1]+lfX11)))+plfX[i]*cos(sqrt(fabs(plfX[i+1]+lfX11)))*sin(fabs(plfX[i+1]+lfX12));
	}
	return lfRes;
}


/**
 * <PRE>
 * 　目的関数のTrid 関数の計算を実行します。
 *	 -(x_{2}-47)\sin{sqrt(x_{2}+dfrac{x_{1}}{2}+47))-x_{1}\sin{\sqrt{\abs{x_{1}-(x_{2}+47)}}}
 * 　大域的最適解 -d^2 \leq X_{1}, X_{2} \leq d^2
 *   f(x)=-50, x= -200
 * </PRE>
 * @param plfX			引数
 * @param iVectorLen	引数の次元数
 * @author kobayashi
 * @since 2015/6/17
 * @version 1.0
 */
double lfTrid( double *plfX, int iGenVector )
{
	double lfRes = 0.0;
	double lfRes1 = 0.0;
	double lfRes2 = 0.0;
	double lfX12 = 0.0;
	int i;

	for( i = 1;i < iGenVector; i++ )
	{
		lfX12 = (plfX[i]-1);
		lfX12 *= lfX12;
		lfRes1 += lfX12;
		lfRes2 += plfX[i]*plfX[i-1];
	}
	lfRes1 += (plfX[0]-1)*(plfX[0]-1);
	lfRes = lfRes1-lfRes2;
	return lfRes;
}

/**
 * <PRE>
 * 　目的関数のk-tablet関数の計算を実行します。
 *	 -\sum^{k}_{i=1}x_{i}^2+\sum^{n}_{i=k+1}(100x_{i})^{2}
 * 　大域的最適解 -5.12 \leq X_{1}, X_{2} \leq 5.12 x = (0,0,0,0...,0)
 *   k=n/4
 * </PRE>
 * @param plfX			引数
 * @param iVectorLen	引数の次元数
 * @author kobayashi
 * @since 2016/8/24
 * @version 0.1
 */
double lfkTablet( double *plfX, int iGenVector )
{
	double lfRes = 0.0;
	double lfXX = 0.0;
	int i;
	int k;

	k = iGenVector/4;

	for( i = 0;i < k; i++ )
	{
		lfRes += plfX[i]*plfX[i];
	}
	for( i = k;i < iGenVector; i++ )
	{
		lfXX = 100.0*plfX[i];
		lfRes += lfXX*lfXX;
	}
	return lfRes;
}

/**
 * <PRE>
 * 　目的関数のSchaffer関数の計算を実行します。
 *	 -\sum^{n-1}_{i=1}(x_{i}^2+(x_{i+1})^{2})^0.25*(\sin^{2}(50(x_{i}^{2}+x_{i+1}^{2})^{0.1})+1.0)
 * 　大域的最適解 -100 \leq X_{1}, X_{2} \leq 100 x = (0,0,0,0...,0)
 *   k=n/4
 * </PRE>
 * @param plfX			引数
 * @param iVectorLen	引数の次元数
 * @author kobayashi
 * @since 2016/9/14
 * @version 0.1
 */
double lfSchaffer( double *plfX, int iGenVector )
{
	double lfRes = 0.0;
	double lfXX = 0.0;
	double lfSquare = 0.0;
	double lfSin = 0.0;
	int i;
	int k;

	for( i = 0;i < iGenVector-1; i++ )
	{
		lfSquare = plfX[i]*plfX[i]+plfX[i+1]*plfX[i+1];
		lfSin = sin(pow(50.0*lfSquare,0.1));
		lfRes += pow(lfSquare, 0.25)*(lfSin*lfSin+1.0);
	}
	return lfRes;
}

/**
 * <PRE>
 * 　目的関数のBohachevsky関数の計算を実行します。
 *	 -\sum^{n-1}_{i=1}(x_{i}^2+2x_{i+1}^{2}-0.3\cos(3\pi x_{i})-0.4cos(4\pi x_{i+1} + 0.7)
 * 　大域的最適解 -5.12 \leq x_{i} \leq 5.12 x = (0,0,0,0...,0)
 * </PRE>
 * @param plfX			引数
 * @param iVectorLen	引数の次元数
 * @author kobayashi
 * @since 2016/9/14
 * @version 0.1
 */
double lfBohachevsky( double *plfX, int iGenVector )
{
	double lfRes = 0.0;
	double lfXX1 = 0.0;
	double lfXX2 = 0.0;
	double lfCos1 = 0.0;
	double lfCos2 = 0.0;
	double lf3pi,lf4pi;
	int i;

	lf3pi = 3.0*pi;
	lf4pi = 4.0*pi;
	for( i = 0;i < iGenVector-1; i++ )
	{
		lfXX1 = plfX[i]*plfX[i];
		lfXX2 = 2.0*plfX[i+1]*plfX[i+1];
		lfCos1 = 0.3*cos(lf3pi*plfX[i]);
		lfCos2 = 0.4*cos(lf4pi*plfX[i+1]);
		lfRes += lfXX1+lfXX2-lfCos1-lfCos2;
	}
	return lfRes;
}
