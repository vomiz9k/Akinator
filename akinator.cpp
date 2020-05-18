#include "musthave_include.h"

char* AUTOBUFF = 0;
int MAXNAMESIZE = 1000;

#define ROBO_SAY(str, arg)                                 \
{                                                          \
    AUTOBUFF = (char*)calloc(MAXNAMESIZE, 1);              \
    printf(str, arg);                                      \
    sprintf(AUTOBUFF, "espeak \"" str "\"", arg);          \
    system(AUTOBUFF);                                      \
    free(AUTOBUFF);                                        \
}

void play_akinator(char* fin_name)
{
    tree_branch* branch = tree_parse(fin_name);

    char c = 0;

    while(1)
    {
        ROBO_SAY("Choose gamemode: \n","");

        printf("\t1: play with me\n");
        printf("\t2: get information about somebody\n");
        printf("\t3: compare two things\n");
        printf("\t4: see the whole tree\n");
        printf("\t5: update tree file\n");
        printf("\t6: make a new tree\n");

        scanf(" %c", &c);

        putchar('\n');
        switch (c)
        {
            case '1':
                ask_question(branch);
            break;

            case '2':
            {
                ROBO_SAY("Who do you want to know about? Choose:\n", "");
                print_existed_values(branch);
                char* str = (char*)calloc(sizeof(char), MAXNAMESIZE);
                scanf(" %[^\n]", str);
                give_info(str, branch);

                free(str);
            }
            break;

            case '3':
            {
                ROBO_SAY("Who do you want to compare? Choose:\n", "");
                print_existed_values(branch);

                char* name1 = (char*)calloc(sizeof(char), MAXNAMESIZE);
                scanf(" %[^\n]", name1);

                char* name2 = (char*)calloc(sizeof(char), MAXNAMESIZE);
                scanf(" %[^\n]", name2);

                compare_two_things(name1, name2, branch);
                free(name1);
                free(name2);
            }
            break;

            case '4':
                tree_print(branch);
            break;

            case '5':
                tree_commit(branch, fin_name);
            break;

            case '6':
            {
                make_new_tree();
                return;
            }
            break;

            default:

                ROBO_SAY("Wrong comma bro\n", "");
            break;
        }


        ROBO_SAY("Try again?\n", "");
        scanf(" %c", &c);

        putchar('\n');
        if (c != 'Y' && c != 'y')
        {
            system("espeak \"Goodbye, human creature\"");
            break;
        }
    }
}

tree_branch* tree_parse(char* fin_name)
{
    assert(fin_name);

    int file_size = 0;
    char* buff = file_read(fin_name, &file_size);
    tree_branch* tree_head = new_branch_construct();
    int currptr = 0;

    recursive_tree_parse(buff, tree_head, &currptr);
    return tree_head;
}

int recursive_tree_parse(char* buff, tree_branch* tree_head, int* currptr)
{
    tree_head->text = strtok(buff + *currptr + 1, "\"");

    *currptr += strlen(tree_head->text) + 3;
    if (buff[*currptr] != '}' && tree_head->text)
    {
        tree_head->left = new_branch_construct();
        recursive_tree_parse(buff, tree_head->left, currptr);

        tree_head->right = new_branch_construct();
        recursive_tree_parse(buff, tree_head->right, currptr);
    }
    (*currptr)++;
    return 0;
}

int ask_question(tree_branch* head)
{
    assert(head);
    char c = 0;

    if(head->left == 0 || head->right == 0)
    {

        ROBO_SAY("I know! This is %s, isn't it?\n", head->text);
        scanf(" %c", &c);

        if (c == 'Y' || c == 'y')
        {
            printf("I won.\n");
            system("espeak \"I WON, STUPID HUMAN CREATURE\"");
        }
        else
        {
            losing_protocole(head);

            ask_question(head);
        }
    }
    else
    {
        ROBO_SAY("Is it %s?\n", head->text);
        scanf(" %c", &c);

        if (c == 'Y' || c == 'y')
            ask_question(head->left);

        else if (c == 'N' || c == 'n')
            ask_question(head->right);
    }
    return 0;

}

tree_branch* new_branch_construct()
{
    tree_branch* branch = (tree_branch*)calloc(1, sizeof(tree_branch));
    assert(branch);

    branch->left  = 0;
    branch->right = 0;

    return branch;
}

void give_info(char* name, tree_branch* head)
{
    assert(name && head);

    stack_t val_stack = {};
    STACKCONSTRUCT(&val_stack);

    if(recursive_give_info(name, head, &val_stack))
    {
        stack_type trash = 0;
        ROBO_SAY("%s is:\n", name);

        while (val_stack.size > 0)
        {
            assert(!stack_pop(&val_stack, &trash));

            if (trash > 0)
            {
                ROBO_SAY("\t%s\n", ((tree_branch*)trash)->text);
            }
            else if (trash < 0)
            {
                ROBO_SAY("\tnot %s\n", ((tree_branch*)(-trash))->text);
            }
        }
    }
    else
    {
        ROBO_SAY("%s not found, stupid human being! I gave you the full list of things that I know!\n", name);
    }
    putchar('\n');
    stack_destruct(&val_stack);
}

int recursive_give_info(char* name, tree_branch* branch, stack_t* val_stack)
{
    stack_type trash;
    if (!strcmp(branch->text, name))
        return 1;

    else if (branch->left || branch->right)
    {
        if (recursive_give_info(name, branch->left, val_stack))
        {
            stack_push(val_stack, (int)branch);
            return 1;
        }

        if (recursive_give_info(name, branch->right, val_stack))
        {
            stack_push(val_stack, -(int)branch);
            return 1;
        }
    }

    else
        return 0;
}
void recursive_tree_make(tree_branch* branch)
{
    assert(branch);
    char c = 0;

    branch->left  = new_branch_construct();
    branch->left->text = (char*)calloc(sizeof(char), MAXNAMESIZE);

    branch->right = new_branch_construct();
    branch->right->text = (char*)calloc(sizeof(char), MAXNAMESIZE);

    printf("What if answer for \"Is it %s?\" is yes? Write a result.\n", branch->text);
    scanf(" %[^\n]", branch->left->text);
    branch->left->text = (char*)realloc(branch->left->text, strlen(branch->left->text) + 1);
    assert(branch->left->text);


    printf("What if answer for \"Is it %s?\" is no? Write a result.\n", branch->text);
    scanf(" %[^\n]", branch->right->text);
    branch->right->text = (char*)realloc(branch->right->text, strlen(branch->right->text) + 1);
    assert(branch->right->text);

    printf("Is %s the question?\n", branch->left->text);
    scanf(" %c", &c);

    if (c == 'y' || c == 'Y')
        recursive_tree_make(branch->left);

    printf("Is %s the question?\n", branch->right->text);
    scanf(" %c", &c);

    if (c == 'y' || c == 'Y')
        recursive_tree_make(branch->right);
}

void compare_two_things(char* name1, char* name2, tree_branch* head)
{
    assert(name1 && name2 && head);

    if(!strcmp(name1, name2))
    {
        ROBO_SAY("You gave me 2 same names, stupid human being! There is nothing to compare.\n", "");
        return;
    }
    stack_t stack_1 = {}, stack_2 = {};
    STACKCONSTRUCT(&stack_1);
    STACKCONSTRUCT(&stack_2);

    int first_search_result = recursive_give_info(name1, head, &stack_1);
    int second_search_result = recursive_give_info(name2, head, &stack_2);

    if(!first_search_result || !second_search_result)
    {
        if(!first_search_result)
        {
            ROBO_SAY("%s not found, stupid human being! I gave you the full list of things that I know!\n", name1);
        }
        else
        {
            ROBO_SAY("%s not found, stupid human being! I gave you the full list of things that I know!\n", name2);
        }
        return;
    };

    print_compare_of(name1, &stack_1, name2, &stack_2);

    stack_destruct(&stack_1);
    stack_destruct(&stack_2);
}

void print_different_parts_of(char* name, stack_t* _stack, stack_type first_elem)
{
    ROBO_SAY("Different parts of %s: ", name);
    while (_stack->size >= 0)
    {
        if (first_elem > 0)
        {
            ROBO_SAY("it is %s, ", ((tree_branch*)first_elem)->text);
        }
        else
        {
            ROBO_SAY("it is not %s, ", ((tree_branch*)(-first_elem))->text);
        }
        if(_stack->size > 0)
            stack_pop(_stack, &first_elem);

        else
            break;
    }
    ROBO_SAY("that\'s all.\n", "");
}

void print_compare_of(char* name1, stack_t* stack_1, char* name2, stack_t* stack_2)
{
    stack_type first_elem = 0;
    stack_type second_elem = 0;

    stack_pop(stack_1, &first_elem);
    stack_pop(stack_2, &second_elem);

    if (first_elem != second_elem)
    {
        ROBO_SAY("They have nothing in common.\n", "");
    }
    else
    {
        ROBO_SAY("Both of them are ", "");
        while (first_elem == second_elem)
        {
            if (first_elem > 0)
            {
                ROBO_SAY("%s, ", ((tree_branch*)first_elem)->text);
            }
            else
            {
                ROBO_SAY("not %s, ", ((tree_branch*)(-first_elem))->text);
            }
            if(stack_1->size > 0 && stack_2->size > 0)
            {
                stack_pop(stack_1, &first_elem);
                stack_pop(stack_2, &second_elem);
            }
            else
                break;
        }
    }
    ROBO_SAY("that\'s all.\n", "");

    print_different_parts_of(name1, stack_1, first_elem);
    print_different_parts_of(name2, stack_2, second_elem);
}


void print_existed_values(tree_branch* tree)
{
    assert(tree);

    if (!tree->left && !tree->right)
    {
        printf("\t- %s\n", tree->text);
        return;
    }
    else
    {
        print_existed_values(tree->left);
        print_existed_values(tree->right);
    }
}



int branch_count(const tree_branch* tree)
{
    if (!tree->left && !tree->right)
    {
        return 1;
    }

    else
    {
        return branch_count(tree->left) + branch_count(tree->right);
    }
}


void tree_print(const tree_branch* head)
{
    assert(head);
    FILE* fout = fopen("goutput.gv", "w");

    fprintf(fout, "digraph First{\n");
    fprintf(fout, "node [shape=\"Mrecord\", style=\"filled\", fillcolor=\"lightblue\"];");
    fprintf(fout, "\"box%0x\" [label=\"%s\"];\n", head, head->text);

    recursive_tree_print(head, fout);

    fprintf(fout, "}");
    fclose(fout);

    system("dot -Tpng goutput.gv -o foutput.png");
    system("foutput.png");
    ROBO_SAY("Stupid human being, trees are in the forest! Here is a binary tree.\n\n", "");
}

void recursive_tree_print (const tree_branch* branch, FILE* fout)
{
    if (branch->left)
    {
        fprintf(fout, "\"box%0x\" [label=\"%s\"];\n", branch->left, (branch->left)->text);
        fprintf(fout, "\"box%0x\" -> \"box%0x\"[label=\"Yes\", color=\"green\"];\n", branch, branch->left);

        recursive_tree_print(branch->left, fout);
    }

    if (branch->right)
    {
        fprintf(fout, "\"box%0x\" [label=\"%s\"];\n", branch->right, (branch->right)->text);
        fprintf(fout, "\"box%0x\" -> \"box%0x\"[label=\"No\", color=\"red\"];\n", branch, branch->right);

        recursive_tree_print(branch->right, fout);
    }
}

int losing_protocole(tree_branch* head)
{
    ROBO_SAY("I lost. What was that?\n", "");

    char* right_ans = (char*) calloc(MAXNAMESIZE, 1);
    assert(right_ans);

    scanf(" %[^\n]", right_ans);

    right_ans = (char*) realloc(right_ans, strlen(right_ans) + 1);
    assert(right_ans);

    ROBO_SAY("What is %s", right_ans);
                ROBO_SAY(" that %s isn\'t?\n", head->text);

    char* question = (char*) calloc(MAXNAMESIZE, 1);
    assert(question);

    scanf(" %[^\n]", question);

    question = (char*) realloc(question, strlen(question) + 1);
    assert(question);

    tree_branch*  left_branch = new_branch_construct();
    tree_branch* right_branch = new_branch_construct();

    left_branch->text  = right_ans;
    right_branch->text = head->text;

    head->text  = question;
    head->left  = left_branch;
    head->right = right_branch;
}


int tree_commit(const tree_branch* head, char* fout_name)
{
    assert(head && fout_name);

    char* buffer = (char*) calloc(sizeof(char), sizeof(tree_branch) + branch_count(head) * MAXNAMESIZE);
    int counter = 0;

    recursive_tree_commit(head, buffer, &counter);

    FILE * fout = fopen(fout_name, "w");
    fprintf(fout, "%s", buffer);

    fclose(fout);
    free(buffer);
}

int recursive_tree_commit(const tree_branch* head, char* buffer, int* buffer_cntr)
{
    buffer[(*buffer_cntr)++] = '{';

    if(head->left == 0 || head->right == 0)
    {
        *buffer_cntr += sprintf(buffer + *buffer_cntr, "\"%s\"}", head->text);
    }
    else
    {
        *buffer_cntr += sprintf(buffer + *buffer_cntr, "\"%s\"", head->text);
        recursive_tree_commit(head->left, buffer, buffer_cntr);

        recursive_tree_commit(head->right, buffer, buffer_cntr);
        buffer[(*buffer_cntr)++] = '}';
    }
    return 0;
}



void make_new_tree()
{
    char* filename = (char*)calloc(sizeof(char), MAXNAMESIZE);
    printf("Write name of file to save new tree to.\n");
    scanf(" %[^\n]", filename);

    tree_branch* head = new_branch_construct();
    head->text = (char*)calloc(sizeof(char), MAXNAMESIZE);

    printf("Write a first head question:\n");
    scanf(" %[^\n]", head->text);
    head->text = (char*)realloc(head->text,strlen(head->text));
    assert(head->text);

    recursive_tree_make(head);

    tree_commit(head, filename);
    char c = 0;
    printf("\nNew tree was made. Check %s. Do you wanna play it?\n", filename);
    scanf(" %c", &c);

    if(c == 'y' || c == 'Y')
    {
        play_akinator(filename);
    }
}

