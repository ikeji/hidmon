# regserial.awk
# Written by senshu.
# 2008-10-20 first release.
# 2008-10-22 HEX nubmer support.
# 2008-10-23 Add -v adr option.
# 2008-11-07 任意のスタート番地指定を可能にした
# 2010-02-22 生成パターン指定を正規表現対応
# 2010-02-23 -v d=1 で生成する番号の確認を可能にした

function hex_rec(adr, data,		i, ch, cksum, s){
#		:08 0068 00 30 00 31 00 32 00 33 00 CA
	cksum = 8 + int(adr/256) + adr%256;
	s = sprintf(":08%04X00", adr);
	for (i=1; i<=4; i++) {
		ch = substr(data, i, 1);
		if ((0 <= ch) && (ch <=9)) {
			ch = ch + 48;	# 48=0x30
		} else {
			ch = index("ABCDEFGHIJKLMNOPQRSTUVWXYZ", ch);
			if (ch == 0) {
				printf("Serial number '%s' Format error.\n", data);
				exit 1;
			}
			ch += 64;
		}
		s = s sprintf("%02X00", ch);
		cksum += ch;
	}
	cksum = cksum % 256;
	if (cksum != 0) {
		cksum = 256 - cksum;
	}
	s = s sprintf("%02X", cksum);
	return s;
}

function write_hex(fname, serial, adr){
	print ":020000020000FC" > fname;
	print hex_rec(adr, serial) > fname;
	print ":00000001FF" > fname;
	close(fname);
}

BEGIN {
	chars = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	chars_len = length(chars);
	PROGNAME = "regserial.awk";

	if (t == "aspx" || t==0) {
		adr = 104;	# 0x68
	} else if (t == "mon88") {
		adr = 6255;	# 0x186f
	} else {
		adr = t;	# 0x68
	}
	cnt = 0;

	if (ARGC < 2) {
		printf("usage: gawk -f %s -v t={aspx|mon88} {-v d=1} Reg-pattern\n", PROGNAME);
		exit 1;
	}

	hint = toupper(ARGV[1]);
	if (length(hint) < 4) {
		print "reg_exp is too short.";
		exit 1;
	} else if (index(hint, "[")==0 && index(hint, "]")==0 ) {
		# no match reg-exp
		fname = "_" hint ".hex";
		if (d != 0) {
			printf("%s: %s\n",  PROGNAME, fname);
		} else {
			write_hex(fname, hint, adr);
		}
		cnt++;
	} else {
		# match reg-exp
		for (i=1; i<=chars_len; i++) {
			for (j=1; j<=chars_len; j++) {
				for (k=1; k<=10; k++) {
					for (l=1; l<=10; l++) {
						serial = substr(chars, i, 1) substr(chars, j, 1) substr(chars, k, 1) substr(chars, l, 1) ;
						if (serial ~ hint) {
							fname = "_" serial ".hex";
							if (d != 0) {
								printf("%s: %s\n",  PROGNAME, fname);
							} else {
								write_hex(fname, serial, adr);
							}
							cnt++;
						}
					}
				}
			}
		}
	}
	if (d != 0) {
		printf("Start adr=0x%04x, %4d generated.\n", adr, cnt);
	} else {
		if (cnt == 1) {
			printf("Start adr=0x%04x, %s file was generated.\n", adr, fname);
		} else {
			printf("Start adr=0x%04x, Created %4d files.\n", adr, cnt);
		}
	}
}
