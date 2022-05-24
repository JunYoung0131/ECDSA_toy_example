#include<stdio.h>
#include<math.h>

#define P 17
#define N 19
#define DEBUG 1

// 타원 좌표 
typedef struct Point {   
    int x;
    int y;
} G;

// a^n mod p
int modP(int a, int n, int p){
	int i;
	int result=1;

	for(i=0; i<n; i++){
		result=(result*a)%p;
	}
	
	return result;
}

// nG 계산 
void calculPKey(int d, G* g){
	int i;
	long t;		// 기울기 
	long t_u;	// 분모 
	long t_l;	// 분 

	if(d>=2){
		for(i=0; i<d-1; i++){
			// 2G=G+G
			if(i==0){
				// 분모계산 
				t_u=(3*(modP(g[0].x, 2, P))+2);
				if(t_u<0)	t_u+=P;
				t_u=fmod(t_u, P);

				// 분자계산 
				t_l=2*g[0].y;
				if(t_l<0)	t_l+=P;
				t_l=fmod(modP(t_l, P-2, P), P);
				
				t=fmod(t_u*t_l, P);
				if(t<0)	t+=P;

				// 좌표 계산 
				g[1].x=fmod(modP(t, 2, P)-(2*g[0].x), P);
				if(g[1].x<0)	g[1].x+=P;
				
				g[1].y=fmod((g[0].x-g[1].x)*t-g[0].y, P);
				if(g[1].y<0)	g[1].y+=P;
				
				// TESTING
				if(DEBUG==1){
					printf("[calculPKey] 2G=(%d, %d)\n", g[1].x, g[1].y);		
				}
			}
			// nG=(n-1)G+G
			else{
				// 분모계산 
				t_u=g[i].y-g[0].y;
				if(t_u<0)	t_u+=P;
				t_u=fmod(t_u, P);

				// 분자계산 
				t_l=g[i].x-g[0].x;
				if(t_l<0)	t_l+=P;
				t_l=fmod(modP(t_l, P-2, P), P);
				
				// 기울기가 0인 경우 
				if(t_l==0){
					g[i+1].x=0;
					g[i+1].y=0;
				}
				// 기울기가 0이 아닌 경우 
				else{
					t=fmod(t_u*t_l, P);
					if(t<0)	t+=P;
					
					// 좌표 계산 
					g[i+1].x=fmod(modP(t, 2, P)-g[0].x-g[i].x, P);
					if(g[i+1].x<0)	g[i+1].x+=P;
					
					g[i+1].y=fmod((g[0].x-g[i+1].x)*t-g[0].y, P);
					if(g[i+1].y<0)	g[i+1].y+=P;
				}

				// TESTING
				if(DEBUG==1){
					printf("[calculPKey] %dG=(%d, %d)\n", i+2, g[i+1].x, g[i+1].y);
				}
			}	
		}
	}
}

// 서명 계산 
int signature(int m, int k, int d, int r){
	int s;
	int k_inver;
	int tmp;
	unsigned long tst;
	
	k_inver=fmod(modP(k, N-2, N), N);
	if(k_inver<0)	k_inver+=N;
	
	tmp=fmod(m+(d*r), N);
	if(tmp<0)	tmp+=N;
	
	s=fmod(k_inver*tmp, N);
	
	if(DEBUG==1){
		printf("\n========================\n");
		printf(" [signature] \n");	
		printf("k_inver = %d\n", k_inver);
		printf("tmp = %d\n", tmp);
		printf("\n========================\n");
	}
	
	return s;
}

void intro(){
	printf("=================================\n");
	printf("            ECC 서명\n");
	printf("         p=17, a=2, b=2\n");
	printf("            G=(5, 1)\n");
	printf("=================================\n\n");
}

// main
int main(){
	int d;	// 개인키 
	G g[N];	// 좌표 
	int k;	// 임의의 k 
	int m;	// 메세지 
	int r, s;	// 서명 값 
	
	if(DEBUG==1){
		printf("=====ECDSA 서명(디버깅 모드)=====\n");
	}
	else{
		intro();
	}
	
	// 1G
	g[0].x=5;
	g[0].y=1;

	// 개인키 입력 
	while(1){
		printf("개인키를 입력하세요.(0<d<20): ");
		scanf("%d", &d);
		
		if(0<d && d<20)
			break;
	}
	
	// k 입력 
	while(1){
		printf("k를 입력하세요.(0<k<20): ");
		scanf("%d", &k);
		
		if(0<k && k<20)
			break;
	}
	
	printf("메세지를 입력하세요.: ");
	scanf("%d", &m);

	if(DEBUG==1){
		// 모든 좌표 계산 
		calculPKey(N, g);
	}
	else{
		// kG 계산 
		calculPKey(k, g);
	}
		
	// r 계산 
	r=fmod(g[k-1].x, N);

	if(r==0){
		printf("[error] r=0 입니다. 다시 시도해주세요.\n");
		return 0;
	} 
	
	// 서명 계산 
	s=signature(m, k, d, r);
	
	if(DEBUG==1){
		printf("\n========================\n");
		printf("   [main] 입출력값 \n");	
		printf("d = %d\n", d);
		printf("k = %d\n", k);
		printf("m = %d\n", m);
		printf("r = %d\n", r);
		printf("s = %d\n", s);
		printf("\n========================\n");
	}
	else{
		printf("\n서명값 (%d, %d)\n", r, s);		
	}

	return 0;
}