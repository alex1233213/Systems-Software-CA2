#include <stdio.h>



int main() { 

	FILE *fr = fopen("/home/alex/Desktop/ca2/server_upload_files/test.tml", "w");

	if(fr == NULL)  {
		printf("err");
	} else { 
		printf("Not null");
	}


}
