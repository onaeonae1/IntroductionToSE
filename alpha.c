#include"util.h"
//Data Store들 선언하기
alm AL; //알람
stopwatch ST; //스톱워치
mode MD; //모드
Time CT; //현재 시간
Time TD; //절대 시간과의 차이
backlight BC;
//0 : Alarm Buzzing, 1 : 대분류, 2: 소분류 , 3 : Stopwatch_Indicator, 4 : Alarm indicator
Bool buttonA_interface(char input){
	if(input=='a' || input =='A'){
		//printf("Button A Selected \n");
		return true;
	}
	return false;
}
Bool buttonB_interface(char input){
	if(input=='b' || input=='B'){
		//printf("Button B Selected \n");
		return true;
	}
	return false;
}
Bool buttonC_interface(char input){
	if(input=='c' || input=='C'){
		//printf("Button C Selected \n");
		return true;
	}
	return false;
}
Bool buttonD_interface(char input){
	if(input=='d' || input=='D'){
		//printf("Button D Selected \n");
		return true;
	}
	return false;
}
void Alarm(){ //소리 울리기
	printf("\a");
}
void init() { //초기화. 프로그램 첫 실행시에 호출됨.
	//콘솔 창 설정
	SetConsoleTitle("DWS for Team #1");
	//마우스 커서 숨기기
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
	CT.YY = 25, CT.MT = 1, CT.DD = 1, CT.HH = 0, CT.MM = 0, CT.SS = 0, CT.MS = 0, CT.WD = -1; // 요일 수정 필요
	TD.YY = now->tm_year - 100 - 19, TD.MT = now->tm_mon + 1 - 1, TD.DD = now->tm_mday - 1, TD.HH = now->tm_hour, TD.MM = now->tm_min, TD.SS = now->tm_sec, TD.MS = milisec, TD.WD = -1;
	//알람 초기화
	AL.alarmTime.YY = 0, AL.alarmTime.MT = 0, AL.alarmTime.DD = 0, AL.alarmTime.HH = 0, AL.alarmTime.MM = 0, AL.alarmTime.SS = 0, AL.alarmTime.MS = 0, AL.alarmTime.WD = -1;
	AL.snooze = true;
	AL.ring = 0;
	//스톱워치
	ST.stopwatchTime.YY = 0, ST.stopwatchTime.MT = 0, ST.stopwatchTime.DD = 0, ST.stopwatchTime.HH = 0, ST.stopwatchTime.MM, ST.stopwatchTime.SS = 0, ST.stopwatchTime.MS = 0, ST.stopwatchTime.WD = -1;
	ST.startTime.YY = 0, ST.startTime.MT = 0, ST.startTime.DD = 0, ST.startTime.HH = 0, ST.startTime.MM = 0, ST.startTime.SS = 0, ST.startTime.MS = 0, ST.startTime.WD = -1;
	ST.lapTime.YY = 0, ST.lapTime.MT = 0, ST.lapTime.DD = 0, ST.lapTime.HH = 0, ST.lapTime.MM = 0, ST.lapTime.SS = 0, ST.lapTime.MS = 0, ST.lapTime.WD = -1;
	ST.initialTime.YY = 0, ST.initialTime.MT = 0, ST.initialTime.DD = 0, ST.initialTime.HH = 0, ST.initialTime.MM = 0, ST.initialTime.SS = 0, ST.initialTime.MS = 0, ST.initialTime.WD = -1; // commint 안 됨
	//show에 관한 초기화
	SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), 9);
	printf("        ####################\n");
	printf("     ###                    ###\n");
	printf("  ###                          ###\n");
	printf("##                                ##\n");
	printf("##                                ##\n");
	printf("##                                ##\n");
	printf("  ###                          ###\n");
	printf("     ###                    ###\n");
	printf("        ####################\n");
	//백라이트
	BC.value = COLOR_DEF;
	Backlight_Controller(BC.value);
	BC.BacklightTime.YY = 0, BC.BacklightTime.MT = 0, BC.BacklightTime.DD = 0, BC.BacklightTime.HH = 0, BC.BacklightTime.MM = 0, BC.BacklightTime.SS = 0, BC.BacklightTime.MS = 0, BC.BacklightTime.WD = -1; // commint 안 됨
	//모드 초기화
	MD.alarm_buzzing = false, MD.alarm_indicator = false, MD.stopwatch_indicator = false;
	MD.category_alpha = 1, MD.category_beta = 1;

}
int Button_Selector() {
	//sleep(1);
	int Selected_Button = 0;
	Bool isA = false, isB = false, isC = false, isD = false;
	int value=-1;
	while (value = linux_kbhit()) { //no button인 경우 이곳을 거치지 않는다.
		isA = false; isB = false; isC = false; isD = false;
		char temp = getch();
		isD = buttonD_interface(temp);
		if(isD){
			Selected_Button = 4;
			break;
		}
		isC = buttonC_interface(temp);
		if(isC){
			Selected_Button = 3;
			break;
		}
		isB = buttonB_interface(temp);
		if(isB){
			Selected_Button = 2;
			break;
		}
		isA = buttonA_interface(temp);
		if(isA){
			Selected_Button  =1;
			break;
		}
		else{
			break;
		}
		if (isD) Selected_Button = 4;
		else if (isC) Selected_Button = 3;
		else if (isB) Selected_Button = 2;
		else if (isA) Selected_Button = 1;
	}
	//Selected Button : 0 = No button, 1 = A, 2 = B, 3 = C, 4 = D
	return Selected_Button;
}
void Mode_Changer(int location, int change){ //MD를 수정할 수 있는 함수, 0~4의 바꿀 위치와 바꿀 값을 인자로 받는다
	//0 : Alarm Buzzing, 1 : 대분류, 2: 소분류 , 3 : Stopwatch_Indicator, 4 : Alarm indicator
	switch(location) {
	case 0:
		MD.alarm_buzzing = change;
		break;
	case 1:
		MD.category_alpha = change;
		break;
	case 2:
		MD.category_beta = change;
		break;
	case 3:
		MD.stopwatch_indicator = change;
		break;
	case 4:
		MD.alarm_indicator = change;
	}
}
int Button_Operator(int Selected_Button) {
	Bool alarm_buzzing = MD.alarm_buzzing;
	int category_alpha = MD.category_alpha;
	int category_beta = MD.category_beta;
	Bool stopwatch_indicator = MD.stopwatch_indicator;
	Bool alarm_indicator = MD.alarm_indicator;

	// 모드의 대분류(category_alpha)-소분류(category_beta)-Selected_Button 순서로 작성
	//알람 울리기가 최우선

	// 버튼이 눌리지 않으면 5초 동안 가만히 있다가 스스로 종료(RTM)
	// 알람을 끔
	if(!alarm_buzzing) {
		if (category_alpha == 1) {
			switch (category_beta) {
			case 1: // 1.1 timekeeping
				switch (Selected_Button) {
				case 1: // A
					Mode_Changer(2, 2);
					break;
				case 2: // B
					break;
				case 3: // C
					Mode_Changer(1, 2);
					Mode_Changer(2, 1);
					break;
				case 4: // D, D버튼이 들어오면 BacklightTime을 현재 시간으로 한다. 나머지 역할은 RTM이 한다.
					BC.value = COLOR_GRN;
					BC.BacklightTime.YY = CT.YY;
					BC.BacklightTime.MT = CT.MT;
					BC.BacklightTime.DD = CT.DD;
					BC.BacklightTime.HH = CT.HH;
					BC.BacklightTime.MM = CT.MM;
					BC.BacklightTime.SS = CT.SS + 2;
					BC.BacklightTime.MS = CT.MS;
					Time* prev = &(BC.BacklightTime);
					BC.BacklightTime = timeCheck(prev);
					break;
				default: break;
				}
				break;
			case 2: // 1.2 timekeeping_change_sec
				switch (Selected_Button) {
				case 1: // A
					Mode_Changer(2, 1);
					break;
				case 2: // B
					if (CT.SS == 59) TD.SS += 59; // 최대치가 된 상태에서 다시 입력하면 최저값으로
					else TD.SS--; // 현재 시각 초 1 증가
					// stopwatch 시작 초도 같이 늘려주기
					if (ST.startTime.SS == 59) {
						ST.startTime.SS = 0;
						ST.startTime.MM++;
					}
					else ST.startTime.SS++;
					break;
				case 3: // C
					Mode_Changer(2, 3);
					break;
				case 4: // D, D버튼이 들어오면 BacklightTime을 현재 시간으로 한다. 나머지 역할은 RTM이 한다.
					BC.value = COLOR_GRN;
					BC.BacklightTime.YY = CT.YY;
					BC.BacklightTime.MT = CT.MT;
					BC.BacklightTime.DD = CT.DD;
					BC.BacklightTime.HH = CT.HH;
					BC.BacklightTime.MM = CT.MM;
					BC.BacklightTime.SS = CT.SS + 2;
					BC.BacklightTime.MS = CT.MS;
					Time* prev = &(BC.BacklightTime);
					BC.BacklightTime = timeCheck(prev);
					break;
				default: break;
				}
				break;
			case 3: // 1.3 timekeeping_change_hr
				switch (Selected_Button) {
				case 1: // A
					Mode_Changer(2, 1);
					break;
				case 2: // B
					if (CT.HH == 23) TD.HH += 23; // 최대치가 된 상태에서 다시 입력하면 최저값으로
					else TD.HH--; // 현재 시각 시간 1 증가
					// stopwatch 시작 시간도 같이 늘려주기
					if (ST.startTime.HH == 23) {
						ST.startTime.HH = 0;
						ST.startTime.DD++;
					}
					else ST.startTime.HH++;
					break;
				case 3: // C
					Mode_Changer(2, 4);
					break;
				case 4: // D, D버튼이 들어오면 BacklightTime을 현재 시간으로 한다. 나머지 역할은 RTM이 한다.
					BC.value = COLOR_GRN;
					BC.BacklightTime.YY = CT.YY;
					BC.BacklightTime.MT = CT.MT;
					BC.BacklightTime.DD = CT.DD;
					BC.BacklightTime.HH = CT.HH;
					BC.BacklightTime.MM = CT.MM;
					BC.BacklightTime.SS = CT.SS + 2;
					BC.BacklightTime.MS = CT.MS;
					Time* prev = &(BC.BacklightTime);
					BC.BacklightTime = timeCheck(prev);
					break;
				default: break;
				}
				break;
			case 4: // 1.4 timekeeping_change_min
				switch (Selected_Button) {
				case 1: // A
					Mode_Changer(2, 1);
					break;
				case 2: // B
					if (CT.MM == 59) TD.MM += 59; // 최대치가 된 상태에서 다시 입력하면 최저값으로
					else TD.MM--; // 현재 시각 분 1 증가
					// stopwatch 시작 분도 같이 늘려주기
					if (ST.startTime.MM == 59) {
						ST.startTime.MM = 0;
						ST.startTime.HH++;
					}
					else ST.startTime.MM++;
					break;
				case 3: // C
					Mode_Changer(2, 5);
					break;
				case 4: // D, D버튼이 들어오면 BacklightTime을 현재 시간으로 한다. 나머지 역할은 RTM이 한다.
					BC.value = COLOR_GRN;
					BC.BacklightTime.YY = CT.YY;
					BC.BacklightTime.MT = CT.MT;
					BC.BacklightTime.DD = CT.DD;
					BC.BacklightTime.HH = CT.HH;
					BC.BacklightTime.MM = CT.MM;
					BC.BacklightTime.SS = CT.SS + 2;
					BC.BacklightTime.MS = CT.MS;
					Time* prev = &(BC.BacklightTime);
					BC.BacklightTime = timeCheck(prev);
				default: break;
				}
				break;
			case 5: // 1.5 timekeeping_change_yr
				switch (Selected_Button) {
				case 1: // A
					Mode_Changer(2, 1);
					break;
				case 2: // B
					if (CT.YY == 99) TD.YY += 80; // 최대치가 된 상태에서 다시 입력하면 최저값으로
					else TD.YY--; // 현재 시각 년 1 증가
					// stopwatch 시작 년도 같이 늘려주기
					if (ST.startTime.YY == 99) ST.startTime.YY = 19;
					else ST.startTime.YY++;
					break;
				case 3: // C
					Mode_Changer(2, 6);
					break;
				case 4: // D, D버튼이 들어오면 BacklightTime을 현재 시간으로 한다. 나머지 역할은 RTM이 한다.
					BC.value = COLOR_GRN;
					BC.BacklightTime.YY = CT.YY;
					BC.BacklightTime.MT = CT.MT;
					BC.BacklightTime.DD = CT.DD;
					BC.BacklightTime.HH = CT.HH;
					BC.BacklightTime.MM = CT.MM;
					BC.BacklightTime.SS = CT.SS + 2;
					BC.BacklightTime.MS = CT.MS;
					Time* prev = &(BC.BacklightTime);
					BC.BacklightTime = timeCheck(prev);
				default: break;
				}
				break;
			case 6: // 1.6 timekeeping_change_mnth
				switch (Selected_Button) {
				case 1: // A
					Mode_Changer(2, 1);
					break;
				case 2: // B
					if (CT.MT == 12) TD.MT += 12; // 최대치가 된 상태에서 다시 입력하면 최저값으로
					else TD.MT--; // 현재 시각 달 1 증가
					// stopwatch 시작 월도 같이 늘려주기
					if (ST.startTime.MT == 12) {
						ST.startTime.MT = 0;
						ST.startTime.YY++;
					}
					else ST.startTime.MT++;
					break;
				case 3: // C
					Mode_Changer(2, 7);
					break;
				case 4: // D, D버튼이 들어오면 BacklightTime을 현재 시간으로 한다. 나머지 역할은 RTM이 한다.
					BC.value = COLOR_GRN;
					BC.BacklightTime.YY = CT.YY;
					BC.BacklightTime.MT = CT.MT;
					BC.BacklightTime.DD = CT.DD;
					BC.BacklightTime.HH = CT.HH;
					BC.BacklightTime.MM = CT.MM;
					BC.BacklightTime.SS = CT.SS + 2;
					BC.BacklightTime.MS = CT.MS;
					Time* prev = &(BC.BacklightTime);
					BC.BacklightTime = timeCheck(prev);
				default: break;
				}
				break;
			case 7: // 1.7 timekeeping_change_day
				switch (Selected_Button) {
				case 1: // A
					Mode_Changer(2, 1);
					break;
				case 2: // B
					// 최대치가 된 상태에서 다시 입력하면 최저값으로
					switch (CT.MT) {
						// 한 달에 31일이 있는 경우
					case 1:
					case 3:
					case 5:
					case 7:
					case 8:
					case 10:
					case 12:
						if (CT.DD == 31) TD.DD += 31; // 최대치가 된 상태에서 다시 입력하면 최저값으로
						else TD.DD--;
						// stopwatch 시작 일도 같이 늘려주기
						if (ST.startTime.DD == 31) {
							ST.startTime.DD = 1;
							ST.startTime.MT++;
						}
						else ST.startTime.DD++;
						break;
						// 한 달에 28일이 있는 경우
					case 2:
						if (CT.YY % 4 == 0) { // 윤년이면
							if (CT.DD == 29) TD.DD += 29; // 최대치가 된 상태에서 다시 입력하면 최저값으로
							else TD.DD--;
						}
						else { // 윤년이 아닌 경우에는
							if (CT.DD == 28) TD.DD += 28;
							else TD.DD--;
						}
						// stopwatch 시작 일도 같이 늘려주기
						if (ST.startTime.YY % 4 == 0) { // stopwatch가 윤년이면
							if (ST.startTime.DD == 29) {
								ST.startTime.DD = 1;
								ST.startTime.MT++;
							}
							else ST.startTime.DD++;
						}
						else { // stopwatch 설정 연도가 윤년이 아닌 경우에는
							if (ST.startTime.DD == 28) {
								ST.startTime.DD = 1;
								ST.startTime.MT++;
							}
							else ST.startTime.DD++;
						}
						break;
						// 한 달에 30일이 있는 경우
					case 4:
					case 6:
					case 9:
					case 11:
						if (CT.DD == 30) TD.DD += 30; // 최대치가 된 상태에서 다시 입력하면 최저값으로
						else TD.DD--;
						// stopwatch 시작 일도 같이 늘려주기
						if (ST.startTime.DD == 30) {
							ST.startTime.DD = 1;
							ST.startTime.MT++;
						}
						else ST.startTime.DD++;
						break;
					}
					break;
				case 3: // C
					Mode_Changer(2, 2);
					break;
				case 4: // D, D버튼이 들어오면 BacklightTime을 현재 시간으로 한다. 나머지 역할은 RTM이 한다.
					BC.value = COLOR_GRN;
					BC.BacklightTime.YY = CT.YY;
					BC.BacklightTime.MT = CT.MT;
					BC.BacklightTime.DD = CT.DD;
					BC.BacklightTime.HH = CT.HH;
					BC.BacklightTime.MM = CT.MM;
					BC.BacklightTime.SS = CT.SS + 2;
					BC.BacklightTime.MS = CT.MS;
					Time* prev = &(BC.BacklightTime);
					BC.BacklightTime = timeCheck(prev);
					break;
				default: break;
				}
				break;
			default: break;
			}
		}
		else if (category_alpha == 2) {
			switch (category_beta) {
			case 1: // 2.1 stopwatch
				switch (Selected_Button) {
				case 1: // A
					if (stopwatch_indicator == 0) {
						// ST.stopwatchTime = 0을 해줌
						ST.stopwatchTime.YY = 0;
						ST.stopwatchTime.MT = 0;
						ST.stopwatchTime.DD = 0;
						ST.stopwatchTime.HH = 0;
						ST.stopwatchTime.MM = 0;
						ST.stopwatchTime.SS = 0;
						ST.stopwatchTime.MS = 0;
						// ST.startTime = 0을 해줌
						ST.startTime.YY = 0;
						ST.startTime.MT = 0;
						ST.startTime.DD = 0;
						ST.startTime.HH = 0;
						ST.startTime.MM = 0;
						ST.startTime.SS = 0;
						ST.startTime.MS = 0;
						// ST.initialTime = 0을 해줌
						ST.initialTime.YY = 0;
						ST.initialTime.MT = 0;
						ST.initialTime.DD = 0;
						ST.initialTime.HH = 0;
						ST.initialTime.MM = 0;
						ST.initialTime.SS = 0;
						ST.initialTime.MS = 0;
						// ST.lapTime = 0을 해줌
						ST.lapTime.YY = 0;
						ST.lapTime.MT = 0;
						ST.lapTime.DD = 0;
						ST.lapTime.HH = 0;
						ST.lapTime.MM = 0;
						ST.lapTime.SS = 0;
						ST.lapTime.MS = 0;
					}
					else if (stopwatch_indicator == 1) {
						ST.lapTime = ST.stopwatchTime;
						Mode_Changer(1, 2);
						Mode_Changer(2, 2);
					}
					break;
				case 2: // B
					if (stopwatch_indicator == 0) {
						Mode_Changer(3, 1);
						//stopwatchTime = CT - startTime + initialtime; (이 작업은 RTM에서 함)
						ST.stopwatchTime.YY = 0;
						ST.stopwatchTime.MT = 0;
						ST.stopwatchTime.DD = 0;
						ST.stopwatchTime.HH = 0;
						ST.stopwatchTime.MM = 0;
						ST.stopwatchTime.SS = 0;
						ST.stopwatchTime.MS = 0;

						ST.startTime.YY = CT.YY;
						ST.startTime.MT = CT.MT;
						ST.startTime.DD = CT.DD;
						ST.startTime.HH = CT.HH;
						ST.startTime.MM = CT.MM;
						ST.startTime.SS = CT.SS;
						ST.startTime.MS = CT.MS;
					}
					else if (stopwatch_indicator == 1) {
						Mode_Changer(3, 0);
						// ST.initialTime = ST.stopwatchTime;
						ST.initialTime.YY = ST.stopwatchTime.YY;
						ST.initialTime.MT = ST.stopwatchTime.MT;
						ST.initialTime.DD = ST.stopwatchTime.DD;
						ST.initialTime.HH = ST.stopwatchTime.HH;
						ST.initialTime.MM = ST.stopwatchTime.MM;
						ST.initialTime.SS = ST.stopwatchTime.SS;
						ST.initialTime.MS = ST.stopwatchTime.MS;
					}
					break;
				case 3: // C
					Mode_Changer(1, 3);
					Mode_Changer(2, 1);
					break;
				case 4: // D, D버튼이 들어오면 BacklightTime을 현재 시간으로 한다. 나머지 역할은 RTM이 한다.
					BC.value = COLOR_GRN;
					BC.BacklightTime.YY = CT.YY;
					BC.BacklightTime.MT = CT.MT;
					BC.BacklightTime.DD = CT.DD;
					BC.BacklightTime.HH = CT.HH;
					BC.BacklightTime.MM = CT.MM;
					BC.BacklightTime.SS = CT.SS + 2;
					BC.BacklightTime.MS = CT.MS;
					Time* prev = &(BC.BacklightTime);
					BC.BacklightTime = timeCheck(prev);
					break;
				default: break;
				}
				break;
			case 2: // 2.2 stopwatch_LAP
				switch (Selected_Button) {
				case 1: // A
					ST.lapTime.YY = ST.stopwatchTime.YY;
					ST.lapTime.MT = ST.stopwatchTime.MT;
					ST.lapTime.DD = ST.stopwatchTime.DD;
					ST.lapTime.HH = ST.stopwatchTime.HH;
					ST.lapTime.MM = ST.stopwatchTime.MM;
					ST.lapTime.SS = ST.stopwatchTime.SS;
					ST.lapTime.MS = ST.stopwatchTime.MS;
					break;
				case 2: // B
					Mode_Changer(2, 1);
					break;
				case 3: // C
					Mode_Changer(1, 3);
					Mode_Changer(2, 1);
					break;
				case 4: // D, D버튼이 들어오면 BacklightTime을 현재 시간으로 한다. 나머지 역할은 RTM이 한다.
					BC.value = COLOR_GRN;
					BC.BacklightTime.YY = CT.YY;
					BC.BacklightTime.MT = CT.MT;
					BC.BacklightTime.DD = CT.DD;
					BC.BacklightTime.HH = CT.HH;
					BC.BacklightTime.MM = CT.MM;
					BC.BacklightTime.SS = CT.SS + 2;
					BC.BacklightTime.MS = CT.MS;
					Time* prev = &(BC.BacklightTime);
					BC.BacklightTime = timeCheck(prev);
					break;
				default: break;
				}
				break;
			default: break;
			}
		}
		else { // MD.category_alpha == 3
			switch (category_beta) {
			case 1: // 3.1 alarm
				switch (Selected_Button) {
				case 1: // A
					Mode_Changer(2, 2);
					break;
				case 2: // B
					if(alarm_indicator) Mode_Changer(4, 0);
					else Mode_Changer(4, 1);
					break;
				case 3: // C
					Mode_Changer(1, 1);
					break;
				case 4: // D, D버튼이 들어오면 BacklightTime을 현재 시간으로 한다. 나머지 역할은 RTM이 한다.
					BC.value = COLOR_GRN;
					BC.BacklightTime.YY = CT.YY;
					BC.BacklightTime.MT = CT.MT;
					BC.BacklightTime.DD = CT.DD;
					BC.BacklightTime.HH = CT.HH;
					BC.BacklightTime.MM = CT.MM;
					BC.BacklightTime.SS = CT.SS + 2;
					BC.BacklightTime.MS = CT.MS;
					Time* prev = &(BC.BacklightTime);
					BC.BacklightTime = timeCheck(prev);
					break;
				default: break;
				}
				break;
			case 2: // 3.2 alarm_change_hr
				switch (Selected_Button) {
				case 1: // A
					Mode_Changer(2, 1);
					break;
				case 2: // B
					if (AL.alarmTime.HH == 23) AL.alarmTime.HH = 0; // 최대치가 된 상태에서 다시 입력하면 최저값으로
					else AL.alarmTime.HH++; // 알람 시작 시간 1 증가
					break;
				case 3: // C
					Mode_Changer(2, 3);
					break;
				case 4: // D, D버튼이 들어오면 BacklightTime을 현재 시간으로 한다. 나머지 역할은 RTM이 한다.
					BC.value = COLOR_GRN;
					BC.BacklightTime.YY = CT.YY;
					BC.BacklightTime.MT = CT.MT;
					BC.BacklightTime.DD = CT.DD;
					BC.BacklightTime.HH = CT.HH;
					BC.BacklightTime.MM = CT.MM;
					BC.BacklightTime.SS = CT.SS + 2;
					BC.BacklightTime.MS = CT.MS;
					Time* prev = &(BC.BacklightTime);
					BC.BacklightTime = timeCheck(prev);
					break;
				default: break;
				}
				break;
			case 3: // alarm_change_min
				switch (Selected_Button) {
				case 1: // A
					Mode_Changer(2, 1);
					break;
				case 2: // B
					if (AL.alarmTime.MM == 59) AL.alarmTime.MM = 0; // 최대치가 된 상태에서 다시 입력하면 최저값으로
					else
					AL.alarmTime.MM++; // 알람 시작 분 1 증가
					break;
				case 3: // C
					Mode_Changer(2, 2);
					break;
				case 4: // D, D버튼이 들어오면 BacklightTime을 현재 시간으로 한다. 나머지 역할은 RTM이 한다.
					BC.value = COLOR_GRN;
					BC.BacklightTime.YY = CT.YY;
					BC.BacklightTime.MT = CT.MT;
					BC.BacklightTime.DD = CT.DD;
					BC.BacklightTime.HH = CT.HH;
					BC.BacklightTime.MM = CT.MM;
					BC.BacklightTime.SS = CT.SS + 2;
					BC.BacklightTime.MS = CT.MS;
					Time* prev = &(BC.BacklightTime);
					BC.BacklightTime = timeCheck(prev);
					break;
				default: break;
				}
				break;
			}
		}
	}
	return Selected_Button;
}
void Realtime_Manager(int Selected_Button) {
	// 1. CT를 동기화 (int 연산들을 시간 범위 내로 맞춰줌)
	struct timeb itb;
	struct tm *now;
	time_t ltime;
	int milisec;
	ftime(&itb);
	ltime = itb.time;
	milisec = itb.millitm;
	now = localtime(&ltime);

	//시간 계산
	CT.YY = now->tm_year - TD.YY;
	CT.MT = now->tm_mon - TD.MT;
	CT.DD = now->tm_mday - TD.DD;
	CT.HH = now->tm_hour - TD.HH;
	CT.MM = now->tm_min - TD.MM;
	CT.SS = now->tm_sec - TD.SS;
	CT.MS = milisec - TD.MS;
	//여기서 CT가 2099년을 초과하였는지 체크하여햐 합니다. 만약 자연스럽게 시간이 흘러 2100년이 된다면, 아래에 보정하는 과정이
	//일어나기 전에 CT.YY에는 잠시 100이상의 값이 저장됩니다. 보정이 끝나면 자연스럽게 2019년 이상으로 바뀌겠지만,
	//TD의 경우 자연스럽게 바뀌는 과정이 없습니다. 다시말해, CT.YY >= 100인 경우 자연스럽게 CT.YY -= 81이 되지만,
	//TD의 값은 변화가 없습니다. 그러므로 자연스럽게 2099년을 초과하는 경우는 TD.YY도 81만큼 빼 주어야 합니다.
	//초과하지 않았따면, CT만 보정하여 주면 됩니다.

	//CT 보정
	CT = timeCheck(&CT);
	if (MD.stopwatch_indicator) {
			ST.stopwatchTime.YY = CT.YY - ST.startTime.YY + ST.initialTime.YY;
			ST.stopwatchTime.MT = CT.MT - ST.startTime.MT + ST.initialTime.MT;
			ST.stopwatchTime.DD = CT.DD - ST.startTime.DD + ST.initialTime.DD;
			ST.stopwatchTime.HH = CT.HH - ST.startTime.HH + ST.initialTime.HH;
			ST.stopwatchTime.MM = CT.MM - ST.startTime.MM + ST.initialTime.MM;
			ST.stopwatchTime.SS = CT.SS - ST.startTime.SS + ST.initialTime.SS;
			ST.stopwatchTime.MS = CT.MS- ST.startTime.MS + ST.initialTime.MS;

			//보정 과정
			ST.stopwatchTime = timeCheck(&ST.stopwatchTime);

			if(ST.stopwatchTime.HH >= 60) { // 60분 이상이면 전체 스탑워치를 초기화한다.
				MD.stopwatch_indicator = 0;

				ST.stopwatchTime.YY = 0;
				ST.stopwatchTime.MT = 0;
				ST.stopwatchTime.DD = 0;
				ST.stopwatchTime.HH = 0;
				ST.stopwatchTime.MM = 0;
				ST.stopwatchTime.SS = 0;
				ST.stopwatchTime.MS = 0;

				ST.startTime.YY = 0;
				ST.startTime.MT = 0;
				ST.startTime.DD = 0;
				ST.startTime.HH = 0;
				ST.startTime.MM = 0;
				ST.startTime.SS = 0;
				ST.startTime.MS = 0;

				ST.initialTime.YY = 0;
				ST.initialTime.MT = 0;
				ST.initialTime.DD = 0;
				ST.initialTime.HH = 0;
				ST.initialTime.MM = 0;
				ST.initialTime.SS = 0;
				ST.initialTime.MS = 0;

				ST.lapTime.YY = 0;
				ST.lapTime.MT = 0;
				ST.lapTime.DD = 0;
				ST.lapTime.HH = 0;
				ST.lapTime.MM = 0;
				ST.lapTime.SS = 0;
				ST.lapTime.MS = 0;
			}

		}

	if(CT.SS >= 5) {//알람이 켜질 수 없는 시간대
		if(MD.alarm_buzzing) {//그런데 알람이 울리다니!
			MD.alarm_buzzing = false;
		}
		AL.snooze = true;//평소 값으로 계속 지정해주기
		AL.ring = 0;
	}
	else {//알람이 켜질 수 있는 시간대
		if(MD.alarm_buzzing) {//알람이 울리는 중
			if(Selected_Button) {//버튼이 입력된 경우
				MD.alarm_buzzing = false;
				AL.snooze = false;//이번 사이클에서 알람을 끕니다
				AL.ring = 0;
			}
			else {//no버튼인 경우
				//그냥 넘어가면 됩니다
				MD.alarm_buzzing = true;
				AL.snooze = true;
			}
		}
		else {//알람이 안 울리는 중
			if(MD.alarm_indicator) {//그런데 시간이 맞으면 알람이 울려야 함
				if(CT.HH == AL.alarmTime.HH && CT.MM == AL.alarmTime.MM) {//그런데 시간이 맞음
					if(AL.snooze) {//그래서 알람 켜기
						MD.alarm_buzzing = true;
						AL.snooze = true;
					}
					else {//그런데 알고보니 이번 알람을 끈 경우
						//그냥 넘어가면 됩니다
						MD.alarm_buzzing = false;
						AL.snooze = false;
						AL.ring = 0;
					}
				}
				else {//그런데 시간이 안 맞음
					MD.alarm_buzzing = false;
					AL.snooze = true;
					AL.ring = 0;
				}
			}
			else {//시간이 맞아도 알람이 울리면 안 됨
				MD.alarm_buzzing = false;
				AL.snooze = true;
				AL.ring = 0;
			}
		}

	}

	//3. Backlight 조작하기
	if(CT.HH==BC.BacklightTime.HH && CT.MM ==BC.BacklightTime.MM && CT.SS==BC.BacklightTime.SS){
		BC.value = COLOR_DEF;
	}
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
	}
	else if (alpha_cat == 2) {//Stopwatch
		gotoxy(12, 3); printf("%s       %s:%s", list[0], list[1], list[2]);
		gotoxy(13, 6); printf("%s\'  %s\"  %s", list[4], list[5], list[6]);
	}
	else if (alpha_cat == 3) {//Alarm
		gotoxy(12, 3); printf("%s       %s/%s", list[0], list[1], list[2]);
		gotoxy(7, 5); printf("%s", list[3]);
		gotoxy(13, 6); printf("%s : %s       ", list[4], list[5]);
	}
}
void Ring_Changer() {
	AL.ring++;
}
void Panel_and_Speaker_Controller() {
	if (MD.alarm_buzzing) {
		if(AL.ring == CT.SS){
			Alarm();
			Ring_Changer();
		}
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
	if (MD.alarm_indicator) temp[1] = 'A';
	configure_set(list, 3, temp);
	list[7][0] = ' '; list[7][1] = ' '; list[7][2] = '\0';

	switch (flag1) {
	case 1: // Timekeeping 모드
		switch (CT.WD) {
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
		case 1: defalut: break; // (1,1)일 경우 blink_location = 0이다.
		case 2: blink_location = 7; break;// 초
		case 3: blink_location = 5; break;// 시간
		case 4: blink_location = 6; break;// 분
		case 6: blink_location = 2; break; // 월
		case 7: blink_location = 3; break;// 일
		case 5: blink_location = 8; break; // 년도
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
			int_to_str(ST.stopwatchTime.MS / 10, temp);
			configure_set(list, 6, temp);
		}
		if (flag2 == 2) {//LAP
			int_to_str(ST.lapTime.MM, temp);
			configure_set(list, 4, temp);
			int_to_str(ST.lapTime.SS, temp);
			configure_set(list, 5, temp);
			int_to_str(ST.lapTime.MS / 10, temp);
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
void Backlight_Controller(){ //색 변경
	//요청된 backlight 색깔대로 변경하는 형태
	if(BC.value==COLOR_DEF || BC.value==COLOR_GRN){
		SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), BC.value);
	}
}
int main(){

	init();
	//테스트 구간. 아직 의미 X
	int Selected_Button= 0;
	while(true){
		Selected_Button = Button_Selector();
		Realtime_Manager(Button_Operator(Selected_Button));
		Panel_and_Speaker_Controller();
		Backlight_Controller();
	}
	return 0;
}
