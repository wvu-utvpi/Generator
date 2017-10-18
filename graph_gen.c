#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <values.h>


struct grey_cycle {
	int len;
	int *ind;
};

void shuffle(int *arr, size_t n)
{
    if (n > 1) 
    {
        size_t i;
		int t;
        srand(time(NULL));
        for (i = 0; i < n - 1; i++) 
        {
          	size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
          	t = arr[j];
			
          	arr[j] = arr[i];			
          	arr[i] = t;
        }
    }
}


void shuffle_2D(int **arr, size_t n)
{
    if (n > 1) 
    {
        size_t i;
		int tt, tw;
        srand(time(NULL));
        for (i = 0; i < n - 1; i++) 
        {
          	size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
          	tt = arr[j][0];
			tw = arr[j][1];
			
          	arr[j][0] = arr[i][0];
			arr[j][1] = arr[i][1];
				
          	arr[i][0] = tt;
			arr[i][1] = tw;
        }
    }
}

//Index to Pair - convert the array index to a source and sink vertex in the CNS
void I2P(int n, int *index, int len, int *x, int *y)
{
	int i = 1;
	*x = 0;
	while((i < len) && (n >= index[i])){
		*x = i;
		i++;
	}
	*y = i + (n - index[*x]);
	
	//Increment from array index location to variable name
	*x += 1;
	*y += 1;
}

int P2I(int x, int y, int *index)
{
	int t, res;
	x--;  //Convert from variable name to index location
	y--;
	
	if(y < x){
		t = x;
		x = y;
		y = t;
		
		res = -1 * ((index[x] + (y - x))-1);
		res--;
	}
	else{
		res = index[x] + (y - x)-1;
	}
	
	return  res; //Return edge index, start from 0
}

int Edge_Type(int pet, int oet)
{
	int res;
	//DEBUG: printf("PET: %d OET: %d\n",pet, oet);
	if(oet == 0){
		srand(time(NULL));
		res = rand() % 2;	//Coin flip
		if(pet == 0){
			return ((rand() % 4) + 1);
			}
		else if((pet == 1) || (pet == 3)){
			if(res == 0)
				return 2;
			return 3;
		}
		else if((pet == 2) || (pet == 4)){
			if(res == 0)
				return 1;
			return 4;
		}
		else{
			printf("Error:  Unexpected previous edge type.\n");
		}
	}
	else{
		switch (pet){
			case 1:
				if ((oet == 1) || (oet == 4)){
					return 2;
				}
				else{
					return 3;
				}
			case 2:
				if ((oet == 1) || (oet == 4)){
					return 4;
				}
				else{
					return 1;
				}
				break;
			case 3:
				if ((oet == 1) || (oet == 4)){
					return 2;
				}
				else{
					return 3;
				}
				break;
			case 4:
				if ((oet == 1) || (oet == 4)){
					return 4;
				}
				else{
					return 1;
				}
				break;
			default :
				printf("Error:  Unexpected previous edge type.\n");
		}
	}
}

//gcc -O4 -DNDEBUG -DWALL -DXRUN -o utvpi_gen graph_gen.c
int main (int argc, char *argv[]){
	int n, m, cc, cl;
	
	//Set default number of cycles and cycle length
	cc = 0;
	cl = 0;
	
	//Define parameters for edge weights
	int pos_min = 5000;
	int pos_max = 20000;
	int neg_min = 1;
	int neg_max = 10;
	
	/*********************************************************************************************************
	**
	** Extract parameters
	**
	***********************************************************************************************************/
	if((argc < 3) || (argc > 5)){
		goto usage;
	} 
	else{
		//DEBUG fprintf(stderr, "Parameters argc: %d, n: %s, m: %s\n\n", argc, argv[1], argv[2]);
		if ( (n = atoi (argv[1])) < 2) goto usage;
		if ( (m = atoi (argv[2])) < 1) goto usage;
		if ( m > (2*n*(n-1))){
			printf("Too many edges, fewer than 4*(n*(n-1)/2) required\n");
			goto cleanup;
		}
		if ( argc >= 4)
			cc = atoi(argv[3]);
		if ( argc == 5)
			cl = atoi(argv[4]);
	}
	//DEBUG fprintf(stderr, "Parameters n: %d, m: %d, cc: %d, cl: %d\n\n", n, m, cc, cl); 
	
	/*********************************************************************************************************
	**
	**  Build Arrays for storing Constraint Network Structure
	**
	*********************************************************************************************************/
	
	//Build an n*(n-1)/2 array to hold connections
	int *BlackEdges = (int *) calloc( (n*(n-1))/2, sizeof(int ));
	int *WhiteEdges = (int *) calloc( (n*(n-1))/2, sizeof(int ));
	int *PGEdges = (int *) calloc( (n*(n-1))/2, sizeof(int ));
	int *NGEdges = (int *) calloc( (n*(n-1))/2, sizeof(int ));
	
	//Build a size n-1 array of indices
	int *index = (int *) calloc( (n-1), sizeof(int) );
	int *cyc_node = (int *) calloc( (n), sizeof(int) );
	
	//Build an array to store the generated negative grey cycles
	struct grey_cycle *cycles = malloc(cc * sizeof(struct grey_cycle));
		

	/*********************************************************************************************************
	**
	** Populate the CNS with random normal edges/constraints
	**
	***********************************************************************************************************/
	
	//Indices for edges of each type
	int start, type;
	int tcnt[4]; //[0] = black, [1] = white, [2] = pgray, [3] = ngray
	tcnt[0] = 0;
	tcnt[1] = 0;
	tcnt[2] = 0;
	tcnt[3] = 0;
	
	srand(time(NULL));
	for (int i = 0; i < m; i++){
		//Roll to determine edge type
		type = 1 + rand()%4; // 1 = Black, 2 = White, 3+4 = Gray, 0 = No edge
		//Safeguard against one type being overused
		start = type;
		while( (tcnt[type-1]) >= (n*(n-1)/2) ){
			type = (type % 4) + 1; //Rotate through range 1...4
			if(type == start){
				printf("Error - Attempted to add too many edges.\n");
				goto cleanup;
			}
		}
		
		if(type == 1){
			//Insert an edge at the corresponding index
			BlackEdges[(tcnt[type-1])] = (pos_min + (rand() % pos_max));
			//Increment the corresponding index
			tcnt[type-1] += 1;
		}
		else if (type == 2){
			WhiteEdges[(tcnt[type-1])] = (pos_min + (rand() % pos_max));
			(tcnt[type-1]) += 1;
		}
		else if (type == 3){
			PGEdges[(tcnt[type-1])] = (pos_min + (rand() % pos_max));
			(tcnt[type-1]) += 1;
		}
		else if (type == 4){
			NGEdges[(tcnt[type-1])] = (pos_min + (rand() % pos_max));
			(tcnt[type-1]) += 1;
		}
		else{
			printf("Error: Unexpected edge type generated\n");
			goto cleanup;
		}
	}
	
	//Build index to hash between edge pairs and 1-D array locations
	int ind = 0;
	int inc = (n-1);
	
	//DEBUG printf("Indices: ");
	for (int i = 0; i < (n-1); i++){
		index[i] = ind;
		cyc_node[i] = i + 1;
			
		ind += inc;
		inc -= 1;
		//DEBUG printf(" %d ", index[i]);
	}
	cyc_node[n-1] = n;
	//DEBUG printf(".\n\n");
			
	//Shuffle the connections
	shuffle(BlackEdges, (n*(n-1))/2);
	shuffle(WhiteEdges, (n*(n-1))/2);
	shuffle(PGEdges, (n*(n-1))/2);
	shuffle(NGEdges, (n*(n-1))/2);
	
	/*********************************************************************************************************
	**
	** Create and insert negative gray cycle(s) as specified
	**
	***********************************************************************************************************/
	//Create an array of all 'n' variables and shuffle it
	shuffle(cyc_node, n);
	
	//Initialize/Create 2-D Array that stores the generated cycles
	int cl_inc = 0;
	int cl_amnt = (cl/10)+1;
	for(int i = 0; i < cc; i++){
		//Make only one shortest cycle - the first one added
		if((cl_inc == 0) && i > 0){
			cl += cl_amnt;
			cl_inc++;
		}
		else if ((cl_inc == 1) && i > (cc/2)){
			cl += cl_amnt;
			cl_inc++;
		}
		
		(cycles[i]).len = cl;
		(cycles[i]).ind = (int *) calloc (cl, sizeof(int));
	}
	
	//Generate cycles and store them in array
	int nc = 0; //Next node to join a cycle
	for(int i = 0; i < cc; i++){
		for(int j = 0; j < (cycles[i]).len; j++){
			if(nc >= n){
				printf("Error:  Not enough variables (%d) to generate the specified cycles (%d).\n\n", n, (cycles[i]).len);
				goto cleanup;
			}
			
			//Add the next variable in the random ordering
			((cycles[i]).ind)[j] = cyc_node[nc];
			nc++;
		}
	}

	
	//Insert generated cycle(s) into the CNS Matrix
	//Initialize tracking variables
	//Cycle Weight (sum)
	int SumW;
	
	//Pairs, Edges, and Edge Types
	int initP, curP, prevP, edge, pet, cet, oet;
	
	//Point to matrix containing edges of the chosen type
	int *grey;
	
	//DEBUG: printf("Cycle Edge: %d\n", pet);
	for(int i = 0; i < cc; i++){
		//Track cycle weight for each cycle
		SumW = 0;
		
		//Extract Pairs
		initP = prevP = (cycles[i]).ind[0];
		pet = cet = 0;
		for(int j = 1; j < (cycles[i]).len; j++){
			curP = (cycles[i]).ind[j];
			
			//Convert Pair to Matrix Index
			edge = P2I(prevP, curP, index);
			//DEBUG: I2P(edge, index, (n-1), &cX, &cY);
			//DEBUG: printf("\n\nX%d and X%d @ edge %d\n",prevP,curP, edge);
			//DEBUG: printf("Verified edge %d same as X%d and X%d \n\n",edge, cX,cY);
			
			//Determine Type
			cet = Edge_Type(pet, 0);
			//DEBUG: printf("Cycle Edge: %d\n", cet);
			
			switch (cet) {
				case (1) :
					grey = BlackEdges;
					break;
				case (2) :
					grey = WhiteEdges;
					break;
				case (3) :
					if(edge < 0){			//If edges is negative then the pair order was flipped, adjust grey edge accordingly
						//cet = 4;			Treat as reversed but don't actually change!  Algorithm will actually be traversing in opposite order.
						grey = NGEdges;
					}
					else
						grey = PGEdges;
					break;
				case (4) :
					if(edge < 0){
						//cet = 3;
						grey = PGEdges;
					}
					else
						grey = NGEdges;
					break;
				default :
					printf("Error: Unexpected type generated for negative grey cycle\n");
					goto cleanup;
			}
			if(initP == prevP)
				oet = cet;
			
			//Take the absolute value of edges
			if(edge < 0)
				edge = -1 * (edge + 1);
			
			//DEBUG: printf("Placing grey cycle at Edge %d\n", edge);
			//Check for existing connection
			if(grey[edge] == 0)
			{
				//No?
				m++;											//Adding additional edge
				grey[edge] = (neg_min + (rand() % neg_max));  	//Set Edge Weight
				SumW += grey[edge];								// Update cycle weight
			}
			else
			{
				//Yes? - Modify, edge count remains same
				grey[edge] = (neg_min + (rand() % neg_max));  	//Set Edge Weight
				SumW += grey[edge];								//Update Cycle weight
			}
			
			//Step to the next pair
			prevP = curP;
			pet = cet;
		}
		//Connect the last variable back to the initial
		curP = initP;
		edge = P2I(prevP, curP, index);
		cet = Edge_Type(pet, oet);
		
		//DEBUG: printf("\n\nX%d and X%d @ edge %d\n",prevP,curP, edge);
		//DEBUG: printf("Cycle Edge: %d\n", cet);
		
		switch (cet) {
			case (1) :
				grey = BlackEdges;
				break;
			case (2) :
				grey = WhiteEdges;
				break;
			case (3) :
				if(edge < 0){			//If edges is negative then the pair order was flipped, adjust grey edge accordingly
					cet = 4;
					grey = NGEdges;
				}
				else
					grey = PGEdges;
				break;
			case (4) :
				if(edge < 0){
					cet = 3;
					grey = PGEdges;
				}
				else
					grey = NGEdges;
				break;
			default :
				printf("Error: Unexpected type generated for negative grey cycle\n");
				goto cleanup;
		}
			
		//Take the absolute value of edges
		if(edge < 0)
			edge = -1 * (edge + 1);
		
		//DEBUG: printf("Placing grey cycle at Edge %d\n", edge);
		//Check for existing connection
		if(grey[edge] == 0)
		{
			//No?
			m++;											//Adding additional edge
			grey[edge] = -1 - SumW;							//Make sum = -1							
		}
		else
		{
			//Yes? - Modify, edge count remains same
			grey[edge] = -1 - SumW;							//Make sum = -1
		}
	}
	
	/*********************************************************************************************************
	**
	** Output the generated graph
	**
	***********************************************************************************************************/
	//Print information on the CNS Matrix as comments
	printf("// Contraint Network containing %d grey cycles with the shortest of length %d.\n", cc, cl);
	printf("// \tParameters N: %d and M: %d\n",n, m);
	for(int i = 0; i < cc; i++){
		printf("\t//Cycle %d:", i);
		for(int j = 0; j < (cycles[i].len); j++){
			printf(" X%d", (cycles[i].ind)[j]);
		}
		printf("\n");
	}
	
	//Print the first line of the CNS file - variable count
	printf("%d variables\n",n);
	
	//Iterate and print the constraint formulae
	int x, y;
	for(int i = 0; i < ((n*(n-1))/2); i++)
	{
		if(BlackEdges[i] != 0){
			I2P( i, index, (n-1), &x, &y);
			printf("-X%d - X%d <= %d\n",x,y,BlackEdges[i]);
		}
		if(WhiteEdges[i] != 0){
			I2P( i, index, (n-1), &x, &y);
			printf("X%d + X%d <= %d\n",x,y,WhiteEdges[i]);
		}
		if(PGEdges[i] != 0){
			I2P( i, index, (n-1), &x, &y);
			printf("X%d - X%d <= %d\n",x,y,PGEdges[i]);
		}
		if(NGEdges[i] != 0){
			I2P( i, index, (n-1), &x, &y);
			printf("-X%d + X%d <= %d\n",x,y,NGEdges[i]);
		}
	}
	
	/*************************************************************************************************************
	**
	**	Clean Up
	**
	*************************************************************************************************************/
	cleanup:
	//Free Connection Matrices
	free(BlackEdges);
	free(WhiteEdges);
	free(NGEdges);
	free(PGEdges);
	
	//index hash & potential cycle node list
	free(index);
	free(cyc_node);
	
	//Free matrix of generated gray cycles
	for(int i = 0; i < cc; i++){
		free( (cycles[i].ind) );
	}
	free(cycles);
	
	return 1;
		
	usage:
		fprintf(stderr, "\nusage: utvpi_gen <# Variables> <# Edges> <# cycles - def 0> <cycle len - def 0>\n");
	return 0;
}
