#ifndef NODE_H_
#define NODE_H_

#include <istream>
#include <ostream>
#include <random>
#include <string>

#include "constants.h"
class Node {
 public:
  Node();  // Constructor with no parameter
  ~Node();  // Destructor.

  void OutputToStream(std::ostream& stream) const;
  void InputFromStream(std::istream& state_stream,
		       std::istream& log_return_stream);

  void TransFrom(const Node* node); 

  const float GetLogReturn() const;
  const int GetState() const;

  const float GetMean() const;
  const float GetSigma() const;

  void OutputStateToStream(std::ostream& stream) const;
  void OutputLogReturnToStream(std::ostream& stream) const;
  
 protected:

  int state_;

  
 private:
  static float GenerateNormalRand();
  static float GenerateUniformRand();

  static std::random_device random_device_; 
  static std::mt19937 random_generator_;
  static std::uniform_real_distribution<float> uniform_;
  static std::normal_distribution<float> normal_;

  float log_return_;
};



#endif  // NODE_H_
