#pragma once

#include "block.h"
#include "p_val.h"

#include <iostream>

namespace prob {

/* Get a branch in intermediary, run tests using Monte Carlo to understand which
 * branch is most likely used */

class probability {

public:
  probability(const block b);
  probability() = default;
  void start();

private:
  pVal m_prob{};
  uint16_t m_cond_count;
  block m_b;
};
} // namespace prob
