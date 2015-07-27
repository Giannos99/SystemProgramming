//http://www.thegeekstuff.com/2012/08/c-linked-list-example/http://www.thegeekstuff.com/2012/08/c-linked-list-example/http://www.thegeekstuff.com/2012/08/c-linked-list-example/
//http://beej.us/guide/bgnet/

#include "MD5.h"

#define MAX_DATA_FOR_BUF 256

typedef struct node node;


struct node {
    	unsigned int value[4];
	
};

struct client_message {

	int fd;//file descriptor tou client p esteile to mhnuma
	int len;//mege8os mhnumatos
	char message[MAX_DATA_FOR_BUF];//to mhnuma
	
};

struct lista
{
    struct client_message client_message;
    struct lista *next;
};

struct lista *head = NULL;
struct lista *curr = NULL;

struct lista* create_list(struct client_message client_message)
{
    printf("mphke sto create list\n");
    struct lista *ptr = (struct lista*)malloc(sizeof(struct lista));
    if(NULL == ptr)
    {
        printf("\n Node creation failed \n");
        return NULL;
    }
    ptr->client_message = client_message;
    ptr->next = NULL;

    head = curr = ptr;
    return ptr;
}

struct lista* add_to_list(struct client_message client_message, int add_to_end)/*1 kanei add sto telos, 0 kanei add sthn arxh */
{
    if(NULL == head)
    {
        return (create_list(client_message));
    }

    if(add_to_end)
        printf("\n Adding node to end of list with value [%d]\n",client_message.fd);
    else
        printf("\n Adding node to beginning of list with value [%d]\n",client_message.fd);

    struct lista *ptr = (struct lista*)malloc(sizeof(struct lista));
    if(NULL == ptr)
    {
        printf("\n Node creation failed \n");
        return NULL;
    }
    ptr->client_message = client_message;
    ptr->next = NULL;

    if(add_to_end)
    {
    	struct lista *temp=head;
    	while(temp->next != NULL)
    	 {
            temp = temp->next;   
   	 }
        temp->next = ptr;
        curr = ptr;
    }
    else
    {
        ptr->next = head;
        head = ptr;
    }
    return ptr;
}

struct lista* search_in_list(int fd/*file_descriptor*/, struct lista **prev)
{
    struct lista *ptr = head;
    struct lista *tmp = NULL;
    int found = 0;
    struct client_message client_message;
    printf("\n Searching the list for value [%d] \n",client_message.fd);

    while(ptr != NULL)
    {
        if(ptr->client_message.fd == fd)
        {
            found = 1;
            break;
        }
        else
        {
            tmp = ptr;
            ptr = ptr->next;
        }
    }

    if(1 == found)
    {
        if(prev)
            *prev = tmp;
        return ptr;
    }
    else
    {
        return NULL;
    }
}

void pop_lista(void){/*diwxnei to prwto stoixeio*/
	struct lista *ptr = head;
	if(head!=NULL){
	
		head=head->next;
		free(ptr);
	}
}
	

int delete_from_list(int val)/*den to xrhsimopoiw auto*/
{
    struct lista *prev = NULL;
    struct lista *del = NULL;
    struct client_message client_message;
    
    printf("\n Deleting value [%d] from list\n",client_message.fd);

    del = search_in_list(client_message.fd,&prev);
    if(del == NULL)
    {
        return -1;
    }
    else
    {
        if(prev != NULL)
            prev->next = del->next;

        if(del == curr)
        {
            curr = prev;
        }
        else if(del == head)
        {
            head = del->next;
        }
    }

    free(del);
    del = NULL;

    return 0;
}

void print_list(void)
{
    struct lista *ptr = head;

    printf("\n -------Printing list Start------- \n");
    while(ptr != NULL)
    {
        printf("\n [%d] \n",ptr->client_message.fd);
        ptr = ptr->next;
    }
    printf("\n -------Printing list End------- \n");

    return;
}
