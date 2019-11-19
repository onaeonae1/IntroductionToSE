typedef struct StopWatch {
	//LapTime은 StartTime을 기반으로 업데이트 된다.
	Time stopwatchTime;
	Time startTime;
	Time lapTime;
	Time initialTime; // commit 안 됨

}stopwatch;
Time BacklightTime; //백라이트 시간, commit 안 됨







void init() { //초기화. 프로그램 첫 실행시에 호출됨. commit 해야 함, ST.initialTime.XX가 커밋되지 않음
	//절대 시간 선언
	struct _timeb itb;
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
	BacklightTime.YY = 0, BacklightTime.MT = 0, BacklightTime.DD = 0, BacklightTime.HH = 0, BacklightTime.MM = 0, BacklightTime.SS = 0, BacklightTime.MS = 0, BacklightTime.WD = -1; // commint 안 됨
	//모드 초기화
	MD.alarm_buzzing = false, MD.alarm_indicator = false, MD.stopwatch_indicator = false;
	MD.category_alpha = 1, MD.category_beta = 1;
	Backlight = Backlight_Controller(COLOR_GRN);
}






//버튼 오퍼레이터 수정이 없었으므로 전체 코드를 붙입니다. commit되지 않은 부분에 주석이 달려 있습니다.
void Button_Operator(int Selected_Button) {
	Bool alarm_buzzing = MD.alarm_buzzing;
	int category_alpha = MD.category_alpha;
	int category_beta = MD.category_beta;
	Bool stopwatch_indicator = MD.stopwatch_indicator;
	Bool alarm_indicator = MD.alarm_indicator;

	// 모드의 대분류(category_alpha)-소분류(category_beta)-Selected_Button 순서로 작성
	if (alarm_buzzing) { //알람 울리기가 최우선
		if (Selected_Button == 0) {
			// 버튼이 눌리지 않으면 5초 동안 가만히 있다가 스스로 종료
		}
		else { // 알람을 끔
			MD.alarm_buzzing = false; // 알람 안 울림 상태로 바꿔주고
			AL.alarmTime.YY = 19;
			AL.alarmTime.MT = 1;
			AL.alarmTime.DD = 1;
			AL.alarmTime.HH = 0;
			AL.alarmTime.MM = 0;
			AL.alarmTime.SS = 0;
			AL.alarmTime.MS = 0; // 알람 설정 시각을 초기화
		}
	}
	else {
		if (category_alpha == 1) {
			switch (category_beta) {
			case 1: // 1.1 timekeeping
				switch (Selected_Button) {
				case 1: // A
					MD.category_beta = 2;
					break;
				case 2: // B
					break;
				case 3: // C
					MD.category_alpha = 2;
					MD.category_beta = 1;
					break;
				case 4: // D, D버튼이 들어오면 BacklightTime을 현재 시간으로 한다. 나머지 역할은 RTM이 한다. commit 안 됨
					BacklightTime.YY = CT.YY;
					BacklightTime.MT = CT.MT;
					BacklightTime.DD = CT.DD;
					BacklightTime.HH = CT.HH;
					BacklightTime.MM = CT.MM;
					BacklightTime.SS = CT.SS;
					BacklightTime.MS = CT.MS;
					break;
				default: break;
				}
				break;
			case 2: // 1.2 timekeeping_change_sec
				switch (Selected_Button) {
				case 1: // A
					MD.category_beta = 1;
					break;
				case 2: // B
					if (CT.SS == 59) TD.SS -= 59; // 최대치가 된 상태에서 다시 입력하면 최저값으로
					else TD.SS++; // 현재 시각 초 1 증가
					// stopwatch 시작 초도 같이 늘려주기
					if (ST.startTime.SS == 59) {
						ST.startTime.SS = 0;
						ST.startTime.MM++;
					}
					else ST.startTime.SS++;
					break;
				case 3: // C
					MD.category_beta = 3;
					break;
				case 4: // D, D버튼이 들어오면 BacklightTime을 현재 시간으로 한다. 나머지 역할은 RTM이 한다. commit 안 됨
					BacklightTime.YY = CT.YY;
					BacklightTime.MT = CT.MT;
					BacklightTime.DD = CT.DD;
					BacklightTime.HH = CT.HH;
					BacklightTime.MM = CT.MM;
					BacklightTime.SS = CT.SS;
					BacklightTime.MS = CT.MS;
					break;
				default: break;
				}
				break;
			case 3: // 1.3 timekeeping_change_hr
				switch (Selected_Button) {
				case 1: // A
					MD.category_beta = 1;
					break;
				case 2: // B
					if (CT.HH == 23) TD.HH -= 23; // 최대치가 된 상태에서 다시 입력하면 최저값으로
					else TD.HH++; // 현재 시각 시간 1 증가
					// stopwatch 시작 시간도 같이 늘려주기
					if (ST.startTime.HH == 23) {
						ST.startTime.HH = 0;
						ST.startTime.DD++;
					}
					else ST.startTime.HH++;
					break;
				case 3: // C
					MD.category_beta = 4;
					break;
				case 4: // D, D버튼이 들어오면 BacklightTime을 현재 시간으로 한다. 나머지 역할은 RTM이 한다. commit 안 됨
					BacklightTime.YY = CT.YY;
					BacklightTime.MT = CT.MT;
					BacklightTime.DD = CT.DD;
					BacklightTime.HH = CT.HH;
					BacklightTime.MM = CT.MM;
					BacklightTime.SS = CT.SS;
					BacklightTime.MS = CT.MS;
					break;
				default: break;
				}
				break;
			case 4: // 1.4 timekeeping_change_min
				switch (Selected_Button) {
				case 1: // A
					MD.category_beta = 1;
					break;
				case 2: // B
					if (CT.MM == 59) TD.MM -= 59; // 최대치가 된 상태에서 다시 입력하면 최저값으로
					else TD.MM++; // 현재 시각 분 1 증가
					// stopwatch 시작 분도 같이 늘려주기
					if (ST.startTime.MM == 59) {
						ST.startTime.MM = 0;
						ST.startTime.HH++;
					}
					else ST.startTime.MM++;
					break;
				case 3: // C
					MD.category_beta = 5;
					break;
				case 4: // D, D버튼이 들어오면 BacklightTime을 현재 시간으로 한다. 나머지 역할은 RTM이 한다. commit 안 됨
					BacklightTime.YY = CT.YY;
					BacklightTime.MT = CT.MT;
					BacklightTime.DD = CT.DD;
					BacklightTime.HH = CT.HH;
					BacklightTime.MM = CT.MM;
					BacklightTime.SS = CT.SS;
					BacklightTime.MS = CT.MS;
					break;
				default: break;
				}
				break;
			case 5: // 1.5 timekeeping_change_yr
				switch (Selected_Button) {
				case 1: // A
					MD.category_beta = 1;
					break;
				case 2: // B
					if (CT.YY == 99) TD.YY -= 80; // 최대치가 된 상태에서 다시 입력하면 최저값으로
					else TD.YY++; // 현재 시각 년 1 증가
					// stopwatch 시작 년도 같이 늘려주기
					if (ST.startTime.YY == 99) ST.startTime.YY = 19;
					else ST.startTime.YY++;
					break;
				case 3: // C
					MD.category_beta = 6;
					break;
				case 4: // D, D버튼이 들어오면 BacklightTime을 현재 시간으로 한다. 나머지 역할은 RTM이 한다. commit 안 됨
					BacklightTime.YY = CT.YY;
					BacklightTime.MT = CT.MT;
					BacklightTime.DD = CT.DD;
					BacklightTime.HH = CT.HH;
					BacklightTime.MM = CT.MM;
					BacklightTime.SS = CT.SS;
					BacklightTime.MS = CT.MS;
					break;
				default: break;
				}
				break;
			case 6: // 1.6 timekeeping_change_mnth
				switch (Selected_Button) {
				case 1: // A
					MD.category_beta = 1;
					break;
				case 2: // B
					if (CT.MT == 12) TD.MT -= 12; // 최대치가 된 상태에서 다시 입력하면 최저값으로
					else TD.MT++; // 현재 시각 달 1 증가
					// stopwatch 시작 월도 같이 늘려주기
					if (ST.startTime.MT == 12) {
						ST.startTime.MT = 0;
						ST.startTime.YY++;
					}
					else ST.startTime.MT++;
					break;
				case 3: // C
					MD.category_beta = 7;
					break;
				case 4: // D, D버튼이 들어오면 BacklightTime을 현재 시간으로 한다. 나머지 역할은 RTM이 한다. commit 안 됨
					BacklightTime.YY = CT.YY;
					BacklightTime.MT = CT.MT;
					BacklightTime.DD = CT.DD;
					BacklightTime.HH = CT.HH;
					BacklightTime.MM = CT.MM;
					BacklightTime.SS = CT.SS;
					BacklightTime.MS = CT.MS;
					break;
				default: break;
				}
				break;
			case 7: // 1.7 timekeeping_change_day
				switch (Selected_Button) {
				case 1: // A
					MD.category_beta = 1;
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
						if (CT.DD == 31) TD.DD -= 31; // 최대치가 된 상태에서 다시 입력하면 최저값으로
						else TD.DD++;
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
							if (CT.DD == 29) TD.DD -= 29; // 최대치가 된 상태에서 다시 입력하면 최저값으로
							else TD.DD++;
						}
						else { // 윤년이 아닌 경우에는
							if (CT.DD == 28) TD.DD -= 28;
							else TD.DD++;
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
						if (CT.DD == 30) TD.DD -= 30; // 최대치가 된 상태에서 다시 입력하면 최저값으로
						else TD.DD++;
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
					MD.category_beta = 2;
					break;
				case 4: // D, D버튼이 들어오면 BacklightTime을 현재 시간으로 한다. 나머지 역할은 RTM이 한다. commit 안 됨
					BacklightTime.YY = CT.YY;
					BacklightTime.MT = CT.MT;
					BacklightTime.DD = CT.DD;
					BacklightTime.HH = CT.HH;
					BacklightTime.MM = CT.MM;
					BacklightTime.SS = CT.SS;
					BacklightTime.MS = CT.MS;
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
					if (stopwatch_indicator == 0) { // commit  안 됨
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
						MD.category_alpha = 2;
						MD.category_beta = 2;
					}
					break;
				case 2: // B
					if (stopwatch_indicator == 0) { // commit 안 됨
						MD.stopwatch_indicator = 1;
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
					else if (stopwatch_indicator == 1) { // commit 안 됨
						MD.stopwatch_indicator = 0;
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
					MD.category_alpha = 3;
					MD.category_beta = 1;
					break;
				case 4: // D, D버튼이 들어오면 BacklightTime을 현재 시간으로 한다. 나머지 역할은 RTM이 한다. commit 안 됨
					BacklightTime.YY = CT.YY;
					BacklightTime.MT = CT.MT;
					BacklightTime.DD = CT.DD;
					BacklightTime.HH = CT.HH;
					BacklightTime.MM = CT.MM;
					BacklightTime.SS = CT.SS;
					BacklightTime.MS = CT.MS;
					break;
				default: break;
				}
				break;
			case 2: // 2.2 stopwatch_LAP
				switch (Selected_Button) {
				case 1: // A
					break;
				case 2: // B
					MD.category_beta = 1;
					break;
				case 3: // C
					MD.category_alpha = 3;
					MD.category_beta = 1;
					break;
				case 4: // D, D버튼이 들어오면 BacklightTime을 현재 시간으로 한다. 나머지 역할은 RTM이 한다. commit 안 됨
					BacklightTime.YY = CT.YY;
					BacklightTime.MT = CT.MT;
					BacklightTime.DD = CT.DD;
					BacklightTime.HH = CT.HH;
					BacklightTime.MM = CT.MM;
					BacklightTime.SS = CT.SS;
					BacklightTime.MS = CT.MS;
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
					MD.category_beta = 2;
					break;
				case 2: // B
					break;
				case 3: // C
					break;
				case 4: // D, D버튼이 들어오면 BacklightTime을 현재 시간으로 한다. 나머지 역할은 RTM이 한다. commit 안 됨
					BacklightTime.YY = CT.YY;
					BacklightTime.MT = CT.MT;
					BacklightTime.DD = CT.DD;
					BacklightTime.HH = CT.HH;
					BacklightTime.MM = CT.MM;
					BacklightTime.SS = CT.SS;
					BacklightTime.MS = CT.MS;
					break;
				default: break;
				}
				break;
			case 2: // 3.2 alarm_change_hr
				switch (Selected_Button) {
				case 1: // A
					break;
				case 2: // B
					if (AL.alarmTime.HH == 23) AL.alarmTime.HH = 0; // 최대치가 된 상태에서 다시 입력하면 최저값으로
					else AL.alarmTime.HH++; // 알람 시작 시간 1 증가
					break;
				case 3: // C
					MD.category_beta = 3;
					break;
				case 4: // D, D버튼이 들어오면 BacklightTime을 현재 시간으로 한다. 나머지 역할은 RTM이 한다. commit 안 됨
					BacklightTime.YY = CT.YY;
					BacklightTime.MT = CT.MT;
					BacklightTime.DD = CT.DD;
					BacklightTime.HH = CT.HH;
					BacklightTime.MM = CT.MM;
					BacklightTime.SS = CT.SS;
					BacklightTime.MS = CT.MS;
					break;
				default: break;
				}
				break;
			case 3: // alarm_change_min
				switch (Selected_Button) {
				case 1: // A
					break;
				case 2: // B
					if (AL.alarmTime.MM = 59) AL.alarmTime.MM = 0; // 최대치가 된 상태에서 다시 입력하면 최저값으로
					AL.alarmTime.MM++; // 알람 시작 분 1 증가
					break;
				case 3: // C
					MD.category_beta = 2;
					break;
				case 4: // D, D버튼이 들어오면 BacklightTime을 현재 시간으로 한다. 나머지 역할은 RTM이 한다. commit 안 됨
					BacklightTime.YY = CT.YY;
					BacklightTime.MT = CT.MT;
					BacklightTime.DD = CT.DD;
					BacklightTime.HH = CT.HH;
					BacklightTime.MM = CT.MM;
					BacklightTime.SS = CT.SS;
					BacklightTime.MS = CT.MS;
					break;
				default: break;
				}
				break;
			}
		}
	}
}
    
    
    
    
    
    //RTM내부에 추가해야 함
    //스톱워치 계산하기 commit 안 됨
	if (MD.stopwatch_indicator) {
		ST.stopwatchTime.YY = CT.YY - ST.startTime.YY + ST.initialTime.YY;
		ST.stopwatchTime.MT = CT.MT - ST.startTime.MT + ST.initialTime.MT;
		ST.stopwatchTime.DD = CT.DD - ST.startTime.DD + ST.initialTime.DD;
		ST.stopwatchTime.HH = CT.HH - ST.startTime.HH + ST.initialTime.HH;
		ST.stopwatchTime.MM = CT.MM - ST.startTime.MM + ST.initialTime.MM;
		ST.stopwatchTime.SS = CT.SS - ST.startTime.SS + ST.initialTime.SS;
		ST.stopwatchTime.MS = CT.MS - ST.startTime.MS + ST.initialTime.MS;
	
		// 보정하는 과정이 필요합니다!!!!!!!!!!!!!!
	
	}
    
    
