//Sam Dressler
//CSCI 465
//11/15/20
//Header for generating the intermediate code in 3-address form

using namespace std;
struct icg_sym_table{
    //Lexeme
    string token_type;
	//Spelling
    string value;
};
typedef struct icg_sym_table icg_symbol;
void handle_assignment(vector<icg_symbol>::iterator);

void generate_three_address_code(vector<icg_symbol> icg_sym_table){
    cout << "IN ICG" << endl;
    icg_symbol temp;
    icg_symbol temp_look_ahead;
    icg_symbol temp_look_behind;
    vector<icg_symbol>::iterator it;
    for(it = icg_sym_table.begin(); it != icg_sym_table.end(); ++it){
        temp = *it;
        if(it != icg_sym_table.begin()){
            temp_look_behind = *(it -1);
        }
        if((it+1) != icg_sym_table.end()){
            temp_look_ahead = *(it + 1);
        }
        //cout << temp.token_type << " : " << temp.value << endl;
        string t_type = temp.token_type;
        string v_value = temp.value;
        /*
            Handle varaible declarations
        */

        /*
            Handle Simplified expressions
        */

        /*
            Handle Assignment statements 
            -Take in a icg_symbol table entry and produce 3 address code for it
        */
        if(t_type.compare("assign") == 0){
            cout << temp_look_behind.value <<" "<< temp.value << " " <<temp_look_ahead.value <<endl;
            // handle_assignment(it);
        }
        /*
            Handle read/wriite I/O calls
        */
    }
}

void handle_assignment(vector<icg_symbol>::iterator entry){
    icg_symbol temp = *entry;
    cout << temp.value << endl;
}