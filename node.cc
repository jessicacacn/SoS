#include "node.h"
#include "scenario.h"
#include "simulation.h"
#include "constants.h"

#include <istream>
#include <ostream>
#include <random>
#include <string>

using std::istream;
using std::ostream;
using std::string;

Node::Node()
  : state_(0),
    log_return_(0.0f) {}

Node::~Node() {}

//Generate Scenario
std::random_device Node::random_device_;
std::mt19937 Node::random_generator_(random_device_());
std::uniform_real_distribution<float> Node::uniform_(0, 1);
std::normal_distribution<float> Node::normal_(0, 1);

void Node::TransFrom(const Node* node) {
  int old_state = node->state_;
  log_return_ = node->GetMean() +
	node->GetSigma() * GenerateNormalRand();
  state_ = (old_state + (GenerateUniformRand() <=
			 kTransProb[old_state])) % 2;
}

float Node::GenerateNormalRand() {
  return normal_(random_generator_);
}

float Node::GenerateUniformRand() {
  return uniform_(random_generator_);
}

const float Node::GetMean() const {
  return kMean[state_];
}

const float Node::GetSigma() const {
  return kSigma[state_];
}

void Node::OutputStateToStream(ostream& stream) const {
  stream << state_ << " ";
}

void Node::OutputLogReturnToStream(ostream& stream) const {
  stream << log_return_ << " ";
}
//

//ESG by input file
void Node::InputFromStream(istream& state_stream, istream& log_return_stream) {
  state_stream >> state_;
  log_return_stream >> log_return_;
}
//

const float Node::GetLogReturn() const {
  return log_return_;
}

const int Node::GetState() const {
  return state_;
}

void Node::OutputToStream(ostream& stream) const {
  stream << state_ << " " << log_return_ << " ";
}

