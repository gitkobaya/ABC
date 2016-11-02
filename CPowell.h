#pragma once
#ifndef _POWELL_H_
#define _POWELL_H_

class CPowell
{
public:
	CPowell();
	virtual ~CPowell();

	void vInitialize();
	void vTerminate();

	/**
	*<PRE>
	* �R�[���o�b�N�֐��̒�`�ł��B
	*</PRE>
	*/
	double(*pflfObjectiveFunction)(double *pplfData, int iVectorLen);

	/**
	* <PRE>
	* �@�����������ړI�֐��̓o�^���s���܂��B
	* </PRE>
	* @param pflfObjectiveFunction �R�[���o�b�N�֐�
	* @author kobayashi
	* @since 2015/6/8
	* @version 0.1
	*/
	void vSetConstraintFunction(double(*pflfObjectiveFunction)(double *plfData, int iVectorLen));

	/**
	* <PRE>
	* �@�o�^�����R�[���o�b�N�֐��̉�@���������s���܂��B
	* </PRE>
	* @author kobayashi
	* @since 2015/6/8
	* @version 0.1
	*/
	void vReleaseCallConstraintFunction();

private:
	double *plfXt;
	double *plfXCom;
	double *plfXiCom;

	const double lfGolden = 1.68034;	// �X�e�b�v���Ƃ̋�Ԃ̊g�嗦
	const double lfGLimit = 100.0;		// ���������Ă͂߂̃X�e�b�v�̍ő�g�嗦
	const double lfTiny = 1.0e-20;		// ���e�덷(�͂����ݖ@�Ŏg�p�B)
	const double lfCGolden = 0.3819660;	// ����������
	const int iIntervalNumber = 100;	// ������
	const double lfZepsilon = 1.0e-10;	// ���e�덷(Brent�̕��@�Ŏg�p)�B
	const double lfTol = 2.0e-4;		// ���e�덷(Powell�@�Ŏg�p
};

#endif /* _POWELL_H_*/
