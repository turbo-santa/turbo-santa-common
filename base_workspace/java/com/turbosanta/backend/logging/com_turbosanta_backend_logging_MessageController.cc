#include "java/com/turbosanta/backend/logging/com_turbosanta_backend_logging_MessageController.h"

#include "cc/backend/logging/message_controller.h"

using backend::logging::MessageController;

jlong Java_com_turbosanta_backend_logging_MessageController_getCommunicationLayerPointer(JNIEnv *, jobject) {
  MessageController* message_controller = MessageController::GetInstance();
  return reinterpret_cast<jlong>(message_controller->communication_layer());
}
