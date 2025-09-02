
#include "my_typedef.h"

#include "uart.h"
#include "utils.h"
int n,m;
int map[20][20];
int fx[5] = {0,0,1,0,-1};
int fy[5] = {0,1,0,-1,0};

void search_path( int x, int y, int k );

static uint32 ck = 0;

int task_search(void)
{
	ck = 0;
    n = 3;
    m = 4;
    for( int x = 1; x < 10; x++ ) {
        for( int y = 1; y < 10; y++ ) {
            map[x][y] = 0;
        }
    }
    search_path( 1,1,1 );

    ee_printf( "s-ck:%d\n", ck );
    if( ck == 37330014 ) {
        return 0;
    }
    else {
        return 1;
    }
}
void search_path( int x, int y, int k )
{
    map[x][y] = k;

    ee_printf( "t3 (%d,%d) %d\n", x, y, k );
    ck = check_iter( ck, x );
    ck = check_iter( ck, y );

    int tx,ty;
    for( int i = 1; i <= 4; i++ ) {
        tx = x + fx[i];
        ty = y + fy[i];

        //xprintf( "t1a %d %d\n", tx, ty );

        if( tx>=1 && tx<=n && ty>=1 && ty<=m && map[tx][ty]==0 ) {
            search_path( tx, ty, k+1 );
        }
    }
}


