#include "simulation.h"

#include <cmath>
#include <iostream>
#include <istream>
#include <memory>
#include <ostream>
#include <vector>

#include "constants.h"
#include "scenario.h"

using std::cerr;
using std::cout;
using std::endl;
using std::istream;
using std::ostream;
using std::string;
using std::unique_ptr;
using std::vector;

Simulation::Simulation(int num_scenarios,
		       int num_nodes)
  : num_scenarios_(num_scenarios),
    num_nodes_(num_nodes) {}

Simulation::~Simulation() {}

void Simulation::ESG() {
  for (int i = 0; i < num_scenarios_; ++i) {
    Scenario* esg_scenario = new Scenario(i, num_nodes_);
    esg_scenario->ESG();
    esg_scenarios_.push_back(unique_ptr<const Scenario>(esg_scenario));
  }
}

void Simulation::OutputStateToStream(ostream& stream) const {
  for (const auto& scenario : esg_scenarios_) {
    scenario->OutputStateToStream(stream);
    stream << endl;
  }
}

void Simulation::OutputLogReturnToStream(ostream& stream) const {
  for (const auto& scenario : esg_scenarios_) {
    scenario->OutputLogReturnToStream(stream);
    stream << endl;
  }
}

void Simulation::InputFromStream(istream& state_stream,
				 istream& log_return_stream) {
  for (int i = 0; i < num_scenarios_; ++i) {
    Scenario* scenario = new Scenario(i, num_nodes_);
    scenario->InputFromStream(state_stream, log_return_stream);
    scenarios_.push_back(unique_ptr<const Scenario>(scenario));
  }
}

void Simulation::OutputToStream(ostream& stream) const {
  for (const auto& scenario : scenarios_) {
    scenario->OutputToStream(stream);
    stream << endl;
  }
}

void Simulation::OutputScenarioResultsToStream(SoSType sos_type,
					        ostream& stock_stream,
					       ostream& hedge_stream,
					       ostream& delta_stream,
					       ostream& bond_stream,
					       ostream& rebal_stream,
					       ostream& liab_stream,
					       ostream& pv_stream) const {
  string filename_insert;
  if (sos_type == RSLN) {
    filename_insert = "RSLN";
  }
  else if (sos_type == BS) {
    filename_insert = "BS";
  }
  
  int simulation_counter=0;
  /* vector<float> pv_rebal_costs;
     float all_pv_rebal_cost=0;*/
  
  for (const auto& scenario : scenarios_) {
    vector<float> stock_prices;
    vector<float> hedges;
    vector<float> deltas;
    vector<float> bonds;
    vector<float> hedge_gls;
    vector<float> rebal_costs;
    vector<float> liab_payouts;
    vector<float> cash_flows;
    float pv_cf;
    float pv_rebal_cost;
    scenario->GetScenarioResult(sos_type,
				 &stock_prices,
				 &hedges,
				 &deltas,
				 &bonds,
				&hedge_gls,
				 &rebal_costs,
				 &liab_payouts,
				 &cash_flows,
				  &pv_cf,
				&pv_rebal_cost);
     for (unsigned int i = 0; i < stock_prices.size(); ++i) {
       stock_stream << stock_prices[i] << " ";
       hedge_stream << hedges[i] << " ";
       delta_stream << deltas[i] << " ";
       bond_stream << bonds[i] << " ";
       rebal_stream << rebal_costs[i] << " ";
       liab_stream << liab_payouts[i] << " ";
     }
     stock_stream << endl;
     hedge_stream << endl;
     delta_stream << endl;
     bond_stream << endl;
     rebal_stream << endl;
     liab_stream << endl;
     pv_stream << pv_cf << " " << pv_rebal_cost << endl;
     /* pv_rebal_costs.push_back(pv_rebal_cost);
	all_pv_rebal_cost += pv_rebal_cost;*/
     
    simulation_counter += 1;
    cout << filename_insert << " simulation " << simulation_counter << endl;
  }
  /* float avg_pv_rebal_cost = all_pv_rebal_cost / kNumRWScenarios;
  float sum_var_pv_rebal_cost = 0;
  for (int i = 0; i < kNumRWScenarios; ++i) {
    float pv_rebal_cost_i = pv_rebal_costs[i];
    sum_var_pv_rebal_cost += (pv_rebal_cost_i - avg_pv_rebal_cost)
      * (pv_rebal_cost_i - avg_pv_rebal_cost);    
  }
  float se_pv_rebal_cost = sqrt(sum_var_pv_rebal_cost / (kNumRWScenarios - 1))
    / sqrt(kNumRWScenarios);
    pv_stream << avg_pv_rebal_cost << " " << se_pv_rebal_cost << endl;*/
  
}

/*
float Simulation::GetAverageStockPrice(float initial_price, int month) const {
  float sum = 0.0;
  int count = 0;
  for (const auto& scenario : scenarios_) {
    sum += scenario->GetStockPrice(initial_price,  kNumMonths - month);
    ++count;
  }
  if (count == 0) {
    cerr << "No simulation found." << endl;
    exit(kExitFailure);
  }
  return sum / count;
}


float Simulation::GetAverageLiability(float initial_price, int month,
				      float shock) const {
  float sum = 0.0;
  int count = 0;
  for (const auto& scenario : scenarios_) {
    sum += scenario->GetLiability(initial_price, month, shock);
    ++count;
  }
  if (count == 0) {
    cerr << "No simulation found." << endl;
    exit(kExitFailure);
  }
  return sum / count;
}
*/


