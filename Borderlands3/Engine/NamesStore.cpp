#include "NamesStore.hpp"

auto NamesStore::begin() noexcept -> NamesIterator
{
	return NamesIterator(*this, 0);
}

auto NamesStore::begin() const noexcept -> NamesIterator
{
	return NamesIterator(*this, 0);
}

auto NamesStore::end() noexcept -> NamesIterator
{
	return NamesIterator(*this);
}

auto NamesStore::end() const noexcept -> NamesIterator
{
	return NamesIterator(*this);
}

NamesIterator::NamesIterator(const NamesStore& _store) :
	store(_store),
	index(_store.GetNamesNum())
{
}

NamesIterator::NamesIterator(const NamesStore& _store, size_t _index) :
	store(_store),
	index(_index)
{
}

void NamesIterator::swap(NamesIterator& other) noexcept
{
	std::swap(index, other.index);
}

NamesIterator& NamesIterator::operator++()
{
	for (++index; index < store.GetNamesNum(); ++index)
		if (store.IsValid(index))
			break;
	return *this;
}

NamesIterator NamesIterator::operator++ (int)
{
	auto tmp(*this);
	++(*this);
	return tmp;
}

bool NamesIterator::operator==(const NamesIterator& rhs) const
{
	return index == rhs.index;
}

bool NamesIterator::operator!=(const NamesIterator& rhs) const
{
	return index != rhs.index;
}

UENameInfo NamesIterator::operator*() const
{
	return { index, store.GetById(index) };
}

UENameInfo NamesIterator::operator->() const
{
	return { index, store.GetById(index) };
}