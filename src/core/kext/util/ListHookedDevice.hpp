#ifndef LISTHOOKEDDEVICE_HPP
#define LISTHOOKEDDEVICE_HPP

#include "base.hpp"
#include "KeyCode.hpp"
#include "List.hpp"
#include "TimerWrapper.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  class ListHookedDevice {
  public:
    class Item : public List::Item {
      friend class ListHookedDevice;

    public:
      Item(IOHIDevice* d);
      virtual ~Item(void) {};

      virtual bool isReplaced(void) const = 0;

      const IOHIDevice* get(void) const { return device_; }
      const DeviceIdentifier& getDeviceIdentifier(void) const { return deviceIdentifier_; }
      DeviceType::DeviceType getDeviceType(void) const { return deviceType_; }

    protected:
      IOHIDevice* device_;
      DeviceIdentifier deviceIdentifier_;
      DeviceType::DeviceType deviceType_;

      virtual bool refresh(void) = 0;

      void setDeviceIdentifier(void);
      void setDeviceType(void);
      static bool isConsumer(const char* name);
    };

    bool initialize(void);
    void terminate(void);

    void push_back(ListHookedDevice::Item* newp);
    void erase(IOHIDevice* p);

    void refresh(void);

    ListHookedDevice::Item* get(const IOHIDevice* device);
    ListHookedDevice::Item* get_replaced(void);

    void getDeviceInformation(BridgeDeviceInformation& out, size_t index);

    static void initializeAll(IOWorkLoop& workloop);
    static void terminateAll(void);
    static void refreshAll(void);
    static void refreshAll_timer_callback(OSObject* owner, IOTimerEventSource* sender);

  protected:
    ListHookedDevice(void) : last_(NULL), list_(NULL) {}
    virtual ~ListHookedDevice(void) {}

    const IOHIDevice* last_;
    List* list_;

  private:
    enum {
      REFRESHALL_TIMER_INTERVAL = 3000,
    };
    static TimerWrapper refreshAll_timer_;
  };
}

#endif
