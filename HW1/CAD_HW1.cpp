#include <bits/stdc++.h>
#include <sys/time.h>

using namespace std;

//===================================//
//           TOOL FUNCTIONS          //
//===================================//
#define COUT_EN true 
// #define COUT_EN false 
#define TIME_EN true 
// #define TIME_EN false

#define NONE     "\e[0m"
#define YELLOW_L "\e[0;33m"
#define YELLOW_S "\e[1;33m"

struct timeval t_start, t_current;
double time_current, time_intervel;

void show_text(string text_in){
    if(COUT_EN) cout << text_in << endl;
    if(TIME_EN) {
        time_intervel = time_current;

        gettimeofday(&t_current,NULL);
        time_current = (t_current.tv_sec - t_start.tv_sec) + (double)(t_current.tv_usec - t_start.tv_usec)/1000000.0;
        time_intervel = time_current - time_intervel;
        cout << setw(6) << YELLOW_S << "   time"
                        << NONE << " = " 
                        << YELLOW_L << time_current 
                        << NONE << ", " 
                        << YELLOW_L << time_intervel 
                        << NONE << endl;
    }
};
//===================================//
//         DEFINE DECLARATION        //
//===================================//

//===================================//
//         GLOBAL DECLARATION        //
//===================================//

int var_num;
// int min_tern = 0;
int min_literal = 0;
string str_in;

class implicant;

vector<implicant> on_set;
vector<implicant> prime_implicant;
vector<implicant> implicant_current;
vector<implicant> implicant_next;
// minimal cover
vector<string> prime_implicant_set;
// vector<vector<string>> POS;
vector<vector<implicant>> POS;
// vector<implicant>         SOP;
// set<implicant>            SOP;
set<string>               SOP;
vector<implicant>         SOP_essential;
vector<string>            minimum_cover;
// vector<set<string>> SOP;


//===================================//
//          CLASS DEFINATION         //
//===================================//
class implicant{
public:
    implicant(string set_in, int group_in, int literal_in){
        set = set_in;
        prime = true;
        group = group_in;
        literal = literal_in;
    };

    void change_prime(bool prime_in){
        prime = prime_in;
        return;
    }
    
    string get_set(){
        return set;
    };

    void change_set(string set_in){
        set = set_in;
        return;
    };

    int get_group(){
        return group;
    };

    int get_literal(){
        return literal;
    }

    bool get_prime(){
        return prime;
    };

private:
    int group;
    int literal;
    bool prime;
    string set;
};

//===================================//
//        FUNCTION DEFINATION        //
//===================================//

// void SOP_tern_compute(vector<implicant> &SOP_tern, int index, int max_index, int literal);
void SOP_tern_compute(set<string> &SOP_tern, int index, int max_index, int literal);

bool equ(implicant imp_1, implicant imp_2){
    return (imp_1.get_set() == imp_2.get_set());
}

//===================================//
//           MAIN FUNCTION           //
//===================================//
int main(int argc, char *argv[]){
	ifstream input_file;
    ofstream output_file;
    input_file.open(argv[1]);
    output_file.open(argv[2]);

    string write_text = "";

    input_file >> str_in >> var_num >> str_in;
    // find prime implicant

    // read implicant
    while(input_file >> str_in)
    {
        if(str_in != ".d")
        {
            int index = 0;
            int value = 0;
            int group = 0;
            string set;
            // ASCII to Dec
            while (str_in[index] != NULL){
                value = value * 10 + (int(str_in[index]) - 48);
                index++;
            }
            // Dec to Binary string
            for (int i = 0; i < var_num; i++){
                if (value % 2 == 1)
                {
                    group++;
                    set += '1';
                }
                else
                    set += '0';
                value = value / 2;
            }
            implicant new_implicant(set, group, var_num);
            implicant_current.push_back(new_implicant);
        }
        else on_set = implicant_current;
    }

    // sort implicant
    for(size_t i = 0; i < implicant_current.size(); i++)
    {
        for (size_t j = i; j < implicant_current.size(); j++)
        {
            if (implicant_current[i].get_group() > implicant_current[j].get_group())
            {
                implicant tmp_implicant = implicant_current[i];
                implicant_current[i] = implicant_current[j];
                implicant_current[j] = tmp_implicant;
            }
        }
    }

    // prime implicant generate
    while(implicant_current.size() > 0)
    {
        for (size_t i = 0; i < implicant_current.size(); i++)
        {
            // bool merge_flag = false;
            for (size_t j = i + 1; j < implicant_current.size(); j++)
            {
                string set_a = implicant_current[i].get_set();
                string set_b = implicant_current[j].get_set();
                int bit_diff = 0;
                string merge_set;

                // gray code check
                for (int k = 0; k < var_num; k++)
                {
                    if(set_a == "1__0"){
                    }
                    if (set_a[k] != set_b[k])
                    {
                        bit_diff++;
                        merge_set += '_';
                    }
                    else {
                        merge_set += set_a[k];
                    }
                        
                    // loop end condition
                    if (bit_diff > 1) 
                        k = var_num;
                }
                // jump over the same implicant
                if (implicant_current[j].get_group() - implicant_current[i].get_group() == 0 && bit_diff == 0)
                {
                    i = j;
                }
                // merge implicant
                else if (implicant_current[j].get_group() - implicant_current[i].get_group() == 1 && bit_diff == 1)
                {
                    implicant_current[i].change_prime(false);
                    implicant_current[j].change_prime(false);

                    bool set_diff = true;
                    for(size_t k = 0; k < implicant_next.size(); k++){
                        if(implicant_next[k].get_set() == merge_set){
                            k = implicant_next.size();
                            set_diff = false;
                        }
                    }
                    if(set_diff){
                        implicant new_implicant(merge_set, implicant_current[i].get_group(), implicant_current[i].get_literal() - 1);
                        implicant_next.push_back(new_implicant);
                    }

                }
                else if (implicant_current[j].get_group() - implicant_current[i].get_group() > 1)
                    j = implicant_current.size();
            }
            // prime implicant check
            if (implicant_current[i].get_prime())
            {
                prime_implicant.push_back(implicant_current[i]);
            }
        }
        implicant_current = implicant_next;
        implicant_next.clear();
    }
  
    // sort prime implicant
    for (size_t i = 0; i < prime_implicant.size() - 1; i++)
    {
        for (size_t j = i + 1; j < prime_implicant.size(); j++)
        {
            int index = var_num - 1;
            string set_a = prime_implicant[i].get_set();
            string set_b = prime_implicant[j].get_set();
            while (set_a[index] == set_b[index])
            {
                index--;
            }
            if (set_b[index] == '1' || (set_b[index] == '0' && set_a[index] == '_'))
            {
                implicant tmp_implicant = prime_implicant[i];
                prime_implicant[i] = prime_implicant[j];
                prime_implicant[j] = tmp_implicant;
            }
        }
    }

    // output the first 15 prime implicant 
    write_text = write_text + ".p " + to_string(prime_implicant.size()) + "\n";
    for (size_t i = 0; i < prime_implicant.size(); i++)
    {
        string var_set;

        string out_set =  prime_implicant[i].get_set();
        for(int j = 0; j < var_num; j++)
        {
            if (out_set[var_num - j - 1] == '1'){
                if (i < 15) write_text += char(65 + j);
                var_set += char(65 + j);
            } 
            else if (out_set[var_num - j - 1] == '0'){
                if (i < 15) {
                    write_text += char(65 + j);
                    write_text += "'";
                }
                var_set += char(65 + j);
                var_set += "'";
            }
        }
        if (i < 15) write_text += "\n";
        
        prime_implicant_set.push_back(var_set);
    }

    for (size_t i = 0; i < on_set.size(); i++)
    {
        // vector<string> POS_tern;
        vector<implicant> POS_tern;
        for(size_t j = 0; j < prime_implicant.size(); j++)
        {
            bool bit_diff = false;
            string set_a = on_set[i].get_set();
            string set_b = prime_implicant[j].get_set();
            for(int k = 0; k < var_num; k++)
            {
                if(set_b[k] != '_' && set_b[k] != set_a[k])
                {
                    bit_diff = true;
                    k = var_num;
                } 
            }
            if(!bit_diff) POS_tern.push_back(prime_implicant[j]);
        }
        POS.push_back(POS_tern);
    }

    set<string> SOP_tern;
    SOP_tern_compute(SOP_tern, 0, POS.size(), 0);


    for(auto sop: SOP){
        minimum_cover.push_back(sop);
    }
    for (size_t i = 0; i < minimum_cover.size() - 1; i++)
    {
        for (size_t j = i + 1; j < minimum_cover.size(); j++)
        {
            int index = var_num - 1;
            string set_a = minimum_cover[i];
            string set_b = minimum_cover[j];
            while (set_a[index] == set_b[index])
            {
                index--;
            }
            if (set_b[index] == '1' || (set_b[index] == '0' && set_a[index] == '_'))
            {
                string tmp_set = minimum_cover[i];
                minimum_cover[i] = minimum_cover[j];
                minimum_cover[j] = tmp_set;
            }
        }
    }

    write_text = write_text + "\n.mc " + to_string(SOP.size()) + "\n";
    for(auto mc: minimum_cover){
        string var_set;

        string out_set =  mc;
        for(int j = 0; j < var_num; j++)
        {
            if (out_set[var_num - j - 1] == '1'){
                write_text += char(65 + j);
                var_set += char(65 + j);
            } 
            else if (out_set[var_num - j - 1] == '0'){
                write_text += char(65 + j);
                write_text += "'";
                var_set += char(65 + j);
                var_set += "'";
            }
        }
        write_text += "\n";
    }
    
    write_text = write_text + "literal=" + to_string(min_literal);

    output_file << write_text;
    input_file.close();
    output_file.close();
    return 0;
    }

void SOP_tern_compute (set<string> &SOP_tern, int index, int max_index, int literal){
    if(min_literal != 0 && literal > min_literal){
        return;
    }
    else if(index == max_index){
        SOP = SOP_tern;
        min_literal = literal;
    }
    else {
        for(auto& pos_element: POS[index]){
            if(SOP_tern.find(pos_element.get_set()) == SOP_tern.end()){
                SOP_tern.insert(pos_element.get_set());
                literal = literal + pos_element.get_literal();
                SOP_tern_compute(SOP_tern, index+1, max_index, literal);

                SOP_tern.erase(pos_element.get_set());
                literal = literal - pos_element.get_literal();
            }
            else SOP_tern_compute(SOP_tern, index+1, max_index, literal);
        }    
    }
    return;
}
