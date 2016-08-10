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

			printf("�ʂ�܂����`\n");

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
	
//	printf("start initialization\n ");
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
	iCorssOverNum		= pcCmd->iGetCrossOverNum();
	lfAlpha			= pcCmd->lfGetAlpha();
	lfBeta			= pcCmd->lfGetBeta();

	// �I���W�i��Artificial Bee Colony Method(2005)
	if( pcCmd->iGetAbcMethod() == 1 )
	{
//		printf("start initialization\n ");
		pcAbc->vInitialize( iGenerationNumber, iAbcDataNum, iAbcVectorDimNum, iAbcSearchNum, iAbcLimitCount );
		pcAbc->vSetRange( lfRange );
//		printf("finish initialization\n ");
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
	//�@�V�o�[�W����
	else if( pcCmd->iGetAbcMethod() == 4 )
	{
		pcAbc->vInitialize( iGenerationNumber, iAbcDataNum, iAbcVectorDimNum, iAbcSearchNum, iAbcLimitCount );
		pcAbc->vSetRange( lfRange );
	}
	//�@�V�o�[�W�������̂Q
	else if( pcCmd->iGetAbcMethod() == 5 )
	{
		pcAbc->vInitialize( iGenerationNumber, iAbcDataNum, iAbcVectorDimNum, iAbcSearchNum, iAbcLimitCount );
		pcAbc->vSetRange( lfRange );
	}
	//�@Memetic ABC Algorithm(2013)
	else if( pcCmd->iGetAbcMethod() == 6 )
	{
		pcAbc->vInitialize( iGenerationNumber, iAbcDataNum, iAbcVectorDimNum, iAbcSearchNum, iAbcLimitCount );
		pcAbc->vSetRange( lfRange );
	}
	// UNDX���������n�C�u���b�hABC�@(��Ď�@)
	else if( pcCmd->iGetAbcMethod() == 7 )
	{
		pcAbc->vInitialize( iGenerationNumber, iAbcDataNum, iAbcVectorDimNum, iAbcSearchNum, iAbcLimitCount, iCrossOverNum, lfAlpha, lfBeta );
		pcAbc->vSetRange( lfRange );
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
 *</PRE>
 * @param pcCmd	�R�}���h�`�F�b�N�N���X
 * @param pcAbc	ABC�A���S���Y�������s����N���X�C���X�^���X
 * @throw CAbcException
 * @author kobayashr
 * @since 0.1 2015/07/27
 * @version 0.2
 */
void vSetObjectiveFunction( CCmdCheck *pcCmd, CAbc *pcAbc )
{
	if( strcmp(pcCmd->pcGetFuncName(), "sphere" ) == 0 )
	{
		pcAbc->vSetConstraintFunction( lfSphere );
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
	else if( strcmp(pcCmd->pcGetFuncName(), "rastrigin" ) == 0 )
	{
		pcAbc->vSetConstraintFunction( lfRastrigin );
	}
	else if( strcmp(pcCmd->pcGetFuncName(), "griewank" ) == 0 )
	{
		pcAbc->vSetConstraintFunction( lfGriewank );
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
	else if( strcmp(pcCmd->pcGetFuncName(), "Branins" ) == 0 )
	{
		pcAbc->vSetConstraintFunction( lfBraninsRCos );
	}
	else if( strcmp(pcCmd->pcGetFuncName(), "Schwefel" ) == 0 )
	{
		pcAbc->vSetConstraintFunction( lfSchwefel );
	}
	else if( strcmp(pcCmd->pcGetFuncName(), "katsuura" ) == 0 )
	{
		pcAbc->vSetConstraintFunction( lfKatsuura );
	}
	else if( strcmp(pcCmd->pcGetFuncName(), "langermann" ) == 0 )
	{
		pcAbc->vSetConstraintFunction( lfLangermann );
	}
	else if( strcmp(pcCmd->pcGetFuncName(), "Shubert" ) == 0 )
	{
		pcAbc->vSetConstraintFunction( lfShubert );
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
		pcAbc->vModified2Abc();
	}
	else if( pcCmd->iGetAbcMethod() == 5 )
	{
		pcAbc->vModified3Abc( iLoc );
	}
	else if( pcCmd->iGetAbcMethod() == 6 )
	{
		pcAbc->vMeAbc( iLoc );
	}
	else if( pcCmd->iGetAbcMethod() == 7 )
	{
		pcAbc->vUndxAbc();
	}
	else
	{
	}
}

/**
 *<PRE>
 *  Artificial Bee Colony�@�����s�������ʂ��o�͂��܂��B
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
	pcAbc->vSetRandom( pcCmd->lfGetRange() );
}
