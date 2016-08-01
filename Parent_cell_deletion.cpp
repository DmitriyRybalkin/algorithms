#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "conio.h"
#include <math.h>
#include <algorithm>
#include <vector>
#include <string>
#include <stack>
#include <queue>
#include <map>
#include <set>
#include <iterator>
#include <iostream>
#include <functional>
#include <sstream>
#include <numeric>

#define MAXCELLS 100

using namespace std;

class CellRemoval
{

public:

	int cellsLeft(vector <int> parent, int deletedCell)
	{
		bool isParent[MAXCELLS];
	
		for(int i=0 ; i<MAXCELLS; i++) isParent[i] = false;
	
		for(int i=0 ; i<parent.size() ; i++){
			for(int j=0 ; j<parent.size() ; j++){
				if(i==parent[j]){
					isParent[i] = true;
					break;
				}
			}
		}
		
		int tmp=0,count=0;
		for(int i=0 ; i<parent.size() ; i++){
			if(isParent[i]) continue;
		
			tmp = i;
			if(tmp==deletedCell) continue;

			while(1){
				if(parent[tmp]==deletedCell) break;
				if(parent[tmp]==-1){
					count++;
					break;
				}

				tmp = parent[tmp];
			}
		}

		return count;

	}

	//Test code is taken from http://acm.osu.edu/ProblemStatement.php?id=481
	void __test()
	{

		static bool s_FirstTime = true; 
		
		if (s_FirstTime) 
		{ 
			s_FirstTime = false; 
			run_test(-1); 
		}
	}

	
public:
	void run_test(int Case) 
	{ 
		if ((Case == -1) || (Case == 0)) test_0(); 
		if ((Case == -1) || (Case == 1)) test_1(); 
		if ((Case == -1) || (Case == 2)) test_2(); 
		if ((Case == -1) || (Case == 3)) test_3(); 
		if ((Case == -1) || (Case == 4)) test_4();  
	}
	
private:
	
	void verify(int Case, const int &Expected, const int &Received) 
	{ 
		cerr << "Test Case #" << Case << "..."; 
		
		if (Expected == Received) cerr << "PASSED" << endl;
		else 
		{ 
			cerr << "FAILED" << endl; cerr << "\tExpected: \"" << Expected << '\"' << endl; cerr << "\tReceived: \"" << Received << '\"' << endl; 
		} 
	}
	
	void test_0() 
	{ 
		int Arr0[] = {-1,0,0,1,1}; 

		vector <int> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); 
		
		int Arg1 = 2; 
		int Arg2 = 2; 
		
		verify(0, Arg2, cellsLeft(Arg0, Arg1)); 
	}
	
	void test_1() 
	{ 
		int Arr0[] = {-1,0,0,1,1}; 

		vector <int> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); 
		
		int Arg1 = 1; 
		int Arg2 = 1; 
		
		verify(1, Arg2, cellsLeft(Arg0, Arg1)); 
	}
	
	void test_2() 
	{ 
		int Arr0[] = {-1,0,0,1,1}; 

		vector <int> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); 
		
		int Arg1 = 0; 
		int Arg2 = 0; 
		
		verify(2, Arg2, cellsLeft(Arg0, Arg1)); 
	}
	
	void test_3() 
	{ 
		int Arr0[] = {-1,0,0,2,2,4,4,6,6}; 

		vector <int> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); 
		
		int Arg1 = 4; 
		int Arg2 = 2; 
		
		verify(3, Arg2, cellsLeft(Arg0, Arg1)); 
	}
	
	void test_4() 
	{ 
		int Arr0[] = {26,2,32,36,40,19,43,24,30,13,21,14,24,21,19,4,30,10,44,12,7,32,17,43,
						35,18,7,36,10,16,5,38,35,4,13,-1,16,26,1,12,2,5,18,40,1,17,38,44,14}; 

		vector <int> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); 
		
		int Arg1 = 24; 
		int Arg2 = 14; 
		
		verify(4, Arg2, cellsLeft(Arg0, Arg1)); 
	}

};

	int main()
	{
		CellRemoval cell;
		cell.__test();
		_getch();

		return 0;
	}