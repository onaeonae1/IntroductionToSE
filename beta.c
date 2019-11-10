#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<sys/timeb.h>
//c에는 bool이 없다. 따라서 이렇게 열거형으로 만들어줘서 사용해야 함.
typedef enum Boolean{
	False=0,
	True = 1
}Bool;
//Data Store들을 정의하기
typedef struct Mode{
	bool alarm_buzzing;
	int category_alpha; //대분류
	int category_beta; //소분류
	bool stopwatch_indicator; //indicator 켜졌는지,
	bool alarm_indicator; //indicator 꺼졌는지,
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
	Time startTime;
}alarm;
typedef struct StopWatch{

}stopwatch;
//Data Store들 선언하기
mode MD; //모드
Time CT; //현재 시간
//0 : Alarm Buzzing, 1 : 대분류, 2: 소분류 , 3 : Stopwatch_Indicator, 4 : Alarm indicator
Bool buttonA_interface(char input){
	if(input=='a' || input =='A'){
		printf("Button A Selected \n");
		return True;
	}
	return False;
}
Bool buttonB_interface(char input){
	if(input=='b' || input=='B'){
		printf("Button B Selected \n");
		return True;
	}
	return False;
}
Bool buttonC_interface(char input){
	if(input=='c' || input=='C'){
		printf("Button C Selected \n");
		return True;
	}
	return False;
}
Bool buttonD_interface(char input){
	if(input=='d' || input=='D'){
		printf("Button D Selected \n");
		return True;
	}
	return False;
}
void readmode(){ // 잠재적 수정사항

}
void init(){ //초기화. 프로그램 첫 실행시에 호출됨.
	//시간 초기화
	CT.YY = 2019, CT.MT=1, CT.DD=1, CT.HH=0, CT.MM=0, CT.SS = 0, CT.MS=0;
	//모드 초기화
	MD.alarm_buzzing = false, MD.alarm_indicator = false, MD.stopwatch_indicator = false;
	MD.category_alpha = 0, MD.category_beta =0;
}
int Button_Selector(){
	struct timeb startTime;
	struct timeb nowTime;
	int msstart;
	int msend;
	ftime(&startTime);
	msstart = startTime.millitm;
	while (true) {
		ftime(&nowTime);
		if (nowTime.millitm - startTime.millitm > 1) {
			break;
		}
		wait()
	}
	int Selected_Button = 0;
	/*
	Bool alpha = False;
	char temp;
	//일정 기간 wait하고. -> 이 부분은 잠재적 수정 사항이다.
	while(alpha==False){
		scanf("%c", &temp);
		//우선순위는 아래와 같이 interface의 실행 순서로 고려한다.
		alpha = buttonD_interface(temp);
		if(alpha==True){
			Selected_Button = 4;
			break;
		}
		alpha = buttonC_interface(temp);
		if(alpha==True){
			Selected_Button = 3;
			break;
		}
		alpha=  buttonB_interface(temp);
		if(alpha==True){
			Selected_Button  = 2;
			break;
		}
		alpha = buttonA_interface(temp);
		if(alpha==True){
			Selected_Button = 1;
			break;
		}
	}
	//Selected Button : 0 = No button, 1 = A, 2 = B, 3 = C, 4 = D
	*/
	return Selected_Button;
}
void Realtime_Manager(){

}
void Mode_Changer(mode Mode_to_Change){ //MD를 수정할 수 있는 함수
	MD = Mode_to_Change; //값 복사
}
void Button_Operator(int Selected_Button){ //노가다로 짤 거 같은데 좀 더 고려 필요
	//모드 읽기
	if(MD.alarm_buzzing==true){ //알람 울리기

	}
	else{

	}
	//그 다음에 버튼 읽기
	if(Selected_Button== '1'){ //A

	}
	if(Selected_Button == '2'){ //B

	}
	if(Selected_Button == '3'){ //C

	}
	if(Selected_Button =='4'){ //D

	}
}
void Panel_and_Speaker_Controller(){

}
void Backlight_Controller(){

}
int main(){
	init();
	while (True) { //일정 시간에 맞춰서 진행되어야 함
		int Selected_Button = 0;
		Selected_Button = Button_Selector();
		Button_Operator(Selected_Button);
	}
	return 0;
}
