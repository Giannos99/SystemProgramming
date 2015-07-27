#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "head.h"


node * allocate_hash_tree(node * root,int fan_out,int height){
	int i=0,j=0;
	
	for(i=0;i<height-1;i++){
		node * newnode;
		newnode=malloc(sizeof(node));
		if(i==0){
			
			newnode->next=NULL;
			newnode->child=NULL;
			newnode->isRoot=0;
			if(height==0){
				newnode->isRoot=2;/*smainei oti einai fulo*/
				
			}
			newnode->noc=0;
			root->noc=fan_out;
			newnode->child=NULL;
			root->child=newnode;
			for(j=1;j<fan_out;j++){
				node * newnd;
				newnd=malloc(sizeof(node));
				newnd->child=NULL;
				newnd->PageNo=0;
				newnd->noc=0;
				newnd->isRoot=0;
				if(height==0){
				newnd->isRoot=2;/*smainei oti einai fulo*/
				}
				if(j+1!=fan_out){
				newnode->next=newnd;
				newnode=newnd;
				}
				else{
				newnd->next=NULL;
				newnode=newnd;
				}
				
				
			}
		}
		else{
			
			
			
		}
	]
}
