#define N 4     // Number of processes
#define M 7     // Domain size

#define sum(a,b) ((a+b)%N)
#define subtract(a,b) ((a-b+N)%N)
#define inc(a)   sum(a,1)
#define dec(a) ((a+N-1)%N)

#define sum1(a,b) ((a+b)%M)
#define subtract1(a,b) ((a-b+M)%M)
#define inc1(a)   sum1(a,1)
#define dec_m(a) ((a+M-1)%M)

#define localInv(i)  ((x[i] == x[dec(i)]) || (x[dec(i)] == inc1(x[i])))
#define globalInv    (localInv(0) && localInv(1) && localInv(2) && localInv(3))

#define noFaults (_nr_pr == N)

ltl convergence{ <>[] globalInv}

int x[N];
		
active [N] proctype P(){
 
  int i = _pid;

do 	 
      :: atomic{(x[dec(i)] == 0) && (x[dec(i)] != x[i]) && (x[dec(i)] != inc1(x[i])) && (x[i] != 0) -> x[i] = 0}
        :: atomic{(x[dec(i)] == 1) && (x[dec(i)] != x[i]) && (x[dec(i)] != inc1(x[i])) && (x[i] != 0) -> x[i] = 0}
        :: atomic{(x[dec(i)] == 2) && (x[dec(i)] != x[i]) && (x[dec(i)] != inc1(x[i])) && (x[i] != 1) -> x[i] = 1}
        :: atomic{(x[dec(i)] >= 3) && (x[dec(i)] != x[i]) && (x[dec(i)] != inc1(x[i])) && (x[i] != subtract1(x[dec(i)], 1) ) -> x[i] = subtract1(x[dec(i)], 1)}
od; 
}

active proctype faults(){
   
    if
       :: x[0] = 0;
       :: x[0] = 1;
       :: x[0] = 2;
       :: x[0] = 3;
       :: x[0] = 4;
       :: x[0] = 5;
       :: x[0] = 6;
    fi;
    if
       :: x[1] = 0;
       :: x[1] = 1;
       :: x[1] = 2;
       :: x[1] = 3;
       :: x[1] = 4;
       :: x[1] = 5;
       :: x[1] = 6;
    fi;
    if
       :: x[2] = 0;
       :: x[2] = 1;
       :: x[2] = 2;
       :: x[2] = 3;
       :: x[2] = 4;
       :: x[2] = 5;
       :: x[2] = 6;
    fi;
    if
       :: x[3] = 0;
       :: x[3] = 1;
       :: x[3] = 2;
       :: x[3] = 3;
       :: x[3] = 4;
       :: x[3] = 5;
       :: x[3] = 6;
    fi;
	
}
