# ECDSA toy example
## 국민대학교 정보보안암호수학과 20195198 노준영

### ecc_signature.c : ECDSA 서명
### ecc_verification.c : ECDSA 검증 

<br>

### 타원곡선 정보
$y^2=x^3+2x+2$  
$p=17$  
$G=(5, 1)$
$n=19$

<br>

### 유의사항
1. $H(m)$ 대신 $m$을 사용하였음.
2. 공개키의 유효성 검증 생략
3. 역수 계산을 페르마 소정리의 의존함