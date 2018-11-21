%{
// HEADERS
#include <stdlib.h>
#include "parser.h"

// variables maintained by the lexical analyser
int yyline = 1;
%}

%option noyywrap

%%
[ \t]+ {  }
#.*\n { yyline++; }
\n { yyline++; }

\-?[0-9]+ {
   yylval.intValue = atoi(yytext);
   return NUMBER;
}

"==" { return EQUALS; }
"!=" { return DIFF; }
"<"  { return LESS; }
">"  { return GREAT; }
"<=" { return LESSEQUAL; }
">=" { return GREATEQUAL; }

"&&" { return ANDLOGIC; }
"||" { return ORLOGIC; }

"+" { return PLUS; }
"-" { return MINUS; }
"*" { return MULT; }
"/" { return DIV; }
"%" { return MOD; }

"=" { return ATTR; }

"int" { return VARINT;}

";"  { return SEMICOLON; }
","  { return COMMA;}
"("  { return OPENPAR; }
")"  { return CLOSEPAR; }
"{"  { return OPENCHAV; }
"}"  { return CLOSECHAV; }

\"[^\"]*\" {
    yylval.strvalue = strdup(yytext);
    return STRING;
}

"for" { return FOR; }
"while"  { return WHILE; }
"printf" { return PRINTF; }
"scanf"  { return SCANF; }
"if"     { return IF; }
"else"   { return ELSE; }

"main" { return MAIN; }

[a-z][a-zA-z0-9]* {
    yylval.varname = strdup(yytext);
    return VARNAME;
}

\&[a-z][a-zA-z0-9]* {
    yylval.varname = strdup(yytext);
    return RVARNAME;
}

.  { yyerror("unexpected character"); }
%%
