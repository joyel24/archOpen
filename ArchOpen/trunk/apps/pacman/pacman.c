#include <api.h>
#include <sys_def/int_timer.h>
#include <target/chip/osd.h>

#include "gfx.h"
#include "Pac.h"

struct OBJECT{
    int x;
    int y;
    int direction;
    int vuln;
    int loop;
    int last_dir;
};


int level=1;

static unsigned int * screenDirect ;
static unsigned char * screenDirect2 ;

struct OBJECT pac = {160,48,0,0,0,2};
struct OBJECT ghost[4] = {{160,80,0,0,0,2},{160,96,0,0,0,2},{160,112,0,0,0,2},{160,128,0,0,0,2}};

void PutIcon16V(int x, int y, int nb);
void PutIcon16B(int x, int y, int nb, int limit);
void LineV16(int x, int y, int sprite, int sx);
void LineH16(int x, int y, int sprite, int sy);
void End(void);
void MoveGhost(int n);
void DirGhost(int n);
void Show(char* string);

char levelmap[10][15][20]={
                          {{60, 1, 1, 1, 1, 1, 1, 1, 1, 1,  1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                           {61, 1, 2, 2, 2, 2, 2, 2, 2, 2,  1, 2, 2, 2, 2, 2, 2, 2, 2, 1},
                           {66, 1, 2, 1, 1, 2, 1, 1, 1, 2,  1, 2, 1, 1, 1, 2, 1, 1, 2, 1},
                           {67, 1, 2, 2, 2, 2, 2, 2, 2, 2,110, 2, 2, 2, 2, 2, 2, 2, 2, 1},
                           {96, 1, 2, 1, 1, 2, 1, 2, 1, 1,  1, 1, 1, 2, 1, 2, 1, 1, 2, 1},
                           {97, 1, 2, 2, 2, 2, 1, 2, 2, 1,111, 1, 2, 2, 1, 2, 2, 2, 2, 1},
                           { 1, 1, 1, 1, 1, 2, 1, 1, 2, 1,112, 1, 2, 1, 1, 2, 1, 1, 1, 1},
                          {108, 2, 2, 2, 2, 2, 2, 2, 2, 1,113, 1, 2, 2, 2, 2, 2, 2, 2,108},
                           { 1, 1, 2, 1, 1, 2, 1, 1, 2, 1,114, 1, 2, 1, 1, 2, 1, 1, 2, 1},
                           {98, 1, 2, 3, 1, 2, 2, 2, 2, 2,  2, 2, 2, 2, 2, 2, 1, 3, 2, 1},
                           {99, 1, 1, 1, 1, 2, 1, 2, 1, 1,  1, 1, 1, 2, 1, 2, 1, 1, 1, 1},
                           {64, 1, 2, 2, 2, 2, 1, 2, 2, 2,  1, 2, 2, 2, 1, 2, 2, 2, 2, 1},
                           {65, 1, 2, 1, 1, 1, 1, 1, 1, 2,  1, 2, 1, 1, 1, 1, 1, 1, 2, 1},
                           {62, 1, 2, 2, 2, 2, 2, 2, 2, 2,  2, 2, 2, 2, 2, 2, 2, 2, 2, 1},
                           {63, 1, 1, 1, 1, 1, 1, 1, 1, 1,  1, 1, 1, 1, 1, 1, 1, 1, 1, 1}},

                          {{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                           { 1, 2, 2, 2, 2, 2, 2, 2, 2, 2,  2, 2, 2, 2, 1, 2, 2, 1, 1, 1},
                           { 1, 1, 2, 1, 2, 1, 2, 2, 1, 1,110, 1, 1, 2, 2, 1, 2, 2, 1, 1},
                           { 1, 1, 2, 2, 1, 2, 2, 1, 2, 2,  2, 2, 2, 1, 2, 2, 1, 2, 1, 1},
                           { 1, 3, 2, 1, 2, 2, 1, 2, 2, 1,  1, 1, 2, 1, 1, 2, 2, 2, 1, 1},
                           { 1, 1, 1, 2, 2, 1, 1, 2, 1, 1,111, 1, 2, 2, 2, 1, 1, 2, 1, 1},
                          {108, 2, 2, 2, 1, 2, 1, 2, 2, 1,112, 1, 2, 1, 2, 1, 1, 2, 2,108},
                           { 1, 1, 2, 1, 1, 2, 1, 1, 2, 1,113, 1, 2, 1, 2, 2, 1, 2, 1, 1},
                           { 1, 1, 2, 2, 1, 2, 2, 2, 2, 1,114, 1, 2, 1, 2, 1, 1, 2, 2, 1},
                           { 1, 1, 2, 1, 1, 2, 1, 1, 2, 2,  2, 2, 2, 1, 2, 2, 1, 2, 1, 1},
                           { 1, 2, 2, 2, 2, 2, 1, 2, 2, 1,  1, 1, 1, 1, 1, 2, 2, 2, 2, 1},
                           { 1, 2, 1, 1, 1, 1, 1, 2, 1, 2,  3, 1, 2, 2, 2, 2, 1, 1, 2, 1},
                           { 1, 2, 2, 1, 2, 1, 1, 1, 1, 2,  1, 1, 1, 1, 1, 2, 1, 3, 2, 1},
                           { 1, 1, 2, 2, 2, 2, 2, 2, 2, 2,  2, 2, 2, 2, 2, 2, 2, 2, 1, 1},
                           { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  1, 1, 1, 1, 1, 1, 1, 1, 1, 1}},

                          {{ 1, 1, 1, 1, 1, 1, 1, 1, 1,  1, 1, 1,  1, 1, 1, 1, 1, 1, 1, 1},
                           { 1, 2, 2, 2, 2, 2, 2, 2, 2,  2, 2, 2,  2, 2, 2, 2, 2, 2, 2, 1},
                           { 1, 3, 1, 1, 1, 1, 2, 2, 1,  1, 1, 2,  1, 1, 2, 2, 1, 1, 3, 1},
                           { 1, 1, 2, 1, 1, 2, 2, 1, 1,111, 1, 2,  1, 2, 1, 2, 2, 1, 1, 1},
                           { 1, 2, 2, 2, 1, 1, 2, 2, 1,112, 1, 2,  1, 2, 1, 1, 2, 2, 1, 1},
                           { 1, 2, 1, 2, 2, 1, 1, 2, 1,113, 1, 2,  2, 2, 2, 1, 1, 2, 2, 1},
                           { 1, 2, 1, 1, 2, 2, 2, 2, 1,114, 1, 1,  2, 1, 2, 1, 1, 1, 2, 1},
                          {108, 2, 2, 1, 1, 2, 2, 1, 2,  2, 2, 1,  1, 1, 2, 1, 1, 2, 2,108},
                           { 1, 1, 2, 2, 1, 1, 2, 2, 1,  2, 2, 2,  2, 2, 2, 1, 2, 2, 1, 1},
                           { 1, 1, 1, 2, 2, 1, 1, 2, 2,  2, 1, 2,  1, 1, 1, 2, 2, 1, 1, 1},
                           { 1, 1, 2, 2, 2, 2, 1, 1, 1,  2, 1, 2,  1, 1, 2, 2, 2, 2, 1, 1},
                           { 1, 1, 2, 1, 1, 2, 2, 1, 1,  2, 1, 2,  1, 2, 2, 1, 1, 2, 1, 1},
                           { 1, 1, 2, 2, 2, 1, 2, 1, 1,  2, 1, 1,  2, 2, 2, 2, 1, 2, 1, 1},
                           { 1, 1, 1, 1, 2, 2, 2, 2, 2,  2, 2, 2,110, 1, 1, 2, 2, 2, 1, 1},
                           { 1, 1, 1, 1, 1, 1, 1, 1, 1,  1, 1, 1,  1, 1, 1, 1, 1, 1, 1, 1}},

                          {{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
                           { 1, 3, 1, 1, 1, 3, 2, 2, 2, 2,  2,  2,  2,  2, 2, 1, 1, 1, 1, 1},
                           { 1, 2, 2, 2, 2, 1, 1, 1, 1, 1,  1,  1,  1,  1, 2, 2, 2, 2, 1, 1},
                           { 1, 1, 1, 1, 2, 1, 1, 1, 1, 1,  2,  2,  2,  2, 2, 1, 1, 2, 1, 1},
                           { 1, 1, 1, 1, 2, 2, 2, 2, 1, 1,  2,  2,  1,  1, 1, 1, 1, 2, 1, 1},
                           { 1, 1, 1, 1, 1, 1, 1, 2, 1, 2,  2,  2,  2,  2, 2, 2, 2, 2, 1, 1},
                           { 1, 1, 1, 2, 2, 2, 1, 2, 1, 2,  1,  1,  1,  1, 1, 2, 1, 1, 1, 1},
                          {108, 2, 2, 2, 1, 1, 1, 2, 2, 2,114,113,112,111, 1, 2, 1, 2, 2,108},
                           { 1, 1, 1, 2, 2, 2, 2, 2, 1, 1,  1,  1,  1,  1, 1, 2, 1, 1, 2, 1},
                           { 1, 1, 1, 1, 2, 1, 1, 2, 1, 1,110,  1,  1,  2, 2, 2, 1, 1, 2, 1},
                           { 1, 2, 2, 2, 2, 2, 2, 2, 2, 2,  2,  1,  1,  1, 1, 1, 1, 2, 2, 1},
                           { 1, 2, 1, 1, 1, 1, 1, 2, 1, 1,  1,  1,  2,  2, 2, 2, 2, 2, 1, 1},
                           { 1, 2, 2, 2, 1, 1, 1, 2, 2, 2,  2,  1,  2,  1, 1, 1, 2, 1, 1, 1},
                           { 1, 1, 1, 2, 2, 3, 1, 2, 1, 1,  2,  2,  2,  1, 1, 1, 2, 1, 1, 1},
                           { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  1,  1,  1,  1, 1, 1, 1, 1, 1, 1}},

                          {{ 1,  1, 1, 1, 1, 1, 1, 1,  1,109,  1,  1, 1, 1, 1, 1, 1, 1, 1, 1},
                          {108,  3, 1, 2, 1, 3, 1, 2,  1,  2,  1,  2, 1, 2, 2, 2, 2, 2, 2,108},
                           { 1,  1, 1, 2, 1, 2, 1, 2,  1,  2,  1,  2, 1, 1, 1, 1, 1, 2, 1, 1},
                           { 1,  2, 1, 2, 1, 2, 1, 2,  1,  2,  1,  2, 2, 2, 2, 2, 1, 2, 1, 1},
                           { 1,  2, 1, 2, 1, 2, 1, 2,  1,  2,  1,  2, 1, 1, 1, 2, 1, 2, 1, 1},
                           { 1,  2, 1, 2, 1, 2, 2, 2,  1,  2,  2,  2, 2, 2, 1, 2, 1, 2, 1, 1},
                           { 1,  2, 2, 2, 1, 2, 1, 2,  1,  1,  1,  1, 1, 2, 1, 2, 1, 2, 1, 1},
                           { 1,  2, 1, 2, 1, 2, 1, 2,114,113,112,111, 1, 2, 1, 2, 1, 2, 1, 1},
                           { 1,  2, 1, 2, 1, 2, 1, 2,  1,  1,  1,  1, 1, 2, 1, 2, 1, 2, 1, 1},
                           { 1,  2, 1, 2, 2, 2, 1, 2,  2,  2,  1,  2, 2, 2, 1, 2, 1, 2, 1, 1},
                           { 1,  2, 1, 2, 1, 2, 1, 2,  1,  2,  1,  1, 1, 1, 1, 2, 1, 2, 1, 1},
                           { 1,  2, 1, 2, 1, 2, 1, 2,  1,  2,  1,  2, 2, 2, 2, 2, 1, 2, 1, 1},
                           { 1,  2, 1, 2, 1, 2, 1, 2,  1,  2,  1,  1, 1, 1, 1, 2, 1, 2, 1, 1},
                           { 1,110, 1, 2, 1, 2, 1, 2,  1,  2,  1,  2, 2, 2, 2, 2, 2, 2, 1, 1},
                           { 1,  1, 1, 1, 1, 1, 1, 1,  1,109,  1,  1, 1, 1, 1, 1, 1, 1, 1, 1}},

                          {{ 1, 1,  1, 1, 1, 1, 1, 1, 1,  1,109,  1, 1, 1, 1, 1, 1, 1, 1, 1},
                          {108, 2,  2, 2, 2, 2, 2, 2, 1,  1,  2,  1, 1, 2, 2, 2, 2, 2, 2,108},
                           { 1, 2,  1, 1, 1, 2, 2, 1, 2,  2,  2,  2, 2, 1, 2, 2, 1, 1, 2, 1},
                           { 1, 2,  3, 1, 2, 2, 1, 2, 2,  1,  1,  1, 2, 2, 1, 2, 2, 1, 1, 1},
                           { 1, 1,  1, 2, 2, 1, 2, 2, 1,  2,  2,  2, 1, 2, 2, 1, 2, 2, 1, 1},
                           { 1, 1,  2, 2, 1, 2, 2, 1, 2,  2,  1,  2, 2, 1, 2, 2, 1, 2, 2, 1},
                           { 1, 1,  2, 1, 1, 2, 1, 2, 2,  1,112,  1, 2, 2, 1, 2, 1, 1, 2, 1},
                           { 1, 1,110, 1, 1, 2, 1, 2, 1,111,  2,113, 1, 2, 1, 2, 1, 1, 2, 1},
                           { 1, 1,  2, 1, 1, 2, 1, 2, 2,  1,114,  1, 2, 2, 1, 2, 1, 1, 2, 1},
                           { 1, 1,  2, 2, 1, 2, 2, 1, 2,  2,  2,  2, 2, 1, 2, 2, 1, 2, 2, 1},
                           { 1, 1,  1, 2, 2, 1, 2, 2, 1,  1,  2,  1, 1, 2, 2, 1, 2, 2, 1, 1},
                           { 1, 2,  3, 1, 2, 2, 1, 2, 2,  2,  2,  2, 2, 2, 1, 2, 2, 1, 1, 1},
                           { 1, 2,  1, 1, 1, 2, 2, 1, 1,  1,  1,  1, 1, 1, 2, 2, 1, 1, 2, 1},
                          {108, 2,  2, 2, 2, 2, 2, 2, 2,  2,  2,  2, 2, 2, 2, 2, 2, 2, 2,108},
                           { 1, 1,  1, 1, 1, 1, 1, 1, 1,  1,109,  1, 1, 1, 1, 1, 1, 1, 1, 1}},

                          {{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,109, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                          {108, 2, 2, 2, 2, 2, 2, 2, 2, 2,110, 2, 2, 2, 2, 2, 2, 2, 2,108},
                           { 1, 1, 2, 1, 1, 1, 1, 1, 1, 1,  1, 1, 1, 1, 1, 1, 1, 1, 2, 1},
                           { 1, 1, 2, 1, 2, 2, 2, 2, 2, 2,  2, 2, 2, 2, 2, 2, 2, 1, 2, 1},
                           { 1, 1, 2, 1, 2, 1, 1, 1, 1, 1,  1, 1, 1, 1, 1, 1, 2, 1, 2, 1},
                           { 1, 1, 2, 1, 2, 1, 2, 2, 2, 1,111, 1, 2, 2, 2, 1, 2, 1, 2, 1},
                           { 1, 1, 2, 1, 2, 1, 2, 1, 2, 1,112, 1, 2, 1, 2, 1, 2, 1, 2, 1},
                           { 1, 1, 2, 2, 2, 1, 2, 1, 3, 1,113, 1, 3, 1, 2, 1, 2, 2, 2, 1},
                           { 1, 1, 2, 1, 2, 1, 2, 1, 1, 1,114, 1, 1, 1, 2, 1, 2, 1, 2, 1},
                           { 1, 1, 2, 1, 2, 1, 2, 2, 2, 2,  2, 2, 2, 2, 2, 1, 2, 1, 2, 1},
                           { 1, 1, 2, 1, 2, 1, 1, 1, 1, 1,  2, 1, 1, 1, 1, 1, 2, 1, 2, 1},
                           { 1, 1, 2, 1, 2, 2, 2, 2, 2, 2,  2, 2, 2, 2, 2, 2, 2, 1, 2, 1},
                           { 1, 1, 2, 1, 1, 1, 1, 1, 1, 1,  1, 1, 1, 1, 1, 1, 1, 1, 2, 1},
                          {108, 2, 2, 2, 2, 2, 2, 2, 2, 2,  2, 2, 2, 2, 2, 2, 2, 2, 2,108},
                           { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,109, 1, 1, 1, 1, 1, 1, 1, 1, 1}},

                          {{ 1, 1,  1, 1, 1, 1, 1, 1, 1, 1,  1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                           { 1, 1,  2, 2, 2, 2, 1, 2, 2, 2,  1, 2, 2, 2, 1, 2, 2, 2, 1, 1},
                           { 1, 1,  1, 1, 1, 2, 2, 2, 1, 2,  2, 2, 1, 2, 2, 2, 1, 1, 2, 1},
                           { 1, 1,  2, 2, 2, 1, 2, 1, 1, 1,  2, 1, 1, 1, 2, 1, 2, 2, 2, 1},
                           { 1, 2,  2, 1, 2, 2, 1, 1, 2, 2,  2, 2, 2, 1, 1, 2, 2, 1, 2, 1},
                           { 1, 1,  2, 2, 1, 2, 2, 1, 2, 1,  1, 1, 2, 1, 2, 2, 1, 2, 2, 1},
                           { 1, 3,  2, 1, 2, 1, 2, 1, 2, 1,111, 1, 2, 1, 2, 1, 2, 1, 2, 1},
                           { 1, 1,110, 2, 2, 1, 2, 2, 2, 1,112, 1, 2, 2, 2, 1, 2, 3, 2, 1},
                           { 1, 3,  2, 1, 2, 1, 2, 1, 2, 1,113, 1, 2, 1, 2, 1, 2, 1, 2, 1},
                           { 1, 1,  2, 2, 1, 2, 2, 1, 2, 1,114, 1, 2, 1, 2, 2, 1, 2, 2, 1},
                           { 1, 2,  2, 1, 2, 2, 1, 1, 2, 2,  2, 2, 2, 1, 1, 2, 2, 1, 2, 1},
                           { 1, 1,  2, 2, 2, 1, 2, 1, 1, 1,  2, 1, 1, 1, 2, 1, 2, 2, 2, 1},
                           { 1, 1,  1, 1, 1, 2, 2, 2, 1, 2,  2, 2, 1, 2, 2, 2, 1, 1, 2, 1},
                           { 1, 1,  2, 2, 2, 2, 1, 2, 2, 2,  1, 2, 2, 2, 1, 2, 2, 2, 1, 1},
                           { 1, 1,  1, 1, 1, 1, 1, 1, 1, 1,  1, 1, 1, 1, 1, 1, 1, 1, 1, 1}},

                          {{ 1,109, 1, 1, 1,109, 1, 1, 1,109, 1, 1, 1,  1,  1, 1, 1, 1, 1, 1},
                           { 1,  2, 1, 2, 2,  2, 1, 2, 2,  2, 1, 2, 2,  2,  1, 1, 2, 3, 1, 1},
                           { 1,  2, 1, 2, 1,  2, 1, 2, 1,  2, 1, 2, 1,  2,  1, 2, 2, 1, 2, 1},
                           { 1,  2, 1, 2, 1,  2, 1, 2, 1,  2, 2, 2, 1,  2,  2, 2, 1, 1, 2, 1},
                           { 1,  2, 1, 2, 1,  2, 1, 2, 1,  2, 1, 2, 1,  2,  1, 1, 2, 2, 2, 1},
                           { 1,  2, 1, 2, 1,  2, 1, 2, 1,  2, 1, 2, 2,  2,  1, 2, 1, 2, 1, 1},
                           { 1,  2, 1, 2, 1,  2, 1, 2, 1,  2, 1, 1, 2,  1,111, 2, 2, 2, 2, 1},
                           { 1,110, 2, 2, 1,  2, 3, 2, 1,  2, 1, 2, 2,114,112, 1, 3, 1, 2, 1},
                           { 1,  2, 1, 2, 1,  2, 1, 2, 1,  2, 1, 1, 2,  1,113, 2, 2, 2, 2, 1},
                           { 1,  2, 1, 2, 1,  2, 1, 2, 1,  2, 1, 2, 2,  2,  1, 2, 1, 2, 1, 1},
                           { 1,  2, 1, 2, 1,  2, 1, 2, 1,  2, 1, 2, 1,  2,  1, 1, 2, 2, 2, 1},
                           { 1,  2, 1, 2, 1,  2, 1, 2, 1,  2, 2, 2, 1,  2,  2, 2, 1, 1, 2, 1},
                           { 1,  2, 1, 2, 1,  2, 1, 2, 1,  2, 1, 2, 1,  2,  1, 2, 2, 1, 2, 1},
                           { 1,  2, 1, 2, 2,  2, 1, 2, 2,  2, 1, 2, 2,  2,  1, 1, 2, 3, 1, 1},
                           { 1,109, 1, 1, 1,109, 1, 1, 1,109, 1, 1, 1,  1,  1, 1, 1, 1, 1, 1}},

                          {{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,109, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                           { 1, 1, 2, 2, 2, 2, 2, 2, 2, 2,  2, 2, 2, 2, 2, 2, 2, 2, 3, 1},
                           { 1, 1, 2, 1, 2, 2, 2, 2, 2, 2,  2, 2, 2, 2, 2, 2, 2, 1, 2, 1},
                           { 1, 1, 2, 2, 2, 2, 2, 2, 2, 2,  2, 2, 2, 2, 2, 2, 2, 2, 2, 1},
                           { 1, 1, 2, 2, 2, 2, 2, 2, 2, 2,110, 2, 2, 2, 2, 2, 2, 2, 2, 1},
                           { 1, 1, 2, 2, 2, 2, 2, 2, 2, 1,  1, 1, 2, 2, 2, 2, 2, 2, 2, 1},
                           { 1, 1, 2, 2, 2, 2, 2, 2, 2, 1,111, 1, 2, 2, 2, 2, 2, 2, 2, 1},
                          {108, 2, 2, 2, 2, 2, 2, 2, 2, 1,112, 1, 2, 2, 2, 2, 2, 2, 2,108},
                           { 1, 1, 2, 2, 2, 2, 2, 2, 2, 1,113, 1, 2, 2, 2, 2, 2, 2, 2, 1},
                           { 1, 1, 2, 2, 2, 2, 2, 2, 2, 1,114, 1, 2, 2, 2, 2, 2, 2, 2, 1},
                           { 1, 1, 2, 2, 2, 2, 2, 2, 2, 2,  2, 2, 2, 2, 2, 2, 2, 2, 2, 1},
                           { 1, 1, 2, 2, 2, 2, 2, 2, 2, 2,  2, 2, 2, 2, 2, 2, 2, 2, 2, 1},
                           { 1, 1, 2, 1, 2, 2, 2, 2, 2, 2,  2, 2, 2, 2, 2, 2, 2, 1, 2, 1},
                           { 1, 1, 3, 2, 2, 2, 2, 2, 2, 2,  2, 2, 2, 2, 2, 2, 2, 2, 2, 1},
                           { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,109, 1, 1, 1, 1, 1, 1, 1, 1, 1}}};


char levelchoice[15][20];

int app_main(int argc, char * * argv)
{
    int i,j,c;
    int total=0,totalsv=0,count=0,countb=0,life=3,score=0,bonusx=0,bonusy=0,start[5][2],x_ori,y_ori,delay=10000;
    char buf[14];

    gfx_openGraphics();

    gfx_planeHide(BMAP1);
    gfx_planeHide(BMAP2);
    gfx_planeHide(VID1);
    gfx_planeHide(VID2);
    
    gfx_planeSetSize(VID1,320,240,32);
    gfx_planeSetSize(BMAP1,320,240,8);
    gfx_planeSetSize(BMAP2,96,32,8);
    gfx_planeGetPos(BMAP1,&x_ori,&y_ori);

    gfx_planeSetPos(BMAP2,x_ori+112*2,y_ori+104);
    gfx_planeSetPos(VID1,x_ori,y_ori);
    gfx_planeSetPos(BMAP1,x_ori,y_ori);

    gfx_planeSetState(BMAP1,(gfx_planeGetState(BMAP1) & 0xFFC7) | OSD_BITMAP_0TRANS | OSD_BITMAP_A0);
    gfx_planeSetState(BMAP2,(gfx_planeGetState(BMAP2) & 0xFFC7) | OSD_BITMAP_0TRANS | OSD_BITMAP_A0);

    set_timer_status(LCD_TIMER,TIMER_MODE_BAT,MODE_DISABLE);
    set_timer_status(LCD_TIMER,TIMER_MODE_DC,MODE_DISABLE);
    set_timer_status(HALT_TIMER,TIMER_MODE_BAT,MODE_DISABLE);
    set_timer_status(HALT_TIMER,TIMER_MODE_DC,MODE_DISABLE);

    gfx_fontSet(STD8X13);

    srand(tmr_getTick());

    (unsigned int*)screenDirect=gfx_planeGetBufferOffset(VID1);
    (unsigned char*)screenDirect2=gfx_planeGetBufferOffset(BMAP1);

    for(i=0;i<240;i++){
        for(j=0;j<320;j++){
            screenDirect[320*i+j] = Pac_data[320*i+j]|((Pac_data[320*i+j]>>8)<<24);
        }
    }

    for (i=0;i<104;i++) {
        gfx_setPalletteRGB(PalData[3*i], PalData[3*i+1], PalData[3*i+2], i);
    }

    gfx_planeShow(VID1);
    gfx_setPlane(BMAP1);
    gfx_clearScreen(COLOR_BLACK);

    while(!btn_readState());
    while(btn_readState());

    gfx_setPlane(VID1);
    gfx_clearScreen(COLOR32_BLACK);

startb:

    gfx_setPlane(BMAP1);
    gfx_clearScreen(COLOR_BLACK);

    for(i=0;i<15;i++){
        for(j=0;j<20;j++){
            levelchoice[i][j]=0;
                if(levelmap[level-1][i][j]!=1) {
                if(levelmap[level-1][i-1][j]!=1) levelchoice[i][j]++;
                if(levelmap[level-1][i][j+1]!=1) levelchoice[i][j]++;
                if(levelmap[level-1][i+1][j]!=1) levelchoice[i][j]++;
                if(levelmap[level-1][i][j-1]!=1) levelchoice[i][j]++;
            }
            if(levelmap[level-1][i][j]==1) PutIcon16V(j*16,i*16,1);
            else if(i==0 && j==19) PutIcon16V(j*16,i*16,7);
            else if(i==0) {
                if(levelmap[level-1][i][j+1]==1) PutIcon16V(j*16,i*16,0);
                else PutIcon16V(j*16,i*16,7);
            }
            else if(j==19) {
                if(levelmap[level-1][i-1][j]==1) PutIcon16V(j*16,i*16,6);
                else PutIcon16V(j*16,i*16,7);
            }
            else {
                if(levelmap[level-1][i-1][j]!=1 && levelmap[level-1][i-1][j+1]!=1 && levelmap[level-1][i][j+1]==1) PutIcon16V(j*16,i*16,0);
                else if(levelmap[level-1][i-1][j]!=1 && levelmap[level-1][i-1][j+1]==1) PutIcon16V(j*16,i*16,4);
                else if(levelmap[level-1][i-1][j]==1 && levelmap[level-1][i-1][j+1]==1) PutIcon16V(j*16,i*16,5);
                else if(levelmap[level-1][i-1][j]==1 && levelmap[level-1][i-1][j+1]!=1) PutIcon16V(j*16,i*16,6);
                else PutIcon16V(j*16,i*16,7);
            }
            if(levelmap[level-1][i][j]==108) PutIcon16V(j*16,i*16,2);
            else if(levelmap[level-1][i][j]==109) PutIcon16V(j*16,i*16,3);
        }
    }

    count=0;
    countb=0;
    total=0;
    for(i=0;i<15;i++){
        for(j=0;j<20;j++) {
            if(levelmap[level-1][i][j]==2) total++;
            else if(levelmap[level-1][i][j]==110) {start[0][0]=j;start[0][1]=i;}
            else if(levelmap[level-1][i][j]==111) {start[1][0]=j;start[1][1]=i;}
            else if(levelmap[level-1][i][j]==112) {start[2][0]=j;start[2][1]=i;}
            else if(levelmap[level-1][i][j]==113) {start[3][0]=j;start[3][1]=i;}
            else if(levelmap[level-1][i][j]==114) {start[4][0]=j;start[4][1]=i;}
        }
    }
    totalsv=1 + rand() % total;

    gfx_planeShow(BMAP1);
    start:
    gfx_setPlane(BMAP1);
    gfx_clearScreen(COLOR_BLACK);
    gfx_putS(0x2F,0x01, 18, 226, "Score:");
    gfx_putS(0x2F,0x01, 188, 226, "Level:");
    sprintf(buf,"%d",score);
    gfx_putS(0x2F,0x01, 66, 226, buf);
    sprintf(buf,"%d",level);
    gfx_putS(0x2F,0x01, 236, 226, buf);

    pac.x=start[0][0]*16;
    pac.y=start[0][1]*16;
    ghost[0].x=start[1][0]*16;
    ghost[0].y=start[1][1]*16;
    ghost[1].x=start[2][0]*16;
    ghost[1].y=start[2][1]*16;
    ghost[2].x=start[3][0]*16;
    ghost[2].y=start[3][1]*16;
    ghost[3].x=start[4][0]*16;
    ghost[3].y=start[4][1]*16;
    pac.direction=0;
    ghost[0].direction=0;
    ghost[1].direction=0;
    ghost[2].direction=0;
    ghost[3].direction=0;
    pac.last_dir=2;
    ghost[0].last_dir=2;
    ghost[1].last_dir=2;
    ghost[2].last_dir=2;
    ghost[3].last_dir=2;
    pac.loop=0;
    ghost[0].loop=0;
    ghost[1].loop=0;
    ghost[2].loop=0;
    ghost[3].loop=0;
    pac.vuln=0;
    ghost[0].vuln=0;
    ghost[1].vuln=0;
    ghost[2].vuln=0;
    ghost[3].vuln=0;

    for(i=1;i<14;i++){
        for(j=1;j<19;j++){
            if(levelmap[level-1][i][j]>=2) PutIcon16B(j*16,i*16,levelmap[level-1][i][j],0);
        }
    }

    if(life>1)  {PutIcon16B(7*16,14*16,29,0); sprintf(buf,"X %d",life-1); gfx_putS(0x2F,0x01,8*16+8,226, buf);}
    else PutIcon16B(7*16,14*16,100,0);
    PutIcon16B(pac.x,pac.y,69,1);
    for(i=0;i<4;i++) PutIcon16B(ghost[i].x,ghost[i].y,69+4*(i+1),1);

    Show("Ready?");

    while(1) {
        c = btn_readState();
        if((!pac.direction && !total) || ((c & 0x10) && (c & 0x40))) {
            for(i=0;i<4;i++) {
                PutIcon16B(pac.x,pac.y,28+i,0);
                for(j=0;j<60000;j++);                                                           //little delay...
            }
            PutIcon16B(pac.x,pac.y,69,1);
            if(level==10) {Show("The End!"); End(); return 1;}
            level++;
            life++;
            goto startb;
        }
        if(c & 0x2000) {End(); return 0;}
        else {
            if(c & 0x8 && levelmap[level-1][pac.y/16][pac.x/16+1]!=1 && !pac.direction) {pac.direction=3; pac.last_dir=pac.direction; pac.loop=16;}
            else if(c & 0x4 && levelmap[level-1][pac.y/16][pac.x/16-1]!=1 && !pac.direction) {pac.direction=1; pac.last_dir=pac.direction; pac.loop=16;}
            else if(c & 0x1 && levelmap[level-1][pac.y/16-1][pac.x/16]!=1 && !pac.direction) {pac.direction=4; pac.last_dir=pac.direction; pac.loop=16;}
            else if(c & 0x2 && levelmap[level-1][pac.y/16+1][pac.x/16]!=1 && !pac.direction) {pac.direction=2; pac.last_dir=pac.direction; pac.loop=16;}
        }
        if(pac.direction==3) {LineV16(pac.x,pac.y,0,0); pac.x++; pac.loop--;}
        else if(pac.direction==1) {LineV16(pac.x+15,pac.y,0,0); pac.x--; pac.loop--;}
        else if(pac.direction==4) {LineH16(pac.x,pac.y+15,0,0); pac.y--; pac.loop--;}
        else if(pac.direction==2) {LineH16(pac.x,pac.y,0,0); pac.y++; pac.loop--;}

        DirGhost(0);
        MoveGhost(0);
        DirGhost(1);
        MoveGhost(1);
        DirGhost(2);
        MoveGhost(2);
        DirGhost(3);
        MoveGhost(3);

        if(pac.direction) {
            if((15-pac.loop)/4>=2) PutIcon16B(pac.x,pac.y,3+pac.direction+32*((15-pac.loop)/4-1),1);
            else PutIcon16B(pac.x,pac.y,35+pac.direction-32*((15-pac.loop)/4),1);
        }
        else PutIcon16B(pac.x,pac.y,3+pac.last_dir+64,1);
        if(ghost[0].direction) {
            if((15-ghost[0].loop)/4>=2) PutIcon16B(ghost[0].x,ghost[0].y,7*!ghost[0].vuln+23*ghost[0].vuln+ghost[0].direction+32*((15-ghost[0].loop)/4-1),1);
            else PutIcon16B(ghost[0].x,ghost[0].y,32+7*!ghost[0].vuln+23*ghost[0].vuln+ghost[0].direction-32*((15-ghost[0].loop)/4),1);
        }
        if(ghost[1].direction) {
            if((15-ghost[1].loop)/4>=2) PutIcon16B(ghost[1].x,ghost[1].y,11*!ghost[1].vuln+23*ghost[1].vuln+ghost[1].direction+32*((15-ghost[1].loop)/4-1),1);
            else PutIcon16B(ghost[1].x,ghost[1].y,32+11*!ghost[1].vuln+23*ghost[1].vuln+ghost[1].direction-32*((15-ghost[1].loop)/4),1);
        }
        if(ghost[2].direction) {
            if((15-ghost[2].loop)/4>=2) PutIcon16B(ghost[2].x,ghost[2].y,15*!ghost[2].vuln+23*ghost[2].vuln+ghost[2].direction+32*((15-ghost[2].loop)/4-1),1);
            else PutIcon16B(ghost[2].x,ghost[2].y,32+15*!ghost[2].vuln+23*ghost[2].vuln+ghost[2].direction-32*((15-ghost[2].loop)/4),1);
        }
        if(ghost[3].direction) {
            if((15-ghost[3].loop)/4>=2) PutIcon16B(ghost[3].x,ghost[3].y,19*!ghost[3].vuln+23*ghost[3].vuln+ghost[3].direction+32*((15-ghost[3].loop)/4-1),1);
            else PutIcon16B(ghost[3].x,ghost[3].y,32+19*!ghost[3].vuln+23*ghost[3].vuln+ghost[3].direction-32*((15-ghost[3].loop)/4),1);
        }
        else PutIcon16B(ghost[3].x,ghost[3].y,19+ghost[3].last_dir+64,1);

        if(levelmap[level-1][pac.y/16][pac.x/16]==2 && !pac.loop) {
            levelmap[level-1][pac.y/16][pac.x/16]=0;
            total--;
            score++;
            sprintf(buf,"%d",score);
            gfx_putS(0x2F,0x01, 66, 226, buf);
        }
        else if(levelmap[level-1][pac.y/16][pac.x/16]==3 && !pac.loop) {
            levelmap[level-1][pac.y/16][pac.x/16]=0;
            ghost[0].vuln=1;
            ghost[1].vuln=1;
            ghost[2].vuln=1;
            ghost[3].vuln=1;
            levelmap[level-1][start[4][1]][start[4][0]]=1;
            PutIcon16B(16*start[4][0],16*start[4][1],1,0);
            count=1000;
        }
        else if((levelmap[level-1][pac.y/16][pac.x/16]==64 ||
                 levelmap[level-1][pac.y/16][pac.x/16]==65 ||
                 levelmap[level-1][pac.y/16][pac.x/16]==66 ||
                 levelmap[level-1][pac.y/16][pac.x/16]==67 ||
                 levelmap[level-1][pac.y/16][pac.x/16]==96 ||
                 levelmap[level-1][pac.y/16][pac.x/16]==97 ||
                 levelmap[level-1][pac.y/16][pac.x/16]==98 ||
                 levelmap[level-1][pac.y/16][pac.x/16]==99) && !pac.loop) {
            levelmap[level-1][pac.y/16][pac.x/16]=0;
            score+=60;
            sprintf(buf,"%d",score);
            gfx_putS(0x2F,0x01, 66, 226, buf);
        }

        if(!pac.loop) pac.direction=0;
        if(!ghost[0].loop) ghost[0].direction=0;
        if(!ghost[1].loop) ghost[1].direction=0;
        if(!ghost[2].loop) ghost[2].direction=0;
        if(!ghost[3].loop) ghost[3].direction=0;
        if(pac.x>=304 || pac.x<=0) {pac.x=304-pac.x; pac.loop=16; pac.direction=pac.last_dir;}
        if(pac.y>=224 || pac.y<=0) {pac.y=224-pac.y; pac.loop=16; pac.direction=pac.last_dir;}
        for(c=0;c<4;c++) {
            if(ghost[c].x>=304 || ghost[c].x<=0) {ghost[c].x=304-ghost[c].x; ghost[c].loop=16; ghost[c].direction=ghost[c].last_dir;}
            if(ghost[c].y>=224 || ghost[c].y<=0) {ghost[c].y=224-ghost[c].y; ghost[c].loop=16; ghost[c].direction=ghost[c].last_dir;}
            if(abs(pac.x-ghost[c].x)<15 && abs(pac.y-ghost[c].y)<15) {
                if(!ghost[c].vuln) {
                    for(i=0;i<4;i++) {
                        PutIcon16B(pac.x,pac.y,92+i,1);
                        for(j=0;j<60000;j++);                                                           //little delay...
                    }
                    PutIcon16B(pac.x,pac.y,107,1);
                    life--;
                    if(!life) {Show("Game Over!"); End(); return 0;}
                    else {Show("Too bad..."); goto start;}
                }
                else {
                    PutIcon16B(ghost[c].x,ghost[c].y,107,1);
                    ghost[c].vuln=0;
                    ghost[c].x=start[(c%3)+1][0]*16;
                    ghost[c].y=start[(c%3)+1][1]*16;
                    ghost[c].loop=0;
                    ghost[c].direction=0;
                    ghost[c].last_dir=2;
                    score+=20;
                    sprintf(buf,"%d",score);
                    gfx_putS(0x2F,0x01, 66, 226, buf);
                }
            }
        }
        if(total==totalsv) {
            totalsv=-1;
            bonusx=rand() % 20;
            bonusy=rand() % 15;
            while(levelmap[level-1][bonusy][bonusx]!=0) {
               bonusx=rand() % 20;
               bonusy=rand() % 15;
            }
            levelmap[level-1][bonusy][bonusx]=64+(rand()%4)+32*(rand()%2);
            PutIcon16B(bonusx*16,bonusy*16,levelmap[level-1][bonusy][bonusx],0);
            countb=700;
        }
        if(countb>0) {countb--; if(countb<=0) {levelmap[level-1][bonusy][bonusx]=0;PutIcon16B(bonusx*16,bonusy*16,107,0);}}
        if(count>0) {count--; if(count<=0) {ghost[0].vuln=0; ghost[1].vuln=0; ghost[2].vuln=0; ghost[3].vuln=0; levelmap[level-1][start[4][1]][start[4][0]]=114; PutIcon16B(16*start[4][0],16*start[4][1],107,0);}}
    
        for(i=0;i<delay;i++) /* Nothing */ ;

    }
}

void PutIcon16V(int x, int y, int nb)
{
    int i,j,k;
    unsigned int* sprite = (unsigned int*) gfx_data;
    k = nb/4;
    nb = nb % 4;
    for(i=0;i<16;i++){
        for(j=0;j<16;j++){
            screenDirect[320*(y+i)+x+j] = sprite[64*(16*k+i)+16*nb+j]|((sprite[64*(16*k+i)+16*nb+j]>>8)<<24);
        }
    }
}

void PutIcon16B(int x, int y, int nb, int limit)
{
    int i,j,k,i1=0,j1=0,i2=16,j2=16;
    k=nb/32;
    nb=nb%32;
    if(limit) {
        if(x<16) j1=16-x;
        else if(x>288) j2=304-x;
        if(y<16) i1=16-y;
        else if(y>208) i2=224-y;
    }
    for(i=i1;i<i2;i++){
        for(j=j1;j<j2;j++){
            screenDirect2[(320*(y+i)+x+j)] = gfx2[512*(16*k+i)+16*nb+j];
        }
    }
}

void LineV16(int x, int y, int sprite, int sx)
{
    int i,k;
    if(sprite) {
        k=sprite/32;
        sprite=sprite%32;
        for(i=0;i<16;i++) screenDirect2[(320*(y+i)+x)] = gfx2[512*(16*k+i)+16*sprite+sx];
    }
    else for(i=0;i<16;i++) screenDirect2[(320*(y+i)+x)] = 0x00;
}

void LineH16(int x, int y, int sprite, int sy)
{
    int i,k;
    if(sprite) {
        k=sprite/32;
        sprite=sprite%32;
        for(i=0;i<16;i++) screenDirect2[(320*y+x+i)] = gfx2[512*(16*k+sy)+16*sprite+i];
    }
    else for(i=0;i<16;i++) screenDirect2[(320*y+x+i)] = 0x00;
}

void End(void)
{
    gfx_closeGraphics();
}

void Show(char* string)
{
    gfx_setPlane(BMAP2);
    gfx_fillRect(0x2F, 0, 0, 96, 32);
    gfx_fillRect(0X01, 2, 2, 92, 28);
    gfx_putS(0x2F,0x01, (96-strlen(string)*8)/2, 10, string);
    gfx_planeShow(BMAP2);
    while(!btn_readState());
    while(btn_readState());
    gfx_planeHide(BMAP2);
    gfx_setPlane(BMAP1);
    return;
}

void DirGhost(int n)
{
    if(!ghost[n].direction) {
        ghost[n].loop=16;
        if((levelmap[level-1][ghost[n].y/16][ghost[n].x/16-1]==1 && ghost[n].last_dir==1)
        || (levelmap[level-1][ghost[n].y/16+1][ghost[n].x/16]==1 && ghost[n].last_dir==2)
        || (levelmap[level-1][ghost[n].y/16][ghost[n].x/16+1]==1 && ghost[n].last_dir==3)
        || (levelmap[level-1][ghost[n].y/16-1][ghost[n].x/16]==1 && ghost[n].last_dir==4)
        || levelchoice[ghost[n].y/16][ghost[n].x/16]>2) {
            if((rand() % 9)>=(level-1)) {
                ghost[n].direction=rand() % 4 +1;
                while((levelmap[level-1][ghost[n].y/16][ghost[n].x/16-1]==1 && ghost[n].direction==1)
                || (levelmap[level-1][ghost[n].y/16+1][ghost[n].x/16]==1 && ghost[n].direction==2)
                || (levelmap[level-1][ghost[n].y/16][ghost[n].x/16+1]==1 && ghost[n].direction==3)
                || (levelmap[level-1][ghost[n].y/16-1][ghost[n].x/16]==1 && ghost[n].direction==4)) {ghost[n].direction=rand() % 4 +1;}
            }
            else {
                if (abs(pac.y-ghost[n].y) > abs(pac.x-ghost[n].x)) {
                    if (pac.y < ghost[n].y) ghost[n].direction = 4-2*ghost[n].vuln;
                    else ghost[n].direction = 2+2*ghost[n].vuln;
                }
                else {
                    if (pac.x < ghost[n].x) ghost[n].direction = 1+2*ghost[n].vuln;
                    else ghost[n].direction = 3-2*ghost[n].vuln;
                }
                while((levelmap[level-1][ghost[n].y/16][ghost[n].x/16-1]==1 && ghost[n].direction==1)
                || (levelmap[level-1][ghost[n].y/16+1][ghost[n].x/16]==1 && ghost[n].direction==2)
                || (levelmap[level-1][ghost[n].y/16][ghost[n].x/16+1]==1 && ghost[n].direction==3)
                || (levelmap[level-1][ghost[n].y/16-1][ghost[n].x/16]==1 && ghost[n].direction==4)) ghost[n].direction=ghost[n].direction % 4 + 1;

            }
        }
        else ghost[n].direction=ghost[n].last_dir;
        ghost[n].last_dir=ghost[n].direction;
    }
}

void MoveGhost(int n)
{
    if(ghost[n].direction==3) {LineV16(ghost[n].x,ghost[n].y,levelmap[level-1][ghost[n].y/16][ghost[n].x/16],16-ghost[n].loop); ghost[n].x++; ghost[n].loop--;}
    else if(ghost[n].direction==1) {LineV16(ghost[n].x+15,ghost[n].y,levelmap[level-1][ghost[n].y/16][(ghost[n].x+15)/16],ghost[n].loop-1); ghost[n].x--; ghost[n].loop--;}
    else if(ghost[n].direction==4) {LineH16(ghost[n].x,ghost[n].y+15,levelmap[level-1][(ghost[n].y+15)/16][ghost[n].x/16],ghost[n].loop-1); ghost[n].y--; ghost[n].loop--;}
    else if(ghost[n].direction==2) {LineH16(ghost[n].x,ghost[n].y,levelmap[level-1][ghost[n].y/16][ghost[n].x/16],16-ghost[n].loop); ghost[n].y++; ghost[n].loop--;}
}
