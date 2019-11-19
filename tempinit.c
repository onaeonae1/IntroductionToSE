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

void Panel_and_Speaker_Controller() {
	if (MD.alarm_buzzing == true) {
		Alarm();
	}
	int flag1 = MD.category_alpha;
	int flag2 = MD.category_beta;
	char list[8][3]; // configure된 값들을 저장
	int blink_location = 0;
	char temp[3] = "  "; // 임시로 쓰일 저장소
		// 깜빡일 위치를 저장
		//만약 깜빡임을 구현하기 위해 깜빡일 위치를
		//표시하여야 할 경우는 0을 저장한다


	//if문들로 각각에 맞게 configure
	//configure 된 값들은 모두 2글자의 문자열이다 + 깜빡일 위치

	//alarm_indicator의 경우 공통되었으므로 미리 만들어 두었다
	if (MD.stopwatch_indicator) temp[1] = 'A';
	configure_set(list, 3, temp);
	list[7][0] = ' '; list[7][1] = ' '; list[7][2] = '\0';

	switch (flag1) {
	case 1: // Timekeeping 모드
		switch (TD.WD) {
		case 0://Sunday
			temp[0] = 'S'; temp[1] = 'U'; break;
		case 1://Monday
			temp[0] = 'M'; temp[1] = 'O'; break;
		case 2://Tuesday
			temp[0] = 'T'; temp[1] = 'U'; break;
		case 3://Wendsday
			temp[0] = 'W'; temp[1] = 'E'; break;
		case 4://Thursday
			temp[0] = 'T'; temp[1] = 'H'; break;
		case 5://Friday
			temp[0] = 'F'; temp[1] = 'R'; break;
		case 6://Saturday
			temp[0] = 'S'; temp[1] = 'A'; break;
		default: break;
		}
		temp[2] = '\0';
		configure_set(list, 0, temp);
		int_to_str(CT.MT + 1, temp);
		if (CT.MT + 1 < 10) temp[0] = ' ';
		configure_set(list, 1, temp);
		int_to_str(CT.DD, temp);
		configure_set(list, 2, temp);
		int_to_str(CT.HH, temp);
		configure_set(list, 4, temp);
		int_to_str(CT.MM, temp);
		configure_set(list, 5, temp);
		int_to_str(CT.SS, temp);
		configure_set(list, 6, temp);
		int_to_str(CT.YY - 100, temp);
		configure_set(list, 7, temp);
		switch (flag2) {
		case 1: case 5: defalut: break; // (1,1)이거나 년도를 바꿀 경우 blink_location = 0이다.
		case 2: blink_location = 7; break;// 초
		case 3: blink_location = 5; break;// 시간
		case 4: blink_location = 6; break;// 분
		case 6: blink_location = 2; break; // 월
		case 7: blink_location = 3; break;// 일
		}
		break;
	case 2: // Stopwatch 모드
		configure_set(list, 0, "ST");
		int_to_str(CT.HH, temp);
		configure_set(list, 1, temp);
		int_to_str(CT.MM, temp);
		configure_set(list, 2, temp);
		if (flag2 == 1) {//Stopwatch
			int_to_str(ST.stopwatchTime.MM, temp);
			configure_set(list, 4, temp);
			int_to_str(ST.stopwatchTime.SS, temp);
			configure_set(list, 5, temp);
			int_to_str(ST.stopwatchTime.MS, temp);
			configure_set(list, 6, temp);
		}
		if (flag2 == 2) {//LAP
			int_to_str(ST.lapTime.MM, temp);
			configure_set(list, 4, temp);
			int_to_str(ST.lapTime.SS, temp);
			configure_set(list, 5, temp);
			int_to_str(ST.lapTime.MS, temp);
			configure_set(list, 6, temp);
		}
		break;
	case 3: // Alarm 모드
		configure_set(list, 0, "AL");
		int_to_str(CT.MT + 1, temp);
		if (CT.MT + 1 < 10) temp[0] = ' ';
		configure_set(list, 1, temp);
		int_to_str(CT.DD, temp);
		configure_set(list, 2, temp);
		int_to_str(AL.alarmTime.HH, temp);
		configure_set(list, 4, temp);
		int_to_str(AL.alarmTime.MM, temp);
		configure_set(list, 5, temp);
		configure_set(list, 6, "  ");
		if (flag2 == 2) blink_location = 5; // 시간
		else if (flag2 == 3) blink_location = 6; // 분
		break;
	default: // 엄밀한 명세에 의하면 없어도 되는 코드
		break;
	}
	//show()를 구현하여 configure된 값들을 표시
	//1초에 한번씩 깜빡일 예정
	if (CT.SS % 2) blink_location = 0;
	show(flag1, list, blink_location);

	return;
}
