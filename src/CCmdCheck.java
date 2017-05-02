
public class CCmdCheck
{

	private int iGenerationNumber;		// �v�Z��
	private int iAbcDataNum;		// ���q�Q�̐�
	private int iAbcVectorDimNum;		// �e���q�Q�̓����x�N�g��
	private int iAbcMethod;			// ���q�Q�œK���@�̎�@�ݒ�
	private String strFuncName;		// �g�p����֐��p�t���O
	private String strConditionName;// ���������
	private int iOutputFlag;		// ���ʏo�͗p�t���O
	private int iIntervalMinNum;		// �Œᔽ����
	private int iAbcSearchNum;		// employ bee�̑���
	private int iAbcLimitCount;		// scout bee����ϐ�
	private int iAbcUpperSearchNum;		// �T���_��ʐ�
	private double lfConvergenceParam;	// ���ւ̎����󋵃p�����[�^
	private double lfFitBound;		// �K���x���e���E�l
	private double lfFitAccuracy;		// �K���x�̎������x
	private double lfRange;			// ���̒T���͈�
	private double lfRangeMin;			// ���̒T���͈͂̍ŏ��l
	private double lfRangeMax;			// ���̒T���͈͂̍ő�l
	private int iCrossOverNum;		// ������
	private double lfAlpha;			// UNDX�p�����[�^�̃�
	private double lfBeta;			// UNDX�p�����[�^�̃�
	private double lfLearningRate;
	private int iParentNum;
	private int iChildrenNum;
	private int iUpperEvalChildrenNum;
	private int iFinishFlag;		// �I������

	public CCmdCheck()
	{
		iGenerationNumber = 0;		// �v�Z��
		iAbcDataNum = 0;			// ���q�Q�̐�
		iAbcVectorDimNum = 0;		// �e���q�Q�̓����x�N�g��
		iAbcMethod = 0;				// ���q�Q�œK���@�̎�@�ݒ�
		strFuncName = null;			// �ړI�֐�
		strConditionName = "none";	// �������
		iOutputFlag = 0;			// ���ʏo�͗p�t���O
		iIntervalMinNum = 0;		// �Œᔽ����
		iAbcSearchNum = 0;			// employ bee�̑���
		iAbcLimitCount = 0;			// scout bee����ϐ�
		iAbcUpperSearchNum = 0;		// �T���_��ʐ�
		lfConvergenceParam = 0.0;	// ���ւ̎����󋵃p�����[�^
		lfFitBound = 0.0;			// �K���x���e���E�l
		lfFitAccuracy = 0.0;		// �K���x�̎������x
		lfAlpha = 0.0;				// Undx�̃�
		lfBeta = 0.0;				// Undx�̃�
		iCrossOverNum = 0;			// ������
		lfRangeMin = lfRangeMax = lfRange = 0.0;
	}

	/**
	 *<PRE>
	 *  ���͂��ꂽ�R�}���h���`�F�b�N
	 *  ver 0.1 ����
	 *  ver 0.2 �I�v�V�������̒ǉ��A����т��̂ق��̏C���B
	 *  ver 0.3 �l�H�m�\�p�O���t�����c�[���쐬�̂��ߐV���ɏC���B
	 *  ver 0.4
	 *</PRE>
	 * @param args	�R�}���h�̓��͍��ڐ�
	 * @param argv	���͂����R�}���h�̏ڍ�
	 * @return	CCMD_SUCCESS
	 * 			CCMD_ERROR_INVALID_FORMAT
	 * 			CCMD_ERROR_INVALID_DATA
	 * 			CCMD_ERROR_MEMORY_ALLOCATE
	 * 			CCMD_ERROR_MEMORY_RELEASE
	 * @author kobayashi
	 * @since 0.1 2014/05/02
	 * @version 0.1
	 */
	public long lCommandCheck( String[] args )
	{
		int i;
		long lRet = 0;

		/* �R�}���h�̃`�F�b�N */
		if( args.length <= 1 )
		{
			return -101;
		}
		if( args == null )
		{
			return -101;
		}
		for( i=0; i< args.length ;i++ )
		{
			/* �v�Z�� */
			if( args[i].equals( "-gn" ) == true )
			{
				lRet = lCommandErrorCheck( args[i] );
				if( lRet != 0 ) return lRet;
				iGenerationNumber = Integer.parseInt( args[i+1] );
				i++;
			}
			/* colony�̐� */
			else if( args[i].equals( "-an" ) == true )
			{
				lRet = lCommandErrorCheck( args[i] );
				if( lRet != 0 ) return lRet;
				iAbcDataNum = Integer.parseInt( args[i+1] );
				i++;
			}
			/* �I�̃x�N�g���� */
			else if( args[i].equals( "-vn" ) == true )
			{
				lRet = lCommandErrorCheck( args[i] );
				if( lRet != 0 ) return lRet;
				iAbcVectorDimNum = Integer.parseInt( args[i+1] );
				i++;
			}
			/* employ bee�̑��� */
			else if( args[i].equals( "-sn" ) == true )
			{
				lRet = lCommandErrorCheck( args[i] );
				if( lRet != 0 ) return lRet;
				iAbcSearchNum = Integer.parseInt( args[i+1] );
				i++;
			}
			/* scout ����ϐ� */
			else if( args[i].equals( "-cl" ) == true )
			{
				lRet = lCommandErrorCheck( args[i] );
				if( lRet != 0 ) return lRet;
				iAbcLimitCount = Integer.parseInt( args[i+1] );
				i++;
			}
			/* �K���x��ʂ̒T���_�� */
			else if( args[i].equals( "-usn" ) == true )
			{
				lRet = lCommandErrorCheck( args[i] );
				if( lRet != 0 ) return lRet;
				iAbcUpperSearchNum = Integer.parseInt( args[i+1] );
				i++;
			}
			/* �K���x�������x */
			else if( args[i].equals( "-fa" ) == true )
			{
				lRet = lCommandErrorCheck( args[i] );
				if( lRet != 0 ) return lRet;
				lfFitAccuracy = Double.parseDouble( args[i+1] );
				i++;
			}
			/* �K���x���e���E�l */
			else if( args[i].equals( "-fb" ) == true )
			{
				lRet = lCommandErrorCheck( args[i] );
				if( lRet != 0 ) return lRet;
				lfFitBound = Double.parseDouble( args[i+1] );
				i++;
			}
			/* �Œᔽ���� */
			else if( args[i].equals( "-imn" ) == true )
			{
				lRet = lCommandErrorCheck( args[i] );
				if( lRet != 0 ) return lRet;
				iIntervalMinNum = Integer.parseInt( args[i+1] );
				i++;
			}
			/* ���ւ̎����󋵃p�����[�^ */
			else if( args[i].equals( "-cp" ) == true )
			{
				lRet = lCommandErrorCheck( args[i] );
				if( lRet != 0 ) return lRet;
				lfConvergenceParam = Double.parseDouble( args[i+1] );
				i++;
			}
			/* ABC�@�̎�@�ݒ� */
			else if( args[i].equals( "-abcm" ) == true )
			{
				lRet = lCommandErrorCheck( args[i] );
				if( lRet != 0 ) return lRet;
				iAbcMethod = Integer.parseInt( args[i+1] );
				i++;
			}
			/* �]�������{����ړI�֐� */
			else if( args[i].equals( "-f" ) == true )
			{
				lRet = lCommandErrorCheck( args[i] );
				if( lRet != 0 ) return lRet;
				strFuncName = args[i+1];
				i++;
			}
			/* �]�������{����ړI�֐� */
			else if (args[i].equals("-cc") == true )
			{
				lRet = lCommandErrorCheck(args[i]);
				if (lRet != 0) return lRet;
				strConditionName = args[i + 1];
				i++;
			}
			/* ��T���͈� */
			else if( args[i].equals( "-r" ) == true )
			{
				lRet = lCommandErrorCheck( args[i] );
				if( lRet != 0 ) return lRet;
				lfRange = Double.parseDouble( args[i+1] );
				i++;
			}
			/* ��T���͈� */
			else if ( args[i].equals( "-rmin" ) == true )
			{
				lRet = lCommandErrorCheck(args[i]);
				if (lRet != 0) return lRet;
				lfRangeMin = Double.parseDouble(args[i + 1]);
				i++;
			}
			/* ��T���͈� */
			else if ( args[i].equals( "-rmax" ) == true )
			{
				lRet = lCommandErrorCheck(args[i]);
				if (lRet != 0) return lRet;
				lfRangeMax = Double.parseDouble(args[i + 1]);
				i++;
			}
			/* ���ʏo�� */
			else if( args[i].equals( "-out" ) == true )
			{
				lRet = lCommandErrorCheck( args[i] );
				if( lRet != 0 ) return lRet;
				iOutputFlag = Integer.parseInt( args[i+1] );
				i++;
			}
			/* ������ */
			else if( args[i].equals( "-cr" ) == true )
			{
				lRet = lCommandErrorCheck( args[i] );
				if( lRet != 0 ) return lRet;
				iCrossOverNum = Integer.parseInt( args[i+1] );
				i++;
			}
			/* Undx�̃p�����[�^�� */
			else if( args[i].equals( "-alpha" ) == true )
			{
				lRet = lCommandErrorCheck( args[i] );
				if( lRet != 0 ) return lRet;
				lfAlpha = Double.parseDouble( args[i+1] );
				i++;
			}
			/* Undx�̃p�����[�^�� */
			else if( args[i].equals( "-beta" ) == true )
			{
				lRet = lCommandErrorCheck( args[i] );
				if( lRet != 0 ) return lRet;
				lfBeta = Double.parseDouble( args[i+1] );
				i++;
			}
			/* Rex�̑I������e�̐� */
			else if( args[i].equals( "-pn" ) == true )
			{
				lRet = lCommandErrorCheck( args[i] );
				if( lRet != 0 ) return lRet;
				iParentNum = Integer.parseInt( args[i+1] );
				i++;
			}
			/* Rex�̐�������q���̐� */
			else if( args[i].equals( "-cn" ) == true )
			{
				lRet = lCommandErrorCheck( args[i] );
				if( lRet != 0 ) return lRet;
				iChildrenNum = Integer.parseInt( args[i+1] );
				i++;
			}
			/* ARex�̐��������q�������ʑI�����鐔 */
			else if( args[i].equals( "-cl" ) == true )
			{
				lRet = lCommandErrorCheck( args[i] );
				if( lRet != 0 ) return lRet;
				iUpperEvalChildrenNum = Integer.parseInt( args[i+1] );
				i++;
			}
			/* ARex�̃p�����[�^�w�K�� */
			else if( args[i].equals( "-lr" ) == true )
			{
				lRet = lCommandErrorCheck( args[i] );
				if( lRet != 0 ) return lRet;
				lfLearningRate = Double.parseDouble( args[i+1] );
				i++;
			}
			/* �I�������̎w�� */
			else if( args[i].equals( "-fin" ) == true )
			{
				lRet = lCommandErrorCheck( args[i] );
				if( lRet != 0 ) return lRet;
				iFinishFlag = Integer.parseInt( args[i+1] );
				i++;
			}
		}
		return lRet;
	}

	/**
	 *<PRE>
	 * ���̓I�v�V�������ǂ������`�F�b�N����
	 * ver 0.1 �V�K�쐬
	 * ver 0.2 �l�H�m�\�p�O���t�����c�[���쐬�p�ɏC���B
	 *</PRE>
	 * @param argc
	 * @param argv
	 * @return 0
	 *	  -1
	 *        -2
	 * @author kobayashi
	 * @since 2013/1/1
	 * @version 0.2
	 */
	private long lCommandErrorCheck( String arg )
	{
		long lRet = 0L;
		if( arg.equals( "-gn" ) == true 		||
			arg.equals( "-an" ) == true			||
			arg.equals( "-vn" ) == true 		||
			arg.equals( "-sn" ) == true  	 	||
			arg.equals( "-cl" ) == true  	 	||
			arg.equals( "-usn" ) == true 		||
			arg.equals( "-fa" ) == true 		||
			arg.equals( "-fb" ) == true 		||
			arg.equals( "-imn" ) == true 		||
			arg.equals( "-cp" ) == true 		||
			arg.equals( "-abcm" ) == true 		||
			arg.equals( "-f" ) == true 			||
			arg.equals( "-cc" ) == true 		||
			arg.equals( "-r" ) == true 			||
			arg.equals( "-rmin") == true		||
			arg.equals( "-rmax") == true		||
			arg.equals( "-out" ) == true  		||
			arg.equals( "-cr" ) == true 		||
			arg.equals( "-alpha" ) == true 		||
			arg.equals( "-beta" ) == true 		||
			arg.equals( "-pn" ) == true 		||
			arg.equals( "-cn" ) == true 		||
			arg.equals( "-cl" ) == true 		||
			arg.equals( "-fin" ) == true 		||
			arg.equals( "-lr" ) == true )
		{
			lRet = 0;
		}
		else
		{
			lRet = -2;
		}
		return lRet;
	}

	/**
	 *<PRE>
	 * �g�p���@��\������B
	 * ver 0.1
	 * ver 0.2 Rex���g�p���邽�߂̃p�����[�^�̒ǉ�
	 *</PRE>
	 * @author kobayashi
	 * @since 2015/8/3
	 * @version 0.2
	 */
	public void vHelp()
	{
		System.out.print("ABC�@�v�Z\n");
		System.out.print("�g�p���@\n");
		System.out.print("abc [-gn][-an][-vn][-sn][-cl][-usn][-fa][-fb][-imn][-cp][-abcm][-f][-cc][-r][-rmin][-rmax][-cr][alpha][-beta][-pn][-cn][-cl][-lr][-fin][-out]\n");
		System.out.print("-gn �X�V��\n");
		System.out.print("-an colony��)\n");
		System.out.print("-vn �I�̃x�N�g����\n");
		System.out.print("-sn employ bee�̐�\n");
		System.out.print("-cl scout bee����ϐ�\n");
		System.out.print("-usn �K���x��ʂ̒T���_��\n");
		System.out.print("-fa �K���x�������x\n");
		System.out.print("-fb �K���x���e���E�l\n");
		System.out.print("-imn �Œᔽ����\n");
		System.out.print("-cp ���ւ̎����󋵃p�����[�^\n");
		System.out.print("-abcm ABC�@�̎�@�ݒ�\n");
		System.out.print("-f �ړI�֐��̐ݒ�\n");
		System.out.print("-cc ��������̐ݒ�\n");
		System.out.print("-r �ړI�֐��̉�T���͈�\n");
		System.out.print("-rmin �ړI�֐��̉�T���͈͂̍ŏ��l\n");
		System.out.print("-rmax �ړI�֐��̉�T���͈͂̍ő�l\n");
		System.out.print("-cr ������\n");
		System.out.print("-alpha Undx�̃�\n");
		System.out.print("-beta Undx�̃�\n");
		System.out.print("-pn Rex�̑I������e�̐�\n");
		System.out.print("-cn Rex�̐�������q���̐�\n");
		System.out.print("-cl ARex�̐��������q���̂�����ʑI�����鐔\n");
		System.out.print("-lr ARex�̊w�K��\n");
		System.out.print("-fin �I�������̐ݒ�1:��, 2:����\n");
		System.out.print("-out ���ʏo��\n");
	}

	public 	int iGetIntervalMinNum()
	{
		return iIntervalMinNum;
	}

	public 	int iGetAbcLimitCount()
	{
		return iAbcLimitCount;
	}

	public 	int iGetAbcSearchNum()
	{
		return iAbcSearchNum;
	}

	public 	int iGetAbcUpperSearchNum()
	{
		return iAbcUpperSearchNum;
	}

	public 	double lfGetConvergenceParam()
	{
		return lfConvergenceParam;
	}

	public 	double lfGetFitBound()
	{
		return lfFitBound;
	}

	public 	double lfGetFitAccuracy()
	{
		return lfFitAccuracy;
	}

	public String strGetFuncName()
	{
		return strFuncName;
	}

	public String strGetConditionName()
	{
		return strConditionName;
	}

	public 	int iGetGenerationNumber()
	{
		return iGenerationNumber;
	}

	public 	int iGetAbcDataNum()
	{
		return iAbcDataNum;
	}

	public 	int iGetAbcVectorDimNum()
	{
		return iAbcVectorDimNum;
	}

	public 	int iGetAbcMethod()
	{
		return iAbcMethod;
	}

	public 	int iGetOutputFlag()
	{
		return iOutputFlag;
	}

	public 	double lfGetRange()
	{
		return lfRange;
	}

	double lfGetRangeMin()
	{
		return lfRangeMin;
	}

	public 	double lfGetRangeMax()
	{
		return lfRangeMax;
	}

	public 	int iGetCrossOverNum()
	{
		return iCrossOverNum;
	}

	public 	double lfGetAlpha()
	{
		return lfAlpha;
	}

	public 	double lfGetBeta()
	{
		return lfBeta;
	}

	public 	double lfGetLearningRate()
	{
		return lfLearningRate;
	}

	public 	int iGetParentNum()
	{
		return iParentNum;
	}

	public 	int iGetChildrenNum()
	{
		return iChildrenNum;
	}

	public int iGetUpperEvalChildrenNum()
	{
		return iUpperEvalChildrenNum;
	}

	public int iGetFinishFlag()
	{
		return iFinishFlag;
	}
}