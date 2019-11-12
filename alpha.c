#include<stdio.h>
#include<windows.h>
#include<time.h>
#include"linux_kbhit.h"
#include"getch.h"
#define COLOR_DEF 4
#define COLOR_GRN 2
//c에는 bool이 없다. 따라서 이렇게 열거형으로 만들어줘서 사용해야 함.
typedef enum Boolean{
	false=0,
	true = 1
}Bool;
//Data Store들을 정의하기
typedef struct Mode{
	Bool alarm_buzzing;
	int category_alpha; //대분류
	int category_beta; //소분류
	Bool stopwatch_indicator; //indicator 켜졌는지,
	Bool alarm_indicator; //indicator 꺼졌는지,
}mode;
typedef struct Time{
	int YY; //년도
	int MT; //월
	int DD; //일
	int HH; //시
	int MM; //분
	int SS; //초
	int MS; //ms
}Time;
typedef struct Alarm{ //시작 시간
	Time alarmTime;
}alm;
typedef struct StopWatch{
	//LapTime은 StartTime을 기반으로 업데이트 된다.
	Time stopwatchTime;
	Time startTime;
	Time lapTime;

}stopwatch;
//Data Store들 선언하기
alm AL; //알람
stopwatch ST; //스톱워치
mode MD; //모드
Time CT; //현재 시간
int Backlight; //글자색
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
Bool buttonNone_interface(char input){
	if(input==NULL){
		//printf("No Button Selected \n");
		return true;
	}
	return false;
}
void init(){ //초기화. 프로그램 첫 실행시에 호출됨.
	//시간 초기화
	CT.YY = 2019, CT.MT=1, CT.DD=1, CT.HH=0, CT.MM=0, CT.SS = 0, CT.MS=0;
	//알람 초기화
	AL.alarmTime.YY = 2019, AL.alarmTime.MT = 1, AL.alarmTime.DD = 1, AL.alarmTime.HH = 0, AL.alarmTime.MM = 0, AL.alarmTime.SS = 0, AL.alarmTime.MS = 0;
	//스톱워치
	ST.stopwatchTime.YY = 2019, ST.stopwatchTime.MT = 1, ST.stopwatchTime.DD = 1, ST.stopwatchTime.HH = 0, ST.stopwatchTime.MM, ST.stopwatchTime.SS = 0, ST.stopwatchTime.MS = 0;
	ST.startTime.YY = 2019, ST.startTime.MT = 1, ST.startTime.DD = 1, ST.startTime.HH = 0, ST.startTime.MM = 0, ST.startTime.SS = 0, ST.startTime.MS = 0;
	ST.lapTime.YY = 2019, ST.lapTime.MT = 1, ST.lapTime.DD = 1, ST.lapTime.HH = 0, ST.lapTime.MM = 0, ST.lapTime.SS = 0, ST.lapTime.MS = 0;
	//모드 초기화
	MD.alarm_buzzing = false, MD.alarm_indicator = false, MD.stopwatch_indicator = false;
	MD.category_alpha = 0, MD.category_beta =0;
	Backlight = Backlight_Controller(COLOR_GRN);
}
int Button_Selector() {
	Sleep(1000);
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
			AL.alarmTime.YY = 2019;
			AL.alarmTime.MT = 1; 
			AL.alarmTime.DD = 1; 
			AL.alarmTime.HH = 0; 
			AL.alarmTime.MM = 0; 
			AL.alarmTime.SS = 0; 
			AL.alarmTime.MS = 0; // 알람 설정 시각을 초기화
		}
	}
	else {	
		if (MD.category_alpha == 1) {
			switch (MD.category_beta) {
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
				case 4: // D
					Backlight = Backlight_Controller(Backlight);
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
					if (CT.SS == 59) { // 최대치가 된 상태에서 다시 입력하면 최저값으로
						CT.SS == 0;
					}
					else CT.SS++; // 현재 시각 초 1 증가
					break;
				case 3: // C
					MD.category_beta = 3;
					break;
				case 4: // D
					Backlight = Backlight_Controller(Backlight);
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
					if (CT.HH == 23) { // 최대치가 된 상태에서 다시 입력하면 최저값으로
						CT.HH == 0;
					}
					else CT.HH++; // 현재 시각 시간 1 증가
					break;
				case 3: // C
					MD.category_beta = 4;
					break;
				case 4: // D
					Backlight = Backlight_Controller(Backlight);
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
					if (CT.MM == 59) { // 최대치가 된 상태에서 다시 입력하면 최저값으로
						CT.MM = 0;
					}
					else CT.MM++; // 현재 시각 분 1 증가
					break;
				case 3: // C
					MD.category_beta = 5;
					break;
				case 4: // D
					Backlight = Backlight_Controller(Backlight);
					break;
				default: break;
				}
				break;
			case 5: // 1.5 timekeeping_change_yr
				switch(Selected_Button) {
				case 1: // A
					MD.category_beta = 1;
					break;
				case 2: // B
					if (CT.YY == 2099) { // 최대치가 된 상태에서 다시 입력하면 최저값으로
						CT.YY = 2019;
					}
					else CT.YY++; // 현재 시각 년 1 증가
					break;
				case 3: // C
					MD.category_beta = 6;
					break;
				case 4: // D
					Backlight = Backlight_Controller(Backlight);
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
					if (CT.MT == 12) { // 최대치가 된 상태에서 다시 입력하면 최저값으로
						CT.MT = 1;
					}
					else CT.MT++; // 현재 시각 달 1 증가
					break;
				case 3: // C
					MD.category_beta = 7;
					break;
				case 4: // D
					Backlight = Backlight_Controller(Backlight);
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
						if (CT.DD == 31) { // 최대치가 된 상태에서 다시 입력하면 최저값으로
							CT.DD = 1;
						}
						else CT.DD++;
						break;
					// 한 달에 28일이 있는 경우(윤달은 제외)
					case 2:
						if (CT.DD == 28) { // 최대치가 된 상태에서 다시 입력하면 최저값으로
							CT.DD = 1;
						}
						else CT.DD++;
						break;
					// 한 달에 30일이 있는 경우
					case 4:
					case 6:
					case 9:
					case 11:
						if (CT.DD == 30) { // 최대치가 된 상태에서 다시 입력하면 최저값으로
							CT.DD = 1;
						}
						else CT.DD++;
						break;
					}
					break;
				case 3: // C
					MD.category_beta = 2;
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
		else if (MD.category_alpha == 2) {
			switch (MD.category_beta) {
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
					}
					else if (stopwatch_indicator == 1) {
						ST.lapTime = ST.stopwatchTime;
						MD.category_alpha = 2;
						MD.category_beta = 2;
					}
					break;
				case 2: // B
					if (stopwatch_indicator == 0) {
						MD.stopwatch_indicator = 1;
						// ST.stopwatchTime += ST.startTime을 해줌
						ST.stopwatchTime.YY += ST.startTime.YY;
						ST.stopwatchTime.MT += ST.startTime.MT;
						ST.stopwatchTime.DD += ST.startTime.DD;
						ST.stopwatchTime.HH += ST.startTime.HH;
						ST.stopwatchTime.MM += ST.startTime.MM;
						ST.stopwatchTime.SS += ST.startTime.SS;
						ST.stopwatchTime.MS += ST.startTime.MS;
						ST.stopwatchTime = CT;
					}
					else if (stopwatch_indicator == 1) {
						MD.stopwatch_indicator = 0;
						// ST.startTime += CT - ST.startTime;
						ST.startTime.YY += (CT.YY - ST.startTime.YY);
						ST.startTime.MT += (CT.MT - ST.startTime.MT);
						ST.startTime.DD += (CT.DD - ST.startTime.DD);
						ST.startTime.HH += (CT.HH - ST.startTime.HH);
						ST.startTime.MM += (CT.MM - ST.startTime.MM);
						ST.startTime.SS += (CT.SS - ST.startTime.SS);
						ST.startTime.MS += (CT.MS - ST.startTime.MS);
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
		else { // MD.category_alpha == 3
			switch (MD.category_beta) {
			case 1: // 3.1 alarm
				switch (Selected_Button) {
				case 1: // A
					MD.category_beta = 2;
					break;
				case 2: // B
					break;
				case 3: // C
					break;
				case 4: // D
					Backlight = Backlight_Controller(Backlight);
					break;
				default: break;
				}
				break;
			case 2: // 3.2 alarm_change_hr
				switch (Selected_Button) {
				case 1: // A
					break;
				case 2: // B
					AL.alarmTime.HH++; // 알람 시작 시간 1 증가
					break;
				case 3: // C
					MD.category_beta = 3;
					break;
				case 4: // D
					Backlight = Backlight_Controller(Backlight);
					break;
				default: break;
				}
				break;
			case 3: // alarm_change_min
				switch (Selected_Button) {
				case 1: // A
					break;
				case 2: // B
					AL.alarmTime.MM++; // 알람 시작 분 1 증가
					break;
				case 3: // C
					MD.category_beta = 2;
					break;
				case 4: // D
					Backlight = Backlight_Controller(Backlight);
					break;
				default: break;
				}
				break;
 			}
		}
	}
}
void Realtime_Manager(){
	//CT를 동기화해줌
}
void Mode_Changer(mode Mode_to_Change){ //MD를 수정할 수 있는 함수
	MD = Mode_to_Change; //값 복사
}

void Panel_and_Speaker_Controller(){

}
int Backlight_Controller(int backlight){ //색 변경
	if(backlight==COLOR_DEF){
		SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), COLOR_GRN);
		return COLOR_GRN;
	}
	else{
		SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), COLOR_DEF);
		return COLOR_DEF;
	}
}
int main(){
	init();
	//테스트 구간. 아직 의미 X
	int Selected_Button= 0;
	while(true){
		Selected_Button = Button_Selector();
		Button_Operator(Selected_Button);
	}
	return 0;
}
