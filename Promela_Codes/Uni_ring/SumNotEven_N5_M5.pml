#define N 5     // Number of processes
#define M 5     // Domain size

#define sum(a,b) ((a+b)%N)
#define subtract(a,b) ((a-b+N)%N)
#define inc(a)   sum(a,1)
#define dec(a) ((a+N-1)%N)

#define sum1(a,b) ((a+b)%M)
#define subtract1(a,b) ((a-b+M)%M)
#define inc1(a)   sum1(a,1)
#define dec1(a) ((a+M-1)%M)

#define sumNotE(a, b) ((a+b) % 2) 

#define localInv(i)  (sumNotE(x[dec(i)], x[i]) == 0)
#define globalInv    (localInv(0) && localInv(1) && localInv(2) && localInv(3) && localInv(4))

#define noFaults (_nr_pr == N)

ltl convergence{ <>[] globalInv}

int x[N];
		
active [N] proctype P(){
 
  int i = _pid;
do 	 
        :: atomic{(x[dec(i)] == 0) && (sumNotE(x[dec(i)], x[i]) != 0) && (x[i] != 1) -> x[i] = 1}
        :: atomic{(x[dec(i)] == 1) && (sumNotE(x[dec(i)], x[i]) != 0) && (x[i] != 1) -> x[i] = 1}
        :: atomic{(x[dec(i)] == 2) && (sumNotE(x[dec(i)], x[i]) != 0) && (x[i] != 1) -> x[i] = 0}
        :: atomic{(x[dec(i)] >= 3) &&  (sumNotE(x[dec(i)], x[i]) != 0) && (x[i] != subtract1( x[dec(i)], 2 )) -> x[i] = subtract1(x[dec(i)], 2 ) }
od; 
}

active proctype faults(){
   
    if
       :: x[0] = 0;
       :: x[0] = 1;
       :: x[0] = 2;
       :: x[0] = 3;
       :: x[0] = 4;
    fi;


    if
       :: x[1] = 0;
       :: x[1] = 1;
       :: x[1] = 2;
       :: x[1] = 3;
       :: x[1] = 4;
    fi;


    if
       :: x[2] = 0;
       :: x[2] = 1;
       :: x[2] = 2;
       :: x[2] = 3;
       :: x[2] = 4;
    fi;


    if
       :: x[3] = 0;
       :: x[3] = 1;
       :: x[3] = 2;
       :: x[3] = 3;
       :: x[3] = 4;
    fi;


    if
       :: x[4] = 0;
       :: x[4] = 1;
       :: x[4] = 2;
       :: x[4] = 3;
       :: x[4] = 4;
    fi;	
}
