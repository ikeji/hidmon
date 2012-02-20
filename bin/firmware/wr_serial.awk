# regserial.awk
# Written by senshu.
# 2008-10-20 first release.
# 2008-10-22 HEX nubmer support.
# 2008-10-23 Add -v adr option.
# 2008-11-07 任意のスタート番地指定を可能にした
# 2010-02-22 生成パターン指定を正規表現対応
# 2010-02-23 -v d=1 で生成する番号の確認を可能にした
# 2010-02-25 重複するシリアル番号を除外し、シリアル番号の生成と書き込みを同時に行う。

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

function wrire_avr(fname, 	cmd, r)
{
	cmd = sprintf("hidspx -qtiny2313 -d10 -fL0xFF -fH0xDB -fX0xFF");
	if (debug_mode) {
		print "(" cmd ")";
	} else {
		r = system(cmd);
		if (r != 0)
			return r;
	}

	cmd = sprintf("hidspx -qtiny2313 -d1 %s %s", FIRM_HEX, fname);

	if (debug_mode) {
		print "(" cmd ")";
	} else {
		r = system(cmd);
		return r;
	}
}

function write_firmware(fname, serial, adr, 	cmd, ret){
	print ":020000020000FC" > fname;
	print hex_rec(adr, serial) > fname;
	print ":00000001FF" > fname;
	close(fname);

	ret = -1;
	while (1) {
		printf("Please set AVR(%s)", serial);
		getline < "/dev/stdin";
		if (wrire_avr(fname)==0) {
				wr_total++;
				ret = 0;
				break;
		} else {
			continue;
		}
	}
	cmd = sprintf("del %s", fname);
	system(cmd);
	return 0;
}

function file_exist(fname, r)
{
	r = getline < fname;
	close(fname);
	return r;
}

BEGIN {
# global variables
	PROGNAME = "wr_serial.awk";
	LOG_FILE = "used_log.csv";
	FIRM_HEX = "main-12.hex";

	chars = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	chars_len = length(chars);
	wr_total = 0;
	skip_total = 0;
	total = 0;
	debug_mode = d;

	if (t == "aspx" || t==0) {
		adr = 104;	# 0x68
	} else if (t == "mon88") {
		adr = 6255;	# 0x186f
	} else {
		adr = t;	# 0x68
	}

	if (ARGC < 2) {
		printf("usage: gawk -f %s -v t={aspx|mon88} {-v d=1} Reg-pattern\n", PROGNAME);
		exit 1;
	}

	if (file_exist(FIRM_HEX) < 0) {
		printf("%s isn't found.\n", FIRM_HEX);
		exit 1;
	}

	hint = toupper(ARGV[1]);
	if (length(hint) < 4) {
		print "reg_exp is too short.";
		exit 1;
	}

	# match reg-exp
	while ((getline line < LOG_FILE) > 0) {
		if (split(line, field, ",") == 2) {
#			printf("[%s]\n", substr(field[1], 2));
			used[substr(field[1], 2)] = 1;
			total++;
		}
	}

	if (index(hint, "[")==0 && index(hint, "]")==0 ) {
		# no match reg-exp
		fname = "_" hint ".hex";
		serial = hint;
		if (serial in used) {
			print "skip #" serial;
			skip_total++;
		} else {
			fname = "_" serial ".hex";
			if (write_firmware(fname, serial, adr) >= 0) {
				printf("#%s, %s\n", serial, strftime("%Y-%m-%d %H:%M:%S")) >> LOG_FILE;
			}
		}
	} else {
		for (i=1; i<=chars_len; i++) {
			for (j=1; j<=chars_len; j++) {
				for (k=1; k<=10; k++) {
					for (l=1; l<=10; l++) {
						serial = \
							substr(chars, i, 1) \
							substr(chars, j, 1) \
							substr(chars, k, 1) \
							substr(chars, l, 1) ;

#						print serial "::" hint;
						if (serial ~ hint) {
							if (serial in used) {
								print "skip #" serial;
								skip_total++;
							} else {
								fname = "_" serial ".hex";
								if (write_firmware(fname, serial, adr) >= 0) {
									printf("#%s, %s\n", serial, strftime("%Y-%m-%d %H:%M:%S")) >> LOG_FILE;
								}
							}
						}
					}
				}
			}
		}
	}
	close(LOG_FILE);
	total += wr_total;
	if (debug_mode) {
		printf("Start adr=0x%04x, Total = %4d, Write = %4d, skip = %d.\n",  adr, total, wr_total, skip_total);
	} else {
		printf("total = %4d, Write = %4d, skip = %d.\n",  total, wr_total, skip_total);
	}

	# sort LOG_FILE
	system(sprintf("sort %s > _%s_", LOG_FILE, LOG_FILE));
	system(sprintf("del %s", LOG_FILE));
	system(sprintf("ren _%s_ %s", LOG_FILE, LOG_FILE));
}

