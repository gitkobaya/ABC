#pragma once
#ifndef _CONSTRAINT_CONDITION_H_
#define _CONSTRAINT_CONDITION_H_

/**
* <PRE>
*   NEDOCS用制約条件
*   算出粒子に対してリミッターをかけます。
*   NEDOCSの各変数は正である必要があるため。
* </PRE>
* @param plfX			引数
* @param iVectorLen	引数の次元数
* @author kobayashi
* @since 2017/03/08
* @version 0.1
*/
extern void vNedocsLimitter(double *plfX, int iGenVector);

/**
* <PRE>
*   制約条件を何もかけません。
* </PRE>
* @param plfX		引数
* @param iVectorLen	引数の次元数
* @author kobayashi
* @since 2017/03/08
* @version 0.1
*/
extern void vNone(double *plfX, int iGenVector);

/**
* <PRE>
*   救急部門用の一般的な制約条件設定
* </PRE>
* @param plfArg		引数
* @param iGenVector	引数の次元数
*/
extern void vSetEDCalibrationCondition(double* plfArg, int iGenVector);

#endif /*_CONSTRAINT_CONDITION_H_*/