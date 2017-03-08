#include<cmath>
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
