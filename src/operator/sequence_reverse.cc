/*!
 * Copyright (c) 2015 by Contributors
 * \file sequence_reverse.cc
 * \brief
 * \author Sebastian Bodenstein
*/
#include "./sequence_reverse-inl.h"

namespace mxnet {
namespace op {
template <>
Operator *CreateOp<cpu>(SequenceReverseParam param, int dtype) {
  Operator *op = NULL;
  MSHADOW_REAL_TYPE_SWITCH(dtype, DType,
                           { op = new SequenceReverseOp<cpu, DType>(param); })
  return op;
}

// DO_BIND_DISPATCH comes from operator_common.h
Operator *SequenceReverseProp::CreateOperatorEx(
    Context ctx, std::vector<TShape> *in_shape,
    std::vector<int> *in_type) const {
  std::vector<TShape> out_shape, aux_shape;
  std::vector<int> out_type, aux_type;
  CHECK(InferType(in_type, &out_type, &aux_type));
  CHECK(InferShape(in_shape, &out_shape, &aux_shape));
  DO_BIND_DISPATCH(CreateOp, param_, (*in_type)[0]);
}

DMLC_REGISTER_PARAMETER(SequenceReverseParam);

MXNET_REGISTER_OP_PROPERTY(SequenceReverse, SequenceReverseProp)
    .describe(R"code(Reverses the elements of each sequence.

This function takes an n-dimensional input array of the form [max_sequence_length, batch_size, other_feature_dims]
and returns an array of the same shape.

Parameter `sequence_length` is used to handle variable-length sequences.
`sequence_length` should be an input array of positive ints of dimension [batch_size].
To use this parameter, set `use_sequence_length` to `True`,
otherwise each example in the batch is assumed to have the max sequence length.

Example::

   x = [[[  1.,   2.,   3.],
         [  4.,   5.,   6.]],

        [[  7.,   8.,   9.],
         [ 10.,  11.,  12.]],

        [[ 13.,  14.,   15.],
         [ 16.,  17.,   18.]]]

   // Batch 1
   B1 = [[  1.,   2.,   3.],
         [  7.,   8.,   9.],
         [ 13.,  14.,  15.]]

   // Batch 2
   B2 = [[  4.,   5.,   6.],
         [ 10.,  11.,  12.],
         [ 16.,  17.,  18.]]

   // returns reverse sequence when sequence_length parameter is not used
   SequenceReverse(x) = [[[ 13.,  14.,   15.],
                          [ 16.,  17.,   18.]],

                         [[  7.,   8.,   9.],
                          [ 10.,  11.,  12.]],

                         [[  1.,   2.,   3.],
                          [  4.,   5.,   6.]]]

   // sequence_length [2,2] means 2 rows of
   // both batch B1 and B2 will be reversed.
   SequenceReverse(x, y=[2,2], use_sequence_length=True) =
                     [[[  7.,   8.,   9.],
                       [ 10.,  11.,  12.]],

                      [[  1.,   2.,   3.],
                       [  4.,   5.,   6.]],

                      [[ 13.,  14.,   15.],
                       [ 16.,  17.,   18.]]]

   // sequence_length [2,3] means 2 of batch B2 and 3 of batch B3
   // will be reversed.
   SequenceReverse(x, y=[2,3], use_sequence_length=True) =
                    [[[  7.,   8.,   9.],
                      [ 16.,  17.,  18.]],

                     [[  1.,   2.,   3.],
                      [ 10.,  11.,  12.]],

                     [[ 13.,  14,   15.],
                      [  4.,   5.,   6.]]]

)code" ADD_FILELINE)
    .add_argument("data", "NDArray-or-Symbol",
                  "n-dimensional input array of the form [max_sequence_length,"
                  " batch_size, other dims] where n>2 ")
    .add_argument("sequence_length", "NDArray-or-Symbol",
                  "vector of sequence lengths of the form [batch_size]")
    .add_arguments(SequenceReverseParam::__FIELDS__());

}  // namespace op
}  // namespace mxnet
