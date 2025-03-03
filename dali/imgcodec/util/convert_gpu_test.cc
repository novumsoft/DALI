// Copyright (c) 2022, NVIDIA CORPORATION & AFFILIATES. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <gtest/gtest.h>
#include <string>
#include <vector>
#include <memory>
#include "dali/test/dali_test.h"
#include "dali/test/dali_test_config.h"
#include "dali/core/tensor_shape_print.h"
#include "dali/core/convert.h"
#include "dali/imgcodec/util/convert_gpu.h"
#include "dali/test/test_tensors.h"
#include "dali/test/tensor_test_utils.h"
#include "dali/core/cuda_stream_pool.h"
#include "dali/kernels/imgproc/color_manipulation/color_space_conversion_impl.h"

namespace dali {
namespace imgcodec {
namespace test {

namespace {

template<class Input, class Output>
struct ConversionTestType {
  using In = Input;
  using Out = Output;
};

using TensorTestData = std::vector<std::vector<std::vector<float>>>;

template<class T>
void init_test_tensor_list(kernels::TestTensorList<T> &list, const TensorTestData &data) {
  TensorShape<> shape = {
    static_cast<int>(data.size()),
    static_cast<int>(data[0].size()),
    static_cast<int>(data[0][0].size()),
  };
  list.reshape({{shape}});
  auto tv = list.cpu()[0];
  for (int i = 0; i < shape[0]; i++)
    for (int j = 0; j < shape[1]; j++)
      for (int k = 0; k < shape[2]; k++)
        *tv(TensorShape<>{i, j, k}) = ConvertSatNorm<T>(data[i][j][k]);
}

template<class T>
SampleView<GPUBackend> get_gpu_sample_view(kernels::TestTensorList<T> &list) {
  auto tv = list.gpu()[0];
  return SampleView<GPUBackend>(tv.data, tv.shape, type2id<T>::value);
}

}  // namespace

template <typename ConversionType>
class ConvertGPUTest : public ::testing::Test {
 public:
  using Input = typename ConversionType::In;
  using Output = typename ConversionType::Out;

  void SetReference(const TensorTestData &data) {
    init_test_tensor_list(reference_list_, data);
    output_list_.reshape({{reference_list_.cpu()[0].shape}});
  }

  void SetInput(const TensorTestData &data) {
    init_test_tensor_list(input_list_, data);
  }

  void CheckConvert(TensorLayout out_layout, DALIImageType out_format,
                    TensorLayout in_layout, DALIImageType in_format,
                    const ROI &roi = {}, float multiplier = 1.0f) {
    int device_id;
    CUDA_CALL(cudaGetDevice(&device_id));
    auto out = get_gpu_sample_view(output_list_);
    auto in = get_gpu_sample_view(input_list_);
    auto stream = CUDAStreamPool::instance().Get(device_id);
    Convert(out, out_layout, out_format, in, in_layout, in_format, stream, roi, multiplier);
    CUDA_CALL(cudaStreamSynchronize(stream));
    Check(output_list_.cpu()[0], reference_list_.cpu()[0], EqualConvertNorm(eps_));
  }

 private:
  kernels::TestTensorList<Input> input_list_;
  kernels::TestTensorList<Output> output_list_;
  kernels::TestTensorList<Output> reference_list_;
  const float eps_ = 0.01f;
};

using ConversionTypes = ::testing::Types<ConversionTestType<uint8_t, int16_t>,
                                         ConversionTestType<float, uint8_t>,
                                         ConversionTestType<uint16_t, float>>;

TYPED_TEST_SUITE(ConvertGPUTest, ConversionTypes);

TYPED_TEST(ConvertGPUTest, Multiply) {
  this->SetInput({
    {
      {0.01f, 0.02f, 0.03f},
      {0.02f, 0.03f, 0.04f},
    },
    {
      {0.1f, 0.2f, 0.3f},
      {0.2f, 0.3f, 0.4f},
    },
  });

  this->SetReference({
    {
      {0.02f, 0.04f, 0.06f},
      {0.04f, 0.06f, 0.08f},
    },
    {
      {0.2f, 0.4f, 0.6f},
      {0.4f, 0.6f, 0.8f},
    },
  });

  this->CheckConvert("HWC", DALI_RGB, "HWC", DALI_RGB, {}, 2.0f);
}

TYPED_TEST(ConvertGPUTest, TransposeFromPlanar) {
  this->SetInput({
    {
      {0.00f, 0.01f, 0.02f, 0.03f},
      {0.10f, 0.11f, 0.12f, 0.13f},
    },
    {
      {0.20f, 0.21f, 0.22f, 0.23f},
      {0.30f, 0.31f, 0.32f, 0.33f},
    },
    {
      {0.40f, 0.41f, 0.42f, 0.43f},
      {0.50f, 0.51f, 0.52f, 0.53f},
    },
  });

  this->SetReference({
    {
      {0.00f, 0.20f, 0.40f},
      {0.01f, 0.21f, 0.41f},
      {0.02f, 0.22f, 0.42f},
      {0.03f, 0.23f, 0.43f},
    },
    {
      {0.10f, 0.30f, 0.50f},
      {0.11f, 0.31f, 0.51f},
      {0.12f, 0.32f, 0.52f},
      {0.13f, 0.33f, 0.53f},
    },
  });

  this->CheckConvert("HWC", DALI_RGB, "CHW", DALI_RGB);
}

TYPED_TEST(ConvertGPUTest, TransposeToPlanar) {
  this->SetInput({
    {
      {0.00f, 0.20f, 0.40f},
      {0.01f, 0.21f, 0.41f},
      {0.02f, 0.22f, 0.42f},
      {0.03f, 0.23f, 0.43f},
    },
    {
      {0.10f, 0.30f, 0.50f},
      {0.11f, 0.31f, 0.51f},
      {0.12f, 0.32f, 0.52f},
      {0.13f, 0.33f, 0.53f},
    },
  });

  this->SetReference({
    {
      {0.00f, 0.01f, 0.02f, 0.03f},
      {0.10f, 0.11f, 0.12f, 0.13f},
    },
    {
      {0.20f, 0.21f, 0.22f, 0.23f},
      {0.30f, 0.31f, 0.32f, 0.33f},
    },
    {
      {0.40f, 0.41f, 0.42f, 0.43f},
      {0.50f, 0.51f, 0.52f, 0.53f},
    }
  });

  this->CheckConvert("CHW", DALI_RGB, "HWC", DALI_RGB);
}

TYPED_TEST(ConvertGPUTest, TransposeWithRoi2D) {
  this->SetInput({
    {
      {0.00f, 0.01f, 0.02f, 0.03f},
      {0.10f, 0.11f, 0.12f, 0.13f},
    },
    {
      {0.20f, 0.21f, 0.22f, 0.23f},
      {0.30f, 0.31f, 0.32f, 0.33f},
    },
    {
      {0.40f, 0.41f, 0.42f, 0.43f},
      {0.50f, 0.51f, 0.52f, 0.53f},
    },
  });

  this->SetReference({
    {
      {0.12f, 0.32f, 0.52f},
      {0.13f, 0.33f, 0.53f},
    },
  });

  this->CheckConvert("HWC", DALI_RGB, "CHW", DALI_RGB, {{1, 2}, {2, 4}});
}

TYPED_TEST(ConvertGPUTest, TransposeWithRoi3D) {
  this->SetInput({
    {
      {0.00f, 0.01f, 0.02f, 0.03f},
      {0.10f, 0.11f, 0.12f, 0.13f},
    },
    {
      {0.20f, 0.21f, 0.22f, 0.23f},
      {0.30f, 0.31f, 0.32f, 0.33f},
    },
    {
      {0.40f, 0.41f, 0.42f, 0.43f},
      {0.50f, 0.51f, 0.52f, 0.53f},
    },
  });

  this->SetReference({
    {
      {0.12f, 0.32f, 0.52f},
      {0.13f, 0.33f, 0.53f},
    },
  });

  this->CheckConvert("HWC", DALI_RGB, "CHW", DALI_RGB, {{1, 2, 0}, {2, 4, 3}});
}

TYPED_TEST(ConvertGPUTest, RGBToYCbCr) {
  this->SetInput({
    {
      {0.1f, 0.2f, 0.3f},
    },
  });

  this->SetReference({
    {
      {0.218f, 0.558f, 0.449f},
    },
  });

  this->CheckConvert("HWC", DALI_YCbCr, "HWC", DALI_RGB);
}

TYPED_TEST(ConvertGPUTest, RGBToBGR) {
  this->SetInput({
    {
      {0.1f, 0.2f, 0.3f},
    },
  });

  this->SetReference({
    {
      {0.3f, 0.2f, 0.1f},
    },
  });

  this->CheckConvert("HWC", DALI_BGR, "HWC", DALI_RGB);
}

TYPED_TEST(ConvertGPUTest, RGBToGray) {
  this->SetInput({
    {
      {0.1f, 0.2f, 0.3f},
    },
  });

  this->SetReference({
    {
      {0.181f},
    },
  });

  this->CheckConvert("HWC", DALI_GRAY, "HWC", DALI_RGB);
}

}  // namespace test
}  // namespace imgcodec
}  // namespace dali
