/*****************************************************
* chomp is an interesting game, there is a demo at :
* http://www.math.ucla.edu/~tom/Games/chomp.html
* I wrote a 8 * 3 demo in cpp, enjoy it!
*****************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <set>

using namespace std;

#define FLUSH while (getchar() != '\n');

int min(int a, int b) {
  return a < b ? a : b;
}


int curState[3] = {8, 8 ,8};

set<int *> AS;
set<int *> PAS;
set<int *> NAS;

// All possible state
void AllState() {
  for (size_t i = 1; i <= 8; i++) {
    for (size_t j = 0; j <= i; j++) {
      for (size_t k = 0; k <= j; k++) {
        int *a = (int *)malloc( 3 * sizeof(int));
        a[0] = i, a[1] = j, a[2] = k;
        AS.insert(a);
      }
    }
  }
}

// Whether we can move from "from state" to "to state"
bool canMove(int *from, int *to) {
  for (size_t i = 0; i < 3; i++) {
    if (from[i] < to[i]) return false;
    else if (from[i] == to[i]) {
      continue;
    } else {
      for (size_t j = i + 1; j < 3; j++) {
        if (min(from[j], to[i]) != to[j]) return false;
      }
      return true;
    }
  }
  return false;
}

/*************************************
  Calculate the N-state and P-state
  1. The end state is P-state.
  2. All the state that can transform to P-state is N-State.
  3. All the state that can only transform to N-State is P-State.
*************************************/ 
void _calNPAS() {
  for (set<int *>::iterator from = AS.begin(); from != AS.end(); from++) {
    for (set<int *>::iterator to = PAS.begin(); to != PAS.end(); to++) {
      if (canMove(*from, *to)) {
        NAS.insert(*from);
      }
    }
  }

  for (set<int *>::iterator from = AS.begin(); from != AS.end(); from++) {
    bool isP = true;
    for (set<int *>::iterator to = AS.begin(); to != AS.end(); to++) {
      if (canMove(*from, *to)) {
        if (NAS.find(*to) == NAS.end()) {
          isP = false;
        }
      }
    }
    if (isP) PAS.insert(*from);
  }
}

// Get N-state and P-state
void calNPAS() {
  bool end = true;
  while (true) {
    end = true;
    for (set<int *>::iterator it = AS.begin(); it != AS.end(); it++) {
      if (NAS.find(*it) == NAS.end() && PAS.find(*it) == PAS.end()) {
        end = false;
        _calNPAS();
      }
    }
//    printf("NAS: %d, PAS: %d\n", (int)NAS.size(), (int)PAS.size());
    if (end) return;
  }
}

// Printf current state
void printChomp(int *state) {
  printf("\n==============================\n");
  for (size_t i = 0; i < 3; i++) {
    printf("%d", 3 - (int)i);
    for (size_t j = 0; j < 8; j++) {
      printf(" ");
      if (j < state[2 - i]) printf("o");
      else printf("*");
    }
    printf("\n");
  }
  printf(" ");
  for (size_t i = 0; i < 8; i++)
    printf(" %d", (int)i + 1);
  printf("\n");
  printf("==============================\n");
}

// Get current state after took the (column, row) position.
void getCurrentState(int row, int column) {
  for (size_t i = 0; i < 3; i ++) {
    if (i + 1 >= row) curState[i] = min(column - 1, curState[i]);
  }
}

// Whether we win?
bool CheckWin() {
  for (size_t i = 0; i < 3; i++) {
    if (curState[0] == 1 && curState[1] == 0 && curState[2] == 0) {
      printChomp(curState);
      printf("You Loose! Try Again?(Y or N) :");
      FLUSH;
      while (true) {
        char ans = getchar();
        if (ans == 'Y' || ans == 'y') return true;
        else if (ans == 'N' || ans == 'n') exit(0);
        else {
          FLUSH;
          printf("You Loose! Try Again?(Y or N) :");
        }
      }
    }
  }
  return false;
}

// Whether we loose?
bool CheckLoose() {
  for (size_t i = 0; i < 3; i++) {
    if (curState[0] == 1 && curState[1] == 0 && curState[2] == 0) {
      printf("You Win! Try Again?(Y or N) :");
      FLUSH;
      while (true) {
        char ans = getchar();
        if (ans == 'Y' || ans == 'y') return true;
        else if (ans == 'N' || ans == 'n') exit(0);
        else {
          FLUSH;
          printf("You Win! Try Again?(Y or N) :");
        }
      }
    }
  }
  return false;
}

// If we can not move to a p-state, just take a random move.
void randomMove() {
  if (curState[2] > 0) curState[2] -= rand() % curState[2] + 1;
  else if (curState[1] > 0) curState[1] -= rand() % curState[1] + 1;
  else if (curState[0] > 0) curState[0] -= rand() % curState[0] + 1;
}

int main() {
  AllState();
  // Init the P-State
  PAS.insert(*(AS.begin()));
  calNPAS();
  printf("Input your move: (a b), a is column number, b is row number, press q to quit\n");
  while (true) {
    int row = 0, column = 0;
    printChomp(curState);

    printf("Your Move: ");
	// Get the user input
    while (true) {
      if (scanf("%d %d", &column, &row) != 2) {
        if (getchar() == 'q') return 0;
        FLUSH;
      }
      if(row == 0 || column == 0 || curState[row - 1] < column || row * column == 1)
        printf("Please Input Valid Row and Column number:");
      else {
        break;
      }
    }
    
    getCurrentState(row, column);
    if (CheckLoose()) {
      curState[0] = 8, curState[1] = 8, curState[2] = 8;
      continue;
    }
    bool canWin = false;

	// If we can move the state to P-State, then we can win!
    for (set<int *>::iterator it = PAS.begin(); it != PAS.end(); it++) {
      if (canMove(curState, *it)) {
        canWin = true;
        for (size_t i = 0; i < 3; i++)
          curState[0] = (*it)[0];
          curState[1] = (*it)[1];
          curState[2] = (*it)[2];
      }
    }
    
    if (canWin == false) randomMove();

    if (CheckWin()) {
      curState[0] = 8, curState[1] = 8, curState[2] = 8;
      continue;
    }
  }
  return 0;
}
