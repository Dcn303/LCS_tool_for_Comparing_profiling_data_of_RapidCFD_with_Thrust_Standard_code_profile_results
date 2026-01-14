//*****************************dynamic_single_string.c**********************//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "dynamic_single_string.h"
int add_char(struct dynamic_single_string *dynamic_string,char charecter_to_add)
{
	if(dynamic_string->string==NULL)
	{
		dynamic_string->string=(char*)malloc(sizeof(char)+2);
		dynamic_string->string[0]=charecter_to_add;
		dynamic_string->string[1]='\0';
		dynamic_string->total_string_char+=1;
		
	}
	else
	{
		int i;
		int last_element_index=dynamic_string->total_string_char;
		int total_element=dynamic_string->total_string_char;
		char *temp;
		 // Need space for: existing chars + new char + null terminator
		temp=(char*)realloc(dynamic_string->string,sizeof(char)*(total_element+2));
		dynamic_string->string=temp;
		dynamic_string->string[last_element_index]=charecter_to_add;
		dynamic_string->string[last_element_index+1]='\0';
		dynamic_string->total_string_char=total_element+1;
	}
	return 0;
}
void initialization_dynamic_string(struct dynamic_single_string *dynamic_string)
{
	dynamic_string->string=NULL;
	dynamic_string->total_string_char=0;
}

/*int main()
{

struct dynamic_single_string string;
initialization(&string);
add_char(&string,'A');
add_char(&string,'B');
printf("%s\n",string.string);
	return 0;
}
*/
