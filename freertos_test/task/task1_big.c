






//https://blog.csdn.net/qq_44310495/article/details/109395413

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "my_typedef.h"
#include "task1_big.h"
#include "uart.h"

#define BN 100

int a[BN],b[BN],c[BN],t[BN];

void set_tmp_data( char*str1 )
{
	setZero(t,BN);
	int len1=strlen(str1);
	for(int i=0;i<len1;i++)
		t[i]=str1[len1-1-i]-'0';
}

int big_max(int a,int b){
	if(a>b)return a;
	else return b;
}

int setZero(int*num,int len){
	for(int i=0;i<len;i++)
		num[i]=0;
}

int compare(int*num1,int*num2){
	int i,j;
	for(i=BN-1;num1[i]==0;i--);
	for(j=BN-1;num2[j]==0;j--);
	if(i<j)return -1;
	else if(i>j)return 1;
	else{
		for(;i>=0;i--){
			if(num1[i]>num2[i])
				return 1;
			else if(num1[i]<num2[i])
				return -1;
		}
		return 0;
	}
}

int copy(int*num1,int*num2){
	int i,j;
	for(i=0;i<BN;i++)
		num1[i]=0;
	for(i=BN-1;num2[i]==0;i--);
	for(j=0;j<=i;j++)
		num1[j]=num2[j];
	return i+1;
}

void big_add(char*str1,char*str2){
	setZero(a,BN);
	setZero(b,BN);
	setZero(c,BN);
	//ee_printf("%s + %s = ",str1,str2);
		
	int i,carry=0;
	int len1=strlen(str1),len2=strlen(str2),len=big_max(len1,len2);
	for(i=0;i<len1;i++)
		a[i]=str1[len1-1-i]-'0';
	for(i=0;i<len2;i++)
		b[i]=str2[len2-1-i]-'0';
	for(i=0;i<len;i++){
		c[i]=(a[i]+b[i]+carry)%10;
		carry=(a[i]+b[i]+carry)/10;
	}
	
	if(carry!=0)
		c[len++]=1;
	// for(i=len-1;i>=0;i--)
	// 	my_putchar('0'+c[i]);
	// ee_printf("\n");
}

void big_sub(char*str1,char*str2){
	setZero(a,BN);
	setZero(b,BN);
	setZero(c,BN);
	//ee_printf("%s - %s = ",str1,str2);
	
	int i,borrow=0;
	int len1=strlen(str1),len2=strlen(str2),len=big_max(len1,len2);
	for(i=0;i<len1;i++)
		a[i]=str1[len1-1-i]-'0';
	for(i=0;i<len2;i++)
		b[i]=str2[len2-1-i]-'0';	
	
	if(len2>len1){ 
		my_putchar('-');
		for(i=0;i<len;i++){
			c[i]=b[i]-a[i]-borrow;
			if(c[i]<0){
				borrow=1;
				c[i]+=10;
			}
			else
				borrow=0;
		}
	}
	else{
		for(i=0;i<len;i++){
			c[i]=a[i]-b[i]-borrow;
			if(c[i]<0&&i+1<len){
				borrow=1;
				c[i]+=10;
			}
			else
				borrow=0;
		}
	}
	if(c[len-1]<0){
		my_putchar('-');
		for(i=0;i<len;i++){
			c[i]=b[i]-a[i]-borrow;
			if(c[i]<0&&i+1<len){
				borrow=1;
				c[i]+=10;
			}
			else
				borrow=0;
		}
	}
	
	
	// while(c[len-1]==0&&len>1)
	// 	len--;
	// for(i=len-1;i>=0;i--)
	// 	my_putchar('0'+c[i]);
	//ee_printf("\n");
}

void big_mul(char*str1,char*str2) {
	setZero(a,BN);
	setZero(b,BN);
	setZero(c,BN);
	//ee_printf("%s * %s = ",str1,str2);
	
	int i,j,k,carry=0,tmp; 
	int len1=strlen(str1),len2=strlen(str2);
	for(i=0;i<len1;i++)
		a[i]=str1[len1-1-i]-'0';
	for(i=0;i<len2;i++)
		b[i]=str2[len2-1-i]-'0';
	
	for(i=0;i<len1;i++){
		for(k=i,j=0;j<len2;j++,k++){
			c[k]+=a[i]*b[j];
		}
	}
	for(i=0;i<=k;i++) {
		c[i]+=carry;
		tmp=c[i];
		c[i]=tmp%10;
		carry=tmp/10;
	}
	
	// for(i=k;c[i]==0;i--);
	// for(;i>=0;i--)
	// 	my_putchar('0'+c[i]);
	// ee_printf("\n");
}

int temp[BN]={0},temp2[BN]={0};

void big_div(char*str1,char*str2)
{
	setZero(a,BN);
	setZero(b,BN);
	setZero(c,BN);
	//ee_printf("%s / %s = ",str1,str2);

	int i,j,tmp,tmp2=0,borrow=0,temp[BN]={0},temp2[BN]={0};
	//int i,j,tmp,tmp2=0,borrow=0;

	int len1=strlen(str1),len2=strlen(str2);
	for(i=0;i<len1;i++)
		a[i]=str1[len1-1-i]-'0';

	for(i=0;i<len2;i++)
		b[i]=str2[len2-1-i]-'0';
	
	if(compare(a,b)<0){
		ee_printf("0\n");
		return;
	}

	while(compare(a,b)>=0){			
		tmp=len1-len2;
		if(tmp==tmp2&&tmp>0)
			tmp--;
		tmp2=tmp;
		setZero(temp2,BN);
		for(i=len1-1;i>=tmp;i--)
			temp2[i]=b[i-tmp];
		copy(temp,a);

		if(compare(temp,temp2)<0)
				continue;
		
		for(j=1;;j++){
			borrow=0;
			for(i=tmp;i<len1;i++){
				temp[i]=a[i]-temp2[i]-borrow;
				if(temp[i]<0){
					borrow=1;
					temp[i]+=10;
				}
				else
					borrow=0;
			}
			
			len1=copy(a,temp);
			c[tmp]=j;
			if(compare(temp,temp2)<0)
				break;
		}
	}
	
	// for(i=BN-1;c[i]==0;i--);
	// for(;i>=0;i--)
	// 	my_putchar('0'+c[i]);
	// ee_printf("\n");
}

int task_big(void)
{
	//ee_printf("[+]\n");
	big_add("265151916516519419516","15616651622161651616"); //280768568138681071132
	set_tmp_data( "280768568138681071132" );
	if( compare( c, t ) != 0 ) {
		return 1;
	}
	//ee_printf("[-]\n");
	big_sub("163165165161984116165","15454874487989819449"); //147710290673994296716
	set_tmp_data( "147710290673994296716" );
	if( compare( c, t ) != 0 ) {
		return 2;
	}
	//ee_printf("[*]\n");
	big_mul("798794149194949451949","98491516194941631316"); //78674446881858985595565898955098215634884
	set_tmp_data( "78674446881858985595565898955098215634884" );
	if( compare( c, t ) != 0 ) {
		return 3;
	}
	//ee_printf("[/]\n");
	// big_div("16516498416516516191981916516165","12345678901"); //1337836383803784157076
	// big_div("16516498416516516516165","12345678901"); //1337836383803
	big_div("16516498416165","1234578901"); //13378
	set_tmp_data( "13378" );
	if( compare( c, t ) != 0 ) {
		return 4;
	}
	return 0;
}


