#ifndef SIMULATION_H_
#define SIMULATION_H_

#include <istream>
#include <memory>
#include <ostream>
#include <vector>

#include "constants.h"

class Node;
class Scenario;

class Simulation {
 public:

  Simulation(int num_scenarios,
		       int num_nodes);
  ~Simulation();

  void ESG();

  void OutputStateToStream(std::ostream& stream) const;
  void OutputLogReturnToStream(std::ostream& stream) const;
  
  void OutputToStream(std::ostream& stream) const;
  void InputFromStream(std::istream& state_stream,
		       std::istream& log_return_stream);

  void OutputScenarioResultsToStream(SoSType sos_type, 
				     std::ostream& stock_stream,
				     std::ostream& hedge_stream,
				     std::ostream& delta_stream,
				     std::ostream& bond_stream,
				     std::ostream& rebal_stream,
				     std::ostream& liab_stream,
				     std::ostream& pv_stream
				     ) const;

 private:
  std::vector<std::unique_ptr<const Scenario>> scenarios_;
  std::vector<std::unique_ptr<const Scenario>> esg_scenarios_;
  int num_scenarios_;
  int num_nodes_;
  int num_months_;
};


#endif  // SIMULATION_H_


  /*  float GetAverageStockPrice(float initial_price, int month) const;
  float GetAverageLiability(float initial_price, int month,
  float shock = 1.0) const;*/
