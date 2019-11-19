//전체
#include<stdio.h>
#include<windows.h>
#include<time.h>
#include<sys/timeb.h>
#include<string.h>
#include"linux_kbhit.h"
#include"getch.h"
#define COLOR_DEF 12
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
	int WD; //요일
}Time;
typedef struct Alm{ //시작 시간
	Time alarmTime;
}alm;
typedef struct StopWatch {
	//LapTime은 StartTime을 기반으로 업데이트 된다.
	Time stopwatchTime;
	Time startTime;
	Time lapTime;
	Time initialTime; // commit 안 됨

}stopwatch;
typedef struct Backlight{
	Time BacklightTime; //backlight 켜기 시작한 시간
	int value; //backlight 색깔 값
}backlight;
Time timeCheck(Time* dest){ //Time 형을 하나 불러와서 범위에 맞는지 체크
	int year = dest->YY; int month = dest->MT;
	int day = dest->DD; int hour = dest->HH;
	int min = dest->MM; int sec = dest->SS;
	struct tm* timeinfo;
	time_t rawtime;
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	timeinfo->tm_year = (year-119) % 81 + 119;
	timeinfo->tm_mon = month;
	timeinfo->tm_mday = day;
	timeinfo->tm_hour = hour;
	timeinfo->tm_min = min;
	timeinfo->tm_sec = sec;
	mktime(timeinfo);

	Time ret;
	ret.YY = timeinfo->tm_year;
	ret.MT = timeinfo->tm_mon;
	ret.DD = timeinfo->tm_mday;
	ret.HH = timeinfo->tm_hour;
	ret.MM = timeinfo->tm_min;
	ret.SS = timeinfo->tm_sec;
	ret.WD = timeinfo->tm_wday;
	ret.MS = dest->MS;
	return ret;
}
void configure_set(char list[7][3], int location, char goal[3]) {
	//list엔 configure 될 값들이
	//location엔 0에서 6사이의 변경할 위치
	//goal에는 바꿀 값이 들어있다
	list[location][0] = goal[0];
	list[location][1] = goal[1];
	list[location][2] = '\0';
	return;
}

void int_to_str(int to, char temp[3]) {
	// to는 두 자리 이하의 자연수라는 것을 가정하고 사용
	if (to < 10) {
		temp[0] = '0';
		temp[1] = (char)(to + '0');
		temp[2] = '\0';
	}
	else {
		temp[0] = (char)(to / 10 + '0');
		temp[1] = (char)(to % 10 + '0');
		temp[2] = '\0';
	}
	return;
}
void gotoxy(int x, int y) {

	printf("\033[%d;%df", y, x);

	fflush(stdout);

}
