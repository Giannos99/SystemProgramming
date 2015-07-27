#include<stdio.h>
#include <string.h>
#include <stdlib.h>
#include "head.h"

#include <math.h>

int main(int argc, char *argv[]){

	int i,j,fan_out=2,height=4,page=6,level;
	unsigned int message1_digest[4];
	node *root;
	FILE * fp1;
	size_t read;
	node **twod;
	char* buffer;
	
	int pagesize=16;
	size_t buffsize;
	int iPage=0;
	int tree_fan_out;
	MD5_CTX context;
	
	
	buffer = malloc (pagesize*sizeof(char));
	buffsize=pagesize;

	twod = malloc( (height) * sizeof( node * ) );
	for( i = 0; i < height; i++ ){
	
	    	twod[i] = malloc( ( exp( log(fan_out) * i )) * sizeof(  node ) );
	}

	
	
	fp1 = fopen(argv[1], "rb"); //open the file1 for binary input
	
	if( fp1 == NULL ) {
		
		perror("Could not open file1");
		return 1;
	}
	
	do {
		
		read = fread(buffer, 1, pagesize, fp1); //issue the read call
		
		if ( read > 0 ) {
		
			//if return value is 0
				if ( read < buffsize ) { //if fewer bytes than requested were returned...
			//fill the remainder of the buffer with zeroes
					memset(buffer + read, 0, buffsize - read);
				}
			printf("read page %u with size %u\n", iPage+1, read);
			iPage++;

			//Ipologismos tis sinopsis (digest) tou minimatos 1
			MD5Init( &context );
			MD5Update( &context , buffer , strlen( buffer) );
			MD5Final( message1_digest , &context );
			/*Ektiposi tis sinopsis tou minimatos 1

			printf("MD5(%s)=",buffer);
			MD5Print( message1_digest );
			printf("\n");*/
			//twod[height-1][iPage-1]=message1_digest;
			
			
			memcpy((twod[height-1][iPage-1].value),message1_digest,4*sizeof(int));
			printf("\n");
			printf("page=%d",iPage);
			MD5Print( twod[height-1][iPage-1].value );
			printf("\n");
			//root=insert_tree(root,message1_digest,tree_fan_out,iPage);
				
		}
	} while (read == buffsize ); //end when a read returned fewer items
	
	if ( ferror(fp1) ){
		perror("An error occured during reading fp1");
		fclose(fp1); //close the file
		return 0;
	}
	
	memset(buffer, 0, buffsize );
	
	MD5Init( &context );
	MD5Update( &context , buffer , strlen( buffer) );
	MD5Final( message1_digest , &context );
	
	while(iPage-1< exp(log(fan_out) * (height-1))){
		//twod[height-1][iPage-1]=message1_digest;
		memcpy((twod[height-1][iPage-1].value),message1_digest,4*sizeof(int));
		iPage++;
	}
	
	level=height-2;
	
	while(level>=0){
	
		for(i=0;i<exp(log(fan_out) * (level));i++){
		
			for(j=(fan_out*i);j<fan_out*(i+1);j++){
			
			MD5Init( &context );
			
			MD5Update( &context , (char *)twod[level+1][j].value ,  sizeof(message1_digest) );
			if(j+1<fan_out*(i+1)){
			
				MD5Update( &context ,(char *)twod[level+1][j+1].value,  sizeof( message1_digest ) );
			}
			
			MD5Final( message1_digest , &context );
			
			}
			
			memcpy((twod[level][i].value),message1_digest,4*sizeof(int));
			//twod[level][i]=message1_digest;
		}
		level--;
	}
	
	printf("To twod[0][0]= ");
	MD5Print( twod[0][0].value);
	printf("\n");
	getchar();

	
	for( i = 0; i < height ; i++ )
	    free( twod[i] );
	free( twod );
	
}
