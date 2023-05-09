#include <algorithm>
#include <cstddef>
#include <functional>
#include <initializer_list>
#include <iterator>
#include <ranges>
#include <stdexcept>
#include <vector>
#include <utility>

template <typename T>
struct HashFunction;

template <typename Key, typename Value, typename Hash = HashFunction<Key>>
class HashBrown
{
   struct Bucket;
   class Iterator;

   public:
      using value_type = std::pair<Key, Value>;
      using reference = value_type&;
      using pointer = value_type*;
      using const_reference = const value_type&;
      using iterator = Iterator;
      using local_iterator = typename std::vector<value_type>::iterator;
      using const_iterator = const Iterator;

      constexpr HashBrown();

      iterator begin();
      const_iterator begin() const;
      const_iterator cbegin() const;

      iterator end();
      const_iterator end() const;
      const_iterator cend() const;
   
      iterator erase(const Key& key);

      void insert(std::initializer_list<value_type> il);
      template <typename InputIt>
      void insert(InputIt first, InputIt last);
      iterator insert(Key key, Value value);
      iterator insert(const value_type& value);
      iterator insert(value_type&& value);
      template <typename... Args>
      iterator emplace(Args&&... args);

      const Value* get(Key&& key) const;
      bool empty() const;

   private:
      std::vector<Bucket> _buckets;
      Hash _hasher;
      std::size_t _items;

      std::size_t get_bucket_index(const Key& key) const;
      float load_factor() const;
      void resize();

      class Iterator {
         public:
            using difference_type = std::ptrdiff_t;
            using value_type = HashBrown<Key, Value, Hash>::value_type;
            using reference = HashBrown<Key, Value, Hash>::reference;
            using pointer = HashBrown<Key, Value, Hash>::pointer;
            using iterator_category = std::forward_iterator_tag;
      
            Iterator(HashBrown<Key, Value, Hash>* hb, std::size_t bucket, std::size_t pos = 0)
               : _hb(hb)
               , _bucket(bucket)
               , _pos(pos)
            {
            }
      
            Iterator& operator++()
            {
               const auto bucket = _hb->_buckets[_bucket];
               
               if (_pos == bucket.items.size())
               {
                  _pos = 0;
                  _bucket++;
               }
               ++_pos;
               return *this;
            }
      
            Iterator& operator++(int)
            {
               auto temp = this;
               ++*this;
               return this;
            }
      
            reference operator*()
            {
               auto& bucket = _hb->_buckets[_bucket];
               return bucket.items[_pos];
            }
      
            pointer operator->()
            {
               return &this->operator*();
            }
      
            friend bool operator==(const Iterator& it_a, const Iterator& it_b)
            {
               return it_a._hb == it_b._hb
                  && it_a._bucket == it_b._bucket
                  && it_a._pos == it_b._pos;
            }
      
            friend bool operator!=(const Iterator& it_a, const Iterator& it_b)
            {
               return !(it_a == it_b);
            }
      
         private:
            HashBrown<Key, Value, Hash>* _hb;
            std::size_t _bucket;
            std::size_t _pos;
      };
};

template <typename Key, typename Value, typename Hash>
constexpr HashBrown<Key, Value, Hash>::HashBrown()
   : _buckets(1)
   , _hasher()
   , _items(0)
{
}

template <typename Key, typename Value, typename Hash>
typename HashBrown<Key, Value, Hash>::iterator HashBrown<Key, Value, Hash>::erase(const Key& key)
{
   const auto bucket_index = get_bucket_index(key);
   auto& bucket = _buckets[bucket_index];

   auto find_it = std::ranges::find_if(bucket.items, [key](const auto& pair) {
         const auto ekey = pair.first;
         return ekey == key;
   });

   --_items;
   auto erase_it = bucket.items.erase(find_it);
   return iterator {
      this,
      bucket_index,
      static_cast<std::size_t>(std::distance(bucket.items.begin(), erase_it))
   };
}


template <typename Key, typename Value, typename Hash>
void HashBrown<Key, Value, Hash>::insert(std::initializer_list<value_type> il)
{
   insert(il.begin(), il.end());
}

template <typename Key, typename Value, typename Hash>
template <typename InputIt>
void HashBrown<Key, Value, Hash>::insert(InputIt first, InputIt last)
{
   for (; first != last; ++first)
   {
      insert(*first);
   }
}

template <typename Key, typename Value, typename Hash>
typename HashBrown<Key, Value, Hash>::iterator HashBrown<Key, Value, Hash>::insert(Key key, Value value)
{

   if (_buckets.empty() || _items < 3 * _buckets.size() / 4) {
      resize();
   }

   const auto bucket_index = get_bucket_index(key);
   auto& bucket = _buckets[bucket_index];
   
   for (auto it = bucket.items.begin(); it != bucket.items.end(); ++it)
   {
      const auto& ekey = it->first;
      auto& evalue = it->second;

      if (ekey == key)
      {
         std::swap(evalue, value);
         return iterator {this,
                         bucket_index,
                         static_cast<std::size_t>(std::distance(bucket.items.begin(), it))
         };
      }
   }

   _items++;
   bucket.items.push_back(std::make_pair(std::forward<Key>(key), std::forward<Value>(value)));
   return iterator {
      this,
      bucket_index,
      bucket.items.size() - 1,
   };
}

template <typename Key, typename Value, typename Hash>
typename HashBrown<Key, Value, Hash>::iterator HashBrown<Key, Value, Hash>::insert(const value_type& value)
{
   const auto& key = value.first;
   const auto& pair_value = value.second;
   return insert(key, pair_value);
}

template <typename Key, typename Value, typename Hash>
typename HashBrown<Key, Value, Hash>::iterator HashBrown<Key, Value, Hash>::insert(value_type&& value)
{
   return emplace(std::forward<value_type>(value));
}

template <typename Key, typename Value, typename Hash>
template <typename... Args>
typename HashBrown<Key, Value, Hash>::iterator HashBrown<Key, Value, Hash>::emplace(Args&&... args)
{
   const std::pair pair = {std::forward<Args>(args)...};
   return insert(pair);
}
      
template <typename Key, typename Value, typename Hash>
typename HashBrown<Key, Value, Hash>::iterator HashBrown<Key, Value, Hash>::begin()
{
   return iterator(this, 0, 0);
}

template <typename Key, typename Value, typename Hash>
typename HashBrown<Key, Value, Hash>::const_iterator HashBrown<Key, Value, Hash>::begin() const
{
   return const_cast<HashBrown*>(this)->begin();
}

template <typename Key, typename Value, typename Hash>
typename HashBrown<Key, Value, Hash>::const_iterator HashBrown<Key, Value, Hash>::cbegin() const
{
   return begin();
}

template <typename Key, typename Value, typename Hash>
typename HashBrown<Key, Value, Hash>::iterator HashBrown<Key, Value, Hash>::end()
{
   const auto bucket = _buckets.size() - 1;
   return Iterator(this, bucket, _items);
}

template <typename Key, typename Value, typename Hash>
typename HashBrown<Key, Value, Hash>::const_iterator HashBrown<Key, Value, Hash>::end() const
{
   return cend();
}

template <typename Key, typename Value, typename Hash>
typename HashBrown<Key, Value, Hash>::const_iterator HashBrown<Key, Value, Hash>::cend() const
{
   return const_cast<HashBrown*>(this)->end();
}

template <typename Key, typename Value, typename Hash>
std::size_t HashBrown<Key, Value, Hash>::get_bucket_index(const Key& key) const
{
   const auto hash = _hasher(key);
   const auto bucket_index = hash % _buckets.size();
   return bucket_index;
}

template <typename Key, typename Value, typename Hash>
float HashBrown<Key, Value, Hash>::load_factor() const
{
   return 1.0;
}

template <typename Key, typename Value, typename Hash>
void HashBrown<Key, Value, Hash>::resize()
{
   const auto determine_target_size = [this]() -> std::size_t {
      const auto size = _buckets.size();
      switch(size)
      {
         case 0:
            return 1;
         default:
            return 2 * size;
      }
   };

   const auto target_size = determine_target_size();
   std::vector<Bucket> new_buckets{target_size};
   
   for (auto & [key, value] : _buckets
                    | std::views::transform([](auto& bucket) { return bucket.items; })
                    | std::views::join)
   {
      const auto hash = Hash{}(key);
      const auto bucket = hash % new_buckets.size();
      new_buckets[bucket].items.emplace_back(key, value);
   }

   std::swap(_buckets, new_buckets);
}

template <typename Key, typename Value, typename Hash>
const Value* HashBrown<Key, Value, Hash>::get(Key&& key) const
{
   const auto bucket_index = get_bucket_index(key);
   const auto& bucket = _buckets[bucket_index];
   const auto find_it = std::ranges::find_if(bucket.items, [key](const auto& pair) {
         const auto ekey = pair.first;
         return ekey == key;
   });

   if (find_it != bucket.items.cend())
   {
      return &find_it->second;
   }
   return nullptr;
}

template <typename Key, typename Value, typename Hash>
bool HashBrown<Key, Value, Hash>::empty() const
{
   return begin() == end();
}

template <typename T>
struct HashFunction
{
   std::size_t operator()(const T& t)
   {
      return std::hash<T>{}(t);
   }
   
   std::size_t operator()(const T& t) const
   {
      return std::hash<T>{}(t);
   }
};

template <typename Key, typename Value, typename Hash>
struct HashBrown<Key, Value, Hash>::Bucket
{
   std::vector<HashBrown<Key, Value, Hash>::value_type> items;
};

