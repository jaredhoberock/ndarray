#pragma once

#include <cstddef>

// ndarray_ref is a mutable view of a multidimensional array of elements.
// The dimensionality of the array is given by Shape, which is a generalized shape type.
// A type is an Shape if
//   1. The type is an integral type, or
//   2. the type is a tuple where each element is a Shape.
template<class T, class Shape>
class ndarray_ref
{
  public:
    using element_type = T;
    using shape_type = Shape;
    using index_type = shape_type;

    // XXX size_type should be the result of the recursive application of shape_product() to .shape()
    //     should implement a shape_size() function for that
    using size_type = std::size_t;

    using reference = element_type&;
    using pointer = element_type*;

    // this iterator traverses in row-major order
    using iterator = pointer;

    ndarray_ref() : ndarray_ref(nullptr) {}

    explicit ndarray_ref(std::nullptr_t) : span(nullptr, shape_type{}) {}

    ndarray_ref(pointer ptr, shape_type shape) : data_(ptr), shape_(shape) {}

    shape_type shape() const
    {
      return shape_;
    }

    size_type size() const
    {
      // shape_size() would return the number of points in the index space implied by its argument
      // return agency::detail::shape_size(shape());
      return agency::detail::shape_cast<size_type>(shape());
    }

    pointer data() const
    {
      return data_;
    }

    reference operator[](const index_type& idx)
    {
      // index_rank() would return the the rank of the index in the index space implied by its argument
      // its rank is its position in a lexicographically sorted array of all the indices in the index space
      // size_type idx_1d = agency::detail::index_rank(idx, shape());
      size_type idx_1d = agency::detail::index_cast<size_type>(idx, shape(), size());

      return data_[idx_1d];
    }

    iterator begin() const
    {
      return data_;
    }

    iterator end() const
    {
      return begin() + size();
    }

  private:
    T* data_;
    shape_type shape_;
};

