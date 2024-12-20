// This file is part of HemeLB and is Copyright (C)
// the HemeLB team and/or their institutions, as detailed in the
// file AUTHORS. This software is provided under the terms of the
// license in the file LICENSE.

#ifndef HEMELB_UTIL_ITERATOR_H
#define HEMELB_UTIL_ITERATOR_H

#include <iostream>
#include <tuple>
#include <compare>

namespace hemelb::util
{
    //! For-ranged loop plus enumeration
    template<class WrappedIterT, class CounterT = std::iter_difference_t<WrappedIterT>>
    class Enumerate
    {
    protected:
        WrappedIterT first;
        WrappedIterT last;

        //! Holds info about enumeration
        struct EnumerateItem
        {
            CounterT index;
            typename WrappedIterT::reference value;
        };

    public:
        class iterator
        {
        protected:
            CounterT index;
            WrappedIterT iter;
        public:
            explicit iterator(WrappedIterT const &iter) : index(0), iter(iter)
            {
            }
            iterator(WrappedIterT const &iter, size_t index) : index(index), iter(iter)
            {
            }
            iterator& operator++()
            {
              ++index;
              ++iter;
              return *this;
            }
            iterator operator++(int) &
            {
              iterator result(iter.iter);
              iter++;
              return result;
            }
            EnumerateItem operator*() const
            {
              return {index, *iter};
            }
            friend auto operator<=>(const iterator&, const iterator&) = default;
        };

        Enumerate(WrappedIterT const &first, WrappedIterT const &last) : first(first), last(last)
        {
        }
        iterator begin() const
        {
          return {first, 0};
        }
        iterator end() const
        {
          return iterator(last, std::distance(first, last));
        }
    };

    //! Ranged-for loops with enumeration
    template<class C>
    auto enumerate(C&& c) -> Enumerate<decltype(begin(std::forward<C>(c)))>
    {
      return {begin(std::forward<C>(c)), end(std::forward<C>(c))};
    }

    //! Ranged-for loops with enumeration
    template<class CounterT, class C>
    auto enumerate_with(C&& c) -> Enumerate<decltype(begin(std::forward<C>(c))), CounterT>
    {
      return {begin(std::forward<C>(c)), end(std::forward<C>(c))};
    }

    //! Ranged-for loops with enumeration
    template<class CONTAINER>
    auto cenumerate(CONTAINER const &x) -> Enumerate<decltype(begin(x))>
    {
      return {begin(x), end(x)};
    }

    //! For-ranged loop over several containers in parallel
    template<class... ContainerTs>
    class Zip
    {
    public:
        //! Iterates over all objects that are being zipped
        template<class ... ITERATOR>
        class zip_iterator
        {
            static constexpr auto N = sizeof...(ITERATOR);
        public:
            using reference = std::tuple<typename std::iterator_traits<ITERATOR>::reference...>;
            //! Inherits from all iterator types
            zip_iterator(ITERATOR const & ... args) : iter(args...)
            {
            }
            //! Inherits from all iterator types
            zip_iterator(ITERATOR && ... args) : iter(std::move(args)...)
            {
            }
            zip_iterator& operator++()
            {
                increment(std::make_index_sequence<N>());
                return *this;
            }
            reference operator*() const
            {
                return deref(std::make_index_sequence<N>());
            }

            //! True if any of the iterator is equal to the corresponding on in b
            friend bool operator==(zip_iterator const &a, zip_iterator const &b)
            {
                return IsEqual<0>(a, b);
            }
            friend bool operator!=(zip_iterator const& a, zip_iterator const &b)
            {
                return !(a == b);
            }

        protected:
            // For each of these helpers, we use an index sequence
            // as the argument to deduce the template arguments `Is`.
            // Should be called with `std::make_index_sequence<N>()`.

            //! Increment
            // Note folding over the comma operator
            template <std::size_t... Is>
            void increment(std::index_sequence<Is...>) {
                (++std::get<Is>(iter), ...);
            }
            //! Comparison
            // Do a fold over logical and
            template<std::size_t... Is>
            friend bool IsEqual(zip_iterator const& a, zip_iterator const &b)
            {
                return ((std::get<Is>(a.iter) == std::get<Is>(b.iter)) && ...);
            }
            //! Derefence
            template <std::size_t... Is>
            reference deref(std::index_sequence<Is...>) const {
                return {*std::get<Is>(iter)...};
            }

            //! Holds each iterator
            std::tuple<ITERATOR...> iter;
        };

        using iterator = zip_iterator<decltype(std::declval<ContainerTs>().begin())...>;

        Zip(ContainerTs && ... cont) : first(cont.begin()...), last(cont.end()...)
        {
        }
        iterator begin() const
        {
          return first;
        }
        iterator end() const
        {
          return last;
        }
      protected:
        iterator first;
        iterator last;
    };

    //! \brief Ranged-for loops over sets of containers in parallel
    //! \code
    //! for(auto const && item: zip(container0, container2, containerN))
    //! {
    //!   std::get<0>(item);
    //!   std::get<1>(item);
    //! }
    //! \endcode
    template<class ... CONTAINER>
    auto zip(CONTAINER && ... x) -> Zip<CONTAINER...>
    {
      return {x...};
    }
    //! \brief Ranged-for loops over sets of (const) containers in parallel
    //! \code
    //! for(auto const && item: zip(container0, container2, containerN))
    //! {
    //!   std::get<0>(item);
    //!   std::get<1>(item);
    //! }
    //! \endcode
    template<class ... CONTAINER>
    auto czip(CONTAINER const & ... x) -> Zip<CONTAINER const &...>
    {
      return {x...};
    }

}

#endif
