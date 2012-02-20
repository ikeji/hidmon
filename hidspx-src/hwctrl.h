/* Port number and port address */

#define	COM1ADR	0x03F8
#define	COM2ADR	0x02F8
#define	COM3ADR	0x03E8
#define	COM4ADR	0x02E8
#define	LPT1ADR	0x0378
#define	LPT2ADR	0x0278
#define	LPT3ADR	0x03BC


/* LPT data port offset and control bit assgnment */

#define	L_DAT	0
#define	B_SCK	0x01
#define B_MOSI	0x02
#define B_RES	0x04
#define B_ENA	0x08
#define	BX_SCK	0x02
#define BX_MOSI	0x01
#define BX_RES	0x04
#define	BX_DIS1	0x08
#define	BX_DIS2	0x10
#define	BL_SCK	0x02
#define BL_MOSI	0x01
#define BL_RES	0x04
#define	BL_DIS	0x08
#define	BA_SCK	0x01
#define	BA_MOSI	0x40
#define	BA_RES	0x02
#define	BS_SCK	0x10
#define	BS_MOSI	0x20
#define	BS_RES	0x80
#define	BS_DIS	0x0C


/* LPT status port offset and bit assignment */

#define	L_STA	1
#define S_BUSY	0x80
#define	S_ACK	0x40
#define	S_PE	0x20
#define	S_SEL	0x10
#define	S_ERR	0x08


/* LPT control port offset and bit assignment */

#define	L_CTL	2
#define	BA_ENA	0x02


/* COM port register offset and bit assignment */

#define	C_DAT	0

#define	C_IMR	1

#define	C_ISR	2

#define	C_FCR	2

#define	C_LCR	3
#define C_MOSI	0x40

#define	C_MCR	4
#define	C_RES	0x01
#define C_SCK	0x02

#define	C_LSR	5

#define	C_MSR	6
#define C_MISO	0x20

#define	C_SCR	7


/* Serial to SPI bridge escaped command */

#define	FLAG			0xFF
#define	SPI_NOP			0
#define	SPI_ENABLE		1
#define	SPI_DISABLE		2
#define	SPI_RCVR		3
#define	SPI_RCVZ		4
#define	SPI_WAIT		5
#define	SPI_RESET		6
#define	SPI_SCK			7
#define	SPI_SETDLY		8
#define	SPI_SETBIDIR	9
#define	SPI_WRPORT		10
#define	SPI_RDPORT		11
#define	SPI_WRDDR		12
#define	SPI_RDDDR		13
#define	SPI_RDPIN		14
#define	SPI_SETMODE		15
#define	SPI_PICMODE		16
#define	SPI_RDPROG		17
#define	SPI_RDEEP		18
#define	SPI_LDPAGE		19
#define	SPI_GETREV		20

