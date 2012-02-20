/* hidasp.h */
#define HIDASP_RST_H_GREEN	0x18
#define HIDASP_RST_H_RED	0x14
#define HIDASP_RST_L_GREEN	0x08
#define HIDASP_RST_L_BOTH	0x00

#define HIDASP_NOP 			0
#define HIDASP_TEST 		1
#define HIDASP_SET_STATUS 	2
#define HIDASP_CMD_TX 		16
#define HIDASP_CMD_TX_1 	17
#define HIDASP_SET_PAGE 	20
#define HIDASP_PAGE_TX 		22
#define HIDASP_PAGE_RD 		23
#define HIDASP_SET_DELAY 	60
#define HIDASP_POKE 		62
#define HIDASP_PEEK 		63

int hidasp_init(char *string);
void hidasp_close();
int hidasp_program_enable(int delay);
int hidasp_cmd(const unsigned char cmd[4], unsigned char res[4]);
int hidasp_page_write(long addr, const unsigned char *wd, int pagesize);
int hidasp_page_read(long addr, unsigned char *wd, int pagesize);

//
int hidWriteFile(HANDLE hHID, char *buf, int Length, ULONG * sz, void *cb);
int hidReadFile(HANDLE hHID, char *buf, int Length, ULONG * sz, void *cb);
