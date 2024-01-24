#include <bits/stdc++.h>
#include <sys/time.h>

using namespace std;

//===================================//
//           TOOL FUNCTIONS          //
//===================================//
// #define COUT_EN true 
#define COUT_EN false 
// #define TIME_EN true 
#define TIME_EN false

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
#define table_width 7

enum gate_type{INV, NOR, NAND};
enum wire_type{INPUT, OUTPUT, INTERNAL};
//===================================//
//         GLOBAL DECLARATION        //
//===================================//
// lib index
vector<double> index_1;
vector<double> index_2;

// INV lib table
double INV_I_capacitance;
vector<vector<double>> INV_cell_rise;
vector<vector<double>> INV_cell_fall;
vector<vector<double>> INV_rise_power;
vector<vector<double>> INV_fall_power;
vector<vector<double>> INV_rise_transition;
vector<vector<double>> INV_fall_transition;

// NOR lib table
double NOR_A1_capacitance;
double NOR_A2_capacitance;
vector<vector<double>> NOR_cell_rise;
vector<vector<double>> NOR_cell_fall;
vector<vector<double>> NOR_rise_power;
vector<vector<double>> NOR_fall_power;
vector<vector<double>> NOR_rise_transition;
vector<vector<double>> NOR_fall_transition;

// NAND lib table
double NAND_A1_capacitance;
double NAND_A2_capacitance;
vector<vector<double>> NAND_cell_rise;
vector<vector<double>> NAND_cell_fall;
vector<vector<double>> NAND_rise_power;
vector<vector<double>> NAND_fall_power;
vector<vector<double>> NAND_rise_transition;
vector<vector<double>> NAND_fall_transition;

// I/O file
ifstream input_netlist;
ifstream input_pattern;
ifstream input_library;
ofstream output_load;
ofstream output_gate_info;
ofstream output_gate_power;
ofstream output_coverage;

// class
class GATE;
class WIRE;

// sub functions
void calculate_gate_cell_delay(GATE* gate);
void calculate_wire_transition_time(WIRE* wire);

// global container
vector<GATE*> gate_vector;
vector<WIRE*> wire_vector;
unordered_map<string, WIRE*> wire_map;

// global variables
int pattern_num = 0;
int toggle_sum = 0;
double total_power = 0;
double toggle_coverage = 0;
//===================================//
//          CLASS DEFINATION         //
//===================================//
class GATE{
public:
    // Class Data //
    WIRE* A1;
    WIRE* A2;
    WIRE* ZN;
    string gate_name;
    int    gate_sum;
    int    gate_type;
    int    toggle_01_num;
    int    toggle_10_num;
    bool   toggle;
    bool   out_value;
    bool   calculation_done;
    double output_load;
    double cell_delay;
    double total_delay;
    double transition_time;
    double input_transition;
    double internal_power;
    double switch_power;

    // Class Constructor //
    GATE(int gate_type_in, string gate_name_in){
        A1 = NULL;
        A2 = NULL;
        ZN = NULL;
        toggle_01_num = 0;
        toggle_10_num = 0;
        toggle = 0;
        out_value = 0;
        calculation_done = 0;
        cell_delay = 0;
        total_delay = 0;
        output_load = 0;
        transition_time = 0;
        input_transition = 0;
        internal_power = 0;
        switch_power = 0;

        gate_type = gate_type_in;
        gate_name = gate_name_in;
        gate_name_in.erase(gate_name_in.begin());
        gate_sum = stoi(gate_name_in);
    }

private:
};
class WIRE{
    public:
        // Class Data //
        int    wire_type;
        string wire_name;
        bool   wire_value;
        double output_load;
        double transition_time;
        GATE*       source;
        vector<GATE*> sink;
        bool   calculation_done;

        // Class Constructor //
        WIRE(int wire_type_in, string wire_name_in){
            wire_type = wire_type_in;
            wire_name = wire_name_in;
            transition_time = 0;
            wire_value = 0;
            if(wire_type_in == OUTPUT) output_load = 0.03;
            else                       output_load = 0;
            if(wire_type_in == INPUT) calculation_done = true;
            else                      calculation_done = false;
        }

        // Class Function //
        void add_sink(GATE* add_gate, double output_load_in){
            sink.push_back(add_gate);
            output_load += output_load_in;
            return;
        }
        void set_output_load(){
            // cout << wire_name << " " << wire_type << " set_start" << endl;
            if(wire_type != INPUT) {
            // if(source != NULL) {
                // cout << source->gate_name << " ";    
                // show_text("-> set output load");
                source->output_load = output_load;
                source->switch_power = (output_load * 0.9 * 0.9) / 2;
            }
            // cout << wire_name << " set_done" << endl;
            return;        
        }

    private:
};

//===================================//
//        FUNCTION DEFINATION        //
//===================================//
double interpolation(int gate_type, int table_type, bool fall_or_rise, double output_load, double input_transition){
    int x1 = -1;
    int x2 = -1;
    int y1 = -1;
    int y2 = -1;
    
    vector<vector<double>> table;
    if(gate_type == INV){
        if     (table_type == 0 && !fall_or_rise) table = INV_cell_fall;
        else if(table_type == 0 && fall_or_rise)  table = INV_cell_rise;
        else if(table_type == 1 && !fall_or_rise) table = INV_fall_power;
        else if(table_type == 1 && fall_or_rise)  table = INV_rise_power;
        else if(table_type == 2 && !fall_or_rise) table = INV_fall_transition;
        else if(table_type == 2 && fall_or_rise)  table = INV_rise_transition;

    }
    else if(gate_type == NOR){
        if     (table_type == 0 && !fall_or_rise) table = NOR_cell_fall;
        else if(table_type == 0 && fall_or_rise)  table = NOR_cell_rise;
        else if(table_type == 1 && !fall_or_rise) table = NOR_fall_power;
        else if(table_type == 1 && fall_or_rise)  table = NOR_rise_power;
        else if(table_type == 2 && !fall_or_rise) table = NOR_fall_transition;
        else if(table_type == 2 && fall_or_rise)  table = NOR_rise_transition;
    }
    else {
        if     (table_type == 0 && !fall_or_rise) table = NAND_cell_fall;
        else if(table_type == 0 && fall_or_rise)  table = NAND_cell_rise;
        else if(table_type == 1 && !fall_or_rise) table = NAND_fall_power;
        else if(table_type == 1 && fall_or_rise)  table = NAND_rise_power;
        else if(table_type == 2 && !fall_or_rise) table = NAND_fall_transition;
        else if(table_type == 2 && fall_or_rise)  table = NAND_rise_transition; 
    }

    // search index 1 
    if (output_load < index_1[0]){
        y1 = 0;
        y2 = 1;
    }
    else if(output_load > index_1[index_1.size() - 1]){
        y1 = index_1.size() - 2;
        y2 = index_1.size() - 1;
    }
    else{
        for(size_t i = 0; i < index_1.size() - 1; i++){
            if(output_load == index_1[i]){
                y1 = i;
                y2 = i;
                i = index_1.size() - 1;
            }
            else if(output_load == index_1[i + 1]){
                y1 = i + 1;
                y2 = i + 1;
                i = index_1.size() - 1;
            }
            else if(output_load > index_1[i] && output_load < index_1[i + 1]){
                y1 = i;
                y2 = i + 1;
                i = index_1.size() - 1;
            }
        }
    }

    // search index 2 
    if (input_transition < index_2[0]){
        x1 = 0;
        x2 = 1;
    }
    else if(input_transition > index_2[index_2.size() - 1]){
        x1 = index_2.size() - 2;
        x2 = index_2.size() - 1;
    }
    else{
        for(size_t i = 0; i < index_2.size() - 1; i++){
            if(input_transition == index_2[i]){
                x1 = i;
                x2 = i;
                i = index_2.size() - 1;
            }
            else if(input_transition == index_2[i + 1]){
                x1 = i + 1;
                x2 = i + 1;
                i = index_2.size() - 1;
            }
            else if(input_transition > index_2[i] && input_transition < index_2[i + 1]){
                x1 = i;
                x2 = i + 1;
                i = index_2.size() - 1;
            }
        }
    }

    // calculate internal value
    double a1 = table[x1][y1];
    double a2 = table[x1][y2];
    double b1 = table[x2][y1];
    double b2 = table[x2][y2];
    double tmp1;
    double tmp2;
    double tmp;

    if (output_load < index_1[0]){
        tmp = (index_1[y1] - output_load) / (index_1[y2] - index_1[y1]);
        tmp1 = a1 - ((a2 - a1) * tmp);
        tmp2 = b1 - ((b2 - b1) * tmp);
    }
    else if (output_load > index_1[index_1.size() - 1]){
        tmp = (output_load - index_1[y2]) / (index_1[y2] - index_1[y1]);
        tmp1 = a2 + ((a2 - a1) * tmp);
        tmp2 = b2 + ((b2 - b1) * tmp);
    }
    else if(y1 == y2){
        tmp1 = a1;
        tmp2 = b1;        
    }
    else{
        tmp = (output_load - index_1[y1]) / (index_1[y2] - index_1[y1]);
        tmp1 = a1 + ((a2 - a1) * tmp);
        tmp2 = b1 + ((b2 - b1) * tmp);
        // tmp1 = a1;
        // tmp2 = b1;
    }

    double number;
    double number_tmp = (tmp2 - tmp1) / (index_2[x2] - index_2[x1]);
    if (input_transition < index_2[0])
    {
        number = tmp1 - number_tmp * (index_2[x1] - input_transition);
    }
    else if (input_transition > index_2[index_2.size() - 1])
    {
        number = tmp2 + number_tmp * (input_transition - index_2[x2]);
    }
    else if(x1 == x2){
        number = tmp1;
    }
    else
    {
        number = tmp1 + number_tmp * (input_transition - index_2[x1]);
        // number = tmp1;
    }
    if(number < 0) return 0;
    else           return number;
};

bool gate_compare(GATE* gate_1, GATE* gate_2){
    return gate_1->gate_sum < gate_2->gate_sum;
}

void calculate_gate_info(GATE* gate){
    // if(gate->cell_delay != 0) return;
    if(gate->calculation_done) return;
    else {
        // int gate_type = gate->gate_type;
        bool A1_value = 0;
        bool A2_value = 0;
        bool pre_value = gate->out_value;
        double A1_total_delay = 0;
        double A2_total_delay = 0;
        double input_total_delay = 0;
        double A1_transition_time = 0;
        double A2_transition_time = 0;
        double input_transition_time = 0;

        // A1 input transition time / total delay / value
        if(gate->A1->wire_type != INPUT){
            // if(gate->A1->transition_time == 0) 
            if(!gate->A1->calculation_done)
                calculate_wire_transition_time(gate->A1);
            A1_total_delay = gate->A1->source->total_delay;
        }
        A1_value = gate->A1->wire_value;
        A1_transition_time = gate->A1->transition_time;
        // A2 input transition time / total delay / value
        if(gate->gate_type != INV){
            if(gate->A2->wire_type != INPUT){
                // if(gate->A2->transition_time == 0)
                if(!gate->A2->calculation_done)
                    calculate_wire_transition_time(gate->A2);
                A2_total_delay = gate->A2->source->total_delay;    
            }
            A2_value = gate->A2->wire_value;
            A2_transition_time = gate->A2->transition_time;
        }

        // get gate out value
        if(gate->gate_type == NOR)       gate->out_value = !(A1_value || A2_value);
        else if(gate->gate_type == NAND) gate->out_value = !(A1_value && A2_value);
        else                             gate->out_value = !A1_value;
        gate->toggle = (pre_value != gate->out_value);
        gate->ZN->wire_value = gate->out_value;

        // get input transition time
        if(A1_total_delay > A2_total_delay){
            if((gate->gate_type == NOR && A2_value) || (gate->gate_type == NAND && !A2_value)){
                input_total_delay = A2_total_delay;
                input_transition_time = A2_transition_time;                
            }
            else {
                input_total_delay = A1_total_delay;
                input_transition_time = A1_transition_time;                  
            }
        }
        else {
            if((gate->gate_type == NOR && !A1_value) || (gate->gate_type == NAND && A1_value)){
                input_total_delay = A2_total_delay;
                input_transition_time = A2_transition_time;                
            }
            else {
                input_total_delay = A1_total_delay;
                input_transition_time = A1_transition_time;                  
            }            
        }
        
        if(gate->toggle || gate->input_transition != input_transition_time || gate->input_transition == 0){
            gate->cell_delay = interpolation(gate->gate_type, 0, gate->out_value, gate->output_load, input_transition_time);
            gate->internal_power = interpolation(gate->gate_type, 1, gate->out_value, gate->output_load, input_transition_time);
            gate->transition_time = interpolation(gate->gate_type, 2, gate->out_value, gate->output_load, input_transition_time);
        }
        gate->calculation_done = true;
        gate->input_transition = input_transition_time;
        gate->total_delay = input_total_delay + gate->cell_delay;

        // accumulate total power
        if(gate->toggle) total_power += (gate->internal_power + gate->switch_power);
        else             total_power += (gate->internal_power);
        
        // accumulate toggle time
        if(!pre_value && gate->out_value && gate->toggle_01_num < 20){
            toggle_sum += 1;
            gate->toggle_01_num += 1;
        } 
        else if(pre_value && !gate->out_value && gate->toggle_10_num < 20){
            toggle_sum += 1;
            gate->toggle_10_num += 1;
        }

        return;
    }
}

void calculate_wire_transition_time(WIRE* wire){
    if(wire->calculation_done) return;
    else{
        if(!wire->source->calculation_done) 
            calculate_gate_info(wire->source);    
        wire->calculation_done = true;                
        wire->transition_time = wire->source->transition_time;
        return;
    }
    // if(wire->wire_type == INPUT) return;
    // else if(wire->transition_time != 0) return;
    // else {
    //     if(wire->source->transition_time == 0)
    //         calculate_gate_info(wire->source);
    //     wire->transition_time = wire->source->transition_time;
    //     return;
    // }
}

string skip_line(int skip_line_num){
    string line;
    for(int i = 0; i < skip_line_num; i++) getline(input_library, line);
    // line.erase(remove(line.begin(), line.end(), ' '), line.end());
    return line;
}

void read_table(vector<vector<double>>* table){
    int line_idx = 0;
    double value = 0;
    vector<double> value_vector;
    string line = "";
    string word = "";

    for(int i = 0; i < table_width; i++){
        getline(input_library, line);
        line_idx = line.find("\"") + 1;
        line.erase(0, line_idx);
        for(int j = 0; j < table_width; j++){
            if(j != 6) line_idx = line.find(',');
            else       line_idx = line.find('"');
            word = line.substr(0, line_idx);
            value = stod(word);
            value_vector.push_back(value);
            line.erase(0, line_idx + 1);
        }
        table->push_back(value_vector);
        value_vector.clear();
    }
    return;
}

void read_input_net(){
    int line_idx = 0;
    string line = "";
    string word = "";
    getline(input_pattern, line);
    
    line.erase(remove(line.begin(), line.end(), ' '), line.end());

    line_idx = line.find("input") + 5;
    line.erase(0, line_idx);

    while(!line.empty()) {
        if(line.find(',') != string::npos) {
            line_idx = line.find(',');
            word = line.substr(0, line_idx);
            line.erase(0, line_idx + 1);
        }
        else {
            word = line;
            line = "";
        }
        WIRE* new_wire = new WIRE(INPUT, word);
        wire_map[word] = new_wire;
        wire_vector.push_back(new_wire);
    }
    return;
}

void read_netlist(){
    while (!input_netlist.eof()){
        int line_idx = 0;
        int line_offset = 0;
        string line = "";
        string word = "";
        getline(input_netlist, line);
        
        line.erase(remove(line.begin(), line.end(), ' '), line.end());

        // clean /* */
        while(line.find("/*") != string::npos){
            string next_line = "";
            next_line = line;

            line_idx = line.find("/*");
            line = line.substr(0, line_idx);
            
            while(next_line.find("*/") == string::npos){
                getline(input_netlist, next_line);
            }

            line_idx = next_line.find("*/");
            next_line.erase(0, line_idx + 2);
            line = line + next_line;
        }
        
        // clean //
        if(line.find("//") != string::npos){
            line_idx = line.find("//");
            line = line.substr(0, line_idx);
        }

        // creater gate inverter
        if(line.find("INVX1") != string::npos ){
            line_idx = line.find("INVX1") + 5;
            line_offset = line.find("(") - line_idx ;
            word = line.substr(line_idx, line_offset);

            GATE *new_gate = new GATE(INV, word);
            WIRE *connect_wire;

            // ZN pin
            line_idx = line.find(".ZN(") + 4;
            word = line.substr(line_idx);
            line_idx = word.find(")");
            word = word.substr(0, line_idx);
            connect_wire = wire_map[word];
            if(connect_wire == NULL) {
                WIRE* new_wire = new WIRE(2, word);
                wire_map[word] = new_wire;
                connect_wire = new_wire;
            }
            connect_wire->source = new_gate;
            new_gate->ZN = connect_wire;

            // I pin
            line_idx = line.find(".I(") + 3;
            word = line.substr(line_idx);
            line_idx = word.find(")");
            word = word.substr(0, line_idx);
            connect_wire = wire_map[word];
            if(connect_wire == NULL) {
                WIRE* new_wire = new WIRE(2, word);
                wire_map[word] = new_wire;
                connect_wire = new_wire;
            }
            connect_wire->add_sink(new_gate, INV_I_capacitance);
            new_gate->A1 = connect_wire;

            gate_vector.push_back(new_gate);
        } 

        // creater gate NOR2
        else if(line.find("NOR2X1") != string::npos ){ 
            line_idx = line.find("NOR2X1") + 6;
            line_offset = line.find("(") - line_idx;
            word = line.substr(line_idx, line_offset);

            GATE *new_gate = new GATE(NOR, word);
            WIRE *connect_wire;

            // ZN pin
            line_idx = line.find(".ZN(") + 4;
            word = line.substr(line_idx);
            line_idx = word.find(")");
            word = word.substr(0, line_idx);
            connect_wire = wire_map[word];
            if(connect_wire == NULL) {
                WIRE* new_wire = new WIRE(2, word);
                wire_map[word] = new_wire;
                connect_wire = new_wire;
            }
            connect_wire->source = new_gate;
            new_gate->ZN = connect_wire;
            // A1 pin
            line_idx = line.find(".A1(") + 4;
            word = line.substr(line_idx);
            line_idx = word.find(")");
            word = word.substr(0, line_idx);
            connect_wire = wire_map[word];
            if(connect_wire == NULL) {
                WIRE* new_wire = new WIRE(2, word);
                wire_map[word] = new_wire;
                connect_wire = new_wire;
            }
            connect_wire->add_sink(new_gate, NOR_A1_capacitance);
            new_gate->A1 = connect_wire;
            // A2 pin
            line_idx = line.find(".A2(") + 4;
            word = line.substr(line_idx);
            line_idx = word.find(")");
            word = word.substr(0, line_idx);
            connect_wire = wire_map[word];
            if(connect_wire == NULL) {
                WIRE* new_wire = new WIRE(2, word);
                wire_map[word] = new_wire;
                connect_wire = new_wire;
            }
            connect_wire->add_sink(new_gate, NOR_A2_capacitance);
            new_gate->A2 = connect_wire;

            gate_vector.push_back(new_gate);
        } 

        // creater gate NAND2
        else if(line.find("NANDX1") != string::npos ){ 
            line_idx = line.find("NANDX1") + 6;
            line_offset = line.find("(") - line_idx;
            word = line.substr(line_idx, line_offset);
            
            GATE *new_gate = new GATE(NAND, word);
            WIRE *connect_wire;

            // ZN pin
            line_idx = line.find(".ZN(") + 4;
            word = line.substr(line_idx);
            line_idx = word.find(")");
            word = word.substr(0, line_idx);
            connect_wire = wire_map[word];
            if(connect_wire == NULL) {
                WIRE* new_wire = new WIRE(2, word);
                wire_map[word] = new_wire;
                connect_wire = new_wire;
            }
            connect_wire->source = new_gate;
            new_gate->ZN = connect_wire;
            // A1 pin
            line_idx = line.find(".A1(") + 4;
            word = line.substr(line_idx);
            line_idx = word.find(")");
            word = word.substr(0, line_idx);
            connect_wire = wire_map[word];
            if(connect_wire == NULL) {
                WIRE* new_wire = new WIRE(2, word);
                wire_map[word] = new_wire;
                connect_wire = new_wire;
            }
            connect_wire->add_sink(new_gate, NAND_A1_capacitance);
            new_gate->A1 = connect_wire;
            // A2 pin
            line_idx = line.find(".A2(") + 4;
            word = line.substr(line_idx);
            line_idx = word.find(")");
            word = word.substr(0, line_idx);
            connect_wire = wire_map[word];
            if(connect_wire == NULL) {
                WIRE* new_wire = new WIRE(2, word);
                wire_map[word] = new_wire;
                connect_wire = new_wire;
            }
            connect_wire->add_sink(new_gate, NAND_A2_capacitance);
            new_gate->A2 = connect_wire;

            gate_vector.push_back(new_gate);
        } 

        // creater output wire
        else if(line.find("output") != string::npos ){
            line_idx = line.find("output") + 6;
            line.erase(0, line_idx);

            while(line.find(';') != string::npos) {
                if(line.find(',') != string::npos) line_idx = line.find(',');
                else                               line_idx = line.find(';');
                word = line.substr(0, line_idx);
                WIRE* new_wire = new WIRE(OUTPUT, word);
                wire_map[word] = new_wire;
                line.erase(0, line_idx + 1);
            }
        } 
    }
    return;
}

void read_library(){
    while (!input_library.eof()){
        int line_idx = 0;
        double value = 0;
        vector<double> value_vector;
        string line = "";
        string word = "";
        getline(input_library, line);

        // read index
        if(line.find("index") != string::npos ){
            line_idx = line.find("(\"") + 2;
            line.erase(0, line_idx);
            // read index 1
            while(line.find('"') != string::npos) {
                if(line.find(',') != string::npos) line_idx = line.find(',');
                else                               line_idx = line.find('"');
                word = line.substr(0, line_idx);
                value = stod(word);
                index_1.push_back(value);
                line.erase(0, line_idx + 1);
            }

            getline(input_library, line);
            line_idx = line.find("(\"") + 2;
            line.erase(0, line_idx);
            // read index 2
            while(line.find('"') != string::npos) {
                if(line.find(',') != string::npos) line_idx = line.find(',');
                else                               line_idx = line.find('"');
                word = line.substr(0, line_idx);
                value = stod(word);
                index_2.push_back(value);
                line.erase(0, line_idx + 1);
            }   
        }

        // read INV lib
        else if(line.find("INV") != string::npos ){
            // read pin I
            line = skip_line(3);
            line_idx = line.find(": ") + 2;
            line.erase(0, line_idx);
            line_idx = line.find(';');
            word = line.substr(0, line_idx);
            value = stod(word);
            INV_I_capacitance = value;

            // read power
            line = skip_line(6);
            read_table(&INV_rise_power);
            line = skip_line(2);
            read_table(&INV_fall_power);
            // read time
            line = skip_line(4);
            read_table(&INV_cell_rise);
            line = skip_line(2);
            read_table(&INV_cell_fall);            
            // read transition
            line = skip_line(2);
            read_table(&INV_rise_transition);
            line = skip_line(2);
            read_table(&INV_fall_transition);
        }

        // read NOR lib
        else if(line.find("NOR2X1") != string::npos ){
            // read pin A1
            line = skip_line(3);
            line_idx = line.find(": ") + 2;
            line.erase(0, line_idx);
            line_idx = line.find(';');
            word = line.substr(0, line_idx);
            value = stod(word);
            NOR_A1_capacitance = value;

            // pin A2
            line = skip_line(4);
            line_idx = line.find(": ") + 2;
            line.erase(0, line_idx);
            line_idx = line.find(';');
            word = line.substr(0, line_idx);
            value = stod(word);
            NOR_A2_capacitance = value;

            // read power
            line = skip_line(6);
            read_table(&NOR_rise_power);
            line = skip_line(2);
            read_table(&NOR_fall_power);
            // read time
            line = skip_line(4);
            read_table(&NOR_cell_rise);
            line = skip_line(2);
            read_table(&NOR_cell_fall);            
            // read transition
            line = skip_line(2);
            read_table(&NOR_rise_transition);
            line = skip_line(2);
            read_table(&NOR_fall_transition);
        }

        // read NAND lib
        else if(line.find("NANDX1") != string::npos ){
            // read pin A1
            line = skip_line(3);
            line_idx = line.find(": ") + 2;
            line.erase(0, line_idx);
            line_idx = line.find(';');
            word = line.substr(0, line_idx);
            value = stod(word);
            NAND_A1_capacitance = value;

            // pin A2
            line = skip_line(4);
            line_idx = line.find(": ") + 2;
            line.erase(0, line_idx);
            line_idx = line.find(';');
            word = line.substr(0, line_idx);
            value = stod(word);
            NAND_A2_capacitance = value;

            // read power
            line = skip_line(6);
            read_table(&NAND_rise_power);
            line = skip_line(2);
            read_table(&NAND_fall_power);
            // read time
            line = skip_line(4);
            read_table(&NAND_cell_rise);
            line = skip_line(2);
            read_table(&NAND_cell_fall);            
            // read transition
            line = skip_line(2);
            read_table(&NAND_rise_transition);
            line = skip_line(2);
            read_table(&NAND_fall_transition);
        }

    }
    return;
}

//===================================//
//           MAIN FUNCTION           //
//===================================//
int main(int argc, char *argv[]){
    gettimeofday(&t_start, NULL);

    int file_name_idx;
    string write_text_1 = "";
    string write_text_2 = "";
    string student_ID = "312510152";
    // string file_path = "./output/";
    string file_path = "";
    string file_name = argv[1];
    
    file_name_idx = file_name.rfind('/');
    file_name.erase(0, file_name_idx + 1);
    file_name_idx = file_name.find(".v");
    file_name = file_name.substr(0, file_name_idx);

    input_netlist.open(argv[1]);
    input_pattern.open(argv[2]);
    input_library.open(argv[3]);
    output_load.open      (file_path + student_ID + "_" + file_name + "_load.txt");
    output_gate_info.open (file_path + student_ID + "_" + file_name + "_gate_info.txt");
    output_gate_power.open(file_path + student_ID + "_" + file_name + "_gate_power.txt");
    output_coverage.open  (file_path + student_ID + "_" + file_name + "_coverage.txt");

    //-----------------------
    // Read Pattern input net  
    //-----------------------
    read_input_net();
    // if(COUT_EN) cout << endl;
    // if(COUT_EN) cout << "-> read pattern input net done !!!" << endl;
    // show_text("-> read pattern input net done !!!");

    //------------------
    // Read Library File  
    //------------------
    read_library();
    input_library.close();
    
    // if(COUT_EN) cout << "-> read library done !!!" << endl;
    // show_text("-> read library done !!!");

    //------------------
    // Read Verilog File  
    //------------------
    read_netlist();
    input_netlist.close();
    sort(gate_vector.begin(), gate_vector.end(), gate_compare);

    // show_text("-> read netlist done !!!");

    //----------------------
    // Calculate Output Load 
    //---------------------- 
    // for(const auto& g: gate_vector){
    //     g->ZN->set_output_load();
    // }
    for(const auto& w: wire_map){
        w.second->set_output_load();
    }
    // show_text("-> output load cal !!!");
    // write output load file
    for(const auto& g: gate_vector){
        write_text_1 += g->gate_name + " " + to_string(g->output_load) + "\n";
    }
    // output_load.precision(6);
    // for(const auto& g: gate_vector){
    //     output_load << g->gate_name << " " << fixed << g->output_load << endl;
    // }
    output_load << write_text_1;
    output_load.close();
    write_text_1.clear();

    // if(COUT_EN) cout << "-> output load done !!!" << endl;
    // show_text("-> output load done !!!");
    
    string line = "";
    getline(input_pattern, line);
    while(line.find(".end") == string::npos){
        //-------------------------
        // Read Pattern Input Value
        //-------------------------
        pattern_num += 1;
        line.erase(remove(line.begin(), line.end(), ' '), line.end());
        line.erase(remove(line.begin(), line.end(), '\t'), line.end());

        int i = 0;
        for(const auto& w: wire_vector){
            if(line[i] == '0') w->wire_value = 0;
            else               w->wire_value = 1;
            i++;
        }
        getline(input_pattern, line);

        //----------------------------
        // Calculate Gate Info & Power
        //----------------------------
        for(const auto& g: gate_vector){
            if(!g->calculation_done) calculate_gate_info(g);
        }
        for(const auto& g: gate_vector){
            write_text_1 += g->gate_name
                         + " " + to_string(g->out_value)
                         + " " + to_string(g->cell_delay)
                         + " " + to_string(g->transition_time)
                         + "\n";
            write_text_2 += g->gate_name
                         + " " + to_string(g->internal_power)
                         + " " + to_string(g->switch_power)
                         + "\n";
        }
        write_text_1 += "\n";
        write_text_2 += "\n";

        if(COUT_EN) cout << "-> PAT." << pattern_num;
        // show_text(" cell info & power done !!!");

        //----------------------------------------
        // Calculate Total Power & Toggle Coverage 
        //----------------------------------------
        double toggle_coverage = (double(toggle_sum) * 100) / (gate_vector.size() * 40);
        // write path file
        output_coverage.precision(6);
        output_coverage << pattern_num << fixed << " " 
                        << total_power << " " ;
        output_coverage.precision(2);                
        output_coverage << toggle_coverage << "%" << endl;
        // output_coverage << toggle_coverage << "% " << toggle_sum << endl;
        output_coverage << endl;

        // reset value
        for(const auto& g: gate_vector){
            g->calculation_done = false;
            g->ZN->calculation_done = false;
        }
        total_power = 0;

        if(COUT_EN) cout << "-> PAT." << pattern_num;
        // show_text(" toggle coverage done !!!");
    }

    output_gate_info << write_text_1;
    output_gate_power << write_text_2;
    output_gate_info.close();
    output_gate_power.close();
    output_coverage.close();
    input_pattern.close();

    // show_text("-> task done !!!"); 
    return 0;
    }
