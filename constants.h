#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#include <cmath>

enum SoSType {
  BS,
  RSLN,
};

const int kExitSuccess = 0;
const int kExitFailure = 1;

const int kNumRWScenarios = 1000;
const int kNumRNScenarios = 1000;

const int kNumMonths = 240;
const int kMaxNumStates = 2;

const float kInterestRate = 0.003;
const float kInitialStockPrice = 1000.0;
const float kGuaranteedValue = 1000.0;
const float kShockInterval = 0.0001;


const float kTransProb[] = {0.04f, 0.2f};
const float kMean[] = {0.009167f, 0.009167f};
//const float kMean[] = {0.007f, 0.007f};
//0.009167=0.015*20/24-0.02*4/24
const float kSigma[] = {0.0457627f, 0.0457627f};

//const float kMean[] = {0.015f, -0.02f};
//const float kSigma[] = {0.035f, 0.08f};


const float kBSSigma = 0.0457627;

#endif  // CONSTANTS_H_
