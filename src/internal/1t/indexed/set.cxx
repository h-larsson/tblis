#include "set.hpp"
#include "internal/1t/dense/set.hpp"

#include "util/tensor.hpp"

namespace tblis
{
namespace internal
{

template <typename T>
void set(const communicator& comm, const config& cfg,
         T alpha, const indexed_varray_view<T>& A, const dim_vector& idx_A_A)
{
    for (len_type i = 0;i < A.num_indices();i++)
    {
        set(comm, cfg, A.dense_lengths(), A.factor(i)*alpha, A.data(i),
            A.dense_strides());
    }
}

#define FOREACH_TYPE(T) \
template void set(const communicator& comm, const config& cfg, \
                  T alpha, const indexed_varray_view<T>& A, const dim_vector&);
#include "configs/foreach_type.h"

}
}
