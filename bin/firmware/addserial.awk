# addserial.awk (genserial.awk で開発開始)
# Written by senshu.
#
# 2008-10-20 first release.
# 2008-10-22 HEX nubmer support.
# 2008-10-23 Add -v adr option.
# 2008-11-07 任意のスタート番地指定を可能にした

# 2009-02-12 GAINER mini対応を追加
# 2009-02-13 エラーチェックを強化
# 2009-02-22 t(type)省略時指定をgainerに変更
# 2010-02-10 t(type)省略時指定をaspxに変更

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

# 対象とするファイル名の変更箇所
# TARGET_BASE ... main-12.hex のファイルを対象にする場合、"main-12"をセットする

BEGIN {
	HEX_EOF = ":00000001FF";
	HEX_BOF = ":020000020000FC";

	if (t==0) {		# Default
		t = "aspx";
	}
	if (t == "gainer") {
		TARGET_BASE = "GAINERmini";
		TARGET_HEX  = TARGET_BASE "-ABCD.hex";
		adr = 16909;	# 0x420D
	} else if (t == "aspx") {
		TARGET_BASE = "main-12";
		TARGET_HEX  = TARGET_BASE ".hex";
		adr = 104;		# 0x68
	} else if (t == "mon88") {
		adr = 6255;		# 0x186f
	} else {
		adr = t;		# 0x68
	}
	cnt = 0;

	if (ARGC < 2) {
		printf("usage: gawk -f addserial.awk -v t={gainer|aspx|mon88} start-number  [end-number]\n");
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
		if (t == "gainer" || t == "aspx") {
			serial = substr(toupper(ARGV[1]), 1, 4);
			fname = "_" TARGET_BASE "-" serial ".hex";
			while ((r = (getline line < TARGET_HEX)) > 0) {
				if (line!=HEX_EOF) {
					print line > fname;
				}
            }
			if (r < 0) {
				printf("%s: File not exits.\n", TARGET_HEX);
				exit 1;
			}
			print hex_rec(adr, serial) > fname;
		} else {
			serial = substr(toupper(ARGV[1]), 1, 4);
			fname = "_" serial ".hex";
			print HEX_BOF > fname;
			print hex_rec(adr, serial) > fname;
		}
		print HEX_EOF > fname;
		close(TARGET_HEX);
		close(fname);
		cnt++;
	} else {
		for (j=start; j<=end; j++) {
			serial = sprintf("%04d", j);
			if (t == "gainer" || t == "aspx") {
				fname = "_" TARGET_BASE "-" serial ".hex";
				while ((r = (getline line < TARGET_HEX)) > 0) {
					if (line!=HEX_EOF) {
						print line > fname;
					}
	            }
				if (r < 0) {
					printf("%s: File not exits.\n", TARGET_HEX);
					exit 1;
				}
				print hex_rec(adr, serial) > fname;
			} else {
				fname = "_" serial ".hex";
				print HEX_BOF > fname;
				print hex_rec(adr, serial) > fname;
			}
			print HEX_EOF > fname;
			close(TARGET_HEX);
			close(fname);
			cnt++;
		}
	}
	if (cnt == 1) {
		printf("Start adr=0x%04x, %s serial file Created.\n", adr, fname);
	} else {
		printf("Start adr=0x%04x, %4d serial file(s) Created.\n", adr, cnt);
	}
}
