void init() { //초기화. 프로그램 첫 실행시에 호출됨. commit 해야 함, ST.initialTime.XX가 커밋되지 않음
	//커서 설정
	 HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	 CONSOLE_CURSOR_INFO info;
	 info.dwSize = 100;
	 info.bVisible = FALSE;
	 SetConsoleCursorInfo(consoleHandle, &info);
	//절대 시간 선언
	struct timeb itb;
	struct tm *now;
	time_t ltime;
	int milisec;
	ftime(&itb);
	ltime = itb.time;
	milisec = itb.millitm;
	now = localtime(&ltime);
	//시간 초기화
	CT.YY = 19, CT.MT = 1, CT.DD = 1, CT.HH = 0, CT.MM = 0, CT.SS = 0, CT.MS = 0, CT.WD = -1; // 요일 수정 필요
	TD.YY = now->tm_year - 100 - 19, TD.MT = now->tm_mon + 1 - 1, TD.DD = now->tm_mday - 1, TD.HH = now->tm_hour, TD.MM = now->tm_min, TD.SS = now->tm_sec, TD.MS = milisec, TD.WD = -1;
	//알람 초기화
	AL.alarmTime.YY = 0, AL.alarmTime.MT = 0, AL.alarmTime.DD = 0, AL.alarmTime.HH = 0, AL.alarmTime.MM = 0, AL.alarmTime.SS = 0, AL.alarmTime.MS = 0, AL.alarmTime.WD = -1;
	//스톱워치
	ST.stopwatchTime.YY = 0, ST.stopwatchTime.MT = 0, ST.stopwatchTime.DD = 0, ST.stopwatchTime.HH = 0, ST.stopwatchTime.MM, ST.stopwatchTime.SS = 0, ST.stopwatchTime.MS = 0, ST.stopwatchTime.WD = -1;
	ST.startTime.YY = 0, ST.startTime.MT = 0, ST.startTime.DD = 0, ST.startTime.HH = 0, ST.startTime.MM = 0, ST.startTime.SS = 0, ST.startTime.MS = 0, ST.startTime.WD = -1;
	ST.lapTime.YY = 0, ST.lapTime.MT = 0, ST.lapTime.DD = 0, ST.lapTime.HH = 0, ST.lapTime.MM = 0, ST.lapTime.SS = 0, ST.lapTime.MS = 0, ST.lapTime.WD = -1;
	ST.initialTime.YY = 0, ST.initialTime.MT = 0, ST.initialTime.DD = 0, ST.initialTime.HH = 0, ST.initialTime.MM = 0, ST.initialTime.SS = 0, ST.initialTime.MS = 0, ST.initialTime.WD = -1; // commint 안 됨
	//백라이트
	BC.value = COLOR_DEF;
	Backlight_Controller(BC.value);
	BC.BacklightTime.YY = 0, BC.BacklightTime.MT = 0, BC.BacklightTime.DD = 0, BC.BacklightTime.HH = 0, BC.BacklightTime.MM = 0, BC.BacklightTime.SS = 0, BC.BacklightTime.MS = 0, BC.BacklightTime.WD = -1; // commint 안 됨
	//모드 초기화
	MD.alarm_buzzing = false, MD.alarm_indicator = false, MD.stopwatch_indicator = false;
	MD.category_alpha = 1, MD.category_beta = 1;
	//백라이트 초기화
	
	
	
	//show에 관한 초기화
	printf("        ####################\n");
	printf("     ###                    ###\n");
	printf("  ###                          ###\n");
	printf("##                                ##\n");
	printf("##                                ##\n");
	printf("##                                ##\n");
	printf("  ###                          ###\n");
	printf("     ###                    ###\n");
	printf("        ####################\n");

}


void gotoxy(int x, int y) {

	printf("\033[%d;%df", y, x);

	fflush(stdout);

}

void show(int alpha_cat, char list[8][3], int blink_location) {
	//alpha_cat는 대분류를 뜻합니다. 대분류에 따라 표시될 화면의 구성이 조금씩 다릅니다.
	//list는 7개의 위치에 해당하는 문자열입니다. 모두 두 글자로 이루어져 있습니다.
	//blink_location은 깜빡일 화면의 위치를 뜻합니다.(0에서 8의 값을 가집니다)
	//blink_location이 0일 경우는 모든 화면을 표시합니다. Panel_and_Speaker_Controller에서
	//시간의 경과를 판별한 뒤, %연산(을 이용할 계획)을 이용하여 깜빡임을 구현합니다.(0과 위치를 적절히 전달)
	//결론은, blink_location이 0이면 모든 위치의 화면을 표시,
	//그렇지 않으면 지정된 위치(blink_location)의 화면을 표시하지 않습니다.
	if (blink_location) {
		list[blink_location - 1][0] = ' ';
		list[blink_location - 1][1] = ' ';
	}

	if (alpha_cat == 1) {//Tikekeeping
		gotoxy(12, 3); printf("%s    %s-%s/%s", list[0], list[7], list[1], list[2]);
		gotoxy(7, 5); printf("%s", list[3]);
		gotoxy(13, 6); printf("%s : %s . %s", list[4], list[5], list[6]);
		/*
		printf("        ####################\n");
		printf("     ###                    ###\n");
		printf("  ###      %s       %s/%s      ###\n", list[0], list[1], list[2]);
		printf("##                                ##\n");
		printf("##    %s                          ##\n", list[3]);
		printf("##          %s : %s . %s          ##\n", list[4], list[5], list[6]);
		printf("  ###                          ###\n");
		printf("     ###                    ###\n");
		printf("        ####################\n");
		*/
	}
	else if (alpha_cat == 2) {//Stopwatch
		gotoxy(12, 3); printf("%s       %s:%s", list[0], list[1], list[2]);
		gotoxy(7, 5); printf("%s", list[3]);
		gotoxy(13, 6); printf("%s\'  %s\"  %s", list[4], list[5], list[6]);
		/*
		printf("        ####################\n");
		printf("     ###                    ###\n");
		printf("  ###      %s       %s:%s      ###\n", list[0], list[1], list[2]);
		printf("##                                ##\n");
		printf("##    %s                          ##\n", list[3]);
		printf("##          %s\'  %s\"  %s          ##\n", list[4], list[5], list[6]);
		printf("  ###                          ###\n");
		printf("     ###                    ###\n");
		printf("        ####################\n");
		*/
	}
	else if (alpha_cat == 3) {//Alarm
		gotoxy(12, 3); printf("%s       %s/%s", list[0], list[1], list[2]);
		gotoxy(7, 5); printf("%s", list[3]);
		gotoxy(13, 6); printf("%s : %s", list[4], list[5]);
		/*
		printf("        ####################\n");
		printf("     ###                    ###\n");
		printf("  ###      %s       %s/%s      ###\n", list[0], list[1], list[2]);
		printf("##                                ##\n");
		printf("##    %s                          ##\n", list[3]);
		printf("##          %s : %s               ##\n", list[4], list[5]);
		printf("  ###                          ###\n");
		printf("     ###                    ###\n");
		printf("        ####################\n");
		*/
	}
}
