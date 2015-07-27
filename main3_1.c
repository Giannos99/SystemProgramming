#include<stdio.h>
#include <string.h>
#include <stdlib.h>
#include "head.h"
#include "MD5.h"

int main(void){

	int i,fan_out=-1,height=-1;
	node *root;
	
	root=malloc(sizeof(node));
	root->isRoot=1;
	root->next=NULL;
	root->child=NULL;
	root->PageNo=0;
	root->noc=0;
	fan_out=3,height=3;
	root=allocate_hash_tree(root,fan_out,height);
	
}
