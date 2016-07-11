#include <algorithm>
#include <cmath>
#include <ctime>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "constants.h"
#include "scenario.h"
#include "simulation.h"


using std::cerr;
using std::cout;
using std::endl;
using std::ios;
using std::ifstream;
using std::ofstream;
using std::clock_t;
using std::string;
using std::unique_ptr;
using std::vector;



int main(int argc, char* argv[]) {

  clock_t t1,t2;
  t1=clock();

  //RW ESG
  ofstream rw_state_output;
  ofstream rw_log_return_output;
  rw_state_output.open ("RW_State_Input.txt", ios::out);
  rw_log_return_output.open ("RW_Log_Return_Input.txt", ios::out);
  Simulation* esg_simulation = new Simulation(kNumRWScenarios, kNumMonths);
  esg_simulation->ESG();
  esg_simulation->OutputStateToStream(rw_state_output);
  esg_simulation->OutputLogReturnToStream(rw_log_return_output);
  rw_state_output.close();
  rw_log_return_output.close();

  //RW simulation
  ifstream rw_state_input;
  ifstream rw_log_return_input;
  rw_state_input.open ("RW_State_Input.txt", ios::in);
  rw_log_return_input.open ("RW_Log_Return_Input.txt", ios::in);
  Simulation* simulation = new Simulation(kNumRWScenarios, kNumMonths);
  simulation->InputFromStream(rw_state_input, rw_log_return_input);
  rw_state_input.close();
  rw_log_return_input.close();

  for (SoSType sos_type: {/*RSLN,*/ BS}) {
     ofstream rw_stock_output;
  ofstream rw_hedge_output;
  ofstream rw_delta_output;
  ofstream rw_bond_output;
  ofstream rw_rebal_output;
  ofstream rw_liab_output;
  ofstream rw_pv_output;
  
  string filename_insert;
  if (sos_type == RSLN) {
    filename_insert = "RSLN";
  }
  else if (sos_type == BS) {
    filename_insert = "BS";
  }
  
   rw_stock_output.open (string("RW_") +
			filename_insert + string("_Stock_Output.txt"),
  			ios::out);
  rw_hedge_output.open (string("RW_") +
			filename_insert + string("_Hedge_Output.txt"),
			ios::out);
  rw_delta_output.open (string("RW_") +
			filename_insert + string("_Delta_Output.txt"),
			ios::out);
  rw_bond_output.open (string("RW_") +
		       filename_insert + string("_Bond_Output.txt"),
		       ios::out);
  rw_rebal_output.open (string("RW_") +
			filename_insert + string("_Rebal_Output.txt"),
			ios::out);
  rw_liab_output.open (string("RW_") +
		       filename_insert + string("_Liab_Output.txt"),
		       ios::out);
  rw_pv_output.open (string("RW_") +
		     filename_insert + string("_PV_Output.txt"),
		     ios::out|ios::app);
  simulation->OutputScenarioResultsToStream(sos_type, 
					     rw_stock_output,
					    rw_hedge_output,
					    rw_delta_output,
					    rw_bond_output,
					    rw_rebal_output,
					    rw_liab_output,
					    rw_pv_output);
    rw_stock_output.close();
  rw_hedge_output.close();
  rw_delta_output.close();
  rw_bond_output.close();
  rw_rebal_output.close();
  rw_liab_output.close();
  rw_pv_output.close();
  }
  
  

t2=clock();
float diff ((float)t2-(float)t1);
float seconds = diff / CLOCKS_PER_SEC / 60;	
 cout << seconds << " minutes" << endl;

  return kExitSuccess;
}


  /*  //RN simulations - 2 sets - initial state 0 and 1
  vector<unique_ptr<Simulation>> rn_simulations;
  for (int initial_state : {0, 1}) {
    ifstream rn_state_input;
    ifstream rn_log_return_input;
    rn_state_input.open (string("RN_State_Input_Ini_") +
		    std::to_string(initial_state) + string(".txt"), ios::in);
    rn_log_return_input.open (string("RN_Log_Return_Input_Ini_") +
		    std::to_string(initial_state) + string(".txt"), ios::in);  
    Simulation* simulation = new Simulation(kNumRNScenarios, kNumMonths);
    simulation->InputFromStream(rn_state_input, rn_log_return_input);
    rn_simulations.push_back(unique_ptr<Simulation>(simulation));
    rn_state_input.close();
    rn_log_return_input.close();
    
    ofstream rn_output;
    rn_output.open (string("RN_Output_Ini_") +
		    std::to_string(initial_state) + string(".txt"), ios::out);
    simulation->OutputToStream(rn_output);
    rn_output.close();
  }
*/
