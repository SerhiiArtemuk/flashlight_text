/**
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "Pool2D.h"

#include <flashlight/autograd/Functions.h>
#include <flashlight/common/Exception.h>
#include <flashlight/nn/Init.h>
#include <flashlight/nn/Utils.h>

namespace fl {

using detail::IntOrPadMode;

Pool2D::Pool2D(
    int wx,
    int wy,
    int sx,
    int sy,
    IntOrPadMode px,
    IntOrPadMode py,
    PoolingMode mode)
    : xFilter_(wx),
      yFilter_(wy),
      xStride_(sx),
      yStride_(sy),
      xPad_(px.padVal),
      yPad_(py.padVal),
      mode_(mode) {}

Variable Pool2D::forward(const Variable& input) {
  auto px = detail::derivePadding(input.dims(0), xFilter_, xStride_, xPad_);
  auto py = detail::derivePadding(input.dims(1), yFilter_, yStride_, yPad_);

  AFML_ASSERT(px >= 0 && py >= 0, "Invalid padding ", AF_ERR_ARG);

  return pool2d(input, xFilter_, yFilter_, xStride_, yStride_, px, py, mode_);
}

std::string Pool2D::prettyString() const {
  std::ostringstream ss;
  ss << "Pool2D";
  switch (mode_) {
    case PoolingMode::MAX:
      ss << "-max";
      break;
    case PoolingMode::AVG_EXCLUDE_PADDING:
      ss << "-avg(without pad)";
      break;
    case PoolingMode::AVG_INCLUDE_PADDING:
      ss << "-avg(with pad)";
      break;
  }
  ss << " (" << xFilter_ << "x" << yFilter_ << ", " << xStride_ << ","
     << yStride_ << ", ";
  if (xPad_ == static_cast<int>(PaddingMode::SAME)) {
    ss << "SAME";
  } else {
    ss << xPad_;
  }
  ss << ",";
  if (yPad_ == static_cast<int>(PaddingMode::SAME)) {
    ss << "SAME";
  } else {
    ss << yPad_;
  }
  ss << ")";
  return ss.str();
}

} // namespace fl