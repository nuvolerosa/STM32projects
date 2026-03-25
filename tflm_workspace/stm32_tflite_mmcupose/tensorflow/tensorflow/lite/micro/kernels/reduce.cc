/* Copyright 2019 The TensorFlow Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

#include "tensorflow/lite/kernels/internal/reference/reduce.h"

#include "tensorflow/lite/c/builtin_op_data.h"
#include "tensorflow/lite/c/common.h"
#include "tensorflow/lite/kernels/internal/quantization_util.h"
#include "tensorflow/lite/kernels/internal/tensor_ctypes.h"
#include "tensorflow/lite/kernels/internal/types.h"
#include "tensorflow/lite/kernels/kernel_util.h"

namespace tflite {
namespace ops {
namespace micro {
namespace reduce {

constexpr int kMaxNumberOfAxis = 4;
constexpr int kMaxNumberOfReducedAxis = 2;

struct OpData {
  int32_t* temp_buffer;
};

void* Init(TfLiteContext* context, const char* buffer, size_t length) {
  TFLITE_DCHECK(context->AllocatePersistentBuffer != nullptr);
  void* data = nullptr;
  if (context->AllocatePersistentBuffer(context, sizeof(OpData), &data) ==
      kTfLiteError) {
    return nullptr;
  }
  auto* op_data = static_cast<OpData*>(data);
  op_data->temp_buffer = nullptr;
  return op_data;
}

TfLiteStatus PrepareSimple(TfLiteContext* context, TfLiteNode* node) {
  // Inputs Tensor (dtype depends on quantization):
  // [0] = Input
  // [1] = Axis

  // Outputs Tensor (dtype depends on quantization):
  // [0] = Output

  // Validate number of inputs and outputs
  TF_LITE_ENSURE_EQ(context, node->inputs->size, 2);
  TF_LITE_ENSURE_EQ(context, node->outputs->size, 1);

  // Validate axis type
  const TfLiteTensor* axis = GetInput(context, node, 1);
  TF_LITE_ENSURE_TYPES_EQ(context, axis->type, kTfLiteInt32);
  return kTfLiteOk;
}

TfLiteStatus PrepareMeanOrSum(TfLiteContext* context, TfLiteNode* node) {
  TF_LITE_ENSURE_OK(context, PrepareSimple(context, node));
  auto* data = static_cast<OpData*>(node->user_data);
  TF_LITE_ENSURE(context, data != nullptr);
  TfLiteTensor* output = GetOutput(context, node, 0);
  const int output_count = NumElements(output);
  TF_LITE_ENSURE(context, output_count > 0);
  void* temp = nullptr;
  if (context->AllocatePersistentBuffer(
          context, static_cast<size_t>(output_count) * sizeof(int32_t),
          &temp) != kTfLiteOk) {
    return kTfLiteError;
  }
  data->temp_buffer = static_cast<int32_t*>(temp);
  return kTfLiteOk;
}

void ResolveAxis(const int* axis_data, int axis_count,
                 tflite::MeanParams* op_params) {
  int i = 0;
  for (; i < axis_count; ++i) {
    op_params->axis[i] = static_cast<int16>(axis_data[i]);
  }
  for (; i < 4; ++i) {
    op_params->axis[i] = 1;
  }
  op_params->axis_count = axis_count;
}

TfLiteStatus EvalMean(TfLiteContext* context, TfLiteNode* node) {
  auto* op_data = static_cast<OpData*>(node->user_data);
  TF_LITE_ENSURE(context, op_data != nullptr);

  const TfLiteTensor* input = GetInput(context, node, 0);
  const TfLiteTensor* axis = GetInput(context, node, 1);
  TfLiteTensor* output = GetOutput(context, node, 0);
  TfLiteReducerParams* params =
      reinterpret_cast<TfLiteReducerParams*>(node->builtin_data);

  int num_axis = static_cast<int>(NumElements(axis));
  int temp_index[kMaxNumberOfAxis];
  int resolved_axis[kMaxNumberOfReducedAxis];

  switch (input->type) {
    case kTfLiteFloat32: {
      tflite::MeanParams op_params;
      ResolveAxis(GetTensorData<int>(axis), num_axis, &op_params);
      // TODO(b/146571391): Support only 4D Input and 2D Axis for Mean until
      // scratch tensor allocation has been implemented in (b/132070898)
      bool is_valid_inputs =
          (NumDimensions(input) == 4 && op_params.axis_count == 2 &&
           ((op_params.axis[0] == 1 && op_params.axis[1] == 2) ||
            (op_params.axis[0] == 2 && op_params.axis[1] == 1)));
      TF_LITE_ENSURE_MSG(
          context, is_valid_inputs == true,
          "Number of Input "
          "dimensions != 4 OR the Axis is not either [1, 2] or [2, 1]");
      // TODO(b/139102329): Handle the below special case in the combined
      // reference method.
      // Defer to specialized implementation for 4D Mean across axes 1 & 2.
      if (params->keep_dims) {
        reference_ops::Mean(op_params, GetTensorShape(input),
                            GetTensorData<float>(input), GetTensorShape(output),
                            GetTensorData<float>(output));
      } else {
        TF_LITE_ENSURE(
            context,
            reference_ops::Mean(GetTensorData<float>(input), input->dims->data,
                                input->dims->size, GetTensorData<float>(output),
                                output->dims->data, output->dims->size,
                                GetTensorData<int>(axis), num_axis,
                                params->keep_dims, temp_index, resolved_axis,
                                GetTensorData<float>(output)));
      }
    } break;
    case kTfLiteInt8: {
      TF_LITE_ENSURE_TYPES_EQ(context, output->type, kTfLiteInt8);
      bool ok = reference_ops::QuantizedMeanOrSum<int8_t, int32_t>(
          GetTensorData<int8_t>(input), input->params.zero_point,
          input->params.scale, input->dims->data, input->dims->size,
          GetTensorData<int8_t>(output), output->params.zero_point,
          output->params.scale, output->dims->data, output->dims->size,
          GetTensorData<int>(axis), num_axis, params->keep_dims, temp_index,
          resolved_axis, op_data->temp_buffer, /*compute_sum=*/false);
      TF_LITE_ENSURE(context, ok);
    } break;
    case kTfLiteUInt8: {
      TF_LITE_ENSURE_TYPES_EQ(context, output->type, kTfLiteUInt8);
      bool ok = reference_ops::QuantizedMeanOrSum<uint8_t, int32_t>(
          GetTensorData<uint8_t>(input), input->params.zero_point,
          input->params.scale, input->dims->data, input->dims->size,
          GetTensorData<uint8_t>(output), output->params.zero_point,
          output->params.scale, output->dims->data, output->dims->size,
          GetTensorData<int>(axis), num_axis, params->keep_dims, temp_index,
          resolved_axis, op_data->temp_buffer, /*compute_sum=*/false);
      TF_LITE_ENSURE(context, ok);
    } break;
    default:
      TF_LITE_ENSURE_MSG(context, false,
                         "MEAN: unsupported input type for this build.");
  }
  return kTfLiteOk;
}
}  // namespace reduce

TfLiteRegistration* Register_MEAN() {
  static TfLiteRegistration r = {/*init=*/reduce::Init,
                                 /*free=*/nullptr,
                                 /*prepare=*/reduce::PrepareMeanOrSum,
                                 /*invoke=*/reduce::EvalMean,
                                 /*profiling_string=*/nullptr,
                                 /*builtin_code=*/0,
                                 /*custom_name=*/nullptr,
                                 /*version=*/0};
  return &r;
}
}  // namespace micro
}  // namespace ops
}  // namespace tflite
