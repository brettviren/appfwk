#ifndef APP_FRAMEWORK_INCLUDE_APP_FRAMEWORK_QUEUEREGISTRY_HXX_
#define APP_FRAMEWORK_INCLUDE_APP_FRAMEWORK_QUEUEREGISTRY_HXX_

#include <cxxabi.h>

// Declarations
namespace appframework {

template<typename T>
std::shared_ptr<NamedQueueI<T>> 
QueueRegistry::get_queue(std::string name) {

  auto itQ = queue_registry_.find(name);
  if ( itQ != queue_registry_.end() ) {
      auto queuePtr =  std::dynamic_pointer_cast<NamedQueueI<T>>(itQ->second.instance);

      if (!queuePtr) {
        int     status;
        std::string realname_target = abi::__cxa_demangle(typeid(T).name(), 0, 0, &status);
        std::string realname_source = abi::__cxa_demangle(itQ->second.type->name(), 0, 0, &status);

        throw QueueTypeMismatch(ERS_HERE, name, realname_source, realname_target);
      }
  } 

  auto itP = this->queue_configmap_.find(name);
  if ( itP != queue_configmap_.end() ) {
    QueueEntry entry = {&typeid(T), create_queue<T>(name, itP->second)};
    queue_registry_[name] = entry;
    return std::dynamic_pointer_cast<NamedQueueI<T>>(entry.instance);

  } else {
    throw std::runtime_error("Queue not found");
  }
}


template<typename T>
std::shared_ptr<NamedObject> QueueRegistry::create_queue(std::string name, const QueueConfig& config) {

  std::shared_ptr<NamedObject> queue;
  switch(config.kind) {
    case QueueConfig::std_deque:
      queue = std::make_shared<NamedStdDeQueue<T>>(name, config.size);
      break;
    default:
      throw std::runtime_error("Unknown queue kind");
  }

  return queue;
}

} // namespace appframework

#endif // APP_FRAMEWORK_INCLUDE_APP_FRAMEWORK_QUEUEREGISTRY_HXX_