using namespace std;
#include <cstdlib>     //NULL
#include <stdio.h>    
#include <stdlib.h>     // srand, rand 
#include <time.h>       // time 
#include <algorithm>    // std::shuffle
#include <array>       //
#include <iostream>    //
#include <random>		// // std::default_random_engine
#include <windows.h >
#include <unordered_map>
#include <string>
#include <cstring>
#include <tuple>
#include <vector>
#define V 9

//this multidimensional array stores the distance matrix , fully converged  for easier coding

int matrix[][V] = { {0,2,11,3,18,14,20,12,5},
					{2,0,13,10,5,3,8,20,17},
					{11,13,0,5,19,21,2,5,8},
					{3,10,5,0,6,4,12,15,1},
					{18,5,19,6,0,12,6,9,7},
					{14,3,21,4,12,0,19,7,4},
					{20,8,2,12,6,19,0,21,13},
					{12,20,5,15,9,7,21,0,6},
					{5,17,8,1,7,4,13,6,0},
};


struct route
{
	int path[V]; // a 9 digit array that gets passed to indiv function to create a unique path
	int ID = 0; // a method of identifying the route 
	int fitness = 0; // the fitness assigned by fitness function 
};

void indiv(int arr[]) // individualizes a route passed to function by filling array and then shuffling it 
{

	//populate array with the numbers 0-8
	for (int i = 0; i < V; i++)
	{
		//arr[i] = i;
		arr[i] = i + 1;

	}

	random_shuffle(arr, arr + V);
	//arr[V-1] = arr[0]; // copying the initial element into the last element gives simulates a round trip per the hw assignment example

}

int fitnessfn(int array[])// calculates route fitness 
{

	int fitness = 0;
	for (int i = 0; i < V-1 ; i++)
	{
		 //cout << matrix[array[i]-1][array[i+1]-1]<<endl;
		fitness += (matrix[array[i]-1][array[i + 1]-1]);

	}

	return fitness;
}

int crossoverhelper(route parA, route parB, int a) //used by reproduce fn 
{
	int n = 0;
	for (int i = 0; i < V; i++) // a loop 
	{
		if (parA.path[i] == a) // if the value in first bit in parB is found in parA
		{
			n = i; // save the index of where this value was found
			return parB.path[n]; // return the value at this index from parB.
		}
	}


}

int IDgenerator() // small helper function for ID'ing routes
{

	int ID = rand() % 9000 + 1000;
	return ID;

}


void initPop(int n, vector <route>& pool) // function creates an initial population of size n
{
	for (int i = 0; i < n; i++)
	{
		route temp; // creating a new route
		indiv(temp.path); // populating the path of temp with a unique individual path
		temp.ID = IDgenerator();
		temp.fitness = fitnessfn(temp.path);
		pool.push_back(temp); // push temp into array

	   
	    cout << "array " << temp.ID << "	"<< "fitness " << temp.fitness << endl;
		for (int j = 0; j < V; j++)
		{
			cout << pool[i].path[j];
		}
		cout << endl;
	   
			

	   


	}
}

bool compareByFitness(route& a, route& b) // helper function for poolSort
{
	return(a.fitness < b.fitness);
}

void poolsort( vector <route>& pool) // sorts by fitness using comparebyfitness fn
{
	sort(pool.begin(), pool.end(), compareByFitness);

}

void arrayHandler(int chA[] , int par[])
{
	int temp[V] = {0};

	int i = 0;
		int	j = 0; 
		int k = 0;

	do {
		bool exists = false;

		

		for (j = 0; j < V; j++) // iterating through child
		{
			if (par[i] == chA[j]) // if the current parent index = child index 
			{
				exists = true;
				break;
			}

		}

		if (exists == false) { temp[k] = par[i]; k++; }

		i++;
	} while (i < V);

	for (int x = 0; x < V; x++) {
		par[x] = temp[x];
	}
	


}
// the documentation/psuedo for this fn was provided in the course material | link: https://www.hindawi.com/journals/cin/2017/7430125/ (used CX :case 1)
void reproduce(route& childA, route& childB, route parentA, route parentB) // function will identify parents by ID 
{

	route parA = parentA;
	route parB = parentB;

	
	/*

	*/
	childA.path[0] = parB.path[0]; //the first index in first child contains the value that was in the first index in the second parent
	int a = parB.path[0]; // we save that value here in int a 

	 // counter var
	for (int i = 0; i < V; i++) // while the current index of second child DNE the first value in the first parent  // CASE 1
	{
		//int firstCycle contains the a value 
		int firstCycle = crossoverhelper(parA, parB, a); //looked for whatever a is inside the first parent , found its index , then returned whatever value was at that index, in the SECOND parent
		int secondCycle = crossoverhelper(parA, parB, firstCycle); // value that was found in second parent passed to fn again, we look for this value in first parent, and then return the value at that index in the SECOND parent    

		childB.path[i] = secondCycle; // after two calls, we have a value we can add to the second Child

		if (childB.path[i] != parA.path[0])
		{

		int forChA = crossoverhelper(parA, parB, secondCycle); // call crossover one more time   
		childA.path[i + 1] = forChA; // now we have a value for first child. 
		a = forChA;

		}
		else
		{
			arrayHandler(childA.path, parB.path);
			arrayHandler(childA.path, parA.path);
			childA.path[i+1] = parB.path[0];
			 a = parB.path[0];
		}
		
	}


	 


}




void currentPop(vector<route>& pool, vector<route>& currPool, int g) //uses reproduction to create a new population
{
	for (int i = 0; i < g; i++)
	{
		route childA, childB, parA, parB; // the offspring

		random_device rd;
		mt19937 eng(rd());
		uniform_int_distribution<> distr(0, ((pool.size() - 1) / 2));


		parA = pool[distr(eng)];
		parB = pool[distr(eng)];

		reproduce(childA, childB, parA, parB);

		childA.fitness = fitnessfn(childA.path);
		childB.fitness = fitnessfn(childB.path);

		currPool.push_back(childA);
		currPool.push_back(childB);
	}


}


int main()
{
	srand(time(0)); // using computer clock to control seed. This seed gets used by random_shuffle function in parentGen Function



	/*
	int child[V] = { 7,2,3,5,0,0,0,0,0 };
	int parent[V] = { 2,8,5,7,6,0,1,4,3 };

	arrayHandler(child, parent);
	*/
	
	
	
		vector <route> InitRoutePool; // Initial population of routes
	vector <route> currPool;
	initPop(10, InitRoutePool);
	poolsort( InitRoutePool);
	
	

//	currentPop(InitRoutePool,currPool ,6);
//	poolsort(currPool);

	
	route childA, childB, parA, parB;


	parA = InitRoutePool[2];
	parB = InitRoutePool[0];

	reproduce(childA, childB, parA, parB);
	
	
	

	

	/*
		for (route i : InitRoutePool)
	{
		cout << "array " << i.ID << "	" << "fitness " << i.fitness << endl;
		for (int j = 0; j < V; j++)
		{
			cout << i.path[j];
		}
		cout << endl;
	}

	for (route i : currPool)
	{
		cout << "array " << i.ID << "	" << "fitness " << i.fitness << endl;
		for (int j = 0; j < V; j++)
		{
			cout << i.path[j];
		}
		cout << endl;
	}
	*/






	/*
	vector <route> InitRoutePool; // Initial population of routes
	initPop(20, InitRoutePool);



	cout<<endl<<endl<<"Pool after Poolsort"<<endl;

	for (route i : InitRoutePool)
	{
		cout << "array " << i.ID << "	" << "fitness " << i.fitness << endl;
		for (int j = 0; j < V; j++)
		{
			cout<< i.path[j];
		}
		cout << endl;
	}

	*/







	cin.get();
	cin.ignore();
}