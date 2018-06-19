/**
 * @author Ryan Benasutti, WPI
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#include "okapi/api/filter/composableFilter.hpp"
#include <utility>

namespace okapi {
ComposableFilterArgs::ComposableFilterArgs(
  const std::initializer_list<std::shared_ptr<Filter>> &ilist)
  : list(ilist) {
}
ComposableFilter::ComposableFilter() {
}

ComposableFilter::ComposableFilter(const std::initializer_list<std::shared_ptr<Filter>> &ilist) {
  for (auto &&elem : ilist) {
    filters.push_back(elem);
  }
}

ComposableFilter::ComposableFilter(const std::vector<std::shared_ptr<Filter>> &ilist) {
  for (auto &&elem : ilist) {
    filters.push_back(elem);
  }
}

ComposableFilter::ComposableFilter(const ComposableFilterArgs &iparams) {
  for (auto &&elem : iparams.list) {
    filters.push_back(elem);
  }
}

double ComposableFilter::filter(const double ireading) {
  if (filters.size() <= 0) {
    return 0;
  }

  // Initial sensor reading
  filters.front()->filter(ireading);

  // Propagate signal
  for (std::size_t i = 1; i < filters.size(); i++) {
    filters[i]->filter(filters[i - 1]->getOutput());
  }

  output = filters.back()->getOutput();
  return output;
}

double ComposableFilter::getOutput() const {
  return output;
}

void ComposableFilter::addFilter(std::shared_ptr<Filter> ifilter) {
  filters.push_back(ifilter);
}
} // namespace okapi