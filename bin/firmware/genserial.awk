BEGIN {
	if (ARGC != 2) {
		printf("usage gawk -f genserial.awk XXXX\n");
		exit 1;
	}
	if (length(ARGV[1]) != 4) {
		printf("serial nber is not 4 charactors.\n");
		exit 1;
	} else {
		n = ARGV[1];
		out_file = "__serial.s";
		print("\t.cseg\n") > out_file;
		print("\t.org $34\n") > out_file;
		print("\t.db '" substr(n,1,1) "',0,'" substr(n,2,1) "',0,'" substr(n,3,1) "',0,'" substr(n,4,1) "',0") > out_file;
		print("\t.exit\n") > out_file;
		close(out_file);

		avrasm = "avrasm2.exe";
		cmd = sprintf("%s -fI %s", avrasm, out_file);
		system(cmd);
		print "__serial.hex is Created.";
	}
}
