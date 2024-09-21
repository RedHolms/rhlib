#include <rh/Array.hpp>
#include <rh/Container.hpp>
#include <rh/List.hpp>

static_assert(rh::Container<rh::List<int>, int>);
static_assert(rh::ConstContainer<rh::List<int>, int>);
static_assert(rh::ConstContainer<rh::List<int> const, int>);
static_assert(rh::ConstContainer<rh::Array<int, 1>, int>);
