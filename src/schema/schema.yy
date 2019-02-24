%{
#include <schema_parser.h>
%}

////////////////////////////////////////

// general settings
%require "3.0"
//%debug
%start file
%locations
%verbose
//%no-lines
%define parse.error verbose

////////////////////////////////////////

// c++ skeleton and options
%skeleton "lalr1.cc"
%define api.value.type variant
%define api.token.constructor // C++ style of handling variants
%define parse.assert // check C++ variant types
%code provides { #include <primitives/helper/bison_yy.h> }
%parse-param { MY_PARSER_DRIVER &driver } // param to yy::parser() constructor (the parsing context)

%code requires
{
#include <Ast.h>
#include <Token.h>
}

%code provides
{
struct MY_PARSER_DRIVER : MY_PARSER
{
    using base = MY_PARSER;

    enum class Mode
    {
        String,
        Tokens,
    };

    Mode parseMode;
    ast::Schema schema;
    Tokens *tokensWrite;
    const Tokens *tokensRead;
    Tokens::const_iterator readIterator;

    symbol_type convert(const Token &t)
    {
        symbol_type s;
        s.type = t.token;
        switch (t.type)
        {
        case Token::Integer:
            s.value.build<int>(boost::get<int>(t.value));
            break;
        case Token::String:
            s.value.build<std::string>(boost::get<std::string>(t.value));
            break;
        }
        return s;
    }

    Token convert(const symbol_type &tok)
    {
        Token t;
        t.token = tok.type;

        by_type bt;
        bt.type = tok.type;
        switch (bt.token())
        {
        case token::INTEGER:
            t.type = Token::Integer;
            t.value = tok.value.as<int>();
            break;
        case token::STRING:
            t.type = Token::String;
            t.value = tok.value.as<std::string>();
            break;
        default:
            t.type = Token::None;
            break;
        }
        return t;
    }

    symbol_type lex()
    {
        if (parseMode == Mode::String)
        {
            auto ret = base::lex();
            if (tokensWrite)
                tokensWrite->push_back(convert(ret));
            return ret;
        }
        else
        {
            return convert(*readIterator++);
        }
    }

    int parse(const std::string &s, Tokens *tokens)
    {
        parseMode = Mode::String;
        tokensWrite = tokens;
        //auto parser(*this); // why copy?
        //int res = parser.base::parse(s);
        //return res;
        return base::parse(s);
    }

    int parse(const Tokens &tokens)
    {
        parseMode = Mode::Tokens;
        tokensRead = &tokens;
        readIterator = tokensRead->cbegin();
        //auto parser(*this); // why copy?
        return /*parser.*/THIS_PARSER::parser::parse();
    }

    void setSchema(const ast::Schema &schema) { this->schema = schema; }
    ast::Schema getSchema() const { return schema; }
};
}

////////////////////////////////////////

// tokens and types
%token EOQ 0 "end of file"
%token ERROR_SYMBOL
%token L_BRACKET R_BRACKET COMMA QUOTE SEMICOLON COLON POINT
       L_CURLY_BRACKET R_CURLY_BRACKET SHARP R_ARROW EQUAL
       L_SQUARE_BRACKET R_SQUARE_BRACKET
%token GLOBALS CLASS FIELD TYPES PROPERTIES DATABASE ENUM

%token <std::string> STRING
%token <int> INTEGER

%type <std::string> string name type key value quoted_string any_value globals
%type <int> integer
%type <ast::Property> key_value_pair property field_content
%type <ast::Properties> properties properties_braced field_contents
%type <ast::Variable> field class_content
%type <ast::Variables> class_contents
%type <ast::Class> class
%type <ast::Classes> classes
%type <ast::Type> type_decl
%type <ast::Types> type_decls database_content database_contents types
%type <ast::Database> database
%type <ast::Databases> databases
%type <ast::EnumItem> enum_var
%type <ast::EnumItems> enum_vars
%type <ast::Enum> enum simple_enum enum_with_properties
%type <ast::Enums> enums
%type <ast::Schema> schema file

////////////////////////////////////////

%%

file: schema EOQ
    { driver.setSchema($1); }
    ;

schema: globals types databases enums classes
    {
        ast::Schema s;
        s.version = $1;
        s.types = $2;
        s.databases = $3;
        s.enums = $4;
        s.classes = $5;
        $$ = s;
    }
    ;

globals: GLOBALS L_CURLY_BRACKET string COLON value R_CURLY_BRACKET SEMICOLON
    {
        $$ = $5;
    }
    ;

enums: enum
    {
        ast::Enums es;
        es.push_back($1);
        $$ = es;
    }
    | enums enum
    {
        $1.push_back($2);
        $$ = $1;
    }
    ;
enum: simple_enum
    { $$ = $1; }
    | enum_with_properties
    { $$ = $1; }
    ;
simple_enum: ENUM string L_CURLY_BRACKET enum_vars R_CURLY_BRACKET SEMICOLON
    {
        ast::Enum e;
        e.name = $2;
        e.items = $4;
        $$ = e;
    }
    ;
enum_with_properties: ENUM string L_CURLY_BRACKET enum_vars PROPERTIES properties_braced R_CURLY_BRACKET SEMICOLON
    {
        ast::Enum e;
        e.name = $2;
        e.items = $4;
        e.properties = $6;
        $$ = e;
    }
    ;

enum_vars: enum_var
    {
        ast::EnumItems es;
        es.push_back($1);
        $$ = es;
    }
    | enum_vars enum_var
    {
        $1.push_back($2);
        $$ = $1;
    }
    ;

enum_var: string COMMA
    {
        ast::EnumItem i{ $1, ast::EnumItem::default_id };
        $$ = i;
    }
    | string EQUAL integer COMMA
    {
        ast::EnumItem i{ $1, $3 };
        $$ = i;
    }
    | string properties_braced COMMA
    {
        ast::EnumItem i{ $1, ast::EnumItem::default_id };
        i.properties = $2;
        $$ = i;
    }
    | string EQUAL integer properties_braced COMMA
    {
        ast::EnumItem i{ $1, $3 };
        i.properties = $4;
        $$ = i;
    }
    ;


databases: database
    {
        ast::Databases ds;
        ds.push_back($1);
        $$ = ds;
    }
    | databases database
    {
        $1.push_back($2);
        $$ = $1;
    }
    ;
database: DATABASE name L_CURLY_BRACKET database_contents R_CURLY_BRACKET SEMICOLON
    {
        ast::Database d;
        d.name = $2;
        d.types = $4;
        $$ = d;
    }
    ;
database_contents: database_content
    {
        ast::Types ts($1.begin(), $1.end());
        $$ = ts;
    }
    | database_contents database_content
    {
        $1.insert($2.begin(), $2.end());
        $$ = $1;
    }
    ;
database_content: types
    { $$ = $1; }
    ;

types: TYPES L_CURLY_BRACKET type_decls R_CURLY_BRACKET SEMICOLON
    { $$ = $3; }
    ;
type_decls: type_decl
    {
        ast::Types ts;
        ts.insert($1);
        $$ = ts;
    }
    | type_decls type_decl
    {
        $1.insert($2);
        $$ = $1;
    }
    ;
type_decl: key R_ARROW value SEMICOLON
    {
        $$ = ast::Type($1, $3);
    }
    ;

classes: class
    {
        ast::Classes cc;
        cc.insert($1);
        $$ = cc;
    }
    | classes class
    {
        $1.insert($2);
        $$ = $1;
    }
    ;
class: CLASS name L_CURLY_BRACKET class_contents R_CURLY_BRACKET SEMICOLON
    {
        ast::Class c;
        c.name = $2;
        c.variables = $4;
        $$ = c;
    }
    | CLASS name L_CURLY_BRACKET class_contents PROPERTIES properties_braced R_CURLY_BRACKET SEMICOLON
    {
        ast::Class c;
        c.name = $2;
        c.variables = $4;
        c.properties = $6;
        $$ = c;
    }
    | CLASS name L_CURLY_BRACKET PROPERTIES properties_braced R_CURLY_BRACKET SEMICOLON
    {
        ast::Class c;
        c.name = $2;
        c.properties = $5;
        $$ = c;
    }
    ;
class_contents: class_content
    {
        ast::Variables vs;
        vs.push_back($1);
        $$ = vs;
    }
    | class_contents class_content
    {
        $1.push_back($2);
        $$ = $1;
    }
    ;
class_content: field
    { $$ = $1; }
    ;

field: FIELD L_CURLY_BRACKET field_contents R_CURLY_BRACKET
    {
        ast::Variable v;
        v.properties = $3;
        $$ = v;
    }
    | type name SEMICOLON
    {
        ast::Variable v;
        v.type = $1;
        v.name = $2;
        $$ = v;
    }
    | type name EQUAL any_value SEMICOLON
    {
        ast::Variable v;
        v.type = $1;
        v.name = $2;
        v.defaultValue = $4;
        $$ = v;
    }
    | type name properties_braced SEMICOLON
    {
        ast::Variable v;
        v.type = $1;
        v.name = $2;
        v.properties = $3;
        $$ = v;
    }
    ;
field_contents: field_content
    {
        ast::Properties ps;
        ps[$1.key] = $1;
        $$ = ps;
    }
    | field_contents field_content
    {
        $1[$2.key] = $2;
        $$ = $1;
    }
    ;
field_content: key_value_pair SEMICOLON
    { $$ = $1; }
    ;

properties_braced: L_CURLY_BRACKET properties R_CURLY_BRACKET
    { $$ = $2; }
    ;

properties: property
    {
        ast::Properties ps;
        ps[$1.key] = $1;
        $$ = ps;
    }
    | properties property
    {
        $1[$2.key] = $2;
        $$ = $1;
    }
    ;
property: key SEMICOLON
    {
        ast::Property p;
        p.key = $1;
        $$ = p;
    }
    | key_value_pair SEMICOLON
    { $$ = $1; }
    | key properties_braced
    {
        ast::Property p;
        p.key = $1;
        p.properties = std::make_shared<ast::Properties>($2);
        $$ = p;
    }
    | key COLON L_SQUARE_BRACKET properties R_SQUARE_BRACKET SEMICOLON
    {
        ast::Property p;
        p.key = $1;
        p.properties = std::make_shared<ast::Properties>($4);
        $$ = p;
    }
    ;

key_value_pair: key COLON value
    {
        ast::Property p;
        p.key = $1;
        p.value = $3;
        $$ = p;
    }
    ;

name: string
    { $$ = $1; }
    ;
type: string
    { $$ = $1; }
    ;
key: string
    { $$ = $1; }
    ;
any_value: value
    { $$ = $1; }
    | integer
    { $$ = std::to_string($1); }
    ;
value: string
    { $$ = $1; }
    | quoted_string
    { $$ = $1; }
    ;
quoted_string: QUOTE string QUOTE
    { $$ = $2; }
    ;
string: STRING
    { $$ = $1; }
    ;
integer: INTEGER
    { $$ = $1; }
    ;

%%
