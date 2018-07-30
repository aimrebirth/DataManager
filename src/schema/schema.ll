%{
#include <schema_parser.h>
%}

%option nounistd
%option yylineno
%option nounput
%option batch
%option never-interactive
%option reentrant
%option noyywrap

DIGIT       [0-9]
DIGITS      {DIGIT}{DIGIT}*
INTEGER     [+-]?{DIGITS}
BININT      0b[01]+

STRING      [[:alpha:]_][[:alnum:]_]*

%x user_string

%%

%{
    // Code run each time yylex is called.
    loc.step();
%}

#.*/\n                  ; // ignore comments

[ \t]+                  loc.step();
\r                      loc.step();
\n                      {
                            loc.lines(yyleng);
                            loc.step();
                        }

";"                     return MAKE(SEMICOLON);
":"                     return MAKE(COLON);
"("                     return MAKE(L_BRACKET);
")"                     return MAKE(R_BRACKET);
"{"                     return MAKE(L_CURLY_BRACKET);
"}"                     return MAKE(R_CURLY_BRACKET);
"["                     return MAKE(L_SQUARE_BRACKET);
"]"                     return MAKE(R_SQUARE_BRACKET);
","                     return MAKE(COMMA);
"\."                    return MAKE(POINT);
"->"                    return MAKE(R_ARROW);
"="                     return MAKE(EQUAL);

globals                 { return MAKE(GLOBALS); }
types                   { return MAKE(TYPES); }
class                   { return MAKE(CLASS); }
database                { return MAKE(DATABASE); }
enum                    { return MAKE(ENUM); }

field                   return MAKE(FIELD);
properties              return MAKE(PROPERTIES);

{INTEGER}               { return MAKE_VALUE(INTEGER, std::stoi(yytext)); }
{BININT}                { return MAKE_VALUE(INTEGER, std::stoi(yytext + 2, nullptr, 2)); }
{STRING}                { return MAKE_VALUE(STRING, yytext); }

\"                      { PUSH_STATE(user_string);  return MAKE(QUOTE); }
<user_string>\"         { POP_STATE();              return MAKE(QUOTE); }
<user_string>(?:[^"\\]|\\.)*/\" {
    int n = 0;
    char *p = yytext;
    while ((p = strstr(p, "\n"))++ != 0)
        n++;
    if (n)
    {
        loc.lines(n);
        loc.step();
    }
    return MAKE_VALUE(STRING, yytext);
}

.                       { /*driver.error(loc, "invalid character");*/ return MAKE(ERROR_SYMBOL); }
<<EOF>>                 return MAKE(EOQ);

%%
