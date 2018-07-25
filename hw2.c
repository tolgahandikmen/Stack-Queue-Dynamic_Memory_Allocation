#include <stdio.h>
#include <stdlib.h>

/*	That structure forms the basis of the stack structure. 
	‘top’ variable keeps track of changes. Always default ‘top’ variable is ‘0’. 
	If any changes has been done, ‘top’ value is changed. 
	‘interruptName’ variable is holds to information about that stack’s interrupt.	*/	
typedef struct {
	char interruptName;
	int top;
} Stack;

/*	That structure forms the basis of the queue structure. 
	‘front’ variable keeps first element information of that queue and ‘rear’ variable keeps last element information of that queue. 
	Always default ‘front’ and ‘rear ’variables are ‘-1’. 
	‘process’ variable is holds to information about that queue’s process.	*/
typedef struct {
	char process;	
	int front;
	int rear;
} Queue;

/*	That structure forms the basis of the client structure. 
	Clients have one stack structure and one queue structure. */
typedef struct {
	Stack* stack;
	Queue* queue;
} Client;	

/*	It checks the all front values for each elements of queue.
	If front != 1, means queue is empty.
	If queue is empty, returns ‘1’, otherwise ‘0’	*/
int isEmptyQueue (Queue* queue, int queueSize) {
	int i;
	for (i=0; i<queueSize; i++){
		if(queue[i].front == 1){
			return 0;
		}
	}
	return 1;	
}

/*	It checks the front values for first element and last element of queue.
	If queue is full, returns ‘1’, otherwise ‘0’	*/
int isFullQueue (Queue* queue, int queueSize) {
	int i;
	if ((queue[0].front == 1) && (queue[queueSize-1].rear == 1))	return 1;
	for (i=0; i<queueSize; i++){
		if(queue[i].rear == 1)
			if(queue[i+1].front == 1) return 1;		
	}
	return 0;
}

/*	It adds process after to the last element (rear) of the queue.
	If element is empty, front and rear are '-1'
	If added element is front of the queue, front value is 1, otherwise 0.
	If added element is rear of the queue, front value is 1, otherwise 0.
	If operation is success, returns ‘1’, otherwise ‘0’	*/
int enQueue (Queue* queue, int queueSize, char process) {
	int i;
	if (!isFullQueue(queue, queueSize)) {
		/* adding to empty queue */
		if (isEmptyQueue(queue, queueSize)) {
			queue[0].front = 1;
			queue[0].rear = 1;
			queue[0].process = process;
			return 1;
		}
		
		/* adding to not empty queue */
		else {
			for (i=0; i<queueSize; i++) {
				if(queue[i].rear == 1){
					queue[i].rear = 0;
					i = (i+1) % queueSize;
					queue[i].front = 0;
					queue[i].rear = 1;
					queue[i].process = process;
					return 1;
				}
			}
		}
	}
	else {
		/*	adding does not success	*/
		return 0;	
	}
}

/*	It deletes first element of the queue (from front) and returns process information of deleted element. 
	If given queue is empty, the function returns ‘0’	*/
char deQueue (Queue* queue, int queueSize) {
	int i;
	char deleted;
	if (!isEmptyQueue(queue,queueSize)) {
		for(i=0; i<queueSize; i++) {
			if(queue[i].front == 1) {
				queue[i].front = -1;
				queue[i].rear = -1;
				deleted = queue[i].process;
				
				i = (i+1) % queueSize;
				if (queue[i].front == 0)	queue[i].front = 1;
				
				return deleted;
			}
		}
	}
	else {
		/*	"Error 3" queue is empty	*/
		return 0;
	}
}

/*	It checks top value of first element in the given stack.
	If top = 0, means stack is empty.
	If stack is empty, returns ‘1’, otherwise ‘0’	*/
int isEmpty (Stack* stack) {
	if (stack[0].top == 0) return 1;
	else return 0;
}

/*	It checks top value of last element in the given stack.
	If stack is full, returns ‘1’, otherwise ‘0’	*/
int isFull (Stack* stack, int stackSize) {
	if (stack[stackSize-1].top != 0) return 1;
	else return 0;
}

/*	It adds interrupt after to the last element of the stack.
	If operation is success, returns ‘1’, otherwise ‘0’	*/
int push (Stack* stack, int stackSize, char interruptName) {
	int i = 0;
	if (!isFull(stack, stackSize)) {
		for (i=0; i<stackSize; i++) {
			if (stack[i].top == 0) {
				break;
			}
		}		
		stack[i].top = 1;
		stack[i].interruptName = interruptName;
		return 1;
	}
	else {
		/*	Error 2"	*/
		return 0;
	}
}

/*	It deletes first element of the stack and returns interrupt information of deleted element. 
	If given stack is empty, the function returns ‘0’	*/
char pop (Stack* stack, int stackSize) {
	int i;
	if (!isEmpty(stack)){
		if(isFull(stack, stackSize)){
			stack[stackSize-1].top = 0;
			return stack[stackSize-1].interruptName;
		}		
		else{
			for (i=stackSize-1; i>=0; i--) {
				if (stack[i].top != 0){
					break;	
				}
			}
			stack[i].top = 0;
			return stack[i].interruptName; 			
		}
	}
	else{
		/*	empty stack	*/
		return 0;
	}
}

int readInputFile_toCreate (char *fileName, int* stackSizesArray,int* queueSizesArray);
int readOperationFile (char *fileName, int** operationArray);
void simulate (Client* client, int** operationArray, int op_size, int client_size, int* stackSizesArray,int* queueSizesArray, char** outcome);

/* Main Function */
int main (int argc, char **argv)
{
	int i,j;
    char *input = argv[1];
    char *input2 = argv[2];
    char *output = argv[3];

	/*	output file is opening	*/
	FILE *f = fopen(output, "w");
    
    int stackSizesArray[2000];
    int queueSizesArray[2000];
    int client_size = 0;
    int op_size = 0;

    
    /*	reserve the space of operation array for holding all operations in a two dimensional array	*/
    int ** operationArray = (int**) malloc (sizeof (int*) * 1000);
	for (i=0; i<1000; i++)
	{
		operationArray[i] = (int*) malloc (sizeof (int) * 3);
	}
    
    /*	reverse the space of all clients according to input file	*/
    client_size = readInputFile_toCreate (input, stackSizesArray, queueSizesArray);
    Client* client = (Client*) malloc(sizeof(Client)*(client_size));
    for (i=0; i<client_size; i++) {
    	client[i].stack = malloc(sizeof(Stack) * stackSizesArray[i]);
    	client[i].queue = malloc(sizeof(Queue) * queueSizesArray[i]);
    	for (j=0; j<stackSizesArray[i]; j++){
    		client[i].stack[j].top = 0;

		}
    	for (j=0; j<queueSizesArray[i]; j++){
    		client[i].queue[j].front = -1;
    		client[i].queue[j].rear = -1;
		}
	}

	/*	reading second input file and setting operation array */
	op_size = readOperationFile(input2, operationArray);
	char** outcome = (char**) malloc(sizeof(char*) * (client_size));
	for (i=0; i<client_size; i++) {
		outcome[i] = (char*) malloc(sizeof(char) * op_size);
	}
	
    for(i=0; i<client_size; i++){
    	for(j=0; j<op_size; j++){
    		outcome[i][j] = 0;
		}
	}
	
	/*	operates all operations according to operation array	*/
	simulate (client, operationArray, op_size, client_size, stackSizesArray, queueSizesArray, outcome);

    for(i=0; i<client_size; i++){
    	for(j=0; j<op_size; j++){
    		fprintf(f,"%c ",outcome[i][j]);
		}
		fprintf(f,"\n");
	}
	fclose(f);
		
	return 0;
}

/*	It separates the operationArray into usable parts. 
	This function invokes the functions needed according to the incoming operation and prints the results to a array. */
void simulate (Client* client, int** operationArray, int op_size, int client_size, int* stackSizesArray,int* queueSizesArray, char** outcome) {
	int i = 0;
	int j = 0;
	int k;
	int client_id;
	char information;
	int check_operation_flag;
	
	for (i=0; i<op_size; i++) {
		/*	parses operation array into operations	*/
		j = 0;

		/*	If operation is 'A' */
		if ((char)operationArray[i][j] == 'A') {
			client_id = (operationArray[i][j+1]-'0') - 1;
			information = (char)operationArray[i][j+2];
			
			/* adds process to queue of related client */
			check_operation_flag = enQueue (client[client_id].queue, queueSizesArray[client_id], information);
			if (!check_operation_flag) {
				for (k=0; k<op_size; k++) {
					if (outcome[client_id][k] == 0)	break;	
				}
				outcome[client_id][k] = '1';
			}
		}

		/*	If operation is 'I' */		
		if ((char)operationArray[i][j] == 'I') {
			client_id = (operationArray[i][j+1]-'0') - 1;
			information = (char)operationArray[i][j+2];
			
			/* adds interrupt to stack of related client */
			check_operation_flag = push(client[client_id].stack, stackSizesArray[client_id], information);
			if (!check_operation_flag) {
				for (k=0; k<op_size; k++) {
					if (outcome[client_id][k] == 0) break;
				}
				outcome[client_id][k] = '2';	
			}
		}	
			
		/*	If operation is 'S' */		
		if ((char)operationArray[i][j] == 'S') {
			client_id = (operationArray[i][j+1]-'0') - 1;
			/* checks to client's stack is empty or not because it need to delete an operation then add to server's queue */
			if (isEmpty(client[client_id].stack)) {
				if (isEmptyQueue(client[client_id].queue, queueSizesArray[client_id])) {
					/* client's queue is not empty also. It need to return ERROR 3 */
					for(k=0; k<op_size; k++) {
						if (outcome[client_id][k] == 0) break;
					}
					outcome[client_id][k] = '3';
				}

				/* client's queue is not empty. It will delete an operation from queue then add to server's queue */
				else {
					information = deQueue (client[client_id].queue, queueSizesArray[client_id]);
					for(k=0; k<op_size; k++) {
						if (outcome[client_id][k] == 0) break;
					}
					outcome[client_id][k] = information;
					check_operation_flag = enQueue (client[client_size-1].queue, queueSizesArray[client_size-1], information);	
					if (!check_operation_flag) {
						for (k=0; k<op_size; k++) {
							if (outcome[client_size-1][k] == 0) break;
						}
						outcome[client_size-1][k] = '1';	
														
					}			
				}
			}

			/* client's stack is not empty. It will delete an operation from stack then add to server's queue */
			else {
				information = pop (client[client_id].stack, stackSizesArray[client_id]);
				for(k=0; k<op_size; k++) {
					if (outcome[client_id][k] == 0) break;
				}
				outcome[client_id][k] = information;
				check_operation_flag = enQueue (client[client_size-1].queue, queueSizesArray[client_size-1], information);
				if (!check_operation_flag) {
					for (k=0; k<op_size; k++) {
						if (outcome[client_size-1][k] == 0) break;
					}
					outcome[client_size-1][k] = '1';				
				}			
			}
		}
				
		/*	If operation is 'O' */		
		if ((char)operationArray[i][j] == 'O') {			
			/* checks to server's stack is empty or not because it need to return an operation */
			if (isEmpty(client[client_size-1].stack)) {
				/* server's stack is empty. It need to return an operation from queue */

				/* checks to server's queue is empty or not because it need to return an operation */
				if (isEmptyQueue(client[client_size-1].queue, queueSizesArray[client_size-1])) {
					/* server's queue is not empty also. It need to return ERROR 3 */
					for(k=0; k<op_size; k++) {
						if (outcome[client_size-1][k] == 0) break;
					}
					outcome[client_size-1][k] = '3';
				}
	
				/* server's queue is not empty. It will return an operation from queue */
				else {
					information = deQueue(client[client_size-1].queue, queueSizesArray[client_size-1]);
					for(k=0; k<op_size; k++) {
						if (outcome[client_size-1][k] == 0) break;
					}
					outcome[client_size-1][k] = information;
				}
			}
			/* server's stack is not empty. It will return an operation from stack */
			else {
				information = pop (client[client_size-1].stack, stackSizesArray[client_size-1]);
				for(k=0; k<op_size; k++) {
					if (outcome[client_size-1][k] == 0) break;
				}
				outcome[client_size-1][k] = information;
			}
		}		
	}
}

/*	Returns operation_size after reading the input file. 
	While function is running, it fills the operationArray up. */
int readOperationFile (char *fileName, int** operationArray){
	int opSize, chr;
	int i=0;
	int j=0;
	FILE *input_file;
    input_file = fopen(fileName, "r");
	
	
    if (input_file == 0)
    {
        /* fopen returns 0, the NULL pointer, on failure */
        printf("Can not open input file\n");
        return -1;
		perror("Canot open input file\n");
        //exit(-1); 
    }
    else 
    {
    	/* setting client size according to first row of txt file */
        fscanf(input_file,"%d ", &opSize);    
		while ((chr = fgetc(input_file)) != EOF )
        {       
        	if( isdigit(chr) || isalpha(chr)){        		
        		operationArray[i][j] = chr;
				j++;				
			}
			else if (chr == '\n') {
					i++;
					j = 0;
			}		 		
        }
	}
	fclose(input_file);
	return opSize;
} 

/*	Returns client_size after reading the input file. 
	While function is running, it fills the stackSizesArray and the queueSizesArray up.	*/
int readInputFile_toCreate (char *fileName, int* stackSizesArray,int* queueSizesArray)
{
    int client_size = 0;
    int i = 0;
	FILE *input_file;
	int stackSize;
	int queueSize;
	
    input_file = fopen(fileName, "r");

	
    if (input_file == 0)
    {
        /* fopen returns 0, the NULL pointer, on failure */
        printf("Can not open input file\n");
        return -1;
		perror("Canot open input file\n");
        //exit(-1);
    }
    else 
    {
    	/* setting client size according to first row of txt file */
        fscanf(input_file,"%d ", &client_size);
        
        while ( fscanf (input_file, "%d %d",&queueSize, &stackSize) != EOF) {
        	stackSizesArray[i] = stackSize;
        	queueSizesArray[i] = queueSize;
        	i++;
		}
    }	
	fclose(input_file);	
	return client_size;
}
