#include<cmath>
#include<cfloat>
#include"mseq.h"
#include"constdata.h"
#include"ConstraintCondition.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////�������

/**
 * <PRE>
 *   NEDOCS�p�������
 *   �Z�o���q�ɑ΂��ă��~�b�^�[�������܂��B
 *   NEDOCS�̊e�ϐ��͐��ł���K�v�����邽�߁B
 * </PRE>
 * @param plfX			����
 * @param iVectorLen	�����̎�����
 * @author kobayashi
 * @since 2017/03/08
 * @version 0.1
 */
void vNedocsLimitter(double *plfX, int iGenVector)
{
	plfX[0] = plfX[0] <= 0.0 ? 0.0 : plfX[0];
	plfX[0] = plfX[0] >= 1342.0 ? 1342.0 : plfX[0];
	plfX[1] = plfX[1] <= 1.0 ? 1.0 : plfX[1];
	plfX[1] = plfX[1] >= 1342.0 ? 1342.0 : plfX[1];
	plfX[2] = plfX[2] <= 0.0 ? 0.0 : plfX[2];
	plfX[2] = plfX[2] >= 1342.0 ? 1342.0 : plfX[2];
	plfX[3] = plfX[3] <= 1.0 ? 1.0 : plfX[3];
	plfX[3] = plfX[3] >= 1342.0 ? 1342.0 : plfX[3];
	plfX[4] = plfX[4] <= 0.0 ? 0.0 : plfX[4];
	plfX[4] = plfX[4] >= 1342.0 ? 1342.0 : plfX[4];
	plfX[5] = plfX[5] <= 0.0 ? 0.0 : plfX[5];
	plfX[5] = plfX[5] >= 1342.0 ? 1342.0 : plfX[5];
	plfX[6] = plfX[6] <= 0.0 ? 0.0 : plfX[6];
	plfX[6] = plfX[6] >= 1342.0 ? 1342.0 : plfX[6];
}

/**
* <PRE>
*   ������������������܂���B
* </PRE>
* @param plfX		����
* @param iVectorLen	�����̎�����
* @author kobayashi
* @since 2017/03/08
* @version 0.1
*/
void vNone(double *plfX, int iGenVector)
{

}

/**
*<PRE>
*  ���~�b�^�[�ł��B
*</PRE>
* @param lfData	���̓f�[�^
* @param lfMax	����l
* @param lfMin	�����l
* @author kobayashi
* @since 2017/04/03
* @version 0.1
*/
double lfLimitter(double lfData, double lfMax, double lfMin)
{
	double lfRes;
	lfRes = lfMin > lfData ? lfMin : lfData;
	lfRes = lfMax < lfRes ? lfMax : lfRes;
	return lfRes;
}

/**
*<PRE>
*  �Ō�t�����K�����ɂȂ��Ă��邩�ǂ����𔻒肵�āA�K�����ɂȂ��Ă��Ȃ���Α��������܂��B
*</PRE>
* @author kobayashi
* @since 2017/04/03
* @version 0.1
*/
bool bUpdateNurseRelativeRoom(double* plfArg, int iRoomLoc, int iNurseLoc,
	double lfMinRangeRoom, double lfMaxRangeRoom, double lfMinRangeNurse, double lfMaxRangeNurse,
	double lfMinRangeTotalNurse, double lfMaxRangeTotalNurse)
{
	double lfRoom, lfNurse, lfMinRoom, lfMinNurse;
	double lfDistance, lfMinDistance;
	bool bRet = false;

	lfMinRoom = lfMinNurse = 1;
	lfMinDistance = DBL_MAX;

	// �����������ɂ݂Ă����B
	for (lfRoom = lfMinRangeRoom; lfRoom <= lfMaxRangeRoom; lfRoom += 1.0)
	{
		// ��ËZ�t�������ɂ݂Ă����B
		for (lfNurse = lfMinRangeNurse; lfNurse <= lfMaxRangeNurse; lfNurse += 1.0)
		{
			// ��t�����y�ъŌ�t�����������𖞂������ǂ����𔻒肵�܂��B
			if (lfMinRangeTotalNurse <= lfRoom*lfNurse && lfRoom*lfNurse <= lfMaxRangeTotalNurse)
			{
				// �����A�ǂ̒l���K�؂Ȓl���𔻒肵�܂��B�����ړx�ɂ�蔻�肵�܂��B
				// ���݂̒l�ɍł��߂��x�N�g���K�؂Ȓl�Ƃ��Ă��̗p���܂��B�x�N�g���v�Z�Ƃ݂Ȃ��A���[�N���b�h�����ɂ��]�������{�B
				lfDistance = sqrt((lfRoom - plfArg[iRoomLoc])*(lfRoom - plfArg[iRoomLoc]) + (lfNurse - plfArg[iNurseLoc])*(lfNurse - plfArg[iNurseLoc]));
				if (lfDistance <= lfMinDistance)
				{
					// ���̂Ƃ��̕������A��t���A�Ō�t�����擾���܂��B
					lfMinRoom = lfRoom;
					lfMinNurse = lfNurse;
					lfMinDistance = lfDistance;
					bRet = true;
				}
			}
		}
	}
	// �Y���f�[�^�̑g�ݍ��킹�����������ꍇ
	if (bRet == true)
	{
		plfArg[iRoomLoc] = lfMinRoom;
		plfArg[iNurseLoc] = lfMinNurse;
	}
	return bRet;
}

/**
*<PRE>
*  ��ËZ�t�����K�����ɂȂ��Ă��邩�ǂ����𔻒肵�āA�K�����ɂȂ��Ă��Ȃ���Α��������܂��B
*</PRE>
* @author kobayashi
* @since 2017/04/03
* @version 0.1
*/
bool bUpdateClinicalEngineerRelativeRoom(double *plfArg, int iRoomLoc, int iClinicalEngineerLoc,
	double lfMinRangeRoom, double lfMaxRangeRoom, double lfMinRangeClinicalEngineer, double lfMaxRangeClinicalEngineer,
	double lfMinRangeTotalClinicalEngineer, double lfMaxRangeTotalClinicalEngineer)
{
	double lfRoom, lfClinicalEngineer, lfMinRoom, lfMinClinicalEngineer;
	double lfDistance, lfMinDistance;
	bool bRet = false;

	lfMinRoom = lfMinClinicalEngineer = 1;
	lfMinDistance = DBL_MAX;

	// �����������ɂ݂Ă����B
	for (lfRoom = lfMinRangeRoom; lfRoom <= lfMaxRangeRoom; lfRoom += 1.0)
	{
		// ��ËZ�t�������ɂ݂Ă����B
		for (lfClinicalEngineer = lfMinRangeClinicalEngineer; lfClinicalEngineer <= lfMaxRangeClinicalEngineer; lfClinicalEngineer += 1.0)
		{
			// ��t�����y�ъŌ�t�����������𖞂������ǂ����𔻒肵�܂��B
			if (lfMinRangeTotalClinicalEngineer <= lfRoom*lfClinicalEngineer && lfRoom*lfClinicalEngineer <= lfMaxRangeTotalClinicalEngineer)
			{
				// �����A�ǂ̒l���K�؂Ȓl���𔻒肵�܂��B�����ړx�ɂ�蔻�肵�܂��B
				// ���݂̒l�ɍł��߂��x�N�g���K�؂Ȓl�Ƃ��Ă��̗p���܂��B�x�N�g���v�Z�Ƃ݂Ȃ��A���[�N���b�h�����ɂ��]�������{�B
				lfDistance = sqrt((lfRoom - plfArg[iRoomLoc])*(lfRoom - plfArg[iRoomLoc]) + (lfClinicalEngineer - plfArg[iClinicalEngineerLoc])*(lfClinicalEngineer - plfArg[iClinicalEngineerLoc]));
				if (lfDistance <= lfMinDistance)
				{
					// ���̂Ƃ��̕������A��t���A�Ō�t�����擾���܂��B
					lfMinRoom = lfRoom;
					lfMinClinicalEngineer = lfClinicalEngineer;
					lfMinDistance = lfDistance;
					bRet = true;
				}
			}
		}
	}
	// �Y���f�[�^�̑g�ݍ��킹�����������ꍇ
	if (bRet == true)
	{
		plfArg[iRoomLoc] = lfMinRoom;
		plfArg[iClinicalEngineerLoc] = lfMinClinicalEngineer;
	}
	return bRet;
}

/**
 *<PRE>
 *  ���������K�����ɂȂ��Ă��邩�ǂ����𔻒肵�āA�K�����ɂȂ��Ă��Ȃ���Α��������܂��B
 *</PRE>
 * @author kobayashi
 * @since 2017/04/03
 * @version 0.1
 */
bool bUpdateRoomAgent(double *plfArg, int iRoomLoc, int iDoctorLoc, int iNurseLoc,
	double lfMinRangeTotalDoctor, double lfMaxRangeTotalDoctor, double lfMinRangeTotalNurse, double lfMaxRangeTotalNurse,
	double lfMinRangeRoom, double lfMaxRangeRoom, double lfMinRangeDoctor, double lfMaxRangeDoctor, double lfMinRangeNurse, double lfMaxRangeNurse)
{

	double lfRoom, lfDoctor, lfNurse, lfMinRoom, lfMinDoctor, lfMinNurse;
	double lfDistance, lfMinDistance;
	bool bRet = false;

	lfMinRoom = lfMinDoctor = lfMinNurse = 1;
	lfMinDistance = DBL_MAX;

	// �����������ɂ݂Ă����B
	for (lfRoom = lfMinRangeRoom; lfRoom <= lfMaxRangeRoom; lfRoom += 1.0)
	{
		// ��t�������ɂ݂Ă����B
		for (lfDoctor = lfMinRangeDoctor; lfDoctor <= lfMaxRangeDoctor; lfDoctor += 1.0)
		{
			// �Ō�t�������ɂ݂Ă����B
			for (lfNurse = lfMinRangeNurse; lfNurse <= lfMaxRangeNurse; lfNurse += 1.0)
			{
				//					System.out.println(lfMinRangeTotalDoctor + "<=" + lfRoom*lfDoctor + "<=" + lfMaxRangeTotalDoctor );
				//					System.out.println(lfMinRangeTotalNurse + "<=" + lfRoom*lfNurse + "<=" + lfMaxRangeTotalNurse );
				// ��t�����y�ъŌ�t�����������𖞂������ǂ����𔻒肵�܂��B
				if ((lfMinRangeTotalDoctor <= lfRoom*lfDoctor && lfRoom*lfDoctor <= lfMaxRangeTotalDoctor) &&
					(lfMinRangeTotalNurse <= lfRoom*lfNurse && lfRoom*lfNurse <= lfMaxRangeTotalNurse))
				{
					// �����A�ǂ̒l���K�؂Ȓl���𔻒肵�܂��B�����ړx�ɂ�蔻�肵�܂��B
					// ���݂̒l�ɍł��߂��x�N�g���K�؂Ȓl�Ƃ��Ă��̗p���܂��B�x�N�g���v�Z�Ƃ݂Ȃ��A���[�N���b�h�����ɂ��]�������{�B
					lfDistance = sqrt((lfRoom - plfArg[iRoomLoc])*(lfRoom - plfArg[iRoomLoc]) + (lfDoctor - plfArg[iDoctorLoc])*(lfDoctor - plfArg[iDoctorLoc]) + (lfNurse - plfArg[iNurseLoc])*(lfNurse - plfArg[iNurseLoc]));
					if (lfDistance <= lfMinDistance)
					{
						// ���̂Ƃ��̕������A��t���A�Ō�t�����擾���܂��B
						lfMinRoom = lfRoom;
						lfMinDoctor = lfDoctor;
						lfMinNurse = lfNurse;
						lfMinDistance = lfDistance;
						bRet = true;
					}
				}
			}
		}
	}
	// �Y���f�[�^�̑g�ݍ��킹�����������ꍇ
	if (bRet == true)
	{
		plfArg[iRoomLoc] = lfMinRoom;
		plfArg[iDoctorLoc] = lfMinDoctor;
		plfArg[iNurseLoc] = lfMinNurse;
	}
	return bRet;
}

/**
* <PRE>
*   �~�}����p�̈�ʓI�Ȑ�������ݒ�
* </PRE>
* @param plfArg		����
* @param iGenVector	�����̎�����
* @author kobayashi
* @since 2017/04/03
* @version 0.1
*/
void vSetEDCalibrationCondition(double* plfArg, int iGenVector )
{
	// �����̐ݒ�
	double lfMinWeight = 0.9;
	double lfMaxWeight = 1.1;
	double lfMinRangeTotalDoctor = 0.0;
	double lfMaxRangeTotalDoctor = 0.0;
	double lfMinRangeTotalNurse = 0.0;
	double lfMaxRangeTotalNurse = 0.0;
	double lfMinRangeTotalClinicalEngineer = 0.0;
	double lfMaxRangeTotalClinicalEngineer = 0.0;
	double lfMinRangeRoom = 0.0;
	double lfMaxRangeRoom = 0.0;
	double lfMinRangeDoctor = 0.0;
	double lfMaxRangeDoctor = 0.0;
	double lfMinRangeNurse = 0.0;
	double lfMaxRangeNurse = 0.0;
	double lfMinRangeClinicalEngineer = 0.0;
	double lfMaxRangeClinicalEngineer = 0.0;
	double lfCompRange1 = 0.0;
	double lfCompRange2 = 0.0;
	bool bCompRangeFlag1 = true;
	bool bCompRangeFlag2 = true;
	bool bCompRangeFlag3 = true;
	bool bRet = false;

	// �f�@���̐��������ݒ肵�܂��B
	// �f�@���̈�t��
	// �f�@���̊Ō�t��
	lfMinRangeTotalDoctor = lfMinWeight * 12;
	lfMaxRangeTotalDoctor = lfMaxWeight * 12;
	lfMinRangeTotalNurse = lfMinWeight * 30;
	lfMaxRangeTotalNurse = lfMaxWeight * 30;
	lfMinRangeRoom = 1;
	lfMaxRangeRoom = 12;
	lfMaxRangeDoctor = 3;
	lfMinRangeDoctor = 1;
	lfMinRangeNurse = 1;
	lfMaxRangeNurse = 30;

	// �f�@���̕�������ݒ肵�܂��B
	// �������ʂ���ɐݒ�B
	plfArg[0] = lfLimitter(plfArg[0], lfMaxRangeRoom, lfMinRangeRoom);
	// �f�@���̈�t����ݒ肵�܂��B
	// �����I�ɍl����1������1�l���S���ゾ�ƍl������̂ŁB2�l�͑������ς������B
	plfArg[14] = lfLimitter(plfArg[14], lfMaxRangeDoctor, lfMinRangeDoctor);
	// �f�@���̊Ō�t����ݒ肵�܂��B
	// �ő吔��ݒ�ł���悤�ɂ��܂��B
	plfArg[15] = lfLimitter(plfArg[15], lfMaxRangeNurse, lfMinRangeNurse);

	bRet = bUpdateRoomAgent(plfArg, 0, 14, 15, lfMinRangeTotalDoctor, lfMaxRangeTotalDoctor, lfMinRangeTotalNurse, lfMaxRangeTotalNurse,
		lfMinRangeRoom, lfMaxRangeRoom, lfMinRangeDoctor, lfMaxRangeDoctor, lfMinRangeNurse, lfMaxRangeNurse);
//		if( bRet == true ) System.out.println("�f�@��OK");
//		else			   System.out.println("�f�@��NG");

	// ��p���̐��������ݒ肵�܂��B
	// ��p���̈�t����ݒ肵�܂��B
	// ��p���̊Ō�t����ݒ肵�܂��B
	lfMinRangeTotalDoctor = lfMinWeight * 15;
	lfMaxRangeTotalDoctor = lfMaxWeight * 15;
	lfMinRangeTotalNurse = lfMinWeight * 82;
	lfMaxRangeTotalNurse = lfMaxWeight * 82;
	lfMinRangeRoom = 1;
	lfMaxRangeRoom = 15;
	lfMinRangeDoctor = 1;
	lfMaxRangeDoctor = 4;
	lfMinRangeNurse = 1;
	lfMaxRangeNurse = 82;

	// ��p���̕�������ݒ肵�܂��B
	// �������ʂ���ɐݒ�B
	plfArg[1] = lfLimitter(plfArg[1], lfMaxRangeRoom, lfMinRangeRoom);
	// ��p���̈�t����ݒ肵�܂��B
	// �ő吔��ݒ�ł���悤�ɂ��܂��B
	plfArg[16] = lfLimitter(plfArg[16], lfMaxRangeNurse, lfMinRangeNurse);
	// ��p���̊Ō�t����ݒ肵�܂��B
	// �ő吔��ݒ�ł���悤�ɂ��܂��B
	plfArg[17] = lfLimitter(plfArg[17], lfMaxRangeNurse, lfMinRangeNurse);

	bRet = bUpdateRoomAgent(plfArg, 1, 16, 17, lfMinRangeTotalDoctor, lfMaxRangeTotalDoctor, lfMinRangeTotalNurse, lfMaxRangeTotalNurse,
		lfMinRangeRoom, lfMaxRangeRoom, lfMinRangeDoctor, lfMaxRangeDoctor, lfMinRangeNurse, lfMaxRangeNurse);
//		if( bRet == true ) System.out.println("��p��OK");
//		else			   System.out.println("��p��NG");

	// ���Î��̐��������ݒ肵�܂��B
	// ���Î��̈�t��
	// ���Î��̊Ō�t��
	lfMinRangeTotalDoctor = lfMinWeight * 31;
	lfMaxRangeTotalDoctor = lfMaxWeight * 31;
	lfMinRangeTotalNurse = lfMinWeight * 42;
	lfMaxRangeTotalNurse = lfMaxWeight * 42;
	lfMinRangeRoom = 2;
	lfMaxRangeRoom = 14;
	lfMinRangeDoctor = 1;
	lfMaxRangeDoctor = 15;
	lfMinRangeNurse = 1;
	lfMaxRangeNurse = 42;
	//		lfMaxRangeNurse = 4;

	// ���Î��̕�������ݒ肵�܂��B
	// �������ʂ���ɐݒ�B
	plfArg[2] = lfLimitter(plfArg[2], lfMaxRangeRoom, lfMinRangeRoom);
	// ���Î��̈�t����ݒ肵�܂��B
	// �����I�ɍl����1������1�l���S���ゾ�ƍl������̂ŁB2�l�͑������ς������B
	plfArg[18] = lfLimitter(plfArg[18], lfMaxRangeDoctor, lfMinRangeDoctor);
	// �e��p���A�e���Î��̊Ō�t���͍ő��4�l���炢�Ɛݒ肵�܂��B
	// ���������𒲂ׂ�3��قǂ���B��p����1��������̐l�����Z�o���邱�Ƃ��ł����̂ł�����Q�l�ɐݒ�B
	plfArg[19] = lfLimitter(plfArg[19], lfMaxRangeNurse, lfMinRangeNurse);

	bRet = bUpdateRoomAgent(plfArg, 2, 18, 19, lfMinRangeTotalDoctor, lfMaxRangeTotalDoctor, lfMinRangeTotalNurse, lfMaxRangeTotalNurse,
		lfMinRangeRoom, lfMaxRangeRoom, lfMinRangeDoctor, lfMaxRangeDoctor, lfMinRangeNurse, lfMaxRangeNurse);
//		if( bRet == true ) System.out.println("���Î�OK");
//		else			   System.out.println("���Î�NG");

	// ���Î��̈�ËZ�t��
	if (plfArg[20] < 1.0) plfArg[20] = 1.0;
	if (plfArg[20] > 3.0) plfArg[20] = 3.0;

	// �ώ@���̐��������ݒ肵�܂��B
	// �ώ@���̊Ō�t��
	lfMinRangeTotalNurse = lfMinWeight * 25;
	lfMaxRangeTotalNurse = lfMaxWeight * 25;
	lfMinRangeRoom = 0;
	lfMaxRangeRoom = 14;
	lfMinRangeNurse = 1;
	lfMaxRangeNurse = 25;
//	lfMaxRangeNurse = 4;

	// �ώ@���̕�������ݒ肵�܂��B
	// �������ʂ���ɐݒ�B
	plfArg[3] = lfLimitter(plfArg[3], lfMaxRangeRoom, lfMinRangeRoom);
	// �ώ@���ɏ�������Ō�t��2�l�ȉ��Ƃ��܂��B
	// ����������1�Ⴞ���ł͂��邪�A�L�ڂ���Ă���A�Z�o���\�ł������̂ł�����x�[�X�ɐݒ�B
	plfArg[21] = lfLimitter(plfArg[21], lfMaxRangeNurse, lfMinRangeNurse);

	bRet = bUpdateNurseRelativeRoom(plfArg, 3, 21, lfMinRangeRoom, lfMaxRangeRoom, lfMinRangeNurse, lfMaxRangeNurse,
		lfMinRangeTotalNurse, lfMaxRangeTotalNurse);

	if (bRet == true)
	{
		// �ώ@���̕�������0�̏ꍇ�͊Ō�t�����蓖�Ă��Ȃ����Ƃ���0�Ƃ��܂��B
		plfArg[3] = plfArg[21] == 0.0 ? 0.0 : plfArg[3];
	}
//		if( bRet == true ) System.out.println("�ώ@��OK");
//		else			   System.out.println("�ώ@��NG");

	// �d�Ǌώ@���̐��������ݒ肵�܂��B
	// �d�Ǌώ@���̊Ō�t��
	lfMinRangeTotalNurse = lfMinWeight * 25;
	lfMaxRangeTotalNurse = lfMaxWeight * 25;
	lfMinRangeRoom = 0;
	lfMaxRangeRoom = 4;
	lfMinRangeNurse = 1;
	lfMaxRangeNurse = 25;
//	lfMaxRangeNurse = 2;

	// �d�Ǌώ@���̕�������ݒ肵�܂��B
	// �������ʂ���ɐݒ�B
	plfArg[4] = lfLimitter(plfArg[4], lfMaxRangeRoom, lfMinRangeRoom);
	// �d�Ǌώ@���ɏ�������Ō�t��2�l�ȉ��Ƃ��܂��B
	// ����������1�Ⴞ���ł͂��邪�A�L�ڂ���Ă���A�Z�o���\�ł������̂ł�����x�[�X�ɐݒ�B
	plfArg[22] = lfLimitter(plfArg[22], lfMaxRangeNurse, lfMinRangeNurse);

	bRet = bUpdateNurseRelativeRoom(plfArg, 4, 22, lfMinRangeRoom, lfMaxRangeRoom, lfMinRangeNurse, lfMaxRangeNurse,
		lfMinRangeTotalNurse, lfMaxRangeTotalNurse);

	if (bRet == true)
	{
		// �d�Ǌώ@���̕�������0�̏ꍇ�͊Ō�t�����蓖�Ă��Ȃ����Ƃ���0�Ƃ��܂��B
		plfArg[4] = plfArg[22] == 0.0 ? 0.0 : plfArg[4];
	}
//	if( bRet == true ) System.out.println("�d�Ǌώ@��OK");
//	else			   System.out.println("�d�Ǌώ@��NG");

	// ICU�̐��������ݒ肵�܂��B
	// ICU�̈�t��
	// ICU�̊Ō�t��

	lfMinRangeTotalDoctor = lfMinWeight * 22;
	lfMaxRangeTotalDoctor = lfMaxWeight * 22;
	lfMinRangeTotalNurse = lfMinWeight * 152;
	lfMaxRangeTotalNurse = lfMaxWeight * 152;
	lfMinRangeRoom = 4;
	lfMaxRangeRoom = 76;
	lfMinRangeDoctor = 1;
	lfMaxRangeDoctor = 76;
//	lfMaxRangeDoctor = 5;
	lfMinRangeNurse = 1;
	lfMaxRangeNurse = 76;
//	lfMaxRangeNurse = 5;

	// ICU���̕�������ݒ肵�܂��B
	// �������ʂ���ɐݒ�B
	plfArg[5] = lfLimitter(plfArg[5], lfMaxRangeRoom, lfMinRangeRoom);
	// ICU�̈�t����ݒ肵�܂��B
	// �ő吔��ݒ�ł���悤�ɂ��܂��B
	plfArg[23] = lfLimitter(plfArg[23], lfMaxRangeDoctor, lfMinRangeDoctor);
	// ICU��1�l����Ō�t����ݒ肵�܂��B
	// �\�[�X�͈�Ö@�̈�Ó_���ɋL�ڋy�ь�����������ɐݒ�B
	plfArg[24] = lfLimitter(plfArg[24], lfMaxRangeNurse, lfMinRangeNurse);

	bRet = bUpdateRoomAgent(plfArg, 5, 23, 24, lfMinRangeTotalDoctor, lfMaxRangeTotalDoctor, lfMinRangeTotalNurse, lfMaxRangeTotalNurse,
		lfMinRangeRoom, lfMaxRangeRoom, lfMinRangeDoctor, lfMaxRangeDoctor, lfMinRangeNurse, lfMaxRangeNurse);
//	if( bRet == true ) System.out.println("ICU OK");
//	else			   System.out.println("ICU NG");

	// HCU�̐��������ݒ肵�܂��B
	// HCU�̈�t��
	// HCU�̊Ō�t��
	lfMinRangeTotalDoctor = lfMinWeight * 8;
	lfMaxRangeTotalDoctor = lfMaxWeight * 8;
	lfMinRangeTotalNurse = lfMinWeight * 96;
	lfMaxRangeTotalNurse = lfMaxWeight * 96;
	lfMinRangeRoom = 0;
	lfMaxRangeRoom = 24;
	lfMinRangeDoctor = 1;
	lfMaxRangeDoctor = 5;
//	lfMaxRangeDoctor = 5;
	lfMinRangeNurse = 1;
	lfMaxRangeNurse = 96;
//	lfMaxRangeNurse = 5;

	// HCU���̕�������ݒ肵�܂��B
	// �������ʂ���ɐݒ�B
	plfArg[6] = lfLimitter(plfArg[6], lfMaxRangeRoom, lfMinRangeRoom);
	// HCU�̈�t����ݒ肵�܂��B(HCU�����݂��Ă���ꍇ�ɐݒ肵�܂��B)
	// ����������L�ڃf�[�^����ݒ�B
	plfArg[25] = lfLimitter(plfArg[25], lfMaxRangeDoctor, lfMinRangeDoctor);
	// HCU��1�l����Ō�t����ݒ肵�܂��B
	// �\�[�X�͈�Ö@�̈�Ó_���ɋL�ڋy�ь�����������ɐݒ�B
	plfArg[26] = lfLimitter(plfArg[26], lfMaxRangeNurse, lfMinRangeNurse);

	bRet = bUpdateRoomAgent(plfArg, 6, 25, 26, lfMinRangeTotalDoctor, lfMaxRangeTotalDoctor, lfMinRangeTotalNurse, lfMaxRangeTotalNurse,
		lfMinRangeRoom, lfMaxRangeRoom, lfMinRangeDoctor, lfMaxRangeDoctor, lfMinRangeNurse, lfMaxRangeNurse);
	if (bRet == true)
	{
		// HCU�̕�������0�̏ꍇ�͈�t�����蓖�Ă��Ȃ����Ƃ���0�Ƃ��܂��B
		// HCU�̕�������0�̏ꍇ�͊Ō�t�����蓖�Ă��Ȃ����Ƃ���0�Ƃ��܂��B
		plfArg[25] = plfArg[6] == 0.0 ? 0.0 : plfArg[25];
		plfArg[26] = plfArg[6] == 0.0 ? 0.0 : plfArg[26];
	}
//	if( bRet == true ) System.out.println("HCU OK");
//	else			   System.out.println("HCU NG");

	// ��ʕa���͐����Ȃ��Ƃ��܂��B

	// ��ʕa���̈�t����ݒ肵�܂��B1�a���ɑ΂���1�l�Ƃ��܂��B
	// ���ۂɂ�2�l�̌Œ�l�Ƃ��Ă��܂��B
	if (plfArg[27] < 1.0) plfArg[27] = 1.0;
	if (plfArg[27] > 2.0) plfArg[27] = 2.0;

	// ��ʕa���̊Ō�t��
	lfMinRangeTotalNurse = lfMinWeight * 1316;
	lfMaxRangeTotalNurse = lfMaxWeight * 1316;
	lfMinRangeRoom = 1;
	lfMaxRangeRoom = 1296;
	lfMinRangeNurse = 1;
	lfMaxRangeNurse = 1296;
//	lfMaxRangeNurse = 5;

	// ��ʕa���̕�������ݒ肵�܂��B
	// �������ʂ���ɐݒ�B
	plfArg[7] = lfLimitter(plfArg[7], lfMaxRangeRoom, lfMinRangeRoom);
	// ��ʕa���̊Ō�t����ݒ肵�܂��B���@����1(1�a��)�ɑ΂��čő�Ō�t7�l�̐ݒ�Ƃ��܂��B
	// �\�[�X�͈�Ö@�̈�Ó_���ɋL�ځB
	plfArg[28] = lfLimitter(plfArg[28], lfMaxRangeNurse, lfMinRangeNurse);

	bRet = bUpdateNurseRelativeRoom(plfArg, 7, 28, lfMinRangeRoom, lfMaxRangeRoom, lfMinRangeNurse, lfMaxRangeNurse,
		lfMinRangeTotalNurse, lfMaxRangeTotalNurse);
//	if( bRet == true ) System.out.println("��ʕa��OK");
//	else			   System.out.println("��ʕa��NG");

	// �ҍ����̐��������ݒ肵�܂��B
	// �ҍ�����1�����Ƃ��܂��B
	plfArg[8] = 1;
	// �ҍ����̊Ō�t�̐���ݒ肵�܂��B
	// 1�l�ȏ�͂���Ɨ\�z�����̂ŁA���̂悤�ɐݒ肵�܂��B
	// ���@����1(1�a��)�ɑ΂��čő�Ō�t7�l�̐ݒ�Ƃ��܂��B(��Ö@�̈�Ó_������ɐݒ肵�܂����B)
	if (plfArg[29] < 1.0) plfArg[29] = 1.0;
	if (plfArg[29] > 7.0) plfArg[29] = 7.0;

	// X�����̐��������ݒ肵�܂��B
	// �����Ɋւ��Ă����l�ɒ������ʂ���ɐݒ�B
	lfMinRangeTotalClinicalEngineer = lfMinWeight * 22;
	lfMaxRangeTotalClinicalEngineer = lfMaxWeight * 22;
	lfMinRangeRoom = 1;
	lfMaxRangeRoom = 18;
	lfMinRangeClinicalEngineer = 1;
	lfMaxRangeClinicalEngineer = 3;
//	lfMaxRangeClinicalEngineer = 3;

	// X�����̕�������ݒ肵�܂��B
	// �������ʂ���ɐݒ�B
	plfArg[9] = lfLimitter(plfArg[9], lfMaxRangeRoom, lfMinRangeRoom);
	// �e�������ɏ��������ËZ�t����1�l�ȏ�3�l�ȉ��Ƃ��܂��B
	// �\�[�X�͈�Ö@�ɂ͓K�����̂݁B�Ȃ̂ŁA1����������3�l���x�Ƃ��܂��B
	// ���������ɂ��ڂ����l���L�ڂ���Ă��Ȃ����Ƃ���B
	plfArg[30] = lfLimitter(plfArg[30], lfMaxRangeClinicalEngineer, lfMinRangeClinicalEngineer);

	bRet = bUpdateClinicalEngineerRelativeRoom(plfArg, 9, 30,
		lfMinRangeRoom, lfMaxRangeRoom, lfMinRangeClinicalEngineer, lfMaxRangeClinicalEngineer,
		lfMinRangeTotalClinicalEngineer, lfMaxRangeTotalClinicalEngineer);
//	if( bRet == true ) System.out.println("X����OK");
//	else			   System.out.println("X����NG");


	// CT���̐��������ݒ肵�܂��B
	lfMinRangeTotalClinicalEngineer = lfMinWeight * 16;
	lfMaxRangeTotalClinicalEngineer = lfMaxWeight * 16;
	lfMinRangeRoom = 1;
	lfMaxRangeRoom = 4;
	lfMaxRangeRoom = 16;
	lfMinRangeClinicalEngineer = 1;
	lfMaxRangeClinicalEngineer = 4;
	//		lfMaxRangeClinicalEngineer = 4;

	// CT���̕�������ݒ肵�܂��B
	// �������ʂ���ɐݒ�B
	plfArg[10] = lfLimitter(plfArg[10], lfMaxRangeRoom, lfMinRangeRoom);
	// �e�������ɏ��������ËZ�t����1�l�ȏ�3�l�ȉ��Ƃ��܂��B
	// �\�[�X�͈�Ö@�ɂ͓K�����̂݁B�Ȃ̂ŁA1����������3�l���x�Ƃ��܂��B
	// ���������ɂ��ڂ����l���L�ڂ���Ă��Ȃ����Ƃ���B
	plfArg[31] = lfLimitter(plfArg[31], lfMaxRangeClinicalEngineer, lfMinRangeClinicalEngineer);

	bRet = bUpdateClinicalEngineerRelativeRoom(plfArg, 10, 31,
		lfMinRangeRoom, lfMaxRangeRoom, lfMinRangeClinicalEngineer, lfMaxRangeClinicalEngineer,
		lfMinRangeTotalClinicalEngineer, lfMaxRangeTotalClinicalEngineer);

	// MRI���̐��������ݒ肵�܂��B
	lfMinRangeTotalClinicalEngineer = lfMinWeight * 14;
	lfMaxRangeTotalClinicalEngineer = lfMaxWeight * 14;
	lfMinRangeRoom = 1;
	lfMaxRangeRoom = 7;
	lfMinRangeClinicalEngineer = 1;
	lfMaxRangeClinicalEngineer = 3;
	//		lfMaxRangeClinicalEngineer = 3;

	// MRI���̕�������ݒ肵�܂��B
	// �������ʂ���ɐݒ�B
	plfArg[11] = lfLimitter(plfArg[11], lfMaxRangeRoom, lfMinRangeRoom);
	// �e�������ɏ��������ËZ�t����1�l�ȏ�3�l�ȉ��Ƃ��܂��B
	// �\�[�X�͈�Ö@�ɂ͓K�����̂݁B�Ȃ̂ŁA1����������3�l���x�Ƃ��܂��B
	// ���������ɂ��ڂ����l���L�ڂ���Ă��Ȃ����Ƃ���B
	plfArg[32] = lfLimitter(plfArg[32], lfMaxRangeClinicalEngineer, lfMinRangeClinicalEngineer);

	bRet = bUpdateClinicalEngineerRelativeRoom(plfArg, 11, 32,
		lfMinRangeRoom, lfMaxRangeRoom, lfMinRangeClinicalEngineer, lfMaxRangeClinicalEngineer,
		lfMinRangeTotalClinicalEngineer, lfMaxRangeTotalClinicalEngineer);
//	if( bRet == true ) System.out.println("MRI��OK");
//	else			   System.out.println("MRI��NG");

	// ���Ǒ��e���̐��������ݒ肵�܂��B
	lfMinRangeTotalClinicalEngineer = lfMinWeight * 9;
	lfMaxRangeTotalClinicalEngineer = lfMaxWeight * 9;
	lfMinRangeRoom = 1;
//	lfMaxRangeRoom = 3;
	lfMaxRangeRoom = 9;
	lfMinRangeClinicalEngineer = 1;
	lfMaxRangeClinicalEngineer = 3;
//	lfMaxRangeClinicalEngineer = 3;

	// ���Ǒ��e���̕�������ݒ肵�܂��B
	// �������ʂ���ɐݒ�B
	plfArg[12] = lfLimitter(plfArg[12], lfMaxRangeRoom, lfMinRangeRoom);
	// �e�������ɏ��������ËZ�t����1�l�ȏ�3�l�ȉ��Ƃ��܂��B
	// �\�[�X�͈�Ö@�ɂ͓K�����̂݁B�Ȃ̂ŁA1����������3�l���x�Ƃ��܂��B
	// ���������ɂ��ڂ����l���L�ڂ���Ă��Ȃ����Ƃ���B
	plfArg[33] = lfLimitter(plfArg[33], lfMaxRangeClinicalEngineer, lfMinRangeClinicalEngineer);

	bRet = bUpdateClinicalEngineerRelativeRoom(plfArg, 12, 33,
		lfMinRangeRoom, lfMaxRangeRoom, lfMinRangeClinicalEngineer, lfMaxRangeClinicalEngineer,
		lfMinRangeTotalClinicalEngineer, lfMaxRangeTotalClinicalEngineer);
//	if( bRet == true ) System.out.println("���Ǒ��e��OK");
//	else			   System.out.println("���Ǒ��e��NG");

	// FAST���̐��������ݒ肵�܂��B
	lfMinRangeTotalClinicalEngineer = lfMinWeight * 12;
	lfMaxRangeTotalClinicalEngineer = lfMaxWeight * 12;
	lfMinRangeRoom = 1;
//	lfMaxRangeRoom = 4;
	lfMaxRangeRoom = 12;
	lfMinRangeClinicalEngineer = 1;
	lfMaxRangeClinicalEngineer = 3;
//	lfMaxRangeClinicalEngineer = 3;

	// �����g���̕�������ݒ肵�܂��B
	// �������ʂ���ɐݒ�B
	plfArg[13] = lfLimitter(plfArg[13], lfMaxRangeRoom, lfMinRangeRoom);
	// �e�������ɏ��������ËZ�t����1�l�ȏ�3�l�ȉ��Ƃ��܂��B
	// �\�[�X�͈�Ö@�ɂ͓K�����̂݁B�Ȃ̂ŁA1����������3�l���x�Ƃ��܂��B
	// ���������ɂ��ڂ����l���L�ڂ���Ă��Ȃ����Ƃ���B
	plfArg[34] = lfLimitter(plfArg[34], lfMaxRangeClinicalEngineer, lfMinRangeClinicalEngineer);

	bRet = bUpdateClinicalEngineerRelativeRoom(plfArg, 13, 34,
		lfMinRangeRoom, lfMaxRangeRoom, lfMinRangeClinicalEngineer, lfMaxRangeClinicalEngineer,
		lfMinRangeTotalClinicalEngineer, lfMaxRangeTotalClinicalEngineer);
//	if( bRet == true ) System.out.println("FAST��OK");
//	else			   System.out.println("FAST��NG");

	// �������\������G�[�W�F���g���̐ݒ�

	// �N�Ԃŗ��@���銳�҂̐l���̎Z�o���K�v�ł��B
	// �}�g���f�B�J���Z���^�[�̏ꍇ�́E�E�E
	// ����l���a�@�̏ꍇ�́E�E�E19379�l
	// �e�f�@���̈�t���͍ő�ł�2�l���炢�ɐݒ肵�܂��B
	// �Z�o���@�͈�Ö@�ɋL�ڂ̂��̂���Z�o�B
	//		// ���҂̑������擾���܂��B
	//		lfPatientsNum = 19379;
	//		// �K�v��t�����Z�o���܂��B
	//		lfDoctorsNum = lfPatientsNum*0.0625+lfPatientsNum*1.0/16.0;
	//		// �K�v�Ō�t�����Z�o���܂��B
	//		lfNursesNum = lfPatientsNum*1.0/3.0+lfPatientsNum*1.0/30.0;
	//		// �����������Z�o���܂��B
	//		lfTotalRoomNum = plfArg[0]+plfArg[1]+plfArg[2]+plfArg[3]+plfArg[4]+plfArg[5]+plfArg[6]+plfArg[7]+plfArg[8]+plfArg[9]+plfArg[10]+plfArg[11]+plfArg[12]+plfArg[13];
	//
	//		lfOneRoomDoctorNum			 = lfDoctorsNum / lfTotalRoomNum;
	//		lfOneRoomNurseNum			 = lfNursesNum / lfTotalRoomNum;
	//		lfOneRoomClinicalEngineerNum = lfClinicalEngineersNum / lfTotalRoomNum;

}