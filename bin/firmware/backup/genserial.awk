# genserial.awk
# Written by senshu.
# 2008-10-20 first release.
# 2008-10-22 HEX nubmer support.
# 2008-10-23 Add -v adr option.
# 2008-11-07 任意のスタート番地指定を可能にした

function hex_rec(adr, data){
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

BEGIN {
	if (t == "aspx" || t==0) {
		adr = 104;	# 0x68
	} else if (t == "mon88") {
		adr = 6255;	# 0x186f
	} else {
		adr = t;	# 0x68
	}
	cnt = 0;

	if (ARGC < 2) {
		printf("usage: gawk -f genserial.awk -v t={aspx|mon88} start-number  [end-number]\n");
		exit 1;
	}
	start = ARGV[1] + 0;
	if (ARGC == 2) {
		end   = ARGV[1] + 0;
	} else {
		end   = ARGV[2] + 0;
	}
	if ((start > end) || (end >= 10000)) {
		printf("Serial number range error.\n");
		exit 1;
	}
	if (start==end) {
		if (length(ARGV[1]) != 4) {
			printf("Serial number length error.\n");
			exit 1;
		}
		basename = substr(toupper(ARGV[1]), 1, 4);
		fname = "_" basename ".hex";
		print ":020000020000FC" > fname;
		print hex_rec(adr, basename) > fname;
		print ":00000001FF" > fname;
		close(fname);
		cnt++;
	} else {
		for (j=start; j<=end; j++) {
			basename = sprintf("%04d", j);
			fname = "_" basename ".hex";
			print ":020000020000FC" > fname;
			print hex_rec(adr, basename) > fname;
			print ":00000001FF" > fname;
			close(fname);
			cnt++;
		}
	}
	if (cnt == 1) {
		printf("Start adr=0x%04x, %s file was generated.\n", adr, fname);
	} else {
		printf("Start adr=0x%04x, Created %4d files.\n", adr, cnt);
	}
}
