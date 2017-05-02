package rcga;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;

import sfmt.Sfmt;
import rankt.Rank_t;
//import objectivefunction.ObjectiveFunctionInterface;

public class CUndx extends CRealCodedGa
{
	private	int iCrossOverNum;
	private	int iChildrenNumber;
	private	double lfAlpha;
	private	double lfBeta;
	private	int iParentOutputFlag;
	private	int iChildrenOutputFlag;
	private	double[][] pplfChildren;
	private	double[] plfChild1;
	private	double[] plfChild2;
	private	int[] piParentLoc;
	private	int iBestLoc;
	private int iParent1Loc;
	private int iParent2Loc;
	private int iParent3Loc;
	private int iSelectParentFlag;
	private int i1stLoc;
	private int i2ndLoc;
//	private ObjectiveFunctionInterface pflfObjectiveFunction;

	public CUndx()
	{
		iCrossOverNum = 0;
		lfAlpha = 0.0;
		lfBeta = 0.0;
		iParentOutputFlag = 0;
		iChildrenOutputFlag = 0;
		iChildrenNumber = 0;
		pplfChildren = null;
		plfChild1 = null;
		plfChild2 = null;
		piParentLoc = null;
		i1stLoc = 0;
		i2ndLoc = 0;
	}

	public void vInitialize( int iGenerationNum, int iGenNum, int iGenVectorData, int iCrossOverNumData )
	{
		int i,j;

		// �����lGA�̏����������s���܂��B
		vInitialize( iGenerationNum, iGenNum, iGenVectorData );

		// �����񐔂�ݒ肵�܂��B
		iCrossOverNum = iCrossOverNumData;

		// �ő�ō쐬�����q���̌���ݒ肵�܂��B
		iChildrenNumber = iCrossOverNum * 2 + 2;
		// �q���̃f�[�^���쐬���܂��B
		pplfChildren = new double[iChildrenNumber][iGenVector];
		for( i = 0;i < iChildrenNumber; i++ )
		{
			for( j = 0;j < iGenVector; j++ )
			{
				pplfChildren[i][j] = 0.0;
			}
		}

		plfChild1 = new double[iGenVector];
		plfChild2 = new double[iGenVector];
		for( i = 0;i < iGenVector; i++ )
		{
			plfChild1[i] = 0.0;
			plfChild2[i] = 0.0;
		}
		piParentLoc = new int[iGenNumber];
		for (i = 0; i < iGenNum; i++)
		{
			piParentLoc[i] = i;
		}
		long seed;
		seed = System.currentTimeMillis();
		rnd = new Sfmt( (int)seed );
	}

	public void vTerminate()
	{
		int i;

		// �p�����N���X�ɑ����郁���o�ϐ��̏I�����������s���܂��B
		vTerminate();

		// �I�����������s���܂��B
		pplfChildren = null;
		plfChild1 = null;
		plfChild2 = null;
		piParentLoc = null;
	}

	public void vImplement()
	{
		int i,j;
		int[] i1,i2,i3;
		int[] i1stGenLoc;
		int[] i2ndGenLoc;
		double[][] plfParent1 = null;
		double[][] plfParent2 = null;
		double[][] plfParent3 = null;

		double lfPrevProb = 0.0;
		double lfProb = 0.0;

		plfParent1 = new double[1][iGenVector];
		plfParent2 = new double[1][iGenVector];
		plfParent3 = new double[1][iGenVector];
		i1 = new int[1];
		i2 = new int[1];
		i3 = new int[1];
		i1stGenLoc = new int[1];
		i2ndGenLoc = new int[1];

		for( i = 0;i < iCrossOverNum; i++ )
		{
			// UNDX��2�̐e��I�����܂��B
			vSelectParent( plfParent1, plfParent2, plfParent3, i1, i2, i3 );

			// �����ŁA�t���O���L���ɂȂ��Ă���ꍇ�Ɍ���e���o�͂��܂��B
			vOutputCurrentParent( plfParent1[0], plfParent2[0] );

			// UNDX�����s���܂��B
			vUndx( plfParent1[0], plfParent2[0], plfParent3[0], lfAlpha, lfBeta, plfChild1, plfChild2 );

			// ���������q����ǉ����܂��B
			for( j = 0;j < iGenVector; j++ )
			{
				pplfChildren[2*i][j] = plfChild1[j];
				pplfChildren[2*i+1][j] = plfChild2[j];
			}
			// �Ō�ɐe�����̏W�c�ɒǉ����܂��B
			for( j = 0;j < iGenVector; j++ )
			{
				pplfChildren[2*iCrossOverNum][j] = plfParent1[0][j];
				pplfChildren[2*iCrossOverNum+1][j] = plfParent2[0][j];
			}

			// �����ŁA�t���O���L���ɂȂ��Ă���ꍇ�Ɍ���q���o�͂��܂��B
			vOutputCurrentChildren( pplfChildren );

		// ����������{���܂��B2�̂悢��`�q��I�����܂�

			// �ł��]���l�̂悢��`�q�ƁA���[���b�g�I���ɂ�茈�肵����`�q��I�����܂��B
			vSelectGens( pplfChildren, i1stGenLoc, i2ndGenLoc );

			// ����I�������e�Ǝq���������܂��B
			for( j = 0;j < iGenVector; j++ )
			{
				pplfGens[i1[0]][j] = pplfChildren[i1stGenLoc[0]][j];
				pplfGens[i2[0]][j] = pplfChildren[i2ndGenLoc[0]][j];
			}
			// ���݂̍ŗǒl�̔ԍ����擾���܂��B
			i1stLoc = i1[0];
			i2ndLoc = i2[0];
			// �ꎞ�I�ɕێ����Ă����q�̏W�����폜���܂��B
			for(i = 0;i < iChildrenNumber; i++ )
			{
				for( j= 0;j < iGenVector;j++ )
				{
					pplfChildren[i][j] = 0.0;
				}
			}
		}
	}

	public void vSelectParent( double[][] pplfParent1, double[][] pplfParent2, double[][] pplfParent3, int[] piLoc1, int[] piLoc2, int[] piLoc3 )
	{
		int i,j;
		double lfSumAbs1, lfSumAbs2, lfSumAbs3;
		int i1, i2, i3;
		int iLoc;
		int iTemp;
		int iFlag = 0;

		if( iSelectParentFlag == 0 )
		{
			// UNDX���s���e��2���肵�܂��B
			// �e�������_����Np�I�����܂��B
			for (i = iGenNumber - 1; i > 0; i--)
			{
				iLoc = (int)((i + 1)*rnd.NextUnif());
				iTemp = piParentLoc[i];
				piParentLoc[i] = piParentLoc[iLoc];
				piParentLoc[iLoc] = iTemp;
			}
			lfSumAbs1 = lfSumAbs2 = lfSumAbs3 = 0.0;
			i1 = piParentLoc[0];
			i2 = piParentLoc[1];
			i3 = piParentLoc[2];
			for (i = 1; i < iGenNumber-1; i++)
			{
				lfSumAbs1 = lfSumAbs2 = lfSumAbs3 = 0.0;
				if( iFlag == 1 )
				{
					i2 = piParentLoc[i - 1];
					i3 = piParentLoc[i];
				}
				else if( iFlag == 2 ) i2 = piParentLoc[i];
				else if (iFlag == 3 ) i3 = piParentLoc[i];
				else if (iFlag == 4 ) i3 = piParentLoc[i];
				else if( iFlag == 5 ) break;

				for (j = 0; j < iGenVector; j++)
				{
					lfSumAbs1 += Math.abs(pplfGens[i1][j] - pplfGens[i2][j]);
					lfSumAbs2 += Math.abs(pplfGens[i1][j] - pplfGens[i3][j]);
					lfSumAbs3 += Math.abs(pplfGens[i2][j] - pplfGens[i3][j]);
				}
				// i1 = i2 = i3�̏ꍇ�i�e�����ׂē������ꍇ�j
				if (lfSumAbs1 <= 0.000000000001 && lfSumAbs2 <= 0.000000000001) iFlag = 1;
				// i1 = i2�̏ꍇ�i1�ڂ�2�ڂ̐e���������ꍇ�j
				else if (lfSumAbs1 <= 0.000000000001) iFlag = 2;
				// i1 = i3�̏ꍇ�i1�ڂ�3�ڂ̐e���������ꍇ�j
				else if (lfSumAbs2 <= 0.000000000001) iFlag = 3;
				// i2 = i3�̏ꍇ�i2�ڂ�3�ڂ̐e���������ꍇ�j
				else if (lfSumAbs3 <= 0.000000000001) iFlag = 4;
				else iFlag = 5;
			}
			pplfParent1[0] = pplfGens[i1];
			pplfParent2[0] = pplfGens[i2];
			pplfParent3[0] = pplfGens[i3];
			piLoc1[0] = i1;
			piLoc2[0] = i2;
			piLoc3[0] = i3;
		}
		else
		{
			pplfParent1[0] = pplfGens[iParent1Loc];
			pplfParent2[0] = pplfGens[iParent2Loc];
			pplfParent3[0] = pplfGens[iParent3Loc];
			piLoc1[0] = iParent1Loc;
			piLoc2[0] = iParent2Loc;
			piLoc3[0] = iParent3Loc;
			iSelectParentFlag = 1;
		}
	}

	void vSelectParent(int iLoc1, int iLoc2)
	{
		int i, j;
		double lfSumAbs1, lfSumAbs2, lfSumAbs3;
		int i1, i2, i3;
		int iLoc;
		int iTemp;
		int iFlag = 0;
		// UNDX���s���e��2���肵�܂��B
		// �e�������_����Np�I�����܂��B
		for (i = iGenNumber - 1; i > 0; i--)
		{
			iLoc = (int)((i + 1)*rnd.NextUnif());
			iTemp = piParentLoc[i];
			piParentLoc[i] = piParentLoc[iLoc];
			piParentLoc[iLoc] = iTemp;
		}
		i1 = iLoc1;
		if (iLoc2 == -1)
		{
			i2 = piParentLoc[0];
			i3 = piParentLoc[1];
		}
		else
		{
			i2 = iLoc2;
			i3 = piParentLoc[0];
		}
		lfSumAbs1 = lfSumAbs2 = lfSumAbs3 = 0.0;
		for (i = 1; i < iGenNumber - 1; i++)
		{
			lfSumAbs1 = lfSumAbs2 = lfSumAbs3 = 0.0;
			if (iFlag == 1)
			{
				i2 = piParentLoc[i - 1];
				i3 = piParentLoc[i];
			}
			else if (iFlag == 2) i2 = piParentLoc[i];
			else if (iFlag == 3) i3 = piParentLoc[i];
			else if (iFlag == 4) i3 = piParentLoc[i];
			else if (iFlag == 5) break;

			for (j = 0; j < iGenVector; j++)
			{
				lfSumAbs1 += Math.abs(pplfGens[i1][j] - pplfGens[i2][j]);
				lfSumAbs2 += Math.abs(pplfGens[i1][j] - pplfGens[i3][j]);
				lfSumAbs3 += Math.abs(pplfGens[i2][j] - pplfGens[i3][j]);
			}
			// i1 = i2 = i3�̏ꍇ�i�e�����ׂē������ꍇ�j
			if (lfSumAbs1 <= 0.000000000001 && lfSumAbs2 <= 0.000000000001) iFlag = 1;
			// i1 = i2�̏ꍇ�i1�ڂ�2�ڂ̐e���������ꍇ�j
			else if (lfSumAbs1 <= 0.000000000001) iFlag = 2;
			// i1 = i3�̏ꍇ�i1�ڂ�3�ڂ̐e���������ꍇ�j
			else if (lfSumAbs2 <= 0.000000000001) iFlag = 3;
			// i2 = i3�̏ꍇ�i2�ڂ�3�ڂ̐e���������ꍇ�j
			else if (lfSumAbs3 <= 0.000000000001) iFlag = 4;
			else iFlag = 5;
		}
		iParent1Loc = i1;
		iParent2Loc = i2;
		iParent3Loc = i3;
		iSelectParentFlag = 1;
	}


	private void vUndx( double[] plfParent1, double[] plfParent2, double[] plfParent3, double lfAlpha, double lfBeta, double[] plfChild1, double[] plfChild2 )
	{
		int i;
		ArrayList<Double> stlUnityVector1,stlMedian;
		ArrayList<Double> stlTempT1,stlTempT2;
		double lfDistTemp = 0.0;
		double lfProduct = 0.0;
		double lfDist1 = 0.0;
		double lfDist2 = 0.0;
		double lfDist3 = 0.0;
		double lfSub1 = 0.0;
		double lfSub2 = 0.0;
		double lfSigma1 = 0.0;
		double lfSigma2 = 0.0;
		double lfS = 0.0;
		double lfTemp1 = 0.0;
		double lfTemp2 = 0.0;
		double lfTemp3 = 0.0;

		stlUnityVector1 = new ArrayList<Double>();
		stlMedian = new ArrayList<Double>();
		stlTempT1 = new ArrayList<Double>();
		stlTempT2 = new ArrayList<Double>();
		lfDist1 = lfDist2 = lfDistTemp = 0.0;
		for( i = 0; i < iGenVector; i++ )
		{
			// 2�̐e�̒��_���Z�o���܂��B
			stlMedian.add( ( plfParent1[i]+plfParent2[i] )/2.0 );
			stlUnityVector1.add( plfParent1[i]-plfParent2[i] );
			// 2�̐e�̋��������߂܂��B
			lfSub1 = (plfParent2[i]-plfParent1[i]);
			lfSub2 = (plfParent3[i]-plfParent1[i]);
			lfDist1 += lfSub1*lfSub1;
			lfDist2 += lfSub2*lfSub2;
			// ��3�̐e��2�̐e�Ƃ̋��������߂܂��B
			lfDistTemp += lfSub1*lfSub2;
			// �����ŁAz1,z2�𐶐����܂��Bz1=N(0,��_{1}^2), z2=N(0,��_{2}^2)�Ȃ̂ŁA����ɏ]���Đ������܂��B
//			stlTempT1.add(lfSgima1*rnd.NextNormal());
//			stlTempT2.add(lfSigma2*rnd.NextNormal());
		}
		lfDist1 = Math.sqrt( lfDist1 );
		lfDist2 = Math.sqrt( lfDist2 );
		lfDistTemp = lfDistTemp/(lfDist1*lfDist2);
		lfDist3 = lfDist2*Math.sqrt(1.0-lfDistTemp*lfDistTemp);
		lfSigma1 = lfDist1*lfAlpha;
		lfSigma2 = lfDist3*lfBeta/Math.sqrt((double)iGenVector);
		for( i = 0;i < iGenVector; i++ )
			stlTempT2.add(lfSigma2*rnd.NextNormal());

		lfProduct = 0.0;
		for( i = 0; i < iGenVector; i++ )
		{
			// �P�ʃx�N�g�����쐬���܂��B
			stlUnityVector1.set(i, stlUnityVector1.get(i) / lfDist1);
		//�܂�z2�ɒ��s����x�N�g�����Z�o���܂��B
			// ���ς����߂܂��B
			lfProduct += stlTempT2.get(i)*stlUnityVector1.get(i);
		}
		// z2�ɒ��s����x�N�g���𐶐����܂��B
		lfS = lfSigma1*rnd.NextNormal();
		// 2�q���𐶐����܂��B
		for( i = 0;i < iGenVector; i++ )
		{
			lfTemp1 = lfProduct*stlUnityVector1.get(i);
			lfTemp2 = lfS*stlUnityVector1.get(i);
			lfTemp3 = stlTempT2.get(i) - lfTemp1 + lfTemp2;
			plfChild1[i] = stlMedian.get(i) + lfTemp3;
			plfChild2[i] = stlMedian.get(i) - lfTemp3;
		}
	}

	private void vSelectGens( double[][] pplfChildren, int[] pi1stGenLoc, int[] pi2ndGenLoc )
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
		tTempRankData = new Rank_t();
		stlFitProb = new ArrayList<Rank_t>();
		lfRes = 0.0;
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
		// �����N�Ɋ�Â����[���b�g�I�������s�B
		iRank = rnd.NextInt(iChildrenNumber-1) + 1;
		i2ndGenLoc = stlFitProb.get(iRank).iLoc;
		// �ŗǌ̂̈ʒu�Ƃ���ȊO�Ń��[���b�g�I���ɂ��I�΂ꂽ�ʒu��ԋp���܂��B
		pi1stGenLoc[0] = i1stGenLoc;
		pi2ndGenLoc[0] = i2ndGenLoc;
	}

	/**
	 * <PRE>
	 * �@���U�͈͓̔��Ɋm���ɐ��K�����𔭐�������悤�ɂ��܂��B
	 * </PRE>
	 * @param sigma ���U
	 * @param mean ����
	 * @author kobayashi
	 * @since 2016/06/16
	 * @version 0.1
	 */
	private double normalRand( double sigma, double mean )
	{
		double lfRes = 0.0;
		double lfMin,lfMax;
		lfMin = -sigma + mean;
		lfMax = sigma + mean;
		for(;;)
		{
			lfRes = sigma*rnd.NextNormal()+mean;
			if( -lfMin <= lfRes && lfRes <= lfMax ) break;
		}
		return lfRes;
	}

	/**
	 * <PRE>
	 * �@�����_�ł̑I�������e���o�͂��܂��B
	 * </PRE>
	 * @author kobayashi
	 * @since 2015/12/16
	 * @version 0.1
	 */
	public void vOutputCurrentParent( double[] plfParent1, double[] plfParent2 )
	{
		int j;
		double lfRes = 0.0;
		double lfAvgDist = 0.0;
		double lfDist = 0.0;
		double lfDist2 = 0.0;
		if( iParentOutputFlag == 1 )
		{
			// �����_�őI���������e���o�͂��܂��B
			for( j = 0;j < iGenVector; j++ )
			{
				System.out.print(plfParent1[j] + "," );
			}
			System.out.print("\n");
			// �����_�őI���������e���o�͂��܂��B
			for( j = 0;j < iGenVector; j++ )
			{
				System.out.print(plfParent2[j] + "," );
			}
			System.out.print("\n");
		}
	}

	/**
	 * <PRE>
	 * �@�����_�ł̐��������q���o�͂��܂��B
	 * </PRE>
	 * @param pplfChildrenData
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
	 * @since 2016/08/10
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
	 * @since 2016/08/10
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
	 * �@���݂̈�`�q�f�[�^�̍ŗǒl���擾���܂��B
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

	public int iGet1stLoc()
	{
		return i1stLoc;
	}

	public int iGet2ndLoc()
	{
		return i2ndLoc;
	}

	public void vSetBeta( double lfBetaData )
	{
		lfBeta = lfBetaData;
	}

	public void vSetAlpha( double lfAlphaData )
	{
		lfAlpha = lfAlphaData;
	}
}
