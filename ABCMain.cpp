#include<cstdio>
#include<cstring>
#include<iostream>
#include"ABC.h"
#include"EvaluationFunction.h"
#include"cmd_check.h"

extern void vInitialize( CCmdCheck *pcCmd, CAbc *pcAbc );
extern void vSetObjectiveFunction( CCmdCheck *pcCmd, CAbc *pcAbc );
extern void vStartAbc( CCmdCheck *pcCmd, CAbc *pcAbc, int iLoc );
extern void vTerminate( CAbc *pcAbc );
extern void vOutputData( CCmdCheck *pcCmd, CAbc *pcAbc );
extern void vSetRandom( CCmdCheck *pcCmd, CAbc *pcAbc );
extern int iFinisher( CCmdCheck *pcCmd, CAbc *pcAbc, int iCount ); 

int main(int argc, char* argv[])
{
	int i;
	long lRet = 0L;
	CAbc abc;
	CCmdCheck cmd;

	try
	{
		lRet = cmd.lCommandCheck( argc, argv );
		if( lRet == 0 )
		{
			// �����������s���܂��B
			vInitialize( &cmd, &abc );

			// �ړI�֐���ݒ肵�܂��B
			vSetObjectiveFunction( &cmd, &abc );


			// �����l��ݒ肵�܂��B
			vSetRandom( &cmd, &abc );
						
			// ���ʂ��o�͂��܂��B
//			vOutputData( &cmd, &abc );
			
			for( i = 0;i < cmd.iGetGenerationNumber(); i++ )
			{
				// ���q�Q�œK�������s���܂��B
				vStartAbc( &cmd, &abc, i );
				
				// ���ʂ��o�͂��܂��B
				vOutputData( &cmd, &abc );

				// �I���������Ď����܂��B
				lRet = iFinisher( &cmd, &abc, i );
				if( lRet == 1) break;
			}
			if( cmd.iGetFinishFlag() == 2 )
			{
				printf("�J��Ԃ���,%d\n",i);
			}
			// �I�����������s���܂��B
			vTerminate( &abc );
		}
		else
		{
			// �R�}���h�̎g�p���@��\�����܂��B
			cmd.vHelp();
		}
	}
	catch( CCmdCheckException cce )
	{
		std::cout << cce.strGetClassName() << "::" << cce.strGetMethodName() << ":" << cce.strGetErrDetail() << std::endl;
		return cce.iGetErrCode();
	}
	catch( CAbcException cae )
	{
		std::cout << cae.strGetClassName() << "::" << cae.strGetMethodName() << ":" << cae.strGetErrDetail() << std::endl;
		return cae.iGetErrCode();
	}
	catch( ... )
	{
		return -1;
	}
	return 0;
}

/**
 *<PRE>
 *  �����������s���܂��B
 *  ver 0.1 ����
 *</PRE>
 * @param pcCmd	�R�}���h�`�F�b�N�N���X
 * @param pcAbc	ABC�A���S���Y�������s����N���X�C���X�^���X
 * @throw CAbcException
 * @author kobayashi
 * @since 0.1 2015/07/28
 * @version 0.1
 */
void vInitialize( CCmdCheck *pcCmd, CAbc *pcAbc )
{
	int iGenerationNumber;
	int iAbcDataNum;
	int iAbcVectorDimNum;
	int iAbcLimitCount;
	int iAbcSearchNum;
	int iIntervalMinNum;
	int iAbcUpperSearchNum;
	double lfConvergenceParam;
	double lfFitBound;
	double lfFitAccuracy;
	double lfRange;
	int iCrossOverNum;
	double lfAlpha;
	double lfBeta;
	int iParentNumber;
	int iChildrenNumber;
	int iUpperEvalChildrenNumber;
	double lfLearningRate;
	
	iGenerationNumber	= pcCmd->iGetGenerationNumber();
	iIntervalMinNum		= pcCmd->iGetIntervalMinNum();
	iAbcDataNum			= pcCmd->iGetAbcDataNum();
	iAbcVectorDimNum	= pcCmd->iGetAbcVectorDimNum();
	iAbcSearchNum		= pcCmd->iGetAbcSearchNum();
	iAbcLimitCount		= pcCmd->iGetAbcLimitCount();
	iAbcUpperSearchNum	= pcCmd->iGetAbcUpperSearchNum();
	lfConvergenceParam	= pcCmd->lfGetConvergenceParam();
	lfFitBound			= pcCmd->lfGetFitBound();
	lfFitAccuracy		= pcCmd->lfGetFitAccuracy();
	lfRange			= pcCmd->lfGetRange();
	iCrossOverNum		= pcCmd->iGetCrossOverNum();
	lfAlpha			= pcCmd->lfGetAlpha();
	lfBeta			= pcCmd->lfGetBeta();
	iChildrenNumber		= pcCmd->iGetChildrenNum();
	iParentNumber		= pcCmd->iGetParentNum();
	iUpperEvalChildrenNumber= pcCmd->iGetUpperEvalChildrenNum();
	lfLearningRate		= pcCmd->lfGetLearningRate();

	// �I���W�i��Artificial Bee Colony Method(2005)
	if( pcCmd->iGetAbcMethod() == 1 )
	{
		pcAbc->vInitialize( iGenerationNumber, iAbcDataNum, iAbcVectorDimNum, iAbcSearchNum, iAbcLimitCount );
		pcAbc->vSetRange( lfRange );
	}
	// �ό`Artificial Bee Colony Method (2011)
	else if( pcCmd->iGetAbcMethod() == 2 )
	{
		pcAbc->vInitialize( iGenerationNumber, iAbcDataNum, iAbcVectorDimNum, iAbcSearchNum, iAbcLimitCount, iIntervalMinNum, iAbcUpperSearchNum, lfConvergenceParam, lfFitBound, lfFitAccuracy );
		pcAbc->vSetRange( lfRange );
	}
	// ������p����Artificial Bee Colony Method (2013)
	else if( pcCmd->iGetAbcMethod() == 3 )
	{
		pcAbc->vInitialize( iGenerationNumber, iAbcDataNum, iAbcVectorDimNum, iAbcSearchNum, iAbcLimitCount );
		pcAbc->vSetRange( lfRange );
	}
	//�@GbestABC�@
	else if( pcCmd->iGetAbcMethod() == 4 )
	{
		pcAbc->vInitialize( iGenerationNumber, iAbcDataNum, iAbcVectorDimNum, iAbcSearchNum, iAbcLimitCount );
		pcAbc->vSetRange( lfRange );
	}
	//�@Memetic ABC�@
	else if( pcCmd->iGetAbcMethod() == 5 )
	{
		pcAbc->vInitialize( iGenerationNumber, iAbcDataNum, iAbcVectorDimNum, iAbcSearchNum, iAbcLimitCount );
		pcAbc->vSetRange( lfRange );
	}
	//�@Memetic ABC Algorithm(2013)
	else if( pcCmd->iGetAbcMethod() == 6 )
	{
//		pcAbc->vInitialize( iGenerationNumber, iAbcDataNum, iAbcVectorDimNum, iAbcSearchNum, iAbcLimitCount );
		pcAbc->vInitialize( iGenerationNumber, iAbcDataNum, iAbcVectorDimNum, iAbcSearchNum, iAbcLimitCount, iCrossOverNum, lfAlpha, lfBeta );
		pcAbc->vSetRange( lfRange );
	}
	// UNDX���������n�C�u���b�hABC�@(��Ď�@)
	else if( pcCmd->iGetAbcMethod() == 7 )
	{
		pcAbc->vInitialize( iGenerationNumber, iAbcDataNum, iAbcVectorDimNum, iAbcSearchNum, iAbcLimitCount, iCrossOverNum, lfAlpha, lfBeta );
		pcAbc->vSetRange( lfRange );
	}
	// REX���������n�C�u���b�hABC�@(��Ď�@3)
	else if( pcCmd->iGetAbcMethod() == 8 )
	{
		pcAbc->vInitialize( iGenerationNumber, iAbcDataNum, iAbcVectorDimNum, iAbcSearchNum, iAbcLimitCount, iIntervalMinNum, iAbcUpperSearchNum, lfConvergenceParam, lfFitBound, lfFitAccuracy, iParentNumber, iChildrenNumber, iUpperEvalChildrenNumber, lfLearningRate );
		pcAbc->vSetRange( lfRange );
	}
	// AREX���������n�C�u���b�hABC�@(��Ď�@4)
	else if( pcCmd->iGetAbcMethod() == 9 )
	{
		pcAbc->vInitialize( iGenerationNumber, iAbcDataNum, iAbcVectorDimNum, iAbcSearchNum, iAbcLimitCount, iIntervalMinNum, iAbcUpperSearchNum, lfConvergenceParam, lfFitBound, lfFitAccuracy, iParentNumber, iChildrenNumber, iUpperEvalChildrenNumber, lfLearningRate );
		pcAbc->vSetRange( lfRange );
	}
	// HJABC�@
	else if (pcCmd->iGetAbcMethod() == 10)
	{
		pcAbc->vInitialize(iGenerationNumber, iAbcDataNum, iAbcVectorDimNum, iAbcSearchNum, iAbcLimitCount, iIntervalMinNum, iAbcUpperSearchNum, lfConvergenceParam, lfFitBound, lfFitAccuracy, iParentNumber, iChildrenNumber, iUpperEvalChildrenNumber, lfLearningRate);
		pcAbc->vSetRange(lfRange);
	}
	// ACABC�@
	else if (pcCmd->iGetAbcMethod() == 11 )
	{
		pcAbc->vInitialize(iGenerationNumber, iAbcDataNum, iAbcVectorDimNum, iAbcSearchNum, iAbcLimitCount, iIntervalMinNum, iAbcUpperSearchNum, lfConvergenceParam, lfFitBound, lfFitAccuracy, iParentNumber, iChildrenNumber, iUpperEvalChildrenNumber, lfLearningRate);
		pcAbc->vSetRange(lfRange);
	}
}

/**
 *<PRE>
 *  �I�����������s���܂��B
 *  ver 0.1 ����
 *</PRE>
 * @param pcAbc	ABC�A���S���Y�������s����N���X�C���X�^���X
 * @throw CAbcException
 * @author kobayashi
 * @since 0.1 2015/07/28
 * @version 0.1
 */
void vTerminate( CAbc *pcAbc )
{
	// �ړI�֐��̃A���C���X�g�[�������s���܂��B
	pcAbc->vReleaseCallConstraintFunction();
	
	// ���q�Q�œK���̏������I�����܂��B
	pcAbc->vTerminate();
}

/**
 *<PRE>
 *  �ړI�֐���ݒ肵�܂��B
 *  ver 0.1 ����
 *  ver 0.2 2016/08/10 �x���`�}�[�N�֐��ǉ�
 *  ver 0.3 2016/09/14 �x���`�}�[�N�֐��ǉ�
 *</PRE>
 * @param pcCmd	�R�}���h�`�F�b�N�N���X
 * @param pcAbc	ABC�A���S���Y�������s����N���X�C���X�^���X
 * @throw CAbcException
 * @author kobayashr
 * @since 0.1 2015/07/27
 * @version 0.3
 */
void vSetObjectiveFunction( CCmdCheck *pcCmd, CAbc *pcAbc )
{
	if( strcmp(pcCmd->pcGetFuncName(), "sphere" ) == 0 )
	{
		pcAbc->vSetConstraintFunction( lfSphere );
	}
	else if( strcmp(pcCmd->pcGetFuncName(), "ellipsoid" ) == 0 )
	{
		pcAbc->vSetConstraintFunction( lfEllipsoid );
	}
	else if( strcmp(pcCmd->pcGetFuncName(), "hyper-ellipsoid" ) == 0 )
	{
		pcAbc->vSetConstraintFunction( lfHyperEllipsoid );
	}
	else if( strcmp(pcCmd->pcGetFuncName(), "axis-parallel-hyper-ellipsoid" ) == 0 )
	{
		pcAbc->vSetConstraintFunction( lfAxisParallelHyperEllipsoid );
	}
	else if( strcmp(pcCmd->pcGetFuncName(), "rotated-hyper-ellipsoid" ) == 0 )
	{
		pcAbc->vSetConstraintFunction( lfRotatedHyperEllipsoid );
	}
	else if( strcmp(pcCmd->pcGetFuncName(), "moved-axis-parallel-hyper-ellipsoid" ) == 0 )
	{
		pcAbc->vSetConstraintFunction( lfMovedAxisParallelHyperEllipsoid );
	}
	else if( strcmp(pcCmd->pcGetFuncName(), "sum-of-different-power" ) == 0 )
	{
		pcAbc->vSetConstraintFunction( lfSumOfDifferentPower );
	}
	else if( strcmp(pcCmd->pcGetFuncName(), "rosenbrock" ) == 0 )
	{
		pcAbc->vSetConstraintFunction( lfRosenbrock );
	}
	else if( strcmp(pcCmd->pcGetFuncName(), "rosenbrockstar" ) == 0 )
	{
		pcAbc->vSetConstraintFunction( lfRosenbrockStar );
	}
	else if( strcmp(pcCmd->pcGetFuncName(), "3rd-de-jongs" ) == 0 )
	{
		pcAbc->vSetConstraintFunction( lf3rdDeJongsFunc );
	}
	else if( strcmp(pcCmd->pcGetFuncName(), "modified-3rd-de-jongs" ) == 0 )
	{
		pcAbc->vSetConstraintFunction( lfModified3rdDeJongsFunc );
	}
	else if( strcmp(pcCmd->pcGetFuncName(), "4th-de-jongs" ) == 0 )
	{
		pcAbc->vSetConstraintFunction( lf4thDeJongsFunc );
	}
	else if( strcmp(pcCmd->pcGetFuncName(), "modified-4th-de-jongs" ) == 0 )
	{
		pcAbc->vSetConstraintFunction( lfModified4thDeJongsFunc );
	}
	else if( strcmp(pcCmd->pcGetFuncName(), "5th-de-jongs" ) == 0 )
	{
		pcAbc->vSetConstraintFunction( lf5thDeJongsFunc );
	}
	else if( strcmp(pcCmd->pcGetFuncName(), "Ackley" ) == 0 )
	{
		pcAbc->vSetConstraintFunction( lfAckley );
	}
	else if( strcmp(pcCmd->pcGetFuncName(), "Easoms" ) == 0 )
	{
		pcAbc->vSetConstraintFunction( lfEasoms );
	}
	else if( strcmp(pcCmd->pcGetFuncName(), "ExtendEasoms" ) == 0 )
	{
		pcAbc->vSetConstraintFunction( lfExtendEasoms );
	}
	else if( strcmp(pcCmd->pcGetFuncName(), "EqualityConstrained" ) == 0 )
	{
		pcAbc->vSetConstraintFunction( lfEqualityConstrained );
	}
	else if( strcmp(pcCmd->pcGetFuncName(), "griewank" ) == 0 )
	{
		pcAbc->vSetConstraintFunction( lfGriewank );
	}
	else if( strcmp(pcCmd->pcGetFuncName(), "michaelwicz" ) == 0 )
	{
		pcAbc->vSetConstraintFunction( lfMichaelwicz );
	}
	else if( strcmp(pcCmd->pcGetFuncName(), "katsuura" ) == 0 )
	{
		pcAbc->vSetConstraintFunction( lfKatsuura );
	}
	else if( strcmp(pcCmd->pcGetFuncName(), "rastrigin" ) == 0 )
	{
		pcAbc->vSetConstraintFunction( lfRastrigin );
	}
	else if( strcmp(pcCmd->pcGetFuncName(), "rastriginshift" ) == 0 )
	{
		pcAbc->vSetConstraintFunction( lfRastriginShift );
	}
	else if( strcmp(pcCmd->pcGetFuncName(), "Schwefel" ) == 0 )
	{
		pcAbc->vSetConstraintFunction( lfSchwefel );
	}
	else if( strcmp(pcCmd->pcGetFuncName(), "Sixhumpcamelback" ) == 0 )
	{
		pcAbc->vSetConstraintFunction( lfSixHumpCamelBack );
	}
	else if( strcmp(pcCmd->pcGetFuncName(), "langermann" ) == 0 )
	{
		pcAbc->vSetConstraintFunction( lfLangermann );
	}
	else if( strcmp(pcCmd->pcGetFuncName(), "Braninsrcos" ) == 0 )
	{
		pcAbc->vSetConstraintFunction( lfBraninsRCos );
	}
	else if( strcmp(pcCmd->pcGetFuncName(), "Shubert" ) == 0 )
	{
		pcAbc->vSetConstraintFunction( lfShubert );
	}
	else if( strcmp(pcCmd->pcGetFuncName(), "dropwave" ) == 0 )
	{
		pcAbc->vSetConstraintFunction( lfDropWave );
	}
	else if( strcmp(pcCmd->pcGetFuncName(), "goldsteinprice" ) == 0 )
	{
		pcAbc->vSetConstraintFunction( lfGoldsteinPrice );
	}
	else if( strcmp(pcCmd->pcGetFuncName(), "Shekelsfoxholes" ) == 0 )
	{
		pcAbc->vSetConstraintFunction( lfShekelsFoxholes );
	}
	else if( strcmp(pcCmd->pcGetFuncName(), "Pavianifoxholes" ) == 0 )
	{
		pcAbc->vSetConstraintFunction( lfPavianiFoxholes );
	}
	else if( strcmp(pcCmd->pcGetFuncName(), "Sineenvelopesinewave" ) == 0 )
	{
		pcAbc->vSetConstraintFunction( lfSineEnvelopeSineWave );
	}
	else if( strcmp(pcCmd->pcGetFuncName(), "Eggholder" ) == 0 )
	{
		pcAbc->vSetConstraintFunction( lfEggHolder );
	}
	else if( strcmp(pcCmd->pcGetFuncName(), "rana" ) == 0 )
	{
		pcAbc->vSetConstraintFunction( lfRana );
	}
	else if( strcmp(pcCmd->pcGetFuncName(), "pathologicaltest" ) == 0 )
	{
		pcAbc->vSetConstraintFunction( lfPathologicalTest );
	}
	else if( strcmp(pcCmd->pcGetFuncName(), "Mastercosniewave" ) == 0 )
	{
		pcAbc->vSetConstraintFunction( lfMasterCosineWave );
	}
	else if( strcmp(pcCmd->pcGetFuncName(), "keane" ) == 0 )
	{
		pcAbc->vSetConstraintFunction( lfKeane );
	}
	else if( strcmp(pcCmd->pcGetFuncName(), "trid" ) == 0 )
	{
		pcAbc->vSetConstraintFunction( lfTrid );
	}
	else if( strcmp(pcCmd->pcGetFuncName(), "ktablet" ) == 0 )
	{
		pcAbc->vSetConstraintFunction( lfkTablet );
	}
	else if( strcmp(pcCmd->pcGetFuncName(), "Schaffer" ) == 0 )
	{
		pcAbc->vSetConstraintFunction( lfSchaffer );
	}
	else if( strcmp(pcCmd->pcGetFuncName(), "Bohachevsky" ) == 0 )
	{
		pcAbc->vSetConstraintFunction( lfBohachevsky );
	}
	else
	{
	}
}

/**
 *<PRE>
 *  Artificial Bee Colony�@�����s���܂��B
 *  ver 0.1 ����
 *</PRE>
 * @param pcCmd	�R�}���h�`�F�b�N�N���X
 * @param pcAbc	ABC�A���S���Y�������s����N���X�C���X�^���X
 * @throw CAbcException
 * @author kobayashi
 * @since 0.1 2015/07/28
 * @version 0.1
 */
void vStartAbc( CCmdCheck *pcCmd, CAbc *pcAbc, int iLoc )
{
	if( pcCmd->iGetAbcMethod() == 1 )
	{
		pcAbc->vAbc();
	}
	else if( pcCmd->iGetAbcMethod() == 2 )
	{
		pcAbc->vModifiedAbc( iLoc );
	}
	else if( pcCmd->iGetAbcMethod() == 3 )
	{
		pcAbc->vCbAbc();
	}
	else if( pcCmd->iGetAbcMethod() == 4 )
	{
		pcAbc->vGAbc();
	}
	else if( pcCmd->iGetAbcMethod() == 5 )
	{
		pcAbc->vMeAbc( iLoc );
	}
	else if( pcCmd->iGetAbcMethod() == 6 )
	{
		pcAbc->vRMAbc( iLoc );
	}
	else if( pcCmd->iGetAbcMethod() == 7 )
	{
		pcAbc->vUndxAbc();
	}
	else if( pcCmd->iGetAbcMethod() == 8 )
	{
		pcAbc->vRexAbc();
	}
	else if( pcCmd->iGetAbcMethod() == 9 )
	{
		pcAbc->vARexAbc();
	}
	else if( pcCmd->iGetAbcMethod() == 10 )
	{
		pcAbc->vHJAbc( iLoc );
	}
	else if (pcCmd->iGetAbcMethod() == 11 )
	{
		pcAbc->vACAbc();
	}
	else
	{
	}
}

/**
 *<PRE>
 *  Artificial Bee Colony�@�����s�������ʂ��o�͂��܂��B
 *  0���w�肷��Ɖ����o�͂��܂���B
 *  ver 0.1 ����
 *</PRE>
 * @param pcCmd	�R�}���h�`�F�b�N�N���X
 * @param pcAbc	ABC�A���S���Y�������s����N���X�C���X�^���X
 * @throw CAbcException
 * @author kobayashi
 * @since 0.1 2015/07/28
 * @version 0.1
 */
void vOutputData( CCmdCheck *pcCmd, CAbc *pcAbc )
{
	if( pcCmd->iGetOutputFlag() == 1 )
	{
		pcAbc->vOutputAbcData();
	}
	else if( pcCmd->iGetOutputFlag() == 2 )
	{
		pcAbc->vOutputVelocityData();
	}
	else if( pcCmd->iGetOutputFlag() == 3 )
	{
		pcAbc->vOutputConstraintFunction();
	}
	else if( pcCmd->iGetOutputFlag() == 4 )
	{
		pcAbc->vOutputGlobalMaxAbcData();
	}
	else if( pcCmd->iGetOutputFlag() == 5 )
	{
		pcAbc->vOutputGlobalMaxAbcDataConstFuncValue();
	}
	else if( pcCmd->iGetOutputFlag() == 5 )
	{
		pcAbc->vOutputGlobalMinAbcData();
	}
	else if( pcCmd->iGetOutputFlag() == 6 )
	{
		pcAbc->vOutputGlobalMinAbcDataConstFuncValue();
	}
	else if( pcCmd->iGetOutputFlag() == 7 )
	{
		pcAbc->vOutputAbcDataLocDist( 0 );
	}
	else if( pcCmd->iGetOutputFlag() == 8 )
	{
		pcAbc->vOutputAbcDataLocDist( 1 );
	}
	else if( pcCmd->iGetOutputFlag() == 9 )
	{
		pcAbc->vOutputLocalMaxAbcData( 0 );
	}
	else if( pcCmd->iGetOutputFlag() == 10 )
	{
		pcAbc->vOutputLocalMaxAbcData( 1 );
	}
}

/**
 *<PRE>
 *  �o�ꂷ�闱�q�̒l�������_���ɐݒ肵�܂��B
 *  ver 0.1 ����
 *</PRE>
 * @param pcCmd	�R�}���h�`�F�b�N�N���X
 * @param pcAbc	ABC�A���S���Y�������s����N���X�C���X�^���X
 * @throw CAbcException
 * @author kobayashi
 * @since 0.1 2015/07/28
 * @version 0.1
 */
void vSetRandom( CCmdCheck *pcCmd, CAbc *pcAbc )
{
	if( pcCmd->iGetAbcMethod() == 2 || pcCmd->iGetAbcMethod() == 8 )
	{
		pcAbc->vSetModifiedRandom( pcCmd->lfGetRange() );
	}
	else
	{
		pcAbc->vSetRandom( pcCmd->lfGetRange() );
	}
}

/**
 *<PRE>
 *  �I��������ݒ肵�܂��B
 *  ver 0.1 ����
 *</PRE>
 * @param pcCmd	�R�}���h�`�F�b�N�N���X
 * @param pcAbc	ABC�A���S���Y�������s����N���X�C���X�^���X
 * @param iCount �J��Ԃ���
 * @throw CAbcException
 * @author kobayashi
 * @since 0.1 2016/09/14
 * @version 0.1
 */
int iFinisher( CCmdCheck *pcCmd, CAbc *pcAbc, int iCount )
{
	int iRet = 0;
	// �񐔂ɂ��I�����w�肵���ꍇ
	if( pcCmd->iGetFinishFlag() == 1 )
	{
		if( iCount == pcCmd->iGetGenerationNumber() )
		{
			iRet = 1;
		}
	}
	// �œK���Ɏ��������ꍇ�ɏI������ꍇ
	else if( pcCmd->iGetFinishFlag() == 2 )
	{
		if( pcAbc->lfGetGlobalMinAbcDataConstFuncValue() <= 0.0000001 )
		{
			iRet = 1;
		}
	}
	else
	{
	}
	return iRet;
}
