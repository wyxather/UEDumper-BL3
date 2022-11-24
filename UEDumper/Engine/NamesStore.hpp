#pragma once

#include <iterator>

#include "UE4/GenericTypes.hpp"

class NamesIterator;

class NamesStore
{
	friend NamesIterator;

public:

	/// <summary>
	/// Initializes this object.
	/// </summary>
	/// <returns>true if it succeeds, false if it fails.</returns>
	static auto Initialize() noexcept -> bool;

	/// <summary>Gets the address of the global names store.</summary>
	/// <returns>The address of the global names store.</returns>
	static auto GetAddress() noexcept -> void*;

	auto begin() noexcept -> NamesIterator;

	auto begin() const noexcept -> NamesIterator;

	auto end() noexcept -> NamesIterator;

	auto end() const noexcept -> NamesIterator;

	/// <summary>
	/// Gets the number of available names.
	/// </summary>
	/// <returns>The number of names.</returns>
	auto GetNamesNum() const noexcept -> std::size_t;

	/// <summary>
	/// Test if the given id is valid.
	/// </summary>
	/// <param name="id">The identifier.</param>
	/// <returns>true if valid, false if not.</returns>
	auto IsValid(std::size_t id) const noexcept -> bool;

	/// <summary>
	/// Gets a name by id.
	/// </summary>
	/// <param name="id">The identifier.</param>
	/// <returns>The name.</returns>
	auto GetById(std::size_t id) const noexcept -> std::string;
};

struct UENameInfo
{
	size_t Index;
	std::string Name;
};

class NamesIterator : public std::iterator<std::forward_iterator_tag, UENameInfo>
{
	const NamesStore& store;
	size_t index;

public:
	NamesIterator(const NamesStore& store);

	explicit NamesIterator(const NamesStore& store, size_t index);

	void swap(NamesIterator& other) noexcept;

	NamesIterator& operator++();

	NamesIterator operator++ (int);

	bool operator==(const NamesIterator& rhs) const;

	bool operator!=(const NamesIterator& rhs) const;

	UENameInfo operator*() const;

	UENameInfo operator->() const;
};
