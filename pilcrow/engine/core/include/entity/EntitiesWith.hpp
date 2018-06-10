#pragma once
#include <type_traits>
#include <vector>

template <typename... Args>
class ConstrainedEntityRef;
class EntityRef;

class EntitiesWithBase {
  friend class ComponentAggregate;

protected:
  virtual void SetEntities(const std::vector<EntityRef> &) = 0;
  virtual void SwapPopEntity(std::size_t index)            = 0;
  virtual void PushEntity(const EntityRef &)               = 0;
};
template <typename... Args>
class EntitiesWith : public EntitiesWithBase {
public:
  using iterator =
    typename std::vector<ConstrainedEntityRef<Args...>>::iterator;
  using const_iterator =
    typename std::vector<ConstrainedEntityRef<Args...>>::const_iterator;

  iterator       begin();
  iterator       end();
  const_iterator begin() const;
  const_iterator end() const;
  const_iterator cbegin() const;

  const_iterator                       cend() const;
  ConstrainedEntityRef<Args...> &      operator[](std::size_t pos);
  const ConstrainedEntityRef<Args...> &operator[](std::size_t pos) const;

private:
  // @@TODO:
  // If you can make this just a reference to the vector
  // in the ComponentAggregate responsible for this EntitiesWith,
  // It will use way less memory be more efficient.
  // Those benefits are because many systems with the same requirements
  // could reference a single vector of constrained entity refs. (less mem)
  // Since they reference the original, updates would not need to be pushed.
  // (less work)
  // Optionally, use style guide to enforce alphabetical ordering instead
  std::vector<ConstrainedEntityRef<Args...>> m_Entities;
  void SetEntities(const std::vector<EntityRef> &) final;
  void SwapPopEntity(std::size_t index) final;
  void PushEntity(const EntityRef &) final;
};
#include "../../src/entity/EntitiesWith.inl"
