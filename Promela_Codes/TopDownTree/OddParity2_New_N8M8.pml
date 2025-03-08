#define N 8  
#define M 8

#define Parent(i)	((i-1)/2)		
#define sum(a,b) ((a+b)%M)

//#define diff(a,b) ((a-b+M)%M)

#define diff(a,b) ((a-b) % 2 ==0)


#define OddParity2(a,b) ((a%2!=0)&&(diff(a,b)))
#define NOddParity2(a,b) (!OddParity2(a,b))
#define localInv(i) (OddParity2(x[Parent(i)],x[i]))
#define globalInv (localInv(1)&&localInv(2)&&localInv(3)&&localInv(4)&&localInv(5)&&localInv(6)&&localInv(7))
#define isEven(x) ((x % 2)==0)
#define noFaults (faults@fin)

ltl convergence { [] (noFaults -> <>[] globalInv) }
//ltl closure {[] ((noFaults && globalInv) -> []globalInv)}

int x[N] = {2, 4, 6, 1, 0, 6, 3, 5};

active [N] proctype P() {
 int i = _pid; 
   do 	 
      :: atomic{ (i == 0) && isEven(x[i]) -> x[i] = sum(x[i], 1) }
      :: atomic{ (i>0) && (x[Parent(i)] == 0)  && (NOddParity2(x[Parent(i)], x[i])) && (x[i] != 1) -> x[i] = 1 }
      :: atomic{ (i>0) && (x[Parent(i)] == 1)   && (NOddParity2(x[Parent(i)], x[i])) && (x[i] != 3) -> x[i] = 3 }
      :: atomic{ (i>0) && (x[Parent(i)] == 2)  && (NOddParity2(x[Parent(i)], x[i])) && (x[i] != 1) -> x[i] = 1 }

  :: atomic{ (i>0) && (x[Parent(i)] >= 3)  && (!isEven(x[Parent(i)]))  &&
   (NOddParity2(x[Parent(i)], x[i])) && (x[i] != 1) -> x[i] = 1 }
   od

/*   
      :: atomic{ (i>0) && (x[Parent(i)] >= 3)  && (!isEven(x[Parent(i)]))  && 
      (NOddParity2(x[Parent(i)], x[i])) && (x[i] != sum(x[Parent(i)], 2)) 
      -> x[i] = sum(x[Parent(i)], 2) } 

sum(x[Parent(i)], 2) may actually work incorrectly if adding 2 units goes beyond M. For example, if M=9 and x[Parent(i)] = 7, then adding 2 will give 0 instead of 9. 

  :: atomic{ (i>0) && (x[Parent(i)] >= 4)  && (!isEven(x[Parent(i)]))  && 
  (NOddParity2(x[Parent(i)], x[i])) && (x[i] != 1) -> x[i] = 1 }
 */
}
active proctype faults() {
   if
    :: x[1] = 0;
    :: x[1] = 1;
    :: x[1] = 2;
    :: x[1] = 3;
    :: x[1] = 4;
    :: x[1] = 5;
    :: x[1] = 6;
    :: x[1] = 7;
 //   :: x[1] = 8;
 //   :: x[1] = 9;
    fi;
    if
    :: x[2] = 0;
    :: x[2] = 1;
    :: x[2] = 2;
    :: x[2] = 3;
    :: x[2] = 4;
    :: x[2] = 5;
    :: x[2] = 6;
    :: x[2] = 7;
 //   :: x[2] = 8;
 //   :: x[2] = 9;
    fi;
    if
    :: x[3] = 0;
    :: x[3] = 1;
    :: x[3] = 2;
    :: x[3] = 3;
    :: x[3] = 4;
    :: x[3] = 5;
    :: x[3] = 6;
    :: x[3] = 7;
 //   :: x[3] = 8;
 //   :: x[3] = 9;
    fi;
    if
    :: x[4] = 0;
    :: x[4] = 1;
    :: x[4] = 2;
    :: x[4] = 3;
    :: x[4] = 4;
    :: x[4] = 5;
    :: x[4] = 6;
    :: x[4] = 7;
 //   :: x[4] = 8;
 //   :: x[4] = 9;
    fi;
    if
    :: x[5] = 0;
    :: x[5] = 1;
    :: x[5] = 2;
    :: x[5] = 3;
    :: x[5] = 4;
    :: x[5] = 5;
    :: x[5] = 6;
    :: x[5] = 7;
 //   :: x[5] = 8;
  //  :: x[5] = 9;
    fi;
    if
    :: x[6] = 0;
    :: x[6] = 1;
    :: x[6] = 2;
    :: x[6] = 3;
    :: x[6] = 4;
    :: x[6] = 5;
    :: x[6] = 6;
    :: x[6] = 7;
  //  :: x[6] = 8;
   // :: x[6] = 9;
    fi;

    if
    :: x[7] = 0;
    :: x[7] = 1;
    :: x[7] = 2;
    :: x[7] = 3;
    :: x[7] = 4;
    :: x[7] = 5;
    :: x[7] = 6;
    :: x[7] = 7;
 //   :: x[7] = 8;
  //  :: x[7] = 9;
    fi;

/*

    if
    :: x[8] = 0;
    :: x[8] = 1;
    :: x[8] = 2;
    :: x[8] = 3;
    :: x[8] = 4;
    :: x[8] = 5;
    :: x[8] = 6;
    :: x[8] = 7;
    :: x[8] = 8;
    :: x[8] = 9;
    fi;
    if
    :: x[9] = 0;
    :: x[9] = 1;
    :: x[9] = 2;
    :: x[9] = 3;
    :: x[9] = 4;
    :: x[9] = 5;
    :: x[9] = 6;
    :: x[9] = 7;
    :: x[9] = 8;
    :: x[9] = 9;
    fi;
*/
 fin: skip
      }
