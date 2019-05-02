#ifndef ONEFLOW_CORE_OPERATOR_GATHER_OP_H_
#define ONEFLOW_CORE_OPERATOR_GATHER_OP_H_

#include "oneflow/core/operator/operator.h"

namespace oneflow {

Shape GatherGetOutShape(const Shape& in, const Shape& indices, int64_t axis);

class GatherOp final : public Operator {
 public:
  OF_DISALLOW_COPY_AND_MOVE(GatherOp);
  GatherOp() = default;
  ~GatherOp() override = default;

  void InitFromOpConf() override;
  const PbMessage& GetCustomizedConf() const override;
  bool NeedOutBlobWhenBackward() const override { return false; }
  void InferBlobDescs(std::function<BlobDesc*(const std::string&)> GetBlobDesc4BnInOp,
                      const ParallelContext* parallel_ctx) const override;
  void VirtualGenKernelConf(std::function<const BlobDesc*(const std::string&)> GetBlobDesc4BnInOp,
                            const ParallelContext* parallel_ctx,
                            KernelConf* kernel_conf) const override;

 private:
  void GetSbpSignatures(
      const std::function<const BlobDesc&(const std::string&)>& LogicalBlobDesc4Ibn,
      SbpSignatureList* sbp_sig_list) const override;
};

}  // namespace oneflow

#endif  // ONEFLOW_CORE_OPERATOR_GATHER_OP_H_
