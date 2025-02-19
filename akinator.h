struct tree_branch
{
    char* text;
    tree_branch* left;
    tree_branch* right;
};

void play_akinator(char* fin_name);

int ask_question(tree_branch* head);

tree_branch* new_branch_construct();

tree_branch* tree_parse(char* fin_name);
int recursive_tree_parse(char* buff, tree_branch* tree_head, int* currptr);

int losing_protocole(tree_branch* head);

void print_existed_values(tree_branch* tree);

void recursive_tree_print(const tree_branch* branch, FILE* fout);
void tree_print (const tree_branch* branch);

int recursive_tree_commit(const tree_branch* head, char* buffer, int* buffer_cntr);
int tree_commit(const tree_branch* head, char* fout_name);

void give_info(char* name, tree_branch* head);
int recursive_give_info(char* name, tree_branch* branch, stack_t* val_stack);


void compare_two_things(char* name1, char* name2, tree_branch* head);

void recursive_tree_make(tree_branch* branch);

void make_new_tree();

void print_different_parts_of(char* name, stack_t* _stack, stack_type first_elem);

void print_compare_of(char* name1, stack_t* stack_1, char* name2, stack_t* stack_2);


