#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <values.h>

//gcc -O4 -DNDEBUG -DWALL -DXRUN -o u2y U2Y.c
int main (int argc, char *argv[]){
	char *str, *t1, *t2, *cns, *op;
	int lbytes = 128;
	int tbytes = 8;
	int succ, len, cur, num_var;
	
	str = (char *) malloc (lbytes+1);
	t1 = (char *) malloc (tbytes+1);
	t2 = (char *) malloc (tbytes+1);
	cns = (char *) malloc (tbytes+1);
	op = (char *) malloc (tbytes+1);
	
 	if(argc < 3){
		printf("Insufficient Arguments.  Please provide a number of nodes.\n");
		return 1;
	} 
	else{
		//printf("Filename: %s\n", argv[1]);
		FILE * ifp, *ofp;
		ifp = fopen(argv[1],"r");
		ofp = fopen(argv[2], "w");
		

	
		//Read out all the comment lines
		do{
			len = getline(&str, &lbytes, ifp);
		} while (str[0] == '/');
			
		//Parse out the number of variables and declare them
		succ = sscanf(str, "%d variables", &num_var);
			
		if(succ == 1){
			cur = 0;
			while(cur < num_var){
				fprintf(ofp, "(define X%d::real)\n", (cur+1));
				cur++;
			}
			fprintf(ofp, "\n\n\n");				
		}
		else{
			printf("Extraction Failed!\n");
		}
		
		//Parse out the equations - may be 1 or 2 terms and 0 or 1 operands
		fprintf(ofp, "(assert (and \n");
		cur = 0;
		while(!feof(ifp)){	
			
			//First scan
			succ = fscanf(ifp, "%s", str);
			
			//If its a term
			if(	((str[0]=='x') || (str[0]=='X')) || 
			   ((str[0] == '-') && ((str[1]=='x') || (str[1]=='X')))){
				
				if(cur == 0){
					strncpy(t1, str, 8);
					cur++;
				}
				else {
					strncpy(t2, str, 8);
					cur++;
				}
			}
			
			//If its an operand
			else if ((str[0]=='-') || (str[0]=='+')){
				strncpy(op, str, 8);
			}
			
			// If its an inequality - acquire final term and print
			else if (str[0]=='<'){
				//Ingore inequality op and get constraint constant
				succ = fscanf(ifp, "%s", cns);
				
				//Print the rest of the equation
				fprintf(ofp, "\t\t(<= ");
				if(cur == 1){	//Single term print
					if(t1[0] == '-'){	//Negative term
						strncpy(str,(t1+1),7);
						strncpy(t1,str,8);
						
						fprintf(ofp, "(* -1 %s) %s)\n",t1, cns);
					}
					else{
						fprintf(ofp, "%s %s)\n",t1, cns);
					}
				}
				else{	//Double term print
					fprintf(ofp, "(%s ",op);
					
					if(t1[0] == '-'){
						strncpy(str,(t1+1),7);
						strncpy(t1,str,8);
						
						fprintf(ofp, "(* -1 %s) ",t1);
					}
					else{
						fprintf(ofp, "%s ",t1);
					}
					
					
					if(t2[0] == '-'){
						strncpy(str,(t2+1),7);
						strncpy(t2,str,8);
						
						fprintf(ofp, "(* -1 %s)) ",t2);
					}
					else{
						fprintf(ofp, "%s) ",t2);
					}
					
					fprintf(ofp, "%s)\n",cns);
				}
				cur = 0;
			}
			//If its anything else - error
			else{
				printf("Unexpected input.  Extraction Failed!\n");
			}
		}
		
		fprintf(ofp, "\t\t)\n\t)\n\n(check)\n\n(show-model)");
	}
	
	return 0;
}