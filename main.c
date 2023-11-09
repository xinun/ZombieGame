#pragma warning(disable:4996)
#include <stdio.h>
#include <conio.h>
#include <Windows.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>	
#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")
#define _CRT_SECURE_NO_WARNINGS
// 색상 정의
#define BLACK	0
#define BLUE1	1
#define GREEN1	2
#define CYAN1	3
#define RED1	4
#define MAGENTA1 5
#define YELLOW1	6
#define GRAY1	7
#define GRAY2	8
#define BLUE2	9
#define GREEN2	10
#define CYAN2	11
#define RED2	12
#define MAGENTA2 13
#define YELLOW2	14
#define WHITE	15
#define ESC 0x1b //  ESC 누르면 종료
#define ENTER 13
#define SPECIAL1 0xe0 // 특수키는 0xe0 + key 값으로 구성된다.
#define SPECIAL2 0x00 // keypad 경우 0x00 + key 로 구성된다.

#define UP  0x48 // Up key는 0xe0 + 0x48 두개의 값이 들어온다.
#define DOWN 0x50
#define LEFT 0x4b
#define RIGHT 0x4d
#define SPACE 0x20
#define UP2		'w'//player2 는 AWSD 로 방향키 대신
#define DOWN2	's'
#define LEFT2	'a'
#define RIGHT2	'd'
#define KEY	"§"
#define G 'g'
#define M 'm'
#define H 'h'
#define WIDTH 92
#define HEIGHT 36
#define MAXBULLET 100//플레이어의 최대 총알 수
#define TRUE 1
#define FALSE 0 
int item[WIDTH][HEIGHT] = { 0 };
int item2[WIDTH][HEIGHT] = { 0 };
int item3[WIDTH][HEIGHT] = { 0 };
int item4[WIDTH][HEIGHT] = { 0 };
int item5[WIDTH][HEIGHT] = { 0 };
int item6[WIDTH][HEIGHT] = { 0 };
int item7[WIDTH][HEIGHT] = { 0 };
int item8[WIDTH][HEIGHT] = { 0 };
int called[2];
int Delay = 20; // 100 msec delay, 이 값을 줄이면 속도가 빨라진다.
int frame_count = 0; // game 진행 frame count 로 속도 조절용으로 사용된다.

int p1_frame_sync_start = 0; //   
int bullet_frame_sync = 5;
int keep_moving = 1; // 1:계속이동, 0:한칸씩이동.
int time_out = 60; // 제한시간
int score[2] = { 0 };
int keys[WIDTH][HEIGHT] = { 0 }; // 1이면 key 있다는 뜻
char tempmap[HEIGHT][WIDTH];
int LIFE2 = 5;
void textcolor(int fg_color, int bg_color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), fg_color | bg_color << 4);
}
void removeCursor(void) { // 커서를 안보이게 한다

	CONSOLE_CURSOR_INFO curInfo;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
	curInfo.bVisible = 0;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
}
void gotoxy(int x, int y) //내가 원하는 위치로 커서 이동
{
	COORD pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);// WIN32API 함수입니다. 이건 알필요 없어요
}
void cls(int bg_color, int text_color) // 화면 지우기
{
	char cmd[100];
	system("cls");
	sprintf(cmd, "COLOR %x%x", bg_color, text_color);
	system(cmd);
}
void draw_box2(int x1, int y1, int x2, int y2)
{
	int x, y;
	//int len = strlen(ch);
	for (x = x1; x <= x2; x += 2) { // 한글은 2칸씩 차지한다.
		gotoxy(x, y1);
		printf("%s", "─"); // ㅂ 누르고 한자키 누르고 선택
		gotoxy(x, y2);
		printf("%s", "─");
	}
	for (y = y1; y <= y2; y++) {
		gotoxy(x1, y);
		printf("%s", "│");
		gotoxy(x2, y);
		printf("%s", "│");
	}
	gotoxy(x1, y1); printf("┌");
	gotoxy(x1, y2); printf("└");
	gotoxy(x2, y1); printf("┐");
	gotoxy(x2, y2); printf("┘");
}
void showplayer1(int ox, int oy, int nx, int ny) //1p
{
	textcolor(GREEN1, BLACK);
	gotoxy(ox, oy);
	printf("  ");
	gotoxy(ox, oy + 1);
	printf("  ");
	gotoxy(nx, ny);
	printf("⊙");
	gotoxy(nx, ny + 1);
	printf("◆");

} //1p
void showplayer2(int ox, int oy, int nx, int ny) //2p
{
	textcolor(YELLOW1, BLACK);
	gotoxy(ox, oy);
	printf("  ");
	gotoxy(ox, oy + 1);
	printf("  ");
	gotoxy(nx, ny);
	printf("⊙");
	gotoxy(nx, ny + 1);
	printf("◆");
	textcolor(WHITE, BLACK);
} //2p
void playererase(int x, int y) {
	gotoxy(x, y);
	printf(" ");
}
int title() {
	cls(BLACK, WHITE);

	draw_box2(0, 0, WIDTH, HEIGHT);
	unsigned char ch;
	int POS = 0;
	textcolor(WHITE, BLACK);
	gotoxy(11, 7);
	printf("■■■■■  ■■■■    ■      ■   ■■■■    ■■■■       ■      ■");
	gotoxy(11, 8);
	printf("	  ■  ■       ■  ■■  ■■   ■    ▼   ■       ■       ■  ■");
	gotoxy(11, 9);
	printf("	■    ■       ■  ■  ■  ■   ■■■     ■       ■         ■");
	gotoxy(11, 10);
	printf("   ■      ■       ■  ■      ■   ■    ▲   ■       ■       ■  ■");
	gotoxy(11, 11);
	printf(" ■        ■       ■  ■      ■   ■    ■   ■       ■      ■    ■");
	gotoxy(11, 12);
	printf("■■■■■   ■■■■   ■      ■   ■■■■     ■■■■      ■      ■");
	gotoxy(11, 13);
	textcolor(RED1, BLACK);
	printf("■■  ■■   ■■■■   ■      ■   ■■■■     ■■■■      ■      ■");
	gotoxy(11, 14);
	printf("■■    ■   ■■  ■           ■   ■  ■■     ■■  ■      ■      ■");
	gotoxy(11, 15);

	printf("■      ■     ■  ■           ■   ■  ■       ■                    ■");
	gotoxy(11, 16);

	printf("■             ■                        ■                             ■");
	textcolor(WHITE, BLACK);
	gotoxy(12, 24);
	printf("SPACEBAR : 메뉴 선택     ←  →  :  메뉴 이동    ");
	gotoxy(13, 28); printf("[게임시작]");
	gotoxy(43, 28); printf("조작법   ");
	gotoxy(73, 28); printf("게임종료   ");
	while (1) {
		if (kbhit() == 1) {
			ch = getch();

			switch (ch) {
			case LEFT:
				if (POS == 0)
					POS = 2;
				else
					POS -= 1;
				break;
			case RIGHT:
				if (POS == 2)
					POS = 0;
				else POS += 1;
				break;
			case SPACE:
				return POS;
				break;
			}
			switch (POS) {
			case 0:
				gotoxy(13, 28); printf("[게임시작]");
				gotoxy(43, 28); printf("조작법");
				gotoxy(50, 28); printf("   ");
				gotoxy(73, 28); printf("게임종료");
				gotoxy(81, 28); printf("   ");

				break;
			case 1:
				gotoxy(13, 28); printf("게임시작");
				gotoxy(21, 28); printf("   ");
				gotoxy(43, 28); printf("[조작법]");
				gotoxy(73, 28); printf("게임종료");
				gotoxy(81, 28); printf("   ");

				break;
			case 2:
				gotoxy(13, 28); printf("게임시작");
				gotoxy(21, 28); printf("  ");
				gotoxy(43, 28); printf("조작법");
				gotoxy(49, 28); printf("  ");
				gotoxy(73, 28); printf("[게임종료]");
				break;

			default: break;
			}
		}
		Sleep(100);
	}

}//타이틀 메뉴
void control() { //조작법 메뉴
	cls(BLACK, WHITE);
	draw_box2(0, 0, WIDTH, HEIGHT);
	gotoxy(44, 10);
	textcolor(BLACK, WHITE);
	printf("조작법");
	gotoxy(29, 24);
	textcolor(WHITE, BLACK);
	printf("아무키나 누르면 타이틀로 돌아갑니다.");
	gotoxy(19, 14);	
	printf("1p : ←↑→↓  방향이동    /  'SPACEBAR' , 'M'       공격");
	gotoxy(19, 18);
	printf("2p :  wasd     방향이동    /      'G'    , 'H'       공격");

	while (1) {
		if (kbhit()) {
			break;
		}
	}
	cls(BLACK, WHITE);
}//조작법
int level() {  //난이도 선택
	draw_box2(0, 0, WIDTH, HEIGHT);
	unsigned char ch;
	int POS = 0;
	int c1, c2;

	textcolor(WHITE, BLACK);
	gotoxy(12, 25);
	printf("SPACEBAR :  선택     ←  →  :   이동    ESC   :   뒤로가기  ");
	gotoxy(42, 5);
	textcolor(BLACK, WHITE);
	printf("게임 플레이");
	textcolor(WHITE, BLACK);

	gotoxy(10, 9);
	printf("1.좀비를 피해 먼저 백신을 먹는 사람이 승리합니다.");
	gotoxy(13, 28); printf("[1단계]");
	gotoxy(43, 28); printf("2단계");
	gotoxy(73, 28); printf("3단계");

	while (1) {
		if (kbhit() == 1) {
			ch = getch();

			switch (ch) {
			case LEFT:
				if (POS == 0)
					POS = 2;
				else
					POS -= 1;
				break;
			case RIGHT:
				if (POS == 2)
					POS = 0;
				else POS += 1;
				break;
			case SPACE:
				return POS;
				break;
			case ESC:
				return 3;
				break;
			}
			switch (POS) {
			case 0:
				gotoxy(10, 5);
				printf("                     ");
				gotoxy(10, 9);
				printf("                                                                ");
				gotoxy(10, 11);
				printf("                                                                ");
				gotoxy(10, 13);
				printf("                                                                 ");
				gotoxy(10, 15);
				printf("                                                                ");
				gotoxy(10, 17);
				printf("                                                                 ");
				gotoxy(10, 19);
				printf("                                                                      ");
				gotoxy(10, 23);
				printf("                                                                  ");
				gotoxy(42, 5);
				textcolor(BLACK, WHITE);
				printf("게임 플레이");
				textcolor(WHITE, BLACK);

				gotoxy(10, 9);
				printf("1.좀비를 피해 먼저 백신을 먹는 사람이 승리합니다.");
				gotoxy(13, 28); printf("[1단계]");
				gotoxy(43, 28); printf("         ");
				gotoxy(43, 28); printf("2단계");
				gotoxy(73, 28); printf("         ");
				gotoxy(73, 28); printf("3단계");

				break;
			case 1:
				gotoxy(10, 9);
				printf("                                                                   ");
				gotoxy(10, 11);
				printf("                                                               ");
				gotoxy(10, 13);
				printf("                                                                ");
				gotoxy(10, 15);
				printf("                                                                ");
				gotoxy(10, 17);
				printf("                                                               ");
				gotoxy(10, 19);
				printf("                                                                      ");
				gotoxy(10, 20);
				printf("                                                               ");
				gotoxy(10, 21);
				printf("                                                               ");
				gotoxy(10, 23);
				printf("                                                                  ");
				gotoxy(42, 5); 
				textcolor(BLACK, WHITE);
				printf("게임 플레이");
				textcolor(WHITE, BLACK);
				gotoxy(10, 9);
				printf("1.좀비를 피해 백신을 5개 먼저 먹는 사람이 승리합니다.           ");
				gotoxy(10, 11);
				printf("2.하트가 모두 소진 되면 사망합니다.");
				gotoxy(10, 13);
				printf("3.1p는 키보드 '스테이스바'와 'M'으로 좀비를 멈출 수 있고   ");
				gotoxy(10, 15);
				printf("  2p는 'G''H'로 좀비를 멈출 수 있습니다.");
				gotoxy(42, 17);
				textcolor(BLACK, WHITE);
				printf("아이템");
				textcolor(WHITE, BLACK);
				gotoxy(10, 19);
				printf("■:속도증가  □:속도 감소  §:백신  ‡:총알               ");
				gotoxy(13, 28); printf("          ");
				gotoxy(13, 28); printf("1단계");
				gotoxy(43, 28); printf("[2단계]");
				gotoxy(73, 28); printf("          ");
				gotoxy(73, 28); printf("3단계");

				break;
			case 2:
					gotoxy(10, 9);
					printf("                                                     ");
					gotoxy(10, 11);
					printf("                                                     ");
					gotoxy(10, 13);
					printf("                                                      ");
					gotoxy(10, 15);
					printf("                                                       ");
					gotoxy(10, 17);
					printf("                                                     ");
					gotoxy(10, 19);
					printf("                                                     ");
					gotoxy(42, 5);
					textcolor(BLACK, WHITE);
					printf("게임 플레이");
					textcolor(WHITE, BLACK);
					gotoxy(10, 9);
					printf("1.좀비를 피해 백신을 5개 먼저 먹는 사람이 승리합니다.");
					gotoxy(10, 11);
					printf("2.하트가 모두 소진 되면 사망합니다.");
					gotoxy(10, 13);
					printf("3.1p는 키보드 '스테이스바'와 'M'으로 두마리의 좀비만 멈출 수 있고");
					gotoxy(10, 15);
					printf("  2p는 'G''H'로 두마리의 좀비만 멈출 수 있습니다.");
					gotoxy(10, 17);
					do {
						c1 = rand() % 16;
						c2 = rand() % 16;
					} while (c1 == c2);
					textcolor(c1, BLACK);
					printf("4. 이제 박스아이템에서 좀비도 아이템을 먹을 수 있습니다.");
					textcolor(WHITE, BLACK);
					gotoxy(10, 19);
					printf("5.좀비가 박스아이템에서 백신을 3개 먹으면 승자 없이 좀비가 승리합니다.");
					gotoxy(42, 21);
					textcolor(BLACK, WHITE);
					printf("아이템");
					textcolor(WHITE, BLACK);
					gotoxy(10, 23);
					printf("■: 속도증가, 속도감소, 백신 , 총알 중 랜덤으로 들어있습니다.");
					gotoxy(13, 28); printf("           ");
					gotoxy(13, 28); printf("1단계");
					gotoxy(43, 28); printf("           ");
					gotoxy(43, 28); printf("2단계");
					gotoxy(73, 28); printf("[3단계]");
					break;
				

			default: break;
			}
		}
		Sleep(100);
	}
}
void show_key(int x, int y)
{
	gotoxy(x, y);
	textcolor(MAGENTA1, BLACK);
	printf(KEY);
	textcolor(BLACK, WHITE);
}
void init_game() //인게임
{
	int x, y;
	char cmd[100];

	int LIFE = 5; //생명
	int LIFE2 = 5;
	srand(time(NULL));
	score[0] = score[1] = 0;
	called[0] = called[1] = 0;
	for (x = 0; x < WIDTH; x++)
		for (y = 0; y < HEIGHT; y++)
			item[x][y] = 0;

	time_out = 60;
	keep_moving = 1;
}

//1p

char map1[HEIGHT][WIDTH + 23] = {
		{"111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111"},
		{"110000000000000000000000000000000000000000000000000000000000000000000000000000000000000000110000000000000001"},
		{"110000000000000000000000000000000000000000000000000000000000000000000000000000000000000000110000000000000001"},
		{"110000000000000000000000000000000000000000000000000000000000000000000000000000000000000000110000000000000001"},
		{"110000000000000000000000000000000000000000000000000000000000000000000000000000000000000000110000000000000001"},
		{"110000000000000000000000000000000000000000000000000000000000000000000000000000000000000000110000000000000001"},
		{"110000000000000000000000000000000000000000000000000000000000000000000000000000000000000000110000000000000001"},
		{"110000000000000000000000000000000000000000000000000000000000000000000000000000000000000000110000000000000001"},
		{"110000000000000000000000000000000000000000000000000000000000000000000000000000000000000000110000000000000001"},
		{"110000000000000000000000000000000000000000000000000000000000000000000000000000000000000000110000000000000001"},
		{"110000000000000000000000000000000000000000000000000000000000000000000000000000000000000000110000000000000001"},
		{"110000000000000000000000000000000000000000000000000000000000000000000000000000000000000000110000000000000001"},
		{"110000000000000000000000000000000000000000000000000000000000000000000000000000000000000000110000000000000001"},
		{"110000000000000000000000000000000000000000000000000000000000000000000000000000000000000000110000000000000001"},
		{"110000000000000000000000000000000000000000000000000000000000000000000000000000000000000000110000000000000001"},
		{"110000000000000000000000000000000000000000000000000000000000000000000000000000000000000000110000000000000001"},
		{"110000000000000000000000000000000000000000000000000000000000000000000000000000000000000000110000000000000001"},
		{"110000000000000000000000000000000000000000000000000000000000000000000000000000000000000000110000000000000001"},
		{"110000000000000000000000000000000000000000000000000000000000000000000000000000000000000000110000000000000001"},
		{"110000000000000000000000000000000000000000000000000000000000000000000000000000000000000000110000000000000001"},
		{"110000000000000000000000000000000000000000000000000000000000000000000000000000000000000000110000000000000001"},
		{"110000000000000000000000000000000000000000000000000000000000000000000000000000000000000000110000000000000001"},
		{"110000000000000000000000000000000000000000000000000000000000000000000000000000000000000000110000000000000001"},
		{"110000000000000000000000000000000000000000000000000000000000000000000000000000000000000000110000000000000001"},
		{"110000000000000000000000000000000000000000000000000000000000000000000000000000000000000000110000000000000001"},
		{"110000000000000000000000000000000000000000000000000000000000000000000000000000000000000000110000000000000001"},
		{"110000000000000000000000000000000000000000000000000000000000000000000000000000000000000000110000000000000001"},
		{"110000000000000000000000000000000000000000000000000000000000000000000000000000000000000000110000000000000001"},
		{"110000000000000000000000000000000000000000000000000000000000000000000000000000000000000000111111111111111111"},
		{"110000000000000000000000000000000000000000000000000000000000000000000000000000000000000000110000000000000001"},
		{"110000000000000000000000000000000000000000000000000000000000000000000000000000000000000000110000000000000001"},
		{"110000000000000000000000000000000000000000000000000000000000000000000000000000000000000000110000000000000001"},
		{"110000000000000000000000000000000000000000000000000000000000000000000000000000000000000000110000000000000001"},
		{"110000000000000000000000000000000000000000000000000000000000000000000000000000000000000000110000000000000001"},
		{"110000000000000000000000000000000000000000000000000000000000000000000000000000000000000000110000000000000001"},
		{"111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111"}
};
void drawmap(int* x, int* y, int* mx, int* my) {
	cls(BLACK, WHITE);
	int h, w;
	for (h = 0; h < HEIGHT; h++) {
		for (w = 0; w < WIDTH + 23; w++) {
			char tmp = map1[h][w];
			if (tmp == '0') {
				printf(" ");
			}
			else if (tmp == '5') {
				*x = w;
				*y = h;
				textcolor(GREEN1, BLACK);
				printf("@");
				textcolor(WHITE, BLACK);
			}
			else if (tmp == '9') {
				*mx = w;
				*my = h;
				textcolor(RED1, BLACK);
				printf("&");
				textcolor(WHITE, BLACK);
			}

			/*else if (tmp == '6') {
				*x = w;
				*y = h;
				textcolor(GREEN1, BLACK);
				printf("§");
				textcolor(WHITE, BLACK);
			}*/
			else if (tmp == '1') {
				textcolor(WHITE, WHITE);
				printf("#");
				textcolor(WHITE, BLACK);

			}
		}
		printf("\n");
	}

}
void showmon(int oldx, int oldy, int newx, int newy) {
	int i = 0;
	if (i == 0) {
		textcolor(RED1, BLACK);
		gotoxy(oldx, oldy);
		printf("  ");
		gotoxy(oldx, oldy + 1);
		printf("  ");
		gotoxy(newx, newy);
		printf("◎");
		gotoxy(newx, newy + 1);
		printf("▣");
	}
	else {
		gotoxy(oldx, oldy);
		printf("  ");
		gotoxy(oldx, oldy + 1);
		printf("  ");
		gotoxy(newx, newy);
		printf("  ");
		gotoxy(newx, newy + 1);
		printf("  ");
	}

}
void erasemon(int oldx, int oldy, int newx, int newy) {
	gotoxy(oldx, oldy);
	printf("  ");
	gotoxy(oldx, oldy + 1);
	printf("  ");
	gotoxy(newx, newy);
	printf("  ");
	gotoxy(newx, newy + 1);
	printf("  ");

}
void showbullet(int oldx, int oldy, int newx, int newy) {
	textcolor(WHITE, BLACK);
	gotoxy(oldx, oldy);
	printf("  ");
	gotoxy(newx, newy);
	printf("＊");
}
void erasebullet(int oldx, int oldy, int newx, int newy) {
	gotoxy(oldx, oldy);
	printf("  ");
	gotoxy(newx, newy);
	printf("  ");
}
void movemon(int oldx, int oldy, int mon_ox, int mon_oy, int mon_nx, int mon_ny) {

	if ((oldx - mon_ox) > 0)
		mon_nx += 1;
	else if ((oldx - mon_ox) < 0)
		mon_nx -= 1;
	if ((oldy - mon_oy) > 0)
		mon_ny += 1;
	else if ((oldy - mon_oy) < 0)
		mon_ny -= 1;
	showmon(mon_ox, mon_oy, mon_nx, mon_ny);
	mon_ox = mon_nx;
	mon_oy = mon_ny;

}
void show_item() { //랜덤 아이템
	int x, y;
	x = rand() % (WIDTH - 5) + 2;
	y = rand() % (HEIGHT - 4) + 2;  // 제일 상단은 피한다
	textcolor(YELLOW2, BLACK);
	gotoxy(x, y);
	printf("■");
	item[x][y] = 1;
	textcolor(WHITE, BLACK);
}
void show_item2() { //랜덤 아이템2
	int x, y;
	x = rand() % (WIDTH - 5) + 2;
	y = rand() % (HEIGHT - 4) + 2;  // 제일 상단은 피한다
	textcolor(YELLOW2, BLACK);
	gotoxy(x, y);
	printf("□");
	item2[x][y] = 1;
	textcolor(WHITE, BLACK);
}
void show_item3() { //랜덤 아이템3
	int x, y;
	x = rand() % (WIDTH - 5) + 2;
	y = rand() % (HEIGHT - 4) + 2;  // 제일 상단은 피한다
	textcolor(YELLOW1, BLACK);
	gotoxy(x, y);
	printf("‡");
	item3[x][y] = 1;
	textcolor(WHITE, BLACK);
}
void show_item4() { //랜덤 아이템4
	int x, y;
	x = rand() % (WIDTH - 5) + 2;
	y = rand() % (HEIGHT - 4) + 2;  // 제일 상단은 피한다
	textcolor(MAGENTA2, BLACK);
	gotoxy(x, y);
	printf("§");
	item4[x][y] = 1;
	textcolor(WHITE, BLACK);
}

void show_item5() { //랜덤 아이템
	int x, y;
	x = rand() % (WIDTH - 5) + 2;
	y = rand() % (HEIGHT - 4) + 2;  // 제일 상단은 피한다
	textcolor(YELLOW2, BLACK);
	gotoxy(x, y);
	printf("■");
	item5[x][y] = 1;
	textcolor(WHITE, BLACK);
}
void show_item6() { //랜덤 아이템2
	int x, y;
	x = rand() % (WIDTH - 5) + 2;
	y = rand() % (HEIGHT - 4) + 2;  // 제일 상단은 피한다
	textcolor(YELLOW2, BLACK);
	gotoxy(x, y);
	printf("■");
	item6[x][y] = 1;
	textcolor(WHITE, BLACK);
}

void show_item7() { //랜덤 아이템4
	int x, y;
	x = rand() % (WIDTH - 5) + 2;
	y = rand() % (HEIGHT - 4) + 2;  // 제일 상단은 피한다
	textcolor(YELLOW2, BLACK);
	gotoxy(x, y);
	printf("■");
	item7[x][y] = 1;
	textcolor(WHITE, BLACK);
}
void show_item8() { //랜덤 아이템4
	int x, y;
	x = rand() % (WIDTH - 5) + 2;
	y = rand() % (HEIGHT - 4) + 2;  // 제일 상단은 피한다
	textcolor(YELLOW2, BLACK);
	gotoxy(x, y);
	printf("■");
	item8[x][y] = 1;
	textcolor(WHITE, BLACK);
}
int gloop1(int mapcode) {  //1단계

START:
	removeCursor();
	int mon_frame_sync = 30;

	int v_size = 0;
	int v_size2 = 0;
	int oldx1 = 82, oldy1 = 2;
	int newx1 = 82, newy1 = 2;
	int oldx2 = 86, oldy2 = 4;
	int newx2 = 86, newy2 = 4;
	int x = 4, y = 31;
	int	mx, my;
	unsigned char ch;
	unsigned char ch1;
	unsigned char ch2;
	int playing = 1;
	int mon_ox = 10, mon_oy = 28; // monster 좌표
	int mon_nx = 10, mon_ny = 28;
	int mon_ox2 = 40, mon_oy2 = 25; // monster 좌표
	int mon_nx2 = 40, mon_ny2 = 25;
	int mon_ox3 = 35, mon_oy3 = 25; // monster 좌표
	int mon_nx3 = 35, mon_ny3 = 25;
	int mon_ox4 = 41, mon_oy4 = 30; // monster 좌표
	int mon_nx4 = 41, mon_ny4 = 30;
	int frame_count = 0;
	int keep_moving;
	int p1_frame_sync = 7; // 처음 시작은 10 frame 마다 이동, 즉, 100msec 마다 이동

	drawmap(&x, &y, &mx, &my);
	showplayer1(oldx1, oldy1, newx1, newy1);
	showplayer2(oldx2, oldy2, newx2, newy2);
	show_key(x, y);
	ch1 = ch2 = 0; // 초기값 정지상태
	keep_moving = 1;
	while (1) {
		if (kbhit() == 1) {  // 키보드가 눌려져 있으면
			ch = getch(); // key 값을 읽는다
			if (ch == ESC) {
				cls(BLACK, WHITE);
				break;
			}
			// ESC 누르면 프로그램 종료 추가
			//
			if (ch == SPECIAL1 || ch == SPECIAL2) { // 만약 특수키
				// 예를 들어 UP key의 경우 0xe0 0x48 두개의 문자가 들어온다.
				ch1 = getch();
				switch (ch1) {
				case UP:
				case DOWN:
				case LEFT:
				case RIGHT:
					keep_moving = 1;
					break;
				default: // 방향키가 아니면 멈춘다
					keep_moving = 1;
				}
			}
			else {
				ch2 = ch;
				// Player2은 AWSD 로 움직인다.
				switch (ch2) {
				case UP2:
				case DOWN2:
				case LEFT2:
				case RIGHT2://player2만 방향 전환
					keep_moving = 1;

					break;
				default:// 방향 전환이 아니면
					keep_moving = 1;

				}
				// 특수 문자가 아니지만 AWSD를 방향키 대신 사용하는 경우 처리
			}
		}
		// 1p monster의 움직임 처리
		if (frame_count % mon_frame_sync == 0) {
			if ((oldx1 - mon_ox) > 0)
				mon_nx += 1;
			else if ((oldx1 - mon_ox) < 0)
				mon_nx -= 1;
			if ((oldy1 - mon_oy) > 0)
				mon_ny += 1;
			else if ((oldy1 - mon_oy) < 0)
				mon_ny -= 1;
			showmon(mon_ox, mon_oy, mon_nx, mon_ny);
			mon_ox = mon_nx;
			mon_oy = mon_ny;
			if ((oldx1 - mon_ox2) > 0)
				mon_nx2 += 1;
			else if ((oldx1 - mon_ox2) < 0)
				mon_nx2 -= 1;
			if ((oldy1 - mon_oy2) > 0)
				mon_ny2 += 1;
			else if ((oldy1 - mon_oy2) < 0)
				mon_ny2 -= 1;
			showmon(mon_ox2, mon_oy2, mon_nx2, mon_ny2);
			mon_ox2 = mon_nx2;
			mon_oy2 = mon_ny2;
			//2p 몬스터 움직임
			if ((oldx2 - mon_ox3) > 0)
				mon_nx3 += 1;
			else if ((oldx2 - mon_ox3) < 0)
				mon_nx3 -= 1;
			if ((oldy2 - mon_oy3) > 0)
				mon_ny3 += 1;
			else if ((oldy2 - mon_oy3) < 0)
				mon_ny3 -= 1;
			showmon(mon_ox3, mon_oy3, mon_nx3, mon_ny3);
			mon_ox3 = mon_nx3;
			mon_oy3 = mon_ny3;
			if ((oldx2 - mon_ox4) > 0)
				mon_nx4 += 1;
			else if ((oldx2 - mon_ox4) < 0)
				mon_nx4 -= 1;
			if ((oldy2 - mon_oy4) > 0)
				mon_ny4 += 1;
			else if ((oldy2 - mon_oy4) < 0)
				mon_ny4 -= 1;
			showmon(mon_ox4, mon_oy4, mon_nx4, mon_ny4);
			mon_ox4 = mon_nx4;
			mon_oy4 = mon_ny4;
		}
		if (keep_moving && frame_count % p1_frame_sync == 0) { // 1p 움직이고 있으면			
			switch (ch1) {
			case UP:
				if (oldy1 > 2)
					newy1 = oldy1 - 1;
				break;
			case DOWN:
				if (oldy1 < HEIGHT - 4)
					newy1 = oldy1 + 1;
				break;
			case LEFT:
				if (oldx1 > 2)
					newx1 = oldx1 - 1;
				break;
			case RIGHT:
				if (oldx1 < WIDTH - 4)
					newx1 = oldx1 + 1;
				break;
			}

			switch (ch2) { //2p
			case UP2:
				if (oldy2 > 2)
					newy2 = oldy2 - 1;
				break;
			case DOWN2:
				if (oldy2 < HEIGHT - 4)
					newy2 = oldy2 + 1;
				break;
			case LEFT2:
				if (oldx2 > 2)
					newx2 = oldx2 - 1;
				break;
			case RIGHT2:
				if (oldx2 < WIDTH - 4)
					newx2 = oldx2 + 1;
				break;
			}
		}
		showplayer1(oldx1, oldy1, newx1, newy1); // Player 이동 표시
		oldx1 = newx1; // 마지막 위치를 기억한다.
		oldy1 = newy1;
		showplayer2(oldx2, oldy2, newx2, newy2); // Player 이동 표시
		oldx2 = newx2; // 마지막 위치를 기억한다.
		oldy2 = newy2;
		keep_moving = 1; //1:계속이동, 0:한번에 한칸씩이동
		if ((newx1 == mon_ox && newy1 == mon_ny) || (newx1 == mon_ox2 && newy1 == mon_ny2) || (newx2 == x && newy2 == y)|| (newx2 == x && newy2+1 == y)) {
				cls(BLACK, WHITE);
			while (1) {
				int c1, c2;
				do { // 색을 변경하면서 Game Over 출력
					c1 = rand() % 16;
					c2 = rand() % 16;
				} while (c1 == c2);
				gotoxy(50, 20);
				printf("2p win");
				textcolor(c1, c2);
				gotoxy(45, 10);
				printf("** Game Over **");
				gotoxy(20, 13);
				textcolor(WHITE, BLACK);
				printf("'R'을 누르면 재시작 하고 'q'를 누르면 난이도 선택 메뉴로 넘어갑니다");
				Sleep(300);
				if (kbhit()) {
					ch = getch();
					if (ch == 'r' || ch == 'q')
						break;
				}
			}
			if (ch == 'r')
				goto START;
			else if (ch == 'q') {
				cls(BLACK, WHITE);

				return 0;
			}
			gotoxy(0, HEIGHT - 1);
		}

		else if ((newx2 == mon_ox3 && newy2 == mon_ny3) || (newx2 == mon_ox4 && newy2 == mon_ny4) || (newx1 == x && newy1 == y) ||(newx1 == x && newy1+1 == y)) {
			cls(BLACK, WHITE);
			while (1) {
				int c1, c2;
				do { // 색을 변경하면서 Game Over 출력
					c1 = rand() % 16;
					c2 = rand() % 16;
				} while (c1 == c2);
				gotoxy(50, 20);
				printf("1p win");
				textcolor(c1, c2);
				gotoxy(45, 10);
				printf("** Game Over **");
				gotoxy(24, 13);
				textcolor(WHITE, BLACK);
				printf("'R'을 누르면 재시작 하고 'q'를 누르면 난이도 선택 메뉴로 넘어갑니다");
				Sleep(300);
				if (kbhit()) {
					ch = getch();
					if (ch == 'r' || ch == 'q')
						break;
				}
			}
			if (ch == 'r')
				goto START;
			else if (ch == 'q') {
				cls(BLACK, WHITE);
				return 0;
			}
			gotoxy(0, HEIGHT - 1);
		}
		Sleep(Delay); // Delay를 줄이면 속도가 빨라진다.
		frame_count++;

	}

}
int gloop2(int mapcode) { //2단계

START:
	init_game();
	int mon_frame_sync = 20;

	int iteminterval = 5;
	int iteminterval2 = 9;
	int v_size = 0;
	int v_size2 = 0;
	int LIFE = 5; //생명
	int LIFE2 = 5;
	int bullet = 5;
	int delay = 10;
	int SPEED = 1;
	int SPEED2 = 1;
	int run_time, start_time, remain_time, last_remain_time;
	int run_time2, start_time2, remain_time2, last_remain_time2;
	int gold_time = 0;
	int gold_time2 = 0;
	start_time = time(NULL);
	start_time2 = time(NULL);

	last_remain_time = remain_time = time_out;
	last_remain_time2 = remain_time2 = time_out;
	int bullet_size2 = 0;
	int oldx1 = 82, oldy1 = 2;
	int newx1 = 82, newy1 = 2;
	int oldx2 = 86, oldy2 = 4;
	int newx2 = 86, newy2 = 4;
	int x = 4, y = 30;
	int	mx, my;
	int i;
	unsigned char ch;
	unsigned char ch1;
	unsigned char ch2;	int playing = 1;
	ch1 = ch2 = 0;
	int box = -1, bnx = -1;
	int boy = -1, bny = -1;
	int box2 = -1, bnx2 = -1;
	int boy2 = -1, bny2 = -1;
	int box3 = -1, bnx3 = -1;
	int boy3 = -1, bny3 = -1;
	int box4 = -1, bnx4 = -1;
	int boy4 = -1, bny4 = -1;
	int mon_ox = 10, mon_oy = 30; // monster 좌표
	int mon_nx = 10, mon_ny = 30;
	int mon_ox2 = 40, mon_oy2 = 30; // monster 좌표
	int mon_nx2 = 40, mon_ny2 = 30;
	int mon_ox3 = 35, mon_oy3 = 31; // monster 좌표
	int mon_nx3 = 35, mon_ny3 = 31;
	int mon_ox4 = 51, mon_oy4 = 32; // monster 좌표
	int mon_nx4 = 51, mon_ny4 = 32;
	int frame_count = 0;
	int keep_moving=1;
	int mon_life = 5;
	int bullet_size = 0;
	int p1_frame_sync = 10; // 처음 시작은 10 frame 마다 이동, 즉, 100msec 마다 이동
	int p2_frame_sync = 10;
	int p1_speed = 1;
	int p2_speed = 1;
	drawmap(&x, &y, &mx, &my);
	removeCursor();
	showplayer1(oldx1, oldy1, newx1, newy1);
	showplayer2(oldx2, oldy2, newx2, newy2);
	textcolor(WHITE, BLACK);
	ch = 0; // 초기값 정지상태
	keep_moving = 0;
	while (1) {
		run_time = time(NULL) - start_time;
		if (run_time > gold_time && (run_time % iteminterval == 0)) {
			show_item();
			show_item2();
			gold_time = run_time; // 마지막 GOLD 표시 시간 기억
		}
		run_time2 = time(NULL) - start_time2;
		if (run_time2 > gold_time2 && (run_time2 % iteminterval2 == 0)) {
			show_item3();
			show_item4();
			gold_time2 = run_time2; // 마지막 GOLD 표시 시간 기억
		}

		if (kbhit() == 1) {  // 키보드가 눌려져 있으면
			ch = getch(); // key 값을 읽는다
			if (ch == ESC) {// ESC 누르면 프로그램 종료 추가
				cls(BLACK, WHITE);
				break;
			}
			if (ch == SPECIAL1 || ch == SPECIAL2) { // 만약 특수키
				// 예를 들어 UP key의 경우 0xe0 0x48 두개의 문자가 들어온다.
				ch1 = getch();
				switch (ch1) {
				case UP:
				case DOWN:
				case LEFT:
				case RIGHT:
				case SPACE:
				case M: 
					keep_moving = 1;
					break;
				default: // 방향키가 아니면 멈춘다
					keep_moving = 1;
				}
			}
			else {	
				ch2 = ch;// Player2은 AWSD 로 움직인다.
				switch (ch2) {
				case UP2:
				case DOWN2:
				case LEFT2:
				case RIGHT2://player2만 방향 전환
				case G:
				case H:
					keep_moving = 1;
					break;
				default:// 방향 전환이 아니면
					keep_moving = 1;
				}
				// 특수 문자가 아니지만 AWSD를 방향키 대신 사용하는 경우 처리
			
			}
			if (ch == SPACE) { //총알 구현		
				if (bullet_size != 0) {
					bnx = box = oldx1 - 1;
					bny = boy = oldy1;
					showbullet(box, boy, bnx, bny); //총알을 그림
					bullet_size--;
				}
			}
			if (ch == M) { //1p 총
				if (bullet_size != 0) {
					bnx2 = box2 = oldx1 - 1;
					bny2 = boy2 = oldy1;
					showbullet(box2, boy2, bnx2, bny2); //총알을 그림
					bullet_size--;
				}
			}
			if (ch2 == G) {  //2p 총
				if (bullet_size2 != 0) {
					bnx3 = box3 = oldx2 - 1;
					bny3 = boy3 = oldy2;
					showbullet(box3, boy3, bnx3, bny3); //총알을 그림
					bullet_size2--;
				}
			}
			if (ch2 == H) {  //2p 총
				if (bullet_size2 != 0) {
					bnx4 = box4 = oldx2 - 1;
					bny4 = boy4 = oldy2;
					showbullet(box4, boy4, bnx4, bny4); //총알을 그림
					bullet_size2--;
				}
			}
		}

		if (keep_moving && frame_count % p1_frame_sync == 0) { // 1p 움직이고 있으면			
			switch (ch1) {
			case UP:
				if (oldy1 > 2)
					newy1 = oldy1 - SPEED;
				if (ch == SPACE) {
					if (boy > 2)
						bny = boy - 1;
				}
				keep_moving = 1;
				break;
			case DOWN:
				if (oldy1 < HEIGHT - 4)
					newy1 = oldy1 + SPEED;
				if (ch == SPACE) {
					if (boy > 2)
						bny = boy + 1;
				}
				keep_moving = 1;
				break;
			case LEFT:
				if (oldx1 > 2)
					newx1 = oldx1 - SPEED;
				if (ch == SPACE) {
					if (box > 2)
						bnx = box - 1;
				}
				keep_moving = 1;

				break;
			case RIGHT:
				if (oldx1 < WIDTH - 4)
					newx1 = oldx1 + SPEED;
				if (ch == SPACE) {
					if (box > 2)
						bnx = box - 1;
				}
				keep_moving = 1;
				break;
			}
		}
			if (keep_moving && frame_count % p2_frame_sync == 0) {
			switch (ch2) {  //2p
			case UP2:
				if (oldy2 > 2)
					newy2 = oldy2 - SPEED2;
				break;
			case DOWN2:
				if (oldy2 < HEIGHT - 4)
					newy2 = oldy2 + SPEED2;
				break;
			case LEFT2:
				if (oldx2 > 2)
					newx2 = oldx2 - SPEED2;
				break;
			case RIGHT2:
				if (oldx2 < WIDTH - 4)
					newx2 = oldx2 + SPEED2;
				break;
			}

		}
		showplayer1(oldx1, oldy1, newx1, newy1); // Player 이동 표시
		oldx1 = newx1; // 마지막 위치를 기억한다.
		oldy1 = newy1;
		keep_moving = 1; //1:계속이동, 0:한번에 한칸씩이동
		showplayer2(oldx2, oldy2, newx2, newy2); // Player 이동 표시
		oldx2 = newx2; // 마지막 위치를 기억한다.
		oldy2 = newy2;
		keep_moving = 1; //1:계속이동, 0:한번에 한칸씩이동

		if (box != -1 && frame_count % bullet_frame_sync == 0) {// 불렛이 몬스터와 닿으면 추적 중지
			// // 아래 총알의 움직임 처리
			if ((mon_ox - box) > 0) {
				bnx = box + 1;
			}
			else if ((mon_ox - box) < 0) {
				bnx = box - 1;
			}
			if ((mon_oy - boy) > 0) {
				bny = boy + 1;
			}
			else if ((mon_oy - boy) < 0) {
				bny = boy - 1;
			}
			if (box == mon_ox && boy == mon_oy) {
				//mon_ox = box;
				//mon_oy = boy;
				erasemon(mon_ox, mon_oy, mon_nx, mon_ny);
				box = -1;
				erasebullet(mon_ox, mon_oy, mon_nx, mon_ny);
			}
			else {
				showbullet(box, boy, bnx, bny); //총알을 그림
				box = bnx;
				boy = bny;
			}
			showbullet(box, boy, bnx, bny); //총알을 그림
			box = bnx;
			boy = bny;
		}
		if (box2 != -1 && frame_count % bullet_frame_sync == 0) {// 2불렛이 2몬스터와 닿으면 추적 중지
			// // 아래 총알의 움직임 처리
			if ((mon_ox2 - box2) > 0) {
				bnx2 = box2 + 1;
			}
			else if ((mon_ox2 - box2) < 0) {
				bnx2 = box2 - 1;
			}
			if ((mon_oy2 - boy2) > 0) {
				bny2 = boy2 + 1;
			}
			else if ((mon_oy2 - boy2) < 0) {
				bny2 = boy2 - 1;
			}
			if (box2 == mon_ox2 && boy2 == mon_oy2) {
				//mon_ox = box;
				//mon_oy = boy;
				erasemon(mon_ox2, mon_oy2, mon_nx2, mon_ny2);
				box2 = -1;
				erasebullet(mon_ox2, mon_oy2, mon_nx2, mon_ny2);
			}
			else {
				showbullet(box2, boy2, bnx2, bny2); //총알을 그림
				box2 = bnx2;
				boy2 = bny2;
			}
			showbullet(box2, boy2, bnx2, bny2); //총알을 그림
			box2 = bnx2;
			boy2 = bny2;
		}
		if (box3 != -1 && frame_count % bullet_frame_sync == 0) {// 2불렛이 2몬스터와 닿으면 추적 중지
			// // 아래 총알의 움직임 처리
			if ((mon_ox3 - box3) > 0) {
				bnx3 = box3 + 1;
			}
			else if ((mon_ox3 - box3) < 0) {
				bnx3 = box3 - 1;
			}
			if ((mon_oy3 - boy3) > 0) {
				bny3 = boy3 + 1;
			}
			else if ((mon_oy3 - boy3) < 0) {
				bny3 = boy3 - 1;
			}
			if (box3 == mon_ox3 && boy3 == mon_oy3) {
				//mon_ox = box;
				//mon_oy = boy;
				erasemon(mon_ox3, mon_oy3, mon_nx3, mon_ny3);
				box3 = -1;
				erasebullet(mon_ox3, mon_oy3, mon_nx3, mon_ny3);
			}
			else {
				showbullet(box3, boy3, bnx3, bny3); //총알을 그림
				box3 = bnx3;
				boy3 = bny3;
			}
			showbullet(box3, boy3, bnx3, bny3); //총알을 그림
			box3 = bnx3;
			boy3 = bny3;
		}
		if (box4 != -1 && frame_count % bullet_frame_sync == 0) {// 2불렛이 2몬스터와 닿으면 추적 중지
			// // 아래 총알의 움직임 처리
			if ((mon_ox4 - box4) > 0) {
				bnx4 = box4 + 1;
			}
			else if ((mon_ox4 - box4) < 0) {
				bnx4 = box4 - 1;
			}
			if ((mon_oy4 - boy4) > 0) {
				bny4 = boy4 + 1;
			}
			else if ((mon_oy4 - boy4) < 0) {
				bny4 = boy4 - 1;
			}
			if (box4 == mon_ox4 && boy4 == mon_oy4) {
				//mon_ox = box;
				//mon_oy = boy;
				erasemon(mon_ox4, mon_oy4, mon_nx4, mon_ny4);
				box4 = -1;
				erasebullet(mon_ox4, mon_oy4, mon_nx4, mon_ny4);
			}
			else {
				showbullet(box4, boy4, bnx4, bny4); //총알을 그림
				box4 = bnx4;
				boy4 = bny4;
			}
			showbullet(box4, boy4, bnx4, bny4); //총알을 그림
			box4 = bnx4;
			boy4 = bny4;
		}
		textcolor(WHITE, BLACK);
		gotoxy(98, 2);
		printf("1p");
		gotoxy(96, 7);
		printf("MAX 10");
		gotoxy(98, 16);
		printf("2p");
		gotoxy(96, 21);
		printf("MAX 10");
		if (item4[oldx1][oldy1] || item4[oldx1][oldy1 + 1]) { //1p 백신 증가
			item4[oldx1][oldy1] = 0;
			item4[oldx1][oldy1 + 1] = 0;
			v_size++;

		}
		if (item4[oldx2][oldy2] || item4[oldx2][oldy2 + 1]) {	//2p 백신 증가

			item4[oldx2][oldy2] = 0;
			item4[oldx2][oldy2 + 1] = 0;
			v_size2++;
		}
		if (v_size == 1) {
			textcolor(MAGENTA2, BLACK);
			gotoxy(95, 11);
			printf("§");
			textcolor(WHITE, BLACK);

		}
		if (v_size == 2) {
			textcolor(MAGENTA2, BLACK);
			gotoxy(95, 11);
			printf("§§");
			textcolor(WHITE, BLACK);

		}
		if (v_size == 3) {
			textcolor(MAGENTA2, BLACK);
			gotoxy(95, 11);
			printf("§§§");
			textcolor(WHITE, BLACK);

		}
		if (v_size == 4) {
			textcolor(MAGENTA2, BLACK);
			gotoxy(95, 11);
			printf("§§§§");
			textcolor(WHITE, BLACK);

		}
		if (v_size2 == 1) {
			textcolor(MAGENTA2, BLACK);
			gotoxy(95, 25);
			printf("§");
			textcolor(WHITE, BLACK);

		}
		if (v_size2 == 2) {
			textcolor(MAGENTA2, BLACK);
			gotoxy(95, 25);
			printf("§§");
			textcolor(WHITE, BLACK);

		}
		if (v_size2 == 3) {
			textcolor(MAGENTA2, BLACK);
			gotoxy(95, 25);
			printf("§§§");
			textcolor(WHITE, BLACK);

		}
		if (v_size2 == 4) {
			textcolor(MAGENTA2, BLACK);
			gotoxy(95, 25);
			printf("§§§§");
			textcolor(WHITE, BLACK);

		}

		if (item[oldx1][oldy1] || item[oldx1][oldy1 + 1]) { //1p 아이템 먹으면 속도 증가
			if (p1_frame_sync != 1) {
				item[oldx1][oldy1] = 0;
				item[oldx1][oldy1 + 1] = 0;
				p1_frame_sync--;
				p1_speed++;

			}
		}
		if (item[oldx2][oldy2] || item[oldx2][oldy2 + 1]) {				//2p 아이템 먹으면 속도 증가
			if (p2_frame_sync != 1) {
				item[oldx2][oldy2] = 0;
				item[oldx2][oldy2 + 1] = 0;
				p2_frame_sync -= 1;
				p2_speed++;
			}
		}
		if (item2[oldx1][oldy1] || item2[oldx1][oldy1 + 1]) { //1p 아이템 먹으면 속도 감소
			if (p1_frame_sync != 11) {
				item2[oldx1][oldy1] = 0;
				item2[oldx1][oldy1 + 1] = 0;
				p1_frame_sync++;
				p1_speed--;
			}
		}
		if (item2[oldx2][oldy2] || item2[oldx2][oldy2 + 1]) {				//2p 아이템 먹으면 속도 감소
			if (p2_frame_sync != 11) {
				item2[oldx2][oldy2] = 0;
				item2[oldx2][oldy2 + 1] = 0;
				p2_frame_sync += 1;
				p2_speed--;
			}
		}
		if (item3[oldx1][oldy1] || item3[oldx1][oldy1 + 1]) { //1p 총알 갯수 증가
			if (bullet_size != 2) {
				item3[oldx1][oldy1] = 0;
				item3[oldx1][oldy1 + 1] = 0;
				bullet_size++;
			}
		}
		if (item3[oldx2][oldy2] || item3[oldx2][oldy2 + 1]) {	//2p 총알 갯수 증가
			if (bullet_size != 2) {
				item3[oldx2][oldy2] = 0;
				item3[oldx2][oldy2 + 1] = 0;
				bullet_size2++;
			}
		}
		if (bullet_size == 0) {
			gotoxy(97, 9);
			printf("총알");
			gotoxy(97, 10);
			printf("    ");
		}

		if (bullet_size == 1) {
			gotoxy(97, 10);
			printf("    ");
			gotoxy(97, 10);
			printf("ⅰ");
		}
		if (bullet_size == 2) {
			gotoxy(97, 10);
			printf("    ");
			gotoxy(97, 10);
			printf("ⅰⅰ");
		}
		if (bullet_size == 0) {
			gotoxy(97, 23);
			printf("총알");
			gotoxy(97, 24);
			printf("     ");
	
		}

		if (bullet_size2 == 1) {
			gotoxy(97, 24);
			printf("     ");
			gotoxy(97, 24);
			printf("ⅰ");
		}
		if (bullet_size2 == 2) {
			gotoxy(97, 24);
			printf("     ");
			gotoxy(97, 24);
			printf("ⅰⅰ");
		}
		if (p1_speed == 1) {
			gotoxy(95, 6);
			printf("속도 : %d", p1_speed);
		}
		else if (p1_speed == 2) {
			gotoxy(95, 6);
			printf("속도 : %d", p1_speed);
		}
		else if (p1_speed == 3) {
			gotoxy(95, 6);
			printf("속도 : %d", p1_speed);
		}
		else if (p1_speed == 4) {
			gotoxy(95, 6);
			printf("속도 : %d", p1_speed);
		}
		else if (p1_speed == 5) {
			gotoxy(95, 6);
			printf("속도 : %d", p1_speed);
		}
		else if (p1_speed == 6) {
			gotoxy(95, 6);
			printf("속도 : %d", p1_speed);
		}
		else if (p1_speed == 7) {
			gotoxy(95, 6);
			printf("속도 : %d", p1_speed);
		}
		else if (p1_speed == 8) {
			gotoxy(95, 6);
			printf("속도 : %d", p1_speed);
		}
		else if (p1_speed == 9) {
			gotoxy(95, 6);
			printf("           ");
			gotoxy(95, 6);
			printf("속도 : %d", p1_speed);
		}
		else if (p1_speed == 10) {
			gotoxy(95, 6);
			printf("속도 : %d", p1_speed);
		}
		if (p2_speed == 1) {
			gotoxy(95, 20);
			printf("속도 : %d", p2_speed);
		}
		else if (p2_speed == 2) {
			gotoxy(95, 20);
			printf("속도 : %d", p2_speed);
		}
		else if (p2_speed == 3) {
			gotoxy(95, 20);
			printf("속도 : %d", p2_speed);
		}
		else if (p2_speed == 4) {
			gotoxy(95, 20);
			printf("속도 : %d", p2_speed);
		}
		else if (p2_speed == 5) {
			gotoxy(95, 20);
			printf("속도 : %d", p2_speed);
		}
		else if (p2_speed == 6) {
			gotoxy(95, 20);
			printf("속도 : %d", p2_speed);
		}
		else if (p2_speed == 7) {
			gotoxy(95, 20);
			printf("속도 : %d", p2_speed);
		}
		else if (p2_speed == 8) {
			gotoxy(95, 20);
			printf("속도 : %d", p2_speed);

		}
		else if (p2_speed == 9) {
			gotoxy(95, 20);
			printf("          ");
			gotoxy(95, 20);
			printf("속도 : %d", p2_speed);
		}
		else if (p2_speed == 10) {
			gotoxy(95, 20);
			printf("         ");
			gotoxy(95, 20);
			printf("속도 : %d", p2_speed);
		}


		// 1p monster의 움직임 처리

		if (frame_count % mon_frame_sync == 0) {
			if (mon_nx != box) {
				if ((oldx1 - mon_ox) > 0)
					mon_nx += 1;
				else if ((oldx1 - mon_ox) < 0)
					mon_nx -= 1;
				if ((oldy1 - mon_oy) > 0)
					mon_ny += 1;
				else if ((oldy1 - mon_oy) < 0)
					mon_ny -= 1;
				showmon(mon_ox, mon_oy, mon_nx, mon_ny);
				mon_ox = mon_nx;
				mon_oy = mon_ny;
			}
		}
		if (frame_count % mon_frame_sync == 0) {
			if (mon_nx2 != box2) {
				if ((oldx1 - mon_ox2) > 0)
					mon_nx2 += 1;
				else if ((oldx1 - mon_ox2) < 0)
					mon_nx2 -= 1;
				if ((oldy1 - mon_oy2) > 0)
					mon_ny2 += 1;
				else if ((oldy1 - mon_oy2) < 0)
					mon_ny2 -= 1;
				showmon(mon_ox2, mon_oy2, mon_nx2, mon_ny2);
				mon_ox2 = mon_nx2;
				mon_oy2 = mon_ny2;
			}
		}
		//2p 몬스터 움직임			
		if (frame_count % mon_frame_sync == 0) {
			if (mon_nx3 != box3) {
				if ((oldx2 - mon_ox3) > 0)
					mon_nx3 += 1;
				else if ((oldx2 - mon_ox3) < 0)
					mon_nx3 -= 1;
				if ((oldy2 - mon_oy3) > 0)
					mon_ny3 += 1;
				else if ((oldy2 - mon_oy3) < 0)
					mon_ny3 -= 1;
				showmon(mon_ox3, mon_oy3, mon_nx3, mon_ny3);
				mon_ox3 = mon_nx3;
				mon_oy3 = mon_ny3;
			}
		}

		if (frame_count % mon_frame_sync == 0) {
			if (mon_nx4 != box4) {

				if ((oldx2 - mon_ox4) > 0)
					mon_nx4 += 1;
				else if ((oldx2 - mon_ox4) < 0)
					mon_nx4 -= 1;
				if ((oldy2 - mon_oy4) > 0)
					mon_ny4 += 1;
				else if ((oldy2 - mon_oy4) < 0)
					mon_ny4 -= 1;
				showmon(mon_ox4, mon_oy4, mon_nx4, mon_ny4);
				mon_ox4 = mon_nx4;
				mon_oy4 = mon_ny4;
			}
		}
		if ((oldx1 == mon_nx && oldy1 == mon_ny) || (oldx1 == mon_nx && oldy1 + 1 == mon_ny)) {  //몬스터 접촉후 무적시간
			if (frame_count % mon_frame_sync == 0) {
				if (mon_nx != box) {
	
					showmon(mon_ox, mon_oy, mon_nx, mon_ny);
					mon_ox = mon_nx;
					mon_oy = mon_ny;
					LIFE = LIFE - 1;
				}
			}
		}
		if ((oldx1 == mon_nx3 && oldy1 == mon_ny3) || (oldx1 == mon_nx3 && oldy1 + 1 == mon_ny3)) {  //몬스터 접촉후 무적시간
			if (frame_count % mon_frame_sync == 0) {
				if (mon_nx3 != box) {
		
					showmon(mon_ox3, mon_oy3, mon_nx3, mon_ny3);
					mon_ox3 = mon_nx3;
					mon_oy3 = mon_ny3;
					LIFE = LIFE - 1;
				}
			}
		}
		if ((oldx1 == mon_nx2 && oldy1 == mon_ny2) || (oldx1 == mon_nx2 && oldy1 + 1 == mon_ny2)) {//몬스터 접촉후 무적시간
			if (frame_count % mon_frame_sync == 0) {
				if (mon_nx2 != box2) {
		
					showmon(mon_ox2, mon_oy2, mon_nx2, mon_ny2);
					mon_ox2 = mon_nx2;
					mon_oy2 = mon_ny2;
					LIFE = LIFE - 1;
				}
			}
		}
		if ((oldx1 == mon_nx4 && oldy1 == mon_ny4) || (oldx1 == mon_nx4 && oldy1 + 1 == mon_ny4)) {//몬스터 접촉후 무적시간
			if (frame_count % mon_frame_sync == 0) {
				if (mon_nx4 != box2) {
	
					showmon(mon_ox4, mon_oy4, mon_nx4, mon_ny4);
					mon_ox4 = mon_nx4;
					mon_oy4 = mon_ny4;
					LIFE = LIFE - 1;
				}
			}
		}
		if ((oldx2 == mon_nx3 && oldy2 == mon_ny3) || (oldx2 == mon_nx3 && oldy2 + 1 == mon_ny3)) {//몬스터 접촉후 무적시간
			if (frame_count % mon_frame_sync == 0) {
				if (mon_nx3 != box3) {

					showmon(mon_ox3, mon_oy3, mon_nx3, mon_ny3);
					mon_ox3 = mon_nx3;
					mon_oy3 = mon_ny3;
					LIFE2 = LIFE2 - 1;
				}
			}
		}

		if ((oldx2 == mon_nx && oldy2 == mon_ny) || (oldx2 == mon_nx && oldy2 + 1 == mon_ny)) {//몬스터 접촉후 무적시간
			if (frame_count % mon_frame_sync == 0) {
				if (mon_nx != box3) {
		
					showmon(mon_ox, mon_oy, mon_nx, mon_ny);
					mon_ox = mon_nx;
					mon_oy = mon_ny;
					LIFE2 = LIFE2 - 1;
				}
			}
		}
		if ((oldx2 == mon_nx4 && oldy2 == mon_ny4) || (oldx2 == mon_nx4 && oldy2 + 1 == mon_ny4)) {//몬스터 접촉후 무적시간
			if (frame_count % mon_frame_sync == 0) {
				if (mon_nx4 != box4) {
				
					showmon(mon_ox4, mon_oy4, mon_nx4, mon_ny4);
					mon_ox4 = mon_nx4;
					mon_oy4 = mon_ny4;
					LIFE2 = LIFE2 - 1;
				}
			}
		}
		if ((oldx2 == mon_nx2 && oldy2 == mon_ny2) || (oldx2 == mon_nx2 && oldy2 + 1 == mon_ny2)) {//몬스터 접촉후 무적시간
			if (frame_count % mon_frame_sync == 0) {
				if (mon_nx2 != box4) {
			
					showmon(mon_ox2, mon_oy2, mon_nx2, mon_ny2);
					mon_ox2 = mon_nx2;
					mon_oy2 = mon_ny2;
					LIFE2 = LIFE2 - 1;
				}
			}
		}
		if (LIFE == 5) {
			textcolor(RED1, BLACK);
			gotoxy(95, 4);
			printf("♥♥♥♥♥");
			textcolor(WHITE, BLACK);
		}
		else if (LIFE == 4) {
			textcolor(RED1, BLACK);
			gotoxy(95, 4);
			printf("♥♥♥♥   ");
			textcolor(WHITE, BLACK);

		}
		else if (LIFE == 3) {
			textcolor(RED1, BLACK);
			gotoxy(95, 4);
			printf("♥♥♥     ");
			textcolor(WHITE, BLACK);

		}
		else if (LIFE == 2) {
			textcolor(RED1, BLACK);
			gotoxy(95, 4);
			printf("♥♥     ");
			textcolor(WHITE, BLACK);

		}
		else if (LIFE == 1) {
			textcolor(RED1, BLACK);
			gotoxy(95, 4);
			printf("♥       ");
			textcolor(WHITE, BLACK);

		}
		if (LIFE2 == 5) {
			textcolor(RED1, BLACK);
			gotoxy(95, 18);
			printf("♥♥♥♥♥");
			textcolor(WHITE, BLACK);
		}
		else if (LIFE2 == 4) {
			textcolor(RED1, BLACK);
			gotoxy(95, 18);
			printf("♥♥♥♥   ");
			textcolor(WHITE, BLACK);

		}
		else if (LIFE2 == 3) {
			textcolor(RED1, BLACK);
			gotoxy(95, 18);
			printf("♥♥♥     ");
			textcolor(WHITE, BLACK);

		}
		else if (LIFE2 == 2) {
			textcolor(RED1, BLACK);
			gotoxy(95, 18);
			printf("♥♥    ");
			textcolor(WHITE, BLACK);

		}
		else if (LIFE2 == 1) {
			textcolor(RED1, BLACK);
			gotoxy(95, 18);
			printf("♥       ");
			textcolor(WHITE, BLACK);

		}
		if ((LIFE == 0) || (v_size2 == 5)) {
			cls(BLACK, WHITE);

			while (1) {
				int c1, c2;
				do { // 색을 변경하면서 Game Over 출력
					c1 = rand() % 16;
					c2 = rand() % 16;
				} while (c1 == c2);
				//	win2p();
				gotoxy(50, 20);
				printf("2p win");
				textcolor(c1, c2);
				gotoxy(45, 10);
				printf("** Game Over **");
				gotoxy(20, 13);
				textcolor(WHITE, BLACK);
				printf("'R'을 누르면 재시작 하고 'q'를 누르면 난이도 선택 메뉴로 넘어갑니다");
				Sleep(300);
				if (kbhit()) {
					ch = getch();
					if (ch == 'r' || ch == 'q')
						break;
				}
			}
			if (ch == 'r')
				goto START;
			else if (ch == 'q') {
				cls(BLACK, WHITE);
				break;
			}
			gotoxy(0, HEIGHT - 1);
		}
		else if ((LIFE2 == 0) || (v_size == 5)) {
			cls(BLACK, WHITE);

			while (1) {
				int c1, c2;
				do { // 색을 변경하면서 Game Over 출력
					c1 = rand() % 16;
					c2 = rand() % 16;
				} while (c1 == c2);
				gotoxy(50, 20);
				printf("1p win");
				textcolor(c1, c2);
				gotoxy(45, 10);
				printf("** Game Over **");
				gotoxy(20, 13);
				textcolor(WHITE, BLACK);
				printf("'R'을 누르면 재시작 하고 'q'를 누르면 난이도 선택 메뉴로 넘어갑니다");
				Sleep(300);
				if (kbhit()) {
					ch = getch();
					if (ch == 'r' || ch == 'q')
						break;
				}
			}

			if (ch == 'r')
				goto START;
			else if (ch == 'q') {
				cls(BLACK, WHITE);
				break;
			}
			gotoxy(0, HEIGHT - 1);
		}
		Sleep(delay);

		// Delay를 줄이면 속도가 빨라진다.
		frame_count++;

	}

}
int gloop3(int mapcode) { //2단계

START:
	init_game();
	int mon_frame_sync1 = 20;
	int mon_frame_sync2 = 17;
	int mon_frame_sync3 = 19;
	int mon_frame_sync4 = 20;
	int mon_frame_sync5 = 22;
	int mon_frame_sync6 = 17;
	int mon_frame_sync7 = 19;
	int mon_frame_sync8 = 20;

	int iteminterval = 4;
	int iteminterval2 = 10;
	int v_size = 0;
	int v_size2 = 0;
	int mon_v_size = 0;
	int LIFE = 5; //생명
	int LIFE2 = 5;
	int bullet = 5;
	int delay = 10;
	int SPEED = 1;
	int SPEED2 = 1;
	int run_time, start_time, remain_time, last_remain_time;
	int run_time2, start_time2, remain_time2, last_remain_time2;
	int gold_time = 0;
	int gold_time2 = 0;
	start_time = time(NULL);
	start_time2 = time(NULL);

	last_remain_time = remain_time = time_out;
	last_remain_time2 = remain_time2 = time_out;
	int bullet_size2 = 0;
	int oldx1 = 82, oldy1 = 2;
	int newx1 = 82, newy1 = 2;
	int oldx2 = 86, oldy2 = 4;
	int newx2 = 86, newy2 = 4;
	int x = 4, y = 30;
	int	mx, my;
	int i;
	unsigned char ch;
	unsigned char ch1;
	unsigned char ch2;	int playing = 1;
	ch1 = ch2 = 0;
	int p1_frame_sync = 7; // 처음 시작은 10 frame 마다 이동, 즉, 100msec 마다 이동
	int p2_frame_sync = 7;
	int box = -1, bnx = -1;
	int boy = -1, bny = -1;
	int box2 = -1, bnx2 = -1;
	int boy2 = -1, bny2 = -1;
	int box3 = -1, bnx3 = -1;
	int boy3 = -1, bny3 = -1;
	int box4 = -1, bnx4 = -1;
	int boy4 = -1, bny4 = -1;
	int mon_ox = 5, mon_oy = 30; // monster 좌표
	int mon_nx = 5, mon_ny = 30;
	int mon_ox2 = 30, mon_oy2 = 30; // monster 좌표
	int mon_nx2 = 30, mon_ny2 = 30;
	int mon_ox3 = 35, mon_oy3 = 31; // monster 좌표
	int mon_nx3 = 35, mon_ny3 = 31;
	int mon_ox4 = 41, mon_oy4 = 32; // monster 좌표
	int mon_nx4 = 41, mon_ny4 = 32;
	int mon_ox5 = 35, mon_oy5 = 30; // monster 좌표
	int mon_nx5 = 35, mon_ny5 = 30;
	int mon_ox6 = 50, mon_oy6 = 32; // monster 좌표
	int mon_nx6 = 50, mon_ny6 = 32;
	int mon_ox7 = 70, mon_oy7 = 33; // monster 좌표
	int mon_nx7 = 70, mon_ny7 = 33;	
	int mon_ox8 = 67, mon_oy8 = 33; // monster 좌표
	int mon_nx8 = 67, mon_ny8 = 33;
	int frame_count = 0;
	int keep_moving = 1;
	int mon_life = 5;
	int bullet_size = 0;
	int p1_speed = 4;
	int p2_speed = 4;
	drawmap(&x, &y, &mx, &my);
	removeCursor();
	showplayer1(oldx1, oldy1, newx1, newy1);
	showplayer2(oldx2, oldy2, newx2, newy2);
	textcolor(WHITE, BLACK);
	ch = 0; // 초기값 정지상태
	keep_moving = 0;
	while (1) {
		run_time = time(NULL) - start_time;
		if (run_time > gold_time && (run_time % iteminterval == 0)) {
			show_item5();
			show_item6();
			gold_time = run_time; // 마지막 GOLD 표시 시간 기억
		}
		run_time2 = time(NULL) - start_time2;
		if (run_time2 > gold_time2 && (run_time2 % iteminterval2 == 0)) {
			show_item8();
			show_item7();
			gold_time2 = run_time2; // 마지막 GOLD 표시 시간 기억
		}

		if (kbhit() == 1) {  // 키보드가 눌려져 있으면
			ch = getch(); // key 값을 읽는다
			if (ch == ESC) {// ESC 누르면 프로그램 종료 추가
				cls(BLACK, WHITE);
				break;
			}
			if (ch == SPECIAL1 || ch == SPECIAL2) { // 만약 특수키
				// 예를 들어 UP key의 경우 0xe0 0x48 두개의 문자가 들어온다.
				ch1 = getch();
				switch (ch1) {
				case UP:
				case DOWN:
				case LEFT:
				case RIGHT:
				case SPACE:
				case M:
					keep_moving = 1;
					break;
				default: // 방향키가 아니면 멈춘다
					keep_moving = 1;
				}
			}
			else {
				ch2 = ch;// Player2은 AWSD 로 움직인다.
				switch (ch2) {
				case UP2:
				case DOWN2:
				case LEFT2:
				case RIGHT2://player2만 방향 전환
				case G:
				case H:
					keep_moving = 1;
					break;
				default:// 방향 전환이 아니면
					keep_moving = 1;
				}
				// 특수 문자가 아니지만 AWSD를 방향키 대신 사용하는 경우 처리

			}
			if (ch == SPACE) { //총알 구현		
				if (bullet_size != 0) {
					bnx = box = oldx1 - 1;
					bny = boy = oldy1;
					showbullet(box, boy, bnx, bny); //총알을 그림
					bullet_size--;
				}
			}
			if (ch == M) { //1p 총
				if (bullet_size != 0) {
					bnx2 = box2 = oldx1 - 1;
					bny2 = boy2 = oldy1;
					showbullet(box2, boy2, bnx2, bny2); //총알을 그림
					bullet_size--;
				}
			}
			if (ch2 == G) {  //2p 총
				if (bullet_size2 != 0) {
					bnx3 = box3 = oldx2 - 1;
					bny3 = boy3 = oldy2;
					showbullet(box3, boy3, bnx3, bny3); //총알을 그림
					bullet_size2--;
				}
			}
			if (ch2 == H) {  //2p 총
				if (bullet_size2 != 0) {
					bnx4 = box4 = oldx2 - 1;
					bny4 = boy4 = oldy2;
					showbullet(box4, boy4, bnx4, bny4); //총알을 그림
					bullet_size2--;
				}
			}
		}

		if (keep_moving && frame_count % p1_frame_sync == 0) { // 1p 움직이고 있으면			
			switch (ch1) {
			case UP:
				if (oldy1 > 2)
					newy1 = oldy1 - SPEED;
				if (ch == SPACE) {
					if (boy > 2)
						bny = boy - 1;
				}
				keep_moving = 1;
				break;
			case DOWN:
				if (oldy1 < HEIGHT - 4)
					newy1 = oldy1 + SPEED;
				if (ch == SPACE) {
					if (boy > 2)
						bny = boy + 1;
				}
				keep_moving = 1;
				break;
			case LEFT:
				if (oldx1 > 2)
					newx1 = oldx1 - SPEED;
				if (ch == SPACE) {
					if (box > 2)
						bnx = box - 1;
				}
				keep_moving = 1;

				break;
			case RIGHT:
				if (oldx1 < WIDTH - 4)
					newx1 = oldx1 + SPEED;
				if (ch == SPACE) {
					if (box > 2)
						bnx = box - 1;
				}
				keep_moving = 1;
				break;
			}
		}
			if (keep_moving && frame_count % p2_frame_sync == 0) {

			switch (ch2) {  //2p
			case UP2:
				if (oldy2 > 2)
					newy2 = oldy2 - SPEED2;
				break;
			case DOWN2:
				if (oldy2 < HEIGHT - 4)
					newy2 = oldy2 + SPEED2;
				break;
			case LEFT2:
				if (oldx2 > 2)
					newx2 = oldx2 - SPEED2;
				break;
			case RIGHT2:
				if (oldx2 < WIDTH - 4)
					newx2 = oldx2 + SPEED2;
				break;
			}

		}
		showplayer1(oldx1, oldy1, newx1, newy1); // Player 이동 표시
		oldx1 = newx1; // 마지막 위치를 기억한다.
		oldy1 = newy1;
		keep_moving = 1; //1:계속이동, 0:한번에 한칸씩이동
		showplayer2(oldx2, oldy2, newx2, newy2); // Player 이동 표시
		oldx2 = newx2; // 마지막 위치를 기억한다.
		oldy2 = newy2;
		keep_moving = 1; //1:계속이동, 0:한번에 한칸씩이동

		if (box != -1 && frame_count % bullet_frame_sync == 0) {// 불렛이 몬스터와 닿으면 추적 중지
			// // 아래 총알의 움직임 처리
			if ((mon_ox - box) > 0) {
				bnx = box + 1;
			}
			else if ((mon_ox - box) < 0) {
				bnx = box - 1;
			}
			if ((mon_oy - boy) > 0) {
				bny = boy + 1;
			}
			else if ((mon_oy - boy) < 0) {
				bny = boy - 1;
			}
			if (box == mon_ox && boy == mon_oy) {
				//mon_ox = box;
				//mon_oy = boy;
				erasemon(mon_ox, mon_oy, mon_nx, mon_ny);
				box = -1;
				erasebullet(mon_ox, mon_oy, mon_nx, mon_ny);
			}
			else {
				showbullet(box, boy, bnx, bny); //총알을 그림
				box = bnx;
				boy = bny;
			}
			showbullet(box, boy, bnx, bny); //총알을 그림
			box = bnx;
			boy = bny;
		}
		if (box2 != -1 && frame_count % bullet_frame_sync == 0) {// 2불렛이 2몬스터와 닿으면 추적 중지
			// // 아래 총알의 움직임 처리
			if ((mon_ox2 - box2) > 0) {
				bnx2 = box2 + 1;
			}
			else if ((mon_ox2 - box2) < 0) {
				bnx2 = box2 - 1;
			}
			if ((mon_oy2 - boy2) > 0) {
				bny2 = boy2 + 1;
			}
			else if ((mon_oy2 - boy2) < 0) {
				bny2 = boy2 - 1;
			}
			if (box2 == mon_ox2 && boy2 == mon_oy2) {
				//mon_ox = box;
				//mon_oy = boy;
				erasemon(mon_ox2, mon_oy2, mon_nx2, mon_ny2);
				box2 = -1;
				erasebullet(mon_ox2, mon_oy2, mon_nx2, mon_ny2);
			}
			else {
				showbullet(box2, boy2, bnx2, bny2); //총알을 그림
				box2 = bnx2;
				boy2 = bny2;
			}
			showbullet(box2, boy2, bnx2, bny2); //총알을 그림
			box2 = bnx2;
			boy2 = bny2;
		}
		if (box3 != -1 && frame_count % bullet_frame_sync == 0) {// 2불렛이 2몬스터와 닿으면 추적 중지
			// // 아래 총알의 움직임 처리
			if ((mon_ox3 - box3) > 0) {
				bnx3 = box3 + 1;
			}
			else if ((mon_ox3 - box3) < 0) {
				bnx3 = box3 - 1;
			}
			if ((mon_oy3 - boy3) > 0) {
				bny3 = boy3 + 1;
			}
			else if ((mon_oy3 - boy3) < 0) {
				bny3 = boy3 - 1;
			}
			if (box3 == mon_ox3 && boy3 == mon_oy3) {
				//mon_ox = box;
				//mon_oy = boy;
				erasemon(mon_ox3, mon_oy3, mon_nx3, mon_ny3);
				box3 = -1;
				erasebullet(mon_ox3, mon_oy3, mon_nx3, mon_ny3);
			}
			else {
				showbullet(box3, boy3, bnx3, bny3); //총알을 그림
				box3 = bnx3;
				boy3 = bny3;
			}
			showbullet(box3, boy3, bnx3, bny3); //총알을 그림
			box3 = bnx3;
			boy3 = bny3;
		}
		if (box4 != -1 && frame_count % bullet_frame_sync == 0) {// 2불렛이 2몬스터와 닿으면 추적 중지
			// // 아래 총알의 움직임 처리
			if ((mon_ox4 - box4) > 0) {
				bnx4 = box4 + 1;
			}
			else if ((mon_ox4 - box4) < 0) {
				bnx4 = box4 - 1;
			}
			if ((mon_oy4 - boy4) > 0) {
				bny4 = boy4 + 1;
			}
			else if ((mon_oy4 - boy4) < 0) {
				bny4 = boy4 - 1;
			}
			if (box4 == mon_ox4 && boy4 == mon_oy4) {
				//mon_ox = box;
				//mon_oy = boy;
				erasemon(mon_ox4, mon_oy4, mon_nx4, mon_ny4);
				box4 = -1;
				erasebullet(mon_ox4, mon_oy4, mon_nx4, mon_ny4);
			}
			else {
				showbullet(box4, boy4, bnx4, bny4); //총알을 그림
				box4 = bnx4;
				boy4 = bny4;
			}
			showbullet(box4, boy4, bnx4, bny4); //총알을 그림
			box4 = bnx4;
			boy4 = bny4;
		}
		textcolor(WHITE, BLACK);
		gotoxy(98, 2);
		printf("1p");
		gotoxy(96, 7);
		printf("MAX 10");
		gotoxy(98, 16);
		printf("2p");
		gotoxy(96, 21);
		printf("MAX 10");
		if (item8[oldx1][oldy1] || item8[oldx1][oldy1 + 1]) { //1p 백신 증가
			item8[oldx1][oldy1] = 0;
			item8[oldx1][oldy1 + 1] = 0;
			v_size++;
		}
		if (item8[oldx2][oldy2] || item8[oldx2][oldy2 + 1]) {	//2p 백신 증가

			item8[oldx2][oldy2] = 0;
			item8[oldx2][oldy2 + 1] = 0;
			v_size2++;
		}
		if (v_size == 1) {
			textcolor(MAGENTA2, BLACK);
			gotoxy(95, 11);
			printf("§");
			textcolor(WHITE, BLACK);

		}
		if (v_size == 2) {
			textcolor(MAGENTA2, BLACK);
			gotoxy(95, 11);
			printf("§§");
			textcolor(WHITE, BLACK);

		}
		if (v_size == 3) {
			textcolor(MAGENTA2, BLACK);
			gotoxy(95, 11);
			printf("§§§");
			textcolor(WHITE, BLACK);

		}
		if (v_size == 4) {
			textcolor(MAGENTA2, BLACK);
			gotoxy(95, 11);
			printf("§§§§");
			textcolor(WHITE, BLACK);

		}
		if (v_size2 == 1) {
			textcolor(MAGENTA2, BLACK);
			gotoxy(95, 25);
			printf("§");
			textcolor(WHITE, BLACK);
		}
		if (v_size2 == 2) {
			textcolor(MAGENTA2, BLACK);
			gotoxy(95, 25);
			printf("§§");
			textcolor(WHITE, BLACK);
		}
		if (v_size2 == 3) {
			textcolor(MAGENTA2, BLACK);
			gotoxy(95, 25);
			printf("§§§");
			textcolor(WHITE, BLACK);
		}
		if (v_size2 == 4) {
			textcolor(MAGENTA2, BLACK);
			gotoxy(95, 25);
			printf("§§§§");
			textcolor(WHITE, BLACK);

		}
		if (item8[mon_ox][mon_oy] || item8[mon_ox][mon_oy + 1]) {	//2p 백신 증가
			item8[mon_ox][mon_oy] = 0;
			item8[mon_ox][mon_oy + 1] = 0;
			mon_v_size++;
		}
		if (item8[mon_ox2][mon_oy2] || item8[mon_ox2][mon_oy2 + 1]) {	//2p 백신 증가
			item8[mon_ox2][mon_oy2] = 0;
			item8[mon_ox2][mon_oy2 + 1] = 0;
			mon_v_size++;
		}
		if (item8[mon_ox3][mon_oy3] || item8[mon_ox3][mon_oy3 + 1]) {	//2p 백신 증가
			item8[mon_ox3][mon_oy3] = 0;
			item8[mon_ox3][mon_oy3 + 1] = 0;
			mon_v_size++;
		}
		if (item8[mon_ox4][mon_oy4] || item8[mon_ox4][mon_oy4 + 1]) {	//2p 백신 증가
			item8[mon_ox4][mon_oy4] = 0;
			item8[mon_ox4][mon_oy4 + 1] = 0;
			mon_v_size++;
		}
		if (item8[mon_ox5][mon_oy5] || item8[mon_ox5][mon_oy5 + 1]) {	//2p 백신 증가
			item8[mon_ox5][mon_oy5] = 0;
			item8[mon_ox5][mon_oy5 + 1] = 0;
			mon_v_size++;
		}
		if (item8[mon_ox6][mon_oy6] || item8[mon_ox6][mon_oy6 + 1]) {	//2p 백신 증가
			item8[mon_ox6][mon_oy6] = 0;
			item8[mon_ox6][mon_oy6 + 1] = 0;
			mon_v_size++;
		}
		if (item8[mon_ox7][mon_oy7] || item8[mon_ox7][mon_oy7 + 1]) {	//2p 백신 증가
			item8[mon_ox7][mon_oy7] = 0;
			item8[mon_ox7][mon_oy7 + 1] = 0;
			mon_v_size++;
		}		
		if (item8[mon_ox8][mon_oy8] || item8[mon_ox8][mon_oy8 + 1]) {	//2p 백신 증가
			item8[mon_ox8][mon_oy8] = 0;
			item8[mon_ox8][mon_oy8 + 1] = 0;
			mon_v_size++;
		}
		gotoxy(97, 30);
		printf("LOSE");
		if (mon_v_size == 1) {
			textcolor(MAGENTA2, BLACK);
			gotoxy(95, 32);
			printf("§");
			textcolor(WHITE, BLACK);

		}
		if (mon_v_size == 3) {
			textcolor(MAGENTA2, BLACK);
			gotoxy(95, 32);
			printf("§§");
			textcolor(WHITE, BLACK);

		}
		if (mon_v_size == 3) {
			textcolor(MAGENTA2, BLACK);
			gotoxy(95, 32);
			printf("§§§");
			textcolor(WHITE, BLACK);

		}

		if (item5[oldx1][oldy1] || item5[oldx1][oldy1 + 1]) { //1p 아이템 먹으면 속도 증가
			if (p1_frame_sync != 1) {
				item5[oldx1][oldy1] = 0;
				item5[oldx1][oldy1 + 1] = 0;
				p1_frame_sync--;
				p1_speed++;
			}
		}
		if (item5[oldx2][oldy2] || item5[oldx2][oldy2 + 1]) {				//2p 아이템 먹으면 속도 증가
			if (p2_frame_sync != 1) {
				item5[oldx2][oldy2] = 0;
				item5[oldx2][oldy2 + 1] = 0;
				p2_frame_sync -= 1;
				p2_speed++;
			}
		}
		if (item6[oldx1][oldy1] || item6[oldx1][oldy1 + 1]) { //1p 아이템 먹으면 속도 감소
			if (p1_frame_sync != 11) {
				item6[oldx1][oldy1] = 0;
				item6[oldx1][oldy1 + 1] = 0;
				p1_frame_sync++;
				p1_speed--;
			}
		}
		if (item6[oldx2][oldy2] || item6[oldx2][oldy2 + 1]) {				//2p 아이템 먹으면 속도 감소
			if (p2_frame_sync != 11) {
				item6[oldx2][oldy2] = 0;
				item6[oldx2][oldy2 + 1] = 0;
				p2_frame_sync += 1;
				p2_speed--;
			}
		}
		if (item7[oldx1][oldy1] || item7[oldx1][oldy1 + 1]) { //1p 총알 갯수 증가
			if (bullet_size != 2) {
				item7[oldx1][oldy1] = 0;
				item7[oldx1][oldy1 + 1] = 0;
				bullet_size++;
			}
		}
		if (item7[oldx2][oldy2] || item7[oldx2][oldy2 + 1]) {	//2p 총알 갯수 증가
			if (bullet_size != 2) {
				item7[oldx2][oldy2] = 0;
				item7[oldx2][oldy2 + 1] = 0;
				bullet_size2++;
			}
		}
		if (bullet_size == 0) {
			gotoxy(97, 9);
			printf("총알");
			gotoxy(97, 10);
			printf("    ");
		}

		if (bullet_size == 1) {
			gotoxy(97, 10);
			printf("    ");
			gotoxy(97, 10);
			printf("ⅰ");
		}
		if (bullet_size == 2) {
			gotoxy(97, 10);
			printf("    ");
			gotoxy(97, 10);
			printf("ⅰⅰ");
		}
		if (bullet_size == 0) {
			gotoxy(97, 23);
			printf("총알");
			gotoxy(97, 24);
			printf("     ");
		}
		if (bullet_size2 == 1) {
			gotoxy(97, 24);
			printf("     ");
			gotoxy(97, 24);
			printf("ⅰ");
		}
		if (bullet_size2 == 2) {
			gotoxy(97, 24);
			printf("     ");
			gotoxy(97, 24);
			printf("ⅰⅰ");
		}
		if (p1_speed == 1) {
			gotoxy(95, 6);
			printf("속도 : %d", p1_speed);
		}
		else if (p1_speed == 2) {
			gotoxy(95, 6);
			printf("속도 : %d", p1_speed);
		}
		else if (p1_speed == 3) {
			gotoxy(95, 6);
			printf("속도 : %d", p1_speed);
		}
		else if (p1_speed == 4) {
			gotoxy(95, 6);
			printf("속도 : %d", p1_speed);
		}
		else if (p1_speed == 5) {
			gotoxy(95, 6);
			printf("속도 : %d", p1_speed);
		}
		else if (p1_speed == 6) {
			gotoxy(95, 6);
			printf("속도 : %d", p1_speed);
		}
		else if (p1_speed == 7) {
			gotoxy(95, 6);
			printf("속도 : %d", p1_speed);
		}
		else if (p1_speed == 8) {
			gotoxy(95, 6);
			printf("속도 : %d", p1_speed);
		}
		else if (p1_speed == 9) {
			gotoxy(95, 6);
			printf("           ");
			gotoxy(95, 6);
			printf("속도 : %d", p1_speed);
		}
		else if (p1_speed == 10) {
			gotoxy(95, 6);
			printf("속도 : %d", p1_speed);
		}
		if (p2_speed == 1) {
			gotoxy(95, 20);
			printf("속도 : %d", p2_speed);
		}
		else if (p2_speed == 2) {
			gotoxy(95, 20);
			printf("속도 : %d", p2_speed);
		}
		else if (p2_speed == 3) {
			gotoxy(95, 20);
			printf("속도 : %d", p2_speed);
		}
		else if (p2_speed == 4) {
			gotoxy(95, 20);
			printf("속도 : %d", p2_speed);
		}
		else if (p2_speed == 5) {
			gotoxy(95, 20);
			printf("속도 : %d", p2_speed);
		}
		else if (p2_speed == 6) {
			gotoxy(95, 20);
			printf("속도 : %d", p2_speed);
		}
		else if (p2_speed == 7) {
			gotoxy(95, 20);
			printf("속도 : %d", p2_speed);
		}
		else if (p2_speed == 8) {
			gotoxy(95, 20);
			printf("속도 : %d", p2_speed);

		}
		else if (p2_speed == 9) {
			gotoxy(95, 20);
			printf("          ");
			gotoxy(95, 20);
			printf("속도 : %d", p2_speed);
		}
		else if (p2_speed == 10) {
			gotoxy(95, 20);
			printf("         ");
			gotoxy(95, 20);
			printf("속도 : %d", p2_speed);
		}


		// 1p monster의 움직임 처리

		if (frame_count % mon_frame_sync1 == 0) {  //1p 1mon
			if (mon_nx != box) {
				if ((oldx1 - mon_ox) > 0)
					mon_nx += 1;
				else if ((oldx1 - mon_ox) < 0)
					mon_nx -= 1;
				if ((oldy1 - mon_oy) > 0)
					mon_ny += 1;
				else if ((oldy1 - mon_oy) < 0)
					mon_ny -= 1;
				showmon(mon_ox, mon_oy, mon_nx, mon_ny);
				mon_ox = mon_nx;
				mon_oy = mon_ny;
			}
		}
		if (frame_count % mon_frame_sync2 == 0) {  //1p 2mon
			if (mon_nx2 != box2) {
				if ((oldx1 - mon_ox2) > 0)
					mon_nx2 += 1;
				else if ((oldx1 - mon_ox2) < 0)
					mon_nx2 -= 1;
				if ((oldy1 - mon_oy2) > 0)
					mon_ny2 += 1;
				else if ((oldy1 - mon_oy2) < 0)
					mon_ny2 -= 1;
				showmon(mon_ox2, mon_oy2, mon_nx2, mon_ny2);
				mon_ox2 = mon_nx2;
				mon_oy2 = mon_ny2;
			}
		}
		if (frame_count % mon_frame_sync5 == 0) {  //1p 5mon
			if (mon_nx5 != box2) {
				if ((oldx1 - mon_ox5) > 0)
					mon_nx5 += 1;
				else if ((oldx1 - mon_ox5) < 0)
					mon_nx5 -= 1;
				if ((oldy1 - mon_oy5) > 0)
					mon_ny5 += 1;
				else if ((oldy1 - mon_oy5) < 0)
					mon_ny5 -= 1;
				showmon(mon_ox5, mon_oy5, mon_nx5, mon_ny5);
				mon_ox5 = mon_nx5;
				mon_oy5 = mon_ny5;
			}
		}
		if (frame_count % mon_frame_sync7 == 0) {  //1p 7mon
			if (mon_nx7 != box2) {
				if ((oldx1 - mon_ox7) > 0)
					mon_nx7 += 1;
				else if ((oldx1 - mon_ox7) < 0)
					mon_nx7 -= 1;
				if ((oldy1 - mon_oy7) > 0)
					mon_ny7 += 1;
				else if ((oldy1 - mon_oy7) < 0)
					mon_ny7 -= 1;
				showmon(mon_ox7, mon_oy7, mon_nx7, mon_ny7);
				mon_ox7 = mon_nx7;
				mon_oy7 = mon_ny7;
			}
		}
		//2p 몬스터 움직임			
		if (frame_count % mon_frame_sync3 == 0) { //2p 3mon
			if (mon_nx3 != box3) {
				if ((oldx2 - mon_ox3) > 0)
					mon_nx3 += 1;
				else if ((oldx2 - mon_ox3) < 0)
					mon_nx3 -= 1;
				if ((oldy2 - mon_oy3) > 0)
					mon_ny3 += 1;
				else if ((oldy2 - mon_oy3) < 0)
					mon_ny3 -= 1;
				showmon(mon_ox3, mon_oy3, mon_nx3, mon_ny3);
				mon_ox3 = mon_nx3;
				mon_oy3 = mon_ny3;
			}
		}

		if (frame_count % mon_frame_sync4 == 0) { //2p 4mon
			if (mon_nx4 != box4) {

				if ((oldx2 - mon_ox4) > 0)
					mon_nx4 += 1;
				else if ((oldx2 - mon_ox4) < 0)
					mon_nx4 -= 1;
				if ((oldy2 - mon_oy4) > 0)
					mon_ny4 += 1;
				else if ((oldy2 - mon_oy4) < 0)
					mon_ny4 -= 1;
				showmon(mon_ox4, mon_oy4, mon_nx4, mon_ny4);
				mon_ox4 = mon_nx4;
				mon_oy4 = mon_ny4;
			}
		}
		if (frame_count % mon_frame_sync6 == 0) { //2p 6mon
			if (mon_nx6 != box4) {
				if ((oldx2 - mon_ox6) > 0)
					mon_nx6 += 1;
				else if ((oldx2 - mon_ox6) < 0)
					mon_nx6 -= 1;
				if ((oldy2 - mon_oy6) > 0)
					mon_ny6 += 1;
				else if ((oldy2 - mon_oy6) < 0)
					mon_ny6 -= 1;
				showmon(mon_ox6, mon_oy6, mon_nx6, mon_ny6);
				mon_ox6 = mon_nx6;
				mon_oy6 = mon_ny6;
			}
		}
		if (frame_count % mon_frame_sync8 == 0) { //2p 8mon
			if (mon_nx8 != box4) {
				if ((oldx2 - mon_ox8) > 0)
					mon_nx8 += 1;
				else if ((oldx2 - mon_ox8) < 0)
					mon_nx8 -= 1;
				if ((oldy2 - mon_oy8) > 0)
					mon_ny8 += 1;
				else if ((oldy2 - mon_oy8) < 0)
					mon_ny8 -= 1;
				showmon(mon_ox8, mon_oy8, mon_nx8, mon_ny8);
				mon_ox8 = mon_nx8;
				mon_oy8 = mon_ny8;
			}
		}
		if ((oldx1 == mon_nx && oldy1 == mon_ny) ||(oldx1==mon_nx5&&oldy1==mon_ny5)|| (oldx1 == mon_nx && oldy1 + 1 == mon_ny)) {  //몬스터 접촉후 무적시간
			if (frame_count % mon_frame_sync1 == 0) {
				if (mon_nx != box) {
					showmon(mon_ox, mon_oy, mon_nx, mon_ny);
					mon_ox = mon_nx;
					mon_oy = mon_ny;
					LIFE = LIFE - 1;
				}
			}
		}
		if ((oldx1 == mon_nx3 && oldy1 == mon_ny3) ||(oldx1==mon_nx6 && oldy1+1==mon_ny6) ||(oldx1 == mon_nx3 && oldy1 + 1 == mon_ny3)||(oldx1==mon_nx7&&oldy1==mon_ny7)) {  //몬스터 접촉후 무적시간
			if (frame_count % mon_frame_sync3 == 0) {
				if (mon_nx3 != box) {
	
					showmon(mon_ox3, mon_oy3, mon_nx3, mon_ny3);
					mon_ox3 = mon_nx3;
					mon_oy3 = mon_ny3;
					LIFE = LIFE - 1;
				}
			}
		}
		if ((oldx1 == mon_nx2 && oldy1 == mon_ny2) || (oldx1 == mon_nx2 && oldy1 + 1 == mon_ny2)||(oldx1==mon_nx8&&oldy1==mon_ny8)) {//몬스터 접촉후 무적시간
			if (frame_count % mon_frame_sync2 == 0) {
				if (mon_nx2 != box2) {
			
					showmon(mon_ox2, mon_oy2, mon_nx2, mon_ny2);
					mon_ox2 = mon_nx2;
					mon_oy2 = mon_ny2;
					LIFE = LIFE - 1;
				}
			}
		}
		if ((oldx1 == mon_nx4 && oldy1 == mon_ny4) || (oldx1 == mon_nx4 && oldy1 + 1 == mon_ny4)||(oldx1==mon_nx8&&oldy1+1==mon_ny8)) {//몬스터 접촉후 무적시간
			if (frame_count % mon_frame_sync4 == 0) {
				if (mon_nx4 != box2) {
					showmon(mon_ox4, mon_oy4, mon_nx4, mon_ny4);
					mon_ox4 = mon_nx4;
					mon_oy4 = mon_ny4;
					LIFE = LIFE - 1;
				}
			}
		}
		if ((oldx2 == mon_nx3 && oldy2 == mon_ny3) ||(oldx2==mon_ox6&&oldy2==mon_ny6) ||(oldx2 == mon_nx3 && oldy2 + 1 == mon_ny3)) {//몬스터 접촉후 무적시간
			if (frame_count % mon_frame_sync3 == 0) {
				if (mon_nx3 != box3) {
					showmon(mon_ox3, mon_oy3, mon_nx3, mon_ny3);
					mon_ox3 = mon_nx3;
					mon_oy3 = mon_ny3;
					LIFE2 = LIFE2 - 1;
				}
			}
		}

		if ((oldx2 == mon_nx && oldy2 == mon_ny) ||(oldx2==mon_nx5&&oldy2==mon_oy5)|| (oldx2 == mon_nx && oldy2 + 1 == mon_ny)) {//몬스터 접촉후 무적시간
			if (frame_count % mon_frame_sync1 == 0) {
				if (mon_nx != box3) {
					showmon(mon_ox, mon_oy, mon_nx, mon_ny);
					mon_ox = mon_nx;
					mon_oy = mon_ny;
					LIFE2 = LIFE2 - 1;
				}
			}
		}
		if ((oldx2 == mon_nx4 && oldy2 == mon_ny4) ||(oldx2==mon_nx7&&oldy2+1==mon_ny7)|| (oldx2 == mon_nx4 && oldy2 + 1 == mon_ny4)) {//몬스터 접촉후 무적시간
			if (frame_count % mon_frame_sync4 == 0) {
				if (mon_nx4 != box4) {
					showmon(mon_ox4, mon_oy4, mon_nx4, mon_ny4);
					mon_ox4 = mon_nx4;
					mon_oy4 = mon_ny4;
					LIFE2 = LIFE2 - 1;
				}
			}
		}
		if ((oldx2 == mon_nx2 && oldy2 == mon_ny2) ||(oldx2==mon_nx7&&oldy2==mon_ny7)|| (oldx2 == mon_nx2 && oldy2 + 1 == mon_ny2)) {//몬스터 접촉후 무적시간
			if (frame_count % mon_frame_sync2 == 0) {
				if (mon_nx2 != box4) {
					showmon(mon_ox2, mon_oy2, mon_nx2, mon_ny2);
					mon_ox2 = mon_nx2;
					mon_oy2 = mon_ny2;
					LIFE2 = LIFE2 - 1;
				}
			}
		}
		if ((oldx2 == mon_nx8 && oldy2 == mon_ny8) || (oldx2 == mon_nx6 && oldy2+1 == mon_ny6) || (oldx2 == mon_nx2 && oldy2 + 1 == mon_ny2)) {//몬스터 접촉후 무적시간
			if (frame_count % mon_frame_sync8 == 0) {
					showmon(mon_ox8, mon_oy8, mon_nx8, mon_ny8);
					mon_ox8 = mon_nx8;
					mon_oy8 = mon_ny8;
					LIFE2 = LIFE2 - 1;
				}
			}
		
		if (item5[mon_ox][mon_oy] || item5[mon_ox][mon_oy + 1]) { //몬스터 아이템 먹으면 속도 증가
			if (mon_frame_sync1 != 5) {
				item5[mon_ox][mon_oy] = 0;
				item5[mon_ox][mon_oy + 1] = 0;
				mon_frame_sync1--;
			}
		}
		if (item5[mon_ox2][mon_oy2] || item5[mon_ox2][mon_oy2 + 1]) { //몬스터 아이템 먹으면 속도 증가
			if (mon_frame_sync2 != 5) {
				item5[mon_ox2][mon_oy2] = 0;
				item5[mon_ox2][mon_oy2 + 1] = 0;
				mon_frame_sync2--;

			}
		}
		if (item5[mon_ox3][mon_oy3] || item5[mon_ox3][mon_oy3 + 1]) { //몬스터 아이템 먹으면 속도 증가
			if (mon_frame_sync3 != 5) {
				item5[mon_ox3][mon_oy3] = 0;
				item5[mon_ox3][mon_oy3 + 1] = 0;
				mon_frame_sync3--;

			}
		}
		if (item5[mon_ox4][mon_oy4] || item5[mon_ox4][mon_oy4 + 1]) { //몬스터 아이템 먹으면 속도 증가
			if (mon_frame_sync4 != 5) {
				item5[mon_ox4][mon_oy4] = 0;
				item5[mon_ox4][mon_oy4 + 1] = 0;
				mon_frame_sync4--;

			}
		}
		if (item5[mon_ox5][mon_oy5] || item5[mon_ox5][mon_oy5 + 1]) { //몬스터 아이템 먹으면 속도 증가
			if (mon_frame_sync5 != 5) {
				item5[mon_ox5][mon_oy5] = 0;
				item5[mon_ox5][mon_oy5 + 1] = 0;
				mon_frame_sync5--;

			}
		}
		if (item5[mon_ox6][mon_oy6] || item5[mon_ox6][mon_oy6 + 1]) { //몬스터 아이템 먹으면 속도 증가
			if (mon_frame_sync6 != 5) {
				item5[mon_ox6][mon_oy6] = 0;
				item5[mon_ox6][mon_oy6 + 1] = 0;
				mon_frame_sync6--;

			}
		}
		if (item5[mon_ox7][mon_oy7] || item5[mon_ox7][mon_oy7 + 1]) { //몬스터 아이템 먹으면 속도 증가
			if (mon_frame_sync7 != 5) {
				item5[mon_ox7][mon_oy7] = 0;
				item5[mon_ox7][mon_oy7 + 1] = 0;
				mon_frame_sync7--;

			}
		}
		if (item5[mon_ox8][mon_oy8] || item5[mon_ox8][mon_oy8 + 1]) { //몬스터 아이템 먹으면 속도 증가
			if (mon_frame_sync8 != 5) {
				item5[mon_ox8][mon_oy8] = 0;
				item5[mon_ox8][mon_oy8 + 1] = 0;
				mon_frame_sync8--;

			}
		}
		if (item6[mon_ox][mon_oy] || item6[mon_ox][mon_oy + 1]) { //몬스터 아이템 먹으면 속도 감소
			if (mon_frame_sync1 != 25) {
				item6[mon_ox][mon_oy] = 0;
				item6[mon_ox][mon_oy + 1] = 0;
				mon_frame_sync1++;
			}
		}
		if (item6[mon_ox2][mon_oy2] || item6[mon_ox2][mon_oy2 + 1]) { //몬스터 아이템 먹으면 속도 감소
			if (mon_frame_sync2 != 25) {
				item6[mon_ox2][mon_oy2] = 0;
				item6[mon_ox2][mon_oy2 + 1] = 0;
				mon_frame_sync2++;
			}
		}
		if (item6[mon_ox3][mon_oy3] || item6[mon_ox3][mon_oy3 + 1]) { //몬스터 아이템 먹으면 속도 감소
			if (mon_frame_sync3 != 25) {
				item6[mon_ox3][mon_oy3] = 0;
				item6[mon_ox3][mon_oy3 + 1] = 0;
				mon_frame_sync3++;
			}
		}
		if (item6[mon_ox4][mon_oy4] || item6[mon_ox4][mon_oy4 + 1]) { //몬스터 아이템 먹으면 속도 감소
			if (mon_frame_sync4 != 25) {
				item6[mon_ox4][mon_oy4] = 0;
				item6[mon_ox4][mon_oy4 + 1] = 0;
				mon_frame_sync4++;
			}
		}	
		if (item6[mon_ox5][mon_oy5] || item6[mon_ox5][mon_oy5 + 1]) { //몬스터 아이템 먹으면 속도 감소
			if (mon_frame_sync5 != 25) {
				item6[mon_ox5][mon_oy5] = 0;
				item6[mon_ox5][mon_oy5 + 1] = 0;
				mon_frame_sync5++;
			}
		}
		if (item6[mon_ox6][mon_oy6] || item6[mon_ox6][mon_oy6 + 1]) { //몬스터 아이템 먹으면 속도 감소
			if (mon_frame_sync6 != 25) {
				item6[mon_ox6][mon_oy6] = 0;
				item6[mon_ox6][mon_oy6 + 1] = 0;
				mon_frame_sync6++;
			}
		}
		if (item6[mon_ox7][mon_oy7] || item6[mon_ox7][mon_oy7 + 1]) { //몬스터 아이템 먹으면 속도 감소
			if (mon_frame_sync7 != 25) {
				item6[mon_ox7][mon_oy7] = 0;
				item6[mon_ox7][mon_oy7 + 1] = 0;
				mon_frame_sync7++;
			}
		}
		if (item6[mon_ox8][mon_oy8] || item6[mon_ox8][mon_oy8 + 1]) { //몬스터 아이템 먹으면 속도 감소
			if (mon_frame_sync8 != 25) {
				item6[mon_ox8][mon_oy8] = 0;
				item6[mon_ox8][mon_oy8 + 1] = 0;
				mon_frame_sync8++;
			}
		}
		if (LIFE == 5) {
			textcolor(RED1, BLACK);
			gotoxy(95, 4);
			printf("♥♥♥♥♥");
			textcolor(WHITE, BLACK);
		}
		else if (LIFE == 4) {
			textcolor(RED1, BLACK);
			gotoxy(95, 4);
			printf("♥♥♥♥   ");
			textcolor(WHITE, BLACK);

		}
		else if (LIFE == 3) {
			textcolor(RED1, BLACK);
			gotoxy(95, 4);
			printf("♥♥♥     ");
			textcolor(WHITE, BLACK);

		}
		else if (LIFE == 2) {
			textcolor(RED1, BLACK);
			gotoxy(95, 4);
			printf("♥♥     ");
			textcolor(WHITE, BLACK);

		}
		else if (LIFE == 1) {
			textcolor(RED1, BLACK);
			gotoxy(95, 4);
			printf("♥       ");
			textcolor(WHITE, BLACK);

		}
		if (LIFE2 == 5) {
			textcolor(RED1, BLACK);
			gotoxy(95, 18);
			printf("♥♥♥♥♥");
			textcolor(WHITE, BLACK);
		}
		else if (LIFE2 == 4) {
			textcolor(RED1, BLACK);
			gotoxy(95, 18);
			printf("♥♥♥♥   ");
			textcolor(WHITE, BLACK);

		}
		else if (LIFE2 == 3) {
			textcolor(RED1, BLACK);
			gotoxy(95, 18);
			printf("♥♥♥     ");
			textcolor(WHITE, BLACK);

		}
		else if (LIFE2 == 2) {
			textcolor(RED1, BLACK);
			gotoxy(95, 18);
			printf("♥♥     ");
			textcolor(WHITE, BLACK);

		}
		else if (LIFE2 == 1) {
			textcolor(RED1, BLACK);
			gotoxy(95, 18);
			printf("♥       ");
			textcolor(WHITE, BLACK);

		}
		if ((LIFE == 0) || (v_size2 == 5)) {
			cls(BLACK, WHITE);
			while (1) {
				int c1, c2;
				do { // 색을 변경하면서 Game Over 출력
					c1 = rand() % 16;
					c2 = rand() % 16;
				} while (c1 == c2);
				//	win2p();
				gotoxy(50, 20);
				printf("2p win");
				textcolor(c1, c2);
				gotoxy(45, 10);
				printf("** Game Over **");
				gotoxy(20, 13);
				textcolor(WHITE, BLACK);
				printf("'R'을 누르면 재시작 하고 'q'를 누르면 난이도 선택 메뉴로 넘어갑니다");
				Sleep(300);
				if (kbhit()) {
					ch = getch();
					if (ch == 'r' || ch == 'q')
						break;
				}
			}
			if (ch == 'r')
				goto START;
			else if (ch == 'q') {
				cls(BLACK, WHITE);
				break;
			}
			gotoxy(0, HEIGHT - 1);
		}
		else if ((LIFE2 == 0) || (v_size == 5)) {
			cls(BLACK, WHITE);
			while (1) {
				int c1, c2;
				do { // 색을 변경하면서 Game Over 출력
					c1 = rand() % 16;
					c2 = rand() % 16;
				} while (c1 == c2);
				gotoxy(50, 20);
				printf("1p win");
				textcolor(c1, c2);
				gotoxy(45, 10);
				printf("** Game Over **");
				gotoxy(20, 13);
				textcolor(WHITE, BLACK);
				printf("'R'을 누르면 재시작 하고 'q'를 누르면 난이도 선택 메뉴로 넘어갑니다");
				Sleep(300);
				if (kbhit()) {
					ch = getch();
					if (ch == 'r' || ch == 'q')
						break;
				}
			}
			if (ch == 'r')
				goto START;
			else if (ch == 'q') {
				cls(BLACK, WHITE);
				break;
			}
			gotoxy(0, HEIGHT - 1);
		}
		else if (mon_v_size == 3) {
			cls(BLACK, WHITE);
			while (1) {
				int c1, c2;
				do { // 색을 변경하면서 Game Over 출력
					c1 = rand() % 16;
					c2 = rand() % 16;
				} while (c1 == c2);
				gotoxy(50, 20);
				printf("ZOMBIE WIN");
				textcolor(c1, c2);
				gotoxy(45, 10);
				printf("** Game Over **");
				gotoxy(20, 13);
				textcolor(WHITE, BLACK);
				printf("'R'을 누르면 재시작 하고 'q'를 누르면 난이도 선택 메뉴로 넘어갑니다");
				Sleep(300);
				if (kbhit()) {
					ch = getch();
					if (ch == 'r' || ch == 'q')
						break;
				}
			}
			if (ch == 'r')
				goto START;
			else if (ch == 'q') {
				cls(BLACK, WHITE);
				break;
			}
			gotoxy(0, HEIGHT - 1);

		}
		Sleep(delay);

		// Delay를 줄이면 속도가 빨라진다.
		frame_count++;

	}

}



int main() {
	int i = 4;
	int POS = 0;
	unsigned char ch;
	removeCursor();
	textcolor(WHITE, BLACK);
	draw_box2(0, 0, WIDTH, HEIGHT);

	while (1) {
	TITLE:		
		PlaySound(TEXT("sound.wav"), 0, SND_ASYNC);

		i = title();

		if (i == 0) {
			cls(BLACK, WHITE);
			textcolor(WHITE, BLACK);

			while (1) {

				i = level();

				if (i == 0) {
					gloop1(0);
				}
				else if (i == 1) {
					gloop2(0);
				}
				else if (i == 2) {
					gloop3(0);
				}
				else if (i = 3) {
					goto TITLE;
				}

			}

		}
		else if (i == 1) {
			control();
		}
		else if (i == 2)
			return 0;
	}
	return 0;
}


