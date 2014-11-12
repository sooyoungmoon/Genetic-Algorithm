// GA Assignment #1
// 2009711993 
// Soo Young Moon

// Main Code

#include "Parameters.h"
#include <stdio.h>
#include <stdlib.h>

void create_population(char [Population_Size][Num_Items], char temp_population[Population_Size][Num_Items]); // create initial population
void evaluate(int w[Num_Items], int p[Num_Items], int fitness[Num_Items], char population[Population_Size][Num_Items]); // evaluate a candidate solution
void select(char population[Population_Size][Num_Items], char temp_population[Population_Size][Num_Items], int fitness[Population_Size]); // select individuals for the next generation
void crossover(char population[Population_Size][Num_Items]); // crossover
void mutate(char population[Population_Size][Num_Items]); // mutate



void main()
{

	char population[Population_Size][Num_Items]; // current population
	char temp_population[Population_Size][Num_Items]; // temporary population
	
	int w[Num_Items]; // weight
	int p[Num_Items]; // profit
	int fitness[Population_Size]; // fitness value	

	// initialize w[], p[], and fitness[]
	{
		char line[1024];

		int temp = 0;

		FILE * dataFile = fopen("TestData(0-1Knapsack)[1].txt", "rt"); // open data file

		if(dataFile ==NULL)
			printf("cannot find the file");
		else
		{			
			int i = 0;

			for(i = 0; i < 5; i++) // skip 5 lines
				fgets(line, 1024, dataFile);

			for(i = 0; i < Num_Items; i++)			
			{

				w[i] = 0;
				p[i] = 0;
				fitness[i] = 0;				

				fscanf(dataFile, "%d", &temp);
				//printf("%d ", temp);

				fscanf(dataFile, "%d", &w[i]); // read a weight value
				//printf("%d ", w[i]);

				fscanf(dataFile, "%d", &p[i]); // read a profit value
				//printf("%d ", p[i]);
				
				//printf("[%d] weight: %d, profit: %d\n", i, w[i], p[i]);				
			}

			
			
			//printf("%d", c);

			fclose(dataFile);

		}// initialize w[], p[], and fitness[]		

		
	}

	// create initial population
	create_population(population, temp_population);


	int t = 0;

	while(t < Num_Generation) // Convergence test
	{
		printf("\nGeneration %d:", t+1);

		evaluate(w, p, fitness, population); // evaluate the current population

		select(population, temp_population, fitness); // select good individuals for the next generation
		
		crossover(temp_population); // mix the selected individuals

		mutate(temp_population); // mutate with low probability


		for(int i = 0; i < Population_Size; i++) // copy the elements of temporary array to the population array
		{										 // In other words, move to the next generation
			for(int j = 0; j < Num_Items; j++)
				population[i][j] = temp_population[i][j];
		}

		t++;
	}
	printf("\n");
	
} // main()

void create_population(char population[Population_Size][Num_Items], char temp_population[Population_Size][Num_Items]) // create initial population
{
	for(int i=0; i < Population_Size; i++)
	{
		//printf("\n individual %d:", i);

		for(int j = 0; j < Num_Items; j++)
		{
			population[i][j] = 0; // initialize each bit to zero
			temp_population[i][j] = 0;

			int r = rand();

			if(r%2==0) // assign zero or one with probability of 0.5 to each bit
				population[i][j] = 0;
			else
				population[i][j] = 1;

			// test code

			//printf("%d ", population[i][j]);
		}	
		
	}

}

void evaluate(int w[Num_Items], int p[Num_Items], int fitness[Population_Size], char population[Population_Size][Num_Items])// evaluate the current generation
{
	int fitness_value = 0;	

	int best_fitness = 0; // the best fitness value of the current 
	
	int sum_weight = 0;

	double avg_fitness= 0.0;

	for(int i = 0; i < Population_Size; i++)
	{
		sum_weight = 0; // initialize weight and fitness_value
		fitness_value = 0;

		for(int j = 0; j < Num_Items; j++)
		{
			int weight = w[j] * population[i][j]; 
			sum_weight += weight;				// accumulate weight
			int profit = p[j] * population[i][j]; // accumulate fitness value
			fitness_value += profit;		
		}		

		if (sum_weight > Weight_Capacity) // constraint check
			fitness[i] = 0; // sum of weights exceeds the capacity
		else
			fitness[i] = fitness_value;		

		avg_fitness += fitness[i];

		if(fitness[i] > best_fitness)
			best_fitness = fitness[i]; // update the best fitness value of the current generation

		//printf("\n Individual %d: %d", i, fitness[i]); 		
	} // for each individual

	avg_fitness = avg_fitness/Population_Size; // compute the average fitness value of the current generation

	printf("\t average fitness: %f", avg_fitness);
	printf("\t best fitness: %d", best_fitness); 

	// evaluate fitness value	
}

void select(char population[Population_Size][Num_Items], char temp_population[Population_Size][Num_Items], int fitness[Population_Size])
{
	int candidate = 0;

	for(int i = 0; i < Population_Size; i++)
	{
		candidate = rand()%Population_Size;

		if(fitness[i] < fitness[candidate])
		{
			for(int j = 0; j < Num_Items; j++)
				temp_population[i][j] = population[candidate][j];

			//printf("\n selected[%d]: %d", i, fitness[candidate]);
		}
		else
		{
			for(int j = 0; j < Num_Items; j++)
				temp_population[i][j] = population[i][j];

			//printf("\n selected[%d]: %d", i, fitness[i]);
		}
		
	}
}

void crossover(char population[Population_Size][Num_Items])
{
	for(int i=0; i< Population_Size/2; i++)
	{
		for(int j = 0; j < Num_Items; j++)
		{
			int r = rand()%2;
			
			int temp = 0;			

			if(r<1) // uniform crossover
			{
				temp = population[i][j];

				population[i][j] = population[2*i][j];

				population[2*i][j] = temp;
			}

		}
	}
}

void mutate(char population[Population_Size][Num_Items])
{
	for(int i = 0; i < Population_Size; i++)
	{
		for(int j = 0; j < Num_Items; j++)
		{
			int r = rand()%100;

			if(r < 1) // mutate with probability of 0.01
			{
				population[i][j]++; // convert one to zero, or zero to one
				population[i][j] = population[i][j]%2;
			}
		}
	}
}