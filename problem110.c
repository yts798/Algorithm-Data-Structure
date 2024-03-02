#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#define INITIAL_MAXSIZE 4

typedef struct stack stk;

struct stack{
	int maxsize;
    int top;
    char *items;
};

typedef struct stackint stkin;

struct stackint{
	int maxsize;
    int top;
    int *items;
};


stk *create_stack(void);
stkin *create_int_stack(void);
void resize_stack(stk *stack, int new_capacity);
int is_empty(stk *stack);
int size(stk *stack);
void push(stk *stack, char item);
char pop(stk *stack);
void pushint(stkin *stack, int item);
int popint(stkin *stack);
char peek(stk *stack);
void free_stack(stk *stack);
stk *reverse_stack(stk *stack);
stkin *reverse_int_stack(stkin *stack);
void print_stack(stk *stack);
stk *examine_opr(stk *stack, int *well_form, int *rank);
int calc_exp(stk *stack_opr, stk *stack_val, int high_rank);

int
main(int argc, char *argv[]) {
    stk *stack_opr = create_stack(), *stack_val = create_stack();
    char get;
    int invalid_char=0, well_form=0, rank=0, ans;
    while ((get=getchar())!=EOF) {
        if (get=='('||get==')'||get=='+'||get=='-'||get=='*'||get=='/') {
            push(stack_opr, get);
        }
        else if ((48<=get)&&(get<=57)) {
            push(stack_opr, '$');
            push(stack_val, get);
        }
        else if (get=='\n') {
        }   
        else {
            invalid_char = 1;
        }
    }
    if(size(stack_val)==0) {
        invalid_char = 1;
    }
    if (invalid_char==1) {
        printf("NotWellFormed\n");
    }
    else {
        stack_opr = reverse_stack(stack_opr);
        stack_val = reverse_stack(stack_val);
        stack_opr = examine_opr(stack_opr, &well_form, &rank);
        //printf("Hey stack_opr is still alive: ");
        //print_stack(stack_opr);
        if (!well_form&&!invalid_char) {
            printf("NotWellFormed\n");
        }
        else {
            printf("Let's calculate it!\n");
            printf("the highest rank is %d\n", rank);
            ans = calc_exp(stack_opr, stack_val, rank);
            printf("the answer of arithmetic expression is %d\n", ans);
        }
    }
    return 0;
}

stk *create_stack(void) {
    stk *new_stack;
    new_stack =  (stk*)malloc(sizeof(*new_stack));
    assert(new_stack!=NULL);
    new_stack->maxsize = 0;
    new_stack->top = 0;
    new_stack->items = NULL;
    resize_stack(new_stack, INITIAL_MAXSIZE);
    return new_stack;
}

stkin *create_int_stack(void) {
    stk *new_stack;
    new_stack =  (stk*)malloc(sizeof(*new_stack));
    assert(new_stack!=NULL);
    new_stack->maxsize = 0;
    new_stack->top = 0;
    new_stack->items = NULL;
    resize_stack(new_stack, INITIAL_MAXSIZE);
    return new_stack;
}

void resize_stack(stk *stack, int new_capacity) {
    void *new_array = realloc(stack->items, sizeof(*(stack->items))*new_capacity);
    assert(new_array!=NULL);
    stack->items=new_array;
    stack->maxsize = new_capacity;
}

int is_empty(stk *stack) {
    return stack->top==0;
}

int size(stk *stack) {
    return stack->top;
}

void push(stk *stack, char item) {
    if (stack->top==stack->maxsize) {
        resize_stack(stack, 2*stack->maxsize);
    }
    stack->items[stack->top++] = item;
}

char pop(stk *stack) {
    if(is_empty(stack)) {
        fprintf(stderr,"Failed to pop\n");
        exit(EXIT_FAILURE);
    }
    char item;
    item = stack->items[--stack->top];
    if (stack->top > 0 && stack->top == stack->maxsize / 4) {
        resize_stack(stack, stack->maxsize / 2);
    }
    //forget to freeeee
    return item;

}

void pushint(stk *stack, int item) {
    if (stack->top==stack->maxsize) {
        resize_stack(stack, 2*stack->maxsize);
    }
    stack->items[stack->top++] = item;
}

int popint(stk *stack) {
    if(is_empty(stack)) {
        fprintf(stderr,"Failed to pop\n");
        exit(EXIT_FAILURE);
    }
    int item;
    item = stack->items[--stack->top];
    if (stack->top > 0 && stack->top == stack->maxsize / 4) {
        resize_stack(stack, stack->maxsize / 2);
    }
    //forget to freeeee
    return item;
}

char peek(stk *stack) {
    if(is_empty(stack)) {
        fprintf(stderr,"Failed to peek\n");
        exit(EXIT_FAILURE);
    }
    return stack->items[stack->top - 1];
}

void free_stack(stk *stack) {
    free(stack->items);
    free(stack);
}

void print_stack(stk *stack) {
    int count, i;
    char val;
    count = size(stack);
    printf("Inside the stack there are: ");
    for (i=0;i<count;i++) {
        val = pop(stack);
        printf("%c", val);
    }
    printf("\n");
}

stk *reverse_stack(stk *stack) {
    int i, siz;
    stk *new_stack;
    new_stack = create_stack();
    siz = size(stack);
    for (i=0;i<siz;i++) {
        push(new_stack, pop(stack));
    }
    return new_stack;
}

stkin *reverse_stack(stkin *stack) {
    int i, siz;
    stk *new_stack;
    new_stack = create_stack();
    siz = size(stack);
    for (i=0;i<siz;i++) {
        push(new_stack, pop(stack));
    }
    return new_stack;
}

stk *examine_opr(stk *stack, int *well_form, int *rank) { 
    stk *copy_stack = create_stack();
    int siz, i, rank_bra_rt = 0, cond = 0, rank_ari=0, rank_bra_lf=0;
    int highest_rank=1, counter=0, diff=0;
    char get;
    siz=size(stack);
    get=pop(stack);
    push(copy_stack, get);
    if (siz==1 && get=='$') {
        *well_form = 1;
        highest_rank = 0;
    }
    else if (get=='(') {
        rank_bra_lf = 1;
        for(i=1;i<siz;i++) {
            get = pop(stack);
            push(copy_stack, get);
            if ((get=='(' && (cond==1 || cond==2)) ||
            (get==')' && (cond==0 || cond==3)) ||
            (get=='$' && (cond==1)) ||
            ((get=='+'||get=='-'||get=='*'||get=='/') && (cond == 0 || 
            cond == 3))) {
                *well_form = 0;
                printf("Ouch I die \n");
                break;
            }

            if(get=='(') {
                cond = 0;
                rank_bra_lf += 1;
            }
            else if (get==')') {
                rank_bra_rt += 1;
                cond = 1;
                counter = 1;
            }
            else if (get=='$') {
                cond = 2;
            }
            else if (get=='+'||get=='-'||get=='*'||get=='/') {
                cond = 3;
                rank_ari += 1;
            }
            *well_form = 1;
            diff = rank_bra_lf-rank_bra_rt;
            if (diff > highest_rank) {
                    //printf("haihiahiahaihaiahaiah");
                    highest_rank = diff;
            }
        }
    }
    
    else {
        *well_form = 0;
    }
    //replace debt with rank_rt_bra
    if (rank_bra_lf!=rank_bra_rt || rank_ari != rank_bra_lf) {
        *well_form = 0;
    }

    *rank = highest_rank;

    //printf("the rb is %d\n", rank_bra);
    //printf("the r_a is %d\n", rank_ari);
    
    //printf("the debit is %d\n", debt);
    //avoid ((8+7)), (8), (8+7+6), (8+7))
    copy_stack = reverse_stack(copy_stack);
    return copy_stack;
}

/*int calc_exp(stk *stack_opr, stk *stack_val, int rank) {
    char arith;
    int ans, val;
    if (size(stack_val)==1) {
        return pop(stack_val)-'0';
    }
    if (size(stack_val)==2) {
        pop(stack_opr);
        pop(stack_opr);
        arith = pop(stack_opr);
        printf("hhahahaha%c", arith);
        ans = pop(stack_val)-'0';
        val = pop(stack_val)-'0';
        //printf("%d\n", ans);
        if (arith=='+') {
            return ans+val;
        }
        else if (arith=='-') {
            return ans-val;
        }

        else if (arith=='*') {
            return ans*val;
        }

        else if (arith=='/') {
            return ans/val;
        }

    }
    return 110;
    
}*/

int calc_exp(stk *stack_opr, stk *stack_val, int high_rank) {
    stk *new_opr = create_stack(), *new_val = create_stack();
    char arith, num_1, num_2, get;
    int result, siz=size(stack_opr), i, rank_diff=0, comp=0, first_not_filled=0;
    if (high_rank==0) {
        get = pop(stack_val);
        //printf("the char result is %c\n", get);
        result = get-'0';
        return result;
    }
    else {
    for (i=0;i<siz;i++) {
        get = pop(stack_opr);
        if (get=='(') {
            rank_diff += 1;
        }
        else if (get==')') {
            rank_diff -= 1;
        }
        if (rank_diff==high_rank) {
            comp=1;
        }
        if (comp==1) {
            if (get=='+'||get=='-'||get=='*'||get=='/') {
                arith = get;
            }
            else if (get=='$') {
                if (!first_not_filled) {
                    num_1 = pop(stack_val);
                    //printf("Change me!");
                    first_not_filled=1;
                    //printf("cnm %c\n", num_1);
                }
                else {
                    num_2 = pop(stack_val);
                    //printf("Change me2222!");
                    //printf("cnm %c\n", num_2);
                }
            }
        }
        else {
            push(new_opr, get);
            if (get=='$') {
                push(new_val, pop(stack_val));
            }
        }
        if (get==')'&&comp) {
            comp=0;
            push(new_opr, '$');
            //printf("cnm %c\n", num_2);
            //ans= num_1-'0';
            //printf("rnm %d\n", ans);
            if(arith=='+') {
                result = (num_1-'0')+(num_2-'0');
            }
            else if(arith=='-') {
                result = (num_1-'0')-(num_2-'0');
            }
            else if(arith=='*') {
                result = (num_1-'0')*(num_2-'0');
            }
            else if(arith=='/') {
                result = (num_1-'0')/(num_2-'0');
            }
            printf("The result is %d\n", result);
            push(new_val, result+'0');
            first_not_filled=0;
        }
    }
    new_opr = reverse_stack(new_opr);
    new_val = reverse_stack(new_val);
    return calc_exp(new_opr, new_val, high_rank-1);
    }
}