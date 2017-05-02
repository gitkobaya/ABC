package rcga;

import sfmt.Sfmt;
import rankt.Rank_t;
import objectivefunction.ObjectiveFunctionInterface;


public class CRealCodedGa
{
	protected int iGenerationNumber;
	protected int iGenNumber;
	protected int iGenVector;
	protected double[][] pplfGens;
	protected Sfmt rnd;
	protected ObjectiveFunctionInterface pflfObjectiveFunction;

	public CRealCodedGa()
	{
		iGenerationNumber = 0;
		iGenNumber = 0;
		iGenVector = 0;
		pplfGens =  null;
	}

	public CRealCodedGa( int iGenerationNum, int iGenNum, int iGenVectorData )
	{
		vInitialize( iGenerationNum, iGenNum, iGenVectorData );
	}

	public void vInitialize( int iGenerationNum, int iGenNum, int iGenVectorData )
	{
		int i,j;

		iGenerationNumber = iGenerationNum;
		iGenNumber = iGenNum;
		iGenVector = iGenVectorData;

		pplfGens = new double[iGenNumber][iGenVector];

		for( i = 0;i < iGenNumber; i++ )
			for( j = 0;j < iGenVector; j++ )
				pplfGens[i][j] = 0.0;

		long seed;
		seed = System.currentTimeMillis();
		rnd = new Sfmt( (int)seed );
	}

	/**
	 * <PRE>
	 * �@�����ɂ��GA��ݒ肵�܂��B
	 * </PRE>
	 * @author kobayashi
	 * @since 2015/10/30
	 * @version 1.0
	 */
	public void vSetRandom()
	{
		int i,j;

		for( i = 0;i < iGenNumber; i++ )
			for( j = 0;j < iGenVector; j++ )
					pplfGens[i][j] = rnd.NextUnif();
	}

	/**
	 * <PRE>
	 * �@�ړI�֐����C���X�g�[�����܂��B
	 * </PRE>
	 * @param lfDomainMin ���������ŏ��l
	 * @param lfDomainMax ���������ő�l
	 * @author kobayashi
	 * @since 2015/10/30
	 * @version 1.0
	 */
	public void vSetRandom( double lfDomainMin, double lfDomainMax )
	{
		int i,j;
		for( i = 0;i < iGenNumber; i++ )
			for( j = 0;j < iGenVector; j++ )
				pplfGens[i][j] = rnd.NextUnif()*(lfDomainMax-lfDomainMin) + lfDomainMin;
	}

	/**
	 * <PRE>
	 * �@�ړI�֐����C���X�g�[�����܂��B
	 * </PRE>
	 * @author kobayashi
	 * @since 2015/10/30
	 * @version 1.0
	 */
	public void vTerminate()
	{
		pplfGens = null;
	}

	/**
	 * <PRE>
	 * �@�ړI�֐����C���X�g�[�����܂��B
	 * �@���ۂɂ̓R�[���o�b�N�֐����C���X�g�[�����܂��B
	 * </PRE>
	 * @param pflfFunction �ړI�֐��̊֐��|�C���^
	 * @author kobayashi
	 * @since 2015/10/30
	 * @version 1.0
	 */
	public void vSetConstraintFunction( ObjectiveFunctionInterface pflfFunction )
	{
		pflfObjectiveFunction = pflfFunction;
	}

	/**
	 * <PRE>
	 * �@�ړI�֐����A���C���X�g�[�����܂��B
	 * �@���ۂɂ̓R�[���o�b�N�֐����A���C���X�g�[�����܂��B
	 * </PRE>
	 * @author kobayashi
	 * @since 2015/10/30
	 * @version 1.0
	 */
	public void vReleaseCallbackConstraintFunction()
	{
		pflfObjectiveFunction = null;
	}

	/**
	 * <PRE>
	 * �@�����l�f�`�̌��ʂ��o�͂��܂��B(�e��`�q�̃x�N�g��)
	 * </PRE>
	 * @author kobayashi
	 * @since 2015/12/16
	 * @version 1.0
	 */
	public void vOutputGenData()
	{
		int i,j;
		// �����_�ł̗��q�̈ʒu���o�͂��܂��B
		for( i = 0; i < iGenNumber; i++ )
		{
			for( j = 0;j < iGenVector; j++ )
				System.out.print(pplfGens[i][j] + "," );
			System.out.print("\n");
		}
	}

	/**
	 * <PRE>
	 * �@���݂̎����lGA�ɖړI�֐���K�p�������ʂ��o�͂��܂��B(�e��`�q�̖ړI�֐��l)
	 * </PRE>
	 * @author kobayashi
	 * @since 2015/12/16
	 * @version 1.0
	 */
	public void vOutputConstraintFunction()
	{
		int i;
		// �����_�ł̊e���q�̖ړI�֐��̒l���o�͂��܂��B
		for( i = 0; i < iGenNumber; i++ )
			System.out.print(pflfObjectiveFunction.lfObjectiveFunction(pplfGens[i]) + "," );
		System.out.print("\n");
	}

	/**
	 * <PRE>
	 * �@�����_�ł̂����Ƃ��悢��`�q�̈ʒu���o�͂��܂��B
	 * </PRE>
	 * @author kobayashi
	 * @since 2015/6/19
	 * @version 1.0
	 */
	public void vOutputGlobalMaxGenData()
	{
		int i;
		int iLoc = 0;
		double lfTemp = 0.0;
		double lfRes = 0.0;
		lfTemp = Double.MAX_VALUE;
		for( i = 0;i < iGenNumber; i++ )
		{
			// �����_�ł̊e��`�q�̖ړI�֐��̒l�������Ƃ��œK�l�ɂȂ��Ă�����̂��o�͂��܂��B
			lfRes = pflfObjectiveFunction.lfObjectiveFunction(pplfGens[i]);
			if( lfRes <= lfTemp )
			{
				lfTemp = lfRes;
				iLoc = i;
			}
		}
		// �œK�l�ɂȂ��Ă����`�q���o�͂��܂��B
		for( i = 0;i < iGenVector; i++ )
			System.out.print(pplfGens[iLoc][i]+",");
		System.out.print("\n");
	}

	/**
	 * <PRE>
	 * �@�����_�ł̂����Ƃ��悢��`�q�̖ړI�֐��l���o�͂��܂��B
	 * </PRE>
	 * @author kobayashi
	 * @since 2015/12/16
	 * @version 1.0
	 */
	public void vOutputGlobalMaxConstFuncValue()
	{
		int i;
		double lfTemp = 0.0;
		double lfRes = 0.0;
		lfTemp = Double.MAX_VALUE;
		for( i = 0;i < iGenNumber; i++ )
		{
			// �����_�ł̊e��`�q�̖ړI�֐��̒l���o�͂��܂��B
			lfRes = pflfObjectiveFunction.lfObjectiveFunction(pplfGens[i]);
			if( lfRes <= lfTemp )
			{
				lfTemp = lfRes;
			}
		}
		// �œK�l�ɂȂ��Ă���ړI�֐��̒l���o�͂��܂��B
		System.out.println( lfTemp );
	}

	/**
	 * <PRE>
	 * �@�����_�ł̂����Ƃ��悢��`�q�y�т��̖ړI�֐��l���o�͂��܂��B
	 * </PRE>
	 * @author kobayashi
	 * @since 2015/12/16
	 * @version 1.0
	 */
	public void vOutputGlobalMaxGenDataConstFuncValue()
	{
		int i;
		int iLoc = 0;
		double lfTemp = 0.0;
		double lfRes = 0.0;
		lfTemp = Double.MAX_VALUE;
		for( i = 0;i < iGenNumber; i++ )
		{
			// �����_�ł̊e��`�q�̖ړI�֐��̒l���o�͂��܂��B
			lfRes = pflfObjectiveFunction.lfObjectiveFunction(pplfGens[i]);
			if( lfRes <= lfTemp )
			{
				lfTemp = lfRes;
				iLoc = i;
			}
		}
		// �œK�l�ɂȂ��Ă����`�q���o�͂��܂��B
		for( i = 0;i < iGenVector; i++ )
			System.out.print( pplfGens[iLoc][i] );
		// �œK�l�ɂȂ��Ă���ړI�֐��̒l���o�͂��܂��B
		System.out.print( lfTemp );
	}
}
