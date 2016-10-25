#ifndef _ABC_H_
#define _ABC_H_

#include"CRealCodedGa.h"
#include"CUndx.h"
#include"CRex.h"
#include<string>

const int ABC_SUCCESS = 0;
const int ABC_FATAL_ERROR = -101;
const int ABC_MEMORY_ALLOCATE_ERROR = -102;
const int ABC_INVALID_ARGUMENT_ERROR = -103;
const int ABC_INVALID_DATA_FORMAT_ERROR = -104;

class CAbc 
{
public:
	/**
	 * <PRE>
	 * �@�R���X�g���N�^
	 * </PRE>
	 * @author kobayashi
	 * @since 2015/8/4
	 * @version 0.1
	 */
	CAbc();

	/**
	 * <PRE>
	 * �@�l���I�R���j�[�̏����������s���܂��B(�R���X�g���N�^)
	 * </PRE>
	 * @param iGenCount       �v�Z��
	 * @param iGenNum         �R���j�[��
	 * @param iGenVectorDim   �T���_�̎�����
	 * @param iSearchNum  �@  �T���_�̐�
	 * @param iLimitCountData �X�V���Ȃ�������
	 * @throw AbcException
	 * @author kobayashi
	 * @since 2015/8/4
	 * @version 0.1
	 */
	CAbc( int iGenCount, int iGenNum, int iGenVectorDim, int iSearchNum, int iLimitCountData );

	/**
	 * <PRE>
	 * �@�l���I�R���j�[�̏����������s���܂��B(�R���X�g���N�^)
	 * </PRE>
	 * @param iGenCount       �v�Z��
	 * @param iGenNum         �R���j�[��
	 * @param iGenVectorDim   �T���_�̎�����
	 * @param iSearchNum  �@  �T���_�̐�
	 * @param iLimitCountData �X�V���Ȃ�������
	 * @param iIntervalMinNum �Œᔽ����
	 * @param lfDrData        �����󋵔���
	 * @param lfBoundData     �K���x���e���E�l
	 * @param lfAccuracyData  �K���x�]�����x
	 * @throw AbcException
	 * @author kobayashi
	 * @since 2015/7/28
	 * @version 0.1
	 */
	CAbc( int iGenCount, int iGenNum, int iGenVectorDim, int iSearchNum, int iLimitCountData, int iIntervalMinNumData, int iAlphaData, double lfDrData, double lfBoundData, double lfAccuracyData );

	/**
	 * <PRE>
	 * �@�f�X�g���N�^
	 * </PRE>
	 * @author kobayashi
	 * @since 2015/6/6
	 * @version 1.0
	 */
	virtual ~CAbc();

	/**
	 * <PRE>
	 * �@�l���I�R���j�[�̏����������s���܂��B
	 * </PRE>
	 * @param iGenCount           �v�Z��
	 * @param iGenNum             �R���j�[��
	 * @param iGenVectorDim       �T���_�̎�����
	 * @param iSearchNum  �@      �T���_�̐�
	 * @param iLimitCountData     �X�V���Ȃ�������
	 * @param iIntervalMinNumData �Œᔽ����
	 * @param iAlphaData          �T���_��ʐ�
	 * @param lfDrData            �����󋵔���
	 * @param lfBoundData         �K���x���e���E�l
	 * @param lfAccuracyData      �K���x�]�����x
	 * @throw AbcException
	 * @author kobayashi
	 * @since 2015/7/28
	 * @version 0.1
	 */
	void vInitialize( int iGenCount, int iGenNum, int iGenVectorDim, int iSearchNum, int iLimitCountData, int iIntervalMinNumData, int iAlphaData, double lfDrData, double lfBoundData, double lfAccuracyData );

	/**
	 * <PRE>
	 * �@�l���I�R���j�[�̏����������s���܂��B
	 * </PRE>
	 * @param iGenCount       �v�Z��
	 * @param iGenNum         �R���j�[��
	 * @param iGenVectorDim   �T���_�̎�����
	 * @param iSearchNum  �@  �T���_�̐�
	 * @param iLimitCountData �X�V���Ȃ�������
	 * @throw AbcException
	 * @author kobayashi
	 * @since 2015/7/28
	 * @version 0.1
	 */
	void vInitialize( int iGenCount, int iGenNum, int iGenVectorDim, int iSearchNum, int iLimitCountData );

	/**
 	* <PRE>
 	* �@�l���I�R���j�[�̏����������s���܂��B
 	* </PRE>
 	* @param iGenCount      	 �v�Z��
 	* @param iGenNum        	 �R���j�[��
 	* @param iGenVectorDim  	 �T���_�̎�����
 	* @param iSearchNum  �@ 	 �T���_�̐�
 	* @param iLimitCountData	 �X�V���Ȃ�������
 	* @param iCrossOverNumData	 ������
 	* @param lfAlpha		 UNDX�ɂ�����Alpha
 	* @param lfBeta			 UNDX�ɂ�����Beta
 	* @throw AbcException
 	* @author kobayashi
 	* @since 2015/8/12
 	* @version 0.1
 	*/
	void vInitialize( int iGenCount, int iGenNum, int iGenVectorDim, int iSearchNum, int iLimitCountData, int iCrossOverNumData, double lfAlpha, double lfBeta );

	/**
	 * <PRE>
	 * �@�l���I�R���j�[�̏����������s���܂��B
	 * </PRE>
	 * @param iGenCount       �v�Z��
	 * @param iGenNum         �R���j�[��
	 * @param iGenVectorDim   �T���_�̎�����
	 * @param iSearchNum  �@  �T���_�̐�
	 * @param iLimitCountData �X�V���Ȃ�������
	 * @param iIntervalMinNum �Œᔽ����
	 * @param iAlphaData      �T���_��ʐ�
	 * @param lfDrData        �����󋵔���
	 * @param lfBoundData     �K���x���e���E�l
	 * @param lfAccuracyData  �K���x�]�����x
	 * @param iCrossOverNumData	 ������
	 * @param lfAlpha		 UNDX�ɂ�����Alpha
	 * @param lfBeta		 UNDX�ɂ�����Beta
	 * @throw AbcException
	 * @author kobayashi
	 * @since 2015/8/19
	 * @version 0.1
	 */
	void vInitialize( int iGenCount, int iGenNum, int iGenVectorDim, int iSearchNum, int iLimitCountData, int iIntervalMinNumData, int iAlphaData, double lfDrData, double lfBoundData, double lfAccuracyData, int iCrossOverNumData, double lfAlphaData, double lfBetaData  );

	/**
	* <PRE>
	* �@�l���I�R���j�[�̏����������s���܂��B
	* </PRE>
	* @param iGenCount       �v�Z��
	* @param iGenNum         �R���j�[��
	* @param iGenVectorDim   �T���_�̎�����
	* @param iSearchNum  �@  �T���_�̐�
	* @param iLimitCountData �X�V���Ȃ�������
	* @param iIntervalMinNum �Œᔽ����
	* @param iAlphaData      �T���_��ʐ�
	* @param lfDrData        �����󋵔���
	* @param lfBoundData     �K���x���e���E�l
	* @param lfAccuracyData  �K���x�]�����x
	* @param iParentNumberData			 ������
	* @param iChildrenNumber			 REX�ɂ�����Alpha
	* @param iUpperEvalChildrenNumber	 UNDX�ɂ�����Beta
	* @param lfLearningRate	 UNDX�ɂ�����Beta
	* @throw AbcException
	* @author kobayashi
	* @since 2016/8/26
	* @version 0.1
	*/
	void vInitialize( int iGenCount, int iGenNum, int iGenVectorDim, int iSearchNum, int iLimitCountData, int iIntervalMinNumData, int iAlphaData, double lfDrData, double lfBoundData, double lfAccuracyData, int iParentNum, int iChildrenNum, int iUpperEvalChildrenNum, double lfLearningRateData  );

	/**
	 * <PRE>
	 * �@�����������ړI�֐��̓o�^���s���܂��B
	 * </PRE>
	 * @param pflfObjectiveFunction �R�[���o�b�N�֐�
	 * @author kobayashi
	 * @since 2015/6/8
	 * @version 1.0
	 */
	void vSetConstraintFunction( double (*pflfObjectiveFunction)( double *plfData, int iVectorLen ) );

	/**
	 * <PRE>
	 * �@�o�^�����R�[���o�b�N�֐��̉�@���������s���܂��B
	 * </PRE>
	 * @author kobayashi
	 * @since 2015/6/8
	 * @version 1.0
	 */
	void vReleaseCallConstraintFunction();

	/**
	 * <PRE>
	 * �@���q����l�����ŏ��������܂��B
	 * </PRE>
	 * @param lfRange ���q�̏����ʒu�̏o���͈�
	 * @author kobayashi
	 * @since 2015/6/8
	 * @version 1.0
	 */
	void vSetRandom( double lfRange );

	/**
	 * <PRE>
	 * �@���q����l�����ŏ��������܂��B(ModifiedAbc�@�p)
	 * </PRE>
	 * @param lfRange ���q�̏����ʒu�̏o���͈�
	 * @author kobayashi
	 * @since 2015/6/8
	 * @version 1.0
	 */
	void vSetModifiedRandom( double lfRange );

	/**
	 * <PRE>
	 * �@�I�����������s���܂��B
	 * </PRE>
	 * @author kobayashi
	 * @since 2015/8/3
	 * @version 1.0
	 */
	void vTerminate();

	/**
	 * <PRE>
	 * �@ABC�@�����s���܂��B
	 * </PRE>
	 * @author kobayashi
	 * @since 2015/6/17
	 * @version 0.1
	 */
	void vAbc();

	/**
	 * <PRE>
	 * �@ABC�@���P��@�����s���܂��B
	 * </PRE>
	 * @author kobayashi
	 * @since 2015/8/3
	 * @version 0.1
	 */
	void vModifiedAbc( int iUpdateCount );

	/**
	 * <PRE>
	 * �@�l�H�I�R���j�[�œK���@�����s���܂��B
	 *	 GBest�𗘗p����ABC�@
	 *   Gbest-guided artificial bee colony algorithm for numerical function optimization.
	 *   Applied Mathematics and Computation, 217(7):3166-3173,2010.
	 * </PRE>
	 * @author kobayashi
	 * @since 2016/7/10
	 * @version 0.1
	 */
	void vGAbc();

	/**
	 * <PRE>
	 * �@�l�H�I�R���j�[�œK���@�����s���܂��B
	 *   ���q�Q�œK���@��IWCFA��K�p
	 * </PRE>
	 * @author kobayashi
	 * @since 2015/8/3
	 * @version 0.1
	 */
	void vIWCFAAbc( int iUpdateCount );

	/**
	 * <PRE>
	 * �@2013 Memetic search in artificial bee colony algorthim���
	 * </PRE>
	 * @author kobayashi
	 * @since 2015/7/28
	 * @version 0.1
	 */
	void vMeAbc( int iUpdateCount );

	/**
	 * <PRE>
	 * �@2014 Randomized Memetic Artificial Bee Colony Algorthim���
	 *   International Journal of Emerging Trends of Technology in Computer Science, vol.3(1), 2014
	 * </PRE>
	 * @author kobayashi
	 * @since 2015/7/28
	 * @version 0.1
	 */
	void vRMAbc( int iUpdateCount );

	/**
	 * <PRE>
	 * �@�l�H�I�R���j�[�œK���@�i�����𓱓�������@�j�����s���܂��B
	 *   A Novel Hybrid Crossover based Artificial Bee Colony Algorithm for Optimization Problem International Journal of Computer Applications 2013���
	 * </PRE>
	 * @author kobayashi
	 * @since 2016/4/11
	 * @version 0.1
	 */
	void vCbAbc();

	/**
	 * <PRE>
	 * �@�l�H�I�R���j�[�œK���@�i�����𓱓�������@�j�����s���܂��B
	 *   ���S����A�����W
	 * </PRE>
	 * @author kobayashi
	 * @since 2016/8/12
	 * @version 0.1
	 */
	void vUndxAbc();
	
	/**
	 * <PRE>
	 * �@�l�H�I�R���j�[�œK���@�i�����𓱓�������@�j�����s���܂��B
	 *   ���S����A�����W
	 *   2011�̍����\���𓱓�
	 * </PRE>
	 * @author kobayashi
	 * @since 2016/8/19
	 * @version 0.1
	 */
	void vUndxEnhancedAbc( int iUpdateCount );

	/**
	 * <PRE>
	 * �@�l�H�I�R���j�[�œK���@�i�����𓱓�������@�j�����s���܂��B
	 *   ���S����A�����W�iEmsanble Real Coded CrossOver��p���܂��B�j
	 * </PRE>
	 * @author kobayashi
	 * @since 2016/8/26
	 * @version 0.1
	 */
	void vRexAbc();

	/**
	 * <PRE>
	 * �@�l�H�I�R���j�[�œK���@�i�����𓱓�������@�j�����s���܂��B
	 *   ���S����A�����W�iAdaptation Emsanble Real Coded CrossOver��p���܂��B�j
	 * </PRE>
	 * @author kobayashi
	 * @since 2016/8/27
	 * @version 0.1
	 */
	void vARexAbc();

	/**
	* <PRE>
	* �@�l�H�I�R���j�[�œK���@�����s���܂��B
	*   HJABC�@��K�p���܂��B
	*   ver 0.1 ����
	*   ver 0.2 �A���S���Y���������ł��Ă��Ȃ������̂Ŏ������B
	* </PRE>
	* @param iUpdateCount
	* @author kobayashi
	* @since 2016/10/3
	* @version 0.1
	*/
	void vHJAbc(int iUpdateCount);

	/**
	* <PRE>
	* �@�l�H�I�R���j�[�œK���@�����s���܂��B
	*   ACABC�@��K�p���܂��B
	*   ver 0.1 ����
	* </PRE>
	* @author kobayashi
	* @since 2016/10/3
	* @version 0.1
	*/
	void vACAbc();

	/**
	* <PRE>
	* �@������@�𓱓������l�H�I�R���j�[�œK���@�����s���܂��B
	*   Ivona Brajevic, Crossover-based artificial bee colony algorithm for constrained optimization problems, Neural Computing & Application (2015) 26:1587-1601.
	*   ver 0.1 ����
	* </PRE>
	* @param iCount ���݂̌v�Z��
	* @author kobayashi
	* @since 2016/10/19
	* @version 0.1
	*/
	void vCBAbc(int iUpdateCount );

	/**
	  *<PRE>
	  * �R�[���o�b�N�֐��̒�`�ł��B
	  *</PRE>
	  */
	double (*pflfObjectiveFunction)( double *pplfData, int iVectorLen );

	/**
	 * <PRE>
	 * �@���q�Q�œK���@��K�p�������ʂ��o�͂��܂��B(�e���q�̈ʒu)
	 * </PRE>
	 * @author kobayashi
	 * @since 2015/6/6
	 * @version 1.0
	 */
	void vOutputAbcData();

	/**
	 * <PRE>
	 * �@���q�Q�œK���@��K�p�������ʂ��o�͂��܂��B(�e���q�̑��x)
	 * </PRE>
	 * @author kobayashi
	 * @since 2015/6/6
	 * @version 1.0
	 */
	void vOutputVelocityData();

	/**
	 * <PRE>
	 * �@���q�Q�œK���@��K�p�������ʂ��o�͂��܂��B(�e���q�̖ړI�֐��l)
	 * </PRE>
	 * @author kobayashi
	 * @since 2015/6/6
	 * @version 1.0
	 */
	void vOutputConstraintFunction();

	/**
	 * <PRE>
	 * �@�����_�ł̍ő�l���q�̈ʒu���o�͂��܂��B
	 * </PRE>
	 * @author kobayashi
	 * @since 2015/6/19
	 * @version 1.0
	 */
	void vOutputGlobalMaxAbcData();

	/**
	 * <PRE>
	 * �@�����_�ł̍ő�l���q�̖ړI�֐��l���o�͂��܂��B
	 * </PRE>
	 * @author kobayashi
	 * @since 2015/6/19
	 * @version 1.0
	 */
	void vOutputGlobalMaxAbcDataConstFuncValue();

	/**
	 * <PRE>
	 * �@�����_�ł̍ŏ��l���q�̈ʒu���o�͂��܂��B
	 * </PRE>
	 * @author kobayashi
	 * @since 2015/6/19
	 * @version 1.0
	 */
	void vOutputGlobalMinAbcData();

	/**
	 * <PRE>
	 * �@�����_�ł̍ŏ��l���q�̖ړI�֐��l���o�͂��܂��B
	 * </PRE>
	 * @author kobayashi
	 * @since 2015/6/19
	 * @version 1.0
	 */
	void vOutputGlobalMinAbcDataConstFuncValue();

	/**
	 * <PRE>
	 * �@�����_�ł̂����Ƃ��悢���q�ʒu�Ƃ��̑��̗��q�Ƃ̋������o�͂��܂��B
	 * </PRE>
	 * @param iOutFlag �o�͕��@���w�肷��t���O
	 * @author kobayashi
	 * @since 2015/6/19
	 * @version 1.0
	 */
	void vOutputAbcDataLocDist( int iOutFlag );
	
	/**
	 * <PRE>
	 * �@�����_�ł̊e���q���Ƃ̍ŗǈʒu���o�͂��܂��B
	 * </PRE>
	 * @param iFlag 0 �ړI�֐��l���o�͂��Ȃ��B
	 * �@�@�@�@�@�@ 1 �ړI�֐��l���o�͂���B
	 * @author kobayashi
	 * @since 2015/7/6
	 * @version 0.2
	 */
	void vOutputLocalMaxAbcData( int iOutFlag );

	void vSetRange( double lfRange );
	double lfGetGlobalMinAbcDataConstFuncValue();
private:
	/**
	 * <PRE>
	 * �@Employ Bee�����s���܂��B
	 * </PRE>
	 * @author kobayashi
	 * @since 2016/8/18
	 * @version 0.1
	 */
	void vEmployBeeOrigin();
	
	/**
	 * <PRE>
	 * �@Employ Bee�����s���܂��B(�����x���o�[�W����)
	 *   2011�̓d�q���ʐM�w��̘_�����
	 *   ver 0.1 
	 * </PRE>
	 * @author kobayashi
	 * @since 2016/8/18
	 * @version 0.1
	 */
	double lfEmployBeeEnhanced( int iUpdateCount );

	/**
	 * <PRE>
	 * �@Employ Bee�����s���܂��B(�����x���o�[�W����)
	 *   2011�̓d�q���ʐM�w��̘_�����
	 *   ver 0.1 
	 * </PRE>
	 * @author kobayashi
	 * @since 2016/8/18
	 * @version 0.1
	 */
	double lfEmployBeeBestEnhanced( int iUpdateCount );

	/**
	 * <PRE>
	 * �@Employ Bee�����s���܂��B
	 *   ver 0.1 
	 *   ver 0.2 NBest�łɏC��
	 * </PRE>
	 * @author kobayashi
	 * @since 2016/8/10
	 * @version 0.2
	 */
	void vEmployBeeBest();
	
	/**
	 * <PRE>
	 * �@Employ Bee�����s���܂��B(GBest��)
	 *   ver 0.1 
	 *   ver 0.2 GBest�łɏC��
	 * </PRE>
	 * @author kobayashi
	 * @since 2016/8/10
	 * @version 0.2
	 */
	void vEmployBeeGBest();

	/**
	 * <PRE>
	 * �@Employ Bee�����s���܂��B
	 *   ver 0.1 
	 *   ver 0.2 2016/08/18 IWCFA(���q�Q�œK���@�̈��@��K�p)
	 * </PRE>
	 * @param lfK
	 * @param lfCoe1
	 * @param lfCoe2
	 * @param iUpdateCount
	 * @author kobayashi
	 * @since 2016/8/10
	 * @version 0.2
	 */
	void vEmployBeeIWCFA( double lfK, double lfCoe1, double lfCoe2, int iUpdateCount );

	/**
	* <PRE>
	*   Ivona Brajevic, Crossover-based artificial bee colony algorithm for constrained optimization problems, Neural Computing & Application (2015) 26:1587-1601.
	*   ver 0.1
	* </PRE>
	* @param lfMr �X�V�p�p�����[�^�[
	* @author kobayashi
	* @since 2016/10/19
	* @version 0.1
	*/
	void vEmployBeeCB(double lfMr);

	/**
	 * <PRE>
	 * �@Onlooker Bee�����s���܂��B
	 * </PRE>
	 * @author kobayashi
	 * @since 2016/8/10
	 * @version 0.1
	 */
	void vOnlookerBeeOrigin();
	
	/**
	 * <PRE>
	 * �@Onlooker Bee�����s���܂��B(�����x���o�[�W����)
	 *   2011�̓d�q���ʐM�w��̘_�����
	 *   ver 0.1 
	 * </PRE>
	 * @author kobayashi
	 * @since 2016/8/18
	 * @version 0.1
	 */
	void vOnlookerBeeEnhanced( int iUpdateCount, double lfFitJudge );

	/**
	 * <PRE>
	 * �@Onlooker Bee�����s���܂��B(�����x���o�[�W����)
	 *   2011�̓d�q���ʐM�w��̘_�����
	 *   ver 0.1 
	 * </PRE>
	 * @author kobayashi
	 * @since 2016/8/18
	 * @version 0.1
	 */
	void vOnlookerBeeBestEnhanced( int iUpdateCount, double lfFitJudge );

	/**
	 * <PRE>
	 * Onlooker Bee�����s���܂��B(NBest��)
	 * ver 0.1
	 * ver 0.2 NBest�ł�ǉ�
	 * </PRE>
	 * @author kobayashi
	 * @since 2016/8/10
	 * @version 0.2
	 */
	void vOnlookerBeeBest();
	
	/**
	 * <PRE>
	 * Onlooker Bee�����s���܂��B(GBest��)
	 * ver 0.1
	 * ver 0.2 NBest�ł�ǉ�
	 * </PRE>
	 * @author kobayashi
	 * @since 2016/8/10
	 * @version 0.2
	 */
	void vOnlookerBeeGBest();

	/**
	 * <PRE>
	 * Onlooker Bee�����s���܂��B(IWCFA��)
	 * ver 0.1
	 * ver 0.2 IWCFA�ł�ǉ�
	 * </PRE>
	 * @param lfK
	 * @param lfCoe1
	 * @param lfCoe2
	 * @param iUpdateCount
	 * @author kobayashi
	 * @since 2016/8/10
	 * @version 0.2
	 */
	void vOnlookerBeeIWCFA( double lfK, double lfCoe1, double lfCoe2, int iUpdateCount );

	/**
	 * <PRE>
	 * Onlooker Bee�����s���܂��B
	 * Randomized Memtic Bee Colony Method�p
	 * </PRE>
	 * @author kobayashi
	 * @since 2016/9/22
	 * @version 0.1
	 */
	void vOnlookerBeeRM();

	/**
	 * <PRE>
	 * Onlooker Bee�����s���܂��B(Hooke-Jeeves�@�p)
	 * ver 0.1 ����
	 * </PRE>
	 * @author kobayashi
	 * @since 2016/10/3
	 * @version 0.1
	 */
	void vOnlookerBeeHJ();

	/**
	 * <PRE>
	 * Scout Bee�����s���܂��B(��{�̎�@)
	 * ver 0.1
	 * </PRE>
	 * @author kobayashi
	 * @since 2016/8/10
	 * @version 0.2
	 */
	void vScoutBeeOrigin();

	/**
	* <PRE>
	*   Ivona Brajevic, Crossover-based artificial bee colony algorithm for constrained optimization problems, Neural Computing & Application (2015) 26:1587-1601.
	*   ver 0.1
	* </PRE>
	* @author kobayashi
	* @since 2016/10/19
	* @version 0.1
	*/
	void vOnlookerBeeCB(double lfMr);
		
	/**
	 * <PRE>
	 * Scout Bee�����s���܂��B
	 * ver 0.1
	 * ver 0.2 ��@�̕ύX�B�i���q�Q�œK���@�̂悤�ȍX�V��@�B�_�����j
	 * </PRE>
	 * @author kobayashi
	 * @since 2016/8/10
	 * @version 0.2
	 */
	void vScoutBeeNormal();

	/**
	 * <PRE>
	 * Scout Bee�����s���܂��B�i���S�A�����W��UNDX�����s����B�j
	 * ver 0.1
	 * </PRE>
	 * @author kobayashi
	 * @since 2016/8/18
	 * @version 0.1
	 */
	void vScoutBeeUndx();
	
	/**
	 * <PRE>
	 * Scout Bee�����s���܂��B�i���S�A�����W��REX�����s����B�j
	 * ver 0.1
	 * </PRE>
	 * @author kobayashi
	 * @since 2016/8/26
	 * @version 0.1
	 */
	void vScoutBeeRex();

	/**
	 * <PRE>
	 * Scout Bee�����s���܂��B�i���S�A�����W��AREX�����s����B�j
	 * ver 0.1
	 * </PRE>
	 * @author kobayashi
	 * @since 2016/8/27
	 * @version 0.1
	 */
	void vScoutBeeARex();

	/**
	* <PRE>
	* Onlooker Bee�����s���܂��B(AC-ABC�p�Z�p�����𗘗p����ABC�@�B)
	* ver 0.1 ����
	* </PRE>
	* @author kobayashi
	* @since 2016/10/13
	* @version 0.1
	*/
	void vOnlookerBeeAC();
		
	/**
	* <PRE>
	*   Ivona Brajevic, Crossover-based artificial bee colony algorithm for constrained optimization problems, Neural Computing & Application (2015) 26:1587-1601.
	*   ver 0.1
	* </PRE>
	* @author kobayashi
	* @since 2016/10/19
	* @version 0.1
	*/
	void vScoutBeeCB(int iCount);
		
	/**
	 * <PRE>
	 * �@�Ǐ��ő�l�A�ŏ��l���擾���܂��B
	 * </PRE>
	 * @author kobayashi
	 * @since 2016/8/10
	 * @version 0.1
	 */
	void vGetLocalMaxMin();

	/**
	 * <PRE>
	 * �@���I�ő�l�A�ŏ��l���擾���܂��B
	 * </PRE>
	 * @author kobayashi
	 * @since 2016/8/10
	 * @version 0.1
	 */
	void vGetGlobalMaxMin();

	void swap(double *swap1,double *swap2);
	void qsort(int start,int end, double *sort );
	double lfNormalRand( double lfSigma, double lfMean );

	bool bHJEmStep(double *plfX1, double *plfX0, std::vector<double> stlStepSize);
	void vModifiedHookeJeevesMethod(std::vector<double> stlStepSize, double *plfX1, double *plfX2, double *plfX0);

private:
	int iGenerationNumber;				// �v�Z��
	int iAbcDataNum;					// �R���j�[�̐�
	int iAbcVectorDimNum;				// �I�̓����x�N�g��
	int iAbcSearchNum;					// �T���_��
	int iAbcLimitCount;					// �X�V���Ȃ�������
	int iAbcIntervalMinNum;				// �Œᔽ����
	int iAbcUpperSearchNum;				// �T���_��ʐ�
	double lfConvergenceParam;			// �����󋵃p�����[�^
	double lfFitBound;					// �K���x���e���E�l
	double lfFitAccuracy;				// �K���x�]�����x
	int *piLocalMaxAbcLoc;				// 
	double **pplfAbcData;				// ABC�f�[�^�z��
	double **pplfNAbcData;				// ABC�X�V�p�f�[�^�z��
	double lfGlobalMaxAbcData;			// ���I�œK�l
	double lfGlobalMinAbcData;			// ���I�ŏ��l
	double *plfGlobalMaxAbcData;		// ���I�œK����\�����q�̃f�[�^
	double *plfGlobalMinAbcData;		// ���I�œK����\�����q�̃f�[�^
	double *plfLocalMaxAbcData;			// �Ǐ��œK����\�����q�̃f�[�^
	double lfLocalMaxAbcData;			// �Ǐ��œK�l
	double **pplfLocalMaxAbcData;		// �Ǐ��œK����\�����q���Ƃ̍ő�l���i�[����f�[�^
	double **pplfLocalMinAbcData;		// �Ǐ��œK����\�����q���Ƃ̍ŏ��l���i�[����f�[�^
	double *plfLocalMaxObjectiveAbcData;// �Ǐ��œK����\�����q�̃f�[�^
	double **pplfVelocityData;			// ABC�f�[�^���x�z��
	double **pplfNVelocityData;			// ABC�X�V�p�f�[�^���x�z��
	double *plfFitProb;					// �K���x���Ίm��
	double *plfFit;						// �K���x
	int *piNonUpdateCount;				// �X�V���Ȃ���
	int *piTotalNonUpdateCount;			// �X�V���Ȃ�����
	double *plfVelocity;				// ���[���b�g�I���ɂ��I�����鑬�x�x�N�g��
	double *plfCrossOverData;			// �����ꎞ�i�[�p�z��
	double lfSolveRange;				// �o�͔͈�
	double lfFitInit;					// ���ϕ]���l
	double lfFitCurrentBest;			// ���݂̍ŗǒl�̓K���x
	double *plfXnew1;					// Memetic Algorithm�p�X�V�z��
	double *plfXnew2;					// Memetic Algorithm�p�X�V�z��
	CUndx *pcUndx;						// UNDX�p�̃C���X�^���X
	int iCrossOverNum;					// ������
	double lfAlpha;						// UNDX�p���e����p��
	double lfBeta;						// UNDX�p���e����p��
	CRex *pcRex;						// REX�p�̃C���X�^���X
	int iParentNumber;					// Rex�p�I������e�̐�
	int iChildrenNumber;				// Rex�p��������q���̐�
	int iUpperEvalChildrenNumber;		// ARex�p���������q���̏�ʂ�I�����鐔
	double lfLearningRate;				// ARex�p�w�K��
	std::vector<Rank_t> stlFitProb;
	int iReCounter;						// HJABC�p�J�E���^�[
	int iHJInterval;					// HJABC�pHookeJeeves�@�K�p�Ԋu
	int iHJCounter;						// HJABC�pHookeJeeves�@�J��Ԃ���
	double *plfX0;						// HJABC�p�X�V�p�ꎞ�i�[�z��
	double *plfX1;						// HJABC�p�X�V�p�ꎞ�i�[�z��
	double *plfX2;						// HJABC�p�X�V�p�ꎞ�i�[�z��
};

class CAbcException
{
public:
	CAbcException();
	CAbcException( int iCode, std::string sMethodName, std::string sClassName, std::string sDetail );
	CAbcException( int iCode, std::string sMethodName, std::string sClassName, std::string sDetail, int iErrorLine );
	~CAbcException();
	void SetErrorInfo( int iCode, std::string sMethodName, std::string sClassName, std::string sDetail );
	void SetErrorInfo( int iCode, std::string sMethodName, std::string sClassName, std::string sDetail, int iErrorLine );
	int iGetErrCode();
	int iGetErrLine();
	std::string strGetMethodName();
	std::string strGetClassName();
	std::string strGetErrDetail();
private:
	int iErrCode;
	int iErrLine;
	std::string strMethodName;
	std::string strClassName;
	std::string strModuleName;
	std::string strErrDetail;
};

#endif /* _ABC_H_ */
