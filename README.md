# C Compiler
The program performs lexical, syntactic, and semantic analysis while referencing a symbol table curated from a custom EBNF grammar and set of semantic rules. 

If the input (source code) passes all tests then the program will send to standard output whatever the input requested to be printed. 

**Grammar:**

Prog ​ ​::= ​ ​StmtList

StmtList ​ ​::= ​ ​{ ​ ​Stmt ​ ​T_SC ​ ​} ​ ​{ ​ ​StmtList ​ ​}

Stmt ​ ​::= ​ ​Decl ​ ​| ​ ​Set ​ ​| ​ ​Print

Decl ​ ​::= ​ ​T_INT ​ ​T_ID ​ ​| ​ ​T_STRING ​ ​T_ID

Set ​ ​::= ​ ​T_SET ​ ​T_ID ​ ​Expr

Print ​ ​::= ​ ​T_PRINT ​ ​Expr ​ ​| ​ ​T_PRINTLN ​ ​Expr

Expr ​ ​::= ​ ​Term ​ ​{ ​ ​(T_PLUS|T_MINUS) ​ ​Expr ​ ​}

Term ​ ​::= ​ ​Primary ​ ​{ ​ ​(T_STAR|T_SLASH) ​ ​Term ​ ​}

Primary ​ ​::= ​ ​T_ICONST ​ ​| ​ ​T_SCONST ​ ​| ​ ​T_ID ​ ​| ​ ​T_LPAREN ​ ​Expr ​ ​T_RPAREN

**Semantic rules:**

1. There ​ ​are ​ ​only ​ ​two ​ ​types: ​ ​integer ​ ​and ​ ​string.

2. Variables ​ ​must ​ ​be ​ ​declared ​ ​before ​ ​they ​ ​are ​ ​used. ​ ​They ​ ​are ​ ​declared ​ ​with ​ ​Decl.

3. There ​ ​is ​ ​one ​ ​single ​ ​scope ​ ​for ​ ​variables.

4. Variable ​ ​names ​ ​must ​ ​be ​ ​unique; ​ ​once ​ ​a ​ ​name ​ ​has ​ ​been ​ ​declared, ​ ​it ​ ​may ​ ​not ​ ​be
declared ​ ​again.

5. An ​ ​integer ​ ​constant ​ ​is ​ ​of ​ ​type ​ ​integer.

6. A ​ ​string ​ ​constant ​ ​is ​ ​of ​ ​type ​ ​string.

7. The ​ ​type ​ ​of ​ ​an ​ ​identifier ​ ​is ​ ​the ​ ​type ​ ​assigned ​ ​when ​ ​it ​ ​is ​ ​declared.

8. The ​ ​type ​ ​of ​ ​an ​ ​Expr ​ ​or ​ ​a ​ ​Term ​ ​is ​ ​the ​ ​type ​ ​of ​ ​the ​ ​result ​ ​of ​ ​the ​ ​operation.

9. Adding ​ ​two ​ ​integers ​ ​results ​ ​in ​ ​an ​ ​integer ​ ​representing ​ ​the ​ ​sum

10. Subtracting ​ ​two ​ ​integers ​ ​results ​ ​in ​ ​an ​ ​integer ​ ​representing ​ ​the ​ ​difference

11. Multiplying ​ ​two ​ ​integers ​ ​results ​ ​in ​ ​an ​ ​integer ​ ​representing ​ ​the ​ ​product

12. Dividing ​ ​two ​ ​integers ​ ​results ​ ​in ​ ​an ​ ​integer ​ ​representing ​ ​the ​ ​quotient

13. Adding ​ ​two ​ ​strings ​ ​results ​ ​in ​ ​a ​ ​string ​ ​representing ​ ​the ​ ​concatenation

14. Multiplying ​ ​an ​ ​integer ​ ​by ​ ​a ​ ​string ​ ​results ​ ​in ​ ​a ​ ​string ​ ​which ​ ​is ​ ​the ​ ​string ​ ​operand ​ ​repeated
integer ​ ​times ​ ​(i.e. ​ ​3 ​ ​* ​ ​“hi” ​ ​is ​ ​“hihihi”)

15. Dividing ​ ​two ​ ​strings ​ ​results ​ ​in ​ ​an ​ ​a ​ ​string ​ ​where ​ ​the ​ ​first ​ ​instance ​ ​of ​ ​the ​ ​denominator ​ ​that
is ​ ​found ​ ​in ​ ​the ​ ​numerator ​ ​is ​ ​removed.

16. ALL ​ ​OTHER ​ ​COMBINATIONS ​ ​OF ​ ​OPERATIONS ​ ​ARE ​ ​UNDEFINED ​ ​and ​ ​are ​ ​an ​ ​error

17. A ​ ​variable ​ ​is ​ ​assigned ​ ​a ​ ​value ​ ​using ​ ​a ​ ​Set. ​ ​The ​ ​type ​ ​of ​ ​the ​ ​variable ​ ​being ​ ​set ​ ​must ​ ​match
the ​ ​type ​ ​of ​ ​the ​ ​expression; ​ ​if ​ ​it ​ ​does ​ ​not ​ ​this ​ ​is ​ ​an ​ ​error.

18. An ​ ​expression ​ ​can ​ ​be ​ ​printed ​ ​using ​ ​either ​ ​“print” ​ ​or ​ ​“println”. ​ ​In ​ ​both ​ ​cases, ​ ​the ​ ​value ​ ​of
the ​ ​Expr ​ ​is ​ ​printed ​ ​to ​ ​the ​ ​standard ​ ​output. ​ ​In ​ ​the ​ ​case ​ ​of ​ ​“println”, ​ ​a ​ ​newline ​ ​is ​ ​printed ​ ​to
standard ​ ​out ​ ​at ​ ​the ​ ​end ​ ​of ​ ​the ​ ​execution ​ ​of ​ ​the ​ ​program.
