#include <type_traits>
#include <cstdio>
#include <algorithm>
#include <type_traits>

#ifdef ALPAKA_INCLUDES
#include <alpaka/core/Align.hpp>
#include <alpaka/core/Assert.hpp>
#include <alpaka/core/BoostPredef.hpp>
#include <alpaka/core/Common.hpp>
#include <alpaka/core/Unused.hpp>
#include <alpaka/meta/IntegerSequence.hpp>
#include <alpaka/meta/Fold.hpp>

using namespace alpaka;
#endif

#define ALPAKA_NO_HOST_ACC_WARNING
#define ALPAKA_FN_HOST_ACC
#define ALPAKA_FN_HOST

//#############################################################################
//! A n-dimensional vector.
template<
	typename TDim,
	typename TVal>
class Vec final
{
public:
	static_assert(TDim::value >= 0u, "Invalid dimensionality");

	using Dim = TDim;
	static constexpr auto s_uiDim = TDim::value;
	using Val = TVal;

private:
#ifdef ALPAKA_INCLUDES
	//! A sequence of integers from 0 to dim-1.
	//! This can be used to write compile time indexing algorithms.
	using IdxSequence = meta::MakeIntegerSequence<std::size_t, TDim::value>;
#endif

public:
	//-----------------------------------------------------------------------------
	// The default constructor is only available when the vector is zero-dimensional.
	ALPAKA_NO_HOST_ACC_WARNING
	template<
		bool B = (TDim::value == 0u),
		typename = typename std::enable_if<B>::type>
	ALPAKA_FN_HOST_ACC Vec() :
		m_data{static_cast<TVal>(0u)}
	{}


	//-----------------------------------------------------------------------------
	//! Value constructor.
	//! This constructor is only available if the number of parameters matches the vector idx.
	ALPAKA_NO_HOST_ACC_WARNING
	template<
		typename TArg0,
		typename... TArgs,
		typename = typename std::enable_if<
			// There have to be dim arguments.
			(sizeof...(TArgs)+1 == TDim::value)
			&&
			(std::is_same<TVal, typename std::decay<TArg0>::type>::value)
			>::type>
	ALPAKA_FN_HOST_ACC Vec(
		TArg0 && arg0,
		TArgs && ... args) :
			m_data{std::forward<TArg0>(arg0), std::forward<TArgs>(args)...}
	{}

#ifdef ALPAKA_INCLUDES
#ifdef ALPAKA_CREATE_VEC_IN_CLASS
	//-----------------------------------------------------------------------------
	//! Creator using func<idx>(args...) to initialize all values of the vector.
	ALPAKA_NO_HOST_ACC_WARNING
	template<
		template<std::size_t> class TTFnObj,
		typename... TArgs,
		typename TIdxSize,
		TIdxSize... TIndices>
	ALPAKA_FN_HOST_ACC static auto createVecFromIndexedFnArbitrary(
		meta::IntegerSequence<TIdxSize, TIndices...> const & indices,
		TArgs && ... args)
	-> Vec<TDim, TVal>
	{
		alpaka::ignore_unused(indices);

		return Vec<TDim, TVal>(
			(TTFnObj<TIndices>::create(std::forward<TArgs>(args)...))...);
	}
	//-----------------------------------------------------------------------------
	//! Creator using func<idx>(args...) to initialize all values of the vector.
	//! The idx is in the range [0, TDim].
	ALPAKA_NO_HOST_ACC_WARNING
	template<
		template<std::size_t> class TTFnObj,
		typename... TArgs>
	ALPAKA_FN_HOST_ACC static auto createVecFromIndexedFn(
		TArgs && ... args)
	-> Vec<TDim, TVal>
	{
		return
			createVecFromIndexedFnArbitrary<
				TTFnObj>(
					IdxSequence(),
					std::forward<TArgs>(args)...);
	}
	//-----------------------------------------------------------------------------
	//! Creator using func<idx>(args...) to initialize all values of the vector.
	//! The idx is in the range [TIdxOffset, TIdxOffset + TDim].
	ALPAKA_NO_HOST_ACC_WARNING
	template<
		template<std::size_t> class TTFnObj,
		typename TIdxOffset,
		typename... TArgs>
	ALPAKA_FN_HOST_ACC static auto createVecFromIndexedFnOffset(
		TArgs && ... args)
	-> Vec<TDim, TVal>
	{
		using IdxSubSequenceSigned = meta::MakeIntegerSequenceOffset<std::intmax_t, TIdxOffset::value, TDim::value>;
		using IdxSubSequence = meta::ConvertIntegerSequence<typename TDim::value_type, IdxSubSequenceSigned>;
		return
			createVecFromIndexedFnArbitrary<
				TTFnObj>(
					IdxSubSequence(),
					std::forward<TArgs>(args)...);
	}
#endif
#endif

	//-----------------------------------------------------------------------------
	ALPAKA_NO_HOST_ACC_WARNING
	ALPAKA_FN_HOST_ACC
	Vec(Vec const &) = default;
	//-----------------------------------------------------------------------------
	ALPAKA_NO_HOST_ACC_WARNING
	ALPAKA_FN_HOST_ACC
	Vec(Vec &&) = default;
	//-----------------------------------------------------------------------------
	ALPAKA_NO_HOST_ACC_WARNING
	ALPAKA_FN_HOST_ACC
	auto operator=(Vec const &) -> Vec & = default;
	//-----------------------------------------------------------------------------
	ALPAKA_NO_HOST_ACC_WARNING
	ALPAKA_FN_HOST_ACC
	auto operator=(Vec &&) -> Vec & = default;
	//-----------------------------------------------------------------------------
	ALPAKA_NO_HOST_ACC_WARNING
	ALPAKA_FN_HOST_ACC ~Vec() = default;

private:
	//#############################################################################
	//! A function object that returns the given value for each index.
	template<
		std::size_t Tidx>
	struct CreateSingleVal
	{
		//-----------------------------------------------------------------------------
		ALPAKA_NO_HOST_ACC_WARNING
		ALPAKA_FN_HOST_ACC static auto create(
			TVal const & val)
		-> TVal
		{
			return val;
		}
	};
public:
	//-----------------------------------------------------------------------------
	//! \brief Single value constructor.
	//!
	//! Creates a vector with all values set to val.
	//! \param val The initial value.
// 	ALPAKA_NO_HOST_ACC_WARNING
// 	ALPAKA_FN_HOST_ACC static auto all(
// 		TVal const & val)
// 	-> Vec<TDim, TVal>
// 	{
// 		return
// 			createVecFromIndexedFn<
// #ifndef ALPAKA_CREATE_VEC_IN_CLASS
// 				TDim,
// #endif
// 				CreateSingleVal>(
// 					val);
// 	}
	//-----------------------------------------------------------------------------
	//! Zero value constructor.
	ALPAKA_NO_HOST_ACC_WARNING
	ALPAKA_FN_HOST_ACC static auto zeros()
	-> Vec<TDim, TVal>
	{
		return all(static_cast<TVal>(0));
	}
	//-----------------------------------------------------------------------------
	//! One value constructor.
	ALPAKA_NO_HOST_ACC_WARNING
	ALPAKA_FN_HOST_ACC static auto ones()
	-> Vec<TDim, TVal>
	{
		return all(static_cast<TVal>(1));
	}

	//-----------------------------------------------------------------------------
	//! Value reference accessor at the given non-unsigned integer index.
	//! \return A reference to the value at the given index.
	ALPAKA_NO_HOST_ACC_WARNING
	template<
		typename TIdx,
		typename = typename std::enable_if<
			std::is_integral<TIdx>::value>::type>
	ALPAKA_FN_HOST_ACC auto operator[](
		TIdx const iIdx)
	-> TVal &
	{
#ifdef ALPAKA_INCLUDES
		core::assertValueUnsigned(iIdx);
#endif
		auto const idx(static_cast<typename TDim::value_type>(iIdx));
#ifdef ALPAKA_INCLUDES
		core::assertGreaterThan<TDim>(idx);
#endif
		return m_data[idx];
	}

	//-----------------------------------------------------------------------------
	//! Value accessor at the given non-unsigned integer index.
	//! \return The value at the given index.
	ALPAKA_NO_HOST_ACC_WARNING
	template<
		typename TIdx,
		typename = typename std::enable_if<
			std::is_integral<TIdx>::value>::type>
	ALPAKA_FN_HOST_ACC auto operator[](
		TIdx const iIdx) const
	-> TVal
	{
#ifdef ALPAKA_INCLUDES
		core::assertValueUnsigned(iIdx);
#endif
		auto const idx(static_cast<typename TDim::value_type>(iIdx));
#ifdef ALPAKA_INCLUDES
		core::assertGreaterThan<TDim>(idx);
#endif
		return m_data[idx];
	}

	//-----------------------------------------------------------------------------
	ALPAKA_NO_HOST_ACC_WARNING
	ALPAKA_FN_HOST_ACC auto operator==(
		Vec const & rhs) const
	-> bool
	{
		for(typename TDim::value_type i(0); i < TDim::value; ++i)
		{
			if((*this)[i] != rhs[i])
			{
				return false;
			}
		}
		return true;
	}
	//-----------------------------------------------------------------------------
	ALPAKA_NO_HOST_ACC_WARNING
	ALPAKA_FN_HOST_ACC auto operator!=(
		Vec const & rhs) const
	-> bool
	{
		return !((*this) == rhs);
	}
	//-----------------------------------------------------------------------------
#ifdef ALPAKA_INCLUDES
	ALPAKA_NO_HOST_ACC_WARNING
	template<
		typename TFnObj,
		std::size_t... TIndices>
	ALPAKA_FN_HOST_ACC auto foldrByIndices(
		TFnObj const & f,
		meta::IntegerSequence<std::size_t, TIndices...> const & indices) const
#ifdef BOOST_NO_CXX14_RETURN_TYPE_DEDUCTION
	-> decltype(
		meta::foldr(
			f,
			((*this)[TIndices])...))
#endif
	{
		alpaka::ignore_unused(indices);

		return
			meta::foldr(
				f,
				((*this)[TIndices])...);
	}
	//-----------------------------------------------------------------------------
	ALPAKA_NO_HOST_ACC_WARNING
	template<
		typename TFnObj>
	ALPAKA_FN_HOST_ACC auto foldrAll(
		TFnObj const & f) const
#ifdef BOOST_NO_CXX14_RETURN_TYPE_DEDUCTION
	-> decltype(
#if (BOOST_COMP_GNUC && (BOOST_COMP_GNUC < BOOST_VERSION_NUMBER(5, 0, 0))) || BOOST_COMP_INTEL || BOOST_COMP_NVCC
		this->foldrByIndices(
#else
		foldrByIndices(
#endif
			f,
			IdxSequence()))
#endif
	{
		return
			foldrByIndices(
				f,
				IdxSequence());
	}
// suppress strange warning produced by nvcc+MSVC in release mode
#if BOOST_COMP_MSVC
#pragma warning(push)
#pragma warning(disable: 4702)  // unreachable code
#endif
	//-----------------------------------------------------------------------------
	//! \return The product of all values.
	ALPAKA_NO_HOST_ACC_WARNING
	ALPAKA_FN_HOST_ACC auto prod() const
	-> TVal
	{
		return foldrAll(
			[](TVal a, TVal b)
			{
				return static_cast<TVal>(a * b);
			});
	}
#if BOOST_COMP_MSVC
#pragma warning(pop)
#endif
	//-----------------------------------------------------------------------------
	//! \return The sum of all values.
	ALPAKA_NO_HOST_ACC_WARNING
	ALPAKA_FN_HOST_ACC auto sum() const
	-> TVal
	{
		return foldrAll(
			[](TVal a, TVal b)
			{
				return static_cast<TVal>(a + b);
			});
	}
	//-----------------------------------------------------------------------------
	//! \return The min of all values.
	ALPAKA_NO_HOST_ACC_WARNING
	ALPAKA_FN_HOST_ACC auto min() const
	-> TVal
	{
		return foldrAll(
			[](TVal a, TVal b)
			{
				return (b < a) ? b : a;
			});
	}
	//-----------------------------------------------------------------------------
	//! \return The max of all values.
	ALPAKA_NO_HOST_ACC_WARNING
	ALPAKA_FN_HOST_ACC auto max() const
	-> TVal
	{
		return foldrAll(
			[](TVal a, TVal b)
			{
				return (b > a) ? b : a;
			});
	}
#endif
	//-----------------------------------------------------------------------------
	//! \return The index of the minimal element.
	ALPAKA_FN_HOST auto minElem() const
	-> typename TDim::value_type
	{
		return
			static_cast<typename TDim::value_type>(
				std::distance(
					std::begin(m_data),
					std::min_element(
						std::begin(m_data),
						std::end(m_data))));
	}
	//-----------------------------------------------------------------------------
	//! \return The index of the maximal element.
	ALPAKA_FN_HOST auto maxElem() const
	-> typename TDim::value_type
	{
		return
			static_cast<typename TDim::value_type>(
				std::distance(
					std::begin(m_data),
					std::max_element(
						std::begin(m_data),
						std::end(m_data))));
	}

private:
	// Zero sized arrays are not allowed, therefore zero-dimensional vectors have one member.
	TVal m_data[TDim::value == 0u ? 1u : TDim::value];
};

#if 0
template<
	typename TDim,
	typename TVal>
class T
{
	static_assert(TDim::value > 0u, "Invalid dimensionality");
	TVal m_a[TDim::value == 0u ? 1u : TDim::value];

	public:
		T() = delete;
	T(TVal a) {m_a[0]=a;}
	T(T const &) = delete;
	T(T &&) = default;
	T& operator=(T const &) = delete;
	T& operator=(T &&) = default;
		~T() = default;

	TVal& operator[](int i) {return m_a[i];}
};

using TI = T<std::integral_constant<long unsigned int, 1>, int>;

static_assert(std::is_trivially_copyable<TI>::value, "T not trivially copyable");
#endif

int main(int argc, char *argv[])
{
	Vec<std::integral_constant< unsigned int, 1>,  unsigned int> t(23u);
// #pragma omp target enter data map(to: t)

#pragma omp teams
	{
		#pragma omp distribute
		for(int i = 0; i<2; ++i)
		{
			#pragma omp parallel
			{
				printf("%d: %d\n", i, t[0]);
			}
		}
	}

	return 0;
}
