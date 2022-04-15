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
#define SEARCH_MODE_NOCASE	0x0001	//�����ִ�Сд
#define SEARCH_MODE_WORD	0x0002	//ȫ��ƥ��

#define MAX_TXT_LEN (1024)

#ifndef __packed
#define __packed __attribute__((aligned (1),packed))
#else
#pragma message "Already defined __packed"
#endif

//���ݼ��ܺ궨�忪��
#define __ENCRPT__

#define min(a,b) (((a) < (b)) ? (a) : (b))
/*********************************************************************/
#pragma pack(1) //���ֽڶ��룬��������ݴ洢��ʽ����һ��
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
#pragma pack(4)//�ָ�Ϊ4�ֽڶ��룬���½ṹΪ����ʱ�ṹ����
typedef struct tagBOOK_INFO
{
	void* DataHandle;//��ǰ�α������ݲ������
	u32 total_unit_num;//��ǰ�α��ĵ�Ԫ����
	u32 unit_offset;
	u32 total_page_num;//��ǰ�α���ҳ������
	u32 page_offset;
	u32 total_txt_num;//��ǰ�α����ı�����
	u32 txt_offset;
	u32 total_audio_num;//��ǰ�α�����Ƶ����
	u32 audio_offset;
}BOOK_INFO;

typedef struct tagUNIT_INFO
{
	BOOK_INFO* BookHandle;
	UNIT_HEAD unit_head;
//	u16 unit_index;
//	u32 unit_offset;
	u32 lesson_offset;
	u16 lesson_num;//��ǰ��Ԫ�Ŀγ�����
}UNIT_INFO;

typedef struct tagLESSON_INFO
{
	BOOK_INFO* BookHandle;
	LESSON_HEAD lesson_head;
//	u16 lesson_index;
//	u32 lesson_offset;
	u32 cell_offset;
	u16 cell_num;//��ǰ�γ̵ĵ����������
}LESSON_INFO;

typedef struct tagPAGE_INFO
{
	BOOK_INFO* BookHandle;
	PAGE_HEAD page_head;
//	u16 page_index;
//	u32 page_offset;
	u32 cell_offset;
	u16 cell_num;//��ǰҳ��ĵ����������
}PAGE_INFO;

typedef struct tagCELL_INFO
{
	u16 cell_index;
	u16 cell_page;//��ǰ�����������ҳ��
	u32 cell_ch_txt_idx;//��ǰ������������ı����
	u32 cell_ch_audio_idx;//��ǰ�������������Ƶ���
	u32 cell_eng_txt_idx;//��ǰ�������Ӣ���ı����
	u32 cell_eng_audio_idx;//��ǰ�������Ӣ����Ƶ���
}CELL_INFO;

/*
	��һ���α�ʵ��������򿪳ɹ����������Ժ����ر�
*/
_BOOL_ OpenBook(BOOK_INFO *pBookInfo, u8* BookName, u8* snTemp1[]);
/*
	�ر�һ���α�ʵ��
*/
void CloseBook(BOOK_INFO *pBookInfo);

/*
	��ȡ�α���һ����Ԫʵ��
*/
_BOOL_ getUnitInfo(BOOK_INFO *pBookInfo, u32 unit_index, UNIT_INFO *pUnitInfo);
/*
	��ȡ�α���һ���γ�ʵ��
*/
_BOOL_ getLessonInfo(UNIT_INFO *pUnitInfo, u32 lesson_index, LESSON_INFO *pLessonInfo);
/*
	��ȡ�α���һ��ҳ��ʵ��
*/
_BOOL_ getPageInfo(BOOK_INFO *pBookInfo, u32 page, PAGE_INFO *pPageInfo);
/*
	�ӿγ̻�ȡһ���������ʵ��
*/
_BOOL_ getCellInfoL(LESSON_INFO *pLessonInfo, u32 cell_index, CELL_INFO *pCellInfo);
/*
	��ҳ�������Ż�ȡһ���������ʵ��
*/
_BOOL_ getCellInfoP(PAGE_INFO *pPageInfo, u32 cell_index, CELL_INFO *pCellInfo);
/*
	��ҳ������ı����ݲ���һ���������ʵ��
*/
_BOOL_ getCellInfoPT(PAGE_INFO *pPageInfo, const u16 *strTxt, CELL_INFO *pCellInfo);

/*
�������ܣ���ȡָ���������ַ���������
���������
	pBookInfo���α��ľ��
	txt_index���ַ���������
	buff���ַ�������Ļ�����
	size����������С
���������
	��buffΪ��ʱ��������Ҫ�������Ĵ�С
	��buff��Ϊ��ʱ��buff�����ַ������ݣ�˫'\0'����
�������أ�
	����������Ϊ��ʱ����������������ַ������ܳ���
	��������Ϊ�ջ���sizeΪ0����ͳ���ַ������ܳ���
*/
u32 getTxtData(BOOK_INFO *pBookInfo, u32 txt_index, u8* buff, u32 size);
/*
�������ܣ���ȡָ����������Ƶ������
���������
	pBookInfo���α��ľ��
	audio_index����Ƶ������
	buff����Ƶ��������Ļ�����
	size����������С
���������
	��buffΪ��ʱ��������Ҫ�������Ĵ�С
	��buff��Ϊ��ʱ��buff��Ƶ����
�������أ�
	����������Ϊ��ʱ���������������Ƶ���ݵ��ܳ���
	��������Ϊ�ջ���sizeΪ0����ͳ����Ƶ���ݵ��ܳ���
*/
u32 getAudioData(BOOK_INFO *pBookInfo, u32 audio_index, u8* buff, u32 size);


/*
�������ܣ���ȡ��ǰ�α����е�Ԫ������ַ���
	���ձ�������ı����Ӣ�ı���˳�����У��м�ͨ���ַ����������ָ���
���������
	pBookInfo���α��ľ��
	buff���ַ�������Ļ�����
	pbuffsize��ָ��ָ�򻺳�����С
���������
	��buffΪ��ʱ��������Ҫ�������Ĵ�С��ͨ��ָ��pbuffsize����
	��buff��Ϊ��ʱ������ַ��������ݣ�pbuffsize��������ܳ���
�������أ�
	����������Ϊ��ʱ����������������ַ������ܸ���
	��������Ϊ�գ���ͳ���ַ����ܸ���
*/
u32 getUnitTitleList(BOOK_INFO *pBookInfo, u8* buff, u32 *pbuffsize);

/*
�������ܣ���ȡ��ǰ�α����пγ̱�����ַ���
	���ձ�������ı����Ӣ�ı���˳�����У��м�ͨ���ַ����������ָ���
���������
	pUnitInfo���α��ľ��
	buff���ַ�������Ļ�����
	pbuffsize��ָ��ָ�򻺳�����С
���������
	��buffΪ��ʱ��������Ҫ�������Ĵ�С��ͨ��ָ��pbuffsize����
	��buff��Ϊ��ʱ������ַ��������ݣ�pbuffsize��������ܳ���
�������أ�
	����������Ϊ��ʱ����������������ַ������ܸ���
	��������Ϊ�գ���ͳ���ַ����ܸ���
*/
u32 getLessonTitleList(UNIT_INFO *pUnitInfo, u8* buff, u32 *pbuffsize);

/*
�������ܣ���ȡ��ǰ�α����пγ����ݵ��ַ���
	���ձ�������ı����Ӣ�ı���˳�����У��м�ͨ���ַ����������ָ���
���������
	pLessonInfo���α��ľ��
	buff���ַ�������Ļ�����
	pbuffsize��ָ��ָ�򻺳�����С
���������
	��buffΪ��ʱ��������Ҫ�������Ĵ�С��ͨ��ָ��pbuffsize����
	��buff��Ϊ��ʱ������ַ��������ݣ�pbuffsize��������ܳ���
�������أ�
	����������Ϊ��ʱ����������������ַ������ܸ���
	��������Ϊ�գ���ͳ���ַ����ܸ���
*/
u32 getLessonTxt(LESSON_INFO *pLessonInfo, u8* buff, u32 *pbuffsize);

/*
�������ܣ���ȡ��ǰҳ�������ݵ��ַ���
	���ձ�������ı����Ӣ�ı���˳�����У��м�ͨ���ַ����������ָ���
���������
	pPageInfo��ҳ��ľ��
	buff���ַ�������Ļ�����
	pbuffsize��ָ��ָ�򻺳�����С
���������
	��buffΪ��ʱ��������Ҫ�������Ĵ�С��ͨ��ָ��pbuffsize����
	��buff��Ϊ��ʱ������ַ��������ݣ�pbuffsize��������ܳ���
�������أ�
	����������Ϊ��ʱ����������������ַ������ܸ���
	��������Ϊ�գ���ͳ���ַ����ܸ���
*/
u32 getPageTxt(PAGE_INFO *pPageInfo, u8* buff, u32 *pbuffsize);

/*
�������ܣ��ַ������ң�ͬwcsstr�������ܣ���ģʽ��Сд�͵���ƥ��

���������
	string��unicode�����ַ���
	strCharSet�������ҵ��ַ���
	mode��ƥ��ģʽ
		ȱʡSEARCH_MODE_DEFΪ��ȫƥ��ģʽ��
		SEARCH_MODE_NOCASEģʽ��������ĸ��Сд��
		SEARCH_MODE_WORDģʽҪ��ȫ��ƥ�䣻
		ע�⣺�����ȫ��ƥ��ģʽ��Ҫ�������ַ���ȥ����β�ķ���ĸ�����ַ�
�������أ�
	���ƥ���ϣ�����ƥ���ַ�����ָ�룬���򷵻ؿ�
*/
u16* ucs2strstr( const u16 *string, const u16 *strCharSet, u16 mode);

#endif
