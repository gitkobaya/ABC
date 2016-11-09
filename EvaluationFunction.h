#ifndef _EVALUATION_FUNCTION_H_
#define _EVALUATION_FUNCTION_H_

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////評価関数

/**
 * <PRE>
 * 　目的関数のSphere(1st De Jong's function)関数の計算を実行します。
 * 　-5.12 <= x_i <= 5.12 f_i(x_i)=0,  x_i=0, i = 1,2,･･･,n
 * 　f(x) = sum(x_{i}^{2})
 *   ver 0.1 初期バージョン
 *   ver 0.2 2016/9/27 関数計算の最適化
 * </PRE>
 * @param plfX			引数
 * @param iVectorLen	引数の次元数
 * @author kobayashi
 * @since 2015/6/6
 * @version 0.2
 */
extern double lfSphere( double *plfX, int iVectorLen );

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
extern double lfEllipsoid( double *plfX, int iVectorLen );

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
extern double lfHyperEllipsoid( double *plfX, int iVectorLen );

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
extern double lfAxisParallelHyperEllipsoid( double *plfX, int iVectorLen );

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
extern double lfRotatedHyperEllipsoid( double *plfX, int iVectorLen );

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
extern double lfMovedAxisParallelHyperEllipsoid( double *plfX, int iVectorLen );

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
extern double lfSumOfDifferentPower( double *plfX, int iVectorLen );

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
extern double lfRosenbrock( double *plfX, int iVectorLen );

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
extern double lfRosenbrockStar( double *plfX, int iVectorLen );

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
extern double lf3rdDeJongsFunc( double *plfX, int iVectorLen );

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
extern double lfModified3rdDeJongsFunc( double *plfX, int iVectorLen );

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
extern double lf4thDeJongsFunc( double *plfX, int iVectorLen );

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
extern double lfModified4thDeJongsFunc( double *plfX, int iVectorLen );

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
extern double lf5thDeJongsFunc( double *plfX, int iVectorLen );

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
extern double lfAckley( double *plfX, int iVectorLen );

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
extern double lfEasoms( double *plfX, int iVectorLen );

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
extern double lfExtendEasoms( double *plfX, int iVectorLen );

/**
 * <PRE>
 * 　目的関数のEquality-Constrained 関数の計算を実行します。
 *   大域的最適解 Xi = 1.0/\sqrt(n) のときf(Xi) = -1 (0 <= Xi <= 1.0)
 *   ver 0.1 初期バージョン
 *   ver 0.2 2016/9/27 関数に誤りがあり修正
 * </PRE>
 * @param plfX			引数
 * @param iVectorLen	引数の次元数
 * @author kobayashi
 * @since 2015/6/6
 * @version 0.1
 */
extern double lfEqualityConstrained( double *plfX, int iVectorLen );

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
extern double lfGriewank( double *plfX, int iVectorLen );

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
extern double lfMichaelwicz( double *plfX, int iVectorLen );

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
extern double lfKatsuura( double *plfX, int iVectorLen );

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
extern double lfRastrigin( double *plfX, int iVectorLen );

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
extern double lfRastriginShift( double *plfX, int iVectorLen );

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
extern double lfSchwefel( double *plfX, int iVectorLen );

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
extern double lfSixHumpCamelBack( double *plfX, int iVectorLen );

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
extern double lfLangermann( double *plfX, int iVectorLen );

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
extern double lfBraninsRCos( double *plfX, int iVectorLen );

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
extern double lfShubert( double *plfX, int iVectorLen );

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
extern double lfDropWave( double *plfX, int iVectorLen );

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
extern double lfGoldsteinPrice( double *plfX, int iVectorLen );

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
extern double lfShekelsFoxholes( double *plfX, int iVectorLen );

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
extern double lfPavianiFoxholes( double *plfX, int iGenVector );

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
extern double lfSineEnvelopeSineWave( double *plfX, int iGenVector );

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
extern double lfEggHolder( double *plfX, int iGenVector );

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
extern double lfRana( double *plfX, int iGenVector );

/**
 * <PRE>
 * 　目的関数のPathological test 関数の計算を実行します。
 *	 -\sum^{n-1}_{i=1}( \dfrac{\sin^{2}(\sqrt(x^{2}_{i+1}+100x^{2}_{i})-0.5)}{(0.001(x^{2}_{i+1}-2x_{i+1}x_{i}+x^{2}_{i} + 1))} + 0.5)
 * 　大域的最適解 -100 \leq X_{i} \leq 100
 * </PRE>
 * @param plfX			引数
 * @param iVectorLen	引数の次元数
 * @author kobayashi
 * @since 2015/9/29
 * @version 0.1
 */
extern double lfPathologicalTest( double *plfX, int iGenVector );

/**
 * <PRE>
 * 　目的関数のMaster's cosine wave 関数の計算を実行します。
 *	 \sum_{i=1}^{n-1}\exp(\dfrac{1}{8}(x_{i+1}^{2}+0.5x_{i}x_{i+1}+x_{I}^{2})\cos(4\sqrt(x_{i+1}^{2}+0.5x_{i}x_{i+1}+x_{I}^{2}))
 * 　大域的最適解 -5 \leq X_{1}, X_{2} \leq 5
 *   f(x)=-50, x= -200
 *   ver 0.1 初期バージョン
 * </PRE>
 * @param plfX			引数
 * @param iVectorLen	引数の次元数
 * @author kobayashi
 * @since 2016/9/28
 * @version 0.1
 */
extern double lfMasterCosineWave( double *plfX, int iGenVector );

/**
 * <PRE>
 * 　目的関数のKeane's 関数の計算を実行します。
 *	 -(x_{2}-47)\sin{sqrt(x_{2}+dfrac{x_{1}}{2}+47))-x_{1}\sin{\sqrt{\abs{x_{1}-(x_{2}+47)}}}
 * 　大域的最適解 0 \leq X_{1}, X_{2} 10
 *   f(x)=-50, x= -200
 *   ver 0.1 初期バージョン
 * </PRE>
 * @param plfX			引数
 * @param iVectorLen	引数の次元数
 * @author kobayashi
 * @since 2016/9/28
 * @version 0.1
 */
extern double lfKeane( double *plfX, int iGenVector );

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
extern double lfTrid( double *plfX, int iGenVector );

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
extern double lfkTablet( double *plfX, int iGenVector );

/**
 * <PRE>
 * 　目的関数のSchaffer関数の計算を実行します。
 *	 -\sum^{n-1}_{i=1}(x_{i}^2+(x_{i+1})^{2})^0.25*(\sin^{2}(50(x_{i}^{2}+x_{i+1}^{2})^{0.1})+1.0)
 * 　大域的最適解 -100 \leq X_{1}, X_{2} \leq 100 x = (0,0,0,0...,0)
 * </PRE>
 * @param plfX			引数
 * @param iVectorLen	引数の次元数
 * @author kobayashi
 * @since 2016/9/14
 * @version 0.1
 */
extern double lfSchaffer( double *plfX, int iGenVector );

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
extern double lfBohachevsky( double *plfX, int iGenVector );

/**
* <PRE>
* 　目的関数のZakharov関数の計算を実行します。
*	 -\sum^{n}_{i=1}(x_{i}^2+(sum^{n}_{i=1}(\dfrac{ix_{i}^2}{2})^{2}+(sum^{n}_{i=1}(\dfrac{ix_{i}^2}{2})^{4})
* 　大域的最適解 -5.12 \leq x_{i} \leq 5.12 x = (0,0,0,0...,0)
*   ver 0.1 初版
* </PRE>
* @param plfX			引数
* @param iVectorLen	引数の次元数
* @author kobayashi
* @since 2016/11/09
* @version 0.1
*/
extern double lfZakharov(double *plfX, int iGenVector);

/**
* <PRE>
* 　目的関数のSalomon Problem関数の計算を実行します。
*	 1-\cos(2\pi\sqrt(\sum^{n}_{i=1}(x_{i}^2)))+0.1*sqrt(\sum^{n}_{i=1}(x_{i}^2))
* 　大域的最適解 -100 \leq x_{i} \leq 100 x = (0,0,0,0...,0)
*   ver 0.1 初版
* </PRE>
* @param plfX			引数
* @param iVectorLen	引数の次元数
* @author kobayashi
* @since 2016/11/09
* @version 0.1
*/
extern double lfSalomonProblem(double *plfX, int iGenVector);

/**
* <PRE>
* 　目的関数のQuartic functionの計算を実行します。
*	 \sum^{n}_{i=1}(ix_{i}^4)+random[0,1)
* 　大域的最適解 -1.28 \leq x_{i} \leq 1.28 x = (0,0,0,0...,0)
*   ver 0.1 初版
* </PRE>
* @param plfX			引数
* @param iVectorLen	引数の次元数
* @author kobayashi
* @since 2016/11/09
* @version 0.1
*/
extern double lfQuarticFunction(double *plfX, int iGenVector);

#endif /* _EVALUATION_FUNCTION_H_ */
