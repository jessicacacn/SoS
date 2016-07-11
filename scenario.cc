#include "scenario.h"

#include <iostream>
#include <istream>
#include <memory>
#include <ostream>
#include <random>
#include <string>
#include <vector>

#include "constants.h"
#include "node.h"
#include "normal_cdf.h"
#include "simulation.h"

using std::cerr;
using std::cout;
using std::endl;
using std::istream;
using std::ostream;
using std::string;
using std::unique_ptr;
using std::vector;

std::random_device Scenario::random_device_;
std::mt19937 Scenario::random_generator_(random_device_());
std::uniform_real_distribution<float> Scenario::uniform_(0, 1);
std::normal_distribution<float> Scenario::normal_(0, 1);

Scenario::Scenario(int id, int num_nodes)
  : id_(id),
    num_nodes_(num_nodes) {}

Scenario::~Scenario() {}

void Scenario::ESG() {
  Node* prev = new Node();
 esg_nodes_.push_back(unique_ptr<Node>(prev));
 for (int i = 1; i <= num_nodes_; ++i) {
    Node* new_esg_node = new Node();
    new_esg_node->TransFrom(prev);
    prev = new_esg_node;
    esg_nodes_.push_back(unique_ptr<Node>(new_esg_node));
  }
}

void Scenario::OutputStateToStream(ostream& stream) const {
  for (const auto& node : esg_nodes_) {
    node->OutputStateToStream(stream);
  }
}

void Scenario::OutputLogReturnToStream(ostream& stream) const {
  for (const auto& node : esg_nodes_) {
    node->OutputLogReturnToStream(stream);
  }
}

void Scenario::InputFromStream(istream& state_stream,
			       istream& log_return_stream) {
  for (int i = 0; i <= num_nodes_; ++i) {
    Node* new_node = new Node();
    new_node->InputFromStream(state_stream, log_return_stream);
    nodes_.push_back(unique_ptr<Node>(new_node));
  }
}


void Scenario::GetScenarioResult(SoSType sos_type,
				 vector<float>* stock_prices,
				 vector<float>* hedges,
				 vector<float>* deltas,
				 vector<float>* bonds,
				 vector<float>* hedge_gls,
				 vector<float>* rebal_costs,
				 vector<float>* liab_payouts,
				 vector<float>* cash_flows,
				 float* pv_cf,
				 float* pv_rebal_cost)
  const {
  float sum_pv_cf=0.0f;  float sum_pv_rebal_cost=0.0f; 
  float prev_delta = 0.0, prev_bond = 0.0;
  // float time_zero_rebal = 0.0f;

  for (int i = 0; i <= num_nodes_; ++i) {
    float stock_price, hedge, delta, bond;
    float rebal_cost, liab_payout = 0.0;
    
    //RSRN Calculations
    if (sos_type == RSLN) {
      stock_price = GetStockPrice(kInitialStockPrice, i);

      float sum_liab = 0.0;
      float sum_shocked_liab = 0.0;
      for (int j = 1; j <= kNumRNScenarios; ++j) {
	float sum_log_return =  (kInterestRate
				 - kBSSigma * kBSSigma / 2)
	  * (kNumMonths - i)
	  + kBSSigma * GenerateNormalRand() * sqrt( kNumMonths - i);
	float rn_liab =  kGuaranteedValue
	  - stock_price * exp(sum_log_return) * 1;
	rn_liab =  -(rn_liab > 0.0 ? rn_liab : 0.0);
	float rn_liab_shock =  kGuaranteedValue
	  - stock_price * exp(sum_log_return) * (1.0 - kShockInterval);
	rn_liab_shock =  -(rn_liab_shock > 0.0 ? rn_liab_shock : 0.0);
	
        sum_liab += rn_liab;
	sum_shocked_liab += rn_liab_shock;
      }
      
      const float liability = sum_liab / kNumRNScenarios;
      const float shocked_liability = sum_shocked_liab / kNumRNScenarios;
      
      const int months_left = kNumMonths - i;
      const float total_interest = exp(-kInterestRate * months_left);
      hedge = -liability * total_interest;
      delta = -(liability - shocked_liability)
	/ (kShockInterval * stock_price) * total_interest;
      bond = hedge - delta * stock_price;
    }
    
    else if (sos_type == BS) {
      stock_price = GetStockPrice(kInitialStockPrice, i);     
      //BS formula
      float d1 = 1/(kBSSigma * sqrt(kNumMonths - i))*
	(log(stock_price / kGuaranteedValue)+
	 (kInterestRate + kBSSigma * kBSSigma / 2)*(kNumMonths - i));
      float d2 = d1 - kBSSigma * sqrt(kNumMonths - i);
      delta = - phi(-d1);
      bond = phi(-d2) * kGuaranteedValue *
	exp(-kInterestRate*(kNumMonths-i));
      hedge = delta * stock_price + bond;
      //
    }
    
    if (i==kNumMonths){
      hedge = 0.0;
      delta = 0.0;
      bond = 0.0;
    }
    
    rebal_cost = -(delta - prev_delta) * stock_price
      -(bond - prev_bond * exp(kInterestRate));
    if (i == kNumMonths) {
      liab_payout =
	std::max(kGuaranteedValue - stock_price, 0.0f);
      liab_payout = -liab_payout;
    }
    float cash_flow = liab_payout + rebal_cost;

    const float discounted_cf = cash_flow * exp(-kInterestRate * i);
    const float discounted_rebal_cost = rebal_cost * exp(-kInterestRate * i);
    sum_pv_cf += discounted_cf;
    sum_pv_rebal_cost += discounted_rebal_cost;
    
    prev_delta = delta;
    prev_bond = bond;
    
    stock_prices->push_back(stock_price);
    hedges->push_back(hedge);
    deltas->push_back(delta);
    bonds->push_back(bond);
    rebal_costs->push_back(rebal_cost);
    liab_payouts->push_back(liab_payout);
    cash_flows->push_back(cash_flow);

    /*  if (i==0) {
      time_zero_rebal = rebal_cost;
      } */
  }
  *pv_cf=sum_pv_cf;
   *pv_rebal_cost=sum_pv_rebal_cost;
  //*pv_rebal_cost = sum_pv_rebal_cost - time_zero_rebal;
}

//generate random numbers
float Scenario::GenerateNormalRand() {
  return normal_(random_generator_);
}

float Scenario::GenerateUniformRand() {
  return uniform_(random_generator_);
}
//

int Scenario::GetState(int month) const {
  return nodes_[month]->GetState();
}

float Scenario::GetStockPrice(float initial_price, int month) const {
  if (month < 0 || unsigned(month) > nodes_.size()) {
    cerr << "There are " << nodes_.size()
	 << " nodes, but trying to access index " << month << endl;
    exit(kExitFailure);
  }
  float sum_log_return = 0.0;
  for (int i = 1; i <= month; ++i) {
    sum_log_return += nodes_[i]->GetLogReturn();
  }
  return initial_price * exp(sum_log_return);
}

float Scenario::GetLiability(float initial_price, int month,
			     float shock) const {
  const float liability = kGuaranteedValue -
    GetStockPrice(initial_price, month) * shock;
  return -(liability > 0.0 ? liability : 0.0);
}

float Scenario::GetRNStockPrice(float initial_price, int month,
				vector<float> rn_log_returns) const {
  float sum_log_return = 0.0;
  for (int i = 0; i < month; ++i) {
    sum_log_return += rn_log_returns[i];
  }
  return initial_price * exp(sum_log_return);
}

float Scenario::GetRNLiability(float initial_price, int month,
			       float shock, vector<float> rn_log_returns) const
{
  const float liability = kGuaranteedValue -
    GetRNStockPrice(initial_price, month, rn_log_returns) * shock;
  return -(liability > 0.0 ? liability : 0.0);
}


void Scenario::OutputToStream(ostream& stream) const {
  for (const auto& node : nodes_) {
    node->OutputToStream(stream);
  }
}




