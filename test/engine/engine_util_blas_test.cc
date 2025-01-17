// Copyright 2022 DeepMind Technologies Limited
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

// Tests for engine/engine_util_blas.c

#include "src/engine/engine_util_blas.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <mujoco/mjtnum.h>
#include "test/fixture.h"

namespace mujoco {
namespace {

using ::testing::ElementsAre;

using EngineUtilBlasTest = MujocoTest;

TEST_F(EngineUtilBlasTest, MjuDot) {
  mjtNum a[] = {1, 2, 3, 4, 5, 6, 7};
  mjtNum b[] = {7, 6, 5, 4, 3, 2, 1};

  // test various vector lengths because mju_dot adds numbers in groups of four
  EXPECT_EQ(mju_dot(a, b, 0), 0);
  EXPECT_EQ(mju_dot(a, b, 1), 7);
  EXPECT_EQ(mju_dot(a, b, 2), 7 + 2*6);
  EXPECT_EQ(mju_dot(a, b, 3), 7 + 2*6 + 3*5);
  EXPECT_EQ(mju_dot(a, b, 4), 7 + 2*6 + 3*5 + 4*4);
  EXPECT_EQ(mju_dot(a, b, 5), 7 + 2*6 + 3*5 + 4*4 + 5*3);
  EXPECT_EQ(mju_dot(a, b, 6), 7 + 2*6 + 3*5 + 4*4 + 5*3 + 6*2);
  EXPECT_EQ(mju_dot(a, b, 7), 7 + 2*6 + 3*5 + 4*4 + 5*3 + 6*2 + 7);
}

TEST_F(EngineUtilBlasTest, MjuMulVecMatVec) {
  mjtNum vec1[] = {1, 2, 3};
  mjtNum vec2[] = {3, 2, 1};
  mjtNum mat[] = {
    1, 2, 3,
    4, 5, 6,
    7, 8, 9
  };

  EXPECT_EQ(mju_mulVecMatVec(vec1, mat, vec2, 3), 204);
}

TEST_F(EngineUtilBlasTest, MjuFill) {
  mjtNum vec[] = {0, 1, 4};
  mju_fill(vec, 3, 4.5);

  EXPECT_EQ(vec[0], 4.5);
  EXPECT_EQ(vec[1], 4.5);
  EXPECT_EQ(vec[2], 4.5);
}

TEST_F(EngineUtilBlasTest, MjuEye) {
  mjtNum mat1[1];
  mju_eye(mat1, 1);
  EXPECT_EQ(mat1[0], 1);

  mjtNum mat3[9];
  mju_eye(mat3, 3);
  EXPECT_THAT(mat3, ElementsAre(1, 0, 0,
                                0, 1, 0,
                                0, 0, 1));
}

TEST_F(EngineUtilBlasTest, MjuSymmetrize) {
  mjtNum mat[] = {
    1,   2.5, 3.5,
    1.5, 2,   4,
    2.5, 3,   3
  };
  mju_symmetrize(mat, 3);
  EXPECT_THAT(mat, ElementsAre(1, 2,   3,
                               2, 2,   3.5,
                               3, 3.5, 3));
}

}  // namespace
}  // namespace mujoco
