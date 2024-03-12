#pragma once

#include <Eigen/Core>

template <typename T>
using Matrix = Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>;

template <typename T>
using Vector = Eigen::Vector<T, Eigen::Dynamic>;

template <typename T>
using MatrixView = Eigen::Map<Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>, 0, Eigen::Stride<Eigen::Dynamic, Eigen::Dynamic>>;

template <typename T>
using VectorView = Eigen::Map<Eigen::Vector<T, Eigen::Dynamic>, 0, Eigen::Stride<Eigen::Dynamic, Eigen::Dynamic>>;
