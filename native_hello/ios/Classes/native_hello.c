//
// Created by CKDVB-mac on 2022/4/6.
//

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <string.h>

#include "book.h"

#define BOOKDAT_FILE_NAME "/data/user/0/com.xjy.memorycard.memory_card/app_flutter/kc.sgd"//"课程数据样例.cour"

u8 gBuff[409600];
void PrintUnicode(const u16* pUnicode, u32 txtlen, u32 txt_count)
{
	u8 *pStr;
	u16 *wptr;

	wptr = (u16*)pUnicode;
	pStr = malloc(txtlen);
	for(; txt_count>1; txt_count--) {
		while (*wptr) wptr++;
		*wptr++ = '\n';
	}
	//WideCharToMultiByte( CP_ACP, 0, (LPCWSTR)pUnicode, -1, pStr, txtlen, NULL, NULL);
	printf("%s\n", pStr);
	if (pStr != NULL)
		free(pStr);
}


void getLessonShow(BOOK_INFO* pBookInfo)
{
	//取所有目录名并显示出来
	u16 i, j;
	UNIT_INFO UnitInfo;
	LESSON_INFO LessonInfo;
	int txtlen;
	MGO_DEBUG("UNIT %d\n", pBookInfo->total_unit_num);
	for (i = 0; i < pBookInfo->total_unit_num; i++)
	{
		//取单元内容
		getUnitInfo(pBookInfo, i, &UnitInfo);
		MGO_DEBUG("UNIT %d, Total %d Lessons\n", i + 1, UnitInfo.lesson_num);
		for (j = 0; j < UnitInfo.lesson_num; j++)
		{
			//取课目录内容
			getLessonInfo(&UnitInfo, j, &LessonInfo);

			txtlen = getTxtData(pBookInfo, LessonInfo.lesson_head.title_ch_txt_idx, gBuff, sizeof(gBuff));
			PrintUnicode((u16*)gBuff, txtlen, 1);
			txtlen = getTxtData(pBookInfo, LessonInfo.lesson_head.title_eng_txt_idx, gBuff, sizeof(gBuff));
			PrintUnicode((u16*)gBuff, txtlen, 1);

		}
		MGO_DEBUG("--------------------------------------------------------------------------\n");
	}
}


void test9data()
{
	//查找第4页里面包含"上"的字符串，找到一个就直接返回
//	PAGE_INFO PageInfo;
//	CELL_INFO CellInfo;
	BOOK_INFO BookInfo;
	u8 snStr[17];

	MGO_DEBUG("=================enter test9===========================\n");
	if (OpenBook(&BookInfo, BOOKDAT_FILE_NAME,&snStr))
	{
		getLessonShow(&BookInfo);

		CloseBook(&BookInfo);
	}
	MGO_DEBUG("--------------------------------------------------------------------------\n");
}

int bookmain()
{
	BOOK_INFO BookInfo;
	u8 snStr[17];

	test9data();

	return 11;
}



int32_t getVersionNative() {
    return 2;
}
