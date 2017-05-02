#pragma once
#ifndef _CONSTRAINT_CONDITION_H_
#define _CONSTRAINT_CONDITION_H_

/**
* <PRE>
*   NEDOCS�p�������
*   �Z�o���q�ɑ΂��ă��~�b�^�[�������܂��B
*   NEDOCS�̊e�ϐ��͐��ł���K�v�����邽�߁B
* </PRE>
* @param plfX			����
* @param iVectorLen	�����̎�����
* @author kobayashi
* @since 2017/03/08
* @version 0.1
*/
extern void vNedocsLimitter(double *plfX, int iGenVector);

/**
* <PRE>
*   ������������������܂���B
* </PRE>
* @param plfX		����
* @param iVectorLen	�����̎�����
* @author kobayashi
* @since 2017/03/08
* @version 0.1
*/
extern void vNone(double *plfX, int iGenVector);

/**
* <PRE>
*   �~�}����p�̈�ʓI�Ȑ�������ݒ�
* </PRE>
* @param plfArg		����
* @param iGenVector	�����̎�����
*/
extern void vSetEDCalibrationCondition(double* plfArg, int iGenVector);

#endif /*_CONSTRAINT_CONDITION_H_*/