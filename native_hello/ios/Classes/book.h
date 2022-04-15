#ifndef __BOOK_H__
#define __BOOK_H__

typedef int             s32;
typedef short           s16;          
typedef char            s8;
typedef unsigned int    u32;
typedef unsigned short  u16;
typedef unsigned char   u8;

typedef int             _BOOL_;

#ifndef NULL
#define NULL (void*)0
#endif

#ifndef FALSE
#define FALSE (0)
#endif
#ifndef TRUE
#define TRUE (1)
#endif

#define DEBUG_APP				1		// 1:debug;	0:release

#if (DEBUG_APP > 0)
#define MGO_DEBUG_ON
#endif

#ifdef MGO_DEBUG_ON
#define MGO_DEBUG printf
#define ASSERT(f) \
		do { \
		if (!(f) )\
					printf("assert: %s: %d",__FILE__,__LINE__); \
		} while (0)


#else

#define MGO_DEBUG
#define printf
#define ASSERT(f)

#endif


#define SEARCH_MODE_DEF		0
#define SEARCH_MODE_NOCASE	0x0001	//不区分大小写
#define SEARCH_MODE_WORD	0x0002	//全词匹配

#define MAX_TXT_LEN (1024)

#ifndef __packed
#define __packed __attribute__((aligned (1),packed))
#else
#pragma message "Already defined __packed"
#endif

//数据加密宏定义开关
#define __ENCRPT__

#define min(a,b) (((a) < (b)) ? (a) : (b))
/*********************************************************************/
#pragma pack(1) //单字节对齐，必须和数据存储格式保持一致
typedef struct tagBOOK_HEAD
{
	u8 MAGIC[6];
	u8 VER[2];
	u8 reserved1[24];
	u32 unit_addr;
	u32 page_addr;
	u32 txt_addr;
	u32 audio_addr;
	u8 reserved2[20];
}BOOK_HEAD;

typedef struct tagUNIT_LIST
{
	u16 unit_num;
	u8 reserved[8];
}UNIT_LIST;

typedef struct tagPAGE_LIST
{
	u16 page_num;
}PAGE_LIST;

typedef struct tagTXT_LIST
{
	u16 txt_num;
}TXT_LIST;

typedef struct tagAUDIO_LIST
{
	u16 audio_num;
}AUDIO_LIST;

typedef struct tagUNIT_HEAD
{
	u16 unit_bpage;
	u16 unit_epage;
	u32 title_ch_txt_idx;
	u32 title_ch_audio_idx;
	u32 title_eng_txt_idx;
	u32 title_eng_audio_idx;
	u8 reserved[8];
	u16 lesson_num;
}UNIT_HEAD;

typedef struct tagLESSON_HEAD
{
	u16 lesson_bpage;
	u16 lesson_epage;
	u32 title_ch_txt_idx;
	u32 title_ch_audio_idx;
	u32 title_eng_txt_idx;
	u32 title_eng_audio_idx;
	u8 reserved[8];
	u16 cell_num;
}LESSON_HEAD;

typedef struct tagPAGE_HEAD
{
	u16 page;
	u16 cell_num;
	u32 cell_offset;
}PAGE_HEAD;

typedef struct tagCELL_HEAD
{
	u32 cell_ch_txt;
	u32 cell_ch_audio;
	u32 cell_eng_txt;
	u32 cell_eng_audio;
}CELL_HEAD;

typedef struct tagCELL_LESSON_HEAD
{
	u16 cell_page;
	CELL_HEAD cell_head;
	u8 reserved[16];
}CELL_LESSON_HEAD;

typedef struct tagCELL_PAGE_HEAD
{
	CELL_HEAD cell_head;
	u8 reserved[16];
}CELL_PAGE_HEAD;

/*********************************************************************/
#pragma pack(4)//恢复为4字节对齐，以下结构为运行时结构定义
typedef struct tagBOOK_INFO
{
	void* DataHandle;//当前课本的数据操作句柄
	u32 total_unit_num;//当前课本的单元总数
	u32 unit_offset;
	u32 total_page_num;//当前课本的页面总数
	u32 page_offset;
	u32 total_txt_num;//当前课本的文本总数
	u32 txt_offset;
	u32 total_audio_num;//当前课本的音频总数
	u32 audio_offset;
}BOOK_INFO;

typedef struct tagUNIT_INFO
{
	BOOK_INFO* BookHandle;
	UNIT_HEAD unit_head;
//	u16 unit_index;
//	u32 unit_offset;
	u32 lesson_offset;
	u16 lesson_num;//当前单元的课程总数
}UNIT_INFO;

typedef struct tagLESSON_INFO
{
	BOOK_INFO* BookHandle;
	LESSON_HEAD lesson_head;
//	u16 lesson_index;
//	u32 lesson_offset;
	u32 cell_offset;
	u16 cell_num;//当前课程的点读数据总数
}LESSON_INFO;

typedef struct tagPAGE_INFO
{
	BOOK_INFO* BookHandle;
	PAGE_HEAD page_head;
//	u16 page_index;
//	u32 page_offset;
	u32 cell_offset;
	u16 cell_num;//当前页面的点读数据总数
}PAGE_INFO;

typedef struct tagCELL_INFO
{
	u16 cell_index;
	u16 cell_page;//当前点读数据所在页面
	u32 cell_ch_txt_idx;//当前点读数据中文文本序号
	u32 cell_ch_audio_idx;//当前点读数据中文音频序号
	u32 cell_eng_txt_idx;//当前点读数据英文文本序号
	u32 cell_eng_audio_idx;//当前点读数据英文音频序号
}CELL_INFO;

/*
	打开一个课本实例，如果打开成功，操作完以后必须关闭
*/
_BOOL_ OpenBook(BOOK_INFO *pBookInfo, u8* BookName, u8* snTemp1[]);
/*
	关闭一个课本实例
*/
void CloseBook(BOOK_INFO *pBookInfo);

/*
	获取课本的一个单元实例
*/
_BOOL_ getUnitInfo(BOOK_INFO *pBookInfo, u32 unit_index, UNIT_INFO *pUnitInfo);
/*
	获取课本的一个课程实例
*/
_BOOL_ getLessonInfo(UNIT_INFO *pUnitInfo, u32 lesson_index, LESSON_INFO *pLessonInfo);
/*
	获取课本的一个页面实例
*/
_BOOL_ getPageInfo(BOOK_INFO *pBookInfo, u32 page, PAGE_INFO *pPageInfo);
/*
	从课程获取一个点读数据实例
*/
_BOOL_ getCellInfoL(LESSON_INFO *pLessonInfo, u32 cell_index, CELL_INFO *pCellInfo);
/*
	从页面根据序号获取一个点读数据实例
*/
_BOOL_ getCellInfoP(PAGE_INFO *pPageInfo, u32 cell_index, CELL_INFO *pCellInfo);
/*
	从页面根据文本内容查找一个点读数据实例
*/
_BOOL_ getCellInfoPT(PAGE_INFO *pPageInfo, const u16 *strTxt, CELL_INFO *pCellInfo);

/*
函数功能：获取指定索引号字符串的数据
输入参数：
	pBookInfo：课本的句柄
	txt_index：字符串索引号
	buff：字符串输出的缓冲区
	size：缓冲区大小
输出参数：
	当buff为空时，计算需要缓冲区的大小
	当buff不为空时，buff返回字符串数据，双'\0'结束
函数返回：
	当缓冲区不为空时，输出到缓冲区中字符串的总长度
	当缓冲区为空或者size为0，则统计字符串的总长度
*/
u32 getTxtData(BOOK_INFO *pBookInfo, u32 txt_index, u8* buff, u32 size);
/*
函数功能：获取指定索引号音频的数据
输入参数：
	pBookInfo：课本的句柄
	audio_index：音频索引号
	buff：音频数据输出的缓冲区
	size：缓冲区大小
输出参数：
	当buff为空时，计算需要缓冲区的大小
	当buff不为空时，buff音频数据
函数返回：
	当缓冲区不为空时，输出到缓冲区音频数据的总长度
	当缓冲区为空或者size为0，则统计音频数据的总长度
*/
u32 getAudioData(BOOK_INFO *pBookInfo, u32 audio_index, u8* buff, u32 size);


/*
函数功能：获取当前课本所有单元标题的字符串
	按照标题的中文标题和英文标题顺序排列，中间通过字符串结束符分隔。
输入参数：
	pBookInfo：课本的句柄
	buff：字符串输出的缓冲区
	pbuffsize：指针指向缓冲区大小
输出参数：
	当buff为空时，计算需要缓冲区的大小，通过指针pbuffsize返回
	当buff不为空时，输出字符串的数据，pbuffsize输出数据总长度
函数返回：
	当缓冲区不为空时，输出到缓冲区中字符串的总个数
	当缓冲区为空，则统计字符串总个数
*/
u32 getUnitTitleList(BOOK_INFO *pBookInfo, u8* buff, u32 *pbuffsize);

/*
函数功能：获取当前课本所有课程标题的字符串
	按照标题的中文标题和英文标题顺序排列，中间通过字符串结束符分隔。
输入参数：
	pUnitInfo：课本的句柄
	buff：字符串输出的缓冲区
	pbuffsize：指针指向缓冲区大小
输出参数：
	当buff为空时，计算需要缓冲区的大小，通过指针pbuffsize返回
	当buff不为空时，输出字符串的数据，pbuffsize输出数据总长度
函数返回：
	当缓冲区不为空时，输出到缓冲区中字符串的总个数
	当缓冲区为空，则统计字符串总个数
*/
u32 getLessonTitleList(UNIT_INFO *pUnitInfo, u8* buff, u32 *pbuffsize);

/*
函数功能：获取当前课本所有课程内容的字符串
	按照标题的中文标题和英文标题顺序排列，中间通过字符串结束符分隔。
输入参数：
	pLessonInfo：课本的句柄
	buff：字符串输出的缓冲区
	pbuffsize：指针指向缓冲区大小
输出参数：
	当buff为空时，计算需要缓冲区的大小，通过指针pbuffsize返回
	当buff不为空时，输出字符串的数据，pbuffsize输出数据总长度
函数返回：
	当缓冲区不为空时，输出到缓冲区中字符串的总个数
	当缓冲区为空，则统计字符串总个数
*/
u32 getLessonTxt(LESSON_INFO *pLessonInfo, u8* buff, u32 *pbuffsize);

/*
函数功能：获取当前页所有内容的字符串
	按照标题的中文标题和英文标题顺序排列，中间通过字符串结束符分隔。
输入参数：
	pPageInfo：页面的句柄
	buff：字符串输出的缓冲区
	pbuffsize：指针指向缓冲区大小
输出参数：
	当buff为空时，计算需要缓冲区的大小，通过指针pbuffsize返回
	当buff不为空时，输出字符串的数据，pbuffsize输出数据总长度
函数返回：
	当缓冲区不为空时，输出到缓冲区中字符串的总个数
	当缓冲区为空，则统计字符串总个数
*/
u32 getPageTxt(PAGE_INFO *pPageInfo, u8* buff, u32 *pbuffsize);

/*
函数功能：字符串查找，同wcsstr函数功能，带模式大小写和单词匹配

输入参数：
	string：unicode编码字符串
	strCharSet：带查找的字符串
	mode：匹配模式
		缺省SEARCH_MODE_DEF为完全匹配模式；
		SEARCH_MODE_NOCASE模式不区分字母大小写；
		SEARCH_MODE_WORD模式要求全词匹配；
		注意：如果是全词匹配模式，要求被搜索字符串去掉首尾的非字母数字字符
函数返回：
	如果匹配上，返回匹配字符串的指针，否则返回空
*/
u16* ucs2strstr( const u16 *string, const u16 *strCharSet, u16 mode);

#endif
