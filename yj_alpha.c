typedef struct StopWatch {
	//LapTime은 StartTime을 기반으로 업데이트 된다.
	Time stopwatchTime;
	Time startTime;
	Time lapTime;
	Time initialTime; // commit 안 됨

}stopwatch;








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
	//모드 초기화
	MD.alarm_buzzing = false, MD.alarm_indicator = false, MD.stopwatch_indicator = false;
	MD.category_alpha = 1, MD.category_beta = 1;
	Backlight = Backlight_Controller(COLOR_GRN);
}







// buttonOperator 내부에서 스톱워치 부분
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
				case 4: // D
					Backlight = Backlight_Controller(Backlight);
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
				case 4: // D
					Backlight = Backlight_Controller(Backlight);
					break;
				default: break;
				}
				break;
			default: break;
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
    
    
