/* hidasp.h */
#define HIDASP_RST_H_GREEN			0x18
#define HIDASP_RST_H_RED			0x14
#define HIDASP_RST_L_GREEN			0x08
#define HIDASP_RST_L_BOTH			0x00


int hidasp_init(char *string);
void hidasp_close();
int hidasp_program_enable(int delay);
int hidasp_cmd(const unsigned char cmd[4], unsigned char res[4]);
int hidasp_page_write(long addr, const unsigned char *wd, int pagesize,int flashsize);
int hidasp_page_read(long addr, unsigned char *wd, int pagesize);

int	hidWriteBuffer(char *buf, int len);
int	hidReadBuffer(char *buf, int len);

/*

 //ˆÈ‰º‚Í #include "../firmware/hidcmd.h" ‚É‚Ä ’è‹`.

#define HIDASP_TEST           0x01
#define HIDASP_SET_STATUS     0x02
#define HIDASP_CMD_TX         0x10
#define HIDASP_CMD_TX_1       0x11
#define HIDASP_SET_PAGE       0x14
#define HIDASP_PAGE_TX        0x20
#define HIDASP_PAGE_RD        0x21
#define HIDASP_PAGE_TX_START  0x22
#define HIDASP_PAGE_TX_FLUSH  0x24
#define HIDASP_SET_DELAY      0x3c
 */
