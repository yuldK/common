#include <array>
#include <type_traits>

#pragma warning(push)
#pragma warning(disable:4100)

namespace ycl
{
	namespace coord
	{
		template <typename Ty, size_t d>
		class dimension_coord
		{
		public:
			using value_type = Ty;

		public:
			constexpr static size_t dimension = d;

		public:
			constexpr dimension_coord() noexcept
				: value{}
			{
			}

			template <typename... Args>
			constexpr dimension_coord(Args&&... args) noexcept
				: dimension_coord{}
			{
				static_assert(sizeof...(Args) == d, "different arguments size!");
				add<dimension, Args...>(std::forward<Args>(args)...);
			}

			constexpr static size_t low_level = dimension - 1;
			using low_level_t = dimension_coord<Ty, low_level>;

			template <typename T>
			constexpr dimension_coord(const low_level_t& low, T&& value) noexcept
				: dimension_coord{}
			{
				static_assert(std::is_convertible_v<T, Ty>, "not convertible type!");
				add(low, std::forward<T>(value));
			}

		public:
			constexpr value_type get(size_t idx) const noexcept { return value.at(idx); }
			constexpr value_type& get(size_t idx) noexcept { return value.at(idx); }

		private:
			template<size_t index, typename rhs_t>
			constexpr void add(rhs_t&& rhs)
			{
				if constexpr (std::is_convertible_v<rhs_t, Ty>)
					value[index] = std::forward<rhs_t>(rhs);
			}

			template<size_t index, typename rhs_t, typename... Args>
			constexpr void add(rhs_t&& rhs, Args&&... args) noexcept
			{
				if constexpr (std::is_convertible_v<rhs_t, Ty>) value[dimension - index] = std::forward<rhs_t>(rhs);
				return add<index - 1, Args...>(std::forward<Args>(args)...);
			}

			template<typename T>
			constexpr void add(const low_level_t& low, T&& v) noexcept
			{
				for (size_t i = 0; i < low_level; ++i) value[i] = low.get(i);
				if constexpr (std::is_convertible_v<T, Ty>) value[low_level] = std::forward<T>(v);
			}

		protected:
			std::array<Ty, dimension> value;
		};

		template <typename Ty>
		class type
		{
		public:
			using value_type = Ty;
			using this_type = type<value_type>;

		public:
			constexpr type() noexcept : val{} {}

#pragma region default operator
			constexpr type(const this_type& other) noexcept { *this = other; }
			constexpr type(const value_type& other) noexcept { *this = other; }
			constexpr this_type& operator=(const this_type& other) noexcept { val = other.val; return *this; }
			constexpr this_type& operator=(const value_type& other) noexcept { val = other; return *this; }

			constexpr type(this_type&& other) noexcept { *this = std::move(other); }
			constexpr type(value_type&& other) noexcept { *this = std::move(other); }
			constexpr this_type& operator=(this_type&& other) noexcept { val = std::move(other.val); return *this; }
			constexpr this_type& operator=(value_type&& other) noexcept { val = std::move(other); return *this; }

			constexpr bool operator==(const this_type& other) const noexcept { return val == other.val; }
			constexpr bool operator==(const value_type& other) const noexcept { return val == other; }

			constexpr this_type& operator+=(const value_type& other) noexcept { val += other; return *this; }
			constexpr this_type& operator+=(const this_type& other) noexcept { val += other.val; return *this; }
			constexpr this_type& operator++() noexcept { ++val; return *this; }
			constexpr this_type operator++(int) noexcept { auto ret = *this;  ++val; return ret; }

			constexpr this_type& operator-=(const value_type& other) noexcept { val -= other; return *this; }
			constexpr this_type& operator-=(const this_type& other) noexcept { val -= other.val; return *this; }
			constexpr this_type& operator--() noexcept { --val; return *this; }
			constexpr this_type operator--(int) noexcept { auto ret = *this;  --val; return ret; }

			constexpr operator value_type() const noexcept { return val; }
#pragma endregion

			constexpr const value_type& get() const noexcept { return val; }

		private:
			Ty val;
		};
	}
}

template <typename Ty>
constexpr ycl::coord::dimension_coord<Ty, 2> operator,(const ycl::coord::type<Ty>& lhs, const ycl::coord::type<Ty>& rhs)
{
	return ycl::coord::dimension_coord<Ty, 2>{ lhs.get(), rhs.get() };
}

template <typename Ty>
constexpr ycl::coord::dimension_coord<Ty, 2> operator,(const ycl::coord::type<Ty>& lhs, Ty&& rhs)
{
	return ycl::coord::dimension_coord<Ty, 2>(lhs.get(), std::forward<Ty>(rhs));
}

template <typename Ty, typename weak_Ty>
constexpr ycl::coord::dimension_coord<Ty, 2> operator,(const ycl::coord::type<Ty>& lhs, weak_Ty&& rhs)
{
	if constexpr (std::is_convertible_v<weak_Ty, Ty>) return ycl::coord::dimension_coord<Ty, 2>{ lhs, static_cast<Ty>(std::forward<weak_Ty>(rhs)) };
}

template <typename Ty, size_t d>
constexpr ycl::coord::dimension_coord<Ty, d + 1> operator,(const ycl::coord::dimension_coord<Ty, d>& lhs, const ycl::coord::type<Ty>& rhs)
{
	return ycl::coord::dimension_coord<Ty, d + 1>{ lhs, rhs.get() };
}

template <typename Ty, size_t d>
constexpr ycl::coord::dimension_coord<Ty, d + 1> operator,(const ycl::coord::dimension_coord<Ty, d>& lhs, Ty&& rhs)
{
	return ycl::coord::dimension_coord<Ty, d + 1>{ lhs, std::forward<Ty>(rhs) };
}

template <typename Ty, size_t d, typename weak_Ty>
constexpr ycl::coord::dimension_coord<Ty, d + 1> operator,(const ycl::coord::dimension_coord<Ty, d>& lhs, weak_Ty&& rhs)
{
	if constexpr (std::is_convertible_v<weak_Ty, Ty>) return ycl::coord::dimension_coord<Ty, d + 1>{ lhs, static_cast<Ty>(std::forward<weak_Ty>(rhs)) };
}

#pragma warning(pop)
