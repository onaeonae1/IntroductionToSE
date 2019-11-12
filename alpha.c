#include<stdio.h>
#include<windows.h>
#define COLOR_DEF 4
#define COLOR_GRN 2
//c에는 bool이 없다. 따라서 이렇게 열거형으로 만들어줘서 사용해야 함.
typedef enum Boolean{
	false=0,
	true = 1
}bool;
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
}time;
typedef struct Alarm{ //시작 시간
	time startTime;
}alarm;
typedef struct StopWatch{

}stopwatch;
//Data Store들 선언하기
mode MD; //모드
time CT; //현재 시간
int Backlight; //글자색
//0 : Alarm Buzzing, 1 : 대분류, 2: 소분류 , 3 : Stopwatch_Indicator, 4 : Alarm indicator
bool buttonA_interface(char input){
	if(input=='a' || input =='A'){
		printf("Button A Selected \n");
		return true;
	}
	return false;
}
bool buttonB_interface(char input){
	if(input=='b' || input=='B'){
		printf("Button B Selected \n");
		return true;
	}
	return false;
}
bool buttonC_interface(char input){
	if(input=='c' || input=='C'){
		printf("Button C Selected \n");
		return true;
	}
	return false;
}
bool buttonD_interface(char input){
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
	//모드 초기화
	MD.alarm_buzzing = false, MD.alarm_indicator = false, MD.stopwatch_indicator = false;
	MD.category_alpha = 0, MD.category_beta =0;
	//글자색 초기화
	Backlight = Backlight_Controller(COLOR_GRN);
}
int Button_Selector(){
	bool flag = false;
	int Selected_Button =0 ;
	char temp;
	//일정 기간 wait하고. -> 이 부분은 잠재적 수정 사항이다.
	while(flag==false){
		scanf("%c", &temp);
		getchar(); //버퍼 비우기
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
	while(true){
		Selected_Button = Button_Selector();
		Button_Operator(Selected_Button);
	}
	return 0;
}
