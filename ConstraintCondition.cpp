#include<cmath>
#include<cfloat>
#include"mseq.h"
#include"constdata.h"
#include"ConstraintCondition.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////制約条件

/**
 * <PRE>
 *   NEDOCS用制約条件
 *   算出粒子に対してリミッターをかけます。
 *   NEDOCSの各変数は正である必要があるため。
 * </PRE>
 * @param plfX			引数
 * @param iVectorLen	引数の次元数
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
*   制約条件を何もかけません。
* </PRE>
* @param plfX		引数
* @param iVectorLen	引数の次元数
* @author kobayashi
* @since 2017/03/08
* @version 0.1
*/
void vNone(double *plfX, int iGenVector)
{

}

/**
*<PRE>
*  リミッターです。
*</PRE>
* @param lfData	入力データ
* @param lfMax	上限値
* @param lfMin	下限値
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
*  看護師数が適正数になっているかどうかを判定して、適正数になっていなければ増減させます。
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

	// 部屋数を順にみていく。
	for (lfRoom = lfMinRangeRoom; lfRoom <= lfMaxRangeRoom; lfRoom += 1.0)
	{
		// 医療技師数を順にみていく。
		for (lfNurse = lfMinRangeNurse; lfNurse <= lfMaxRangeNurse; lfNurse += 1.0)
		{
			// 医師総数及び看護師総数が条件を満たすかどうかを判定します。
			if (lfMinRangeTotalNurse <= lfRoom*lfNurse && lfRoom*lfNurse <= lfMaxRangeTotalNurse)
			{
				// 判定後、どの値が適切な値かを判定します。距離尺度により判定します。
				// 現在の値に最も近いベクトル適切な値としてを採用します。ベクトル計算とみなし、ユークリッド距離により評価を実施。
				lfDistance = sqrt((lfRoom - plfArg[iRoomLoc])*(lfRoom - plfArg[iRoomLoc]) + (lfNurse - plfArg[iNurseLoc])*(lfNurse - plfArg[iNurseLoc]));
				if (lfDistance <= lfMinDistance)
				{
					// そのときの部屋数、医師数、看護師数を取得します。
					lfMinRoom = lfRoom;
					lfMinNurse = lfNurse;
					lfMinDistance = lfDistance;
					bRet = true;
				}
			}
		}
	}
	// 該当データの組み合わせが見つかった場合
	if (bRet == true)
	{
		plfArg[iRoomLoc] = lfMinRoom;
		plfArg[iNurseLoc] = lfMinNurse;
	}
	return bRet;
}

/**
*<PRE>
*  医療技師数が適正数になっているかどうかを判定して、適正数になっていなければ増減させます。
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

	// 部屋数を順にみていく。
	for (lfRoom = lfMinRangeRoom; lfRoom <= lfMaxRangeRoom; lfRoom += 1.0)
	{
		// 医療技師数を順にみていく。
		for (lfClinicalEngineer = lfMinRangeClinicalEngineer; lfClinicalEngineer <= lfMaxRangeClinicalEngineer; lfClinicalEngineer += 1.0)
		{
			// 医師総数及び看護師総数が条件を満たすかどうかを判定します。
			if (lfMinRangeTotalClinicalEngineer <= lfRoom*lfClinicalEngineer && lfRoom*lfClinicalEngineer <= lfMaxRangeTotalClinicalEngineer)
			{
				// 判定後、どの値が適切な値かを判定します。距離尺度により判定します。
				// 現在の値に最も近いベクトル適切な値としてを採用します。ベクトル計算とみなし、ユークリッド距離により評価を実施。
				lfDistance = sqrt((lfRoom - plfArg[iRoomLoc])*(lfRoom - plfArg[iRoomLoc]) + (lfClinicalEngineer - plfArg[iClinicalEngineerLoc])*(lfClinicalEngineer - plfArg[iClinicalEngineerLoc]));
				if (lfDistance <= lfMinDistance)
				{
					// そのときの部屋数、医師数、看護師数を取得します。
					lfMinRoom = lfRoom;
					lfMinClinicalEngineer = lfClinicalEngineer;
					lfMinDistance = lfDistance;
					bRet = true;
				}
			}
		}
	}
	// 該当データの組み合わせが見つかった場合
	if (bRet == true)
	{
		plfArg[iRoomLoc] = lfMinRoom;
		plfArg[iClinicalEngineerLoc] = lfMinClinicalEngineer;
	}
	return bRet;
}

/**
 *<PRE>
 *  部屋数が適正数になっているかどうかを判定して、適正数になっていなければ増減させます。
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

	// 部屋数を順にみていく。
	for (lfRoom = lfMinRangeRoom; lfRoom <= lfMaxRangeRoom; lfRoom += 1.0)
	{
		// 医師数を順にみていく。
		for (lfDoctor = lfMinRangeDoctor; lfDoctor <= lfMaxRangeDoctor; lfDoctor += 1.0)
		{
			// 看護師数を順にみていく。
			for (lfNurse = lfMinRangeNurse; lfNurse <= lfMaxRangeNurse; lfNurse += 1.0)
			{
				//					System.out.println(lfMinRangeTotalDoctor + "<=" + lfRoom*lfDoctor + "<=" + lfMaxRangeTotalDoctor );
				//					System.out.println(lfMinRangeTotalNurse + "<=" + lfRoom*lfNurse + "<=" + lfMaxRangeTotalNurse );
				// 医師総数及び看護師総数が条件を満たすかどうかを判定します。
				if ((lfMinRangeTotalDoctor <= lfRoom*lfDoctor && lfRoom*lfDoctor <= lfMaxRangeTotalDoctor) &&
					(lfMinRangeTotalNurse <= lfRoom*lfNurse && lfRoom*lfNurse <= lfMaxRangeTotalNurse))
				{
					// 判定後、どの値が適切な値かを判定します。距離尺度により判定します。
					// 現在の値に最も近いベクトル適切な値としてを採用します。ベクトル計算とみなし、ユークリッド距離により評価を実施。
					lfDistance = sqrt((lfRoom - plfArg[iRoomLoc])*(lfRoom - plfArg[iRoomLoc]) + (lfDoctor - plfArg[iDoctorLoc])*(lfDoctor - plfArg[iDoctorLoc]) + (lfNurse - plfArg[iNurseLoc])*(lfNurse - plfArg[iNurseLoc]));
					if (lfDistance <= lfMinDistance)
					{
						// そのときの部屋数、医師数、看護師数を取得します。
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
	// 該当データの組み合わせが見つかった場合
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
*   救急部門用の一般的な制約条件設定
* </PRE>
* @param plfArg		引数
* @param iGenVector	引数の次元数
* @author kobayashi
* @since 2017/04/03
* @version 0.1
*/
void vSetEDCalibrationCondition(double* plfArg, int iGenVector )
{
	// 部屋の設定
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

	// 診察室の制約条件を設定します。
	// 診察室の医師数
	// 診察室の看護師数
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

	// 診察室の部屋数を設定します。
	// 調査結果を基に設定。
	plfArg[0] = lfLimitter(plfArg[0], lfMaxRangeRoom, lfMinRangeRoom);
	// 診察室の医師数を設定します。
	// 現実的に考えて1部屋に1人が担当医だと考えられるので。2人は多く見積もった。
	plfArg[14] = lfLimitter(plfArg[14], lfMaxRangeDoctor, lfMinRangeDoctor);
	// 診察室の看護師数を設定します。
	// 最大数を設定できるようにします。
	plfArg[15] = lfLimitter(plfArg[15], lfMaxRangeNurse, lfMinRangeNurse);

	bRet = bUpdateRoomAgent(plfArg, 0, 14, 15, lfMinRangeTotalDoctor, lfMaxRangeTotalDoctor, lfMinRangeTotalNurse, lfMaxRangeTotalNurse,
		lfMinRangeRoom, lfMaxRangeRoom, lfMinRangeDoctor, lfMaxRangeDoctor, lfMinRangeNurse, lfMaxRangeNurse);
//		if( bRet == true ) System.out.println("診察室OK");
//		else			   System.out.println("診察室NG");

	// 手術室の制約条件を設定します。
	// 手術室の医師数を設定します。
	// 手術室の看護師数を設定します。
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

	// 手術室の部屋数を設定します。
	// 調査結果を基に設定。
	plfArg[1] = lfLimitter(plfArg[1], lfMaxRangeRoom, lfMinRangeRoom);
	// 手術室の医師数を設定します。
	// 最大数を設定できるようにします。
	plfArg[16] = lfLimitter(plfArg[16], lfMaxRangeNurse, lfMinRangeNurse);
	// 手術室の看護師数を設定します。
	// 最大数を設定できるようにします。
	plfArg[17] = lfLimitter(plfArg[17], lfMaxRangeNurse, lfMinRangeNurse);

	bRet = bUpdateRoomAgent(plfArg, 1, 16, 17, lfMinRangeTotalDoctor, lfMaxRangeTotalDoctor, lfMinRangeTotalNurse, lfMaxRangeTotalNurse,
		lfMinRangeRoom, lfMaxRangeRoom, lfMinRangeDoctor, lfMaxRangeDoctor, lfMinRangeNurse, lfMaxRangeNurse);
//		if( bRet == true ) System.out.println("手術室OK");
//		else			   System.out.println("手術室NG");

	// 初療室の制約条件を設定します。
	// 初療室の医師数
	// 初療室の看護師数
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

	// 初療室の部屋数を設定します。
	// 調査結果を基に設定。
	plfArg[2] = lfLimitter(plfArg[2], lfMaxRangeRoom, lfMinRangeRoom);
	// 初療室の医師数を設定します。
	// 現実的に考えて1部屋に1人が担当医だと考えられるので。2人は多く見積もった。
	plfArg[18] = lfLimitter(plfArg[18], lfMaxRangeDoctor, lfMinRangeDoctor);
	// 各手術室、各初療室の看護師数は最大で4人くらいと設定します。
	// 公刊文献を調べて3例ほどから。手術室が1部屋当りの人数を算出することができたのでそれを参考に設定。
	plfArg[19] = lfLimitter(plfArg[19], lfMaxRangeNurse, lfMinRangeNurse);

	bRet = bUpdateRoomAgent(plfArg, 2, 18, 19, lfMinRangeTotalDoctor, lfMaxRangeTotalDoctor, lfMinRangeTotalNurse, lfMaxRangeTotalNurse,
		lfMinRangeRoom, lfMaxRangeRoom, lfMinRangeDoctor, lfMaxRangeDoctor, lfMinRangeNurse, lfMaxRangeNurse);
//		if( bRet == true ) System.out.println("初療室OK");
//		else			   System.out.println("初療室NG");

	// 初療室の医療技師数
	if (plfArg[20] < 1.0) plfArg[20] = 1.0;
	if (plfArg[20] > 3.0) plfArg[20] = 3.0;

	// 観察室の制約条件を設定します。
	// 観察室の看護師数
	lfMinRangeTotalNurse = lfMinWeight * 25;
	lfMaxRangeTotalNurse = lfMaxWeight * 25;
	lfMinRangeRoom = 0;
	lfMaxRangeRoom = 14;
	lfMinRangeNurse = 1;
	lfMaxRangeNurse = 25;
//	lfMaxRangeNurse = 4;

	// 観察室の部屋数を設定します。
	// 調査結果を基に設定。
	plfArg[3] = lfLimitter(plfArg[3], lfMaxRangeRoom, lfMinRangeRoom);
	// 観察室に所属する看護師は2人以下とします。
	// 公刊文献で1例だけではあるが、記載されており、算出も可能であったのでそれをベースに設定。
	plfArg[21] = lfLimitter(plfArg[21], lfMaxRangeNurse, lfMinRangeNurse);

	bRet = bUpdateNurseRelativeRoom(plfArg, 3, 21, lfMinRangeRoom, lfMaxRangeRoom, lfMinRangeNurse, lfMaxRangeNurse,
		lfMinRangeTotalNurse, lfMaxRangeTotalNurse);

	if (bRet == true)
	{
		// 観察室の部屋数が0の場合は看護師も割り当てがないことから0とします。
		plfArg[3] = plfArg[21] == 0.0 ? 0.0 : plfArg[3];
	}
//		if( bRet == true ) System.out.println("観察室OK");
//		else			   System.out.println("観察室NG");

	// 重症観察室の制約条件を設定します。
	// 重症観察室の看護師数
	lfMinRangeTotalNurse = lfMinWeight * 25;
	lfMaxRangeTotalNurse = lfMaxWeight * 25;
	lfMinRangeRoom = 0;
	lfMaxRangeRoom = 4;
	lfMinRangeNurse = 1;
	lfMaxRangeNurse = 25;
//	lfMaxRangeNurse = 2;

	// 重症観察室の部屋数を設定します。
	// 調査結果を基に設定。
	plfArg[4] = lfLimitter(plfArg[4], lfMaxRangeRoom, lfMinRangeRoom);
	// 重症観察室に所属する看護師は2人以下とします。
	// 公刊文献で1例だけではあるが、記載されており、算出も可能であったのでそれをベースに設定。
	plfArg[22] = lfLimitter(plfArg[22], lfMaxRangeNurse, lfMinRangeNurse);

	bRet = bUpdateNurseRelativeRoom(plfArg, 4, 22, lfMinRangeRoom, lfMaxRangeRoom, lfMinRangeNurse, lfMaxRangeNurse,
		lfMinRangeTotalNurse, lfMaxRangeTotalNurse);

	if (bRet == true)
	{
		// 重症観察室の部屋数が0の場合は看護師も割り当てがないことから0とします。
		plfArg[4] = plfArg[22] == 0.0 ? 0.0 : plfArg[4];
	}
//	if( bRet == true ) System.out.println("重症観察室OK");
//	else			   System.out.println("重症観察室NG");

	// ICUの制約条件を設定します。
	// ICUの医師数
	// ICUの看護師数

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

	// ICU室の部屋数を設定します。
	// 調査結果を基に設定。
	plfArg[5] = lfLimitter(plfArg[5], lfMaxRangeRoom, lfMinRangeRoom);
	// ICUの医師数を設定します。
	// 最大数を設定できるようにします。
	plfArg[23] = lfLimitter(plfArg[23], lfMaxRangeDoctor, lfMinRangeDoctor);
	// ICUの1人当り看護師数を設定します。
	// ソースは医療法の医療点数に記載及び公刊文献を基に設定。
	plfArg[24] = lfLimitter(plfArg[24], lfMaxRangeNurse, lfMinRangeNurse);

	bRet = bUpdateRoomAgent(plfArg, 5, 23, 24, lfMinRangeTotalDoctor, lfMaxRangeTotalDoctor, lfMinRangeTotalNurse, lfMaxRangeTotalNurse,
		lfMinRangeRoom, lfMaxRangeRoom, lfMinRangeDoctor, lfMaxRangeDoctor, lfMinRangeNurse, lfMaxRangeNurse);
//	if( bRet == true ) System.out.println("ICU OK");
//	else			   System.out.println("ICU NG");

	// HCUの制約条件を設定します。
	// HCUの医師数
	// HCUの看護師数
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

	// HCU室の部屋数を設定します。
	// 調査結果を基に設定。
	plfArg[6] = lfLimitter(plfArg[6], lfMaxRangeRoom, lfMinRangeRoom);
	// HCUの医師数を設定します。(HCUが存在している場合に設定します。)
	// 公刊文献上記載データから設定。
	plfArg[25] = lfLimitter(plfArg[25], lfMaxRangeDoctor, lfMinRangeDoctor);
	// HCUの1人当り看護師数を設定します。
	// ソースは医療法の医療点数に記載及び公刊文献を基に設定。
	plfArg[26] = lfLimitter(plfArg[26], lfMaxRangeNurse, lfMinRangeNurse);

	bRet = bUpdateRoomAgent(plfArg, 6, 25, 26, lfMinRangeTotalDoctor, lfMaxRangeTotalDoctor, lfMinRangeTotalNurse, lfMaxRangeTotalNurse,
		lfMinRangeRoom, lfMaxRangeRoom, lfMinRangeDoctor, lfMaxRangeDoctor, lfMinRangeNurse, lfMaxRangeNurse);
	if (bRet == true)
	{
		// HCUの部屋数が0の場合は医師も割り当てがないことから0とします。
		// HCUの部屋数が0の場合は看護師も割り当てがないことから0とします。
		plfArg[25] = plfArg[6] == 0.0 ? 0.0 : plfArg[25];
		plfArg[26] = plfArg[6] == 0.0 ? 0.0 : plfArg[26];
	}
//	if( bRet == true ) System.out.println("HCU OK");
//	else			   System.out.println("HCU NG");

	// 一般病棟は制限なしとします。

	// 一般病棟の医師数を設定します。1病床に対して1人とします。
	// 実際には2人の固定値としています。
	if (plfArg[27] < 1.0) plfArg[27] = 1.0;
	if (plfArg[27] > 2.0) plfArg[27] = 2.0;

	// 一般病棟の看護師数
	lfMinRangeTotalNurse = lfMinWeight * 1316;
	lfMaxRangeTotalNurse = lfMaxWeight * 1316;
	lfMinRangeRoom = 1;
	lfMaxRangeRoom = 1296;
	lfMinRangeNurse = 1;
	lfMaxRangeNurse = 1296;
//	lfMaxRangeNurse = 5;

	// 一般病棟の部屋数を設定します。
	// 調査結果を基に設定。
	plfArg[7] = lfLimitter(plfArg[7], lfMaxRangeRoom, lfMinRangeRoom);
	// 一般病棟の看護師数を設定します。入院患者1(1病床)に対して最大看護師7人の設定とします。
	// ソースは医療法の医療点数に記載。
	plfArg[28] = lfLimitter(plfArg[28], lfMaxRangeNurse, lfMinRangeNurse);

	bRet = bUpdateNurseRelativeRoom(plfArg, 7, 28, lfMinRangeRoom, lfMaxRangeRoom, lfMinRangeNurse, lfMaxRangeNurse,
		lfMinRangeTotalNurse, lfMaxRangeTotalNurse);
//	if( bRet == true ) System.out.println("一般病棟OK");
//	else			   System.out.println("一般病棟NG");

	// 待合室の制約条件を設定します。
	// 待合室は1部屋とします。
	plfArg[8] = 1;
	// 待合室の看護師の数を設定します。
	// 1人以上はいると予想されるので、そのように設定します。
	// 入院患者1(1病床)に対して最大看護師7人の設定とします。(医療法の医療点数を基に設定しました。)
	if (plfArg[29] < 1.0) plfArg[29] = 1.0;
	if (plfArg[29] > 7.0) plfArg[29] = 7.0;

	// X線室の制約条件を設定します。
	// 総数に関しても同様に調査結果を基に設定。
	lfMinRangeTotalClinicalEngineer = lfMinWeight * 22;
	lfMaxRangeTotalClinicalEngineer = lfMaxWeight * 22;
	lfMinRangeRoom = 1;
	lfMaxRangeRoom = 18;
	lfMinRangeClinicalEngineer = 1;
	lfMaxRangeClinicalEngineer = 3;
//	lfMaxRangeClinicalEngineer = 3;

	// X線室の部屋数を設定します。
	// 調査結果を基に設定。
	plfArg[9] = lfLimitter(plfArg[9], lfMaxRangeRoom, lfMinRangeRoom);
	// 各検査室に所属する医療技師数は1人以上3人以下とします。
	// ソースは医療法には適当数のみ。なので、1部屋あたり3人程度とします。
	// 公刊文献にも詳しい値が記載されていないことから。
	plfArg[30] = lfLimitter(plfArg[30], lfMaxRangeClinicalEngineer, lfMinRangeClinicalEngineer);

	bRet = bUpdateClinicalEngineerRelativeRoom(plfArg, 9, 30,
		lfMinRangeRoom, lfMaxRangeRoom, lfMinRangeClinicalEngineer, lfMaxRangeClinicalEngineer,
		lfMinRangeTotalClinicalEngineer, lfMaxRangeTotalClinicalEngineer);
//	if( bRet == true ) System.out.println("X線室OK");
//	else			   System.out.println("X線室NG");


	// CT室の制約条件を設定します。
	lfMinRangeTotalClinicalEngineer = lfMinWeight * 16;
	lfMaxRangeTotalClinicalEngineer = lfMaxWeight * 16;
	lfMinRangeRoom = 1;
	lfMaxRangeRoom = 4;
	lfMaxRangeRoom = 16;
	lfMinRangeClinicalEngineer = 1;
	lfMaxRangeClinicalEngineer = 4;
	//		lfMaxRangeClinicalEngineer = 4;

	// CT室の部屋数を設定します。
	// 調査結果を基に設定。
	plfArg[10] = lfLimitter(plfArg[10], lfMaxRangeRoom, lfMinRangeRoom);
	// 各検査室に所属する医療技師数は1人以上3人以下とします。
	// ソースは医療法には適当数のみ。なので、1部屋あたり3人程度とします。
	// 公刊文献にも詳しい値が記載されていないことから。
	plfArg[31] = lfLimitter(plfArg[31], lfMaxRangeClinicalEngineer, lfMinRangeClinicalEngineer);

	bRet = bUpdateClinicalEngineerRelativeRoom(plfArg, 10, 31,
		lfMinRangeRoom, lfMaxRangeRoom, lfMinRangeClinicalEngineer, lfMaxRangeClinicalEngineer,
		lfMinRangeTotalClinicalEngineer, lfMaxRangeTotalClinicalEngineer);

	// MRI室の制約条件を設定します。
	lfMinRangeTotalClinicalEngineer = lfMinWeight * 14;
	lfMaxRangeTotalClinicalEngineer = lfMaxWeight * 14;
	lfMinRangeRoom = 1;
	lfMaxRangeRoom = 7;
	lfMinRangeClinicalEngineer = 1;
	lfMaxRangeClinicalEngineer = 3;
	//		lfMaxRangeClinicalEngineer = 3;

	// MRI室の部屋数を設定します。
	// 調査結果を基に設定。
	plfArg[11] = lfLimitter(plfArg[11], lfMaxRangeRoom, lfMinRangeRoom);
	// 各検査室に所属する医療技師数は1人以上3人以下とします。
	// ソースは医療法には適当数のみ。なので、1部屋あたり3人程度とします。
	// 公刊文献にも詳しい値が記載されていないことから。
	plfArg[32] = lfLimitter(plfArg[32], lfMaxRangeClinicalEngineer, lfMinRangeClinicalEngineer);

	bRet = bUpdateClinicalEngineerRelativeRoom(plfArg, 11, 32,
		lfMinRangeRoom, lfMaxRangeRoom, lfMinRangeClinicalEngineer, lfMaxRangeClinicalEngineer,
		lfMinRangeTotalClinicalEngineer, lfMaxRangeTotalClinicalEngineer);
//	if( bRet == true ) System.out.println("MRI室OK");
//	else			   System.out.println("MRI室NG");

	// 血管造影室の制約条件を設定します。
	lfMinRangeTotalClinicalEngineer = lfMinWeight * 9;
	lfMaxRangeTotalClinicalEngineer = lfMaxWeight * 9;
	lfMinRangeRoom = 1;
//	lfMaxRangeRoom = 3;
	lfMaxRangeRoom = 9;
	lfMinRangeClinicalEngineer = 1;
	lfMaxRangeClinicalEngineer = 3;
//	lfMaxRangeClinicalEngineer = 3;

	// 血管造影室の部屋数を設定します。
	// 調査結果を基に設定。
	plfArg[12] = lfLimitter(plfArg[12], lfMaxRangeRoom, lfMinRangeRoom);
	// 各検査室に所属する医療技師数は1人以上3人以下とします。
	// ソースは医療法には適当数のみ。なので、1部屋あたり3人程度とします。
	// 公刊文献にも詳しい値が記載されていないことから。
	plfArg[33] = lfLimitter(plfArg[33], lfMaxRangeClinicalEngineer, lfMinRangeClinicalEngineer);

	bRet = bUpdateClinicalEngineerRelativeRoom(plfArg, 12, 33,
		lfMinRangeRoom, lfMaxRangeRoom, lfMinRangeClinicalEngineer, lfMaxRangeClinicalEngineer,
		lfMinRangeTotalClinicalEngineer, lfMaxRangeTotalClinicalEngineer);
//	if( bRet == true ) System.out.println("血管造影室OK");
//	else			   System.out.println("血管造影室NG");

	// FAST室の制約条件を設定します。
	lfMinRangeTotalClinicalEngineer = lfMinWeight * 12;
	lfMaxRangeTotalClinicalEngineer = lfMaxWeight * 12;
	lfMinRangeRoom = 1;
//	lfMaxRangeRoom = 4;
	lfMaxRangeRoom = 12;
	lfMinRangeClinicalEngineer = 1;
	lfMaxRangeClinicalEngineer = 3;
//	lfMaxRangeClinicalEngineer = 3;

	// 超音波室の部屋数を設定します。
	// 調査結果を基に設定。
	plfArg[13] = lfLimitter(plfArg[13], lfMaxRangeRoom, lfMinRangeRoom);
	// 各検査室に所属する医療技師数は1人以上3人以下とします。
	// ソースは医療法には適当数のみ。なので、1部屋あたり3人程度とします。
	// 公刊文献にも詳しい値が記載されていないことから。
	plfArg[34] = lfLimitter(plfArg[34], lfMaxRangeClinicalEngineer, lfMinRangeClinicalEngineer);

	bRet = bUpdateClinicalEngineerRelativeRoom(plfArg, 13, 34,
		lfMinRangeRoom, lfMaxRangeRoom, lfMinRangeClinicalEngineer, lfMaxRangeClinicalEngineer,
		lfMinRangeTotalClinicalEngineer, lfMaxRangeTotalClinicalEngineer);
//	if( bRet == true ) System.out.println("FAST室OK");
//	else			   System.out.println("FAST室NG");

	// 部屋を構成するエージェント数の設定

	// 年間で来院する患者の人数の算出が必要です。
	// 筑波メディカルセンターの場合は・・・
	// 聖隷浜松病院の場合は・・・19379人
	// 各診察室の医師数は最大でも2人くらいに設定します。
	// 算出方法は医療法に記載のものから算出。
	//		// 患者の総数を取得します。
	//		lfPatientsNum = 19379;
	//		// 必要医師数を算出します。
	//		lfDoctorsNum = lfPatientsNum*0.0625+lfPatientsNum*1.0/16.0;
	//		// 必要看護師数を算出します。
	//		lfNursesNum = lfPatientsNum*1.0/3.0+lfPatientsNum*1.0/30.0;
	//		// 総部屋数を算出します。
	//		lfTotalRoomNum = plfArg[0]+plfArg[1]+plfArg[2]+plfArg[3]+plfArg[4]+plfArg[5]+plfArg[6]+plfArg[7]+plfArg[8]+plfArg[9]+plfArg[10]+plfArg[11]+plfArg[12]+plfArg[13];
	//
	//		lfOneRoomDoctorNum			 = lfDoctorsNum / lfTotalRoomNum;
	//		lfOneRoomNurseNum			 = lfNursesNum / lfTotalRoomNum;
	//		lfOneRoomClinicalEngineerNum = lfClinicalEngineersNum / lfTotalRoomNum;

}