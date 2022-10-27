    typedef int TElem;

    #ifndef LSTACK_H_INCLUDED
    #define LSTACK_H_INCLUDED

    struct Node;
    struct LnkStack;
    typedef struct Node *PtrToNode;
    typedef struct LnkStack *PtrToStack;
    typedef PtrToNode PNode;
    typedef PtrToStack LStack;

    int IsEmptyStack( LStack S );
    LStack CreateStack( void );
    void RemoveStack( LStack *PS );
    void MakeEmptyStack( LStack S );
    void Push( TElem X, LStack S );
    TElem Top( LStack S );
    void Pop( LStack S );
    TElem TopAndPop( LStack S );
    void PrintStack( LStack S );
	
    void MyPrintf(const char *format, ...);

    #endif // LSTACK_H_INCLUDED
