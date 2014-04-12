//  Copyright (c) 2013, Facebook, Inc.  All rights reserved.
//  This source code is licensed under the BSD-style license found in the
//  LICENSE file in the root directory of this source tree. An additional grant
//  of patent rights can be found in the PATENTS file in the same directory.
//
#include "rocksdb/memtablerep.h"
#include "db/memtable.h"
#include "db/skiplist.h"
#include "db/skiplist-herlihy_lf/intset.h"

// ADD HERE -- IMITATE THIS

namespace rocksdb {
namespace {
class ConcurrentSkipListRep : public MemTableRep {
  SkipList<const char*, const MemTableRep::KeyComparator&> skip_list_;
  sl_intset_t* intset;

public:
  explicit ConcurrentSkipListRep(const MemTableRep::KeyComparator& compare, Arena* arena)
    : skip_list_(compare, arena) {
      // Allocate and instantiate intset
  }

  // Insert key into the list.
  // REQUIRES: nothing that compares equal to key is currently in the list.
  virtual void Insert(const char* key) override {
  }

  virtual void Insert(const char* key, const char* val) override {
    //sl_add(intset, key, val);
  }


  // Returns true iff an entry that compares equal to key is in the list.
  virtual bool Contains(const char* key) const override {
    return skip_list_.Contains(key);
    // sl_contains(intset, skey_t key);
  }

  virtual size_t ApproximateMemoryUsage() override {
    // All memory is allocated through arena; nothing to report here
    return 0;
  }

  virtual void Get(const LookupKey& k, void* callback_args,
                   bool (*callback_func)(void* arg,
                                         const char* entry)) override {
    // ConcurrentSkipListRep::Iterator iter(&skip_list_);
    // Slice dummy_slice;
    // for (iter.Seek(dummy_slice, k.memtable_key().data());
    //      iter.Valid() && callback_func(callback_args, iter.key());
    //      iter.Next()) {
    // }
  }

  virtual ~ConcurrentSkipListRep() override { }

  // Iteration over the contents of a skip list

  // ADD HERE - STUPID ITERATOR
  class Iterator : public MemTableRep::Iterator {
    //SkipList<const char*, const MemTableRep::KeyComparator&>::Iterator iter_;
   public:
    // Initialize an iterator over the specified list.
    // The returned iterator is not valid.
    explicit Iterator() {}

    virtual ~Iterator() override { }

    // Returns true iff the iterator is positioned at a valid node.
    virtual bool Valid() const override {
      //return iter_.Valid();
      return false;
    }

    // Returns the key at the current position.
    // REQUIRES: Valid()
    virtual const char* key() const override {
      //return iter_.key();
      return nullptr;
    }

    // Advances to the next position.
    // REQUIRES: Valid()
    virtual void Next() override {
      //iter_.Next();
    }

    // Advances to the previous position.
    // REQUIRES: Valid()
    virtual void Prev() override {
      //iter_.Prev();
    }

    // Advance to the first entry with a key >= target
    virtual void Seek(const Slice& user_key, const char* memtable_key)
        override {
      // if (memtable_key != nullptr) {
      //   iter_.Seek(memtable_key);
      // } else {
      //   iter_.Seek(EncodeKey(&tmp_, user_key));
      // }
    }

    // Position at the first entry in list.
    // Final state of iterator is Valid() iff list is not empty.
    virtual void SeekToFirst() override {
      // iter_.SeekToFirst();
    }

    // Position at the last entry in list.
    // Final state of iterator is Valid() iff list is not empty.
    virtual void SeekToLast() override {
      // iter_.SeekToLast();
    }
   //protected:
    //std::string tmp_;       // For passing to EncodeKey
  };

  // Unhide default implementations of GetIterator
  using MemTableRep::GetIterator;

  virtual MemTableRep::Iterator* GetIterator() override {
    // return new ConcurrentSkipListRep::Iterator(&skip_list_);
    return nullptr;

  }
};
}

MemTableRep* ConcurrentSkipListFactory::CreateMemTableRep(
    const MemTableRep::KeyComparator& compare, Arena* arena,
    const SliceTransform*) {
  return new ConcurrentSkipListRep(compare, arena);
}

} // namespace rocksdb
