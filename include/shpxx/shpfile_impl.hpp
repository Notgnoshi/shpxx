#pragma once

#include "shpxx/concepts/has_class_name.hpp"
#include "shpxx/exceptions.hpp"
#include "shpxx/shape_type.hpp"
#include "shpxx/shp_handle.hpp"
#include "shpxx/shpfile.hpp"

#include <cstddef>
#include <limits>
#include <optional>
#include <stdexcept>

namespace shpxx {

template<shpxx::concepts::IsFeature FeatureT>
auto shpfile_t::begin() const -> const_feature_iterator_t<FeatureT>
{
    throw_if_incompatible<FeatureT>();

    if (size() > 0)
    {
        return const_feature_iterator_t<FeatureT>(*this, 0);
    }

    return end<FeatureT>();
}

template<shpxx::concepts::IsFeature FeatureT>
auto shpfile_t::end() const -> const_feature_iterator_t<FeatureT>
{
    throw_if_incompatible<FeatureT>();

    return const_feature_iterator_t<FeatureT>(*this);
}

template<shpxx::concepts::IsFeature FeatureT>
auto shpfile_t::at(std::size_t index) const -> std::optional<FeatureT>
{
    if (index >= size())
    {
        throw std::out_of_range("index greater than size of shapefile");
    }

    auto object = read_index(index);
    if (!object)
    {
        return std::nullopt;
    }

    throw_if_incompatible<FeatureT>();

    return FeatureT(*object);
}

template<shpxx::concepts::HasCompatibilityCheck CompatibilityCheckT>
    requires shpxx::concepts::HasClassName<CompatibilityCheckT>
void shpfile_t::throw_if_incompatible() const
{
    if (!CompatibilityCheckT::is_compatible(type()))
    {
        throw incompatible_feature_type_error_t(
            "specified feature type " + CompatibilityCheckT::class_name() +
            " is not compatible with file containing type " + to_string(type()));
    }
}

template<shpxx::concepts::IsFeature FeatureT>
shpfile_t::const_feature_iterator_t<FeatureT>::const_feature_iterator_t(const shpfile_t& file) :
    m_file(file), m_curr_index(std::numeric_limits<std::size_t>::max()), m_curr_obj(std::nullopt)
{
}

template<shpxx::concepts::IsFeature FeatureT>
shpfile_t::const_feature_iterator_t<FeatureT>::const_feature_iterator_t(const shpfile_t& file,
                                                                        std::size_t index) :
    m_file(file), m_curr_index(index), m_curr_obj(m_file.at<FeatureT>(m_curr_index))
{
}

template<shpxx::concepts::IsFeature FeatureT>
auto shpfile_t::const_feature_iterator_t<FeatureT>::operator*() const noexcept -> reference
{
    return m_curr_obj;
}

template<shpxx::concepts::IsFeature FeatureT>
auto shpfile_t::const_feature_iterator_t<FeatureT>::operator->() const noexcept -> pointer
{
    return &m_curr_obj;
}

template<shpxx::concepts::IsFeature FeatureT>
auto shpfile_t::const_feature_iterator_t<FeatureT>::operator++() noexcept
    -> const_feature_iterator_t&
{
    if (m_curr_index != std::numeric_limits<std::size_t>::max())
    {
        if (++m_curr_index < m_file.size())
        {
            m_curr_obj = m_file.at<FeatureT>(m_curr_index);
        } else
        {
            m_curr_index = std::numeric_limits<std::size_t>::max();
        }
    }
    return *this;
}

template<shpxx::concepts::IsFeature FeatureT>
auto shpfile_t::const_feature_iterator_t<FeatureT>::operator++(int) noexcept
    -> const_feature_iterator_t
{
    auto self_move = const_feature_iterator_t<FeatureT>(m_file);
    self_move.m_curr_index = m_curr_index;
    self_move.m_curr_obj = std::move(m_curr_obj);

    this->operator++();

    return self_move;
}

template<shpxx::concepts::IsFeature FeatureT>
auto shpfile_t::const_feature_iterator_t<FeatureT>::operator==(
    shpfile_t::const_feature_iterator_t<FeatureT> other) const noexcept -> bool
{
    return &m_file == &other.m_file && m_curr_index == other.m_curr_index;
}

template<shpxx::concepts::IsFeature FeatureT>
auto shpfile_t::const_feature_iterator_t<FeatureT>::operator!=(
    shpfile_t::const_feature_iterator_t<FeatureT> other) const noexcept -> bool
{
    return !(*this == other);
}

}  // namespace shpxx
