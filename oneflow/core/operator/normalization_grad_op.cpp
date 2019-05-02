#include "oneflow/core/operator/normalization_grad_op.h"
#include "oneflow/core/kernel/kernel_util.h"

namespace oneflow {

void NormalizationGradOp::InitFromOpConf() {
  const NormalizationGradOpConf& conf = op_conf().normalization_grad_conf();
#ifdef WITH_CUDA
  if (DevIsGpuAndEnableCudnn()) { CHECK_GE(conf.epsilon(), CUDNN_BN_MIN_EPSILON); }
#endif
  EnrollInputBn("dy", false);
  EnrollInputBn("x", false);
  CHECK_EQ(conf.has_mean(), conf.has_inv_variance());
  if (conf.has_mean() || conf.has_inv_variance()) {
    EnrollInputBn("mean", false);
    EnrollInputBn("inv_variance", false);
  }
  if (conf.has_gamma()) {
    EnrollInputBn("gamma", false);
  } else {
    if (DevIsGpuAndEnableCudnn()) {
      EnrollConstBufBn("gamma");
    } else {
      UNIMPLEMENTED();
    }
  }
  if (conf.has_dx()) {
    EnrollOutputBn("dx", false);
  } else {
    if (DevIsGpuAndEnableCudnn()) {
      EnrollOutputBn("dx", false);
    } else {
      UNIMPLEMENTED();
    }
  }
  if (conf.has_gamma_diff()) {
    EnrollOutputBn("gamma_diff", false);
  } else {
    if (DevIsGpuAndEnableCudnn()) {
      EnrollOutputBn("gamma_diff", false);
    } else {
      UNIMPLEMENTED();
    }
  }
  if (conf.has_beta_diff()) {
    EnrollOutputBn("beta_diff", false);
  } else {
    if (DevIsGpuAndEnableCudnn()) {
      EnrollOutputBn("beta_diff", false);
    } else {
      UNIMPLEMENTED();
    }
  }
}

const PbMessage& NormalizationGradOp::GetCustomizedConf() const {
  return op_conf().normalization_grad_conf();
}

void NormalizationGradOp::InferBlobDescs(
    std::function<BlobDesc*(const std::string&)> GetBlobDesc4BnInOp,
    const ParallelContext* parallel_ctx) const {
  const NormalizationGradOpConf& conf = op_conf().normalization_grad_conf();
  const BlobDesc* x = GetBlobDesc4BnInOp("x");
  const BlobDesc* dy = GetBlobDesc4BnInOp("dy");
  CHECK_EQ(dy->data_type(), x->data_type());
  CHECK_EQ(dy->shape(), x->shape());
  const DataType data_type = dy->data_type();
  BlobDesc* dx = GetBlobDesc4BnInOp("dx");
  if (dx) { *dx = *x; }
  const Shape param_shape({x->shape().At(conf.axis())});
  const auto CheckParamBlobDesc = [&](const std::string& bn) {
    const BlobDesc* blob_desc = GetBlobDesc4BnInOp(bn);
    if (blob_desc != nullptr) {
      CHECK_EQ(blob_desc->data_type(), data_type);
      CHECK_EQ(blob_desc->shape(), param_shape);
    }
  };
  const auto SetParamBlobDesc = [&](const std::string& bn) {
    BlobDesc* blob_desc = GetBlobDesc4BnInOp(bn);
    if (blob_desc != nullptr) {
      blob_desc->set_data_type(data_type);
      blob_desc->mut_shape() = param_shape;
    }
  };
  CheckParamBlobDesc("mean");
  CheckParamBlobDesc("inv_variance");
  CheckParamBlobDesc("gamma");
  SetParamBlobDesc("gamma_diff");
  SetParamBlobDesc("beta_diff");
}

REGISTER_OP(OperatorConf::kNormalizationGradConf, NormalizationGradOp);

}  // namespace oneflow
