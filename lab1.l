%{
#include<stdio.h>
#undef yywrap
#define yywrap() 1
%}

%%
[a-z A-Z _]([a-z A-Z 0-9 _])* {

printf("valid");

}

.* {

printf("invalid");
}
%%
int main()
{
printf("enter an input string to check valid identifier is or not: ");
yylex();
return 0;

}


