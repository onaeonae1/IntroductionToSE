#include<stdio.h>
#include<windows.h>
#include<time.h>
#include"linux_kbhit.h"
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
alm AL;
stopwatch ST;
mode MD; //모드
Time CT; //현재 시간
int Backlight; //글자색
//0 : Alarm Buzzing, 1 : 대분류, 2: 소분류 , 3 : Stopwatch_Indicator, 4 : Alarm indicator
Bool buttonA_interface(char input){
	if(input=='a' || input =='A'){
		printf("Button A Selected \n");
		return true;
	}
	return false;
}
Bool buttonB_interface(char input){
	if(input=='b' || input=='B'){
		printf("Button B Selected \n");
		return true;
	}
	return false;
}
Bool buttonC_interface(char input){
	if(input=='c' || input=='C'){
		printf("Button C Selected \n");
		return true;
	}
	return false;
}
Bool buttonD_interface(char input){
	if(input=='d' || input=='D'){
		printf("Button D Selected \n");
		return true;
	}
	return false;
}
void readmode(){ // 잠재적 수정사항

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
int Button_Selector(){
	Bool flag = false;
	int Selected_Button =0 ;
	char temp='0';
	//일정 기간 wait하고. -> 이 부분은 잠재적 수정 사항이다.
	Sleep(1);
	int value=-1;
	value=linux_kbhit();
	while(value!=-1){
		//int value = linux_kbhit(); //하나 받기
		printf("%d\n", value);
		if((value>=97 && value<=100)|| (value>=65 && value<=68)){ //버튼이 눌렸는지만 판별
			temp = value;
		}
		if(temp=='0'){
			printf("No Button Selected \n");
			break;
		}
		printf("%c\n", temp);
		//우선순위는 아래와 같이 interface의 실행 순서로 고려한다.
		flag = buttonD_interface(temp);
		if(flag==true){
			Selected_Button = 4;
			break;
		}
		flag = buttonC_interface(temp);
		if(flag==true){
			Selected_Button = 3;
			break;
		}
		flag=  buttonB_interface(temp);
		if(flag==true){
			Selected_Button  = 2;
			break;
		}
		flag = buttonA_interface(temp);
		if(flag==true){
			Selected_Button = 1;
			break;
		}
	}
	//Selected Button : 0 = No button, 1 = A, 2 = B, 3 = C, 4 = D
	printf("SELECTOR OFF\n");
	return Selected_Button;
}
void Realtime_Manager(){
	//CT를 동기화해줌
}
void Mode_Changer(mode Mode_to_Change){ //MD를 수정할 수 있는 함수
	MD = Mode_to_Change; //값 복사
}
void Button_Operator(int Selected_Button){ //노가다로 짤 거 같은데 좀 더 고려 필요
	printf("Selected Button is : %d\n",Selected_Button);
	/*
	//모드 읽기
	if(MD.alarm_buzzing==true){ //알람 울리기

	}
	else{

	}
	*/
	//그 다음에 버튼 읽기
	if(Selected_Button== 1){ //A

	}
	if(Selected_Button == 2){ //B

	}
	if(Selected_Button == 3){ //C

	}
	if(Selected_Button ==4){ //D
		printf("Button D Operated \n");
		Backlight = Backlight_Controller(Backlight);
	}
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
	int previous_key = 0;
	while(true){
		Selected_Button = Button_Selector();
		Button_Operator(Selected_Button);
	}
	return 0;
}
