
# include <stdio.h>
# include <iostream>
# include <string.h>
# include <string>
# include <vector>
# include <stdlib.h>
using namespace std;

enum TokenType {
    CONSTANT, IDENT, LPAR, RPAR, PLUS,
    MINUS, MUTI, DIVI, SEMI, ERROR,
    GREATER, GE, COLON,
    LESS, LE, EQUAL, ASSIGN,
    NOT_EQUAL, PP, MM, LMPAR, RMPAR,
    LLPAR, RLPAR, PERCENT, EXP, EXC,
    VER, OR, PE, ME, TE, DE, RE, RS, LS,
    AMP, AND, INT, FLOAT, CHAR, BOOL, STRING,
    VOID, IF, ELSE, WHILE, DO, RETURN, COMMA,
    QUEST, CIN, COUT
};

typedef char * CharPtr;
typedef char Str100[100];

struct Token {
    CharPtr tokenStr ;
    TokenType type ;
    int row ;
    int coloum ;
    Token * next;
}; // struct Token

typedef Token * TokenPtr;

struct Variable_of_c {
    Str100 variableName ;
    Str100 variableValue ;
    TokenType variableType ;
    int value_int ;
    float value_float ;
    Str100 value_string ;
    bool value_bool ;
    int arrayNum ;
    int arraySize ;
    Str100 value_char ;
    bool isArray ;
    vector<Variable_of_c> array ;
} ; // struct variable

typedef Variable_of_c * VariablePtr ;

struct Function_of_c {
    Str100 functionName ;
    Str100 functionType ;
    TokenPtr functionBodyHead ;
    TokenPtr functionBodyEnd ;
} ; // struct Function_of_c

struct Parameter_of_c {
    Str100 parameterType ;
    Str100 parameterName ;
} ; // struct Parameter_of_c

struct PPorMM_Caculate {
    Str100 identName ;
    TokenType ppormm ;
    int arrayNum ;
} ; // struct PPorMMCaculate

struct Value_of_output {
    Str100 name ;
    Str100 value ;
    TokenType type ;
    int value_int ;
    float value_float ;
    Str100 value_string ;
    bool value_bool ;
    Str100 value_char ;
    int arraynum ;
    Value_of_output * next ;
} ; // sturct Value_of_output

vector<Value_of_output> gOutputlist ;
vector<Variable_of_c> gVariablelist ;
vector<Variable_of_c> gLocalVariablelist ;
vector<Function_of_c> gFunctionlist ;

TokenPtr gWhileHead = NULL , gWhileRear = NULL, gWhileWalk ;
TokenPtr gFront = NULL, gRear = NULL, gwalk = NULL;
TokenPtr gNextToken ;
TokenPtr gCurrentToken ;
bool IsWhile = false ;
bool gDone = false ; // Exit the interpreter
bool gCallByPeek ;
bool gIsFunctionDefine ;

bool gNowIsTrueSoDO;
int gline = 1 ;
int gcoloum = 0 ;
int gTheFirstColoumOfChar = 0 ;
int gCurrentLine = 1 ;
bool gIsCout ;
bool gDoneBoolExp ;  // for checking is or is not bool exprssion
bool gPeek ;     // if have peek token first, then gPeek is true
bool gErrorMessage ; // if ErrorMessage has send, then gErrorMessage is true
bool gIsIdentError ;
bool gHasGetToken = false ; // if has GetToken is true and return false, error occur
bool SkipChar( char ch ) ;
void GetChar( char & ch ) ;
TokenPtr Assort( char ch ) ;
TokenPtr MakeToken( Str100 str, TokenType type ) ;
void SkipComment() ;
void GetToken( TokenPtr &token ) ;
bool IsDigit( char ch ) ;
bool ISIdentifier( char ch ) ;
bool IsWhiteSpace( char ch ) ;
bool IsComment( char ch ) ;
bool Quit( TokenPtr walk ) ;
void PeekToken( TokenPtr & token ) ;
void GetToken( TokenPtr & token ) ;
bool IsFloat( Str100 tokenStr ) ;

bool IsFloat( Str100 tokenStr ) {
    
    for ( int i = 0 ; tokenStr[i] != '\0' ; i++ ) {
        if ( tokenStr[i] == '.' )
            return true;
    } // for
    
    return false ;
} // IsFloat()

void PeekToken( TokenPtr & token ) {
    if ( gWhileWalk != NULL ) {
        token = gWhileWalk ;
    } // if
    else if ( gPeek == true ) {
        token = gNextToken ;
    } // if
    else {
        gCallByPeek = true ;
        GetToken( gNextToken ) ;
        gCallByPeek = false;
        gPeek = true ;
        token = gNextToken ;
    } // else
    
} // PeekToken()

void GetToken( TokenPtr &token ) {
    if ( gWhileWalk != NULL ) {
        token = gWhileWalk ;
        if ( gWhileWalk != gWhileRear )
            gWhileWalk = gWhileWalk -> next ;
    } // if
    else if ( gPeek ) {
        token = gNextToken ;
        gPeek = false ;
        
    } // if
    else {
        char ch ;
        GetChar( ch ) ;
        while ( SkipChar( ch ) ) {
            if ( IsComment( ch ) )
                SkipComment() ;
            GetChar( ch ) ;
        } // while
        
        gTheFirstColoumOfChar = gcoloum;
        token = Assort( ch ) ;
   
    } // else

    if ( !gCallByPeek ) {
        gCurrentToken = token ;
        gHasGetToken = true ;
        gCurrentLine = token -> row ;
    } // if
} // GetToken()

void GetChar( char & ch ) {
    scanf( "%c", &ch ) ;
    if ( ch == '\n' ) {
        gcoloum = 0;
        gline++ ;
    } // if
    else
        gcoloum++ ;
} // end GetChar()

bool SkipChar( char ch ) {
    if ( IsWhiteSpace( ch ) || ( ch == '/' && IsComment( ch ) ) )
        return true;
    else
        return false;
} // end SkipChar()

bool IsWhiteSpace( char ch ) {
    if ( ch == ' ' || ch == '\n' || ch == '\r' || ch == '\t' )
        return true;
    else
        return false;
} // end IsWhiteSpace()

bool IsComment( char ch ) {
    char tmp = cin.peek();
    if ( ch == '/' && tmp == '/' )
        return true;
    else
        return false;
} // end IsComment()

void SkipComment() {
    char ch = '\0';
    while ( ch != '\n' )
        GetChar( ch ) ;
} // end SkipComment()

bool IsIdentifier( char ch ) {
    if ( ( 'A' <= ch && 'Z' >= ch ) || ( 'a' <= ch && 'z' >= ch ) )
        return true;
    else
        return false;
} // end IsIdentifier()

bool IsDigit( char ch ) {
    if ( ch >= '0' && ch <= '9' )
        return true;
    else if ( ch == '.' ) {
        char ch = cin.peek();
        if ( ch >= '0' && ch <= '9' )
            return true;
        else
            return false;
    } // else if
    else
        return false;
} // end IsDigit()

TokenPtr MakeToken( Str100 str, TokenType type ) {
    TokenPtr tmp = new Token ;
    tmp -> next = NULL;
    tmp -> tokenStr = new char[100];
    tmp -> type = type;
    tmp -> row = gline;
    tmp -> coloum = gTheFirstColoumOfChar ;
    strcpy( tmp -> tokenStr, str );
    if ( gFront == NULL ) {
        gFront = tmp;
        gRear = tmp;
    } // if
    else {
        gRear -> next = tmp;
        gRear = tmp;
    } // else
    
    return tmp;
} // end MakeToken()

TokenPtr Assort( char ch ) {
    char tmp[100] = "\0";
    char input[2] = {'\0'};
    char nextchar ;
    TokenPtr token ;
    if ( IsIdentifier( ch ) ) {
        input[0] = ch ;
        strcat( tmp, input ) ;
        TokenType type ;
        nextchar = cin.peek() ;
        while ( IsIdentifier( nextchar ) || IsDigit( nextchar ) || nextchar == '_' ) {
            
            GetChar( ch ) ;
            input[0] = ch ;
            strcat( tmp, input ) ;
            nextchar = cin.peek() ;
        } // while
        
        if ( strcmp( tmp, "int" ) == 0 )
            type = INT ;
        else if ( strcmp( tmp, "float" ) == 0 )
            type = FLOAT ;
        else if ( strcmp( tmp, "char" ) == 0 )
            type = CHAR;
        else if ( strcmp( tmp, "bool" ) == 0 )
            type = BOOL ;
        else if ( strcmp( tmp, "string" ) == 0 )
            type = STRING ;
        else if ( strcmp( tmp, "void" ) == 0 )
            type = VOID ;
        else if ( strcmp( tmp, "if" ) == 0 )
            type = IF ;
        else if ( strcmp( tmp, "else" ) == 0 )
            type = ELSE;
        else if ( strcmp( tmp, "while" ) == 0 )
            type = WHILE ;
        else if ( strcmp( tmp, "do" ) == 0 )
            type = DO ;
        else if ( strcmp( tmp, "return" ) == 0 )
            type = RETURN ;
        else if ( strcmp( tmp, "true" ) == 0 )
            type = CONSTANT ;
        else if ( strcmp( tmp, "false" ) == 0 )
            type = CONSTANT ;
        else
            type = IDENT ;
        
        token = MakeToken( tmp, type );
        
    } // if
    else if ( IsDigit( ch ) ) {
        TokenType type = CONSTANT ;
        input[0] = ch ;
        bool point = false;
        if ( ch == '.' ) {
            point = true ;
            input[0] = ch ;
            strcat( tmp, input );
            nextchar = cin.peek();
            if ( nextchar >= '0' && nextchar <= '9' ) {
                
                while ( nextchar >= '0' && nextchar <= '9' ) {
                    GetChar( ch );
                    input[0] = ch ;
                    strcat( tmp, input );
                    nextchar = cin.peek();
                } // while
            } // if
            else
                type = ELSE ;
            
        } // if
        else {
            input[0] = ch ;
            strcat( tmp, input ) ;
            nextchar = cin.peek();
            
            while ( ( nextchar >= '0' && nextchar <= '9' ) || ( nextchar == '.' && !point ) ) {
                GetChar( ch ) ;
                if ( ch == '.' ) {
                    point = true;
                } // if
                
                input[0] = ch ;
                strcat( tmp, input ) ;
                nextchar = cin.peek() ;
            } // while
        } // else
        
        token = MakeToken( tmp, type );
        
    } // else if
    else {
        input[0] = ch ;
        strcat( tmp, input ) ;
        TokenType type ;
        if ( ch == '+' ) {
            type = PLUS ;
            char tmpofchar = cin.peek() ;
            if ( tmpofchar == '+' ) {
                type = PP ;
                GetChar( ch ) ;
                input[0] = ch ;
                strcat( tmp, input );
            } // if
            else if ( tmpofchar == '=' ) {
                type = PE ;
                GetChar( ch ) ;
                input[0] = ch ;
                strcat( tmp, input );
            } // if
        } // if
        else if ( ch == '-' ) {
            type = MINUS ;
            char tmpofchar = cin.peek() ;
            if ( tmpofchar == '-' ) {
                type = MM ;
                GetChar( ch ) ;
                input[0] = ch ;
                strcat( tmp, input );
            } // if
            else if ( tmpofchar == '=' ) {
                type = ME ;
                GetChar( ch ) ;
                input[0] = ch ;
                strcat( tmp, input );
            } // else if
        } // else if
        else if ( ch == '*' ) {
            type = MUTI ;
            char tmpofchar = cin.peek() ;
            if ( tmpofchar == '=' ) {
                type = TE ;
                GetChar( ch ) ;
                input[0] = ch ;
                strcat( tmp, input );
            } // if
        } // else if
        else if ( ch == '/' ) {
            type = DIVI ;
            char tmpofchar = cin.peek() ;
            if ( tmpofchar == '=' ) {
                type = DE ;
                GetChar( ch ) ;
                input[0] = ch ;
                strcat( tmp, input );
            } // if
        } // else if
        else if ( ch == '=' ) {
            type = ASSIGN ;
            char tmpofchar = cin.peek() ;
            if ( tmpofchar == '=' )  {
                type = EQUAL ;
                GetChar( ch ) ;
                input[0] = ch ;
                strcat( tmp, input );
            } // if
        } // else if
        else if ( ch == ';' )
            type = SEMI ;
        else if ( ch == '(' )
            type = LPAR ;
        else if ( ch == ')' )
            type = RPAR ;
        else if ( ch == '[' )
            type = LMPAR ;
        else if ( ch == ']' )
            type = RMPAR ;
        else if ( ch == '{' )
            type = LLPAR ;
        else if ( ch == '}' )
            type = RLPAR ;
        else if ( ch == ',' )
            type = COMMA ;
        else if ( ch == '?' )
            type = QUEST ;
        else if ( ch == '%' ) {
            type = PERCENT;
            char tmpofchar = cin.peek() ;
            if ( tmpofchar == '=' ) {
                type = RE ;
                GetChar( ch ) ;
                input[0] = ch ;
                strcat( tmp, input );
            } // if
        } // else if
        else if ( ch == '^' )
            type = EXP ;
        else if ( ch == '|' ) {
            type = VER ;
            char tmpofchar = cin.peek() ;
            if ( tmpofchar == '|' ) {
                type = OR ;
                GetChar( ch ) ;
                input[0] = ch ;
                strcat( tmp, input );
            } // if
        } // else if
        else if ( ch == '!' ) {
            type = EXC ;
            char tmpofchar = cin.peek() ;
            if ( tmpofchar == '=' ) {
                type = NOT_EQUAL ;
                GetChar( ch ) ;
                input[0] = ch ;
                strcat( tmp, input ) ;
            } // if
        } // else if
        else if ( ch == '&' ) {
            type = AMP;
            char tmpofchar = cin.peek() ;
            if ( tmpofchar == '&' ) {
                type = AND ;
                GetChar( ch );
                input[0] = ch;
                strcat( tmp, input );
            } // else if
        } // else if
        else if ( ch == '<' ) {
            type = LESS ;
            char tmpofchar = cin.peek() ;
            if ( tmpofchar == '=' ) {
                type = LE;
                GetChar( ch );
                input[0] = ch;
                strcat( tmp, input );
            } // if
            else if ( tmpofchar == '<' ) {
                type = LS ;
                GetChar( ch );
                input[0] = ch;
                strcat( tmp, input );
            } // else if
        } // else if
        else if ( ch == '>' ) {
            type = GREATER ;
            char tmpofchar = cin.peek() ;
            if ( tmpofchar == '=' ) {
                type = GE ;
                GetChar( ch );
                input[0] = ch;
                strcat( tmp, input );
            } // if
            else if ( tmpofchar == '>' ) {
                type = RS ;
                GetChar( ch );
                input[0] = ch;
                strcat( tmp, input );
            } // else if
        } // else if
        else if ( ch == ':' )
            type = COLON ;
        else if ( ch == '\'' ) {
            type = CONSTANT ;
            GetChar( ch ) ;
            while ( ch != '\'' ) {
                input[0] = ch;
                strcat( tmp, input ) ;
                GetChar( ch ) ;
            } // while
            
            input[0] = ch ;
            strcat( tmp, input ) ;
            
        } // else if
        else if ( ch == '"' ) {
            type = CONSTANT ;
            GetChar( ch ) ;
            while ( ch != '"' ) {
                input[0] = ch;
                strcat( tmp, input ) ;
                GetChar( ch ) ;
            } // while
            
            input[0] = ch ;
            strcat( tmp, input ) ;
            
        } // else if
        else
            type = ERROR ;
        
        token = MakeToken( tmp, type );
    } // else
    
    return token ;
} // end Assort()


bool IsVariableExist( Str100 variableName ) {
    for ( int i = 0 ; i < gVariablelist.size() ; i++ ) {
        if ( strcmp( variableName, gVariablelist[i].variableName ) == 0 )
            return true;
    } // for
    
    if ( strcmp( variableName, "Done" ) == 0 )
        return true ;
    else if ( strcmp( variableName, "ListAllVariables" ) == 0 )
        return true ;
    else if ( strcmp( variableName, "ListAllFunctions" ) == 0 )
        return true ;
    else if ( strcmp( variableName, "ListVariable" ) == 0 )
        return true ;
    else if ( strcmp( variableName, "ListFunction" ) == 0 )
        return true ;
    else
        return false;
} // IsVariableExist()

class Parser {
    public :
    Str100 mFunctionName ;
    Str100 mMaybeIsFunctionName ;
    Str100 mListFunctionOrVariableName ;
    Variable_of_c mTmpOfFunctionOrIdent ;
    Variable_of_c mTmpOfLocalDeclaration ;
    Function_of_c mTmpofFunction ;
    int mBeginLine ;
    vector<Variable_of_c> mTmpOfFunctionOrIdentList ;
    vector<Variable_of_c> mTmpOfLocalVariableList ;
    vector<PPorMM_Caculate> mTmpOfPMCaculateList ;
    bool mAssign ;
    bool mPPorMMHasExist ;
    bool mIsCompound_Statement ;
    bool mIsFunction ;
    bool mIsFunctionDefinition ;
    bool mFunctionHasBeenExecuted ;
    bool mMaybeIsFunction ;
    bool mIsParameter ;
    bool mHaveParameter ;
    bool User_input() {
        // cout << "User_input" << endl ;
        Value_of_output output ;
        cout << "> " ;
        ResetAllVariable() ;
        TokenPtr nextToken ;
        PeekToken( nextToken ) ;
        if ( nextToken -> row == 1 )
            mBeginLine = nextToken -> row ;
        else
            mBeginLine = gCurrentLine + 1 ;
        
        if ( Definition() )
            MessageOfCommand( 1 ) ;
        else if ( !gHasGetToken && Statement( output ) )
            MessageOfCommand( 2 ) ;
        else {
            ErrorMessage( ) ;
            return false ;
        } // else
        
        while ( !gDone ) {
            PeekToken( nextToken ) ;
  
            mBeginLine = gCurrentLine + 1 ;
            strcpy( output.value, "\0" ) ;
            strcpy( output.name, "\0" ) ;
            // clean the list of function or Ident name
            // reset mIsFunction, mIsFunctionDefinition mFunctionHasBeenExecuted, mIsCompound_Statement
            // reset gErrorMessage, gHasGetToken
            ResetAllVariable() ;
            
            cout << "> " ;
            
            
            if ( Definition() )
                MessageOfCommand( 1 ) ;
            else if ( !gHasGetToken && Statement( output ) )
                MessageOfCommand( 2 ) ;
            else {
                ErrorMessage() ;
                return false ;
            } // else
            
        } // while
        
        return true ;
    } // User_input()
    
    bool Definition() {
        // cout << "Definition" << endl ;
        TokenPtr currentToken ;
        TokenPtr nextToken ;
        PeekToken( nextToken ) ;
        Str100 tmpOfFunctionType ;
        if ( nextToken -> type == VOID ) {
            // void function definition
            
            GetToken( currentToken ) ; // VOID
            strcpy( tmpOfFunctionType, currentToken -> tokenStr ) ;
            PeekToken( nextToken ) ;
            if ( nextToken -> type == IDENT ) {
                GetToken( currentToken ) ; // Identifier
                strcpy( mMaybeIsFunctionName, currentToken -> tokenStr ) ;
                if ( Function_Definition_Without_ID() ) {
                    TokenPtr functionBodyHead = currentToken ;
                    mTmpofFunction.functionBodyHead = functionBodyHead ;
                    strcpy( mTmpofFunction.functionName, mMaybeIsFunctionName );
                    strcpy( mTmpofFunction.functionType, tmpOfFunctionType ) ;
                    return true ;
                } // if
                else
                    return false ;
            } // if
            else
                return false;
        } // if
        else if ( Type_Specifier() ) {
            PeekToken( nextToken ) ;
            strcpy( tmpOfFunctionType, mTmpofFunction.functionType ) ;
            if ( nextToken -> type == IDENT ) {
                // function definition or variable declaration
                
                GetToken( currentToken ) ; // Identifier
                strcpy( mTmpOfFunctionOrIdent.variableName, currentToken -> tokenStr );
                strcpy( mMaybeIsFunctionName, currentToken -> tokenStr ) ;
                if ( Function_Definition_Or_Declarators() ) {
                    if ( mIsFunctionDefinition ) {
                        TokenPtr functionBodyHead = currentToken ;
                        mTmpofFunction.functionBodyHead = functionBodyHead ;
                        strcpy( mTmpofFunction.functionName, currentToken -> tokenStr );
                        strcpy( mTmpofFunction.functionType, tmpOfFunctionType ) ;
                    } // if
                    
                    return true;
                } // if
                else
                    return false;
                
            } // if
            else
                return false;
        } // else if
        else
            return false;
    } // Definition()
    
    bool Type_Specifier() {
        // cout << "Type_Specifier" << endl ;
        TokenPtr nextToken ;
        TokenPtr currentToken ;
        PeekToken( nextToken ) ;
        if ( nextToken -> type == INT ||
               nextToken -> type == FLOAT ||
               nextToken -> type == CHAR ||
               nextToken -> type == STRING ||
               nextToken -> type == BOOL ) {
            GetToken( currentToken ) ;
            mTmpOfFunctionOrIdent.variableType = currentToken -> type ;
            mTmpOfLocalDeclaration.variableType = currentToken -> type ;
            strcpy( mTmpofFunction.functionType, currentToken -> tokenStr ) ;
            
            return true;
        } // if
        else
            return false;
    } // Type_Specifier()
    
    bool Function_Definition_Or_Declarators() {
        // cout << "Function_Definition_Or_Declarators" << endl ;
        if ( Function_Definition_Without_ID() ) {
            mIsFunctionDefinition = true ;
            gIsFunctionDefine = true ;
            
            return true;
        } // if
        else if ( Rest_Of_Declarators() )
            return true;
        else
            return false;
    } // Function_Definition_Or_Declarators()
    
    bool Rest_Of_Declarators() {
        // cout << "Rest_Of_Declarators" << endl ;
        TokenPtr nextToken ;
        TokenPtr currentToken ;
        PeekToken( nextToken ) ;
        // [
        if ( nextToken -> type == LMPAR ) {
            GetToken( currentToken ) ; // "["
            PeekToken( nextToken ) ;
            if ( nextToken -> type == CONSTANT ) {
                GetToken( currentToken ) ; // CONSTANT
                mTmpOfFunctionOrIdent.arraySize = atoi( currentToken -> tokenStr ) ;
                mTmpOfFunctionOrIdent.isArray = true ;
                PeekToken( nextToken ) ;
                if ( nextToken -> type == RMPAR ) {
                    GetToken( currentToken ) ;
                } // if
                else
                    return false;
            } // if
            else
                return false;
        } // if
        else {
            mTmpOfFunctionOrIdent.arraySize = -1 ;
            mTmpOfFunctionOrIdent.isArray = false ;
        } // else
        // ]
        
        PeekToken( nextToken ) ;
        mTmpOfFunctionOrIdentList.push_back( mTmpOfFunctionOrIdent ) ;
        while ( nextToken -> type == COMMA ) {
            GetToken( currentToken ) ; // ","
            PeekToken( nextToken ) ;
            if ( nextToken -> type == IDENT ) {
                GetToken( currentToken ) ;
              
                strcpy( mTmpOfFunctionOrIdent.variableName, currentToken -> tokenStr ) ;
               
            } // if
            else
                return false ;
            
            PeekToken( nextToken ) ;
            // [
            if ( nextToken -> type == LMPAR ) {
                GetToken( currentToken ) ; // "["
                PeekToken( nextToken ) ;
                if ( nextToken -> type == CONSTANT ) {
                    GetToken( currentToken ) ;
                    mTmpOfFunctionOrIdent.arraySize = atoi( currentToken -> tokenStr ) ;
                    mTmpOfFunctionOrIdent.isArray = true ;
                    PeekToken( nextToken ) ;
                    if ( nextToken -> type == RMPAR ) {
                        GetToken( currentToken ) ;
                    } // if
                    else
                        return false;
                } // if
                else
                    return false;
            } // if
            else {
                mTmpOfFunctionOrIdent.isArray = false ;
                mTmpOfFunctionOrIdent.arraySize = -1 ;
            } // else
            // ]
            if ( mIsCompound_Statement ) {
                mTmpOfLocalVariableList.push_back( mTmpOfFunctionOrIdent ) ;
            } // if
            else
                mTmpOfFunctionOrIdentList.push_back( mTmpOfFunctionOrIdent ) ;
            
            PeekToken( nextToken ) ;
        } // while
        
        PeekToken( nextToken ) ;
        if ( nextToken -> type == SEMI ) {
            GetToken( currentToken );
            return true;
        } // if
        else
            return false;
    } // Rest_Of_Declarators()
    
    // set function definition to true //
    bool Function_Definition_Without_ID() {
        Value_of_output value ;
        // cout << "Function_Definition_Without_ID" << endl ;
        TokenPtr currentToken ;
        TokenPtr nextToken ;
        PeekToken( nextToken ) ;
        if ( nextToken -> type == LPAR ) {
            mIsFunctionDefinition = true ;
            gIsFunctionDefine = true ;
            GetToken( currentToken ) ; // "("
        } // if
        else
            return false;
        
        // [
        PeekToken( nextToken ) ;
        if ( nextToken -> type == VOID )
            GetToken( currentToken ) ;
        else
            Formal_Parameter_List() ;
        // ]
        
        PeekToken( nextToken ) ;
        if ( nextToken -> type == RPAR )
            GetToken( currentToken ) ; // ")"
        else
            return false;
        
        if ( Compound_Statement( value ) )
            return true;
        else
            return false;
        
    } // Function_Definition_Without_ID()
    
    bool Formal_Parameter_List() {
        // Only function's parameter
        // Ex int AddTwo( int x )
        // int x will execute in here
        // cout << "Formal_Parameter_List" << endl ;
        TokenPtr nextToken ;
        TokenPtr currentToken ;
        if ( Type_Specifier() ) ;
        else
            return false;
        // [
        PeekToken( nextToken ) ;
        if ( nextToken -> type == AMP )
            GetToken( currentToken ) ; // "&"
        // ]
        
        PeekToken( nextToken ) ;
        if ( nextToken -> type == IDENT ) {
            GetToken( currentToken );
            strcpy( mTmpOfLocalDeclaration.variableName, currentToken -> tokenStr ) ;
            gLocalVariablelist.push_back( mTmpOfLocalDeclaration ) ;
        } // if
        else
            return false;
        
        // [
        PeekToken( nextToken ) ;
        if ( nextToken -> type == LMPAR ) {
            GetToken( currentToken ) ; // "["
            PeekToken( nextToken ) ;
            if ( nextToken -> type == CONSTANT )
                GetToken( currentToken ) ;
            else
                return false;
            
            PeekToken( nextToken ) ;
            if ( nextToken -> type == RMPAR )
                GetToken( currentToken ) ;
            else
                return false;
        } // if
        // ]
        
        PeekToken( nextToken ) ;
        while ( nextToken -> type == COMMA ) {
            GetToken( currentToken ) ; // ","
            if ( Type_Specifier() ) ;
            else
                return false;
            // [
            PeekToken( nextToken ) ;
            if ( nextToken -> type == AMP )
                GetToken( currentToken ) ; // "&"
            // ]
            
            PeekToken( nextToken ) ;
            if ( nextToken -> type == IDENT ) {
                GetToken( currentToken );
                strcpy( mTmpOfLocalDeclaration.variableName, currentToken -> tokenStr ) ;
                gLocalVariablelist.push_back( mTmpOfLocalDeclaration ) ;
            } // if
            else
                return false;
            
            // [
            PeekToken( nextToken ) ;
            if ( nextToken -> type == LMPAR ) {
                GetToken( currentToken ) ; // "["
                PeekToken( nextToken ) ;
                if ( nextToken -> type == CONSTANT )
                    GetToken( currentToken ) ;
                else
                    return false;
                
                PeekToken( nextToken ) ;
                if ( nextToken -> type == RMPAR )
                    GetToken( currentToken ) ;
                else
                    return false;
            } // if
            // ]
            
            PeekToken( nextToken ) ;
        } // while
        
        return true ;
    } // Formal_Parameter_List()
    
    bool Compound_Statement( Value_of_output &value ) {
        
        // cout << "Compound_Statement" << endl ;
        
        TokenPtr currentToken ;
        TokenPtr nextToken ;
        PeekToken( nextToken ) ;
        vector<Variable_of_c> variableNeedToDelete ;
        if ( nextToken -> type == LLPAR ) {
            GetToken( currentToken ) ; // "{"
            mIsCompound_Statement = true ;
            while ( Declaration() || Statement( value ) ) {
                if ( mTmpOfLocalVariableList.size() != 0 ) {
                    for ( int i = 0 ; i < mTmpOfLocalVariableList.size() ; i++ ) {
                        variableNeedToDelete.push_back( mTmpOfLocalVariableList[i] ) ;
                        gLocalVariablelist.push_back( mTmpOfLocalVariableList[i] ) ;
                    } // for
                    
                    // clean list for next round
                    mTmpOfLocalVariableList.clear() ;
                    
                } // if
            } // while
            
            PeekToken( nextToken ) ;
            if ( nextToken -> type == RLPAR ) {
                GetToken( currentToken ) ; // "}"
                // clean local variablelist
                // to deal with muti compound statement
                for ( int i = 0 ; i < gLocalVariablelist.size() ; i ++ ) {
                    for ( int j = 0 ; j < variableNeedToDelete.size() ; j++ ) {
                        if ( strcmp( gLocalVariablelist[i].variableName, variableNeedToDelete[j].variableName ) == 0 )
                            gLocalVariablelist.erase( gLocalVariablelist.begin() + i );
                    } // for
                } // for
                
                if ( mIsFunctionDefinition ) {
                    TokenPtr functionBodyEnd = currentToken ;
                    mTmpofFunction.functionBodyEnd = functionBodyEnd ;
                } // if
                
                return true;
            } // if
            else
                return false;
        } // if
        else
            return false;
    } // Compound_Statement()
    
    bool Declaration() {
        // In here only call by compound statement
        // cout << "Declaration" << endl ;
        
        TokenPtr currentToken ;
        TokenPtr nextToken ;
        
        if ( Type_Specifier() ) {
            PeekToken( nextToken ) ;
            if ( nextToken -> type == IDENT ) {
                GetToken( currentToken ) ; // IDENT
                strcpy( mTmpOfLocalDeclaration.variableName, currentToken -> tokenStr );
                mTmpOfLocalVariableList.push_back( mTmpOfLocalDeclaration );
                if ( Rest_Of_Declarators() )
                    return true;
                else
                    return false;
            } // if
            else
                return false;
        } // if
        else
            return false;
    } // Declaration()
    
    bool Statement( Value_of_output &value ) {
        
        // cout << "Statement" << endl ;
        TokenPtr currentToken ;
        TokenPtr nextToken ;
        PeekToken( nextToken ) ;
        
        if ( nextToken -> type == SEMI ) {
            GetToken( currentToken ) ; // ";"
            return true;
        } // if
        else if ( Expression( value ) ) {
            PeekToken( nextToken ) ;
            if ( nextToken -> type == SEMI ) {
                GetToken( currentToken ) ; // ";"
                if ( mIsFunction )
                    ExecuteFunction( mFunctionName ) ;
                
                return true;
            } // if
        } // else if
        else if ( nextToken -> type == RETURN ) {
            GetToken( currentToken ) ; // "return"
            // [
            Expression( value ) ;
            // ]
            PeekToken( nextToken ) ;
            if ( gErrorMessage ) {
                return false ;
            } // if
            
            if ( nextToken -> type == SEMI ) {
                GetToken( currentToken ) ; // ";"
                return true;
            } // if
        } // else if
        else if ( Compound_Statement( value ) )
            return true;
        else if ( nextToken -> type == IF ) {
            GetToken( currentToken ) ; // "if"
            PeekToken( nextToken ) ;
            Value_of_output trueOrFlase ;
            Value_of_output value1, value2 ;
            
            vector<Value_of_output> outputlist ;
            vector<Variable_of_c> variablelist ;
            vector<Variable_of_c> localVariablelist ;
            
            vector<Value_of_output> outputlist_after ;
            vector<Variable_of_c> variablelist_after ;
            vector<Variable_of_c> localVariablelist_after ;
            
            if ( nextToken -> type == LPAR ) {
                GetToken( currentToken ) ; // "("
                if ( Expression( trueOrFlase ) ) {
                    PeekToken( nextToken ) ;
                    
                    if ( nextToken -> type == RPAR ) {
                        GetToken( currentToken ) ; // ")"
                        // save data
                        outputlist = gOutputlist ;
                        variablelist = gVariablelist ;
                        localVariablelist = gLocalVariablelist ;
                        
                        if ( Statement( value1 ) ) {
                            PeekToken( nextToken ) ;
                            // [
                            if ( nextToken -> type == ELSE ) {
                                GetToken( currentToken ) ; // "else"
                                // save data
                                outputlist_after = gOutputlist ;
                                variablelist_after = gVariablelist ;
                                localVariablelist_after = gLocalVariablelist ;
                                // recover data
                                // and do next statement
                                gOutputlist = outputlist ;
                                gVariablelist = variablelist ;
                                gLocalVariablelist = localVariablelist ;
                                if ( Statement( value2 ) ) {
                                    if ( trueOrFlase.value_bool ) {
                                        // if ( T )
                                        // else
                                        // so recover to after data
                                        gOutputlist = outputlist_after ;
                                        gVariablelist = variablelist_after  ;
                                        gLocalVariablelist = localVariablelist_after ;
                                        value = value2 ;
                                    } // if
                                    else
                                        value = value1 ;
                                    return true ;
                                } // if
                                else
                                    return false ;
                            } // if
                            else {
                                // if ( exp ) is false
                                if ( !trueOrFlase.value_bool ) {
                                    // recover data
                                    gOutputlist = outputlist ;
                                    gVariablelist = variablelist ;
                                    gLocalVariablelist = localVariablelist ;
                                } // if
                            } // else
                            // ]
                            
                            return true;
                        } // if
                    } // if
                } // if
            } // if
        } // else if
        else if ( nextToken -> type == WHILE ) {
            GetToken( currentToken ) ; // "while"
            gWhileHead = gCurrentToken ;
            PeekToken( nextToken ) ;
            bool exp ;
            vector<Value_of_output> outputlist ;
            vector<Variable_of_c> variablelist ;
            vector<Variable_of_c> localVariablelist ;
            
            vector<Value_of_output> outputlist_after ;
            vector<Variable_of_c> variablelist_after ;
            vector<Variable_of_c> localVariablelist_after ;
            
            Value_of_output value_orignal, value_afterwhile, trueOrFalse ;
            value_orignal = value ;
            if ( nextToken -> type == LPAR ) {
                GetToken( currentToken ) ; // "("
                if ( Expression( trueOrFalse ) ) {
                    exp = trueOrFalse.value_bool ;
                    PeekToken( nextToken ) ;
                    
                    outputlist = gOutputlist ;
                    variablelist = gVariablelist ;
                    localVariablelist = gLocalVariablelist ;
                    if ( nextToken -> type == RPAR ) {
                        GetToken( currentToken ) ; // ")"
                        if ( Statement( value_afterwhile ) ) {
                            if ( !exp ) {
                                
                                
                                gOutputlist = outputlist ;
                                gVariablelist = variablelist ;
                                gLocalVariablelist = localVariablelist ;
                                value = value_orignal ;
                                gWhileHead = NULL ;
                                gWhileRear = NULL ;
                                gWhileWalk = NULL ;
                            } // if
                            else {
                                value = value_afterwhile ;
                                gWhileRear = gCurrentToken ;
                                gWhileWalk = gWhileHead ;
                            } // else
                            
                            return true;
                        } // if
                    } // if
                } // if
            } // if
        } // else if
        else if ( nextToken -> type == DO ) {
            GetToken( currentToken ) ; // "do"
            if ( Statement( value ) ) {
                PeekToken( nextToken ) ;
                if ( nextToken -> type == WHILE ) {
                    GetToken( currentToken ) ; // "while"
                    PeekToken( nextToken ) ;
                    if ( nextToken -> type == LPAR ) {
                        GetToken( currentToken ) ; // "("
                        if ( Expression( value ) ) {
                            PeekToken( nextToken ) ;
                            if ( nextToken -> type == RPAR ) {
                                GetToken( currentToken ) ; // ")"
                                PeekToken( nextToken ) ;
                                if ( nextToken -> type == SEMI ) {
                                    GetToken( currentToken ) ;
                                    return true;
                                } // if
                            } // if
                        } // if
                    } // if
                } // if
            } // if
        } // else if

        return false;
    } // Statement()
    
    bool Expression( Value_of_output &value ) {
        // cout << "Expression" << endl ;
        TokenPtr currentToken ;
        TokenPtr nextToken ;
        if ( Basic_Expression( value ) ) {
            PeekToken( nextToken ) ;
            while ( nextToken -> type == COMMA ) {
                GetToken( currentToken ) ; // ","
                if ( !Basic_Expression( value ) )
                    return false;
                else
                    PeekToken( nextToken );
            } // while
            
            return true;
        } // if
        else
            return false;
    } // Expression()
    
    bool Basic_Expression( Value_of_output &value ) {
        // cout << "Basic_Expression" << endl ;
        TokenPtr currentToken ;
        TokenPtr nextToken ;
        PeekToken( nextToken ) ;
        TokenType signtype ;
        if ( nextToken -> type == IDENT ) {
            GetToken( currentToken ) ; // IDENT
            if ( !ExamIsVariableExist( currentToken -> tokenStr ) ) {
                return false ;
            } // if
            
            strcpy( mTmpOfFunctionOrIdent.variableName, currentToken -> tokenStr ) ;
            if ( Rest_Of_Identifier_Started_Basic_Exp( value, currentToken -> tokenStr ) ) {
                if ( mIsFunction )
                    strcpy( mFunctionName, currentToken -> tokenStr ) ;
                return true;
            } // if
        } // if
        // ++a --a, add first, and calculate or assign
        else if ( nextToken -> type == PP || nextToken -> type == MM ) {
            
            GetToken( currentToken ) ; // PP OR MM
            TokenType signIsPPorMM = currentToken -> type ;
            PeekToken( nextToken ) ;
            if ( nextToken -> type == IDENT ) {
                GetToken( currentToken ) ;
                
                if ( Rest_Of_PPMM_Identifier_Started_Basic_Exp( value, signIsPPorMM, currentToken -> tokenStr ) )
                    return true ;
            } // if
        } // else if
        else if ( Sign( signtype ) ) {
            TokenType tmpsign = signtype ;
            while ( Sign( signtype ) ) {
                if ( tmpsign == MINUS && signtype == MINUS )
                    signtype = PLUS ;
                else if ( signtype == PLUS && tmpsign == MINUS )
                    signtype = MINUS ;
                else if ( signtype == MINUS && tmpsign == PLUS )
                    signtype = MINUS ;
                else
                    signtype = PLUS;
            } // while
            
            
            if ( Signed_Unary_Exp( value, signtype ) ) {
                PeekToken( nextToken ) ;
                if ( Romce_and_Romloe( value ) )
                    return true ;
                
            } // if
            
        } // else if
        else {
            if ( nextToken -> type == CONSTANT || nextToken -> type == LPAR ) {
                if ( nextToken -> type == LPAR ) {
                    GetToken( currentToken ) ; // "("
                    if ( Expression( value ) ) {
                        PeekToken( nextToken ) ;
                        if ( nextToken -> type == RPAR ) {
                            GetToken( currentToken ) ; // ")"
                        } // if
                        else
                            return false ;
                    } // if
                    else
                        return false ;
                } // if
                else {
                    GetToken( currentToken ) ; // CONSTANT
                    if ( mIsParameter )
                        strcpy( mListFunctionOrVariableName, currentToken -> tokenStr ) ;
                    strcpy( value.value, currentToken -> tokenStr ) ;

                    bool maybeIsFloat = false ;
                    for ( int i = 0 ; i < strlen( currentToken -> tokenStr ) ; i++ ) {
                        if ( currentToken -> tokenStr[i] == '.' )
                            maybeIsFloat = true ;
                    } // for
                    
                    if ( value.value[0] == '"' ) {
                        for ( int i = 0 ; i < strlen( value.value ) ; i++ )
                            value.value[i] = value.value[i+1] ;
                        int len = strlen( value.value ) ;
                        value.value[len-1] = '\0' ;
                        strcpy( value.value_string, value.value ) ;
                        value.type = STRING ;
                    } // if
                    else if ( value.value[0] == '\'' ) {
                        for ( int i = 0 ; i < strlen( value.value ) ; i++ )
                            value.value[i] = value.value[i+1] ;
                        int len = strlen( value.value ) ;
                        value.value[len-1] = '\0' ;
                        strcpy( value.value_char, value.value ) ;
                        value.type = CHAR ;
                    } // else if
                    else if ( strcmp( currentToken -> tokenStr, "true" ) == 0 ||
                              strcmp( currentToken -> tokenStr, "false" ) == 0 ) {
                        if ( strcmp( currentToken -> tokenStr, "true" ) == 0 )
                            value.value_bool = true ;
                        else
                            value.value_bool = false ;
                        value.type = BOOL ;
                    } // else if
                    else if ( maybeIsFloat ) {
                        value.value_float = atof( currentToken -> tokenStr ) ;
                        value.type = FLOAT ;
                    } // else if
                    else { // int
                        value.value_int = atoi( currentToken -> tokenStr ) ;
                        value.type = INT ;
                    } // else
                } // else
                
                
                if ( Romce_and_Romloe( value ) )
                    return true ;
            } // if
        } // else
        
        // otherwise
        return false;
    } // Basic_Expression()
    
    bool Rest_Of_Identifier_Started_Basic_Exp( Value_of_output &value, Str100 AssignIdentName ) {
        // cout << "Rest_Of_Identifier_Started_Basic_Exp" << endl ;
        TokenPtr nextToken ;
        TokenPtr currentToken ;
        PeekToken( nextToken ) ;
        TokenType assignType ;
        TokenType signIsPPorMM ;
        Value_of_output arrayNum ;
        arrayNum.value_int = -1 ;
        // [
        if ( nextToken -> type == LMPAR ) {
            GetToken( currentToken ) ; // "["
            if ( Expression( arrayNum ) ) {
                PeekToken( nextToken ) ;
                if ( nextToken -> type == RMPAR )
                    GetToken( currentToken ) ;
                else
                    return false ;
            } // if
            else
                return false;
        } // if
        // ]
        
        
        GetVariableValue( AssignIdentName, value, arrayNum.value_int ) ;
        
        PeekToken( nextToken ) ;
        if ( nextToken -> type == LPAR ) {
            // check again if is function name
            if ( !IsFunctionExist( mTmpOfFunctionOrIdent.variableName ) )
                return false ;
            
            GetToken( currentToken ) ; // "("
            // [
            Actual_Parameter_List( value ) ;
            // ]
            if ( gErrorMessage )
                return false ;
            PeekToken( nextToken ) ;
            if ( nextToken -> type == RPAR ) {
                mIsFunction = true ;
                GetToken( currentToken ) ;
            } // if
            else
                return false ;
            PeekToken( nextToken ) ;
            if ( Romce_and_Romloe( value ) )
                return true ;
            else
                return false ;
        } // if
        else if ( Assignment_Operator( assignType ) ) {
            Value_of_output orignalValue ;
            if ( Basic_Expression( value ) ) {
            
                GetVariableValue( AssignIdentName, orignalValue, arrayNum.value_int ) ;
                if ( assignType == TE ) {
                    if ( orignalValue.type == FLOAT ) {
                        if ( value.type == FLOAT ) {
                            orignalValue.value_float = orignalValue.value_float * value.value_float ;
                        } // if
                        else {
                            orignalValue.value_float = orignalValue.value_float * value.value_int ;
                        } // else
                        
                    } // if
                    else if ( orignalValue.type == INT ) {
                        if ( value.type == FLOAT )
                            orignalValue.value_int = orignalValue.value_int * value.value_float ;
                        else
                            orignalValue.value_int = orignalValue.value_int * value.value_int ;
                    } // if

                    AssignVariableValue( AssignIdentName, orignalValue, arrayNum.value_int ) ;
                } // if
                else if ( assignType == DE ) {
                    if ( orignalValue.type == FLOAT ) {
                        if ( value.type == FLOAT ) {
                            orignalValue.value_float = orignalValue.value_float / value.value_float ;
                        } // if
                        else {
                            orignalValue.value_float = orignalValue.value_float / value.value_int ;
                        } // else
                        
                    } // if
                    else if ( orignalValue.type == INT ) {
                        if ( value.type == FLOAT )
                            orignalValue.value_int = orignalValue.value_int / value.value_float ;
                        else
                            orignalValue.value_int = orignalValue.value_int / value.value_int ;
                        
                    } // if
                    
                    AssignVariableValue( AssignIdentName, orignalValue, arrayNum.value_int ) ;
                } // else if
                else if ( assignType == RE ) {
                    orignalValue.value_int = orignalValue.value_int % value.value_int ;
                
                    AssignVariableValue( AssignIdentName, orignalValue, arrayNum.value_int ) ;
                } // else if
                else if ( assignType == PE ) {
                    
                    if ( orignalValue.type == STRING ) {
                        if ( value.type == STRING )
                            strcat( orignalValue.value_string, value.value_string ) ;
                        else
                            strcat( orignalValue.value_string, value.value_char ) ;
                        
                    } // if
                    else {
                        if ( orignalValue.type == FLOAT ) {
                            if ( value.type == FLOAT )
                                orignalValue.value_float = orignalValue.value_float + value.value_float ;
                            else
                                orignalValue.value_float = orignalValue.value_float + value.value_int ;
                            
                        } // if
                        else if ( orignalValue.type == INT ) {
                            if ( value.type == FLOAT )
                                orignalValue.value_int = orignalValue.value_int + value.value_float ;
                            else
                                orignalValue.value_int = orignalValue.value_int + value.value_int ;
                        } // if
                    } // else
                    
                    AssignVariableValue( AssignIdentName, orignalValue, arrayNum.value_int ) ;
                    
                } // else if
                else if ( assignType == ME ) {
                    if ( orignalValue.type == FLOAT ) {
                        if ( value.type == FLOAT ) {
                            orignalValue.value_float = orignalValue.value_float - value.value_float ;
                        } // if
                        else {
                            orignalValue.value_float = orignalValue.value_float - value.value_int ;
                        } // else
                        
                    } // if
                    else if ( orignalValue.type == INT ) {
                        if ( value.type == FLOAT )
                            orignalValue.value_int = orignalValue.value_int - value.value_float ;
                        else
                            orignalValue.value_int = orignalValue.value_int - value.value_int ;
                    } // if
                    
                    AssignVariableValue( AssignIdentName, orignalValue, arrayNum.value_int ) ;
                } // else if
                else {
                    if ( value.type == FLOAT ) {
                        if ( orignalValue.type == FLOAT )
                            orignalValue.value_float = value.value_float ;
                        else if ( orignalValue.type == INT )
                            orignalValue.value_int = value.value_float ;
                        /*
                        else if ( orignalValue.type == BOOL )
                        else if ( orignalValue.type == STRING )
                        else if ( orignalValue.type == CHAR ) {
                        */
                    } // if
                    else if ( value.type == INT ) {
                        if ( orignalValue.type == FLOAT )
                            orignalValue.value_float = value.value_int ;
                        else if ( orignalValue.type == INT )
                            orignalValue.value_int = value.value_int ;
                    } // else if
                    else if ( value.type == BOOL ) {
                        if ( orignalValue.type == BOOL )
                            orignalValue.value_bool = value.value_bool ;
                    // else error
                    } // else if
                    else if ( value.type == STRING ) {
                        strcpy( orignalValue.value_string, value.value_string );
                    } // else if
                    else if ( value.type == CHAR ) {
                        strcpy( orignalValue.value_char, value.value_char ) ;
                    } // else if
                    
                    AssignVariableValue( AssignIdentName, orignalValue, arrayNum.value_int ) ;
                    
                } // else
                
                value = orignalValue ;
             
                return true ;
            } // if
            else
                return false;
        } // if
        else {
            if ( nextToken -> type == PP || nextToken -> type == MM ) {
                GetToken( currentToken ) ;
                PPorMM_Caculate tmp ;
                signIsPPorMM = currentToken -> type ;
                tmp.ppormm = signIsPPorMM ;
                tmp.ppormm = currentToken -> type ;
                tmp.arrayNum = arrayNum.value_int ;
                strcpy( tmp.identName, AssignIdentName ) ;
                mTmpOfPMCaculateList.push_back( tmp ) ;
                PPorMMCaculate() ;
            } // if
            
            if ( Romce_and_Romloe( value ) ) {
                if ( mTmpOfPMCaculateList.size() != 0 ) {
                    PPorMMCaculate() ;
                } // if
                
                return true ;
            } // if
            else
                return false ;
        } // else
        
    } // Rest_Of_Identifier_Started_Basic_Exp()
    
    bool Rest_Of_PPMM_Identifier_Started_Basic_Exp( Value_of_output &value, TokenType signIsPPorMM, Str100 variableName ) {
        
        // cout << "Rest_Of_PPMM_Identifier_Started_Basic_Exp" << endl ;
        TokenPtr nextToken ;
        TokenPtr currentToken ;
        PeekToken( nextToken ) ;
        Value_of_output arrayNum ;
        arrayNum.value_int = -1 ;
        // [
        if ( nextToken -> type == LMPAR ) {
            GetToken( currentToken ) ; // "["
            if ( Expression( arrayNum ) ) {
                PeekToken( nextToken ) ;
                if ( nextToken -> type == RMPAR )
                    GetToken( currentToken ) ;
                else
                    return false ;
                
            } // if
            else
                return false;
        } // if
        // ]
        
        if ( !ExamIsVariableExist( variableName ) )
            return false ;
        GetVariableValue( variableName, value, arrayNum.value_int ) ;
        
        if ( value.type == FLOAT ) {
            if ( signIsPPorMM == PP )
                value.value_float++ ;
            else // if ( signIsPPorMM == MM )
                value.value_float--;
        } // if
        else if ( value.type == INT ) {
            if ( signIsPPorMM == PP )
                value.value_int++ ;
            else // if ( signIsPPorMM == MM )
                value.value_int--;
        } // else if
        
        AssignVariableValue( variableName, value, arrayNum.value_int ) ;
        
        if ( Romce_and_Romloe( value ) )
            return true ;
        else
            return false;
        
    } // Rest_Of_PPMM_Identifier_Started_Basic_Exp()
    
    bool Sign( TokenType &type ) {
        
        // cout << "Sign" << endl ;
        TokenPtr nextToken ;
        TokenPtr currentToken ;
        PeekToken( nextToken ) ;
        if ( nextToken -> type == PLUS ||
             nextToken -> type == MINUS ||
             nextToken -> type == EXC ) {
            GetToken( currentToken ) ;
            type = currentToken -> type ;
            return true;
        } // if
        else
            return false;
    } // Sign()
    
    bool Actual_Parameter_List( Value_of_output &value ) {
        // cout << "Actual_Parameter_List" << endl ;
        TokenPtr nextToken ;
        TokenPtr currentToken ;
        mIsParameter = true ;
        if ( Basic_Expression( value ) ) {
            PeekToken( nextToken ) ;
            while ( nextToken -> type == COMMA ) {
                GetToken( currentToken ) ; // ","
                if ( Basic_Expression( value ) )
                    PeekToken( nextToken ) ;
                else
                    return false;
            } // while
            
            mHaveParameter = true ;
            return true ;
        } // if
        
        mIsParameter = false ;
        return false;
    } // Actual_Parameter_List()
    
    bool Assignment_Operator( TokenType & type ) {
        
        // cout << "Assignment_Operator" << endl ;
        TokenPtr nextToken ;
        TokenPtr currentToken ;
        PeekToken( nextToken ) ;
        if ( nextToken -> type == ASSIGN ||
               nextToken -> type == TE ||
               nextToken -> type == DE ||
               nextToken -> type == RE ||
               nextToken -> type == PE ||
               nextToken -> type == ME ) {
            GetToken( currentToken ) ;
            type = currentToken -> type ;
            return true ;
        } // if
        else
            return false ;
    } // Assignment_Operator()
    
    // rest_of_maybe_conditional_exp_and_rest_of_maybe_logical_OR_exp
    bool Romce_and_Romloe( Value_of_output &value ) {
        vector<Value_of_output> outputlist ;
        vector<Variable_of_c> variablelist ;
        vector<Variable_of_c> localVariablelist ;
        
        vector<Value_of_output> outputlist_after ;
        vector<Variable_of_c> variablelist_after ;
        vector<Variable_of_c> localVariablelist_after ;
        
        // cout << "Romace_and_Romloe" << endl ;
        TokenPtr nextToken ;
        TokenPtr currentToken ;
        Value_of_output value1, value2;
        bool exp ;
        if ( Rest_Of_Maybe_Logical_OR_Exp( value ) ) {
            PeekToken( nextToken ) ;
            // [
            if ( nextToken -> type == QUEST ) {
                GetToken( currentToken ) ; // "?"
                if ( value.type == BOOL )
                    exp = value.value_bool ;
                // else error
                
                // save orignal state
                outputlist = gOutputlist ;
                variablelist = gVariablelist ;
                localVariablelist = gLocalVariablelist ;
                
                if ( Basic_Expression( value1 ) ) {
                    PeekToken( nextToken ) ;
                    
                    if ( nextToken -> type == COLON ) {
                        GetToken( currentToken ) ; // ":"
                        outputlist_after = gOutputlist ;
                        variablelist_after = gVariablelist ;
                        localVariablelist_after = gLocalVariablelist ;
                        
                        // recover data
                        gOutputlist = outputlist ;
                        gVariablelist = variablelist ;
                        gLocalVariablelist = localVariablelist ;
                        if ( Basic_Expression( value2 ) ) {
                            // gVar, gOut, gLocal now is BasicEXP2's answer
                            // if exp is true, useBasicExp1's answer
                            if ( exp ) {
                                gOutputlist = outputlist_after;
                                gVariablelist = variablelist_after;
                                gLocalVariablelist = localVariablelist_after ;
                                value = value1;
                            } // if
                            else
                                value = value2;
                            
                            return true ;
                            
                        } // if
                    } // if
                } // if
            } // if
            // ]
            if ( gErrorMessage )
                return false ;
            
            return true ;
        } // if
        
        return false ;
    } // Romce_and_Romloe()
    
    bool Rest_Of_Maybe_Logical_OR_Exp( Value_of_output &value ) {
        Value_of_output value2 ;
        // cout << "Rest_Of_Maybe_Logical_OR_Exp" << endl ;
        TokenPtr nextToken ;
        TokenPtr currentToken ;
        bool cmp1, cmp2 ;
        if ( Rest_Of_Maybe_Logical_AND_Exp( value ) ) {
            PeekToken( nextToken ) ;
            while ( nextToken -> type == OR ) {
                GetToken( currentToken ) ; // "||"
                if ( !Maybe_Logical_AND_Exp( value2 ) )
                    return false;
                else {
                    
                    cmp1 = value.value_bool ;
                    cmp2 = value2.value_bool ;
                    if ( cmp1 || cmp2 )
                        value.value_bool = true ;
                    else
                        value.value_bool = false ;
                    
                    PeekToken( nextToken ) ;
                    
                } // else
            } // while
            
            return true;
        } // if
        
        return false ;
    } // Rest_Of_Maybe_Logical_OR_Exp()
    
    bool Maybe_Logical_AND_Exp( Value_of_output &value ) {
        Value_of_output value2 ;
        // cout << "Maybe_Logical_AND_Exp" << endl ;
        TokenPtr nextToken ;
        TokenPtr currentToken ;
        bool cmp1, cmp2 ;
        if ( Maybe_bit_OR_exp( value ) ) {
            PeekToken( nextToken ) ;
            while ( nextToken -> type == AND ) {
                GetToken( currentToken ) ; // "&&"
                if ( !Maybe_bit_OR_exp( value2 ) )
                    return false;
                else {
                    cmp1 = value.value_bool ;
                    cmp2 = value2.value_bool ;
                    if ( cmp1 && cmp2 )
                        value.value_bool = true;
                    else
                        value.value_bool = false;
                    
                    PeekToken( nextToken ) ;
                } // else
            } // while
            
            return true ;
        } // if
        
        return false ;
    } // Maybe_Logical_AND_Exp()
    
    bool Rest_Of_Maybe_Logical_AND_Exp( Value_of_output &value ) {
        Value_of_output value2 ;
        // cout << "Rest_Of_Maybe_Logical_AND_Exp" << endl ;
        TokenPtr nextToken ;
        TokenPtr currentToken ;
        bool cmp1, cmp2 ;
        if ( Rest_Of_Maybe_Bit_OR_Exp( value ) ) {
            PeekToken( nextToken ) ;
            while ( nextToken -> type == AND ) {
                GetToken( currentToken ) ; // "&&"
                if ( !Maybe_bit_OR_exp( value2 ) )
                    return false;
                else {
                    cmp1 = value.value_bool ;
                    cmp2 = value2.value_bool ;
                    if ( cmp1 && cmp2 )
                        value.value_bool = true;
                    else
                        value.value_bool = false;
                    
                    PeekToken( nextToken ) ;
                } // else
            } // while
            
            return true ;
        } // if
        
        return false ;
    } // Rest_Of_Maybe_Logical_AND_Exp()
    
    bool Maybe_bit_OR_exp( Value_of_output &value ) {
        Value_of_output value2 ;
        // cout << "Maybe_bit_OR_exp" << endl ;
        TokenPtr nextToken ;
        TokenPtr currentToken ;
        if ( Maybe_Bit_Ex_OR_Exp( value ) ) {
            PeekToken( nextToken ) ;
            while ( nextToken -> type == VER ) {
                GetToken( currentToken ) ; // "|"
                if ( !Maybe_Bit_Ex_OR_Exp( value2 ) )
                    return false ;
                else
                    PeekToken( nextToken ) ;
            } // while
            
            return true;
        } // if
        
        return false;
    } // Maybe_bit_OR_exp()
    
    bool Rest_Of_Maybe_Bit_OR_Exp( Value_of_output &value ) {
        Value_of_output value2 ;
        // cout << "Rest_Of_Maybe_Bit_OR_Exp" << endl ;
        TokenPtr nextToken ;
        TokenPtr currentToken ;
        if ( Rest_Of_Maybe_Bit_Ex_OR_Exp( value ) ) {
            PeekToken( nextToken ) ;
            while ( nextToken -> type == VER ) {
                GetToken( currentToken ) ; // "|"
                if ( !Maybe_Bit_Ex_OR_Exp( value2 ) )
                    return false;
                else {
                    PeekToken( nextToken ) ;
          
                } // else
            } // while
            
            return true ;
        } // if
        
        return false;
    } // Rest_Of_Maybe_Bit_OR_Exp()
    
    bool Maybe_Bit_Ex_OR_Exp( Value_of_output &value ) {
        Value_of_output value2 ;
        // cout << "Maybe_Bit_Ex_OR_Exp" << endl ;
        TokenPtr nextToken ;
        TokenPtr currentToken ;
        if ( Maybe_Bit_AND_Exp( value ) ) {
            PeekToken( nextToken ) ;
            while ( nextToken -> type == EXP ) {
                GetToken( currentToken ) ; // "^"
                if ( !Maybe_Bit_AND_Exp( value2 ) )
                    return false ;
                else {
            
                    PeekToken( nextToken );
                } // else
            } // while
            
            return true ;
        } // if
        
        return false;
    } // Maybe_Bit_Ex_OR_Exp()
    
    bool Rest_Of_Maybe_Bit_Ex_OR_Exp( Value_of_output &value ) {
        Value_of_output value2 ;
        // cout << "Rest_Of_Maybe_Bit_Ex_OR_Exp" << endl ;
        TokenPtr nextToken ;
        TokenPtr currentToken ;
        if ( Rest_Of_Maybe_Bit_AND_Exp( value ) ) {
            PeekToken( nextToken ) ;
            while ( nextToken -> type == EXP ) {
                GetToken( currentToken ) ; // "^"
                if ( !Maybe_Bit_AND_Exp( value2 ) )
                    return false ;
                else {
            
                    PeekToken( nextToken );
                } // else
            } // while
            
            return true ;
        } // if
        
        return false;
    } // Rest_Of_Maybe_Bit_Ex_OR_Exp()
    
    bool Maybe_Bit_AND_Exp( Value_of_output &value ) {
        
        // cout << "Maybe_Bit_AND_Exp" << endl ;
        TokenPtr nextToken ;
        TokenPtr currentToken ;
        if ( Maybe_Equality_exp( value ) ) {
            PeekToken( nextToken ) ;
            while ( nextToken -> type ==  AMP ) {
                GetToken( currentToken ) ; // "&"
                if ( !Maybe_Equality_exp( value ) )
                    return false ;
                else
                    PeekToken( nextToken );
            } // while
            
            return true ;
        } // if
        
        return false;
    } // Maybe_Bit_AND_Exp()
    
    bool Rest_Of_Maybe_Bit_AND_Exp( Value_of_output &value ) {
        
        // cout << "Rest_Of_Maybe_Bit_AND_Exp" << endl ;
        TokenPtr nextToken ;
        TokenPtr currentToken ;
        if ( Rest_Of_Maybe_Equality_Exp( value ) ) {
            PeekToken( nextToken ) ;
            while ( nextToken -> type ==  AMP ) {
                GetToken( currentToken ) ; // "&"
                if ( !Rest_Of_Maybe_Equality_Exp( value ) )
                    return false ;
                else
                    PeekToken( nextToken ) ;
            } // while
            
            return true ;
        } // if
        
        
        return false;
    } // Rest_Of_Maybe_Bit_AND_Exp()
    
    bool Maybe_Equality_exp( Value_of_output &value ) {
        Value_of_output value2 ;
        // cout << "Maybe_Equality_exp" << endl ;
        TokenPtr nextToken ;
        TokenPtr currentToken ;
        float cmp1, cmp2;
        if ( Maybe_Relational_Exp( value ) ) {
            PeekToken( nextToken ) ;
            while ( nextToken -> type ==  EQUAL || nextToken -> type == NOT_EQUAL ) {
                GetToken( currentToken ) ; // "==" "!="
                if ( !Maybe_Relational_Exp( value2 ) )
                    return false ;
                else {
                    if ( value.type == FLOAT )
                        cmp1 = value.value_float ;
                    else if ( value.type == INT )
                        cmp1 = value.value_int ;
                    
                    if ( value2.type == FLOAT )
                        cmp2 = value2.value_float ;
                    else if ( value2.type == INT )
                        cmp2 = value2.value_int ;
                    // else error
                    
                    // trun into bool
                    value.type = BOOL ;
                    if ( currentToken -> type == EQUAL ) {
                        if ( cmp1 == cmp2 )
                            value.value_bool = true ;
                        else
                            value.value_bool = false ;
                    } // if
                    else {
                        if ( cmp1 != cmp2 )
                            value.value_bool = true ;
                        else
                            value.value_bool = false ;
                    } // else
                    
                    PeekToken( nextToken );
                    
                } // else
            } // while
            
            return true ;
        } // if
        
        
        return false;
    } // Maybe_Equality_exp()
    
    bool Rest_Of_Maybe_Equality_Exp( Value_of_output &value ) {
        Value_of_output value2 ;
        // cout << "Rest_Of_Maybe_Equality_Exp" << endl ;
        TokenPtr nextToken ;
        TokenPtr currentToken ;
        float cmp1, cmp2 ;
        if ( Rest_Of_Maybe_Relational_Exp( value ) ) {
            PeekToken( nextToken ) ;
            while ( nextToken -> type ==  EQUAL || nextToken -> type == NOT_EQUAL ) {
                GetToken( currentToken ) ; // "==" "!="
                if ( !Maybe_Relational_Exp( value2 ) )
                    return false ;
                else {
                    if ( value.type == FLOAT )
                        cmp1 = value.value_float ;
                    else if ( value.type == INT )
                        cmp1 = value.value_int ;
                    
                    if ( value2.type == FLOAT )
                        cmp2 = value2.value_float ;
                    else if ( value2.type == INT )
                        cmp2 = value2.value_int ;
                    // else error
                    
                    // trun into bool
                    value.type = BOOL ;
                    if ( currentToken -> type == EQUAL ) {
                        if ( cmp1 == cmp2 )
                            value.value_bool = true ;
                        else
                            value.value_bool = false ;
                    } // if
                    else {
                        if ( cmp1 != cmp2 )
                            value.value_bool = true ;
                        else
                            value.value_bool = false ;
                    } // else
                    
                    PeekToken( nextToken );
                    
                } // else
            } // while
            
            return true ;
        } // if
        
        return false;
    } // Rest_Of_Maybe_Equality_Exp()
    
    bool Maybe_Relational_Exp( Value_of_output &value ) {
        Value_of_output value2 ;
        // cout << "Maybe_Relational_Exp" << endl ;
        TokenPtr nextToken ;
        TokenPtr currentToken ;
        float cmp1, cmp2 ;
        if ( Maybe_Shift_Exp( value ) ) {
            PeekToken( nextToken ) ;
            while ( nextToken -> type ==  LESS || nextToken -> type == GREATER ||
                   nextToken -> type == LE || nextToken -> type == GE ) {
                GetToken( currentToken ) ; // ">" ">=" "<" "<="
                if ( !Maybe_Shift_Exp( value2 ) )
                    return false ;
                else {
                    if ( value.type == INT )
                        cmp1 = value.value_int ;
                    else if ( value.type == FLOAT )
                        cmp1 = value.value_float ;
                    
                    if ( value2.type == INT )
                        cmp2 = value2.value_int ;
                    else if ( value2.type == FLOAT )
                        cmp2 = value2.value_float ;
                    // else is error
                    
                    // turn into bool
                    value.type = BOOL ;
                    
                    if ( currentToken -> type == LESS ) {
                        if ( cmp1 < cmp2 )
                            value.value_bool = true ;
                        else
                            value.value_bool = false ;
                    } // if
                    else if ( currentToken -> type == LE ) {
                        if ( cmp1 <= cmp2 )
                            value.value_bool = true ;
                        else
                            value.value_bool = false ;
                    } // if
                    else if ( currentToken -> type == GREATER ) {
                        if ( cmp1 > cmp2 )
                            value.value_bool = true ;
                        else
                            value.value_bool = false ;
                    } // if
                    else if ( currentToken -> type == GE ) {
                        if ( cmp1 >= cmp2 )
                            value.value_bool = true ;
                        else
                            value.value_bool = false ;
                    } // if
                    
                    PeekToken( nextToken ) ;
                } // else
            } // while
            
            return true ;
        } // if
        
        return false;
    } // Maybe_Relational_Exp()
    
    bool Rest_Of_Maybe_Relational_Exp( Value_of_output &value ) {
        Value_of_output value2 ;
        // cout << "Rest_Of_Maybe_Relational_Exp" << endl ;
        TokenPtr nextToken ;
        TokenPtr currentToken ;
        float cmp1, cmp2 ;
        if ( Rest_Of_Maybe_Shift_Exp( value ) ) {
            PeekToken( nextToken ) ;
            while ( nextToken -> type ==  LESS || nextToken -> type == GREATER ||
                   nextToken -> type == LE || nextToken -> type == GE ) {
                GetToken( currentToken ) ; // ">" ">=" "<" "<="
                if ( !Maybe_Shift_Exp( value2 ) )
                    return false ;
                else {
                    if ( value.type == INT )
                        cmp1 = value.value_int ;
                    else if ( value.type == FLOAT )
                        cmp1 = value.value_float ;
                    
                    if ( value2.type == INT )
                        cmp2 = value2.value_int ;
                    else if ( value2.type == FLOAT )
                        cmp2 = value2.value_float ;
                    // else is error
                    
                    // turn into bool
                    value.type = BOOL ;
                    
                    if ( currentToken -> type == LESS ) {
                        if ( cmp1 < cmp2 )
                            value.value_bool = true ;
                        else
                            value.value_bool = false ;
                    } // if
                    else if ( currentToken -> type == LE ) {
                        if ( cmp1 <= cmp2 )
                            value.value_bool = true ;
                        else
                            value.value_bool = false ;
                    } // if
                    else if ( currentToken -> type == GREATER ) {
                        if ( cmp1 > cmp2 )
                            value.value_bool = true ;
                        else
                            value.value_bool = false ;
                    } // if
                    else if ( currentToken -> type == GE ) {
                        if ( cmp1 >= cmp2 )
                            value.value_bool = true ;
                        else
                            value.value_bool = false ;
                    } // if
                    
                    PeekToken( nextToken ) ;
                } // else
            } // while
            
            return true ;
        } // if
        
        
        return false ;
    } // Rest_Of_Maybe_Relational_Exp()
    
    bool Maybe_Shift_Exp( Value_of_output &value ) {
        Value_of_output value2 ;
        bool nowIsCout = false ;
        // cout << "Maybe_Shift_Exp" << endl ;
        TokenPtr nextToken ;
        TokenPtr currentToken ;
        if ( strcmp( value.name, "cout" ) == 0 ) {
            gIsCout = true ;
            nowIsCout = true ;
        } // if
        
        if ( Maybe_Additive_Exp( value ) ) {
            PeekToken( nextToken ) ;
            if ( nowIsCout && nextToken -> type != LS )
                return false;
            while ( nextToken -> type ==  LS || nextToken -> type == RS ) {
      
                GetToken( currentToken ) ; // ">>" "<<"
                
                
                if ( !Maybe_Additive_Exp( value2 ) )
                    return false ;
                else {
                    // value, value2 must be int
                    // else is error
                    if ( nowIsCout ) {
                        
                        gOutputlist.push_back( value2 ) ;
                    } // if
                    else {
                        if ( currentToken -> type == LS ) {
                            value.value_int = value.value_int << value2.value_int ;
                        } // if
                        else
                            value.value_int = value.value_int >> value2.value_int ;
                    } // else
                    
                    PeekToken( nextToken );
                } // else
                
                
            } // while
            
            return true ;
        } // if
        
        return false;
    } // Maybe_Shift_Exp()
    
    bool Rest_Of_Maybe_Shift_Exp( Value_of_output &value ) {
        Value_of_output value2 ;
        // cout << "Rest_Of_Maybe_Shift_Exp" << endl ;
        TokenPtr nextToken ;
        TokenPtr currentToken ;
        bool nowIsCout = false;
        if ( strcmp( value.name, "cout" ) == 0 ) {
            gIsCout = true ;
            nowIsCout = true ;
        } // if
        
        if ( Rest_Of_Maybe_Additive_Exp( value ) ) {
            PeekToken( nextToken ) ;
            if ( nowIsCout && nextToken -> type != LS )
                return false;
            while ( nextToken -> type ==  LS || nextToken -> type == RS ) {

                GetToken( currentToken ) ; // ">>" "<<"

                
                if ( !Maybe_Additive_Exp( value2 ) )
                    return false ;
                else {
                    // value, value2 must be int
                    // else is error
                    if ( nowIsCout ) {
                        gOutputlist.push_back( value2 ) ;
                    } // if
                    else {
                        if ( currentToken -> type == LS ) {
                            value.value_int = value.value_int << value2.value_int ;
                        } // if
                        else
                            value.value_int = value.value_int >> value2.value_int ;
                    } // else
                    
                    PeekToken( nextToken );
                } // else
              
            } // while
            
            return true ;
        } // if
        
        return false;
    } // Rest_Of_Maybe_Shift_Exp()
    
    bool Maybe_Additive_Exp( Value_of_output &value ) {
        Value_of_output value2 ;
        // cout << "Maybe_Additive_Exp" << endl ;
        TokenPtr nextToken ;
        TokenPtr currentToken ;
        if ( Maybe_Mult_Exp( value ) ) {
            PeekToken( nextToken ) ;
            while ( nextToken -> type ==  PLUS || nextToken -> type == MINUS ) {
                GetToken( currentToken ) ; // "+" "-"
                
                if ( !Maybe_Mult_Exp( value2 ) )
                    return false ;
                else {
                    PeekToken( nextToken );
                    if ( value.type == STRING || value.type == CHAR ) {
                        if ( value.type == STRING ) {
                            if ( value2.type == STRING )
                                strcat( value.value_string, value2.value_string ) ;
                            else
                                strcat( value.value_string, value2.value_char ) ;
                        } // if
                        
                        if ( value.type == CHAR ) {
                            value.type = STRING ; // true in to STRING
                            strcpy( value.value_string, value.value_char ) ;
                            if ( value2.type == STRING )
                                strcat( value.value_string, value2.value_string ) ;
                            else
                                strcat( value.value_string, value2.value_char ) ;
                        } // if
                    } // if
                    else {
                        if ( currentToken -> type == PLUS ) {
                            if ( value.type == FLOAT ) {
                                if ( value2.type == FLOAT )
                                    value.value_float = value.value_float + value2.value_float ;
                                else
                                    value.value_float = value.value_float + value2.value_int ;
                            } // if
                            else { // if ( value.type == INT ) {
                                if ( value2.type == FLOAT )
                                    value.value_int = value.value_int + value2.value_float ;
                                else
                                    value.value_int = value.value_int + value2.value_int ;
                            } // else
                        } // if
                        else { // MM
                            if ( value.type == FLOAT ) {
                                if ( value2.type == FLOAT )
                                    value.value_float = value.value_float - value2.value_float ;
                                else
                                    value.value_float = value.value_float - value2.value_int ;
                            } // if
                            else { // if ( value.type == INT ) {
                                if ( value2.type == FLOAT ) {
                                    // trun into float
                                    value.type = FLOAT ;
                                    value.value_float = value.value_int - value2.value_float ;
                                } // if
                                else
                                    value.value_int = value.value_int - value2.value_int ;
                                
                            } // else
                        } // else
                    } // else
                } // else
            } // while
            
            return true ;
        } // if
        
        return false;
    } // Maybe_Additive_Exp()
    
    bool Rest_Of_Maybe_Additive_Exp( Value_of_output &value ) {
        Value_of_output value2 ;
        // cout << "Rest_Of_Maybe_Additive_Exp" << endl ;
        TokenPtr nextToken ;
        TokenPtr currentToken ;
        if ( Rest_Of_Maybe_Mult_Exp( value ) ) {
            PeekToken( nextToken ) ;
            while ( nextToken -> type ==  PLUS || nextToken -> type == MINUS ) {
                GetToken( currentToken ) ; // "+" "-"
                
                if ( !Maybe_Mult_Exp( value2 ) )
                    return false ;
                else {
                    PeekToken( nextToken );
                    if ( value.type == STRING || value.type == CHAR ) {
                    
                        if ( value.type == STRING ) {
                            if ( value2.type == STRING )
                                strcat( value.value_string, value2.value_string ) ;
                            else
                                strcat( value.value_string, value2.value_char ) ;
                        } // if
                        
                        if ( value.type == CHAR ) {
                            value.type = STRING ; // true in to STRING
                            strcpy( value.value_string, value.value_char ) ;
                            if ( value2.type == STRING )
                                strcat( value.value_string, value2.value_string ) ;
                            else
                                strcat( value.value_string, value2.value_char ) ;
                        } // if
                      
                    } // if
                    else {
                        if ( currentToken -> type == PLUS ) {
                            if ( value.type == FLOAT ) {
                                if ( value2.type == FLOAT )
                                    value.value_float = value.value_float + value2.value_float ;
                                else
                                    value.value_float = value.value_float + value2.value_int ;
                            } // if
                            else { // if ( value.type == INT ) {
                                if ( value2.type == FLOAT )
                                    value.value_int = value.value_int + value2.value_float ;
                                else {
                                    value.value_int = value.value_int + value2.value_int ;
                                } // else
                            } // else
                        } // if
                        else { // MM
                            if ( value.type == FLOAT ) {
                                if ( value2.type == FLOAT )
                                    value.value_float = value.value_float - value2.value_float ;
                                else
                                    value.value_float = value.value_float - value2.value_int ;
                            } // if
                            else { // if ( value.type == INT ) {
                                if ( value2.type == FLOAT ) {
                                    // trun into float
                                    value.type = FLOAT ;
                                    value.value_float = value.value_int - value2.value_float ;
                                } // if
                                else
                                    value.value_int = value.value_int - value2.value_int ;
                            
                            } // else
                        } // else
                        
                    } // else
                } // else
            } // while
            
            return true ;
        } // if
        
        return false;
    } // Rest_Of_Maybe_Additive_Exp()
    
    bool Maybe_Mult_Exp( Value_of_output &value ) {
        TokenPtr nextToken ;
        // cout << "Maybe_Mult_Exp" << endl ;
        if ( Unary_Exp( value ) ) {
            PeekToken( nextToken ) ;
            if ( Rest_Of_Maybe_Mult_Exp( value ) )
                return true ;
            
        } // if
        
        return false;
    } // Maybe_Mult_Exp()
    
    bool Rest_Of_Maybe_Mult_Exp( Value_of_output &value ) {
        Value_of_output value2 ;
        // cout << "Rest_Of_Maybe_Mult_Exp" << endl ;
        // * cloud be empty *
        TokenPtr nextToken ;
        TokenPtr currentToken ;
        PeekToken( nextToken ) ;
        while ( nextToken -> type == MUTI || nextToken -> type == DIVI ||
               nextToken -> type == PERCENT ) {
            
            GetToken( currentToken ) ; // "*" "/" "%"
            if ( !Unary_Exp( value2 ) )
                return false;
            else {
                PeekToken( nextToken );
                /*
                cout << value.name << " : ";
                cout << value.value_int << " ";
                cout << value2.name << " : ";
                cout << value2.value_int << endl ;
   */
                if ( currentToken -> type == MUTI ) {
                    if ( value.type == FLOAT ) {
                        if ( value2.type == FLOAT )
                            value.value_float = value.value_float * value2.value_float ;
                        else
                            value.value_float = value.value_float * value2.value_int ;
                    } // if
                    else { // if ( value.type == INT ) {
                        if ( value2.type == FLOAT ) {
                            // turn in to float
                            value.type = FLOAT ;
                            value.value_float = value.value_int * value2.value_float ;
                        } // if
                        else
                            value.value_int = value.value_int * value2.value_int ;
                    } // else
                } // if
                else if ( currentToken -> type == DIVI ) {
                    if ( value.type == FLOAT ) {
                        if ( value2.type == FLOAT )
                            value.value_float = value.value_float / value2.value_float ;
                        else
                            value.value_float = value.value_float / value2.value_int ;
                    } // if
                    else { // if ( value.type == INT ) {
                        if ( value2.type == FLOAT ) {
                            // turn in to float
                            value.type = FLOAT ;
                            value.value_float = value.value_int / value2.value_float ;
                        } // if
                        else
                            value.value_int = value.value_int / value2.value_int ;
                    } // else
                } // else if
                else { // PERCENT
                    value.value_int = value.value_int % value2.value_int ;
                    // else error
                } // else
                
            } // else
            
        } // while
        
        return true;
    } // Rest_Of_Maybe_Mult_Exp()
    
    bool Unary_Exp( Value_of_output &value ) {
        TokenType signtype ;
        // cout << "Unary_Exp" << endl ;
        TokenPtr nextToken ;
        TokenPtr currentToken ;
        PeekToken( nextToken ) ;
        TokenType signIsPPorMM ;
        Value_of_output arrayNum ;
        arrayNum.value_int = -1 ;
        bool porm = false;
        if ( Sign( signtype ) ) {
            TokenType tmpsign = signtype ;
            while ( Sign( signtype ) ) {
                if ( tmpsign == MINUS && signtype == MINUS )
                    signtype = PLUS ;
                else if ( signtype == PLUS && tmpsign == MINUS )
                    signtype = MINUS ;
                else if ( signtype == MINUS && tmpsign == PLUS )
                    signtype = MINUS ;
                else
                    signtype = PLUS;
            } // while
            
            if ( Signed_Unary_Exp( value, signtype ) )
                return true ;
        } // if
        else if ( Unsigned_Unary_Exp( value ) ) {
            return true ;
        } // else if
        else {
            // [
            if ( nextToken -> type == PP || nextToken -> type == MM ) {
                GetToken( currentToken ) ;
                signIsPPorMM = currentToken -> type ;
                porm = true ;
            } // if
            // ]
            PeekToken( nextToken ) ;
            if ( nextToken -> type == IDENT ) {
                GetToken( currentToken ) ; // IDENT
                Str100 variableName ;
                strcpy( variableName, currentToken -> tokenStr ) ;
                if ( !ExamIsVariableExist( currentToken -> tokenStr ) )
                    return false ;
                
                PeekToken( nextToken ) ;
                // [
                if ( nextToken -> type == LMPAR ) {
                    GetToken( currentToken ) ; // "["
                    if ( Expression( arrayNum ) ) {
                        PeekToken( nextToken ) ;
                        if ( nextToken -> type == RMPAR ) {
                            GetToken( currentToken ) ; // "]"
                        } // if
                        else
                            return false ;
                    } // if
                    else
                        return false;
                } // if
                // ]
                
                GetVariableValue( variableName, value, arrayNum.value_int ) ;
              
                // ++a --a
                if ( porm ) {
                    if ( value.type == FLOAT ) {
                        if ( signIsPPorMM == PP )
                            value.value_float++ ;
                        else // if ( signIsPPorMM == MM )
                            value.value_float--;
                    } // if
                    else if ( value.type == INT ) {
                        if ( signIsPPorMM == PP )
                            value.value_int++ ;
                        else // if ( signIsPPorMM == MM )
                            value.value_int--;
                    } // else if
                    // else error
                    
                    AssignVariableValue( variableName, value, arrayNum.value_int ) ;
                    
                } // if

                
                return true ;
            } // if
        } // else
        
        return false;
    } // Unary_Exp()
    
    bool Signed_Unary_Exp( Value_of_output &value, TokenType sign ) {
        
        // cout << "Signed_Unary_Exp" << endl ;
        TokenPtr nextToken ;
        TokenPtr currentToken ;
        PeekToken( nextToken ) ;
        Value_of_output arrayNum ;
        Str100 variableName ;
        arrayNum.value_int = -1 ;
        if ( nextToken -> type == IDENT ) {
            GetToken( currentToken ) ; // IDENT
            strcpy( variableName, currentToken ->tokenStr ) ;
            if ( !ExamIsVariableExist( currentToken -> tokenStr ) )
                return false ;
            // [
            // function
            PeekToken( nextToken ) ;
            if ( nextToken -> type == LPAR ) {
                GetToken( currentToken ) ; // "("
                // [
                Actual_Parameter_List( value ) ;
                // ]
                if ( gErrorMessage )
                    return false;
                
                PeekToken( nextToken ) ;
                if ( nextToken -> type == RPAR ) {
                    GetToken( currentToken ) ;
                   
                } // if
            } // if
            // arrayNum
            else if ( nextToken -> type == LMPAR ) {
                GetToken( currentToken ) ; // "["
                if ( Expression( arrayNum ) ) {
                    PeekToken( nextToken ) ;
                    if ( nextToken -> type == RMPAR ) {
                        GetToken( currentToken ) ;
                       
                    } // if
                } // if
            } // else if
            // ]
            
            GetVariableValue( variableName, value, arrayNum.value_int ) ;
            
            if ( sign == MINUS ) {
                if ( value.type == FLOAT )
                    value.value_float = value.value_float * -1 ;
                else if ( value.type == INT )
                    value.value_int = value.value_int * -1 ;
                // else error
            } // if
            else if ( sign == EXC ) {
                if ( value.type == BOOL )
                    value.value_bool = !value.value_bool ;
                // else error
            } // else if
            
            PeekToken( nextToken ) ;
     
            return true ;
        } // if
        else if ( nextToken -> type == CONSTANT ) {
            GetToken( currentToken ) ;
            strcpy( value.value, currentToken -> tokenStr ) ;
            bool maybeIsFloat = false ;
            for ( int i = 0 ; i < strlen( currentToken -> tokenStr ) ; i++ ) {
                if ( currentToken -> tokenStr[i] == '.' )
                    maybeIsFloat = true ;
            } // for
            
            if ( value.value[0] == '"' ) {
                for ( int i = 0 ; i < strlen( value.value ) ; i++ )
                    value.value[i] = value.value[i+1] ;
                int len = strlen( value.value ) ;
                value.value[len-1] = '\0' ;
                strcpy( value.value_string, value.value ) ;
                value.type = STRING ;
            } // if
            else if ( value.value[0] == '\'' ) {
                for ( int i = 0 ; i < strlen( value.value ) ; i++ )
                    value.value[i] = value.value[i+1] ;
                int len = strlen( value.value ) ;
                value.value[len-1] = '\0' ;
                strcpy( value.value_char, value.value ) ;
                value.type = CHAR ;
            } // else if
            else if ( strcmp( currentToken -> tokenStr, "true" ) == 0 ||
                          strcmp( currentToken -> tokenStr, "false" ) == 0 ) {
                if ( strcmp( currentToken -> tokenStr, "true" ) == 0 )
                    value.value_bool = true ;
                else
                    value.value_bool = false ;
                
                if ( sign == EXC )
                    value.value_bool = !value.value_bool ;
                value.type = BOOL ;
            } // else if
            else if ( maybeIsFloat ) {
                value.value_float = atof( currentToken -> tokenStr ) ;
                value.type = FLOAT ;
                if ( sign == MINUS )
                    value.value_float = value.value_float * -1;
            } // else if
            else { // int
                value.value_int = atoi( currentToken -> tokenStr ) ;
                value.type = INT ;
                
                if ( sign == MINUS )
                    value.value_int = value.value_int * -1;
            } // else
            
            return true ;
        } // else if
        else if ( nextToken -> type == LPAR ) {
            GetToken( currentToken ) ; // "("
            if ( Expression( value ) ) {
                PeekToken( nextToken ) ;
                if ( nextToken -> type == RPAR ) {
                    GetToken( currentToken ) ;
                    if ( value.type == INT ) {
                        if ( sign == MINUS )
                            value.value_int = value.value_int * -1;
                    } // if
                    else if ( value.type == FLOAT ) {
                        if ( sign == MINUS )
                            value.value_float = value.value_float * -1;
                    } // else if
                    else if ( value.type == BOOL ) {
                        if ( sign == EXC )
                            value.value_bool = !value.value_bool ;
                    } // else if
                    
                    return true ;
                } // if
            } // if
        } // else if
        
        return false;
    } // Signed_Unary_Exp()
    
    bool Unsigned_Unary_Exp( Value_of_output &value ) {
        
        // cout << "Unsigned_Unary_Exp" << endl ;
        TokenPtr nextToken ;
        TokenPtr currentToken ;
        Str100 functionName ;
        PeekToken( nextToken ) ;
        PPorMM_Caculate tmp ;
        Value_of_output arrayNum ;
        arrayNum.value_int = -1 ;
        if ( nextToken -> type == IDENT ) {
            GetToken( currentToken ) ; // IDENT
            Str100 variableName ;
            strcpy( variableName, currentToken -> tokenStr ) ;
            
            strcpy( tmp.identName, currentToken -> tokenStr ) ;
            
            if ( !ExamIsVariableExist( currentToken -> tokenStr ) )
                return false ;

            PeekToken( nextToken ) ;
            
            strcpy( functionName, currentToken -> tokenStr ) ;
            // [
            if ( nextToken -> type == LPAR ) {
                GetToken( currentToken ) ; // "("
                // [
                Actual_Parameter_List( value ) ;
                // ]
                if ( gErrorMessage )
                    return false ;
                
                PeekToken( nextToken ) ;
                if ( nextToken -> type == RPAR ) {
                    GetToken( currentToken ) ;
                } // if
            } // if
            else if ( nextToken -> type == LMPAR ) {
                GetToken( currentToken ) ; // "["
                if ( Expression( arrayNum ) ) {
                    PeekToken( nextToken ) ;
                    if ( nextToken -> type == RMPAR ) {
                        GetToken( currentToken ) ; // "]"
                    } // if
                    else
                        return false ;
                } // if
                else
                    return false;
            } // else if
            // ]
            PeekToken( nextToken ) ;
            GetVariableValue( variableName, value, arrayNum.value_int );
            // [
            // a++ caculate in the end of statement
            
            if ( nextToken -> type == PP || nextToken -> type == MM ) {
                GetToken( currentToken ) ;
                tmp.ppormm = currentToken -> type ;
                tmp.arrayNum = arrayNum.value_int ;
                mTmpOfPMCaculateList.push_back( tmp ) ;
                
                PPorMMCaculate() ;
            } // if
            // ]
            return true ;
        } // if
        else if ( nextToken -> type == CONSTANT ) {
            GetToken( currentToken ) ;
            strcpy( value.value, currentToken -> tokenStr ) ;
            bool maybeIsFloat = false ;
            for ( int i = 0 ; i < strlen( currentToken -> tokenStr ) ; i++ ) {
                if ( currentToken -> tokenStr[i] == '.' )
                    maybeIsFloat = true ;
            } // for
            
            if ( value.value[0] == '"' ) {
                for ( int i = 0 ; i < strlen( value.value ) ; i++ )
                    value.value[i] = value.value[i+1] ;
                int len = strlen( value.value ) ;
                value.value[len-1] = '\0' ;
                strcpy( value.value_string, value.value ) ;
                value.type = STRING ;
            } // if
            else if ( value.value[0] == '\'' ) {
                for ( int i = 0 ; i < strlen( value.value ) ; i++ )
                    value.value[i] = value.value[i+1] ;
                int len = strlen( value.value ) ;
                value.value[len-1] = '\0' ;
                strcpy( value.value_char, value.value ) ;
                value.type = CHAR ;
            } // else if
            else if ( strcmp( currentToken -> tokenStr, "true" ) == 0 ||
                         strcmp( currentToken -> tokenStr, "false" ) == 0 ) {
                if ( strcmp( currentToken -> tokenStr, "true" ) == 0 )
                    value.value_bool = true ;
                else
                    value.value_bool = false ;
                value.type = BOOL ;
            } // else if
            else if ( maybeIsFloat ) {
                value.value_float = atof( currentToken -> tokenStr ) ;
                value.type = FLOAT ;
            } // else if
            else { // int
                value.value_int = atoi( currentToken -> tokenStr ) ;
                value.type = INT ;
            } // else
            
            return true ;
        } // else if
        else if ( nextToken -> type == LPAR ) {
            GetToken( currentToken ) ; // "("
            if ( Expression( value ) ) {
                PeekToken( nextToken ) ;
                if ( nextToken -> type == RPAR ) {
                    GetToken( currentToken ) ;
                    return true ;
                } // if
            } // if
        } // else if
        
        return false;
    } // Unsigned_Unary_Exp()
    
    void ExecuteFunction( Str100 functionName ) {
        // cout << "ExcuteFunction" << endl ;
        if ( gIsFunctionDefine )
            return ;
        mFunctionHasBeenExecuted = true ;

        if ( strcmp( functionName, "Done" ) == 0 ) {
            gDone = true ;
            return ;
        } // if
        else if ( strcmp( functionName, "ListAllVariables" ) == 0 ) {
            // sort
            
            int index = gVariablelist.size() ;
            while ( index > 1 ) {
                index--;
                for ( int i = 0 ; i < index ; i++ ) {
                    if ( strcmp( gVariablelist[i].variableName, gVariablelist[i+1].variableName ) > 0 ) {
                        Variable_of_c tmp = gVariablelist[i] ;
                        gVariablelist[i] = gVariablelist[i+1] ;
                        gVariablelist[i+1] = tmp ;
                    } // if
                } // for
        
            } // while
            
            // print
            for ( int i = 0 ; i < gVariablelist.size() ; i++ )
                cout << gVariablelist[i].variableName << endl ;
            cout << "LOCAL : " <<endl;
            for ( int i = 0 ; i < gLocalVariablelist.size() ; i++ )
                cout << gLocalVariablelist[i].variableName << endl ;
        } // else if
        else if ( strcmp( functionName, "ListAllFunctions" ) == 0 ) {
            // sort
            int index = gFunctionlist.size() ;
            while ( index > 1 ) {
                index--;
                for ( int i = 0 ; i < index ; i++ ) {
                    if ( strcmp( gFunctionlist[i].functionName, gFunctionlist[i+1].functionName ) > 0 ) {
                        Function_of_c tmp = gFunctionlist[i] ;
                        gFunctionlist[i] = gFunctionlist[i+1] ;
                        gFunctionlist[i+1] = tmp ;
                    } // if
                } // for
            } // while
            
            for ( int i = 0 ; i < gFunctionlist.size() ; i++ ) {
                if ( strcmp( gFunctionlist[i].functionName, "ListAllFunctions" ) != 0  &&
                     strcmp( gFunctionlist[i].functionName, "ListAllVariables" ) != 0  &&
                     strcmp( gFunctionlist[i].functionName, "ListFunction" ) != 0  &&
                     strcmp( gFunctionlist[i].functionName, "ListVariable" ) != 0  &&
                     strcmp( gFunctionlist[i].functionName, "Done" ) != 0  )
                  cout << gFunctionlist[i].functionName << "()" << endl ;
            } // for
            
        } // else if
        else if ( strcmp( mFunctionName, "ListVariable" ) == 0 ) {
            // clean ""
            for ( int i = 0 ; i < strlen( mListFunctionOrVariableName ) ; i++ ) {
                mListFunctionOrVariableName[i] = mListFunctionOrVariableName[i+1] ;
            } // for
            
            int len = strlen( mListFunctionOrVariableName ) ;
            mListFunctionOrVariableName[len-1] = '\0' ;
            // clean end
            // start print
            for ( int i = 0 ; i < gVariablelist.size() ; i++ ) {
                if ( strcmp( mListFunctionOrVariableName, gVariablelist[i].variableName ) == 0 ) {
                    if ( gVariablelist[i].isArray ) {
                        cout << gVariablelist[i].variableType << " " ;
                        cout << gVariablelist[i].variableName << " " ;
                        
                        cout << "[ " << gVariablelist[i].arraySize << " ]" ;
                        for ( int arynum = 0 ; arynum < gVariablelist[i].array.size() ; arynum++ ) {
                            cout << "ary Num : " << gVariablelist[i].array[arynum].arrayNum << " V:" ;
                            cout << gVariablelist[i].array[arynum].value_int << " type :" ;
                            cout << gVariablelist[i].array[arynum].variableType << endl ;
                        } // for
                    } // if
                    else {
                        cout << gVariablelist[i].variableType << " " ;
                        cout << gVariablelist[i].variableName << " " ;
                        cout << gVariablelist[i].value_int  ;
                        if ( gVariablelist[i].variableType == BOOL ) {
                            if ( gVariablelist[i].value_bool )
                                cout << "true" ;
                            else
                                cout << "false" ;
                        } // if
                        else if ( gVariablelist[i].variableType == CHAR )
                            cout << gVariablelist[i].value_char ;
                        
                        if ( gVariablelist[i].arraySize != -1 )
                            cout << "[ " << gVariablelist[i].arraySize << " ]" ;
                        
                        cout << " ;" << endl ;
                    } // else
                } // if
            } // for
        } // else if
        else if ( strcmp( mFunctionName, "ListFunction" ) == 0 ) {
            // clean ""
            for ( int i = 0 ; i < strlen( mListFunctionOrVariableName ) ; i++ ) {
                mListFunctionOrVariableName[i] = mListFunctionOrVariableName[i+1] ;
            } // for
            
            int len = strlen( mListFunctionOrVariableName ) ;
            mListFunctionOrVariableName[len-1] = '\0' ;
            // clean end
            for ( int i = 0 ; i < gFunctionlist.size() ; i++ ) {
                if ( strcmp( gFunctionlist[i].functionName, mListFunctionOrVariableName ) == 0 ) {
                    TokenPtr walk = gFunctionlist[i].functionBodyHead ;
                    cout << gFunctionlist[i].functionType << " " ;
                    cout << gFunctionlist[i].functionName ;
                    int space = 0 ;
                    int smallPar = 0 ;
                    bool oneLineStatement = false ;
                    while ( walk != gFunctionlist[i].functionBodyEnd ) {
                        
                        // Error will occur in some situation
                        // maybe is empty copound statement ??
                        walk = walk -> next ;
                        if ( oneLineStatement ) {
                            space = space - 2 ;
                            oneLineStatement = false ;
                        } // if
                        
                        cout << walk -> tokenStr ;
                        if ( walk -> type == RPAR ) {
                            smallPar-- ;
                            if ( smallPar == 0 && walk -> next -> type != LLPAR && walk -> next -> type != SEMI ) {
                                space = space + 2 ;
                                cout << endl ;
                                for ( int i = space ; i > 0 ; i-- )
                                    cout << " " ;
                                oneLineStatement = true ;
                            } // if
                            else
                                cout << " " ;
                        } // if
                        else if ( walk -> type == LPAR ) {
                            smallPar++ ;
                            if ( walk -> next -> type != RPAR )
                                cout << " " ;
                           
                        } // else if
                        else if ( walk -> type == ELSE ) {
                            if ( walk -> next -> type != LLPAR && walk -> next -> type != SEMI &&
                                     walk -> next -> type != IF ) {
                                space = space + 2 ;
                                cout << endl ;
                                for ( int i = space ; i > 0 ; i-- )
                                    cout << " " ;
                                oneLineStatement = true ;
                            } // if
                            else
                                cout << " " ;
                        } // else if
                        else if ( walk -> type == RLPAR ) {
                            cout << endl ;
                            if ( walk -> next -> type == RLPAR )
                                space = space - 2 ;
                            
                            for ( int i = space ; i > 0 ; i-- )
                                cout << " " ;
                        } // if
                        else if ( walk -> type == SEMI ) {
                            cout << endl ;
                            if ( walk -> next -> type == RLPAR )
                                space = space - 2 ;
                            
                            for ( int i = space ; i > 0 ; i-- )
                                cout << " " ;
                        } // else if
                        else if ( walk -> type == LLPAR ) {
                            cout << endl ;
                            space = space + 2 ;
                            if ( walk -> next -> type == RLPAR )
                                space = space - 2 ;
                            
                            for ( int i = space ; i > 0 ; i-- )
                                cout << " " ;
                        } // else if
                        else if ( walk != gFunctionlist[i].functionBodyEnd ) {
                            if ( walk -> type == IDENT && walk -> next -> type == LPAR ) ;
                            else if ( walk -> type == IDENT && walk -> next -> type == LMPAR ) ;
                            else if ( walk -> type == IDENT && walk -> next -> type == PP ) ;
                            else if ( walk -> type == IDENT && walk -> next -> type == MM ) ;
                            else if ( walk -> type == PP && walk -> next -> type == IDENT ) ;
                            else if ( walk -> type == MM && walk -> next -> type == IDENT ) ;
                            else
                                cout << " " ;
                        } // else if
                        
                    } // while
                    
                    cout << endl ;
                } // else if
            } // for
            
        } // else if
        
    } // ExecuteFunction()
    
    void ReplaceReDefinition( Variable_of_c newDefinition ) {

        for ( int i = 0 ; i < gVariablelist.size() ; i++ ) {
            if ( strcmp( gVariablelist[i].variableName, newDefinition.variableName ) == 0 ) {
                gVariablelist.erase( gVariablelist.begin() + i );
                gVariablelist.push_back( newDefinition ) ;
            } // if
        } // for
        
    } // ReplaceReDefinition()
    
    void ReplaceReDefinition( Function_of_c newDefinition ) {

        for ( int i = 0 ; i < gFunctionlist.size() ; i++ ) {
            if ( strcmp( gFunctionlist[i].functionName, newDefinition.functionName ) == 0 ) {
                gFunctionlist.erase( gFunctionlist.begin() + i );
                gFunctionlist.push_back( newDefinition ) ;
            } // if
        } // for
        
    } // ReplaceReDefinition()
    
    void MessageOfCommand( int messageType ) {
        if ( gDone || gErrorMessage ) return ;
        else if ( messageType == 1 ) {
            if ( mIsFunctionDefinition ) {
                if ( IsFunctionExist( mTmpofFunction.functionName ) ) {
                    cout << "New definition of " << mTmpofFunction.functionName ;
                    cout << "() entered ..." << endl ;
                    ReplaceReDefinition( mTmpofFunction ) ;
                } // if
                else {
                    gFunctionlist.push_back( mTmpofFunction ) ;
                    cout << "Definition of " << mMaybeIsFunctionName << "() entered ..." << endl ;
                } // else
            } // if
            else {
                for ( int i = 0 ; i < mTmpOfFunctionOrIdentList.size() ; i++ ) {
                 
                    if ( IsVariableExist( mTmpOfFunctionOrIdentList[i].variableName ) ) {
                        cout << "New definition of " << mTmpOfFunctionOrIdentList[i].variableName ;
                        cout << " entered ..." << endl ;
                        ReplaceReDefinition( mTmpOfFunctionOrIdentList[i] );
                    } // if
                    else {
                        cout << "Definition of " << mTmpOfFunctionOrIdentList[i].variableName << " entered ..." << endl ;
                        gVariablelist.push_back( mTmpOfFunctionOrIdentList[i] ) ;
              
                    } // else
                } // for
            } // else
        } // else if
        else if ( messageType == 2 ) {
            if ( gIsCout ) {
                for ( int i = 0 ; i < gOutputlist.size() ; i ++ ) {
                    if ( gOutputlist[i].type == INT )
                        cout << gOutputlist[i].value_int ;
                    else if ( gOutputlist[i].type == FLOAT )
                        cout << gOutputlist[i].value_float ;
                    else if ( gOutputlist[i].type == CHAR )
                        cout << gOutputlist[i].value_char ;
                    else if ( gOutputlist[i].type == BOOL ) {
                        if ( gOutputlist[i].value_bool )
                            cout << "true" ;
                        else
                            cout << "false" ;
                    } // else if
                    else if ( gOutputlist[i].type == STRING ) {
                        for ( int j = 0 ; j < strlen( gOutputlist[i].value_string ) ; j++ ) {
                            if ( gOutputlist[i].value_string[j] == '\\' ) {
                                if ( gOutputlist[i].value_string[j+1] == 'n' ) {
                                    j++;
                                    cout << endl ;
                                } // if
                            } // if
                            else
                                cout << gOutputlist[i].value_string[j] ;
                            
                        } // for
                    } // else if
                    
                } // for
            } // if
            
            if ( gWhileWalk == NULL )
                cout << "Statement executed ..." << endl ;
        } // else if
    } // MessageOfCommand()
    
    void ErrorMessage() {
        TokenPtr nextToken ;
        PeekToken( nextToken ) ;
        int errorLine = nextToken -> row ;
        if ( gErrorMessage )
            return ;
        if ( nextToken -> type == ERROR ) {
            cout << "Line " << errorLine - mBeginLine + 1 << " : ";
            cout << "unrecognized token with first char : '" << nextToken -> tokenStr << "'" << endl ;
        } // if
        else {
            cout << "Line " << nextToken -> row - mBeginLine + 1 << " : ";
            cout << "unexpected token : '" << nextToken -> tokenStr << "'" << endl ;
        } // else
        
        gErrorMessage = true ;
    } // ErrorMessage()
    
    void ErrorMessage( Str100 undefinedIdent ) {
        gIsIdentError = true ;
        int errorLine = gCurrentToken -> row ;
        cout << "Line " << errorLine - mBeginLine + 1 << " : ";
        cout << "undefined identifier : '" << undefinedIdent << "'" << endl ;
        gErrorMessage = true ;
    } // ErrorMessage()
    
    bool IsLocalVariableExist( Str100 variableName ) {
        // Exam not only local variable but also global variable
        for ( int i = 0 ; i < gLocalVariablelist.size() ; i++ )  {
            if ( strcmp( gLocalVariablelist[i].variableName, variableName ) == 0 )
                return true ;
        } // for
        
        for ( int i = 0 ; i < gVariablelist.size() ; i ++ ) {
            if ( strcmp( gVariablelist[i].variableName, variableName ) == 0 )
                return true ;
        } // for
        
        return false ;
    } // IsLocalVariableExist()
    
    bool IsFunctionExist( Str100 variableName ) {
        for ( int i = 0 ; i < gFunctionlist.size() ; i++ ) {
            if ( strcmp( variableName, gFunctionlist[i].functionName ) == 0 )
                return true ;
        } // for
        
        return false ;
    } // IsFunctionExist()
    
    bool ExamIsVariableExist( Str100 variableName ) {
        TokenPtr nextToken ;
        
        if ( mIsCompound_Statement ) {
            if ( IsLocalVariableExist( variableName ) ) {
                return true ;
            } // if
            else if ( IsFunctionExist( variableName ) )
                return true ;
            else {
                PeekToken( nextToken ) ;
                if ( nextToken -> type == LPAR )
                    return true ;
            } // else
        } // if
        else if ( IsVariableExist( variableName ) ) {
            return true ;
        } // else if
        else {
            if ( IsFunctionExist( variableName ) ) {
                PeekToken( nextToken ) ;
                if ( nextToken -> type == LPAR ) {
                    return true ;
                } // if
            } // if
        } // else
        
        ErrorMessage( variableName ) ;
        return false ;
    } // ExamIsVariableExist()
    
    void GetVariableValue( Str100 variableName, Value_of_output &value, int arrayNum ) {
        value.arraynum = arrayNum ;
        strcpy( value.name, variableName ) ;
        for ( int i = 0 ; i < gLocalVariablelist.size() ; i ++ ) {
            if ( strcmp( gLocalVariablelist[i].variableName, variableName ) == 0 ) {
                
                if ( !gLocalVariablelist[i].isArray ) {
                    
                    value.type = gLocalVariablelist[i].variableType  ;
                    if ( value.type == INT  )
                        value.value_int = gLocalVariablelist[i].value_int ;
                    else if ( value.type == FLOAT )
                        value.value_float = gLocalVariablelist[i].value_float ;
                    else if ( value.type == STRING )
                        strcpy( value.value_string, gLocalVariablelist[i].value_string ) ;
                    else if ( value.type == BOOL ) {
                        if ( gLocalVariablelist[i].value_bool )
                            value.value_bool = true;
                        else
                            value.value_bool = false ;
                    } // else if
                    else if ( value.type == CHAR )
                        strcpy( value.value_char, gLocalVariablelist[i].value_char ) ;
                } // if
                else {
                    int num = 0 ;
                    for ( int k = 0 ; k < gLocalVariablelist[i].array.size() ; k++ ) {
                        if ( gLocalVariablelist[i].array[k].arrayNum == arrayNum )
                            num = k ;
                    } // for
                    //
                    value.type = gLocalVariablelist[i].variableType ;
                    if ( num < gLocalVariablelist[i].array.size() ) {
                        if ( value.type == INT  ) {
                            value.value_int = gLocalVariablelist[i].array[num].value_int ;
                        } // if
                        else if ( value.type == FLOAT )
                            value.value_float = gLocalVariablelist[i].array[num].value_float ;
                        else if ( value.type == STRING ) {
                            strcpy( value.value_string, gLocalVariablelist[i].array[num].value_string ) ;
                        } // else if
                        else if ( value.type == BOOL ) {
                            if ( gLocalVariablelist[i].array[num].value_bool )
                                value.value_bool = true;
                            else
                                value.value_bool = false ;
                        } // else if
                        else if ( value.type == CHAR )
                            strcpy( value.value_char, gLocalVariablelist[i].array[num].value_char ) ;
                        
                    } // if
                    else { // not assign yet
                        value.type = gLocalVariablelist[i].variableType ;
                    } // else
                } // else
                
                return ;
            } // if
        } // for
        
        for ( int i = 0 ; i < gVariablelist.size() ; i ++ ) {
            if ( strcmp( gVariablelist[i].variableName, variableName ) == 0 ) {
                
                if ( !gVariablelist[i].isArray ) {
              
                    value.type = gVariablelist[i].variableType  ;
                    if ( value.type == INT  )
                        value.value_int = gVariablelist[i].value_int ;
                    else if ( value.type == FLOAT )
                        value.value_float = gVariablelist[i].value_float ;
                    else if ( value.type == STRING )
                        strcpy( value.value_string, gVariablelist[i].value_string ) ;
                    else if ( value.type == BOOL ) {
                        if ( gVariablelist[i].value_bool )
                            value.value_bool = true;
                        else
                            value.value_bool = false ;
                    } // else if
                    else if ( value.type == CHAR )
                        strcpy( value.value_char, gVariablelist[i].value_char ) ;
                } // if
                else {
                    int num = 0 ;
                    for ( int k = 0 ; k < gVariablelist[i].array.size() ; k++ ) {
                        if ( gVariablelist[i].array[k].arrayNum == arrayNum )
                            num = k ;
                    } // for
                    //
                    value.type = gVariablelist[i].variableType ;
                    if ( num < gVariablelist[i].array.size() ) {
                        if ( value.type == INT  ) {
                            value.value_int = gVariablelist[i].array[num].value_int ;
                        } // if
                        else if ( value.type == FLOAT )
                            value.value_float = gVariablelist[i].array[num].value_float ;
                        else if ( value.type == STRING ) {
                            strcpy( value.value_string, gVariablelist[i].array[num].value_string ) ;
                        } // else if
                        else if ( value.type == BOOL ) {
                            if ( gVariablelist[i].array[num].value_bool )
                                value.value_bool = true;
                            else
                                value.value_bool = false ;
                        } // else if
                        else if ( value.type == CHAR )
                            strcpy( value.value_char, gVariablelist[i].array[num].value_char ) ;
                        
                    } // if
                    else { // not assign yet
                        value.type = gVariablelist[i].variableType ;
                    } // else
                } // else
                
            } // if
        } // for
    } // GetVariableValue()
    
    void AssignVariableValue( Str100 variableName, Value_of_output value, int arrayNum ) {
        Variable_of_c tmp ;
        
        for ( int i = 0 ; i < gLocalVariablelist.size() ; i ++ ) {
            if ( strcmp( gLocalVariablelist[i].variableName, variableName ) == 0 ) {
                if ( arrayNum == -1 ) {
                    if ( gLocalVariablelist[i].variableType == INT ) {
                        gLocalVariablelist[i].value_int = value.value_int ;
                    } // if
                    else if ( gLocalVariablelist[i].variableType == FLOAT ) {
                        gLocalVariablelist[i].value_float = value.value_float ;
                    } // else if
                    else if ( gLocalVariablelist[i].variableType == STRING ) {
                        strcpy( gLocalVariablelist[i].value_string, value.value_string ) ;
                    } // else if
                    else if ( gLocalVariablelist[i].variableType == BOOL ) {
                        gLocalVariablelist[i].value_bool = value.value_bool ;
                    } // else if
                    else if ( gLocalVariablelist[i].variableType == CHAR ) {
                        strcpy( gLocalVariablelist[i].value_char, value.value_char );
                    } // else if
                } // if
                else {
                    int num = -1;
                    for ( int j = 0 ; j < gLocalVariablelist[i].array.size() ; j++ ) {
                        if ( gLocalVariablelist[i].array[j].arrayNum == arrayNum )
                            num = j ;
                    } // for
                    // num == -1 means vairalbe[arraynum] has not define yet
                    if ( num == -1 ) {
                        if ( gLocalVariablelist[i].variableType == INT ) {
                            tmp.value_int = value.value_int ;
                            tmp.variableType = INT ;
                        } // if
                        else if ( gLocalVariablelist[i].variableType == FLOAT ) {
                            tmp.value_float = value.value_float ;
                            tmp.variableType = FLOAT ;
                        } // else if
                        else if ( gLocalVariablelist[i].variableType == STRING ) {
                            strcpy( tmp.value_string, value.value_string ) ;
                            tmp.variableType = STRING ;
                        } // else if
                        else if ( gLocalVariablelist[i].variableType == BOOL ) {
                            tmp.value_bool = value.value_bool ;
                            tmp.variableType = BOOL ;
                        } // else if
                        else if ( gLocalVariablelist[i].variableType == CHAR ) {
                            strcpy( tmp.value_char, value.value_char );
                            tmp.variableType = CHAR ;
                        } // else if
                        
                        tmp.arrayNum = arrayNum ;
                        gLocalVariablelist[i].array.push_back( tmp ) ;
                    } // if
                    // else repalce the orignal value
                    else {
                        if ( gLocalVariablelist[i].variableType == INT ) {
                            gLocalVariablelist[i].array[num].value_int = value.value_int ;
                        } // if
                        else if ( gLocalVariablelist[i].variableType == FLOAT ) {
                            gLocalVariablelist[i].array[num].value_float = value.value_float ;
                        } // else if
                        else if ( gLocalVariablelist[i].variableType == STRING ) {
                            strcpy( gLocalVariablelist[i].array[num].value_string, value.value_string );
                        } // else if
                        else if ( gLocalVariablelist[i].variableType == BOOL ) {
                            gLocalVariablelist[i].array[num].value_bool = value.value_bool ;
                        } // else if
                        else if ( gLocalVariablelist[i].variableType == CHAR ) {
                            strcpy( gLocalVariablelist[i].array[num].value_char, value.value_char );
                        } // else if
                        
                        gLocalVariablelist[i].isArray = true ;
                    } // else
                } // else
                
                return ;
            } // if
        } // for
        
        for ( int i = 0 ; i < gVariablelist.size() ; i ++ ) {
            if ( strcmp( gVariablelist[i].variableName, variableName ) == 0 ) {
                if ( arrayNum == -1 ) {
                    if ( gVariablelist[i].variableType == INT ) {
                        gVariablelist[i].value_int = value.value_int ;
                    } // if
                    else if ( gVariablelist[i].variableType == FLOAT ) {
                        gVariablelist[i].value_float = value.value_float ;
                    } // else if
                    else if ( gVariablelist[i].variableType == STRING ) {
                        strcpy( gVariablelist[i].value_string, value.value_string ) ;
                    } // else if
                    else if ( gVariablelist[i].variableType == BOOL ) {
                        gVariablelist[i].value_bool = value.value_bool ;
                    } // else if
                    else if ( gVariablelist[i].variableType == CHAR ) {
                        strcpy( gVariablelist[i].value_char, value.value_char );
                    } // else if
                } // if
                else {
                    int num = -1;
                    for ( int j = 0 ; j < gVariablelist[i].array.size() ; j++ ) {
                        if ( gVariablelist[i].array[j].arrayNum == arrayNum )
                            num = j ;
                    } // for
                    // num == -1 means vairalbe[arraynum] has not define yet
                    if ( num == -1 ) {
                        if ( gVariablelist[i].variableType == INT ) {
                            tmp.value_int = value.value_int ;
                            tmp.variableType = INT ;
                        } // if
                        else if ( gVariablelist[i].variableType == FLOAT ) {
                            tmp.value_float = value.value_float ;
                            tmp.variableType = FLOAT ;
                        } // else if
                        else if ( gVariablelist[i].variableType == STRING ) {
                            strcpy( tmp.value_string, value.value_string ) ;
                            tmp.variableType = STRING ;
                        } // else if
                        else if ( gVariablelist[i].variableType == BOOL ) {
                            tmp.value_bool = value.value_bool ;
                            tmp.variableType = BOOL ;
                        } // else if
                        else if ( gVariablelist[i].variableType == CHAR ) {
                            strcpy( tmp.value_char, value.value_char );
                            tmp.variableType = CHAR ;
                        } // else if
                        
                        tmp.arrayNum = arrayNum ;
                        gVariablelist[i].array.push_back( tmp ) ;
                    } // if
                    // else repalce the orignal value
                    else {
                        if ( gVariablelist[i].variableType == INT ) {
                            gVariablelist[i].array[num].value_int = value.value_int ;
                        } // if
                        else if ( gVariablelist[i].variableType == FLOAT ) {
                            gVariablelist[i].array[num].value_float = value.value_float ;
                        } // else if
                        else if ( gVariablelist[i].variableType == STRING ) {
                            strcpy( gVariablelist[i].array[num].value_string, value.value_string );
                        } // else if
                        else if ( gVariablelist[i].variableType == BOOL ) {
                            gVariablelist[i].array[num].value_bool = value.value_bool ;
                        } // else if
                        else if ( gVariablelist[i].variableType == CHAR ) {
                            strcpy( gVariablelist[i].array[num].value_char, value.value_char );
                        } // else if
                        
                        gVariablelist[i].isArray = true ;
                    } // else
                } // else
                
                return ;
            } // if
        } // for
    } // AssignVariableValue()
    
    void PPorMMCaculate() {
        Value_of_output value ;
        int arrayNum ;
        for ( int i = 0 ; i < mTmpOfPMCaculateList.size() ; i++ ) {
            arrayNum = mTmpOfPMCaculateList[i].arrayNum ;
            GetVariableValue( mTmpOfPMCaculateList[i].identName, value, arrayNum );
         
            if ( value.type == FLOAT ) {
                
                if ( mTmpOfPMCaculateList[i].ppormm == PP )
                    value.value_float++;
                else // if ( signIsPPorMM == MM )
                    value.value_float--;
            } // if
            else if ( value.type == INT ) {
                if ( mTmpOfPMCaculateList[i].ppormm == PP )
                    value.value_int++;
                else // if ( signIsPPorMM == MM )
                    value.value_int--;
            } // else if
            // else error
            
            AssignVariableValue( mTmpOfPMCaculateList[i].identName, value, arrayNum ) ;

        } // for
        
        mTmpOfPMCaculateList.clear() ;
    } // PPorMMCaculate()
    
    void PrintOutputList() {
        for ( int i = 0 ; i < gOutputlist.size() ; i++ ) {
            if ( gOutputlist[i].type == INT )
                cout << gOutputlist[i].value_int ;
            else if ( gOutputlist[i].type == STRING )
                cout << gOutputlist[i].value_string ;

        }
    } // PrintOutputList()
    
    void ResetAllVariable() {
        gErrorMessage = false ;
        gHasGetToken = false ;
        gIsIdentError = false ;
        gIsCout = false ;
        gIsFunctionDefine = false ;
        mIsParameter = false ;
        mIsFunction = false ;
        mAssign = false ;
        mHaveParameter = false ;
        mIsFunctionDefinition = false ;
        mFunctionHasBeenExecuted = false ;
        mIsCompound_Statement = false ;
        mMaybeIsFunction = false ;
        mPPorMMHasExist = false;
        mTmpOfFunctionOrIdentList.clear() ;
        mTmpOfLocalVariableList.clear() ;
        gLocalVariablelist.clear() ;
        gOutputlist.clear() ;
        mTmpOfPMCaculateList.clear() ;
    } // ResetAllVariable()
}; // end class Parser()

int main() {
    printf( "Our-C running ...\n" ) ;
    // char ch = '\0' ;
    // int testNum ;
    // scanf( "%d%c", &testNum, &ch ) ;
    TokenPtr nextToken ;
    TokenPtr currentToken ;
    Parser p ;
    int errorLine = 1;

    gwalk = gFront ;
    PeekToken( nextToken ) ;
    Variable_of_c cout, cin ;
    Function_of_c done, listAllVariable ;
    Function_of_c listVaraible, listAllFunc, listFunc ;
    // Define function
    strcpy( cout.variableName, "cout" ) ;
    strcpy( cin.variableName, "cin" ) ;
    strcpy( done.functionName, "Done" ) ;
    strcpy( listAllVariable.functionName, "ListAllVariables" ) ;
    strcpy( listVaraible.functionName, "ListVariable" ) ;
    strcpy( listAllFunc.functionName, "ListAllFunctions" ) ;
    strcpy( listFunc.functionName, "ListFunction" ) ;
    gFunctionlist.push_back( done );
    gFunctionlist.push_back( listAllVariable ) ;
    gFunctionlist.push_back( listVaraible ) ;
    gFunctionlist.push_back( listAllFunc ) ;
    gFunctionlist.push_back( listFunc );
    gVariablelist.push_back( cout ) ;
    gVariablelist.push_back( cin ) ;
    while ( !gDone ) {
        if ( !p.User_input() ) {
            // skip error line
            if ( !gIsIdentError )
                GetToken( currentToken ) ;
            errorLine = gCurrentToken -> row ;
            PeekToken( nextToken ) ;
            while ( errorLine == nextToken -> row ) {
                GetToken( currentToken ) ;
                PeekToken( nextToken ) ;
            } // while
            
        } // if
    } // while
    
    printf( "Our-C exited ..." ) ;
} // main()

