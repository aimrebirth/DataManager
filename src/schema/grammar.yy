%{
#include <assert.h>
#include <iostream>
#include <string>
#include <type_traits>

#include <Polygon4/DataManager/MemoryManager.h>

#include <Ast.h>

using namespace ast;

// Prevent using <unistd.h> because of bug in flex.
#define YY_NO_UNISTD_H 1
#include "lexer.h"
#include "grammar.hpp"

void yyerror(const YYLTYPE *yylloc, const std::string &msg);

extern Schema *schema;
extern MemoryManager *parserMemoryManager;

#define CREATE(type, ...) parserMemoryManager->create<type>(__VA_ARGS__)
#define CREATE_IF_NULL(v, type, ...) if (v == nullptr) v = parserMemoryManager->create<type>(__VA_ARGS__)
#define SET_NULL(v) v = nullptr
#define RESET(v) v = decltype(v)()

%}

%require "2.5"
%debug
%start file
%locations
%verbose
//%no-lines
%error-verbose

%define api.pure full
%define api.push-pull push

%union {
    // lexer vars
    int intVal;
    double doubleVal;
    char *rawStrVal;

    // parser vars
    std::string *strVal;
}

%token EOQ 0 "end of file"
%token ERROR_SYMBOL
%token  L_BRACKET R_BRACKET COMMA QUOTE SEMICOLON COLON POINT
        L_CURLY_BRACKET R_CURLY_BRACKET SHARP R_ARROW EQUAL
%token GLOBALS CLASS FIELD TYPES PROPERTIES DATABASE ENUM
%token <rawStrVal> STRING
%token <intVal> INTEGER

%type <strVal> string name type key value quoted_string any_value

%%

file: file_contents EOQ
    ;

file_contents: /* empty */
    | globals
    ;

globals: global
    | global globals
    ;

global: GLOBALS L_CURLY_BRACKET globals1 R_CURLY_BRACKET SEMICOLON
    | TYPES L_CURLY_BRACKET type_decls R_CURLY_BRACKET SEMICOLON
    {
        schema->types = pd->types;
        RESET(pd->types);
    }
    | databases
    {
        schema->databases = pd->databases;
        RESET(pd->databases);
    }
    | classes
    {
        schema->classes = pd->classes;
        RESET(pd->classes);
    }
    | enums
    {
        schema->enums = pd->enums;
        RESET(pd->enums);
    }
    ;

enums: enum
    | enum enums
    ;

enum: simple_enum
    | enum_with_properties
    ;

simple_enum: ENUM string L_CURLY_BRACKET enum_vars R_CURLY_BRACKET SEMICOLON
    {
        pd->enum_.name = *$2;
        pd->enums.push_back(pd->enum_);
        RESET(pd->enum_var_id);
        RESET(pd->enum_);
    }
    ;
enum_with_properties: ENUM string L_CURLY_BRACKET enum_vars PROPERTIES properties_braced R_CURLY_BRACKET SEMICOLON
    {
        pd->enum_.name = *$2;
        pd->enum_.properties = pd->properties;
        pd->enums.push_back(pd->enum_);
        RESET(pd->enum_var_id);
        RESET(pd->enum_);
        RESET(pd->properties);
    }
    ;

enum_vars: enum_var
    | enum_var enum_vars
    ;

enum_var: string COMMA
    {
        pd->enum_.items.push_back({*$1, 0});
    }
    | string EQUAL INTEGER COMMA
    {
        pd->enum_.items.push_back({*$1, $3});
    }
    | string properties_braced COMMA
    {
        EnumItem item = {*$1, 0};
        item.properties = pd->properties;
        RESET(pd->properties);
        pd->enum_.items.push_back(item);
    }
    | string EQUAL INTEGER properties_braced COMMA
    {
        EnumItem item = {*$1, $3};
        item.properties = pd->properties;
        RESET(pd->properties);
        pd->enum_.items.push_back(item);
    }
    ;

globals1: string COLON value
    {
        schema->version = *$3;
    }
    ;

databases: database
    | database databases
    ;
database: DATABASE name L_CURLY_BRACKET database_contents R_CURLY_BRACKET SEMICOLON
    {
        pd->database.name = *$2;
        pd->databases.push_back(pd->database);
        RESET(pd->database);
    }
    ;
database_contents: database_content
    | database_content database_contents
    ;
database_content: TYPES L_CURLY_BRACKET type_decls R_CURLY_BRACKET
    {
        pd->database.types = pd->types;
        RESET(pd->types);
    }
    ;

type_decls: type_decl
    | type_decl type_decls
    ;
type_decl: key R_ARROW value SEMICOLON
    {
        pd->types.insert(Type(*$1, *$3));
    }
    ;

classes: class
    | class classes
    ;
class: CLASS name L_CURLY_BRACKET class_contents R_CURLY_BRACKET SEMICOLON
    {
        pd->class_.name = *$2;
        auto p = pd->classes.insert(pd->class_);
        RESET(pd->class_);
        pd->variable_id = 0;
        assert(p.second);
    }
    ;
class_contents: class_content
    | class_content class_contents
    ;
class_content: field
    ;

field: FIELD L_CURLY_BRACKET field_contents R_CURLY_BRACKET
    {
        Variable variable;
        variable.properties = pd->properties;
        pd->class_.variables.push_back(variable);
        RESET(pd->properties);
    }
    | type name SEMICOLON
    {
        Variable variable;
        variable.id = pd->variable_id++;
        variable.type = *$1;
        variable.name = *$2;
        pd->class_.variables.push_back(variable);
    }
    | type name EQUAL any_value SEMICOLON
    {
        Variable variable;
        variable.id = pd->variable_id++;
        variable.type = *$1;
        variable.name = *$2;
        variable.defaultValue = *$4;
        pd->class_.variables.push_back(variable);
    }
    | type name properties_braced SEMICOLON
    {
        Variable variable;
        variable.id = pd->variable_id++;
        variable.type = *$1;
        variable.name = *$2;
        variable.properties = pd->properties;
        RESET(pd->properties);
        pd->class_.variables.push_back(variable);
    }
    | PROPERTIES properties_braced
    {
        pd->class_.properties.insert(pd->properties.begin(), pd->properties.end());
        RESET(pd->properties);
    }
    ;
field_contents: field_content
    | field_content field_contents
    ;
field_content: key_value_pair SEMICOLON
    ;

properties_braced: L_CURLY_BRACKET properties R_CURLY_BRACKET
    ;

properties: property
    | property properties
    ;
property: value SEMICOLON
    {
        Property p;
        p.key = *$1;
        pd->properties[*$1] = p;
    }
    | key_value_pair SEMICOLON
    {
        pd->properties[pd->property.key] = pd->property;
        RESET(pd->property);
    }
    | key properties_braced
    {
        Property p;
        p.key = *$1;
        p.properties = std::make_shared<Properties>(pd->properties);
        RESET(pd->properties);
        pd->properties[*$1] = p;
    }
    ;

key_value_pair: key COLON value
    {
        pd->property.key = *$1;
        pd->property.value = *$3;
    }
    ;

name: string
    ;
type: string
    ;
key: string
    ;
any_value: value
    | INTEGER
    {
        $$ = CREATE(std::string, std::to_string($1));
    }
    ;
value: string
    | quoted_string
    ;
quoted_string: QUOTE string QUOTE
    { $$ = $2; }
    ;
string: STRING
    {
        auto p = CREATE(std::string, $1);
        free($1);
        $$ = p;
    }
    ;

%%

void yyerror(const YYLTYPE *yylloc, const std::string &msg)
{
    std::cerr << yylloc->first_line << ":" << yylloc->first_column << " " << msg << std::endl;
}
