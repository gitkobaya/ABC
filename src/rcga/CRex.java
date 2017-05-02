package rcga;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;

import objectivefunction.ObjectiveFunctionInterface;
import rankt.Rank_t;
import sfmt.Sfmt;

public class CRex extends CRealCodedGa
{
	private int iParentOutputFlag;
	private int iChildrenOutputFlag;
	private int iParentNumber;
	private int iChildrenNumber;
	private int iUpperEvalChildrenNumber;
	private int iDistanceFlag;
	private double[][] pplfChildren;
	private double[] plfTempVector;
	private double[] plfCentroid;
	private double[] plfChildVector;
	private double[] plfChildCentroid;
	private double[] plfUpperEvalCentroid;
	private double[] plfNormalizeRand;
	private double[][] pplfNormalizeRand;
	private double[] plfChildrenCentroid;
	private double[] plfUpperEvalChildrenCentroid;
	private double[] plfCentroidSteep;
	private double lfLcdp;
	private double lfLavg;
	private double lfLearningRate;
	private double lfAlpha;
	private int[] piParentLoc;
	private int iBestLoc;
//	private ObjectiveFunctionInterface pflfObjectiveFunction;

	public CRex()
	{
		iParentOutputFlag = 0;
		iChildrenOutputFlag = 0;
		iParentNumber = 0;
		iChildrenNumber = 0;
		pplfChildren = null;
		piParentLoc = null;
		iDistanceFlag = 2;
	}

	public void vInitialize( int iGenerationNum, int iGenNum, int iGenVectorData, int iParentNumberData, int iChildrenNumberData )
	{
		int i,j;
		// �e�̐�������ݒ肵�܂��B
		iParentNumber = iParentNumberData > iGenNum ? iGenNum : iParentNumberData;

		// �q���̐�������ݒ肵�܂��B
		iChildrenNumber = iChildrenNumberData;

		// �����lGA�̏����������s���܂��B
		vInitialize( iGenerationNum, iGenNum, iGenVectorData );

		// �e�I��p�z��ł��B
		piParentLoc = new int[iGenNum];
		// �q���̃f�[�^���쐬���܂��B
		pplfChildren = new double[iChildrenNumber][iGenVector];
		// REX�v�Z�p�x�N�g���f�[�^�ꎞ�ۑ��ϐ��ł��B
		plfTempVector = new double[iGenVector];
		// �d�S�̌v�Z���ʂ�ێ����܂��B
		plfCentroid = new double[iGenVector];
		plfChildVector = new double[iGenVector];
		plfNormalizeRand = new double[iParentNumber];
		plfChildrenCentroid = new double[iGenVector];
		plfUpperEvalChildrenCentroid = new double[iGenVector];
		plfCentroidSteep = new double[iGenVector];

		for( i = 0;i < iChildrenNumber; i++ )
			for( j = 0;j < iGenVector; j++ )
				pplfChildren[i][j] = 0.0;
		for( i = 0;i < iGenVector; i++ )
		{
			plfCentroid[i] = 0.0;
			plfChildVector[i] = 0.0;
			plfTempVector[i] = 0.0;
			plfUpperEvalChildrenCentroid[i] = 0.0;
			plfChildrenCentroid[i] = 0.0;
			plfCentroidSteep[i] = 0.0;
		}
		for( i = 0;i < iParentNumber; i++ )
		{
			plfNormalizeRand[i] = 0.0;
			piParentLoc[i] = i;
		}
		for( i = 0;i < iGenNum; i++ )
			piParentLoc[i] = i;
		long seed;
		seed = System.currentTimeMillis();
		rnd = new Sfmt( (int)seed );
	}

	public void vInitialize( int iGenerationNum, int iGenNum, int iGenVectorData, int iParentNumberData, int iChildrenNumberData, double lfLearningRateData,int iUpperEvalChildrenNumData )
	{
		int i,j;

		// �e�̐�������ݒ肵�܂��B
		iParentNumber = iParentNumberData > iGenNum ? iGenNum : iParentNumberData;

		// �q���̐�������ݒ肵�܂��B
		iChildrenNumber = iChildrenNumberData;

		// �����lGA�̏����������s���܂��B
		vInitialize( iGenerationNum, iGenNum, iGenVectorData );

		// �w�K���̐ݒ�����܂��B
		lfLearningRate = lfLearningRateData;

		// ������̕]���l��ʂ̎q���̐���臒l��ݒ肵�܂��B
		iUpperEvalChildrenNumber = iUpperEvalChildrenNumData > iChildrenNumber ? iChildrenNumber : iUpperEvalChildrenNumData;

		lfAlpha = 1.0;

		// �e�I��p�z��ł��B
		piParentLoc = new int[iGenNum];
		// �q���̃f�[�^���쐬���܂��B
		pplfChildren = new double[iChildrenNumber][iGenVector];
		pplfNormalizeRand = new double[iChildrenNumber][iParentNumber];
		plfTempVector = new double[iGenVector];
		plfCentroid = new double[iGenVector];
		plfChildVector = new double[iGenVector];
		plfNormalizeRand = new double[iParentNumber];
		plfChildrenCentroid = new double[iGenVector];
		plfUpperEvalChildrenCentroid = new double[iGenVector];
		plfCentroidSteep = new double[iGenVector];

		for( i = 0;i < iChildrenNumber; i++ )
			for( j = 0;j < iGenVector; j++ )
				pplfChildren[i][j] = 0.0;

		for( i = 0;i < iGenVector; i++ )
		{
			plfCentroid[i] = 0.0;
			plfChildVector[i] = 0.0;
			plfTempVector[i] = 0.0;
			plfUpperEvalChildrenCentroid[i] = 0.0;
			plfChildrenCentroid[i] = 0.0;
			plfCentroidSteep[i] = 0.0;
		}

		for( i = 0;i < iParentNumber; i++ )
			plfNormalizeRand[i] = 0.0;

		for( i = 0;i < iGenNum; i++ )
			piParentLoc[i] = i;

		for( i = 0;i < iChildrenNumber; i++ )
			for( j = 0;j < iParentNumber; j++ )
				pplfNormalizeRand[i][j] = 0.0;

		long seed;
		seed = System.currentTimeMillis();
		rnd = new Sfmt( (int)seed );
	}

	public void vTerminate()
	{
		int i;

		// �p�����N���X�ɑ����郁���o�ϐ��̏I�����������s���܂��B
		vTerminate();

		piParentLoc = null;
		pplfChildren = null;
		pplfNormalizeRand = null;
		plfTempVector = null;
		plfCentroid = null;
		plfChildVector = null;
		plfNormalizeRand = null;
		plfChildrenCentroid = null;
		plfUpperEvalChildrenCentroid = null;
		plfCentroidSteep = null;
	}

	public void vRex()
	{
		int i,j,k;
		int iLoc;
		int iTemp = 0;
		double lfSigma = 0.0;
		ArrayList<Rank_t> stlFitProb;
		Rank_t tTempRankData;

	/* JGG���f�� */
		stlFitProb = new ArrayList<Rank_t>();
		tTempRankData = new Rank_t();

		// �e�������_����Np�I�����܂��B
		for( i = iGenNumber-1; i > 0 ; i-- )
		{
			iLoc = (int)((i+1)*rnd.NextUnif());
			iTemp = piParentLoc[i];
			piParentLoc[i] = piParentLoc[iLoc];
			piParentLoc[iLoc] = iTemp;
		}

		// �d�S���Z�o���܂��B
		for( j = 0;j < iGenVector; j++ )
		{
			plfCentroid[j] = 0.0;
			for( i = 0;i < iParentNumber; i++ )
				plfCentroid[j] += pplfGens[piParentLoc[i]][j];
			plfCentroid[j] /= (double)iParentNumber;
		}
		// REX(RealCoded Emsanble )�����s���܂��B������Nc����s���ANc�̎q���𐶐����܂��B
		// ���v�ʈ�`�ɂ����镁�Օ��U���Z�o���܂��B
//		lfSigma = 1.0/(double)sqrt( (double)iParentNumber );
		lfSigma = Math.sqrt(3.0/(double)iParentNumber);

		for( i = 0;i < iChildrenNumber; i++ )
		{
			for( j = 0;j < iParentNumber; j++ )
			{
//				plfNormalizeRand[j] = lfSigma*rnd.NextNormal();
				plfNormalizeRand[j] = lfSigma*(2.0*rnd.NextUnif()-1.0);
			}
			for( k = 0;k < iGenVector; k++ )
			{
			// REX�����s���āA�q���𐶐����܂��B
				// ���K�����ɂ�藐���𔭐������܂��B
				plfTempVector[k] = 0.0;
				for( j = 0;j < iParentNumber; j++ )
					plfTempVector[k] += plfNormalizeRand[j] * ( pplfGens[piParentLoc[j]][k] - plfCentroid[k] );
				pplfChildren[i][k] = plfCentroid[k] + plfTempVector[k];
			}
			// �]���l��Np�Z�o���܂��B
			tTempRankData.lfFitProb = pflfObjectiveFunction.lfObjectiveFunction( pplfChildren[i] );
			tTempRankData.iLoc = i;
			stlFitProb.add( tTempRankData );
		}

		// �ړI�֐��l�ɂ��\�[�g�����{���܂��B
		// �ړI�֐��l�ɂ��\�[�g�����{���܂��B(�����A�~���ɂ������ꍇ��1��-1��ԋp����̂��t�ɂ���B)
		Collections.sort( stlFitProb, new Comparator<Rank_t>(){
			@Override
			public int compare( Rank_t a, Rank_t b )
			{
				if( a.lfFitProb > b.lfFitProb )
					return 1;
				else if( a.lfFitProb < b.lfFitProb )
					return -1;
				return 0;
			}
		});

		// �e�����ւ��܂��B(JGG���f���̏ꍇ�͐e�͂��ׂĕύX������̂Ƃ��܂��B)
		for( i = 0; i < iParentNumber; i++ )
			for( j = 0;j < iGenVector; j++ )
				pplfGens[piParentLoc[i]][j] = pplfChildren[stlFitProb.get(i).iLoc][j];
		// ���݂̍ŗǈʒu���擾���܂��B
		iBestLoc = piParentLoc[0];
	}

	public void vRexStar()
	{
		int i,j,k;
		int iLoc;
		int iMaxSize = 0;
		int iOverLapLoc = 0;
		double lfSigma = 0.0;
		ArrayList<Rank_t> stlFitProb;
		Rank_t tTempRankData;
		ArrayList<Integer> stlSelectParentLoc;

	/* JGG���f�� */
		stlSelectParentLoc = new ArrayList<Integer>();
		stlFitProb = new ArrayList<Rank_t>();
		tTempRankData = new Rank_t();

		// �e�������_����Np�I�����܂��B
		for(;;)
		{
			iLoc = rnd.NextInt( iGenNumber );
			// �I�������e�Əd�Ȃ��Ă��Ȃ����������܂��B
			iOverLapLoc = -1;
			for( i = 0;i < stlSelectParentLoc.size(); i++ )
			{
				if( stlSelectParentLoc.get(i) == iLoc )
				{
					iOverLapLoc = i;
					break;
				}
			}
			// �d�Ȃ��Ă��Ȃ���΁A�ʒu��ǉ����܂��B
			if( iOverLapLoc == -1 )
			{
				stlSelectParentLoc.add( iLoc );
				iMaxSize++;
			}
			// �w�肵���e�̐��ɂȂ�����I�����܂��B
			if( iMaxSize == iParentNumber ) break;
		}
		// �d�S���Z�o���܂��B
		for( i = 0;i < iGenVector; i++ )
			plfCentroid[i] = 0.0;
		for( i = 0;i < iParentNumber; i++ )
			for( j = 0;j < iGenVector; j++ )
				plfCentroid[j] += ( pplfGens[stlSelectParentLoc.get(i)][j] );
		for( i = 0;i < iParentNumber; i++ )
			plfCentroid[i] /= (double)iParentNumber;
	// REX(RealCoded Emsanble )�����s���܂��B������Nc����s���ANc�̎q���𐶐����܂��B
		// ���v�ʈ�`�ɂ����镁�Օ��U���Z�o���܂��B
		lfSigma = 1.0/(double)Math.sqrt( (double)iParentNumber );

		for( i = 0;i < iChildrenNumber; i++ )
		{
			// ���K�����ɂ�藐���𔭐������܂��B
			for( j = 0;j < iParentNumber; j++ )
				plfNormalizeRand[j] = lfSigma*rnd.NextNormal();
			for( j = 0;j < iGenVector; j++ )
			{
				plfTempVector[j] = 0.0;
				plfChildVector[j] = 0.0;
			}
			for( j = 0;j < iParentNumber; j++ )
			{
			// REX�����s���āA�q���𐶐����܂��B
				for( k = 0;k < iGenVector; k++ )
					plfTempVector[k] += plfNormalizeRand[j] * ( pplfGens[stlSelectParentLoc.get(j)][k] - plfCentroid[k] );
			}
			for( k = 0;k < iGenVector; k++ )
				plfChildVector[k] = plfCentroid[k] + plfTempVector[k];
			for( j = 0;j < iGenVector; j++ )
				pplfChildren[i][j] = plfChildVector[j];
		}

		// �]���l��Np�Z�o���܂��B
		for( i = 0;i < iChildrenNumber; i++ )
		{
			tTempRankData.lfFitProb = pflfObjectiveFunction.lfObjectiveFunction( pplfChildren[i] );
			tTempRankData.iLoc = i;
			stlFitProb.add( tTempRankData );
		}
		// �ړI�֐��l�ɂ��\�[�g�����{���܂��B
		Collections.sort( stlFitProb, new Comparator<Rank_t>(){
			@Override
			public int compare( Rank_t a, Rank_t b )
			{
				return a.lfFitProb > b.lfFitProb ? 1 : 0;
			}
		});
		// �e�����ւ��܂��B(JGG���f���̏ꍇ�͐e�͂��ׂĕύX������̂Ƃ��܂��B)
		for( i = 0; i < iParentNumber; i++ )
			for( j = 0;j < iGenVector; j++ )
				pplfGens[stlSelectParentLoc.get(i)][j] = pplfChildren[stlFitProb.get(i).iLoc][j];
	}

	/**
	 * <PRE>
	 *    �K���I�����l����AREX�����s���܂��B
	 * </PRE>
	 * @author kobayashi
	 * @since 2016/6/10
	 * @version 1.0
	 */
	public void vARex()
	{
		int i,j,k;
		int iLoc;
		int iMaxSize = 0;
		int iOverLapLoc = 0;
		int iTemp = 0;
		double lfSigma = 0.0;
		double l;
		double lAvgDist = 0.0;
		double lfTemp = 0.0;
		ArrayList<Rank_t> stlFitProb;
		ArrayList<Rank_t> stlParentFitProb;
		Rank_t tTempRankData;
		ArrayList<Integer> stlSelectParentLoc;

		int i1stGenLoc = 0;
		int i2ndGenLoc = 0;

	/* JGG���f�� */
		stlSelectParentLoc = new ArrayList<Integer>();
		tTempRankData = new Rank_t();
		stlFitProb = new ArrayList<Rank_t>();
		stlParentFitProb = new ArrayList<Rank_t>();

		// �e�������_����Np�I�����܂��B
		for( i = iGenNumber-1; i >= 0 ; i-- )
		{
			iLoc = (int)((i+1)*rnd.NextUnif());
			iTemp = piParentLoc[i];
			piParentLoc[i] = piParentLoc[iLoc];
			piParentLoc[iLoc] = iTemp;
			if( i <= iParentNumber )
			{
				// �e�f�[�^��K���x�Ń\�[�g���邽�߁A�f�[�^�������܂��B
				tTempRankData.lfFitProb = pflfObjectiveFunction.lfObjectiveFunction( pplfGens[piParentLoc[i]] );
				tTempRankData.iLoc = piParentLoc[i];
				stlParentFitProb.add( tTempRankData );
			}
		}

//		std::sort( stlParentFitProb.begin(), stlParentFitProb.end(), CCompareToRank() );
		Collections.sort( stlParentFitProb, new Comparator<Rank_t>(){
			@Override
			public int compare( Rank_t a, Rank_t b )
			{
				return a.lfFitProb > b.lfFitProb ? 1 : 0;
			}
		});

		// �d�S�y�ь������S�~�����Z�o���܂��B
		for( j = 0;j < iGenVector; j++ )
		{
			plfCentroid[j] = 0.0;
			plfCentroidSteep[j] = 0.0;
			for( i = 0;i < iParentNumber; i++ )
			{
				plfCentroid[j] += pplfGens[stlParentFitProb.get(i).iLoc][j];
				plfCentroidSteep[j] += 2.0*(double)(iParentNumber+1-(i+1))*pplfGens[stlParentFitProb.get(i).iLoc][j];
			}
			plfCentroid[j] /= (double)iParentNumber;
			plfCentroidSteep[j] /= (double)(iParentNumber*(iParentNumber+1));
		}
	// REX(RealCoded Emsanble )�����s���܂��B������Nc����s���ANc�̎q���𐶐����܂��B
		// ���v�ʈ�`�ɂ����镁�Օ��U���Z�o���܂��B
		lfSigma = 1.0/(double)Math.sqrt( (double)iParentNumber-1 );
//		lfSigma = Math.sqrt(3.0/(double)(iParentNumber));

		for( i = 0;i < iChildrenNumber; i++ )
		{
			// ���K�����ɂ�藐���𔭐������܂��B
			for( j = 0;j < iParentNumber; j++ )
			{
				plfNormalizeRand[j] = lfSigma*rnd.NextNormal();
//				plfNormalizeRand[j] = lfSigma*(2.0*rnd.NextUnif()-1.0);
				if( iDistanceFlag == 2 ) pplfNormalizeRand[i][j] = plfNormalizeRand[j];
			}
			for( k = 0;k < iGenVector; k++ )
			{
			// REX�����s���āA�q���𐶐����܂��B
				plfTempVector[k] = 0.0;
				for( j = 0;j < iParentNumber; j++ )
					plfTempVector[k] += plfNormalizeRand[j] * ( pplfGens[stlParentFitProb.get(j).iLoc][k] - plfCentroid[k] );
				pplfChildren[i][k] = plfCentroidSteep[k] + lfAlpha*plfTempVector[k];
			}
			// �]���l��Np�Z�o���܂��B
			tTempRankData.lfFitProb = pflfObjectiveFunction.lfObjectiveFunction( pplfChildren[i] );
			tTempRankData.iLoc = i;
			stlFitProb.add( tTempRankData );
		}

		// �ړI�֐��l�ɂ��\�[�g�����{���܂��B
		Collections.sort( stlFitProb, new Comparator<Rank_t>(){
			@Override
			public int compare( Rank_t a, Rank_t b )
			{
				return a.lfFitProb > b.lfFitProb ? 1 : 0;
			}
		});

		// �g�����K���x���v�Z���܂��B
		if( iDistanceFlag == 1 )
		{
			// �����̒��S�����߂܂��B
			// �]���l��ʃʃ��̎q���̒��S�����߂܂��B
			for( j = 0;j < iGenVector; j++ )
			{
				plfChildrenCentroid[j] = 0.0;
				plfUpperEvalChildrenCentroid[j] = 0.0;
				for( i = 0;i < iChildrenNumber; i++ )
				{
					plfChildrenCentroid[j] += pplfChildren[stlFitProb.get(i).iLoc][j];
					plfUpperEvalChildrenCentroid[j] += pplfChildren[stlFitProb.get(i).iLoc][j];
				}
				plfChildrenCentroid[j] /= (double)(iChildrenNumber);
				plfUpperEvalChildrenCentroid[j] /= (double)iUpperEvalChildrenNumber;
			}
			vAerEuclide( stlParentFitProb );
		}
		else if( iDistanceFlag == 2 )
		{
			vAerMahalanobis( stlFitProb );
		}

			// �e�����ւ��܂��B(JGG���f���̏ꍇ�͐e�͂��ׂĕύX������̂Ƃ��܂��B)
		for( i = 0; i < iParentNumber; i++ )
			for( j = 0;j < iGenVector; j++ )
				pplfGens[stlParentFitProb.get(i).iLoc][j] = pplfChildren[stlFitProb.get(i).iLoc][j];
		// ���݂̍ŗǈʒu���擾���܂��B
		iBestLoc = stlParentFitProb.get(0).iLoc;
	}

	/**
	 * <PRE>
	 * �@�g�����K���x�����[�N���b�h�����ɂ��v�Z���܂��B
	 * </PRE>
	 * @author kobayashi
	 * @since 2016/8/25
	 * @version 0.1
	 */
	public void vAerEuclide( ArrayList<Rank_t> stlParentFitProb )
	{
		int i,j;
		double lfTemp;
		double lfSigma;
		double lfNorm = 0.0;

		// Ldcp���Z�o���܂��B
		lfSigma = 1.0/(double)Math.sqrt( (double)iParentNumber-1 );
		lfLcdp = 0.0;
		for( i = 0;i < iGenVector; i++ )
		{
			lfTemp = plfUpperEvalChildrenCentroid[i] - plfChildrenCentroid[i];
			lfLcdp += lfTemp*lfTemp;
		}

		// Lavg���Z�o���܂��B
		lfLavg = 0.0;
		for( i = 0;i < iParentNumber; i++ )
		{
			lfNorm = 0.0;
			for( j = 0;j < iGenVector; j++ )
			{
				lfTemp = pplfGens[stlParentFitProb.get(i).iLoc][j] - plfCentroid[j];
				lfNorm += lfTemp*lfTemp;
			}
			lfLavg += lfNorm;
		}
		lfLavg = lfAlpha*lfAlpha*lfSigma*lfSigma/(double)iUpperEvalChildrenNumber;

		// ���̍X�V���s���܂��B
		lfTemp = lfAlpha * Math.sqrt( (1.0-lfLearningRate)+lfLearningRate*lfLcdp/lfLavg );
		lfAlpha = lfTemp < 1.0 ? 1.0 : lfTemp;
	}

	/**
	 * <PRE>
	 * �@�g�����K���x�����[�N���b�h�����ɂ��v�Z���܂��B
	 * </PRE>
	 * @author kobayashi
	 * @since 2016/8/25
	 * @version 0.1
	 */
	public void vAerMahalanobis( ArrayList<Rank_t> stlFitProb )
	{
		int i,j;
		double lfTemp;
		double lfSigma;
		double lfRandAvg = 0.0;
		double lfRandAvgSumSquare = 0.0;
		double lfRandAvgSumSquareAvg = 0.0;

		// Ldcp���Z�o���܂��B
		lfSigma = 1.0/(double)Math.sqrt( (double)iParentNumber );
//		lfSigma = Math.sqrt( 3.0/(double)iParentNumber );
		lfLcdp = 0.0;
		for( i = 0;i < iParentNumber; i++ )
		{
			lfRandAvg = 0.0;
			for( j = 0;j < iUpperEvalChildrenNumber; j++ )
			{
				lfRandAvg += pplfNormalizeRand[stlFitProb.get(j).iLoc][i];
			}
			lfRandAvg /= (double)iUpperEvalChildrenNumber;
			lfRandAvgSumSquare += lfRandAvg*lfRandAvg;
			lfRandAvgSumSquareAvg += lfRandAvg;
		}
		lfRandAvgSumSquareAvg = lfRandAvgSumSquareAvg*lfRandAvgSumSquareAvg/(double)iParentNumber;
		lfLcdp = lfAlpha*lfAlpha*(iParentNumber-1)*(lfRandAvgSumSquare-lfRandAvgSumSquareAvg);

		// Lavg���Z�o���܂��B
		lfLavg = lfAlpha*lfAlpha*lfSigma*lfSigma*(iParentNumber-1)*(iParentNumber-1)/(double)iUpperEvalChildrenNumber;

		// ���̍X�V���s���܂��B
		lfTemp = lfAlpha * Math.sqrt( (1.0-lfLearningRate)+lfLearningRate*lfLcdp/lfLavg );
		lfAlpha = lfTemp < 1.0 ? 1.0 : lfTemp;
	}

	public void vSelectGens( double[][] pplfChildren, int[] pi1stGenLoc, int[] pi2ndGenLoc )
	{
		int i;
		double lfProb = 0.0;
		double lfPrevProb = 0.0;
		double lfRes = 0.0;
		double lf1stGen = Double.MAX_VALUE;
		double lfRand = 0.0;
		int i1stGenLoc = Integer.MAX_VALUE;
		int i2ndGenLoc = Integer.MAX_VALUE;
		int iRank = 0;
		ArrayList<Rank_t> stlFitProb;
		Rank_t tTempRankData;
		// �܂��A�K���x�֐��̒l���v�Z���܂��B
		lfRes = 0.0;
		tTempRankData = new Rank_t();
		stlFitProb = new ArrayList<Rank_t>();

		for( i = 0;i < iChildrenNumber; i++ )
		{
			tTempRankData.lfFitProb = pflfObjectiveFunction.lfObjectiveFunction( pplfChildren[i] );
			tTempRankData.iLoc = i;
			stlFitProb.add( tTempRankData );
			lfRes += stlFitProb.get(i).lfFitProb;
			if( stlFitProb.get(i).lfFitProb < lf1stGen )
			{
				lf1stGen = stlFitProb.get(i).lfFitProb;
				i1stGenLoc = i;
			}
		}
		// �ړI�֐��l�ɂ��\�[�g�����{���܂��B
		Collections.sort( stlFitProb, new Comparator<Rank_t>(){
			@Override
			public int compare( Rank_t a, Rank_t b )
			{
				return a.lfFitProb > b.lfFitProb ? 1: 0;
			}
		});
		// �����N�Ɋ�Â����[���b�g�I�������s�B
		iRank = rnd.NextInt( iChildrenNumber-1 ) + 1;
		i2ndGenLoc = stlFitProb.get(iRank).iLoc;
		// �ŗǌ̂̈ʒu�Ƃ���ȊO�Ń��[���b�g�I���ɂ��I�΂ꂽ�ʒu��ԋp���܂��B
		pi1stGenLoc[0] = i1stGenLoc;
		pi2ndGenLoc[0] = i2ndGenLoc;
	}

	/**
	 * <PRE>
	 * �@�����_�ł̑I�������e���o�͂��܂��B
	 * </PRE>
	 * @author kobayashi
	 * @since 2015/12/16
	 * @version 0.1
	 */
	public void vOutputCurrentParent( ArrayList<Integer> stlSelectParentLocData )
	{
		int i,j;
		double lfRes = 0.0;
		double lfAvgDist = 0.0;
		double lfDist = 0.0;
		double lfDist2 = 0.0;
		if( iParentOutputFlag == 1 )
		{
			for(i = 0;i < stlSelectParentLocData.size(); i++ )
			{
				// �����_�őI���������e���o�͂��܂��B
				for( j = 0;j < iGenVector; j++ )
				{
					System.out.print( pplfGens[stlSelectParentLocData.get(i)][j] + "," );
				}
				System.out.print("\n");
			}
		}
	}

	/**
	 * <PRE>
	 * �@�����_�ł̐��������q���o�͂��܂��B
	 * </PRE>
	 * @author kobayashi
	 * @since 2015/12/16
	 * @version 0.1
	 */
	public void vOutputCurrentChildren( double[][] pplfChildrenData )
	{
		int i,j;

		if( iChildrenOutputFlag == 1 )
		{
			for( i= 0; i < iChildrenNumber; i++ )
			{
				for( j = 0;j < iGenVector; j++ )
				{
					System.out.print(pplfChildrenData[i][j] + "," );
				}
				System.out.print("\n");
			}
		}
	}

	/**
	 * <PRE>
	 * �@��`�q�ɒl��ݒ肵�܂��B
	 *   ver 0.1 ����
	 * </PRE>
	 * @param pplfGenData
	 * @author kobayashi
	 * @since 2016/08/26
	 * @version 0.1
	 */
	public void vSetGenData( double[][] pplfGenData )
	{
		int i,j;
		for( i= 0; i < iGenNumber; i++ )
		{
			for( j = 0;j < iGenVector; j++ )
			{
				pplfGens[i][j] = pplfGenData[i][j];
			}
		}
	}

	/**
	 * <PRE>
	 * �@���݂̈�`�q�f�[�^���擾���܂��B
	 *   ver 0.1 ����
	 * </PRE>
	 * @param pplfGenData
	 * @author kobayashi
	 * @since 2016/08/26
	 * @version 0.1
	 */
	public void vGetGenData( double[][] pplfGenData )
	{
		int i,j;
		for( i= 0; i < iGenNumber; i++ )
		{
			for( j = 0;j < iGenVector; j++ )
			{
				pplfGenData[i][j] = pplfGens[i][j];
			}
		}
	}

	/**
	 * <PRE>
	 * �@���݂̈�`�q�f�[�^���擾���܂��B
	 *   ver 0.1 ����
	 * </PRE>
	 * @param plfGenData �����_�ł̈�`�q�f�[�^�i�x�X�g�l�j
	 * @author kobayashi
	 * @since 2016/09/14
	 * @version 0.1
	 */
	public void vGetBestGenData( double[] plfGenData )
	{
		int j;
		for( j = 0;j < iGenVector; j++ )
		{
			plfGenData[j] = pplfGens[iBestLoc][j];
		}
	}
}