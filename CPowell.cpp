#include<memory>
#include<cmath>
#include"CPowell.h"

CPowell::CPowell()
{
}

CPowell::CPowell( int iVectorDimNum )
{
	vInitialize(iVectorDimNum);
}

CPowell::~CPowell()
{

}

void CPowell::vInitialize( int iVectorDimNum )
{
	int i;
	iVectorLen = iVectorDimNum;
	try
	{
		plfXt = new double[iVectorLen];
		plfXCom = new double[iVectorLen];
		plfXiCom = new double[iVectorLen];
		plfPt = new double[iVectorLen];
		plfXit = new double[iVectorLen];
		plfPtt = new double[iVectorLen];

		for (i = 0; i < iVectorLen; i++)
		{
			plfXt[i] = 0.0;
			plfXCom[i] = 0.0;
			plfXiCom[i] = 0.0;
			plfPt[i] = 0.0;
			plfXit[i] = 0.0;
			plfPtt[i] = 0.0;
		}
	}
	catch( std::bad_alloc ba )
	{

	}
}

void CPowell::vTerminate()
{
	if (plfXt != NULL)
	{
		delete[] plfXt;
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
	if (plfPt != NULL)
	{
		delete[] plfPt;
		plfPt = NULL;
	}
	if (plfXit != NULL)
	{
		delete[] plfXit;
		plfXit = NULL;
	}
	if (plfPtt != NULL)
	{
		delete[] plfPtt;
		plfPtt = NULL;
	}
}

/**
* <PRE>
* �@�ړI�֐����C���X�g�[�����܂��B
* �@���ۂɂ̓R�[���o�b�N�֐����C���X�g�[�����܂��B
* </PRE>
* @param pflfFunction �ړI�֐��̊֐��|�C���^
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
* �@�ړI�֐����A���C���X�g�[�����܂��B
* �@���ۂɂ̓R�[���o�b�N�֐����A���C���X�g�[�����܂��B
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
	for (i = 0; i < iNCom; i++)
	{
		plfXt[i] = plfXCom[i] + lfShift*plfXiCom[i];
	}
	return pflfObjectiveFunction(plfXt, iVectorLen);
}

/**
 * <PRE>
 *    �͂����ݖ@�iNumerical Recipe in C���Q�ƁB�j
 *    ver 0.1 2016/11/02 ����
 * </PRE>
 * @param
 *
 * @since 2016/11/02
 * @version 0.1
 */ 
void CPowell::vMnbrak( double *plfAx, double *plfBx, double *plfCx, double *plfFa, double *plfFb, double *plfFc )
{
	double lfUlim, lfU, lfR, lfQ, lfFu, lfDum;

	*plfFa = lfF1Dim( *plfAx );
	*plfFb = lfF1Dim( *plfBx );

	if (*plfFb > *plfFa )
	{
		lfDum = *plfAx;	*plfAx = *plfBx; *plfBx = lfDum;
		lfDum = *plfFb;	*plfFb = *plfFa; *plfFa = lfDum;
	}
	// C�̏����ݒ�
	*plfCx = *plfBx + lfGolden*(*plfBx-*plfAx);
	*plfFc = lfF1Dim( *plfCx );

	// �͂����݂ɐ�������܂ŌJ��Ԃ��B
	while( *plfFb > *plfFc )
	{
		// a, b, c�����������O��u�����߂�B
		lfR = (*plfBx - *plfAx)*(*plfFb-*plfFc);
		lfQ = (*plfBx - *plfCx)*(*plfFb-*plfFa);
		lfU = *plfBx - ((*plfBx-*plfCx)*lfQ-(*plfBx-*plfAx)*lfR)/(2.0*lfSgn( lfMax(fabs(lfQ-lfR), lfTiny), lfQ-lfR ) );
		lfUlim = *plfBx + lfGLimit*( *plfCx-*plfBx );

		// ����ȏ�i�܂��ɗl�X�ȉ\���𒲂ׂ�B
		if( (*plfBx-lfU)*(lfU-*plfCx) > 0.0 )
		{
			lfFu = lfF1Dim( lfU );
			if( lfFu < *plfFc )
			{
				*plfAx = *plfBx;
				*plfBx = lfU;
				*plfFa = *plfFb;
				*plfFb = lfFu;
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
		else if( (*plfCx-lfU)*(lfU-lfUlim) > 0.0 )
		{
			lfFu = lfF1Dim( lfU );
			if( lfFu < *plfFc )
			{
				*plfBx = *plfCx; *plfCx = lfU; lfU = *plfCx + lfGolden*( *plfCx-*plfBx );
				*plfFb = *plfFc; *plfFc = lfFu; lfFu = lfF1Dim( lfU );
			}
		}
		else if( (lfU-lfUlim)*(lfUlim-*plfCx) >= 0.0 )
		{
			lfU = lfUlim;
			lfFu = lfF1Dim( lfU );
		}
		else
		{
			lfU = *plfCx + lfGolden*(*plfCx-*plfBx);
			lfFu = lfF1Dim( lfU );
		}
		*plfAx = *plfBx; *plfBx = *plfCx; *plfCx = lfU;
		*plfFa = *plfFb; *plfFb = *plfFc; *plfFc = lfFu;
	}
}

/**
 * <PRE>
 *    Brent�̕��@
 * </PRE>
 *
 *
 *
 */ 
double CPowell::lfBrent( double lfAx, double lfBx, double lfCx, double lfTol, double *plfXMin )
{
	int iInter;
	double lfA, lfB, lfD, lfETemp, lfFu, lfFv, lfFw, lfFx, lfP, lfQ, lfR, lfTol1, lfTol2, lfU, lfV, lfW, lfX, lfXm;
	double lfE = 0.0;

	// a < b�ɂ���B
	lfA = ( lfAx < lfCx ? lfAx : lfCx);
	lfB = ( lfAx > lfCx ? lfAx : lfCx);

	// ������
	lfX = lfW = lfV = lfBx;

	lfFw = lfFv = lfFx = lfF1Dim(lfX);

	for(iInter = 1; iInter <= iIntervalNumber; iInter++)
	{
		lfXm = 0.5*(lfA+lfB);
		lfTol2 = 2.0*(lfTol1 = lfTol*fabs(lfX)+lfZepsilon);
		if( fabs(lfX-lfXm) <= ( lfTol2-0.5*(lfB-lfA)))
		{
			*plfXMin = lfX;
			return lfFx;
		}
		if( fabs( lfE ) > lfTol1 )
		{
			lfR = ( lfX-lfW)*(lfFx-lfFv);
			lfQ = ( lfX-lfV)*(lfFx-lfFw);
			lfP = ( lfX-lfV )*lfQ-(lfX-lfW)*lfR;
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
				lfD = lfP/lfQ;
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
 *   n�����̓_p[1,...n]��n�����̕���xi[1,...n]��^�����, ���̃��[�`����p�����xi�ɉ����ē�������,
 *   �֐�func(p)���ŏ��ɂȂ�悤�ɂ���. xi��p�����ۂɓ������ψʗʂŏ㏑�������. plfRet�ɂ͍ŏ��Xp�ł̊֐��l������.
 *   ���ۂ̎d���̓��[�`��mnbrak, brent���Ăяo���čs��.
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

	iNCom = iN;
	for(i = 0; i < iN; i++ )
	{
		plfXCom[i] = plfP[i];
		plfXiCom[i] = plfXi[i];
	}
	lfAx = 0.0;
	lfXX = 1.0;
	vMnbrak( &lfAx, &lfXX, &lfBx, &lfFa, &lfFx, &lfFb );
	*plfRet = lfBrent( lfAx, lfXX, lfBx, lfTol, &lfXMin );
	for( i = 0;i < iN; i++ )
	{
		plfXi[i] *= lfXMin;
		plfP[i] += plfXi[i];
	}
}

void CPowell::vPowell( double *plfP, double **pplfXi, int iN, double lfFtol, int *piInter, double *plfFRet )
{
	int i, j, iBig;
	double lfDel, lfFp, lfFptt, lfT;

	*plfFRet = pflfObjectiveFunction( plfP, iN );
	for (i = 0; i < iN; i++)
	{
		plfPt[i] = plfP[i];
	}
	for (*piInter = 1;; ++(*piInter))
	{
		lfFp = *plfFRet;
		iBig = 0;
		// �֐��l�̍ő匸���ʂ����߂�ϐ�
		lfDel = 0.0;
		// �e�����ŁA�����W���̑S�v�f�ɂ��ă��[�v
		for (i = 0; i < iN; i++)
		{
			// �������R�s�[
			for (j = 0; j < iN; j++)
			{
				plfXit[j] = pplfXi[j][i];
			}
			lfFptt = *plfFRet;
			// ����ɉ����čŏ���
			vLineMin(plfP, plfXit, iN, plfFRet);
			// �ő�̌��ۂł���΋L�^����B
			if (fabs(lfFptt - *plfFRet) > lfDel)
			{
				lfDel = fabs(lfFptt - *plfFRet);
				iBig = i;
			}
		}
		// �I������
		if (2.0*fabs(lfFp - *plfFRet) <= lfFtol*(fabs(lfFp) + fabs(*plfFRet)))
		{
			return;
		}
		if (*piInter == iItmax) break;
		for (j = 0; j < iN; j++)
		{
			plfPtt[j] = 2.0*plfP[j] - plfPt[j];
			plfXit[j] = plfP[j] - plfPt[j];
			plfPt[j] = plfP[j];
		}
		lfFptt = pflfObjectiveFunction( plfPtt, iN );
		if (lfFptt < lfFp)
		{
			lfT = 2.0*( lfFp-2.0*(*plfFRet)+lfFptt)*( lfFp-(*plfFRet)-lfDel )*(lfFp - (*plfFRet) - lfDel) -lfDel*(lfFp-lfFptt)*(lfFp - lfFptt);
			if (lfT < 0.0)
			{
				vLineMin( plfP, plfXit, iN, plfFRet );
				for (j = 0; j < iN; j++)
				{
					pplfXi[j][iBig] = pplfXi[j][iN-1];
					pplfXi[j][iN-1] = plfXit[j];
				}
			}
		}
	}
}

double CPowell::lfSgn(double lfA, double lfB)
{
	return ( lfB >= 0.0 ? fabs(lfA) : -fabs(lfA) );
}

double CPowell::lfMax(double lfA, double lfB)
{
	return (lfA >= lfB ? lfA : lfB);
}
