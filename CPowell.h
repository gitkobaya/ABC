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
	const double lfGolden = 1.68034;	// �X�e�b�v���Ƃ̋�Ԃ̊g�嗦
	const double lfGLimit = 100.0;		// ���������Ă͂߂̃X�e�b�v�̍ő�g�嗦
	const double lfTiny = 1.0e-20;		// ���e�덷(�͂����ݖ@�Ŏg�p�B)
	const double lfCGolden = 0.3819660;	// ����������
	const int iIntervalNumber = 100;	// ������
	const double lfZepsilon = 1.0e-10;	// ���e�덷(Brent�̕��@�Ŏg�p)�B
	const double lfTol = 2.0e-4;		// ���e�덷(Powell�@�Ŏg�p
	const int iItmax = 200;
};

#endif /* _POWELL_H_*/
