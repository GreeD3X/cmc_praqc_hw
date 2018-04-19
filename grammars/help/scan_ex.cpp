enum type_of_lex
{
  LEX_NULL, /*0*/
  LEX_AND, LEX_BEGIN, LEX_BOOL, LEX_DO, LEX_ELSE, LEX_END, LEX_IF, LEX_FALSE, LEX_INT, /*9*/
  LEX_NOT, LEX_OR, LEX_PROGRAM, LEX_READ, LEX_THEN, LEX_TRUE, LEX_VAR, LEX_WHILE, LEX_WRITE, /*18*/
  LEX_FIN, /*19*/
  LEX_SEMICOLON, LEX_COMMA, LEX_COLON, LEX_ASSIGN, LEX_LPAREN, LEX_RPAREN, LEX_EQ, LEX_LSS, /*27*/
  LEX_GTR, LEX_PLUS, LEX_MINUS, LEX_TIMES, LEX_SLASH, LEX_LEQ, LEX_NEQ, LEX_GEQ, /*35*/
  LEX_NUM, /*36*/
  LEX_ID, /*37*/
  POLIZ_LABEL, /*38*/
  POLIZ_ADDRESS, /*39*/
  POLIZ_GO, /*40*/
  POLIZ_FGO}; /*41*/
 
/////////////////////////  Класс Lex  //////////////////////////
 
class Lex
{
  type_of_lex t_lex;
  int v_lex;
public:
                      Lex ( type_of_lex t = LEX_NULL, int v = 0): t_lex (t), v_lex (v) {}
         type_of_lex  get_type ()  { return t_lex; }
         int          get_value () { return v_lex; }
  friend ostream &    operator<< (ostream &s, Lex l)
                      {
                        s << '(' << l.t_lex << ',' << l.v_lex << ");" ;
                        return s;
                      }
};
 
/////////////////////  Класс Ident  ////////////////////////////
 
class Ident
{
         char       * name;
         bool         declare;
         type_of_lex  type;
         bool         assign;
         int          value;
public:
                      Ident() { declare = false; assign = false; }
         char       * get_name () { return name; }
         void         put_name (const char *n)
                      {
                        name = new char [ strlen(n)+1];
                        strcpy(name,n);
                      }
         bool         get_declare () { return declare; }
         void         put_declare () { declare = true; }
         type_of_lex  get_type    () { return type; }
         void         put_type    ( type_of_lex t ) { type = t; }
         bool         get_assign  () { return assign; }
         void         put_assign  (){ assign = true; }
         int          get_value   () { return value; }
         void         put_value   (int v){ value = v; }
};
 
//////////////////////  Класс Tabl_ident  ///////////////////////
 
class Tabl_ident
{
         Ident      * p;
         int          size;
         int          top;
public:
                      Tabl_ident ( int max_size )
                      {
                        p = new Ident [ size = max_size ];
                        top = 1;
                      }
                     ~Tabl_ident () { delete [] p; }
         Ident      & operator[] ( int k ) { return p[k]; }
         int          put ( const char *buf );
};
 
int Tabl_ident::put ( const char *buf )
{
  for ( int j = 1; j < top; j++ )
    if ( !strcmp ( buf, p[j].get_name() ) )
      return j;
  p[top].put_name(buf);
  ++top;
  return top-1;
}
 
/////////////////////////////////////////////////////////////////
 
template < class T, int max_size >
class Stack
{
         T            s [max_size];
         int          top;
public:
                      Stack () { top = 0; }
         void         reset () { top = 0; }
         void         push ( T i );
         T            pop ();
         bool         is_empty () { return top == 0; }
         bool         is_full  () { return top == max_size; }
};
 
template < class T, int max_size >
void Stack < T, max_size > :: push (T i)
{
  if ( !is_full() )
    s [top++] = i;
  else
    throw "Stack_is_full";
}
 
template <class T, int max_size >
T Stack < T, max_size > :: pop ()
{
  if ( !is_empty() )
    return s[--top];
  else
    throw "stack_is_empty";
}
 
/////////////////////////  Класс Poliz  /////////////////////////////
 
class Poliz
{
         Lex        * p;
         int          size;
         int          free;
public:
                      Poliz (int max_size)
                      {
                        p = new Lex [size = max_size];
                        free = 0;
                      }
                     ~Poliz() { delete [] p; }
         void         put_lex ( Lex l )
                      {
                        p [free] = l;
                        free++;
                      }
         void         put_lex ( Lex l, int place) { p [place] = l; }
         void         blank    () { free++; }
         int          get_free () { return free; }
         Lex        & operator[] (int index)
                      {
                        if ( index > size )
                          throw "POLIZ:out of array";
                        else
                          if ( index > free )
                            throw "POLIZ:indefinite element of array";
                          else
                            return p[index];
                      }
         void         print ()
                      {
                        for ( int i = 0; i < free; ++i )
                          cout << p[i];
                      }
};
 
////////////////////////////////////////////////////////////////////
 
Tabl_ident TID ( 100 );
 
/////////////////////  Класс Scanner  //////////////////////////////
 
class Scanner
{
         enum         state { H, IDENT, NUMB, COM, ALE, DELIM, NEQ };
  static char       * TW    [];
  static type_of_lex  words [];
  static char       * TD    [];
  static type_of_lex  dlms  [];
         state        CS;
         FILE       * fp;
         char         c;
         char         buf [ 80 ];
         int          buf_top;
         void         clear ()
                      {
                        buf_top = 0;
                        for ( int j = 0; j < 80; j++ )
                          buf[j] = '\0';
                      }
         void         add ()
                      {
                        buf [ buf_top++ ] = c;
                      }
         int          look ( const char *buf, char **list )
                      {
                        int i = 0;
                        while (list[i])
                        {
                          if ( !strcmp(buf, list[i]) )
                            return i;
                          ++i;
                        }
                        return 0;
                      }
         void         gc ()
                      {
                        c = fgetc (fp);
                      }
public:
                      Scanner ( const char * program )
                      {
                        fp = fopen ( program, "r" );
                        CS = H;
                        clear();
                        gc();
                      }
         Lex          get_lex ();
};
 
char *
Scanner::TW    [] = {"", "and", "begin", "bool", "do", "else", "end", "if", "false", "int", "not", "or", "program",
                            "read", "then", "true", "var", "while", "write", NULL};
 
type_of_lex
Scanner::words [] = {LEX_NULL, LEX_AND, LEX_BEGIN, LEX_BOOL, LEX_DO, LEX_ELSE, LEX_END, LEX_IF, LEX_FALSE, LEX_INT,
                     LEX_NOT, LEX_OR, LEX_PROGRAM, LEX_READ, LEX_THEN, LEX_TRUE, LEX_VAR, LEX_WHILE, LEX_WRITE, LEX_NULL};
 
char *
Scanner::TD    [] = {"", "@", ";", ",", ":", ":=", "(", ")", "=", "<", ">", "+", "-", "*", "/", "<=", "!=", ">=", NULL};
 
type_of_lex
Scanner::dlms  [] = {LEX_NULL, LEX_FIN, LEX_SEMICOLON, LEX_COMMA, LEX_COLON, LEX_ASSIGN, LEX_LPAREN, LEX_RPAREN, LEX_EQ,
                     LEX_LSS, LEX_GTR, LEX_PLUS, LEX_MINUS, LEX_TIMES, LEX_SLASH, LEX_LEQ, LEX_NEQ, LEX_GEQ, LEX_NULL};
 
Lex Scanner::get_lex () 
{
  int d, j;
 
  CS = H;
  do
  {
    switch(CS)
    {
      case H:
        if ( c==' ' || c == '\n' || c== '\r' || c == '\t' ) 
          gc();
        else if ( isalpha(c) )
        {
          clear();
          add();
          gc();
          CS = IDENT;
        }
        else if ( isdigit(c) )
        {
          d = c - '0';
          gc();
          CS = NUMB;
        }
        else 
          CS = DELIM;
        break;
      case IDENT:
        if ( isalpha(c) || isdigit(c) ) 
        {
          add(); 
          gc();
        }
        else if ( j = look (buf, TW) )
          return Lex (words[j], j);
        else
        {
          j = TID.put(buf);
          return Lex (LEX_ID, j);
        }
        break;
      case NUMB:
        if ( isdigit(c) ) 
        {
          d = d * 10 + (c - '0'); gc();
        }
        else
          return Lex ( LEX_NUM, d);
        break;
      case COM:
        if ( c == '}' )
        {
          gc();
          CS = H;
        }
        else if (c == '@' || c == '{' )
          throw c;
        else
          gc();
        break;
      case ALE:
        if ( c== '=')
        {
          add();
          gc();
          j = look ( buf, TD );
          return Lex ( dlms[j], j);
        }
        else
        {
          j = look ( buf, TD );
          return Lex ( dlms[j], j );
        }
        break;
      case NEQ:
        if (c == '=')
        {
          add();
          gc();
          j = look ( buf, TD );
          return Lex ( LEX_NEQ, j );
        }
        else
          throw '!';
        break;
      case DELIM:
        clear();
        add();
        if ( j = look ( buf, TD) )
        {
          gc();
          return Lex ( dlms[j], j );
        }
        else
          throw c;
      break;
    }//end switch
  } while (true);
}
