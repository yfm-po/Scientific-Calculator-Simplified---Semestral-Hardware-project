    // SS 2019, KPI FEI TUKE

    #include "lstack.h"
    #include "err.h"
    #include <stdlib.h>

    struct Node
    {
        PNode	Next;
        TElem	Elem;
    };

    struct LnkStack
    {
        PNode	First;
        PNode	Last;
    };

    int IsEmptyStack( LStack S )
    {
        if(S == NULL) Error("IsEmptyStack: incorrect stack!");
        return S->First == NULL;
    }

    LStack CreateStack( void )
    {
        LStack S;
        S = malloc(sizeof(struct LnkStack));
        if(S == NULL)Error("CreateStack: out of memory!");
        S->First = NULL;
        S->Last = NULL;
        return S;
    }

    // Removes a stack
    // Receives a pointer to LStack, not LStack itself
    void RemoveStack( LStack *PS )
    {
        if(PS == NULL) Error("RemoveStack: incorrect pointer!");
        if(*PS == NULL) return;
        LStack S = *PS;
        MakeEmptyStack(S);
        free(S);
        *PS = NULL;
    }

    void MakeEmptyStack( LStack S )
    {
        if(S == NULL) Error("MakeEmptyStack: incorrect stack!");
        PNode PTop;
        while(!IsEmptyStack(S)){
            PTop = S->First;
            if(S->First == S->Last) S->Last = NULL;
            S->First = S->First->Next;
            free(PTop);
        }
    }

    // Inserting element to the beginning of a list
    void Push( TElem X, LStack S )
    {
        PNode PNew;
        if(S == NULL) Error("Push: incorrect stack!");
        PNew = malloc(sizeof(struct Node));
        if(PNew == NULL) Error("Push: out of memory!");
        PNew->Elem = X;
        PNew->Next = S->First;
        if(IsEmptyStack(S)) S->Last = PNew;
        S->First = PNew;
    }

    // Prints elements in the tack S
    void PrintStack( LStack S )
    {
        if(S == NULL) Error("PrintStack: incorrect stack!");
        //if(IsEmptyStack(S)) printf("Empty stack");
        else{
            PNode PTmp = S->First;
            printf("Top> ");
            while(PTmp != NULL){
                printf("%d ", PTmp->Elem);
                PTmp = PTmp->Next;
            }
        }
    }

    TElem Top(LStack S)
    {
        if (S == NULL) Error("Wrong input!");
        return S->First->Elem;
    }

    void Pop(LStack S)
    {
        if (S == NULL) Error("Wrong input!");
        PNode PTop = S->First;
        if (S->First == S->Last) S->Last = NULL;
        S->First = S->First->Next;
        free(PTop);
    }

    TElem TopAndPop(LStack S)
    {
        TElem X = Top(S);
        Pop(S);
        return X;
    }

    
