#include <stdio.h>
#include <time.h>
#include<string.h>
int main() {
	//freopen("input.txt", "r", stdin);
	//freopen("output.txt", "w", stdout);
	time_t rawtime;
	struct tm* timeinfo;
	int year, month, day;
	char weekday[20];
	strcpy(weekday, "SUMNTUWDTHFRST");
	/* 사용자로 부터 날짜를 입력 받는다. */
	//printf("Enter year: ");
	scanf("%d", &year);
	//printf("Enter month: ");
	scanf("%d", &month);
	//printf("Enter day: ");
	scanf("%d", &day);

	/* rawtime 에 time 함수로 현재 시간 정보로 세팅한 뒤,
	   사용자로 부터 입력받은 데이터로 년/월/일 정보를 수정한다. */
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	timeinfo->tm_year = year - 1900;
	timeinfo->tm_mon = month - 1;
	timeinfo->tm_mday = day;

	/* mktime 함수를 호출하면 년/월/일 데이터에 따라
	   tm_wday 와 tm_yday 를 설정한다. 이 때 리턴되는 time_t 데이터는 사용하지
	   않는다.*/
	mktime(timeinfo);
	//윤년 평년에 따라 일 수정을 자동으로 해줌
  //요일도 자동으로 처리됨
  //범위 체크도 자동으로 되고 자리 오르내림이 됨
  //ex1 ) 2019년 1월 12341일 -> 2052년 10월 14일 MN
  //ex2 ) 2019년 -3월 31일 -> 2018년 10월 1일 MN
  printf("%d 년 %d 월 %d 일 \n", timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday);
	printf("%c%c", weekday[(timeinfo->tm_wday) * 2], weekday[((timeinfo->tm_wday) * 2 + 1)]);
  
	return 0;
}
