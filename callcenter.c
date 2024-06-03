#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"
#include "stack.h"

/*
 * Definition: This structure is the basis of a call that will reside within the Call Center. 
 * When the user creates a call, it will create a CallInfo struct and they will fill in information for the member variables.
 *
 * Params:
 *   callID - Number of the call 
 * 	 caller_name - Caller Name
 *   caller_reason - Caller reason
 */
struct CallInfo {
	int callID; 
	char caller_name[25];
	char caller_reason[75];
};

/*
 * Definition: This structure is the "manager". It is created to keep track of the queue and stack that are used as call center functions.
 * This struct is where the queue and stack for answered/pending calls are stored. 
 *
 * Params:
 *   stack - Once a call is answered, it is pushed onto this stack. 
 *   queue - Once a call is made, it's enqueued ontoo this queue. 
 *   num_pending_calls - Keeps track of the number of calls on the queue
 *   num_answered_calls - Keeps track of the number of calls on the stack
 */
struct CallCenter {
	struct stack* stack;
	struct queue* queue; 
	int num_pending_calls;
	int num_answered_calls;
};

/*
 * Definition: This function prints out the menu for the user to choose from. 
 *
 * Params:
 *  none
 * 
 * Return: 
 *   This function returns the menu choice the user chooses (1-5)
 */
int print_menu(){
	int menu_choice = -1;
	printf("---Welcome Back To Work---\n");
	printf("1. Recieve a new call\n");
	printf("2. Answer a call\n");
	printf("3. Current state of the stack - answered calls\n");
	printf("4. Current state of the queue - calls to be answered\n");
	printf("5. Quit\n");
	printf("Your Choice: ");
	scanf("%d", &menu_choice);
	printf("\n");

	return menu_choice;
}

/*
 * Definition: This function will create a CallInfo struct and ask the user to fill in the information about it's member variables. 
    		   It will then be enqueued to the queue to ismulate creating/recieving a call. 
 *
 * Params: 
 *   cc - Call Center struct
 * 
 * Return: none
 *   
 */
void recieve_call(struct CallCenter* cc){
	struct CallInfo* call = malloc(sizeof(struct CallInfo)); //Create a call

	printf("Enter Caller's Name: ");
	scanf("%s", call->caller_name);

	printf("Enter Call Reason: ");
	scanf("%s", call->caller_reason);
	printf("\n");

	// Increment number of calls in call center & set ID number of current call
	cc->num_pending_calls += 1;
	call->callID = cc->num_pending_calls; 

	queue_enqueue(cc->queue, call); // push call to queue
	return;
}

/*
 * Definition: This function enqueues a call from the queue, and pushes it onto the stack simulating answering a call and putting it in the stack. 
 *
 * Params:
 * 	cc - Call Center struct
 * 
 * Return: none
 *   
 */
void answer_call(struct CallCenter* cc){

	void* answered_call = queue_front(cc->queue); // return top of queue call
	queue_dequeue(cc->queue); // pop call from queue
	stack_push(cc->stack, answered_call); // push call to stack

	// decrement number of calls in queue and increment number in stack
	cc->num_pending_calls--;
	cc->num_answered_calls++;

	struct CallInfo* call = (struct CallInfo*)stack_top(cc->stack);

	printf("The following call has been answered and added to the stack!\n");
	printf("\n");		
	printf("==========================\n");
	printf("Call ID: %d\n", call->callID);
	printf("Caller's Name: %s\n", call->caller_name);
	printf("Call Reason: %s\n", call->caller_reason);
	printf("==========================\n");
	printf("\n");

	return;
}

/*
 * Definition: This function shows the number of calls in the stack. It also displays the details of the most recently answered call (top of the stack).
 *
 * Params:
 *  cc - Call Center struct
 * 
 * Return: none
 *   
 */
void show_answered_calls(struct CallCenter* cc){

	struct CallInfo* call = (struct CallInfo*)stack_top(cc->stack); //return most recent call fron the stack

	printf("Number of calls answered: %d\n", cc->num_answered_calls);
	printf("Details of the last call answered \n");
	printf("\n");
	printf("==========================\n");
	printf("Call ID: %d\n", call->callID);
	printf("Caller's Name: %s\n", call->caller_name);
	printf("Call Reason: %s\n", call->caller_reason);
	printf("==========================\n");
	return;
}

/*
 * Definition: This function shows the user the number of calls that are pending/in the queue. 
 * 			   It shows the most recent one in the queue/ the next one to be answered. 
 *
 * Params:
 *   cc - Call Center struct
 * 
 * Return: none
 *   
 */
void show_pending_calls(struct CallCenter* cc){
	printf("Number of calls to be answered: %d\n", cc->num_pending_calls);

	if(cc->num_pending_calls == 0){
		return;
	}
	struct CallInfo* call = (struct CallInfo*)queue_front(cc->queue);
	printf("Details of the first call to be answered\n");
	printf("\n");
	printf("==========================\n");
	printf("Call ID: %d\n", call->callID);
	printf("Caller's Name: %s\n", call->caller_name);
	printf("Call Reason: %s\n", call->caller_reason);
	printf("==========================\n");
	printf("\n");
	return;
}

/*
 * Definition: This function frees all allocated memory on the stack, queue, and the call center
 *
 * Params:
 *   cc - CallCenter struct 
 * 
 * Return: 
 * 	none 
 *   
 */
void free_mem(struct CallCenter* cc){

	// return calls and free them until there are none left on the queue
    while (!queue_isempty(cc->queue)) {
        struct CallInfo* call = (struct CallInfo*)queue_dequeue(cc->queue); 
        free(call);
    }

	// return calls and free them until there are none left on the stack
    while (!stack_isempty(cc->stack)) {
        struct CallInfo* call = (struct CallInfo*)stack_pop(cc->stack); 
        free(call);
    }

    queue_free(cc->queue); // free queue
    stack_free(cc->stack); // free stack
    free(cc); // free Call Center 
}


int main(int argc, char *argv[]) {

	struct CallCenter* call_center = malloc(sizeof(struct CallCenter));
	call_center->stack = stack_create();
	call_center->queue = queue_create();
	call_center->num_pending_calls = 0;
	call_center->num_answered_calls = 0;

	int menu_choice = -1;

	do {
		menu_choice = print_menu();

		if(menu_choice == 1){
			recieve_call(call_center);
		}
		else if(menu_choice == 2){
			answer_call(call_center);
		}
		else if(menu_choice == 3){
			show_answered_calls(call_center);
		}
		else if(menu_choice == 4){
			show_pending_calls(call_center);
		}
		else if(menu_choice == 5){
			
		}

	}while(menu_choice != 5);

	free_mem(call_center);

	printf("Have a good day!\n");



	return 0;
}
