#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "hsa.h"

#include <map>

namespace hsaperf {

class Counter;
class CounterGroup;
class Pmu;

struct VarDataMap {};
struct CounterGroupMap {};
struct HsaCounterGroupTypeMask {};

Pmu **getAvailablePmus(hsa::Device *pDevice, uint32_t &numPmus);
Pmu **getAvailablePmus(hsa::RuntimeApi *pRuntime, uint32_t &numPmus);

class Pmu {

 public:
  enum info { INFO_MAX };
  enum paramter { PARAM_MAX };
  enum PMU_PROFILE_STATES { PMU_IDLE, PMU_START, PMU_STOP };

  Pmu();
  virtual ~Pmu();
  CounterGroup *getCounterGroupById(uint32_t id);
  CounterGroup **getAllCounterGroups(uint32_t &numGrp);
  PMU_PROFILE_STATES getCurrentState();
  virtual bool begin() = 0;
  virtual bool end() = 0;
  virtual bool waitForCompletion(uint32_t milliseconds) = 0;
  bool getParameter(uint32_t param, uint32_t &retSize, void **ppData);
  bool setParameter(uint32_t param, uint32_t paramSize, void *pData);
  bool getInfo(uint32_t info, uint32_t &retSize, void **ppData);

 protected:
  void clearCounterGroupMap();
  bool setInfo(uint32_t info, uint32_t size, void *pData);

  CounterGroupMap m_counterGroupMap;
  PMU_PROFILE_STATES m_state;
  VarDataMap m_paramTable;
  VarDataMap m_infoTable;
};

class CounterGroup {

 public:
  enum info { INFO_MAX };
  enum parameter { PARAM_MAX };

  CounterGroup();
  virtual ~CounterGroup();
  virtual bool setEnable(bool b);
  bool isEnabled();
  virtual Counter *createCounter() = 0;
  virtual bool enableCounter(Counter *pCntr) = 0;
  virtual bool destroyCounter(Counter *pCntr) = 0;
  virtual Counter **getEnabledCounters(uint32_t &num) = 0;
  virtual Counter **getAllCounters(uint32_t &num) = 0;
  bool getParameter(uint32_t param, uint32_t &retSize, void **ppData);
  bool setParameter(uint32_t param, uint32_t paramSize, void *pData);
  bool getInfo(uint32_t info, uint32_t &retSize, void **ppData);

 protected:
  virtual void _initCounterGroupType() = 0;
  bool setInfo(uint32_t info, uint32_t size, void *pData);

  HsaCounterGroupTypeMask m_groupType;
  bool m_enabled;
  VarDataMap m_paramTable;
  VarDataMap m_infoTable;
};

class Counter {
 public:
  enum parameter { PARAM_MAX };

  Counter();
  Counter(const Counter &a);
  virtual ~Counter();
  bool isResultReady();
  virtual bool getResult(uint64_t *pResult) = 0;
  bool getParameter(uint32_t param, uint32_t &retSize, void **ppData);
  bool setParameter(uint32_t param, uint32_t paramSize, void *pData);

 protected:
  bool m_isResultReady;
  uint64_t m_result;
  VarDataMap m_paramTable;
};

class TraceGroup {
 public:
  enum info { INFO_MAX };
  enum parameter { PARAM_MAX };

  TraceGroup();
  ~TraceGroup();
  virtual uint32_t getCollectedBufferCount() = 0;
  virtual bool lock(uint32_t bufferId) = 0;
  virtual bool unlock(uint32_t bufferId) = 0;
  bool insertUserData(uint32_t type, void *pData, uint32_t data_size);
  virtual bool setEnable(bool b);
  bool isEnabled();
  virtual Counter *createCounter() = 0;
  virtual bool enableCounter(Counter *pCntr) = 0;
  virtual bool destroyCounter(Counter *pCntr) = 0;
  virtual Counter **getEnabledCounters(uint32_t &num) = 0;
  virtual Counter **getAllCounters(uint32_t &num) = 0;
  bool getParameter(uint32_t param, uint32_t &retSize, void **ppData);
  bool setParameter(uint32_t param, uint32_t paramSize, void *pData);
  bool getInfo(uint32_t info, uint32_t &retSize, void **ppData);

 protected:
  virtual void _initCounterGroupType() = 0;
  bool setInfo(uint32_t info, uint32_t size, void *pData);

  void *m_pBuffer;
  uint32_t m_bufferSize;
  uint32_t m_writePtrOffset;
  uint32_t m_numWrapped;
  HsaCounterGroupTypeMask m_groupType;
  bool m_enabled;
  VarDataMap m_paramTable;
};

class InfoSet {
 public:
  enum info { INFO_MAX };

  InfoSet();
  virtual ~InfoSet();
  bool getInfo(uint32_t info, uint32_t &retSize, void **ppData);

 protected:
  bool setInfo(uint32_t info, uint32_t size, void *pData);

  VarDataMap m_infoTable;
};

class ParameterSet {
 public:
  enum parameter { PARAM_MAX };

  ParameterSet();
  virtual ~ParameterSet();
  bool getParameter(uint32_t param, uint32_t &retSize, void **ppData);
  bool setParameter(uint32_t param, uint32_t paramSize, void *pData);

 protected:
  VarDataMap m_paramTable;
};

class VarData {
 public:
  VarData();
  virtual ~VarData();
  void clear();
  bool set(uint32_t size, void *pData);
  uint32_t get(uint32_t size, void *pData);
  uint32_t getSize();
};


} // namespace hsaperf
