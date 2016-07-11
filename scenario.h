#ifndef SCENARIO_H_
#define SCENARIO_H_

#include <istream>
#include <memory>
#include <ostream>
#include <random>
#include <string>
#include <vector>

#include "constants.h"
#include "node.h"

class Node;
class Simulation;

class Scenario {
 public:
  
  Scenario(int id, int num_nodes);
  ~Scenario();

  int GetState(int month) const;
  float GetStockPrice(float initial_price, int month) const;
  float GetLiability(float initial_price, int month, float shock) const;
  int size() const { return nodes_.size(); }

  float GetRNStockPrice(float initial_price, int month,
			std::vector<float> rn_log_returns) const;
  float GetRNLiability(float initial_price, int month,
		       float shock, std::vector<float> rn_log_returns) const;
  
  void OutputToStream(std::ostream& stream) const;
  void InputFromStream(std::istream& state_stream,
		       std::istream& log_return_stream);

  void ESG();
  void OutputStateToStream(std::ostream& stream) const;
  void OutputLogReturnToStream(std::ostream& stream) const;

  void GetScenarioResult(SoSType sos_type,
			 std::vector<float>* stock_prices,
			 std::vector<float>* hedges,
			 std::vector<float>* deltas,
			 std::vector<float>* bonds,
			 std::vector<float>* hedge_gls,
			 std::vector<float>* rebal_costs,
			 std::vector<float>* liab_payouts,
			 std::vector<float>* cash_flows,
			 float* pv_cf, float* pv_rebal_cost) const;

 private:
  const int id_;
  const int num_nodes_;
  std::vector<std::unique_ptr<Node>> nodes_;
  std::vector<std::unique_ptr<Node>> esg_nodes_;

  static float GenerateNormalRand();
  static float GenerateUniformRand();

  static std::random_device random_device_;
  static std::mt19937 random_generator_;
  static std::uniform_real_distribution<float> uniform_;
  static std::normal_distribution<float> normal_;
};

#endif  // SCENARIO_H_
