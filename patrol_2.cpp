/*==============================================================================

   ���񃋁[�g�Ǘ� [patrol.cpp]
	Author :	
	Date   :
--------------------------------------------------------------------------------

==============================================================================*/
#include "patrol.h"

// ���[�g���W�l
const char g_Root[2][ROOT_MAX2] =
{
	// �オX ����Y
	{13,19,22,22,13, 4,6},
	{7 ,4 ,7 ,11,11,11,5}
};

const char g_Root_2[2][ROOT_MAX2_1] =
{
	// �オX ����Y
	{12,19,22,16,19,2,6}, //������
	{5 ,3 ,7 ,11,11,6,2}
};

char SetRouteX2(char number)
{
	return g_Root[0][number];
}

char SetRouteY2(char number)
{
	return g_Root[1][number];

}

char SetRouteX2_1(char number)
{
	return g_Root_2[0][number];
}

char SetRouteY2_1(char number)
{
	return g_Root_2[1][number];

}