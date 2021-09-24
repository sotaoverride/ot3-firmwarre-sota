#include "motor-control/core/step_motor.hpp"

#include "common/firmware/timer_interrupt.h"
#include "motor-control/core/motor_messages.hpp"

using namespace motor_messages;

static auto handler_class = motor_handler::MotorInterruptHandler<
    freertos_message_queue::FreeRTOSMessageQueue>();

void step_motor() {
    if (handler_class.can_step()) {
        toggle_step_pin();
        handler_class.increment_counter();
    } else {
        if (handler_class.has_messages()) {
            handler_class.update_move();
        } else {
            handler_class.finish_current_move();
        }
    }
}

void start_motor_handler(
    freertos_message_queue::FreeRTOSMessageQueue<Move>& queue) {
    handler_class.set_message_queue(&queue);
}

void reset_motor_handler() { handler_class.reset(); }