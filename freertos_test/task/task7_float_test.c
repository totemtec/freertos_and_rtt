
#include "my_typedef.h"

float g_A;
float g_B;
float g_ADD;
float g_SUB;
float g_MUL;
float g_DIV;

float test_float_val( float t )
{
    return t*t;
}

int float_test_error(void)
{
    bool err = 0;
    //double da = 123;
    //double db = da*23;

    float a = 123.45f;
    float b = 23;

    g_A = 56.789f;
    g_B = 34;

    

    err++; //err = 1
    if( a < b || g_B > g_A || a <= g_A || b >= g_B ) {
        return err;
    }
    
    float add = a + b;
    float sub = a - b;
    float mul = a * b;
    float div = a / b;

    //ee_printf( "+:%d\n", (int)add); //146.45
    //ee_printf( "-:%d\n", (int)sub); //100.45
    //ee_printf( "*:%d\n", (int)mul); //2839.35
    //ee_printf( "/:%d\n", (int)div); //5.36739
    //ee_printf( "^a:%d\n", (int)test_float_val(a)); //15239.9025
    //ee_printf( "^b:%d\n", (int)test_float_val(b)); //529

    err++; // err = 2
    if( (int)add != 146 || (int)sub != 100 || (int)mul != 2839 || (int)div != 5 || (int)test_float_val(a) != 15239 || (int)test_float_val(b) != 529 ) {
        //ee_printf( "E1\n");
        return err;
    }

    g_ADD = g_A + g_B;
    g_SUB = g_A - g_B;
    g_MUL = g_A * g_B;
    g_DIV = g_A / g_B;

    //ee_printf( "+:%d\n", (int)g_ADD); //90.789
    //ee_printf( "-:%d\n", (int)g_SUB); //22.789
    //ee_printf( "*:%d\n", (int)g_MUL); //1930.826
    //ee_printf( "/:%d\n", (int)g_DIV); //1.67026
    //ee_printf( "^A:%d\n", (int)test_float_val(g_A)); //3224.9905
    //ee_printf( "^B:%d\n", (int)test_float_val(g_B)); //1156
    
    err++; //err = 3
    if( (int)g_ADD != 90 || (int)g_SUB != 22 || (int)g_MUL != 1930 || (int)g_DIV != 1 || (int)test_float_val(g_A) != 3224 || (int)test_float_val(g_B) != 1156 ) {
        //ee_printf( "E2\n");
        return err;
    }

    return 0;
}


