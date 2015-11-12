/**
	fallout teriminal replica. Includes hacking game.
**/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <ncurses.h>

#define KEY_DELAY 15000

WINDOW *title;
WINDOW *Lhex;
WINDOW *Rhex;
WINDOW *Lboard;
WINDOW *Rboard;
WINDOW *console;

char *random_set = "{}[]<>\"!$^@+*=_,.%|;:\'\\/";

//========================= FUNCTION DECLARATIONS ============================//
void draw_loading_scroll();

void screen_init();

WINDOW * createwin(int height, int width, int starty,int startx);

char * genboard(); //TODO

void term_print(char *str); 

int update_title(int tries);


//===========================================================================//

int main(int argc, char* argv[]) {

	//seed PRNG
	srand(time(NULL));

	// create correct password container
	char *password = malloc(11 * sizeof(char));
	int tries = 4;

	// ncurses initialization
	initscr();
	raw();
	keypad(stdscr, TRUE);
	noecho();
	cbreak();
	nodelay(stdscr, TRUE);

	// game code
	draw_loading_scroll();
	nodelay(stdscr, FALSE);
	getch();
	nodelay(stdscr, TRUE);
	
	clear();
	refresh();

	// draw windows, populate board with random chars and possible passwds.
	// need to write algorithm to find strings between parentheses
	screen_init();
	update_title(tries);

	//TODO input
	nodelay(stdscr, FALSE);
	getch();
	nodelay(stdscr, TRUE);

	// main game loop?
		
	//delete windows
	
	//end ncurses
	endwin();
	return 0;
}

// ugly, consider cleaning up
void draw_loading_scroll() {

	cbreak();

	int i = 0;
	int skip = 0;
	char ch = 'a';
	char *crawl = "\
WELCOME TO ROBCO INDUSTRIES (TM) TERMLINK\n\n\
\
>SET TERMINAL/INQUIRE\n\n\
\
RIT-V300\n\n\
\
>SET FILE/PROTECTION-OWNER:RWED ACCOUNTS.F\n\
>SET HALT RESTART/MAINT\n\n\
\
Initializing Robco Industries(TM) HF Boot Agent v2.3.0\n\
RETROS BIOS\n\
RBIOS-4.02.08.00 52EE5.E7.E8\n\
Copyright 2201-2203 Robco Ind.\n\
Uppermem: 64 KB\n\
Root (5A8)\n\
Maintenance Mode\n\n\
\
>RUN DEBUG/ACCOUNTS.F\n\
";
	for(i = 0; crawl[i] != '\0'; i++) {
		printw("%c", crawl[i]);
		ch = getch();
		if(ch == 's') {
			skip = 1;
		}
		if(!skip) {
			usleep(KEY_DELAY);
		}
		fflush(stdout);
		refresh();
	}
}

// TODO: create screen wipe effect
void screen_init() {

	int i = 0;
	
	// fix window scope	
	title = createwin(4,40,0,0);
	Lhex = createwin(17,6,6,0);
	Rhex = createwin(17,6,6,21);
	Lboard = createwin(17,12,6,8);
	Rboard = createwin(17,12,6,28);
	console = createwin(17,12,6,42);	
	
	//create board array
	char * board  = malloc(409 * sizeof(char));

	//static strings
	char *titlev = "ROBCO INDUSTRIES (TM) TERMLINK PROTOCOL\nENTER PASSWORD NOW";
	char *Lhexv = "0xF9AC0xF9B80xF9C40xF9D00xF9DC0xF9E80xF9F40xFA000xFA0C0x\
FA180xFA240xFA300xFA3C0xFA480xFA540xFA600xFA6C";
	char *Rhexv = "0xFA780xFA840xFA900xFA9C0xFAA80xFAB40xFAC00xFACC0x\
FAD80xFAE40xFAF00xFAFC0xFB080xFB140xFB200xFB2C0xFB38";
	
	//generate title
	werase(title);
	//box(title,0,0);
	mvwprintw(title,0,0,titlev);
	update_title(4);
	wrefresh(title);

	//generate Lhex
	werase(Lhex);
	mvwprintw(Lhex,0,0,Lhexv);
	wrefresh(Lhex);
	
	//generate Rhex)
	werase(Rhex);
	mvwprintw(Rhex,0,0,Rhexv);
	wrefresh(Rhex);
	
	//generate board
	board = genboard();
	char * ltext = malloc(205 * sizeof(char));
	char * rtext = malloc(205 * sizeof(char));	

	//this is ghetto, please clean
	for(i = 0; i < 206; i++) {
		ltext[i] = board[i];
	}
	ltext[204] = '\0';

	for(i = 0; i < 206; i++) {
		rtext[i] = board[i+205];
	}
	rtext[204] = '\0';
	
	werase(Rboard);
	mvwprintw(Lboard,0,0,ltext);
	wrefresh(Lboard);

	werase(Rboard);
	mvwprintw(Rboard,0,0,rtext);
	wrefresh(Rboard);

	//TODO generate console
	wrefresh(console);

}

WINDOW *createwin(int height, int width, int starty, int startx) {
	WINDOW *local_win;

	local_win = newwin(height, width, starty, startx);
	//box(local_win, 0, 0);
	wrefresh(local_win);
	
	return local_win;
}

char * genboard() {
	int i = 0;
	char *str = malloc(409 * sizeof(char));;
	
	FILE *words = fopen("words.txt","r");

	// read in and add words
	// char ** full of words

	for(i = 0; i < 408; i++) {
		str[i] = random_set[rand() % 24];
	}
	
	fclose(words);
	return str;	
}

void term_print(char *str) {
	int i = 0;

	for(i = 0; str[i] != '\0'; i++) {
		printf("%c", str[i]);
		usleep(KEY_DELAY);
	}
}
int update_title(int tries) {
	int i = 0;	
	mvwprintw(title,4,0,"%i ATTEMPTS LEFT: ");
	
	if(tries  > 0) {
		for(i = 0; i < tries; i++) {
			mvwprintw(title,4,18+2*i,"Û");
		}	
	}
	wrefresh(title);
	return 0;
}
