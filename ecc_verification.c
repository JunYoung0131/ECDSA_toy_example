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

// 서명 검증
void verification(int m, int r, int s, G* q, G* g){
	int w;
	int u1, u2;
	int x1, x2;
	int v;
	
	// w 계산 
	w=fmod(modP(s, N-2, N), N);
	if(w<0)	w+=N;
	
	u1=fmod(m*w, N);
	if(u1<0)	u1+=N;
	u2=fmod(r*w, N);
	if(u2<0)	u2+=N;
	
	// u1*G, u2*Q 계산
	if(DEBUG==1){
		// 모든 좌표 계산
		calculPKey(N, g);
		calculPKey(N, q);
	}
	else{
		// kG 계산
		calculPKey(u1, g);
		calculPKey(u2, q);
	}

	x1=fmod(g[u1-1].x+q[u2-1].x, N);
	if(x1<0)	x1+=N;
	x2=fmod(g[u1-1].y+q[u2-1].y, N);
	if(x2<0)	x2+=N;
	
	if(x1==0 && x2==0){
		printf("[error] \n");
	}
	else{
		v=fmod(x1, N);
		if(v<0)	v+=N;
		
		if(DEBUG==1){
			printf("\n========================\n");
			printf("   [verification]\n");	
			printf("w = %d\n", w);
			printf("u1 = %d\n", u1);
			printf("u2 = %d\n", u2);
			printf("v = %d\n", v);
			printf("\n========================\n");
		}
		
		if(v==r){
			printf("\n올바른 서명입니다.\n");
		}
		else{
			printf("\n올바른 서명이 아닙니다.\n");
		} 
	}
}

void intro(){
	printf("=================================\n");
	printf("            ECC 검증\n");
	printf("         p=17, a=2, b=2\n");
	printf("            G=(5, 1)\n");
	printf("=================================\n\n");
}

int main(){
	G q[20];	
	G g[20];
	int r, s;
	int m;	// 메시지

	if(DEBUG==1){
		printf("=====ECDSA 검증(디버깅 모드)=====\n");
	}
	else{
		intro();
	}
	
	// 1G
	g[0].x=5;
	g[0].y=1;
	
	// 공개키 입력
	printf("공개키의 x좌표를 입력하세요.: ");
	scanf("%d", &q[0].x);
	printf("공개키의 y좌표를 입력하세요.: ");
	scanf("%d", &q[0].y);
	
	// r 입력
	while(1){
		printf("r을 입력하세요.(0<r<20): ");
		scanf("%d", &r);
		
		if(0<r && r<20)
			break;
	}
	
	// s 입력
	while(1){
		printf("s을 입력하세요.(0<s<20): ");
		scanf("%d", &s);
		
		if(0<s && s<20)
			break;
	}
	
	printf("메세지를 입력하세요.: ");
	scanf("%d", &m);
	
	if(DEBUG==1){
		printf("\n========================\n");
		printf("   [main] 입출력값 \n");	
		printf("Q = (%d, %d)\n", q[0].x, q[0].y);
		printf("m = %d\n", m);
		printf("r = %d\n", r);
		printf("s = %d\n", s);
		printf("\n========================\n");
	}
	
	// 서명 검증
	verification(m, r, s, q, g);
		
	return 0;
}