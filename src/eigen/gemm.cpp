#include "internal.h"

extern "C" void cnGEMM(const CnMat *_src1, const CnMat *_src2, double alpha, const CnMat *_src3, double beta,
					   CnMat *_dst, enum cnGEMMFlags tABC) {
	if (_src3) {
		assert(_src3->data != _src2->data);
		assert(_src3->data != _src1->data);
		assert(_src3->data != _dst->data);
	}
	//assert(_src2->data != _src1->data);
	assert(_src2->data != _dst->data);
	assert(_src1->data != _dst->data);
	auto src1 = CONVERT_TO_EIGEN(_src1);
	auto src2 = CONVERT_TO_EIGEN(_src2);

	auto dst = CONVERT_TO_EIGEN(_dst);

	EIGEN_RUNTIME_SET_IS_MALLOC_ALLOWED(false);
	if (tABC & CN_GEMM_FLAG_A_T)
		if (tABC & CN_GEMM_FLAG_B_T)
			dst.noalias() = alpha * src1.transpose() * src2.transpose();
		else
			dst.noalias() = alpha * src1.transpose() * src2;
	else {
		if (tABC & CN_GEMM_FLAG_B_T)
			dst.noalias() = alpha * src1 * src2.transpose();
		else
			dst.noalias() = alpha * src1 * src2;
	}

	if (_src3) {
		auto src3 = CONVERT_TO_EIGEN(_src3);
		if (tABC & CN_GEMM_FLAG_C_T)
			dst.noalias() += beta * src3.transpose();
		else
			dst.noalias() += beta * src3;
	}
	//assert(cn_is_finite(_dst));
}
