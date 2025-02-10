%{
    #include<stdio.h>
    #undef yywrap
    #define yywrap() 1
    int s_c = 0, ch_c = 0;
%}

%%
[ ]+ { s_c++; }
. { ch_c++; }
%%

int main() {
    printf("Enter the input string: \n");
    yylex();

    printf("\nNumber of spaces = %d", s_c);
    printf("\nNumber of characters = %d\n", ch_c);

    return 0;
}
