#include <bits/stdc++.h>
#include <sys/time.h>

using namespace std;

//===================================//
//         DEFINE DECLARATION        //
//===================================//
// #define COUT_EN true 
#define COUT_EN false 

//===================================//
//         GLOBAL DECLARATION        //
//===================================//
// lib index
vector<double> index_1 = {0.001400,0.003000,0.006200,0.012500,0.025100,0.050400,0.101000};
vector<double> index_2 = {0.0208,0.0336,0.06,0.1112,0.2136,0.4192,0.8304};

// INV lib table
double INV_I_capacitance = 0.0109115;
vector<vector<double>> INV_cell_rise = {
    {0.013379,0.016044,0.019907,0.022481,0.031726,0.040185,0.050385},
    {0.01525,0.018376,0.022972,0.026053,0.03705,0.047418,0.059875},
    {0.018871,0.022403,0.028108,0.03261,0.045789,0.059925,0.076608},
    {0.02597,0.029306,0.033033,0.042608,0.054479,0.080008,0.10394},
    {0.040047,0.040538,0.045817,0.058067,0.076355,0.09933,0.14683},
    {0.066295,0.068558,0.073341,0.083717,0.10821,0.14397,0.18904},
    {0.12223,0.12441,0.1292,0.13863,0.15947,0.20864,0.2792}
};
vector<vector<double>> INV_cell_fall = {
    {0.015975,0.018648,0.022904,0.028037,0.031885,0.041402,0.053623},
    {0.018156,0.021149,0.026094,0.032287,0.03668,0.048075,0.062383},
    {0.022594,0.02559,0.031664,0.039739,0.044932,0.059061,0.077439},
    {0.031149,0.034095,0.040822,0.051786,0.060032,0.076833,0.10231},
    {0.047947,0.047918,0.057692,0.063035,0.082424,0.10735,0.14034},
    {0.079511,0.081269,0.085301,0.093628,0.11641,0.1545,0.202},
    {0.14604,0.14772,0.15158,0.15967,0.17681,0.22372,0.29848}
};
vector<vector<double>> INV_rise_transition = {
    {0.017522,0.021682,0.028803,0.041672,0.052872,0.081109,0.12837},
    {0.02198,0.024661,0.033109,0.046939,0.060235,0.088586,0.13712},
    {0.029796,0.032708,0.038207,0.056464,0.07806,0.099037,0.15561},
    {0.044871,0.046301,0.05161,0.068661,0.10222,0.12468,0.18211},
    {0.076836,0.076772,0.076054,0.089262,0.12837,0.18676,0.23216},
    {0.1417,0.14121,0.14198,0.14212,0.17126,0.2441,0.35429},
    {0.27081,0.26681,0.26943,0.27422,0.27429,0.33228,0.47341}
};
vector<vector<double>> INV_fall_transition = {
    {0.017349,0.017893,0.024178,0.035104,0.051821,0.075654,0.12316},
    {0.018614,0.022157,0.027649,0.040185,0.059238,0.085087,0.13344},
    {0.027419,0.029372,0.034899,0.049446,0.072893,0.10429,0.15246},
    {0.043657,0.044863,0.049056,0.060723,0.091025,0.13231,0.18596},
    {0.07653,0.073661,0.079082,0.087928,0.12085,0.17316,0.24881},
    {0.13749,0.13518,0.13793,0.14913,0.16585,0.22913,0.33942},
    {0.26533,0.26248,0.26553,0.26592,0.28688,0.33103,0.45258}
};

// NOR lib table
double NOR_A1_capacitance = 0.0105008;
double NOR_A2_capacitance = 0.0108106;
vector<vector<double>> NOR_cell_rise = {
    {0.023104,0.026347,0.032439,0.037211,0.050187,0.075629,0.10893},
    {0.027062,0.029858,0.036397,0.042338,0.055823,0.083858,0.11942},
    {0.034333,0.037002,0.043649,0.050318,0.064815,0.089926,0.13827},
    {0.048838,0.051443,0.053267,0.064115,0.083493,0.11205,0.17069},
    {0.076172,0.077668,0.081485,0.08978,0.11263,0.14904,0.20359},
    {0.13351,0.13486,0.13847,0.14635,0.16278,0.20974,0.27949},
    {0.24777,0.24888,0.25234,0.25971,0.27596,0.30968,0.40393}
};
vector<vector<double>> NOR_cell_fall = {
    {0.017693,0.020473,0.024855,0.029528,0.03475,0.038561,0.030505},
    {0.019856,0.022665,0.027792,0.033415,0.03982,0.045554,0.039165},
    {0.024076,0.026936,0.032932,0.040339,0.048928,0.057779,0.054661},
    {0.03228,0.035141,0.041568,0.051796,0.064313,0.069003,0.080192},
    {0.048376,0.051227,0.057825,0.070186,0.089026,0.09852,0.11886},
    {0.078389,0.08013,0.090011,0.091689,0.11283,0.15092,0.17976},
    {0.14202,0.14372,0.14735,0.15483,0.17074,0.21341,0.2901}
};
vector<vector<double>> NOR_rise_transition = {
    {0.047372,0.047388,0.05291,0.069558,0.079757,0.11295,0.16298},
    {0.055454,0.057237,0.061359,0.076048,0.089491,0.12218,0.1786},
    {0.070803,0.069305,0.07765,0.08839,0.10706,0.15805,0.20462},
    {0.097384,0.095745,0.10667,0.11394,0.1372,0.17641,0.23705},
    {0.16606,0.16368,0.16716,0.17,0.18593,0.23581,0.33823},
    {0.2963,0.29,0.29077,0.29997,0.29596,0.34204,0.4312},
    {0.55605,0.54072,0.53969,0.54185,0.56377,0.57026,0.65233}
};
vector<vector<double>> NOR_fall_transition = {
    {0.0178,0.020672,0.025912,0.038106,0.054758,0.087109,0.14053},
    {0.02211,0.024126,0.029235,0.043016,0.061952,0.094784,0.15217},
    {0.02905,0.032076,0.035248,0.05071,0.073218,0.11045,0.17034},
    {0.045602,0.046546,0.049838,0.065112,0.089226,0.13818,0.22076},
    {0.07724,0.077419,0.079545,0.087836,0.11532,0.18049,0.25971},
    {0.13593,0.13398,0.1414,0.14681,0.16852,0.22528,0.3437},
    {0.25981,0.25461,0.25367,0.26263,0.27702,0.33235,0.44369}
};

// NAND lib table
double NAND_A1_capacitance = 0.00683597;
double NAND_A2_capacitance = 0.00798456;
vector<vector<double>> NAND_cell_rise = {
    {0.020844,0.02431,0.030696,0.039694,0.048205,0.072168,0.10188},
    {0.024677,0.027942,0.035042,0.045467,0.054973,0.082349,0.11539},
    {0.032068,0.035394,0.042758,0.055361,0.065991,0.090936,0.13847},
    {0.046811,0.049968,0.057164,0.064754,0.086481,0.11676,0.15744},
    {0.073919,0.078805,0.080873,0.091007,0.11655,0.1579,0.21448},
    {0.13162,0.13363,0.1383,0.14793,0.1685,0.22032,0.30054},
    {0.24661,0.24835,0.25294,0.26221,0.282,0.32417,0.42783}
};
vector<vector<double>> NAND_cell_fall = {
    {0.020382,0.023257,0.027894,0.033548,0.03574,0.04179,0.044857},
    {0.024029,0.026956,0.032263,0.039221,0.042085,0.050402,0.056668},
    {0.031269,0.034119,0.039869,0.049176,0.053737,0.065333,0.07578},
    {0.045259,0.047829,0.053987,0.057975,0.072805,0.089369,0.10958},
    {0.071393,0.072867,0.076097,0.082901,0.10269,0.13035,0.16091},
    {0.12714,0.12851,0.13175,0.13825,0.15292,0.19222,0.2452},
    {0.23846,0.23967,0.2429,0.24923,0.26252,0.29317,0.37116}
};
vector<vector<double>> NAND_rise_transition = {
    {0.030236,0.03225,0.038219,0.052208,0.080479,0.10719,0.15147},
    {0.039514,0.039353,0.046985,0.061163,0.087011,0.11697,0.17515},
    {0.051087,0.053959,0.058155,0.070827,0.10934,0.15223,0.20641},
    {0.085969,0.08422,0.092301,0.096621,0.12925,0.19941,0.26597},
    {0.15372,0.14705,0.15469,0.1603,0.17955,0.24852,0.35789},
    {0.28669,0.28022,0.28121,0.28888,0.27863,0.3316,0.48256},
    {0.5525,0.53592,0.53441,0.53503,0.55683,0.54271,0.65957}
};
vector<vector<double>> NAND_fall_transition = {
    {0.027802,0.028745,0.035327,0.047243,0.067704,0.097889,0.15165},
    {0.03416,0.03663,0.041513,0.054197,0.079795,0.11013,0.16545},
    {0.048764,0.050279,0.053838,0.06597,0.098021,0.13303,0.19592},
    {0.076954,0.077267,0.077694,0.08918,0.11528,0.17286,0.23832},
    {0.13307,0.13316,0.13508,0.14106,0.16249,0.2137,0.3243},
    {0.24709,0.24136,0.24516,0.25064,0.26398,0.31964,0.44603},
    {0.47444,0.46022,0.46284,0.46654,0.48133,0.48929,0.60317}
};

// I/O file
ifstream input_file;
ofstream output_load_file;
ofstream output_delay_file;
ofstream output_path_file;

// class
class GATE;
class WIRE;

// sub functions
void calculate_gate_cell_delay(GATE* gate);
void calculate_wire_transition_time(WIRE* wire);

// global container
vector<GATE*> gate_vector;
unordered_map<string, WIRE*> wire_map;

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
    bool   out_value;
    double output_load;
    double cell_delay;
    double total_delay;
    double transition_time;
    double input_transition;

    // Class Constructor //
    GATE(int gate_type_in, string gate_name_in){
        A1 = NULL;
        A2 = NULL;
        ZN = NULL;
        out_value = 0;
        cell_delay = 0;
        total_delay = 0;
        output_load = 0;
        transition_time = 0;
        input_transition = 0;

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
        double output_load;
        double transition_time;
        GATE*       source;
        vector<GATE*> sink;

        // Class Constructor //
        WIRE(int wire_type_in, string wire_name_in){
            wire_type = wire_type_in;
            wire_name = wire_name_in;
            transition_time = 0;
            source = NULL;
            if(wire_type_in == 1) output_load = 0.03;
            else                  output_load = 0;
        }

        // Class Function //
        void add_sink(GATE* add_gate, double output_load_in){
            sink.push_back(add_gate);
            output_load += output_load_in;
            if(source != NULL) source->output_load = output_load;

            return;
        }
        void set_output_load(){
            if(wire_type != 0) source->output_load = output_load;
            return;        
        }

    private:
};

//===================================//
//        FUNCTION DEFINATION        //
//===================================//
double interpolation(int gate_type, bool delay_or_transition, bool rise_or_fall, double output_load, double input_transition){
    int x1 = -1;
    int x2 = -1;
    int y1 = -1;
    int y2 = -1;
    
    vector<vector<double>> table;
    if(gate_type == 0){
        if     (delay_or_transition  && rise_or_fall)  table = INV_fall_transition;
        else if(delay_or_transition  && !rise_or_fall) table = INV_rise_transition;
        else if(!delay_or_transition && rise_or_fall)  table = INV_cell_fall;
        else if(!delay_or_transition && !rise_or_fall) table = INV_cell_rise;
    }
    else if(gate_type == 1){
        if     (delay_or_transition  && rise_or_fall)  table = NOR_fall_transition;
        else if(delay_or_transition  && !rise_or_fall) table = NOR_rise_transition;
        else if(!delay_or_transition && rise_or_fall)  table = NOR_cell_fall;
        else if(!delay_or_transition && !rise_or_fall) table = NOR_cell_rise;
    }
    else {
        if     (delay_or_transition  && rise_or_fall)  table = NAND_fall_transition;
        else if(delay_or_transition  && !rise_or_fall) table = NAND_rise_transition;
        else if(!delay_or_transition && rise_or_fall)  table = NAND_cell_fall;
        else if(!delay_or_transition && !rise_or_fall) table = NAND_cell_rise;  
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

    return number;
};

bool load_compare(GATE* gate_1, GATE* gate_2){
    if(gate_1->output_load != gate_2->output_load)
		return gate_1->output_load > gate_2->output_load;
	else{
        return gate_1->gate_sum < gate_2->gate_sum;
	}
}

bool delay_compare(GATE* gate_1, GATE* gate_2){
    if(gate_1->cell_delay != gate_2->cell_delay)
		return gate_1->cell_delay > gate_2->cell_delay;
	else{
        return gate_1->gate_sum < gate_2->gate_sum;
	}
}

void calculate_gate_cell_delay(GATE* gate){
    if(gate->cell_delay != 0) return;
    else {
        double A1_total_delay = 0;
        double A2_total_delay = 0;
        double input_total_delay = 0;
        double A1_transition_time = 0;
        double A2_transition_time = 0;
        double input_transition_time = 0;

        // A1 input transition time / total delay
        if(gate->A1->wire_type != 0){
            if(gate->A1->transition_time == 0) 
                calculate_wire_transition_time(gate->A1);
            A1_total_delay = gate->A1->source->total_delay;
        }
        A1_transition_time = gate->A1->transition_time;
        // A2 input transition time / total delay
        if(gate->gate_type != 0){
            if(gate->A2->wire_type != 0){
                if(gate->A2->transition_time == 0)
                    calculate_wire_transition_time(gate->A2);
                A2_total_delay = gate->A2->source->total_delay;    
            }
            A2_transition_time = gate->A2->transition_time;
        }

        // get input transition time
        if(A1_total_delay > A2_total_delay){
            input_total_delay = A1_total_delay;
            input_transition_time = A1_transition_time;
        }
        else if(A1_total_delay < A2_total_delay){
            input_total_delay = A2_total_delay;
            input_transition_time = A2_transition_time;
        }
        else {
            input_total_delay = A1_total_delay;
            if(A1_transition_time > A2_transition_time) input_transition_time = A1_transition_time;
            else                                        input_transition_time = A2_transition_time;
        }
        gate->input_transition = input_transition_time;

        // calculate cell delay
        double rise_delay = interpolation(gate->gate_type, 0, 0, gate->output_load, input_transition_time);
        double fall_delay = interpolation(gate->gate_type, 0, 1, gate->output_load, input_transition_time);

        // calculate transition time
        double rise_transition = interpolation(gate->gate_type, 1, 0, gate->output_load, input_transition_time);
        double fall_transition = interpolation(gate->gate_type, 1, 1, gate->output_load, input_transition_time);

        // restore calculation result
        if(rise_delay > fall_delay){
            gate->out_value = 1;
            gate->cell_delay = rise_delay;
            gate->transition_time = rise_transition;
            gate->total_delay = input_total_delay + rise_delay + 0.005;
        }  
        else{
            gate->out_value = 0;
            gate->cell_delay = fall_delay;
            gate->transition_time = fall_transition;
            gate->total_delay = input_total_delay + fall_delay + 0.005;
        }
    
        return;
    }
}

void calculate_wire_transition_time(WIRE* wire){
    if(wire->wire_type == 0) return;
    else if(wire->transition_time != 0) return;
    else {
        if(wire->source->transition_time == 0)
            calculate_gate_cell_delay(wire->source);
        wire->transition_time = wire->source->transition_time;
        return;
    }
}

void path_retrace(WIRE* wire){
    if(wire->wire_type == 0){
        output_path_file << " " << wire->wire_name;
        return;
    } 
    else{
        double cell_delay    = wire->source->cell_delay;
        double current_delay = wire->source->total_delay;
        double previous_delay_A1;
        if(wire->source->A1->wire_type == 0)
            previous_delay_A1 = 0;
        else 
            previous_delay_A1 = wire->source->A1->source->total_delay;

        if(current_delay == previous_delay_A1 + cell_delay + 0.005) 
            path_retrace(wire->source->A1);
        else 
            path_retrace(wire->source->A2);
        output_path_file << " -> " << wire->wire_name;
        return;
    }
}

void read_netlist(){
    while (!input_file.eof()){
        int line_idx = 0;
        int line_offset = 0;
        string line = "";
        string word = "";
        getline(input_file, line);
        
        line.erase(remove(line.begin(), line.end(), ' '), line.end());

        // clean /* */
        while(line.find("/*") != string::npos){
            string next_line = "";
            next_line = line;

            line_idx = line.find("/*");
            line = line.substr(0, line_idx);
            
            while(next_line.find("*/") == string::npos){
                getline(input_file, next_line);
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

            GATE *new_gate = new GATE(0, word);
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

            GATE *new_gate = new GATE(1, word);
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
            
            GATE *new_gate = new GATE(2, word);
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

        // creater input wire
        else if(line.find("input") != string::npos ){
            line_idx = line.find("input") + 5;
            line.erase(0, line_idx);

            while(line.find(';') != string::npos) {
                if(line.find(',') != string::npos) line_idx = line.find(',');
                else                               line_idx = line.find(';');
                word = line.substr(0, line_idx);
                // word = regex_replace(word, regex(R"(^\s+|\s+$)"), "");
                WIRE* new_wire = new WIRE(0, word);
                wire_map[word] = new_wire;
                line.erase(0, line_idx + 1);
            }
        } 

        // creater output wire
        else if(line.find("output") != string::npos ){
            line_idx = line.find("output") + 6;
            line.erase(0, line_idx);

            while(line.find(';') != string::npos) {
                if(line.find(',') != string::npos) line_idx = line.find(',');
                else                               line_idx = line.find(';');
                word = line.substr(0, line_idx);
                WIRE* new_wire = new WIRE(1, word);
                wire_map[word] = new_wire;
                line.erase(0, line_idx + 1);
            }
        } 

        // creater internal wire
        // else if(line.find("wire") != string::npos ){
        //     line_idx = line.find("wire") + 4;
        //     line.erase(0, line_idx);

        //     while(line.find(';') != string::npos) {
        //         if(line.find(',') != string::npos) line_idx = line.find(',');
        //         else                               line_idx = line.find(';');
        //         word = line.substr(0, line_idx);
        //         WIRE* new_wire = new WIRE(2, word);
        //         wire_map[word] = new_wire;
        //         line.erase(0, line_idx + 1);
        //     }
        // }
    
    }
}

//===================================//
//           MAIN FUNCTION           //
//===================================//
int main(int argc, char *argv[]){
    int file_name_idx;
    string student_ID = "312510152";
    string file_name = argv[1];
    
 
    file_name_idx = file_name.rfind('/');
    file_name.erase(0, file_name_idx + 1);
    file_name_idx = file_name.find(".v");
    file_name = file_name.substr(0, file_name_idx);

    input_file.open(argv[1]);
    output_load_file.open (student_ID + "_" + file_name + "_load.txt");
    output_delay_file.open(student_ID + "_" + file_name + "_delay.txt");
    output_path_file.open (student_ID + "_" + file_name + "_path.txt");
    output_load_file.precision(6);

    struct timeval t1,t2;
    double timeuse;
    gettimeofday(&t1,NULL);

    //------------------
    // Read Verilog File  
    //------------------
    read_netlist();
    input_file.close();

    if(COUT_EN) cout << endl;
    if(COUT_EN) cout << "-> input file done !!!" << endl;

    gettimeofday(&t2,NULL);
    timeuse = (t2.tv_sec - t1.tv_sec) + (double)(t2.tv_usec - t1.tv_usec)/1000000.0;
    if(COUT_EN) cout<<"time = "<<timeuse<<endl;  //輸出時間（單位：ｓ）
    
    //----------------------
    // Calculate Output Load 
    //---------------------- 
    for(const auto& w: wire_map){
        w.second->set_output_load();
    }
    sort(gate_vector.begin(), gate_vector.end(), load_compare);



    string write_text = "";
    if(COUT_EN) cout << "-> write start" << endl;
    gettimeofday(&t2,NULL);
    timeuse = (t2.tv_sec - t1.tv_sec) + (double)(t2.tv_usec - t1.tv_usec)/1000000.0;
    if(COUT_EN) cout<<"time = "<<timeuse<<endl;  //輸出時間（單位：ｓ）
    // write output load file
    output_load_file.precision(6);
    for(const auto& g: gate_vector){
        write_text += g->gate_name + " " + to_string(g->output_load) + "\n";
        // output_load_file << g->gate_name << " " << fixed << g->output_load << endl;
    }
    output_load_file << write_text;    
    if(COUT_EN) cout << "-> output load done !!!" << endl;
    gettimeofday(&t2,NULL);
    timeuse = (t2.tv_sec - t1.tv_sec) + (double)(t2.tv_usec - t1.tv_usec)/1000000.0;
    if(COUT_EN) cout<<"time = "<<timeuse<<endl;  //輸出時間（單位：ｓ）

    output_load_file.close();

    if(COUT_EN) cout << "-> output load done !!!" << endl;
    gettimeofday(&t2,NULL);
    timeuse = (t2.tv_sec - t1.tv_sec) + (double)(t2.tv_usec - t1.tv_usec)/1000000.0;
    if(COUT_EN) cout<<"time = "<<timeuse<<endl;  //輸出時間（單位：ｓ）
    




    //---------------------
    // Calculate Cell Delay
    //---------------------
    for(const auto& g: gate_vector){
        calculate_gate_cell_delay(g);
    }
    sort(gate_vector.begin(), gate_vector.end(), delay_compare);

    write_text = "";
    output_delay_file.precision(6);
    for(const auto& g: gate_vector){
        write_text += g->gate_name
                    + " " + to_string(g->out_value)
                    + " " + to_string(g->cell_delay) 
                    + " " + to_string(g->transition_time)
                    + "\n";
        // output_delay_file << g->gate_name
        // << " " << g->out_value << fixed
        // << " " << g->cell_delay 
        // << " " << g->transition_time
        // << endl;
    }
    output_delay_file << write_text;
    output_delay_file.close();

    if(COUT_EN) cout << "-> cell delay done !!!" << endl;
    gettimeofday(&t2,NULL);
    timeuse = (t2.tv_sec - t1.tv_sec) + (double)(t2.tv_usec - t1.tv_usec)/1000000.0;
    if(COUT_EN) cout<<"time = "<<timeuse<<endl;  //輸出時間（單位：ｓ）


    //------------------------------
    // Find Longest And Shorest Path 
    //------------------------------
    double max_delay = 0;
    double min_delay = 0;
    WIRE* longest_path_output  = NULL;
    WIRE* shortest_path_output = NULL;
    for(const auto& w: wire_map){
        if(w.second->wire_type == 1){
            if(max_delay == 0 || w.second->source->total_delay > max_delay){
                max_delay = w.second->source->total_delay;
                longest_path_output = w.second;
            }
            if(min_delay == 0 || w.second->source->total_delay < min_delay){
                min_delay = w.second->source->total_delay;
                shortest_path_output = w.second;
            }
        }
    }
    // write path file
    output_path_file.precision(6);
    output_path_file << "Longest delay = " << fixed << max_delay - 0.005 << ", the path is:";
    path_retrace(longest_path_output);
    output_path_file << endl;
    output_path_file << "Shortest delay = " << fixed << min_delay - 0.005 << ", the path is:";
    path_retrace(shortest_path_output);
    output_path_file << endl;

    if(COUT_EN) cout << "-> path find done !!!" << endl;

    if(COUT_EN) cout << "-> task done !!!" << endl;
    if(COUT_EN) cout << endl;
    output_path_file.close();
    return 0;
    }
