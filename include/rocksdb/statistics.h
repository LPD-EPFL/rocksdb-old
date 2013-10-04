// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#ifndef STORAGE_ROCKSDB_INCLUDE_STATISTICS_H_
#define STORAGE_ROCKSDB_INCLUDE_STATISTICS_H_

#include <atomic>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <string>
#include <memory>
#include <vector>

namespace leveldb {

/**
 * Keep adding ticker's here.
 * Any ticker should have a value less than TICKER_ENUM_MAX.
 * Add a new ticker by assigning it the current value of TICKER_ENUM_MAX
 * Add a string representation in TickersNameMap below.
 * And incrementing TICKER_ENUM_MAX.
 */
enum Tickers {
  BLOCK_CACHE_MISS,
  BLOCK_CACHE_HIT,
  BLOOM_FILTER_USEFUL, // no. of times bloom filter has avoided file reads.

  /**
   * COMPACTION_KEY_DROP_* count the reasons for key drop during compaction
   * There are 3 reasons currently.
   */
  COMPACTION_KEY_DROP_NEWER_ENTRY, // key was written with a newer value.
  COMPACTION_KEY_DROP_OBSOLETE, // The key is obsolete.
  COMPACTION_KEY_DROP_USER, // user compaction function has dropped the key.

  // Number of keys written to the database via the Put and Write call's
  NUMBER_KEYS_WRITTEN,
  // Number of Keys read,
  NUMBER_KEYS_READ,
  // Bytes written / read
  BYTES_WRITTEN,
  BYTES_READ,
  NO_FILE_CLOSES,
  NO_FILE_OPENS,
  NO_FILE_ERRORS,
  // Time system had to wait to do LO-L1 compactions
  STALL_L0_SLOWDOWN_MICROS,
  // Time system had to wait to move memtable to L1.
  STALL_MEMTABLE_COMPACTION_MICROS,
  // write throttle because of too many files in L0
  STALL_L0_NUM_FILES_MICROS,
  RATE_LIMIT_DELAY_MILLIS,

  NO_ITERATORS, // number of iterators currently open

  // Number of MultiGet calls, keys read, and bytes read
  NUMBER_MULTIGET_CALLS,
  NUMBER_MULTIGET_KEYS_READ,
  NUMBER_MULTIGET_BYTES_READ,

  // Number of deletes records that were not required to be
  // written to storage because key does not exist
  NUMBER_FILTERED_DELETES,
  NUMBER_MERGE_FAILURES,
  SEQUENCE_NUMBER,

  // number of times bloom was checked before creating iterator on a
  // file, and the number of times the check was useful in avoiding
  // iterator creation (and thus likely IOPs).
  BLOOM_FILTER_PREFIX_CHECKED,
  BLOOM_FILTER_PREFIX_USEFUL,

  // Number of times we had to reseek inside an iteration to skip
  // over large number of keys with same userkey.
  NUMBER_OF_RESEEKS_IN_ITERATION,

  // Record the number of calls to GetUpadtesSince. Useful to keep track of
  // transaction log iterator refreshes
  GET_UPDATES_SINCE_CALLS,

  TICKER_ENUM_MAX
};

// The order of items listed in  Tickers should be the same as
// the order listed in TickersNameMap
const std::vector<std::pair<Tickers, std::string>> TickersNameMap = {
  { BLOCK_CACHE_MISS, "rocksdb.block.cache.miss" },
  { BLOCK_CACHE_HIT, "rocksdb.block.cache.hit" },
  { BLOOM_FILTER_USEFUL, "rocksdb.bloom.filter.useful" },
  { COMPACTION_KEY_DROP_NEWER_ENTRY, "rocksdb.compaction.key.drop.new" },
  { COMPACTION_KEY_DROP_OBSOLETE, "rocksdb.compaction.key.drop.obsolete" },
  { COMPACTION_KEY_DROP_USER, "rocksdb.compaction.key.drop.user" },
  { NUMBER_KEYS_WRITTEN, "rocksdb.number.keys.written" },
  { NUMBER_KEYS_READ, "rocksdb.number.keys.read" },
  { BYTES_WRITTEN, "rocksdb.bytes.written" },
  { BYTES_READ, "rocksdb.bytes.read" },
  { NO_FILE_CLOSES, "rocksdb.no.file.closes" },
  { NO_FILE_OPENS, "rocksdb.no.file.opens" },
  { NO_FILE_ERRORS, "rocksdb.no.file.errors" },
  { STALL_L0_SLOWDOWN_MICROS, "rocksdb.l0.slowdown.micros" },
  { STALL_MEMTABLE_COMPACTION_MICROS, "rocksdb.memtable.compaction.micros" },
  { STALL_L0_NUM_FILES_MICROS, "rocksdb.l0.num.files.stall.micros" },
  { RATE_LIMIT_DELAY_MILLIS, "rocksdb.rate.limit.delay.millis" },
  { NO_ITERATORS, "rocksdb.num.iterators" },
  { NUMBER_MULTIGET_CALLS, "rocksdb.number.multiget.get" },
  { NUMBER_MULTIGET_KEYS_READ, "rocksdb.number.multiget.keys.read" },
  { NUMBER_MULTIGET_BYTES_READ, "rocksdb.number.multiget.bytes.read" },
  { NUMBER_FILTERED_DELETES, "rocksdb.number.deletes.filtered" },
  { NUMBER_MERGE_FAILURES, "rocksdb.number.merge.failures" },
  { SEQUENCE_NUMBER, "rocksdb.sequence.number" },
  { BLOOM_FILTER_PREFIX_CHECKED, "rocksdb.bloom.filter.prefix.checked" },
  { BLOOM_FILTER_PREFIX_USEFUL, "rocksdb.bloom.filter.prefix.useful" },
  { NUMBER_OF_RESEEKS_IN_ITERATION, "rocksdb.number.reseeks.iteration" },
  { GET_UPDATES_SINCE_CALLS, "rocksdb.getupdatessince.calls" }
};

/**
 * Keep adding histogram's here.
 * Any histogram whould have value less than HISTOGRAM_ENUM_MAX
 * Add a new Histogram by assigning it the current value of HISTOGRAM_ENUM_MAX
 * Add a string representation in HistogramsNameMap below
 * And increment HISTOGRAM_ENUM_MAX
 */
enum Histograms {
  DB_GET,
  DB_WRITE,
  COMPACTION_TIME,
  TABLE_SYNC_MICROS,
  COMPACTION_OUTFILE_SYNC_MICROS,
  WAL_FILE_SYNC_MICROS,
  MANIFEST_FILE_SYNC_MICROS,
  // TIME SPENT IN IO DURING TABLE OPEN
  TABLE_OPEN_IO_MICROS,
  DB_MULTIGET,
  READ_BLOCK_COMPACTION_MICROS,
  READ_BLOCK_GET_MICROS,
  WRITE_RAW_BLOCK_MICROS,

  STALL_L0_SLOWDOWN_COUNT,
  STALL_MEMTABLE_COMPACTION_COUNT,
  STALL_L0_NUM_FILES_COUNT,
  HARD_RATE_LIMIT_DELAY_COUNT,
  SOFT_RATE_LIMIT_DELAY_COUNT,
  NUM_FILES_IN_SINGLE_COMPACTION,
  HISTOGRAM_ENUM_MAX
};

const std::vector<std::pair<Histograms, std::string>> HistogramsNameMap = {
  { DB_GET, "rocksdb.db.get.micros" },
  { DB_WRITE, "rocksdb.db.write.micros" },
  { COMPACTION_TIME, "rocksdb.compaction.times.micros" },
  { TABLE_SYNC_MICROS, "rocksdb.table.sync.micros" },
  { COMPACTION_OUTFILE_SYNC_MICROS, "rocksdb.compaction.outfile.sync.micros" },
  { WAL_FILE_SYNC_MICROS, "rocksdb.wal.file.sync.micros" },
  { MANIFEST_FILE_SYNC_MICROS, "rocksdb.manifest.file.sync.micros" },
  { TABLE_OPEN_IO_MICROS, "rocksdb.table.open.io.micros" },
  { DB_MULTIGET, "rocksdb.db.multiget.micros" },
  { READ_BLOCK_COMPACTION_MICROS, "rocksdb.read.block.compaction.micros" },
  { READ_BLOCK_GET_MICROS, "rocksdb.read.block.get.micros" },
  { WRITE_RAW_BLOCK_MICROS, "rocksdb.write.raw.block.micros" },
  { STALL_L0_SLOWDOWN_COUNT, "rocksdb.l0.slowdown.count"},
  { STALL_MEMTABLE_COMPACTION_COUNT, "rocksdb.memtable.compaction.count"},
  { STALL_L0_NUM_FILES_COUNT, "rocksdb.num.files.stall.count"},
  { HARD_RATE_LIMIT_DELAY_COUNT, "rocksdb.hard.rate.limit.delay.count"},
  { SOFT_RATE_LIMIT_DELAY_COUNT, "rocksdb.soft.rate.limit.delay.count"},
  { NUM_FILES_IN_SINGLE_COMPACTION, "rocksdb.numfiles.in.singlecompaction" }
};

struct HistogramData {
  double median;
  double percentile95;
  double percentile99;
  double average;
  double standard_deviation;
};


class Histogram {
 public:
  // clear's the histogram
  virtual void Clear() = 0;
  virtual ~Histogram();
  // Add a value to be recorded in the histogram.
  virtual void Add(uint64_t value) = 0;

  virtual std::string ToString() const = 0;

  // Get statistics
  virtual double Median() const = 0;
  virtual double Percentile(double p) const = 0;
  virtual double Average() const = 0;
  virtual double StandardDeviation() const = 0;
  virtual void Data(HistogramData * const data) const = 0;

};

/**
 * A dumb ticker which keeps incrementing through its life time.
 * Not thread safe. Locking is currently managed by external leveldb lock
 */
class Ticker {
 public:
  Ticker() : count_(0) { }

  inline void setTickerCount(uint64_t count) {
    count_ = count;
  }

  inline void recordTick(int count = 1) {
    count_ += count;
  }

  inline uint64_t getCount() {
    return count_;
  }

 private:
  std::atomic_uint_fast64_t count_;
};

// Analyze the performance of a db
class Statistics {
 public:

  virtual long getTickerCount(Tickers tickerType) = 0;
  virtual void recordTick(Tickers tickerType, uint64_t count = 0) = 0;
  virtual void setTickerCount(Tickers tickerType, uint64_t count) = 0;
  virtual void measureTime(Histograms histogramType, uint64_t time) = 0;

  virtual void histogramData(Histograms type, HistogramData * const data) = 0;
  // String representation of the statistic object.
  std::string ToString();
};

// Create a concrete DBStatistics object
std::shared_ptr<Statistics> CreateDBStatistics();

// Ease of Use functions
inline void RecordTick(std::shared_ptr<Statistics> statistics,
                       Tickers ticker,
                       uint64_t count = 1) {
  assert(HistogramsNameMap.size() == HISTOGRAM_ENUM_MAX);
  assert(TickersNameMap.size() == TICKER_ENUM_MAX);
  if (statistics) {
    statistics->recordTick(ticker, count);
  }
}

inline void SetTickerCount(std::shared_ptr<Statistics> statistics,
                           Tickers ticker,
                           uint64_t count) {
  assert(HistogramsNameMap.size() == HISTOGRAM_ENUM_MAX);
  assert(TickersNameMap.size() == TICKER_ENUM_MAX);
  if (statistics) {
    statistics->setTickerCount(ticker, count);
  }
}

}  // namespace leveldb

#endif  // STORAGE_ROCKSDB_INCLUDE_STATISTICS_H_