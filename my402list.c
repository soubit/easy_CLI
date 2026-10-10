

#include "cs402.h"
#include "my402list.h"



int My402ListAppend(My402List* list, void* obj){
    if (My402ListEmpty(list)){
        
        // list is empty
        My402ListElem *temp = (My402ListElem *)malloc(sizeof (My402ListElem));        



        if (temp == NULL)
            return FALSE; // memroy is not allocated

        temp->obj = obj;


        list->anchor.next = temp;
        list->anchor.prev = temp;

        temp->next = &(list->anchor);
        temp->prev = &(list->anchor);
        

    }
    else{
        My402ListElem *last = My402ListLast(list);

        My402ListElem *temp  = (My402ListElem *)malloc(sizeof (My402ListElem));
        
        if (temp == NULL){
            return FALSE;
        }

        temp->obj = obj;

        
        temp->next = last->next;
        last->next = temp;
    
        
        temp->prev = last;
        list->anchor.prev = temp;



    }

    list->num_members ++;
    return TRUE;
}

 int  My402ListPrepend(My402List* list, void* obj){
    if (My402ListEmpty(list)){
        
        // list is empty
        My402ListElem *temp = (My402ListElem *)malloc(sizeof (My402ListElem));        

        if (temp == NULL){
            return FALSE; // memroy is not allocated
        }

        temp->obj = obj;


        list->anchor.next = temp;
        list->anchor.prev = temp;

        temp->next = &list->anchor;
        temp->prev = &list->anchor;
        

    }
    else{

        My402ListElem *first = My402ListFirst(list);

        My402ListElem *temp = (My402ListElem*)malloc(sizeof (My402ListElem));

        if(temp == NULL){
            return FALSE;
        }
        
        temp->obj = obj;


        
        
        temp->prev = first->prev;
        first->prev = temp;

        temp->next = first;
        list->anchor.next = temp;
        


    }

    list->num_members ++;
    return TRUE;
 }





 
 int  My402ListInsertAfter(My402List* list, void* obj, My402ListElem* elem){
    if (elem==NULL){
        return My402ListAppend(list,obj);

    }else{
        // insert elem between elem and elem->prev
        My402ListElem *temp = (My402ListElem *) malloc(sizeof (My402ListElem));
        
        if (temp == NULL){
            return FALSE;
        }

        temp->obj = obj;

        // elem->prev->next = temp;
        // temp->prev = elem->prev;
        // elem->prev = temp;
        // temp->next = elem;

        
        
        temp->next = elem->next;
        elem->next = temp;

        temp->next->prev = temp;
        temp->prev = elem;

        
        list->num_members++;
        return TRUE;

    }
 }

int  My402ListInsertBefore(My402List* list, void* obj, My402ListElem* elem){
    if (elem == NULL){
        return My402ListPrepend(list,obj);

    }else{
        My402ListElem *temp = (My402ListElem *) malloc(sizeof (My402ListElem));

        if (temp == NULL){
            return FALSE;
        }


        temp->obj = obj;

    

        temp->prev = elem->prev;
        elem->prev = temp;

        temp->next = elem;
        temp->prev ->next = temp;

        list->num_members++;
        return TRUE;    
    }
}




 void My402ListUnlink(My402List* list, My402ListElem* elem){

    if (My402ListLength(list)==1){

        elem->obj = NULL;
        free(elem);

        list->anchor.next = NULL;
        list->anchor.prev = NULL;
    }else{


        My402ListElem *left = elem->prev;
        My402ListElem *right = elem->next;


        left->next = right;
        right->prev = left;

        elem->obj = NULL;
        free(elem);
    }

    list->num_members --;

 }


 void My402ListUnlinkAll(My402List* list){


    My402ListElem *elem = My402ListFirst(list);

    while (elem!= NULL){
        My402ListElem *temp = elem;
        elem = My402ListNext(list,elem);

        My402ListUnlink(list,temp);
    }

 }




int My402ListLength (My402List* list){
    return list->num_members;
}


int My402ListEmpty(My402List* list){
    return list->num_members <= 0;
}

My402ListElem *My402ListFirst(My402List* list){
    return list->anchor.next;
}

My402ListElem *My402ListLast(My402List* list){
    return list->anchor.prev;
}

My402ListElem *My402ListNext(My402List* list, My402ListElem* elem){
    if ( elem == My402ListLast(list)){
        return NULL;
    }

    return elem->next;
}
My402ListElem *My402ListPrev(My402List* list, My402ListElem* elem){
    if (elem == My402ListFirst(list)){
        return NULL;
    }

    return elem->prev;
}






My402ListElem *My402ListFind(My402List* list , void* obj){
    for (My402ListElem * elem = My402ListFirst(list);
         elem != NULL;
         elem = My402ListNext(list,elem)
        ){
            if (elem->obj == obj)
                return elem;
        }

        return NULL;

}



void My402ListForEach (My402List* list, void (*each_operation)(void *)){
    for (My402ListElem *elem = My402ListFirst(list);
         elem!= NULL;
         elem = My402ListNext(list,elem)
        ){
            each_operation(elem->obj);
        }
}



int My402ListInit(My402List* list){
   
    list->anchor.next = NULL;
    list->anchor.prev = NULL;
    list->anchor.obj = NULL;

    list->num_members = 0 ;
    return TRUE;
}


