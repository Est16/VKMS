#include <stdio.h>
#include <math.h>


int main () {
	
	double buf[1];
	buf[0]= 128;
	
	double v[8]= {0, 0, 0, 0, 0, 0, 0, 0};
	double s[8]= {0, 0, 0, 0, 0, 0, 0, 0};
	
	for (int i=0; i<8; i++) {
		
		v[i]= buf[0] & pow(2,i);
		
		
		if (v[i]== 1||v[i]== 2||v[i]== 4||v[i]== 8||v[i]== 16||v[i]== 32||v[i]== 64||v[i]== 128) {
			
			s[i]=1;
			printf("%d", s[i]);
			
		} else 
		{
			s[i]=0;
			printf("%d", s[i]);
		}
			
	}
	
	
	
	
	
	
	
	return 0;
	
	
	
	
}
